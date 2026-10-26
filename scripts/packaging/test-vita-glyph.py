#!/usr/bin/env python3
"""Compare production PolyFontText's legacy and batched prepared triangles.

Runs under WSL with 64/32-bit ASan/UBSan; no renderer, SDK or game data needed.
The legacy fixture enlarges only its actor array so long strings have an oracle.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
source = (root / 'src/carma2/common/font.c').read_text()


def function(name):
    start = source.index('br_material* C2_HOOK_FASTCALL ' + name + '(') if name == 'GetPolyFontMaterial' else source.index('void C2_HOOK_FASTCALL ' + name + '(')
    end = source.index('\n// FUNCTION:', start)
    return source[start:end]


with tempfile.TemporaryDirectory(prefix='c2-glyph-') as tmp:
    tmp = Path(tmp)
    (tmp / 'font_functions.h').write_text(
        '#ifndef C2_FONT_PAGE\n#define C2_FONT_PAGE 256\n#endif\n'
        + function('GetPolyFontMaterial') + '\n#undef C2_GLYPH_BATCH\n'
        + function('PolyFontText').replace(' PolyFontText(', ' LegacyPolyFontText(')
        + '\n#define C2_GLYPH_BATCH 1\n' + function('PolyFontText'))
    for arch in [[], ['-m32']]:
        binary = tmp / ('test32' if arch else 'test64')
        includes = ['src/brender/include', 'src/carma2/hooks', 'src/carma2/include',
                    'src/carma2/s3/include', 'src/carma2/platform', str(tmp)]
        subprocess.run(['gcc', *arch, '-std=c11', '-O1', '-g', '-Wall',
                        '-fsanitize=address,undefined', '-fno-pie', '-no-pie',
                        '-DC2_GLYPH_BATCH', '-DCARPOCALYPSE2_FIX_BUGS',
                        *['-I' + str(root / p) for p in includes],
                        str(root / 'scripts/packaging/tests/vita_glyph_main.c'), '-lm', '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0'))
