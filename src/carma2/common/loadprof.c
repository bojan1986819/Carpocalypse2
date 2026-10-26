#include "loadprof.h"
#ifndef CARPOCALYPSE2_MATCHING
#include "platform.h"
#include "carpocalypse2_macros.h"
#include "utility.h"
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__vita__)
#include <psp2/kernel/processmgr.h>
#else
#include <time.h>
#endif
#ifdef __vita__
extern void C2VitaLog(const char* format, ...);
#endif

/* PDGetTotalTime returns zero before PDInitTimer, partway through startup.
 * Use a clock that is valid at the very first load phase instead. */
static tU32 LoadClock(void) {
#ifdef _WIN32
    return (tU32)GetTickCount();
#elif defined(__vita__)
    return (tU32)(sceKernelGetProcessTimeWide() / 1000);
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (tU32)((unsigned long long)now.tv_sec * 1000 + now.tv_nsec / 1000000);
#endif
}

C2LoadCounts gC2LoadCounts;
static struct {
    const char* name;
    tU32 start;
    C2LoadCounts counts;
} scopes[32];
static unsigned depth;

void C2LoadBegin(const char* name) {
    if (depth < CARPOCALYPSE2_ASIZE(scopes)) {
        scopes[depth].name = name;
        scopes[depth].start = LoadClock();
        scopes[depth].counts = gC2LoadCounts;
    }
    depth++;
}

void C2LoadEnd(void) {
    char line[512];
    C2LoadCounts before;
    tU32 now;
    if (depth == 0 || --depth >= CARPOCALYPSE2_ASIZE(scopes)) return;
    now = LoadClock();
    before = scopes[depth].counts;
    snprintf(line, sizeof(line),
        "LOAD phase=%s depth=%u end_ms=%u ms=%u PFfopen=%llu packed=%llu fopen=%llu reads=%llu bytes=%llu total_fopen=%llu total_bytes=%llu total_PFfopen=%llu total_packed=%llu total_reads=%llu",
        scopes[depth].name, depth, now, now - scopes[depth].start,
        gC2LoadCounts.pfopen - before.pfopen, gC2LoadCounts.packed - before.packed,
        gC2LoadCounts.fopen - before.fopen, gC2LoadCounts.reads - before.reads,
        gC2LoadCounts.bytes - before.bytes, gC2LoadCounts.fopen, gC2LoadCounts.bytes,
        gC2LoadCounts.pfopen, gC2LoadCounts.packed, gC2LoadCounts.reads);
    dr_dprintf("%s", line);
#ifdef __vita__
    C2VitaLog("%s\n", line);
#endif
    {
        extern void C2_HOOK_FASTCALL C2CheckRegistriesAt(const char* pTitle);
        C2CheckRegistriesAt(scopes[depth].name);
    }
}

FILE* C2LoadFopen(const char* path, const char* mode) {
    gC2LoadCounts.fopen++;
    return fopen(path, mode);
}
size_t C2LoadFread(void* out, size_t size, size_t count, FILE* file) {
    size_t got = fread(out, size, count, file);
    gC2LoadCounts.reads++;
    gC2LoadCounts.bytes += got * size;
    return got;
}
int C2LoadFgetc(FILE* file) {
    int ch = fgetc(file);
    gC2LoadCounts.reads++;
    if (ch != EOF) gC2LoadCounts.bytes++;
    return ch;
}
char* C2LoadFgets(char* out, int size, FILE* file) {
    char* result = fgets(out, size, file);
    gC2LoadCounts.reads++;
    if (result) gC2LoadCounts.bytes += strlen(result);
    return result;
}
#endif
