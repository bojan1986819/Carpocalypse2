#!/usr/bin/env python3
"""Apply audited local fixes to the pinned VitaGL source, idempotently."""
import os,subprocess
from pathlib import Path
source=Path(os.environ['C2_VITAGL_SOURCE'])
pinned='7635e3ceb3c52e34ca2e51d2281628d6c756a35b'
assert subprocess.check_output(['git','-C',str(source),'rev-parse','HEAD'],text=True).strip()==pinned
for patch in sorted((Path(__file__).parent/'vitagl-patches').glob('*.patch')):
    cmd=['git','-C',str(source),'apply']
    check=subprocess.run([*cmd,'--check',str(patch)],capture_output=True)
    if check.returncode==0:
        subprocess.run([*cmd,str(patch)],check=True)
        print('Applied',patch.name)
    else:
        subprocess.run([*cmd,'--reverse','--check',str(patch)],check=True)
        print('Already applied',patch.name)
