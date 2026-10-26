/* Bootstrap file installation, separate from SDL so it can be tested on a host. */
#include "vita_data.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define PATH_CAPACITY 1024

static int copy_error(const char* path) {
    fprintf(stderr, "Bundled data: %s: %s\n", path, strerror(errno));
    fflush(stderr);
    return 0;
}

static int joined(char* result, const char* directory, const char* name) {
    int n = snprintf(result, PATH_CAPACITY, "%s/%s", directory, name);
    if (n < 0 || n >= PATH_CAPACITY) {
        errno = ENAMETOOLONG;
        return copy_error(directory);
    }
    return 1;
}

static int copy_missing_file(const char* source, const char* destination,
    C2VitaCopyProgress progress, void* context) {
    struct stat info;
    char temporary[PATH_CAPACITY];
    unsigned char buffer[65536];
    FILE* input;
    FILE* output;
    size_t n;
    int ok = 1;
    int failure = 0;
    int name_length;

    if (stat(destination, &info) == 0) {
        if (!S_ISREG(info.st_mode)) { errno = EEXIST; return copy_error(destination); }
        /* Preserve existing settings, saves and manually installed game data. */
        return !progress || progress(context, source, 0);
    }
    if (errno != ENOENT) return copy_error(destination);
    name_length = snprintf(temporary, sizeof(temporary), "%s.c2-part", destination);
    if (name_length < 0 || name_length >= (int)sizeof(temporary)) {
        errno = ENAMETOOLONG;
        return copy_error(destination);
    }
    input = fopen(source, "rb");
    if (!input) return copy_error(source);
    output = fopen(temporary, "wb");
    if (!output) { fclose(input); return copy_error(temporary); }
    while ((n = fread(buffer, 1, sizeof(buffer), input)) != 0) {
        if (fwrite(buffer, 1, n, output) != n) { failure = errno ? errno : EIO; ok = 0; break; }
        if (progress && !progress(context, source, (unsigned int)n)) {
            fprintf(stderr, "Bundled data installation cancelled; retry on next launch.\n");
            failure = EINTR;
            ok = 0;
            break;
        }
    }
    if (ferror(input)) { failure = EIO; ok = 0; }
    if (fclose(input) != 0) { failure = errno; ok = 0; }
    if (fclose(output) != 0) { failure = errno; ok = 0; }
    if (ok && rename(temporary, destination) != 0) { failure = errno; ok = 0; }
    if (!ok) { errno = failure ? failure : EIO; copy_error(destination); remove(temporary); }
    return ok;
}

int C2VitaCopyMissingTree(const char* source, const char* destination,
    C2VitaCopyProgress progress, void* context) {
    DIR* directory;
    struct dirent* entry;
    struct stat info;
    int ok = 1;

    if (stat(source, &info) != 0) return copy_error(source);
    if (!S_ISDIR(info.st_mode)) { errno = ENOTDIR; return copy_error(source); }
    if (mkdir(destination, 0777) != 0 && errno != EEXIST) return copy_error(destination);
    if (stat(destination, &info) != 0) return copy_error(destination);
    if (!S_ISDIR(info.st_mode)) { errno = ENOTDIR; return copy_error(destination); }
    directory = opendir(source);
    if (!directory) return copy_error(source);
    for (;;) {
        char from[PATH_CAPACITY], to[PATH_CAPACITY];
        errno = 0;
        entry = readdir(directory);
        if (!entry) { if (errno) ok = copy_error(source); break; }
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        if (!joined(from, source, entry->d_name) || !joined(to, destination, entry->d_name)) { ok = 0; break; }
        if (stat(from, &info) != 0) { ok = copy_error(from); break; }
        if (S_ISDIR(info.st_mode)) ok = C2VitaCopyMissingTree(from, to, progress, context);
        else if (S_ISREG(info.st_mode)) ok = copy_missing_file(from, to, progress, context);
        else { errno = EINVAL; ok = copy_error(from); }
        if (!ok) break;
    }
    closedir(directory);
    return ok;
}
