/*
 * Case-insensitive path resolution for case-sensitive filesystems.
 *
 * The retail game was built for Windows where file names are case-insensitive,
 * and it requests data files in their original upper-case spelling ("DATA",
 * "GENERAL.TXT", ...). On Linux this fails when the data tree uses lower-case
 * names. This module resolves each path component against the real directory
 * contents, accepting any letter casing.
 *
 * Where the build adds `-Wl,--wrap=fopen` (Linux and Android; guarded by
 * CARPOCALYPSE2_CASEFIX_WRAP, since Apple's linker has no --wrap), the
 * __wrap_fopen entry point below intercepts every fopen() call in the binary:
 * the plain open is attempted first so correct-case paths behave exactly as
 * before, and only on failure do we walk the directory chain looking for a
 * case-insensitive match.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef __vita__
#include <stdatomic.h>
#include <SDL3/SDL.h>
/* File probes repeat the same directory prefixes many times during loading.
 * Keep bounded per-thread results; writes invalidate every thread's cache.
 * Missing leaf names are cached too, until a writer may create them. */
static atomic_uint case_generation;
typedef struct {
    unsigned generation, clock;
    struct { char key[1024], value[1024]; unsigned stamp; } entries[64];
    struct {
        char parent[1024], names[1024][64];
        unsigned stamp, count;
        int complete;
    } directories[8];
    char fixed[1024];
} CaseContext;
/* VitaSDK's GCC emutls falls back to one process-global slot without gthreads.
 * SDL uses native thread IDs, so its TLS is safe for the music worker too. */
static SDL_TLSID case_tls;
static CaseContext* case_context(void) {
    CaseContext* context = SDL_GetTLS(&case_tls);
    if (!context) {
        context = SDL_calloc(1, sizeof(*context));
        if (context && !SDL_SetTLS(&case_tls, context, SDL_free)) {
            SDL_free(context);
            context = NULL;
        }
    }
    return context;
}
static void invalidate_case_cache(void) {
    atomic_fetch_add(&case_generation, 1);
}
static int cached_component(CaseContext* context, const char* key, char* value) {
    unsigned i, generation = atomic_load(&case_generation);
    if (context->generation != generation) {
        memset(context->entries, 0, sizeof(context->entries));
        memset(context->directories, 0, sizeof(context->directories));
        context->clock = 0;
        context->generation = generation;
    }
    for (i = 0; i < 64; ++i) {
        if (context->entries[i].stamp && !strcasecmp(key, context->entries[i].key)) {
            strcpy(value, context->entries[i].value);
            context->entries[i].stamp = ++context->clock;
            return 1;
        }
    }
    return 0;
}
/* Index directory names once instead of re-reading the same large directory
 * for every distinct missing texture. Incomplete indexes fall back to the
 * ordinary scan for misses, preserving long names and oversized directories. */
static int directory_match(CaseContext* context, const char* parent, const char* name, char* match) {
    unsigned i, slot = 0;
    for (i = 0; i < 8; ++i) {
        if (context->directories[i].stamp && !strcmp(parent, context->directories[i].parent)) {slot = i;break;}
        if (context->directories[i].stamp < context->directories[slot].stamp) slot = i;
    }
    if (i == 8) {
        DIR* dir;
        struct dirent* entry;
        memset(&context->directories[slot], 0, sizeof(context->directories[slot]));
        strcpy(context->directories[slot].parent, parent);
        context->directories[slot].complete = 1;
        dir = opendir(parent);
        if (dir) {
            while ((entry = readdir(dir)) != NULL) {
                unsigned count = context->directories[slot].count;
                if (count == 1024 || strlen(entry->d_name) >= 64) {
                    context->directories[slot].complete = 0;
                    continue;
                }
                strcpy(context->directories[slot].names[count], entry->d_name);
                ++context->directories[slot].count;
            }
            closedir(dir);
        }
    }
    context->directories[slot].stamp = ++context->clock;
    match[0] = 0;
    for (i = 0; i < context->directories[slot].count; ++i) {
        if (!strcasecmp(name, context->directories[slot].names[i])) {
            strcpy(match, context->directories[slot].names[i]);
            return 1;
        }
    }
    return context->directories[slot].complete;
}
static void cache_component(CaseContext* context, const char* key, const char* value) {
    unsigned i, slot = 0;
    for (i = 0; i < 64; ++i)
        if (context->entries[i].stamp < context->entries[slot].stamp) slot = i;
    strcpy(context->entries[slot].key, key);
    strcpy(context->entries[slot].value, value);
    context->entries[slot].stamp = ++context->clock;
}
#endif

const char* carpocalypse2_fix_path_case(const char* pPath) {
#ifdef __vita__
    CaseContext* context;
    char* fixed;
    char key[1024] = "";
    char* saved;
    int cacheable;
    struct stat info;
#else
    static char fixed[1024];
#endif
    char walk[1024];
    char* component;

    if (pPath == NULL || strlen(pPath) >= sizeof(walk)) {
        return pPath;
    }
#ifdef __vita__
    if (stat(pPath, &info) == 0) return pPath;
    /* ux0: is case-insensitive (exFAT): a name it cannot find exists in no other casing either. The
     * walk below cost ~13 ms per missing texture variant, ~3 s of every boot. */
    if (strchr(pPath, ':') == NULL || strncmp(pPath, "ux0:", 4) == 0) return pPath;
    context = case_context();
    if (!context) return pPath;
    fixed = context->fixed;
    cacheable = strchr(pPath, ':') != NULL; /* Relative paths depend on cwd. */
#endif
    strcpy(walk, pPath);
    fixed[0] = '\0';
    if (walk[0] == '/') {
        strcat(fixed, "/");
    }

#ifdef __vita__
    component = strtok_r(walk, "/", &saved);
#else
    component = strtok(walk, "/");
#endif
    while (component != NULL) {
        DIR* dir;
        struct dirent* entry;
        char match[512];
        int found;

#ifdef __vita__
        if (key[0]) strcat(key, "/");
        strcat(key, component);
        if (cacheable && cached_component(context, key, fixed)) {
            component = strtok_r(NULL, "/", &saved);
            continue;
        }
        /* A native mount prefix is a root, not an entry in the current cwd. */
        if (fixed[0] == '\0' && strchr(component, ':')) {
            strcpy(fixed, component);
            cache_component(context, key, fixed);
            component = strtok_r(NULL, "/", &saved);
            continue;
        }
        if (cacheable && directory_match(context, fixed[0] ? fixed : ".", component, match)) {
            if (fixed[0] && fixed[strlen(fixed) - 1] != '/') strcat(fixed, "/");
            strcat(fixed, match[0] ? match : component);
            cache_component(context, key, fixed);
            component = strtok_r(NULL, "/", &saved);
            continue;
        }
#endif
        dir = opendir(fixed[0] == '\0' ? "." : fixed);
        if (dir == NULL) {
            /* Cannot inspect this level; keep the rest verbatim. */
            if (fixed[0] != '\0' && fixed[strlen(fixed) - 1] != '/') {
                strcat(fixed, "/");
            }
            strcat(fixed, component);
#ifdef __vita__
            if (cacheable) cache_component(context, key, fixed);
            component = strtok_r(NULL, "/", &saved);
#else
            component = strtok(NULL, "/");
#endif
            continue;
        }
        match[0] = '\0';
        found = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (strcasecmp(entry->d_name, component) == 0) {
                strncpy(match, entry->d_name, sizeof(match) - 1);
                match[sizeof(match) - 1] = '\0';
                found = 1;
                break;
            }
        }
        closedir(dir);

        if (fixed[0] != '\0' && fixed[strlen(fixed) - 1] != '/') {
            strcat(fixed, "/");
        }
        if (found) {
            strcat(fixed, match);
        } else {
            strcat(fixed, component);
        }
#ifdef __vita__
        if (cacheable) cache_component(context, key, fixed);
        component = strtok_r(NULL, "/", &saved);
#else
        component = strtok(NULL, "/");
#endif
    }

    return fixed;
}

#ifdef CARPOCALYPSE2_CASEFIX_WRAP
extern FILE* __real_fopen(const char* pPath, const char* pMode);

FILE* __wrap_fopen(const char* pPath, const char* pMode) {
    FILE* f;

#ifdef __vita__
    if (strpbrk(pMode, "wa+")) invalidate_case_cache();
#endif
#ifdef __vita__
    Uint64 open_start = SDL_GetTicksNS();
#endif
    f = __real_fopen(pPath, pMode);
#ifdef __vita__
    if (getenv("C2_TEST_OPENLOG") != NULL) {
        /* test: what plain opens, misses and the case-fixing retries cost */
        static unsigned opens, misses;
        static Uint64 open_ns, fix_ns;
        extern void C2VitaLog(const char* format, ...);
        Uint64 t = SDL_GetTicksNS();
        opens++;
        open_ns += t - open_start;
        if (f == NULL) {
            const char* fixed = carpocalypse2_fix_path_case(pPath);
            f = __real_fopen(fixed, pMode);
            misses++;
            fix_ns += SDL_GetTicksNS() - t;
            if (misses <= 40) C2VitaLog("TEST OPEN miss %s -> %s\n", pPath, f != NULL ? "fixed" : "absent");
        }
        if (opens % 50 == 0) C2VitaLog("TEST OPEN opens=%u open_ms=%llu misses=%u fix_ms=%llu\n", opens,
            (unsigned long long)(open_ns / 1000000), misses, (unsigned long long)(fix_ns / 1000000));
    }
#endif
    if (f == NULL) {
        const char* fixed = carpocalypse2_fix_path_case(pPath);
        f = __real_fopen(fixed, pMode);
        if (getenv("CARPOCALYPSE2_FOPEN_DEBUG") != NULL) {
            fprintf(stderr, "[fopen MISS] %s%s\n", pPath, (f != NULL) ? "  (case-fixed OK)" : "");
            if (f == NULL && fixed != pPath) {
                fprintf(stderr, "[fopen miss2] %s\n", fixed);
            }
        }
    }
#ifdef __vita__
    if (strpbrk(pMode, "wa+")) invalidate_case_cache();
#endif
    return f;
}

/* The S3 sound layer opens data files with open() rather than fopen(), so we
 * wrap open()/open64() through the same case-insensitive resolver. */
extern int __real_open(const char* pPath, int pFlags, ...);
extern int __real_open64(const char* pPath, int pFlags, ...);

static int case_fix_open(const char* pPath, int pFlags, mode_t pMode) {
    int fd;

#ifdef __vita__
    if ((pFlags & (O_CREAT | O_TRUNC | O_WRONLY | O_RDWR)) != 0) invalidate_case_cache();
#endif
    fd = (int)__real_open(pPath, pFlags, pMode);
    if (fd < 0) {
        const char* fixed = carpocalypse2_fix_path_case(pPath);
        fd = (int)__real_open(fixed, pFlags, pMode);
        if (getenv("CARPOCALYPSE2_FOPEN_DEBUG") != NULL) {
            fprintf(stderr, "[open MISS] %s%s\n", pPath, (fd >= 0) ? "  (case-fixed OK)" : "");
        }
    }
#ifdef __vita__
    if ((pFlags & (O_CREAT | O_TRUNC | O_WRONLY | O_RDWR)) != 0) invalidate_case_cache();
#endif
    return fd;
}

int __wrap_open(const char* pPath, int pFlags, ...) {
    mode_t mode = 0;
    va_list ap;

    if ((pFlags & O_CREAT) != 0) {
        va_start(ap, pFlags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
    }
    return case_fix_open(pPath, pFlags, mode);
}

int __wrap_open64(const char* pPath, int pFlags, ...) {
    mode_t mode = 0;
    va_list ap;

    if ((pFlags & O_CREAT) != 0) {
        va_start(ap, pFlags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
    }
    return case_fix_open(pPath, pFlags, mode);
}
#endif

#ifdef __vita__
#include <psp2/io/stat.h>

/* Texture probing (IsValidFile, GetLastModificationTime) stats several candidate names per texture
 * in directories of up to ~500 files; exFAT finds a name by scanning the directory, ~7 ms a stat on
 * the card, ~3 s of every boot. Each ux0: directory is listed once instead (names and modification
 * times); the game's texture saving drops the listings (C2VitaStatCacheInvalidate). */
typedef struct {
    char* name;
    unsigned mtime, size;
} StatEntry;
typedef struct {
    char dir[512];
    StatEntry* entries;
    int count, listed;
    unsigned stamp;
} StatDirectory;
static StatDirectory stat_dirs[16];
static unsigned stat_clock;

static unsigned stat_time(const SceDateTime* t) {
    /* seconds since 1970 (days from civil date), as stat's st_mtime orders them */
    int y = t->year - (t->month <= 2);
    unsigned era = (unsigned)(y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - (int)era * 400);
    unsigned doy = (153u * (t->month + (t->month > 2 ? -3 : 9)) + 2) / 5 + t->day - 1;
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    unsigned days = era * 146097u + doe - 719468u;
    return days * 86400u + t->hour * 3600u + t->minute * 60u + t->second;
}

void C2VitaStatCacheInvalidate(void) {
    int i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < stat_dirs[i].count; j++) free(stat_dirs[i].entries[j].name);
        free(stat_dirs[i].entries);
        memset(&stat_dirs[i], 0, sizeof(stat_dirs[i]));
    }
}

static StatDirectory* stat_directory(const char* dir) {
    int i, slot = 0, capacity = 0;
    DIR* d;
    struct dirent* entry;
    StatDirectory* sd;
    for (i = 0; i < 16; i++) {
        if (stat_dirs[i].stamp != 0 && strcasecmp(stat_dirs[i].dir, dir) == 0) {
            stat_dirs[i].stamp = ++stat_clock;
            return &stat_dirs[i];
        }
        if (stat_dirs[i].stamp < stat_dirs[slot].stamp) slot = i;
    }
    sd = &stat_dirs[slot];
    for (i = 0; i < sd->count; i++) free(sd->entries[i].name);
    free(sd->entries);
    memset(sd, 0, sizeof(*sd));
    strcpy(sd->dir, dir);
    sd->stamp = ++stat_clock;
    unsigned long long list_start = SDL_GetTicksNS();
    d = opendir(dir);
    if (d == NULL) {
        SceIoStat st;
        /* no such directory: nothing in it exists (other failures stat each name) */
        sd->listed = sceIoGetstat(dir, &st) < 0;
        return sd;
    }
    while ((entry = readdir(d)) != NULL) {
        if (sd->count == capacity) {
            StatEntry* grown;
            capacity = capacity ? capacity * 2 : 64;
            grown = realloc(sd->entries, capacity * sizeof(StatEntry));
            if (grown == NULL) break;
            sd->entries = grown;
        }
        sd->entries[sd->count].name = strdup(entry->d_name);
        if (sd->entries[sd->count].name == NULL) break;
        sd->entries[sd->count].mtime = stat_time(&entry->d_stat.st_mtime);
        sd->entries[sd->count].size = entry->d_stat.st_size > 0xffffffffu ? 0xffffffffu : (unsigned)entry->d_stat.st_size;
        sd->count++;
    }
    closedir(d);
    sd->listed = 1;
    if (getenv("C2_TEST_LOADDETAIL") != NULL) {
        extern void C2VitaLog(const char* format, ...);
        C2VitaLog("TEST STATDIR %s entries=%d %llu us\n", dir, sd->count, (unsigned long long)((SDL_GetTicksNS() - list_start) / 1000));
    }
    return sd;
}

/* 1 = exists (*pMtime, *pSize set), 0 = absent, -1 = not a ux0: path (the caller stats it). */
int C2VitaStatCached(const char* pPath, unsigned* pMtime, unsigned* pSize) {
    char dir[512];
    const char* slash;
    const char* name;
    StatDirectory* sd;
    size_t length;
    int i;
    if (pPath == NULL || strncmp(pPath, "ux0:", 4) != 0) return -1;
    slash = strrchr(pPath, '/');
    if (slash == NULL || slash[1] == '\0') return -1;
    length = (size_t)(slash - pPath);
    if (length >= sizeof(dir)) return -1;
    memcpy(dir, pPath, length);
    dir[length] = '\0';
    name = slash + 1;
    sd = stat_directory(dir);
    for (i = 0; i < sd->count; i++) {
        if (strcasecmp(sd->entries[i].name, name) == 0) {
            if (pMtime != NULL) *pMtime = sd->entries[i].mtime;
            if (pSize != NULL) *pSize = sd->entries[i].size;
            return 1;
        }
    }
    if (!sd->listed) {
        /* a directory that could not be listed: ask the file system */
        SceIoStat st;
        if (sceIoGetstat(pPath, &st) < 0) return 0;
        if (pMtime != NULL) *pMtime = stat_time(&st.st_mtime);
        if (pSize != NULL) *pSize = st.st_size > 0xffffffffu ? 0xffffffffu : (unsigned)st.st_size;
        return 1;
    }
    return 0;
}
#endif
