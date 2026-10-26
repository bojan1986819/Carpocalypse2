/* Production framebuffer create/resize/free bodies are included by test-vitagl.py.
 * Native GL resource allocation/completeness services are mocked. */
#include <glad/glad.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define BREND_DRIVER_GL 1
#define BREND_FN(a,b) a##b
#define BRE_OK 0
#define BRE_FAIL 1
#define BRT_OFFSCREEN 1
#define BRT_DEPTH 2
#define GL_CHECK_ERROR() ((void)0)
typedef int br_error;
typedef struct br_device_pixelmap {
    int use_type, pm_width, pm_height; const char* pm_identifier;
    struct {GLuint glFbo, glTex;} asBack;
    struct {struct br_device_pixelmap* backbuffer; GLuint glDepth;} asDepth;
} br_device_pixelmap;
static int colors, depths, delete_depths, delete_colors, framebuffers, complete=1, attachments;
static GLuint framebuffer, renderbuffer;
static int GLBackScale(br_device_pixelmap* pm) {(void)pm;return 1;}
static void BrWarning(const char* fmt, ...) {(void)fmt;}
static void gen_framebuffers(GLsizei n, GLuint* ids) {assert(n==1);*ids=100+ ++framebuffers;}
static void bind_framebuffer(GLenum target, GLuint id) {assert(target==GL_FRAMEBUFFER);framebuffer=id;}
static void gen_textures(GLsizei n, GLuint* ids) {assert(n==1);*ids= ++colors;}
static void bind_texture(GLenum t,GLuint id) {(void)id;assert(t==GL_TEXTURE_2D);}
static void param(GLenum t,GLenum p,GLint v) {(void)t;(void)p;(void)v;}
static void image(GLenum t,GLint l,GLint i,GLsizei w,GLsizei h,GLint b,GLenum f,GLenum ty,const void* data) {
    assert(t==GL_TEXTURE_2D&&!l&&i==GL_RGBA8&&w==640&&h==480&&!b&&f==GL_RGBA&&ty==GL_UNSIGNED_BYTE&&!data);
}
static void attach_texture(GLenum t,GLenum a,GLenum tt,GLuint id,GLint l) {
    assert(t==GL_FRAMEBUFFER&&a==GL_COLOR_ATTACHMENT0&&tt==GL_TEXTURE_2D&&id&&!l&&framebuffer);++attachments;
}
static void gen_renderbuffers(GLsizei n,GLuint* ids) {assert(n==1);*ids=200+ ++depths;}
static void bind_renderbuffer(GLenum t,GLuint id) {assert(t==GL_RENDERBUFFER);renderbuffer=id;}
static void storage(GLenum t,GLenum i,GLsizei w,GLsizei h) {
    assert(t==GL_RENDERBUFFER&&i==GL_DEPTH_COMPONENT24&&w==640&&h==480&&renderbuffer);
}
static void attach_renderbuffer(GLenum t,GLenum a,GLenum rt,GLuint id) {
    assert(t==GL_FRAMEBUFFER&&a==GL_DEPTH_ATTACHMENT&&rt==GL_RENDERBUFFER&&id&&framebuffer);++attachments;
}
static void delete_renderbuffers(GLsizei n,const GLuint* ids) {assert(n==1&&*ids>=200);++delete_depths;}
static void delete_textures(GLsizei n,const GLuint* ids) {assert(n==1&&*ids>0&&*ids<200);++delete_colors;}
static void delete_framebuffers(GLsizei n,const GLuint* ids) {assert(n==1&&*ids>=100);}
static GLenum status(GLenum t) {assert(t==GL_FRAMEBUFFER&&framebuffer);return complete?GL_FRAMEBUFFER_COMPLETE:0;}
#include "framebuffer_production.h"
int main(void) {
    br_device_pixelmap back={.use_type=BRT_OFFSCREEN,.pm_width=640,.pm_height=480};
    br_device_pixelmap depth={.use_type=BRT_DEPTH,.pm_width=640,.pm_height=480};
    depth.asDepth.backbuffer=&back;
#define SET(name,fn) glad_##name=fn
    SET(glGenFramebuffers,gen_framebuffers);SET(glBindFramebuffer,bind_framebuffer);
    SET(glGenTextures,gen_textures);SET(glBindTexture,bind_texture);SET(glTexParameteri,param);
    SET(glTexImage2D,image);SET(glFramebufferTexture2D,attach_texture);
    SET(glGenRenderbuffers,gen_renderbuffers);SET(glBindRenderbuffer,bind_renderbuffer);
    SET(glRenderbufferStorage,storage);SET(glFramebufferRenderbuffer,attach_renderbuffer);
    SET(glDeleteRenderbuffers,delete_renderbuffers);SET(glDeleteTextures,delete_textures);
    SET(glDeleteFramebuffers,delete_framebuffers);SET(glCheckFramebufferStatus,status);
    /* glDrawBuffers deliberately NULL: unsupported and never called on Vita. */
    assert(!DevicePixelmapRecreateRenderBuffers(&back)&&colors==1&&framebuffers==1&&!framebuffer);
    assert(!DevicePixelmapRecreateRenderBuffers(&depth)&&depths==1&&colors==1&&!framebuffer&&!renderbuffer);
    assert(!DevicePixelmapRecreateRenderBuffers(&depth)&&depths==2&&delete_depths==1&&colors==1);
    complete=0;assert(DevicePixelmapRecreateRenderBuffers(&back)==BRE_FAIL&&!framebuffer);
    DevicePixelmapDeleteResources(&depth);assert(!depth.asDepth.glDepth&&delete_depths==2);
    DevicePixelmapDeleteResources(&back);assert(!back.asBack.glTex&&!back.asBack.glFbo&&delete_colors==2);
    assert(attachments==4);
    puts("PASS production Vita framebuffer color/depth create, reallocate, incomplete failure and teardown; no depth texture or unsupported draw-buffer call; ASan/UBSan; native GL mocked");
}
