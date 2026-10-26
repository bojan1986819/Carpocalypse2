#ifndef C2_VITA_GLYPH_H
#define C2_VITA_GLYPH_H
#include <brender/brender.h>

/* One run per string/page, split only at the fixed geometry capacity. */
void C2VitaGlyphStringBegin(void);
void C2VitaGlyphSubmit(br_actor* root, const br_model* model,
    const br_material* material, float x, float y);
void C2VitaGlyphStringEnd(void);
int C2VitaGlyphHasActors(void);
void C2VitaGlyphClean(void);
void C2VitaGlyphSceneEnded(void);
void C2VitaGlyphStats(unsigned* glyphs, unsigned* draws);
#endif
