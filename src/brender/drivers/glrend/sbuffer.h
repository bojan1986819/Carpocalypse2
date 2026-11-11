/*
 * Stored buffer structure
 */
#ifndef _SBUFFER_H_
#define _SBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_BUFFER_STORED_PRIVATE

/*
 * Block structure
 */
typedef struct br_buffer_stored {
    /*
     * Dispatch table
     */
    struct br_buffer_stored_dispatch *dispatch;

    /*
     * Standard object identifier
     */
    const char *identifier;

    br_device   *device;
    br_renderer *renderer;

    /*
     * Source pixelmap
     */
    br_pixelmap *source;

    /*
     * Copy of source flags
     */
    br_uint_16 source_flags;

    /*
     * OpenGL texture handle
     */
    GLuint gl_tex;

    GLenum gl_internal_format;
    GLenum gl_format;
    GLenum gl_type;

    br_boolean blended;

    /*
     * Object query templates.
     * FIXME: These should be stored on the device, but that can't
     *  be done until everything's ported to C.
     */
    struct br_tv_template *templates;

    br_boolean paletted_source_dirty;
    br_uint_32 palette_revision;
    struct br_device_clut *palette_pointer;

    /* port: hash of the pixels last uploaded from a non-paletted memory pixelmap, so updates
     * that change nothing (the HUD re-sends several textures every frame) skip the upload */
    br_boolean content_valid;
    br_uint_32 content_hash;

    /* port: RGBA4444 sources updated after their first upload (HUD maps redrawn by the game): the
     * converted pixels last uploaded, compared with the new ones as they are converted */
    br_uint_16* shadow;
    int shadow_w, shadow_h;
    int updates_4444;

} br_buffer_stored;

#endif

#ifdef __cplusplus
};
#endif
#endif
