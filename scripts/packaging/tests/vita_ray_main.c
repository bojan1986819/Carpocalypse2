#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint32_t tU32;
typedef float br_scalar;
typedef struct { float v[3]; } br_vector3;
typedef struct { br_vector3 min,max; } bounds;
typedef struct { bounds bounds; struct { unsigned char red,grn,blu; } vertices[2]; } br_model;
typedef struct actor { br_model* model; struct { struct { int mat; } t; } t; } br_actor;
typedef struct { br_actor* car_actor; br_actor* car_master_actor; br_vector3 pos; } car;
typedef struct { br_vector3 pos; } ped;
typedef int br_matrix34;
typedef int br_pixelmap;
struct { unsigned start_time; car* car; ped* ped; } gProximity_rays[2];
int gNo_2d_effects=1, gCamera_to_world, gProx_ray_shade_table;
br_actor actor; br_actor* gLine_actor=&actor; br_actor* gCamera=&actor; br_actor* gDont_render_actor=&actor;
br_model model; br_model* gLine_model=&model; ped peds[2]; ped* gPedestrian_array=peds;
#define CARPOCALYPSE2_ASIZE(a) (sizeof(a)/sizeof((a)[0]))
#define C2_HOOK_FASTCALL
#define ePipe_chunk_prox_ray 0
#define BR_MODU_ALL 0
#define C2VP_RAYS 16
static unsigned calls,limit=4096,begins,ends,logs;
static br_vector3 points[4096];
static void C2VitaPerfBegin(unsigned x){assert(x==16);++begins;}
static void C2VitaPerfEnd(unsigned x){assert(x==16);++ends;}
static void C2VitaLog(const char* fmt,...){(void)fmt;++logs;}
static unsigned GetTotalTime(void){return 100;}
static void ARStartPipingSession(int x){(void)x;}
static void AREndPipingSession(void){}
static void BrActorRemove(br_actor* x){(void)x;}
static void BrActorAdd(br_actor* x,br_actor* y){(void)x;(void)y;}
static void BrModelUpdate(br_model* x,int y){(void)x;(void)y;}
static void AddProxRayToPipingSession(int a,car* b,ptrdiff_t c,unsigned d){(void)a;(void)b;(void)c;(void)d;}
static void BrVector3Set(br_vector3* a,float x,float y,float z){*a=(br_vector3){{x,y,z}};}
static void BrMatrix34ApplyV(br_vector3* a,br_vector3* b,int* c){(void)c;*a=*b;}
static void BrVector3Add(br_vector3* a,br_vector3* b,br_vector3* c){for(int i=0;i<3;i++)a->v[i]=b->v[i]+c->v[i];}
static void BrVector3Sub(br_vector3* a,br_vector3* b,br_vector3* c){for(int i=0;i<3;i++)a->v[i]=b->v[i]-c->v[i];}
static void BrVector3Scale(br_vector3* a,br_vector3* b,float c){for(int i=0;i<3;i++)a->v[i]=b->v[i]*c;}
static void BrVector3Copy(br_vector3* a,br_vector3* b){*a=*b;}
static float BrVector3Length(br_vector3* a){return sqrtf(a->v[0]*a->v[0]+a->v[1]*a->v[1]+a->v[2]*a->v[2]);}
static void BrVector3Normalise(br_vector3* a,br_vector3* b){float d=BrVector3Length(b);if(d!=0)for(int i=0;i<3;i++)a->v[i]=b->v[i]/d;}
static void DRMatrix34TApplyP(br_vector3* a,br_vector3* b,int* c){(void)c;*a=*b;}
static float SRandomPosNeg(float x){return x*((float)rand()/RAND_MAX*2-1);}
static void DrawLine3D(br_vector3* a,br_vector3* b,br_pixelmap* c,br_pixelmap* d,int e){(void)b;(void)c;(void)d;(void)e;assert(calls<limit);points[calls++]=*a;}
#undef __vita__
#define RenderElectroBastardRays Baseline
#include "ray.h"
#undef RenderElectroBastardRays
#define __vita__ 1
#include "ray.h"
int main(int argc,char** argv){
 car c={&actor,&actor,{{0,0,0}}};actor.model=&model;gProximity_rays[0].car=&c;gProximity_rays[0].ped=peds;
 for(int i=0;i<250;i++){
  float distance=(float)i*.1f; peds[0].pos=(br_vector3){{distance,.02f,0}};gProximity_rays[0].start_time=99;calls=0;limit=4096;
  Baseline(0,0,0,0,0);unsigned n=calls;br_vector3 saved[4096];memcpy(saved,points,sizeof(points));calls=0;
  RenderElectroBastardRays(0,0,0,0,0);assert(calls==n);assert(!memcmp(points,saved,n*sizeof(points[0])));
 }
 float distances[]={26,100,100000000.f,FLT_MAX,INFINITY,NAN};
 for(unsigned i=0;i<sizeof(distances)/sizeof(*distances);i++){
  peds[0].pos=(br_vector3){{distances[i],0,0}};gProximity_rays[0].start_time=99;calls=0;limit=257;
  if(argc>1) Baseline(0,0,0,0,0); else RenderElectroBastardRays(0,0,0,0,0);
  assert(calls<=257); if(isfinite(distances[i]) && distances[i]<1e10f)assert(points[calls-1].v[0]==distances[i]);else assert(!calls);
 }
 assert(begins==ends);assert(logs);puts("PASS production lightning:250 normal rays byte-identical; distant/nonfinite rays bounded, endpoints preserved");
}
