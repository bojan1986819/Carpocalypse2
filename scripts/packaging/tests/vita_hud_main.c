/* Compare production Vita APO pixels against the unchanged desktop function. */
#include "vita_hud.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define C2_HOOK_FASTCALL
#define BR_MAPU_ALL 1
#define BR_MATU_COLOURMAP 2
#define BR_RSTYLE_FACES 3
typedef struct {int width,height,row_bytes;void* pixels;void* stored;} br_pixelmap;
typedef struct {br_pixelmap* colour_map;} br_material;
typedef struct {int render_style;} br_actor;
static int gPrev_ps_apo_levels[3],gPrev_ps_drawn_levels[3],gPS_apo_level_changed[3];
static struct {struct {int power_up_slots[3];} current_car;} gProgram_state;
static br_pixelmap maps[3],icons[3],blocks[3],powerbar;
static uint16_t images[3][64*15],icon_pixels[3][6*15],block_pixels[3][2*5],background[64*15];
static br_pixelmap *gArmour_colour_map=&maps[0],*gPower_colour_map=&maps[1],*gOffensive_colour_map=&maps[2];
static br_pixelmap *gPowerbar=&powerbar,*gIcon_armour=&icons[0],*gIcon_power=&icons[1],*gIcon_offense=&icons[2];
static br_pixelmap *gIcon_litBloc1=&blocks[0],*gIcon_grnBlock1=&blocks[1],*gIcon_greyBloc1=&blocks[2];
static br_material materials[3],*gArmour_material=&materials[0],*gPower_material=&materials[1],*gOffence_material=&materials[2];
static br_actor actors[3],*gArmour_actor=&actors[0],*gPower_actor=&actors[1],*gOffense_actor=&actors[2];
static unsigned uploads,copies;
static void DRPixelmapCopy(br_pixelmap* dst,br_pixelmap* src) {
    assert(dst->width==src->width && dst->height==src->height);
    memcpy(dst->pixels,src->pixels,src->row_bytes*src->height);++copies;
}
static void DRPixelmapRectangleMaskedCopy(br_pixelmap* dst,int dx,int dy,br_pixelmap* src,int sx,int sy,int w,int h) {
    for(int y=0;y<h;++y) for(int x=0;x<w;++x) {
        assert(dx+x>=0 && dx+x<dst->width && dy+y>=0 && dy+y<dst->height);
        uint16_t c=((uint16_t*)src->pixels)[(sy+y)*src->width+sx+x];
        if(c) ((uint16_t*)dst->pixels)[(dy+y)*dst->width+dx+x]=c;
    }
}
static void BrMapUpdate(br_pixelmap* map,int flags) {assert(flags==BR_MAPU_ALL);map->stored=map;++uploads;}
static void BrMaterialUpdate(br_material* mat,int flags) {assert(mat->colour_map && flags==BR_MATU_COLOURMAP);}
#include "hud_reset_production.h"
#include "apo_production.h"
#undef __vita__
#define DoPSPowerHeadup BaselineDoPSPowerHeadup
#include "apo_production.h"
#undef DoPSPowerHeadup
int main(void) {
    for(unsigned i=0;i<64*15;++i) background[i]=0x1000+i;
    powerbar=(br_pixelmap){64,15,128,background,0};
    for(int i=0;i<3;++i) {
        maps[i]=(br_pixelmap){64,15,128,images[i],0};
        icons[i]=(br_pixelmap){6,15,12,icon_pixels[i],0};
        blocks[i]=(br_pixelmap){2,5,4,block_pixels[i],0};
        for(int j=0;j<6*15;++j) icon_pixels[i][j]=0x2000+i*0x100+j;
        for(int j=0;j<2*5;++j) block_pixels[i][j]=0x3000+i*0x100+j;
    }
    int old_level[3]={-1,-1,-1},old_slots[3]={-1,-1,-1};
    C2VitaResetHUDCache();
    for(int frame=0;frame<240;++frame) {
        if(frame==120) {C2VitaResetHUDCache();for(int i=0;i<3;++i) old_level[i]=old_slots[i]=-1;}
        for(int i=0;i<3;++i) {
            int level=(frame/20+i)%4+1,slots=(frame/40+i)%4+6;
            gProgram_state.current_car.power_up_slots[i]=slots;
            const char* name=i==0?"A":i==1?"P":"O";
            uploads=copies=0;actors[i].render_style=0;
            DoPSPowerHeadup(level,name);
            unsigned expected=level!=old_level[i] || slots!=old_slots[i];
            assert(uploads==expected && copies==expected && actors[i].render_style==BR_RSTYLE_FACES);
            old_level[i]=level;old_slots[i]=slots;
            uint16_t actual[64*15];memcpy(actual,images[i],sizeof(actual));
            uploads=copies=0;BaselineDoPSPowerHeadup(level,name);
            assert(uploads==1 && copies==1 && !memcmp(actual,images[i],sizeof(actual)));
        }
    }
    unsigned char damage[64*1024+1]={0};
    assert(C2VitaDamageHUDChanged(damage,100));
    for(unsigned i=0;i<1000;++i) assert(!C2VitaDamageHUDChanged(damage,100));
    damage[67]=0x80;assert(C2VitaDamageHUDChanged(damage,100)); /* blinking/new damage */
    assert(!C2VitaDamageHUDChanged(damage,100));
    assert(C2VitaDamageHUDChanged(damage,99));
    C2VitaHUDReset();assert(C2VitaDamageHUDChanged(damage,99));
    assert(C2VitaDamageHUDChanged(damage,sizeof(damage)));
    assert(C2VitaDamageHUDChanged(damage,99)); /* oversized bypass invalidates */
    assert(C2VitaDamageHUDChanged(NULL,99));assert(C2VitaDamageHUDChanged(damage,0));
    puts("PASS production Vita APO bars:240frames/3bars pixel-identical to desktop baseline, unchanged bars zero copies/uploads, level/slot/reinit changes redraw, actors visible; damage byte snapshot detects changes/blinks/size/reset/bypass; ASan/UBSan; GL mocked");
}
