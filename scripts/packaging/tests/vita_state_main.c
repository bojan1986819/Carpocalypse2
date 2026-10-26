#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdalign.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#define __vita__ 1
#define BREND_DRIVER_GL 1
#define BR_MAX_LIGHTS 48
#define BR_MAX_CLIP_PLANES 6
#define BREND_FN(a,b) a##GL##b
#define BREND_CLASS(x) x
#define JOIN(a,b) a##b
#define BREND_CMETHOD_DECL(a,b) JOIN(a,b)
#define BR_FALSE 0
#define BR_TRUE 1
#define BRE_OK 0
#define BRE_FAIL -1
#define BRT_MODEL 99
#define BR_STATE_SURFACE 1
#define BR_STATE_MATRIX 2
#define BR_STATE_ENABLE 4
#define BR_STATE_LIGHT 8
#define BR_STATE_CLIP 16
#define BR_STATE_BOUNDS 32
#define BR_STATE_CULL 64
#define BR_STATE_OUTPUT 128
#define BR_STATE_PRIMITIVE 256
enum { BRT_MATRIX, BRT_CLIP, BRT_CULL, BRT_SURFACE, BRT_PRIMITIVE, BRT_HIDDEN_SURFACE, BRT_LIGHT, BRT_OUTPUT, BRT_ENABLE, BRT_BOUNDS };
typedef unsigned br_token,br_colour,br_uint_32,GLuint;
typedef int br_int_32,br_boolean,br_error;
typedef float br_scalar;
typedef struct {float m[4][3];} br_matrix34;
typedef struct {float m[4][4];} br_matrix4;
typedef struct {float m[3][2];} br_matrix23;
typedef struct {float v[3];} br_vector3_f;
typedef struct {float v[4];} br_vector4;
typedef struct {char data[128];} shader_data_scene;
typedef struct primitive br_primitive;
typedef struct table br_order_table;
typedef struct heap br_primitive_heap;
typedef struct { br_token t; uintptr_t v; } br_token_value;
#include "state.h"
void StateGLCopy(state_stack*,state_stack*,uint32_t);
void StateGLTouchLights(state_stack*);
#define INIT(part) static void StateGLInit##part(state_all* s){(void)s;}
INIT(Matrix) INIT(Clip) INIT(Cull) INIT(Surface) INIT(Primitive) INIT(Output) INIT(Hidden)
static void StateGLInitLight(state_all* s){memset(s->default_.light,0,sizeof(s->default_.light));s->default_.valid=~0u;StateGLTouchLights(&s->default_);for(int i=0;i<48;i++)s->templates.light[i]=(void*)(uintptr_t)(i+1);}
static unsigned report_push,report_copy;
void C2VitaLog(const char* fmt,...){(void)fmt;va_list ap;va_start(ap,fmt);report_push=va_arg(ap,unsigned);report_copy=va_arg(ap,unsigned);va_end(ap);}
#include "state.c"
typedef struct { state_all state; } br_renderer;
static int BrTokenValueSet(void* dst,unsigned* m,unsigned t,uintptr_t v,struct br_tv_template* tp){state_stack* s=dst;int i=(uintptr_t)tp-1;assert(i>=0&&i<48);s->light[i].colour=v;*m=1;(void)t;return 0;}
static int BrTokenValueSetMany(void* dst,int* count,unsigned* m,br_token_value* tv,struct br_tv_template* tp){*count=1;return BrTokenValueSet(dst,m,tv->t,tv->v,tp);}
#include "writers.h"
static state_stack refs[MAX_STATE_STACK],saved;
static void equal(state_stack* a,state_stack* b){assert(!memcmp(a,b,offsetof(state_stack,light_revision)));}
int main(void){
 br_renderer r={0};state_all* s=&r.state;StateGLInit(s,0);refs[0]=*s->current;
 // Deep inheritance and genuine overflow rejection, plus zero-revision fallback.
 for(int i=1;i<MAX_STATE_STACK;i++){assert(StateGLPush(s,0));refs[i]=refs[i-1];equal(s->current,&refs[i]);}
 assert(!StateGLPush(s,0));for(int i=1;i<MAX_STATE_STACK;i++)assert(StateGLPop(s,0));assert(!StateGLPop(s,0));
 for(int n=0;n<100000;n++){
  unsigned k=rand()%6;int top=s->top;
  if(k==0&&top<MAX_STATE_STACK-1){refs[top+1]=refs[top];assert(StateGLPush(s,(unsigned)rand()));}
  else if(k==1&&top>0)assert(StateGLPop(s,0));
  else if(k==2){int i=rand()%48;unsigned c=rand();assert(!br_rendererpartSet(&r,BRT_LIGHT,i,0,c));refs[top].light[i].colour=c;}
  else if(k==3){int i=rand()%48,count=0;br_token_value tv={0,(unsigned)rand()};assert(!br_rendererpartSetMany(&r,BRT_LIGHT,i,&tv,&count));refs[top].light[i].colour=tv.v;}
  else if(k==4){StateGLCopy(&saved,s->current,~0u);s->current->surface.colour=rand();StateGLCopy(s->current,&saved,~0u);}
  else {s->current->matrix.model_to_view.m[0][0]=(float)n;refs[top].matrix=s->current->matrix;}
  equal(s->current,&refs[s->top]);
 }
 while(s->top)StateGLPop(s,0);
 for(int n=0;n<20000;n++){assert(StateGLPush(s,BR_STATE_MATRIX));assert(StateGLPop(s,BR_STATE_MATRIX));}
 assert(report_push>20000 && report_copy<report_push);printf("PASS production renderer state:100k nested push/pop/light token/copy operations; %u pushes %u light copies; full inheritance and bounds,32/64-bit\n",report_push,report_copy);
}
