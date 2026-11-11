#include "loadprof.h"
#ifndef CARPOCALYPSE2_MATCHING
#include "platform.h"
#include "carpocalypse2_macros.h"
#include "utility.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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
int gC2LoadSites = -1;
static struct { void* site; unsigned long long reads; } sites[256];

unsigned long long C2LoadMicros(void) {
#ifdef _WIN32
    return (unsigned long long)GetTickCount() * 1000ull;
#elif defined(__vita__)
    return sceKernelGetProcessTimeWide();
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (unsigned long long)now.tv_sec * 1000000ull + now.tv_nsec / 1000;
#endif
}

/* test: running totals of named load sub-steps, logged on every 50th note and at each top-level end */
static struct { const char* name; unsigned long long us, n; } notes[48];
static void DumpNotes(const char* pWhen) {
    unsigned i;
    for (i = 0; i < CARPOCALYPSE2_ASIZE(notes) && notes[i].name != NULL; i++) {
#ifdef __vita__
        C2VitaLog("TEST LOADNOTE at %s: %s n=%llu total_ms=%llu\n", pWhen, notes[i].name, notes[i].n, notes[i].us / 1000);
#endif
    }
}
void C2LoadNote(const char* pName, unsigned long long pMicros) {
    unsigned i;
    for (i = 0; i < CARPOCALYPSE2_ASIZE(notes); i++) {
        if (notes[i].name == NULL || strcmp(notes[i].name, pName) == 0) {
            notes[i].name = pName;
            notes[i].us += pMicros;
            if (++notes[i].n % 50 == 0) {
#ifdef __vita__
                C2VitaLog("TEST LOADNOTE %s n=%llu total_ms=%llu\n", pName, notes[i].n, notes[i].us / 1000);
#endif
            }
            return;
        }
    }
}

int C2LoadDetail(void) {
    static int detail = -1;
    if (detail < 0) detail = getenv("C2_TEST_LOADDETAIL") != NULL;
    return detail;
}

void C2LoadSite(void* pSite) {
    unsigned h = ((unsigned)(uintptr_t)pSite >> 1) % CARPOCALYPSE2_ASIZE(sites), n;
    for (n = 0; n < CARPOCALYPSE2_ASIZE(sites); n++, h = (h + 1) % CARPOCALYPSE2_ASIZE(sites)) {
        if (sites[h].site == pSite || sites[h].site == NULL) {
            sites[h].site = pSite;
            sites[h].reads++;
            return;
        }
    }
}

static void DumpSites(const char* pName) {
    unsigned i, k;
    for (k = 0; k < 12; k++) {
        unsigned best = 0;
        for (i = 1; i < CARPOCALYPSE2_ASIZE(sites); i++) {
            if (sites[i].reads > sites[best].reads) best = i;
        }
        if (sites[best].reads == 0) break;
#ifdef __vita__
        C2VitaLog("TEST READSITE %s %p reads=%llu (C2LoadSite at %p)\n", pName, sites[best].site, sites[best].reads, (void*)C2LoadSite);
#endif
        sites[best].reads = 0;
    }
    memset(sites, 0, sizeof(sites));
}
static struct {
    const char* name;
    tU32 start;
    C2LoadCounts counts;
} scopes[32];
static unsigned depth;

void C2LoadBegin(const char* name) {
    if (gC2LoadSites < 0) gC2LoadSites = getenv("C2_TEST_READSITES") != NULL;
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
    if (gC2LoadSites > 0 && (depth == 0 || strcmp(scopes[depth].name, "ExtractColumns") == 0)) DumpSites(scopes[depth].name);
    if (depth == 0 && C2LoadDetail()) DumpNotes(scopes[depth].name);
#ifdef __vita__
    C2VitaLog("%s\n", line);
#endif
    {
        extern void C2_HOOK_FASTCALL C2CheckRegistriesAt(const char* pTitle);
        C2CheckRegistriesAt(scopes[depth].name);
    }
}

FILE* C2LoadFopen(const char* path, const char* mode) {
    static int log = -1;
    unsigned long long start;
    FILE* f;
    gC2LoadCounts.fopen++;
    if (log < 0) log = getenv("C2_TEST_OPENLOG") != NULL;
    if (!log) return fopen(path, mode);
    /* test: what opens and misses cost */
    start = C2LoadMicros();
    f = fopen(path, mode);
    C2LoadNote(f != NULL ? "open-hit" : "open-miss", C2LoadMicros() - start);
#ifdef __vita__
    {
        static int shown;
        if (f == NULL && shown++ < 30) C2VitaLog("TEST OPEN miss %s %llu us\n", path, C2LoadMicros() - start);
    }
#endif
    return f;
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
