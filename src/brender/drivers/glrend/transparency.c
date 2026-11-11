/* Alpha surfaces in a combined bucket/Z-buffer scene must follow opaque geometry.
 * Snapshot geometry and resolved uniforms: effects reuse/update/free source models
 * and materials between RenderAdd calls. Never retain their mutable pointers.
 * The old BRender 1.4 bucket adaptation remains disabled.
 */
#include "drv.h"
#include "transparency.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct transparent_vertex {
    float p[3], uv[2], normal[3], colour[4];
} transparent_vertex;

typedef struct transparent_gl_state {
    GLint texture, min_filter, mag_filter, wrap_s, wrap_t;
    GLint src_rgb, dst_rgb, src_alpha, dst_alpha, depth_func, cull_mode;
    GLint viewport[4], framebuffer;
    GLfloat anisotropy, offset_factor, offset_units;
    GLboolean blend, depth, depth_write, cull, offset, colour_write[4];
} transparent_gl_state;

typedef struct gl_transparent_draw {
    struct gl_transparent_draw* next;
    float depth;
    unsigned int seq; /* queue order: equal depths keep it */
    GLsizei vertex_count, index_count;
    shader_data_model model;
#ifdef __vita__
    /* Native Cg reads only this128byte prefix, not the4608byte light array.
     * Thousands of transparent groups otherwise exhaust the Vita game heap. */
    unsigned char scene[offsetof(shader_data_scene, lights)];
#else
    shader_data_scene scene;
#endif
    transparent_gl_state gl;
    /* Followed by vertex_count vertices, then index_count ushort indices. */
} gl_transparent_draw;

static void capture_gl(transparent_gl_state* s) {
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &s->texture);
    if (s->texture != 0) {
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &s->min_filter);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &s->mag_filter);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &s->wrap_s);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &s->wrap_t);
        if (GLAD_GL_EXT_texture_filter_anisotropic)
            glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &s->anisotropy);
    }
    glGetIntegerv(GL_BLEND_SRC_RGB, &s->src_rgb);
    glGetIntegerv(GL_BLEND_DST_RGB, &s->dst_rgb);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &s->src_alpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &s->dst_alpha);
    glGetIntegerv(GL_DEPTH_FUNC, &s->depth_func);
    glGetIntegerv(GL_CULL_FACE_MODE, &s->cull_mode);
    glGetIntegerv(GL_VIEWPORT, s->viewport);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &s->framebuffer);
    glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &s->offset_factor);
    glGetFloatv(GL_POLYGON_OFFSET_UNITS, &s->offset_units);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &s->depth_write);
    glGetBooleanv(GL_COLOR_WRITEMASK, s->colour_write);
    s->blend = glIsEnabled(GL_BLEND);
    s->depth = glIsEnabled(GL_DEPTH_TEST);
    s->cull = glIsEnabled(GL_CULL_FACE);
    s->offset = glIsEnabled(GL_POLYGON_OFFSET_FILL);
}

static void enable(GLenum cap, GLboolean value) {
    if (value) glEnable(cap); else glDisable(cap);
}

static void apply_gl(const transparent_gl_state* s) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s->framebuffer);
    glViewport(s->viewport[0], s->viewport[1], s->viewport[2], s->viewport[3]);
    glBindTexture(GL_TEXTURE_2D, s->texture);
    if (s->texture != 0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s->min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s->mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s->wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s->wrap_t);
        if (GLAD_GL_EXT_texture_filter_anisotropic)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, s->anisotropy);
    }
    enable(GL_BLEND, s->blend);
    enable(GL_DEPTH_TEST, s->depth);
    enable(GL_CULL_FACE, s->cull);
    enable(GL_POLYGON_OFFSET_FILL, s->offset);
    glBlendFuncSeparate(s->src_rgb, s->dst_rgb, s->src_alpha, s->dst_alpha);
    glDepthFunc(s->depth_func);
    glDepthMask(s->depth_write);
    glCullFace(s->cull_mode);
    glPolygonOffset(s->offset_factor, s->offset_units);
    glColorMask(s->colour_write[0], s->colour_write[1], s->colour_write[2], s->colour_write[3]);
}

/* The queued groups of a frame live in reused memory chunks and a pointer array, sorted once at the
 * flush (a group's copy and a sorted list insert each cost an allocation and a list walk; a tree-lined
 * view queues 100+ groups a frame). */
typedef struct transparent_chunk {
    struct transparent_chunk* next;
    size_t size, used;
} transparent_chunk;
#define CHUNK_HEADER ((sizeof(transparent_chunk) + 15) & ~(size_t)15)
static transparent_chunk* chunks;
static transparent_chunk* chunk_current;
static gl_transparent_draw** queued;
static int queued_count, queued_capacity;

static void* arena_alloc(size_t bytes) {
    transparent_chunk* c;
    bytes = (bytes + 15) & ~(size_t)15;
    for (c = chunk_current; c != NULL; c = c->next) {
        if (c->size - c->used >= bytes) {
            void* p = (unsigned char*)c + CHUNK_HEADER + c->used;
            c->used += bytes;
            chunk_current = c;
            return p;
        }
    }
    {
        size_t size = bytes > 1024 * 1024 ? bytes : 1024 * 1024;
        transparent_chunk** link = &chunks;
        c = malloc(CHUNK_HEADER + size);
        if (c == NULL) return NULL;
        c->next = NULL;
        c->size = size;
        c->used = bytes;
        while (*link != NULL) link = &(*link)->next;
        *link = c;
        chunk_current = c;
        return (unsigned char*)c + CHUNK_HEADER;
    }
}

static void arena_reset(void) {
    transparent_chunk* c;
    for (c = chunks; c != NULL; c = c->next) c->used = 0;
    chunk_current = chunks;
    queued_count = 0;
}

static int compare_draws(const void* a, const void* b) {
    const gl_transparent_draw* x = *(gl_transparent_draw* const*)a;
    const gl_transparent_draw* y = *(gl_transparent_draw* const*)b;
    if (x->depth != y->depth) return x->depth > y->depth ? -1 : 1; /* back to front */
    return x->seq < y->seq ? -1 : x->seq > y->seq;
}

br_boolean GLQueueTransparent(br_renderer* renderer, const struct v11group* group, const shader_data_model* model) {
    const state_stack* state = renderer->state.current;
    gl_transparent_draw* draw;
    transparent_vertex* vertices;
    br_uint_16* indices;
    float z = 0.f;
    br_size_t bytes;
    int i, j;

    /* Pure Z-buffer/UI/shadow passes keep their immediate ordering. */
    if (state->hidden.type != BRT_BUCKET_AND_BUFFER && state->hidden.type != BRT_BUCKET_SORT)
        return BR_FALSE;
    if (state->output.depth == NULL || state->surface.force_front || state->surface.force_back)
        return BR_FALSE;
    if (!(state->prim.flags & PRIMF_BLEND) &&
        !(state->prim.colour_map != NULL && state->prim.colour_map->blended))
        return BR_FALSE;
    if (group->nvertices == 0 || group->nfaces == 0)
        return BR_FALSE;

    if (queued_count == queued_capacity) {
        int capacity = queued_capacity ? queued_capacity * 2 : 256;
        gl_transparent_draw** grown = realloc(queued, capacity * sizeof(*queued));
        if (grown == NULL) BrFailure("Out of memory queueing transparent geometry");
        queued = grown;
        queued_capacity = capacity;
    }
    bytes = sizeof(*draw) + group->nvertices * sizeof(*vertices) + group->nfaces * 3 * sizeof(*indices);
    draw = arena_alloc(bytes);
    if (draw == NULL) BrFailure("Out of memory snapshotting transparent geometry");
    draw->vertex_count = group->nvertices;
    draw->index_count = group->nfaces * 3;
    draw->model = *model;
    memcpy(&draw->scene, &renderer->state.cache.scene, sizeof(draw->scene));
    capture_gl(&draw->gl);
    /* Opaque depth is complete at replay; sorted alpha must not occlude itself. */
    draw->gl.depth_write = GL_FALSE;
    vertices = (transparent_vertex*)(draw + 1);
    indices = (br_uint_16*)(vertices + group->nvertices);
    for (i = 0; i < group->nvertices; i++) {
        const struct fmt_vertex* v = &group->vertices[i];
        br_colour c = group->vertex_colours[i];
        for (j = 0; j < 3; j++) {
            vertices[i].p[j] = v->p.v[j];
            vertices[i].normal[j] = v->n.v[j];
        }
        vertices[i].uv[0] = v->map.v[0];
        vertices[i].uv[1] = v->map.v[1];
        vertices[i].colour[0] = BR_RED(c) / 255.f;
        vertices[i].colour[1] = BR_GRN(c) / 255.f;
        vertices[i].colour[2] = BR_BLU(c) / 255.f;
        vertices[i].colour[3] = BR_ALPHA(c) / 255.f;
        z += v->p.v[0] * model->model_view.m[0][2]
            + v->p.v[1] * model->model_view.m[1][2]
            + v->p.v[2] * model->model_view.m[2][2] + model->model_view.m[3][2];
    }
    for (i = 0; i < group->nfaces; i++)
        for (j = 0; j < 3; j++) indices[3 * i + j] = group->faces[i].vertices[j];
    draw->depth = -z / group->nvertices;
    /* back to front at the flush, groups of equal depth in queue order */
    draw->seq = (unsigned int)queued_count;
    draw->next = NULL;
    queued[queued_count++] = draw;
    return BR_TRUE;
}

static void init_buffers(br_renderer* renderer, HVIDEO video) {
    const GLint attributes[] = {video->brenderProgram.attributes.aPosition, video->brenderProgram.attributes.aUV,
        video->brenderProgram.attributes.aNormal, video->brenderProgram.attributes.aColour};
    const GLint sizes[] = {3, 2, 3, 4};
    const size_t offsets[] = {offsetof(transparent_vertex, p), offsetof(transparent_vertex, uv),
        offsetof(transparent_vertex, normal), offsetof(transparent_vertex, colour)};
    int i;
    if (renderer->transparent_vao != 0) return;
    glGenVertexArrays(1, &renderer->transparent_vao);
    glGenBuffers(1, &renderer->transparent_vbo);
    glGenBuffers(1, &renderer->transparent_ibo);
    glBindVertexArray(renderer->transparent_vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->transparent_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->transparent_ibo);
    for (i = 0; i < 4; i++) {
        if (attributes[i] < 0) continue;
        glEnableVertexAttribArray(attributes[i]);
        glVertexAttribPointer(attributes[i], sizes[i], GL_FLOAT, GL_FALSE, sizeof(transparent_vertex), (void*)offsets[i]);
    }
}

/* Only the state that differs from the previous group (all of it for the first). */
static void apply_gl_changes(const transparent_gl_state* s, const transparent_gl_state* last) {
    if (last == NULL) {
        apply_gl(s);
        return;
    }
    if (s->framebuffer != last->framebuffer) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s->framebuffer);
    if (memcmp(s->viewport, last->viewport, sizeof(s->viewport)) != 0)
        glViewport(s->viewport[0], s->viewport[1], s->viewport[2], s->viewport[3]);
    if (s->texture != last->texture) glBindTexture(GL_TEXTURE_2D, s->texture);
    if (s->texture != 0 && (s->texture != last->texture || s->min_filter != last->min_filter || s->mag_filter != last->mag_filter
            || s->wrap_s != last->wrap_s || s->wrap_t != last->wrap_t || s->anisotropy != last->anisotropy)) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s->min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s->mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s->wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s->wrap_t);
        if (GLAD_GL_EXT_texture_filter_anisotropic)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, s->anisotropy);
    }
    if (s->blend != last->blend) enable(GL_BLEND, s->blend);
    if (s->depth != last->depth) enable(GL_DEPTH_TEST, s->depth);
    if (s->cull != last->cull) enable(GL_CULL_FACE, s->cull);
    if (s->offset != last->offset) enable(GL_POLYGON_OFFSET_FILL, s->offset);
    if (s->src_rgb != last->src_rgb || s->dst_rgb != last->dst_rgb || s->src_alpha != last->src_alpha || s->dst_alpha != last->dst_alpha)
        glBlendFuncSeparate(s->src_rgb, s->dst_rgb, s->src_alpha, s->dst_alpha);
    if (s->depth_func != last->depth_func) glDepthFunc(s->depth_func);
    if (s->depth_write != last->depth_write) glDepthMask(s->depth_write);
    if (s->cull_mode != last->cull_mode) glCullFace(s->cull_mode);
    if (s->offset_factor != last->offset_factor || s->offset_units != last->offset_units)
        glPolygonOffset(s->offset_factor, s->offset_units);
    if (memcmp(s->colour_write, last->colour_write, sizeof(s->colour_write)) != 0)
        glColorMask(s->colour_write[0], s->colour_write[1], s->colour_write[2], s->colour_write[3]);
}

void GLFlushTransparent(br_renderer* renderer) {
    static transparent_vertex* vertex_buffer;
    static br_uint_16* index_buffer;
    static size_t vertex_capacity, index_capacity;
    HVIDEO video;
    transparent_gl_state saved;
    const transparent_gl_state* last = NULL;
    const void* last_scene = NULL;
    GLint vao, array_buffer, uniform_buffer;
    unsigned int groups = 0, bytes = 0;
    int i = 0, fast = 1;
    if (queued_count == 0) return;
#ifdef __vita__
    {
        /* test: C2_TEST_AB=transp alternates with one upload and full state per group */
        extern int C2VitaABFeature(const char* pFeature, int pDefault);
        fast = C2VitaABFeature("transp", 1);
    }
#endif
    video = &renderer->pixelmap->screen->asFront.video;
#ifdef __vita__
    {
        extern int gGLModelUboValid;
        gGLModelUboValid = 0; /* this pass writes the model block */
    }
#endif
    qsort(queued, (size_t)queued_count, sizeof(*queued), compare_draws);
    capture_gl(&saved);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &array_buffer);
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &uniform_buffer);
    init_buffers(renderer, video);
    glBindVertexArray(renderer->transparent_vao);
    while (i < queued_count) {
        /* the next run of groups whose vertices fit 16-bit indices: one upload for all of them */
        int first = i, k;
        size_t nv = 0, ni = 0, at_v = 0, at_i = 0;
        while (i < queued_count && (i == first || (fast && nv + queued[i]->vertex_count <= 65535))) {
            nv += queued[i]->vertex_count;
            ni += queued[i]->index_count;
            i++;
        }
        if (nv > vertex_capacity) {
            transparent_vertex* grown = realloc(vertex_buffer, nv * sizeof(*vertex_buffer));
            if (grown == NULL) BrFailure("Out of memory drawing transparent geometry");
            vertex_buffer = grown;
            vertex_capacity = nv;
        }
        if (ni > index_capacity) {
            br_uint_16* grown = realloc(index_buffer, ni * sizeof(*index_buffer));
            if (grown == NULL) BrFailure("Out of memory drawing transparent geometry");
            index_buffer = grown;
            index_capacity = ni;
        }
        for (k = first; k < i; k++) {
            const gl_transparent_draw* draw = queued[k];
            const transparent_vertex* vertices = (const transparent_vertex*)(draw + 1);
            const br_uint_16* indices = (const br_uint_16*)(vertices + draw->vertex_count);
            int n;
            memcpy(vertex_buffer + at_v, vertices, draw->vertex_count * sizeof(*vertices));
            for (n = 0; n < draw->index_count; n++) index_buffer[at_i + n] = (br_uint_16)(indices[n] + at_v);
            at_v += draw->vertex_count;
            at_i += draw->index_count;
            groups++;
            bytes += sizeof(*draw) + draw->vertex_count * sizeof(*vertices) + draw->index_count * sizeof(*indices);
        }
        glBindBuffer(GL_ARRAY_BUFFER, renderer->transparent_vbo);
        glBufferData(GL_ARRAY_BUFFER, nv * sizeof(*vertex_buffer), vertex_buffer, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni * sizeof(*index_buffer), index_buffer, GL_STREAM_DRAW);
        at_i = 0;
        for (k = first; k < i; k++) {
            const gl_transparent_draw* draw = queued[k];
            apply_gl_changes(&draw->gl, last);
            last = fast ? &draw->gl : NULL;
            if (!fast || last_scene == NULL || memcmp(last_scene, &draw->scene, sizeof(draw->scene)) != 0) {
                glBindBuffer(GL_UNIFORM_BUFFER, video->brenderProgram.uboScene);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(draw->scene), &draw->scene);
                last_scene = &draw->scene;
            }
            glBindBuffer(GL_UNIFORM_BUFFER, video->brenderProgram.uboModel);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(draw->model), &draw->model);
            glDrawElements(GL_TRIANGLES, draw->index_count, GL_UNSIGNED_SHORT, (const void*)(at_i * sizeof(*index_buffer)));
            at_i += draw->index_count;
        }
    }
    arena_reset();
    glBindBuffer(GL_UNIFORM_BUFFER, video->brenderProgram.uboScene);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, GL_SCENE_UNIFORM_BYTES, &renderer->state.cache.scene);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
    apply_gl(&saved);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
    {
        static int logging = -1;
        static unsigned int scenes;
        if (logging < 0) logging = getenv("C2_OBJLOG") != NULL;
        if (logging && (++scenes % 120) == 1)
            BrWarning("ALPHALOG groups=%u snapshot_bytes=%u", groups, bytes);
    }
}

void GLFreeTransparent(br_renderer* renderer) {
    arena_reset();
    renderer->transparent_head = NULL;
    if (renderer->transparent_vao != 0) glDeleteVertexArrays(1, &renderer->transparent_vao);
    if (renderer->transparent_vbo != 0) glDeleteBuffers(1, &renderer->transparent_vbo);
    if (renderer->transparent_ibo != 0) glDeleteBuffers(1, &renderer->transparent_ibo);
    renderer->transparent_vao = renderer->transparent_vbo = renderer->transparent_ibo = 0;
}
