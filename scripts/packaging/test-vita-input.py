#!/usr/bin/env python3
"""Host test of production KeyBegin/PDSetKeyArray with mocked SDL/pad input.
Extracts those unmodified functions to avoid linking unused platform/hardware
routines. This checks polling and release integration, not hardware input.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
sdk = Path(os.environ["VITASDK"])
source = (root / "src/carma2/platform/sdl3/sdl3_input.c").read_text()

def function(name):
    match = re.search(r"^void (?:C2_HOOK_FASTCALL )?" + name + r"\([^\n]*\) \{", source, re.M)
    assert match, name
    end = match.end()
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[match.start():end]

prefix = r"""
#include <SDL3/SDL.h>
#include "vita_controls.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define __vita__ 1
int gASCII_table[151], gASCII_shift_table[151], gScan_code[SDL_SCANCODE_COUNT];
unsigned int gKeys_pressed;
static int keys[151], bindings[77], racing;
static bool keyboard[SDL_SCANCODE_COUNT];
static C2VitaPad pad;
const bool* SDL_GetKeyboardState(int* n) { *n=SDL_SCANCODE_COUNT;return keyboard; }
void SDL3ServiceMessages(void) {}
void C2VitaReadKeys(int* k,int mark) { C2VitaMapKeys(&pad,racing,bindings,k,mark); }
"""
suffix = r"""
static void poll(unsigned b,int race,int mark) {
    pad.buttons=b;racing=race;PDSetKeyArray(keys,mark);
}
static void released(void) {
    int i;poll(0,racing,1);for(i=0;i<151;i++)assert(keys[i]==0);
}
int main(void) {
    int i;
    unsigned buttons[]={C2V_R,C2V_L,C2V_LEFT,C2V_RIGHT,C2V_CROSS,C2V_SQUARE,C2V_CIRCLE,C2V_TRIANGLE,C2V_SELECT,C2V_UP,C2V_DOWN};
    int actions[]={49,50,47,48,54,45,60,72,57,55,56};
    KeyBegin();for(i=0;i<77;i++)bindings[i]=i+20;
    /* Unmapped scan-code slot specifically exercises native key clearing. */
    bindings[54]=62;assert(gScan_code[12+62]==0);
    for(i=0;i<11;i++) {
        poll(buttons[i],1,1);assert(keys[bindings[actions[i]]]==1);
        poll(buttons[i],1,1);assert(keys[bindings[actions[i]]]==1);
        released();
    }
    poll(C2V_CROSS,1,1);assert(keys[62]==1);
    poll(C2V_CROSS,0,1);assert(keys[51]==1 && keys[62]==0);released();
    poll(C2V_CIRCLE,0,1);assert(keys[63]==1);released();
    pad.lx=80;poll(0,0,1);assert(keys[71]==1);pad.lx=0;released();
    pad.rx=-80;poll(0,1,1);assert(keys[bindings[31]]==1);pad.rx=0;released();
    poll(C2V_R|C2V_SQUARE,1,1);assert(keys[bindings[49]]==1 && keys[bindings[45]]==1);released();
    poll(C2V_START,1,1);assert(keys[63]==1);released();
    poll(C2V_R,1,1);poll(C2V_R,1,2);assert(keys[bindings[49]]==2);
    poll(0,1,2);assert(keys[bindings[49]]==0);
    puts("PASS production SDL key polling: 11 button holds/releases, unmapped scan slot, menu/race transition, stick/combos/Start, poll marks; mocked input, no game/hardware");
}
"""
with tempfile.TemporaryDirectory(prefix="c2-vita-input-") as tmp:
    tmp = Path(tmp)
    fixture = tmp / "input.c"
    fixture.write_text(prefix + function("KeyBegin") + "\n" + function("PDSetKeyArray") + suffix)
    includes = [tmp / "include", root / "src/carma2/platform/sdl3", root / "src/carma2/s3/include", root / "src/carma2/include", root / "src/carma2/hooks", root / "src/brender", root / "src/brender/include"]
    # Only SDL headers come from the SDK; target libc headers must not reach host GCC.
    sdl_headers = tmp / "include"
    sdl_headers.mkdir()
    (sdl_headers / "SDL3").symlink_to(sdk / "arm-vita-eabi/include/SDL3", target_is_directory=True)
    subprocess.run(["gcc", "-std=c11", *["-I" + str(p) for p in includes], str(fixture), str(root / "src/carma2/platform/sdl3/vita_controls.c"), "-o", str(tmp / "input")], check=True)
    subprocess.run([str(tmp / "input")], check=True)
