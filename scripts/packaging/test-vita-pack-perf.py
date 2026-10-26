#!/usr/bin/env python3
"""Real host streams; production optional cache and scope timings, no game run."""
from pathlib import Path
import subprocess, tempfile
root=Path(__file__).resolve().parents[2]
platform=root/'src/carma2/platform/sdl3'
with tempfile.TemporaryDirectory(prefix='c2-vita-pack-') as directory:
    p=Path(directory);(p/'SDL3').mkdir()
    (p/'SDL3/SDL.h').write_text((root/'scripts/packaging/tests/vita_perf_sdl_mock.h').read_text())
    flags=['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie','-I'+str(p),'-I'+str(platform)]
    subprocess.run([*flags,'-Dfread=pack_test_read','-Dmalloc=pack_test_malloc','-c',str(platform/'vita_pack.c'),'-o',str(p/'pack.o')],check=True)
    subprocess.run([*flags,str(root/'scripts/packaging/tests/vita_pack_perf_main.c'),str(p/'pack.o'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
