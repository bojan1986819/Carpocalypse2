/* GLAD resolves this adapter while the native VitaGL API remains unwrapped in
 * this translation unit. Track the state queries VitaGL does not implement;
 * transparent draws depend on restoring the exact texture/buffer state. */
#include "vita_gl.h"
#include "vita_pack.h"
#include "vita_perf.h"
#include "vita_glqueue.h"
#include <vitaGL.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <psp2/gxm.h>

#ifndef GL_UNIFORM_BUFFER_BINDING
#define GL_UNIFORM_BUFFER_BINDING 0x8a28
#endif
#ifndef GL_VERTEX_ARRAY_BINDING
#define GL_VERTEX_ARRAY_BINDING 0x85b5
#endif

enum { TEXTURE_SLOTS = 16384 };
/* known: bit per parameter that was actually sent to VitaGL for this id, so a
 * repeated identical value can be skipped (the defaults below are only what
 * glGetTexParameteriv reports before the first set). */
static struct { GLint min, mag, s, t; unsigned char known; unsigned char content; unsigned short w, h; } textures[TEXTURE_SLOTS];
/* content: what level 0 can make the BRender fragment shader discard (shader variants). */
enum { TEXC_KNOWN = 1, TEXC_BLACK = 2, TEXC_LOW_ALPHA = 4, TEXC_CUTOUT = 8 /* alpha only 0 or full */ };
static GLuint uniform_buffer, vertex_array;
static GLint bound_texture_2d(void);

static void tracked_gen_textures(GLsizei count, GLuint* ids) {
    int i;
    glGenTextures(count, ids);
    for (i = 0; i < count; ++i) if (ids[i] < TEXTURE_SLOTS) {
        textures[ids[i]].min = GL_NEAREST_MIPMAP_LINEAR;
        textures[ids[i]].mag = GL_LINEAR;
        textures[ids[i]].s = textures[ids[i]].t = GL_REPEAT;
        textures[ids[i]].known = 0;
        textures[ids[i]].content = 0;
    }
}
/* Texel properties for the variant selection: a pure black texel (colour key) or an alpha
 * below the alpha test (clip(a - 0.004): 8-bit alpha 0 or 1). Filtering and mipmaps only average
 * texels, so a texture without such texels can never produce a discarding sample. */
static unsigned char texture_content(GLsizei w, GLsizei h, GLenum format, GLenum type, const GLvoid* data) {
    size_t n = (size_t)w * (size_t)h, i;
    unsigned char c = TEXC_KNOWN;
    if (data == NULL || w <= 0 || h <= 0) return 0;
    if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
        const unsigned char* p = (const unsigned char*)data;
        for (i = 0; i < n; ++i, p += 4) {
            if ((p[0] | p[1] | p[2]) == 0) c |= TEXC_BLACK;
            if (p[3] <= 1) c |= TEXC_LOW_ALPHA;
        }
        return c;
    }
    if (format == GL_RGBA && type == GL_UNSIGNED_SHORT_4_4_4_4) {
        const unsigned short* p = (const unsigned short*)data;
        int partial = 0;
        for (i = 0; i < n; ++i) {
            unsigned a = p[i] & 0xf;
            if ((p[i] >> 4) == 0) c |= TEXC_BLACK;
            if (a == 0) c |= TEXC_LOW_ALPHA;
            else if (a != 0xf) partial = 1;
        }
        if ((c & TEXC_LOW_ALPHA) && !partial) c |= TEXC_CUTOUT;
        return c;
    }
    if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) {
        const unsigned short* p = (const unsigned short*)data;
        for (i = 0; i < n; ++i) if (p[i] == 0) { c |= TEXC_BLACK; break; }
        return c;
    }
    return 0;
}
static void tracked_tex_parameter(GLenum target, GLenum name, GLint value) {
    GLint id = target == GL_TEXTURE_2D ? bound_texture_2d() : -1;
    if (id >= 0 && id < TEXTURE_SLOTS) {
        GLint* slot = NULL;
        unsigned char bit = 0;
        switch (name) {
        case GL_TEXTURE_MIN_FILTER: slot = &textures[id].min; bit = 1; break;
        case GL_TEXTURE_MAG_FILTER: slot = &textures[id].mag; bit = 2; break;
        case GL_TEXTURE_WRAP_S: slot = &textures[id].s; bit = 4; break;
        case GL_TEXTURE_WRAP_T: slot = &textures[id].t; bit = 8; break;
        }
        if (slot != NULL) {
            if ((textures[id].known & bit) && *slot == value) return;
            *slot = value;
            textures[id].known |= bit;
        }
    }
    glTexParameteri(target, name, value);
}
static void texture_parameter(GLenum target, GLenum name, GLint* value) {
    GLint id = target == GL_TEXTURE_2D ? bound_texture_2d() : -1;
    *value = 0;
    if (id < 0 || id >= TEXTURE_SLOTS) return;
    switch (name) {
    case GL_TEXTURE_MIN_FILTER: *value = textures[id].min; break;
    case GL_TEXTURE_MAG_FILTER: *value = textures[id].mag; break;
    case GL_TEXTURE_WRAP_S: *value = textures[id].s; break;
    case GL_TEXTURE_WRAP_T: *value = textures[id].t; break;
    }
}
static void tracked_bind_buffer(GLenum target, GLuint id) {
    if (target == GL_UNIFORM_BUFFER) uniform_buffer = id;
    glBindBuffer(target, id);
}
static void tracked_bind_base(GLenum target, GLuint index, GLuint id) {
    glBindBufferBase(target, index, id);
    /* Native VitaGL updates only the indexed binding. OpenGL also updates the
     * generic binding used by the following BufferData/BufferSubData calls. */
    if (target == GL_UNIFORM_BUFFER) tracked_bind_buffer(target, id);
}
static void tracked_bind_vao(GLuint id) {
    vertex_array = id;
    glBindVertexArray(id);
}
static void integer_state(GLenum name, GLint* value) {
    if (name == GL_UNIFORM_BUFFER_BINDING) *value = uniform_buffer;
    else if (name == GL_VERTEX_ARRAY_BINDING) *value = vertex_array;
    else glGetIntegerv(name, value);
}
static GLuint native_shader(GLenum type) {
    return glCreateShader(type == GL_VERTEX_SHADER ? GL_CG_VERTEX_SHADER_EXT : GL_CG_FRAGMENT_SHADER_EXT);
}

static unsigned int frame_overlay_checks, frame_overlay_empty, frame_overlay_rows;
static Uint64 frame_overlay_ns;
static unsigned int frame_draws, frame_vaos, frame_buffer_bytes, frame_upload_bytes, frame_gc_retries;
static Uint64 frame_buffer_ms, frame_upload_ms, frame_draw_ms, frame_vao_ms, frame_clear_ms, frame_blit_ms, frame_image_ms, frame_mipmap_ms;
unsigned long long C2VitaGLTicksNS(void) {return (unsigned long long)SDL_GetTicksNS();}
static unsigned int frame_overlay_scope[17];
void C2VitaGLRecordOverlayScope(void) {
    extern unsigned C2VitaPerfActiveMask(void);
    unsigned m = C2VitaPerfActiveMask(), bit;
    for (bit = 16; bit < 17 && bit != (unsigned)-1; --bit) if (m & (1u << bit)) { ++frame_overlay_scope[bit]; return; }
    ++frame_overlay_scope[0];
}
void C2VitaGLRecordOverlayRows(int rows) { frame_overlay_rows += (unsigned)rows; }
void C2VitaGLRecordOverlayCheck(int empty, unsigned long long elapsed_ns) {
    ++frame_overlay_checks;
    if (empty) ++frame_overlay_empty;
    frame_overlay_ns += elapsed_ns;
}
void C2VitaGLLogOverlayBenchmark(unsigned pixels, unsigned long long scalar_ns, unsigned long long vector_ns, int valid) {
    extern void C2VitaLog(const char* format, ...);
    C2VitaLog("OVERLAY SCAN benchmark pixels=%u passes=4 scalar_us=%llu neon_us=%llu valid=%d\n",
        pixels, scalar_ns/1000, vector_ns/1000, valid);
}
/* Per-draw wrappers run ~1200 times a frame; a clock read is a kernel call, so
 * only take it on frames whose timings are logged. */
static Uint64 detail_ticks(void) {
    extern int C2VitaPerfDetail;
    return C2VitaPerfDetail ? SDL_GetTicks() : 0;
}
/* Last uploaded BRender model / scene uniform state, for the shader variant choice
 * (offsets are pinned by the _Static_asserts in shader_brender.c). */
static struct { int lighting, uv_source, disable_colour_key, disable_texture, fog_enabled, prelit, valid; } ubo_model;
static int ubo_clip_planes;
static void capture_uniforms(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) {
    const unsigned char* p = (const unsigned char*)data;
    if (target != GL_UNIFORM_BUFFER || data == NULL || offset != 0) return;
    if (size == 576) {
        memcpy(&ubo_model.lighting, p + 512, 4);
        memcpy(&ubo_model.uv_source, p + 516, 4);
        memcpy(&ubo_model.disable_colour_key, p + 520, 4);
        memcpy(&ubo_model.disable_texture, p + 524, 4);
        memcpy(&ubo_model.fog_enabled, p + 528, 4);
        memcpy(&ubo_model.prelit, p + 572, 4);
        ubo_model.valid = 1;
    } else if (size >= 116) {
        memcpy(&ubo_clip_planes, p + 112, 4);
    }
}
static void measured_buffer_data(GLenum target, GLsizei size, const GLvoid* data, GLenum usage) {
    Uint64 start = detail_ticks();
    capture_uniforms(target, 0, size, data);
    glBufferData(target, size, data, usage);
    frame_buffer_ms += detail_ticks() - start;
    if (size > 0) frame_buffer_bytes += size;
}
static void measured_buffer_subdata(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) {
    Uint64 start = detail_ticks();
    capture_uniforms(target, offset, size, data);
    glBufferSubData(target, offset, size, data);
    frame_buffer_ms += detail_ticks() - start;
    if (size > 0) frame_buffer_bytes += size;
}
/* BRender shader variants (shader_brender.c builds them lazily). */
static GLuint brender_main_program, current_program;
static int brender_active;
static GLuint (*brender_variant)(unsigned key);
static unsigned variant_draws, variant_switches, variant_fallbacks, fast_draws, fast_switches;
void C2VitaGLSetBrenderVariants(GLuint main_program, GLuint (*get)(unsigned key)) {
    brender_main_program = main_program;
    brender_variant = get;
}
/* ids of variant programs already handed out (callers may save and restore one) */
enum { VARIANT_PROGRAMS = 128 };
static GLuint variant_programs[VARIANT_PROGRAMS];
static unsigned variant_program_count;
static int is_brender_program(GLuint program) {
    unsigned i;
    if (program == 0) return 0;
    if (program == brender_main_program) return 1;
    for (i = 0; i < variant_program_count; ++i) if (variant_programs[i] == program) return 1;
    return 0;
}
static void tracked_use_program(GLuint program) {
    current_program = program;
    brender_active = is_brender_program(program);
    glUseProgram(program);
}
static GLint unit0_texture(void);
static int shadow_blend_off(void);
static void select_brender_variant(void) {
    unsigned key = 0, content;
    GLuint program;
    GLint texture;
    if (!brender_active || brender_variant == NULL || !ubo_model.valid) return;
    if (ubo_clip_planes > 0) key |= 1;
    if (ubo_model.uv_source == 1 || ubo_model.uv_source == 2) key |= 2;
    if (!ubo_model.disable_texture) {
        key |= 4;
        texture = unit0_texture();
        content = texture > 0 && texture < TEXTURE_SLOTS ? textures[texture].content : 0;
        if (!(content & TEXC_KNOWN)) content = TEXC_BLACK | TEXC_LOW_ALPHA;
        if (!ubo_model.disable_colour_key && (content & TEXC_BLACK)) key |= 8;
        if (content & TEXC_LOW_ALPHA) key |= 16;
        if ((content & TEXC_CUTOUT) && shadow_blend_off()) key |= 128; /* cut-out drawn unblended */
    }
    if (ubo_model.fog_enabled) key |= 32;
    if (ubo_model.lighting == 1 || ubo_model.prelit == 1) key |= 64;
    program = brender_variant(key);
    if (program == 0) { program = brender_main_program; ++variant_fallbacks; }
    else {
        ++variant_draws;
        if (!is_brender_program(program) && variant_program_count < VARIANT_PROGRAMS)
            variant_programs[variant_program_count++] = program;
    }
    if (program != current_program) {
        ++variant_switches;
        current_program = program;
        glUseProgram(program);
    }
}
/* last draw, for the stall monitor (plain stores; read from another thread only for logging) */
static volatile unsigned stall_draws, stall_count, stall_mode, stall_program, stall_texture;
void C2VitaGLStallInfo(unsigned* draws, unsigned* count, unsigned* mode, unsigned* program, unsigned* texture) {
    *draws = stall_draws; *count = stall_count; *mode = stall_mode; *program = stall_program; *texture = stall_texture;
}
static GLint unit0_texture(void);
static void counted_draw(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {
    Uint64 start = detail_ticks();
    select_brender_variant();
    ++frame_draws;
    stall_draws = frame_draws; stall_count = (unsigned)count; stall_mode = (unsigned)mode;
    stall_program = (unsigned)current_program; stall_texture = (unsigned)unit0_texture();
    glDrawElements(mode, count, type, indices);
    frame_draw_ms += detail_ticks() - start;
}

/* GL state shadow. VitaGL emits GXM state commands (depth func/write, cull mode,
 * depth bias) on every call even when the value is unchanged, and the renderer
 * re-applies the full material state per group. Skip calls that would not
 * change the GL state; VitaGL itself restores GXM state across scenes and
 * clears. vita_video.c draws with direct VitaGL calls and invalidates after. */
/* Values are stored +1 so that 0 (the zero-initialised state) means unknown. */
static struct {
    unsigned char depth_test, cull_face, blend, polygon_offset_fill, scissor_test, depth_mask, colour_mask;
    GLenum depth_func, cull_mode, front_face;
    GLenum blend_func[4];
    float offset_factor, offset_units;
    int offset_valid, blend_valid;
    int units_valid; /* active_unit/bound_2d mirror VitaGL exactly */
    GLenum active_unit;
    GLuint bound_2d[4];
} gl_shadow;
static int shadow_blend_off(void) {
    return gl_shadow.blend == 1; /* known disabled */
}
void C2VitaGLInvalidateState(void) {
    memset(&gl_shadow, 0, sizeof(gl_shadow));
    brender_active = 0;
    current_program = (GLuint)-1;
}
static unsigned char* shadow_cap(GLenum cap) {
    switch (cap) {
    case GL_DEPTH_TEST: return &gl_shadow.depth_test;
    case GL_CULL_FACE: return &gl_shadow.cull_face;
    case GL_BLEND: return &gl_shadow.blend;
    case GL_POLYGON_OFFSET_FILL: return &gl_shadow.polygon_offset_fill;
    case GL_SCISSOR_TEST: return &gl_shadow.scissor_test;
    }
    return NULL;
}
/* Escape hatch for the material back-face culling: ux0:/data/c2port/NOCULL.SET keeps it off. */
static int culling_disabled(void) {
    static int state = -1;
    if (state < 0) {
        FILE* f = fopen("ux0:/data/c2port/NOCULL.SET", "rb");
        state = f != NULL;
        if (f) fclose(f);
        {
            extern void C2VitaLog(const char* format, ...);
            C2VitaLog("VITA CULL back-face culling %s\n", state ? "DISABLED by NOCULL.SET" : "enabled");
        }
    }
    return state;
}
static void shadow_enable(GLenum cap) {
    unsigned char* s;
    if (cap == GL_CULL_FACE && culling_disabled()) return;
    s = shadow_cap(cap);
    if (s != NULL) { if (*s == 2) return; *s = 2; }
    glEnable(cap);
}
static void shadow_disable(GLenum cap) {
    unsigned char* s = shadow_cap(cap);
    if (s != NULL) { if (*s == 1) return; *s = 1; }
    glDisable(cap);
}
static void shadow_depth_func(GLenum func) {
    if (gl_shadow.depth_func == func + 1) return;
    gl_shadow.depth_func = func + 1;
    glDepthFunc(func);
}
static void shadow_depth_mask(GLboolean flag) {
    unsigned char v = flag ? 2 : 1;
    if (gl_shadow.depth_mask == v) return;
    gl_shadow.depth_mask = v;
    glDepthMask(flag);
}
static void shadow_front_face(GLenum mode) {
    /* set per model draw (winding follows det(mvp)); VitaGL re-applies the cull mode each call */
    if (gl_shadow.front_face == mode + 1) return;
    gl_shadow.front_face = mode + 1;
    glFrontFace(mode);
}
static void shadow_cull_face(GLenum mode) {
    if (gl_shadow.cull_mode == mode + 1) return;
    gl_shadow.cull_mode = mode + 1;
    glCullFace(mode);
}
static void shadow_colour_mask(GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
    unsigned char m = (unsigned char)(1 + ((r != 0) | (g != 0) << 1 | (b != 0) << 2 | (a != 0) << 3));
    if (gl_shadow.colour_mask == m) return;
    gl_shadow.colour_mask = m;
    glColorMask(r, g, b, a);
}
static void shadow_polygon_offset(GLfloat factor, GLfloat units) {
    if (gl_shadow.offset_valid && gl_shadow.offset_factor == factor && gl_shadow.offset_units == units) return;
    gl_shadow.offset_valid = 1;
    gl_shadow.offset_factor = factor;
    gl_shadow.offset_units = units;
    glPolygonOffset(factor, units);
}
static void shadow_blend_func_separate(GLenum s_rgb, GLenum d_rgb, GLenum s_a, GLenum d_a) {
    if (gl_shadow.blend_valid && gl_shadow.blend_func[0] == s_rgb && gl_shadow.blend_func[1] == d_rgb
            && gl_shadow.blend_func[2] == s_a && gl_shadow.blend_func[3] == d_a) return;
    gl_shadow.blend_valid = 1;
    gl_shadow.blend_func[0] = s_rgb; gl_shadow.blend_func[1] = d_rgb;
    gl_shadow.blend_func[2] = s_a; gl_shadow.blend_func[3] = d_a;
    glBlendFuncSeparate(s_rgb, d_rgb, s_a, d_a);
}
static void shadow_blend_func(GLenum s, GLenum d) {
    shadow_blend_func_separate(s, d, s, d);
}
/* Texture bindings are recorded (never skipped) so parameter tracking needs no
 * glGetIntegerv round trip per call. Units stay unknown until the first
 * glActiveTexture through this adapter. */
static void shadow_active_texture(GLenum unit) {
    glActiveTexture(unit);
    if (unit >= GL_TEXTURE0 && unit < GL_TEXTURE0 + 4 && gl_shadow.units_valid) {
        gl_shadow.active_unit = unit;
    } else if (unit >= GL_TEXTURE0 && unit < GL_TEXTURE0 + 4) {
        GLint id = 0;
        unsigned i;
        /* learn every unit's 2D binding once, then restore the requested unit */
        for (i = 0; i < 4; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
            gl_shadow.bound_2d[i] = (GLuint)id;
        }
        glActiveTexture(unit);
        gl_shadow.active_unit = unit;
        gl_shadow.units_valid = 1;
    } else gl_shadow.units_valid = 0;
}
static void shadow_bind_texture(GLenum target, GLuint id) {
    glBindTexture(target, id);
    if (target == GL_TEXTURE_2D && gl_shadow.units_valid) gl_shadow.bound_2d[gl_shadow.active_unit - GL_TEXTURE0] = id;
}
static GLint unit0_texture(void) {
    return gl_shadow.units_valid ? (GLint)gl_shadow.bound_2d[0] : -1;
}
static GLint bound_texture_2d(void) {
    GLint id = 0;
    if (gl_shadow.units_valid) return (GLint)gl_shadow.bound_2d[gl_shadow.active_unit - GL_TEXTURE0];
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
    return id;
}
extern void vglFastDrawElementsTexture(GLsizei count, const GLvoid* gl_indices);
/* The next group of a stored model whose material state equals that of the group drawn last except
 * for its texture (gstored.c): binds that texture with the filtering the last one was given and
 * draws, reusing everything else of the last draw in vitaGL (patch 0006); when the texture needs
 * another shader variant it is a normal draw. 0: not possible here (the caller draws normally). */
int C2VitaGLFastTextureDraw(GLuint texture, GLsizei count, const void* offset) {
    GLuint last, program;
    GLint min, mag;
    Uint64 start;
    if (!brender_active || brender_variant == NULL || !ubo_model.valid || ubo_model.disable_texture
            || !gl_shadow.units_valid || gl_shadow.active_unit != GL_TEXTURE0 || texture == 0 || texture >= TEXTURE_SLOTS
            || C2VitaGLQueueActive()) {
        return 0;
    }
    last = gl_shadow.bound_2d[0];
    if (last == 0 || last >= TEXTURE_SLOTS || (textures[last].known & 3) != 3) return 0;
    start = detail_ticks();
    min = textures[last].min;
    mag = textures[last].mag;
    shadow_bind_texture(GL_TEXTURE_2D, texture);
    tracked_tex_parameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    tracked_tex_parameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    program = current_program;
    select_brender_variant();
    ++frame_draws;
    stall_draws = frame_draws; stall_count = (unsigned)count; stall_mode = GL_TRIANGLES;
    stall_program = (unsigned)current_program; stall_texture = texture;
    if (current_program != program) { ++fast_switches; glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, offset); }
    else { ++fast_draws; vglFastDrawElementsTexture(count, offset); }
    frame_draw_ms += detail_ticks() - start;
    return 1;
}
static void shadow_delete_textures(GLsizei count, const GLuint* ids) {
    GLsizei i;
    unsigned u;
    glDeleteTextures(count, ids);
    /* VitaGL rebinds units holding a deleted texture to 0, like GL */
    for (i = 0; i < count; ++i) {
        for (u = 0; u < 4; ++u) if (gl_shadow.bound_2d[u] == ids[i]) gl_shadow.bound_2d[u] = 0;
        if (ids[i] < TEXTURE_SLOTS) textures[ids[i]].known = 0;
    }
}
static void counted_vaos(GLsizei count, GLuint* ids) {
    Uint64 start = SDL_GetTicks();
    glGenVertexArrays(count, ids);
    frame_vao_ms += SDL_GetTicks() - start;
    if (count > 0) frame_vaos += count;
}
static void measured_delete_vaos(GLsizei count, const GLuint* ids) {
    Uint64 start = SDL_GetTicks();
    glDeleteVertexArrays(count, ids);
    frame_vao_ms += SDL_GetTicks() - start;
}
static void measured_clear(GLbitfield mask) {
    Uint64 start = SDL_GetTicks();
    glClear(mask);
    frame_clear_ms += SDL_GetTicks() - start;
}
static void measured_blit(GLint sx0, GLint sy0, GLint sx1, GLint sy1,
    GLint dx0, GLint dy0, GLint dx1, GLint dy1, GLbitfield mask, GLenum filter) {
    Uint64 start = SDL_GetTicks();
    glBlitFramebuffer(sx0,sy0,sx1,sy1,dx0,dy0,dx1,dy1,mask,filter);
    frame_blit_ms += SDL_GetTicks() - start;
}
unsigned C2VitaGLWindowFrames; /* frames so far in the current 120-frame FPS window (test screenshots) */
/* the time at the last frame swap (C2VitaGLFrameStats): a clock for checks that need no precision */
unsigned long long C2VitaGLFrameNS;
void C2VitaGLFrameStats(unsigned int frame, int world, unsigned long long interval, unsigned long long swap_ms) {
    C2VitaGLFrameNS = SDL_GetTicksNS();
    static unsigned int world_frames;
    extern void C2VitaLog(const char* format, ...);
    int logged = frame <= 8 || (world && ++world_frames <= 30) || frame % 600 == 0;
    {
        /* Real frame rate over every frame (the detail lines below only see one timed frame),
         * plus where vglSwapBuffers blocks (counters from vitagl-patches/0003). */
        extern uint32_t vgl_c2_swap_endscene_us, vgl_c2_swap_queue_us, vgl_c2_swap_gc_us;
        static Uint64 window_start;
        static unsigned window_frames, window_world;
        static uint32_t end0, queue0, gc0;
        Uint64 now = SDL_GetTicks();
        if (window_start == 0) {
            window_start = now;
            end0 = vgl_c2_swap_endscene_us; queue0 = vgl_c2_swap_queue_us; gc0 = vgl_c2_swap_gc_us;
        }
        ++window_frames;
        C2VitaGLWindowFrames = window_frames % 120;
        window_world += world != 0;
        if (window_frames == 120) {
            Uint64 span = now - window_start;
            extern float GetYon(void); /* draw distance (depth.c) */
            C2VitaLog("VITA FPS frames=120 world_frames=%u yon=%.0f span_ms=%llu fps=%.1f avg_frame_ms=%.1f swap_endscene_us=%u swap_queue_us=%u swap_gc_us=%u ab=%d\n",
                window_world, (double)GetYon(), (unsigned long long)span, span ? 120000.0 / (double)span : 0.0, (double)span / 120.0,
                (unsigned)((vgl_c2_swap_endscene_us - end0) / 120), (unsigned)((vgl_c2_swap_queue_us - queue0) / 120),
                (unsigned)((vgl_c2_swap_gc_us - gc0) / 120), C2VitaABName() != NULL ? C2VitaAB : -1);
            if (C2VitaABName() != NULL) C2VitaAB = !C2VitaAB; /* in-run A/B: the next window runs the other variant */
            window_start = now;
            window_frames = window_world = 0;
            end0 = vgl_c2_swap_endscene_us; queue0 = vgl_c2_swap_queue_us; gc0 = vgl_c2_swap_gc_us;
        }
    }
    if (logged)
        C2VitaLog("VGL PERF frame=%u world=%d interval=%llu ms draws=%u buffer_bytes=%u buffer_ms=%llu upload_bytes=%u upload_ms=%llu gc_retries=%u free_vram=%u free_ram=%u draw_ms=%llu swap_ms=%llu vao_new=%u vao_ms=%llu clear_ms=%llu blit_ms=%llu image_ms=%llu mipmap_ms=%llu overlay_checks=%u overlay_empty=%u overlay_us=%llu overlay_rows=%u\n",
            frame, world, interval, frame_draws, frame_buffer_bytes, (unsigned long long)frame_buffer_ms,
            frame_upload_bytes, (unsigned long long)frame_upload_ms, frame_gc_retries,
            (unsigned)vglMemFree(VGL_MEM_VRAM), (unsigned)vglMemFree(VGL_MEM_RAM),
            (unsigned long long)frame_draw_ms, swap_ms, frame_vaos, (unsigned long long)frame_vao_ms,
            (unsigned long long)frame_clear_ms, (unsigned long long)frame_blit_ms,
            (unsigned long long)frame_image_ms, (unsigned long long)frame_mipmap_ms,
            frame_overlay_checks, frame_overlay_empty, (unsigned long long)(frame_overlay_ns/1000), frame_overlay_rows);
    {
        char line[256]; int n = 0; unsigned bit;
        for (bit = 0; bit < 17; ++bit)
            if (frame_overlay_scope[bit]) n += snprintf(line + n, sizeof(line) - n, " %u:%u", bit, frame_overlay_scope[bit]);
        line[n] = 0;
        /* only on logged frames: it used to cost an unbuffered memory-card write every frame */
        if (logged) C2VitaLog("VGL OVERLAY checks by innermost scope (bit:count, 0=none 4=render 6=shadows 7=world 8=hud 13=hud_scene 14=geometry 15=redraw):%s\n", line);
        memset(frame_overlay_scope, 0, sizeof(frame_overlay_scope));
    }
    if (logged)
        C2VitaLog("VITA VARIANT draws=%u fallbacks=%u switches=%u programs=%u texture_only=%u (variant changed %u)\n",
            variant_draws, variant_fallbacks, variant_switches, variant_program_count, fast_draws, fast_switches);
    variant_draws = variant_fallbacks = variant_switches = fast_draws = fast_switches = 0;
    frame_overlay_checks = frame_overlay_empty = frame_overlay_rows = 0;
    frame_overlay_ns = 0;
    frame_draws = frame_vaos = frame_buffer_bytes = frame_upload_bytes = frame_gc_retries = 0;
    if (frame == 1 || (world && world_frames == 1)) C2VitaPackCacheLog();
    frame_buffer_ms = frame_upload_ms = frame_draw_ms = frame_vao_ms = frame_clear_ms = frame_blit_ms = frame_image_ms = frame_mipmap_ms = 0;
}

/* Bounded, opt-in tracing for the startup section that currently stops without
 * a dump. This is disabled before normal menu/race rendering. */
static unsigned int texture_trace, texture_operations;
void C2VitaGLTraceTextures(int enabled) {
    texture_trace = enabled != 0;
    texture_operations = 0;
}
static Uint64 texture_begin(const char* operation, GLint level, GLsizei width,
    GLsizei height, GLenum format, GLenum type) {
    GLint id = 0;
    extern void C2VitaLog(const char* format, ...);
    if (!texture_trace || ++texture_operations > 512) return 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
    C2VitaLog("GPU texture begin #%u %s id=%d level=%d size=%dx%d format=%x type=%x free_vram=%u free_ram=%u\n",
        texture_operations, operation, id, level, width, height, format, type,
        (unsigned)vglMemFree(VGL_MEM_VRAM), (unsigned)vglMemFree(VGL_MEM_RAM));
    return SDL_GetTicks();
}
static void texture_end(const char* operation, Uint64 start) {
    extern void C2VitaLog(const char* format, ...);
    if (texture_trace && texture_operations <= 512)
        C2VitaLog("GPU texture end #%u %s elapsed=%llu ms\n", texture_operations,
            operation, (unsigned long long)(SDL_GetTicks() - start));
}
static void traced_tex_image(GLenum target, GLint level, GLint internal, GLsizei width,
    GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data) {
    Uint64 start = texture_begin("image", level, width, height, format, type);
    Uint64 upload_start = SDL_GetTicks();
    if (target == GL_TEXTURE_2D && level == 0) {
        GLint id = bound_texture_2d();
        if (id > 0 && id < TEXTURE_SLOTS) {
            textures[id].content = texture_content(width, height, format, type, data);
            textures[id].w = (unsigned short)width;
            textures[id].h = (unsigned short)height;
        }
    }
    glTexImage2D(target, level, internal, width, height, border, format, type, data);
    frame_image_ms += SDL_GetTicks() - upload_start;
    texture_end("image", start);
}
static void traced_tex_subimage(GLenum target, GLint level, GLint x, GLint y,
    GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data) {
    Uint64 start = texture_begin("subimage", level, width, height, format, type);
    Uint64 upload_start = SDL_GetTicks();
    if (target == GL_TEXTURE_2D && level == 0) {
        GLint id = bound_texture_2d();
        if (id > 0 && id < TEXTURE_SLOTS) {
            unsigned char c = texture_content(width, height, format, type, data);
            if (x == 0 && y == 0 && width == textures[id].w && height == textures[id].h) textures[id].content = c;
            else if (!(c & TEXC_KNOWN)) textures[id].content = 0;
            else if (textures[id].content & TEXC_KNOWN) textures[id].content |= c;
        }
    }
    glTexSubImage2D(target, level, x, y, width, height, format, type, data);
    frame_upload_ms += SDL_GetTicks() - upload_start;
    if (width > 0 && height > 0) frame_upload_bytes += width * height * (type == GL_UNSIGNED_SHORT_5_6_5 ? 2 : 4);
    texture_end("subimage", start);
}
static void traced_mipmap(GLenum target) {
    Uint64 start = texture_begin("mipmap", 0, 0, 0, 0, 0);
    Uint64 mipmap_start = SDL_GetTicks();
    glGenerateMipmap(target);
    /* Native mipmaps use asynchronous GXM transfers, but texture replacement
     * can immediately free/reuse their destination while last_frame is UNUSED.
     * Complete those writes before returning ownership to the caller. A render
     * glFinish is insufficient: transfers have a separate completion API. */
    {
        int result = sceGxmTransferFinish();
        static unsigned int failures;
        extern void C2VitaLog(const char* format, ...);
        if (result < 0 && failures++ < 4)
            C2VitaLog("GPU mipmap transfer wait failed: %08x\n", (unsigned)result);
    }
    frame_mipmap_ms += SDL_GetTicks() - mipmap_start;
    texture_end("mipmap", start);
}

void C2VitaGLResetBindings(void) {
    C2VitaGLInvalidateState();
    tracked_bind_vao(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    tracked_bind_buffer(GL_UNIFORM_BUFFER, 0);
}

/* Only expose the driver API. GLAD also probes hundreds of optional entry
 * points that VitaGL would otherwise report as native errors. */
static const char* const required[] = {
        "glActiveTexture",
        "glAttachShader",
        "glBindAttribLocation",
        "glBindBuffer",
        "glBindBufferBase",
        "glBindFramebuffer",
        "glBindRenderbuffer",
        "glBindTexture",
        "glBindVertexArray",
        "glBlendFunc",
        "glBlendFuncSeparate",
        "glBlitFramebuffer",
        "glBufferData",
        "glBufferSubData",
        "glCheckFramebufferStatus",
        "glClear",
        "glClearColor",
        "glClearDepth",
        "glColorMask",
        "glCompileShader",
        "glCreateProgram",
        "glCreateShader",
        "glCullFace",
        "glDeleteBuffers",
        "glDeleteFramebuffers",
        "glDeleteProgram",
        "glDeleteRenderbuffers",
        "glDeleteShader",
        "glDeleteTextures",
        "glDeleteVertexArrays",
        "glDepthFunc",
        "glDepthMask",
        "glDisable",
        "glDrawElements",
        "glEnable",
        "glEnableVertexAttribArray",
        "glFrontFace",
        "glFramebufferRenderbuffer",
        "glFramebufferTexture2D",
        "glGenBuffers",
        "glGenFramebuffers",
        "glGenRenderbuffers",
        "glGenTextures",
        "glGenVertexArrays",
        "glGenerateMipmap",
        "glGetAttribLocation",
        "glGetBooleanv",
        "glGetError",
        "glGetFloatv",
        "glGetIntegerv",
        "glGetProgramInfoLog",
        "glGetProgramiv",
        "glGetShaderInfoLog",
        "glGetShaderiv",
        "glGetString",
        "glGetStringi",
        "glGetTexParameteriv",
        "glGetUniformBlockIndex",
        "glGetUniformLocation",
        "glIsEnabled",
        "glLinkProgram",
        "glPixelStorei",
        "glPolygonMode",
        "glPolygonOffset",
        "glRenderbufferStorage",
        "glScissor",
        "glShaderSource",
        "glTexImage2D",
        "glTexParameterf",
        "glTexParameteri",
        "glTexSubImage2D",
        "glUniform1f",
        "glUniform1i",
        "glUniform4f",
        "glUniformBlockBinding",
        "glUseProgram",
        "glVertexAttribPointer",
        "glViewport",
    };

static void* vita_gl_lookup(const char* name);
/* Every entry point goes through the GL command queue (vita_glqueue.c), which hands out its
 * recording/waiting wrappers around the adapters below. */
void* C2VitaGLGetProcAddress(const char* name) {
    extern void* C2VitaGLQueueWrap(const char* pName, void* pOriginal);
    void* function = vita_gl_lookup(name);
    return function != NULL ? C2VitaGLQueueWrap(name, function) : NULL;
}

static void* vita_gl_lookup(const char* name) {
    unsigned int i;
#define PROC(glname, function) if (strcmp(name, glname) == 0) return (void*)function
    PROC("glGenTextures", tracked_gen_textures);
    PROC("glTexParameteri", tracked_tex_parameter);
    PROC("glGetTexParameteriv", texture_parameter);
    PROC("glBindBuffer", tracked_bind_buffer);
    PROC("glBindBufferBase", tracked_bind_base);
    PROC("glBindVertexArray", tracked_bind_vao);
    PROC("glGetIntegerv", integer_state);
    PROC("glCreateShader", native_shader);
    PROC("glTexImage2D", traced_tex_image);
    PROC("glTexSubImage2D", traced_tex_subimage);
    PROC("glGenerateMipmap", traced_mipmap);
    PROC("glBufferData", measured_buffer_data);
    PROC("glBufferSubData", measured_buffer_subdata);
    PROC("glDrawElements", counted_draw);
    PROC("glGenVertexArrays", counted_vaos);
    PROC("glDeleteVertexArrays", measured_delete_vaos);
    PROC("glClear", measured_clear);
    PROC("glBlitFramebuffer", measured_blit);
    PROC("glEnable", shadow_enable);
    PROC("glDisable", shadow_disable);
    PROC("glDepthFunc", shadow_depth_func);
    PROC("glDepthMask", shadow_depth_mask);
    PROC("glCullFace", shadow_cull_face);
    PROC("glFrontFace", shadow_front_face);
    PROC("glColorMask", shadow_colour_mask);
    PROC("glPolygonOffset", shadow_polygon_offset);
    PROC("glBlendFunc", shadow_blend_func);
    PROC("glBlendFuncSeparate", shadow_blend_func_separate);
    PROC("glActiveTexture", shadow_active_texture);
    PROC("glBindTexture", shadow_bind_texture);
    PROC("glDeleteTextures", shadow_delete_textures);
    PROC("glUseProgram", tracked_use_program);
#undef PROC
    for (i = 0; i < sizeof(required) / sizeof(required[0]); ++i) {
        if (strcmp(name, required[i]) == 0) return vglGetProcAddress(name);
    }
    return NULL;
}

/* Keep this list in sync with glrend and its commonrend sources. */
int C2VitaGLValidate(void) {
    unsigned int i;
    extern void C2VitaLog(const char* format, ...);
    for (i = 0; i < sizeof(required) / sizeof(required[0]); ++i) {
        if (!C2VitaGLGetProcAddress(required[i])) {
            C2VitaLog("VitaGL required API missing: %s\n", required[i]);
            return 0;
        }
    }
    C2VitaLog("VitaGL required API validation OK (%u functions)\n", i);
    return 1;
}

/* Preserve native VitaGL API/shader/patcher diagnostics without flooding
 * the card with per-draw errors. Linked through --wrap=sceClibPrintf. */
int __wrap_sceClibPrintf(const char* format, ...) {
    static unsigned int messages;
    char message[768];
    int length;
    va_list args;
    extern void C2VitaLog(const char* format, ...);
    va_start(args, format);
    length = vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    if (strstr(message, "failed with a requested size")) ++frame_gc_retries;
    if (++messages <= 64) C2VitaLog("VGL native: %s", message);
    else if (messages == 65) C2VitaLog("VGL native: further native messages limited; first64 retained\n");
    return length;
}
