#!/usr/bin/env python3
"""Production Vita APO changes vs desktop pixel output, no game or GPU run."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'src/carma2/common/displays.c').read_text()
def body(signature):
    start=source.index(signature);end=source.index('{',start)+1;depth=1
    while depth:
        depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[start:end]
assert 'C2VitaResetHUDCache();' in body('void C2_HOOK_FASTCALL InitHeadups(')
assert 'C2VitaDamageHUDChanged(' in body('void C2_HOOK_FASTCALL DoDamageScreen(')
with tempfile.TemporaryDirectory(prefix='c2-vita-hud-') as directory:
    p=Path(directory)
    (p/'hud_reset_production.h').write_text(body('static void C2VitaResetHUDCache('))
    (p/'apo_production.h').write_text(body('void C2_HOOK_FASTCALL DoPSPowerHeadup('))
    subprocess.run(['gcc','-std=c11','-D__vita__','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-I'+str(p),'-I'+str(root/'src/carma2/platform/sdl3'),str(root/'scripts/packaging/tests/vita_hud_main.c'),
        str(root/'src/carma2/platform/sdl3/vita_hud.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
