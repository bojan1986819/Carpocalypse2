/* Synthetic native-pad snapshots; no game or hardware is launched. */
#include "vita_controls.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
static int keys[151],bindings[77];
static void map(unsigned buttons,int racing,int lx,int ly) {
    C2VitaPad p={0};p.buttons=buttons;p.lx=lx;p.ly=ly;
    memset(keys,0,sizeof(keys));C2VitaMapKeys(&p,racing,bindings,keys,7);
}
int main(void) {
    int i,prev=0;
    const unsigned buttons[]={C2V_R,C2V_L,C2V_LEFT,C2V_RIGHT,C2V_CROSS,C2V_SQUARE,C2V_CIRCLE,C2V_TRIANGLE,C2V_UP,C2V_DOWN};
    const int actions[]={49,50,47,48,54,45,60,72,61,63};
    for(i=0;i<77;i++)bindings[i]=i+20;
    for(i=0;i<10;i++){map(buttons[i],1,0,0);assert(keys[bindings[actions[i]]]==7);}
    map(C2V_CROSS,0,0,0);assert(keys[51]==7 && !keys[bindings[54]]);
    map(C2V_CIRCLE,0,0,0);assert(keys[63]==7 && !keys[bindings[60]]);
    map(C2V_START,1,0,0);assert(keys[63]==7);
    map(C2V_LEFT|C2V_UP,0,0,0);assert(keys[70]==7 && keys[72]==7);
    map(0,0,80,80);assert(keys[71]==7 && keys[73]==7);
    map(C2V_SELECT,1,0,0);assert(!keys[bindings[57]]);
    map(0,1,0,0);assert(keys[bindings[57]]==7);map(0,1,0,0);map(0,1,0,0);map(0,1,0,0);assert(!keys[bindings[57]]);
    map(C2V_SELECT|C2V_SQUARE,1,0,0);assert(keys[bindings[67]]==7 && !keys[bindings[45]]);
    map(C2V_SELECT|C2V_TRIANGLE,1,0,0);assert(keys[bindings[68]]==7 && !keys[bindings[72]]);
    map(C2V_SELECT|C2V_CIRCLE,1,0,0);assert(keys[bindings[69]]==7 && !keys[bindings[60]]);
    map(0,1,0,0);assert(!keys[bindings[57]]);
    map(C2V_R|C2V_SQUARE,1,0,0);assert(keys[bindings[49]]==7 && keys[bindings[45]]==7);
    assert(C2VitaSteering(0)==0 && C2VitaSteering(-24)==0 && C2VitaSteering(24)==0);
    assert(C2VitaSteering(127)==65535 && C2VitaSteering(-128)==-65535);
    for(i=0;i<=127;i++){int s=C2VitaSteering(i);assert(s>=prev);assert(C2VitaSteering(-i)==-s);prev=s;}
    puts("PASS Vita action bindings, menu/race context, combos, deadzone, full-range monotonic symmetric steering; synthetic snapshots, no hardware/game");
    return 0;
}
