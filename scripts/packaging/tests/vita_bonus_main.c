#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint32_t tU32;
enum {eTime_bonus_initial_pause=1,eTime_bonus_race_bonus,eTime_bonus_tb_up,eTime_bonus_tb_pause,eTime_bonus_tb_down,eTime_bonus_end_pause};
/* Production gTimer is a signed int (retail); OLD_UNSIGNED_TIMER rebuilds the old decomp's tU32. */
#ifdef OLD_UNSIGNED_TIMER
static uint32_t gTimer;
#else
static int gTimer;
#endif
static uint32_t now,gTime_bonus_start,last_rattle_time;static int gTime_bonus_state,gCompletion_bonus_post_race,gTime_bonus,gRace_finished;
static struct {int credits,skill_level;} gProgram_state;
static int gCredits_per_second_time_bonus[3]={2,3,4},gPedestrians_outlet,gRace_bonus_headup,gTime_bonus_headup;
enum{eMiscString_completion_bonus_colon,eMiscString_time_bonus_colon,eSoundId_Clap};
static tU32 GetTotalTime(void) {return now;}
static const char* GetMiscString(int x) {(void)x;return "BONUS";}
static void DRS3StartSound(int x,int y) {(void)x;(void)y;}
static void ChangeHeadupText(int x,const char*s) {(void)x;assert(strlen(s)<64);}
static void step(void) {tU32 the_time;int bonus;char the_text[128];
#include "bonus.h"
}
int main(void) {
 const uint32_t timers[]={1,999,1000,123456,600000},deltas[]={16,33,100,250,1000};
 for(unsigned i=0;i<5;i++)for(unsigned j=0;j<5;j++)for(unsigned wrap=0;wrap<2;wrap++) {
  now=wrap?UINT32_MAX-3000:500000;gTime_bonus_start=now;gTimer=(int)timers[i];last_rattle_time=0;gTime_bonus_state=1;gTime_bonus=0;
  gProgram_state.credits=0;gProgram_state.skill_level=0;gCompletion_bonus_post_race=5000;gRace_finished=10000;
  unsigned frames=0;while(gRace_finished!=1 && ++frames<10000) {now+=deltas[j];step();assert((uint32_t)gTimer<=timers[i]);}
  assert(gRace_finished==1 && gTimer==0 && gTime_bonus==0);
  assert(gProgram_state.credits==5000+(int)(timers[i]/1000)*2);
 }
 puts("PASS production race-end state machine:50timer/frame-step/wrap cases finish with zero timer/bonus and exact awarded credits; ASan/UBSan, sounds/headups mocked");
}
