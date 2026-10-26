#!/usr/bin/env python3
"""Real host filesystem checks of production Vita-only path cache. No game run."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory(prefix='c2-vita-case-') as d:
 p=Path(d)
 (p/'SDL3').mkdir()
 (p/'SDL3/SDL.h').write_text('''#pragma once
#include <stdbool.h>
#include <stdlib.h>
typedef struct {int value;} SDL_TLSID;
void* SDL_GetTLS(SDL_TLSID*);
bool SDL_SetTLS(SDL_TLSID*,const void*,void (*)(void*));
#define SDL_calloc calloc
#define SDL_free free
''')
 for name in ['ux0:/data/assets','ux0:/data/huge','ux0:/data/long','one','two']:(p/name).mkdir(parents=True)
 (p/'one/file.pix').write_text('one');(p/'two/File.Pix').write_text('two')
 for i in range(500):(p/f'ux0:/data/assets/filler{i}.pix').touch()
 for i in range(1100):(p/f'ux0:/data/huge/Filler{i}.pix').touch()
 (p/('ux0:/data/long/'+('LongName'*12)+'.PiX')).touch()
 subprocess.run(['gcc','-std=gnu11','-D__vita__','-DCARPOCALYPSE2_CASEFIX_WRAP',
  '-I'+str(p),'-Dopendir=case_opendir','-g','-fsanitize=address,undefined','-fno-pie','-no-pie','-pthread',
  '-c',str(root/'src/carma2/platform/sdl3/sdl3_casefix.c'),'-o',str(p/'case.o')],check=True)
 subprocess.run(['gcc','-std=gnu11','-I'+str(p),'-g','-fsanitize=address,undefined','-fno-pie','-no-pie','-pthread',
  str(root/'scripts/packaging/tests/vita_casefix_main.c'),str(p/'case.o'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],cwd=p,check=True)
