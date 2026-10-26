/* VitaGL owns GXM. SDL is used for window events, input, audio and files only;
 * no SDL renderer or software screen presentation is created in this build. */
#include "vita_video.h"
#include "vita_gl.h"
#include "vita_perf.h"
#include <vitaGL.h>
#include <brender/brender.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <psp2/display.h>

extern void C2VitaLog(const char* format, ...);
extern int gGLFrameHasWorld;
static int initialized, loading, preparing_menu;
static GLuint font_texture;
static Uint64 last_progress;
static unsigned int frames;
static Uint64 last_swap;

static void draw_text(float x, float y, const char* text) {
    glBindTexture(GL_TEXTURE_2D, font_texture);
    glBegin(GL_QUADS);
    for (; *text; ++text, x += 16.f) {
        unsigned c = (unsigned char)*text;
        float u = (c & 15) / 16.f, v = (c >> 4) / 16.f;
        glTexCoord2f(u, v); glVertex2f(x, y);
        glTexCoord2f(u + 3.f/128, v); glVertex2f(x + 12, y);
        glTexCoord2f(u + 3.f/128, v + 5.f/128); glVertex2f(x + 12, y + 20);
        glTexCoord2f(u, v + 5.f/128); glVertex2f(x, y + 20);
    }
    glEnd();
}

void C2VitaVideoLoadingProgress(void) {
    Uint64 now = SDL_GetTicks();
    if (!initialized || !loading) return;
    if (last_progress && now - last_progress < 100) return;
    last_progress = now;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    C2VitaGLResetBindings();
    glUseProgram(0);
    glViewport(0, 0, 960, 544);
    glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE); glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1); glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0,960,544,0,-1,1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,1,1,1);
    draw_text(48,216,"Carmageddon 2 - VitaGL");
    draw_text(48,264,preparing_menu ? "Preparing main menu..." : "Loading game data...");
    draw_text(48,312,"Please wait");
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_BLEND); glDisable(GL_TEXTURE_2D);
    C2VitaGLInvalidateState();
    vglSwapBuffers(GL_FALSE);
}

void C2VitaVideoPreparingMenu(void) {
    preparing_menu = 1;
    last_progress = 0;
    C2VitaVideoLoadingProgress();
}

int C2VitaVideoInit(SDL_Window* window) {
    static const char* compiler_paths[] = {
        "ur0:/data/libshacccg.suprx", "ur0:/data/external/libshacccg.suprx"
    };
    Uint8 atlas[128 * 128 * 4];
    br_font* font = BrFontFixed3x5;
    int i, x, y, found = 0;
    GLboolean resolution_clamped;
    (void)window;
    for (i = 0; i < 2; ++i) {
        SDL_PathInfo info = {0};
        if (SDL_GetPathInfo(compiler_paths[i], &info) && info.type == SDL_PATHTYPE_FILE) {
            found = 1;
            C2VitaLog("VitaGL shader compiler file: %s\n", compiler_paths[i]);
        }
    }
    if (!found) {
        SDL_SetError("VitaGL needs libshacccg.suprx in ur0:data or ur0:data/external. See VITA_BOOT.LOG.");
        C2VitaLog("VitaGL startup stopped: shader compiler file not found\n");
        return 0;
    }
    /* Newlib already reserves the game heap. Leave native user RAM available
     * for audio/thread/dialog allocations instead of consuming it all. */
    /* The three loading text lines use VitaGL immediate-mode vertices.
     * Reserve 64 KiB for them; a zero pool leaves native vertex writes NULL.
     * This API returns whether the resolution was clamped, NOT success.
     * GL_FALSE is the normal result when 960x544 was accepted. */
    C2VitaLog("VitaGL initialization starting: 960x544, no MSAA, loading pool=65536\n");
    /* All non-default VAOs use VBO-backed attributes. Native reset_vao needs
     * 256 bytes for 16 default float4s; 1 KiB avoids 64 KiB per temporary model. */
    vglSetVertexAttribPoolSize(256 * 1024, 1024);
    vglSetupScratchMemory(GL_TRUE, GL_TRUE);
    resolution_clamped = vglInitExtended(64*1024,960,544,32*1024*1024,SCE_GXM_MULTISAMPLE_NONE);
    C2VitaLog("VitaGL initialization returned: resolution_clamped=%u\n", (unsigned)resolution_clamped);
    if (!C2VitaGLValidate()) {
        SDL_SetError("VitaGL is missing a required renderer function. See VITA_BOOT.LOG.");
        return 0;
    }
    initialized = loading = 1;
    preparing_menu = frames = 0;
    last_progress = last_swap = 0;
    C2VitaLog("VitaGL initialized: %s / %s; native GPU renderer, no software fallback\n",
        glGetString(GL_VERSION), glGetString(GL_RENDERER));
    memset(atlas,0,sizeof(atlas));
    for (i=0;i<256;++i) for(y=0;y<5;++y) for(x=0;x<3;++x) {
        if (font->glyphs[font->encoding[i]+y] & (0x80 >> x)) {
            Uint8* pixel = atlas + (((i>>4)*8+y)*128 + (i&15)*8+x)*4;
            pixel[0]=pixel[1]=pixel[2]=pixel[3]=255;
        }
    }
    glGenTextures(1,&font_texture); glBindTexture(GL_TEXTURE_2D,font_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,128,128,0,GL_RGBA,GL_UNSIGNED_BYTE,atlas);
    C2VitaLog("VitaGL loading font texture ready; first GPU frame starting\n");
    C2VitaVideoLoadingProgress();
    C2VitaLog("VitaGL first loading frame submitted\n");
    return 1;
}

/* Frame rate setting (Options > Graphics > FRAME RATE), saved in ux0:/data/c2port/FRAMERATE.CFG:
 * 0 = 30 fps cap (default): submit a frame no sooner than two display refreshes after the previous
 *     one, so fast frames are paced evenly instead of alternating 60/30. Slower frames are not delayed.
 * 1 = 60 fps: no cap, swaps still wait for vertical sync.
 * 2 = unlimited: no cap and no vsync (fastest, may tear). */
#define C2_FRAMERATE_CFG "ux0:/data/c2port/FRAMERATE.CFG"
static int frame_mode = -1;
static void apply_frame_mode(void) {
    vglWaitVblankStart(frame_mode == 2 ? GL_FALSE : GL_TRUE);
    C2VitaLog("VITA PACE frame rate %s\n", frame_mode == 0 ? "30 fps cap" : frame_mode == 1 ? "60 fps (vsync)" : "unlimited (no vsync)");
}
int C2VitaGetFrameMode(void) {
    if (frame_mode < 0) {
        FILE* f = fopen(C2_FRAMERATE_CFG, "rb");
        int c = f != NULL ? fgetc(f) : EOF;
        if (f) fclose(f);
        if (c >= '0' && c <= '2') {
            frame_mode = c - '0';
        } else {
#ifdef C2_VITA_UNCAPPED
            frame_mode = 2;
#else
            /* legacy switch file from the test builds */
            f = fopen("ux0:/data/c2port/FPS60.SET", "rb");
            frame_mode = f != NULL ? 1 : 0;
            if (f) fclose(f);
#endif
        }
        apply_frame_mode();
    }
    return frame_mode;
}
void C2VitaSetFrameMode(int mode) {
    FILE* f;
    if (mode < 0 || mode > 2 || mode == C2VitaGetFrameMode()) return;
    frame_mode = mode;
    f = fopen(C2_FRAMERATE_CFG, "wb");
    if (f != NULL) {
        fputc('0' + mode, f);
        fclose(f);
    }
    apply_frame_mode();
}
static void pace_frame(void) {
    static unsigned last_vcount;
    static int have_last;
    unsigned now;
    int vblanks = C2VitaGetFrameMode() == 0 ? 2 : 1;
    now = (unsigned)sceDisplayGetVcount();
    if (vblanks > 1 && have_last) {
        /* The vblank counter is 16 bits: it wraps to 0 every 65536 vblanks (~18.2 min at 59.94 Hz).
         * A plain difference then went negative and this loop waited forever. */
        while (((now - last_vcount) & 0xffff) < (unsigned)vblanks) {
            sceDisplayWaitVblankStart();
            now = (unsigned)sceDisplayGetVcount();
        }
    }
    last_vcount = now;
    have_last = 1;
}

void C2VitaVideoSwap(int ready) {
    Uint64 swap_start;
    if (!initialized) return;
    if (!ready) {
        last_progress = 0;
        C2VitaVideoLoadingProgress();
        return;
    }
    loading = 0;
    pace_frame();
    swap_start = SDL_GetTicks();
    vglSwapBuffers(GL_FALSE);
    {
        Uint64 now = SDL_GetTicks();
        if (++frames <= 8 || (frames <= 6000 && frames % 300 == 0)) {
            struct mallinfo memory = mallinfo();
            C2VitaLog("VitaGL GPU swap frame=%u time=%llu ms interval=%llu ms heap_used=%u\n",
                frames, (unsigned long long)now, (unsigned long long)(last_swap ? now-last_swap : 0),
                (unsigned)memory.uordblks);
        }
        C2VitaGLFrameStats(frames, gGLFrameHasWorld, last_swap ? now-last_swap : 0, now-swap_start);
        last_swap = now;
    }
}

void C2VitaVideoShutdown(void) {
    C2VitaPerfShutdown();
    if (!initialized) return;
    glDeleteTextures(1,&font_texture);
    /* VitaGL has no public context teardown API. GXM pools live until process
     * exit; release our texture while that context is still alive. */
    initialized = loading = preparing_menu = 0;
    font_texture = 0;
}
