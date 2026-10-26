#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory(prefix='c2-geo-cache-') as d:
    p=Path(d)
    for arch in [[],['-m32']]:
        subprocess.run(['gcc',*arch,'-std=c11','-O2','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
            '-I'+str(root/'src/brender/drivers/commonrend'),str(root/'scripts/packaging/tests/vita_geometry_cache_main.c'),'-o',str(p/'test')],check=True)
        subprocess.run([str(p/'test')],check=True)
