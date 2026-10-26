#include "prefix.h"
#include <math.h>
#define C2_HOOK_CDECL
#define C2_HOOK_STDCALL
#define BR_MAC2(a,b,c,d) ((a)*(b)+(c)*(d))
#define BR_MAC3(a,b,c,d,e,f) (BR_MAC2(a,b,c,d)+(e)*(f))
#define BR_MAC4(a,b,c,d,e,f,g,h) (BR_MAC3(a,b,c,d,e,f)+(g)*(h))
#define BR_SCALAR(x) (x)
#define BR_SCALAR_EPSILON 0.00000011920928955078125f
#define BRT_DONT_CARE 100
#define BRT_LENGTH_PRESERVING 101
#define BRT_PERSPECTIVE 102
#define BRT_PARALLEL 103
#define Z 2
#define A(x,y) A->m[x][y]
#define B(x,y) B->m[x][y]
#define C(x,y) C->m[x][y]
#define M(x,y) mat->m[x][y]
typedef br_vector3_f br_vector3;
#include "native_math.h"
static void BrVector3CopyMat34Col(br_vector3* a,const br_matrix34* m,int i){for(int j=0;j<3;j++)a->v[j]=m->m[j][i];}
static void BrVector3CopyMat34Row(br_vector3* a,const br_matrix4* m,int i){for(int j=0;j<3;j++)a->v[j]=m->m[i][j];}
static void BrVector4Set(br_vector4* a,float x,float y,float z,float w){*a=(br_vector4){{x,y,z,w}};}
static unsigned inverses;
static float CountedInverse(br_matrix4* a,const br_matrix4* b){++inverses;return BrMatrix4Inverse(a,b);}
static void C2VitaLog(const char* fmt,...){(void)fmt;}
#define BrMatrix4Inverse CountedInverse
#undef __vita__
#define UpdateMatrices BaselineMatrices
#define EyeInModel BaselineEye
#define StateGLUpdateModel BaselineModel
#define StateGLReset BaselineReset
#include "model.h"
#undef UpdateMatrices
#undef EyeInModel
#undef StateGLUpdateModel
#undef StateGLReset
#define __vita__ 1
#include "model.h"
int main(void){
 state_cache original={0},fast={0};state_matrix m={0};
 for(int n=0;n<3000;n++){
  for(int i=0;i<4;i++)for(int j=0;j<3;j++){m.model_to_view.m[i][j]=(rand()%101-50)*.03f;m.view_to_environment.m[i][j]=(rand()%101-50)*.03f;}
  if(n%7==0)memset(&m.model_to_view,0,sizeof(m.model_to_view));
  BrMatrix4Identity(&m.view_to_screen);m.view_to_screen.m[2][3]=-.2f;
  m.model_to_view_hint=n%2?BRT_LENGTH_PRESERVING:0;m.view_to_screen_hint=n%3?BRT_PERSPECTIVE:BRT_PARALLEL;m.view_to_environment_hint=n%5?0:BRT_DONT_CARE;
  if(n%11==0){BaselineReset(&original);StateGLReset(&fast);assert(!fast.model_input_valid);}
  for(int repeat=0;repeat<4;repeat++){
   inverses=0;BaselineModel(&original,&m);assert(inverses==2);inverses=0;StateGLUpdateModel(&fast,&m);assert(inverses==(repeat?0:1));
   assert(!memcmp(&original.model,&fast.model,sizeof(original.model)));
  }
 }
 puts("PASS production matrix cache:12000 draws bit-identical with actual BRender matrix math; singular/nonuniform/transformed/environment/hint/reset cases, duplicate inverse removed;32/64-bit");
}
