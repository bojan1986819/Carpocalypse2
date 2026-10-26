#ifndef C2_VITA_TEXT_H
#define C2_VITA_TEXT_H
#include <brender/brender.h>
/* Scoped to the final HUD text redraw. Each string is rendered immediately;
 * only scene setup/teardown is shared. Glyph slots live through sceneEnd. */
int C2VitaTextSceneIsOpen(void);
void C2VitaTextBatchBegin(void);
void C2VitaTextBatchFlush(void);
void C2VitaTextBatchEnd(void);
void C2VitaTextSceneRender(br_actor* root, br_actor* camera, br_pixelmap* colour, br_pixelmap* depth);
#endif
