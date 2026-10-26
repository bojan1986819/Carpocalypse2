#ifndef C2_VITA_PACK_H
#define C2_VITA_PACK_H
#include <stddef.h>
#include <stdio.h>
size_t C2VitaReadPack(void* destination, size_t size, FILE* file, const char* path);
void C2VitaPackCacheClear(void);
void C2VitaPackCacheLog(void);
#endif
