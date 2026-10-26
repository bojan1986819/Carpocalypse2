#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
s=(root/'src/carma2/common/pedestrn.c').read_text();a=s.index('void C2_HOOK_FASTCALL RenderElectroBastardRays(');b=s.index('// FUNCTION:',a+20)
with tempfile.TemporaryDirectory(prefix='c2-ray-') as d:
 p=Path(d);(p/'ray.h').write_text(s[a:b])
 subprocess.run(['gcc','-std=c11','-O2','-g','-fsanitize=address,undefined,float-cast-overflow','-fno-pie','-no-pie','-I'+str(p),str(root/'scripts/packaging/tests/vita_ray_main.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=10)
 old=subprocess.run([str(p/'test'),'old'],capture_output=True,timeout=10)
 assert old.returncode!=0,'old unbounded path unexpectedly passed'
 print('PASS regression rejects original unbounded lightning loop')
