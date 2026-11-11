/*
 * port: FBO-backed back buffers for glrend.
 *
 * Every offscreen pixelmap made by match() owns an FBO with a colour texture (and the
 * depth pixelmap matched from it attaches its depth texture to that FBO). The FBO is
 * render_scale times the pixelmap size, so the 3D is drawn at the window resolution
 * while the game keeps addressing a 640x480 screen.
 *
 * CPU access (directLock, rectangleCopyTo, fills while locked) goes into a 640x480
 * RGB565 overlay that starts out as the magenta key colour. Before any GPU work on the
 * buffer (scene begin, fill, copy, double buffer) the overlay is drawn into the FBO
 * with the magenta pixels discarded and reset, which keeps the order the game drew in.
 */
#include "drv.h"
#include <brassert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "brgl_display.h"
#if defined(__vita__) || defined(C2_FAST_OVERLAY)
#include "overlay_scan.h"
#if defined(__vita__) || defined(C2_FAST_OVERLAY)
#define C2_OVERLAY_FAST 1
#else
#define C2_OVERLAY_FAST 0
#endif
#if C2_OVERLAY_FAST && !defined(__vita__)
/* desktop test build of the Vita overlay path: statistics hooks are no-ops */
static void C2VitaGLRecordOverlayScope(void) {}
static void C2VitaGLRecordOverlayCheck(int empty, unsigned long long ns) { (void)empty; (void)ns; }
static void C2VitaGLRecordOverlayRows(int rows) { (void)rows; }
static unsigned long long C2VitaGLTicksNS(void) { return 0; }
#endif
#if C2_OVERLAY_FAST
/* root whose overlay the caller has proven all key since its last flush (no 2D writes since) */
static br_device_pixelmap* gl_overlay_clean_root;
static unsigned char gl_overlay_rows[1024]; /* rows with content found by the last flush */
static GLuint gl_overlay_draw_texture;
/* Overlay textures used in rotation: updating a texture drawn within the last FRAME_PURGE_FREQ (4)
 * VitaGL frames makes VitaGL clone the whole 640x480 texture first (~7 ms even for a few rows), so the
 * ring must be longer than that window (one non-empty flush per frame in a race). */
#define GL_OVERLAY_RING 6
static br_device_pixelmap* gl_ring_root;
static GLuint gl_ring[GL_OVERLAY_RING];
static unsigned char gl_ring_rows[GL_OVERLAY_RING][1024]; /* 1 = row in that texture may not be all key */
static int gl_ring_index;
#endif
#if defined(__vita__)
#include "../../../carma2/platform/sdl3/vita_perf.h"
#define VPB(x) do { if (C2VitaPerfDetail) C2VitaPerfBegin(x); else C2VitaPhasePush(x); } while (0)
#define VPE(x) do { if (C2VitaPerfDetail) C2VitaPerfEnd(x); else C2VitaPhasePop(); } while (0)
#else
#define VPB(x) ((void)0)
#define VPE(x) ((void)0)
#endif
#endif

#define OVERLAY_KEY BR_COLOUR_565(31, 0, 31)

static br_device_pixelmap* root_of(br_device_pixelmap* self) {
    if (self->use_type == BRT_DEPTH && self->asDepth.backbuffer != NULL)
        self = self->asDepth.backbuffer;
    return self->root != NULL ? self->root : self;
}

/* Where CPU 2D writes may land (devpixmp.h asBack.dirty*): writes through a sub-pixelmap stay inside
 * its rectangle (BRender clips to it), a copy inside its destination; anything on the root itself
 * may be anywhere. */
static void mark_dirty_rect(br_device_pixelmap* root, int x, int y, int w, int h) {
    root->asBack.possiblyDirty = 1;
    if (w <= 0 || h <= 0) {
        return;
    }
    if (root->asBack.dirtyX0 >= root->asBack.dirtyX1) {
        root->asBack.dirtyX0 = x;
        root->asBack.dirtyY0 = y;
        root->asBack.dirtyX1 = x + w;
        root->asBack.dirtyY1 = y + h;
    } else {
        if (x < root->asBack.dirtyX0) root->asBack.dirtyX0 = x;
        if (y < root->asBack.dirtyY0) root->asBack.dirtyY0 = y;
        if (x + w > root->asBack.dirtyX1) root->asBack.dirtyX1 = x + w;
        if (y + h > root->asBack.dirtyY1) root->asBack.dirtyY1 = y + h;
    }
}

/* Rectangle the game declared for its next writes to the whole back buffer (C2GLOverlayHint). */
static int gl_hint_x, gl_hint_y, gl_hint_w, gl_hint_h;

void C2GLOverlayHint(int x, int y, int w, int h) {
    gl_hint_x = x;
    gl_hint_y = y;
    gl_hint_w = w;
    gl_hint_h = h;
}

void GLBackMarkDirty(br_device_pixelmap* self) {
    br_device_pixelmap* root = root_of(self);
#ifdef __vita__
    if (getenv("C2_TEST_LOCKLOG") != NULL) {
        static int logged;
        extern unsigned C2VitaPerfActiveMask(void);
        extern void C2VitaLog(const char* format, ...);
        if (logged++ < 300) {
            C2VitaLog("TEST MARK sub=%d hint=%d rect=%d,%d %dx%d scopes=0x%x ret=%p\n", self != root, gl_hint_w, self->pm_base_x, self->pm_base_y,
                self->pm_width, self->pm_height, C2VitaPerfActiveMask(), __builtin_return_address(0));
        }
    }
#endif
    if (self == root && gl_hint_w > 0 && gl_hint_h > 0) {
        mark_dirty_rect(root, gl_hint_x, gl_hint_y, gl_hint_w, gl_hint_h);
    } else if (self == root) {
        root->asBack.possiblyDirty = 1;
        root->asBack.dirtyAll = 1;
    } else {
        mark_dirty_rect(root, self->pm_base_x, self->pm_base_y, self->pm_width, self->pm_height);
    }
}

static void clear_dirty(br_device_pixelmap* root) {
    root->asBack.possiblyDirty = 0;
    root->asBack.dirtyAll = 0;
    root->asBack.dirtyX0 = root->asBack.dirtyX1 = 0;
}

#if C2_OVERLAY_FAST
/* Flush pm's overlay now and declare it clean until C2VitaOverlayEndClean: the caller guarantees that
 * no CPU 2D writes to that target happen in between (e.g. the 3D part of a render pass). */
void C2VitaOverlayBeginClean(void* pm);
#endif
int GLBackScale(const br_device_pixelmap* self) {
    int s = self->screen != NULL ? self->screen->asFront.render_scale : 1;
    return s > 0 ? s : 1;
}

/* Map a logical sub-area into its FBO; UI occupies a centred proportional area.
 * World passes use the entire target and a correspondingly wider camera. */
void GLBackViewport(br_device_pixelmap* self, int* x, int* y, int* w, int* h) {
    br_device_pixelmap* root = root_of(self);
    int s = GLBackScale(root);
    *x = self->pm_base_x * s;
    /* Keep the renderer's existing base_y==0 convention for cropped views. */
    *y = self->pm_base_y == 0 ? 0 :
        ((self->sub_pixelmap ? self->parent_height : root->pm_height) - self->pm_base_y - self->pm_height) * s;
    *w = self->pm_width * s; *h = self->pm_height * s;
    if (root->screen != NULL && root->pm_width == root->screen->pm_width && root->pm_height == root->screen->pm_height)
        C2GLScaleUIRect(root->pm_width * s, root->pm_height * s, x,y,w,h);
}

void GLBackDrawTexture(br_device_pixelmap* screen, GLuint tex, int vx, int vy, int vw, int vh, int flip, int discard_purple) {
    HVIDEO hVideo = &screen->asFront.video;

    glViewport(vx, vy, vw, vh);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(screen->asFront.screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen->asFront.screen_buffer_ebo);
    glUseProgram(hVideo->defaultProgram.program);
    glUniform1f(hVideo->defaultProgram.uFlipVertically, (float)flip);
    glUniform1i(hVideo->defaultProgram.uDiscardPurplePixels, discard_purple);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    GL_CHECK_ERROR();
}

/* The overlay draw (no flip, key discarded), limited to a scissor rectangle in FBO pixels. */
void GLBackDrawTextureScissored(br_device_pixelmap* screen, GLuint tex, int vx, int vy, int vw, int vh, int sx, int sy, int sw, int sh) {
    HVIDEO hVideo = &screen->asFront.video;

    glViewport(vx, vy, vw, vh);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glScissor(sx, sy, sw, sh);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(screen->asFront.screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen->asFront.screen_buffer_ebo);
    glUseProgram(hVideo->defaultProgram.program);
    glUniform1f(hVideo->defaultProgram.uFlipVertically, 0.f);
    glUniform1i(hVideo->defaultProgram.uDiscardPurplePixels, 1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glDisable(GL_SCISSOR_TEST);
    GL_CHECK_ERROR();
}

#ifdef __vita__
/* One bounded read-only calibration on the first newly filled buffer. It lets
 * the hardware log distinguish old scalar scan cost from other scene work. */
static void benchmark_overlay_check(const br_uint_16* pixels, int count) {
    static int done;
    unsigned long long scalar_ns = 0, vector_ns = 0, start;
    volatile int valid = 1;
    if (done || count <= 0) return;
    done = 1;
    for (int pass = 0; pass < 4; ++pass) {
        start = C2VitaGLTicksNS();
        valid &= GLOverlayAllKeyScalar(pixels, (size_t)count, OVERLAY_KEY);
        scalar_ns += C2VitaGLTicksNS() - start;
        start = C2VitaGLTicksNS();
        valid &= GLOverlayAllKey(pixels, (size_t)count, OVERLAY_KEY);
        vector_ns += C2VitaGLTicksNS() - start;
    }
    C2VitaGLLogOverlayBenchmark((unsigned)count, scalar_ns, vector_ns, valid);
}
#endif

void GLBackEnsureOverlay(br_device_pixelmap* root) {
    br_uint_16* px;
    int n;
#ifndef __vita__
    int i;
#endif

    if (root->asBack.lockedPixels != NULL)
        return;

    n = root->pm_width * root->pm_height;
    root->asBack.lockedPixels = BrMemAllocate(n * sizeof(br_uint_16), BR_MEMORY_PIXELS);
    px = root->asBack.lockedPixels;
#ifdef __vita__
    GLOverlayFillKey(px, (size_t)n, OVERLAY_KEY);
#else
    for (i = 0; i < n; i++)
        px[i] = OVERLAY_KEY;
#endif

#ifdef __vita__
    benchmark_overlay_check(px, n);
#endif
    glGenTextures(1, &root->asBack.overlayTexture);
    glBindTexture(GL_TEXTURE_2D, root->asBack.overlayTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, root->pm_width, root->pm_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    GL_CHECK_ERROR();
}

void GLBackBindTarget(br_device_pixelmap* self) {
    glBindFramebuffer(GL_FRAMEBUFFER, root_of(self)->asBack.glFbo);
}

#if C2_OVERLAY_FAST
/* Selects the next ring texture as gl_overlay_draw_texture; returns its row map. */
static unsigned char* overlay_ring_next(br_device_pixelmap* root) {
    int w = root->pm_width, h = root->pm_height;
    if (h > 1024 || gl_ring_root != root) {
        int r;
        for (r = 0; r < GL_OVERLAY_RING; ++r) {
            if (gl_ring[r] == 0) {
                glGenTextures(1, &gl_ring[r]);
                glBindTexture(GL_TEXTURE_2D, gl_ring[r]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
            }
            memset(gl_ring_rows[r], 1, sizeof(gl_ring_rows[r])); /* unknown texture contents: refresh all */
        }
        gl_ring_root = root;
    }
    gl_ring_index = (gl_ring_index + 1) % GL_OVERLAY_RING;
    gl_overlay_draw_texture = gl_ring[gl_ring_index];
    return gl_ring_rows[gl_ring_index];
}
#endif

#ifdef __vita__
void GLBackDrawTextureScissored(br_device_pixelmap* screen, GLuint tex, int vx, int vy, int vw, int vh, int sx, int sy, int sw, int sh);

/* Flush of an overlay whose 2D writes since the last flush are known to lie in its dirty rectangle
 * (sub-pixelmap unlocks, copies): only that rectangle (plus a 2 pixel margin) is scanned, uploaded
 * into a ring texture at the same place and drawn, scissored to it, with the mapping of the
 * full-screen overlay draw. The rest of the buffer is all key (every write outside is tracked); the
 * rest of the texture is not drawn. Returns 0 when it cannot (the full flush runs).
 * C2_TEST_OVLCHECK also scans the whole buffer and logs content outside the rectangle. */
static int flush_overlay_rect(br_device_pixelmap* root) {
    static br_uint_16* staging;
    static size_t staging_pixels;
    br_uint_16* px = root->asBack.lockedPixels;
    int W = root->pm_width, H = root->pm_height;
    int x0 = root->asBack.dirtyX0 - 2, y0 = root->asBack.dirtyY0 - 2;
    int x1 = root->asBack.dirtyX1 + 2, y1 = root->asBack.dirtyY1 + 2;
    int y, top = -1, bottom = -1, w, uy0, uy1, uh;
    unsigned long long start;
    unsigned char* texture_rows;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > W) x1 = W;
    if (y1 > H) y1 = H;
    if (H > 1024 || x0 >= x1 || y0 >= y1) {
        return 0;
    }
    w = x1 - x0;
    start = C2VitaGLTicksNS();
    for (y = y0; y < y1; ++y) {
        if (!GLOverlayAllKey(px + (size_t)y * W + x0, (size_t)w, OVERLAY_KEY)) {
            if (top < 0) top = y;
            bottom = y + 1;
        }
    }
    C2VitaGLRecordOverlayCheck(top < 0, C2VitaGLTicksNS() - start);
    {
        static unsigned rect_flushes;
        extern void C2VitaLog(const char* format, ...);
        if (++rect_flushes % 600 == 1) {
            C2VitaLog("VITA OVERLAY rect flush %u: %d,%d-%d,%d content rows %d-%d scan %llu us\n", rect_flushes, x0, y0, x1, y1,
                top, bottom, (C2VitaGLTicksNS() - start) / 1000);
        }
    }
    {
        static int check = -1;
        if (check < 0) check = getenv("C2_TEST_OVLCHECK") != NULL;
        if (check) {
            static unsigned checks, misses;
            extern unsigned C2VitaPerfActiveMask(void);
            extern void C2VitaLog(const char* format, ...);
            int miss = 0;
            for (y = 0; y < H && !miss; ++y) {
                const br_uint_16* row = px + (size_t)y * W;
                if (y < y0 || y >= y1) {
                    miss = !GLOverlayAllKey(row, (size_t)W, OVERLAY_KEY);
                } else {
                    miss = (x0 > 0 && !GLOverlayAllKey(row, (size_t)x0, OVERLAY_KEY))
                        || (x1 < W && !GLOverlayAllKey(row + x1, (size_t)(W - x1), OVERLAY_KEY));
                }
            }
            ++checks;
            if (miss && misses++ < 20) {
                C2VitaLog("TEST OVLCHECK content outside %d,%d-%d,%d (row %d) scopes=0x%x\n", x0, y0, x1, y1, y - 1, C2VitaPerfActiveMask());
            }
            if (checks % 3000 == 0) {
                C2VitaLog("TEST OVLCHECK rect flushes=%u with content outside=%u last rect %d,%d-%d,%d\n", checks, misses, x0, y0, x1, y1);
            }
        }
    }
    if (top < 0) {
        clear_dirty(root);
        return 1;
    }
    /* upload the content rows and one key row either side (inside the scanned rectangle) */
    uy0 = top > y0 ? top - 1 : top;
    uy1 = bottom < y1 ? bottom + 1 : bottom;
    uh = uy1 - uy0;
    if (staging_pixels < (size_t)w * uh) {
        br_uint_16* grown = realloc(staging, (size_t)w * uh * sizeof(br_uint_16));
        if (grown == NULL) {
            return 0;
        }
        staging = grown;
        staging_pixels = (size_t)w * uh;
    }
    for (y = uy0; y < uy1; ++y) {
        memcpy(staging + (size_t)(y - uy0) * w, px + (size_t)y * W + x0, (size_t)w * sizeof(br_uint_16));
    }
    texture_rows = overlay_ring_next(root);
    for (y = uy0; y < uy1; ++y) {
        texture_rows[y] = 1; /* a full flush using this texture resets these rows */
    }
    glBindTexture(GL_TEXTURE_2D, gl_overlay_draw_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x0, uy0, w, uh, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, staging);
    glBindTexture(GL_TEXTURE_2D, 0);
    C2VitaGLRecordOverlayRows(uh);

    glBindFramebuffer(GL_FRAMEBUFFER, root->asBack.glFbo);
    {
        /* The full overlay quad covers the viewport; with nearest sampling, fragment column fx shows
         * texel column floor((fx - vx + .5) * W / vw) and fragment row fy (bottom-up) texel row
         * floor((vy + vh - fy - .5) * H / vh). Scissor to the fragments showing uploaded texels. */
        int vx, vy, vw, vh, fx0, fx1, fy0, fy1;
        GLBackViewport(root, &vx, &vy, &vw, &vh);
        fx0 = (int)ceil(vx + (double)x0 * vw / W - .5);
        fx1 = (int)ceil(vx + (double)x1 * vw / W - .5);
        fy0 = (int)floor(vy + vh - .5 - (double)uy1 * vh / H) + 1;
        fy1 = (int)floor(vy + vh - .5 - (double)uy0 * vh / H) + 1;
        if (fx1 > fx0 && fy1 > fy0) {
            GLBackDrawTextureScissored(root->screen, gl_overlay_draw_texture, vx, vy, vw, vh, fx0, fy0, fx1 - fx0, fy1 - fy0);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    for (y = uy0; y < uy1; ++y) {
        GLOverlayFillKey(px + (size_t)y * W + x0, (size_t)w, OVERLAY_KEY);
    }
    clear_dirty(root);
    return 1;
}
#endif

#if defined(__vita__)
static void GLBackFlushOverlay_timed(br_device_pixelmap* self);
void GLBackFlushOverlay(br_device_pixelmap* self) {
    VPB(C2VP_G_OVERLAY);
    GLBackFlushOverlay_timed(self);
    VPE(C2VP_G_OVERLAY);
}
static void GLBackFlushOverlay_timed(br_device_pixelmap* self) {
#else
void GLBackFlushOverlay(br_device_pixelmap* self) {
#endif
    br_device_pixelmap* root = root_of(self);
    int n;
#ifndef __vita__
    int i;
#endif
    br_uint_16* px;

    /* the overlay is always mapped, so any CPU write may be pending: always flush */
    if (root->use_type != BRT_OFFSCREEN || root->asBack.lockedPixels == NULL)
        return;
#if C2_OVERLAY_FAST
    C2VitaGLRecordOverlayScope();
    if (root == gl_overlay_clean_root)
        return; /* caller proved the buffer is still all key since the previous flush */
#endif
#ifdef __vita__
    {
        /* Inside a race frame, a buffer no tracked 2D write (lock, copy) touched since its last flush is
         * not rescanned: 12000 flushes of a TIMBER3 race found content without the flag once, outside a
         * frame. The frame's present flush always scans, so a write through a kept pixel pointer still
         * reaches the screen in that frame. */
        extern unsigned C2VitaPerfActiveMask(void);
        extern int C2VitaABFeature(const char* pFeature, int pDefault);
        unsigned scopes = C2VitaPerfActiveMask();
        if (!root->asBack.possiblyDirty && scopes != 0 && !(scopes & (1u << C2VP_PRESENT))
                && C2VitaABFeature("flagskip", 1)) {
            return;
        }
        /* The present flush is the safety net for writes through a kept pixel pointer, which a race
         * practically never makes: unflagged, it rescans every 8th frame only. */
        if (!root->asBack.possiblyDirty && (scopes & (1u << C2VP_PRESENT)) && C2VitaABFeature("presentskip", 1)) {
            static unsigned present_flushes;
            if ((++present_flushes & 7) != 0) {
                return;
            }
        }
        /* Writes known to lie in a rectangle: flush just that, in race frames (scopes != 0; verified
         * there with C2_TEST_OVLCHECK; menus write text through the mapped pointer). Every 8th present
         * flush still scans the whole buffer (the safety net for writes through a kept pixel pointer). */
        if (root->asBack.possiblyDirty && !root->asBack.dirtyAll && root->asBack.dirtyX0 < root->asBack.dirtyX1
                && scopes != 0 && C2VitaABFeature("ovlrect", 1)) {
            static unsigned present_rect_flushes;
            if (!(scopes & (1u << C2VP_PRESENT)) || (++present_rect_flushes & 7) != 0) {
                if (flush_overlay_rect(root)) {
                    return;
                }
            }
        }
    }
#endif

#if C2_OVERLAY_FAST
    /* CPU pixels stay mapped and can be written without a lock, so a dirty
     * flag cannot prove this is empty. Do not upload/draw an all-key overlay:
     * VitaGL must otherwise clone a recently drawn640x480texture per flush. */
    /* One pass per row finds both whether anything was drawn and which rows to upload (a race
     * frame always has content, so a whole-buffer check followed by the row scan read it twice). */
    px = root->asBack.lockedPixels;
    unsigned long long check_start = C2VitaGLTicksNS();
    int empty = 1;
    {
        int w = root->pm_width, h = root->pm_height, y;
        for (y = 0; y < h && y < 1024; ++y) {
            gl_overlay_rows[y] = !GLOverlayAllKey(px + (size_t)y * w, (size_t)w, OVERLAY_KEY);
            empty &= !gl_overlay_rows[y];
        }
        if (h > 1024 && !GLOverlayAllKey(px + (size_t)1024 * w, (size_t)(h - 1024) * w, OVERLAY_KEY))
            empty = 0;
    }
    C2VitaGLRecordOverlayCheck(empty, C2VitaGLTicksNS() - check_start);
#ifdef __vita__
    {
        /* diagnostics: content found although no tracked 2D write (lock, copy) flagged the buffer =
         * the game wrote through a kept pixel pointer; decides whether unflagged scans can be skipped */
        static unsigned flagged, unflagged, unflagged_content, flagged_empty;
        extern unsigned C2VitaPerfActiveMask(void);
        extern void C2VitaLog(const char* format, ...);
        if (root->asBack.possiblyDirty) { ++flagged; flagged_empty += empty; }
        else { ++unflagged; if (!empty && unflagged_content++ < 12) C2VitaLog("VITA OVERLAY unflagged content scopes=0x%x\n", C2VitaPerfActiveMask()); }
        if ((flagged + unflagged) % 6000 == 0)
            C2VitaLog("VITA OVERLAY flushes flagged=%u (empty %u) unflagged=%u (with content %u)\n", flagged, flagged_empty, unflagged, unflagged_content);
    }
#endif
#ifdef __vita__
    {
        /* test: C2_TEST_OVLDUMP=<s> saves the first 24 non-empty overlays after <s> s of play as
         * OVL_<k>_<scopes>.565 (raw RGB565, pm_width x pm_height), to see what the game draws in 2D */
        static int dumped = -1;
        static unsigned long long at_ns;
        if (dumped < 0) {
            dumped = getenv("C2_TEST_OVLDUMP") != NULL ? 0 : 24;
            at_ns = dumped == 0 ? C2VitaGLTicksNS() + (unsigned long long)atoi(getenv("C2_TEST_OVLDUMP")) * 1000000000ull : 0;
        }
        if (!empty && dumped < 24 && C2VitaGLTicksNS() >= at_ns) {
            extern unsigned C2VitaPerfActiveMask(void);
            extern void C2VitaLog(const char* format, ...);
            char name[96];
            FILE* f;
            snprintf(name, sizeof(name), "ux0:/data/c2port/OVL_%d_%x.565", dumped, C2VitaPerfActiveMask());
            f = fopen(name, "wb");
            if (f != NULL) {
                fwrite(root->asBack.lockedPixels, 2, (size_t)root->pm_width * root->pm_height, f);
                fclose(f);
            }
            C2VitaLog("TEST OVERLAY DUMP %s flagged=%d %dx%d\n", name, root->asBack.possiblyDirty, root->pm_width, root->pm_height);
            dumped++;
        }
    }
#endif
    if (empty) {
        clear_dirty(root);
        return;
    }
#endif

    /* These uploads are packed RGBA8/RGB565; VitaGL uses alignment1. */
#ifndef __vita__

    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
#endif
#if C2_OVERLAY_FAST
    {
        /* Upload only rows that hold content now or held content in this texture after its previous
         * upload (those must be reset to the key), so the texture still equals the CPU buffer and the
         * full-screen draw below is unchanged. A full 640x480 upload costs ~12 ms on the Vita.
         * Rotate through textures: updating a texture drawn within the last FRAME_PURGE_FREQ (4) VitaGL
         * frames makes VitaGL clone the whole 640x480 texture first (~7 ms even for a few rows), so the
         * ring must be longer than that window (one non-empty flush per frame in a race). */
        unsigned char* texture_rows;
        unsigned char* rows = gl_overlay_rows;
        int w = root->pm_width, h = root->pm_height, y, uploaded = 0;
        texture_rows = overlay_ring_next(root);
        glBindTexture(GL_TEXTURE_2D, gl_overlay_draw_texture);
        /* rows[] was filled by the scan above */
        for (y = 0; y < h;) {
            int y0, y1;
            if (y < 1024 && !rows[y] && !texture_rows[y]) { ++y; continue; }
            y0 = y;
            while (y < h && (y >= 1024 || rows[y] || texture_rows[y])) ++y;
            y1 = y;
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y0, w, y1 - y0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, px + (size_t)y0 * w);
            uploaded += y1 - y0;
        }
        for (y = 0; y < h && y < 1024; ++y) texture_rows[y] = rows[y];
        C2VitaGLRecordOverlayRows(uploaded);
    }
#else
    glBindTexture(GL_TEXTURE_2D, root->asBack.overlayTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, root->pm_width, root->pm_height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, root->asBack.lockedPixels);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, root->asBack.glFbo);
    {
        int x,y,w,h;
        GLBackViewport(root,&x,&y,&w,&h);
#if C2_OVERLAY_FAST
        GLBackDrawTexture(root->screen, gl_overlay_draw_texture, x,y,w,h,0,1);
#else
        GLBackDrawTexture(root->screen, root->asBack.overlayTexture, x,y,w,h,0,1);
#endif
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    n = root->pm_width * root->pm_height;
    px = root->asBack.lockedPixels;
#if C2_OVERLAY_FAST
    (void)n;
    {   /* only rows that held content need resetting; the rest is already key */
        int w = root->pm_width, h = root->pm_height, y;
        for (y = 0; y < h; ++y)
            if (y >= 1024 || gl_overlay_rows[y])
                GLOverlayFillKey(px + (size_t)y * w, (size_t)w, OVERLAY_KEY);
    }
#else
    for (i = 0; i < n; i++)
        px[i] = OVERLAY_KEY;
#endif
    clear_dirty(root);
}

/* Pixelmap rectangle (top-left origin, pixelmap units) -> FBO pixels (bottom-left origin). */
static void rect_to_fbo(br_device_pixelmap* self, br_device_pixelmap* root, const br_rectangle* r, GLint out[4]) {
    int s = GLBackScale(root);
    int x = self->pm_base_x + r->x;
    int y = self->pm_base_y + r->y;

    out[0] = x * s;
    out[1] = (root->pm_height - (y + r->h)) * s;
    out[2] = r->w * s;
    out[3] = r->h * s;
    /* Whole-target clears/copies include the unused menu borders as well. */
    if (root->screen != NULL && root->pm_width == root->screen->pm_width && root->pm_height == root->screen->pm_height
            && !(x == 0 && y == 0 && r->w == root->pm_width && r->h == root->pm_height))
        C2GLScaleUIRect(root->pm_width * s, root->pm_height * s,&out[0],&out[1],&out[2],&out[3]);
}

br_error GLBackRectangleFill(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour) {
    br_device_pixelmap* root = root_of(self);
    br_rectangle r;
    GLint fr[4];

    if (PixelmapRectangleClip(&r, rect, (br_pixelmap*)self) == BR_CLIP_REJECT)
        return BRE_OK;

    /* keep the order: whatever the CPU drew before this fill goes in first */
    GLBackFlushOverlay(self);

    rect_to_fbo(self, root, &r, fr);
    glBindFramebuffer(GL_FRAMEBUFFER, root->asBack.glFbo);
    glEnable(GL_SCISSOR_TEST);
    glScissor(fr[0], fr[1], fr[2], fr[3]);
    if (self->use_type == BRT_DEPTH) {
        glDepthMask(GL_TRUE);
        glClearDepth(1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
    } else {
        float cr, cg, cb;
        if (self->pm_type == BR_PMT_RGB_565) {
            cr = ((colour >> 11) & 0x1f) / 31.f;
            cg = ((colour >> 5) & 0x3f) / 63.f;
            cb = (colour & 0x1f) / 31.f;
        } else {
            cr = BR_RED(colour) / 255.f;
            cg = BR_GRN(colour) / 255.f;
            cb = BR_BLU(colour) / 255.f;
        }
        glClearColor(cr, cg, cb, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERROR();
    return BRE_OK;
}

/* port: the frontend backdrop on a wide display, drawn straight into the FBO at a uniform zoom
 * that fills the width (cropping a little at the top and bottom) rather than into the overlay. */
static void draw_wide_backdrop(br_device_pixelmap* root, br_device_pixelmap* src) {
    static GLuint tex;
    static const void* cached_pixels;
    static br_uint_32 cached_sum;
    const br_uint_16* px = src->pm_pixels;
    br_uint_32 sum = 0;
    int s = GLBackScale(root), i, fbo_w, fbo_h, h;

    for (i = 0; i < 4096; i++) {
        int y = (i * 37) % src->pm_height, x = (i * 101) % src->pm_width;
        sum = sum * 31 + ((const br_uint_16*)((const br_uint_8*)px + y * src->pm_row_bytes))[x];
    }
    /* keep the order: CPU pixels drawn before the backdrop go first */
    GLBackFlushOverlay(root);
    if (tex == 0) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, root->pm_width, root->pm_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
        cached_pixels = NULL;
    }
    glBindTexture(GL_TEXTURE_2D, tex);
    if (cached_pixels != src->pm_pixels || cached_sum != sum) {
        if (src->pm_row_bytes == root->pm_width * 2) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, root->pm_width, root->pm_height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, src->pm_pixels);
        } else {
            for (i = 0; i < root->pm_height; i++) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, root->pm_width, 1, GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
                    (const br_uint_8*)src->pm_pixels + i * src->pm_row_bytes);
            }
        }
        cached_pixels = src->pm_pixels;
        cached_sum = sum;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    fbo_w = root->pm_width * s;
    fbo_h = root->pm_height * s;
    h = (int)(fbo_h * gGLPresentationAspect * 3.f / 4.f + .5f);
    glBindFramebuffer(GL_FRAMEBUFFER, root->asBack.glFbo);
    GLBackDrawTexture(root->screen, tex, 0, (fbo_h - h) / 2, fbo_w, h, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gGLFrameHasWide = 1;
}

/* Memory pixelmap -> device: written into the CPU overlay. */
br_error GLBackRectangleCopyTo(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* sr) {
    br_device_pixelmap* root = root_of(self);
    br_rectangle r;
    br_point dp;
    br_uint_16* dst;
    br_uint_32* map = NULL;
    int x, y;

    if (self->use_type != BRT_OFFSCREEN)
        return BRE_UNSUPPORTED;
    if (PixelmapRectangleClipTwo(&r, &dp, sr, p, (br_pixelmap*)self, (br_pixelmap*)src) == BR_CLIP_REJECT)
        return BRE_OK;
    if (src->pm_pixels == NULL)
        return BRE_FAIL;

    GLBackEnsureOverlay(root);
    if (gGLWideBackdrop && !gGLWorldRendering && gGLPresentationAspect > 4.f / 3.f + .01f && self == root
            && root->screen != NULL && src->pm_type == BR_PMT_RGB_565 && dp.x == 0 && dp.y == 0
            && r.x == 0 && r.y == 0 && r.w == root->pm_width && r.h == root->pm_height) {
        draw_wide_backdrop(root, src);
        return BRE_OK;
    }
    if (src->pm_type == BR_PMT_INDEX_8) {
        if (src->pm_map != NULL)
            map = src->pm_map->pixels;
        else if (root->clut != NULL)
            map = root->clut->entries;
        else
            return BRE_FAIL;
    } else if (src->pm_type != BR_PMT_RGB_565) {
        return BRE_UNSUPPORTED;
    }
#ifdef __vita__
    if (getenv("C2_TEST_LOCKLOG") != NULL) {
        static int logged;
        extern unsigned C2VitaPerfActiveMask(void);
        extern void C2VitaLog(const char* format, ...);
        if (logged++ < 400) {
            C2VitaLog("TEST COPYTO sub=%d dst=%d,%d size=%dx%d src_type=%d src=%dx%d scopes=0x%x ret=%p\n", self != root,
                self->pm_base_x + dp.x, self->pm_base_y + dp.y, r.w, r.h, src->pm_type, src->pm_width, src->pm_height,
                C2VitaPerfActiveMask(), __builtin_return_address(0));
        }
    }
#endif

    for (y = 0; y < r.h; y++) {
        int sy = src->pm_base_y + r.y + y;
        int dy = self->pm_base_y + dp.y + y;
        dst = (br_uint_16*)root->asBack.lockedPixels + dy * root->pm_width + self->pm_base_x + dp.x;
        if (src->pm_type == BR_PMT_RGB_565) {
            const br_uint_16* s16 = (const br_uint_16*)((const br_uint_8*)src->pm_pixels + sy * src->pm_row_bytes) + src->pm_base_x + r.x;
            memcpy(dst, s16, r.w * sizeof(br_uint_16));
        } else {
            const br_uint_8* s8 = (const br_uint_8*)src->pm_pixels + sy * src->pm_row_bytes + src->pm_base_x + r.x;
            for (x = 0; x < r.w; x++) {
                br_uint_32 c = map[s8[x]];
                dst[x] = (br_uint_16)(((BR_RED(c) >> 3) << 11) | ((BR_GRN(c) >> 2) << 5) | (BR_BLU(c) >> 3));
            }
        }
    }
    mark_dirty_rect(root, self->pm_base_x + dp.x, self->pm_base_y + dp.y, r.w, r.h);
    return BRE_OK;
}

/* Device -> device copy between back buffers. */
br_error GLBackRectangleCopy(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* sr) {
    br_device_pixelmap* droot = root_of(self);
    br_device_pixelmap* sroot = root_of(src);
    br_rectangle r;
    br_point dp;
    br_rectangle dr;
    GLint s4[4], d4[4];

    if (ObjectDevice(src) != self->device || src->use_type != BRT_OFFSCREEN || self->use_type != BRT_OFFSCREEN)
        return BRE_UNSUPPORTED;
    if (PixelmapRectangleClipTwo(&r, &dp, sr, p, (br_pixelmap*)self, (br_pixelmap*)src) == BR_CLIP_REJECT)
        return BRE_OK;

    GLBackFlushOverlay(src);
    GLBackFlushOverlay(self);

    rect_to_fbo(src, sroot, &r, s4);
    dr.x = dp.x;
    dr.y = dp.y;
    dr.w = r.w;
    dr.h = r.h;
    rect_to_fbo(self, droot, &dr, d4);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sroot->asBack.glFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, droot->asBack.glFbo);
    glBlitFramebuffer(s4[0], s4[1], s4[0] + s4[2], s4[1] + s4[3], d4[0], d4[1], d4[0] + d4[2], d4[1] + d4[3],
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERROR();
    return BRE_OK;
}

#if C2_OVERLAY_FAST
void C2VitaOverlayBeginClean(void* pm) {
    br_device_pixelmap* self = (br_device_pixelmap*)pm;
    br_device_pixelmap* root;
    if (self == NULL || self->dispatch == NULL) return;
    root = root_of(self);
    if (root->use_type != BRT_OFFSCREEN || root->asBack.lockedPixels == NULL) return;
    gl_overlay_clean_root = NULL;
    GLBackFlushOverlay(self); /* leaves the buffer all key */
    gl_overlay_clean_root = root;
}
void C2VitaOverlayEndClean(void) {
    gl_overlay_clean_root = NULL;
}
#endif
