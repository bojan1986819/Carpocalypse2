#ifndef CARPOCALYPSE2_JOYSTICK_H
#define CARPOCALYPSE2_JOYSTICK_H

#include "c2_hooks.h"

#include "carpocalypse2_types.h"

#include <stdio.h>

extern int gJoystick_index;
extern int gUNK_00595f8c;
extern int gUNK_00596310;
extern float gJoystick_x_steering;
extern float gJoystick_y_throttle;
extern int gUNK_00595f9c;


extern int C2_HOOK_FASTCALL LoadJoystickPreferences(void);

void C2_HOOK_FASTCALL FUN_0045c240(void);

void C2_HOOK_FASTCALL FUN_0045c420(void);
void C2_HOOK_FASTCALL FUN_0045c910(tCar_spec* pCar, const br_vector3* pPosition, br_vector3* pForce, float pMagnitude);

int C2_HOOK_FASTCALL ReadJoystickInfo(FILE* pFile, tButtonJoystickInfo* pInfo);

int C2_HOOK_FASTCALL WriteJoystickInfo(FILE* pFile, tButtonJoystickInfo* pInfo);

int C2_HOOK_FASTCALL FUN_0045b7e0(FILE* pFile, char* pString);

extern float gForce_feedback_upper_limit;

extern float Force_feedback_lower_limit;

extern float gOriginal_joystick_x;

extern float gOriginal_joystick_y;

extern int gOriginal_joystick_fbb;

extern int gOriginal_joystick_dpad;

extern int gOrig_joystick_index;


extern tHeadup_text_buffer gJoystick_headup_buffer_0079d8a0;
extern tHeadup_text_buffer gHeadup_text_buffer_0079d880;

void C2_HOOK_FASTCALL SetupFFBValues(void);


void C2_HOOK_FASTCALL InitJoysticks(void);

float C2_HOOK_FASTCALL GetJoystickX(void);

float C2_HOOK_FASTCALL GetJoystickY(void);

int C2_HOOK_FASTCALL GetJoystickFBBGain(void);

void C2_HOOK_FASTCALL SetJoystickX(float pValue);

void C2_HOOK_FASTCALL SetJoystickY(float pValue);

void C2_HOOK_FASTCALL SetJoystickFFBGain(int pValue);

void C2_HOOK_FASTCALL SetJoystickDPadEnabled(int pEnabled);

int C2_HOOK_FASTCALL ToggleJoystickYInvert(void);

void C2_HOOK_FASTCALL SetAllFFBEffectsGain(int pGain);

int C2_HOOK_FASTCALL SetJoystickFFBForce(int pMagnitude);

int C2_HOOK_FASTCALL SetJoystickFFBSteeringForce(int pMagnitude);

int C2_HOOK_FASTCALL IncreaseJoystickFFBGain(void);

void C2_HOOK_FASTCALL FUN_0045a070(void);

void C2_HOOK_FASTCALL Joystick_BackupSettings(void);

void C2_HOOK_FASTCALL EnableJoysticks(void);

void C2_HOOK_FASTCALL DisableJoysticks(void);

void C2_HOOK_FASTCALL AdjustHeadupTextBuffer(tHeadup_text_buffer* pText_buffer);
int C2_HOOK_FASTCALL InsertHeadupText(tHeadup_text_buffer* pText_buffer, tHeadup_text* pText, int pSortAlphabetically);

int C2_HOOK_FASTCALL FUN_CheckJoystickHeadupButtons(tButtonJoystickInfo** pJoystick_info);

void C2_HOOK_FASTCALL MungeJoystickHeadups(void);

int C2_HOOK_FASTCALL HasCurrentJoystick(void);

#endif /* CARPOCALYPSE2_JOYSTICK_H */
