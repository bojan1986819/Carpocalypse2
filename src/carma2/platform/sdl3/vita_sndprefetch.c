/* Background read of the sound bank's WAV files during startup.
 *
 * The Vita loads every sample up front (C2VitaPreloadSamples), about 240 small WAV files. Opening
 * and reading each one from the memory card takes ~25 ms, over 6 s of the boot in total, while the
 * main thread is busy with CPU work (BRender, fonts) for the first seconds. A worker thread reads
 * the files' bytes into memory from the start of the boot; S3BufferWav takes them from here instead
 * of reading the card. The worker only does file I/O and malloc; every game structure is still
 * built on the main thread. A file the main thread asks for before the worker reached it is simply
 * read by the main thread as before.
 *
 * Most of that time is the ~240 opens, not the 6 MB. The worker therefore also keeps all the files
 * in one cache file (SNDCACHE.BIN: the directory's names, sizes and modification times, then the
 * data in that order). When the directory still matches it, a boot reads that one file in sequence
 * instead; otherwise the files are read one by one as before and the cache is written again. */
#include "vita_sndprefetch.h"

#include <SDL3/SDL.h>
#include <dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define PREFETCH_DIR "ux0:/data/c2port/DATA/SOUND"
#define PREFETCH_MAX_FILES 512
#define PREFETCH_MAX_BYTES (16 * 1024 * 1024)
#define CACHE_PATH "ux0:/data/c2port/SNDCACHE.BIN"
#define CACHE_TEMP_PATH "ux0:/data/c2port/SNDCACHE.TMP"
#define CACHE_MAGIC 0x50533243u /* "C2SP" */
#define CACHE_VERSION 1u
#define CACHE_STAGING_BYTES (1024 * 1024)

enum { kQueued, kReading, kReady, kTaken };

typedef struct {
    char name[64];
    void* data;
    int size;
    int state;
} tPrefetch_entry;

/* one per file in the cache, in data order */
typedef struct {
    char name[64];
    int size;
    unsigned short mtime[6]; /* year, month, day, hour, minute, second */
} tCache_index;

static tPrefetch_entry gEntries[PREFETCH_MAX_FILES];
static tCache_index gListing[PREFETCH_MAX_FILES]; /* what the directory holds now */
static int gCount;
static SDL_Mutex* gLock;
static SDL_Condition* gReady;
static SDL_Thread* gThread;
static char gDirectory[512];
static unsigned long long gStart_ns, gWorker_ns, gBytes;
static int gRead_files, gTaken_files, gMissed_files;
static int gUse_cache, gCache_written;

extern void C2VitaLog(const char* format, ...);
extern const char* carpocalypse2_fix_path_case(const char* pPath);

static int ReadWhole(SceUID pFd, void* pData, int pSize) {
    int done = 0;
    while (done < pSize) {
        int got = sceIoRead(pFd, (char*)pData + done, pSize - done);
        if (got <= 0) return 0;
        done += got;
    }
    return 1;
}

static int WriteWhole(SceUID pFd, const void* pData, int pSize) {
    int done = 0;
    while (done < pSize) {
        int put = sceIoWrite(pFd, (const char*)pData + done, pSize - done);
        if (put <= 0) return 0;
        done += put;
    }
    return 1;
}

/* Whether SNDCACHE.BIN describes exactly the files listed in gListing. */
static int CacheMatches(void) {
    unsigned header[3];
    SceUID fd = sceIoOpen(CACHE_PATH, SCE_O_RDONLY, 0);
    int i, ok;
    tCache_index* index;
    if (fd < 0) return 0;
    ok = ReadWhole(fd, header, sizeof(header)) && header[0] == CACHE_MAGIC && header[1] == CACHE_VERSION
        && header[2] == (unsigned)gCount;
    index = ok ? malloc(gCount * sizeof(tCache_index)) : NULL;
    ok = index != NULL && ReadWhole(fd, index, gCount * (int)sizeof(tCache_index));
    for (i = 0; ok && i < gCount; i++) {
        ok = strcmp(index[i].name, gListing[i].name) == 0 && index[i].size == gListing[i].size
            && memcmp(index[i].mtime, gListing[i].mtime, sizeof(index[i].mtime)) == 0;
    }
    free(index);
    sceIoClose(fd);
    return ok;
}

/* Hands a file's bytes (or none) to gEntries[pIndex]; the main thread may be waiting for it. */
static void Publish(int pIndex, void* pData, int pSize, unsigned long long* pBytes) {
    SDL_LockMutex(gLock);
    if (gEntries[pIndex].state == kReading) {
        gEntries[pIndex].data = pData;
        gEntries[pIndex].size = pData != NULL ? pSize : 0;
        gEntries[pIndex].state = pData != NULL ? kReady : kTaken; /* a failed read falls back to the main thread */
        if (pData != NULL) {
            *pBytes += (unsigned long long)pSize;
            gRead_files++;
        }
    } else {
        free(pData); /* the main thread read it itself */
    }
    SDL_BroadcastCondition(gReady);
    SDL_UnlockMutex(gLock);
}

/* Claims gEntries[pIndex] for the worker; 0 when the main thread already took it over. */
static int Claim(int pIndex, unsigned long long pBytes) {
    int claimed;
    SDL_LockMutex(gLock);
    claimed = gEntries[pIndex].state == kQueued && pBytes <= PREFETCH_MAX_BYTES;
    if (claimed) gEntries[pIndex].state = kReading;
    SDL_UnlockMutex(gLock);
    return claimed;
}

static void ReadFromCache(void) {
    SceUID fd = sceIoOpen(CACHE_PATH, SCE_O_RDONLY, 0);
    unsigned long long bytes = 0;
    int i, ok = fd >= 0
        && sceIoLseek(fd, (SceOff)(3 * sizeof(unsigned) + gCount * sizeof(tCache_index)), SCE_SEEK_SET) >= 0;
    for (i = 0; i < gCount; i++) {
        void* data = NULL;
        int size = gListing[i].size;
        if (!Claim(i, bytes)) {
            if (ok) ok = sceIoLseek(fd, size, SCE_SEEK_CUR) >= 0;
            continue;
        }
        if (ok) {
            data = malloc(size > 0 ? (size_t)size : 1);
            if (data == NULL || !ReadWhole(fd, data, size)) {
                free(data);
                data = NULL;
                ok = 0; /* the rest falls back to the main thread */
            }
        }
        Publish(i, data, size, &bytes);
    }
    if (fd >= 0) sceIoClose(fd);
    gBytes = bytes;
}

static void ReadFilesAndWriteCache(void) {
    unsigned long long bytes = 0;
    char* staging = malloc(CACHE_STAGING_BYTES);
    int staged = 0, i;
    SceUID cache = -1;
    unsigned header[3] = { CACHE_MAGIC, CACHE_VERSION, 0 };

    header[2] = (unsigned)gCount;
    if (staging != NULL) {
        cache = sceIoOpen(CACHE_TEMP_PATH, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0666);
        if (cache >= 0 && (!WriteWhole(cache, header, sizeof(header))
                || !WriteWhole(cache, gListing, gCount * (int)sizeof(tCache_index)))) {
            sceIoClose(cache);
            cache = -1;
        }
    }
    for (i = 0; i < gCount; i++) {
        char path[640];
        SceUID fd;
        void* data = NULL;
        int size = gListing[i].size;
        int mine = Claim(i, bytes);

        if (!mine && cache < 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", gDirectory, gEntries[i].name);
        fd = sceIoOpen(path, SCE_O_RDONLY, 0);
        if (fd >= 0) {
            data = malloc(size > 0 ? (size_t)size : 1);
            if (data != NULL && !ReadWhole(fd, data, size)) {
                free(data);
                data = NULL;
            }
            sceIoClose(fd);
        }
        if (cache >= 0) {
            /* the file in the cache, in listing order (a file that cannot be read spoils the cache) */
            int at = 0;
            while (data != NULL && at < size) {
                int take = size - at < CACHE_STAGING_BYTES - staged ? size - at : CACHE_STAGING_BYTES - staged;
                memcpy(staging + staged, (char*)data + at, (size_t)take);
                staged += take;
                at += take;
                if (staged == CACHE_STAGING_BYTES) {
                    if (!WriteWhole(cache, staging, staged)) data = NULL;
                    staged = 0;
                }
            }
            if (data == NULL) {
                sceIoClose(cache);
                cache = -1;
                sceIoRemove(CACHE_TEMP_PATH);
            }
        }
        if (mine) {
            Publish(i, data, size, &bytes);
        } else {
            free(data);
        }
    }
    if (cache >= 0) {
        int ok = staged == 0 || WriteWhole(cache, staging, staged);
        sceIoClose(cache);
        if (ok) {
            sceIoRemove(CACHE_PATH);
            gCache_written = sceIoRename(CACHE_TEMP_PATH, CACHE_PATH) >= 0;
        } else {
            sceIoRemove(CACHE_TEMP_PATH);
        }
    }
    free(staging);
    gBytes = bytes;
}

static int SDLCALL PrefetchMain(void* unused) {
    (void)unused;
    if (gUse_cache) {
        ReadFromCache();
    } else {
        ReadFilesAndWriteCache();
    }
    SDL_LockMutex(gLock);
    gWorker_ns = SDL_GetTicksNS() - gStart_ns;
    SDL_UnlockMutex(gLock);
    return 0;
}

static int IsWav(const char* pName) {
    size_t n = strlen(pName);
    return n > 4 && strcasecmp(pName + n - 4, ".wav") == 0;
}

void C2VitaSoundPrefetchStart(void) {
    const char* directory;
    DIR* dir;
    struct dirent* entry;

    if (gThread != NULL || gLock != NULL) {
        return;
    }
    directory = carpocalypse2_fix_path_case(PREFETCH_DIR);
    SDL_strlcpy(gDirectory, directory, sizeof(gDirectory));
    dir = opendir(gDirectory);
    if (dir == NULL) {
        C2VitaLog("VITA SOUND prefetch: no %s\n", gDirectory);
        return;
    }
    while ((entry = readdir(dir)) != NULL && gCount < PREFETCH_MAX_FILES) {
        if (IsWav(entry->d_name) && strlen(entry->d_name) < sizeof(gEntries[0].name)
                && entry->d_stat.st_size >= 0 && entry->d_stat.st_size <= PREFETCH_MAX_BYTES) {
            tCache_index* listed = &gListing[gCount];
            SDL_strlcpy(gEntries[gCount].name, entry->d_name, sizeof(gEntries[0].name));
            gEntries[gCount].state = kQueued;
            memset(listed, 0, sizeof(*listed));
            SDL_strlcpy(listed->name, entry->d_name, sizeof(listed->name));
            listed->size = (int)entry->d_stat.st_size;
            listed->mtime[0] = entry->d_stat.st_mtime.year;
            listed->mtime[1] = entry->d_stat.st_mtime.month;
            listed->mtime[2] = entry->d_stat.st_mtime.day;
            listed->mtime[3] = entry->d_stat.st_mtime.hour;
            listed->mtime[4] = entry->d_stat.st_mtime.minute;
            listed->mtime[5] = entry->d_stat.st_mtime.second;
            gCount++;
        }
    }
    closedir(dir);
    gLock = SDL_CreateMutex();
    gReady = SDL_CreateCondition();
    if (gLock == NULL || gReady == NULL) {
        C2VitaLog("VITA SOUND prefetch unavailable: %s\n", SDL_GetError());
        gCount = 0;
        return;
    }
    gUse_cache = CacheMatches();
    gStart_ns = SDL_GetTicksNS();
    gThread = SDL_CreateThread(PrefetchMain, "C2 sound prefetch", NULL);
    if (gThread == NULL) {
        C2VitaLog("VITA SOUND prefetch thread failed: %s\n", SDL_GetError());
        gCount = 0;
        return;
    }
    C2VitaLog("VITA SOUND prefetch started: %d files in %s, %s\n", gCount, gDirectory,
        gUse_cache ? "from SNDCACHE.BIN" : "file by file (writing SNDCACHE.BIN)");
}

int C2VitaSoundPrefetchTake(const char* pPath, void** pData, int* pSize) {
    const char* name;
    const char* slash;
    int i;

    if (gThread == NULL || pPath == NULL) {
        return 0;
    }
    name = pPath;
    for (slash = pPath; *slash != '\0'; slash++) {
        if (*slash == '/' || *slash == '\\') {
            name = slash + 1;
        }
    }
    SDL_LockMutex(gLock);
    for (i = 0; i < gCount; i++) {
        if (strcasecmp(gEntries[i].name, name) == 0) {
            break;
        }
    }
    if (i == gCount) {
        SDL_UnlockMutex(gLock);
        return 0;
    }
    if (gEntries[i].state == kQueued) {
        gEntries[i].state = kTaken; /* the worker skips it; the caller reads the file itself */
        gMissed_files++;
        SDL_UnlockMutex(gLock);
        return 0;
    }
    while (gEntries[i].state == kReading) {
        SDL_WaitCondition(gReady, gLock);
    }
    if (gEntries[i].state != kReady) {
        SDL_UnlockMutex(gLock);
        return 0;
    }
    *pData = gEntries[i].data;
    *pSize = gEntries[i].size;
    gEntries[i].data = NULL;
    gEntries[i].state = kTaken;
    gTaken_files++;
    SDL_UnlockMutex(gLock);
    return 1;
}

void C2VitaSoundPrefetchFinish(void) {
    int i;
    int unused = 0;

    if (gThread == NULL) {
        return;
    }
    SDL_WaitThread(gThread, NULL);
    gThread = NULL;
    for (i = 0; i < gCount; i++) {
        if (gEntries[i].data != NULL) {
            free(gEntries[i].data);
            gEntries[i].data = NULL;
            unused++;
        }
        gEntries[i].state = kTaken;
    }
    C2VitaLog("VITA SOUND prefetch: read=%d bytes=%llu taken=%d main_thread=%d unused=%d worker_ms=%llu%s\n",
        gRead_files, gBytes, gTaken_files, gMissed_files, unused, gWorker_ns / 1000000ull,
        gUse_cache ? " (cache)" : gCache_written ? " (cache written)" : "");
}
