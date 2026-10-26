"""Compile the production glyph lookup/cleanup with and without its port cache.

Run in a compiler environment: python scripts/test_font_probe.py [cc]
Tests the actual function bodies with mocked I/O/decoder ownership. Both paths
must return identical maps, while the port path must perform fewer PF probes.
"""
import pathlib
import re
import subprocess
import sys
import tempfile

root = pathlib.Path(__file__).resolve().parents[1]
source = (root / "src/carma2/common/font.c").read_text()
functions = []
for name in ("GetThisFuckingPixelmap", "KillThePixies"):
    match = re.search(r"^[^\n]*\b" + name + r"\([^\n]*\) \{\n", source, re.M)
    assert match, name
    end = source.index("\n}", match.end()) + 2
    functions.append(source[match.start():end])

fixture = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef _WIN32
#define DRStricmp _stricmp
#else
#include <strings.h>
#define DRStricmp strcasecmp
#endif
#define C2_HOOK_FASTCALL
#define CARPOCALYPSE2_ASIZE(a) (sizeof(a)/sizeof(*(a)))
typedef char tPath_name[256];
typedef struct { const char* identifier; int value; } br_pixelmap;
static br_pixelmap maps[2][3] = {
    { {"BLANK", 10}, {"65", 11}, {"66", 12} },
    { {"BLANK", 20}, {"65", 21}, {"66", 22} }
};
static br_pixelmap loose = { "loose", 30 };
static br_pixelmap* gPixelmap_buffer[16];
static int gPixelmap_buffer_size, probes, closes, frees, decodes;
#ifndef CARPOCALYPSE2_MATCHING
static tPath_name gLoaded_pixies_path;
#endif
static void PathCat(char* out, const char* a, const char* b) { sprintf(out, "%s/%s", a, b); }
static FILE* PFfopen(const char* path, const char* mode) {
    assert(!strcmp(mode, "rb")); ++probes;
    return strstr(path, "loose/") ? NULL : (FILE*)1;
}
static void PFfclose(FILE* f) { assert(f == (FILE*)1); ++closes; }
static int BrPixelmapLoadMany(const char* path, br_pixelmap** out, size_t n) {
    int i, set = path[0] == 'B'; assert(n >= 3); ++decodes;
    for (i=0; i<3; ++i) out[i] = &maps[set][i];
    return 3;
}
static br_pixelmap* DRImageLoad(const char* path) { assert(strstr(path,"loose/")); return &loose; }
static void BrPixelmapFree(br_pixelmap* p) { assert(p); ++frees; }
'''
checks = r'''
int main(void) {
    assert(GetThisFuckingPixelmap("A", "BLANK.PIX", 1) == &maps[0][0]);
    assert(GetThisFuckingPixelmap("A", "65.PIX", 0) == &maps[0][1]);
    assert(GetThisFuckingPixelmap("A", "66.pix", 0) == &maps[0][2]);
    assert(GetThisFuckingPixelmap("A", "67.PIX", 0) == NULL);
    /* A different path must still probe, even with a populated decoded set. */
    assert(GetThisFuckingPixelmap("B", "65.PIX", 0) == &maps[0][1]);
    KillThePixies(); assert(frees == 3 && gPixelmap_buffer_size == 0);
    assert(GetThisFuckingPixelmap("A", "65.PIX", 0) == NULL);
    assert(GetThisFuckingPixelmap("B", "BLANK.PIX", 1) == &maps[1][0]);
    assert(GetThisFuckingPixelmap("B", "66.PIX", 0) == &maps[1][2]);
    KillThePixies(); assert(frees == 6);
    assert(GetThisFuckingPixelmap("loose", "BLANK.PIX", 1) == &loose);
    assert(GetThisFuckingPixelmap("loose", "65.PIX", 0) == &loose);
    assert(decodes == 2 && closes == probes - 2);
    printf("PASS probes=%d frees=%d decodes=%d\n", probes, frees, decodes);
    return 0;
}
'''

cc = sys.argv[1] if len(sys.argv) > 1 else ("cl" if sys.platform == "win32" else "cc")
with tempfile.TemporaryDirectory(prefix="font-probe-") as temp:
    temp = pathlib.Path(temp)
    c = temp / "fixture.c"
    c.write_text(fixture + "\n".join(functions) + checks)
    outputs = []
    for legacy in (True, False):
        exe = temp / ("legacy.exe" if legacy else "cached.exe")
        define = ["-DCARPOCALYPSE2_MATCHING"] if legacy else []
        if pathlib.Path(cc).stem.lower() == "cl":
            command = [cc, "/nologo", "/O2", *define, str(c), "/Fe:" + str(exe)]
        else:
            command = [cc, "-O2", "-g", "-fsanitize=address,undefined", *define, str(c), "-o", str(exe)]
        subprocess.run(command, cwd=temp, check=True, capture_output=True)
        output = subprocess.check_output([str(exe)], text=True).strip()
        print(("legacy: " if legacy else "cached: ") + output)
        outputs.append(int(re.search(r"probes=(\d+)", output)[1]))
    assert outputs[1] < outputs[0]
