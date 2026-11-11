#include <string.h>
/*
 * Stored buffer methods
 */
#include <stddef.h>
#include <stdint.h>
#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#include "brassert.h"
#include "drv.h"

/*
 * Set up a static device object
 */
void BufferStoredGLInitFields(struct br_buffer_stored* self) {
    self->gl_tex = 0;
    self->content_valid = BR_FALSE;
    self->content_hash = 0;
    self->shadow = NULL;
    self->shadow_w = self->shadow_h = 0;
    self->updates_4444 = 0;
}

/* FNV-1a over the visible rows, plus the layout, of a memory pixelmap */
static br_uint_32 pixelmap_content_hash(const br_pixelmap* pm, int pixel_bytes) {
    br_uint_32 h = 2166136261u;
    int y;
    size_t i, n = (size_t)pm->width * pixel_bytes;

    h = (h ^ (br_uint_32)pm->type) * 16777619u;
    h = (h ^ (br_uint_32)pm->width) * 16777619u;
    h = (h ^ (br_uint_32)pm->height) * 16777619u;
    for (y = 0; y < pm->height; y++) {
        const br_uint_8* row = (const br_uint_8*)pm->pixels + (size_t)(pm->base_y + y) * pm->row_bytes + (size_t)pm->base_x * pixel_bytes;
        if ((n & 3) == 0 && ((uintptr_t)row & 3) == 0) {
            const br_uint_32* w = (const br_uint_32*)row;
            for (i = 0; i < n / 4; i++)
                h = (h ^ w[i]) * 16777619u;
        } else {
            for (i = 0; i < n; i++)
                h = (h ^ row[i]) * 16777619u;
        }
    }
    return h;
}

static br_boolean is_compatible(br_buffer_stored* self, br_pixelmap* pm, GLenum internal_format) {
    if (self->source == NULL)
        return BR_FALSE;

    if (self->gl_tex == 0 || self->gl_internal_format == 0 || self->gl_format == 0 || self->gl_type == 0)
        return BR_FALSE;

    if (self->source->width != pm->width || self->source->height != pm->height)
        return BR_FALSE;

    if (self->gl_internal_format != internal_format)
        return BR_FALSE;

    return BR_TRUE;
}

static const char* gl_strerror(GLenum err) {
    static char errbuf[64];
    const char* s;

    switch (err) {
    case 0:
        s = "GL_NO_ERROR";
        break;
    case GL_INVALID_ENUM:
        s = "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        s = "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        s = "GL_INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        s = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        s = "GL_OUT_OF_MEMORY";
        break;
    default:
        s = NULL;
    }

    if (s != NULL)
        BrSprintfN(errbuf, sizeof(errbuf) - 1, "error %d (%s)", err, s);
    else
        BrSprintfN(errbuf, sizeof(errbuf) - 1, "error %d", err);

    errbuf[sizeof(errbuf) - 1] = '\0';
    return errbuf;
}


/* ARGB4444 -> RGBA4444 (rotate left by 4) of pm into dst (w x h, packed); returns whether dst changed,
 * and whether any pixel is not opaque. */
static int convert_argb4444(br_uint_16* dst, const br_pixelmap* pm, br_boolean* has_alpha, br_boolean* partial_alpha) {
    int w = pm->width, h = pm->height, x, y;
    br_uint_16 changed = 0, alpha = 0xf, partial = 0;

    for (y = 0; y < h; y++) {
        const br_uint_16* src = (const br_uint_16*)((const br_uint_8*)pm->pixels + (size_t)(pm->base_y + y) * pm->row_bytes) + pm->base_x;
        br_uint_16* out = dst + (size_t)y * w;
        x = 0;
#ifdef __ARM_NEON
        {
            uint16x8_t diff = vdupq_n_u16(0), all = vdupq_n_u16(0xffff), part = vdupq_n_u16(0);
            const uint16x8_t nibble = vdupq_n_u16(0xf), zero = vdupq_n_u16(0);
            for (; x + 8 <= w; x += 8) {
                uint16x8_t v = vld1q_u16(src + x);
                uint16x8_t r = vorrq_u16(vshlq_n_u16(v, 4), vshrq_n_u16(v, 12));
                uint16x8_t a = vandq_u16(r, nibble);
                diff = vorrq_u16(diff, veorq_u16(r, vld1q_u16(out + x)));
                all = vandq_u16(all, r);
                part = vorrq_u16(part, vmvnq_u16(vorrq_u16(vceqq_u16(a, zero), vceqq_u16(a, nibble))));
                vst1q_u16(out + x, r);
            }
            {
                uint16x4_t d = vorr_u16(vget_low_u16(diff), vget_high_u16(diff));
                uint16x4_t a = vand_u16(vget_low_u16(all), vget_high_u16(all));
                uint16x4_t p = vorr_u16(vget_low_u16(part), vget_high_u16(part));
                changed |= vget_lane_u16(d, 0) | vget_lane_u16(d, 1) | vget_lane_u16(d, 2) | vget_lane_u16(d, 3);
                alpha &= vget_lane_u16(a, 0) & vget_lane_u16(a, 1) & vget_lane_u16(a, 2) & vget_lane_u16(a, 3);
                partial |= vget_lane_u16(p, 0) | vget_lane_u16(p, 1) | vget_lane_u16(p, 2) | vget_lane_u16(p, 3);
            }
        }
#endif
        for (; x < w; x++) {
            br_uint_16 r = (br_uint_16)((src[x] << 4) | (src[x] >> 12));
            changed |= r ^ out[x];
            alpha &= r;
            partial |= (r & 0xf) != 0 && (r & 0xf) != 0xf;
            out[x] = r;
        }
    }
    *has_alpha = (alpha & 0xf) != 0xf;
    *partial_alpha = partial != 0;
    return changed != 0;
}

#include <stdio.h>
#ifdef __vita__
/* test statistics of 4444 texture updates (logged every 600 updates) */
extern unsigned long long C2VitaGLTicksNS(void);
static unsigned long long gUpd_hash_ns, gUpd_expand_ns, gUpd_image_ns, gUpd_mip_ns;
static unsigned gUpd_count, gUpd_skipped;
#define UPD_T0() unsigned long long upd_t = C2VitaGLTicksNS()
#define UPD_T(acc) do { unsigned long long upd_n = C2VitaGLTicksNS(); acc += upd_n - upd_t; upd_t = upd_n; } while (0)
#else
#define UPD_T0() ((void)0)
#define UPD_T(acc) ((void)0)
#endif
static br_error updateMemory(br_buffer_stored* self, br_pixelmap* pm) {
    GLint internal_format;
    GLenum format, type, err;
    GLsizeiptr elem_bytes;
    br_error r;

    /*
     * The pixelmap is a plain BRender memory pixelmap. Make sure that the pixels can be accessed
     */
    if ((pm->flags & BR_PMF_NO_ACCESS) || pm->pixels == NULL)
        do { static int lg; if (lg++ < 4) BrWarning("glrend: updateMemory fail at line %d for '%s' type %d flags %x pixels %p", __LINE__, pm->identifier ? pm->identifier : "?", pm->type, pm->flags, pm->pixels); return BRE_FAIL; } while (0);

    /* Paletted textures convert at draw time against the current palette, so only the 16-bit
     * types skip an update that would upload the same pixels again. */
    UPD_T0();
    if (pm->type == BR_PMT_RGB_565) {
        br_uint_32 hash = pixelmap_content_hash(pm, 2);
        UPD_T(gUpd_hash_ns);
        if (self->content_valid && self->gl_tex != 0 && self->source == pm && self->content_hash == hash
                && self->source_flags == pm->flags) {
#ifdef __vita__
            gUpd_skipped++;
#endif
            return BRE_OK;
        }
        self->content_valid = BR_FALSE;
        self->content_hash = hash;
    } else if (pm->type != BR_PMT_RGBA_4444) {
        self->content_valid = BR_FALSE;
    }

    if (pm->type == BR_PMT_RGBA_4444) {
        /* port: the game's ARGB4444 (alpha in the top nibble, as the 3Dfx path uses it) is GL's
         * RGBA4444 (alpha in the bottom nibble) rotated by 4 bits; vitaGL stores that format natively
         * (a copy), so it is uploaded as is. A texture updated again (the HUD maps, redrawn by the
         * game every frame) keeps the pixels last uploaded: the new ones are compared while they are
         * converted, an unchanged update uploads nothing and a changed one rewrites the texture. */
        int w = pm->width, h = pm->height, fresh, changed;
        br_boolean has_alpha = BR_FALSE, partial_alpha = BR_FALSE;
        br_uint_16* pixels;

        self->updates_4444++;
        fresh = self->gl_tex == 0 || self->shadow == NULL || self->shadow_w != w || self->shadow_h != h
            || self->source != pm || self->source_flags != pm->flags || !self->content_valid;
        if (self->updates_4444 > 1 && (self->shadow == NULL || self->shadow_w != w || self->shadow_h != h)) {
            if (self->shadow != NULL) {
                BrResFree(self->shadow);
            }
            self->shadow = BrResAllocate(self, (br_size_t)w * h * sizeof(br_uint_16), BR_MEMORY_PIXELS);
            self->shadow_w = self->shadow != NULL ? w : 0;
            self->shadow_h = self->shadow != NULL ? h : 0;
        }
        pixels = self->shadow != NULL ? self->shadow : BrScratchAllocate((br_size_t)w * h * sizeof(br_uint_16));
        changed = convert_argb4444(pixels, pm, &has_alpha, &partial_alpha);
        UPD_T(gUpd_expand_ns);
        if (!fresh && !changed) {
#ifdef __vita__
            gUpd_skipped++;
#endif
            return BRE_OK;
        }
        if (self->gl_tex == 0) {
            glGenTextures(1, &self->gl_tex);
        }
        glBindTexture(GL_TEXTURE_2D, self->gl_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#ifndef __vita__
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
#endif
        if (fresh) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels);
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels);
        }
        UPD_T(gUpd_image_ns);
#ifndef __vita__
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#endif
        glGenerateMipmap(GL_TEXTURE_2D);
        UPD_T(gUpd_mip_ns);
#ifdef __vita__
        if (++gUpd_count % 600 == 0) {
            extern void C2VitaLog(const char* format, ...);
            C2VitaLog("VITA TEX4444 uploads=%u skipped=%u per upload: convert %llu image %llu mipmap %llu us (last %dx%d)\n",
                gUpd_count, gUpd_skipped, gUpd_expand_ns / (gUpd_count + gUpd_skipped) / 1000,
                gUpd_image_ns / gUpd_count / 1000, gUpd_mip_ns / gUpd_count / 1000, w, h);
        }
#endif
        glBindTexture(GL_TEXTURE_2D, 0);
        if (pixels != self->shadow) {
            BrScratchFree(pixels);
        }
        /* Fully opaque RGBA textures still belong in the opaque depth pass. */
        /* port: alpha only 0 or 15 is a cut-out (foliage, fences): drawn in the opaque pass with an
         * alpha test (vita_gl.c), exactly as blending it would look, without the sorted pass (and the
         * track batch can merge it, trackbatch.c). C2_TEST_NOCUTOUT keeps blending them. */
        self->blended = has_alpha && (partial_alpha || getenv("C2_TEST_NOCUTOUT") != NULL);
        self->paletted_source_dirty = BR_FALSE;
        self->source = pm;
        self->source_flags = pm->flags;
        self->content_valid = BR_TRUE;
        GL_CHECK_ERROR();
        return BRE_OK;
    }

    r = VIDEOI_BrPixelmapGetTypeDetails(pm->type, &internal_format, &format, &type, &elem_bytes, &self->blended);
    if (r != BRE_OK)
        return r;

    /*
     * If we're compatible, update the existing texture.
     */
    if (is_compatible(self, pm, internal_format) == BR_TRUE) {
        ASSERT(self->gl_tex != 0);

        BR_FATAL0("updateMemory path not implemented\n");

        do { static int lg; if (lg++ < 4) BrWarning("glrend: updateMemory fail at line %d for '%s' type %d flags %x pixels %p", __LINE__, pm->identifier ? pm->identifier : "?", pm->type, pm->flags, pm->pixels); return BRE_FAIL; } while (0);
    }

    if (self->gl_tex == 0) {
        glGenTextures(1, &self->gl_tex);
        glBindTexture(GL_TEXTURE_2D, self->gl_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, pm->width, pm->height, 0, format, type, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if ((err = glGetError()) != 0) {
            BR_ERROR1("GLREND: glGenTextures() failed with %s", gl_strerror(err));
            do { static int lg; if (lg++ < 4) BrWarning("glrend: updateMemory fail at line %d for '%s' type %d flags %x pixels %p", __LINE__, pm->identifier ? pm->identifier : "?", pm->type, pm->flags, pm->pixels); return BRE_FAIL; } while (0);
        }
    }

    //BR_WARNING("updateMemory");

    glBindTexture(GL_TEXTURE_2D, self->gl_tex);
    self->paletted_source_dirty = BR_FALSE;
    if (pm->type == BR_PMT_INDEX_8) {
        // if paletted, then wait until we are displaying the texture to convert it to 32 bit
        self->paletted_source_dirty = BR_TRUE;
        self->palette_pointer = ObjectDevice(self)->clut;
    } else if (pm->type == BR_PMT_RGB_565) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pm->width, pm->height, format, type, pm->pixels);
    } else {
        do { static int lg; if (lg++ < 4) BrWarning("glrend: updateMemory fail at line %d for '%s' type %d flags %x pixels %p", __LINE__, pm->identifier ? pm->identifier : "?", pm->type, pm->flags, pm->pixels); return BRE_FAIL; } while (0);
    }

    if ((err = glGetError()) != 0) {
        BR_FATAL1("GLREND: glTexImage2D() failed with %s", gl_strerror(err));
        glBindTexture(GL_TEXTURE_2D, 0);
        do { static int lg; if (lg++ < 4) BrWarning("glrend: updateMemory fail at line %d for '%s' type %d flags %x pixels %p", __LINE__, pm->identifier ? pm->identifier : "?", pm->type, pm->flags, pm->pixels); return BRE_FAIL; } while (0);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    self->source = pm;
    self->source_flags = pm->flags;
    self->content_valid = pm->type == BR_PMT_RGB_565;

    glBindTexture(GL_TEXTURE_2D, 0);
    GL_CHECK_ERROR();
    return BRE_OK;
}

br_error BREND_CMETHOD_DECL(BREND_CLASS(br_buffer_stored_), update)(struct br_buffer_stored* self,
    struct br_device_pixelmap* pm, br_token_value* tv) {
    br_device* pm_device;
    (void)tv;

    if (pm->pm_type != BR_PMT_INDEX_8 && pm->pm_type != BR_PMT_RGB_565 && pm->pm_type != BR_PMT_RGBA_4444) {
        static int logged;
        if (logged++ < 10)
            BrWarning("glrend: texture '%s' type %d %dx%d unsupported", pm->pm_identifier ? pm->pm_identifier : "?", pm->pm_type, pm->pm_width, pm->pm_height);
        return BRE_FAIL;
    }

    /*
     * Find out where the pixelmap comes from
     */
    pm_device = ObjectDevice(pm);
    if (pm_device == NULL) {
        return updateMemory(self, (br_pixelmap*)pm);
    } else if (pm_device == self->device) {
        ASSERT(self->source == NULL || self->source == pm);
        self->gl_tex = 0; /* Unused for us. */
        self->source = (br_pixelmap*)pm;
        self->source_flags = pm->pm_flags;
        return BRE_OK;
    } else {
        /*
         * The pixelmap is from another device, we can't use it
         */
        return BRE_FAIL;
    }
}

void BREND_CMETHOD_DECL(BREND_CLASS(br_buffer_stored_), free)(br_object* _self) {
    br_buffer_stored* self = (br_buffer_stored*)_self;

    glDeleteTextures(1, &self->gl_tex);
    self->gl_tex = 0;

    ObjectContainerRemove(self->renderer, (br_object*)self);
    /* and the object itself (with its templates and shadow): it was kept, ~700 per race */
    BrResFreeNoCallback(self);
}

GLuint BufferStoredGLGetTexture(br_buffer_stored* self) {
    if (self->source == NULL)
        return self->gl_tex;

    if (ObjectDevice(self->source) == self->device) {
        br_device_pixelmap* pm = (br_device_pixelmap*)self->source;
        switch (pm->use_type) {
        case BRT_NONE:
        default:
            return 0;

        case BRT_OFFSCREEN:
            return pm->asBack.glTex;

        case BRT_DEPTH:
            return pm->asDepth.glDepth;
        }
    }

    return self->gl_tex;
}
