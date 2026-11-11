/*
 * Geometry format
 */
#ifndef _GSTORED_H_
#define _GSTORED_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gl_vertex_f {
    /*
     * Mapping coordinates
     */
    br_vector2_f map;

    /*
     * Surface normal at vertex
     */
    br_vector3_f n;

    /*
     * Colour
     */
    br_vector4_f c;
} gl_vertex_f;

typedef struct gl_groupinfo {
    /*
     * Index count for glDrawElements().
     */
    GLsizei count;

    /*
     * Offset for glDrawElements()
     */
    void* offset;

    /*
     * The group itself
     */
    struct v11group* group;

    /*
     * Stored renderer state for the group
     */
    br_renderer_state_stored* stored;

    /*
     * Stored default renderer state for the group
     */
    br_renderer_state_stored* default_state;
} gl_groupinfo;

#ifdef BR_GEOMETRY_STORED_PRIVATE

#include "gstored_base.h"

/*
 * Private state of geometry format
 */
typedef struct br_geometry_stored {
    BR_GEOMETRY_STORED_BASE;

    GLuint gl_vao;
    GLuint gl_vbo_posn;
    GLuint gl_vbo;
    GLuint gl_ibo;
    GLenum gl_primitive;

    gl_groupinfo* groups;
    /* Vita geometry cache: the model changed since the buffers were filled (bit 0: vertices, bit 1:
     * indices too); refilled when a group is first drawn from them (blended groups never are) */
    int vita_pending_refresh;
    /* Vita: the order its groups are drawn in (gstored.c draw_order), for vita_order_groups groups */
    br_uint_16* vita_order;
    int vita_order_groups;
} br_geometry_stored;

void StoredGLUseEdges(br_geometry_stored* self);
void StoredGLRefresh(br_geometry_stored* self, int rebuild_indices);
void StoredGLMarkRefresh(br_geometry_stored* self, int rebuild_indices);
void StoredGLMarkIndexRefresh(br_geometry_stored* self);
void StoredGLApplyRefresh(br_geometry_stored* self);

#endif

#ifdef __cplusplus
};
#endif
#endif /* _GSTORED_H_ */
