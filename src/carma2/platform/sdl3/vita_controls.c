/* Pure button mapping is shared with the host-side control fixture. */
#include "vita_controls.h"
static void key(int* keys,int mark,int index) {
    if (index>=0 && index<151) keys[index]=mark;
}
static void action(int* keys,int mark,const int* bindings,int index) {
    key(keys,mark,bindings[index]);
}
int C2VitaSteering(int axis) {
    int magnitude=axis<0 ? -axis : axis;
    if (magnitude<=24) return 0;
    if (magnitude>127) magnitude=127;
    magnitude=(magnitude-24)*65535/103;
    return axis<0 ? -magnitude : magnitude;
}
void C2VitaMapKeys(const C2VitaPad* pad,int racing,const int* bindings,int* keys,int mark) {
    unsigned b=pad->buttons;
    if (!racing) {
        if ((b&C2V_LEFT) || pad->lx<-48) key(keys,mark,70);
        if ((b&C2V_RIGHT) || pad->lx>48) key(keys,mark,71);
        if ((b&C2V_UP) || pad->ly<-48) key(keys,mark,72);
        if ((b&C2V_DOWN) || pad->ly>48) key(keys,mark,73);
        if (b&C2V_CROSS) key(keys,mark,51);
        if (b&(C2V_CIRCLE|C2V_START)) key(keys,mark,63);
        return;
    }
    if (b&C2V_START) key(keys,mark,63);
    /* Select + Square/Triangle/Circle buys armour/power/offense with credits (C2 has no upgrade
     * menu, APO is bought in the race); a plain Select tap toggles the map when released. */
    {
        static int select_held, select_combo, map_pulse;
        if (b&C2V_SELECT) {
            if (!select_held) select_combo=0;
            select_held=1;
            if (b&(C2V_SQUARE|C2V_TRIANGLE|C2V_CIRCLE)) select_combo=1;
            if (b&C2V_SQUARE) action(keys,mark,bindings,67);
            if (b&C2V_TRIANGLE) action(keys,mark,bindings,68);
            if (b&C2V_CIRCLE) action(keys,mark,bindings,69);
            b&=~(unsigned)(C2V_SQUARE|C2V_TRIANGLE|C2V_CIRCLE|C2V_SELECT);
        } else if (select_held) {
            select_held=0;
            if (!select_combo) map_pulse=3; /* a few polls so the edge-triggered toggle sees it */
        }
        if (map_pulse>0) { --map_pulse; action(keys,mark,bindings,57); }
    }
    if (b&C2V_R) action(keys,mark,bindings,49);
    if (b&C2V_L) action(keys,mark,bindings,50);
    if (b&C2V_LEFT) action(keys,mark,bindings,47);
    if (b&C2V_RIGHT) action(keys,mark,bindings,48);
    if (b&C2V_CROSS) action(keys,mark,bindings,54);
    if (b&C2V_SQUARE) action(keys,mark,bindings,45);
    if (b&C2V_CIRCLE) action(keys,mark,bindings,60);
    if (b&C2V_TRIANGLE) action(keys,mark,bindings,72);
    /* the gearbox is automatic, so the d-pad up/down gear keys are free: power-ups instead */
    if (b&C2V_UP) action(keys,mark,bindings,61);   /* use selected power-up */
    if (b&C2V_DOWN) action(keys,mark,bindings,63); /* next power-up in the inventory */
    if (pad->rx<-48) action(keys,mark,bindings,31);
    if (pad->rx>48) action(keys,mark,bindings,32);
    if (pad->ry<-48) action(keys,mark,bindings,33);
    if (pad->ry>48) action(keys,mark,bindings,34);
}
