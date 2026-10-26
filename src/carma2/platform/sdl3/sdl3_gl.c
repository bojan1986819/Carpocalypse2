/*
 * OpenGL screen for the SDL3 port: creates the GL context on the SDL window and
 * opens the glrend BRender driver (src/brender/drivers/glrend) on it.
 *
 * Selected with the environment variable C2_RENDERER=gl; anything else keeps the
 * softrend + SDLREND memory screen.
 *
 * Desktop uses an OpenGL 3.2 core context. The shaders and GL calls of glrend are
 * kept to the subset shared with OpenGL ES 3.0 so the same driver can later run on
 * Android (GLES 3) and PS Vita (vitaGL).
 */
#include "sdl3.h"
#include "sdl3_display.h"
#include <brgl_display.h>
#ifdef __vita__
#include "vita_gl.h"
#include "vita_video.h"
extern int gInitialisation_finished;
#endif

#include <SDL3/SDL.h>

#include <brender/brender.h>
#ifndef BR_EXPORT
#define BR_EXPORT C2_HOOK_CDECL
#endif
#include "brglrend.h"

#include "core/fw/devsetup.h"
#include "core/fw/devlist.h"
#include "drivers/ddi/brender14.h"

extern SDL_Window* g_SDL_Window;

/* read by glrend: wireframe debug view and texture anisotropy (0 = off) */
int g_wireframe_mode = 0;
int gAnisotropy_level = 8;
int gGLRenderScale = 0; /* back buffer scale (0 = from the window size) */

static int g_use_gl = -1;
static SDL_GLContext g_gl_context;

int carpocalypse2_UseGL(void) {
#ifdef __vita__
    return 1;
#endif
    if (g_use_gl < 0) {
        const char* e = SDL_getenv("C2_RENDERER");
        g_use_gl = e != NULL && SDL_strcasecmp(e, "gl") == 0;
    }
    return g_use_gl;
}

/* Call before SDL_CreateWindow(); returns the extra window flags. */
SDL_WindowFlags carpocalypse2_GLWindowFlags(void) {
#ifdef __vita__
    /* Native VitaGL owns GXM; SDL must not create an EGL/GL context. */
    return 0;
#endif
    if (!carpocalypse2_UseGL()) {
        return 0;
    }
#if defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_IOS)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    return SDL_WINDOW_OPENGL;
}

static void* C2_HOOK_CDECL gl_get_proc_address(const char* name) {
#ifdef __vita__
    return C2VitaGLGetProcAddress(name);
#else
    return (void*)SDL_GL_GetProcAddress(name);
#endif
}

/* Present the expanded world; UI is proportionally mapped inside this area. */
static void C2_HOOK_CDECL gl_get_viewport(int* x, int* y, float* width_multiplier, float* height_multiplier) {
    int w = 640, h = 480;

    SDL_GetWindowSizeInPixels(g_SDL_Window, &w, &h);
    {
        int vw,vh;
        C2GLFitOutput(w,h,x,y,&vw,&vh);
        gGLPresentationAspect = (float)vw / vh;
        *width_multiplier = (float)vw / 640.f;
        *height_multiplier = (float)vh / 480.f;
    }
}

static void C2_HOOK_CDECL gl_swap_buffers(br_pixelmap* pm) {
    (void)pm;
#ifdef __vita__
    C2VitaVideoSwap(gInitialisation_finished);
#else
    SDL_GL_SwapWindow(g_SDL_Window);
    { /* test harness: C2_TEST_FPS=n caps the frame rate (e.g. 30 to behave like the Vita) */
        static int fps = -1;
        static Uint64 last;
        if (fps < 0) { const char* e = SDL_getenv("C2_TEST_FPS"); fps = e != NULL ? SDL_atoi(e) : 0; }
        if (fps > 0) {
            Uint64 frame = 1000 / (Uint64)fps, now = SDL_GetTicks();
            if (last != 0 && now - last < frame) SDL_Delay((Uint32)(frame - (now - last)));
            last = SDL_GetTicks();
        }
    }
#endif
    gGLFrameHasWorld = 0;
    gGLFrameHasWide = 0;
}

static br_device_gl_callback_procs g_gl_callbacks = {
    gl_get_proc_address,
    gl_get_viewport,
    gl_swap_buffers,
};

br_error carpocalypse2_GLBeginScreen(br_pixelmap** screen) {
    br_error r;

#ifndef __vita__
    g_gl_context = SDL_GL_CreateContext(g_SDL_Window);
    if (g_gl_context == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext failed (%s)", SDL_GetError());
        return 0x1002; /* BRE_FAIL */
    }
    SDL_GL_MakeCurrent(g_SDL_Window, g_gl_context);
    SDL_GL_SetSwapInterval(1);
#endif

    /* render the 3D at the desktop resolution, so a maximised/fullscreen window stays sharp */
#ifdef __vita__
    gGLRenderScale = 1;
#else
    {
        const char* e = SDL_getenv("C2_GL_SCALE");
        const SDL_DisplayMode* dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
        gGLRenderScale = dm != NULL ? SDL_max((dm->h + 479) / 480, (dm->w + 639) / 640) : 2;
        if (e != NULL && SDL_atoi(e) > 0)
            gGLRenderScale = SDL_atoi(e);
    }
#endif

    BrDevAddStatic(NULL, BrDrv1GLBegin, NULL);
    r = BrDevBeginVar(screen, "glrend",
        BRT_WIDTH_I32, 640,
        BRT_HEIGHT_I32, 480,
        BRT_PIXEL_BITS_I32, 16,
        BRT_PIXEL_TYPE_U8, BR_PMT_RGB_565,
        BRT_OPENGL_CALLBACKS_P, &g_gl_callbacks,
        BR_NULL_TOKEN);
#ifdef __vita__
    SDL_Log("glrend: BrDevBeginVar -> %d, native VitaGL GPU backend", r);
#else
    SDL_Log("glrend: BrDevBeginVar -> %d, GL %s / %s", r,
        (const char*)SDL_GL_GetCurrentContext() ? "context ok" : "no context", SDL_GetCurrentVideoDriver());
#endif
    return r;
}

void PDDisplayFlush(br_pixelmap* pm) {
    if (carpocalypse2_UseGL() && pm != NULL) BrPixelmapFlush(pm);
}
