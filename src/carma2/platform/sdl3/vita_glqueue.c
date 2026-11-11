/* GL command queue: vitaGL work on a second core.
 *
 * Every GL entry point glrend uses resolves through C2VitaGLGetProcAddress, which hands out the
 * wrappers below. While the queue is running, the calls that only set state or draw from GPU
 * buffers are recorded into a ring buffer (with any data they point at copied in) and a render
 * thread replays them in order; the main thread goes on with the game and BRender's own CPU work.
 * Every other call (queries, object creation/deletion, texture uploads from CPU memory, client-side
 * arrays) first waits until the render thread has executed everything queued before it and then
 * runs directly, so vitaGL is never used by two threads at once and every call still sees the
 * state its predecessors left. The frame swap is queued as a callback; the main thread can be at
 * most one frame ahead of the render thread.
 *
 * glGetError does not wait: the render thread checks for errors after each command and keeps the
 * first one it sees, which glGetError returns (it only feeds diagnostics). */
#include "vita_glqueue.h"

#include <SDL3/SDL.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#ifdef __vita__
#include <psp2/kernel/threadmgr.h>
/* user CPU cores 0 and 1 (SCE_KERNEL_CPU_MASK_USER_0 / _1): game on one, vitaGL on the other */
#define C2_CPU_MASK_MAIN 0x10000
#define C2_CPU_MASK_RENDER 0x20000
#endif

extern void C2VitaLog(const char* format, ...);

#define QUEUE_BYTES (4u * 1024u * 1024u)
#define QUEUE_MAX_PAYLOAD (256u * 1024u)

typedef union {
    uint64_t u;
    int64_t i;
    float f;
    double d;
    const void* p;
} tArg;

typedef struct {
    uint32_t op;
    uint32_t size; /* bytes of this command, header and payload included, multiple of 8 */
} tCommand_header;

enum {
    OP_WRAP = 0,
    OP_CALLBACK,
    OP_ACTIVE_TEXTURE, OP_BIND_BUFFER, OP_BIND_BUFFER_BASE, OP_BIND_FRAMEBUFFER, OP_BIND_TEXTURE,
    OP_BIND_VERTEX_ARRAY, OP_BLEND_FUNC, OP_BLEND_FUNC_SEPARATE, OP_BUFFER_DATA, OP_BUFFER_SUB_DATA,
    OP_CLEAR, OP_CLEAR_COLOR, OP_COLOR_MASK, OP_CULL_FACE, OP_DEPTH_FUNC, OP_DEPTH_MASK, OP_DISABLE,
    OP_DRAW_ELEMENTS, OP_ENABLE, OP_FRONT_FACE, OP_PIXEL_STOREI, OP_POLYGON_MODE, OP_POLYGON_OFFSET,
    OP_SCISSOR, OP_TEX_PARAMETERF, OP_TEX_PARAMETERI, OP_UNIFORM1F, OP_UNIFORM1I, OP_UNIFORM4F,
    OP_USE_PROGRAM, OP_VIEWPORT,
    OP_DELETE_BUFFERS, OP_DELETE_TEXTURES, OP_DELETE_VERTEX_ARRAYS, OP_DELETE_FRAMEBUFFERS,
    OP_ENABLE_VERTEX_ATTRIB_ARRAY, OP_VERTEX_ATTRIB_POINTER,
    OP_TEX_SUB_IMAGE_2D, OP_BLIT_FRAMEBUFFER, OP_CLEAR_DEPTH,
    OP_COUNT
};

/* original entry points (the vita_gl.c adapters or vitaGL itself) */
static PFNGLACTIVETEXTUREPROC o_glActiveTexture;
static PFNGLATTACHSHADERPROC o_glAttachShader;
static PFNGLBINDATTRIBLOCATIONPROC o_glBindAttribLocation;
static PFNGLBINDBUFFERPROC o_glBindBuffer;
static PFNGLBINDBUFFERBASEPROC o_glBindBufferBase;
static PFNGLBINDFRAMEBUFFERPROC o_glBindFramebuffer;
static PFNGLBINDRENDERBUFFERPROC o_glBindRenderbuffer;
static PFNGLBINDTEXTUREPROC o_glBindTexture;
static PFNGLBINDVERTEXARRAYPROC o_glBindVertexArray;
static PFNGLBLENDFUNCPROC o_glBlendFunc;
static PFNGLBLENDFUNCSEPARATEPROC o_glBlendFuncSeparate;
static PFNGLBLITFRAMEBUFFERPROC o_glBlitFramebuffer;
static PFNGLBUFFERDATAPROC o_glBufferData;
static PFNGLBUFFERSUBDATAPROC o_glBufferSubData;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC o_glCheckFramebufferStatus;
static PFNGLCLEARPROC o_glClear;
static PFNGLCLEARCOLORPROC o_glClearColor;
static PFNGLCLEARDEPTHPROC o_glClearDepth;
static PFNGLCOLORMASKPROC o_glColorMask;
static PFNGLCOMPILESHADERPROC o_glCompileShader;
static PFNGLCREATEPROGRAMPROC o_glCreateProgram;
static PFNGLCREATESHADERPROC o_glCreateShader;
static PFNGLCULLFACEPROC o_glCullFace;
static PFNGLDELETEBUFFERSPROC o_glDeleteBuffers;
static PFNGLDELETEFRAMEBUFFERSPROC o_glDeleteFramebuffers;
static PFNGLDELETEPROGRAMPROC o_glDeleteProgram;
static PFNGLDELETERENDERBUFFERSPROC o_glDeleteRenderbuffers;
static PFNGLDELETESHADERPROC o_glDeleteShader;
static PFNGLDELETETEXTURESPROC o_glDeleteTextures;
static PFNGLDELETEVERTEXARRAYSPROC o_glDeleteVertexArrays;
static PFNGLDEPTHFUNCPROC o_glDepthFunc;
static PFNGLDEPTHMASKPROC o_glDepthMask;
static PFNGLDISABLEPROC o_glDisable;
static PFNGLDRAWELEMENTSPROC o_glDrawElements;
static PFNGLENABLEPROC o_glEnable;
static PFNGLENABLEVERTEXATTRIBARRAYPROC o_glEnableVertexAttribArray;
static PFNGLFRONTFACEPROC o_glFrontFace;
static PFNGLFRAMEBUFFERRENDERBUFFERPROC o_glFramebufferRenderbuffer;
static PFNGLFRAMEBUFFERTEXTURE2DPROC o_glFramebufferTexture2D;
static PFNGLGENBUFFERSPROC o_glGenBuffers;
static PFNGLGENFRAMEBUFFERSPROC o_glGenFramebuffers;
static PFNGLGENRENDERBUFFERSPROC o_glGenRenderbuffers;
static PFNGLGENTEXTURESPROC o_glGenTextures;
static PFNGLGENVERTEXARRAYSPROC o_glGenVertexArrays;
static PFNGLGENERATEMIPMAPPROC o_glGenerateMipmap;
static PFNGLGETATTRIBLOCATIONPROC o_glGetAttribLocation;
static PFNGLGETBOOLEANVPROC o_glGetBooleanv;
static PFNGLGETERRORPROC o_glGetError;
static PFNGLGETFLOATVPROC o_glGetFloatv;
static PFNGLGETINTEGERVPROC o_glGetIntegerv;
static PFNGLGETPROGRAMINFOLOGPROC o_glGetProgramInfoLog;
static PFNGLGETPROGRAMIVPROC o_glGetProgramiv;
static PFNGLGETSHADERINFOLOGPROC o_glGetShaderInfoLog;
static PFNGLGETSHADERIVPROC o_glGetShaderiv;
static PFNGLGETSTRINGPROC o_glGetString;
static PFNGLGETSTRINGIPROC o_glGetStringi;
static PFNGLGETTEXPARAMETERIVPROC o_glGetTexParameteriv;
static PFNGLGETUNIFORMBLOCKINDEXPROC o_glGetUniformBlockIndex;
static PFNGLGETUNIFORMLOCATIONPROC o_glGetUniformLocation;
static PFNGLISENABLEDPROC o_glIsEnabled;
static PFNGLLINKPROGRAMPROC o_glLinkProgram;
static PFNGLPIXELSTOREIPROC o_glPixelStorei;
static PFNGLPOLYGONMODEPROC o_glPolygonMode;
static PFNGLPOLYGONOFFSETPROC o_glPolygonOffset;
static PFNGLRENDERBUFFERSTORAGEPROC o_glRenderbufferStorage;
static PFNGLSCISSORPROC o_glScissor;
static PFNGLSHADERSOURCEPROC o_glShaderSource;
static PFNGLTEXIMAGE2DPROC o_glTexImage2D;
static PFNGLTEXPARAMETERFPROC o_glTexParameterf;
static PFNGLTEXPARAMETERIPROC o_glTexParameteri;
static PFNGLTEXSUBIMAGE2DPROC o_glTexSubImage2D;
static PFNGLUNIFORM1FPROC o_glUniform1f;
static PFNGLUNIFORM1IPROC o_glUniform1i;
static PFNGLUNIFORM4FPROC o_glUniform4f;
static PFNGLUNIFORMBLOCKBINDINGPROC o_glUniformBlockBinding;
static PFNGLUSEPROGRAMPROC o_glUseProgram;
static PFNGLVERTEXATTRIBPOINTERPROC o_glVertexAttribPointer;
static PFNGLVIEWPORTPROC o_glViewport;

/* ---- queue state ---- */

static uint8_t* gRing;
static _Atomic uint32_t gHead;            /* bytes published by the main thread (monotonic) */
static _Atomic uint32_t gTail;            /* bytes executed by the render thread (monotonic) */
static _Atomic int gConsumer_sleeping;
static _Atomic int gProducer_waiting;     /* main thread waits for ring space or a drained queue */
static _Atomic int gStop;
static _Atomic int gActive;
static _Atomic uint32_t gError;           /* first GL error seen by the render thread */
static _Atomic uint32_t gFrames_done;     /* swap callbacks executed */
static uint32_t gFrames_queued;
static SDL_Semaphore* gWork;
static SDL_Semaphore* gProgress;
static SDL_Thread* gThread;
static GLuint gProducer_vao;              /* vertex array bound as seen by the main thread */
static void MirrorForget(void);
static void MirrorReset(void);
/* C2_TEST_NODEDUP: queue redundant state calls anyway (diagnostics) */
static int gDedup = 1;
/* Without the render thread the same mirror drops redundant state calls before they reach
 * vita_gl.c and vitaGL (set per frame by C2VitaGLQueueDirectFrame; main thread only). */
static int gDirect_mirror;

/* statistics: main thread counters are plain, render thread ones atomic */
static uint64_t gStat_queued, gStat_syncs, gStat_sync_wait_ns, gStat_space_wait_ns, gStat_frame_wait_ns;
static uint32_t gStat_sync_by[96];
static _Atomic uint64_t gStat_worker_busy_ns;

/* The render thread itself calls wrapped entry points too: the adapters compile shader variants
 * lazily while a draw is replayed (glCreateShader, glGetUniformLocation, ...). Those must run
 * straight through; queueing or draining from the render thread would wait for itself (the 00.58
 * menu hang). */
static _Atomic uint64_t gRender_thread_id;

static uint64_t CurrentThreadId(void) {
#ifdef __vita__
    return (uint64_t)(uint32_t)sceKernelGetThreadId();
#else
    return (uint64_t)SDL_GetCurrentThreadID();
#endif
}

static int IsMainThreadWork(void) {
    return atomic_load(&gActive) != 0 && CurrentThreadId() != atomic_load(&gRender_thread_id);
}

static void WakeConsumer(void) {
    if (atomic_exchange(&gConsumer_sleeping, 0)) {
        SDL_SignalSemaphore(gWork);
    }
}

/* render thread progress, for the stall diagnostics below */
static _Atomic uint32_t gWorker_op;
static _Atomic uint32_t gWorker_done;

/* The main thread has been waiting for the render thread since pStart: from 2 s on, log what the
 * render thread is doing every 2 s; after 15 s crash on purpose so the system writes a core dump
 * with both threads (menus have no stall watchdog). */
static void WaitCheck(const char* pWhere, uint64_t pStart, uint64_t* pNext_log) {
    uint64_t now = SDL_GetTicksNS();
    uint64_t waited = now - pStart;

    if (waited < 2000000000ull || now < *pNext_log) {
        return;
    }
    *pNext_log = now + 2000000000ull;
    C2VitaLog("VITA GLQ main thread waits in %s for %llu ms: head=%u tail=%u render op=%u done=%u sleeping=%d frames queued=%u done=%u\n",
        pWhere, (unsigned long long)(waited / 1000000ull), atomic_load(&gHead), atomic_load(&gTail),
        atomic_load(&gWorker_op), atomic_load(&gWorker_done), atomic_load(&gConsumer_sleeping),
        gFrames_queued, atomic_load(&gFrames_done));
    if (waited > 15000000000ull) {
#ifdef __vita__
        extern void C2VitaLogFlush(void);
        C2VitaLog("VITA GLQ stall: crashing on purpose for a core dump\n");
        C2VitaLogFlush();
        *(volatile int*)0 = 0x61A11;
#endif
    }
}

/* Producer side. Commands are written at gLocal_head (main thread only) and made visible to the
 * render thread in batches: one release store of gHead per PUBLISH_BYTES instead of one per
 * command (every atomic is a barrier on the Cortex-A9; per-command publishing cost the 00.61 test
 * builds more than the render thread saved). Every wait and every frame flushes first. */
#define PUBLISH_BYTES 2048u
static uint32_t gLocal_head;   /* written, maybe not yet published */
static uint32_t gTail_seen;    /* last render thread position read by the main thread */

static void Flush(void) {
    if (atomic_load_explicit(&gHead, memory_order_relaxed) == gLocal_head) {
        return;
    }
    /* sequentially consistent store/load pair against the render thread's sleep check
     * (store sleeping, load head): one of the two sides always sees the other */
    atomic_store(&gHead, gLocal_head);
    if (atomic_load(&gConsumer_sleeping)) {
        WakeConsumer();
    }
}

/* Reserves `bytes` (multiple of 8) of contiguous ring space and returns it; Publish() commits it. */
static uint8_t* Reserve(uint32_t bytes) {
    uint64_t wait_start = 0, next_log = 0;

    for (;;) {
        uint32_t head = gLocal_head;
        uint32_t pos = head % QUEUE_BYTES;
        uint32_t contiguous = QUEUE_BYTES - pos;
        uint32_t need = bytes <= contiguous ? bytes : contiguous + bytes;

        if (QUEUE_BYTES - (head - gTail_seen) < need) {
            gTail_seen = atomic_load_explicit(&gTail, memory_order_acquire);
        }
        if (QUEUE_BYTES - (head - gTail_seen) >= need) {
            if (bytes > contiguous) {
                tCommand_header* wrap = (tCommand_header*)(gRing + pos);
                wrap->op = OP_WRAP;
                wrap->size = contiguous;
                gLocal_head = head + contiguous;
                continue;
            }
            return gRing + pos;
        }
        {
            uint64_t start = SDL_GetTicksNS();
            if (wait_start == 0) {
                wait_start = start;
            }
            Flush();
            atomic_store(&gProducer_waiting, 1);
            WakeConsumer();
            if (QUEUE_BYTES - (gLocal_head - atomic_load(&gTail)) < need) {
                SDL_WaitSemaphoreTimeout(gProgress, 5);
            }
            gStat_space_wait_ns += SDL_GetTicksNS() - start;
            WaitCheck("ring space", wait_start, &next_log);
        }
    }
}

static void Publish(uint32_t bytes) {
    gLocal_head += bytes;
    gStat_queued++;
    if (gLocal_head - atomic_load_explicit(&gHead, memory_order_relaxed) >= PUBLISH_BYTES) {
        Flush();
    }
}

static uint32_t CommandBytes(int nargs, uint32_t payload) {
    return (uint32_t)((sizeof(tCommand_header) + (size_t)nargs * sizeof(tArg) + payload + 7u) & ~7u);
}

static tArg* Begin(uint32_t op, int nargs, uint32_t payload, uint32_t* bytes_out) {
    uint32_t bytes = CommandBytes(nargs, payload);
    tCommand_header* h = (tCommand_header*)Reserve(bytes);
    h->op = op;
    h->size = bytes;
    *bytes_out = bytes;
    return (tArg*)(h + 1);
}

#define QUEUE0(OP)                 do { uint32_t b_; (void)Begin(OP, 0, 0, &b_); Publish(b_); } while (0)
#define QUEUE1(OP, A)              do { uint32_t b_; tArg* a_ = Begin(OP, 1, 0, &b_); a_[0] = (A); Publish(b_); } while (0)
#define QUEUE2(OP, A, B)           do { uint32_t b_; tArg* a_ = Begin(OP, 2, 0, &b_); a_[0] = (A); a_[1] = (B); Publish(b_); } while (0)
#define QUEUE3(OP, A, B, C)        do { uint32_t b_; tArg* a_ = Begin(OP, 3, 0, &b_); a_[0] = (A); a_[1] = (B); a_[2] = (C); Publish(b_); } while (0)
#define QUEUE4(OP, A, B, C, D)     do { uint32_t b_; tArg* a_ = Begin(OP, 4, 0, &b_); a_[0] = (A); a_[1] = (B); a_[2] = (C); a_[3] = (D); Publish(b_); } while (0)
#define QUEUE5(OP, A, B, C, D, E)  do { uint32_t b_; tArg* a_ = Begin(OP, 5, 0, &b_); a_[0] = (A); a_[1] = (B); a_[2] = (C); a_[3] = (D); a_[4] = (E); Publish(b_); } while (0)

static tArg U(uint64_t v) { tArg a; a.u = v; return a; }
static tArg I(int64_t v) { tArg a; a.i = v; return a; }
static tArg F(float v) { tArg a; a.u = 0; a.f = v; return a; }
static tArg P(const void* v) { tArg a; a.u = 0; a.p = v; return a; }

/* Waits until the render thread has executed everything queued so far. */
static void Drain(void) {
    int spins = 0;
    uint64_t start = 0, next_log = 0;

    Flush();
    while (atomic_load(&gTail) != gLocal_head) {
        if (start == 0) {
            start = SDL_GetTicksNS();
        }
        if (++spins < 64) {
            continue;
        }
        atomic_store(&gProducer_waiting, 1);
        WakeConsumer();
        if (atomic_load(&gTail) != gLocal_head) {
            SDL_WaitSemaphoreTimeout(gProgress, 5);
        }
        WaitCheck("drain", start, &next_log);
    }
    if (start != 0) {
        gStat_sync_wait_ns += SDL_GetTicksNS() - start;
    }
}

static void SyncFor(int which) {
    if (!IsMainThreadWork()) {
        return;
    }
    gStat_syncs++;
    if (which >= 0 && which < (int)(sizeof(gStat_sync_by) / sizeof(gStat_sync_by[0]))) {
        gStat_sync_by[which]++;
    }
    Drain();
}

void C2VitaGLQueueSync(void) {
    SyncFor(-1);
}

/* ---- render thread ---- */

static void Execute(const tCommand_header* h) {
    const tArg* a = (const tArg*)(h + 1);

    switch (h->op) {
    case OP_CALLBACK:
        ((void (*)(void*))a[0].p)((void*)a[1].p);
        break;
    case OP_ACTIVE_TEXTURE: o_glActiveTexture((GLenum)a[0].u); break;
    case OP_BIND_BUFFER: o_glBindBuffer((GLenum)a[0].u, (GLuint)a[1].u); break;
    case OP_BIND_BUFFER_BASE: o_glBindBufferBase((GLenum)a[0].u, (GLuint)a[1].u, (GLuint)a[2].u); break;
    case OP_BIND_FRAMEBUFFER: o_glBindFramebuffer((GLenum)a[0].u, (GLuint)a[1].u); break;
    case OP_BIND_TEXTURE: o_glBindTexture((GLenum)a[0].u, (GLuint)a[1].u); break;
    case OP_BIND_VERTEX_ARRAY: o_glBindVertexArray((GLuint)a[0].u); break;
    case OP_BLEND_FUNC: o_glBlendFunc((GLenum)a[0].u, (GLenum)a[1].u); break;
    case OP_BLEND_FUNC_SEPARATE: o_glBlendFuncSeparate((GLenum)a[0].u, (GLenum)a[1].u, (GLenum)a[2].u, (GLenum)a[3].u); break;
    case OP_BUFFER_DATA:
        o_glBufferData((GLenum)a[0].u, (GLsizeiptr)a[1].i, a[2].u ? (const void*)(a + 4) : NULL, (GLenum)a[3].u);
        break;
    case OP_BUFFER_SUB_DATA:
        o_glBufferSubData((GLenum)a[0].u, (GLintptr)a[1].i, (GLsizeiptr)a[2].i, (const void*)(a + 3));
        break;
    case OP_CLEAR: o_glClear((GLbitfield)a[0].u); break;
    case OP_CLEAR_COLOR: o_glClearColor(a[0].f, a[1].f, a[2].f, a[3].f); break;
    case OP_COLOR_MASK: o_glColorMask((GLboolean)a[0].u, (GLboolean)a[1].u, (GLboolean)a[2].u, (GLboolean)a[3].u); break;
    case OP_CULL_FACE: o_glCullFace((GLenum)a[0].u); break;
    case OP_DEPTH_FUNC: o_glDepthFunc((GLenum)a[0].u); break;
    case OP_DEPTH_MASK: o_glDepthMask((GLboolean)a[0].u); break;
    case OP_DISABLE: o_glDisable((GLenum)a[0].u); break;
    case OP_DRAW_ELEMENTS: o_glDrawElements((GLenum)a[0].u, (GLsizei)a[1].i, (GLenum)a[2].u, a[3].p); break;
    case OP_ENABLE: o_glEnable((GLenum)a[0].u); break;
    case OP_FRONT_FACE: o_glFrontFace((GLenum)a[0].u); break;
    case OP_PIXEL_STOREI: o_glPixelStorei((GLenum)a[0].u, (GLint)a[1].i); break;
    case OP_POLYGON_MODE: o_glPolygonMode((GLenum)a[0].u, (GLenum)a[1].u); break;
    case OP_POLYGON_OFFSET: o_glPolygonOffset(a[0].f, a[1].f); break;
    case OP_SCISSOR: o_glScissor((GLint)a[0].i, (GLint)a[1].i, (GLsizei)a[2].i, (GLsizei)a[3].i); break;
    case OP_TEX_PARAMETERF: o_glTexParameterf((GLenum)a[0].u, (GLenum)a[1].u, a[2].f); break;
    case OP_TEX_PARAMETERI: o_glTexParameteri((GLenum)a[0].u, (GLenum)a[1].u, (GLint)a[2].i); break;
    case OP_UNIFORM1F: o_glUniform1f((GLint)a[0].i, a[1].f); break;
    case OP_UNIFORM1I: o_glUniform1i((GLint)a[0].i, (GLint)a[1].i); break;
    case OP_UNIFORM4F: o_glUniform4f((GLint)a[0].i, a[1].f, a[2].f, a[3].f, a[4].f); break;
    case OP_USE_PROGRAM: o_glUseProgram((GLuint)a[0].u); break;
    case OP_VIEWPORT: o_glViewport((GLint)a[0].i, (GLint)a[1].i, (GLsizei)a[2].i, (GLsizei)a[3].i); break;
    case OP_DELETE_BUFFERS: o_glDeleteBuffers((GLsizei)a[0].i, (const GLuint*)(a + 1)); break;
    case OP_DELETE_TEXTURES: o_glDeleteTextures((GLsizei)a[0].i, (const GLuint*)(a + 1)); break;
    case OP_DELETE_VERTEX_ARRAYS: o_glDeleteVertexArrays((GLsizei)a[0].i, (const GLuint*)(a + 1)); break;
    case OP_DELETE_FRAMEBUFFERS: o_glDeleteFramebuffers((GLsizei)a[0].i, (const GLuint*)(a + 1)); break;
    case OP_ENABLE_VERTEX_ATTRIB_ARRAY: o_glEnableVertexAttribArray((GLuint)a[0].u); break;
    case OP_TEX_SUB_IMAGE_2D:
        o_glTexSubImage2D((GLenum)a[0].u, (GLint)a[1].i, (GLint)a[2].i, (GLint)a[3].i, (GLsizei)a[4].i, (GLsizei)a[5].i,
            (GLenum)a[6].u, (GLenum)a[7].u, (const void*)(a + 8));
        break;
    case OP_BLIT_FRAMEBUFFER:
        o_glBlitFramebuffer((GLint)a[0].i, (GLint)a[1].i, (GLint)a[2].i, (GLint)a[3].i, (GLint)a[4].i, (GLint)a[5].i,
            (GLint)a[6].i, (GLint)a[7].i, (GLbitfield)a[8].u, (GLenum)a[9].u);
        break;
    case OP_CLEAR_DEPTH: o_glClearDepth(a[0].d); break;
    case OP_VERTEX_ATTRIB_POINTER:
        o_glVertexAttribPointer((GLuint)a[0].u, (GLint)a[1].i, (GLenum)a[2].u, (GLboolean)a[3].u, (GLsizei)a[4].i, a[5].p);
        break;
    default:
        break;
    }
    if (h->op != OP_CALLBACK && o_glGetError != NULL) {
        GLenum e = o_glGetError();
        uint32_t none = GL_NO_ERROR;
        if (e != GL_NO_ERROR) {
            atomic_compare_exchange_strong(&gError, &none, (uint32_t)e);
        }
    }
}

static int SDLCALL RenderThreadMain(void* unused) {
    (void)unused;
    atomic_store(&gRender_thread_id, CurrentThreadId());
#ifdef __vita__
    {
        /* test: C2_TEST_GLQ_CORE=0/1/2 picks the render thread's user core (default 1) */
        const char* core = getenv("C2_TEST_GLQ_CORE");
        int mask = core != NULL && core[0] >= '0' && core[0] <= '2' ? 0x10000 << (core[0] - '0') : C2_CPU_MASK_RENDER;
        C2VitaLog("VITA GLQ render thread affinity 0x%x: %d\n", mask, sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), mask));
    }
#endif
    for (;;) {
        uint32_t tail = atomic_load_explicit(&gTail, memory_order_relaxed);
        uint32_t head = atomic_load_explicit(&gHead, memory_order_acquire);

        if (tail == head) {
            if (atomic_exchange(&gProducer_waiting, 0)) {
                SDL_SignalSemaphore(gProgress);
            }
            if (atomic_load(&gStop)) {
                return 0;
            }
            atomic_store(&gConsumer_sleeping, 1);
            if (atomic_load(&gHead) == tail && !atomic_load(&gStop)) {
                SDL_WaitSemaphoreTimeout(gWork, 100);
            }
            atomic_store(&gConsumer_sleeping, 0);
            continue;
        }
        {
            uint64_t start = SDL_GetTicksNS();
            unsigned since_store = 0;
            while (tail != head) {
                const tCommand_header* h = (const tCommand_header*)(gRing + tail % QUEUE_BYTES);
                uint32_t size = h->size;
                if (h->op != OP_WRAP) {
                    atomic_store_explicit(&gWorker_op, h->op, memory_order_relaxed);
                    Execute(h);
                    atomic_fetch_add_explicit(&gWorker_done, 1, memory_order_relaxed);
                }
                tail += size;
                /* progress is published every 32 commands, at the end of the batch and whenever
                 * the main thread waits (space, drain or frame) */
                if (++since_store >= 32 || tail == head || atomic_load_explicit(&gProducer_waiting, memory_order_relaxed)) {
                    since_store = 0;
                    atomic_store_explicit(&gTail, tail, memory_order_release);
                    if (atomic_load_explicit(&gProducer_waiting, memory_order_relaxed) && atomic_exchange(&gProducer_waiting, 0)) {
                        SDL_SignalSemaphore(gProgress); /* space freed / progress for a waiting main thread */
                    }
                }
            }
            atomic_fetch_add(&gStat_worker_busy_ns, SDL_GetTicksNS() - start);
        }
    }
}

int C2VitaGLQueueStart(void) {
    if (atomic_load(&gActive)) {
        return 1;
    }
    if (gRing == NULL) {
        gRing = SDL_malloc(QUEUE_BYTES);
        gWork = SDL_CreateSemaphore(0);
        gProgress = SDL_CreateSemaphore(0);
        if (gRing == NULL || gWork == NULL || gProgress == NULL) {
            C2VitaLog("VITA GLQ unavailable: %s\n", SDL_GetError());
            return 0;
        }
    }
    atomic_store(&gHead, 0);
    atomic_store(&gTail, 0);
    gLocal_head = 0;
    gTail_seen = 0;
    atomic_store(&gStop, 0);
#ifdef __vita__
    {
        /* vitaGL (shader compilation, GXM scene setup) needs far more stack than SDL's default */
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)RenderThreadMain);
        SDL_SetStringProperty(props, SDL_PROP_THREAD_CREATE_NAME_STRING, "C2 render");
        SDL_SetNumberProperty(props, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 1024 * 1024);
        gThread = SDL_CreateThreadWithProperties(props);
        SDL_DestroyProperties(props);
    }
#else
    gThread = SDL_CreateThread(RenderThreadMain, "C2 render", NULL);
#endif
    if (gThread == NULL) {
        C2VitaLog("VITA GLQ thread failed: %s\n", SDL_GetError());
        return 0;
    }
    gFrames_queued = atomic_load(&gFrames_done);
    MirrorReset();
    gDedup = getenv("C2_TEST_NODEDUP") == NULL;
    atomic_store(&gActive, 1);
#ifdef __vita__
    /* test: C2_TEST_GLQ_NOPIN leaves the main thread free to move between cores */
    if (getenv("C2_TEST_GLQ_NOPIN") == NULL) {
        C2VitaLog("VITA GLQ main thread affinity: %d\n", sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), C2_CPU_MASK_MAIN));
    }
#endif
    C2VitaLog("VITA GLQ render thread started (ring %u KB)\n", QUEUE_BYTES / 1024);
    return 1;
}

void C2VitaGLQueueStop(void) {
    if (!atomic_load(&gActive)) {
        return;
    }
    Drain();
    atomic_store(&gActive, 0);
    atomic_store(&gStop, 1);
    SDL_SignalSemaphore(gWork);
    SDL_WaitThread(gThread, NULL);
    gThread = NULL;
}

int C2VitaGLQueueActive(void) {
    return atomic_load(&gActive) != 0;
}

void C2VitaGLQueueCall(void (*pFunction)(void*), void* pArgument) {
    if (!IsMainThreadWork()) {
        pFunction(pArgument);
        return;
    }
    MirrorForget(); /* the callback may change any GL state */
    QUEUE2(OP_CALLBACK, P((const void*)pFunction), P(pArgument));
}

static void CountFrameDone(void* unused) {
    (void)unused;
    atomic_fetch_add(&gFrames_done, 1);
}

void C2VitaGLQueueFrame(void (*pSwap)(void*), void* pArgument) {
    if (!IsMainThreadWork()) {
        pSwap(pArgument);
        return;
    }
    /* at most one frame in flight: the previous frame's swap must have run */
    if (gFrames_queued - atomic_load(&gFrames_done) >= 1) {
        uint64_t start = SDL_GetTicksNS(), next_log = 0;
        Flush();
        while (gFrames_queued - atomic_load(&gFrames_done) >= 1) {
            atomic_store(&gProducer_waiting, 1);
            WakeConsumer();
            if (gFrames_queued - atomic_load(&gFrames_done) >= 1) {
                SDL_WaitSemaphoreTimeout(gProgress, 5);
            }
            WaitCheck("frame", start, &next_log);
        }
        gStat_frame_wait_ns += SDL_GetTicksNS() - start;
    }
    QUEUE2(OP_CALLBACK, P((const void*)pSwap), P(pArgument));
    QUEUE2(OP_CALLBACK, P((const void*)CountFrameDone), P(NULL));
    gFrames_queued++;
    Flush();
}

void C2VitaGLQueueFrameLog(unsigned int pFrame) {
    static const char* const names[] = {
        "other", "Get*", "Gen/Delete", "Tex(Sub)Image2D", "GenerateMipmap", "Shader/Program", "Framebuffer",
        "VertexAttrib", "BufferData(large)", "DrawElements(no VAO)", "Blit", "IsEnabled", "PolygonMode"
    };
    unsigned i;
    char by[512];
    int at = 0;

    if (!IsMainThreadWork()) {
        return;
    }
    by[0] = '\0';
    for (i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (gStat_sync_by[i] != 0 && at < (int)sizeof(by) - 40) {
            at += SDL_snprintf(by + at, sizeof(by) - at, " %s=%u", names[i], gStat_sync_by[i]);
        }
    }
    C2VitaLog("VITA GLQ frame=%u queued=%llu syncs=%llu sync_wait_us=%llu space_wait_us=%llu frame_wait_us=%llu worker_busy_us=%llu syncs_by:%s\n",
        pFrame, (unsigned long long)gStat_queued, (unsigned long long)gStat_syncs,
        (unsigned long long)(gStat_sync_wait_ns / 1000), (unsigned long long)(gStat_space_wait_ns / 1000),
        (unsigned long long)(gStat_frame_wait_ns / 1000),
        (unsigned long long)(atomic_exchange(&gStat_worker_busy_ns, 0) / 1000), by);
    gStat_queued = gStat_syncs = gStat_sync_wait_ns = gStat_space_wait_ns = gStat_frame_wait_ns = 0;
    memset(gStat_sync_by, 0, sizeof(gStat_sync_by));
}

/* ---- main-thread mirror of GL state ----
 *
 * While the queue runs, a state query would have to wait for the render thread. The main thread
 * already knows most of the answers: it issued every state change itself. Queued state calls record
 * their values here; queries of mirrored state are answered from the mirror when the value is known
 * and otherwise wait once, read the real value and keep it. Anything that runs GL behind the
 * wrappers' back (the swap and other queued callbacks) forgets the whole mirror.
 * C2_TEST_MIRRORCHECK=1 also reads the real value on every mirrored answer and logs mismatches. */
enum {
    M_BLEND, M_DEPTH_TEST, M_CULL_FACE, M_OFFSET_FILL, M_SCISSOR_TEST,
    M_BLEND_FUNC, M_DEPTH_FUNC, M_DEPTH_MASK, M_CULL_MODE, M_COLOUR_MASK, M_OFFSET, M_VIEWPORT,
    M_DRAW_FBO, M_VAO, M_ARRAY_BUFFER, M_UNIFORM_BUFFER, M_ACTIVE_UNIT, M_TEX0, M_TEX1, M_TEX2, M_TEX3, M_FRONT_FACE,
    M_COUNT
};
typedef struct {
    uint8_t valid;
    union { GLint i[4]; GLfloat f[4]; } v;
} tMirror_value;
static tMirror_value gMirror[M_COUNT];
#define MIRROR_TEXTURES 512
static struct { GLuint name; uint8_t valid_mask; GLint p[5]; } gMirror_tex[MIRROR_TEXTURES];
static int gMirror_check = -1;
static uint32_t gMirror_hits, gMirror_misses, gMirror_mismatches;

/* Bindings and fixed-function state are forgotten; texture parameters are kept (only the
 * wrapped glTexParameter* and glDeleteTextures change them). */
static void MirrorForget(void) {
    memset(gMirror, 0, sizeof(gMirror));
}

static void MirrorReset(void) {
    MirrorForget();
    memset(gMirror_tex, 0, sizeof(gMirror_tex));
}

static void MirrorSet(int pWhich, GLint a, GLint b, GLint c, GLint d) {
    gMirror[pWhich].valid = 1;
    gMirror[pWhich].v.i[0] = a;
    gMirror[pWhich].v.i[1] = b;
    gMirror[pWhich].v.i[2] = c;
    gMirror[pWhich].v.i[3] = d;
}

static int MirrorCap(GLenum pCap) {
    switch (pCap) {
    case GL_BLEND: return M_BLEND;
    case GL_DEPTH_TEST: return M_DEPTH_TEST;
    case GL_CULL_FACE: return M_CULL_FACE;
    case GL_POLYGON_OFFSET_FILL: return M_OFFSET_FILL;
    case GL_SCISSOR_TEST: return M_SCISSOR_TEST;
    default: return -1;
    }
}

static int MirrorTexParam(GLenum pName) {
    switch (pName) {
    case GL_TEXTURE_MIN_FILTER: return 0;
    case GL_TEXTURE_MAG_FILTER: return 1;
    case GL_TEXTURE_WRAP_S: return 2;
    case GL_TEXTURE_WRAP_T: return 3;
    case 0x84FE: return 4; /* GL_TEXTURE_MAX_ANISOTROPY_EXT, set (not queried) as float bits */
    default: return -1;
    }
}

/* the texture bound to GL_TEXTURE_2D of the active unit, or -1 when not known */
static GLint MirrorBoundTexture(void) {
    int unit;
    if (!gMirror[M_ACTIVE_UNIT].valid) {
        return -1;
    }
    unit = gMirror[M_ACTIVE_UNIT].v.i[0];
    if (unit < 0 || unit > 3 || !gMirror[M_TEX0 + unit].valid) {
        return -1;
    }
    return gMirror[M_TEX0 + unit].v.i[0];
}

static int MirrorTexSlot(GLuint pName, int pCreate) {
    unsigned h = (pName * 2654435761u) % MIRROR_TEXTURES, i;
    for (i = 0; i < MIRROR_TEXTURES; i++, h = (h + 1) % MIRROR_TEXTURES) {
        if (gMirror_tex[h].name == pName && gMirror_tex[h].valid_mask != 0) {
            return (int)h;
        }
        if (gMirror_tex[h].valid_mask == 0) {
            if (!pCreate) {
                return -1;
            }
            gMirror_tex[h].name = pName;
            return (int)h;
        }
    }
    return -1;
}

static void MirrorTexParamSet(GLenum pTarget, GLenum pName, GLint pValue) {
    GLint tex = MirrorBoundTexture();
    int p = MirrorTexParam(pName), slot;
    if (pTarget != GL_TEXTURE_2D || p < 0) {
        return;
    }
    if (tex < 0) {
        /* some texture changed and it is not known which one */
        memset(gMirror_tex, 0, sizeof(gMirror_tex));
        return;
    }
    if (tex == 0) {
        return;
    }
    slot = MirrorTexSlot((GLuint)tex, 1);
    if (slot >= 0) {
        gMirror_tex[slot].p[p] = pValue;
        gMirror_tex[slot].valid_mask |= (uint8_t)(1u << p);
    }
}

/* float texture parameters are mirrored as their bits; integer-valued ones as the value */
static GLint MirrorFloatBits(GLenum pName, GLfloat pValue) {
    GLint bits;
    if (pName != 0x84FE) {
        return (GLint)pValue;
    }
    memcpy(&bits, &pValue, sizeof(bits));
    return bits;
}

static int MirrorTexParamKnown(GLenum pTarget, GLenum pName, GLint pValue) {
    GLint tex = MirrorBoundTexture();
    int p = MirrorTexParam(pName), slot;
    if (pTarget != GL_TEXTURE_2D || p < 0 || tex <= 0) {
        return 0;
    }
    slot = MirrorTexSlot((GLuint)tex, 0);
    return slot >= 0 && (gMirror_tex[slot].valid_mask & (1u << p)) && gMirror_tex[slot].p[p] == pValue;
}

/* Answers glGetIntegerv/Booleanv/Floatv-style queries from the mirror: count values, or 0. */
static int MirrorGet(GLenum pName, GLint* pOut, GLfloat* pOut_f) {
    int w = -1, n = 1, first = 0;
    switch (pName) {
    case GL_TEXTURE_BINDING_2D: {
        GLint tex = MirrorBoundTexture();
        if (tex < 0) return 0;
        pOut[0] = tex;
        return 1;
    }
    case GL_BLEND_SRC_RGB: w = M_BLEND_FUNC; first = 0; break;
    case GL_BLEND_DST_RGB: w = M_BLEND_FUNC; first = 1; break;
    case GL_BLEND_SRC_ALPHA: w = M_BLEND_FUNC; first = 2; break;
    case GL_BLEND_DST_ALPHA: w = M_BLEND_FUNC; first = 3; break;
    case GL_DEPTH_FUNC: w = M_DEPTH_FUNC; break;
    case GL_DEPTH_WRITEMASK: w = M_DEPTH_MASK; break;
    case GL_CULL_FACE_MODE: w = M_CULL_MODE; break;
    case GL_COLOR_WRITEMASK: w = M_COLOUR_MASK; n = 4; break;
    case GL_VIEWPORT: w = M_VIEWPORT; n = 4; break;
    case GL_DRAW_FRAMEBUFFER_BINDING: w = M_DRAW_FBO; break;
    case GL_VERTEX_ARRAY_BINDING: w = M_VAO; break;
    case GL_ARRAY_BUFFER_BINDING: w = M_ARRAY_BUFFER; break;
    case GL_UNIFORM_BUFFER_BINDING: w = M_UNIFORM_BUFFER; break;
    case GL_POLYGON_OFFSET_FACTOR: w = M_OFFSET; first = 0; break;
    case GL_POLYGON_OFFSET_UNITS: w = M_OFFSET; first = 1; break;
    default: return 0;
    }
    if (!gMirror[w].valid) {
        return 0;
    }
    if (w == M_OFFSET) {
        pOut_f[0] = gMirror[w].v.f[first];
        return 1;
    }
    memcpy(pOut, &gMirror[w].v.i[first], (size_t)n * sizeof(GLint));
    return n;
}

/* Stores a value read from GL into the mirror (after a waiting query). */
static void MirrorLearn(GLenum pName, const GLint* pValue, const GLfloat* pValue_f) {
    int w;
    switch (pName) {
    case GL_TEXTURE_BINDING_2D:
        if (gMirror[M_ACTIVE_UNIT].valid && gMirror[M_ACTIVE_UNIT].v.i[0] >= 0 && gMirror[M_ACTIVE_UNIT].v.i[0] <= 3) {
            MirrorSet(M_TEX0 + gMirror[M_ACTIVE_UNIT].v.i[0], pValue[0], 0, 0, 0);
        }
        return;
    case GL_DEPTH_FUNC: w = M_DEPTH_FUNC; break;
    case GL_DEPTH_WRITEMASK: w = M_DEPTH_MASK; break;
    case GL_CULL_FACE_MODE: w = M_CULL_MODE; break;
    case GL_COLOR_WRITEMASK: MirrorSet(M_COLOUR_MASK, pValue[0], pValue[1], pValue[2], pValue[3]); return;
    case GL_VIEWPORT: MirrorSet(M_VIEWPORT, pValue[0], pValue[1], pValue[2], pValue[3]); return;
    case GL_DRAW_FRAMEBUFFER_BINDING: w = M_DRAW_FBO; break;
    case GL_VERTEX_ARRAY_BINDING: w = M_VAO; break;
    case GL_ARRAY_BUFFER_BINDING: w = M_ARRAY_BUFFER; break;
    case GL_UNIFORM_BUFFER_BINDING: w = M_UNIFORM_BUFFER; break;
    case GL_ACTIVE_TEXTURE: MirrorSet(M_ACTIVE_UNIT, pValue[0] - GL_TEXTURE0, 0, 0, 0); return;
    case GL_POLYGON_OFFSET_FACTOR:
    case GL_POLYGON_OFFSET_UNITS:
        /* learnt in pairs only (glPolygonOffset sets both) */
        (void)pValue_f;
        return;
    default:
        return; /* the blend functions are learnt from glBlendFunc* only */
    }
    MirrorSet(w, pValue[0], 0, 0, 0);
}

static int MirrorChecking(void) {
    if (gMirror_check < 0) {
        gMirror_check = getenv("C2_TEST_MIRRORCHECK") != NULL;
    }
    return gMirror_check;
}

static void MirrorMismatch(const char* pWhat, GLenum pName, GLint pMirror, GLint pReal) {
    if (++gMirror_mismatches <= 40) {
        C2VitaLog("VITA GLQ MIRROR MISMATCH %s 0x%04x: mirror=%d real=%d\n", pWhat, (unsigned)pName, (int)pMirror, (int)pReal);
    }
}

void C2VitaGLQueueMirrorLog(void) {
    if (gMirror_hits + gMirror_misses == 0) {
        return;
    }
    C2VitaLog("VITA GLQ mirror hits=%u misses=%u mismatches=%u\n", gMirror_hits, gMirror_misses, gMirror_mismatches);
    gMirror_hits = gMirror_misses = 0;
}

/* ---- wrappers ---- */

enum {
    SYNC_OTHER, SYNC_GET, SYNC_GENDEL, SYNC_TEXIMAGE, SYNC_MIPMAP, SYNC_SHADER, SYNC_FRAMEBUFFER,
    SYNC_ATTRIB, SYNC_BIGDATA, SYNC_NOVAO, SYNC_BLIT, SYNC_ISENABLED, SYNC_POLYMODE
};

/* Test profiler (C2_TEST_GLPROF): which call sites make the waiting calls, queue on or off.
 * Logged with the address of this function, so the call sites can be mapped onto the ELF. */
#define PROF_SLOTS 128
static struct { const void* caller; uint32_t cat, count; } gProf[PROF_SLOTS];
static int gProf_on = -1;
static void Profile(uint32_t pCat, const void* pCaller) {
    unsigned h, i;
    if (gProf_on < 0) {
        gProf_on = getenv("C2_TEST_GLPROF") != NULL;
    }
    if (!gProf_on || CurrentThreadId() == atomic_load(&gRender_thread_id)) {
        return;
    }
    h = (unsigned)(((uintptr_t)pCaller >> 1) * 2654435761u) % PROF_SLOTS;
    for (i = 0; i < PROF_SLOTS; i++, h = (h + 1) % PROF_SLOTS) {
        if (gProf[h].caller == pCaller && gProf[h].cat == pCat) {
            gProf[h].count++;
            return;
        }
        if (gProf[h].caller == NULL) {
            gProf[h].caller = pCaller;
            gProf[h].cat = pCat;
            gProf[h].count = 1;
            return;
        }
    }
}

/* Test draw profiler (C2_TEST_DRAWPROF): draw groups per model name. */
#define DRAWPROF_SLOTS 256
static struct { const char* name; uint32_t groups; } gDrawprof[3][DRAWPROF_SLOTS]; /* [0] draw groups, [1] refill kB, [2] model actors walked */
static int gDrawprof_on = -1;
static uint32_t gDrawprof_total[3];
static void NameProfile(int pKind, const char* pModel_name, uint32_t pWeight) {
    unsigned h, i;
    if (gDrawprof_on < 0) {
        gDrawprof_on = getenv("C2_TEST_DRAWPROF") != NULL;
    }
    if (!gDrawprof_on) {
        return;
    }
    gDrawprof_total[pKind] += pWeight;
    h = (unsigned)(((uintptr_t)pModel_name >> 2) * 2654435761u) % DRAWPROF_SLOTS;
    for (i = 0; i < DRAWPROF_SLOTS; i++, h = (h + 1) % DRAWPROF_SLOTS) {
        if (gDrawprof[pKind][h].groups != 0 && gDrawprof[pKind][h].name == pModel_name) {
            gDrawprof[pKind][h].groups += pWeight;
            return;
        }
        if (gDrawprof[pKind][h].groups == 0) {
            gDrawprof[pKind][h].name = pModel_name;
            gDrawprof[pKind][h].groups = pWeight;
            return;
        }
    }
}

void C2VitaDrawProfile(const char* pModel_name) {
    NameProfile(0, pModel_name, 1);
}

/* model actors walked per frame, by actor (or model) name */
void C2VitaActorProfile(const char* pName) {
    NameProfile(2, pName, 1);
}

/* refilled geometry (kB) per model name: which animated models cost the most */
void C2VitaRefreshProfile(unsigned pBytes) {
    extern const char* gDebug_render_model_name;
    NameProfile(1, gDebug_render_model_name, (pBytes + 1023) / 1024);
}

static void DrawProfileLog(unsigned int pFrames) {
    static const char* const kinds[3] = { "groups", "refill_kB", "actors" };
    int n, k;
    if (gDrawprof_on != 1) {
        return;
    }
    for (k = 0; k < 3; k++) {
        C2VitaLog("VITA DRAWPROF frames=%u %s_per_frame=%u.%02u\n", pFrames, kinds[k], gDrawprof_total[k] / pFrames, gDrawprof_total[k] * 100 / pFrames % 100);
        for (n = 0; n < 20; n++) {
            unsigned i, best = DRAWPROF_SLOTS;
            for (i = 0; i < DRAWPROF_SLOTS; i++) {
                if (gDrawprof[k][i].groups != 0 && (best == DRAWPROF_SLOTS || gDrawprof[k][i].groups > gDrawprof[k][best].groups)) best = i;
            }
            if (best == DRAWPROF_SLOTS) break;
            C2VitaLog("VITA DRAWPROF %s %u.%02u/frame %s\n", kinds[k], gDrawprof[k][best].groups / pFrames, gDrawprof[k][best].groups * 100 / pFrames % 100,
                gDrawprof[k][best].name != NULL ? gDrawprof[k][best].name : "(no name)");
            gDrawprof[k][best].groups = 0;
        }
    }
    memset(gDrawprof, 0, sizeof(gDrawprof));
    memset(gDrawprof_total, 0, sizeof(gDrawprof_total));
}

void C2VitaGLQueueProfileLog(unsigned int pFrames) {
    static const char* const names[] = {
        "other", "Get*", "Gen/Delete", "Tex(Sub)Image2D", "GenerateMipmap", "Shader/Program", "Framebuffer",
        "VertexAttrib", "BufferData(large)", "DrawElements(no VAO)", "Blit", "IsEnabled", "PolygonMode"
    };
    int n;
    DrawProfileLog(pFrames);
    if (gProf_on != 1) {
        return;
    }
    C2VitaLog("VITA GLPROF frames=%u anchor=%p (C2VitaGLQueueProfileLog)\n", pFrames, (void*)C2VitaGLQueueProfileLog);
    C2VitaGLQueueMirrorLog();
    for (n = 0; n < 40; n++) {
        unsigned i, best = PROF_SLOTS;
        for (i = 0; i < PROF_SLOTS; i++) {
            if (gProf[i].caller != NULL && (best == PROF_SLOTS || gProf[i].count > gProf[best].count)) {
                best = i;
            }
        }
        if (best == PROF_SLOTS || gProf[best].count == 0) {
            break;
        }
        C2VitaLog("VITA GLPROF total=%u per_frame=%u.%02u caller=%p %s\n", gProf[best].count,
            gProf[best].count / pFrames, gProf[best].count * 100 / pFrames % 100,
            gProf[best].caller, gProf[best].cat < 13 ? names[gProf[best].cat] : "?");
        gProf[best].count = 0;
    }
    memset(gProf, 0, sizeof(gProf));
}

#define SYNC_V(NAME, CAT, PARAMS, ARGS) \
    static void APIENTRY s_##NAME PARAMS { Profile(CAT, __builtin_return_address(0)); SyncFor(CAT); o_##NAME ARGS; }
#define SYNC_R(RET, NAME, CAT, PARAMS, ARGS) \
    static RET APIENTRY s_##NAME PARAMS { Profile(CAT, __builtin_return_address(0)); SyncFor(CAT); return o_##NAME ARGS; }

SYNC_V(glAttachShader, SYNC_SHADER, (GLuint p, GLuint s), (p, s))
SYNC_V(glBindAttribLocation, SYNC_SHADER, (GLuint p, GLuint i, const GLchar* n), (p, i, n))
SYNC_V(glBindRenderbuffer, SYNC_FRAMEBUFFER, (GLenum t, GLuint r), (t, r))
SYNC_R(GLenum, glCheckFramebufferStatus, SYNC_FRAMEBUFFER, (GLenum t), (t))
SYNC_V(glCompileShader, SYNC_SHADER, (GLuint s), (s))
SYNC_R(GLuint, glCreateProgram, SYNC_SHADER, (void), ())
SYNC_R(GLuint, glCreateShader, SYNC_SHADER, (GLenum t), (t))
SYNC_V(glDeleteProgram, SYNC_SHADER, (GLuint p), (p))
SYNC_V(glDeleteRenderbuffers, SYNC_GENDEL, (GLsizei n, const GLuint* b), (n, b))
SYNC_V(glDeleteShader, SYNC_SHADER, (GLuint s), (s))
SYNC_V(glFramebufferRenderbuffer, SYNC_FRAMEBUFFER, (GLenum t, GLenum a, GLenum rt, GLuint r), (t, a, rt, r))
SYNC_V(glFramebufferTexture2D, SYNC_FRAMEBUFFER, (GLenum t, GLenum a, GLenum tt, GLuint tex, GLint l), (t, a, tt, tex, l))
SYNC_V(glGenFramebuffers, SYNC_GENDEL, (GLsizei n, GLuint* b), (n, b))
SYNC_V(glGenRenderbuffers, SYNC_GENDEL, (GLsizei n, GLuint* b), (n, b))
SYNC_V(glGenerateMipmap, SYNC_MIPMAP, (GLenum t), (t))
SYNC_R(GLint, glGetAttribLocation, SYNC_SHADER, (GLuint p, const GLchar* n), (p, n))
SYNC_V(glGetProgramInfoLog, SYNC_SHADER, (GLuint p, GLsizei b, GLsizei* l, GLchar* i), (p, b, l, i))
SYNC_V(glGetProgramiv, SYNC_SHADER, (GLuint p, GLenum n, GLint* v), (p, n, v))
SYNC_V(glGetShaderInfoLog, SYNC_SHADER, (GLuint s, GLsizei b, GLsizei* l, GLchar* i), (s, b, l, i))
SYNC_V(glGetShaderiv, SYNC_SHADER, (GLuint s, GLenum n, GLint* v), (s, n, v))
SYNC_R(const GLubyte*, glGetString, SYNC_GET, (GLenum n), (n))
SYNC_R(const GLubyte*, glGetStringi, SYNC_GET, (GLenum n, GLuint i), (n, i))
SYNC_R(GLuint, glGetUniformBlockIndex, SYNC_SHADER, (GLuint p, const GLchar* n), (p, n))
SYNC_R(GLint, glGetUniformLocation, SYNC_SHADER, (GLuint p, const GLchar* n), (p, n))
SYNC_V(glLinkProgram, SYNC_SHADER, (GLuint p), (p))
SYNC_V(glRenderbufferStorage, SYNC_FRAMEBUFFER, (GLenum t, GLenum f, GLsizei w, GLsizei h), (t, f, w, h))
SYNC_V(glShaderSource, SYNC_SHADER, (GLuint s, GLsizei c, const GLchar* const* str, const GLint* l), (s, c, str, l))
SYNC_V(glTexImage2D, SYNC_TEXIMAGE, (GLenum t, GLint l, GLint i, GLsizei w, GLsizei h, GLint b, GLenum f, GLenum ty, const void* p), (t, l, i, w, h, b, f, ty, p))
SYNC_V(glUniformBlockBinding, SYNC_SHADER, (GLuint p, GLuint i, GLuint b), (p, i, b))

/* mirrored queries: no wait when the main thread knows the answer; the profiler sees only waits */
static void APIENTRY s_glGetIntegerv(GLenum n, GLint* d) {
    GLint m[4];
    GLfloat f;
    int count, i;
    if (!IsMainThreadWork()) {
        o_glGetIntegerv(n, d);
        return;
    }
    count = n == GL_POLYGON_OFFSET_FACTOR || n == GL_POLYGON_OFFSET_UNITS ? 0 : MirrorGet(n, m, &f);
    if (count > 0) {
        gMirror_hits++;
        if (MirrorChecking()) {
            SyncFor(SYNC_GET);
            o_glGetIntegerv(n, d);
            for (i = 0; i < count; i++) {
                if (d[i] != m[i]) MirrorMismatch("int", n, m[i], d[i]);
            }
        }
        memcpy(d, m, (size_t)count * sizeof(GLint));
        return;
    }
    gMirror_misses++;
    Profile(SYNC_GET, __builtin_return_address(0));
    SyncFor(SYNC_GET);
    o_glGetIntegerv(n, d);
    MirrorLearn(n, d, NULL);
}

static void APIENTRY s_glGetBooleanv(GLenum n, GLboolean* d) {
    GLint m[4], real[4];
    GLfloat f;
    int count, i;
    if (!IsMainThreadWork()) {
        o_glGetBooleanv(n, d);
        return;
    }
    count = n == GL_DEPTH_WRITEMASK || n == GL_COLOR_WRITEMASK ? MirrorGet(n, m, &f) : 0;
    if (count > 0) {
        gMirror_hits++;
        if (MirrorChecking()) {
            SyncFor(SYNC_GET);
            o_glGetBooleanv(n, d);
            for (i = 0; i < count; i++) {
                if ((d[i] != 0) != (m[i] != 0)) MirrorMismatch("bool", n, m[i], d[i]);
            }
        }
        for (i = 0; i < count; i++) d[i] = m[i] != 0;
        return;
    }
    gMirror_misses++;
    Profile(SYNC_GET, __builtin_return_address(0));
    SyncFor(SYNC_GET);
    o_glGetBooleanv(n, d);
    if (n == GL_DEPTH_WRITEMASK || n == GL_COLOR_WRITEMASK) {
        for (i = 0; i < (n == GL_COLOR_WRITEMASK ? 4 : 1); i++) real[i] = d[i] != 0;
        MirrorLearn(n, real, NULL);
    }
}

static void APIENTRY s_glGetFloatv(GLenum n, GLfloat* d) {
    GLint m[4];
    GLfloat f, real;
    if (!IsMainThreadWork()) {
        o_glGetFloatv(n, d);
        return;
    }
    if ((n == GL_POLYGON_OFFSET_FACTOR || n == GL_POLYGON_OFFSET_UNITS) && MirrorGet(n, m, &f) > 0) {
        gMirror_hits++;
        if (MirrorChecking()) {
            SyncFor(SYNC_GET);
            o_glGetFloatv(n, &real);
            if (real != f) MirrorMismatch("float*1000", n, (GLint)(f * 1000.f), (GLint)(real * 1000.f));
        }
        d[0] = f;
        return;
    }
    gMirror_misses++;
    Profile(SYNC_GET, __builtin_return_address(0));
    SyncFor(SYNC_GET);
    o_glGetFloatv(n, d);
}

static GLboolean APIENTRY s_glIsEnabled(GLenum c) {
    int w;
    GLboolean real;
    if (!IsMainThreadWork()) {
        return o_glIsEnabled(c);
    }
    w = MirrorCap(c);
    if (w >= 0 && gMirror[w].valid) {
        gMirror_hits++;
        if (MirrorChecking()) {
            SyncFor(SYNC_ISENABLED);
            real = o_glIsEnabled(c);
            if ((real != 0) != (gMirror[w].v.i[0] != 0)) MirrorMismatch("enabled", c, gMirror[w].v.i[0], real);
        }
        return gMirror[w].v.i[0] != 0;
    }
    gMirror_misses++;
    Profile(SYNC_ISENABLED, __builtin_return_address(0));
    SyncFor(SYNC_ISENABLED);
    real = o_glIsEnabled(c);
    if (w >= 0) MirrorSet(w, real != 0, 0, 0, 0);
    return real;
}

static void APIENTRY s_glGetTexParameteriv(GLenum t, GLenum n, GLint* v) {
    GLint tex;
    int p, slot;
    if (!IsMainThreadWork()) {
        o_glGetTexParameteriv(t, n, v);
        return;
    }
    tex = MirrorBoundTexture();
    p = MirrorTexParam(n);
    if (p == 4) p = -1; /* anisotropy is mirrored as float bits: not an integer answer */
    slot = t == GL_TEXTURE_2D && p >= 0 && tex > 0 ? MirrorTexSlot((GLuint)tex, 0) : -1;
    if (slot >= 0 && (gMirror_tex[slot].valid_mask & (1u << p))) {
        gMirror_hits++;
        if (MirrorChecking()) {
            GLint real;
            SyncFor(SYNC_GET);
            o_glGetTexParameteriv(t, n, &real);
            if (real != gMirror_tex[slot].p[p]) MirrorMismatch("texparam", n, gMirror_tex[slot].p[p], real);
        }
        v[0] = gMirror_tex[slot].p[p];
        return;
    }
    gMirror_misses++;
    Profile(SYNC_GET, __builtin_return_address(0));
    SyncFor(SYNC_GET);
    o_glGetTexParameteriv(t, n, v);
    if (t == GL_TEXTURE_2D && p >= 0 && tex > 0) {
        slot = MirrorTexSlot((GLuint)tex, 1);
        if (slot >= 0) {
            gMirror_tex[slot].p[p] = v[0];
            gMirror_tex[slot].valid_mask |= (uint8_t)(1u << p);
        }
    }
}

/* Deletions need no reply: they are ordered commands like the state calls. A deleted object that is
 * bound leaves its binding UNKNOWN in the mirror, not 0: vitaGL keeps the dangling binding (it does
 * not unbind on delete), so the next bind - even of 0 - must reach it (00.61 test: a dropped
 * glBindVertexArray(0) after deleting the bound VAO corrupted the heap). */
static void QueueDelete(uint32_t pOp, GLsizei n, const GLuint* pIds) {
    uint32_t bytes;
    tArg* a = Begin(pOp, 1, (uint32_t)n * (uint32_t)sizeof(GLuint), &bytes);
    a[0] = I(n);
    memcpy(a + 1, pIds, (size_t)n * sizeof(GLuint));
    Publish(bytes);
}

static void APIENTRY s_glDeleteBuffers(GLsizei n, const GLuint* b) {
    GLsizei i;
    for (i = 0; (gDirect_mirror || IsMainThreadWork()) && i < n && b != NULL; i++) {
        if (gMirror[M_ARRAY_BUFFER].valid && gMirror[M_ARRAY_BUFFER].v.i[0] == (GLint)b[i]) gMirror[M_ARRAY_BUFFER].valid = 0;
        if (gMirror[M_UNIFORM_BUFFER].valid && gMirror[M_UNIFORM_BUFFER].v.i[0] == (GLint)b[i]) gMirror[M_UNIFORM_BUFFER].valid = 0;
    }
    if (!IsMainThreadWork()) {
        o_glDeleteBuffers(n, b);
        return;
    }
    if (n <= 0 || b == NULL || n > 4096) {
        Profile(SYNC_GENDEL, __builtin_return_address(0));
        SyncFor(SYNC_GENDEL);
        o_glDeleteBuffers(n, b);
        return;
    }
    QueueDelete(OP_DELETE_BUFFERS, n, b);
}

static void APIENTRY s_glDeleteTextures(GLsizei n, const GLuint* t) {
    GLsizei i;
    int u, slot;
    for (i = 0; (gDirect_mirror || IsMainThreadWork()) && i < n && t != NULL; i++) {
        for (u = 0; u < 4; u++) {
            if (gMirror[M_TEX0 + u].valid && gMirror[M_TEX0 + u].v.i[0] == (GLint)t[i]) gMirror[M_TEX0 + u].valid = 0;
        }
        slot = MirrorTexSlot(t[i], 0);
        if (slot >= 0) gMirror_tex[slot].valid_mask = 0, gMirror_tex[slot].name = 0;
    }
    if (!IsMainThreadWork()) {
        o_glDeleteTextures(n, t);
        return;
    }
    if (n <= 0 || t == NULL || n > 4096) {
        Profile(SYNC_GENDEL, __builtin_return_address(0));
        SyncFor(SYNC_GENDEL);
        o_glDeleteTextures(n, t);
        return;
    }
    QueueDelete(OP_DELETE_TEXTURES, n, t);
}

static void APIENTRY s_glDeleteVertexArrays(GLsizei n, const GLuint* v) {
    GLsizei i;
    for (i = 0; (gDirect_mirror || IsMainThreadWork()) && i < n && v != NULL; i++) {
        if (gMirror[M_VAO].valid && gMirror[M_VAO].v.i[0] == (GLint)v[i]) gMirror[M_VAO].valid = 0;
        if (gProducer_vao == v[i]) gProducer_vao = 0;
    }
    if (!IsMainThreadWork()) {
        o_glDeleteVertexArrays(n, v);
        return;
    }
    if (n <= 0 || v == NULL || n > 4096) {
        Profile(SYNC_GENDEL, __builtin_return_address(0));
        SyncFor(SYNC_GENDEL);
        o_glDeleteVertexArrays(n, v);
        return;
    }
    QueueDelete(OP_DELETE_VERTEX_ARRAYS, n, v);
}

static void APIENTRY s_glDeleteFramebuffers(GLsizei n, const GLuint* f) {
    GLsizei i;
    for (i = 0; (gDirect_mirror || IsMainThreadWork()) && i < n && f != NULL; i++) {
        if (gMirror[M_DRAW_FBO].valid && gMirror[M_DRAW_FBO].v.i[0] == (GLint)f[i]) gMirror[M_DRAW_FBO].valid = 0;
    }
    if (!IsMainThreadWork()) {
        o_glDeleteFramebuffers(n, f);
        return;
    }
    if (n <= 0 || f == NULL || n > 4096) {
        Profile(SYNC_GENDEL, __builtin_return_address(0));
        SyncFor(SYNC_GENDEL);
        o_glDeleteFramebuffers(n, f);
        return;
    }
    QueueDelete(OP_DELETE_FRAMEBUFFERS, n, f);
}

/* Vertex array state: queued; a pointer is an offset (queueable) only with an array buffer bound. */
static void APIENTRY s_glEnableVertexAttribArray(GLuint i) {
    if (!IsMainThreadWork()) {
        o_glEnableVertexAttribArray(i);
        return;
    }
    QUEUE1(OP_ENABLE_VERTEX_ATTRIB_ARRAY, U(i));
}

static void APIENTRY s_glVertexAttribPointer(GLuint i, GLint s, GLenum t, GLboolean n, GLsizei st, const void* p) {
    uint32_t bytes;
    tArg* a;
    if (!IsMainThreadWork()) {
        o_glVertexAttribPointer(i, s, t, n, st, p);
        return;
    }
    if (!gMirror[M_ARRAY_BUFFER].valid || gMirror[M_ARRAY_BUFFER].v.i[0] == 0) {
        Profile(SYNC_ATTRIB, __builtin_return_address(0));
        SyncFor(SYNC_ATTRIB);
        o_glVertexAttribPointer(i, s, t, n, st, p);
        return;
    }
    a = Begin(OP_VERTEX_ATTRIB_POINTER, 6, 0, &bytes);
    a[0] = U(i);
    a[1] = I(s);
    a[2] = U(t);
    a[3] = U(n);
    a[4] = I(st);
    a[5] = P(p);
    Publish(bytes);
}

/* Names come from small pools filled with one waiting call each: a frame that creates and frees
 * objects does not wait for the render thread per object. */
#define GEN_POOL 32
typedef struct { GLuint names[GEN_POOL]; int count; } tGen_pool;
static tGen_pool gPool_buffers, gPool_textures, gPool_vaos;
static void PoolTake(tGen_pool* pPool, PFNGLGENBUFFERSPROC pGen, GLsizei n, GLuint* pOut, const void* pCaller) {
    while (n-- > 0) {
        if (pPool->count == 0) {
            Profile(SYNC_GENDEL, pCaller);
            SyncFor(SYNC_GENDEL);
            pGen(GEN_POOL, pPool->names);
            pPool->count = GEN_POOL;
        }
        *pOut++ = pPool->names[--pPool->count];
    }
}

static void APIENTRY s_glGenBuffers(GLsizei n, GLuint* b) {
    if (!IsMainThreadWork()) {
        o_glGenBuffers(n, b);
        return;
    }
    PoolTake(&gPool_buffers, o_glGenBuffers, n, b, __builtin_return_address(0));
}

static void APIENTRY s_glGenTextures(GLsizei n, GLuint* t) {
    if (!IsMainThreadWork()) {
        o_glGenTextures(n, t);
        return;
    }
    PoolTake(&gPool_textures, (PFNGLGENBUFFERSPROC)o_glGenTextures, n, t, __builtin_return_address(0));
}

static void APIENTRY s_glGenVertexArrays(GLsizei n, GLuint* v) {
    if (!IsMainThreadWork()) {
        o_glGenVertexArrays(n, v);
        return;
    }
    PoolTake(&gPool_vaos, (PFNGLGENBUFFERSPROC)o_glGenVertexArrays, n, v, __builtin_return_address(0));
}

/* Texture uploads from CPU memory: queued with a copy of the pixels when the size is known (tightly
 * packed 16-bit RGB565 / RGBA4444 / RGBA5551 or 32-bit RGBA bytes; vitaGL unpacks with alignment 1). */
static void APIENTRY s_glTexSubImage2D(GLenum t, GLint l, GLint x, GLint y, GLsizei w, GLsizei h, GLenum f, GLenum ty, const void* p) {
    uint32_t bpp = 0, bytes, size;
    tArg* a;
    if (!IsMainThreadWork()) {
        o_glTexSubImage2D(t, l, x, y, w, h, f, ty, p);
        return;
    }
    if (ty == GL_UNSIGNED_SHORT_5_6_5 || ty == GL_UNSIGNED_SHORT_4_4_4_4 || ty == GL_UNSIGNED_SHORT_5_5_5_1) bpp = 2;
    else if (ty == GL_UNSIGNED_BYTE && f == GL_RGBA) bpp = 4;
    size = bpp != 0 && w > 0 && h > 0 ? (uint32_t)w * (uint32_t)h * bpp : 0;
    if (p == NULL || size == 0 || size > QUEUE_MAX_PAYLOAD) {
        Profile(SYNC_TEXIMAGE, __builtin_return_address(0));
        SyncFor(SYNC_TEXIMAGE);
        o_glTexSubImage2D(t, l, x, y, w, h, f, ty, p);
        return;
    }
    a = Begin(OP_TEX_SUB_IMAGE_2D, 8, size, &bytes);
    a[0] = U(t); a[1] = I(l); a[2] = I(x); a[3] = I(y); a[4] = I(w); a[5] = I(h); a[6] = U(f); a[7] = U(ty);
    memcpy(a + 8, p, size);
    Publish(bytes);
}

static void APIENTRY s_glBlitFramebuffer(GLint a0, GLint b0, GLint c0, GLint d0, GLint e0, GLint f0, GLint g0, GLint h0, GLbitfield m, GLenum fl) {
    uint32_t bytes;
    tArg* a;
    if (!IsMainThreadWork()) {
        o_glBlitFramebuffer(a0, b0, c0, d0, e0, f0, g0, h0, m, fl);
        return;
    }
    a = Begin(OP_BLIT_FRAMEBUFFER, 10, 0, &bytes);
    a[0] = I(a0); a[1] = I(b0); a[2] = I(c0); a[3] = I(d0); a[4] = I(e0);
    a[5] = I(f0); a[6] = I(g0); a[7] = I(h0); a[8] = U(m); a[9] = U(fl);
    Publish(bytes);
}

static void APIENTRY s_glClearDepth(GLdouble d) {
    uint32_t bytes;
    tArg* a;
    if (!IsMainThreadWork()) {
        o_glClearDepth(d);
        return;
    }
    a = Begin(OP_CLEAR_DEPTH, 1, 0, &bytes);
    a[0].d = d;
    Publish(bytes);
}

/* queued calls: run directly (on the main thread) while the queue is stopped */
static void APIENTRY q_glActiveTexture(GLenum t) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_ACTIVE_UNIT].valid && gMirror[M_ACTIVE_UNIT].v.i[0] == (GLint)t - GL_TEXTURE0)) return; MirrorSet(M_ACTIVE_UNIT, (GLint)t - GL_TEXTURE0, 0, 0, 0); } o_glActiveTexture(t); return; } if (gDedup && (gMirror[M_ACTIVE_UNIT].valid && gMirror[M_ACTIVE_UNIT].v.i[0] == (GLint)t - GL_TEXTURE0)) return; MirrorSet(M_ACTIVE_UNIT, (GLint)t - GL_TEXTURE0, 0, 0, 0); QUEUE1(OP_ACTIVE_TEXTURE, U(t)); }
static void APIENTRY q_glBindBuffer(GLenum t, GLuint b) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && ((t == GL_ARRAY_BUFFER && gMirror[M_ARRAY_BUFFER].valid && gMirror[M_ARRAY_BUFFER].v.i[0] == (GLint)b) || (t == GL_UNIFORM_BUFFER && gMirror[M_UNIFORM_BUFFER].valid && gMirror[M_UNIFORM_BUFFER].v.i[0] == (GLint)b))) return; if (t == GL_ARRAY_BUFFER) MirrorSet(M_ARRAY_BUFFER, (GLint)b, 0, 0, 0); else if (t == GL_UNIFORM_BUFFER) MirrorSet(M_UNIFORM_BUFFER, (GLint)b, 0, 0, 0); } o_glBindBuffer(t, b); return; } if (gDedup && ((t == GL_ARRAY_BUFFER && gMirror[M_ARRAY_BUFFER].valid && gMirror[M_ARRAY_BUFFER].v.i[0] == (GLint)b) || (t == GL_UNIFORM_BUFFER && gMirror[M_UNIFORM_BUFFER].valid && gMirror[M_UNIFORM_BUFFER].v.i[0] == (GLint)b))) return; if (t == GL_ARRAY_BUFFER) MirrorSet(M_ARRAY_BUFFER, (GLint)b, 0, 0, 0); else if (t == GL_UNIFORM_BUFFER) MirrorSet(M_UNIFORM_BUFFER, (GLint)b, 0, 0, 0); QUEUE2(OP_BIND_BUFFER, U(t), U(b)); }
static void APIENTRY q_glBindBufferBase(GLenum t, GLuint i, GLuint b) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (t == GL_UNIFORM_BUFFER) MirrorSet(M_UNIFORM_BUFFER, (GLint)b, 0, 0, 0); } o_glBindBufferBase(t, i, b); return; } if (t == GL_UNIFORM_BUFFER) MirrorSet(M_UNIFORM_BUFFER, (GLint)b, 0, 0, 0); QUEUE3(OP_BIND_BUFFER_BASE, U(t), U(i), U(b)); }
static void APIENTRY q_glBindFramebuffer(GLenum t, GLuint f) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (t == GL_FRAMEBUFFER || t == GL_DRAW_FRAMEBUFFER) MirrorSet(M_DRAW_FBO, (GLint)f, 0, 0, 0); } o_glBindFramebuffer(t, f); return; } if (t == GL_FRAMEBUFFER || t == GL_DRAW_FRAMEBUFFER) MirrorSet(M_DRAW_FBO, (GLint)f, 0, 0, 0); QUEUE2(OP_BIND_FRAMEBUFFER, U(t), U(f)); }
static void APIENTRY q_glBindTexture(GLenum t, GLuint x) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (t == GL_TEXTURE_2D && MirrorBoundTexture() == (GLint)x)) return; if (t == GL_TEXTURE_2D && gMirror[M_ACTIVE_UNIT].valid && gMirror[M_ACTIVE_UNIT].v.i[0] >= 0 && gMirror[M_ACTIVE_UNIT].v.i[0] <= 3) MirrorSet(M_TEX0 + gMirror[M_ACTIVE_UNIT].v.i[0], (GLint)x, 0, 0, 0); else if (t == GL_TEXTURE_2D) { gMirror[M_TEX0].valid = gMirror[M_TEX1].valid = gMirror[M_TEX2].valid = gMirror[M_TEX3].valid = 0; } } o_glBindTexture(t, x); return; } if (gDedup && (t == GL_TEXTURE_2D && MirrorBoundTexture() == (GLint)x)) return; if (t == GL_TEXTURE_2D && gMirror[M_ACTIVE_UNIT].valid && gMirror[M_ACTIVE_UNIT].v.i[0] >= 0 && gMirror[M_ACTIVE_UNIT].v.i[0] <= 3) MirrorSet(M_TEX0 + gMirror[M_ACTIVE_UNIT].v.i[0], (GLint)x, 0, 0, 0); else if (t == GL_TEXTURE_2D) { gMirror[M_TEX0].valid = gMirror[M_TEX1].valid = gMirror[M_TEX2].valid = gMirror[M_TEX3].valid = 0; } QUEUE2(OP_BIND_TEXTURE, U(t), U(x)); }
static void APIENTRY q_glBindVertexArray(GLuint a) { gProducer_vao = a; if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_VAO].valid && gMirror[M_VAO].v.i[0] == (GLint)a)) return; MirrorSet(M_VAO, (GLint)a, 0, 0, 0); } o_glBindVertexArray(a); return; } if (gDedup && (gMirror[M_VAO].valid && gMirror[M_VAO].v.i[0] == (GLint)a)) return; MirrorSet(M_VAO, (GLint)a, 0, 0, 0); QUEUE1(OP_BIND_VERTEX_ARRAY, U(a)); }
static void APIENTRY q_glBlendFunc(GLenum s, GLenum d) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_BLEND_FUNC].valid && gMirror[M_BLEND_FUNC].v.i[0] == (GLint)s && gMirror[M_BLEND_FUNC].v.i[1] == (GLint)d && gMirror[M_BLEND_FUNC].v.i[2] == (GLint)s && gMirror[M_BLEND_FUNC].v.i[3] == (GLint)d)) return; MirrorSet(M_BLEND_FUNC, (GLint)s, (GLint)d, (GLint)s, (GLint)d); } o_glBlendFunc(s, d); return; } if (gDedup && (gMirror[M_BLEND_FUNC].valid && gMirror[M_BLEND_FUNC].v.i[0] == (GLint)s && gMirror[M_BLEND_FUNC].v.i[1] == (GLint)d && gMirror[M_BLEND_FUNC].v.i[2] == (GLint)s && gMirror[M_BLEND_FUNC].v.i[3] == (GLint)d)) return; MirrorSet(M_BLEND_FUNC, (GLint)s, (GLint)d, (GLint)s, (GLint)d); QUEUE2(OP_BLEND_FUNC, U(s), U(d)); }
static void APIENTRY q_glBlendFuncSeparate(GLenum a, GLenum b, GLenum c, GLenum d) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_BLEND_FUNC].valid && gMirror[M_BLEND_FUNC].v.i[0] == (GLint)a && gMirror[M_BLEND_FUNC].v.i[1] == (GLint)b && gMirror[M_BLEND_FUNC].v.i[2] == (GLint)c && gMirror[M_BLEND_FUNC].v.i[3] == (GLint)d)) return; MirrorSet(M_BLEND_FUNC, (GLint)a, (GLint)b, (GLint)c, (GLint)d); } o_glBlendFuncSeparate(a, b, c, d); return; } if (gDedup && (gMirror[M_BLEND_FUNC].valid && gMirror[M_BLEND_FUNC].v.i[0] == (GLint)a && gMirror[M_BLEND_FUNC].v.i[1] == (GLint)b && gMirror[M_BLEND_FUNC].v.i[2] == (GLint)c && gMirror[M_BLEND_FUNC].v.i[3] == (GLint)d)) return; MirrorSet(M_BLEND_FUNC, (GLint)a, (GLint)b, (GLint)c, (GLint)d); QUEUE4(OP_BLEND_FUNC_SEPARATE, U(a), U(b), U(c), U(d)); }
static void APIENTRY q_glClear(GLbitfield m) { if (!IsMainThreadWork()) { o_glClear(m); return; } QUEUE1(OP_CLEAR, U(m)); }
static void APIENTRY q_glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { if (!IsMainThreadWork()) { o_glClearColor(r, g, b, a); return; } QUEUE4(OP_CLEAR_COLOR, F(r), F(g), F(b), F(a)); }
static void APIENTRY q_glColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_COLOUR_MASK].valid && gMirror[M_COLOUR_MASK].v.i[0] == (r != 0) && gMirror[M_COLOUR_MASK].v.i[1] == (g != 0) && gMirror[M_COLOUR_MASK].v.i[2] == (b != 0) && gMirror[M_COLOUR_MASK].v.i[3] == (a != 0))) return; MirrorSet(M_COLOUR_MASK, r != 0, g != 0, b != 0, a != 0); } o_glColorMask(r, g, b, a); return; } if (gDedup && (gMirror[M_COLOUR_MASK].valid && gMirror[M_COLOUR_MASK].v.i[0] == (r != 0) && gMirror[M_COLOUR_MASK].v.i[1] == (g != 0) && gMirror[M_COLOUR_MASK].v.i[2] == (b != 0) && gMirror[M_COLOUR_MASK].v.i[3] == (a != 0))) return; MirrorSet(M_COLOUR_MASK, r != 0, g != 0, b != 0, a != 0); QUEUE4(OP_COLOR_MASK, U(r), U(g), U(b), U(a)); }
static void APIENTRY q_glCullFace(GLenum m) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_CULL_MODE].valid && gMirror[M_CULL_MODE].v.i[0] == (GLint)m)) return; MirrorSet(M_CULL_MODE, (GLint)m, 0, 0, 0); } o_glCullFace(m); return; } if (gDedup && (gMirror[M_CULL_MODE].valid && gMirror[M_CULL_MODE].v.i[0] == (GLint)m)) return; MirrorSet(M_CULL_MODE, (GLint)m, 0, 0, 0); QUEUE1(OP_CULL_FACE, U(m)); }
static void APIENTRY q_glDepthFunc(GLenum f) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_DEPTH_FUNC].valid && gMirror[M_DEPTH_FUNC].v.i[0] == (GLint)f)) return; MirrorSet(M_DEPTH_FUNC, (GLint)f, 0, 0, 0); } o_glDepthFunc(f); return; } if (gDedup && (gMirror[M_DEPTH_FUNC].valid && gMirror[M_DEPTH_FUNC].v.i[0] == (GLint)f)) return; MirrorSet(M_DEPTH_FUNC, (GLint)f, 0, 0, 0); QUEUE1(OP_DEPTH_FUNC, U(f)); }
static void APIENTRY q_glDepthMask(GLboolean f) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_DEPTH_MASK].valid && gMirror[M_DEPTH_MASK].v.i[0] == (f != 0))) return; MirrorSet(M_DEPTH_MASK, f != 0, 0, 0, 0); } o_glDepthMask(f); return; } if (gDedup && (gMirror[M_DEPTH_MASK].valid && gMirror[M_DEPTH_MASK].v.i[0] == (f != 0))) return; MirrorSet(M_DEPTH_MASK, f != 0, 0, 0, 0); QUEUE1(OP_DEPTH_MASK, U(f)); }
static void APIENTRY q_glDisable(GLenum c) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (MirrorCap(c) >= 0 && gMirror[MirrorCap(c)].valid && gMirror[MirrorCap(c)].v.i[0] == 0)) return; if (MirrorCap(c) >= 0) MirrorSet(MirrorCap(c), 0, 0, 0, 0); } o_glDisable(c); return; } if (gDedup && (MirrorCap(c) >= 0 && gMirror[MirrorCap(c)].valid && gMirror[MirrorCap(c)].v.i[0] == 0)) return; if (MirrorCap(c) >= 0) MirrorSet(MirrorCap(c), 0, 0, 0, 0); QUEUE1(OP_DISABLE, U(c)); }
static void APIENTRY q_glEnable(GLenum c) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (MirrorCap(c) >= 0 && gMirror[MirrorCap(c)].valid && gMirror[MirrorCap(c)].v.i[0] == 1)) return; if (MirrorCap(c) >= 0) MirrorSet(MirrorCap(c), 1, 0, 0, 0); } o_glEnable(c); return; } if (gDedup && (MirrorCap(c) >= 0 && gMirror[MirrorCap(c)].valid && gMirror[MirrorCap(c)].v.i[0] == 1)) return; if (MirrorCap(c) >= 0) MirrorSet(MirrorCap(c), 1, 0, 0, 0); QUEUE1(OP_ENABLE, U(c)); }
static void APIENTRY q_glFrontFace(GLenum m) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_FRONT_FACE].valid && gMirror[M_FRONT_FACE].v.i[0] == (GLint)m)) return; MirrorSet(M_FRONT_FACE, (GLint)m, 0, 0, 0); } o_glFrontFace(m); return; } if (gDedup && (gMirror[M_FRONT_FACE].valid && gMirror[M_FRONT_FACE].v.i[0] == (GLint)m)) return; MirrorSet(M_FRONT_FACE, (GLint)m, 0, 0, 0); QUEUE1(OP_FRONT_FACE, U(m)); }
static void APIENTRY q_glPixelStorei(GLenum n, GLint v) { if (!IsMainThreadWork()) { o_glPixelStorei(n, v); return; } QUEUE2(OP_PIXEL_STOREI, U(n), I(v)); }
static void APIENTRY q_glPolygonMode(GLenum f, GLenum m) { if (!IsMainThreadWork()) { o_glPolygonMode(f, m); return; } QUEUE2(OP_POLYGON_MODE, U(f), U(m)); }
static void APIENTRY q_glPolygonOffset(GLfloat f, GLfloat u) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_OFFSET].valid && gMirror[M_OFFSET].v.f[0] == f && gMirror[M_OFFSET].v.f[1] == u)) return; gMirror[M_OFFSET].valid = 1; gMirror[M_OFFSET].v.f[0] = f; gMirror[M_OFFSET].v.f[1] = u; } o_glPolygonOffset(f, u); return; } if (gDedup && (gMirror[M_OFFSET].valid && gMirror[M_OFFSET].v.f[0] == f && gMirror[M_OFFSET].v.f[1] == u)) return; gMirror[M_OFFSET].valid = 1; gMirror[M_OFFSET].v.f[0] = f; gMirror[M_OFFSET].v.f[1] = u; QUEUE2(OP_POLYGON_OFFSET, F(f), F(u)); }
static void APIENTRY q_glScissor(GLint x, GLint y, GLsizei w, GLsizei h) { if (!IsMainThreadWork()) { o_glScissor(x, y, w, h); return; } QUEUE4(OP_SCISSOR, I(x), I(y), I(w), I(h)); }
static void APIENTRY q_glTexParameterf(GLenum t, GLenum n, GLfloat v) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (MirrorTexParamKnown(t, n, MirrorFloatBits(n, v)))) return; MirrorTexParamSet(t, n, MirrorFloatBits(n, v)); } o_glTexParameterf(t, n, v); return; } if (gDedup && (MirrorTexParamKnown(t, n, MirrorFloatBits(n, v)))) return; MirrorTexParamSet(t, n, MirrorFloatBits(n, v)); QUEUE3(OP_TEX_PARAMETERF, U(t), U(n), F(v)); }
static void APIENTRY q_glTexParameteri(GLenum t, GLenum n, GLint v) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (MirrorTexParamKnown(t, n, v))) return; MirrorTexParamSet(t, n, v); } o_glTexParameteri(t, n, v); return; } if (gDedup && (MirrorTexParamKnown(t, n, v))) return; MirrorTexParamSet(t, n, v); QUEUE3(OP_TEX_PARAMETERI, U(t), U(n), I(v)); }
static void APIENTRY q_glUniform1f(GLint l, GLfloat v) { if (!IsMainThreadWork()) { o_glUniform1f(l, v); return; } QUEUE2(OP_UNIFORM1F, I(l), F(v)); }
static void APIENTRY q_glUniform1i(GLint l, GLint v) { if (!IsMainThreadWork()) { o_glUniform1i(l, v); return; } QUEUE2(OP_UNIFORM1I, I(l), I(v)); }
static void APIENTRY q_glUniform4f(GLint l, GLfloat a, GLfloat b, GLfloat c, GLfloat d) { if (!IsMainThreadWork()) { o_glUniform4f(l, a, b, c, d); return; } QUEUE5(OP_UNIFORM4F, I(l), F(a), F(b), F(c), F(d)); }
static void APIENTRY q_glUseProgram(GLuint p) { if (!IsMainThreadWork()) { o_glUseProgram(p); return; } QUEUE1(OP_USE_PROGRAM, U(p)); }
static void APIENTRY q_glViewport(GLint x, GLint y, GLsizei w, GLsizei h) { if (!IsMainThreadWork()) { if (gDirect_mirror) { if (gDedup && (gMirror[M_VIEWPORT].valid && gMirror[M_VIEWPORT].v.i[0] == x && gMirror[M_VIEWPORT].v.i[1] == y && gMirror[M_VIEWPORT].v.i[2] == w && gMirror[M_VIEWPORT].v.i[3] == h)) return; MirrorSet(M_VIEWPORT, x, y, w, h); } o_glViewport(x, y, w, h); return; } if (gDedup && (gMirror[M_VIEWPORT].valid && gMirror[M_VIEWPORT].v.i[0] == x && gMirror[M_VIEWPORT].v.i[1] == y && gMirror[M_VIEWPORT].v.i[2] == w && gMirror[M_VIEWPORT].v.i[3] == h)) return; MirrorSet(M_VIEWPORT, x, y, w, h); QUEUE4(OP_VIEWPORT, I(x), I(y), I(w), I(h)); }

static void APIENTRY q_glBufferData(GLenum t, GLsizeiptr size, const void* data, GLenum usage) {
    uint32_t bytes;
    tArg* a;
    uint32_t payload = data != NULL && size > 0 ? (uint32_t)size : 0;

    if (!IsMainThreadWork()) {
        o_glBufferData(t, size, data, usage);
        return;
    }
    if (size < 0 || payload > QUEUE_MAX_PAYLOAD) {
        SyncFor(SYNC_BIGDATA);
        o_glBufferData(t, size, data, usage);
        return;
    }
    a = Begin(OP_BUFFER_DATA, 4, payload, &bytes);
    a[0] = U(t);
    a[1] = I(size);
    a[2] = U(data != NULL);
    a[3] = U(usage);
    if (payload != 0) {
        memcpy(a + 4, data, payload);
    }
    Publish(bytes);
}

static void APIENTRY q_glBufferSubData(GLenum t, GLintptr offset, GLsizeiptr size, const void* data) {
    uint32_t bytes;
    tArg* a;

    if (!IsMainThreadWork()) {
        o_glBufferSubData(t, offset, size, data);
        return;
    }
    if (data == NULL || size < 0 || (uint64_t)size > QUEUE_MAX_PAYLOAD) {
        SyncFor(SYNC_BIGDATA);
        o_glBufferSubData(t, offset, size, data);
        return;
    }
    a = Begin(OP_BUFFER_SUB_DATA, 3, (uint32_t)size, &bytes);
    a[0] = U(t);
    a[1] = I(offset);
    a[2] = I(size);
    memcpy(a + 3, data, (size_t)size);
    Publish(bytes);
}

static void APIENTRY q_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if (!IsMainThreadWork()) {
        o_glDrawElements(mode, count, type, indices);
        return;
    }
    if (gProducer_vao == 0) {
        /* indices (or the vertex attributes) may be client memory the caller reuses right away */
        SyncFor(SYNC_NOVAO);
        o_glDrawElements(mode, count, type, indices);
        return;
    }
    QUEUE4(OP_DRAW_ELEMENTS, U(mode), I(count), U(type), P(indices));
}

static GLenum APIENTRY q_glGetError(void) {
    if (!IsMainThreadWork()) {
        return o_glGetError();
    }
    return (GLenum)atomic_exchange(&gError, GL_NO_ERROR);
}

void* C2VitaGLQueueWrap(const char* pName, void* pOriginal) {
#define QUEUED(NAME) if (strcmp(pName, #NAME) == 0) { o_##NAME = (void*)pOriginal; { static void* check_; check_ = (void*)(1 ? q_##NAME : o_##NAME); (void)check_; } return (void*)q_##NAME; }
#define SYNCED(NAME) if (strcmp(pName, #NAME) == 0) { o_##NAME = (void*)pOriginal; { static void* check_; check_ = (void*)(1 ? s_##NAME : o_##NAME); (void)check_; } return (void*)s_##NAME; }
    if (pOriginal == NULL) {
        return NULL;
    }
    QUEUED(glActiveTexture) QUEUED(glBindBuffer) QUEUED(glBindBufferBase) QUEUED(glBindFramebuffer)
    QUEUED(glBindTexture) QUEUED(glBindVertexArray) QUEUED(glBlendFunc) QUEUED(glBlendFuncSeparate)
    QUEUED(glBufferData) QUEUED(glBufferSubData) QUEUED(glClear) QUEUED(glClearColor) QUEUED(glColorMask)
    QUEUED(glCullFace) QUEUED(glDepthFunc) QUEUED(glDepthMask) QUEUED(glDisable) QUEUED(glDrawElements)
    QUEUED(glEnable) QUEUED(glFrontFace) QUEUED(glGetError) QUEUED(glPixelStorei) QUEUED(glPolygonMode)
    QUEUED(glPolygonOffset) QUEUED(glScissor) QUEUED(glTexParameterf) QUEUED(glTexParameteri)
    QUEUED(glUniform1f) QUEUED(glUniform1i) QUEUED(glUniform4f) QUEUED(glUseProgram) QUEUED(glViewport)
    SYNCED(glAttachShader) SYNCED(glBindAttribLocation) SYNCED(glBindRenderbuffer) SYNCED(glBlitFramebuffer)
    SYNCED(glCheckFramebufferStatus) SYNCED(glClearDepth) SYNCED(glCompileShader) SYNCED(glCreateProgram)
    SYNCED(glCreateShader) SYNCED(glDeleteBuffers) SYNCED(glDeleteFramebuffers) SYNCED(glDeleteProgram)
    SYNCED(glDeleteRenderbuffers) SYNCED(glDeleteShader) SYNCED(glDeleteTextures) SYNCED(glDeleteVertexArrays)
    SYNCED(glEnableVertexAttribArray) SYNCED(glFramebufferRenderbuffer) SYNCED(glFramebufferTexture2D)
    SYNCED(glGenBuffers) SYNCED(glGenFramebuffers) SYNCED(glGenRenderbuffers) SYNCED(glGenTextures)
    SYNCED(glGenVertexArrays) SYNCED(glGenerateMipmap) SYNCED(glGetAttribLocation) SYNCED(glGetBooleanv)
    SYNCED(glGetFloatv) SYNCED(glGetIntegerv) SYNCED(glGetProgramInfoLog) SYNCED(glGetProgramiv)
    SYNCED(glGetShaderInfoLog) SYNCED(glGetShaderiv) SYNCED(glGetString) SYNCED(glGetStringi)
    SYNCED(glGetTexParameteriv) SYNCED(glGetUniformBlockIndex) SYNCED(glGetUniformLocation) SYNCED(glIsEnabled)
    SYNCED(glLinkProgram) SYNCED(glRenderbufferStorage) SYNCED(glShaderSource) SYNCED(glTexImage2D)
    SYNCED(glTexSubImage2D) SYNCED(glUniformBlockBinding) SYNCED(glVertexAttribPointer)
#undef QUEUED
#undef SYNCED
    /* not a function this queue knows: must not be called while the queue runs */
    C2VitaLog("VITA GLQ: %s is not wrapped; it bypasses the render thread\n", pName);
    return pOriginal;
}

/* Called at every frame swap on the main thread: the swap and the loading screen use vitaGL directly,
 * so without the render thread the mirror starts every frame unknown. */
void C2VitaGLQueueDirectFrame(void) {
    extern int C2VitaABFeature(const char* pFeature, int pDefault);
    if (atomic_load(&gActive)) {
        gDirect_mirror = 0;
        return;
    }
    MirrorForget();
    gDirect_mirror = C2VitaABFeature("directmirror", 0);
}
