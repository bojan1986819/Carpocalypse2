/* Tests production units; all native GL/SDL services below are host mocks. */
#include <vitaGL.h>
#include <SDL3/SDL.h>
#include <brender/brender.h>
#include "vita_gl.h"
#include "vita_video.h"
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int gGLFrameHasWorld;
void glBufferData(GLenum t, GLsizei size, const GLvoid* p, GLenum u) { (void)t;(void)size;(void)p;(void)u; }
void glBufferSubData(GLenum t, GLintptr o, GLsizeiptr n, const GLvoid* p) { (void)t;(void)o;(void)n;(void)p; }
static GLuint texture, vao, ubo, next_texture=7;
static unsigned int pending_mipmap, transfer_finishes;
static unsigned int native_lookups, texture_logs, memory_queries, subimages, mipmaps;
static int compiler_path, resolution_clamped, init_calls, swaps, quads, uploads, deleted, fail_lookup;
static Uint64 ticks=1000,swap_delay;
static char last_perf[1024];
void glDrawElements(GLenum m, GLsizei n, GLenum t, const GLvoid* p) {(void)m;(void)n;(void)t;(void)p;ticks+=5;}
static unsigned int legacy_pool_bytes, legacy_used, attrib_configured;
static char error[256];
static const unsigned short encoding[256]={0};
static const unsigned char glyphs[5]={0xa0,0xa0,0xa0,0xa0,0xa0};
static br_font font={encoding,glyphs};
br_font* BrFontFixed3x5=&font;
void C2VitaLog(const char* format, ...) {
    if (!strncmp(format,"GPU texture ",12)) ++texture_logs;
    if (!strncmp(format,"VGL PERF ",9)) {va_list args;va_start(args,format);vsnprintf(last_perf,sizeof(last_perf),format,args);va_end(args);}
}
size_t vglMemFree(vglMemType type) {
    assert(type==VGL_MEM_VRAM || type==VGL_MEM_RAM);
    ++memory_queries; return 32*1024*1024;
}
int SDL_GetPathInfo(const char* path, SDL_PathInfo* info) {
    assert(info->type==0);
    if ((compiler_path==1 && !strcmp(path,"ur0:/data/libshacccg.suprx")) ||
        (compiler_path==2 && !strcmp(path,"ur0:/data/external/libshacccg.suprx"))) {
        info->type=SDL_PATHTYPE_FILE; return 1;
    }
    return 0;
}
Uint64 SDL_GetTicks(void) { return ticks; }
Uint64 SDL_GetTicksNS(void) {return ticks*1000000;}
int SDL_SetError(const char* text, ...) { snprintf(error,sizeof(error),"%s",text); return 0; }
void vglSetVertexAttribPoolSize(uint32_t main_size, uint32_t aux_size) { assert(main_size==256*1024 && aux_size==1024); ++attrib_configured; }
void vglSetupScratchMemory(GLboolean dynamic, GLboolean stream) { assert(dynamic && stream); }
GLboolean vglInitExtended(int pool, int w, int h, int threshold, SceGxmMultisampleMode msaa) {
    assert(pool==64*1024 && w==960 && h==544 && threshold==32*1024*1024 && msaa==0);
    assert(attrib_configured==(unsigned)init_calls+1);
    legacy_pool_bytes=pool;
    ++init_calls; return resolution_clamped;
}
void vglSwapBuffers(GLboolean dialog) { assert(!dialog); ++swaps; legacy_used=0; ticks+=swap_delay; }
const GLubyte* glGetString(GLenum name) {
    if (name==GL_VERSION) return (const GLubyte*)"OpenGL ES 2.0 VitaGL";
    if (name==GL_EXTENSIONS) return (const GLubyte*)"GL_EXT_texture_filter_anisotropic";
    return (const GLubyte*)"mock VitaGL";
}
void glGetIntegerv(GLenum name, GLint* value) {
    if (name==GL_TEXTURE_BINDING_2D) *value=texture;
    else if (name==GL_NUM_EXTENSIONS) *value=0;
    else *value=123;
}
void glGenTextures(GLsizei count, GLuint* ids) { while(count--) *ids++=next_texture++; }
void glDeleteTextures(GLsizei n, const GLuint* ids) { assert(n==1 && *ids); ++deleted; }
void glBindTexture(GLenum target, GLuint id) { assert(target==GL_TEXTURE_2D); texture=id; }
void glTexParameteri(GLenum target, GLenum name, GLint value) {(void)target;(void)name;(void)value;}
void glBindBuffer(GLenum target, GLuint id) { if(target==GL_UNIFORM_BUFFER) ubo=id; }
typedef struct {int unused;} vbo;
static vbo* ubo_buf[14];
static GLint ubo_offset[14];
#define UBOS_NUM 14
#define THREAD_SAFE() ((void)0);
#define SET_GL_ERROR_WITH_VALUE(err,value) do {assert(0);return;} while(0);
#include "native_base.h"
#undef THREAD_SAFE
#undef SET_GL_ERROR_WITH_VALUE

void C2VitaPackCacheLog(void) {}
void glDeleteVertexArrays(GLsizei n, const GLuint* ids) {(void)n;(void)ids;}
void glBlitFramebuffer(GLint a,GLint b,GLint c,GLint d,GLint e,GLint f,GLint g,GLint h,GLbitfield mask,GLenum filter) {(void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)mask;(void)filter;}
void glGenVertexArrays(GLsizei n, GLuint* ids) {while(n--) *ids++=100;}
void glBindVertexArray(GLuint id) { vao=id; }
GLuint glCreateShader(GLenum type) { assert(type==GL_CG_VERTEX_SHADER_EXT || type==GL_CG_FRAGMENT_SHADER_EXT); return type; }
void glBindFramebuffer(GLenum target, GLuint id) { assert(target==GL_FRAMEBUFFER && id==0); }
void glUseProgram(GLuint id) { assert(!id); }
void glViewport(GLint x, GLint y, GLsizei w, GLsizei h) { assert(!x && !y && w==960 && h==544); }
void glDisable(GLenum cap) {(void)cap;}
void glEnable(GLenum cap) {(void)cap;}
void glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { assert(!r&&!g&&!b&&a==1); }
void glClear(GLbitfield mask) {assert(mask==GL_COLOR_BUFFER_BIT);}
void glMatrixMode(GLenum mode) {(void)mode;}
void glLoadIdentity(void) {}
void glOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
    assert(l==0 && r==960 && b==544 && t==0 && n==-1 && f==1);
}
void glActiveTexture(GLenum unit) {assert(unit==GL_TEXTURE0);}
void glBlendFunc(GLenum src, GLenum dst) {assert(src==GL_SRC_ALPHA && dst==GL_ONE_MINUS_SRC_ALPHA);}
void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {assert(r==1&&g==1&&b==1&&a==1);}
void glBegin(GLenum mode) {assert(mode==GL_QUADS);++quads;}
void glEnd(void) {}
void glTexCoord2f(GLfloat u, GLfloat v) {assert(u>=0 && u<=1 && v>=0 && v<=1);}
void glVertex2f(GLfloat x, GLfloat y) {
    assert(x>=0 && x<=960 && y>=0 && y<=544);
    legacy_used += 24 * sizeof(float); /* Native LEGACY_VERTEX_STRIDE. */
    assert(legacy_used <= legacy_pool_bytes);
}
void glTexImage2D(GLenum target, GLint level, GLint internal, GLsizei w, GLsizei h,
    GLint border, GLenum format, GLenum type, const GLvoid* data) {
    assert(!pending_mipmap); /* Upload/replacement must never race queued writes. */
    const Uint8* pixels=data;
    assert(target==GL_TEXTURE_2D && !level && internal==GL_RGBA && w==128 && h==128);
    assert(!border && format==GL_RGBA && type==GL_UNSIGNED_BYTE && data);
    assert(pixels[3]==255 && pixels[7]==0 && pixels[11]==255); ++uploads;
}
void glTexSubImage2D(GLenum target, GLint level, GLint x, GLint y, GLsizei w,
    GLsizei h, GLenum format, GLenum type, const GLvoid* data) {
    assert(target==GL_TEXTURE_2D && level==1 && x==2 && y==3 && w==4 && h==5);
    assert(format==GL_RGB && type==GL_UNSIGNED_SHORT_5_6_5 && data);
    assert(!pending_mipmap);
    ++subimages; ticks+=7;
}
void glGenerateMipmap(GLenum target) {
    assert(target==GL_TEXTURE_2D && !pending_mipmap);
    pending_mipmap=1; ++mipmaps; ticks+=13;
}
int sceGxmTransferFinish(void) {
    assert(pending_mipmap);
    pending_mipmap=0; ++transfer_finishes;
    return 0;
}
static void dummy(void) {}
void* vglGetProcAddress(const char* name) {
    ++native_lookups;
    assert(strcmp(name,"glGetPointerv") && strcmp(name,"glVertexAttrib4Nusv"));
    if(fail_lookup && !strcmp(name,"glCheckFramebufferStatus")) return NULL;
#define PROC(n) if (!strcmp(name,#n)) return (void*)n
    PROC(glGetString); PROC(glGetIntegerv);
#undef PROC
    return (void*)dummy;
}

int main(int argc, char** argv) {
    GLint value; GLuint ids[2]; int previous; Uint64 saved_ticks;
    (void)argv; resolution_clamped = argc > 1;
    assert(!C2VitaVideoInit(NULL) && strstr(error,"libshacccg.suprx") && !init_calls && !swaps);
    compiler_path=2; fail_lookup=1;
    assert(!C2VitaVideoInit(NULL) && strstr(error,"required") && !swaps);
    fail_lookup=0;
    assert(C2VitaVideoInit(NULL) && init_calls==2 && swaps==1 && uploads==1 && quads==3);
    memory_queries=0;
    assert(C2VitaGLValidate());
    previous=native_lookups;
    assert(!C2VitaGLGetProcAddress("glGetPointerv"));
    assert(!C2VitaGLGetProcAddress("glVertexAttrib4Nusv"));
    assert(native_lookups==(unsigned)previous);
    assert(gladLoadGLLoader(C2VitaGLGetProcAddress));
    assert(GLVersion.major==2 && GLVersion.minor==0 && !GLAD_GL_EXT_texture_filter_anisotropic);
    assert(glad_glGenFramebuffers && glad_glFramebufferRenderbuffer && glad_glBindVertexArray && glad_glUniformBlockBinding);
    /* Tracing must preserve calls/bindings, stop logging at the bound, and
     * add no memory-query/log work once disabled. */
    glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,128,128,0,GL_RGBA,GL_UNSIGNED_BYTE,
        (const Uint8[]){0,0,0,255,0,0,0,0,0,0,0,255});
    assert(uploads==2 && !memory_queries && !texture_logs);
    saved_ticks=ticks;
    C2VitaGLTraceTextures(1);
    glad_glTexSubImage2D(GL_TEXTURE_2D,1,2,3,4,5,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,ids);
    assert(subimages==1 && memory_queries==2 && texture_logs==2);
    for (previous=0;previous<513;++previous) glad_glGenerateMipmap(GL_TEXTURE_2D);
    assert(mipmaps==513 && texture_logs==1024 && memory_queries==1024);
    C2VitaGLTraceTextures(0);
    glad_glGenerateMipmap(GL_TEXTURE_2D);
    assert(mipmaps==514 && texture_logs==1024 && memory_queries==1024);
    assert(transfer_finishes==mipmaps && !pending_mipmap);
    ticks=saved_ticks;
    glad_glGenTextures(2,ids);
    texture=ids[0];
    glad_glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,&value); assert(value==GL_NEAREST_MIPMAP_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    texture=ids[1];
    glad_glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,&value); assert(value==GL_NEAREST_MIPMAP_LINEAR);
    texture=ids[0];
    glad_glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,&value); assert(value==GL_LINEAR);
    glad_glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,&value); assert(value==GL_CLAMP_TO_EDGE);
    /* A reused native texture ID must regain defaults, not old sampler state. */
    next_texture=ids[0]; glad_glGenTextures(1,ids);
    glad_glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,&value); assert(value==GL_NEAREST_MIPMAP_LINEAR);
    glad_glBindBuffer(GL_UNIFORM_BUFFER,12);
    glad_glGetIntegerv(GL_UNIFORM_BUFFER_BINDING,&value); assert(value==12 && ubo==12);
    glad_glBindBufferBase(GL_UNIFORM_BUFFER,1,34);
    glad_glGetIntegerv(GL_UNIFORM_BUFFER_BINDING,&value); assert(value==34 && ubo==34 && (uintptr_t)ubo_buf[1]==34);
    glad_glBindVertexArray(99);
    glad_glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&value); assert(value==99 && vao==99);
    assert(glad_glCreateShader(GL_VERTEX_SHADER)==GL_CG_VERTEX_SHADER_EXT);
    assert(glad_glCreateShader(GL_FRAGMENT_SHADER)==GL_CG_FRAGMENT_SHADER_EXT);
    C2VitaGLResetBindings();
    glad_glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&value); assert(value==0 && !vao);
    glad_glGetIntegerv(GL_UNIFORM_BUFFER_BINDING,&value); assert(value==0 && !ubo);
    C2VitaVideoLoadingProgress(); assert(swaps==1);
    ticks+=101; C2VitaVideoLoadingProgress(); assert(swaps==2);
    C2VitaVideoPreparingMenu(); assert(swaps==3);
    C2VitaVideoSwap(0); assert(swaps==4);
    C2VitaGLFrameStats(999,0,0,0); /* reset initialization counters */
    C2VitaGLRecordOverlayCheck(1,1234);
    glad_glGenVertexArrays(2,ids);
    glad_glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,NULL);
    glad_glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,NULL);
    swap_delay=9;
    C2VitaVideoSwap(1); assert(swaps==5);
    assert(strstr(last_perf,"draws=2 ") && strstr(last_perf,"draw_ms=10 swap_ms=9 vao_new=2"));
    assert(strstr(last_perf,"overlay_checks=1 overlay_empty=1 overlay_us=1"));
    swap_delay=0;
    previous=quads; ticks+=200; C2VitaVideoLoadingProgress(); assert(quads==previous && swaps==5);
    C2VitaVideoShutdown(); C2VitaVideoShutdown(); assert(deleted==1);
    C2VitaVideoSwap(1); assert(swaps==5);
    puts("PASS production VitaGL adapter/GLAD loader/loading: zero/fallback init result, compiler prerequisite/failure, required API check, native shaders, sampler reuse, UBO/VAO state, loading atlas/pool capacity, throttle/swap/shutdown; ASan/UBSan; GL/SDL mocked");
    return 0;
}

void C2VitaPerfShutdown(void) {} /* monitor lifecycle tested by pack/perf fixture */
