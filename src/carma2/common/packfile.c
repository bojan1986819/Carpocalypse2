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
} tPackEntryStream;
static int gPackTrace = -1;
static tPackEntryStream gEntryStreams[CARPOCALYPSE2_ASIZE(gTwatVfsFiles)];

static size_t PackReadAt(tPackEntryStream* entry, size_t pos, void* buffer, size_t bytes) {
    size_t got;
    if (fseek(entry->source->stream, (long)(entry->offset + pos), SEEK_SET) != 0) return 0;
    got = fread(buffer, 1, bytes, entry->source->stream);
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
    {
        FILE* disk = fopen(pPath, mode);
        if (disk != NULL) {
            return disk;
        }
#if defined(CARPOCALYPSE2_FIX_BUGS) && !defined(_WIN32) && !defined(__vita__)
        return CI_FopenCaseInsensitive(pPath, mode);
#else
        return NULL;
#endif
    }
}

// FUNCTION: CARMA2_HW 0x004b4880
int C2_HOOK_FASTCALL PFfgetc(FILE* pFile) {
#ifndef CARPOCALYPSE2_MATCHING
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return fgetc(pFile); }
    return PackGetByte((uintptr_t)pFile);
#else

    tTwatVfsFile* twtFile;
    int result;

#ifdef CARPOCALYPSE2_MATCHING
    if ((intptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#else
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#endif
        return fgetc(pFile);
    }
    twtFile = &gTwatVfsFiles[(uintptr_t)pFile];
    if (twtFile->pos >= twtFile->end) {
        twtFile->error = -1;
        return EOF;
    }
    twtFile->error = 0;
    result = *twtFile->pos;
    twtFile->pos++;
    return result;
#endif
}

// PFgetc

// FUNCTION: CARMA2_HW 0x004b4900
char* C2_HOOK_FASTCALL PFfgets(char* buffer, br_size_t size, FILE* pFile) {
#ifndef CARPOCALYPSE2_MATCHING
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
    if (PackReadAt(entry, at, &previous, 1) != 1) { file->error = -1; return EOF; }
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
int C2_HOOK_FASTCALL PFftell(FILE* pF) {
#ifndef CARPOCALYPSE2_MATCHING
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return ftell(pF); }
    gTwatVfsFiles[(uintptr_t)pF].error = 0;
    return (int)gEntryStreams[(uintptr_t)pF].pos;
#else

    int pos;

    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
        return ftell(pF);
    }
    pos = gTwatVfsFiles[(uintptr_t)pF].pos - gTwatVfsFiles[(uintptr_t)pF].start;
    gTwatVfsFiles[(uintptr_t)pF].error = 0;
    return pos;
#endif
}

// FUNCTION: CARMA2_HW 0x004b4b70
int C2_HOOK_FASTCALL PFfseek(FILE* pF, int offset, int whence) {
#ifndef CARPOCALYPSE2_MATCHING
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
#else

    tTwatVfsFile* twtFile;
#ifdef CARPOCALYPSE2_MATCHING
    tU8 *newpos;
#endif

#ifdef CARPOCALYPSE2_MATCHING
    if ((intptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#else
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#endif
        return fseek(pF, offset, whence);
    }
    twtFile = &gTwatVfsFiles[(uintptr_t)pF];
#ifndef CARPOCALYPSE2_MATCHING
    {
        int64_t base;
        int64_t target;
        switch (whence) {
        case SEEK_SET: base = 0; break;
        case SEEK_CUR: base = twtFile->pos - twtFile->start; break;
        case SEEK_END: base = twtFile->end - twtFile->start; break;
        default: twtFile->error = -1; return -1;
        }
        target = base + offset;
        if (target < 0 || target > twtFile->end - twtFile->start) {
            twtFile->error = -1;
            return -1;
        }
        twtFile->pos = twtFile->start + (size_t)target;
        twtFile->error = 0;
        return 0;
    }
#else
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
#endif
#endif
}

// FUNCTION: CARMA2_HW 0x004b4be0
void C2_HOOK_FASTCALL PFrewind(FILE* pF) {
#ifndef CARPOCALYPSE2_MATCHING
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { rewind(pF); return; }
    gEntryStreams[(uintptr_t)pF].pos = 0;
    gTwatVfsFiles[(uintptr_t)pF].error = 0;
    return;
#else

    tTwatVfsFile* twtFile;

#ifdef CARPOCALYPSE2_MATCHING
    if ((int)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#else
    if ((uintptr_t)pF >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#endif
        rewind(pF);
    } else {
        twtFile = &gTwatVfsFiles[(uintptr_t)pF];
        twtFile->pos = twtFile->start;
        twtFile->error = 0;
    }
#endif
}

// FUNCTION: CARMA2_HW 0x004b4c10
int C2_HOOK_FASTCALL PFfeof(FILE* pFile) {
#ifndef CARPOCALYPSE2_MATCHING
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) { return feof(pFile); }
    return gEntryStreams[(uintptr_t)pFile].pos >= gEntryStreams[(uintptr_t)pFile].size;
#else

    tTwatVfsFile* twtFile;

#ifdef CARPOCALYPSE2_MATCHING
    if ((int)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#else
    if ((uintptr_t)pFile >= CARPOCALYPSE2_ASIZE(gTwatVfsFiles)) {
#endif
        return feof(pFile);
    } else {
        twtFile = &gTwatVfsFiles[(uintptr_t)pFile];
        return twtFile->pos >= twtFile->end;
    }
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
