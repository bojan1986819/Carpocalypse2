/* Production empty-overlay handling; GL services mocked, no GPU execution. */
#include <glad/glad.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "overlay_scan.h"
#define BRT_OFFSCREEN 1
#define BRT_DEPTH 2
#define OVERLAY_KEY 0xf81f
#define GL_CHECK_ERROR() ((void)0)
typedef uint16_t br_uint_16;
typedef struct br_device_pixelmap {
    int use_type, pm_width, pm_height;
    struct br_device_pixelmap *root, *screen;
    struct {void* lockedPixels; GLuint overlayTexture, glFbo; int possiblyDirty;} asBack;
    struct {struct br_device_pixelmap* backbuffer;} asDepth;
} br_device_pixelmap;
static unsigned uploads, draws, overlay_checks, overlay_empty, benchmarks;
unsigned long long C2VitaGLTicksNS(void) {static unsigned long long clock;return ++clock*1000;}
void C2VitaGLRecordOverlayCheck(int empty,unsigned long long ns) {assert(ns==1000);++overlay_checks;overlay_empty+=empty;}
void C2VitaGLLogOverlayBenchmark(unsigned n,unsigned long long scalar,unsigned long long vector,int valid) {assert(n==640*480 && scalar==4000 && vector==4000 && valid);++benchmarks;}
static void GLBackViewport(br_device_pixelmap* p,int* x,int* y,int* w,int* h) {
    *x=*y=0; *w=p->pm_width; *h=p->pm_height;
}
static void GLBackDrawTexture(br_device_pixelmap* p,GLuint tex,int x,int y,int w,int h,int flip,int key) {
    (void)p; assert(tex==7 && !x && !y && w==640 && h==480 && !flip && key); ++draws;
}
static void bind_texture(GLenum t,GLuint id) {assert(t==GL_TEXTURE_2D && (id==7 || !id));}
static void bind_framebuffer(GLenum t,GLuint id) {assert(t==GL_FRAMEBUFFER && (id==8 || !id));}
static void subimage(GLenum t,GLint l,GLint x,GLint y,GLsizei w,GLsizei h,GLenum f,GLenum ty,const void* data) {
    assert(t==GL_TEXTURE_2D && !l && !x && !y && w==640 && h==480 && f==GL_RGB && ty==GL_UNSIGNED_SHORT_5_6_5);
    assert(((const br_uint_16*)data)[12345]==0x1234); ++uploads;
}
#include "overlay_production.h"
int main(void) {
    static br_uint_16 pixels[640*480];
    br_device_pixelmap root={.use_type=BRT_OFFSCREEN,.pm_width=640,.pm_height=480};
    br_device_pixelmap sub={.root=&root};
    root.asBack.lockedPixels=pixels;root.asBack.overlayTexture=7;root.asBack.glFbo=8;
    glad_glBindTexture=bind_texture;glad_glBindFramebuffer=bind_framebuffer;glad_glTexSubImage2D=subimage;
    for (unsigned i=0;i<640*480;++i) pixels[i]=OVERLAY_KEY;
    benchmark_overlay_check(pixels,640*480);benchmark_overlay_check(pixels,640*480);assert(benchmarks==1);
    for (unsigned i=0;i<1000;++i) GLBackFlushOverlay(&sub);
    assert(!uploads && !draws && overlay_checks==1000 && overlay_empty==1000);
    /* Direct writes through permanently mapped pixels must survive even
     * without a dirty flag or lock notification. */
    assert(!root.asBack.possiblyDirty); pixels[12345]=0x1234;
    GLBackFlushOverlay(&sub);assert(uploads==1 && draws==1);
    for(unsigned i=0;i<640*480;++i) assert(pixels[i]==OVERLAY_KEY);
    GLBackFlushOverlay(&sub);assert(uploads==1 && draws==1);
    puts("PASS production overlay:1000empty flushes issue no GPU uploads/draws; unmarked mapped CPU write uploads once and is cleared; root/submap;ASan/UBSan; GL mocked");
}
