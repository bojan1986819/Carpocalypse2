#!/usr/bin/env python3
"""Run unchanged production bundle/marker logic with a mock setup display.
Uses real host files and production copy code. Does not run SDL or Vita/game.
"""
import ctypes
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
source = (root / "src/carma2/platform/sdl3/vita_platform.c").read_text()
def function(name):
    match = re.search(r"^static int " + name + r"\([^\n]*\) \{", source, re.M)
    assert match, name
    end = match.end()
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[match.start():end]

types = re.search(r"typedef struct \{[^}]*\} C2VitaInstallDisplay;", source).group()
prefix = r"""
#include "vita_data.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define C2VitaLog(...) fprintf(stderr,__VA_ARGS__)
typedef void SDL_Window;
typedef void SDL_Renderer;
typedef uint64_t Uint64;
#define SDL_WINDOW_FULLSCREEN 1
static const char* data_hint;
static int cancelled, calls;
void* SDL_CreateWindow(const char* name, int w, int h, int flags) {return NULL;}
void* SDL_CreateRenderer(void* w, const char* name) {return NULL;}
void SDL_SetRenderScale(void* r, int x, int y) {}
void SDL_DestroyRenderer(void* r) {}
void SDL_DestroyWindow(void* w) {}
"""
progress = r"""
static int install_progress(void* context, const char* path, unsigned int bytes) {
    ((C2VitaInstallDisplay*)context)->copied += bytes;
    calls++;
    return !cancelled;
}
"""
exports = r"""
int test_run(void) {return prepare_bundled_data();}
void test_cancel(int value) {cancelled=value;calls=0;}
int test_calls(void) {return calls;}
"""
with tempfile.TemporaryDirectory(prefix="c2-vita-bootstrap-") as tmp:
    tmp = Path(tmp)
    fixture = tmp / "bootstrap.c"
    fixture.write_text(prefix + types + progress + function("read_bundle_id") + "\n" + function("prepare_bundled_data") + exports)
    library = tmp / "bootstrap.so"
    subprocess.run(["gcc", "-std=c11", "-O2", "-Wall", "-Werror", "-shared", "-fPIC", "-I" + str(root / "src/carma2/platform/sdl3"), str(fixture), str(root / "src/carma2/platform/sdl3/vita_data.c"), "-o", str(library)], check=True)
    dll = ctypes.CDLL(str(library))
    before = Path.cwd()
    os.chdir(tmp)
    try:
        # On the host the app0: mount is represented by a local test directory.
        assert dll.test_run() == 1 and not Path("C2BUNDLE.ID").exists()
        payload = Path("app0:/bundled")
        (payload / "DATA").mkdir(parents=True)
        (payload / "DATA/DATA.TWT").write_bytes(b"archive")
        assert dll.test_run() == 0  # malformed/missing identity cannot mark success
        (payload / "bundle.id").write_text("a" * 64)
        # Missing music: DATA can complete but marker must stay absent.
        assert dll.test_run() == 0 and not Path("C2BUNDLE.ID").exists()
        (payload / "MUSIC").mkdir()
        (payload / "MUSIC/Track02.ogg").write_bytes(b"music")
        Path("DATA/OPTIONS.TXT").write_text("my settings")
        (payload / "DATA/OPTIONS.TXT").write_text("default")
        dll.test_cancel(1)
        assert dll.test_run() == 0 and not Path("C2BUNDLE.ID").exists()
        dll.test_cancel(0)
        assert dll.test_run() == 1
        assert Path("C2BUNDLE.ID").read_text() == "a" * 64
        assert Path("MUSIC/Track02.ogg").read_bytes() == b"music"
        assert Path("DATA/OPTIONS.TXT").read_text() == "my settings"
        dll.test_cancel(0)
        assert dll.test_run() == 1 and dll.test_calls() == 0  # fast next launch
        # Different bundle retries missing files but keeps installed user files.
        (payload / "bundle.id").write_text("b" * 64)
        (payload / "DATA/new.bin").write_bytes(b"new")
        assert dll.test_run() == 1 and Path("DATA/new.bin").read_bytes() == b"new"
        assert Path("C2BUNDLE.ID").read_text() == "b" * 64
        assert Path("DATA/OPTIONS.TXT").read_text() == "my settings"
    finally:
        os.chdir(before)
    print("PASS production bundle bootstrap: small VPK, invalid ID, incomplete/cancelled setup, retry, preserved settings, completion marker/fast boot and new bundle; mock display, host files only")
