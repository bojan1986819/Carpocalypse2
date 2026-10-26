#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'src/carma2/common/mainloop.c').read_text()
a=source.index('                the_time = GetTotalTime() - gTime_bonus_start;')
b=source.index('switch (gTime_bonus_state)',a);end=source.index('{',b)+1;depth=1
while depth:
    depth+=(source[end]=='{')-(source[end]=='}');end+=1
bonus=source[a:end]
source=(root/'src/carma2/common/controls.c').read_text();a=source.index('void C2_HOOK_FASTCALL ToggleTargetLock(void)')
target=source[a:source.index('// Key:',a+10)]
with tempfile.TemporaryDirectory(prefix='c2-bonus-') as d:
    p=Path(d);(p/'bonus.h').write_text(bonus);(p/'target.h').write_text(target)
    # The old decomp text: elapsed time from the state enum and an unsigned (tU32) timer.
    (p/'old').mkdir()
    (p/'old'/'bonus.h').write_text(bonus.replace('GetTotalTime() - gTime_bonus_start;','GetTotalTime() - gTime_bonus_state;',1))
    flags=['gcc','-std=c11','-O2','-g','-fsanitize=address,undefined','-fno-pie','-no-pie']
    for kind in ['bonus','target']:
        subprocess.run([*flags,'-I'+str(p),'-D__vita__',str(root/('scripts/packaging/tests/vita_'+kind+'_main.c')),'-o',str(p/'test')],check=True)
        subprocess.run([str(p/'test')],check=True)
    subprocess.run([*flags,'-I'+str(p/'old'),'-DOLD_UNSIGNED_TIMER',str(root/'scripts/packaging/tests/vita_bonus_main.c'),'-o',str(p/'oldtest')],check=True)
    result=subprocess.run([str(p/'oldtest')],capture_output=True)
    assert result.returncode!=0,'old countdown unexpectedly finished'
    print('PASS regression rejects original runaway bonus state/timer code')
