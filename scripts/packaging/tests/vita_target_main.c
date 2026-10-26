#include <assert.h>
#include <stdio.h>
#include <string.h>
#define C2_HOOK_FASTCALL
enum {eNet_mode_none,eNet_game_type_foxy=5,eMiscString_no_target_to_lock_onto,eMiscString_locked_onto,eMiscString_target_lock_off};
struct car {int car_ID;};static struct car a={0x0102},b={0x0304},*gTarget_lock_car_1,*gTarget_lock_car_2;
static int gNet_mode,gTarget_lock_enabled,messages,names;
static struct {int type;struct {int show_players_on_map;} options;} game,*gCurrent_net_game=&game;
static struct {char player_name[32];} player={"NET"};
static const char* GetMiscString(int x) {(void)x;return "MESSAGE";}
static const char* GetDriverName(int x,int y) {assert(x==3 && y==4);++names;return "DRIVER";}
#define NetPlayerFromCar(c) ((void)(c),&player)
static void NewTextHeadupSlot2(int a,int b,int c,int d,const char*s,int e) {(void)a;(void)b;(void)c;(void)d;(void)e;assert(s);++messages;}
#include "target.h"
int main(void) {
 gTarget_lock_car_1=&a;gTarget_lock_car_2=NULL;ToggleTargetLock();assert(messages==1 && !names && !gTarget_lock_enabled);
 gTarget_lock_car_1=NULL;ToggleTargetLock();assert(messages==2);
 gTarget_lock_car_1=&a;gTarget_lock_car_2=&b;ToggleTargetLock();assert(gTarget_lock_enabled && names==1);ToggleTargetLock();assert(!gTarget_lock_enabled && names==1);
 gNet_mode=1;game.type=eNet_game_type_foxy;unsigned n=messages;ToggleTargetLock();assert(messages==n);
 puts("PASS production target-lock:null secondary target from device dump handled; valid target on/off and network exclusions retained;ASan/UBSan");
}
