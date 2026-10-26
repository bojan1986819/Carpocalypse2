#include "input.h"

#include "globvars.h"
#include "grafdata.h"
#include "utility.h"

#include "platform.h"

#include "carpocalypse2_macros.h"

#include "c2_string.h"

#define NBR_ROLLING_LETTERS 500
#define ROLLING_LETTER_LOOP_RANDOM 96
#include "input.h"
#include "carpocalypse2_types.h"
#include "carpocalypse2_macros.h"
#include "platform.h"
// GLOBAL: CARMA2_HW 0x006571f8
tMouse_coord gCurrent_mouse_position = {-1, -1};

// GLOBAL: CARMA2_HW 0x00657200
int gGo_ahead_keys[3] = { 51, 52, 106 };

// GLOBAL: CARMA2_HW 0x0065720c
int gLast_key_down = -1000;

// GLOBAL: CARMA2_HW 0x0068c1d0
tU32 gLast_key_down_time;

// GLOBAL: CARMA2_HW 0x0068c1c4
extern int gEdge_trigger_mode;
// GLOBAL: CARMA2_HW 0x0068bed4
extern int gKey_poll_counter;

// GLOBAL: CARMA2_HW 0x0068c144
tU32 gLast_roll;

// GLOBAL: CARMA2_HW 0x0068c140
int gCurrent_cursor;

// GLOBAL: CARMA2_HW 0x0068be88
tRolling_letter* gRolling_letters;

// GLOBAL: CARMA2_HW 0x0068be90
int gLetter_x_coords[15];

// GLOBAL: CARMA2_HW 0x0068bed0
int gVisible_length;

// GLOBAL: CARMA2_HW 0x0068be48
int gLetter_y_coords[15];

// GLOBAL: CARMA2_HW 0x0068be84
int gThe_length;

// GLOBAL: CARMA2_HW 0x0068c150
char gThe_text[0x70];

// GLOBAL: CARMA2_HW 0x0068c1c0
int gCurrent_position;

// GLOBAL: CARMA2_HW 0x0068c1c8
tU32 gLast_poll_keys;

// GLOBAL: CARMA2_HW 0x006621e4
int gINT_006621e4 = 1;

// GLOBAL: CARMA2_HW 0x006621e8
int gINT_006621e8 = 1;

// GLOBAL: CARMA2_HW 0x006621ec
int gINT_006621ec = 1;

// GLOBAL: CARMA2_HW 0x006621f0
int gINT_006621f0 = 1;

// GLOBAL: CARMA2_HW 0x0068be44
int gJoy1_x;

// GLOBAL: CARMA2_HW 0x0068c13c
int gJoy1_y;

// GLOBAL: CARMA2_HW 0x0068be40
int gJoy2_x;

// GLOBAL: CARMA2_HW 0x0068becc
int gJoy2_y;

// GLOBAL: CARMA2_HW 0x0068bed8
int gModifiers_down;

extern tU32 gKeys_pressed;

// GLOBAL: CARMA2_HW 0x0074b5c0
tJoy_array gJoy_array;

// GLOBAL: CARMA2_HW 0x0068bee0
tKey_array gKey_array;

// GLOBAL: CARMA2_HW 0x0068bed4
int gKey_poll_counter;

// GLOBAL: CARMA2_HW 0x0074b5e0
int gKey_mapping[77];

// GLOBAL: CARMA2_HW 0x0068c1c4
int gEdge_trigger_mode;


int C2_HOOK_FASTCALL AnyModifiersDown(void) {
    int i;

    for (i = 0; i < 11; i++) {
        if (gKey_array[i] != 0) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x00482590
tKey_down_result C2_HOOK_FASTCALL EdgeTriggeryKey(int pKey_index, int pReset) {
    tU32 now;

    if (pReset) {
        gLast_key_down = -1;
        gLast_key_down_time = 0;
        return tKey_down_no;
    }
    CheckKeysForMouldiness();
    if (!gEdge_trigger_mode) {
        return gKey_array[pKey_index];
    }
    if (gKey_array[pKey_index] == 0) {
        if (pKey_index == gLast_key_down) {
            gLast_key_down_time = 0;
            gLast_key_down = -1000;
        }
        return tKey_down_no;
    }
    now = PDGetTotalTime();
    if (pKey_index == gLast_key_down) {
        if (gModifiers_down == AnyModifiersDown()) {
            if (now - gLast_key_down_time < 300) {
                return tKey_down_still;
            } else {
                gLast_key_down_time = now;
                return tKey_down_repeat;
            }
        } else {
            gLast_key_down_time = 0;
            gLast_key_down = -gLast_key_down;
            return tKey_down_no;
        }
    }
    if (pKey_index == -gLast_key_down) {
        return tKey_down_no;
    }
    if (pKey_index < 11) {
        gLast_key_down_time = 0;
        gLast_key_down = -1000;
        return tKey_down_yes;
    }
    gLast_key_down = pKey_index;
    gLast_key_down_time = now;
    gModifiers_down = AnyModifiersDown();
    return tKey_down_yes;
}

// FUNCTION: CARMA2_HW 0x004833a0
int C2_HOOK_FASTCALL KeyIsDownNoMouldiness(int pKey_index) {

    return gKey_array[gKey_mapping[pKey_index]];
}

// C2PORT-NONMATCHING: best 43.37% | difftest PASS 2000 trials (spec 00483cf0_AddRollingLetter.json and branch specs) | reason equivalent slot allocation and letter output with different register scheduling
// FUNCTION: CARMA2_HW 0x00483cf0
int C2_HOOK_FASTCALL AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type) {
    tRolling_letter* let;
    int i;

    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        let = &gRolling_letters[i];
        if (let->number_of_letters < 0) {
            break;
        }
    }
    if (i == NBR_ROLLING_LETTERS) {
        return -1;
    }
    let->x_coord = pX;
    let->y_coord = pY;
    let->rolling_type = rolling_type;
    switch (rolling_type) {
    case eRT_looping_random:
        let->number_of_letters = 9;
        break;
    case eRT_looping_single:
        let->number_of_letters = 2;
        break;
    default:
        let->number_of_letters = IRandomBetween(3, 9);
        break;
    }

    let->current_offset = (float)(let->number_of_letters * gCurrent_graf_data->save_slot_letter_height);
    for (i = (rolling_type != eRT_looping_random ? 1 : 0); i < let->number_of_letters; i++) {
        if (rolling_type == eRT_numeric) {
            /* The (tU8) cast makes sure extended ASCII is positive. */
            let->letters[i] = (tS8)pChar;
        } else {
            let->letters[i] = IRandomBetween('A', 'Z' + 1);
        }
    }
    if (rolling_type != eRT_looping_random) {
        /* The (tU8) cast makes sure extended ASCII is positive. */
        let->letters[0] = (tS8)pChar;
    }

    return rolling_type == eRT_looping_random ? -1 : 0;
}

// FUNCTION: CARMA2_HW 0x00483dd0
void C2_HOOK_FASTCALL AddRollingString(char* pText, int pX, int pY, tRolling_type pRolling_type) {
    unsigned int i;

    for (i = 0; i < strlen(pText); i++) {
        AddRollingLetter(pText[i], i * gCurrent_graf_data->rolling_letter_x_pitch + pX, pY, pRolling_type);
    }
}

// FUNCTION: CARMA2_HW 0x00483e20
void C2_HOOK_FASTCALL AddRollingNumber(tU32 pNumber, int pWidth, int pX, int pY) {
    char the_string[32];
    unsigned int i;

    sprintf(the_string, "%0.*d", pWidth, pNumber);
    for (i = 0; i < strlen(the_string); i++) {
        AddRollingLetter(the_string[i], i * gCurrent_graf_data->rolling_letter_x_pitch + pX, pY, eRT_numeric);
    }
}

// FUNCTION: CARMA2_HW 0x00483e90
void C2_HOOK_FASTCALL InputNoOp(void) {
}

// FUNCTION: CARMA2_HW 0x00484000
int C2_HOOK_FASTCALL ChangeCharTo(int pSlot_index, int pChar_index, char pNew_char) {
    int x_coord;
    int y_coord;
    int i;
    tRolling_letter* let;
    tRolling_type new_type;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, save_slot_letter_height, 0x24);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, rolling_letter_x_pitch, 0x10);

    if (pChar_index >= gVisible_length || pChar_index < 0) {
        return -1;
    }
    y_coord = gLetter_y_coords[pSlot_index];
    x_coord = gLetter_x_coords[pSlot_index] + pChar_index * gCurrent_graf_data->rolling_letter_x_pitch;

    if (pNew_char == ROLLING_LETTER_LOOP_RANDOM) {
        new_type = eRT_looping_random;
    } else if (pNew_char >= '0' && pNew_char <= '9') {
        new_type = eRT_numeric;
    } else {
        new_type = eRT_alpha;
    }

    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        let = &gRolling_letters[i];
        if (let->number_of_letters >= 0 && x_coord == let->x_coord && y_coord == let->y_coord) {
            break;
        }
    }
    if (i >= NBR_ROLLING_LETTERS) {
        return AddRollingLetter(pNew_char, x_coord, y_coord, new_type);
    }
    if (pNew_char != ROLLING_LETTER_LOOP_RANDOM) {
        /* The (tU8) cast makes sure extended ASCII is positive. */
        let->letters[0] = (tU8)pNew_char;
    }
    if (pNew_char == ' ') {
        let->letters[0] = ' ';
    }
    let->rolling_type = new_type;
    let->current_offset = (float)(gCurrent_graf_data->save_slot_letter_height * let->number_of_letters);
    return i;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00482770
void C2_HOOK_FASTCALL SetJoystickArrays(int* pKeys, int pMark) {
    int i;

    for (i = 0; i < 44; i++) {
        pKeys[107 + i] = 0;
    }
    PDReadJoysticks();
    for (i = 0; i < 8; i++) {
        gJoy_array[i] = -1;
    }
    for (i = 0; i < 36; i++) {
        pKeys[107 + i] = 0;
    }
    if (gINT_006621e4 || gINT_006621e8) {
        if (PDGetJoy1Button1()) {
            pKeys[107] = pMark;
        }
        if (PDGetJoy1Button2()) {
            pKeys[108] = pMark;
        }
        if (PDGetJoy1Button3()) {
            pKeys[109] = pMark;
        }
        if (PDGetJoy1Button4()) {
            pKeys[110] = pMark;
        }
    }
    if (gINT_006621ec || gINT_006621f0) {
        if (PDGetJoy2Button1()) {
            pKeys[139] = pMark;
        }
        if (PDGetJoy2Button2()) {
            pKeys[140] = pMark;
        }
        if (PDGetJoy2Button3()) {
            pKeys[141] = pMark;
        }
        if (PDGetJoy2Button4()) {
            pKeys[142] = pMark;
        }
    }
    if (gINT_006621e4) {
        int new_joy1_x = PDGetJoy1X();
        if (new_joy1_x != -1) {
            if (new_joy1_x < 0x8000) {
                if (new_joy1_x < 0x4000 && gJoy1_x >= 0x4000) {
                    pKeys[143] = pMark;
                }
                gJoy_array[0] = 2 * (0x8000 - new_joy1_x);
            } else {
                if (new_joy1_x > 0xc000 && gJoy1_x <= 0xc000) {
                    pKeys[144] = pMark;
                }
                gJoy_array[1] = 2 * (new_joy1_x - 0x8000);
            }
        }
        gJoy1_x = new_joy1_x;
    }
    if (gINT_006621e8) {
        int new_joy1_y = PDGetJoy1Y();
        if (new_joy1_y != -1) {
            if (new_joy1_y < 0x8000) {
                if (new_joy1_y < 0x4000 && gJoy1_y >= 0x4000) {
                    pKeys[145] = pMark;
                }
                gJoy_array[2] = 2 * (0x8000 - new_joy1_y);
            }
            else {
                if (new_joy1_y > 0xc000 && gJoy1_y <= 0xc000) {
                    pKeys[146] = pMark;
                }
                gJoy_array[3] = 2 * (new_joy1_y - 0x8000);
            }
        }
        gJoy1_y = new_joy1_y;
    }
    if (gINT_006621ec) {
        int new_joy2_x = PDGetJoy2X();
        if (new_joy2_x != -1) {
            if (new_joy2_x < 0x8000) {
                if (new_joy2_x < 0x4000 && gJoy2_x >= 0x4000) {
                    pKeys[147] = pMark;
                }
                gJoy_array[4] = 2 * (0x8000 - new_joy2_x);
            } else {
                if (new_joy2_x > 0xc000 && gJoy2_x <= 0xc000) {
                    pKeys[148] = pMark;
                }
                gJoy_array[5] = 2 * (new_joy2_x - 0x8000);
            }
        }
        gJoy2_x = new_joy2_x;
    }
    if (gINT_006621f0) {
        int new_joy2_y = PDGetJoy2Y();
        if (new_joy2_y != -1) {
            if (new_joy2_y < 0x8000) {
                if (new_joy2_y < 0x4000 && gJoy2_y >= 0x4000) {
                    pKeys[149] = pMark;
                }
                gJoy_array[6] = 2 * (0x8000 - new_joy2_y);
            } else {
                if (new_joy2_y > gJoystick_deadzone + 0x8000) {
                    if (new_joy2_y > 0xc000 && gJoy2_y <= 0xc000) {
                        pKeys[150] = pMark;
                    }
                    gJoy_array[7] = 2 * (new_joy2_y - 0x8000);
                }
            }
        }
        gJoy2_y = new_joy2_y;
    }
}
#pragma auto_inline(on)
// SetJoystickArrays

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00481eb0
void C2_HOOK_FASTCALL PollKeys(void) {
    int i;
    int mark;

    gKey_poll_counter += 1;
    PDSetKeyArray(gKey_array, gKey_poll_counter);
    mark = gKey_poll_counter;

    for (i = 0; i < 44; i++) {
        gKey_array[107 + i] = 0;
    }
    PDReadJoysticks();
    for (i = 0; i < 8; i++) {
        gJoy_array[i] = -1;
    }
    for (i = 0; i < 36; i++) {
        gKey_array[107 + i] = 0;
    }
    if (gINT_006621e4 || gINT_006621e8) {
        if (PDGetJoy1Button1()) {
            gKey_array[107] = mark;
        }
        if (PDGetJoy1Button2()) {
            gKey_array[108] = mark;
        }
        if (PDGetJoy1Button3()) {
            gKey_array[109] = mark;
        }
        if (PDGetJoy1Button4()) {
            gKey_array[110] = mark;
        }
    }
    if (gINT_006621ec || gINT_006621f0) {
        if (PDGetJoy2Button1()) {
            gKey_array[139] = mark;
        }
        if (PDGetJoy2Button2()) {
            gKey_array[140] = mark;
        }
        if (PDGetJoy2Button3()) {
            gKey_array[141] = mark;
        }
        if (PDGetJoy2Button4()) {
            gKey_array[142] = mark;
        }
    }
    if (gINT_006621e4) {
        int new_joy1_x = PDGetJoy1X();
        if (new_joy1_x != -1) {
            if (new_joy1_x < 0x8000) {
                if (new_joy1_x < 0x4000 && gJoy1_x >= 0x4000) {
                    gKey_array[143] = mark;
                }
                gJoy_array[0] = 2 * (0x8000 - new_joy1_x);
            } else {
                if (new_joy1_x > 0xc000 && gJoy1_x <= 0xc000) {
                    gKey_array[144] = mark;
                }
                gJoy_array[1] = 2 * (new_joy1_x - 0x8000);
            }
        }
        gJoy1_x = new_joy1_x;
    }
    if (gINT_006621e8) {
        int new_joy1_y = PDGetJoy1Y();
        if (new_joy1_y != -1) {
            if (new_joy1_y < 0x8000) {
                if (new_joy1_y < 0x4000 && gJoy1_y >= 0x4000) {
                    gKey_array[145] = mark;
                }
                gJoy_array[2] = 2 * (0x8000 - new_joy1_y);
            }
            else {
                if (new_joy1_y > 0xc000 && gJoy1_y <= 0xc000) {
                    gKey_array[146] = mark;
                }
                gJoy_array[3] = 2 * (new_joy1_y - 0x8000);
            }
        }
        gJoy1_y = new_joy1_y;
    }
    if (gINT_006621ec) {
        int new_joy2_x = PDGetJoy2X();
        if (new_joy2_x != -1) {
            if (new_joy2_x < 0x8000) {
                if (new_joy2_x < 0x4000 && gJoy2_x >= 0x4000) {
                    gKey_array[147] = mark;
                }
                gJoy_array[4] = 2 * (0x8000 - new_joy2_x);
            } else {
                if (new_joy2_x > 0xc000 && gJoy2_x <= 0xc000) {
                    gKey_array[148] = mark;
                }
                gJoy_array[5] = 2 * (new_joy2_x - 0x8000);
            }
        }
        gJoy2_x = new_joy2_x;
    }
    if (gINT_006621f0) {
        int new_joy2_y = PDGetJoy2Y();
        if (new_joy2_y != -1) {
            if (new_joy2_y < 0x8000) {
                if (new_joy2_y < 0x4000 && gJoy2_y >= 0x4000) {
                    gKey_array[149] = mark;
                }
                gJoy_array[6] = 2 * (0x8000 - new_joy2_y);
            } else {
                if (new_joy2_y > gJoystick_deadzone + 0x8000) {
                    if (new_joy2_y > 0xc000 && gJoy2_y <= 0xc000) {
                        gKey_array[150] = mark;
                    }
                    gJoy_array[7] = 2 * (new_joy2_y - 0x8000);
                }
            }
        }
        gJoy2_y = new_joy2_y;
    }

    PDSetKeysFromJoystick(gKey_array);
    gLast_poll_keys = PDGetTotalTime();
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00482160
void C2_HOOK_FASTCALL CyclePollKeys(void) {
    int i;

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gKey_array); i++) {
        if (gKey_array[i] > gKey_poll_counter) {
            gKey_array[i] = 0;
            if (i > 143) {
                gJoy_array[i - 143] = -1;
            }
        }
    }
    gKey_poll_counter = 0;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004821a0
void C2_HOOK_FASTCALL ResetPollKeys(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gKey_array) != 151);
    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gJoy_array) != 8);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gKey_array); i++) {
        gKey_array[i] = 0;
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gJoy_array); i++) {
        gJoy_array[i] = -1;
    }
}

// FUNCTION: CARMA2_HW 0x004821c0
void C2_HOOK_FASTCALL CheckKeysForMouldiness(void) {

    if (PDGetTotalTime() - gLast_poll_keys > 500) {
        ResetPollKeys();
        CyclePollKeys();
        PollKeys();
    }
}

// FUNCTION: CARMA2_HW 0x004824c0
int C2_HOOK_FASTCALL EitherMouseButtonDown(void) {
    int x_coord;
    int y_coord;
    int click_x_coord;
    int click_y_coord;
    int left_button_down;
    int right_button_down;
    int previous_x;
    int previous_y;

    previous_x = gCurrent_mouse_position.x;
    previous_y = gCurrent_mouse_position.y;
    GetMousePosition(&x_coord, &y_coord);
    if (previous_x == x_coord && previous_y == y_coord && PDGetMouseClickPosition(&click_x_coord, &click_y_coord) != 0
            && x_coord == click_x_coord && y_coord == click_y_coord) {
        return 2;
    }
    PDMouseButtons(&left_button_down, &right_button_down);
    return (left_button_down || right_button_down) ? 1 : 0;
}

// AnyModifiersDown

// EdgeTriggeryKey

// FUNCTION: CARMA2_HW 0x00482550
int C2_HOOK_FASTCALL PDKeyDown(int pKey_index) {
    tKey_down_result result;

    result = EdgeTriggeryKey(pKey_index, 0);
    if (!gEdge_trigger_mode || pKey_index <= 10) {
        return result != tKey_down_no;
    }
    return result == tKey_down_yes || result == tKey_down_repeat;
}

// PDKeyDown3

// FUNCTION: CARMA2_HW 0x004829f0
int C2_HOOK_FASTCALL FUN_004829f0(int pKey_index) {

    return gKey_array[pKey_index];
}

// FUNCTION: CARMA2_HW 0x00482a00
int C2_HOOK_FASTCALL PDAnyKeyDown(void) {
    int i;
    int mark;
    tKey_down_result key_down_result;

    if (PDGetTotalTime() - gLast_poll_keys > 500) {
            ResetPollKeys();
            for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gKey_array); i++) {
                if (gKey_array[i] > gKey_poll_counter) {
                    gKey_array[i] = 0;
                    if (i > 143) {
                        gJoy_array[i - 143] = -1;
                    }
                }
            }
            gKey_poll_counter = 0;
        gKey_poll_counter = 1;
        PDSetKeyArray(gKey_array, gKey_poll_counter);
        mark = gKey_poll_counter;
    
        for (i = 0; i < 44; i++) {
            gKey_array[107 + i] = 0;
        }
        PDReadJoysticks();
        for (i = 0; i < 8; i++) {
            gJoy_array[i] = -1;
        }
        for (i = 0; i < 36; i++) {
            gKey_array[107 + i] = 0;
        }
        if (gINT_006621e4 || gINT_006621e8) {
            if (PDGetJoy1Button1()) {
                gKey_array[107] = mark;
            }
            if (PDGetJoy1Button2()) {
                gKey_array[108] = mark;
            }
            if (PDGetJoy1Button3()) {
                gKey_array[109] = mark;
            }
            if (PDGetJoy1Button4()) {
                gKey_array[110] = mark;
            }
        }
        if (gINT_006621ec || gINT_006621f0) {
            if (PDGetJoy2Button1()) {
                gKey_array[139] = mark;
            }
            if (PDGetJoy2Button2()) {
                gKey_array[140] = mark;
            }
            if (PDGetJoy2Button3()) {
                gKey_array[141] = mark;
            }
            if (PDGetJoy2Button4()) {
                gKey_array[142] = mark;
            }
        }
        if (gINT_006621e4) {
            int new_joy1_x = PDGetJoy1X();
            if (new_joy1_x != -1) {
                if (new_joy1_x < 0x8000) {
                    if (new_joy1_x < 0x4000 && gJoy1_x >= 0x4000) {
                        gKey_array[143] = mark;
                    }
                    gJoy_array[0] = 2 * (0x8000 - new_joy1_x);
                } else {
                    if (new_joy1_x > 0xc000 && gJoy1_x <= 0xc000) {
                        gKey_array[144] = mark;
                    }
                    gJoy_array[1] = 2 * (new_joy1_x - 0x8000);
                }
            }
            gJoy1_x = new_joy1_x;
        }
        if (gINT_006621e8) {
            int new_joy1_y = PDGetJoy1Y();
            if (new_joy1_y != -1) {
                if (new_joy1_y < 0x8000) {
                    if (new_joy1_y < 0x4000 && gJoy1_y >= 0x4000) {
                        gKey_array[145] = mark;
                    }
                    gJoy_array[2] = 2 * (0x8000 - new_joy1_y);
                }
                else {
                    if (new_joy1_y > 0xc000 && gJoy1_y <= 0xc000) {
                        gKey_array[146] = mark;
                    }
                    gJoy_array[3] = 2 * (new_joy1_y - 0x8000);
                }
            }
            gJoy1_y = new_joy1_y;
        }
        if (gINT_006621ec) {
            int new_joy2_x = PDGetJoy2X();
            if (new_joy2_x != -1) {
                if (new_joy2_x < 0x8000) {
                    if (new_joy2_x < 0x4000 && gJoy2_x >= 0x4000) {
                        gKey_array[147] = mark;
                    }
                    gJoy_array[4] = 2 * (0x8000 - new_joy2_x);
                } else {
                    if (new_joy2_x > 0xc000 && gJoy2_x <= 0xc000) {
                        gKey_array[148] = mark;
                    }
                    gJoy_array[5] = 2 * (new_joy2_x - 0x8000);
                }
            }
            gJoy2_x = new_joy2_x;
        }
        if (gINT_006621f0) {
            int new_joy2_y = PDGetJoy2Y();
            if (new_joy2_y != -1) {
                if (new_joy2_y < 0x8000) {
                    if (new_joy2_y < 0x4000 && gJoy2_y >= 0x4000) {
                        gKey_array[149] = mark;
                    }
                    gJoy_array[6] = 2 * (0x8000 - new_joy2_y);
                } else {
                    if (new_joy2_y > gJoystick_deadzone + 0x8000) {
                        if (new_joy2_y > 0xc000 && gJoy2_y <= 0xc000) {
                            gKey_array[150] = mark;
                        }
                        gJoy_array[7] = 2 * (new_joy2_y - 0x8000);
                    }
                }
            }
            gJoy2_y = new_joy2_y;
        }
    
        PDSetKeysFromJoystick(gKey_array);
        gLast_poll_keys = PDGetTotalTime();
    
        }
    for (i = 0x8e; i >= 0; i--) {
        if (gKey_array[i] != 0) {
            if (i == 4 || !gEdge_trigger_mode) {
                return i;
            }
            key_down_result = EdgeTriggeryKey(i, 0);
            switch (key_down_result) {
            case tKey_down_no:
            case tKey_down_still:
                break;
            case tKey_down_yes:
            case tKey_down_repeat:
                return i;
            }
        }
    }
    return -1;
}

// FUNCTION: CARMA2_HW 0x00482d70
int C2_HOOK_FASTCALL AnyKeyDown(void) {
    int i;
    int key;
    int* keys;
    int x;
    int y;
    int click_x;
    int click_y;
    int left_button;
    int right_button;
    int previous_x;
    int previous_y;

    if (500 < (tU32)(PDGetTotalTime() - gLast_poll_keys)) {
        keys = gKey_array;
        for (i = 0x97; i != 0; i--) {
            *keys++ = 0;
        }
        keys = gJoy_array;
        for (i = 8; i != 0; i--) {
            *keys++ = -1;
        }
        for (i = 0; i < 0x25c; i += 4) {
            if (*(int*)((char*)gKey_array + i) > gKey_poll_counter) {
                *(int*)((char*)gKey_array + i) = 0;
                if (i > 0x23c) {
                    *(int*)((char*)gJoy_array + i - 0x240) = -1;
                }
            }
        }
        gKey_poll_counter = 1;
        PDSetKeyArray(gKey_array, gKey_poll_counter);
        SetJoystickArrays(gKey_array, gKey_poll_counter);
        PDSetKeysFromJoystick(gKey_array);
        gLast_poll_keys = PDGetTotalTime();
    }
    key = 0x8e;
    keys = &gKey_array[key];
    do {
        if (*keys != 0 && keys != &gKey_array[4]) {
            if (!gEdge_trigger_mode) {
                goto key_found;
            }
            i = EdgeTriggeryKey(key, 0);
            if (i == 1 || i == 3) {
                goto key_found;
            }
        }
        keys--;
        key--;
    } while ((tS32)keys >= (tS32)gKey_array);
    if (gEdge_trigger_mode) {
        EdgeTriggeryKey(0, 1);
    }
    key = -1;
key_found:
    if (key == -1 || key == 4) {
        previous_x = gCurrent_mouse_position.x;
        previous_y = gCurrent_mouse_position.y;
        GetMousePosition(&x, &y);
        if (previous_x == x && previous_y == y) {
            if (PDGetMouseClickPosition(&click_x, &click_y) != 0 && x == click_x && y == click_y) {
                return 1;
            }
        }
        PDMouseButtons(&left_button, &right_button);
        if (left_button == 0 && right_button == 0) {
            return 0;
        }
    }
    return 1;
}

// GLOBAL: CARMA2_HW 0x0068c1d4
tU32 gKev_sum = 0;
// GLOBAL: CARMA2_HW 0x0068c1d8
tU32 gKev_code = 0;
// GLOBAL: CARMA2_HW 0x0068c1dc
tU32 gKev_code2 = 0;
// GLOBAL: CARMA2_HW 0x00657210
int gKev_last_key = -1;
// GLOBAL: CARMA2_HW 0x00657214
int gKev_last_single_key = -1;
// GLOBAL: CARMA2_HW 0x0068be8c
tU32 gKev_last_time = 0;
// GLOBAL: CARMA2_HW 0x0068c1e0
tU32 gKev_return_val[2];

// C2PORT-NONMATCHING: best 53.66% | difftest PASS 2000 trials (spec difftest/specs/00482f10_KevKeyService.json) | reason codegen of the checksum block (register/stack order), logic checked
// FUNCTION: CARMA2_HW 0x00482f10
tU32* C2_HOOK_FASTCALL KevKeyService(void) {
    tU32 keys;

    gKev_return_val[0] = 0;
    gKev_return_val[1] = 0;
    keys = gKeys_pressed;

    if (keys < 0x6B) {
        gKev_last_single_key = gKeys_pressed;
    } else {
        if (keys > 0x6b00) {
            gKev_sum = 0;
            gKev_code = 0;
            return gKev_return_val;
        }
        if ((keys & 0xff) == gKev_last_single_key) {
            keys = keys >> 8;
        } else if (keys >> 8 == gKev_last_single_key) {
            keys = keys & 0xff;
        } else {
            gKev_sum = 0;
            gKev_code = 0;
            return gKev_return_val;
        }
    }

    if (keys != 0 && keys != gKev_last_key) {
        gKev_sum += keys;
        gKev_code += keys << 11;
        gKev_code = (gKev_code >> 17) + (gKev_code << 4);
        gKev_code2 = (gKev_code2 >> 29) + keys * keys + (gKev_code2 << 3);
        gKev_last_time = PDGetTotalTime();
    } else if ((tU32)PDGetTotalTime() > gKev_last_time + 1000) {
        gKev_return_val[0] = (gKev_code >> 11) + (gKev_sum << 21);
        gKev_return_val[1] = gKev_code2;
        // printf("final value: code=%lx, code2=%lx\n", return_val[0], return_val[1]);
        gKev_code = 0;
        gKev_code2 = 0;
        gKev_sum = 0;
    }
    gKev_last_key = keys;
    return gKev_return_val;
}

// FUNCTION: CARMA2_HW 0x00483040
int C2_HOOK_FASTCALL KeyIsDown(int pKey_index) {
    int i;
    int mark;

    if (PDGetTotalTime() - gLast_poll_keys > 500) {
        ResetPollKeys();
        for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gKey_array); i++) {
            if (gKey_array[i] > gKey_poll_counter) {
                gKey_array[i] = 0;
                if (i > 143) {
                    gJoy_array[i - 143] = -1;
                }
            }
        }
        gKey_poll_counter = 0;
    gKey_poll_counter = 1;
    PDSetKeyArray(gKey_array, gKey_poll_counter);
    mark = gKey_poll_counter;

    for (i = 0; i < 44; i++) {
        gKey_array[107 + i] = 0;
    }
    PDReadJoysticks();
    for (i = 0; i < 8; i++) {
        gJoy_array[i] = -1;
    }
    for (i = 0; i < 36; i++) {
        gKey_array[107 + i] = 0;
    }
    if (gINT_006621e4 || gINT_006621e8) {
        if (PDGetJoy1Button1()) {
            gKey_array[107] = mark;
        }
        if (PDGetJoy1Button2()) {
            gKey_array[108] = mark;
        }
        if (PDGetJoy1Button3()) {
            gKey_array[109] = mark;
        }
        if (PDGetJoy1Button4()) {
            gKey_array[110] = mark;
        }
    }
    if (gINT_006621ec || gINT_006621f0) {
        if (PDGetJoy2Button1()) {
            gKey_array[139] = mark;
        }
        if (PDGetJoy2Button2()) {
            gKey_array[140] = mark;
        }
        if (PDGetJoy2Button3()) {
            gKey_array[141] = mark;
        }
        if (PDGetJoy2Button4()) {
            gKey_array[142] = mark;
        }
    }
    if (gINT_006621e4) {
        int new_joy1_x = PDGetJoy1X();
        if (new_joy1_x != -1) {
            if (new_joy1_x < 0x8000) {
                if (new_joy1_x < 0x4000 && gJoy1_x >= 0x4000) {
                    gKey_array[143] = mark;
                }
                gJoy_array[0] = 2 * (0x8000 - new_joy1_x);
            } else {
                if (new_joy1_x > 0xc000 && gJoy1_x <= 0xc000) {
                    gKey_array[144] = mark;
                }
                gJoy_array[1] = 2 * (new_joy1_x - 0x8000);
            }
        }
        gJoy1_x = new_joy1_x;
    }
    if (gINT_006621e8) {
        int new_joy1_y = PDGetJoy1Y();
        if (new_joy1_y != -1) {
            if (new_joy1_y < 0x8000) {
                if (new_joy1_y < 0x4000 && gJoy1_y >= 0x4000) {
                    gKey_array[145] = mark;
                }
                gJoy_array[2] = 2 * (0x8000 - new_joy1_y);
            }
            else {
                if (new_joy1_y > 0xc000 && gJoy1_y <= 0xc000) {
                    gKey_array[146] = mark;
                }
                gJoy_array[3] = 2 * (new_joy1_y - 0x8000);
            }
        }
        gJoy1_y = new_joy1_y;
    }
    if (gINT_006621ec) {
        int new_joy2_x = PDGetJoy2X();
        if (new_joy2_x != -1) {
            if (new_joy2_x < 0x8000) {
                if (new_joy2_x < 0x4000 && gJoy2_x >= 0x4000) {
                    gKey_array[147] = mark;
                }
                gJoy_array[4] = 2 * (0x8000 - new_joy2_x);
            } else {
                if (new_joy2_x > 0xc000 && gJoy2_x <= 0xc000) {
                    gKey_array[148] = mark;
                }
                gJoy_array[5] = 2 * (new_joy2_x - 0x8000);
            }
        }
        gJoy2_x = new_joy2_x;
    }
    if (gINT_006621f0) {
        int new_joy2_y = PDGetJoy2Y();
        if (new_joy2_y != -1) {
            if (new_joy2_y < 0x8000) {
                if (new_joy2_y < 0x4000 && gJoy2_y >= 0x4000) {
                    gKey_array[149] = mark;
                }
                gJoy_array[6] = 2 * (0x8000 - new_joy2_y);
            } else {
                if (new_joy2_y > gJoystick_deadzone + 0x8000) {
                    if (new_joy2_y > 0xc000 && gJoy2_y <= 0xc000) {
                        gKey_array[150] = mark;
                    }
                    gJoy_array[7] = 2 * (new_joy2_y - 0x8000);
                }
            }
        }
        gJoy2_y = new_joy2_y;
    }

    PDSetKeysFromJoystick(gKey_array);
    gLast_poll_keys = PDGetTotalTime();

    }
    if (pKey_index == -2) {
        return 1;
    } else if (pKey_index == -1) {
        for (i = 0; i < CARPOCALYPSE2_ASIZE(gGo_ahead_keys); i++) {
            if (gKey_array[gGo_ahead_keys[i]]) {
                return 1;
            }
        }
        return 0;
    } else {
        return gKey_array[gKey_mapping[pKey_index]];
    }
}

// KeyIsDownNoMouldiness

// C2PORT-NONMATCHING: best 70.47% | difftest PASS 2000 trials (spec difftest/specs/004833b0_WaitForNoKeys.json) | reason input polling and mouse branch layout differ under VC5
// FUNCTION: CARMA2_HW 0x004833b0
void C2_HOOK_FASTCALL WaitForNoKeys(void) {
    int key;
    int now;
    int i;
    int result;
    int mouse[12];
    int mouse_x;
    int mouse_y;
    int active;
    int minus_one;
    int zero;

    minus_one = -1;
    zero = 0;
    for (;;) {
        CheckQuit();
        now = PDGetTotalTime();
        if (500 < (tU32)(now - gLast_poll_keys)) {
            for (i = 0; i < 0x97; i++) {
                gKey_array[i] = zero;
            }
            for (i = 0; i < 8; i++) {
                gJoy_array[i] = minus_one;
            }
            for (i = 0; i < 0x25c; i += 4) {
                if (*(int*)((char*)gKey_array + i) > gKey_poll_counter) {
                    *(int*)((char*)gKey_array + i) = zero;
                    if (i > 0x23c) {
                        gJoy_array[i / 4 - 143] = minus_one;
                    }
                }
            }
            gKey_poll_counter = 1;
            PDSetKeyArray(gKey_array, gKey_poll_counter);
            SetJoystickArrays(gKey_array, gKey_poll_counter);
            PDSetKeysFromJoystick(gKey_array);
            gLast_poll_keys = PDGetTotalTime();
        }

        key = 0x8e;
        {
            int *p_key = &gKey_array[0x8e];
            do {
                if (*p_key != 0 && p_key != &gKey_array[4]) {
                    if (gEdge_trigger_mode == 0) {
                        goto key_found;
                    }
                    result = EdgeTriggeryKey(key, 0);
                    switch (result) {
                    case 0:
                    case 2:
                        goto key_scan_next;
                    case 1:
                    case 3:
                        goto key_found;
                    }
                }
key_scan_next:
                p_key--;
                key--;
            } while ((tS32)p_key >= (tS32)gKey_array);
            if (gEdge_trigger_mode != 0) {
                EdgeTriggeryKey(0, 1);
            }
            key = minus_one;
key_found:
            ;
        }

        if ((key == -1 || key == 4) &&
                ((mouse_x = gCurrent_mouse_position.x,
                  mouse_y = gCurrent_mouse_position.y,
                  GetMousePosition(&mouse[0], &mouse[1]),
                  mouse_x != mouse[0] ||
                          (mouse_y != mouse[1] ||
                                  ((result = PDGetMouseClickPosition(&mouse[4], &mouse[5])) == 0 ||
                                          (mouse[0] != mouse[4] || mouse[1] != mouse[5]))))) &&
                (PDMouseButtons(&mouse[6], &mouse[7]), mouse[6] == 0 && mouse[7] == 0)) {
            active = 0;
        } else {
            active = 1;
        }
        if (!active) {
            mouse_x = gCurrent_mouse_position.x;
            mouse_y = gCurrent_mouse_position.y;
            GetMousePosition(&mouse[2], &mouse[3]);
            if (mouse_x == mouse[2] && mouse_y == mouse[3] &&
                    (result = PDGetMouseClickPosition(&mouse[8], &mouse[9])) != 0 &&
                    mouse[2] == mouse[8] && mouse[3] == mouse[9]) {
                result = 2;
            } else {
                PDMouseButtons(&mouse[10], &mouse[11]);
                result = mouse[10] == 0 && mouse[11] == 0 ? 0 : 1;
            }
            if (result == 0) {
                CheckQuit();
                return;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004835d0
void C2_HOOK_FASTCALL FUN_004835d0(void) {
    int bVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int uVar5;
    int* piVar6;
    int* puVar7;
    /* Shared stack slots in binary order, from local_30 through local_4. */
    int localVars[12];

    for (;;) {
        CheckQuit();
        iVar4 = PDGetTotalTime();
        if (500 < (tU32)(iVar4 - gLast_poll_keys)) {
            puVar7 = gKey_array;
            for (iVar4 = 0x97; iVar4 != 0; iVar4--) {
                *puVar7++ = 0;
            }
            puVar7 = gJoy_array;
            for (iVar4 = 8; iVar4 != 0; iVar4--) {
                *puVar7++ = -1;
            }
            CyclePollKeys();
            gKey_poll_counter++;
            PDSetKeyArray(gKey_array, gKey_poll_counter);
            SetJoystickArrays(gKey_array, gKey_poll_counter);
            PDSetKeysFromJoystick(gKey_array);
            gLast_poll_keys = PDGetTotalTime();
        }
        iVar4 = 0x8e;
        piVar6 = &gKey_array[0x8e];
        do {
            if (*piVar6 != 0 && piVar6 != &gKey_array[4]) {
                if (gEdge_trigger_mode == 0) {
                    goto key_found_first;
                }
                uVar5 = EdgeTriggeryKey(iVar4, 0);
                switch (uVar5) {
                case 0:
                case 2:
                    goto key_scan_next_first;
                case 1:
                case 3:
                    goto key_found_first;
                }
            }
key_scan_next_first:
            piVar6--;
            iVar4--;
        } while ((tS32)piVar6 >= (tS32)gKey_array);
        if (gEdge_trigger_mode != 0) {
            EdgeTriggeryKey(0, 1);
        }
        iVar4 = -1;
key_found_first:
        if (iVar4 != -1 && iVar4 != 4) {
            break;
        }
        iVar2 = gCurrent_mouse_position.x;
        iVar3 = gCurrent_mouse_position.y;
        GetMousePosition(&localVars[0], &localVars[1]);
        if (iVar2 == localVars[0] && iVar3 == localVars[1]) {
            iVar4 = PDGetMouseClickPosition(&localVars[4], &localVars[5]);
            if (iVar4 != 0 && localVars[0] == localVars[4] && localVars[1] == localVars[5]) {
                break;
            }
        }
        PDMouseButtons(&localVars[6], &localVars[7]);
        if (localVars[6] != 0 || localVars[7] != 0) {
            break;
        }
        iVar2 = gCurrent_mouse_position.x;
        iVar3 = gCurrent_mouse_position.y;
        GetMousePosition(&localVars[2], &localVars[3]);
        if (iVar2 == localVars[2] && iVar3 == localVars[3]) {
            iVar4 = PDGetMouseClickPosition(&localVars[8], &localVars[9]);
            if (iVar4 != 0 && localVars[2] == localVars[8] && localVars[3] == localVars[9]) {
                break;
            }
        }
        PDMouseButtons(&localVars[10], &localVars[11]);
        if (localVars[10] != 0 || localVars[11] != 0) {
            break;
        }
    }
    CheckQuit();
    do {
        CheckQuit();
        iVar4 = PDGetTotalTime();
        if (500 < (tU32)(iVar4 - gLast_poll_keys)) {
            puVar7 = gKey_array;
            for (iVar4 = 0x97; iVar4 != 0; iVar4--) {
                *puVar7++ = 0;
            }
            puVar7 = gJoy_array;
            for (iVar4 = 8; iVar4 != 0; iVar4--) {
                *puVar7++ = -1;
            }
            CyclePollKeys();
            PollKeys();
        }
        iVar4 = 0x8e;
        piVar6 = &gKey_array[0x8e];
        do {
            if (*piVar6 != 0 && piVar6 != &gKey_array[4]) {
                if (gEdge_trigger_mode == 0) {
                    goto key_found_second;
                }
                uVar5 = EdgeTriggeryKey(iVar4, 0);
                switch (uVar5) {
                case 0:
                case 2:
                    goto key_scan_next_second;
                case 1:
                case 3:
                    goto key_found_second;
                }
            }
key_scan_next_second:
            piVar6--;
            iVar4--;
        } while ((tS32)piVar6 >= (tS32)gKey_array);
        if (gEdge_trigger_mode != 0) {
            EdgeTriggeryKey(0, 1);
        }
        iVar4 = -1;
key_found_second:
        if (iVar4 == -1 || iVar4 == 4) {
            iVar2 = gCurrent_mouse_position.x;
            iVar3 = gCurrent_mouse_position.y;
            GetMousePosition(&localVars[3], &localVars[2]);
            if (iVar2 == localVars[3] && iVar3 == localVars[2]) {
                iVar4 = PDGetMouseClickPosition(&localVars[11], &localVars[10]);
                if (iVar4 != 0 && localVars[3] == localVars[11] && localVars[2] == localVars[10]) {
                    goto mouse_activity_found;
                }
            }
            PDMouseButtons(&localVars[9], &localVars[8]);
            if (localVars[9] != 0 || localVars[8] != 0) {
                goto mouse_activity_found;
            }
            bVar1 = 0;
        } else {
mouse_activity_found:
            bVar1 = 1;
        }
        if (!bVar1) {
            iVar2 = gCurrent_mouse_position.x;
            iVar3 = gCurrent_mouse_position.y;
            GetMousePosition(&localVars[1], &localVars[0]);
            if (iVar2 == localVars[1] && iVar3 == localVars[0] &&
                    (iVar4 = PDGetMouseClickPosition(&localVars[7], &localVars[6]), iVar4 != 0 &&
                     localVars[1] == localVars[7] && localVars[0] == localVars[6])) {
                iVar4 = 2;
            } else {
                PDMouseButtons(&localVars[5], &localVars[4]);
                if (localVars[5] == 0 && localVars[4] == 0) {
                    iVar4 = 0;
                } else {
                    iVar4 = 1;
                }
            }
            if (iVar4 == 0) {
                CheckQuit();
                return;
            }
        }
    } while (1);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00483c10
void C2_HOOK_FASTCALL GetMousePosition(int *pX, int *pY) {

    PDGetMousePosition(pX, pY);
    if (*pX < 0) {
        *pX = 0;
    } else if (*pX > gGraf_specs[gGraf_spec_index].total_width) {
        *pX = gGraf_specs[gGraf_spec_index].total_width;
    }
    if (*pY < 0) {
        *pY = 0;
        gCurrent_mouse_position.x = *pX;
        gCurrent_mouse_position.y = *pY;
        return;
    } else if (*pY > gGraf_specs[gGraf_spec_index].total_height) {
        *pY = gGraf_specs[gGraf_spec_index].total_height;
    }
    gCurrent_mouse_position.x = *pX;
    gCurrent_mouse_position.y = *pY;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00483c90
void C2_HOOK_FASTCALL InitRollingLetters(void) {
    int i;

    gLast_roll = 0;
    gCurrent_cursor = -1;
    gRolling_letters = BrMemAllocate(NBR_ROLLING_LETTERS * sizeof(tRolling_letter), kMem_misc);
    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        gRolling_letters[i].number_of_letters = -1;
    }
}

// FUNCTION: CARMA2_HW 0x00483ce0
void C2_HOOK_FASTCALL EndRollingLetters(void) {
    BrMemFree(gRolling_letters);
}

// AddRollingLetter

// ChangeCharTo

// FUNCTION: CARMA2_HW 0x004840d0
void C2_HOOK_FASTCALL FUN_004840d0(int pSlot_index) {
    unsigned i;

    i = 0;
    if (0 < gThe_length) {
        do {
            ChangeCharTo(pSlot_index, i, i < strlen(gThe_text) ? gThe_text[i] : ' ');
            i += 1;
        } while ((int)i < gThe_length);
    }
}

// FUNCTION: CARMA2_HW 0x00484120
void C2_HOOK_FASTCALL RevertTyping(int pSlot_index, char* pRevert_str) {
    unsigned i;

    for (i = 0; (int)i < gThe_length; i++) {
        ChangeCharTo(pSlot_index, i, i >= strlen(pRevert_str) ? ' ' : pRevert_str[i]);
    }
}

// FUNCTION: CARMA2_HW 0x00484170
void C2_HOOK_FASTCALL StartTyping(int pSlot_index, char* pText, int pVisible_length) {
    gThe_length = pVisible_length;
    strcpy(gThe_text, pText);
    gCurrent_position = strlen(gThe_text);
    gVisible_length = pVisible_length;
    gCurrent_cursor = ChangeCharTo(pSlot_index, gCurrent_position, ROLLING_LETTER_LOOP_RANDOM);
}

// FUNCTION: CARMA2_HW 0x00484600
void C2_HOOK_FASTCALL EdgeTriggerModeOn(void) {

    gEdge_trigger_mode = 1;
}


// FUNCTION: CARMA2_HW 0x00484610
void C2_HOOK_FASTCALL EdgeTriggerModeOff(void) {

    gEdge_trigger_mode = 0;
}
