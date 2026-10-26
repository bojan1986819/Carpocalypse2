/* porttest: link stubs for platform entry points that the SDL3 platform does not
 * implement yet (joystick/FFB via DirectInput, DirectPlay-style network host,
 * MIDI/CDA channel control). Each logs once through NOT_IMPLEMENTED(). */
#include "c2_hooks.h"

#include <stddef.h>

int gUNK_00595f9c = 1;
int gForceFeedbackAvailable = 0;
int gCurrent_joystick_effect_index = -1;

const char* C2_HOOK_FASTCALL GetCurrentJoystickName(void) { NOT_IMPLEMENTED(); return "none"; }
int C2_HOOK_FASTCALL IsJoystickPresent(int pIndex) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL SetJoystickFFBCollisionForce(long pMagnitude, long pDirection) { NOT_IMPLEMENTED(); return 0; }
void C2_HOOK_FASTCALL SelectNextDInputJoystick(void) { NOT_IMPLEMENTED(); }
int C2_HOOK_FASTCALL PDFindJoystickEffect(const char* effectName) { NOT_IMPLEMENTED(); return -1; }
void C2_HOOK_FASTCALL StartDinputEffect(int pIndex) { NOT_IMPLEMENTED(); }
void C2_HOOK_FASTCALL StopDinputEffect(int pIndex) { NOT_IMPLEMENTED(); }
void C2_HOOK_FASTCALL UnloadDinputFFBEffectAtIndex(int pIndex) { NOT_IMPLEMENTED(); }
void* C2_HOOK_FASTCALL FUN_00458430(int pIndex) { NOT_IMPLEMENTED(); return NULL; }

int C2_HOOK_STDCALL PDNetHostGame(void** pHost_address) { NOT_IMPLEMENTED(); return 0; }
void C2_HOOK_FASTCALL PDNetSetPlayerSystemInfo(void* pPlayer_info, const void* pSender_address) { NOT_IMPLEMENTED(); }

int C2_HOOK_FASTCALL PDS3ResumeMidiChannel(void* pChannel) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3UnmuteMidiChannel(void* pChannel) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3MuteMidiChannel(void* pChannel) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3PauseMidiChannel(void* pChannel) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3StopMidiChannelWrapper(void* pChannel, int a2, int a3, int a4, int a5, int a6) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3UpdateMidiChannelPitch(void* pChannel, int pPitch) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3ResumeCDA(void* pChannel) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3PauseCDA(unsigned int pUnused_arg) { NOT_IMPLEMENTED(); return 0; }
int C2_HOOK_FASTCALL PDS3Return0_B(void) { return 0; }
