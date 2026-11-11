#include "packfile.h"
#ifndef CARPOCALYPSE2_MATCHING
#include "loadprof.h"
#endif

#include "loading1.h"
#include "utility.h"
#include "errors.h"
#include "sound.h"
#include "platform.h"
#include "carpocalypse2_macros.h"

#include <string.h>
#ifndef CARPOCALYPSE2_MATCHING
#include <limits.h>
#ifdef __vita__
#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <psp2/kernel/threadmgr.h>
#include <stdlib.h>
#include <strings.h>
#endif
/* Profile the port PF path; retail source tokens stay unchanged. */
#define fopen C2LoadFopen
#define fread C2LoadFread
#define fgetc C2LoadFgetc
#define fgets C2LoadFgets
#endif
// GLOBAL: CARMA2_HW 0x00691b40
tTwatVfsMountPoint gTwatVfsMountPoints[5];

// GLOBAL: CARMA2_HW 0x00692070
tTwatVfsFile gTwatVfsFiles[51];

// GLOBAL: CARMA2_HW 0x006923a0
int gDisableTiffConversionStackPos;

// GLOBAL: CARMA2_HW 0x00692068
int gDisableTiffConversionStack[2];

// GLOBAL: CARMA2_HW 0x0068c724
int gDisableTiffConversion;

#ifndef CARPOCALYPSE2_MATCHING
/* A mount and its entries share an unbuffered source. Each entry retains its
 * own logical position/window; all physical reads seek explicitly. Sources
 * outlive unmount until their last entry closes, independent of slot reuse. */
#define PACK_WINDOW_BYTES (64u * 1024u)
typedef struct {
    FILE* stream;
    unsigned references;
} tPackSource;
/* At most five mounts plus fifty entries can retain distinct generations. */
static tPackSource gPackSources[CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints) + CARPOCALYPSE2_ASIZE(gTwatVfsFiles) - 1];
static tPackSource* gPackStreams[CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints)];

static void PackReleaseSource(tPackSource* source) {
    if (source != NULL && --source->references == 0) {
        fclose(source->stream);
        source->stream = NULL;
    }
}
typedef struct {
    tPackSource* source;
    FILE* edits; /* Rare changed-byte ungetc: disk journal preserves retail writes. */
    size_t offset, size, pos, window_pos, window_size;
    const tU8* memory; /* a loose file read whole (gPF_loose_into_memory): the window is all of it */
} tPackEntryStream;
int gPF_loose_into_memory;
static int gPackTrace = -1;
static tPackEntryStream gEntryStreams[CARPOCALYPSE2_ASIZE(gTwatVfsFiles)];

static size_t PackReadAt(tPackEntryStream* entry, size_t pos, void* buffer, size_t bytes) {
    size_t got;
    if (entry->memory != NULL) {
        got = pos < entry->size ? entry->size - pos : 0;
        if (got > bytes) got = bytes;
        memcpy(buffer, entry->memory + pos, got);
    } else {
        if (fseek(entry->source->stream, (long)(entry->offset + pos), SEEK_SET) != 0) return 0;
        got = fread(buffer, 1, bytes, entry->source->stream);
    }
    if (entry->edits != NULL) {
        size_t at;
        int ch;
        rewind(entry->edits);
        while (fread(&at, sizeof(at), 1, entry->edits) == 1 && (ch = fgetc(entry->edits)) != EOF) {
            if (at >= pos && at - pos < got) ((tU8*)buffer)[at - pos] = (tU8)ch;
        }
    }
    return got;
}

static int PackGetByte(size_t slot) {
    tPackEntryStream* entry = &gEntryStreams[slot];
    tTwatVfsFile* file = &gTwatVfsFiles[slot];
    if (entry->pos >= entry->size) { file->error = -1; return EOF; }
    if (entry->pos < entry->window_pos || entry->pos - entry->window_pos >= entry->window_size) {
        size_t bytes = entry->size - entry->pos;
        if (bytes > PACK_WINDOW_BYTES) bytes = PACK_WINDOW_BYTES;
        entry->window_pos = entry->pos;
        entry->window_size = PackReadAt(entry, entry->pos, file->start, bytes);
        if (entry->window_size == 0) { file->error = -1; return EOF; }
    }
    file->error = 0;
    return file->start[entry->pos++ - entry->window_pos];
}

static tTWTVFS OpenPackFileLazy(const char* path) {
    char pack_path[sizeof(tPath_name) + 5];
    FILE* stream;
    tTwatHeader top;
    tTwatHeader* header;
    size_t table_bytes;
    size_t offset;
    long disk_bytes;
    int twt, i;
    tPackSource* source = NULL;

    if (strlen(path) >= sizeof(gTwatVfsMountPoints[0].path)) {
        return -1;
    }
    snprintf(pack_path, sizeof(pack_path), "%s.TWT", path);
    stream = fopen(pack_path, "rb");
#if defined(CARPOCALYPSE2_FIX_BUGS)
    if (stream == NULL) {
        const char* base = strrchr(path, '/');
        const char* backslash = strrchr(path, '\\');
        if (backslash != NULL && (base == NULL || backslash > base)) {
            base = backslash;
        }
        if (base != NULL && base[1] != '\0'
            && snprintf(pack_path, sizeof(pack_path), "%s/%s.TWT", path, base + 1) < sizeof(pack_path)) {
            stream = fopen(pack_path, "rb");
        }
    }
#endif
    if (stream == NULL) {
        return -1;
    }
    setvbuf(stream, NULL, _IONBF, 0);
    for (twt = 0; twt < CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); twt++) {
        if (gTwatVfsMountPoints[twt].header == NULL) {
            break;
        }
    }
    if (twt == CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints)
        || fread(&top, sizeof(top), 1, stream) != 1
        || top.nbFiles < 0 || top.twatFileSize > INT_MAX
        || top.twatFileSize < sizeof(top)
        || (size_t)top.nbFiles > (top.twatFileSize - sizeof(top)) / sizeof(tTwatFileHeader)
        || fseek(stream, 0, SEEK_END) != 0) {
        fclose(stream);
        return -1;
    }
    disk_bytes = ftell(stream);
    if (disk_bytes < 0 || (unsigned long)disk_bytes < top.twatFileSize
        || fseek(stream, sizeof(top), SEEK_SET) != 0) {
        fclose(stream);
        return -1;
    }
    table_bytes = sizeof(top) + (size_t)top.nbFiles * sizeof(tTwatFileHeader);
    header = BrMemAllocate(table_bytes, kMem_packed_file);
    if (header == NULL) {
        fclose(stream);
        return -1;
    }
    memcpy(header, &top, sizeof(top));
    if (fread(header->fileHeaders, sizeof(tTwatFileHeader), top.nbFiles, stream) != top.nbFiles) {
        BrMemFree(header);
        fclose(stream);
        return -1;
    }
    offset = table_bytes;
    for (i = 0; i < header->nbFiles; i++) {
        tTwatFileHeader* entry = &header->fileHeaders[i];
        if (offset > top.twatFileSize || entry->fileSize > top.twatFileSize - offset
            || memchr(entry->filename, '\0', sizeof(entry->filename)) == NULL) {
            BrMemFree(header);
            fclose(stream);
            return -1;
        }
        entry->data_offset = (tU32)offset;
        offset += ((size_t)entry->fileSize + 3u) & ~(size_t)3u;
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gPackSources); i++) {
        if (gPackSources[i].stream == NULL) { source = &gPackSources[i]; break; }
    }
    if (source == NULL) { BrMemFree(header); fclose(stream); return -1; }
    source->stream = stream;
    source->references = 1; /* mount ownership */
    strcpy(gTwatVfsMountPoints[twt].path, path);
    gPackStreams[twt] = source;
    gTwatVfsMountPoints[twt].data = NULL; /* Payload is never resident at mount time. */
    gTwatVfsMountPoints[twt].header = header;
    dr_dprintf("PACK OPEN %s entries=%d bytes=%u lazy_header_bytes=%u payload_bytes_read=0",
        pack_path, header->nbFiles, header->twatFileSize, (unsigned int)table_bytes);
    return twt;
}
#endif

// FUNCTION: CARMA2_HW 0x004b4570
void C2_HOOK_FASTCALL InitPackFiles(void) {
    int i;

#ifdef __vita__
    C2VitaPackCacheClear();
#endif

    C2_HOOK_BUG_ON(sizeof(tTwatVfsMountPoint) != 264);

    for (i = 1; i < (int)CARPOCALYPSE2_ASIZE(gTwatVfsFiles); i++) {
        gTwatVfsFiles[i].start = NULL;
    }
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); i++) {
        gTwatVfsMountPoints[i].header = NULL;
    }

    gDisableTiffConversionStackPos = 0;
}

// FUNCTION: CARMA2_HW 0x004b45b0

tTWTVFS C2_HOOK_FASTCALL OpenPackFile(const char* path) {
#ifndef CARPOCALYPSE2_MATCHING
    CARPOCALYPSE2_BUG_ON(sizeof(tTwatFileHeader) != 56);
    return OpenPackFileLazy(path);
#else
    tPath_name twatFilePath;
    FILE* f;
    tTWTVFS twt;
    tU32 fileSize;
    tU8* fileData;
    int i;

    // file header must be 56 bytes for compatibility with .TWT files
    CARPOCALYPSE2_BUG_ON(sizeof(tTwatFileHeader) != 56);

    strcpy(twatFilePath, path);
    strcat(twatFilePath, ".TWT");

    f = fopen(twatFilePath, "rb");
#if defined(CARPOCALYPSE2_FIX_BUGS)
    if (f == NULL) {
        /* retail expects "<path>.TWT" (e.g. <root>/DATA.TWT); also accept
         * the pack inside its own directory (<root>/DATA/DATA.TWT). */
        const char* base = strrchr(path, '/');

        if (base != NULL && base[1] != '\0') {
            snprintf(twatFilePath, sizeof(twatFilePath), "%s/%s.TWT", path, base + 1);
            f = fopen(twatFilePath, "rb");
        }
    }
#endif
    if (f != NULL) {
        for (twt = 0; ; twt++) {
            if (twt >= (int)CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints)) {
                fclose(f);
                return -1;
            }
            if (gTwatVfsMountPoints[twt].header == NULL) {
                break;
            }
        }
        strcpy(gTwatVfsMountPoints[twt].path, path);
        fileSize = ReadU32(f);
        rewind(f);

#ifdef __vita__
        { extern void C2VitaLog(const char* format, ...);
          C2VitaLog("PACK LOAD %u bytes: %s\n", (unsigned)fileSize, twatFilePath); }
#endif
        gTwatVfsMountPoints[twt].header = BrMemAllocate(fileSize, kMem_packed_file);
#ifdef __vita__
        C2VitaReadPack(gTwatVfsMountPoints[twt].header, fileSize, f, twatFilePath);
#else
        fread(gTwatVfsMountPoints[twt].header, fileSize, 1, f);
#endif
        SplungeSomeData(gTwatVfsMountPoints[twt].header, fileSize);
        gTwatVfsMountPoints[twt].data = (tU8*)gTwatVfsMountPoints[twt].header;
        fclose(f);

        fileData = (tU8*)&gTwatVfsMountPoints[twt].header->fileHeaders[gTwatVfsMountPoints[twt].header->nbFiles];
        for (i = 0; i < gTwatVfsMountPoints[twt].header->nbFiles; i++) {
            gTwatVfsMountPoints[twt].header->fileHeaders[i].data_offset = (tU32)(fileData - (tU8*)gTwatVfsMountPoints[twt].header);
            fileData += (gTwatVfsMountPoints[twt].header->fileHeaders[i].fileSize + 3u) & ~3u;
        }
        return twt;
    }
    return -1;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4730
void C2_HOOK_FASTCALL ClosePackFile(tTWTVFS twt) {

#ifndef CARPOCALYPSE2_MATCHING
    if (twt < 0 || twt >= CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints)) {
        return;
    }
    if (gPackStreams[twt] != NULL) {
        PackReleaseSource(gPackStreams[twt]);
        gPackStreams[twt] = NULL;
    }
    gTwatVfsMountPoints[twt].data = NULL;
#endif
    if (twt >= 0) {
        BrMemFree(gTwatVfsMountPoints[twt].header);
        gTwatVfsMountPoints[twt].header = NULL;
    }
}

// FUNCTION: CARMA2_HW 0x004b4760
int C2_HOOK_FASTCALL PFfclose(FILE* pFile) {

    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return fclose(pFile);
    } else {
#ifndef CARPOCALYPSE2_MATCHING
        tPackEntryStream* entry = &gEntryStreams[(uintptr_t)pFile];
        PackReleaseSource(entry->source);
        if (entry->edits != NULL) fclose(entry->edits);
        memset(entry, 0, sizeof(*entry));
        BrMemFree(gTwatVfsFiles[(uintptr_t)pFile].start);
#endif
        gTwatVfsFiles[(uintptr_t)pFile].start = NULL;
#ifndef CARPOCALYPSE2_MATCHING
        gTwatVfsFiles[(uintptr_t)pFile].pos = NULL;
        gTwatVfsFiles[(uintptr_t)pFile].end = NULL;
        gTwatVfsFiles[(uintptr_t)pFile].error = 0;
        return 0;
#endif
    }
}

// Windows gets neither this fallback nor the dirent.h it is built on: MSVC
// has no dirent.h, and its filesystem is case-insensitive, so the exact-name
// fopen below already succeeds on retail case.
#if defined(CARPOCALYPSE2_FIX_BUGS) && !defined(_WIN32) && !defined(__vita__)
#include <dirent.h>

// Case-insensitive filesystem fallback helper. The retail game assumed a
// case-insensitive filesystem (Windows) and hard-codes paths such as
// ".../CARS/64X48X8/EAGLE3.TXT" while the data ships with lower-case names
// (".../CARS/64x48x8/eagle3.txt"). When an exact open fails on a case-sensitive
// host, resolve the path by matching every component case-insensitively.
static FILE* CI_FopenCaseInsensitive(const char* pPath, const char* mode) {
    char resolved[256];
    char comp[256];
    const char* p;
    size_t len;
    size_t comp_len;

    if (pPath == NULL || mode == NULL || pPath[0] == '\0') {
        return NULL;
    }

    resolved[0] = '\0';

    // Split the path into components and resolve them one directory at a time,
    // matching each component case-insensitively against the parent directory.
    p = pPath;
    while (p[0] == '/') {
        p++;
    }

    for (;;) {
        comp_len = strcspn(p, "/");
        if (comp_len >= sizeof(comp)) {
            return NULL;
        }
        memcpy(comp, p, comp_len);
        comp[comp_len] = '\0';

        {
            DIR* d = opendir(resolved[0] == '\0' ? "/" : resolved);
            struct dirent* e;
            int found = 0;
            if (d == NULL) {
                return NULL;
            }
            while ((e = readdir(d)) != NULL) {
                if (DRStricmp(e->d_name, comp) == 0) {
                    len = strlen(resolved);
                    if (len > 0 && resolved[len - 1] != '/') {
                        strcat(resolved, "/");
                    } else if (len == 0) {
                        strcat(resolved, "/");
                    }
                    strcat(resolved, e->d_name);
                    found = 1;
                    break;
                }
            }
            closedir(d);
            if (!found) {
                return NULL;
            }
        }

        p += comp_len;
        if (p[0] == '\0' || p[1] == '\0') {
            break;
        }
        if (p[0] == '/') {
            p++;
        }
    }

    return fopen(resolved, mode);
}
#endif

#ifdef __vita__
/* Session cache of whole loose files read through gPF_loose_into_memory (the ped animations: 71
 * files of ~60 KB a race, each open a ~10 ms directory lookup on the card). A background thread
 * fills it from a directory after startup, while the menus run; the main thread waits for a file
 * the thread is reading and reads anything else itself, then keeps a copy. */
#define FILE_CACHE_ENTRIES 192
#define FILE_CACHE_BUDGET (12u * 1024u * 1024u)
typedef struct {
    char path[256];
    tU8* data;
    unsigned size;
    volatile int state; /* 0 free, 1 being read, 2 ready, 3 failed */
} tFileCacheEntry;
static tFileCacheEntry gFile_cache[FILE_CACHE_ENTRIES];
static unsigned gFile_cache_bytes;
static SceKernelLwMutexWork gFile_cache_mutex;
static int gFile_cache_ready;
static char gFile_prefetch_dir[256];

static void FileCacheInit(void) {
    if (!gFile_cache_ready && sceKernelCreateLwMutex(&gFile_cache_mutex, "C2 file cache", 0, 0, NULL) >= 0) {
        gFile_cache_ready = 1;
    }
}

/* The entry for pPath, or a new one marked being read (*pNew set); NULL when full. */
static tFileCacheEntry* FileCacheEntry(const char* pPath, int* pNew) {
    tFileCacheEntry* found = NULL;
    tFileCacheEntry* free_entry = NULL;
    int i;
    *pNew = 0;
    if (!gFile_cache_ready || strlen(pPath) >= sizeof(gFile_cache[0].path)) return NULL;
    sceKernelLockLwMutex(&gFile_cache_mutex, 1, NULL);
    for (i = 0; i < FILE_CACHE_ENTRIES; i++) {
        if (gFile_cache[i].state == 0) {
            if (free_entry == NULL) free_entry = &gFile_cache[i];
        } else if (strcasecmp(gFile_cache[i].path, pPath) == 0) {
            found = &gFile_cache[i];
            break;
        }
    }
    if (found == NULL && free_entry != NULL) {
        strcpy(free_entry->path, pPath);
        free_entry->state = 1;
        found = free_entry;
        *pNew = 1;
    }
    sceKernelUnlockLwMutex(&gFile_cache_mutex, 1);
    return found;
}

/* Keeps pData (malloc'ed, pSize bytes) for pEntry within the budget, or marks the entry failed. */
static void FileCacheStore(tFileCacheEntry* pEntry, tU8* pData, unsigned pSize) {
    sceKernelLockLwMutex(&gFile_cache_mutex, 1, NULL);
    if (pData != NULL && gFile_cache_bytes + pSize <= FILE_CACHE_BUDGET) {
        gFile_cache_bytes += pSize;
        pEntry->data = pData;
        pEntry->size = pSize;
        pEntry->state = 2;
    } else {
        free(pData);
        pEntry->state = 3;
    }
    sceKernelUnlockLwMutex(&gFile_cache_mutex, 1);
}

/* The prefetch reads the directory from one cache file (CACHE_<parent>_<dir>.BIN in the game folder:
 * names, sizes and modification times, then the data in that order) while the directory still
 * matches it: one open instead of one per file. Otherwise it reads the files and writes the cache. */
#define DIR_CACHE_MAX 256
#define DIR_CACHE_MAGIC 0x43443243u /* "C2DC" */
typedef struct {
    char name[64];
    int size;
    unsigned short mtime[6]; /* year, month, day, hour, minute, second */
} tDirCacheIndex;
static tDirCacheIndex gDir_listing[DIR_CACHE_MAX];

static int IoReadAll(SceUID pFd, void* pData, int pSize) {
    int done = 0;
    while (done < pSize) {
        int got = sceIoRead(pFd, (char*)pData + done, pSize - done);
        if (got <= 0) return 0;
        done += got;
    }
    return 1;
}

static int IoWriteAll(SceUID pFd, const void* pData, int pSize) {
    int done = 0;
    while (done < pSize) {
        int put = sceIoWrite(pFd, (const char*)pData + done, pSize - done);
        if (put <= 0) return 0;
        done += put;
    }
    return 1;
}

/* "ux0:/data/c2port/CACHE_PEDS_MOVES.<pExtension>" for ".../PEDS/MOVES". */
static void DirCachePath(char* pOut, size_t pSize, const char* pExtension) {
    char tail[128];
    const char* last = strrchr(gFile_prefetch_dir, '/');
    const char* parent = last;
    char* c;
    while (parent != NULL && parent > gFile_prefetch_dir && parent[-1] != '/') parent--;
    snprintf(tail, sizeof(tail), "%s", parent != NULL ? parent : gFile_prefetch_dir);
    for (c = tail; *c != '\0'; c++) {
        if (*c == '/' || *c == ':' || *c == '\\') *c = '_';
    }
    snprintf(pOut, pSize, "ux0:/data/c2port/CACHE_%s.%s", tail, pExtension);
}

static int FilePrefetchThread(SceSize args, void* argp) {
    SceUID dir, cache;
    SceIoDirent entry;
    char cache_path[256], temp_path[256];
    unsigned header[3];
    int count = 0, i, use_cache = 0;
    (void)args;
    (void)argp;
    dir = sceIoDopen(gFile_prefetch_dir);
    if (dir < 0) return 0;
    memset(&entry, 0, sizeof(entry));
    while (sceIoDread(dir, &entry) > 0 && count < DIR_CACHE_MAX) {
        tDirCacheIndex* listed = &gDir_listing[count];
        if (SCE_S_ISDIR(entry.d_stat.st_mode) || strlen(entry.d_name) >= sizeof(listed->name)
                || entry.d_stat.st_size < 0 || entry.d_stat.st_size > 16 * 1024 * 1024) {
            continue;
        }
        memset(listed, 0, sizeof(*listed));
        strcpy(listed->name, entry.d_name);
        listed->size = (int)entry.d_stat.st_size;
        listed->mtime[0] = entry.d_stat.st_mtime.year;
        listed->mtime[1] = entry.d_stat.st_mtime.month;
        listed->mtime[2] = entry.d_stat.st_mtime.day;
        listed->mtime[3] = entry.d_stat.st_mtime.hour;
        listed->mtime[4] = entry.d_stat.st_mtime.minute;
        listed->mtime[5] = entry.d_stat.st_mtime.second;
        count++;
    }
    sceIoDclose(dir);
    DirCachePath(cache_path, sizeof(cache_path), "BIN");
    DirCachePath(temp_path, sizeof(temp_path), "TMP");

    cache = sceIoOpen(cache_path, SCE_O_RDONLY, 0);
    if (cache >= 0 && IoReadAll(cache, header, sizeof(header)) && header[0] == DIR_CACHE_MAGIC && header[1] == 1
            && header[2] == (unsigned)count) {
        tDirCacheIndex* index = malloc(count * sizeof(tDirCacheIndex) + 1);
        use_cache = index != NULL && IoReadAll(cache, index, count * (int)sizeof(tDirCacheIndex));
        for (i = 0; use_cache && i < count; i++) {
            use_cache = memcmp(&index[i], &gDir_listing[i], sizeof(tDirCacheIndex)) == 0;
        }
        free(index);
    }
    if (use_cache) {
        int ok = 1;
        for (i = 0; i < count; i++) {
            char path[256];
            tFileCacheEntry* cached;
            int is_new = 0;
            tU8* data = NULL;
            int size = gDir_listing[i].size;
            snprintf(path, sizeof(path), "%s/%s", gFile_prefetch_dir, gDir_listing[i].name);
            cached = FileCacheEntry(path, &is_new);
            if (cached == NULL || !is_new) {
                if (ok) ok = sceIoLseek(cache, size, SCE_SEEK_CUR) >= 0;
                continue;
            }
            if (ok) {
                data = malloc(size > 0 ? (size_t)size : 1);
                if (data != NULL && !IoReadAll(cache, data, size)) {
                    free(data);
                    data = NULL;
                    ok = 0; /* the main thread reads the rest itself */
                }
            }
            FileCacheStore(cached, data, data != NULL ? (unsigned)size : 0);
        }
        sceIoClose(cache);
        return 0;
    }
    if (cache >= 0) sceIoClose(cache);

    {
        char* staging = malloc(1024 * 1024);
        int staged = 0;
        SceUID out = staging != NULL ? sceIoOpen(temp_path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0666) : -1;
        header[0] = DIR_CACHE_MAGIC;
        header[1] = 1;
        header[2] = (unsigned)count;
        if (out >= 0 && (!IoWriteAll(out, header, sizeof(header))
                || !IoWriteAll(out, gDir_listing, count * (int)sizeof(tDirCacheIndex)))) {
            sceIoClose(out);
            out = -1;
            sceIoRemove(temp_path);
        }
        for (i = 0; i < count; i++) {
            char path[256];
            tFileCacheEntry* cached;
            int is_new = 0;
            SceUID fd;
            tU8* data = NULL;
            int size = gDir_listing[i].size;
            snprintf(path, sizeof(path), "%s/%s", gFile_prefetch_dir, gDir_listing[i].name);
            cached = FileCacheEntry(path, &is_new);
            if (!is_new && out < 0) {
                continue;
            }
            fd = sceIoOpen(path, SCE_O_RDONLY, 0);
            if (fd >= 0) {
                data = malloc(size > 0 ? (size_t)size : 1);
                if (data != NULL && !IoReadAll(fd, data, size)) {
                    free(data);
                    data = NULL;
                }
                sceIoClose(fd);
            }
            if (out >= 0) {
                int at = 0;
                while (data != NULL && at < size) {
                    int take = size - at < 1024 * 1024 - staged ? size - at : 1024 * 1024 - staged;
                    memcpy(staging + staged, data + at, (size_t)take);
                    staged += take;
                    at += take;
                    if (staged == 1024 * 1024) {
                        if (!IoWriteAll(out, staging, staged)) at = -1;
                        staged = 0;
                    }
                    if (at < 0) break;
                }
                if (data == NULL || at < 0) {
                    sceIoClose(out);
                    out = -1;
                    sceIoRemove(temp_path);
                }
            }
            if (cached != NULL && is_new) {
                FileCacheStore(cached, data, data != NULL ? (unsigned)size : 0);
            } else {
                free(data);
            }
        }
        if (out >= 0) {
            int ok = staged == 0 || IoWriteAll(out, staging, staged);
            sceIoClose(out);
            if (ok) {
                sceIoRemove(cache_path);
                sceIoRename(temp_path, cache_path);
            } else {
                sceIoRemove(temp_path);
            }
        }
        free(staging);
    }
    return 0;
}

void PFPrefetchDirectory(const char* pDir) {
    SceUID thread;
    FileCacheInit();
    if (!gFile_cache_ready || gFile_prefetch_dir[0] != '\0' || strlen(pDir) >= sizeof(gFile_prefetch_dir)) return;
    strcpy(gFile_prefetch_dir, pDir);
    thread = sceKernelCreateThread("C2 file prefetch", FilePrefetchThread, 0x10000100, 0x4000, 0, 0, NULL);
    if (thread >= 0) sceKernelStartThread(thread, 0, NULL);
}

/* A cached copy of pPath in BrMem (the PF slot owns it), or NULL with *pEntry set when the caller
 * should read the file and hand it to FileCacheStore. */
static tU8* FileCacheLookup(const char* pPath, unsigned* pSize, tFileCacheEntry** pEntry) {
    tFileCacheEntry* cached;
    int is_new;
    tU8* copy;
    *pEntry = NULL;
    FileCacheInit();
    cached = FileCacheEntry(pPath, &is_new);
    if (cached == NULL) return NULL;
    if (is_new) {
        *pEntry = cached;
        return NULL;
    }
    while (cached->state == 1) sceKernelDelayThread(1000); /* the prefetch thread is reading it */
    if (cached->state != 2) return NULL;
    copy = BrMemAllocate(cached->size > 0 ? cached->size : 1, kMem_packed_file);
    if (copy != NULL) memcpy(copy, cached->data, cached->size);
    *pSize = cached->size;
    return copy;
}
#endif

#ifndef CARPOCALYPSE2_MATCHING
/* port: whether PFfopen would find pPath in a mounted pack (the same match, no slot taken) */
int PFIsInMountedPack(const char* pPath) {
    int twt, i;
    for (twt = 0; twt < CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); twt++) {
        tTwatVfsMountPoint* mount_point = &gTwatVfsMountPoints[twt];
        size_t twt_path_len = strlen(mount_point->path);
        if (mount_point->header == NULL || strncmp(pPath, mount_point->path, twt_path_len) != 0
            || (pPath[twt_path_len] != '/' && pPath[twt_path_len] != '\\')) {
            continue;
        }
        for (i = 0; i < mount_point->header->nbFiles; i++) {
            if (DRStricmp(mount_point->header->fileHeaders[i].filename, pPath + twt_path_len + 1) == 0) {
                return 1;
            }
        }
    }
    return 0;
}
#endif

// C2PORT-NONMATCHING: best 60.42% | difftest PASS 2000 trials (spec difftest/specs/004b4780_PFfopen.json) | reason VC5 register allocation and native VFS pointer layout
// FUNCTION: CARMA2_HW 0x004b4780
FILE* C2_HOOK_FASTCALL PFfopen(const char* pPath, const char* mode) {
    tTwatVfsMountPoint* mount_point;
#ifdef CARPOCALYPSE2_MATCHING
    tU32* mount_data;
#endif
    int i;
    int file_index;
    size_t twt_path_len;

#ifndef CARPOCALYPSE2_MATCHING
    int twt;
    gC2LoadCounts.pfopen++;
    for (twt = 0; twt < CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); twt++) {
        mount_point = &gTwatVfsMountPoints[twt];
        twt_path_len = strlen(mount_point->path);
        if (mount_point->header == NULL || strncmp(pPath, mount_point->path, twt_path_len) != 0
            || (pPath[twt_path_len] != '/' && pPath[twt_path_len] != '\\')) {
            continue;
        }
        for (i = 0; i < mount_point->header->nbFiles; i++) {
            tTwatFileHeader* entry = &mount_point->header->fileHeaders[i];
            if (DRStricmp(entry->filename, pPath + twt_path_len + 1) == 0) {
                for (file_index = 1; file_index < CARPOCALYPSE2_ASIZE(gTwatVfsFiles); file_index++) {
                    tU8* data;
                    if (gTwatVfsFiles[file_index].start != NULL) {
                        continue;
                    }
                    tPackEntryStream* state = &gEntryStreams[file_index];
                    data = BrMemAllocate(PACK_WINDOW_BYTES, kMem_packed_file);
                    if (data == NULL) return NULL;
                    memset(state, 0, sizeof(*state));
                    gC2LoadCounts.packed++;
                    state->source = gPackStreams[twt];
                    state->source->references++;
                    state->offset = entry->data_offset;
                    state->size = entry->fileSize;
                    gTwatVfsFiles[file_index].start = data;
                    gTwatVfsFiles[file_index].pos = NULL;
                    gTwatVfsFiles[file_index].end = NULL;
                    gTwatVfsFiles[file_index].error = 0;
                    if (gPackTrace < 0) gPackTrace = getenv("C2_PACK_TRACE") != NULL;
                    if (gPackTrace) dr_dprintf("PACK ENTRY %s/%s bytes=%u allocation=%u window=%u",
                        mount_point->path, entry->filename, entry->fileSize,
                        (unsigned int)PACK_WINDOW_BYTES, (unsigned int)PACK_WINDOW_BYTES);
                    return (FILE*)(uintptr_t)file_index;
                }
                return NULL;
            }
        }
    }
#else
    for (mount_data = (tU32*)&gTwatVfsMountPoints[0].data; (intptr_t)mount_data < (intptr_t)&gTwatVfsMountPoints[CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints)].data; mount_data += sizeof(tTwatVfsMountPoint) / sizeof(tU32)) {
        mount_point = (tTwatVfsMountPoint*)((tU8*)mount_data - offsetof(tTwatVfsMountPoint, data));
        if (mount_point->header != NULL && strstr(pPath, mount_point->path) == pPath) {
            twt_path_len = strlen(mount_point->path);
            for (i = 0; i < mount_point->header->nbFiles; i++) {
                if (DRStricmp(mount_point->header->fileHeaders[i].filename, &pPath[twt_path_len + 1]) == 0) {
                    tU8* data = mount_point->data + mount_point->header->fileHeaders[i].data_offset;
                    for (file_index = 1; file_index < (int)CARPOCALYPSE2_ASIZE(gTwatVfsFiles); file_index++) {
                        if (gTwatVfsFiles[file_index].start != NULL) {
                            continue;
                        }
                        gTwatVfsFiles[file_index].start = data;
                        gTwatVfsFiles[file_index].pos = data;
                        gTwatVfsFiles[file_index].end = data + mount_point->header->fileHeaders[i].fileSize;
                        gTwatVfsFiles[file_index].error = 0;
                        return (FILE*)(file_index);
                    }
                    return NULL;
                }
            }
        }
    }
#endif
#ifdef __vita__
    /* A file read whole: one open (each open is a directory lookup on the card, ~3 ms) and one read;
     * any failure takes the stdio path below. */
    if (gPF_loose_into_memory && strchr(mode, 'r') != NULL && strchr(mode, '+') == NULL) {
        tFileCacheEntry* fill;
        unsigned cached_size = 0;
        tU8* cached = FileCacheLookup(pPath, &cached_size, &fill);
        SceUID fd = cached != NULL ? -1 : sceIoOpen(pPath, SCE_O_RDONLY, 0);
        if (cached != NULL) {
            for (file_index = 1; file_index < CARPOCALYPSE2_ASIZE(gTwatVfsFiles); file_index++) {
                tPackEntryStream* state = &gEntryStreams[file_index];
                if (gTwatVfsFiles[file_index].start != NULL) {
                    continue;
                }
                memset(state, 0, sizeof(*state));
                state->memory = cached;
                state->size = cached_size;
                state->window_size = cached_size;
                gTwatVfsFiles[file_index].start = cached;
                gTwatVfsFiles[file_index].pos = NULL;
                gTwatVfsFiles[file_index].end = NULL;
                gTwatVfsFiles[file_index].error = 0;
                return (FILE*)(uintptr_t)file_index;
            }
            BrMemFree(cached);
        }
        if (fd < 0 && fill != NULL) {
            FileCacheStore(fill, NULL, 0);
            fill = NULL;
        }
        if (fd >= 0) {
            SceOff length = sceIoLseek(fd, 0, SCE_SEEK_END);
            if (length >= 0 && length <= 16 * 1024 * 1024 && sceIoLseek(fd, 0, SCE_SEEK_SET) == 0) {
                for (file_index = 1; file_index < CARPOCALYPSE2_ASIZE(gTwatVfsFiles); file_index++) {
                    tPackEntryStream* state = &gEntryStreams[file_index];
                    tU8* data;
                    if (gTwatVfsFiles[file_index].start != NULL) {
                        continue;
                    }
                    data = BrMemAllocate(length > 0 ? (br_size_t)length : 1, kMem_packed_file);
                    if (data == NULL) {
                        break;
                    }
                    if (sceIoRead(fd, data, (SceSize)length) != (int)length) {
                        BrMemFree(data);
                        break;
                    }
                    sceIoClose(fd);
                    if (fill != NULL) {
                        tU8* keep = malloc(length > 0 ? (size_t)length : 1);
                        if (keep != NULL) memcpy(keep, data, (size_t)length);
                        FileCacheStore(fill, keep, (unsigned)length);
                        fill = NULL;
                    }
                    memset(state, 0, sizeof(*state));
                    state->memory = data;
                    state->size = (size_t)length;
                    state->window_size = (size_t)length;
                    gTwatVfsFiles[file_index].start = data;
                    gTwatVfsFiles[file_index].pos = NULL;
                    gTwatVfsFiles[file_index].end = NULL;
                    gTwatVfsFiles[file_index].error = 0;
                    return (FILE*)(uintptr_t)file_index;
                }
            }
            sceIoClose(fd);
        }
        if (fill != NULL) {
            FileCacheStore(fill, NULL, 0);
        }
    }
#endif
    {
        FILE* disk = fopen(pPath, mode);
#if defined(CARPOCALYPSE2_FIX_BUGS) && !defined(_WIN32) && !defined(__vita__)
        if (disk == NULL) {
            disk = CI_FopenCaseInsensitive(pPath, mode);
        }
#endif
#ifndef CARPOCALYPSE2_MATCHING
        /* A reader that seeks a lot (the ped animations: once per bone) reads the whole file in one
         * go instead of refilling the stdio buffer after every seek. Text mode is the same bytes
         * outside Windows. */
        if (disk != NULL && gPF_loose_into_memory && strchr(mode, 'r') != NULL && strchr(mode, '+') == NULL
#ifdef _WIN32
                && strchr(mode, 't') == NULL
#endif
                ) {
            long length = fseek(disk, 0, SEEK_END) == 0 ? ftell(disk) : -1;
            for (file_index = 1; length >= 0 && length <= 16 * 1024 * 1024 && file_index < CARPOCALYPSE2_ASIZE(gTwatVfsFiles); file_index++) {
                tPackEntryStream* state = &gEntryStreams[file_index];
                tU8* data;
                if (gTwatVfsFiles[file_index].start != NULL) {
                    continue;
                }
                data = BrMemAllocate(length > 0 ? (br_size_t)length : 1, kMem_packed_file);
#ifdef __vita__
                /* one system read: newlib's fread splits it into stdio buffer sized ones */
                if (data != NULL) {
                    SceUID fd = sceIoOpen(pPath, SCE_O_RDONLY, 0);
                    int got = fd >= 0 ? sceIoRead(fd, data, (SceSize)length) : -1;
                    if (fd >= 0) sceIoClose(fd);
                    if (got == (int)length) {
                        goto loose_read;
                    }
                }
#endif
                if (data == NULL || fseek(disk, 0, SEEK_SET) != 0 || fread(data, 1, (size_t)length, disk) != (size_t)length) {
                    if (data != NULL) BrMemFree(data);
                    break;
                }
#ifdef __vita__
            loose_read:
#endif
                fclose(disk);
                memset(state, 0, sizeof(*state));
                state->memory = data;
                state->size = (size_t)length;
                state->window_size = (size_t)length;
                gTwatVfsFiles[file_index].start = data;
                gTwatVfsFiles[file_index].pos = NULL;
                gTwatVfsFiles[file_index].end = NULL;
                gTwatVfsFiles[file_index].error = 0;
                return (FILE*)(uintptr_t)file_index;
            }
            if (fseek(disk, 0, SEEK_SET) != 0) {
                fclose(disk);
                return NULL;
            }
        }
#endif
        /* A 64 KB stdio buffer once made the race load 7 s -> 21 s (00.57-00.59): the ped animation
         * reader seeks once per bone and every fseek drops the buffer. Those files are now read whole
         * (gPF_loose_into_memory); other files get a 16 KB buffer instead of newlib's 1 KB, each
         * refill being a card read with a fixed cost. */
#ifdef __vita__
        if (disk != NULL && strchr(mode, 'r') != NULL && strchr(mode, '+') == NULL) {
            setvbuf(disk, NULL, _IOFBF, 16 * 1024);
        }
#endif
        return disk;
    }
}

// FUNCTION: CARMA2_HW 0x004b4880
int C2_NAKED C2_HOOK_FASTCALL PFfgetc(FILE* pFile) {
#ifdef CARPOCALYPSE2_MATCHING
    __asm {
        cmp ecx, 0x33
        jb pffgetc_small
        push ecx
        call fgetc
        add esp, 4
        ret

    pffgetc_small:
        shl ecx, 4
        mov edx, dword ptr [ecx + gTwatVfsFiles + 4]
        mov eax, dword ptr [ecx + gTwatVfsFiles + 8]
        lea ecx, [ecx + gTwatVfsFiles]
        cmp edx, eax
        jb pffgetc_read
        or eax, 0xffffffff
        mov dword ptr [ecx + 0xc], eax
        ret

    pffgetc_read:
        mov dword ptr [ecx + 0xc], 0
        movsx eax, byte ptr [edx]
        inc edx
        mov dword ptr [ecx + 4], edx
        ret
    }
#else
    if (gC2LoadSites > 0) C2LoadSite(C2_LOAD_CALLER());
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return fgetc(pFile); }
    return PackGetByte((uintptr_t)pFile);
#endif
}

// PFgetc

// FUNCTION: CARMA2_HW 0x004b4900
char* C2_HOOK_FASTCALL PFfgets(char* buffer, br_size_t size, FILE* pFile) {
#ifndef CARPOCALYPSE2_MATCHING
    if (gC2LoadSites > 0) C2LoadSite(C2_LOAD_CALLER());
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return fgets(buffer, size, pFile); }
    size_t at = 0;
    int ch;
    if (size == 0 || PFfeof(pFile)) { gTwatVfsFiles[(uintptr_t)pFile].error = -1; return NULL; }
    while (at + 1 < size && !PFfeof(pFile)) {
        ch = PackGetByte((uintptr_t)pFile);
        if (ch == EOF) { buffer[at] = 0; return at ? buffer : NULL; }
        buffer[at++] = (char)ch;
        if (ch == '\n') break;
    }
    buffer[at] = 0;
    gTwatVfsFiles[(uintptr_t)pFile].error = 0;
    return buffer;
#else

    tTwatVfsFile* twtFile;
    int c;
    char* writePtr;
    int i;

#ifdef CARPOCALYPSE2_MATCHING
    if ((intptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#else
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#endif
        return fgets(buffer, size, pFile);
    }
    twtFile = &gTwatVfsFiles[(uintptr_t)pFile];
    writePtr = buffer;
#ifndef CARPOCALYPSE2_MATCHING
    /* The retail pre-increment could read the following pack entry at EOF. */
    if (size == 0 || twtFile->pos >= twtFile->end) {
        twtFile->error = -1;
        return NULL;
    }
#endif
    for (i = 0; i < (int)size - 1; i++) {
        c = (int)*twtFile->pos++;
        if (c == -1) {
            *writePtr = '\0';
            twtFile->error = -1;
            return NULL;
        }
        *writePtr++ = c;
        if (c == '\n') {
            break;
        }
        if (twtFile->pos >= twtFile->end) {
            break;
        }
    }
    *writePtr = '\0';
    twtFile->error = 0;
    return buffer;
#endif
}

// FUNCTION: CARMA2_HW 0x004b49a0
int C2_HOOK_FASTCALL PFungetc(int ch, FILE* pFile) {
#ifndef CARPOCALYPSE2_MATCHING
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return ungetc(ch, pFile); }
    tPackEntryStream* entry = &gEntryStreams[(uintptr_t)pFile];
    tTwatVfsFile* file = &gTwatVfsFiles[(uintptr_t)pFile];
    size_t at;
    tU8 previous;
    if (entry->pos == 0 || ch == EOF) { file->error = -1; return EOF; }
    at = entry->pos - 1;
    /* The byte just read is still in the window (which carries any earlier edits); reading it
     * back from the pack is a card read, once per text line through GetALine. */
    if (entry->window_size != 0 && at >= entry->window_pos && at - entry->window_pos < entry->window_size) {
        previous = file->start[at - entry->window_pos];
    } else if (PackReadAt(entry, at, &previous, 1) != 1) { file->error = -1; return EOF; }
    if (previous != (tU8)ch) {
        if (entry->edits == NULL) {
            entry->edits = tmpfile();
            if (entry->edits != NULL) setvbuf(entry->edits, NULL, _IONBF, 0);
        }
        if (entry->edits == NULL || fseek(entry->edits, 0, SEEK_END) != 0
            || fwrite(&at, sizeof(at), 1, entry->edits) != 1
            || fputc((tU8)ch, entry->edits) == EOF || fflush(entry->edits) != 0) {
            file->error = -1; return EOF;
        }
        if (at >= entry->window_pos && at - entry->window_pos < entry->window_size)
            file->start[at - entry->window_pos] = (tU8)ch;
    }
    entry->pos = at;
    file->error = 0;
    return ch;
#else

    tTwatVfsFile* twtFile;

    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return ungetc(ch, pFile);
    }
    twtFile = &gTwatVfsFiles[(intptr_t)pFile];
    if (twtFile->pos > twtFile->start && ch != -1) {
        twtFile->pos--;
        *twtFile->pos = ch;
        twtFile->error = 0;
        return ch;
    } else {
        twtFile->error = -1;
        return -1;
    }
#endif
}

// C2PORT-NONMATCHING: best 43.24% | difftest PASS 2000 trials (spec difftest/specs/004b49f0_PFfread.json) | reason VC5 register allocation and pointer scheduling
// FUNCTION: CARMA2_HW 0x004b49f0
br_size_t C2_HOOK_FASTCALL PFfread(void* buf, br_size_t size, unsigned int n, void* f) {
#ifdef CARPOCALYPSE2_MATCHING
    int read_size;
    tU8 *pos;
    tU8 *end;
#endif

#ifndef CARPOCALYPSE2_MATCHING
    if (gC2LoadSites > 0) C2LoadSite(C2_LOAD_CALLER());
#endif
    if ((uintptr_t)f >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return fread(buf, size, n, f);
    }
#ifndef CARPOCALYPSE2_MATCHING
    {
        tPackEntryStream* entry = &gEntryStreams[(uintptr_t)f];
        tTwatVfsFile* file = &gTwatVfsFiles[(uintptr_t)f];
        size_t remaining = entry->size - entry->pos, bytes, got;
        if (size == 0 || n == 0) return 0;
        bytes = n > remaining / size ? remaining : (size_t)size * n;
        /* Bulk reads bypass the window and go straight into caller storage. */
        if (bytes >= PACK_WINDOW_BYTES) {
            got = PackReadAt(entry, entry->pos, buf, bytes);
            entry->pos += got;
        } else {
            got = 0;
            while (got < bytes) {
                size_t take;
                if (PackGetByte((uintptr_t)f) == EOF) break;
                entry->pos--;
                take = entry->window_size - (entry->pos - entry->window_pos);
                if (take > bytes - got) take = bytes - got;
                memcpy((tU8*)buf + got, file->start + entry->pos - entry->window_pos, take);
                got += take;
                entry->pos += take;
            }
        }
        file->error = got == bytes ? 0 : -1;
        return got / size;
    }
#else
    read_size = size * n;
    end = gTwatVfsFiles[(uintptr_t)f].end;
    pos = gTwatVfsFiles[(uintptr_t)f].pos;
    if (end - pos >= read_size) {
        end = pos + read_size;
    } else {
        n = ((end - pos) / size) * size;
        read_size = size * n;//
    }
    memcpy(buf, gTwatVfsFiles[(uintptr_t)f].pos, read_size);
    gTwatVfsFiles[(uintptr_t)f].pos = end;
    gTwatVfsFiles[(uintptr_t)f].error = 0;
    return n;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4a80
int C2_HOOK_FASTCALL PFfwrite(const void* buf, br_size_t size, unsigned int n, void* f) {

    if ((uintptr_t)f >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return fwrite(buf, size, n, f);
    }
    FatalError(kFatalError_WriteAttemptToPackedFile_S, "unknown");
}

// FUNCTION: CARMA2_HW 0x004b4b00
int C2_NAKED C2_HOOK_FASTCALL PFftell(FILE* pF) {
#ifdef CARPOCALYPSE2_MATCHING
    __asm {
        cmp ecx, 0x33
        jb pfftell_small
        push ecx
        call ftell
        add esp, 4
        ret

    pfftell_small:
        shl ecx, 4
        mov eax, dword ptr [ecx + gTwatVfsFiles + 4]
        mov edx, dword ptr [ecx + gTwatVfsFiles]
        lea ecx, [ecx + gTwatVfsFiles]
        sub eax, edx
        mov dword ptr [ecx + 0xc], 0
        ret
    }
#else
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return ftell(pF); }
    gTwatVfsFiles[(uintptr_t)pF].error = 0;
    return (int)gEntryStreams[(uintptr_t)pF].pos;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4b70
int C2_HOOK_FASTCALL PFfseek(FILE* pF, int offset, int whence) {
#ifdef CARPOCALYPSE2_MATCHING
    tTwatVfsFile* twtFile;
    tU8 *newpos;

    if ((intptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return fseek(pF, offset, whence);
    }
    twtFile = &gTwatVfsFiles[(uintptr_t)pF];
    switch (whence) {
    case SEEK_SET:
        newpos = twtFile->start + offset;
        break;
    case SEEK_END:
        newpos = twtFile->end + offset;
        break;
    case SEEK_CUR:
        newpos = twtFile->pos + offset;
        break;
    }
    if (newpos >= twtFile->start && newpos <= twtFile->end) {
        twtFile->pos = newpos;
        twtFile->error = 0;
        return 0;
    } else {
        twtFile->error = -1;
        return -1;
    }
#else
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return fseek(pF, offset, whence); }
    tPackEntryStream* entry = &gEntryStreams[(uintptr_t)pF];
    tTwatVfsFile* file = &gTwatVfsFiles[(uintptr_t)pF];
    int64_t base, target;
    switch (whence) {
    case SEEK_SET: base = 0; break;
    case SEEK_CUR: base = entry->pos; break;
    case SEEK_END: base = entry->size; break;
    default: file->error = -1; return -1;
    }
    target = base + offset;
    if (target < 0 || target > entry->size) { file->error = -1; return -1; }
    entry->pos = (size_t)target;
    file->error = 0;
    return 0;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4be0
void C2_NAKED C2_HOOK_FASTCALL PFrewind(FILE* pF) {
#ifdef CARPOCALYPSE2_MATCHING
    __asm {
        cmp ecx, 0x33
        jb pfrewind_small
        push ecx
        call rewind
        add esp, 4
        ret

    pfrewind_small:
        shl ecx, 4
        lea eax, [ecx + gTwatVfsFiles]
        mov ecx, dword ptr [ecx + gTwatVfsFiles]
        mov dword ptr [eax + 0xc], 0
        mov dword ptr [eax + 4], ecx
        ret
    }
#else
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { rewind(pF); return; }
    gEntryStreams[(uintptr_t)pF].pos = 0;
    gTwatVfsFiles[(uintptr_t)pF].error = 0;
    return;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4c10
int C2_NAKED C2_HOOK_FASTCALL PFfeof(FILE* pFile) {
#ifdef CARPOCALYPSE2_MATCHING
    __asm {
        cmp ecx, 0x33
        jb pffeof_small
        mov eax, dword ptr [ecx + 0xc]
        and eax, 0x10
        ret

    pffeof_small:
        shl ecx, 4
        mov edx, dword ptr [ecx + gTwatVfsFiles + 8]
        lea eax, [ecx + gTwatVfsFiles]
        mov ecx, dword ptr [ecx + gTwatVfsFiles + 4]
        cmp ecx, edx
        sbb eax, eax
        inc eax
        ret
    }
#else
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return feof(pFile); }
    return gEntryStreams[(uintptr_t)pFile].pos >= gEntryStreams[(uintptr_t)pFile].size;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4c80
void C2_HOOK_FASTCALL PFForEveryFile(const char* pThe_path, tPDForEveryFileRecurse_cbfn pAction_routine) {
    int twt;
    int i;
    char buffer[256];

    for (twt = 0; twt < (int)CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); twt++) {
        if (gTwatVfsMountPoints[twt].header == NULL) {
            continue;
        }
        if (DRStricmp(pThe_path, gTwatVfsMountPoints[twt].path) != 0) {
            continue;
        }
        for (i = 0; i < gTwatVfsMountPoints[twt].header->nbFiles; i++) {
            PathCat(buffer, pThe_path, gTwatVfsMountPoints[twt].header->fileHeaders[i].filename);
            pAction_routine(buffer);
        }
        return;
    }
    PDForEveryFile(pThe_path, pAction_routine);
}

// FUNCTION: CARMA2_HW 0x004b4d30
void C2_HOOK_FASTCALL PFForEveryFile2(const char* path, tEnumPathCallback pCallback, void* data) {
    int twt;
    int i;
    tPath_name twt_filePath;

    for (twt = 0; twt < (int)CARPOCALYPSE2_ASIZE(gTwatVfsMountPoints); twt++) {
        if (gTwatVfsMountPoints[twt].header != NULL && DRStricmp(gTwatVfsMountPoints[twt].path, path) == 0) {
            for (i = 0; i < gTwatVfsMountPoints[twt].header->nbFiles; i++) {
                PathCat(twt_filePath, path, gTwatVfsMountPoints[twt].header->fileHeaders[i].filename);
                pCallback(twt_filePath, data);
            }
            return;
        }
    }
    PDEnumPath(path, pCallback, data);
}

// FUNCTION: CARMA2_HW 0x004b4df0
tTWTVFS C2_HOOK_FASTCALL OpenPackFileAndSetTiffLoading(const char* path) {
    tTWTVFS res;

    res = OpenPackFile(path);
    if (res >= 0) {
        gDisableTiffConversionStack[gDisableTiffConversionStackPos] = gDisableTiffConversion;
        gDisableTiffConversionStackPos++;
        gDisableTiffConversion = 1;
    }
    return res;
}

// FUNCTION: CARMA2_HW 0x004b4e20
void C2_HOOK_FASTCALL ClosePackFileAndSetTiffLoading(tTWTVFS twt) {

    if (twt >= 0) {
        gDisableTiffConversionStackPos -= 1;
        gDisableTiffConversion = gDisableTiffConversionStack[gDisableTiffConversionStackPos];
        ClosePackFile(twt);
    }
}

// FUNCTION: CARMA2_HW 0x004b4e60
void C2_HOOK_FASTCALL PackFileRevertTiffLoading(void) {
    int count;

    count = gDisableTiffConversionStackPos;
    if (count != 0) { // or > 0 if it is unsigned
        gDisableTiffConversionStack[count] = gDisableTiffConversion;
        gDisableTiffConversion = gDisableTiffConversionStack[count - 1];
    }
}

// FUNCTION: CARMA2_HW 0x004b4e90
void C2_HOOK_FASTCALL PackFileRerevertTiffLoading(void) {

    if (gDisableTiffConversionStackPos != 0) {
        gDisableTiffConversion = gDisableTiffConversionStack[gDisableTiffConversionStackPos];
    }
}

// FUNCTION: CARMA2_HW 0x004b4eb0
void C2_HOOK_FASTCALL PackfileNoOp(void) {
}
