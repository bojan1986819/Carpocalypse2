#ifndef C2_VITA_VIDEO_H
#define C2_VITA_VIDEO_H

#include <SDL3/SDL.h>

int C2VitaVideoInit(SDL_Window* window);
void C2VitaVideoShutdown(void);
void C2VitaVideoLoadingProgress(void);
void C2VitaVideoPreparingMenu(void);
void C2VitaVideoSwap(int ready);

#endif
