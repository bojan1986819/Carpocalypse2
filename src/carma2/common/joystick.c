#include "joystick.h"

#include "controls.h"
#include "displays.h"
#include "globvars.h"
#include "input.h"
#include "loading.h"
#include "platform.h"
#include "utility.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"

#ifdef CARPOCALYPSE2_MATCHING
#include "win32/win32_dinput.h"
#endif

#include <math.h>

#include "c2_string.h"
#include "joystick.h"

#include "loading1.h"
#include "utility.h"
#include "platform.h"
#include "globvars.h"
#include "packfile.h"
#include "input.h"
#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"
#include "c2_string.h"
#include <string.h>
extern void C2_HOOK_FASTCALL ChangeHeadupFont(int pHeadup_index, int pFont);
extern void C2_HOOK_FASTCALL ChangeHeadupText(int pHeadup_index, char* pText);
extern void C2_HOOK_FASTCALL FUN_0045a0c0(void);
extern const char* C2_HOOK_FASTCALL GetCurrentJoystickName(void);
// GLOBAL: CARMA2_HW 0x00595f84
int gUNK_00595f84 = 7500;
int C2_HOOK_FASTCALL InsertHeadupText(tHeadup_text_buffer* pText_buffer, tHeadup_text* pText, int pSortAlphabetically);
// GLOBAL: CARMA2_HW 0x00595f88
int gJoystick_index = -1;

// GLOBAL: CARMA2_HW 0x00596308
int gINT_00596308 = -1;

// GLOBAL: CARMA2_HW 0x00595f8c
int gUNK_00595f8c = -1;

// GLOBAL: CARMA2_HW 0x00596310
int gUNK_00596310 = 50;

// GLOBAL: CARMA2_HW 0x00595f90
float gJoystick_x_steering = 1.f;

// GLOBAL: CARMA2_HW 0x00595f94
float gJoystick_y_throttle = 1.f;



// GLOBAL: CARMA2_HW 0x00655e5c
float Force_feedback_lower_limit = 1.5f;
// GLOBAL: CARMA2_HW 0x00655e58
float gForce_feedback_upper_limit = 2.7f;
// GLOBAL: CARMA2_HW 0x00589f80
const double gJoystick_collision_force_scale = 10000.0;
// GLOBAL: CARMA2_HW 0x00589f88
const float gJoystick_collision_force_min_length = 0.0000002384185791015625f;
// GLOBAL: CARMA2_HW 0x00589f90
const double gJoystick_collision_force_one = 1.0;
// GLOBAL: CARMA2_HW 0x00589f98
const float gJoystick_collision_force_one_f = 1.0f;
// GLOBAL: CARMA2_HW 0x00589f9c
const float gJoystick_collision_force_zero_f = 0.0f;
// GLOBAL: CARMA2_HW 0x00589fa0
const double gJoystick_collision_force_radians_to_degrees = 57.29577951308;
// GLOBAL: CARMA2_HW 0x0079d8a0
tHeadup_text_buffer gJoystick_headup_buffer_0079d8a0;
// GLOBAL: CARMA2_HW 0x0079d880
tHeadup_text_buffer gHeadup_text_buffer_0079d880;
// GLOBAL: CARMA2_HW 0x00688700
int gOrig_joystick_index;

extern int gINT_0068b8e4;
extern int gINT_0068b8e8;
extern int gRace_head_ups[22];

// GLOBAL: CARMA2_HW 0x00688704
float gOriginal_joystick_x;
// GLOBAL: CARMA2_HW 0x00688708
float gOriginal_joystick_y;
// GLOBAL: CARMA2_HW 0x0068870c
int gOriginal_joystick_fbb;
// GLOBAL: CARMA2_HW 0x00688710
int gOriginal_joystick_dpad;
int gOrig_joystick_index;

/* ported from common/joystick.c */
void C2_HOOK_FASTCALL SetupFFBValues(void) {
    tPath_name path;
    FILE *f;
    char s[256];

    PathCat(path, gApplication_path, "FFB.TXT");
    f = DRfopen(path, "rt");
    GetALineAndDontArgue(f, s);
    sscanf(strtok(s, "\t ,/"), "%f", &gForce_feedback_upper_limit);
    GetALineAndDontArgue(f, s);
    sscanf(strtok(s, "\t ,/"), "%f", &Force_feedback_lower_limit);
    PFfclose(f);
}

#ifdef CARPOCALYPSE2_MATCHING
extern int gCountEnumeratedJoystickDinputDevices;
extern IDirectInputA* gDirectInputJoystickHandle;
extern int gDirectInputJoystickEnumerated;
extern BOOL CALLBACK Win32DInputJoystickEnum(const DIDEVICEINSTANCEA* pDeviceInstance, void* pContext);
extern tDirectInputJoystickInfo* C2_HOOK_FASTCALL JoystickDInputGetInfo(int pJoystickIndex, tU32* pButtonMask, tU32* pAxisMask);
#endif
extern void C2_HOOK_CDECL dr_dprintf(const char* fmt_string, ...);

// C2PORT-NONMATCHING: best 84.96% | difftest PASS 2000 trials (spec difftest/specs/004575b0_InitJoysticks.json) | reason VC5 register allocation and initialization-loop layout
// FUNCTION: CARMA2_HW 0x004575b0
void C2_HOOK_FASTCALL InitJoysticks(void) {
#ifdef CARPOCALYPSE2_MATCHING
    int i;
    int joystick_index;
    IDirectInputDevice2A** devices;
    tU32* fields;
    IDirectInputDevice2A* device;

    if (gCountEnumeratedJoystickDinputDevices == 0) {
        if (gDirectInputJoystickHandle == NULL) {
            C2_HOOK_BUG_ON(sizeof(tDirectInputJoystickInfo) != 0x68);
            memset(gDirectInputJoystickDevices, 0, 16 * sizeof(*gDirectInputJoystickDevices));
            fields = (tU32*)&gDirectInputJoystickInfos[0].axisMask;
            do {
                fields[-1] = 0;
                fields[0] = 0;
                fields[1] = 0;
                ((tU8*)fields)[8] = 0;
                fields[0x17] = 0;
                fields[0x18] = 0;
                fields += 0x1a;
            } while ((int)fields < (int)((tU32*)gDirectInputJoystickInfos + 0x1a * 16 + 1));
            memset(gDirectInputEffects, 0, 30 * sizeof(*gDirectInputEffects));
            gJoystickFFB = 0;
            gCountEnumeratedJoystickDinputDevices = 0;
            DirectInputCreateA(GetModuleHandleA(NULL), 0x0500, &gDirectInputJoystickHandle, NULL);
            if (gDirectInputJoystickHandle == NULL) {
                goto finish;
            }
            gDirectInputJoystickEnumerated = 0;
        }
        if (!gDirectInputJoystickEnumerated) {
            gDirectInputJoystickEnumerated = 1;
            if (gCountEnumeratedJoystickDinputDevices == 0) {
                gDirectInputJoystickHandle->lpVtbl->EnumDevices(gDirectInputJoystickHandle, DIDEVTYPE_JOYSTICK, Win32DInputJoystickEnum, gDirectInputJoystickHandle, DIEDFL_ATTACHEDONLY);
            }
            if (gCountEnumeratedJoystickDinputDevices != 0) {
                i = 0;
                devices = gDirectInputJoystickDevices;
                do {
                    JoystickDInputGetInfo(i, NULL, NULL);
                    device = *devices;
                    if (device != NULL && gDirectInputJoystickHandle != NULL) {
                        HRESULT acquire_result;
                        acquire_result = IDirectInputDevice2_Acquire(device);
                        if (acquire_result == DI_OK) {
                            dr_dprintf("Joystick #%d acquired\n", i);
                        } else if (acquire_result == DI_NOEFFECT) {
                            dr_dprintf("Joystick #%d already acquired\n", i);
                        } else if (acquire_result == DIERR_INVALIDPARAM) {
                            dr_dprintf("Joystick %d DIERR_INVALIDPARAM\n", i);
                        } else if (acquire_result == DIERR_OTHERAPPHASPRIO) {
                            dr_dprintf("Joystick %d DIERR_OTHERAPPHASPRIO\n", i);
                        } else {
                            dr_dprintf("Unknown Error acquiring joystick %d\n", i);
                        }
                    }
                    devices++;
                    i++;
                } while ((int)devices < (int)(gDirectInputJoystickDevices + CARPOCALYPSE2_ASIZE(gDirectInputJoystickDevices)));
                gJoystick_index = 0;
            }
            if (gJoystick_index >= 0 && InitForceFeedback()) {
                ResetDInputJoystickFFB(gJoystick_index);
                RegisterJoystickFFBForces();
            }
        }
    }
finish:
    if (gJoystickFFB != 0) {
        joystick_index = 0;
        devices = gDirectInputJoystickDevices;
        do {
            device = *devices;
            if (device != NULL && (gJoystickFFB & (1 << joystick_index)) != 0) {
                IDirectInputDevice2_SendForceFeedbackCommand(device, DISFFC_RESET);
            }
            devices++;
            joystick_index++;
        } while ((int)devices < (int)(gDirectInputJoystickDevices + CARPOCALYPSE2_ASIZE(gDirectInputJoystickDevices)));
    }
#else
    PDInitJoysticks();
#endif
}

typedef struct {
    const char* name;
    int key;
} tJoystick_action_entry;

// GLOBAL: CARMA2_HW 0x00596250
tJoystick_action_entry gJoystick_actions[21] = {
    { "Recover", 0x3c },
    { "Reapir", 0x2d },
    { "Hand Brake", 0x36 },
    { "Use Powerup", 0x3d },
    { "Next Powerup", 0x40 },
    { "Prev Powerup", 0x3f },
    { "Wheel Spin", 0x38 },
    { "Map", 0x39 },
    { "Cockpit View", 0x2e },
    { "Buy Armour", 0x43 },
    { "Buy Power", 0x44 },
    { "Buy Offensive", 0x45 },
    { "Target Lock", 0x48 },
    { "Cycle Target", 0x49 },
    { "Camera Mode", 0x4b },
    { "Doors", 0x4a },
    { "Horn", 0x3b },
    { "Accelerate", 0x31 },
    { "Brake", 0x32 },
    { "Left", 0x2f },
    { "Right", 0x30 },
};

// FUNCTION: CARMA2_HW 0x0045c240
void C2_HOOK_FASTCALL FUN_0045c240(void) {
    int x;
    tJoystick_action_entry* entry;
    int* headup;
    char* dest;

    if (gINT_0068b8e8) {
        gINT_0068b8e8 = 0;
        if (gAcc_poly_actor != NULL && gRender_acc_poly_actor != 0) {
            BrActorRemove(gAcc_poly_actor);
            gRender_acc_poly_actor = 0;
        }
    } else {
        gINT_0068b8e8 = 1;
        PlaceHeadupAt(gProgram_state.frame_rate_headup, 10, 350);
        x = 120;
        headup = gRace_head_ups;
        do {
            PlaceHeadupAt(*headup, 10, x);
            ChangeHeadupFont(*headup, -2);
            headup++;
            x += 10;
        } while (x < 340);
        CreateAccentPolyActor(8.0f, 118.0f, 130.0f, 224.0f);
        if (gJoystick_headup_buffer_0079d8a0.count == 0) {
            if (gJoystick_headup_buffer_0079d8a0.field_0x14 < 5) {
                gJoystick_headup_buffer_0079d8a0.field_0x14 = 10;
            }
            gJoystick_headup_buffer_0079d8a0.texts = NULL;
            gJoystick_headup_buffer_0079d8a0.count = 0;
            gJoystick_headup_buffer_0079d8a0.pos_last_add = 0;
            gJoystick_headup_buffer_0079d8a0.text_last_add = NULL;
            gJoystick_headup_buffer_0079d8a0.field_0x4 = NULL;
            gJoystick_headup_buffer_0079d8a0.field_0x10 = 22;
            entry = gJoystick_actions;
            do {
                dest = malloc(0x3c);
                if (dest != NULL) {
                    *(int*)(dest + 0x38) = 0;
                    strncpy(dest, entry->name, 0x28);
                    InsertHeadupText(&gJoystick_headup_buffer_0079d8a0, (tHeadup_text*)dest, 0);
                }
                entry++;
            } while ((int)entry < (int)(gJoystick_actions + CARPOCALYPSE2_ASIZE(gJoystick_actions)));
        }
    }
}

// FUNCTION: CARMA2_HW 0x0045c590
unsigned int C2_HOOK_FASTCALL GetJoystickButtonIndexForKey(int pKey) {
    tButtonJoystickInfo* joystick_info;
    unsigned int i;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        for (i = 0; i < joystick_info->count_buttons; i++) {
            if (gJoystick_actions[joystick_info->buttons[i]].key == pKey) {
                return i;
            }
        }
    }
    return -1;
}

// FUNCTION: CARMA2_HW 0x0045c8d0
float C2_HOOK_FASTCALL GetJoystickX(void) {

    tButtonJoystickInfo* joystick_info;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        return joystick_info->field_0xd8;
    }
    return gJoystick_x_steering;
}

// FUNCTION: CARMA2_HW 0x0045c8f0
float C2_HOOK_FASTCALL GetJoystickY(void) {

    tButtonJoystickInfo* joystick_info;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        return joystick_info->field_0xdc;
    }
    return gJoystick_y_throttle;
}

// FUNCTION: CARMA2_HW 0x0045c7f0
int C2_HOOK_FASTCALL GetJoystickFBBGain(void) {

    tButtonJoystickInfo* joystick_info;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        return joystick_info->field_0xe4;
    }
    return gUNK_00596310;
}

// FUNCTION: CARMA2_HW 0x0045c810
void C2_HOOK_FASTCALL SetJoystickX(float pValue) {
    tButtonJoystickInfo* joystick;

    joystick = PDGetCurrentJoystickData();
    if (joystick != NULL) {
        joystick->field_0xd8 = pValue;
        if (joystick->field_0xd8 > 2.0) {
            joystick->field_0xd8 = 2.0f;
        }
        if (joystick->field_0xd8 < 0.0) {
            joystick->field_0xd8 = 0.0f;
        }
        gJoystick_x_steering = joystick->field_0xd8;
    }
}

// FUNCTION: CARMA2_HW 0x0045c870
void C2_HOOK_FASTCALL SetJoystickY(float pValue) {
    tButtonJoystickInfo* joystick;

    joystick = PDGetCurrentJoystickData();
    if (joystick != NULL) {
        joystick->field_0xdc = pValue;
        if (joystick->field_0xdc > 2.0) {
            joystick->field_0xdc = 2.0f;
        }
        if (joystick->field_0xdc < 0.0) {
            joystick->field_0xdc = 0.0f;
        }
        gJoystick_y_throttle = joystick->field_0xdc;
    }
}

// FUNCTION: CARMA2_HW 0x0045c7b0
void C2_HOOK_FASTCALL SetJoystickFFBGain(int pValue) {

    tButtonJoystickInfo* data;

    data = PDGetCurrentJoystickData();
    if (data != NULL) {
        if (pValue < 0) {
            pValue = 0;
        }
        if (pValue > 100) {
            pValue = 100;
        }
        data->field_0xe4 = pValue;
        gUNK_00596310 = pValue;
        SetAllFFBEffectsGain(pValue);
    }
}

extern void C2_HOOK_FASTCALL SelectNextDInputJoystick(void);

// FUNCTION: CARMA2_HW 0x0045c420
void C2_HOOK_FASTCALL FUN_0045c420(void) {
    tButtonJoystickInfo* joystick;

    SelectNextDInputJoystick();
    joystick = PDGetCurrentJoystickData();
    if (joystick != NULL) {
        gJoystick_x_steering = joystick->field_0xd8;
        gJoystick_y_throttle = joystick->field_0xdc;
        gUNK_00595f9c = joystick->field_0xd4;
    }
}

// FUNCTION: CARMA2_HW 0x0045c570
void C2_HOOK_FASTCALL SetJoystickDPadEnabled(int pEnabled) {
    tButtonJoystickInfo* joystick_info;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        joystick_info->field_0xe0 = pEnabled;
    }
}

void C2_HOOK_FASTCALL Joystick_BackupSettings(void) {

    gOrig_joystick_index = gJoystick_index;
    gOriginal_joystick_x = GetJoystickX();
    gOriginal_joystick_y = GetJoystickY();
    gOriginal_joystick_fbb = GetJoystickFBBGain();
    gOriginal_joystick_dpad = PDIsJoystickDPadEnabled();
    gJoystick_index = gOrig_joystick_index;
}

// FUNCTION: CARMA2_HW 0x0045bcc0
int C2_HOOK_FASTCALL CountAmpersandLines(FILE* pFile) {
    char s[256];
    long start;
    long end;
    int count;

    count = 0;
    rewind(pFile);
    start = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    end = ftell(pFile);
    if (end - start < 0x20) {
        return 0;
    }
    rewind(pFile);
    while (!feof(pFile)) {
        GetAString(pFile, s);
        if (s[0] == '&') {
            count++;
        }
    }
    rewind(pFile);
    return count;
}

int C2_HOOK_FASTCALL InsertHeadupText(tHeadup_text_buffer* pText_buffer, tHeadup_text* pText, int pSortAlphabetically);

// C2PORT-NONMATCHING: best 93.33% | difftest PASS 2000 trials (spec difftest/specs/0045a7f0_FUN_0045a7f0.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x0045a7f0
int C2_HOOK_FASTCALL FUN_0045a7f0(char* pFilename, int* pArg2) {
    char local_100[256];
    char* substr;
    void* this_ptr;
    char* mem;

    this_ptr = (void*)pArg2[0];
    substr = (char*)pArg2[1];
    memcpy(local_100, pFilename, strlen(pFilename) + 1);
    DRstrlwr(local_100);
    if (strstr(local_100, "POWERUP") == NULL && strstr(local_100, "CPOINT") == NULL && strstr(local_100, "TRACKX") == NULL) {
        if (strstr(local_100, substr) == NULL) {
            if (strcmp(substr, ".*") != 0) {
                return 0;
            }
        }
        mem = (char*)malloc(0x3c);
        if (mem != NULL) {
            *(int*)(mem + 0x38) = 0;
            strncpy(mem, local_100, 0x28);
            StringTransformToLower(mem);
            mem[0] = (char)toupper((int)*mem);
            InsertHeadupText(this_ptr, (tHeadup_text*)mem, 1);
        }
    }
    return 0;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 59.59% | difftest PASS 2000 trials (spec difftest/specs/0045a920_InsertHeadupText.json) | reason vc5-regalloc-string
// FUNCTION: CARMA2_HW 0x0045a920
int C2_HOOK_FASTCALL InsertHeadupText(tHeadup_text_buffer* pText_buffer, tHeadup_text* pText, int pSortAlphabetically) {
    tHeadup_text* pHead;
    tHeadup_text* pPrevious;
    tHeadup_text* pNext;
    int pos;
    int comparison;

    pos = 0;
    pHead = pText_buffer->texts;
    if (pHead == NULL) {
        pText_buffer->texts = pText;
        pText->next = NULL;
        pText_buffer->count = 1;
        pText_buffer->pos_last_add = 0;
        pText_buffer->text_last_add = pText;
        return 0;
    }
    if (!pSortAlphabetically) {
        pPrevious = pHead;
        for (pNext = pHead->next; pNext != NULL; pNext = pPrevious->next) {
            pPrevious = pNext;
            pos++;
        }
        pText->next = NULL;
        pPrevious->next = pText;
    } else {
        comparison = strcmp(pHead->text, pText->text);
        if (comparison > 0) {
            pText_buffer->texts = pText;
            pText->next = pHead;
            pPrevious = pHead;
        } else {
            pPrevious = pHead;
            pNext = pHead->next;
            while (pNext != NULL) {
                comparison = strcmp(pNext->text, pText->text);
                if (comparison > 0) {
                    pos++;
                    pText->next = pNext;
                    pPrevious->next = pText;
                    break;
                }
                pPrevious = pNext;
                pNext = pPrevious->next;
                pos++;
            }
            if (pNext == NULL) {
                pText->next = NULL;
                pPrevious->next = pText;
            }
        }
    }
    pText_buffer->count++;
    pText_buffer->pos_last_add = pos;
    pText_buffer->text_last_add = pPrevious;
    return pos;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x0045b790
void C2_HOOK_FASTCALL FUN_0045b790(void* pThis) {
    tJoystick_action_entry* entry;
    char* dest;

    entry = gJoystick_actions;
    do {
        dest = malloc(0x3c);
        if (dest != NULL) {
            *(int*)(dest + 0x38) = 0;
            strncpy(dest, entry->name, 0x28);
            InsertHeadupText(pThis, (tHeadup_text*)dest, 0);
        }
        entry++;
    } while ((int)entry < (int)(gJoystick_actions + CARPOCALYPSE2_ASIZE(gJoystick_actions)));
}

// FUNCTION: CARMA2_HW 0x0045b7e0
int C2_HOOK_FASTCALL FUN_0045b7e0(FILE* pFile, char* pString) {
    fpos_t file_pos;
    char search_string[256];
    char line[256];
    unsigned int i;
    long start;
    long end;

    rewind(pFile);
    start = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    end = ftell(pFile);
    if (end - start < 0x20) {
        return -1;
    }
    strcpy(search_string, pString);
    for (i = 0; i < strlen(search_string); i++) {
        if (search_string[i] == ' ') {
            search_string[i] = '_';
        }
    }
    rewind(pFile);
    while (!feof(pFile)) {
        if (fgetpos(pFile, &file_pos) != 0) {
            PDFatalError("getpos error");
        }
        GetAString(pFile, line);
        if (strstr(line, search_string) != NULL) {
            if (fsetpos(pFile, &file_pos) != 0) {
                PDFatalError("setpos error");
            }
            return 1;
        }
    }
    rewind(pFile);
    return 0;
}

// FUNCTION: CARMA2_HW 0x0045b920
int C2_HOOK_FASTCALL WriteJoystickInfo(FILE* pFile, tButtonJoystickInfo* pInfo) {
    char formatted[256];
    char joystick_name[256];
    unsigned int i;

    strcpy(joystick_name, pInfo->productName);
    for (i = 0; i < strlen(joystick_name); i++) {
        if (joystick_name[i] == ' ') {
            joystick_name[i] = '_';
        }
    }
    sprintf(formatted, "&%s\n", joystick_name);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "%d\n", 0x20);
    fwrite(formatted, 1, strlen(formatted), pFile);
    for (i = 0; i < 0x20; i++) {
        sprintf(formatted, "%d\n", pInfo->buttons[i]);
        fwrite(formatted, 1, strlen(formatted), pFile);
    }
    sprintf(formatted, "%d\n", pInfo->field_0xd4);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "%.3f, %.3f\n", (double)pInfo->field_0xd8, (double)pInfo->field_0xdc);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "%d\n", pInfo->field_0xe0);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "%d\n", pInfo->field_0xe4);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "%d\n", pInfo->field_0xe8);
    fwrite(formatted, 1, strlen(formatted), pFile);
    sprintf(formatted, "\n");
    fwrite(formatted, 1, strlen(formatted), pFile);
    return 1;
}

// FUNCTION: CARMA2_HW 0x0045bbc0
int C2_HOOK_FASTCALL ReadJoystickInfo(FILE* pFile, tButtonJoystickInfo* pInfo) {
    char name[256];
    unsigned int i;
    size_t count;

    GetAString(pFile, name);
    for (i = 0; i < strlen(name); i++) {
        if (name[i] == '_') {
            name[i] = ' ';
        }
    }
    strcpy(pInfo->productName, name + 1);

    count = GetAnInt(pFile);
    for (i = 0; i < CARPOCALYPSE2_ASIZE(pInfo->buttons); i++) {
        if (i < count) {
            pInfo->buttons[i] = GetAnInt(pFile);
        } else {
            pInfo->buttons[i] = -1;
        }
    }
    pInfo->field_0xd4 = GetAnInt(pFile);
    GetPairOfScalars(pFile, &pInfo->field_0xd8, &pInfo->field_0xdc);
    pInfo->field_0xe0 = GetAnInt(pFile);
    pInfo->field_0xe4 = GetAnInt(pFile);
    pInfo->field_0xe8 = GetAnInt(pFile);
    return 1;
}

// FUNCTION: CARMA2_HW 0x0045c0b0
int C2_HOOK_FASTCALL LoadJoystickPreferences(void) {
    tPath_name path;
    tButtonJoystickInfo* info;
    FILE* f;

    info = PDGetCurrentJoystickData();
    if (info == NULL) {
        return 0;
    }
    PathCat(path, gApplication_path, "JOYSTICK.TXT");
    f = fopen(path, "rt");
    if (f == NULL) {
        return 0;
    }
    if (FUN_0045b7e0(f, GetCurrentJoystickName()) > 0) {
        ReadJoystickInfo(f, info);
        fclose(f);
    } else {
        fclose(f);
        remove(path);
    }
    gJoystick_x_steering = info->field_0xd8;
    gJoystick_y_throttle = info->field_0xdc;
    gUNK_00595f9c = info->field_0xd4;
    gUNK_00595f84 = info->field_0xe4;
    gUNK_00596310 += 25;
    if (gUNK_00596310 > 100) {
        gUNK_00596310 = 0;
    }
    SetAllFFBEffectsGain(gUNK_00596310);
    if (info->field_0xe8 != 0) {
        EnableJoysticks();
    } else {
        DisableJoysticks();
    }
    return 1;
}

// FUNCTION: CARMA2_HW 0x0045bd50
void C2_HOOK_FASTCALL EnableJoysticks(void) {
    tButtonJoystickInfo* joystick_info;

    FUN_0045a070();
    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        joystick_info->field_0xe8 = 1;
    }
}

// FUNCTION: CARMA2_HW 0x0045bd70
void C2_HOOK_FASTCALL DisableJoysticks(void) {
    tButtonJoystickInfo* joystick_info;

    joystick_info = PDGetCurrentJoystickData();
    if (joystick_info != NULL) {
        joystick_info->field_0xe8 = 0;
    }
    FUN_0045a0c0();
}

// FUNCTION: CARMA2_HW 0x0045b0a0
void C2_HOOK_FASTCALL AdjustHeadupTextBuffer(tHeadup_text_buffer* pText_buffer) {
    tHeadup_text* node;
    tHeadup_text* current;
    int index;
    int i;

    node = pText_buffer->texts;
    if (node == NULL) {
        return;
    }
    if (pText_buffer->field_0x4 == NULL) {
        pText_buffer->field_0x4 = pText_buffer->texts;
        if (pText_buffer->count - pText_buffer->pos_last_add < pText_buffer->field_0x10) {
            node = pText_buffer->texts;
            for (i = pText_buffer->count - pText_buffer->field_0x10; i > 0; i--) {
                node = node->next;
            }
            pText_buffer->field_0x4 = node;
            return;
        }
    }
    current = pText_buffer->field_0x4;
    index = 0;
    for (node = pText_buffer->texts; node != current; node = node->next) {
        index++;
    }
    if (pText_buffer->pos_last_add >= index + pText_buffer->field_0x10
            || pText_buffer->pos_last_add <= index) {
        if (pText_buffer->pos_last_add == index + pText_buffer->field_0x10) {
            if (pText_buffer->count - index < pText_buffer->field_0x10) {
                return;
            }
            pText_buffer->field_0x4 = current->next;
            return;
        }
        if (pText_buffer->count - pText_buffer->pos_last_add < pText_buffer->field_0x10) {
            node = pText_buffer->texts;
            for (i = pText_buffer->count - pText_buffer->field_0x10; i > 0; i--) {
                node = node->next;
            }
            pText_buffer->field_0x4 = node;
            return;
        }
        pText_buffer->field_0x4 = pText_buffer->text_last_add;
    }
}

int C2_HOOK_FASTCALL FUN_CheckJoystickHeadupButtons(tButtonJoystickInfo** pJoystick_info) {

    if (!PDKeyDown(65)) {
        return 1;
    }
    if (gAccent_poly_prims[2].t != -1) {
        gAccent_poly_prims[2].t = -1;
    } else {
        tHeadup_text* text;
        int last_index;
        size_t i;

        *pJoystick_info = PDGetCurrentJoystickData();
        if (*pJoystick_info == NULL) {
            return 0;
        }

        i = 0;
        last_index = -1;
        for (text = gJoystick_headup_buffer_0079d8a0.texts; text != NULL; text = text->next, i++) {
            if (text == gJoystick_headup_buffer_0079d8a0.text_last_add) {
                last_index = i;
            }
        }
        for (i = 0; i < (*pJoystick_info)->count_buttons; i++) {
            if ((*pJoystick_info)->buttons[i] == last_index && (*pJoystick_info)->buttons[i] != -1) {
                (*pJoystick_info)->buttons[i] = -1;
            }
        }
    }
    while (PDKeyDown(65)) {
        /* brr */
        /* FIXME: replace with udelay */
    }
    return 1;
}

// FUNCTION: CARMA2_HW 0x0045b3b0
void C2_HOOK_FASTCALL MungeJoystickHeadups(void) {
    tButtonJoystickInfo* joystick_info;
    tHeadup_text* headup_text;
    tHeadup_text* text;
    tU32 the_time;
    int last_index;
    int i;
    size_t j;
    char s[64];

    last_index = -1;
    joystick_info = NULL;
    if (gINT_0068b8e8) {
        the_time = PDGetTotalTime();
        if (PDKeyDown(65)) {
            if (gINT_00596308 != -1) {
                gINT_00596308 = -1;
            } else {
                joystick_info = PDGetCurrentJoystickData();
                if (joystick_info == NULL) {
                    goto munge;
                }
                i = 0;
                for (text = gJoystick_headup_buffer_0079d8a0.texts; text != NULL; text = text->next, i++) {
                    if (text == gJoystick_headup_buffer_0079d8a0.text_last_add) {
                        last_index = i;
                    }
                }
                for (j = 0; j < joystick_info->count_buttons; j++) {
                    if (joystick_info->buttons[j] == last_index && joystick_info->buttons[j] != -1) {
                        joystick_info->buttons[j] = -1;
                    }
                }
            }
            while (PDKeyDown(65)) {
            }
        }
        if (gINT_00596308 != -1) {
            goto munge;
        }
        if (PDKeyDown(64)) {
            if (gINT_00596308 == -1) {
                i = 0;
                for (text = gJoystick_headup_buffer_0079d8a0.texts; text != NULL; text = text->next, i++) {
                    if (text == gJoystick_headup_buffer_0079d8a0.text_last_add) {
                        last_index = i;
                    }
                }
                gINT_00596308 = last_index;
            }
            while (PDKeyDown(64)) {
            }
        }
        if (PDKeyDown(66)) {
            if (gJoystick_headup_buffer_0079d8a0.texts != NULL
                    && gJoystick_headup_buffer_0079d8a0.text_last_add != NULL
                    && gJoystick_headup_buffer_0079d8a0.text_last_add != gJoystick_headup_buffer_0079d8a0.texts) {
                gJoystick_headup_buffer_0079d8a0.pos_last_add -= 1;
                gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.texts;
                for (i = 0; i < gJoystick_headup_buffer_0079d8a0.pos_last_add; i++) {
                    gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.text_last_add->next;
                }
            }
            while (PDGetTotalTime() - the_time < 100) {
            }
        }
        if (PDKeyDown(67)) {
            if (gJoystick_headup_buffer_0079d8a0.texts != NULL
                    && gJoystick_headup_buffer_0079d8a0.text_last_add != NULL
                    && gJoystick_headup_buffer_0079d8a0.text_last_add->next != NULL) {
                gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.text_last_add->next;
                gJoystick_headup_buffer_0079d8a0.pos_last_add += 1;
            }
            while (PDGetTotalTime() - the_time < 100) {
            }
        }
        if (PDKeyDown(68)) {
            for (i = gJoystick_headup_buffer_0079d8a0.field_0x10 - 1; i > 0; i--) {
                if (gJoystick_headup_buffer_0079d8a0.text_last_add != NULL
                        && gJoystick_headup_buffer_0079d8a0.text_last_add != gJoystick_headup_buffer_0079d8a0.texts) {
                    gJoystick_headup_buffer_0079d8a0.pos_last_add -= 1;
                    gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.texts;
                    for (j = 0; j < (size_t)gJoystick_headup_buffer_0079d8a0.pos_last_add; j++) {
                        gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.text_last_add->next;
                    }
                }
            }
            while (PDKeyDown(68)) {
            }
        }
        if (PDKeyDown(69)) {
            for (i = gJoystick_headup_buffer_0079d8a0.field_0x10 - 1; i > 0; i--) {
                if (gJoystick_headup_buffer_0079d8a0.text_last_add != NULL
                        && gJoystick_headup_buffer_0079d8a0.text_last_add->next != NULL) {
                    gJoystick_headup_buffer_0079d8a0.text_last_add = gJoystick_headup_buffer_0079d8a0.text_last_add->next;
                    gJoystick_headup_buffer_0079d8a0.pos_last_add += 1;
                }
            }
            while (PDKeyDown(69)) {
            }
        }
munge:
        AdjustHeadupTextBuffer(&gJoystick_headup_buffer_0079d8a0);
        headup_text = gJoystick_headup_buffer_0079d8a0.field_0x4;
        joystick_info = PDGetCurrentJoystickData();
        if (joystick_info == NULL) {
            return;
        }
    } else if (gINT_0068b8e4) {
        return;
    }

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gRace_head_ups); i++) {

        if (gINT_0068b8e8 && headup_text != NULL) {
            int font;

            if (i == gINT_00596308) {
                font = -3;
            } else if (headup_text == gJoystick_headup_buffer_0079d8a0.text_last_add) {
                font = -1;
            } else {
                font = -2;
            }
            ChangeHeadupFont(gRace_head_ups[i], font);
            if (i == gINT_00596308) {
                sprintf(s, "%s      ???", headup_text->text);
            } else {
                sprintf(s, "%s", headup_text->text);
                if (joystick_info != NULL) {
                    for (j = 0; j < joystick_info->count_buttons; j++) {
                        if (i == joystick_info->buttons[j]) {
                            sprintf(s, "%s      #%02d", headup_text->text, (int)j);
                        }
                    }
                }
            }
            ChangeHeadupText(gRace_head_ups[i], s);
            headup_text = headup_text->next;
        } else {
            ChangeHeadupText(gRace_head_ups[i], "");
        }
    }
}

// FUNCTION: CARMA2_HW 0x0045a0f0
int C2_HOOK_FASTCALL HasCurrentJoystick(void) {

    return gJoystick_index != -1;
}

// FUNCTION: CARMA2_HW 0x0045a2e0
int C2_HOOK_FASTCALL PeekChar(FILE* pFile) {
    char c;

    c = fgetc(pFile);
    return ungetc(c, pFile);
}

// FUNCTION: CARMA2_HW 0x0045a300
int C2_HOOK_FASTCALL ReadStringUntil(FILE* pF, char* pBuffer, int pBuffer_size, char pLimit_char) {
    int i;
    char s[2];

    strcpy(pBuffer, "");
    s[1] = '\0';
    for (i = 0; (s[0] = fgetc(pF)) != pLimit_char && i < pBuffer_size && !feof(pF); i++) {
        strcat(pBuffer, s);
    }
    if (feof(pF)) {
        return 1;
    } else {
        return 0;
    }
}

// C2PORT-NONMATCHING: best 50.55% | difftest PASS 2000 trials (spec difftest/specs/0045c910_FUN_0045c910.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x0045c910
void C2_HOOK_FASTCALL FUN_0045c910(tCar_spec* pCar, const br_vector3* pPosition, br_vector3* pForce, float pMagnitude) {
    float force_length;
    float inverse_force_length;
    br_vector3 normalized_position;
    register int angle_degrees;
    int magnitude;
    float force_x;
    float force_y;
    float force_z;

    (void)pForce;
    (void)pCar;

    if (pMagnitude > gForce_feedback_upper_limit) {
        pMagnitude = gForce_feedback_upper_limit;
    }
    if (pMagnitude < Force_feedback_lower_limit) {
        pMagnitude = Force_feedback_lower_limit;
    }
    magnitude = (int)(pMagnitude * (gJoystick_collision_force_scale / gForce_feedback_upper_limit));

    force_x = pPosition->v[0];
    force_y = pPosition->v[1];
    force_z = pPosition->v[2];
    force_length = sqrt(force_x * force_x + force_y * force_y + force_z * force_z);
    if (force_length > gJoystick_collision_force_min_length) {
        inverse_force_length = gJoystick_collision_force_one / force_length;
        normalized_position.v[0] = force_x * inverse_force_length;
        normalized_position.v[1] = force_y * inverse_force_length;
        normalized_position.v[2] = force_z * inverse_force_length;
    }
    else {
        normalized_position.v[0] = gJoystick_collision_force_one_f;
        normalized_position.v[1] = gJoystick_collision_force_zero_f;
        normalized_position.v[2] = gJoystick_collision_force_zero_f;
    }

    angle_degrees = (int)(acos(-(double)normalized_position.v[2]) * gJoystick_collision_force_radians_to_degrees);
    if (angle_degrees < 45) {
        angle_degrees = 90 - angle_degrees;
    }
    if (angle_degrees > 135) {
        angle_degrees = 270 - angle_degrees;
    }
    if (normalized_position.v[0] <= 0.0f) {
        angle_degrees = 360 - angle_degrees;
    }

    SetJoystickFFBCollisionForce(magnitude, angle_degrees * 100);
}
