#ifndef C2_SDL3_DISPLAY_H
#define C2_SDL3_DISPLAY_H
#include "carpocalypse2_types.h"
void carpocalypse2_DisplayInit(void);
void carpocalypse2_DisplayRefresh(void);
void carpocalypse2_DisplayMouse(float x, float y, float* gx, float* gy);
void PDAddDisplayOptions(tFrontend_spec* options);
float PDWidescreenFactor(void);
int PDSetWorldRendering(int active);
int PDSetWideBackdrop(int active);
#endif
