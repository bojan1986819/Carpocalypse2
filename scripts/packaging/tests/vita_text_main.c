#include "vita_text.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#define C2_HOOK_FASTCALL
#define C2_HOOK_BUG_ON(x) ((void)0)
#define CARPOCALYPSE2_ASIZE(a) (sizeof(a)/sizeof((a)[0]))
#define BR_MATU_ALL 1
typedef unsigned char tU8;typedef uint16_t tU16;typedef uint32_t tU32;typedef float br_scalar;
typedef enum {eJust_left,eJust_right,eJust_centre} tJustification;
typedef struct {int used,glyph_width,index;br_material* material;br_model* model;struct {float v[2];} texCoord;} tPolyFontGlyph;
typedef struct {int available,widthOfBlank,interCharacterSpacing,fontCharacterHeight,fontSize;tPolyFontGlyph glyphs[256];} tPolyFont;
static tPolyFont gPoly_fonts[27];static br_model models[27][256];static br_pixelmap texture;
static br_pixelmap* gTexture_maps[1024];
static br_material material_store[80],*gPoly_font_materials[80];
static tU16 polyFontMaterialCounter;static int currentPolyFontMaterialIdx;
static br_actor hud,camera,string_root,actors[256],*gPolyfont_glyph_actors[256];
static br_actor *gHUD_root=&hud,*gHUD_camera=&camera,*gString_root_actor=&string_root;
static br_pixelmap back={3,4,5,6,640,480},depth;
static br_pixelmap *gBack_screen=&back,*gRender_screen=&back,*gDepth_buffer=&depth;
static int gCount_polyfont_glyph_actors,gRender_poly_text=1;
static unsigned begins,ends,open_scene,mutations;
struct event {int fontchar,model;float x,y,z;};
static struct event events[100000],reference[100000];static unsigned count;
static void emit(int fc,int model,float x,float y,float z) {
 assert(count<CARPOCALYPSE2_ASIZE(events));events[count++]=(struct event){fc,model,x,y,z};
}
void C2VitaLog(const char* format,...) {(void)format;}
void BrZbSceneRenderBegin(br_actor* r,br_actor* c,br_pixelmap* p,br_pixelmap* d) {
 assert(!open_scene && r==&hud && c==&camera && d==&depth && p->width>0);
 ++begins;open_scene=1;
}
static void walk(br_actor* a) {
 if(a->model) {assert(a->material);br_vector3 v=a->t.t.translate.t;emit((uintptr_t)a->material->user&65535,a->model->id,v.v[0],v.v[1],v.v[2]);}
 for(br_actor* ch=a->children;ch;ch=ch->next) walk(ch);
}
void BrZbSceneRenderAdd(br_actor* a) {assert(open_scene && a->parent==&hud);walk(a);}
void BrZbSceneRenderEnd(void) {assert(open_scene);++ends;open_scene=0;}
void BrZbSceneRender(br_actor* r,br_actor* c,br_pixelmap* p,br_pixelmap* d) {
 BrZbSceneRenderBegin(r,c,p,d);for(br_actor* a=r->children;a;a=a->next) BrZbSceneRenderAdd(a);BrZbSceneRenderEnd();
}
static void BrActorAdd(br_actor* p,br_actor* a) {assert(!a->parent);a->next=p->children;a->parent=p;p->children=a;}
static void BrActorRemove(br_actor* a) {
 assert(a->parent);br_actor** p=&a->parent->children;while(*p!=a) {assert(*p);p=&(*p)->next;}*p=a->next;a->parent=a->next=NULL;
}
static void BrVector3Set(br_vector3* v,float x,float y,float z) {*v=(br_vector3){{x,y,z}};}
static void BrMatrix23Scale(br_matrix23* m,float x,float y) {memset(m,0,sizeof(*m));m->m[0][0]=x;m->m[1][1]=y;}
static void BrMaterialUpdate(br_material* m,int f) {assert(!open_scene && m && f==BR_MATU_ALL);++mutations;}
static void CheckAvailabilityOfThisFont(int f) {assert(!open_scene);gPoly_fonts[f].available=1;}
#include "text_cleanup.h"
static int GetPolyFontIndexToReplaceDRfontWith(const void* font);
static int CharacterWidth(int f,tU8 c) {return gPoly_fonts[f].glyphs[c].used?gPoly_fonts[f].glyphs[c].glyph_width:gPoly_fonts[f].widthOfBlank;}
static int GetSpacing(int f) {return gPoly_fonts[f].interCharacterSpacing;}
static int PolyFontHeight(int f) {return gPoly_fonts[f].fontCharacterHeight;}
typedef struct {int id;} tDR_font;static tDR_font gFonts[28];
static int GetPolyFontIndexToReplaceDRfontWith(const void* font) {return ((const tDR_font*)font)->id;}
enum {eHeadup_unused,eHeadup_text,eHeadup_coloured_text,eHeadup_fancy,eView_forward=0};
typedef struct {int type,cockpit_anchored,x,y,clever,right_edge,flash_period,flash_state;unsigned end_time,last_flash;
 union {struct {const void* font;int colour;char text[256];} text_info;struct {tDR_font* coloured_font;int colour;char text[256];} coloured_text_info;} data;} tHeadup;
static tHeadup gHeadups[37];static struct {int which_view;} gProgram_state;
static int gScreen_wobble_x=2,gScreen_wobble_y=3,gHeadup_detail_level=2;
static int gCredits_won_headup=1,gPed_kill_count_headup=2,gCar_kill_count_headup=3,gLaps_headup=4;
static int Flash(int period,unsigned* last,int* state) {*last+=period;*state=!*state;return *state;}
static void BrPixelmapText(br_pixelmap* p,int x,int y,int colour,const void* font,const char* text) {
 assert(!open_scene && p==&back);(void)font;
 unsigned sum=0;for(const unsigned char* c=(const unsigned char*)text;*c;c++)sum=sum*33+*c;
 emit(-1,(int)sum,x,y,colour);
}
#undef __vita__
#define GetPolyFontMaterial BaselineGetPolyFontMaterial
#define PolyFontText BaselinePolyFontText
#define DRPixelmapText BaselineDRPixelmapText
#define DRPixelmapCleverText BaselineDRPixelmapCleverText
#define RedrawHeadupTexts BaselineRedrawHeadupTexts
#include "text_production.h"
#undef GetPolyFontMaterial
#undef PolyFontText
#undef DRPixelmapText
#undef DRPixelmapCleverText
#undef RedrawHeadupTexts
#define __vita__ 1
#include "text_production.h"
static unsigned vita_deferred;
#undef __vita__
static void BaselineFirstColoured(tHeadup* the_headup,int i) {
 int x_offset,y_offset;br_pixelmap* render_screen=gRender_screen;
 switch(the_headup->type) {
#include "text_first_coloured.h"
 default:break;
 }
}
#define __vita__ 1
static void FirstColoured(tHeadup* the_headup,int i) {
 int x_offset,y_offset;br_pixelmap* render_screen=gRender_screen;
 switch(the_headup->type) {
#include "text_first_coloured.h"
 default:break;
 }
}
static void reset(void) {
 assert(!open_scene && !gCount_polyfont_glyph_actors && !string_root.parent);
 count=begins=ends=mutations=0;currentPolyFontMaterialIdx=0;polyFontMaterialCounter=0;
 memset(material_store,0,sizeof(material_store));
 memset(gPoly_fonts,0,sizeof(gPoly_fonts));
 for(int f=0;f<27;f++) {
  gPoly_fonts[f].available=1;gPoly_fonts[f].widthOfBlank=3;gPoly_fonts[f].interCharacterSpacing=1;gPoly_fonts[f].fontCharacterHeight=12+f;gPoly_fonts[f].fontSize=16;
  for(int c=0;c<256;c++) {models[f][c].id=f*256+c;gPoly_fonts[f].glyphs[c]=(tPolyFontGlyph){c!=' ',5+c%4,0,NULL,&models[f][c],{{0.1f,0.2f}}};}
 }
 for(int i=0;i<80;i++)gPoly_font_materials[i]=&material_store[i];
 for(int i=0;i<256;i++)gPolyfont_glyph_actors[i]=&actors[i];
 for(int i=0;i<28;i++)gFonts[i].id=i%27;
 gTexture_maps[0]=&texture;
}
static void headups(unsigned seed) {
 srand(seed);memset(gHeadups,0,sizeof(gHeadups));
 for(unsigned i=0;i<37;i++) {
  tHeadup* h=&gHeadups[i];h->type=1+rand()%3;h->cockpit_anchored=rand()%2;h->x=rand()%620;h->y=rand()%450;
  h->data.coloured_text_info.coloured_font=&gFonts[rand()%27];h->data.text_info.colour=rand()%255;h->clever=rand()%2;
  unsigned len=rand()%220;
  for(unsigned j=0;j<len;j++)h->data.text_info.text[j]="AZabcd09 !?"[rand()%10];
  h->data.text_info.text[len]=0;h->right_edge=630;
  if(i%4==0)h->end_time=500;
  if(i%5==0)h->flash_period=200;
  if(i%7==0 && len>3)h->data.text_info.text[1]=(char)-3;
 }
 gProgram_state.which_view=seed%2;gHeadup_detail_level=seed%6;
}
int main(void) {
 BrActorAdd(&hud,&camera);
 for(unsigned trial=0;trial<120;trial++) {
  reset();headups(trial);BaselineRedrawHeadupTexts(1000);unsigned n=count,old=begins;memcpy(reference,events,count*sizeof(*events));
  reset();headups(trial);RedrawHeadupTexts(1000);
  assert(count==n && !memcmp(reference,events,n*sizeof(*events)) && begins<=old && begins==ends);
  assert(!open_scene && !gCount_polyfont_glyph_actors && !string_root.parent && back.origin_x==3 && back.base_y==6);
 }
 /* All common HUD glyphs already loaded/cached by the first HUD pass. */
 reset();memset(gHeadups,0,sizeof(gHeadups));gProgram_state.which_view=0;
 for(unsigned i=0;i<37;i++) {gHeadups[i].type=eHeadup_coloured_text;gHeadups[i].data.coloured_text_info.coloured_font=&gFonts[0];strcpy(gHeadups[i].data.text_info.text,"SPEED 123");}
 for(const char* s="SPEED 123";*s;s++)GetPolyFontMaterial(0,*s);
 RedrawHeadupTexts(0);assert(begins==1 && ends==1 && count==37*8);
 /* Integrated first-coloured + first glyph scene + final redraw. The old
  * path repeats up to248glyphs and exhausts the shared actor pool; the new
  * path leaves it free and produces the identical complete final296glyphs. */
 reset();gRender_poly_text=0;
 for(unsigned i=0;i<37;i++)BaselineFirstColoured(&gHeadups[i],i);
 assert(gCount_polyfont_glyph_actors==248);
 BrActorAdd(&hud,&string_root);BrZbSceneRender(&hud,&camera,&back,&depth);BrActorRemove(&string_root);CleanPolyFontDanglers();
 unsigned duplicates=count;gRender_poly_text=1;RedrawHeadupTexts(0);unsigned final=count-duplicates;
 assert(final==296);memcpy(reference,events+duplicates,final*sizeof(*events));
 reset();gRender_poly_text=0;vita_deferred=0;
 for(unsigned i=0;i<37;i++)FirstColoured(&gHeadups[i],i);
 assert(vita_deferred==37 && !gCount_polyfont_glyph_actors && !count);
 gRender_poly_text=1;RedrawHeadupTexts(0);assert(count==final && !memcmp(reference,events,final*sizeof(*events)));
 reset();gRender_poly_text=1;FirstColoured(&gHeadups[0],0);assert(count==8); // direct call still draws
 puts("PASS production coloured HUD dedup:37slots,248duplicate glyphs removed,296 final glyphs preserved beyond256actors; direct drawing retained");
 /* Reuse the256 actors for successive200-character strings, then skip a256
  * character string exactly like the baseline instead of overflowing a batch. */
 reset();char large[257];memset(large,'A',256);large[256]=0;
 C2VitaTextBatchBegin();large[200]=0;PolyFontText(large,10,20,0,eJust_left,1);PolyFontText(large,20,30,0,eJust_right,1);
 large[200]='A';PolyFontText(large,20,30,0,eJust_centre,1);C2VitaTextBatchEnd();assert(count==400 && begins==1 && ends==1);
 /* A font upload ends the live scene before touching GPU state. */
 reset();C2VitaTextBatchBegin();PolyFontText("A",0,0,0,eJust_left,1);gPoly_fonts[1].available=0;PolyFontText("B",0,0,1,eJust_left,1);C2VitaTextBatchEnd();assert(count==2 && begins==2 && ends==2);
 /* Target/viewport changes cannot reuse incompatible scene setup. */
 reset();C2VitaTextBatchBegin();BrActorAdd(&hud,&string_root);C2VitaTextSceneRender(&hud,&camera,&back,&depth);back.origin_x++;
 C2VitaTextSceneRender(&hud,&camera,&back,&depth);C2VitaTextBatchEnd();back.origin_x--;BrActorRemove(&string_root);assert(begins==2 && ends==2);
 C2VitaTextBatchEnd();assert(begins==ends); /* defensive unmatched end */
 puts("PASS production HUD text batching:120 mixed/expired/flashing/clever/wobbling scenarios match exact ordered glyph+CPU draw stream;37 warm strings use1scene;200char actor reuse/256char limit, material eviction and font upload barriers, viewport split/cleanup;ASan/UBSan; BRender/GPU recorded mocks");
}
