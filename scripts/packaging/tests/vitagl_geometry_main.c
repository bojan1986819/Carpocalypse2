/* Production geometry allocator/free, GL upload and resource services mocked. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BREND_DRIVER_GL 1
#define BREND_FN(a,b) a##b
#define BREND_CMETHOD_DECL(a,b) stored_##b
#define BREND_CLASS(a) a
#define BR_MEMORY_OBJECT 84
#define BR_MEMORY_OBJECT_DATA 35
#define BR_TRUE 1
#define GL_TRIANGLES 4
#define GL_ARRAY_BUFFER 1
#define GL_ELEMENT_ARRAY_BUFFER 2
typedef unsigned int GLuint;
typedef unsigned short br_uint_16;
typedef void br_object;
typedef struct {size_t bytes;} gl_groupinfo;
struct v11group {unsigned int nvertices,nfaces;};
struct v11model {br_uint_16 ngroups;struct v11group* groups;};
typedef struct facility {void* object_list;} facility;
typedef struct br_geometry_v1_model {facility* renderer_facility;void* device;} br_geometry_v1_model;
typedef struct fake_screen {struct {int video;} asFront;} fake_screen;
typedef struct fake_pm {fake_screen* screen;} fake_pm;
typedef struct br_renderer {fake_pm* pixelmap;} br_renderer;
typedef struct br_geometry_stored {
 const void* dispatch;const char* identifier;void* device;br_geometry_v1_model* gv1model;
 struct v11model* model;int shared;gl_groupinfo* groups;
 GLuint gl_primitive,gl_vbo_posn,gl_vbo,gl_ibo,gl_vao;
} br_geometry_stored;
static const int geometryStoredDispatch;
typedef struct allocation {struct allocation* parent;struct allocation* children;struct allocation* next;size_t size;max_align_t alignment;} allocation;
static size_t live,live_bytes,entries;
static void* BrResAllocate(void* parent,size_t size,int type) {
 (void)type;allocation* n=calloc(1,sizeof(*n)+size);assert(n);n->size=size;
 if(parent){n->parent=(allocation*)parent-1;n->next=n->parent->children;n->parent->children=n;}
 live++;live_bytes+=size;return n+1;
}
static void BrResFreeNoCallback(void* p) {
 allocation* n=(allocation*)p-1;
 while(n->children)BrResFreeNoCallback(n->children+1);
 if(n->parent){allocation** link=&n->parent->children;while(*link!=n)link=&(*link)->next;*link=n->next;}
 live--;live_bytes-=n->size;free(n);
}
static void ObjectContainerAddFront(void* f,void* o){(void)f;(void)o;entries++;}
static void ObjectContainerRemove(void* f,void* o){(void)f;(void)o;assert(entries);entries--;}
static GLuint build_vbo_posn(const struct v11model* m,size_t n){(void)m;(void)n;return 1;}
static GLuint build_vbo(const struct v11model* m,size_t n){(void)m;(void)n;return 2;}
static GLuint build_ibo(const struct v11model* m,size_t n,gl_groupinfo* g){(void)m;(void)n;(void)g;return 3;}
static GLuint create_vao(void* v,GLuint a,GLuint b,GLuint c){(void)v;assert(a==1&&b==2&&c==3);return 4;}
static void glBindVertexArray(GLuint v){(void)v;}
static void glBindBuffer(int target,GLuint v){(void)target;(void)v;}
static void glDeleteVertexArrays(int n,const GLuint* id){assert(n==1&&*id==4);}
static void glDeleteBuffers(int n,const GLuint* id){assert(n==1&&*id>=1&&*id<=3);}
#include "geometry_production.h"
int main(void) {
 facility factory={0};fake_screen screen={0};fake_pm pm={&screen};br_renderer renderer={&pm};
 struct v11group groups[37];struct v11model model={0,groups};
 factory.object_list=BrResAllocate(NULL,4,84);
 br_geometry_v1_model* geometry=BrResAllocate(NULL,sizeof(*geometry),84);geometry->renderer_facility=&factory;
 size_t before=live,bytes=live_bytes;
 for(int i=0;i<20000;i++){
  model.ngroups=1+i%37;for(int j=0;j<37;j++){groups[j].nvertices=3;groups[j].nfaces=1;}
  br_geometry_stored* stored=GeometryStoredAllocate(geometry,"IMMEDIATE",&renderer,&model);
  assert(live==before+2&&entries==1);
  stored_free(stored);
  assert(live==before&&live_bytes==bytes&&entries==0);
 }
 BrResFreeNoCallback(geometry);BrResFreeNoCallback(factory.object_list);assert(!live&&!live_bytes);
 puts("PASS 20000 production stored-geometry create/free cycles, changing group counts: CPU group ownership bounded; ASan/UBSan; GPU/resource hierarchy services mocked");
}
