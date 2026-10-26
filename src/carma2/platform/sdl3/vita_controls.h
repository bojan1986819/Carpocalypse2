#ifndef C2_VITA_CONTROLS_H
#define C2_VITA_CONTROLS_H
#include "carpocalypse2_types.h"
enum {
    C2V_UP=1, C2V_DOWN=2, C2V_LEFT=4, C2V_RIGHT=8,
    C2V_CROSS=16, C2V_CIRCLE=32, C2V_SQUARE=64, C2V_TRIANGLE=128,
    C2V_L=256, C2V_R=512, C2V_START=1024, C2V_SELECT=2048
};
typedef struct { unsigned buttons; int lx,ly,rx,ry; } C2VitaPad;
void C2VitaMapKeys(const C2VitaPad* pad, int racing, const int* bindings, int* keys, int mark);
int C2VitaSteering(int axis);
#ifdef __vita__
void C2VitaReadKeys(int* keys,int mark);
void C2VitaReadSteering(tJoystick* joystick);
void C2VitaLog(const char* format, ...);
void C2VitaCheckpoint(const char* title);
const char* C2VitaBootError(void);
int C2VitaOpenBootLog(void);
int C2VitaInitSDL(void);
int C2VitaPrepareData(void);
const char* C2VitaDataHint(void);
#endif
#endif
