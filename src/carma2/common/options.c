#include "options.h"

#include "errors.h"
#include "globvars.h"
#include "loading.h"
#include "utility.h"
#include "brucetrk.h"
#include "depth.h"
#include "graphics1.h"
#include "sound.h"
#include "spark.h"
#include "world.h"

#include "c2_string.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"
// (options.h contents inlined in the merged headers)
#include "loading1.h"
#include "packfile.h"
#include "globvars.h"
#include "drmem.h"
#include "flicplay.h"
#include "font.h"
#include "graphics.h"
#include "interface-graphics.h"
#include "intrface.h"
#include "network.h"
#include "platform.h"
extern void C2_HOOK_FASTCALL LoadKeyMapping(void);
extern void C2_HOOK_FASTCALL FUN_004b3ec0(void);
extern int gUNK_0065bd80;
extern int gUNK_0065bda0;
extern int gUNK_0065bdc0;
extern int gUNK_0065bde0;
extern int gUNK_0065be00;
extern int gUNK_0065be20;
extern int gUNK_0065be40;
extern int gUNK_0065be60;
extern int gUNK_0065be80;
extern int gUNK_0065bea0;
extern int gUNK_0065bec0;
extern int gUNK_0065bee0;
extern float gUNK_0058a6e0;
// GLOBAL: CARMA2_HW 0x006869e0
extern int gBackup_key_mappings[4][77];
extern int gKey_map_index;
extern int gKey_mapping[77];
extern int gOrig_key_map_index;

// GLOBAL: CARMA2_HW 0x00688458
char* gKey_names[153];

// GLOBAL: CARMA2_HW 0x006918d0
char* gOptions_key_names[153];

// GLOBAL: CARMA2_HW 0x0065cd70
tInterface_spec gInterface_0065cd70;

// GLOBAL: CARMA2_HW 0x0065ca90
tInterface_spec gInterface_0065ca90;

// GLOBAL: CARMA2_HW 0x0065c780
tInterface_spec gInterface_0065c780;

// GLOBAL: CARMA2_HW 0x0065c068
tInterface_spec gInterface_0065c068;

// GLOBAL: CARMA2_HW 0x00691798
int gINT_00691798[77];

// GLOBAL: CARMA2_HW 0x0069178c
int gINT_0069178c;

// GLOBAL: CARMA2_HW 0x00691b34
int gINT_00691b34;

// GLOBAL: CARMA2_HW 0x00691b38
int gINT_00691b38;

extern int gUNK_00691788;
extern tU8* gUNK_006918cc;

// GLOBAL: CARMA2_HW 0x0058a6e4
const br_scalar gFloat_0058a6e4 = 0.2f;

// GLOBAL: CARMA2_HW 0x0058a6e8
const br_scalar gFloat_0058a6e8 = 1.0f;

// GLOBAL: CARMA2_HW 0x0058a6ec
const br_scalar gFloat_0058a6ec = 0.5f;

// GLOBAL: CARMA2_HW 0x0058a6f0
const br_scalar gFloat_0058a6f0 = 0.25f;

// GLOBAL: CARMA2_HW 0x00688440
int gOrig_key_map_index;

// GLOBAL: CARMA2_HW 0x006869e0
int gBackup_key_mappings[4][77];


#include "errors.h"
#include "globvars.h"
#include "loading1.h"
#include "utility.h"

#include "c2_string.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"


// FUNCTION: CARMA2_HW 0x004b39f0
void C2_HOOK_FASTCALL DoOptions(void) {
    union {
        float yon_value;
        int key_map_index;
    } local_208;
    int show_error;
    char keymap_filename[256];
    char keymap_path[256];
    int result;
    FILE* file;
    char** key_name;
    float yon_factor;
    int i;

    PrintMemoryDump(0, "BEFORE ENTERING OPTIONS");
    PreloadBunchOfFlics(1);
    result = DoInterfaceScreen(&gInterface_0065cd70, 0, (gProgram_state.track_spec.the_actor != NULL) + 1);
    if (result < 3) {
        if (result == 2) {
            local_208.key_map_index = gKey_map_index;
            memcpy(gINT_00691798, gKey_mapping, sizeof(gKey_mapping));
            show_error = 0;
            gUNK_006918cc = (tU8*)&gInterface_0065ca90;
            PathCat(keymap_path, gApplication_path, "KEYMAP_X.TXT");
            file = DRfopen(keymap_path, "rt");
            if (file == NULL) {
                FatalError(0x6a);
            }
            key_name = gOptions_key_names;
            while ((int)key_name < (int)&gOptions_key_names[CARPOCALYPSE2_ASIZE(gOptions_key_names)]) {
                PFfgets(keymap_filename, sizeof(keymap_filename), file);
                StripControls(keymap_filename);
                *key_name = BrMemAllocate(strlen(keymap_filename) + 1, kMem_misc);
                strcpy(*key_name, keymap_filename);
                key_name++;
            }
            PFfclose(file);
            LoadFont(9);
            LoadFont(11);
            LoadFont(10);
            LoadFont(12);
            gINT_00691b34 = -1;
            gINT_0069178c = 0x12;
            gINT_00691b38 = 8;
            do {
                result = DoInterfaceScreen(&gInterface_0065ca90, show_error != 0, 0);
                if (result >= 0) {
                    break;
                }
                FadePaletteDown();
                NetFullScreenMessage(0xe5, 0);
                FadePaletteDown();
                show_error = 1;
            } while (result < 0);
            if (result == 0) {
                PathCat(keymap_filename, gApplication_path, "KEYMAP_X.TXT");
                keymap_filename[strlen(keymap_filename) - 5] = gKey_map_index + '0';
                PDFileUnlock(keymap_filename);
                file = DRfopen(keymap_filename, "wb");
                if (file == NULL) {
                    FatalError(9);
                }
                for (i = 0; i < CARPOCALYPSE2_ASIZE(gKey_mapping); i++) {
                    fprintf(file, "%d", gKey_mapping[i]);
                    fputc('\r', file);
                    fputc('\n', file);
                }
                PFfclose(file);
            } else {
                memcpy(gKey_mapping, gINT_00691798, sizeof(gKey_mapping));
                gKey_map_index = local_208.key_map_index;
            }
            DisposeFont(9);
            DisposeFont(11);
            DisposeFont(10);
            DisposeFont(12);
            FUN_004b4070();
            if (gProgram_state.racing != 0) {
                FadePaletteDown();
            } else {
                RunFlic(0xab);
            }
        } else if (result == 1) {
            gUNK_006918cc = (tU8*)&gInterface_0065c780;
            gUNK_00691788 = -1;
            LoadFont(12);
            gUNK_0065bd80 = GetCarSimplificationLevel();
            if (gUNK_0065bd80 >= 2) {
                gUNK_0065bd80--;
            }
            switch (GetCarTexturingLevel()) {
            case 0:
                gUNK_0065bda0 = 2;
                break;
            case 1:
                gUNK_0065bda0 = 1;
                break;
            case 2:
                gUNK_0065bda0 = 0;
                break;
            }
            switch (GetShadowLevel()) {
            case 0:
                gUNK_0065bdc0 = 2;
                break;
            case 1:
                gUNK_0065bdc0 = 1;
                break;
            default:
                gUNK_0065bdc0 = 0;
                break;
            }
            switch (GetWallTexturingLevel()) {
            case 0:
                gUNK_0065bde0 = 2;
                break;
            case 1:
                gUNK_0065bde0 = 1;
                break;
            case 2:
                gUNK_0065bde0 = 0;
                break;
            }
            switch (GetRoadTexturingLevel()) {
            case 0:
                gUNK_0065be00 = 2;
                break;
            case 1:
                gUNK_0065be00 = 0;
                break;
            }
            local_208.yon_value = GetYon();
            gUNK_0065be20 = (int)((gUNK_0058a6e0 - local_208.yon_value) * gFloat_0058a6e4);
            if (gUNK_0065be20 > 3) {
                gUNK_0065be20 = 3;
            }
            yon_factor = GetYonFactor();
            if (yon_factor < gFloat_0058a6e8) {
                if (yon_factor < gFloat_0058a6ec) {
                    gUNK_0065be40 = 2;
                    if (yon_factor < gFloat_0058a6f0) {
                        gUNK_0065be40 = 3;
                    }
                } else {
                    gUNK_0065be40 = 1;
                }
            } else {
                gUNK_0065be40 = 0;
            }
            gUNK_0065be60 = !GetSkyTextureOn();
            gUNK_0065be80 = !GetDepthCueingOn();
            gUNK_0065bea0 = !GetAccessoryRendering();
            gUNK_0065bec0 = !GetSmokeOn();
            i = GetSoundDetailLevel();
            gUNK_0065bee0 = 2 - i;
            if (DoInterfaceScreen(&gInterface_0065c780, 0, 0) == 0) {
                FUN_004b3ec0();
            }
            if (gProgram_state.racing != 0) {
                FadePaletteDown();
                DisposeFont(12);
            } else {
                RunFlic(0xa1);
                DisposeFont(12);
            }
        } else if (result == 0) {
            DoInterfaceScreen(&gInterface_0065c068, 0, 0);
            if (gProgram_state.racing != 0) {
                FadePaletteDown();
            } else {
                RunFlic(0x97);
            }
        }
    }
    UnlockBunchOfFlics(1);
    PrintMemoryDump(0, "AFTER ENTERING OPTIONS");
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b4020
void C2_HOOK_FASTCALL StripControls(char* pStr) {
    int i;
    int len;

    len = strlen(pStr);
    for (i = 0; i < len; i++) {
        if ((tU8)pStr[i] < ' ') {
            memcpy(&pStr[i], &pStr[i + 1], (len - i) * sizeof(char));
            len--;
#ifdef CARPOCALYPSE2_FIX_BUGS
            /* correctly handle stripping multiple control characters */
            i--;
#endif
        }
    }
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL LoadKeyNames(void) {
    int i;
    FILE* f;
    tPath_name the_path;
    char s[256];

    PathCat(the_path, gApplication_path, "KEYNAMES.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(kFatalError_CantOpenKeyNamesFile);
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gKey_names); i++) {
        PFfgets(s, sizeof(s), f);
        StripControls(s);
        gKey_names[i] = BrMemAllocate(strlen(s) + 1, kMem_misc);
        strcpy(gKey_names[i], s);
    }
    PFfclose(f);
}

void C2_HOOK_FASTCALL DisposeKeyNames(void) {
    int i;

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gKey_names); i++) {
        BrMemFree(gKey_names[i]);
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b4070
void C2_HOOK_FASTCALL FUN_004b4070(void) {
    char** p;

    p = gOptions_key_names;
    do {
        BrMemFree(*p);
        p++;
    } while ((int)p < (int)&gOptions_key_names[CARPOCALYPSE2_ASIZE(gOptions_key_names)]);
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL BackupKeyMappings(void) {
    int i;

    gOrig_key_map_index = gKey_map_index;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gBackup_key_mappings) != 4);
    C2_HOOK_BUG_ON(sizeof(gKey_mapping) != 4 * 77);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gBackup_key_mappings); i++) {

        gKey_map_index = i;
        LoadKeyMapping();
        memcpy(gBackup_key_mappings[i],
            gKey_mapping,
            sizeof(gKey_mapping));
    }
    gKey_map_index = gOrig_key_map_index;
    memcpy(gKey_mapping,
        gBackup_key_mappings[gKey_map_index],
        sizeof(gKey_mapping));
}

// GLOBAL: CARMA2_HW 0x0065bd80
int gUNK_0065bd80;

// GLOBAL: CARMA2_HW 0x0065bda0
int gUNK_0065bda0;

// GLOBAL: CARMA2_HW 0x0065bdc0
int gUNK_0065bdc0;

// GLOBAL: CARMA2_HW 0x0065bde0
int gUNK_0065bde0;

// GLOBAL: CARMA2_HW 0x0065be00
int gUNK_0065be00;

// GLOBAL: CARMA2_HW 0x0065be20
int gUNK_0065be20;

// GLOBAL: CARMA2_HW 0x0065be40
int gUNK_0065be40;

// GLOBAL: CARMA2_HW 0x0065be60
int gUNK_0065be60;

// GLOBAL: CARMA2_HW 0x0065be80
int gUNK_0065be80;

// GLOBAL: CARMA2_HW 0x0065bea0
int gUNK_0065bea0;

// GLOBAL: CARMA2_HW 0x0065bec0
int gUNK_0065bec0;

// GLOBAL: CARMA2_HW 0x0065bee0
int gUNK_0065bee0;

// GLOBAL: CARMA2_HW 0x0058a6f4
float gUNK_0058a6f4 = 5.0f;

// GLOBAL: CARMA2_HW 0x0058a6e0
float gUNK_0058a6e0 = 35.0f;

// C2PORT-NONMATCHING: best 85.17% | difftest PASS 2000 trials (spec difftest/specs/004b3ec0_FUN_004b3ec0.json) | reason regalloc-switch-chain
// FUNCTION: CARMA2_HW 0x004b3ec0
void C2_HOOK_FASTCALL FUN_004b3ec0(void) {
    int level;

    SetCarSimplificationLevel(gUNK_0065bd80 >= 2 ? gUNK_0065bd80 + 1 : gUNK_0065bd80);
    level = -1;
    switch (gUNK_0065bda0) {
    case 0: level = 2; break;
    case 1: level = 1; break;
    case 2: level = 0; break;
    }
    if (level != -1) {
        SetCarTexturingLevel(level);
    }
    level = -1;
    switch (gUNK_0065bdc0) {
    case 0: level = 3; break;
    case 1: level = 1; break;
    case 2: level = 0; break;
    }
    if (level != -1) {
        SetShadowLevel(level);
    }
    level = -1;
    switch (gUNK_0065bde0) {
    case 0: level = 2; break;
    case 1: level = 1; break;
    case 2: level = 0; break;
    }
    if (level != -1) {
        SetWallTexturingLevel(level);
    }
    level = -1;
    switch (gUNK_0065be00) {
    case 0: level = 1; break;
    case 2: level = 0; break;
    }
    if (level != -1) {
        SetRoadTexturingLevel(level);
    }
    SetYon(gUNK_0058a6e0 - (float)gUNK_0065be20 * gUNK_0058a6f4);
    switch (gUNK_0065be40) {
    case 0:
        SetYonFactor(1.0f);
        break;
    case 1:
        SetYonFactor(0.5f);
        break;
    case 2:
        SetYonFactor(0.25f);
        break;
    case 3:
        SetYonFactor(0.125f);
        break;
    default:
        break;
    }
    SetSkyTextureOn(1 - gUNK_0065be60);
    SetDepthCueingOn(1 - gUNK_0065be80);
    SetAccessoryRendering(1 - gUNK_0065bea0);
    SetSmokeOn(1 - gUNK_0065bec0);
    SetSoundDetailLevel(2 - gUNK_0065bee0);
    SaveOptions();
}
