#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BREND_FN(a,b) fake_##a##_##b
#define BRT_LINE 2
#define BRT_TRIANGLE 1
typedef int br_token;
typedef struct {int id;} br_geometry_v1_model;
typedef struct {int id;} br_renderer;
typedef void br_object;
/* Concrete array element sizes, including position/UV/normal and edge/equation. */
typedef struct {float p[3],uv[2],n[3];} vertex;
typedef struct {uint16_t v[3],e[3];float eq[4];} face;
/* Replace void array declarations above using the exact pointed-to types. */
struct v11group {void* stored;face* faces;vertex* vertices;uint32_t* vertex_colours;uint16_t nfaces,nvertices;};
struct v11model {uint32_t size,flags;uint16_t ngroups;struct v11group* groups;};
typedef struct br_geometry_stored br_geometry_stored;
struct dispatch {void (*_free)(br_object*);};
struct br_geometry_stored {struct dispatch* dispatch;struct v11model* model;int line;};
static unsigned builds,destructions,live;static int fail_snapshot,fail_geometry;
void GLVitaGeometryCacheForget(br_geometry_stored*);
static void destroy(br_object* p) {++destructions;--live;GLVitaGeometryCacheForget(p);free(p);}
static struct dispatch dispatch={destroy};
static br_geometry_stored* fake_GeometryStored_Allocate(br_geometry_v1_model* o,const char* id,br_renderer*r,struct v11model*m) {
 (void)o;(void)r;assert(!strcmp(id,"VITA-CACHED"));if(fail_geometry)return NULL;
 br_geometry_stored*g=malloc(sizeof(*g));assert(g);*g=(br_geometry_stored){&dispatch,m,0};++builds;++live;return g;
}
static void StoredGLUseEdges(br_geometry_stored* g) {g->line=1;}
static unsigned refreshes;
static unsigned index_rebuilds;
void C2VitaRefreshProfile(unsigned bytes) {(void)bytes;}
void StoredGLRefresh(br_geometry_stored* g, int rebuild_indices) {(void)g;++refreshes;index_rebuilds+=rebuild_indices!=0;}
void StoredGLMarkRefresh(br_geometry_stored* g, int rebuild_indices) {StoredGLRefresh(g, rebuild_indices);}
void StoredGLApplyRefresh(br_geometry_stored* g) {(void)g;}
int C2VitaABFeature(const char* f, int d) {(void)f;return d;}
static void* cache_malloc(size_t n) {return fail_snapshot?NULL:malloc(n);}
#define malloc cache_malloc
#define GLVitaGeometryCacheForget GLVitaGeometryCacheForget
#include "vita_geometry_cache.h"
#undef malloc
void C2VitaLog(const char* s,...) {(void)s;}
unsigned long long C2VitaGLTicksNS(void) {static unsigned long long t;return t+=1000;}
static struct v11model* make(size_t size) {
 struct v11model*m=calloc(1,size);assert(m);m->size=size;m->ngroups=1;
 m->groups=(void*)(m+1);struct v11group*g=m->groups;
 g->nvertices=4;g->nfaces=2;g->vertices=(void*)(g+1);g->faces=(void*)(g->vertices+4);g->vertex_colours=(void*)(g->faces+2);
 for(unsigned i=0;i<4;i++){g->vertices[i].p[0]=i;g->vertex_colours[i]=0xffaabbcc;}
 return m;
}
int main(void) {
 br_geometry_v1_model owner={0},other={1};br_renderer renderer={0},renderer2={1};
 struct v11model*m=make(4096);br_geometry_stored*g=GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE);assert(g);
 for(unsigned i=0;i<10000;i++) assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE)==g);
 assert(builds==1 && live==1);
 unsigned char* fields[]={ (void*)&m->groups->vertices[0].p[0],(void*)&m->groups->vertices[1].uv[0],(void*)&m->groups->vertices[2].n[0],(void*)&m->groups->vertex_colours[2],(void*)&m->groups->faces[0].v[1],(void*)&m->groups->faces[0].e[1],(void*)&m->groups->faces[1].eq[2],(void*)&m->groups->stored,(void*)&m->flags};
 for(unsigned i=0;i<sizeof(fields)/sizeof(fields[0]);i++) {*fields[i]^=1;assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE)==g);assert(builds==1 && refreshes==i+1 && live==1);} assert(index_rebuilds==3); /* only the three face mutations rebuild indices */
 /* a changed block size (effect models) is refreshed in place too, with a resized snapshot */
 unsigned before=refreshes;m->size-=64;assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE)==g);m->size+=64;assert(builds==1 && live==1 && refreshes==before+1 && vita_geo_bytes==4096-64);
 assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE)==g);assert(builds==1 && vita_geo_bytes==4096);
 g=GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_LINE);assert(g->line && live==2);
 assert(GLVitaGeometryCacheGet(&other,&renderer,m,BRT_TRIANGLE));assert(GLVitaGeometryCacheGet(&owner,&renderer2,m,BRT_TRIANGLE));
 /* Facility-driven destruction must remove metadata before owner shutdown. */
 g->dispatch->_free((br_object*)g);GLVitaGeometryCacheClearOwner(&owner);GLVitaGeometryCacheClearOwner(&other);assert(!live && !vita_geo_bytes);
 /* Identical prepared address reused with changed bytes must rebuild. */
 assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE));m->groups->vertices[0].p[0]+=1;
 unsigned old=refreshes;assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE));assert(refreshes==old+1);
 GLVitaGeometryCacheClearOwner(&owner);
 fail_snapshot=1;assert(!GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE) && !live);fail_snapshot=0;
 fail_geometry=1;assert(!GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE) && !live);fail_geometry=0;
 /* Foreign or too-large models fall back, without reading beyond arrays. */
 vertex*oldv=m->groups->vertices;m->groups->vertices=(void*)1;assert(!GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE));m->groups->vertices=oldv;
 m->size=0;assert(!GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE));m->size=VITA_GEO_MAX_MODEL+1;assert(!GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE));free(m);
 /* The merged static track (about 1 MB) is cached: one build for many draws. */
 m=make(1024*1024);old=builds;g=GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE);assert(g);
 for(unsigned i=0;i<100;i++) assert(GLVitaGeometryCacheGet(&owner,&renderer,m,BRT_TRIANGLE)==g);
 assert(builds==old+1);GLVitaGeometryCacheClearOwner(&owner);free(m);
 struct v11model*many[600];
 for(unsigned i=0;i<600;i++){many[i]=make(8192);assert(GLVitaGeometryCacheGet(&owner,&renderer,many[i],BRT_TRIANGLE));assert(live<=512 && vita_geo_bytes<=VITA_GEO_BUDGET);}
 GLVitaGeometryCacheClearOwner(&owner);assert(!live && !vita_geo_bytes && builds==destructions);
 for(unsigned i=0;i<600;i++)free(many[i]);
 puts("PASS production exact geometry cache:10000unchanged draws1build, position/UV/normal/colour/indices/edges/equation/material/header mutations rebuild; line/owner/renderer isolation; address reuse; facility destruction+owner cleanup; optional allocation failure/foreign/oversize fallback;512slots/2MiB bounds;ASan/UBSan, GPU allocation mocked");
}
