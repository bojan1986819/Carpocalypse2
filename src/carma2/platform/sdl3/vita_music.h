#ifndef C2_VITA_MUSIC_H
#define C2_VITA_MUSIC_H

#include <SDL3/SDL.h>

/* Main-thread lifecycle; decoding runs on one joined worker per track. */
SDL_AudioStream* C2VitaMusicStart(const char* path);
void C2VitaMusicStop(void);
int C2VitaMusicPlaying(void);

#endif
