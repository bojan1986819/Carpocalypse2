#!/usr/bin/env python3
"""Production HUD text redraw equivalence; BRender/GPU draws recorded on host."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
platform=root/'src/carma2/platform/sdl3'
def body(path,signature):
    text=path.read_text();start=text.index(signature);end=text.index('{',start)+1;depth=1
    while depth:
        depth+=(text[end]=='{')-(text[end]=='}');end+=1
    return text[start:end]
font=root/'src/carma2/common/font.c'
code='\n'.join(body(font,s) for s in [
    'br_material* C2_HOOK_FASTCALL GetPolyFontMaterial(',
    'void C2_HOOK_FASTCALL PolyFontText(',
    'void C2_HOOK_FASTCALL DRPixelmapText(',
    'void C2_HOOK_FASTCALL DRPixelmapCleverText('])
code+='\n'+body(root/'src/carma2/common/displays.c','void C2_HOOK_FASTCALL RedrawHeadupTexts(')
cleanup=body(root/'src/carma2/common/graphics1.c','void C2_HOOK_FASTCALL CleanPolyFontDanglers(')
with tempfile.TemporaryDirectory(prefix='c2-vita-text-') as directory:
    p=Path(directory);(p/'brender').mkdir()
    (p/'brender/brender.h').write_text((root/'scripts/packaging/tests/vita_text_brender_mock.h').read_text())
    (p/'text_production.h').write_text(code)
    displays=(root/'src/carma2/common/displays.c').read_text()
    first=body(root/'src/carma2/common/displays.c','void C2_HOOK_FASTCALL DoHeadups(')
    first=first[first.index('                    case eHeadup_coloured_text:'):first.index('                    case eHeadup_image:')]
    (p/'text_first_coloured.h').write_text(first)

    (p/'text_cleanup.h').write_text(cleanup)
    for charmode in ['-fsigned-char','-funsigned-char']:
        subprocess.run(['gcc','-std=c11','-O2','-g','-DCARPOCALYPSE2_FIX_BUGS',charmode,
            '-fsanitize=address,undefined','-fno-pie','-no-pie','-I'+str(p),'-I'+str(platform),
            str(root/'scripts/packaging/tests/vita_text_main.c'),str(platform/'vita_text.c'),'-o',str(p/'test')],check=True)
        subprocess.run([str(p/'test')],check=True)
