/* Exact native viewport/blit code and production present block. CPU only. */
#include <glad/glad.h>
#undef glViewport
#undef glBindFramebuffer
#undef glBlitFramebuffer
#include <assert.h>
#include <math.h>
#include <stdio.h>
#define THREAD_SAFE() ((void)0);
#define SET_GL_ERROR(e) do {assert(0);return;} while(0);
#define DISPLAY_HEIGHT 544
#define DISPLAY_WIDTH_FLOAT 960.f
#define DISPLAY_HEIGHT_FLOAT 544.f
static int is_rendering_display,skip_viewport_override;
static float x_scale,x_port,y_scale,y_port,z_scale,z_port;
static void* gxm_context;
typedef struct {int width,height;} framebuffer;
static framebuffer fb={640,480},*in_use_framebuffer=&fb;
static struct {int x,y,w,h;} gl_viewport;
typedef struct {float x,y,z,w;} vector4f;
static void vglSetViewport(void* context,float xp,float xs,float yp,float ys,float zp,float zs) {
    (void)context;(void)xp;(void)xs;(void)yp;(void)ys;(void)zp;(void)zs;
}
#include "orientation_production.h"
static unsigned read_fbo,write_fbo,blits;
static vector4f output_position,output_texcoord;
static void glBindFramebuffer(GLenum target,GLuint id) {
    if(target==GL_READ_FRAMEBUFFER || target==GL_FRAMEBUFFER) read_fbo=id;
    if(target==GL_DRAW_FRAMEBUFFER || target==GL_FRAMEBUFFER) write_fbo=id;
}
static void glBlitFramebuffer(GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,
    GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter) {
    assert(read_fbo==42 && !write_fbo);
    assert(mask==GL_COLOR_BUFFER_BIT && filter==GL_NEAREST);
    unsigned readFramebuffer=read_fbo;
    framebuffer* read_fb=&fb;
    is_rendering_display=1;
    #include "native_blit_coordinates.h"
    output_position=position;output_texcoord=texcoord;++blits;
}
typedef struct {int pm_width,pm_height,scale;struct {GLuint glFbo;} asBack;} br_device_pixelmap;
static int GLBackScale(br_device_pixelmap* p) {return p->scale;}
static void present(br_device_pixelmap* self,br_device_pixelmap* root,int x,int y,float wm,float hm) {
    #include "present_production.h"
}
int main(void) {
    is_rendering_display=0;glViewport(0,0,640,480);
    assert(y_port+y_scale==480 && y_port-y_scale==0);
    is_rendering_display=1;glViewport(0,0,960,544);
    assert(y_port+y_scale==0 && y_port-y_scale==544);
    br_device_pixelmap root={640,480,1,{42}},front={640,480,1,{0}};
    present(&front,&root,0,0,960.f/640,544.f/480);
    assert(blits==1 && !read_fbo && !write_fbo);
    assert(output_position.x==-1 && output_position.y==1);
    assert(output_position.z==1 && output_position.w==-1);
    assert(output_texcoord.x==0 && output_texcoord.y==1);
    assert(output_texcoord.z==1 && output_texcoord.w==0);
    root.scale=2;fb.width=1280;fb.height=960;
    present(&front,&root,8,16,1,1);
    assert(blits==2 && output_texcoord.y==1 && output_texcoord.w==0);
    assert(fabsf(output_position.x-(16.f/960-1))<1e-6f);
    puts("PASS production Vita final-present native FBO blit, scaled source, display destination and binding reset; exact pinned viewport/blit coordinate composition; CPU only, hardware orientation pending");
}
