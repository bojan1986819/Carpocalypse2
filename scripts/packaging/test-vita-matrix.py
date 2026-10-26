#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2];c=root/'src/brender/drivers/commonrend'
def function(text,signature):
 a=text.index(signature);b=text.index('{',a)+1;d=1
 while d:d+=(text[b]=='{')-(text[b]=='}');b+=1
 return text[a:b]
with tempfile.TemporaryDirectory(prefix='c2-matrix-') as d:
 p=Path(d);(p/'drv.h').write_text('');(p/'state.h').write_text((c/'state.h').read_text())
 prefix=(root/'scripts/packaging/tests/vita_state_main.c').read_text().split('void StateGLCopy')[0]
 (p/'prefix.h').write_text(prefix)
 math=(root/'src/brender/core/math/matrix4.c').read_text();names=['BrMatrix4Copy','BrMatrix4Mul','BrMatrix4Identity','Determinant3','BrMatrix4Determinant','BrMatrix4Adjoint','BrMatrix4Inverse','BrMatrix4Copy34']
 funcs=[]
 import re
 for name in names:
  match=re.search(r'(?:void|br_scalar) C2_HOOK_\w+ '+name+r'\(',math);funcs.append(function(math,match.group()))
 more=[(root/'src/brender/core/math/matrix34.c','void C2_HOOK_CDECL BrMatrix34Mul('),(root/'src/brender/drivers/glrend/compat/compat.c','void C2_HOOK_CDECL BrMatrix4Transpose('),(root/'src/brender/drivers/glrend/video.c','br_matrix4* VIDEOI_D3DtoGLProjection(')]
 funcs += [function(path.read_text(),sig) for path,sig in more]
 (p/'native_math.h').write_text('\n'.join(f[:f.index('{')]+';' for f in funcs)+'\n'+'\n'.join(funcs))
 cache=(c/'cache.c').read_text();s='\n'.join(function(cache,sig) for sig in ['static void UpdateMatrices(','static br_vector4 EyeInModel(','void BREND_FN(State, UpdateModel)('])
 reset=function(cache,'void BREND_FN(State, Reset)(').split('    BrVector4Set(&cache->scene')[0]+'}\n'
 (p/'model.h').write_text(s+'\n'+reset)
 for arch in [[],['-m32']]:
  subprocess.run(['gcc',*arch,'-std=c11','-O2','-g','-ffp-contract=off','-fsanitize=address,undefined','-fno-pie','-no-pie','-I'+str(p),str(root/'scripts/packaging/tests/vita_matrix_main.c'),'-lm','-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
