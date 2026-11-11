#!/usr/bin/env python3
"""Production GL command queue against recording mock GL calls, host pthreads, ASan/UBSan and TSan."""
from pathlib import Path
import subprocess, tempfile
root = Path(__file__).resolve().parents[2]
platform = root / 'src/carma2/platform/sdl3'
glad = root / 'src/brender/drivers/glrend/include'
with tempfile.TemporaryDirectory(prefix='c2-vita-glqueue-') as directory:
    p = Path(directory)
    (p / 'SDL3').mkdir()
    (p / 'SDL3/SDL.h').write_text((root / 'difftest/vita_glqueue_sdl_mock.h').read_text())
    for sanitizer in ('address,undefined', 'thread'):
        flags = ['gcc', '-std=c11', '-D_GNU_SOURCE', '-g', '-O1', '-Wall', '-Werror',
                 '-fsanitize=' + sanitizer, '-I' + str(p), '-I' + str(platform), '-I' + str(glad)]
        out = p / ('test-' + sanitizer.split(',')[0])
        subprocess.run([*flags, str(platform / 'vita_glqueue.c'), str(root / 'difftest/vita_glqueue_main.c'),
                        '-lpthread', '-o', str(out)], check=True)
        # ThreadSanitizer cannot cope with the high-entropy address layout of newer kernels
        run = ['setarch', subprocess.run(['uname', '-m'], capture_output=True, text=True).stdout.strip(), '-R', str(out)] if sanitizer == 'thread' else [str(out)]
        subprocess.run(run, check=True)
