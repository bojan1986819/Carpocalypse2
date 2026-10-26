#if defined(__vita__) || defined(C2_GLYPH_BATCH)
#include "vita_text.h"
#include "vita_glyph.h"
#ifdef __vita__
extern void C2VitaLog(const char* format, ...);
#else
#include <stdio.h>
#define C2VitaLog(...) fprintf(stderr, __VA_ARGS__)
#endif
static unsigned batch_depth, strings, scenes, barriers, batches;
static int scene_open;
static br_actor *scene_root, *scene_camera;
static br_pixelmap *scene_colour, *scene_depth;
static struct { int x,y,bx,by,w,h; } viewport;

int C2VitaTextSceneIsOpen(void) { return scene_open; }

void C2VitaTextBatchFlush(void) {
    if (scene_open) {
        BrZbSceneRenderEnd();
        scene_open = 0;
        C2VitaGlyphSceneEnded();
        ++barriers;
    }
}
void C2VitaTextBatchBegin(void) {
    if (batch_depth++ == 0) strings = scenes = barriers = 0;
}
void C2VitaTextBatchEnd(void) {
    unsigned glyphs, draws;
    if (!batch_depth || --batch_depth) return;
    C2VitaTextBatchFlush();
    C2VitaGlyphStats(&glyphs, &draws);
    if (++batches <= 8 || batches % 120 == 0)
        C2VitaLog("VITA TEXT batch=%u strings=%u scenes=%u barriers=%u glyphs=%u glyph_batches=%u\n",
            batches, strings, scenes, barriers, glyphs, draws);
}
void C2VitaTextSceneRender(br_actor* root, br_actor* camera, br_pixelmap* colour, br_pixelmap* depth) {
    br_actor* child;
    if (!batch_depth) {
        BrZbSceneRender(root,camera,colour,depth);
        return;
    }
    ++strings;
    if (scene_open && (scene_root != root || scene_camera != camera
            || scene_colour != colour || scene_depth != depth
            || viewport.x != colour->origin_x || viewport.y != colour->origin_y
            || viewport.bx != colour->base_x || viewport.by != colour->base_y
            || viewport.w != colour->width || viewport.h != colour->height))
        C2VitaTextBatchFlush();
    if (!scene_open) {
        scene_root=root; scene_camera=camera; scene_colour=colour; scene_depth=depth;
        viewport.x=colour->origin_x; viewport.y=colour->origin_y;
        viewport.bx=colour->base_x; viewport.by=colour->base_y;
        viewport.w=colour->width; viewport.h=colour->height;
        BrZbSceneRenderBegin(root,camera,colour,depth);
        scene_open=1;
        ++scenes;
    }
    /* Full BrZbSceneRender traverses the world's children, ignoring its own
     * transform. Add each child preserves that rule and inherited root state.
     * Font materials force foreground drawing. Glyph pool slots additionally
     * retain their geometry and material snapshots until this scene ends. */
    for (child=root->children;child;child=child->next)
        BrZbSceneRenderAdd(child);
}

#endif
