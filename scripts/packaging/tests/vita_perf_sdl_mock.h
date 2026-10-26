#pragma once
#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
typedef uint64_t Uint64; typedef uint32_t Uint32;
typedef struct { _Atomic int value; } SDL_AtomicInt;
static inline int SDL_GetAtomicInt(SDL_AtomicInt* p) {return atomic_load(&p->value);}
static inline int SDL_SetAtomicInt(SDL_AtomicInt* p,int v) {return atomic_exchange(&p->value,v);}
typedef void SDL_Thread;
#define SDLCALL
#define SDL_THREAD_PRIORITY_LOW 0
Uint64 SDL_GetTicksNS(void); Uint64 SDL_GetTicks(void);
SDL_Thread* SDL_CreateThread(int (*fn)(void*),const char* name,void* arg);
void SDL_WaitThread(SDL_Thread*,int*);void SDL_Delay(Uint32);
bool SDL_SetCurrentThreadPriority(int);const char* SDL_GetError(void);
