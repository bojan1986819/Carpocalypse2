#!/usr/bin/env python3
"""32-bit native allocator regression: requires host gcc-multilib/ASan."""
from pathlib import Path
import os,subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=Path(os.environ['C2_VITAGL_SOURCE'])/'source/utils/mem_utils.c'
text=source.read_text()
start=text.index('// Lightweighted mutexes')
end=text.index('\n#endif\n\n#ifdef PHYCONT_ON_DEMAND',start)
with tempfile.TemporaryDirectory(prefix='c2-vgl-heap-') as directory:
    p=Path(directory)
    (p/'native_heap.h').write_text(text[start:end])
    subprocess.run(['gcc','-m32','-O2','-g','-pthread','-fsanitize=address,undefined',
        '-fno-sanitize=alignment','-fno-pie','-no-pie','-I'+str(p),
        str(root/'scripts/packaging/tests/vitagl_heap_main.c'),'-o',str(p/'test')],check=True)
    env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')
    # Native header chunks deliberately live until process termination. The
    # allocator supports unaligned block metadata on Vita; x86 emulates that.
    subprocess.run([str(p/'test')],check=True,env=env)

    baseline=subprocess.check_output(['git','-C',str(source.parents[2]),'show','HEAD:source/utils/mem_utils.c'],text=True)
    for name,broken in [('original',baseline),('old-copy-length',text.replace('vgl_fast_memcpy(newptr, ptr, block->size - MIN_FREE_BLOCK_SIZE);','vgl_fast_memcpy(newptr, ptr, block->size - 4);'))]:
        start=broken.index('// Lightweighted mutexes')
        end=broken.index('\n#endif\n\n#ifdef PHYCONT_ON_DEMAND',start)
        (p/'native_heap.h').write_text(broken[start:end])
        subprocess.run(['gcc','-m32','-O2','-g','-pthread','-fsanitize=address,undefined','-fno-sanitize=alignment',
            '-fno-pie','-no-pie','-I'+str(p),str(root/'scripts/packaging/tests/vitagl_heap_main.c'),'-o',str(p/'test')],check=True)
        result=subprocess.run([str(p/'test')],capture_output=True,env=env)
        assert result.returncode!=0,name+' unexpectedly passed'
        print('PASS regression rejects '+name+' allocator')
