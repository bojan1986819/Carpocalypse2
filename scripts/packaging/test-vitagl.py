#!/usr/bin/env python3
"""Host checks of production VitaGL adapter/loader/loading and FBO allocation.
Native GL, SDL and BRender font services are mocked: no GPU shader compilation,
game launch or Vita performance claim. VITAGL_SOURCE selects pinned library.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
vgl = Path(os.environ['C2_VITAGL_SOURCE']) / 'source'
glroot = root / 'src/brender/drivers/glrend'
platform = root / 'src/carma2/platform/sdl3'
header = (vgl / 'vitaGL.h').read_text()
lookup = (vgl / 'lookup.c').read_text()
initializer = (vgl / 'vgl.c').read_text()
assert 'GLboolean res_fallback = GL_FALSE;' in initializer
assert 'return res_fallback;' in initializer
assert 'return vglInitWithCustomThreshold(pool_size, width, height, ram_threshold,' in initializer
assert re.search(r'#define LEGACY_VERTEX_STRIDE\s+24', (vgl / 'shared.h').read_text())
assert 'if (legacy_pool_size)' in (vgl / 'gxm.c').read_text()
assert 'legacy_pool_ptr[0] = x;' in (vgl / 'ffp.c').read_text()


source = (platform / 'vita_gl.c').read_text()
required = set(re.findall(r'^        "(gl\w+)",', source, re.M))
native = set(re.findall(r'\{"(gl\w+)"', lookup))
wrapped = set(re.findall(r'PROC\("(gl\w+)"', source))
assert not required - native - wrapped, required - native - wrapped
# Audit commonrend too: its framebuffer code is compiled into glrend.
used = set()
for p in [*glroot.glob('*.c'), *(glroot / 'compat').glob('*.c'),
          *(glroot.parent / 'commonrend').glob('*.c')]:
    if p.name == 'glad.c': continue
    text = re.sub(r'/\*.*?\*/|//[^\n]*', '', p.read_text(), flags=re.S)
    used.update(re.findall(r'\b(gl[A-Z]\w+)\s*\(', text))
# First is local helper; other three are excluded/disabled in Vita paths.
excluded = {'glContextIsOpenGLES', 'glDetachShader', 'glDrawBuffers', 'glGetTexParameterfv'}
assert not used - excluded - required, used - excluded - required

fixture = root / 'scripts/packaging/tests/vitagl_main.c'
# Use project GLAD types/constants and exact native declarations; undefine the
# GLAD remaps so production native units call the mocked native functions.
needed = set(re.findall(r'\b((?:gl|vgl)[A-Z]\w+)\s*\(',
    source + (platform / 'vita_video.c').read_text() + fixture.read_text()))
needed -= {'gladLoadGLLoader'}
prototypes = []
for name in sorted(needed):
    match = re.search(r'^[\w *]+\b' + name + r'\([^;\n]*\);', header, re.M)
    if match: prototypes.append(match.group())
remaps = re.findall(r'^#define (gl\w+) ', (glroot / 'include/glad/glad.h').read_text(), re.M)
memtype = re.search(r'typedef enum \{\s*VGL_MEM_VRAM,.*?\} vglMemType;', header, re.S).group()
extra = re.findall(r'^#define GL_(?:CG_VERTEX_SHADER_EXT|CG_FRAGMENT_SHADER_EXT|QUADS|PROJECTION|MODELVIEW)\s+[^\n]*', header, re.M)
with tempfile.TemporaryDirectory(prefix='c2-vitagl-') as directory:
    tmp = Path(directory)
    (tmp / 'SDL3').mkdir(); (tmp / 'brender').mkdir(); (tmp / 'psp2').mkdir()
    (tmp / 'psp2/gxm.h').write_text('#pragma once\nint sceGxmTransferFinish(void);\n')
    (tmp / 'vitaGL.h').write_text('#pragma once\n#include <stddef.h>\n#include <glad/glad.h>\n' + memtype + '\n' +
        ''.join('#undef '+name+'\n' for name in remaps) + '\n'.join(extra) +
        '\ntypedef int SceGxmMultisampleMode;\n#define SCE_GXM_MULTISAMPLE_NONE 0\n' +
        '\n'.join(prototypes))
    (tmp / 'SDL3/SDL.h').write_text('''#pragma once
#include <stdint.h>
typedef uint8_t Uint8; typedef uint64_t Uint64; typedef void SDL_Window;
typedef struct { int type; } SDL_PathInfo;
#define SDL_PATHTYPE_FILE 1
int SDL_GetPathInfo(const char*, SDL_PathInfo*);
Uint64 SDL_GetTicks(void); Uint64 SDL_GetTicksNS(void); int SDL_SetError(const char*, ...);
''')
    (tmp / 'brender/brender.h').write_text('''#pragma once
typedef struct { const unsigned short* encoding; const unsigned char* glyphs; } br_font;
extern br_font* BrFontFixed3x5;
''')
    text=(vgl / 'custom_shaders.c').read_text()
    start=text.index('void glBindBufferBase(')
    end=text.index('\nvoid ',start+1)
    # The only adaptation is widening a fake host pointer, not binding semantics.
    (tmp / 'native_base.h').write_text(text[start:end].replace('(vbo *)buffer','(vbo *)(uintptr_t)buffer'))
    binary = tmp / 'test'
    subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined', '-fno-pie','-no-pie',
        '-D__vita__', '-I'+str(tmp), '-I'+str(glroot / 'include'), '-I'+str(platform),
        str(fixture), str(platform / 'vita_gl.c'), str(platform / 'vita_video.c'),
        str(glroot / 'glad.c'), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
    subprocess.run([str(binary), 'resolution-clamped'], check=True)
    def body(path, signature):
        text = path.read_text()
        start = text.index(signature)
        end = text.index('{', start) + 1
        depth = 1
        while depth:
            depth += (text[end] == '{') - (text[end] == '}')
            end += 1
        return text[start:end]
    (tmp / 'framebuffer_production.h').write_text(
        body(glroot.parent / 'commonrend/devpixmp.c', 'br_error BREND_FN(DevicePixelmap, RecreateRenderBuffers)') + '\n' +
        body(glroot / 'devpixmp.c', 'void BREND_FN(DevicePixelmap, DeleteResources)'))
    subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-D__vita__', '-I'+str(tmp), '-I'+str(glroot / 'include'),
        str(root / 'scripts/packaging/tests/vitagl_framebuffer_main.c'), str(glroot / 'glad.c'),
        '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)

    present=body(glroot / 'devpmglf.c','br_error BR_CMETHOD_DECL(br_device_pixelmap_glf, doubleBuffer)')
    native_present=re.search(r'#ifdef __vita__\n(.*?)#else',present,re.S).group(1)
    assert 'glBlitFramebuffer(' in native_present and 'GLBackDrawTexture(' not in native_present
    (tmp / 'present_production.h').write_text(native_present)
    (tmp / 'orientation_production.h').write_text(
        body(vgl / 'misc.c','void glViewport(')+'\n'+
        body(vgl / 'shared.h','static inline __attribute__((always_inline)) void vector4f_convert_to_local_space('))
    blit=(vgl / 'framebuffers.c').read_text()
    a=blit.index('\tvector4f position;',blit.index('void glBlitNamedFramebuffer('))
    b=blit.index('\n\tvoid *vertex_buffer;',a)
    (tmp / 'native_blit_coordinates.h').write_text(blit[a:b])
    subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-I'+str(tmp),'-I'+str(glroot / 'include'),str(root / 'scripts/packaging/tests/vitagl_orientation_main.c'),
        '-lm','-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

    (tmp / 'overlay_production.h').write_text(
        body(glroot / 'backbuf.c', 'static void benchmark_overlay_check')+'\n'+
        body(glroot / 'backbuf.c', 'static br_device_pixelmap* root_of')+'\n'+
        body(glroot / 'backbuf.c', 'void GLBackFlushOverlay'))
    subprocess.run(['gcc','-std=c11','-O2','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-D__vita__','-I'+str(tmp),'-I'+str(glroot / 'include'),
        '-I'+str(glroot),str(root / 'scripts/packaging/tests/vitagl_overlay_main.c'),str(glroot / 'glad.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

    (tmp / 'vao_pool_production.h').write_text(
        body(vgl / 'custom_shaders.c','float *reserve_attrib_pool(')+'\n'+
        body(vgl / 'buffers.c','void vglSetVertexAttribPoolSize(')+'\n'+
        body(vgl / 'buffers.c','void reset_vao('))
    subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-I'+str(tmp),str(root / 'scripts/packaging/tests/vitagl_vao_pool_main.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

    geometry=(glroot.parent / 'commonrend/gstored.c')
    (tmp / 'geometry_production.h').write_text(
        body(geometry, 'br_geometry_stored* BREND_FN(GeometryStored, Allocate)')+'\n'+
        body(geometry, 'static void BREND_CMETHOD_DECL(BREND_CLASS(br_geometry_stored), free)'))
    subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',
        '-I'+str(tmp),str(root / 'scripts/packaging/tests/vitagl_geometry_main.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

    text=(glroot / 'transparency.c').read_text()
    (tmp / 'snapshot_production.h').write_text(text[text.index('typedef struct transparent_vertex'):text.index('static void capture_gl')])
    assignment=next(line for line in text.splitlines() if 'memcpy(&draw->scene,' in line)
    (tmp / 'snapshot_copy.h').write_text(assignment)
    for defines in [[],['-D__vita__']]:
        subprocess.run(['gcc','-std=c11','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',*defines,
            '-I'+str(tmp),'-I'+str(glroot / 'include'),'-I'+str(glroot.parent / 'commonrend'),
            str(root / 'scripts/packaging/tests/vitagl_snapshot_main.c'),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)

print(f'PASS native API source coverage: {len(required)} required functions; lookup audit; no game/hardware')
