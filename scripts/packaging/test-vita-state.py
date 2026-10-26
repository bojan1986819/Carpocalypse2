#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2];c=root/'src/brender/drivers/commonrend'
def function(text, signature):
 a=text.index(signature);b=text.index('{',a)+1;d=1
 while d:d+=(text[b]=='{')-(text[b]=='}');b+=1
 return text[a:b]
with tempfile.TemporaryDirectory(prefix='c2-state-') as d:
 p=Path(d);(p/'drv.h').write_text('');(p/'commonrend.h').write_text('')
 (p/'state.h').write_text((c/'state.h').read_text())
 s=(c/'state.c').read_text().replace('#include "drv.h"','#include "state.h"')
 (p/'state.c').write_text(s)
 s=(c/'renderer.c').read_text();(p/'writers.h').write_text('\n'.join(function(s,'static br_error BREND_CMETHOD_DECL(BREND_CLASS(br_renderer), '+f+')') for f in ['partSet','partSetMany']))
 assert 'StateGLTouchLights(&state->default_)' in (c/'state_light.c').read_text()
 for arch in [[],['-m32']]:
  subprocess.run(['gcc',*arch,'-std=c11','-O2','-g','-fsanitize=address,undefined','-fno-pie','-no-pie','-I'+str(p),str(root/'scripts/packaging/tests/vita_state_main.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
