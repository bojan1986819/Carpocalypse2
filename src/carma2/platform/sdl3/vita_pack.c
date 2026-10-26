/* Read-only asset packs are repeatedly mounted during drone/font loading.
 * Cache original file bytes, never the mutable mounted copy (ungetc and header
 * fixups can alter that copy). A miss or failed optional allocation retains
 * the normal fread path. No game structures or mount lifetimes change. */
#include "vita_pack.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

enum { PACK_SLOTS = 8, PACK_BUDGET = 4 * 1024 * 1024 };
static struct {
    char path[1024];
    unsigned char* bytes;
    size_t size;
    unsigned stamp;
} packs[PACK_SLOTS];
static size_t cached_bytes;
static unsigned cache_clock, hits, reads;
static uint64_t disk_bytes, saved_bytes;

static void discard(unsigned slot) {
    cached_bytes -= packs[slot].size;
    free(packs[slot].bytes);
    memset(&packs[slot], 0, sizeof(packs[slot]));
}
static unsigned oldest(int occupied_only) {
    unsigned i, slot = PACK_SLOTS;
    for (i = 0; i < PACK_SLOTS; ++i)
        if ((!occupied_only || packs[i].bytes) &&
            (slot == PACK_SLOTS || packs[i].stamp < packs[slot].stamp)) slot = i;
    return slot;
}
void C2VitaPackCacheClear(void) {
    unsigned i;
    for (i = 0; i < PACK_SLOTS; ++i) discard(i);
    cache_clock = hits = reads = 0;
    disk_bytes = saved_bytes = 0;
}
size_t C2VitaReadPack(void* destination, size_t size, FILE* file, const char* path) {
    unsigned i, slot;
    size_t result;
    int eligible = size && size <= PACK_BUDGET && strlen(path) < sizeof(packs[0].path);
    if (eligible) for (i = 0; i < PACK_SLOTS; ++i) {
        if (packs[i].bytes && !strcasecmp(path, packs[i].path)) {
            if (packs[i].size == size) {
                if (fseek(file, (long)size, SEEK_SET)) break;
                memcpy(destination, packs[i].bytes, size);
                packs[i].stamp = ++cache_clock;
                ++hits;
                saved_bytes += size;
                return 1;
            }
            discard(i); /* A changed size must never reuse old bytes. */
        }
    }
    result = fread(destination, size, 1, file);
    ++reads;
    if (result) disk_bytes += size;
    if (!result || !eligible) return result;
    slot = oldest(0);
    discard(slot);
    while (cached_bytes + size > PACK_BUDGET) discard(oldest(1));
    packs[slot].bytes = malloc(size);
    if (!packs[slot].bytes) return result;
    memcpy(packs[slot].bytes, destination, size);
    strcpy(packs[slot].path, path);
    packs[slot].size = size;
    packs[slot].stamp = ++cache_clock;
    cached_bytes += size;
    return result;
}
void C2VitaPackCacheLog(void) {
    extern void C2VitaLog(const char* format, ...);
    C2VitaLog("PACK CACHE hits=%u reads=%u disk_bytes=%llu saved_bytes=%llu resident=%u budget=%u\n",
        hits, reads, (unsigned long long)disk_bytes, (unsigned long long)saved_bytes,
        (unsigned)cached_bytes, (unsigned)PACK_BUDGET);
}
