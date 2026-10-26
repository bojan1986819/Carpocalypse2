/* Run via test_lazy_twt.py. Compare the actual PF code against a whole-pack
 * direct parse; the reference allocation is deliberately outside BrMem. */
#include "packfile.h"
#include "loadprof.h"
#include <time.h>
tU32 C2_HOOK_FASTCALL PDGetTotalTime(void) { return (tU32)(clock() * 1000 / CLOCKS_PER_SEC); }
#include "loading.h"
#include "utility.h"
#include "platform.h"
#include "errors.h"
#include <brender/brender.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s (pack=%s)\n", __FILE__, __LINE__, #x, current_pack); exit(1); } } while (0)
static const char* current_pack = "synthetic";
static size_t live_bytes, peak_bytes, max_request, allocations;
static unsigned int pack_count, entry_count, text_tests, binary_tests;
static unsigned long long compared_bytes;
extern tTwatVfsFile gTwatVfsFiles[51];

typedef struct { size_t size; unsigned int magic; } Allocation;
void* C2_HOOK_CDECL BrMemAllocate(br_size_t size, br_uint_8 type) {
    Allocation* block = malloc(sizeof(*block) + size + 16);
    CHECK(type == kMem_packed_file && block != NULL);
    block->size = size;
    block->magic = 0x1234abcd;
    memset(block + 1, 0, size);
    memset((unsigned char*)(block + 1) + size, 0xa5, 16);
    live_bytes += size;
    if (live_bytes > peak_bytes) peak_bytes = live_bytes;
    if (size > max_request) max_request = size;
    allocations++;
    return block + 1;
}
void C2_HOOK_CDECL BrMemFree(void* ptr) {
    Allocation* block;
    int i;
    if (ptr == NULL) return;
    block = (Allocation*)ptr - 1;
    CHECK(block->magic == 0x1234abcd);
    for (i = 0; i < 16; i++) CHECK(((unsigned char*)ptr)[block->size + i] == 0xa5);
    live_bytes -= block->size;
    allocations--;
    block->magic = 0;
    free(block);
}
void C2_HOOK_CDECL dr_dprintf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt); vprintf(fmt, ap); va_end(ap);
    putchar('\n');
}
void C2_HOOK_CDECL FatalError(int code, ...) { fprintf(stderr, "FatalError %d\n", code); exit(1); }
int C2_HOOK_FASTCALL DRStricmp(const char* a, const char* b) {
#ifdef _WIN32
    return _stricmp(a, b);
#else
    return strcasecmp(a, b);
#endif
}
void C2_HOOK_FASTCALL PathCat(char* out, const char* a, const char* b) { sprintf(out, "%s/%s", a, b); }
void C2_HOOK_FASTCALL PDForEveryFile(const char* path, tPDForEveryFile_cbfn cb) { CHECK(0); }
int C2_HOOK_FASTCALL PDEnumPath(const char* path, tEnumPathCallback cb, void* data) { CHECK(0); return 0; }

static unsigned int le32(const unsigned char* p) {
    return (unsigned int)p[0] | (unsigned int)p[1] << 8 | (unsigned int)p[2] << 16 | (unsigned int)p[3] << 24;
}
static void put32(unsigned char* p, unsigned int n) {
    p[0] = n; p[1] = n >> 8; p[2] = n >> 16; p[3] = n >> 24;
}
static void readers(FILE* file, const unsigned char* expected, size_t size) {
    unsigned char buffer[257];
    size_t at = 0, want, j;
    CHECK(PFftell(file) == 0);
    CHECK(PFfeof(file) == (size == 0));
    /* Compare fgetc including 0xff, and the packed reader's error reset. */
    while (at < size) { CHECK(PFfgetc(file) == expected[at]); at++; }
    CHECK(PFfeof(file) && PFfgetc(file) == EOF);
    CHECK(gTwatVfsFiles[(uintptr_t)file].error == -1);
    CHECK(PFftell(file) == size && gTwatVfsFiles[(uintptr_t)file].error == 0);
    CHECK(PFfgetc(file) == EOF);
    PFrewind(file);
    CHECK(PFftell(file) == 0 && gTwatVfsFiles[(uintptr_t)file].error == 0);
    CHECK(PFfseek(file, -1, SEEK_SET) == -1 && PFftell(file) == 0);
    CHECK(PFfseek(file, 0, 999) == -1);
    CHECK(PFfseek(file, INT_MAX, SEEK_CUR) == (size == INT_MAX ? 0 : -1));
    CHECK(PFfseek(file, 0, SEEK_END) == 0 && PFfeof(file));
    CHECK(PFfgets((char*)buffer, sizeof(buffer), file) == NULL);
    CHECK(PFfseek(file, -(int)size, SEEK_CUR) == 0);
    CHECK(PFfread(buffer, 0, UINT_MAX, file) == 0 && PFftell(file) == 0);
    CHECK(PFfread(buffer, 7, 0, file) == 0 && PFftell(file) == 0);
    /* Exact expected fgets bytes, retaining CR and embedded NUL/0xff. */
    at = 0;
    while (at < size) {
        want = 0;
        while (want < sizeof(buffer) - 1 && at + want < size) {
            if (expected[at + want++] == '\n') break;
        }
        memset(buffer, 0x5a, sizeof(buffer));
        CHECK(PFfgets((char*)buffer, sizeof(buffer), file) == (char*)buffer);
        CHECK(memcmp(buffer, expected + at, want) == 0 && buffer[want] == 0);
        at += want;
        CHECK(PFftell(file) == at);
    }
    CHECK(PFfeof(file) && PFfgets((char*)buffer, 2, file) == NULL);
    /* Partial multi-byte elements: count complete elements, consume remainder. */
    for (j = 1; j <= 8; j++) {
        size_t tail = size < 13 ? size : 13;
        CHECK(PFfseek(file, -(int)tail, SEEK_END) == 0);
        memset(buffer, 0x5a, sizeof(buffer));
        CHECK(PFfread(buffer, j, 20, file) == tail / j);
        CHECK(memcmp(buffer, expected + size - tail, tail) == 0 && buffer[tail] == 0x5a);
        CHECK(PFftell(file) == size && PFfeof(file));
    }
    PFrewind(file);
    if (size != 0) {
        CHECK(PFfgetc(file) == expected[0]);
        CHECK(PFungetc(expected[0], file) == expected[0]);
        CHECK(PFfgetc(file) == expected[0]);
    }
    PFrewind(file);
}

static unsigned int enum_count;
static void C2_HOOK_FASTCALL enumerate(const char* path) { CHECK(path != NULL); enum_count++; }
static int C2_HOOK_FASTCALL enumerate2(const char* path, void* data) {
    CHECK(data == &enum_count); enumerate(path); return 0;
}

static void verify_pack(const char* path) {
    FILE *disk, *file;
    char mount_path[256], entry_path[512];
    unsigned char *whole, *actual;
    size_t disk_size, offset, i, count, table_bytes, mount_max, mount_peak;
    tTWTVFS mount;
    current_pack = path;
    CHECK(strlen(path) < sizeof(mount_path));
    strcpy(mount_path, path);
    mount_path[strlen(mount_path) - 4] = '\0';
    disk = fopen(path, "rb"); CHECK(disk != NULL);
    CHECK(fseek(disk, 0, SEEK_END) == 0);
    disk_size = ftell(disk); rewind(disk);
    whole = malloc(disk_size); CHECK(whole != NULL);
    CHECK(fread(whole, 1, disk_size, disk) == disk_size); fclose(disk);
    CHECK(disk_size >= 8 && le32(whole) == disk_size);
    count = le32(whole + 4);
    table_bytes = 8 + count * 56;
    CHECK(table_bytes <= disk_size && live_bytes == 0);
    peak_bytes = max_request = 0;
    mount = OpenPackFile(mount_path); CHECK(mount >= 0);
    mount_max = max_request; mount_peak = peak_bytes;
    CHECK(live_bytes == table_bytes && allocations == 1 && max_request == table_bytes);
    CHECK(gTwatVfsMountPoints[mount].data == NULL);
    enum_count = 0; PFForEveryFile(mount_path, enumerate); CHECK(enum_count == count);
    enum_count = 0; PFForEveryFile2(mount_path, enumerate2, &enum_count); CHECK(enum_count == count);
    offset = table_bytes;
    for (i = 0; i < count; i++) {
        const unsigned char* entry = whole + 8 + 56 * i;
        size_t size = le32(entry), at;
        unsigned char chunk[4093];
        CHECK(offset + size <= disk_size && memchr(entry + 4, 0, 48) != NULL);
        snprintf(entry_path, sizeof(entry_path), "%s/%s", mount_path, entry + 4);
        {
            unsigned long long opens = gC2LoadCounts.fopen;
            file = PFfopen(entry_path, "rb"); CHECK(file != NULL && (uintptr_t)file < 51);
            CHECK(gC2LoadCounts.fopen == opens); /* entries reuse the mounted source */
        }
        CHECK(live_bytes == table_bytes + 65536);
        CHECK(max_request <= (table_bytes > 65536 ? table_bytes : 65536));
        actual = malloc(size != 0 ? size : 1); CHECK(actual != NULL);
        CHECK(PFfread(actual, 1, size, file) == size);
        CHECK(memcmp(actual, whole + offset, size) == 0);
        CHECK(PFftell(file) == size && PFfeof(file));
        CHECK(PFfgetc(file) == EOF);
        PFrewind(file);
        /* A second pass with non-aligned chunk boundaries exercises positions. */
        for (at = 0; at < size;) {
            size_t want = size - at < sizeof(chunk) ? size - at : sizeof(chunk);
            CHECK(PFfread(chunk, 1, sizeof(chunk), file) == want);
            CHECK(memcmp(chunk, whole + offset + at, want) == 0);
            at += want;
            CHECK(PFftell(file) == at);
        }
        PFrewind(file);
        /* Interleave byte, line and bulk reads at every window boundary;
         * seek both directions and disturb an independent handle. */
        {
            size_t boundary;
            FILE* other = PFfopen(entry_path, "rb"); CHECK(other != NULL);
            for (boundary = 65536; boundary < size + 65536; boundary += 65536) {
                size_t start = boundary > 17 ? boundary - 17 : 0;
                size_t want, used = 0;
                if (start >= size) break;
                CHECK(PFfseek(file, (int)start, SEEK_SET) == 0);
                CHECK(PFfgetc(file) == whole[offset + start]); used++;
                want = 0;
                while (want < 36 && start + used + want < size) {
                    if (whole[offset + start + used + want++] == '\n') break;
                }
                if (want != 0) {
                    CHECK(PFfgets((char*)chunk, 37, file) == (char*)chunk);
                    CHECK(memcmp(chunk, whole + offset + start + used, want) == 0 && chunk[want] == 0);
                    used += want;
                }
                want = size - start - used < 101 ? size - start - used : 101;
                CHECK(PFfread(chunk, 1, 101, file) == want);
                CHECK(memcmp(chunk, whole + offset + start + used, want) == 0);
                CHECK(PFftell(file) == start + used + want);
                CHECK(PFfseek(other, (int)start, SEEK_SET) == 0);
                CHECK(PFfgetc(other) == whole[offset + start]);
                CHECK(PFfseek(file, (int)start, SEEK_SET) == 0);
                CHECK(PFfgetc(file) == whole[offset + start]);
            }
            CHECK(PFfclose(other) == 0);
        }
        PFrewind(file);
        if (strstr((char*)entry + 4, ".TXT") != NULL && text_tests < 6) {
            readers(file, whole + offset, size); text_tests++;
        } else if (strstr((char*)entry + 4, ".TXT") == NULL && binary_tests < 6) {
            readers(file, whole + offset, size); binary_tests++;
        }
        /* Owned entry must remain readable when its mount closes and is reused. */
        if (i + 1 == count) {
            ClosePackFile(mount);
            CHECK(live_bytes == 65536);
            mount = OpenPackFile(mount_path); CHECK(mount >= 0);
            PFrewind(file);
            CHECK(PFfread(actual, 1, size, file) == size);
            CHECK(memcmp(actual, whole + offset, size) == 0);
        }
        CHECK(PFfclose(file) == 0 && live_bytes == table_bytes);
        free(actual);
        compared_bytes += size;
        entry_count++;
        offset += (size + 3u) & ~(size_t)3u;
    }
    if (strstr(path, "TIMB.TWT") != NULL) {
        printf("TIMB MEMORY pack=%zu mount_peak=%zu largest_mount_allocation=%zu largest_entry_allocation=%zu entry_phase_peak=%zu\n",
            disk_size, mount_peak, mount_max, max_request, peak_bytes);
    }
    ClosePackFile(mount); CHECK(live_bytes == 0 && allocations == 0);
    free(whole); pack_count++;
}

static void write_fixture(const char* path, const unsigned char* bytes, size_t size) {
    FILE* f = fopen(path, "wb"); CHECK(f != NULL);
    CHECK(fwrite(bytes, 1, size, f) == size); CHECK(fclose(f) == 0);
}
static void fixtures(void) {
    unsigned char pack[132] = {0}; /* 8 + two 56-byte records + 12 payload bytes */
    const unsigned char payload[] = { 'a', '\r', '\n', 0, 255, 'z', '\n', 'b', 'c' };
    FILE* files[50];
    FILE* file;
    tTWTVFS mount, mounts[5];
    size_t i;
    put32(pack, sizeof(pack)); put32(pack + 4, 2);
    put32(pack + 8, sizeof(payload)); strcpy((char*)pack + 12, "TEXT.TXT");
    strcpy((char*)pack + 68, "EMPTY.BIN");
    memcpy(pack + 120, payload, sizeof(payload));
    write_fixture("lazy_fixture.TWT", pack, sizeof(pack));
    mount = OpenPackFile("lazy_fixture"); CHECK(mount >= 0);
    file = PFfopen("lazy_fixture/EMPTY.BIN", "rb"); CHECK(file != NULL);
    readers(file, pack, 0); CHECK(PFfclose(file) == 0);
    for (i = 0; i < 50; i++) {
        files[i] = PFfopen("lazy_fixture/TEXT.TXT", "rb"); CHECK(files[i] != NULL);
    }
    CHECK(PFfopen("lazy_fixture/TEXT.TXT", "rb") == NULL);
    CHECK(PFfgetc(files[0]) == 'a' && PFungetc('X', files[0]) == 'X');
    CHECK(PFfgetc(files[0]) == 'X' && PFfgetc(files[1]) == 'a');
    PFrewind(files[0]);
    CHECK(PFfgetc(files[0]) == 'X'); /* retail ungetc mutates the entry */
    PFrewind(files[2]); readers(files[2], payload, sizeof(payload));
    ClosePackFile(mount);
    for (i = 0; i < 50; i++) CHECK(PFfclose(files[i]) == 0);
    CHECK(live_bytes == 0 && allocations == 0);
    for (i = 0; i < 5; i++) { mounts[i] = OpenPackFile("lazy_fixture"); CHECK(mounts[i] >= 0); }
    CHECK(OpenPackFile("lazy_fixture") == -1);
    for (i = 0; i < 5; i++) ClosePackFile(mounts[i]);
    /* Fifty orphan generations plus five current mounts exercise every source
     * slot. Reusing a mount must not redirect any retained entry's source. */
    for (i = 0; i < 50; i++) {
        mount = OpenPackFile("lazy_fixture"); CHECK(mount >= 0);
        files[i] = PFfopen("lazy_fixture/TEXT.TXT", "rb"); CHECK(files[i] != NULL);
        ClosePackFile(mount);
    }
    for (i = 0; i < 5; i++) { mounts[i] = OpenPackFile("lazy_fixture"); CHECK(mounts[i] >= 0); }
    for (i = 0; i < 50; i++) {
        readers(files[i], payload, sizeof(payload));
        CHECK(PFfclose(files[i]) == 0);
    }
    for (i = 0; i < 5; i++) ClosePackFile(mounts[i]);
    CHECK(live_bytes == 0 && allocations == 0);
    /* Invalid/truncated inputs must not leak handles or allocations. */
    put32(pack + 4, UINT_MAX); write_fixture("lazy_fixture.TWT", pack, sizeof(pack));
    CHECK(OpenPackFile("lazy_fixture") == -1);
    put32(pack + 4, 2); put32(pack + 8, UINT_MAX); write_fixture("lazy_fixture.TWT", pack, sizeof(pack));
    CHECK(OpenPackFile("lazy_fixture") == -1);
    put32(pack + 8, sizeof(payload)); write_fixture("lazy_fixture.TWT", pack, 7);
    CHECK(OpenPackFile("lazy_fixture") == -1);
    CHECK(live_bytes == 0 && allocations == 0);
    /* Exercise disk fallback and all wrappers on a real FILE*. */
    write_fixture("lazy_fixture.TWT", payload, sizeof(payload));
    file = PFfopen("lazy_fixture.TWT", "rb"); CHECK((uintptr_t)file >= 51);
    CHECK(PFfgetc(file) == 'a' && PFftell(file) == 1);
    CHECK(PFfseek(file, 0, SEEK_END) == 0 && PFfgetc(file) == EOF && PFfeof(file));
    PFrewind(file); CHECK(!PFfeof(file));
    { char line[10]; CHECK(PFfgets(line, sizeof(line), file) != NULL && strcmp(line, "a\r\n") == 0); }
    CHECK(PFfclose(file) == 0); CHECK(remove("lazy_fixture.TWT") == 0);
}

static void window_fixture(void) {
    size_t size = 3 * 65536 + 19, i;
    unsigned char* pack = calloc(1, 64 + ((size + 3) & ~(size_t)3));
    unsigned char* actual = malloc(size);
    FILE *file, *other;
    tTWTVFS mount;
    CHECK(pack && actual);
    put32(pack, 64 + ((size + 3) & ~(size_t)3)); put32(pack + 4, 1);
    put32(pack + 8, size); strcpy((char*)pack + 12, "WINDOW.BIN");
    for (i = 0; i < size; i++) pack[64 + i] = (unsigned char)(i * 19 + i / 251);
    write_fixture("lazy_window.TWT", pack, 64 + ((size + 3) & ~(size_t)3));
    mount = OpenPackFile("lazy_window"); CHECK(mount >= 0);
    file = PFfopen("lazy_window/WINDOW.BIN", "rb"); CHECK(file);
    other = PFfopen("lazy_window/WINDOW.BIN", "rb"); CHECK(other);
    CHECK(PFfseek(file, 65537, SEEK_SET) == 0);
    CHECK(PFungetc('X', file) == 'X' && PFungetc('Y', file) == 'Y');
    CHECK(PFfgetc(file) == 'Y' && PFfgetc(file) == 'X');
    CHECK(PFfseek(file, 0, SEEK_END) == 0 && PFungetc('Z', file) == 'Z');
    ClosePackFile(mount);
    PFrewind(file);
    CHECK(PFfread(actual, 1, size, file) == size);
    pack[64 + 65535] = 'Y'; pack[64 + 65536] = 'X'; pack[64 + size - 1] = 'Z';
    CHECK(memcmp(actual, pack + 64, size) == 0);
    /* Repeated window eviction must retain the mutations, even after seeks. */
    PFrewind(file);
    readers(file, pack + 64, size);
    CHECK(PFfseek(other, 65535, SEEK_SET) == 0);
    CHECK(PFfgetc(other) == (unsigned char)(65535u * 19 + 65535u / 251));
    CHECK(PFfclose(file) == 0 && PFfclose(other) == 0);
    CHECK(live_bytes == 0 && allocations == 0);
    CHECK(remove("lazy_window.TWT") == 0);
    free(pack); free(actual);
}
int main(int argc, char** argv) {
    FILE* manifest;
    char path[1024];
    CHECK(argc == 2);
    InitPackFiles(); fixtures(); window_fixture();
    manifest = fopen(argv[1], "r"); CHECK(manifest != NULL);
    while (fgets(path, sizeof(path), manifest) != NULL) {
        path[strcspn(path, "\r\n")] = 0;
        if (path[0] != 0) verify_pack(path);
    }
    fclose(manifest);
    CHECK(pack_count > 0 && text_tests > 0 && binary_tests > 0);
    printf("PASS packs=%u entries=%u bytes=%llu text_reader_cases=%u binary_reader_cases=%u live_allocations=%zu\n",
        pack_count, entry_count, compared_bytes, text_tests, binary_tests, allocations);
    return 0;
}
