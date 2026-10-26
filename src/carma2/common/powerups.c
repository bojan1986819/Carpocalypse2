#include "powerups.h"

#include "animation.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "errors.h"
#include "explosions.h"
#include "funk.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "init.h"
#include "input.h"
#include "loading.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pedestrn.h"
#include "physics.h"
#include "piping.h"
#include "platform.h"
#include "raycast.h"
#include "shrapnel.h"
#include "skidmark.h"
#include "smashing.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"

#include "brender/brender.h"

#include "carpocalypse2_macros.h"

#include "c2_string.h"
#include "carpocalypse2_types.h"

#include <math.h>
#ifdef __vita__
#include "sdl3/vita_controls.h"
#include "sdl3/vita_gl.h"
#include <stdlib.h>
#endif
extern const char gEmpty_string_00676920[];
extern br_material* oily_material;
extern float gPedestrian_speed_factor;
extern int gExploding_pedestrians;
extern int gPed_dismemberfest;
extern int gBlind_pedestrians;
extern int gImmortal_peds;
extern int gPeds_suicidal;
extern int gDancing_peds;
extern int gPanicking_peds;
extern int gDrunk_pedestrians;
extern float gPed_gravity_multiplier;
extern float gMutant_speed;
// GLOBAL: CARMA2_HW 0x0058abf0
const double gDOUBLE_0058abf0 = 0.45;

// GLOBAL: CARMA2_HW 0x0058abf8
const double gDOUBLE_0058abf8 = 0.99;

// GLOBAL: CARMA2_HW 0x0058ac00
const float gFLOAT_0058ac00 = 0.5f;

// GLOBAL: CARMA2_HW 0x0058ac0c
const float gFLOAT_0058ac0c = 0.0f;

// GLOBAL: CARMA2_HW 0x0058abe0
const double gDouble_0058abe0 = 6.9;

// GLOBAL: CARMA2_HW 0x0058abe8
const double gDouble_0058abe8 = 6.283185307179586;

// GLOBAL: CARMA2_HW 0x0058ac10
const float gFLOAT_0058ac10 = 2.384185791015625e-7f;

// GLOBAL: CARMA2_HW 0x0058ac18
const double gDOUBLE_0058ac18 = 1.0;

// GLOBAL: CARMA2_HW 0x0058ac20
const float gFLOAT_0058ac20 = 1.0f;

// GLOBAL: CARMA2_HW 0x0058ac48
const double gDOUBLE_0058ac48 = 1.4285714285714286;

// GLOBAL: CARMA2_HW 0x0058ac40
const float gFLOAT_0058ac40 = 0.1f;

// GLOBAL: CARMA2_HW 0x0058ac8c
const float gFLOAT_0058ac8c = 25.0f;

// GLOBAL: CARMA2_HW 0x0058ac88
const float gFLOAT_0058ac88 = 0.025f;

// GLOBAL: CARMA2_HW 0x0058ac80
const double gDOUBLE_0058ac80 = 0.1;

// GLOBAL: CARMA2_HW 0x0058ac60
const double gDOUBLE_0058ac60 = -1000.0;

// GLOBAL: CARMA2_HW 0x0058ac68
const double gDOUBLE_0058ac68 = 0.15;

// GLOBAL: CARMA2_HW 0x0058ac78
const double gDOUBLE_0058ac78 = 1000.0;

// GLOBAL: CARMA2_HW 0x0058ac70
const double gDOUBLE_0058ac70 = -0.15;

// GLOBAL: CARMA2_HW 0x0058ac50
const float gBounce_rate_const = 1000.0f;

// GLOBAL: CARMA2_HW 0x0065feb0
const char* gRepeatability_names[4] = {
    "none",
    "singleshot",
    "repeated",
    "doitregardless",
};

// GLOBAL: CARMA2_HW 0x0065ebb0
const char* gFizzle_names[3] = {
    "CIRCLES",
    "SQUARES",
    "DIAMONDS",
};

// GLOBAL: CARMA2_HW 0x006a52d0
tSmashable_item_spec gPowerup_pickup_smashable;

// GLOBAL: CARMA2_HW 0x006a7ce0
tSmashable_item_spec gPowerup_respawn_smashable;

// GLOBAL: CARMA2_HW 0x006a3660
tSmashable_item_spec gUnknown_smashable_006a3660;

// GLOBAL: CARMA2_HW 0x006a0ac0
br_pixelmap* gFizzle_in[3];

// GLOBAL: CARMA2_HW 0x006a0950
int gFizzle_height;

// GLOBAL: CARMA2_HW 0x006a0ad0
int gNumber_of_powerups;

// GLOBAL: CARMA2_HW 0x006a0a54
extern tPowerup* gPowerup_array;

extern tNet_mode gNet_mode;
extern int gRecovery_voucher_count;
extern int gIt_or_fox;
extern int gThis_net_player_index;

void C2_HOOK_FASTCALL AddVouchers(int amount);
void C2_HOOK_FASTCALL RecalculateCarMassMomentOfInertia(tCar_spec* car);
// GLOBAL: CARMA2_HW 0x0074d1a4
extern int gNet_powerup_time_replacement;

void C2_HOOK_FASTCALL ScalePeds(br_vector3* scale);
void C2_HOOK_STDCALL ScalePedHeads(float head_scale);
void C2_HOOK_FASTCALL MakePedsEthereal(void);
void C2_HOOK_FASTCALL MakePedsNotEthereal(void);
int C2_HOOK_FASTCALL FindPedestrianNearCar(tCar_spec* car, br_scalar radius);
int C2_HOOK_FASTCALL NapalmPedestrianNearCar(tCar_spec* car);
int C2_HOOK_FASTCALL DoPedestrianRepulsifictor(tCar_spec* pCar, float pF0, float pF1, float pF2, float pF3);
// GLOBAL: CARMA2_HW 0x006a0948
int gNumber_of_powerup_respawn_specs;

// GLOBAL: CARMA2_HW 0x006a0a64
tPowerup_respawn_spec* gPowerup_respawn_specs;

// GLOBAL: CARMA2_HW 0x006a0ad8
void* gDrug_trip_data;

// GLOBAL: CARMA2_HW 0x0075d33c
int gPowerup_net_state[56];

static void C2_HOOK_FASTCALL PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    tU32* dst;
    tU32* src;

    (void)pPixelmap;
    dst = (tU32*)gRender_palette->pixels;
    src = (tU32*)gDrug_trip_data;
    for (i = 1; i < 0xe0; i++) {
        dst[i] = src[(i + pOffset) & 0xff];
    }
    dst[0] = src[0];
    for (i = 0xe0; i < 0x100; i++) {
        dst[i] = src[i];
    }
}

// GLOBAL: CARMA2_HW 0x006a0a68
int gPissed_next_time;

// Amplitudes, periods, and phase offsets, each with twelve reserved slots.
// GLOBAL: CARMA2_HW 0x00705be4
float gPissed_flicker[36];

// GLOBAL: CARMA2_HW 0x00705c74
float gPissed_hold_time;

// GLOBAL: CARMA2_HW 0x0058ac90
double gConst_kangeroo_force_factor = 0.14492753623188406;

// GLOBAL: CARMA2_HW 0x006a0a50
tU8* gRace_powerup_respawn_bools;

// GLOBAL: CARMA2_HW 0x006a0a60
int gNet_powerup_time_between_goodies_min;

// GLOBAL: CARMA2_HW 0x006a090c
int gNet_powerup_time_between_goodies_max;

// GLOBAL: CARMA2_HW 0x006a0450
int gNet_count_powerup_goodies;

// GLOBAL: CARMA2_HW 0x006a094c
int* gNet_powerup_goodies;

// GLOBAL: CARMA2_HW 0x0065e910
tGot_proc* gGot_procs[56] = {
    GotCredits,
    SetPedSpeed,
    SetPedSize,
    SetPedExplode,
    PickAtRandom,
    SetInvulnerability,
    SetFreeRepairs,
    DoInstantRepair,
    FreezeTimer,
    SetEngineFactor,
    SetUnderwater,
    GotTimeOrPower,
    TrashBodywork,
    TakeDrugs,
    SetOpponentsSpeed,
    SetCopsSpeed,
    SetGravity,
    SetPinball,
    SetWallclimb,
    SetBouncey,
    SetSuspension,
    SetTyreGrip,
    SetDamageMultiplier,
    SetImmortalPeds,
    SetStupidPedestrians,
    SetSuicidalPedestrians,
    GotVouchers,
    SetMassMultiplier,
    SetInstantHandbrake,
    ShowPedestrians,
    HitMine,
    SetElectroBastard,
    SetPedHeadSize,
    SetMutantCorpses,
    ShitMine,
    PissOutOil,
    KangerooJump,
    AnnihilatePeds,
    RepulseOpponents,
    SetPedBrittle,
    SetGhostPeds,
    SetDancingPeds,
    SetPanickingPeds,
    SetLowGravityPeds,
    SetPissedPeds,
    RepulsePedestrians,
    GainAPO,
    GainAPOPotential,
    SetPissed,
    NapalmPeds,
    PedValium,
    PowerupCancel,
    GrowATail,
    CutOffTail,
    TurnOnCloaking,
    NULL,
};

// GLOBAL: CARMA2_HW 0x0065e9f0
tLose_proc* gLose_procs[56] = {
    NULL,
    ResetPedSpeed,
    ResetPedSize,
    ResetPedExplode,
    NULL,
    ResetInvulnerability,
    ResetFreeRepairs,
    NULL,
    UnfreezeTimer,
    ResetEngineFactor,
    ResetUnderwater,
    NULL,
    NULL,
    PukeDrugsBackUp,
    ResetOpponentsSpeed,
    ResetCopsSpeed,
    ResetGravity,
    ResetPinball,
    ResetWallclimb,
    ResetBouncey,
    ResetSuspension,
    ResetTyreGrip,
    ResetDamageMultiplier,
    ResetImmortalPeds,
    ResetStupidPedestrians,
    ResetSuicidalPeds,
    NULL,
    ResetMassMultiplier,
    ResetInstantHandbrake,
    HidePedestrians,
    NULL,
    ResetElectroBastard,
    ResetPedHeadSize,
    ResetMutantCorpses,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ResetPedBrittle,
    ResetGhostPeds,
    ResetDancingPeds,
    ResetPanickingPeds,
    ResetLowGravityPeds,
    ResetPissedPeds,
    NULL,
    NULL,
    NULL,
    ResetPissed,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    TurnOffCloaking,
    NULL,
};

// GLOBAL: CARMA2_HW 0x0065ead0
tPeriodic_proc* gPeriodic_procs[56] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    TheEffectsOfDrugs,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    DoBouncey,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MungeVouchers,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PissedEffect,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

// GLOBAL: CARMA2_HW 0x00705560
tShit_mine gShit_mines[20];

// GLOBAL: CARMA2_HW 0x006a0960
tRepulse_link gRepulse_links[6];

// GLOBAL: CARMA2_HW 0x006a0acc
tPhysics_joint* gMutant_tail_first_joint;

// GLOBAL: CARMA2_HW 0x006a0908
tPhysics_object* gMutant_tail_first_collision_info;

// GLOBAL: CARMA2_HW 0x00705540
int gMutant_tail_state;

// GLOBAL: CARMA2_HW 0x006a0ae0
br_model* gModel_powerup_armor;

// GLOBAL: CARMA2_HW 0x006a0ae4
br_model* gModel_powerup_power;

// GLOBAL: CARMA2_HW 0x006a0ae8
br_model* gModel_powerup_offence;

// GLOBAL: CARMA2_HW 0x006a0a70
int gInventory[20];

// GLOBAL: CARMA2_HW 0x006a0a6c
int gINT_006a0a6c;

// GLOBAL: CARMA2_HW 0x006a0a5c
int gINT_006a0a5c;

// GLOBAL: CARMA2_HW 0x006a0458
tRespawn_powerup gRespawn_powerups[100];

// GLOBAL: CARMA2_HW 0x006a0a58
int gInventory_selected;

// GLOBAL: CARMA2_HW 0x006a0944
int gNumber_of_icons;

// GLOBAL: CARMA2_HW 0x006a0aec
tU32 gNext_goody_time;

// GLOBAL: CARMA2_HW 0x006a0958
undefined4 gDAT_006a0958;

// FUNCTION: CARMA2_HW 0x004dedf0
void C2_HOOK_FASTCALL SendCurrentPowerups(void) {
    tCar_spec* car;
    tNet_message_chunk* message;
    int locals[3];
    int player_id;
    int net_player_index;
    int powerup_index;
    int* powerup_time;
    int current_time;
    tNet_game_player_info* net_player;

    player_id = locals[2];
    locals[1] = 0;
    do {
        locals[0] = locals[1] == 0 ? 1 : GetCarCount(locals[1]);
        locals[2] = 0;
        if (locals[0] > 0) {
            do {
                if (locals[1] == 0) {
                    car = &gProgram_state.current_car;
                } else {
                    car = GetCarSpec(locals[1], locals[2]);
                }
                net_player_index = 0;
                if (gNumber_of_net_players > 0) {
                    net_player = gNet_players;
                    while (net_player_index < gNumber_of_net_players) {
                        if (net_player->car == car) {
                            player_id = gNet_players[net_player_index].ID;
                            break;
                        }
                        net_player_index++;
                        net_player++;
                    }
                }
                powerup_index = 0;
                if (gNumber_of_powerups > 0) {
                    powerup_time = car->powerups;
                    do {
                        if (*powerup_time != 0) {
                            message = NetStartBroadcastContents(eNet_message_chunk_type_powerup, 0);
                            if (message != NULL) {
                                message->powerup.field_0x10 = 1;
                                message->powerup.player_id = player_id;
                                message->powerup.powerup_index = powerup_index;
                                current_time = GetTotalTime();
                                message->powerup.time_remaining = *powerup_time - current_time;
                                NetBroadcastContents(message);
                            }
                        }
                        powerup_index++;
                        powerup_time++;
                    } while (powerup_index < gNumber_of_powerups);
                }
                locals[2]++;
            } while (locals[2] < locals[0]);
        }
        locals[1]++;
    } while (locals[1] <= 1);
}

// FUNCTION: CARMA2_HW 0x004df1e0
int C2_HOOK_FASTCALL GetPowerupCount(void) {

    return 0;
}

// FUNCTION: CARMA2_HW 0x004df1f0
void C2_HOOK_FASTCALL GetPowerupPosition(int pIndex, br_vector3* pPosition) {

}

void C2_HOOK_FASTCALL RemoveRepulseEffect(tRepulse_link* pRepulse) {

    pRepulse->time = 0;
    if (pRepulse->actor->parent == gNon_track_actor) {
        BrActorRemove(pRepulse->actor);
    }
}

void C2_HOOK_FASTCALL ResetRepulseEffects(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gRepulse_links) != 6);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gRepulse_links); i++) {
        RemoveRepulseEffect(&gRepulse_links[i]);
    }
}

// FUNCTION: CARMA2_HW 0x004da600
void C2_HOOK_FASTCALL EmptyInventory(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gInventory) != 20);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gInventory); i++) {
        gInventory[i] = -1;
    }
    gInventory_selected = -1;
    gINT_006a0a6c = 0;
    gINT_006a0a5c = 0;
}

// FUNCTION: CARMA2_HW 0x004da630
void C2_HOOK_FASTCALL ResetPowerups(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gRespawn_powerups) != 100);
    C2_HOOK_BUG_ON(sizeof(gRespawn_powerups[0]) != 0xc);

    gRace_powerup_respawn_bools = gPowerup_respawn_specs[gNet_mode == eNet_mode_none ? 0 : 1].bools;

    for (i = 0; i < gNumber_of_powerups; i++) {
        gPowerup_array[i].got_time = 0;
        gPowerup_array[i].current_value = -1;
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gRespawn_powerups); i++) {
        gRespawn_powerups[i].actor = NULL;
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gPickedup_powerups); i++) {
        if (gPickedup_powerups[i].icon_actor != NULL) {
            gPickedup_powerups[i].icon_actor->render_style = BR_RSTYLE_NONE;
        }
    }
    EmptyInventory();

    gNumber_of_icons = 0;
    gDAT_006a0958 = 0;
    gInventory_cycling = 0;
    gInventory_timeout = 0;
    gCount_cloaked_cars = 0;
    ResetRepulseEffects();
    gNext_goody_time = 0;
}

static br_pixelmap* DRMapFindMeAPixie(const char* name) {
    char s[256];
    br_pixelmap* map;

    map = BrMapFind(name);
    if (map != NULL) {
        return map;
    }
    strcpy(s, name);
    strcat(s, ".PIX");
    map = BrMapFind(s);
    if (map != NULL) {
        return map;
    }
    strcpy(s, name);
    strcat(s, ".pix");
    map = BrMapFind(s);
    if (map != NULL) {
        return map;
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x004d96c0
void C2_HOOK_FASTCALL LoadPowerups(void) {
    FILE* f;
    tPath_name the_path;
    tTWTVFS twtvfs;
    int action_index;
    int first;
    int i, j, k;
    int time;
    int count_index_groups;
    int index_range;
    char s[256];
    tPowerup* the_powerup;
#ifdef __vita__
    int trace_powerups = getenv("C2_VITA_ASSET_TRACE") != NULL;
#endif

    C2_HOOK_BUG_ON(sizeof(tPowerup) != 172);

    if (gPowerup_txt_path == NULL) {
        SetDefaultPowerupFilename();
    }
    PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(the_path, the_path, "POWERUPS");
#ifdef __vita__
    if (trace_powerups) C2VitaLog("POWERUPS begin: %s\n", the_path);
    C2VitaGLTraceTextures(trace_powerups);
#endif
    twtvfs = OpenPackFileAndSetTiffLoading(the_path);
    LoadAllImagesInDirectory(&gMisc_storage_space, the_path);
#ifdef __vita__
    if (trace_powerups) C2VitaLog("POWERUPS images registered; closing pack %d\n", (int)twtvfs);
#endif
    ClosePackFileAndSetTiffLoading(twtvfs);
#ifdef __vita__
    if (trace_powerups) C2VitaLog("POWERUPS pack closed; finding fizzle maps\n");
#endif

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gFizzle_in); i++) {
#ifdef __vita__
        if (trace_powerups) C2VitaLog("POWERUPS finding fizzle %s\n", gFizzle_names[i]);
#endif
        gFizzle_in[i] = DRMapFindMeAPixie(gFizzle_names[i]);
#ifdef __vita__
        if (trace_powerups) C2VitaLog("POWERUPS fizzle %s = %p\n", gFizzle_names[i], (void*)gFizzle_in[i]);
        if (gFizzle_in[i] == NULL) FatalError(kFatalError_CantFindFile_S, gFizzle_names[i]);
#endif
    }
    gFizzle_height = gFizzle_in[0]->height / 4;

    PathCat(the_path, gApplication_path, gPowerup_txt_path);
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(kFatalError_CannotLoadCarResolutionIndependentFile);
    }
    /* Number of powerups */
    gNumber_of_powerups = GetAnInt(f);
#ifdef __vita__
    if (trace_powerups) C2VitaLog("POWERUPS definitions %s count=%d\n", the_path, gNumber_of_powerups);
#endif
    gPowerup_array = BrMemAllocate(sizeof(tPowerup) * gNumber_of_powerups, kMem_powerup_array);
    for (i = 0; i < gNumber_of_powerups; i++) {
        the_powerup = &gPowerup_array[i];

        GetALineAndDontArgue(f, the_powerup->message);
        if (strcmp(the_powerup->message, "dummy") == 0) {
            the_powerup->type = ePowerup_dummy;
        } else {
            if (strcmp(the_powerup->message, "n/a") == 0) {
                the_powerup->message[0] = '\0';
            }
            GetAString(f, s);
#ifdef __vita__
            if (trace_powerups && i < 256) C2VitaLog("POWERUPS item=%d icon=%s lookup\n", i, s);
#endif
            the_powerup->icon = DRMapFindMeAPixie(s);
            if (the_powerup->icon != NULL) {
#ifdef __vita__
                if (trace_powerups && i < 256) C2VitaLog("POWERUPS item=%d map update type=%u size=%dx%d\n",
                    i, the_powerup->icon->type, the_powerup->icon->width, the_powerup->icon->height);
#endif
                BrMapAdd(the_powerup->icon);
                BRPM_convert(the_powerup->icon, gBack_screen->type);
            }
            if (the_powerup->icon != NULL) {
#ifdef __vita__
                if (trace_powerups && i < 256) C2VitaLog("POWERUPS item=%d billboard begin\n", i);
#endif
                the_powerup->icon_actor = CreateBillBoard(the_powerup->icon);
#ifdef __vita__
                if (trace_powerups && i < 256) C2VitaLog("POWERUPS item=%d billboard complete\n", i);
#endif
                the_powerup->icon_actor->material->extra_prim = the_powerup->material_prims;
            }
#ifndef CARPOCALYPSE2_MATCHING
            else {
                dr_dprintf("POWERUP %d '%s': icon '%s' not found, drawn without icon", i, the_powerup->message, s);
            }
#endif


            /* Fizzle type */
            the_powerup->fizzle_type = GetAnInt(f);
            /* Time limit (-1 instantaneous, 0 whole race, x seconds) */
            time = 1000 * GetAnInt(f);
            the_powerup->duration = time;
            if (time < 0) {
                the_powerup->type = ePowerup_instantaneous;
            } else if (time == 0) {
                the_powerup->type = ePowerup_whole_race;
            } else {
                the_powerup->type = ePowerup_timed;
            }
            /* Initial value for keyboard operable powerups
             * (zero if not key operable,
             *  positive if time in seconds,
             *  negative if number of uses)
             */
            the_powerup->initial_value = GetAnInt(f);
            if (the_powerup->initial_value > 0) {
                the_powerup->initial_value *= 1000;
            }
            /* Action index */
            action_index = GetAnInt(f);
            if (action_index >= 0) {
                the_powerup->got_proc = gGot_procs[action_index];
                the_powerup->lose_proc = gLose_procs[action_index];
                the_powerup->periodic_proc = gPeriodic_procs[action_index];
            } else {
                the_powerup->lose_proc = NULL;
                the_powerup->periodic_proc = NULL;
                the_powerup->got_proc = NULL;
            }
            /*  Number of floating point params */
            the_powerup->number_of_float_params = GetAnInt(f);
            the_powerup->float_params = BrMemAllocate(sizeof(float) * the_powerup->number_of_float_params, kMem_powerup_parms);
            for (j = 0; j < the_powerup->number_of_float_params; j++) {
                the_powerup->float_params[j] = GetAFloat(f);
            }
            /* Number of integer params */
            the_powerup->number_of_integer_params = GetAnInt(f);
            the_powerup->integer_params = BrMemAllocate(sizeof(int) * the_powerup->number_of_integer_params, kMem_powerup_parms);
            for (j = 0; j < the_powerup->number_of_integer_params; j++) {
                the_powerup->integer_params[j] = GetAnInt(f);
            }
            /* Group inclusion */
            the_powerup->group_inclusion = GetAnInt(f);
            /* Pratcam sequence */
            the_powerup->prat_cam_event = GetAnInt(f);
            /* Flags (1 = net_global, 2 = net_inappropriate, 4 = off_before_snapweld) */
            the_powerup->net_type = GetAnInt(f);
        }
    }

#ifdef __vita__
    C2VitaGLTraceTextures(0);
    if (trace_powerups) C2VitaLog("POWERUPS icons complete; respawn definitions\n");
#endif
    /* RESPAWN SPECS */

    /* Number of respawn specs */
    gNumber_of_powerup_respawn_specs = GetAnInt(f);
    gPowerup_respawn_specs = BrMemAllocate(gNumber_of_powerup_respawn_specs * sizeof(tPowerup_respawn_spec), kMem_powerup_array);
    for (i = 0; i < gNumber_of_powerup_respawn_specs; i++) {
        gPowerup_respawn_specs[i].bools = BrMemAllocate(gNumber_of_powerups * sizeof(tU8), kMem_powerup_array);
        /* Number of index groups */
        count_index_groups = GetAnInt(f);
        for (j = 0; j < count_index_groups; j++) {
            index_range = GetAnInt(f);
            if (index_range >= 0) {
                /* index_range is index */
                gPowerup_respawn_specs[i].bools[index_range] = 1;
            } else {
                /* index_range is range */
                first = GetAnInt(f);
                for (k = first; k < first - index_range; k++) {
                    gPowerup_respawn_specs[i].bools[k] = 1;
                }
            }
        }
    }
    gRace_powerup_respawn_bools = gPowerup_respawn_specs[0].bools;

    /* NETWORK AUTO-GOODIES */

    /* Min,max times between Auto-goodies */
    GetPairOfInts(f, &gNet_powerup_time_between_goodies_min, &gNet_powerup_time_between_goodies_max);
    gNet_powerup_time_between_goodies_min *= 1000;
    gNet_powerup_time_between_goodies_max *= 1000;
    /* Number of goodies to choose from */
    gNet_count_powerup_goodies = GetAnInt(f);
    gNet_powerup_goodies = BrMemAllocate(gNet_count_powerup_goodies * sizeof(int), kMem_powerup_array);
    for (i = 0; i < gNet_count_powerup_goodies; i++) {
        gNet_powerup_goodies[i] = GetAnInt(f);
    }
    PFfclose(f);
}

// FUNCTION: CARMA2_HW 0x004d9d00
void C2_HOOK_FASTCALL FUN_004d9d00(tPhysics_object* pObject, int pMode) {
    int previous_dismemberfest;

    if ((pObject->field_0x1a0 & 0xf0000) != 0) {
        if (!IsItReallyOKThatWeDontMakeAnyEffortToProtectAnySadFuckersOutThereThatDontWishToSeeInnocentPeopleBlownToBitsByHighExplosiveMinesAndShells()) {
            return;
        }
        if ((*(tU8*)&(*(tPedestrian**)((tU8*)pObject->owner + 0xe4))->character->field_0x14 & 4) == 0 && gCrush_deferred == 0) {
            FUN_004d35e0(*(tPedestrian**)((tU8*)pObject->owner + 0xe4));
        }
        if (pMode != 0xe) {
            pObject->v.v[0] = 0.0f;
            pObject->v.v[1] = 0.0f;
            pObject->v.v[2] = 0.0f;
        }
        pObject->v.v[1] = (float)((double)FRandomBetween(0.7f, 1.0f) / ((double)pObject->M * gDouble_0058abe0) + pObject->v.v[1]);
        pObject->omega.v[2] = (float)((double)FRandomPosNeg(0.5f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[2]);
        pObject->disable_move_rotate = 0;
        pObject->omega.v[0] = (float)((double)FRandomPosNeg(0.5f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[0]);
        previous_dismemberfest = gPed_dismemberfest;
        SetPedFall((tPedestrian*)(gPed_dismemberfest = 1, *(tPedestrian**)((tU8*)pObject->owner + 0xe4)),
                500, 0x61, 0x61, gProgram_state.current_car.collision_info);
        gPed_dismemberfest = previous_dismemberfest;
        return;
    }

    if (pObject == NULL || pObject->flags_0x238 != 0x10) {
        pObject->v.v[1] = (float)((double)FRandomBetween(1.0f, 2.0f) / ((double)pObject->M * gDouble_0058abe0) + pObject->v.v[1]);
        pObject->omega.v[2] = (float)((double)FRandomPosNeg(0.5f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[2]);
        pObject->disable_move_rotate = 0;
        pObject->omega.v[0] = (float)((double)FRandomPosNeg(0.5f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[0]);
    } else {
        pObject->v.v[1] = (float)((double)FRandomBetween(3.0f, 4.0f) / ((double)pObject->M * gDouble_0058abe0) + pObject->v.v[1]);
        pObject->omega.v[2] = (float)((double)FRandomPosNeg(1.75f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[2]);
        pObject->disable_move_rotate = 0;
        pObject->omega.v[0] = (float)((double)FRandomPosNeg(1.75f) * gDouble_0058abe8 / pObject->M + pObject->omega.v[0]);
    }
}

// FUNCTION: CARMA2_HW 0x004d9ea0
void C2_HOOK_FASTCALL InitPowerups(void) {

    {
        br_pixelmap* repulse;
        int j;
        br_actor** actor;

        repulse = LoadPixelmap("REPULSE.PIX");
        if (repulse == NULL) {
            FatalError(kFatalError_CantFindFile_S, "REPULSE.PIX");
        }
        BrMapAdd(repulse);

        C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gRepulse_links) != 6);
        C2_HOOK_BUG_ON(sizeof(tRepulse_link) != 0x28);

        for (actor = &gRepulse_links[0].actor; (int)actor < (int)&gRepulse_links[6].actor; actor += sizeof(tRepulse_link) / sizeof(*actor)) {
            tRepulse_link* link = (tRepulse_link*)((tU8*)actor - offsetof(tRepulse_link, actor));
            br_model* model;
            br_material* material;
            br_face* face;
            br_scalar* map_v;

            *actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
            model = BrModelAllocate(NULL, 102, 100);
            model->flags |= BR_MODF_UPDATEABLE;
            material = BrMaterialAllocate(gEmpty_string_00676920);
            material->colour_map = repulse;
            material->flags &= ~BR_MATF_LIGHT;
            material->flags |= BR_MATF_TWO_SIDED;
            BrMaterialAdd(material);
            BlendifyMaterial(material, 100);
            BrMaterialUpdate(material, BR_MATU_ALL);
            face = model->faces;
            for (j = 0; j < model->nfaces / 2; j++, face++) {
                face->material = material;
                face->vertices[0] = j + 0;
                face->vertices[1] = j + 1;
                face->vertices[2] = j + 1 + model->nvertices / 2;
            }
            face = &model->faces[model->nfaces / 2];
            for (j = model->nfaces / 2; j < model->nfaces; j++, face++) {
                face->material = material;
                face->vertices[0] = j + 1;
                face->vertices[1] = j + 2;
                face->vertices[2] = j + 1 - model->nvertices / 2;
            }
            map_v = &model->vertices[0].map.v[1];
            for (j = 0; j < model->nvertices / 2; j++, map_v += sizeof(br_vertex) / sizeof(*map_v)) {
                map_v[0] = 0.f;
                map_v[-1] = (float)(j & 1);
            }
            // The original starts this strip at nfaces/2, not nvertices/2.
            j = model->nvertices / 2;
            map_v = &model->vertices[model->nfaces / 2].map.v[1];
            for (; j < model->nvertices; j++, map_v += sizeof(br_vertex) / sizeof(*map_v)) {
                map_v[0] = 1.f;
                map_v[-1] = (float)(j & 1);
            }
            link->actor->model = model;
            link->actor->render_style = BR_RSTYLE_FACES;
            link->material = material;
            BrModelAdd(model);
        }
    }
    {
        int i;
        tPhysics_object* parent_collision_info;
        tPath_name the_path;
        tTWTVFS twt;

        parent_collision_info = NULL;
        PathCat(the_path, gApplication_path, "TAIL");
        twt = OpenPackFileAndSetTiffLoading(the_path);
        LoadAllStuffInDirectory(&gMisc_storage_space, the_path, kRendererShadingType_Specular);
        ClosePackFileAndSetTiffLoading(twt);
        for (i = 0; i < gCount_mutant_tail_parts; i++) {
            int is_ball;
            tPhysics_object* collision_info;
            br_model* model;

            is_ball = i == gCount_mutant_tail_parts - 1;
            if (is_ball) {
                model = BrModelFind("TAILBALL");
                collision_info = MungeSphereObject(model CARPOCALYPSE2_THISCALL_EDX, gMass_mutant_tail_ball);
            } else {
                model = BrModelFind("TAILLINK");
                collision_info = MungeBoxObject(model CARPOCALYPSE2_THISCALL_EDX, gMass_mutant_tail_link);
            }

            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, physics_joint1, 0x188);

            collision_info->flags_0x238 = 0x20;
            collision_info->physics_joint1 = AllocatePhysicsJoint(1, kMem_physics_joint);
            collision_info->physics_joint1->type = eJoint_ball_n_socket;
            BrVector3SetFloat(&collision_info->physics_joint1->field_0x08,
                            0.f, 0.f, (float)(model->bounds.min.v[2] * (is_ball ? gDOUBLE_0058abf0 : gDOUBLE_0058abf8)));
            if (i != 0) {
                BrVector3SetFloat(&collision_info->physics_joint1->field_0x14,
                                0.f, 0.f, (float)(model->bounds.max.v[2] * (is_ball ? gDOUBLE_0058abf0 : gDOUBLE_0058abf8)));
                PhysicsAddObject(parent_collision_info, collision_info);
            } else {
                gMutant_tail_first_collision_info = collision_info;
                gMutant_tail_first_joint = collision_info->physics_joint1;

                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, type, 0x00);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, friction, 0x04);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, field_0x08, 0x08);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, hinge_axis2, 0x20);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, hinge_axis3, 0x2c);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, hinge_axis, 0x38);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, count_limits, 0x50);
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_joint, limits, 0x54);

                collision_info->physics_joint1->count_limits = 1;
                BrVector3SetFloat(&collision_info->physics_joint1->limits[0].child, 0.f, 0.f, 1.f);
                BrVector3SetFloat(&collision_info->physics_joint1->limits[0].parent, 0.f, 0.f, 1.f);
            }
            parent_collision_info = collision_info;
            BrVector3SetFloat(&collision_info->physics_joint1->hinge_axis2, 0.f, 0.f, 1.f);
            BrVector3SetFloat(&collision_info->physics_joint1->hinge_axis3, 0.f, 1.f, 0.f);
            BrVector3SetFloat(&collision_info->physics_joint1->hinge_axis, 1.f, 0.f, 0.f);
            BrMatrix34Translate(&collision_info->transform_matrix, 0.f, 0.f, 0.f);
            BrMatrix34Translate(&collision_info->actor->t.t.mat, 0.f, 0.f, 0.f);
        }
        gMutant_tail_state = 0;
    }
    InitMineShit();
}

// C2PORT-NONMATCHING: best 96.30% | difftest PASS 2000 trials (spec 004e06a0_RemoveTail.json) | reason VC5 loop null-check ordering
// FUNCTION: CARMA2_HW 0x004e06a0
void C2_HOOK_FASTCALL RemoveTail(void) {
    tPhysics_object* node;

    if (gMutant_tail_state == 0) {
        return;
    }
    node = gMutant_tail_first_collision_info;
    while (node != NULL) {
        BrActorRemove(node->actor);
        node = node->child;
    }
    if (gMutant_tail_state == 2) {
        PHILRemoveFromParentChain(gMutant_tail_first_collision_info);
    } else {
        PHILRemoveObject(gMutant_tail_first_collision_info);
    }
    gMutant_tail_first_collision_info->physics_joint1 = gMutant_tail_first_joint;
    gMutant_tail_state = 0;
}

// C2PORT-NONMATCHING: best 32.50% | difftest PASS 2000 trials (spec difftest/specs/004da730_CloseDownPowerUps.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x004da730
void C2_HOOK_FASTCALL CloseDownPowerUps(void) {
    int i;
    int j;
    tLose_proc** lose_proc;
    tPowerup* powerup;
    tNet_message* message;

    C2_HOOK_BUG_ON(sizeof(tPowerup) != 0xac);

    if (gNumber_of_powerups <= 0) {
        return;
    }
    i = 0;
    lose_proc = &gPowerup_array[0].lose_proc;
    do {
        powerup = (tPowerup*)((tU8*)lose_proc - offsetof(tPowerup, lose_proc));
        if (powerup->got_time != 0) {
            if (*lose_proc != NULL) {
                (*lose_proc)(powerup, powerup->car);
            }
            if (powerup->net_type & 0x80) {
                ARPowerupLose(powerup, powerup->car);
            }
            if (gNet_mode != eNet_mode_none) {
                message = NetBuildGuaranteedMessage(0x16, 0);
                *(int*)((tU8*)message + 0x28) = 2;
                *(tPlayer_ID*)((tU8*)message + 0x1c) = gLocal_net_ID;
                *(int*)((tU8*)message + 0x20) = i;
                NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
            }
            gPowerup_net_state[i] = 0;
            if (powerup->initial_value == 0) {
                for (j = 0; j < gNumber_of_icons; j++) {
                    if (gPickedup_powerups[j].powerup == powerup) {
                        gPickedup_powerups[j].fizzle_stage = 3;
                        gPickedup_powerups[j].fizzle_direction = -1;
                        gPickedup_powerups[j].fizzle_start = GetTotalTime();
                        break;
                    }
                }
            }
            powerup->got_time = 0;
        }
        i++;
        lose_proc += sizeof(tPowerup) / sizeof(*lose_proc);
    } while (i < gNumber_of_powerups);
}

// FUNCTION: CARMA2_HW 0x004def00
void C2_HOOK_FASTCALL CloseDownPowerUpsForCar(tCar_spec* pCar) {
    int powerup_address;
    int message_address;
    tU32 total_time;
    int* active_powerup;
    int* picked_up_powerup;
    int powerup_offset;
    int i;
    int j;

    if (((pCar != NULL) && (pCar->driver == eDriver_local_human)) && (i = 0, 0 < gNumber_of_powerups)) {
        powerup_offset = 0;
        active_powerup = pCar->powerups;
        do {
            if ((*active_powerup != 0) && (powerup_address = powerup_offset + (int)gPowerup_array, (((tPowerup*)powerup_address)->net_type & 1) == 0)) {
                if (((tPowerup*)powerup_address)->got_time != 0 && ((tPowerup*)powerup_address)->lose_proc != NULL) {
                    ((tPowerup*)powerup_address)->lose_proc((tPowerup*)powerup_address, ((tPowerup*)powerup_address)->car);
                }
                if (((tPowerup*)powerup_address)->net_type & 0x80) {
                    ARPowerupLose((tPowerup*)powerup_address, ((tPowerup*)powerup_address)->car);
                }
                if (gNet_mode != 0) {
                    message_address = (int)NetBuildGuaranteedMessage(0x16, 0);
                    *(int*)(message_address + 0x28) = 2;
                    *(tPlayer_ID*)(message_address + 0x1c) = gLocal_net_ID;
                    *(int*)(message_address + 0x20) = (powerup_address - (int)gPowerup_array) / 0xac;
                    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, (tNet_message*)message_address, 0);
                }
                gPowerup_net_state[(powerup_address - (int)gPowerup_array) / 0xac] = 0;
                if (((tPowerup*)powerup_address)->initial_value == 0 && (j = 0, 0 < gNumber_of_icons)) {
                    picked_up_powerup = (int*)&gPickedup_powerups[0].powerup;
                    while (j < gNumber_of_icons) {
                        if (powerup_address == *picked_up_powerup) {
                            gPickedup_powerups[j].fizzle_stage = 3;
                            gPickedup_powerups[j].fizzle_direction = -1;
                            total_time = GetTotalTime();
                            gPickedup_powerups[j].fizzle_start = total_time;
                            break;
                        }
                        j++;
                        picked_up_powerup += 5;
                    }
                }
                ((tPowerup*)powerup_address)->got_time = 0;
            }
            i++;
            active_powerup++;
            powerup_offset += 0xac;
        } while (i < gNumber_of_powerups);
    }
}

// FUNCTION: CARMA2_HW 0x004df070
void C2_HOOK_FASTCALL LoseCarPowerups(tCar_spec* pCar) {
    if (pCar == NULL || pCar->driver != eDriver_local_human) {
        return;
    }
    C2_HOOK_BUG_ON(sizeof(tPowerup) != 0xac);
    {
        int i;

        i = 0;
        if (gNumber_of_powerups > 0) {
            int j;
            int powerup_offset;
            int* car_powerup;
            tPowerup* powerup;
            tNet_message* message;

            powerup_offset = 0;
            car_powerup = pCar->powerups;
            do {
                if (*car_powerup != 0) {
                    powerup = (tPowerup*)((tU8*)gPowerup_array + powerup_offset);
                    if (powerup->net_type & eNet_powerup_off_before_snapweld) {
                        if (powerup->got_time != 0 && powerup->lose_proc != NULL) {
                            powerup->lose_proc(powerup, powerup->car);
                        }
                        if (powerup->net_type & 0x80) {
                            ARPowerupLose(powerup, powerup->car);
                        }
                        if (gNet_mode != eNet_mode_none) {
                            message = NetBuildGuaranteedMessage(0x16, 0);
                            *(int*)((tU8*)message + 0x28) = 2;
                            *(int*)((tU8*)message + 0x1c) = gLocal_net_ID;
                            *(int*)((tU8*)message + 0x20) = powerup - gPowerup_array;
                            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
                        }
                        gPowerup_net_state[powerup - gPowerup_array] = 0;
                        if (powerup->initial_value == 0) {
                            for (j = 0; j < gNumber_of_icons; j++) {
                                if (powerup == gPickedup_powerups[j].powerup) {
                                    gPickedup_powerups[j].fizzle_stage = 3;
                                    gPickedup_powerups[j].fizzle_direction = -1;
                                    gPickedup_powerups[j].fizzle_start = GetTotalTime();
                                    break;
                                }
                            }
                        }
                        powerup->got_time = 0;
                    }
                }
                i++;
                car_powerup++;
                powerup_offset += sizeof(tPowerup);
            } while (i < gNumber_of_powerups);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004da870
void C2_HOOK_FASTCALL AddRepulseLink(tU32 pTime, tCar_spec* pCar, undefined4 pField_0x24, br_vector3* pOrigin, br_vector3* pVictim, float pField_0x18, float pField_0x1c) {
    tRepulse_link* link;
    tU32 link_time;
    tU32 min_time;
    int index;
    int best;
    int i;

    min_time = 0x7fffffff;
    best = (int)pOrigin;
    index = -1;
    i = 0;
    link = &gRepulse_links[0];
    while ((int)link < (int)&gRepulse_links[6]) {
        link_time = link->time;
        if (link_time == 0) {
            index = i;
            break;
        }
        if (link_time < min_time) {
            min_time = link_time;
            best = i;
        }
        link++;
        i++;
    }
    if (index < 0) {
        index = best;
    }
    gRepulse_links[index].pos_origin = pOrigin;
    gRepulse_links[index].pos_victim = pVictim;
    gRepulse_links[index].time = pTime;
    gRepulse_links[index].field_0x10 = 0xffffffff;
    gRepulse_links[index].field_0x18 = pField_0x18;
    gRepulse_links[index].field_0x1c = pField_0x1c;
    gRepulse_links[index].car = pCar;
    gRepulse_links[index].field_0x24 = pField_0x24;
}

// FUNCTION: CARMA2_HW 0x004da910
void C2_HOOK_FASTCALL RemoveRepulseLinksForCar(tCar_spec* pCar) {
    char* link;
    br_actor* actor;

    link = (char*)gRepulse_links;
    do {
        if (*(int*)link != 0 && (*(tCar_spec**)(link + offsetof(tRepulse_link, car)) == pCar || *(undefined4*)(link + offsetof(tRepulse_link, field_0x24)) == (undefined4)pCar)) {
            *(int*)link = 0;
            actor = *(br_actor**)(link + offsetof(tRepulse_link, actor));
            if (actor->parent == gNon_track_actor) {
                BrActorRemove(actor);
            }
        }
        link += sizeof(tRepulse_link);
    } while ((int)link < (int)&gRepulse_links[6]);
}

// FUNCTION: CARMA2_HW 0x004efa00
void C2_HOOK_FASTCALL ReadPowerupSmashables(FILE* pF) {
    int i;
    int d1, d2;

    memset(&gPowerup_pickup_smashable, 0, sizeof(tSmashable_item_spec));
    gPowerup_pickup_smashable.mode_data.shrapnel.connotations.count_sounds = GetAnInt(pF);
    for (i = 0; i < gPowerup_pickup_smashable.mode_data.shrapnel.connotations.count_sounds; i++) {
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.sounds[i] = LoadSingleSound(&gTrack_storage_space, GetAnInt(pF));
    }
    ReadShrapnelSpec(pF, gPowerup_pickup_smashable.mode_data.shrapnel.connotations.shrapnel, &gPowerup_pickup_smashable.mode_data.shrapnel.connotations.count_shrapnel);
    ReadExplosion(pF, &gPowerup_pickup_smashable.mode_data.shrapnel.connotations.special_effects.explosion_animation);
    ReadSpillData(pF, &gPowerup_pickup_smashable.mode_data.shrapnel.connotations.special_effects.slick);
    ReadNonCarCuboidActivation(pF, &gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.activations);
    ReadShrapnelSideEffects(pF, &gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.side_effects);
    gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.extension_flags = GetAnInt(pF);
    if (gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.extension_flags & 1) {
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.extension_arg = GetAnInt(pF);
    }
    gPowerup_pickup_smashable.mode_data.shrapnel.connotations.side_effects.room_turn_on_code = GetAnInt(pF);
    gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.frequency = GetALineAndInterpretCommand(pF, gRepeatability_names, CARPOCALYPSE2_ASIZE(gRepeatability_names));
    if (gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.frequency != kRepeatability_None) {
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.field_0x4 = GetAScalar(pF);
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.field_0x8 = GetAScalar(pF);
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.field_0xc = GetAnInt(pF);
        gPowerup_pickup_smashable.mode_data.shrapnel.connotations.award.field_0x10 = GetAnInt(pF);
    }
    gPowerup_pickup_smashable.mode_data.shrapnel.connotations.variable_changes.count = GetAnInt(pF);
    for (i = 0; i < gPowerup_pickup_smashable.mode_data.shrapnel.connotations.variable_changes.count; i++) {
        GetPairOfInts(pF, &d1, &d2);
        ((tSmash_run_time_variable_change*)((char*)&gPowerup_pickup_smashable + offsetof(tSmashable_item_spec, mode_data) + offsetof(tSmashable_item_spec_shrapnel, connotations.variable_changes) + 2))[i].field_0x0 = d2;
        ((tSmash_run_time_variable_change*)((char*)&gPowerup_pickup_smashable + offsetof(tSmashable_item_spec, mode_data) + offsetof(tSmashable_item_spec_shrapnel, connotations.variable_changes) + 2))[i].field_0x2 = d1;
    }
    gPowerup_pickup_smashable.trigger_type = kSmashableTrigger_Model | kSmashableTrigger_Number;
    gPowerup_pickup_smashable.mode = kSmashableMode_Remove;

    memset(&gPowerup_respawn_smashable, 0, sizeof(tSmashable_item_spec));
    gPowerup_respawn_smashable.mode_data.shrapnel.connotations.count_sounds = GetAnInt(pF);
    for (i = 0; i < gPowerup_respawn_smashable.mode_data.shrapnel.connotations.count_sounds; i++) {
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.sounds[i] = LoadSingleSound(&gTrack_storage_space, GetAnInt(pF));
    }
    ReadShrapnelSpec(pF, gPowerup_respawn_smashable.mode_data.shrapnel.connotations.shrapnel, &gPowerup_respawn_smashable.mode_data.shrapnel.connotations.count_shrapnel);
    ReadExplosion(pF, &gPowerup_respawn_smashable.mode_data.shrapnel.connotations.special_effects.explosion_animation);
    ReadSpillData(pF, &gPowerup_respawn_smashable.mode_data.shrapnel.connotations.special_effects.slick);
    ReadNonCarCuboidActivation(pF, &gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.activations);
    ReadShrapnelSideEffects(pF, &gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.side_effects);
    gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.extension_flags = GetAnInt(pF);
    if (gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.extension_flags & 1) {
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.extension_arg = GetAnInt(pF);
    }
    gPowerup_respawn_smashable.mode_data.shrapnel.connotations.side_effects.room_turn_on_code = GetAnInt(pF);
    gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.frequency = GetALineAndInterpretCommand(pF, gRepeatability_names, CARPOCALYPSE2_ASIZE(gRepeatability_names));
    if (gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.frequency != kRepeatability_None) {
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.field_0x4 = GetAScalar(pF);
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.field_0x8 = GetAScalar(pF);
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.field_0xc = GetAnInt(pF);
        gPowerup_respawn_smashable.mode_data.shrapnel.connotations.award.field_0x10 = GetAnInt(pF);
    }
    gPowerup_respawn_smashable.mode_data.shrapnel.connotations.variable_changes.count = GetAnInt(pF);
    for (i = 0; i < gPowerup_respawn_smashable.mode_data.shrapnel.connotations.variable_changes.count; i++) {
        GetPairOfInts(pF, &d1, &d2);
        ((tSmash_run_time_variable_change*)((char*)&gPowerup_respawn_smashable + offsetof(tSmashable_item_spec, mode_data) + offsetof(tSmashable_item_spec_shrapnel, connotations.variable_changes) + 2))[i].field_0x2 = d1;
        ((tSmash_run_time_variable_change*)((char*)&gPowerup_respawn_smashable + offsetof(tSmashable_item_spec, mode_data) + offsetof(tSmashable_item_spec_shrapnel, connotations.variable_changes) + 2))[i].field_0x0 = d2;
    }
    gPowerup_respawn_smashable.trigger_type = kSmashableTrigger_Model | kSmashableTrigger_Number;
    gPowerup_respawn_smashable.mode = kSmashableMode_Remove;

    memset(&gUnknown_smashable_006a3660, 0, sizeof(gUnknown_smashable_006a3660));
    gUnknown_smashable_006a3660.trigger_type = kSmashableTrigger_Model | kSmashableTrigger_Number;
    gUnknown_smashable_006a3660.mode = kSmashableMode_Remove;
}
// FUNCTION: CARMA2_HW 0x004da530
void C2_HOOK_FASTCALL InitMineShit(void) {
    tPath_name the_path;
    tTWTVFS twt;
    int i;

    PathCat(the_path, gApplication_path, "MINE");
    twt = OpenPackFileAndSetTiffLoading(the_path);
    LoadAllStuffInDirectory(&gMisc_storage_space, the_path, kRendererShadingType_Specular);
    ClosePackFileAndSetTiffLoading(twt);

    C2_HOOK_BUG_ON(sizeof(tShit_mine) != 56);
    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gShit_mines) != 20);
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gShit_mines); i++) {
        gShit_mines[i].flags = 4;
        gShit_mines[i].field_0x35 = 0;
        gShit_mines[i].collision_info = MungeSphereObject(BrModelFind("SHITMINE.ACT") CARPOCALYPSE2_THISCALL_EDX, gMass_mine);
        gShit_mines[i].collision_info->uid = i;
        gShit_mines[i].collision_info->flags_0x238 = 0x40;
        gShit_mines[i].collision_info->owner = &gShit_mines[i];
        BrMatrix34Translate(&gShit_mines[i].collision_info->transform_matrix, 0.f, 0.f, 0.f);
        BrMatrix34Translate(&gShit_mines[i].collision_info->actor->t.t.mat, 0.f, 0.f, 0.f);
    }
}

void C2_HOOK_FASTCALL FillInMine(tNet_message_chunk* pChunk, tShit_mine* pMine, int pArg3) {

#ifndef CARPOCALYPSE2_MATCHING
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk, mine_explode.mine_index, 0x2);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk, mine_explode.owner_index, 0x3);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk, mine_explode.powerup_index, 0x4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk, mine_explode.player_index, 0x5);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk, mine_explode.field_0x8, 0x8);
#endif

    pChunk->mine_explode.mine_index = pMine - gShit_mines;
    pChunk->mine_explode.owner_index = pMine->field_0x35;
    pChunk->mine_explode.powerup_index = pMine->field_0x34;
    pChunk->mine_explode.player_index = NetPlayerFromCar(pMine->field_0x30) - gNet_players;
    pChunk->mine_explode.field_0x8 = pArg3;
    GetHierarchyNetworkStuff(pMine->collision_info, pChunk->mine_explode.object_hierarchy, NET_OBJECT_HIERARCHY_DEFAULT_SIZE);
}

// FUNCTION: CARMA2_HW 0x004dd1b0
void C2_HOOK_FASTCALL MinePreCollisionStuff(void) {
    int i;

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gShit_mines); i++) {
        tPhysics_object* obj;

        if (!(gShit_mines[i].flags & 0x1)) {
            continue;
        }
        obj = gShit_mines[i].collision_info;
        if (obj->field_0x261 != 0x31 || obj->message_time != gPHIL_last_physics_tick) {
            continue;
        }
        obj->field_0xf0 = 1;
        SelectCrushNetwork(&obj->field_0x294, obj, 2);
        obj->field_0x261 = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004ddab0
int C2_HOOK_FASTCALL MineExplode(tShit_mine *pMine) {
    tNet_message* message;
    br_vector3 offset;
    int hierarchy_size;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message, contents, 0x18);

    if (pMine->flags & 0x2) {
        return 0;
    }
    pMine->flags |= 0x2;
    if (gNet_mode == eNet_mode_host) {
        hierarchy_size = GetHierarchyNetworkSize(pMine->collision_info);
        message = NetBuildGuaranteedMessage(49, hierarchy_size + 12);
        FillInMine(&message->contents, pMine, -1);
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, 0);
    }
    PHILRemoveObject(pMine->collision_info);
    pMine->next_think_time = GetTotalTime() + 1600;
    BrVector3Negate(&offset, &gExplosion_pix_animation_groups.groups[0].offset);
    Explode(&gExplosion_pix_animation_groups, pMine->collision_info->actor, &pMine->collision_info->actor->model->bounds, &offset);
    DRS3StartSound3D(gXXX_outlet, eSoundId_Explosion, &pMine->collision_info->actor->t.t.translate.t, &gZero_v__car, 1, 255, BR_FIXED_INT(1), BR_FIXED_INT(1));
    PipeSingleEndShitMine(pMine->collision_info->actor);
    if (pMine->collision_info->actor->parent != NULL) {
        BrActorRemove(pMine->collision_info->actor);
    }
    pMine->collision_info->actor->render_style = BR_RSTYLE_NONE;
    return 1;
}

int C2_HOOK_FASTCALL CarVulnerableByMine(tCar_spec* pCar) {

    if (pCar->field_0x4c8 > 1.f) {
        return 0;
    }
    if (gNet_mode != eNet_mode_none) {
        tNet_game_player_info* net_player = NetPlayerFromCar(pCar);
        if (net_player != NULL && net_player->field_0x80 != 0) {
            return 0;
        }
    }
    if (pCar->invulnerable_no_crushage) {
        return 0;
    }
    return 1;
}

// FUNCTION: CARMA2_HW 0x004ddc10
void C2_HOOK_FASTCALL FUN_004ddc10(unsigned int* pFlags) {

    *pFlags |= 0x20;
}

// FUNCTION: CARMA2_HW 0x004ddc20
void C2_HOOK_FASTCALL CollideShitMine(tPhysics_object* pObject1, tPhysics_object* pObject2, float pCollision_speed) {
    tShit_mine* mine;
    tPhysics_object* other_object;
    tCar_spec* car;
    void* other_owner;
    tNet_game_player_info* net_player;

    mine = NULL;
    other_object = NULL;
    if (gNet_mode == 3) {
        return;
    }
    if (pObject1 != NULL && pObject1->flags_0x238 == '@') {
        mine = (tShit_mine*)pObject1->owner;
        other_object = pObject2;
        if (mine->flags & 0x20) {
            return;
        }
    } else if (pObject2 != NULL && pObject2->flags_0x238 == '@') {
        mine = (tShit_mine*)pObject2->owner;
        other_object = pObject1;
        if (mine->flags & 0x20) {
            return;
        }
    }
    if (mine != NULL && other_object != NULL) {
        other_owner = other_object->owner;
        car = (tCar_spec*)other_owner;
        if (other_owner == NULL || other_object->flags_0x238 != 1) {
            car = NULL;
        }
        if (car == NULL || car->driver == 0 || (car->driver != 4 && car->driver != 2 && car->driver != 3)
                || (*(tU32*)((tU8*)car + 0x100) & 0x10000) == 0) {
            if (other_object->flags_0x238 == '@') {
                ((tShit_mine*)other_owner)->flags |= 0x20;
                return;
            }
            if (other_owner == NULL || other_object->flags_0x238 != 1
                    || ((((tCar_spec*)other_owner)->driver != 0
                            && (((tCar_spec*)other_owner)->driver == 2 || ((tCar_spec*)other_owner)->driver == 3))
                        || ((tCar_spec*)other_owner)->driver <= 5)) {
                FUN_004d9d00(other_object, 0);
                mine->flags |= 0x20;
                return;
            }
            if (car->field_0x4c8 <= gDOUBLE_0058ac18
                    && (gNet_mode == 0 || (net_player = NetPlayerFromCar(car)) == NULL
                        || (net_player = NetPlayerFromCar(car), net_player->field_0x80 == 0))) {
                mine->car = car;
                mine->flags |= 0x80;
                mine->max_damage = (float)(car->softness_factor * gDOUBLE_0058ac48);
                mine->flags |= 0x20;
                return;
            }
            mine->flags |= 0x20;
            return;
        }
    }
    if (pCollision_speed <= mine->field_0x1c || gPinball_factor != gFLOAT_0058ac0c) {
        if (pCollision_speed > gFLOAT_0058ac88) {
            DRS3StartSound3D(gCar_outlet, 0x151b, &mine->collision_info->actor->t.t.translate.t,
                    &gZero_v__car, 1, 0xff, 0x10000, 0x10000);
        }
        return;
    }
    mine->flags |= 0x20;
}

// FUNCTION: CARMA2_HW 0x004ddde0
void C2_HOOK_FASTCALL FUN_004ddde0(tCar_spec* pCar) {
    int* p;

    p = (int*)&gShit_mines[0].field_0x30;
    do {
        if ((*(tU8*)(p - 12) & 1) != 0) {
            if ((tCar_spec*)p[-2] == pCar) {
                p[-2] = 0;
            }
            if ((tCar_spec*)*p == pCar) {
                *p = 0;
                MineExplode((tShit_mine*)(p - 12));
            }
        }
        p += 14;
    } while ((int)p < (int)((tU8*)gShit_mines + sizeof(gShit_mines) + offsetof(tShit_mine, field_0x30)));
}

// C2PORT-NONMATCHING: best 51.03% | difftest PASS 2000 trials (spec difftest/specs/004dde20_ProcessShitMines.json) | reason regalloc-loop,x87-order
// FUNCTION: CARMA2_HW 0x004dde20
void C2_HOOK_FASTCALL ProcessShitMines(tU32 pTime) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gShit_mines) != 20);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gShit_mines); i++) {
        tShit_mine* mine = &gShit_mines[i];
        tU32 flags = mine->flags;

        if (flags & 0x1) {
        if (flags & 0x80) {
            tCar_spec *car = mine->car;

            if (car != NULL) {
                int car_vulnerable;

                flags &= ~0x80;
                mine->flags = flags;
                car_vulnerable = 1;
                if (car->field_0x4c8 > gDOUBLE_0058ac18) {
                    car_vulnerable = 0;
                } else if (gNet_mode != eNet_mode_none) {
                    tNet_game_player_info *net_player;

                    net_player = NetPlayerFromCar(car);
                    if (net_player != NULL) {
                        net_player = NetPlayerFromCar(car);
                        if (net_player->field_0x80 != 0) {
                            car_vulnerable = 0;
                        }
                    }
                }
                if (car->invulnerable_no_crushage) {
                    car_vulnerable = 0;
                }
                if (car_vulnerable) {
                    TotallySpamTheModel(car, mine->max_damage * gFLOAT_0058ac40);
                }
                if (car_vulnerable && (gNet_mode == eNet_mode_none || gNet_mode == eNet_mode_host)) {
                    car->collision_info->v.v[1] = BR_ADD(car->collision_info->v.v[1], FRandomBetween(mine->field3_0xc, mine->initial_y_speed_factor) / BR_MUL(car->collision_info->M, gDouble_0058abe0));
                    car->collision_info->omega.v[2] = BR_ADD(car->collision_info->omega.v[2], BR_MUL(FRandomPosNeg(mine->initial_z_omega_factor), gDouble_0058abe8) / car->collision_info->M);
                    car->collision_info->omega.v[0] = BR_ADD(car->collision_info->omega.v[0], BR_MUL(FRandomPosNeg(mine->initial_x_omega_factor), gDouble_0058abe8) / car->collision_info->M);
                    car->collision_info->disable_move_rotate = 0;

                    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x49c, 0x49c);
                    car->collision_info->field_0x49c = 40 + gPHIL_last_physics_tick;
                    if (!car->invulnerable_no_damage) {
                        int j;

                        C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(car->damage_units) != 12);
                        for (j = 0; j < CARPOCALYPSE2_ASIZE(car->damage_units); j++) {
                            DoDamage(car, j, IRandomBetween(0, (int)(25.f * mine->max_damage)));
                        }
                        if (car != mine->field_0x30) {
                            car->last_hit_by = mine->field_0x30;
                            car->time_last_hit = PDGetTotalTime();
                            CrashEarnings(car, NULL);
                        }
                    }
                }
            }
        }
        flags = mine->flags;
        if (flags & 0x20) {
            flags &= ~0x20;
            mine->flags = flags;
            MineExplode(mine);
        } else if (flags & 0x2) {
            if (mine->next_think_time < pTime) {
                flags &= ~0x3;
                mine->flags = flags;
            }
        } else {
            if ((flags & 0x4) && mine->next_think_time < pTime) {
                MineExplode(mine);
            }
        }
        }
    }
}

// FUNCTION: CARMA2_HW 0x00502e00
void C2_HOOK_FASTCALL MaxOutAPO(void) {
    int i;

    for (i = 0; i < 3; i++) {
        gProgram_state.current_car.power_up_levels[i] = gInitial_powerup_slots[i];
        gProgram_state.current_car.power_up_slots[i] = gInitial_powerup_slots[i];
    }
}

// FUNCTION: CARMA2_HW 0x00502e20
void C2_HOOK_FASTCALL IncreaseAPOLevel(tCar_spec* pCar, int pIndex) {

    pCar->power_up_levels[pIndex]++;
    NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(pIndex + 0xaf));
}

void C2_HOOK_FASTCALL CheckRespawnQueue(tU32 pTime) {
    int i;

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gRespawn_powerups); i++) {
        tRespawn_powerup *respawn_powerup = &gRespawn_powerups[i];
        if (respawn_powerup->actor == NULL || (int)pTime < (int)respawn_powerup->respawn_time) {
            continue;
        }
        if (respawn_powerup->actor->render_style != BR_RSTYLE_FACES) {
            respawn_powerup->actor->render_style = BR_RSTYLE_FACES;
        }
        PipeSinglePowerupRespawn(respawn_powerup->actor, respawn_powerup->index);
        DoPowerupRespawnSmash(respawn_powerup->actor);
        respawn_powerup->actor = NULL;
    }
}

// FUNCTION: CARMA2_HW 0x004db880
void C2_HOOK_FASTCALL MungePowerupStuff(undefined4 pArg1) {
    tU32 time;

    time = GetTotalTime();
    MungeRepulseRays(time);
    CheckRespawnQueue(time);
}

tU32 C2_HOOK_FASTCALL GetNextGoodyTime(tU32 pTime) {

    return pTime + IRandomBetween(gNet_powerup_time_between_goodies_min, gNet_powerup_time_between_goodies_max);
}

void C2_HOOK_FASTCALL AutoGoody(tU32 pTime) {

    if (gNext_goody_time == 0) {
        gNext_goody_time = GetNextGoodyTime(pTime);
    } else if (gNet_mode != eNet_mode_none
            && !gRace_finished
            && !gProgram_state.current_car.knackered
            && gCurrent_net_game->options.random_car_choice
            && pTime > gNext_goody_time) {
        gNext_goody_time = GetNextGoodyTime(pTime);
        GotPowerupEarwig(&gProgram_state.current_car, gNet_powerup_goodies[IRandomBetween(0, gNet_count_powerup_goodies - 1)], 2, 0);
    }
}

void C2_HOOK_FASTCALL LosePowerup(tPowerup* pPowerup) {

    LosePowerupX(pPowerup, 1);
}

int C2_HOOK_FASTCALL GotPowerupXX(tCar_spec* pCar, int pArg2, int pArg3) {
    int value;

    value = 0;
    if (gNet_mode != eNet_mode_none) {
        if (pCar != NULL && pCar->driver ==  eDriver_local_human) {
            value = 1;
        }
    }
    return GotPowerupX(pCar, pArg2, value, pArg3, 0);
}

void C2_HOOK_FASTCALL MungeKeyboardPowerup(void) {
    tPowerup* powerup;

    if (gInventory_selected < 0) {
        return;
    }
    if (gCountdown) {
        return;
    }
    if (gRace_finished) {
        return;
    }
    if (gProgram_state.current_car.knackered) {
        return;
    }
    powerup = &gPowerup_array[gInventory[gInventory_selected]];
    if (gINT_006a0a5c) {
        gINT_006a0a5c = KeyIsDown(61);
    } else if (gINT_006a0a6c != KeyIsDown(61)) {
        gINT_006a0a6c = !gINT_006a0a6c && !gProgram_state.current_car.knackered;
        if (gINT_006a0a6c) {
            int result;

            result = GotPowerupXX(&gProgram_state.current_car, gInventory[gInventory_selected], 1);
            if (result >= 0 && powerup->initial_value < 0) {
                powerup->value += 1;
                if (powerup->value >= 0) {
                    KeyboardPowerupFinished(powerup, 1);
                }
            }
        } else {
            LosePowerup(powerup);
        }
    }
    if (powerup->initial_value > 0 && gINT_006a0a6c) {
        powerup->value -= gFrame_period;
        if (powerup->value <= 0) {
            KeyboardPowerupFinished(powerup, 1);
        }
    }
}

// The original keeps a cursor to lose_proc across callbacks.
#define PERIODIC_POWERUP ((tPowerup*)((tU8*)lose_proc - offsetof(tPowerup, lose_proc)))
// FUNCTION: CARMA2_HW 0x004db1c0
void C2_HOOK_FASTCALL DoPowerupPeriodics(tU32 pFrame_period) {
    tU32 the_time;
    int i;
    tPowerup* powerup;
    tLose_proc** lose_proc;

    the_time = GetTotalTime();
    AutoGoody(the_time);
    if (gNet_mode != eNet_mode_none) {
        for (i = 0; i < gCount_cloaked_cars; i++) {
            DoCamouflageThing(gCloaked_cars[i]);
        }
    }
    i = 0;
    if (gNumber_of_powerups > 0) {
        lose_proc = &gPowerup_array[0].lose_proc;
        do {
            if (PERIODIC_POWERUP->got_time != 0) {
                powerup = PERIODIC_POWERUP;
                if (PERIODIC_POWERUP->type == ePowerup_timed && the_time >= PERIODIC_POWERUP->lose_time && PERIODIC_POWERUP->initial_value == 0) {
                    if (*lose_proc != NULL) {
                        (*lose_proc)(powerup, PERIODIC_POWERUP->car);
                    }
                    if (PERIODIC_POWERUP->net_type & 0x80) {
                        ARPowerupLose(powerup, PERIODIC_POWERUP->car);
                    }
                    if (gNet_mode != eNet_mode_none) {
                        tNet_message* msg;

                        msg = NetBuildGuaranteedMessage(0x16, 0);
                        msg->contents.powerup.field_0x10 = 2;
                        msg->contents.powerup.player_id = gLocal_net_ID;
                        msg->contents.powerup.powerup_index = powerup - gPowerup_array;
                        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
                    }
                    gProgram_state.current_car.powerups[powerup - gPowerup_array] = 0;
                    if (PERIODIC_POWERUP->initial_value == 0) {
                        int j;

                        for (j = 0; j < gNumber_of_icons; j++) {
                            if (PERIODIC_POWERUP == gPickedup_powerups[j].powerup) {
                                gPickedup_powerups[j].fizzle_stage = 3;
                                gPickedup_powerups[j].fizzle_direction = -1;
                                gPickedup_powerups[j].fizzle_start = GetTotalTime();
                                break;
                            }
                        }
                    }
                    PERIODIC_POWERUP->got_time = 0;
                } else if (PERIODIC_POWERUP->current_value == 0 && PERIODIC_POWERUP->initial_value == 0) {
                    if (*lose_proc != NULL) {
                        (*lose_proc)(powerup, PERIODIC_POWERUP->car);
                    }
                    if (PERIODIC_POWERUP->net_type & 0x80) {
                        ARPowerupLose(powerup, PERIODIC_POWERUP->car);
                    }
                    if (gNet_mode != eNet_mode_none) {
                        tNet_message* msg;

                        msg = NetBuildGuaranteedMessage(0x16, 0);
                        msg->contents.powerup.field_0x10 = 2;
                        msg->contents.powerup.player_id = gLocal_net_ID;
                        msg->contents.powerup.powerup_index = powerup - gPowerup_array;
                        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
                    }
                    gProgram_state.current_car.powerups[powerup - gPowerup_array] = 0;
                    if (PERIODIC_POWERUP->initial_value == 0) {
                        int j;

                        for (j = 0; j < gNumber_of_icons; j++) {
                            if (PERIODIC_POWERUP == gPickedup_powerups[j].powerup) {
                                gPickedup_powerups[j].fizzle_stage = 3;
                                gPickedup_powerups[j].fizzle_direction = -1;
                                gPickedup_powerups[j].fizzle_start = GetTotalTime();
                                break;
                            }
                        }
                    }
                    PERIODIC_POWERUP->got_time = 0;
                } else if (PERIODIC_POWERUP->periodic_proc != NULL) {
                    PERIODIC_POWERUP->periodic_proc(powerup, pFrame_period);
                }
            }
            i++;
            lose_proc += sizeof(tPowerup) / sizeof(*lose_proc);
        } while (i < gNumber_of_powerups);
    }
    if (gInventory_selected < 0) {
        return;
    }
    if (gCountdown) {
        return;
    }
    if (gRace_finished) {
        return;
    }
    if (gProgram_state.current_car.knackered) {
        return;
    }
    powerup = &gPowerup_array[gInventory[gInventory_selected]];
    if (gINT_006a0a5c) {
        gINT_006a0a5c = KeyIsDown(61);
    } else if (KeyIsDown(61) != gINT_006a0a6c) {
        gINT_006a0a6c = !gINT_006a0a6c && !gProgram_state.current_car.knackered;
        if (gINT_006a0a6c) {
            int result;

            result = GotPowerupXX(&gProgram_state.current_car, gInventory[gInventory_selected], 1);
            if (result >= 0 && powerup->initial_value < 0) {
                powerup->value += 1;
                if (powerup->value >= 0) {
                    int j;

                    LosePowerupX(powerup, 1);
                    j = gInventory_selected;
                    gInventory_selected = -1;
                    gINT_006a0a5c = 1;
                    gInventory[j] = -1;
                    for (j = 0; j < (int)CARPOCALYPSE2_ASIZE(gInventory); j++) {
                        if (gInventory[j] >= 0) {
                            gInventory_selected = j;
                            break;
                        }
                    }
                }
            }
        } else {
            LosePowerup(powerup);
        }
    }
    if (powerup->initial_value > 0 && gINT_006a0a6c) {
        powerup->value -= gFrame_period;
        if (powerup->value <= 0) {
            KeyboardPowerupFinished(powerup, 1);
        }
    }
}
#undef PERIODIC_POWERUP

// FUNCTION: CARMA2_HW 0x004df650
void C2_HOOK_CDECL RenderSpinningPowerup(br_actor* actor, br_model* model, br_material* material, void* order_table, br_uint_8 style, int on_screen) {

    if (actor->user == NULL) {
        actor->user = (void*)(uintptr_t)GetTotalTime();
    } else {
        tU32 prev;
        tU32 now;

        now = GetTotalTime();
        prev = (tU32)(uintptr_t)actor->user;
        actor->user = (void*)(uintptr_t)now;
        BrMatrix34PreRotateY(&actor->t.t.mat, BR_ANGLE_DEG((now - prev) / 16));
    }
    BrZsModelRender(actor, model, material, order_table, style, on_screen, 0);
}

// C2PORT-NONMATCHING: best 40.7% | difftest PASS 2000 trials (spec 004df570_SetSpinningPowerup.json) | reason logic matches; VC5 loop and register allocation differ
// FUNCTION: CARMA2_HW 0x004df570
void C2_HOOK_FASTCALL SetSpinningPowerup(br_actor* pActor, int pOpacity) {
    // GLOBAL: CARMA2_HW 0x0065ebc0
    static br_token_value spinning_powerup_extra_prim[] = {
        { BRT_BLEND_B, { 1 } },
        { BRT_OPACITY_X, { BrScalarToFixed(75) } },
    };
    int i;

    if (pActor == NULL) {
        return;
    }
    BrMatrix34PreTranslate(&pActor->t.t.mat, 0.f, .1f, 0.f);
    pActor->model->flags |= BR_MODF_CUSTOM;
    pActor->model->custom = RenderSpinningPowerup;
    if (pOpacity == 0 || pOpacity == 255) {
        return;
    }
    if (pActor->material != NULL) {
        pActor->material->extra_prim = spinning_powerup_extra_prim;
        pActor->material->extra_prim[1].v.x = BrScalarToFixed(pOpacity);
        BrMaterialUpdate(pActor->material, BR_MATU_EXTRA_PRIM);
        return;
    }
    for (i = 0; i < pActor->model->nfaces; i++) {
        br_material* material;

        material = pActor->model->faces[i].material;
        if (material != NULL) {
            material->extra_prim = spinning_powerup_extra_prim;
            material->extra_prim[1].v.x = BrScalarToFixed(pOpacity);
            BrMaterialUpdate(material, BR_MATU_EXTRA_PRIM);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004dfe10
void C2_HOOK_CDECL RenderChangingPowerup(br_actor* actor, br_model* model, br_material* material, void* order_table, br_uint_8 style, int on_screen) {

    if (actor->user == NULL) {
        actor->user = (void*)(uintptr_t)GetTotalTime();
        actor->model = gModel_powerup_armor;
    } else {
        tU32 prev;
        tU32 now;
        tU32 diff;

        now = GetTotalTime();
        prev = (tU32)(uintptr_t)actor->user;
        diff = (prev - now) / 16;
        if (diff > 15) {
            if (actor->model == gModel_powerup_armor) {
                actor->model = gModel_powerup_power;
            } else if (actor->model == gModel_powerup_power) {
                actor->model = gModel_powerup_offence;
            } else {
                actor->model = gModel_powerup_armor;
            }
            actor->user = (void*)(uintptr_t)now;
        }
        BrMatrix34PreRotateY(&actor->t.t.mat, BR_ANGLE_DEG(diff + 12));
    }
    BrZsModelRender(actor, model, material, order_table, style, on_screen, 0);
}

br_model* C2_HOOK_FASTCALL DuplicateModel(br_model* pModel, const char* pName) {
    br_model* copy;

    copy = BrModelAllocate(pName, pModel->nvertices, pModel->nfaces);
    if (copy == NULL) {
        return copy;
    }
    BrVector3Copy(&copy->pivot, &pModel->pivot);
    copy->flags = pModel->flags;
    copy->user = pModel->user;
    copy->crease_angle = pModel->crease_angle;
    copy->radius = pModel->radius;
    copy->bounds = pModel->bounds;
    memcpy(copy->vertices, pModel->vertices, pModel->nvertices * sizeof(br_vertex));
    memcpy(copy->faces, pModel->faces, pModel->nfaces * sizeof(br_face));
    return copy;
}

// FUNCTION: CARMA2_HW 0x004df6c0
void C2_HOOK_FASTCALL SetChangingPowerup(br_actor* pActor) {

    if (pActor == NULL) {
        return;
    }
    if (gModel_powerup_armor == NULL || gModel_powerup_power == NULL || gModel_powerup_offence == NULL) {

        gModel_powerup_armor = DuplicateModel(BrModelFind("&68powerup1.ACT"), "PowArm");
        gModel_powerup_armor->flags |= BR_MODF_CUSTOM;
        gModel_powerup_armor->custom = RenderChangingPowerup;
        BrModelAdd(gModel_powerup_armor);

        gModel_powerup_power = DuplicateModel(BrModelFind("&70powerup1.ACT"), "PowPow");
        gModel_powerup_power->flags |= BR_MODF_CUSTOM;
        gModel_powerup_power->custom = RenderChangingPowerup;
        BrModelAdd(gModel_powerup_power);

        gModel_powerup_offence = DuplicateModel(BrModelFind("&69powerup1.ACT"), "PowOff");
        gModel_powerup_offence->flags |= BR_MODF_CUSTOM;
        gModel_powerup_offence->custom = RenderChangingPowerup;
        BrModelAdd(gModel_powerup_offence);
    }

    if (gModel_powerup_armor == NULL) {
        tPath_name path;
        char s[256];
        FILE* f;
        int i;
        br_actor* actor;

        PathCat(path, gApplication_path, "POWRSHIT.TXT");
        f = DRfopen(path, "rt");
        if (f == NULL) {
            sprintf(s, "Cant open %s", "POWRSHIT.TXT");
            PDFatalError(s);
        }
        for (i = 0; i < 67; i++) {
            GetAString(f, s);
        }
        actor = LoadActor(s);
        if (actor != NULL && actor->model != NULL) {
            gModel_powerup_armor = DuplicateModel(actor->model, "PowArm");
            gModel_powerup_armor->flags |= BR_MODF_CUSTOM;
            gModel_powerup_armor->custom = RenderChangingPowerup;
            BrModelAdd(gModel_powerup_armor);
            BrActorFree(actor);
        }
        PFfclose(f);
    }

    if (gModel_powerup_power == NULL) {
        tPath_name path;
        char s[256];
        FILE* f;
        int i;
        br_actor* actor;

        PathCat(path, gApplication_path, "POWRSHIT.TXT");
        f = DRfopen(path, "rt");
        if (f == NULL) {
            sprintf(s, "Cant open %s", "POWRSHIT.TXT");
            PDFatalError(s);
        }
        for (i = 0; i < 68; i++) {
            GetAString(f, s);
        }
        actor = LoadActor(s);
        if (actor != NULL && actor->model != NULL) {
            gModel_powerup_power = DuplicateModel(actor->model, "PowPow");
            gModel_powerup_power->flags |= BR_MODF_CUSTOM;
            gModel_powerup_power->custom = RenderChangingPowerup;
            BrModelAdd(gModel_powerup_power);
            BrActorFree(actor);
        }
        PFfclose(f);
    }

    if (gModel_powerup_offence == NULL) {
        tPath_name path;
        char s[256];
        FILE* f;
        int i;
        br_actor* actor;

        PathCat(path, gApplication_path, "POWRSHIT.TXT");
        f = DRfopen(path, "rt");
        if (f == NULL) {
            sprintf(s, "Cant open %s", "POWRSHIT.TXT");
            PDFatalError(s);
        }
        for (i = 0; i < 69; i++) {
            GetAString(f, s);
        }
        actor = LoadActor(s);
        if (actor != NULL && actor->model != NULL) {
            gModel_powerup_offence = DuplicateModel(actor->model, "PowOff");
            gModel_powerup_offence->flags |= BR_MODF_CUSTOM;
            gModel_powerup_offence->custom = RenderChangingPowerup;
            BrModelAdd(gModel_powerup_offence);
            BrActorFree(actor);
        }
        PFfclose(f);
    }
    BrMatrix34PreTranslate(&pActor->t.t.mat, .0f, .1f, .0f);
    pActor->model = gModel_powerup_armor;
    pActor->model->flags |= BR_MODF_CUSTOM;
    pActor->model->custom = RenderChangingPowerup;
}

// FUNCTION: CARMA2_HW 0x004d8d10
char* C2_HOOK_FASTCALL GetPowerupMessage(int pIndex) {

    return gPowerup_array[pIndex].message;
}

// FUNCTION: CARMA2_HW 0x004d8d30
void C2_HOOK_FASTCALL GotPowerup(tCar_spec* pCar, int pIndex) {

    GotPowerupEarwig(pCar, pIndex, 1, 1);
}

// FUNCTION: CARMA2_HW 0x004dc170
int C2_HOOK_FASTCALL GotCredits(tPowerup* powerup, tCar_spec* car) {
    int credits;
    char str[256];

    if (car != NULL && car->driver == eDriver_local_human) {
        credits = (IRandomBetween(powerup->integer_params[0], powerup->integer_params[1]) / 100) * 100;
        strcpy(str, powerup->message);
        strcat(str, " ");
        EarnCredits2(credits, str);
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc720
int C2_HOOK_FASTCALL SetPedSpeed(tPowerup* powerup, tCar_spec* car) {

    gPedestrian_speed_factor = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc750
int C2_HOOK_FASTCALL SetPedSize(tPowerup* powerup, tCar_spec* car) {
    br_vector3 scale;

    BrVector3Set(&scale, powerup->float_params[0], powerup->float_params[1], powerup->float_params[2]);
    ScalePeds(&scale);
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc7d0
int C2_HOOK_FASTCALL SetPedExplode(tPowerup* powerup, tCar_spec* car) {

    gExploding_pedestrians = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de7a0
int C2_HOOK_FASTCALL PickAtRandom(tPowerup* powerup, tCar_spec* car) {
    int idx;
    int pIndex;

    idx = IRandomBetween(0, powerup->number_of_integer_params - 1);
    pIndex = powerup->integer_params[idx];
    GotPowerupEarwig(car, pIndex, 1, 1);
    return -1;
}

// FUNCTION: CARMA2_HW 0x004dc830
int C2_HOOK_FASTCALL SetInvulnerability(tPowerup* powerup, tCar_spec* car) {

    car->invulnerable_no_crushage = 1;
    car->invulnerable_no_damage = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc8a0
int C2_HOOK_FASTCALL SetFreeRepairs(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gFree_repairs = 1;
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc990
int C2_HOOK_FASTCALL DoInstantRepair(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        TotallyRepairCar();
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc930
int C2_HOOK_FASTCALL FreezeTimer(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gFreeze_timer = !gFreeze_timer;
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dca30
int C2_HOOK_FASTCALL SetEngineFactor(tPowerup* powerup, tCar_spec* car) {
    if (powerup->initial_value > 0) {
        TriggerPedestrianReaction("Afterburner", 1);
    }
    car->field_0x4d4 = powerup->float_params[0];
    car->grip_multiplier = powerup->float_params[1];
    SetCarSuspGiveAndHeight(car, powerup->float_params[2], powerup->float_params[3],
        powerup->float_params[6], powerup->float_params[4], powerup->float_params[5]);
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcab0
int C2_HOOK_FASTCALL SetUnderwater(tPowerup* powerup, tCar_spec* car) {

    car->underwater_ability = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc470
int C2_HOOK_FASTCALL GotTimeOrPower(tPowerup* powerup, tCar_spec* car) {

    if (gNet_mode) {
        return GotPowerupX(car, gNet_powerup_time_replacement, 1, 1, 0);
    }
    if (car != NULL && car->driver == eDriver_local_human) {
        AwardTime(IRandomBetween(powerup->integer_params[0], powerup->integer_params[1]));
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcae0
int C2_HOOK_FASTCALL TrashBodywork(tPowerup* powerup, tCar_spec* car) {

    TotallySpamTheModel(car, (float)(*(float*)((unsigned char*)car + 0x18d0) * 0.1f * (10.0 / 7.0)));
    if (car != NULL && car->driver == eDriver_local_human) {
        DRS3StartSound2(gXXX_outlet, 0x1388, 1, 0xff, 0xff, -1, -1);
        DRS3StartSound2(gXXX_outlet, 0x1389, 1, 0xff, 0xff, -1, -1);
        DRS3StartSound2(gXXX_outlet, 0x138a, 1, 0xff, 0xff, -1, -1);
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcb90
int C2_HOOK_FASTCALL TakeDrugs(tPowerup* powerup, tCar_spec* car) {
    if (car != NULL && car->driver == eDriver_local_human) {
        if (gDrug_trip_data == NULL) {
            gDrug_trip_data = BrMemAllocate(0x400, 0xfd);
            memcpy(gDrug_trip_data, gRender_palette->pixels, 0x400);
        }
        gOn_drugs = 1;
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcca0
int C2_HOOK_FASTCALL SetOpponentsSpeed(tPowerup* powerup, tCar_spec* car) {
    int i;

    for (i = 0; i < gCurrent_race.number_of_racers && gCurrent_race.opponent_list[i].index != 0x1d; i++) {
    }
    if (i < gCurrent_race.number_of_racers) {
        SetEngineFactor(gPowerup_array + 5, gCurrent_race.opponent_list[i].car_spec);
    }
    gOpponent_speed_factor = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcd70
int C2_HOOK_FASTCALL SetCopsSpeed(tPowerup* powerup, tCar_spec* car) {

    gCop_speed_factor = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcda0
int C2_HOOK_FASTCALL SetGravity(tPowerup* powerup, tCar_spec* car) {

    gGravity_multiplier = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcdd0
int C2_HOOK_FASTCALL SetPinball(tPowerup* powerup, tCar_spec* car) {

    gPinball_factor = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dce00
int C2_HOOK_FASTCALL SetWallclimb(tPowerup* powerup, tCar_spec* car) {

    car->wall_climber_mode = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dce30
int C2_HOOK_FASTCALL SetBouncey(tPowerup* powerup, tCar_spec* car) {

    car->bounce_rate = gBounce_rate_const / powerup->float_params[0];
    car->bounce_amount = powerup->float_params[1];
    car->last_bounce = 0;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dce80
int C2_HOOK_FASTCALL SetSuspension(tPowerup* powerup, tCar_spec* car) {

    SetCarSuspGiveAndHeight(car,
                            ((volatile float*)powerup->float_params)[0],
                            ((volatile float*)powerup->float_params)[1],
                            ((volatile float*)powerup->float_params)[4],
                            ((volatile float*)powerup->float_params)[2],
                            ((volatile float*)powerup->float_params)[3]);
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dced0
int C2_HOOK_FASTCALL SetTyreGrip(tPowerup* powerup, tCar_spec* car) {

    car->grip_multiplier = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcf80
int C2_HOOK_FASTCALL SetDamageMultiplier(tPowerup* powerup, tCar_spec* car) {

    car->damage_multiplier = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcf00
int C2_HOOK_FASTCALL SetImmortalPeds(tPowerup* powerup, tCar_spec* car) {

    gImmortal_peds = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc8f0
int C2_HOOK_FASTCALL SetStupidPedestrians(tPowerup* powerup, tCar_spec* car) {

    gBlind_pedestrians = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dcf40
int C2_HOOK_FASTCALL SetSuicidalPedestrians(tPowerup* powerup, tCar_spec* car) {

    gPeds_suicidal = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de7d0
int C2_HOOK_FASTCALL GotVouchers(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        AddVouchers(*powerup->integer_params);
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004debb0
int C2_HOOK_FASTCALL SetMassMultiplier(tPowerup* powerup, tCar_spec* car) {

    car->field_0x4c8 = powerup->float_params[0];
    if (gNet_mode) {
        RecalculateCarMassMomentOfInertia(car);
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de820
int C2_HOOK_FASTCALL SetInstantHandbrake(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gInstant_handbrake = 1;
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dec10
int C2_HOOK_FASTCALL ShowPedestrians(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gShow_peds_on_map = 1;
    }
    return powerup - gPowerup_array;
}

// C2PORT-NONMATCHING: best 83.18% | difftest PASS 2000 trials (spec difftest/specs/004dea40_HitMine.json) | reason x87-codegen
// FUNCTION: CARMA2_HW 0x004dea40
int C2_HOOK_FASTCALL HitMine(tPowerup* powerup, tCar_spec* car) {
    int j;
    volatile float damage;
    int damage_limit;

    if (car->field_0x4c8 <= gDOUBLE_0058ac18) {
        if (gNet_mode != 0) {
            if (NetPlayerFromCar(car) != NULL && NetPlayerFromCar(car)->field_0x80 != 0) {
                goto hit_mine_done;
            }
        }

        car->collision_info->v.v[1] = (float)((double)FRandomBetween(powerup->float_params[0], powerup->float_params[1])
            / ((double)car->collision_info->M * gDouble_0058abe0) + car->collision_info->v.v[1]);
        car->collision_info->omega.v[2] = (float)((double)FRandomPosNeg(powerup->float_params[2]) * gDouble_0058abe8
            / car->collision_info->M + car->collision_info->omega.v[2]);
        car->collision_info->omega.v[0] = (float)((double)FRandomPosNeg(powerup->float_params[3]) * gDouble_0058abe8
            / car->collision_info->M + car->collision_info->omega.v[0]);

        if (car != NULL && car->driver > 5) {
            damage = (float)(car->softness_factor * gDOUBLE_0058ac48);
            if (!car->invulnerable_no_damage) {
                damage_limit = (int)(damage * gFLOAT_0058ac8c);
                for (j = 0; j < 12; j++) {
                    DamageUnit(car, j, IRandomBetween(0, damage_limit));
                }
            }
            if (!car->invulnerable_no_crushage) {
                TotallySpamTheModel(car, damage * gFLOAT_0058ac40);
            }
        }
        car->collision_info->disable_move_rotate = 0;
    }

hit_mine_done:
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dec60
int C2_HOOK_FASTCALL SetElectroBastard(tPowerup* powerup, tCar_spec* car) {

    car->proxy_ray_distance = powerup->float_params[0] * powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dec90
void C2_HOOK_FASTCALL ResetElectroBastard(tPowerup* powerup, tCar_spec* car) {

    car->proxy_ray_distance = 0.0f;
}

// FUNCTION: CARMA2_HW 0x004dc7a0
int C2_HOOK_FASTCALL SetPedHeadSize(tPowerup* powerup, tCar_spec* car) {
    br_scalar head_scale;

    head_scale = powerup->float_params[0];
    ScalePedHeads(head_scale);
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004deca0
int C2_HOOK_FASTCALL SetMutantCorpses(tPowerup* powerup, tCar_spec* car) {

    gMutant_speed = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dd3f0
void C2_HOOK_FASTCALL ConfigureShitMine(tShit_mine* pMine, tPowerup* pPowerup, tCar_spec* pCar) {

    pMine->flags = 5;
    pMine->flags = 5 + (pPowerup->integer_params[0] != 0 ? 0x40 : 0);
    pMine->next_think_time = GetTotalTime() - (tU32)(pPowerup->float_params[0] * gDOUBLE_0058ac60);
    pMine->field3_0xc = pPowerup->float_params[1];
    pMine->initial_y_speed_factor = pPowerup->float_params[2];
    pMine->initial_z_omega_factor = pPowerup->float_params[3];
    pMine->initial_x_omega_factor = pPowerup->float_params[4];
    pMine->field_0x1c = pPowerup->float_params[5];
    pMine->field_0x20 = pPowerup->float_params[6];
    pMine->field_0x24 = pPowerup->float_params[7];
    pMine->collision_info->field_0xed = 1;
    pMine->collision_info->disable_move_rotate = 0;
    pMine->collision_info->flags &= ~0x40;
    if (pMine->collision_info->actor->parent == NULL) {
        BrActorAdd(gNon_track_actor, pMine->collision_info->actor);
    }
    pMine->collision_info->actor->render_style = BR_RSTYLE_FACES;
    pMine->field_0x34 = pPowerup - gPowerup_array;
    PHILAddActiveObject(pMine->collision_info, NULL, NULL, NULL);
    PHILSetObjectProperty(pMine->collision_info, 0, 1.0);
    PHILSetObjectProperty(pMine->collision_info, 7, 1);
    PHILSetObjectProperty(pMine->collision_info, 6, 1);
    pMine->field_0x30 = pCar;
    pMine->collision_info->last_special_volume = pCar->collision_info->last_special_volume;
    pMine->collision_info->auto_special_volume = pCar->collision_info->auto_special_volume;
    pMine->collision_info->water_d = pCar->collision_info->water_d;
    pMine->collision_info->water_normal.v[0] = pCar->collision_info->water_normal.v[0];
    pMine->collision_info->water_normal.v[1] = pCar->collision_info->water_normal.v[1];
    pMine->collision_info->water_normal.v[2] = pCar->collision_info->water_normal.v[2];
}

// FUNCTION: CARMA2_HW 0x004dd590
void C2_HOOK_FASTCALL InitializeMineMotion(tShit_mine* pMine, tCar_spec* pCar) {
    br_matrix34 car_transform;
    br_vector3 impulse;

    if (pMine->flags & 0x40) {
        BrMatrix34Translate(&pMine->collision_info->actor->t.t.mat,
            0.f,
            (pCar->collision_info->bb1.max.v[1] - pCar->collision_info->bb1.min.v[1]) * gFLOAT_0058ac00,
            pCar->collision_info->bb1.min.v[2] - gDOUBLE_0058ac68);
    } else {
        BrMatrix34Translate(&pMine->collision_info->actor->t.t.mat,
            0.f,
            (pCar->collision_info->bb1.max.v[1] - pCar->collision_info->bb1.min.v[1]) * gFLOAT_0058ac00,
            pCar->collision_info->bb1.max.v[2] - gDOUBLE_0058ac70);
    }

    BrMatrix34Copy(&car_transform, &pCar->collision_info->transform_matrix);
    BrMatrix34Post(&pMine->collision_info->actor->t.t.mat, &car_transform);
    BrMatrix34Copy(&pMine->collision_info->transform_matrix, &pMine->collision_info->actor->t.t.mat);

    impulse.v[0] = 0.f;
    impulse.v[1] = pMine->field_0x20;
    impulse.v[2] = pMine->field_0x24;
    BrMatrix34ApplyV(&pMine->collision_info->v, &impulse, &car_transform);

    pMine->collision_info->v.v[0] = pCar->collision_info->v.v[0] + pMine->collision_info->v.v[0];
    pMine->collision_info->v.v[1] = pCar->collision_info->v.v[1] + pMine->collision_info->v.v[1];
    pMine->collision_info->v.v[2] = pCar->collision_info->v.v[2] + pMine->collision_info->v.v[2];
    if (pMine->flags & 0x40) {
        pMine->collision_info->v.v[0] = pCar->collision_info->v.v[0] + pMine->collision_info->v.v[0];
        pMine->collision_info->v.v[1] = pCar->collision_info->v.v[1] + pMine->collision_info->v.v[1];
        pMine->collision_info->v.v[2] = pCar->collision_info->v.v[2] + pMine->collision_info->v.v[2];
    }
    pMine->collision_info->omega.v[0] = 0.f;
    pMine->collision_info->omega.v[1] = 0.f;
    pMine->collision_info->omega.v[2] = 0.f;
}

// C2PORT-NONMATCHING: best 72.37% | difftest PASS 2000 trials (spec difftest/specs/004dd210_ReceiveMineUpdate.json) | reason VC5 register allocation and branch layout
// FUNCTION: CARMA2_HW 0x004dd210
void C2_HOOK_FASTCALL ReceiveMineUpdate(tNet_message_chunk_mine_explode* pChunk) {
    int mine_offset;
    tShit_mine* mine;
    tU32 mine_index;
    int player_index;
    tU32 hierarchy_size;
    tCar_spec* car;
    tPowerup* powerup;

    mine_index = pChunk->mine_index;
    player_index = pChunk->player_index;
    mine_offset = mine_index * sizeof(tShit_mine);
    mine = (tShit_mine*)((tU8*)gShit_mines + mine_offset);
    powerup = &gPowerup_array[pChunk->powerup_index];
    car = gNet_players[0].car;
    if (player_index >= 0 && player_index < gNumber_of_net_players) {
        car = gNet_players[player_index].car;
    }
    if (pChunk->field_0x8 == -1) {
        if (mine->field_0x35 != pChunk->owner_index
                && ((mine->flags & 0x2) || !(mine->flags & 0x1))) {
            return;
        }
        if (!(mine->flags & 0x1)) {
            ConfigureShitMine(mine, powerup, car);
        }
        SelectCrushNetwork((undefined4*)pChunk->object_hierarchy, mine->collision_info, 2);
        MineExplode(mine);
        return;
    }
    if (mine->flags & 0x1) {
        if (mine->field_0x35 == pChunk->owner_index) {
            goto set_owner;
        }
    }
    if (!(mine->flags & 0x1)) {
        ConfigureShitMine(mine, powerup, car);
    }
    if (player_index >= 0 && player_index < gNumber_of_net_players) {
        InitializeMineMotion(mine, mine->field_0x30);
    } else {
        BrMatrix34Translate(&mine->collision_info->transform_matrix, 1000.f, 1000.f, 1000.f);
        BrMatrix34Copy(&mine->collision_info->actor->t.t.mat,
            &mine->collision_info->transform_matrix);
    }
    if (mine->flags & 0x40) {
        DRS3StartSound3D(gXXX_outlet, 0x151d,
            &mine->collision_info->actor->t.t.translate.t, &gZero_v__car,
            1, 0xff, 0x10000, 0x10000);
    } else {
        DRS3StartSound3D(gXXX_outlet, 0x151c,
            &mine->collision_info->actor->t.t.translate.t, &gZero_v__car,
            1, 0xff, 0x10000, 0x10000);
    }
    if (mine->field_0x30 != NULL && mine->field_0x30->driver == eDriver_local_human) {
        TriggerPedestrianReaction("Mine_Shit", 0);
    }
set_owner:
    mine->field_0x35 = pChunk->owner_index;
    mine->collision_info->message_time = pChunk->field_0x8;
    mine->collision_info->field_0x261 = pChunk->header.type;
    mine->collision_info->disable_move_rotate = 0;
    hierarchy_size = pChunk->header.size - 0xc;
    if (hierarchy_size > 0x48) {
        hierarchy_size = 0x48;
    }
    memcpy(&mine->collision_info->field_0x294, pChunk->object_hierarchy, hierarchy_size);
}

// FUNCTION: CARMA2_HW 0x004dd700
int C2_HOOK_FASTCALL ShitMine(tPowerup* powerup, tCar_spec* car) {
    tShit_mine* mine = NULL;
    int i;
    int replacement;
    tU32 now;
    tU32 remaining;
    tU32 lifetime;
    br_scalar distance;
    br_vector3 displacement;
    br_vector3 normal;
    br_vector3 car_pos;
    br_vector3 mine_pos;
    br_vector3 hit_pos;
    br_scalar x, y, z;
    tPhysics_object* object1;
    tPhysics_object* object2;
    br_scalar length;
    br_scalar scale;

    if (gNet_mode == eNet_mode_client) {
        goto done;
    }
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gShit_mines); i++) {
        if (!(gShit_mines[i].flags & 1)) {
            mine = &gShit_mines[i];
            break;
        }
    }
    if (mine == NULL) {
        now = GetTotalTime();
        lifetime = (tU32)(powerup->float_params[0] * gDOUBLE_0058ac78);
        replacement = -1;
        for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gShit_mines); i++) {
            if ((gShit_mines[i].flags & 4) && !(gShit_mines[i].flags & 2)) {
                remaining = gShit_mines[i].next_think_time - now;
                if (remaining < lifetime) {
                    lifetime = remaining;
                    replacement = i;
                }
            }
        }
        if (replacement == -1) {
            goto done;
        }
        mine = &gShit_mines[replacement];
    }
    ConfigureShitMine(mine, powerup, car);
    mine->field_0x30 = car;
    mine->field_0x35++;
    InitializeMineMotion(mine, car);
    if (gNet_mode == eNet_mode_none) {
        PipeSingleMatrix34Type66(mine->collision_info->actor);
    }
    if (mine->flags & 0x40) {
        DRS3StartSound3D(gXXX_outlet, 0x151d, &mine->collision_info->actor->t.t.translate.t,
            &gZero_v__car, 1, 0xff, 0x10000, 0x10000);
    } else {
        DRS3StartSound3D(gXXX_outlet, 0x151c, &mine->collision_info->actor->t.t.translate.t,
            &gZero_v__car, 1, 0xff, 0x10000, 0x10000);
    }
    if (mine->field_0x30 != NULL && mine->field_0x30->driver == eDriver_local_human) {
        TriggerPedestrianReaction("Mine_Shit", 0);
    }
    BrMatrix34ApplyP(&car_pos, &car->collision_info->cmpos, &car->collision_info->transform_matrix);
    BrMatrix34ApplyP(&mine_pos, &mine->collision_info->cmpos, &mine->collision_info->transform_matrix);
    x = mine_pos.v[0] - car_pos.v[0];
    y = mine_pos.v[1] - car_pos.v[1];
    z = mine_pos.v[2] - car_pos.v[2];
    displacement.v[2] = z;
    length = sqrt(BR_SQR(x) + BR_SQR(y) + z * displacement.v[2]);
    if (length > gFLOAT_0058ac10) {
        scale = gDOUBLE_0058ac18 / length;
        normal.v[0] = x * scale;
        normal.v[1] = y * scale;
        normal.v[2] = displacement.v[2] * scale;
    } else {
        BrVector3Set(&normal, 1.f, 0.f, 0.f);
    }
    displacement.v[0] = x + normal.v[0] * gDOUBLE_0058ac68;
    displacement.v[1] = y + normal.v[1] * gDOUBLE_0058ac68;
    displacement.v[2] = displacement.v[2] + normal.v[2] * gDOUBLE_0058ac68;
    if (!TestForObjectInSensiblePlace(mine->collision_info, gList_collision_infos, &displacement, &gWorld_callbacks)) {
        object1 = mine->collision_info;
        object2 = gSensible_place_blocker;
    } else {
        if (FUN_004226d0(&car_pos, &displacement, &normal, &distance, &hit_pos,
                &gFace_list__car[car->collision_info->box_face_start],
                car->collision_info->box_face_end - car->collision_info->box_face_start) == NULL
            && FUN_004226d0(&car_pos, &displacement, &normal, &distance, &hit_pos,
                &gFace_list__car[mine->collision_info->box_face_start],
                mine->collision_info->box_face_end - mine->collision_info->box_face_start) == NULL) {
            goto done;
        }
        scale = -(gFLOAT_0058ac20 - distance) - gDOUBLE_0058ac80;
        x = displacement.v[0];
        y = displacement.v[1];
        z = displacement.v[2];
        displacement.v[0] = x * scale;
        displacement.v[1] = y * scale;
        displacement.v[2] = z * scale;
        BrVector3Accumulate((br_vector3*)mine->collision_info->transform_matrix.m[3], &displacement);
        object1 = car->collision_info;
        object2 = mine->collision_info;
    }
    CollideShitMine(object1, object2, 1.f);
done:
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de000
void C2_HOOK_FASTCALL FUN_004de000(br_actor* pActor, const br_vector3* pUnused) {

    if (pActor->parent == NULL) {
        BrActorAdd(gNon_track_actor, pActor);
    }
    pActor->render_style = BR_RSTYLE_FACES;
}

// FUNCTION: CARMA2_HW 0x004de020
void C2_HOOK_FASTCALL FUN_004de020(br_actor* pActor, const br_vector3* pUnused) {

    if (pActor->parent != NULL) {
        BrActorRemove(pActor);
    }
    pActor->render_style = BR_RSTYLE_NONE;
}

// FUNCTION: CARMA2_HW 0x004de040
int C2_HOOK_FASTCALL PissOutOil(tPowerup* powerup, tCar_spec* car) {

    TriggerPedestrianReaction("Oil_Slick", 0);
    QueueOilSpill2(
        NULL,
        car->car_master_actor,
        car->car_actor->model,
        0,
        0.35f,
        0.6f,
        9.0e-5f,
        3.0e-4f,
        0.2f,
        1.5f,
        2.5f,
        0.01f,
        0.15f,
        oily_material,
        0);
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de0c0
int C2_HOOK_FASTCALL KangerooJump(tPowerup* powerup, tCar_spec* car) {

    TriggerPedestrianReaction("Kangeroo", 0);
    PratcamEvent(0x2a);
    *(float*)((tU8*)car->collision_info + 0x6c) = powerup->float_params[0] * gConst_kangeroo_force_factor + *(float*)((tU8*)car->collision_info + 0x6c);
    if (car != NULL && car->driver == eDriver_local_human) {
        DRS3StartSound(gXXX_outlet, 0x2332);
    }
    *((tU8*)car->collision_info + 0xec) = 0;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de140
int C2_HOOK_FASTCALL AnnihilatePeds(tPowerup* powerup, tCar_spec* car) {
    int result;

    if (FindPedestrianNearCar(car, powerup->float_params[0]) != 0) {
        result = powerup - gPowerup_array;
    } else {
        result = -1;
    }
    if (result != -1) {
        TriggerPedestrianReaction("Ped_Ray", 0);
    }
    return result;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 63.05% | difftest PASS 2000 trials (spec difftest/specs/004de1f0_RepulseOpponents2.json) | reason x87-order, stack-layout
// FUNCTION: CARMA2_HW 0x004de1f0
static int C2_HOOK_FASTCALL RepulseOpponents2(tPowerup* powerup, tCar_spec* car, int pTell_net) {
    tU32 now;
    int found_count;
    int category;
    int car_count;
    int car_index;
    tCar_spec* target_car;
    float delta_x;
    float delta_y;
    float delta_z;
    float distance;
    volatile float falloff;
    float force;
    float random_range;
    volatile float random_x;
    volatile float random_y;
    volatile float random_z;
    tNet_message* message;
    volatile int stack_pad[6];

    stack_pad[5] = 0;
    found_count = 0;
    now = GetTotalTime();
    category = 0;
    do {
        if (category == 0) {
            car_count = 1;
        } else {
            car_count = GetCarCount((tVehicle_type)category);
        }
        car_index = 0;
        if (car_count > 0) {
            do {
                if (category == 0) {
                    target_car = &gProgram_state.current_car;
                } else {
                    target_car = GetCarSpec((tVehicle_type)category, car_index);
                }
                if (car != target_car) {
                    delta_y = target_car->pos.v[1] - car->pos.v[1];
                    delta_x = target_car->pos.v[0] - car->pos.v[0];
                    delta_z = target_car->pos.v[2] - car->pos.v[2];
                    distance = (float)sqrt(delta_y * delta_y + delta_x * delta_x + delta_z * delta_z);
                    if (distance < powerup->float_params[3]) {
                        found_count++;
                        if (gNet_mode != 3) {
                            falloff = ((powerup->float_params[3] - distance) / powerup->float_params[3]) / target_car->field_0x4c8;
                            if (distance <= gFLOAT_0058ac10) {
                                delta_y = 0.0f;
                                delta_z = 0.0f;
                                distance = gFLOAT_0058ac20;
                            } else {
                                distance = (float)(gDOUBLE_0058ac18 / distance);
                                delta_y *= distance;
                                delta_z *= distance;
                                distance *= delta_x;
                            }
                            if (fabs(delta_y) < gDOUBLE_0058ac80) {
                                delta_y = powerup->float_params[1];
                            }
                            force = falloff * powerup->float_params[0];
                            target_car->collision_info->v.v[0] = distance * force + target_car->collision_info->v.v[0];
                            target_car->collision_info->v.v[1] = delta_y * force + target_car->collision_info->v.v[1];
                            target_car->collision_info->v.v[2] = delta_z * force + target_car->collision_info->v.v[2];
                            random_range = powerup->float_params[2] * falloff;
                            random_x = FRandomPosNeg(random_range);
                            random_y = FRandomPosNeg(random_range);
                            random_z = FRandomPosNeg(random_range);
                            target_car->collision_info->omega.v[0] = random_x + target_car->collision_info->omega.v[0];
                            target_car->collision_info->omega.v[1] = random_y + target_car->collision_info->omega.v[1];
                            target_car->collision_info->omega.v[2] = random_z + target_car->collision_info->omega.v[2];
                            target_car->collision_info->disable_move_rotate = 0;
                            target_car->collision_info->field_0x49c = gPHIL_last_physics_tick + 0x28;
                            target_car->time_last_hit = PDGetTotalTime();
                            target_car->last_hit_by = car;
                        }
                        AddRepulseLink(now, car, (undefined4)target_car, &car->pos, &target_car->pos, 0.06f, 0.12f);
                    }
                }
                car_index++;
            } while (car_index < car_count);
        }
        category++;
    } while (category <= 4);
    if (gNet_mode != 0 && pTell_net != 0) {
        message = NetBuildGuaranteedMessage(0x23, 0);
        *(tPlayer_ID*)((tU8*)message + 0x1c) = gLocal_net_ID;
        *(int*)((tU8*)message + 0x20) = powerup - gPowerup_array;
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
    }
    if (found_count != 0) {
        if (car != NULL && car->driver == eDriver_local_human) {
            DRS3StartSound3D(gXXX_outlet, 0x1324, &car->pos, &gZero_v__car, 1, 0xff, 0x10000, 0x10000);
        }
        if (found_count != 0) {
            return powerup - gPowerup_array;
        }
    }
    return -1;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004de1e0
int C2_HOOK_FASTCALL RepulseOpponents(tPowerup* powerup, tCar_spec* car) {

    return RepulseOpponents2(powerup, car, 1);
}

// FUNCTION: CARMA2_HW 0x004dc800
int C2_HOOK_FASTCALL SetPedBrittle(tPowerup* powerup, tCar_spec* car) {

    gPed_dismemberfest = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de930
int C2_HOOK_FASTCALL SetGhostPeds(tPowerup* powerup, tCar_spec* car) {

    MakePedsEthereal();
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de870
int C2_HOOK_FASTCALL SetDancingPeds(tPowerup* powerup, tCar_spec* car) {

    gDancing_peds = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de8a0
int C2_HOOK_FASTCALL SetPanickingPeds(tPowerup* powerup, tCar_spec* car) {

    gPanicking_peds = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de8d0
int C2_HOOK_FASTCALL SetLowGravityPeds(tPowerup* powerup, tCar_spec* car) {

    gPed_gravity_multiplier = powerup->float_params[0];
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de900
int C2_HOOK_FASTCALL SetPissedPeds(tPowerup* powerup, tCar_spec* car) {

    gDrunk_pedestrians = 1;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de540
int C2_HOOK_FASTCALL RepulsePedestrians(tPowerup* powerup, tCar_spec* car) {
    int result;
    float* params;

    params = powerup->float_params;
    if (DoPedestrianRepulsifictor(car, params[0], params[2], params[1], params[3]) != 0) {
        result = powerup - gPowerup_array;
    } else {
        result = -1;
    }
    if (result != -1) {
        TriggerPedestrianReaction("Repulse", 0);
    }
    return result;
}

// FUNCTION: CARMA2_HW 0x004de5a0
int C2_HOOK_FASTCALL ReceivedRepulseOpponents(tNet_message_chunk_powerup* pMessage) {

    return RepulseOpponents2(&gPowerup_array[pMessage->powerup_index], NetCarFromPlayerID(pMessage->player_id), 0);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004dc3e0
static int C2_HOOK_FASTCALL GainAPO2(tCar_spec* car, int index, int amount) {
    int old;

    if (amount < 0) {
        int slots = car->power_up_slots[index];
        int levels = car->power_up_levels[index];
        if (levels >= slots) {
            return 0;
        }
        amount = slots - levels;
    }
    if (index == 1 && gNet_mode != eNet_mode_none
        && gCurrent_net_game->type == eNet_game_type_foxy
        && gThis_net_player_index == gIt_or_fox) {
        return 0;
    }
    old = car->power_up_levels[index];
    car->power_up_levels[index] = old + amount;
    if (car->power_up_levels[index] > car->power_up_slots[index]) {
        car->power_up_levels[index] = car->power_up_slots[index];
    }
    return car->power_up_levels[index] - old;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 46.04% | difftest PASS 2000 trials (spec difftest/specs/004dc240_GainAPO.json) | reason equivalent APO selection and updates use different register and branch layout
// FUNCTION: CARMA2_HW 0x004dc240
int C2_HOOK_FASTCALL GainAPO(tPowerup* powerup, tCar_spec* car) {
    int powerup_index;
    int old0, old1, old2;
    int* params;
    int a0, a1, a2;
    int idx;
    int amount;
    int orig_idx;
    int dir;

    powerup_index = powerup - gPowerup_array;
    old0 = car->power_up_levels[0];
    old1 = car->power_up_levels[1];
    old2 = car->power_up_levels[2];
    params = powerup->integer_params;
    a0 = params[0];
    a1 = params[1];
    a2 = params[2];
    if (a0 != 0 && a1 != 0 && a2 != 0) {
        GainAPO2(car, 0, a0);
        GainAPO2(car, 1, a1);
        GainAPO2(car, 2, a2);
    } else {
        if (a0 != 0) {
            idx = 0;
            amount = a0;
        } else if (a1 != 0) {
            idx = 1;
            amount = a1;
        } else {
            idx = 2;
            amount = a2;
        }
        if (GainAPO2(car, idx, amount) == 0) {
            orig_idx = idx;
            dir = PercentageChance(50) ? 1 : -1;
            idx += dir;
            if (idx < 0) {
                idx = 2;
            } else if (idx > 2) {
                idx = 0;
            }
            if (GainAPO2(car, idx, amount) == 0) {
                idx += dir;
                if (idx < 0) {
                    idx = 2;
                } else if (idx > 2) {
                    idx = 0;
                }
                GainAPO2(car, idx, amount);
            }
            powerup_index += idx - orig_idx;
        }
    }
    if (old0 == car->power_up_levels[0]
        && old1 == car->power_up_levels[1]
        && old2 == car->power_up_levels[2]) {
        if (gNet_mode != eNet_mode_none) {
            EarnCredits(gSubstitution_value_APO.initial_network[gCurrent_net_game->type]);
        } else {
            EarnCredits(gSubstitution_value_APO.initial[gProgram_state.skill_level]);
        }
        return -1;
    }
    return powerup_index;
}

// C2PORT-NONMATCHING: best 31.02% | difftest PASS 2000 trials (spec 004dc4d0_GainAPOPotential.json) | reason register allocation and statement scheduling
// FUNCTION: CARMA2_HW 0x004dc4d0
int C2_HOOK_FASTCALL GainAPOPotential(tPowerup* powerup, tCar_spec* car) {
    int powerup_index;
    int old0, old1, old2;
    int* params;
    int a0, a1, a2;
    int idx;
    int amount;
    int orig_idx;
    int dir;
    int old;
    int nw;

    powerup_index = powerup - gPowerup_array;
    old0 = car->power_up_slots[0];
    old1 = car->power_up_slots[1];
    old2 = car->power_up_slots[2];
    params = powerup->integer_params;
    a0 = params[0];
    a1 = params[1];
    a2 = params[2];
    if (a0 != 0 && a1 != 0 && a2 != 0) {
        nw = old0 + a0;
        car->power_up_slots[0] = nw;
        if (nw > gInitial_powerup_slots[0]) {
            car->power_up_slots[0] = gInitial_powerup_slots[0];
        }
        nw = car->power_up_slots[1] + a1;
        car->power_up_slots[1] = nw;
        if (nw > gInitial_powerup_slots[1]) {
            car->power_up_slots[1] = gInitial_powerup_slots[1];
        }
        nw = car->power_up_slots[2] + a2;
        car->power_up_slots[2] = nw;
        if (nw > gInitial_powerup_slots[2]) {
            car->power_up_slots[2] = gInitial_powerup_slots[2];
        }
    } else {
        if (a0 != 0) {
            idx = 0;
            amount = a0;
        } else if (a1 != 0) {
            idx = 1;
            amount = a1;
        } else {
            idx = 2;
            amount = a2;
        }
        old = car->power_up_slots[idx];
        nw = old + amount;
        car->power_up_slots[idx] = nw;
        if (nw > gInitial_powerup_slots[idx]) {
            car->power_up_slots[idx] = gInitial_powerup_slots[idx];
        }
        if (car->power_up_slots[idx] == old) {
            orig_idx = idx;
            dir = PercentageChance(50) ? 1 : -1;
            idx += dir;
            if (idx < 0) {
                idx = 2;
            } else if (idx > 2) {
                idx = 0;
            }
            old = car->power_up_slots[idx];
            nw = old + amount;
            car->power_up_slots[idx] = nw;
            if (nw > gInitial_powerup_slots[idx]) {
                car->power_up_slots[idx] = gInitial_powerup_slots[idx];
            }
            if (car->power_up_slots[idx] == old) {
                idx += dir;
                if (idx < 0) {
                    idx = 2;
                } else if (idx > 2) {
                    idx = 0;
                }
                old = car->power_up_slots[idx];
                nw = old + amount;
                car->power_up_slots[idx] = nw;
                if (nw > gInitial_powerup_slots[idx]) {
                    car->power_up_slots[idx] = gInitial_powerup_slots[idx];
                }
            }
            powerup_index += idx - orig_idx;
        }
    }
    if (old0 == car->power_up_slots[0]
        && old1 == car->power_up_slots[1]
        && old2 == car->power_up_slots[2]) {
        if (gNet_mode != eNet_mode_none) {
            EarnCredits(gPotential_substitution_value_APO.initial_network[gCurrent_net_game->type]);
        } else {
            EarnCredits(gPotential_substitution_value_APO.initial[gProgram_state.skill_level]);
        }
        return -1;
    }
    return powerup_index;
}

// FUNCTION: CARMA2_HW 0x004df200
void C2_HOOK_FASTCALL FUN_004df200(tPowerup* pPowerup, tCar_spec* pCar) {

    if (pPowerup->got_proc != NULL) {
        ((void (C2_HOOK_FASTCALL *)(tPowerup*, tCar_spec*))pPowerup->got_proc)(pPowerup, pCar);
    }
}

// FUNCTION: CARMA2_HW 0x004df210
void C2_HOOK_FASTCALL FUN_004df210(tPowerup* pPowerup, tCar_spec* pCar) {
    if (pPowerup->lose_proc != NULL) {
        pPowerup->lose_proc(pPowerup, pCar);
    }
}

// FUNCTION: CARMA2_HW 0x004df220
int C2_HOOK_FASTCALL SetPissed(tPowerup* powerup, tCar_spec* car) {
    if (car != NULL && car->driver == eDriver_local_human) {
        float* pOut;
        int k;

        DRS3StartSound2(gXXX_outlet, 0xe11, 1, 0xff, 0xff, -1, -1);
        gPissed_next_time = GetTotalTime() + IRandomBetween(0x5dc, 0xbb8);
        gCredit_multiplier = *powerup->integer_params;
        gPissed_hold_time = powerup->float_params[3];

        pOut = gPissed_flicker + 12;
        do {
            float* p = pOut;
            for (k = 1; k; k--) {
                float tmp;

                p[-12] = powerup->float_params[0];
                tmp = FRandomBetween(powerup->float_params[1], powerup->float_params[2]);
                p[0] = tmp;
                p[12] = FRandomBetween(0.0f, tmp);
                p += 1;
            }
            pOut += 3;
        } while ((int)pOut < (int)(gPissed_flicker + 15));
    }
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004de190
int C2_HOOK_FASTCALL NapalmPeds(tPowerup* powerup, tCar_spec* car) {
    int result;

    if (NapalmPedestrianNearCar(car) != 0) {
        result = powerup - gPowerup_array;
    } else {
        result = -1;
    }
    if (result != -1) {
        TriggerPedestrianReaction("Flamer", 0);
    }
    return result;
}

// FUNCTION: CARMA2_HW 0x004dfec0
int C2_HOOK_FASTCALL PedValium(tPowerup* powerup, tCar_spec* car) {

    if (CalmDownAllPeds() != 0) {
        NewTextHeadupSlot(4,0, 3000, -4, GetMiscString(eMiscString_pedestrians_are_calm_now));
        DRS3StartSound2(gXXX_outlet, eSoundId_PedValium, 1, 0xff, 0xff, -1, -1);
        return powerup - gPowerup_array;
    }
    return -1;
}

// FUNCTION: CARMA2_HW 0x004dff30
int C2_HOOK_FASTCALL PowerupCancel(tPowerup* powerup, tCar_spec* car) {
    int i;
    int j;
    tPowerup* p;
    tNet_message* msg;
    int any = 0;
    tPowerup* saved = powerup;

    if (car == NULL) {
        return -1;
    }
    if (car->driver != eDriver_local_human) {
        return -1;
    }
    for (i = 0, p = gPowerup_array; i < gNumber_of_powerups; i++, p++) {
        if (p->got_time == 0) {
            continue;
        }
        if (p->lose_proc != NULL) {
            p->lose_proc(p, p->car);
        }
        if (p->net_type & 0x80) {
            ARPowerupLose(p, p->car);
        }
        if (gNet_mode != eNet_mode_none) {
            msg = NetBuildGuaranteedMessage(0x16, 0);
            *(int*)((tU8*)msg + 0x28) = 2;
            *(tPlayer_ID*)((tU8*)msg + 0x1c) = gLocal_net_ID;
            *(int*)((tU8*)msg + 0x20) = p - gPowerup_array;
            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
        }
        gPowerup_net_state[p - gPowerup_array] = 0;
        if (p->initial_value == 0) {
            for (j = 0; j < gNumber_of_icons; j++) {
                if (gPickedup_powerups[j].powerup == p) {
                    gPickedup_powerups[j].fizzle_stage = 3;
                    gPickedup_powerups[j].fizzle_direction = -1;
                    gPickedup_powerups[j].fizzle_start = GetTotalTime();
                    break;
                }
            }
        }
        p->got_time = 0;
        any = 1;
    }
    if (gMutant_tail_state == 2) {
        for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gInventory); i++) {
            if (gInventory[i] == 0x5f) {
                gInventory[i] = -1;
            }
        }
        PHILRemoveFromParentChain(gMutant_tail_first_collision_info);
        gMutant_tail_first_collision_info->physics_joint1 = NULL;
        PHILAddActiveObject(gMutant_tail_first_collision_info, NULL, NULL, NULL);
        PHILSetObjectProperty(gMutant_tail_first_collision_info, 0, 1.0);
        PHILSetObjectProperty(gMutant_tail_first_collision_info, 1, 1);
        PHILSetObjectProperty(gMutant_tail_first_collision_info, 7, 1);
        PHILSetObjectProperty(gMutant_tail_first_collision_info, 6, 1);
        {
            tPowerup* tailp = &gPowerup_array[94];
            if (tailp->got_time != 0 && tailp->lose_proc != NULL) {
                tailp->lose_proc(tailp, tailp->car);
            }
            if (tailp->net_type & 0x80) {
                ARPowerupLose(tailp, tailp->car);
            }
            if (gNet_mode != eNet_mode_none) {
                msg = NetBuildGuaranteedMessage(0x16, 0);
                *(int*)((tU8*)msg + 0x28) = 2;
                *(tPlayer_ID*)((tU8*)msg + 0x1c) = gLocal_net_ID;
                *(int*)((tU8*)msg + 0x20) = tailp - gPowerup_array;
                NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
            }
            gPowerup_net_state[tailp - gPowerup_array] = 0;
            if (tailp->initial_value == 0) {
                for (j = 0; j < gNumber_of_icons; j++) {
                    if (gPickedup_powerups[j].powerup == tailp) {
                        gPickedup_powerups[j].fizzle_stage = 3;
                        gPickedup_powerups[j].fizzle_direction = -1;
                        gPickedup_powerups[j].fizzle_start = GetTotalTime();
                        break;
                    }
                }
            }
            tailp->got_time = 0;
        }
        gMutant_tail_state = 1;
        gINT_00705b70 = GetTotalTime();
        any = 1;
    }
    if (!any) {
        return -1;
    }
    NewTextHeadupSlot(4, 0, 0xbb8, -4, GetMiscString(0x113));
    DRS3StartSound2(gXXX_outlet, 0xeda, 1, 0xff, 0xff, -1, -1);
    return saved - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004e02b0
int C2_HOOK_FASTCALL GrowATail(tPowerup* powerup, tCar_spec* car) {
    tPhysics_object* node;
    tPhysics_joint* joint;
    br_matrix34 tmp;
    br_vector3 tmp_v;

    if (gMutant_tail_state == 2) {
        return -1;
    }
    if (gMutant_tail_state == 1) {
        PHILRemoveObject(gMutant_tail_first_collision_info);
        gMutant_tail_first_collision_info->physics_joint1 = gMutant_tail_first_joint;
    } else {
        PipeSingleSpecial(1);
    }
    BrVector3SetFloat(&gMutant_tail_first_collision_info->physics_joint1->field_0x14,
        0.0f,
        (car->collision_info->bb1.max.v[1] - car->collision_info->bb1.min.v[1]) * 0.5f,
        car->collision_info->bb1.max.v[2] * 0.95f);
    PhysicsAddObject(car->collision_info, gMutant_tail_first_collision_info);
    node = gMutant_tail_first_collision_info;
    while (node != NULL) {
        joint = node->physics_joint1;
        if (node->actor->parent == NULL) {
            BrActorAdd(gNon_track_actor, node->actor);
        }
        node->owner = (void*)car;
        BrMatrix34Translate(&node->actor->t.t.mat,
            -joint->field_0x08.v[0],
            -joint->field_0x08.v[1],
            -joint->field_0x08.v[2]);
        BrMatrix34Copy(&tmp, &car->collision_info->actor->t.t.mat);
        BrMatrix34Post(&node->actor->t.t.mat, &tmp);
        BrMatrix34Rotate(&tmp, (br_angle)0xf8d4, (br_vector3*)&node->actor->t.t.mat);
        BrMatrix34Post(&node->actor->t.t.mat, &tmp);
        BrMatrix34ApplyP(&tmp_v, &joint->field_0x14, &node->parent->actor->t.t.mat);
        BrMatrix34PostTranslate(&node->actor->t.t.mat, tmp_v.v[0], tmp_v.v[1], tmp_v.v[2]);
        BrMatrix34Copy(&node->transform_matrix, &node->actor->t.t.mat);
        node->field_0xed = 1;
        node = node->child;
    }
    GotPowerupEarwig(car, 0x5f, 0, 1);
    DRS3StartSound2(gXXX_outlet, 0xf3d, 1, 0xff, 0xff, -1, -1);
    gMutant_tail_state = 2;
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004e04c0
int C2_HOOK_FASTCALL CutOffTail(tPowerup* powerup, tCar_spec* car) {
    int i;
    tNet_message* msg;
    tPowerup* tailp;

    (void)car;
    if (gMutant_tail_state != 2) {
        return powerup - gPowerup_array;
    }
    PHILRemoveFromParentChain(gMutant_tail_first_collision_info);
    gMutant_tail_first_collision_info->physics_joint1 = NULL;
    PHILAddActiveObject(gMutant_tail_first_collision_info, NULL, NULL, NULL);
    PHILSetObjectProperty(gMutant_tail_first_collision_info, 0, 1.0);
    PHILSetObjectProperty(gMutant_tail_first_collision_info, 1, 1);
    PHILSetObjectProperty(gMutant_tail_first_collision_info, 7, 1);
    PHILSetObjectProperty(gMutant_tail_first_collision_info, 6, 1);
    tailp = &gPowerup_array[94];
    if (tailp->got_time != 0 && tailp->lose_proc != NULL) {
        tailp->lose_proc(tailp, tailp->car);
    }
    if (tailp->net_type & 0x80) {
        ARPowerupLose(tailp, tailp->car);
    }
    if (gNet_mode != eNet_mode_none) {
        msg = NetBuildGuaranteedMessage(0x16, 0);
        *(int*)((tU8*)msg + 0x28) = 2;
        *(tPlayer_ID*)((tU8*)msg + 0x1c) = gLocal_net_ID;
        *(int*)((tU8*)msg + 0x20) = tailp - gPowerup_array;
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
    }
    gPowerup_net_state[tailp - gPowerup_array] = 0;
    if (tailp->initial_value == 0) {
        for (i = 0; i < gNumber_of_icons; i++) {
            if (gPickedup_powerups[i].powerup == tailp) {
                gPickedup_powerups[i].fizzle_stage = 3;
                gPickedup_powerups[i].fizzle_direction = -1;
                gPickedup_powerups[i].fizzle_start = GetTotalTime();
                break;
            }
        }
    }
    tailp->got_time = 0;
    DRS3StartSound2(gXXX_outlet, 0xf3e, 1, 0xff, 0xff, -1, -1);
    gMutant_tail_state = 1;
    gINT_00705b70 = GetTotalTime();
    return powerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004dc9c0
void C2_HOOK_FASTCALL ResetPedSpeed(tPowerup* powerup, tCar_spec* car) {

    gPedestrian_speed_factor = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004dc9d0
void C2_HOOK_FASTCALL ResetPedSize(tPowerup* powerup, tCar_spec* car) {
    br_vector3 scale = { 1.0f, 1.0f, 1.0f };

    ScalePeds(&scale);
}

// FUNCTION: CARMA2_HW 0x004dca10
void C2_HOOK_FASTCALL ResetPedExplode(tPowerup* powerup, tCar_spec* car) {

    gExploding_pedestrians = 0;
}

// FUNCTION: CARMA2_HW 0x004dc860
void C2_HOOK_FASTCALL ResetInvulnerability(tPowerup* powerup, tCar_spec* car) {
    if (gNet_mode) {
        if (NetPlayerFromCar(car)->field_0x80 != 0) {
            return;
        }
    }
    *(int *)((tU8 *)car + 0x1b4) = 0;
    *(int *)((tU8 *)car + 0x1b8) = 0;
}

// FUNCTION: CARMA2_HW 0x004dc8d0
void C2_HOOK_FASTCALL ResetFreeRepairs(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gFree_repairs = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004dc970
void C2_HOOK_FASTCALL UnfreezeTimer(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gFreeze_timer = 0;
    }
}

void C2_HOOK_FASTCALL FUN_0045c760(void);

// FUNCTION: CARMA2_HW 0x004de5d0
void C2_HOOK_FASTCALL ResetEngineFactor(tPowerup* powerup, tCar_spec* car) {
    float f = 1.0f;

    FUN_0045c760();
    car->field_0x4d4 = f;
    car->grip_multiplier = f;
    ((void (C2_HOOK_FAKE_THISCALL *)(tCar_spec*, float, float, float, float, float))SetCarSuspGiveAndHeight)(car, f, f, f, 0.0f, 0.0f);
}

// FUNCTION: CARMA2_HW 0x004de600
void C2_HOOK_FASTCALL ResetUnderwater(tPowerup* powerup, tCar_spec* car) {

    car->underwater_ability = 0;
}

// C2PORT-NONMATCHING: best 40.00% | difftest PASS 2000 trials (spec difftest/specs/004de610_PukeDrugsBackUp.json) | reason palette-copy loop codegen
// FUNCTION: CARMA2_HW 0x004de610
void C2_HOOK_FASTCALL PukeDrugsBackUp(tPowerup* powerup, tCar_spec* car) {

    (void)powerup;
    if (car == NULL) {
        return;
    }
    if (car->driver != eDriver_local_human) {
        return;
    }
    gOn_drugs = 0;
    memcpy(gRender_palette->pixels, gDrug_trip_data, 0x400);
    PaletteFuckedUpByDrugs(gRender_palette, 0);
    ResetPalette();
}

// FUNCTION: CARMA2_HW 0x004de6b0
void C2_HOOK_FASTCALL ResetOpponentsSpeed(tPowerup* powerup, tCar_spec* car) {
    int count = gCurrent_race.number_of_racers;
    int i;

    for (i = 0; i < count; i++) {
        if (gCurrent_race.opponent_list[i].index == 0x1d) {
            break;
        }
    }
    if (i < count) {
        tCar_spec* opp_car = gCurrent_race.opponent_list[i].car_spec;
        float f = 1.0f;

        FUN_0045c760();
        opp_car->field_0x4d4 = f;
        opp_car->grip_multiplier = f;
        SetCarSuspGiveAndHeight(opp_car, f, f, f, 0.0f, 0.0f);
    }
    gOpponent_speed_factor = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004de710
void C2_HOOK_FASTCALL ResetCopsSpeed(tPowerup* powerup, tCar_spec* car) {

    gCop_speed_factor = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004de720
void C2_HOOK_FASTCALL ResetGravity(tPowerup* powerup, tCar_spec* car) {

    gGravity_multiplier = gDefault_gravity;
}

// FUNCTION: CARMA2_HW 0x004de730
void C2_HOOK_FASTCALL ResetPinball(tPowerup* powerup, tCar_spec* car) {

    gPinball_factor = 0.0f;
}

// FUNCTION: CARMA2_HW 0x004de740
void C2_HOOK_FASTCALL ResetWallclimb(tPowerup* powerup, tCar_spec* car) {

    car->wall_climber_mode = 0;
}

// FUNCTION: CARMA2_HW 0x004de750
void C2_HOOK_FASTCALL ResetBouncey(tPowerup* powerup, tCar_spec* car) {

    car->bounce_rate = 0.0f;
    car->bounce_amount = 0.0f;
}

// FUNCTION: CARMA2_HW 0x004de760
void C2_HOOK_FASTCALL ResetSuspension(tPowerup* powerup, tCar_spec* car) {

    SetCarSuspGiveAndHeight(car, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
}

// FUNCTION: CARMA2_HW 0x004de790
void C2_HOOK_FASTCALL ResetTyreGrip(tPowerup* powerup, tCar_spec* car) {

    car->grip_multiplier = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004de780
void C2_HOOK_FASTCALL ResetDamageMultiplier(tPowerup* powerup, tCar_spec* car) {

    car->damage_multiplier = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004dcf30
void C2_HOOK_FASTCALL ResetImmortalPeds(tPowerup* powerup, tCar_spec* car) {

    gImmortal_peds = 0;
}

// FUNCTION: CARMA2_HW 0x004dc920
void C2_HOOK_FASTCALL ResetStupidPedestrians(tPowerup* powerup, tCar_spec* car) {

    gBlind_pedestrians = 0;
}

// FUNCTION: CARMA2_HW 0x004dcf70
void C2_HOOK_FASTCALL ResetSuicidalPeds(tPowerup* powerup, tCar_spec* car) {

    gPeds_suicidal = 0;
}

// FUNCTION: CARMA2_HW 0x004debf0
void C2_HOOK_FASTCALL ResetMassMultiplier(tPowerup* powerup, tCar_spec* car) {

    car->field_0x4c8 = 1.0f;
    if (gNet_mode) {
        RestoreCarPixelmaps(car);
    }
}

// FUNCTION: CARMA2_HW 0x004de850
void C2_HOOK_FASTCALL ResetInstantHandbrake(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gInstant_handbrake = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004dec40
void C2_HOOK_FASTCALL HidePedestrians(tPowerup* powerup, tCar_spec* car) {

    if (car && car->driver == eDriver_local_human) {
        gShow_peds_on_map = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004dca00
void C2_HOOK_FASTCALL ResetPedHeadSize(tPowerup* powerup, tCar_spec* car) {

    ScalePedHeads(1.0f);
}

// FUNCTION: CARMA2_HW 0x004decd0
void C2_HOOK_FASTCALL ResetMutantCorpses(tPowerup* powerup, tCar_spec* car) {

    gMutant_speed = 0.0f;
}

// FUNCTION: CARMA2_HW 0x004dece0
void C2_HOOK_FASTCALL ReceivedPowerup(tNet_message_chunk_powerup* pMessage) {
    tPowerup* powerup;
    tCar_spec* car;

    if (!gProgram_state.racing || pMessage->powerup_index >= gNumber_of_powerups || pMessage->powerup_index < 0) {
        return;
    }
    powerup = &gPowerup_array[pMessage->powerup_index];
    if (pMessage->field_0x10 != 0 && pMessage->field_0x10 != 1) {
        if ((powerup->net_type & 1) == 0 && powerup->lose_proc != NULL) {
            car = NetCarFromPlayerID(pMessage->player_id);
            if (car != NULL) {
                powerup->lose_proc(powerup, car);
                car->powerups[pMessage->powerup_index] = 0;
            }
        }
        return;
    }
    if ((powerup->net_type & 1) != 0) {
        GotPowerupX(&gProgram_state.current_car, pMessage->powerup_index, 0, pMessage->field_0x10 == 0, pMessage->time_remaining);
    } else if (pMessage->player_id != gLocal_net_ID && powerup->got_proc != NULL) {
        car = NetCarFromPlayerID(pMessage->player_id);
        if (car != NULL) {
            powerup->got_proc(powerup, car);
            switch (powerup->type) {
            case ePowerup_instantaneous:
                break;
            case ePowerup_timed:
                car->powerups[pMessage->powerup_index] = GetTotalTime() + pMessage->time_remaining;
                break;
            case ePowerup_whole_race:
                car->powerups[pMessage->powerup_index] = -1;
                return;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004dca20
void C2_HOOK_FASTCALL ResetPedBrittle(tPowerup* powerup, tCar_spec* car) {

    gPed_dismemberfest = 0;
}

// FUNCTION: CARMA2_HW 0x004de9a0
void C2_HOOK_FASTCALL ResetGhostPeds(tPowerup* powerup, tCar_spec* car) {

    MakePedsNotEthereal();
}

// FUNCTION: CARMA2_HW 0x004de960
void C2_HOOK_FASTCALL ResetDancingPeds(tPowerup* powerup, tCar_spec* car) {

    gDancing_peds = 0;
}

// FUNCTION: CARMA2_HW 0x004de970
void C2_HOOK_FASTCALL ResetPanickingPeds(tPowerup* powerup, tCar_spec* car) {

    gPanicking_peds = 0;
}

// FUNCTION: CARMA2_HW 0x004de980
void C2_HOOK_FASTCALL ResetLowGravityPeds(tPowerup* powerup, tCar_spec* car) {

    gPed_gravity_multiplier = 1.0f;
}

// FUNCTION: CARMA2_HW 0x004de990
void C2_HOOK_FASTCALL ResetPissedPeds(tPowerup* powerup, tCar_spec* car) {

    gDrunk_pedestrians = 0;
}

// FUNCTION: CARMA2_HW 0x004df300
void C2_HOOK_FASTCALL ResetPissed(tPowerup* powerup, tCar_spec* car) {
    float x;
    float y;
    float z;

    gCredit_multiplier = 0;

    x = gCamera->t.t.mat.m[3][0];
    y = gCamera->t.t.mat.m[3][1];
    z = gCamera->t.t.mat.m[3][2];
    BrMatrix34Identity(&gCamera->t.t.mat);
    gCamera->t.t.mat.m[3][0] = x;
    gCamera->t.t.mat.m[3][1] = y;
    gCamera->t.t.mat.m[3][2] = z;

    x = gRearview_camera->t.t.mat.m[3][0];
    y = gRearview_camera->t.t.mat.m[3][1];
    z = gRearview_camera->t.t.mat.m[3][2];
    BrMatrix34Identity(&gRearview_camera->t.t.mat);
    gRearview_camera->t.t.mat.m[3][0] = x;
    gRearview_camera->t.t.mat.m[3][1] = y;
    gRearview_camera->t.t.mat.m[3][2] = z;
}

// C2PORT-NONMATCHING: best 47.37% | difftest PASS 2000 trials (spec difftest/specs/004dcc00_TheEffectsOfDrugs.json) | reason VC5 palette copy and rotation loop scheduling
// FUNCTION: CARMA2_HW 0x004dcc00
void C2_HOOK_FASTCALL TheEffectsOfDrugs(tPowerup* powerup, tU32 time) {
    int offset;

    (void)powerup;
    (void)time;
    offset = GetTotalTime() / 100;
    if (offset == 0) {
        memcpy(gRender_palette->pixels, gDrug_trip_data, 0x400);
    }
    PaletteFuckedUpByDrugs(gRender_palette, offset);
    ResetPalette();
}

// FUNCTION: CARMA2_HW 0x004de9b0
void C2_HOOK_FASTCALL DoBouncey(tPowerup* powerup, tU32 time) {
    double elapsed;

    elapsed = (double)(GetTotalTime() - gProgram_state.current_car.last_bounce);
    if (elapsed < gProgram_state.current_car.bounce_rate
        || gProgram_state.current_car.number_of_wheels_on_ground <= 2) {
        return;
    }
    PratcamEvent(0x2a);
    gProgram_state.current_car.last_bounce = GetTotalTime();
    gProgram_state.current_car.collision_info->v.v[1] =
        gProgram_state.current_car.collision_info->v.v[1] + gProgram_state.current_car.bounce_amount * 0.144928;
    DRS3StartSound(gXXX_outlet, 0x2332);
    gProgram_state.current_car.collision_info->disable_move_rotate = 0;
}

// FUNCTION: CARMA2_HW 0x004de810
void C2_HOOK_FASTCALL MungeVouchers(tPowerup* powerup, tU32 time) {

    (void)time;
    powerup->current_value = GetRecoverVoucherCount();
}

// FUNCTION: CARMA2_HW 0x004df3b0
void C2_HOOK_FASTCALL PissedEffect(tPowerup* powerup, tU32 time) {
    tU32 now;
    float* pOut;
    float* p;
    int k;
    float amp;
    float period;
    float phase_off;
    double ph;
    double c;
    int idx;
    float v;

    (void)powerup;
    (void)time;
    now = GetTotalTime();
    if (gCredit_multiplier == 0) {
        return;
    }
    pOut = gPissed_flicker + 12;
    do {
        p = pOut;
        for (k = 1; k != 0; k--) {
            amp = p[-12];
            if (amp != 0.0f) {
                period = p[0];
                phase_off = p[12];
                ph = fmod((double)now + phase_off, period) / period * 6.283185307179586;
                if (ph > 4.71238898038469) {
                    idx = (int)((6.283185307179586 - ph) * 40.74366543152521);
                    c = (double)gCosine_array[idx];
                } else if (ph > 3.141592653589793) {
                    idx = (int)((ph - 3.141592653589793) * 40.74366543152521);
                    c = -(double)gCosine_array[idx];
                } else if (ph > 1.5707963267948966) {
                    idx = (int)((3.141592653589793 - ph) * 40.74366543152521);
                    c = -(double)gCosine_array[idx];
                } else {
                    idx = (int)(ph * 40.74366543152521);
                    c = (double)gCosine_array[idx];
                }
                v = (float)((c * amp + 1.0f) * gCamera_angle * 182.04444444444445);
                *(tU16*)((tU8*)gCamera->type_data + 6) = (tU16)(int)v;
            }
            p += 1;
        }
        pOut += 3;
    } while (pOut < (gPissed_flicker + 15));
    if (now > (tU32)gPissed_next_time) {
        int pitch;
        int r;
        int sound;

        pitch = IRandomBetween(0x5dc, 0xbb8);
        r = PercentageChance(50);
        sound = r ? 0xe12 : 0xe13;
        DRS3StartSound2(gXXX_outlet, sound, 1, 0xff, 0xff, pitch, -1);
        gPissed_next_time = GetTotalTime() + IRandomBetween(0x5dc, 0xbb8);
    }
}

// C2PORT-NONMATCHING: best 59.55% | difftest PASS 2000 trials (spec difftest/specs/004dcfb0_SendMines.json) | reason VC5 x87 order and register allocation
// FUNCTION: CARMA2_HW 0x004dcfb0
void C2_HOOK_FASTCALL SendMines(tU32 pNext_frame_time) {
    int j;
    int k;
    int hierarchy_size;
    int chunk_size;
    tShit_mine* mine;
    tU8 tmp_buf[572];
    tNet_message_chunk_mine_explode* tmp = (tNet_message_chunk_mine_explode*)tmp_buf;
    tNet_message_chunk* chunk;
    br_vector3* mine_pos;
    br_vector3* car_pos;
    tCar_spec* car;
    float dx, dy, dz;
    tPlayer_ID target_id;
    tPlayer_ID proxy_id;
    int check_idx;

    if (gNet_mode == eNet_mode_host && gNumber_of_net_players != 1) {
        for (mine = gShit_mines; mine < &gShit_mines[CARPOCALYPSE2_ASIZE(gShit_mines)]; mine++) {
        if (!(mine->flags & 1)) {
            continue;
        }
        tmp->mine_index = (tU8)(mine - gShit_mines);
        tmp->owner_index = mine->field_0x35;
        tmp->powerup_index = mine->field_0x34;
        tmp->player_index = (tU8)(NetPlayerFromCar(mine->field_0x30) - gNet_players);
        tmp->field_0x8 = (tS32)pNext_frame_time;
        GetHierarchyNetworkStuff(mine->collision_info, tmp->object_hierarchy, NET_OBJECT_HIERARCHY_DEFAULT_SIZE);
        hierarchy_size = GetHierarchyNetworkSize(mine->collision_info);
        chunk_size = (int)((float)hierarchy_size - (-12.0f));
        tmp->header.size = (tU8)chunk_size;
        tmp->header.type = 0x31;
        for (j = 1; j < gNumber_of_net_players; j++) {
            target_id = gNet_players[j].ID;
            proxy_id = gNet_players[j].field_0xd0;
            if (proxy_id == 0) {
                check_idx = j;
            } else {
                check_idx = 0;
                if (gNumber_of_net_players - 1 > 0) {
                    for (k = 0; k < gNumber_of_net_players - 1; k++) {
                        if (gNet_players[k].ID == proxy_id) {
                            break;
                        }
                    }
                    check_idx = k;
                }
            }
            mine_pos = (br_vector3*)&mine->collision_info->transform_matrix.m[3][0];
            car = gNet_players[check_idx].car;
            car_pos = (br_vector3*)&car->collision_info->transform_matrix.m[3][0];
            dx = mine_pos->v[0];
            dy = mine_pos->v[1];
            dz = mine_pos->v[2];
            dx -= car_pos->v[0];
            dy -= car_pos->v[1];
            dz -= car_pos->v[2];
            if (BR_SQR(dx) + BR_SQR(dy) + BR_SQR(dz) < 400.0f) {
                chunk = NetGetToPlayerContents(0x31, chunk_size, target_id);
                if (chunk != NULL) {
                    memcpy(chunk, tmp, chunk_size);
                }
            }
        }
    }
    }
}

// C2PORT-NONMATCHING: best 97.4% | difftest PASS 2000 trials (spec 004db630_LosePowerupX.json) | reason restored network message and initial-value fizzle guard; remaining struct/global alias
// FUNCTION: CARMA2_HW 0x004db630
void C2_HOOK_FASTCALL LosePowerupX(tPowerup* pPowerup, undefined4 pArg2) {
    int i;
    tNet_message* msg;

    (void)pArg2;
    if (pPowerup->got_time != 0 && pPowerup->lose_proc != NULL) {
        pPowerup->lose_proc(pPowerup, pPowerup->car);
    }
    if (pPowerup->net_type & 0x80) {
        ARPowerupLose(pPowerup, pPowerup->car);
    }
    if (gNet_mode != eNet_mode_none) {
        msg = NetBuildGuaranteedMessage(0x16, 0);
        *(int*)((tU8*)msg + 0x28) = 2;
        *(tPlayer_ID*)((tU8*)msg + 0x1c) = gLocal_net_ID;
        *(int*)((tU8*)msg + 0x20) = pPowerup - gPowerup_array;
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
    }
    gProgram_state.current_car.powerups[pPowerup - gPowerup_array] = 0;
    if (pPowerup->initial_value == 0) {
        for (i = 0; i < gNumber_of_icons; i++) {
            if (gPickedup_powerups[i].powerup == pPowerup) {
                gPickedup_powerups[i].fizzle_stage = 3;
                gPickedup_powerups[i].fizzle_direction = -1;
                gPickedup_powerups[i].fizzle_start = GetTotalTime();
                break;
            }
        }
    }
    pPowerup->got_time = 0;
}

// FUNCTION: CARMA2_HW 0x004db730
void C2_HOOK_FASTCALL KeyboardPowerupFinished(tPowerup* pPowerup, int pSelect_new) {
    int i;
    tNet_message* msg;

    if (pPowerup->initial_value < 0 || gINT_006a0a6c != 0) {
        if (pPowerup->got_time != 0 && pPowerup->lose_proc != NULL) {
            pPowerup->lose_proc(pPowerup, pPowerup->car);
        }
        if (pPowerup->net_type & 0x80) {
            ARPowerupLose(pPowerup, pPowerup->car);
        }
        if (gNet_mode != eNet_mode_none) {
            msg = NetBuildGuaranteedMessage(0x16, 0);
            *(int*)((tU8*)msg + 0x28) = 2;
            *(tPlayer_ID*)((tU8*)msg + 0x1c) = gLocal_net_ID;
            *(int*)((tU8*)msg + 0x20) = pPowerup - gPowerup_array;
            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
        }
        gPowerup_net_state[pPowerup - gPowerup_array] = 0;
        if (pPowerup->initial_value == 0) {
            for (i = 0; i < gNumber_of_icons; i++) {
                if (gPickedup_powerups[i].powerup == pPowerup) {
                    gPickedup_powerups[i].fizzle_stage = 3;
                    gPickedup_powerups[i].fizzle_direction = -1;
                    gPickedup_powerups[i].fizzle_start = GetTotalTime();
                    break;
                }
            }
        }
        pPowerup->got_time = 0;
    }
    gINT_006a0a5c = 1;
    if (!pSelect_new) {
        return;
    }
    i = gInventory_selected;
    gInventory_selected = -1;
    gInventory[i] = -1;
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gInventory); i++) {
        if (gInventory[i] >= 0) {
            gInventory_selected = i;
            break;
        }
    }
}

// FUNCTION: CARMA2_HW 0x004daff0
int C2_HOOK_FASTCALL DrawSinglePowerupIcon(int pDraw, int pTime, tPowerup* pPowerup, tHeadup_icon* pIcon, int pX, int pY, int pUpdate_x) {
    br_pixelmap *fizzle_pix;
    char s[8];
    int timer;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_space_between_icon_text_dx, 0x294);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_space_between_icon_text_dy, 0x2a0);

    if (pIcon != NULL && pIcon->fizzle_stage < 4) {
        fizzle_pix = gFizzle_in[pPowerup->fizzle_type];
        if (pDraw) {
            PrintPowerupIconIn3D(pX, pY, pIcon, pPowerup, 1, pTime);
        }
        if (pUpdate_x) {
            return pX + fizzle_pix->width;
        }
        return 0;
    }
    if (pDraw) {
        PrintPowerupIconIn3D(pX, pY, pIcon, pPowerup, 0, pTime);
    }
    if (pPowerup->initial_value < 0) {
        sprintf(s, "%d", -pPowerup->value);
        if (pDraw) {
            DRPixelmapText(gBack_screen,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx,
                pY + gCurrent_graf_data->power_up_space_between_icon_text_dy,
                &gFonts[2],
                s,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx + 30);
        }
        if (pUpdate_x) {
            return pX + DRTextWidth(&gFonts[2], s);
        }
    } else if (pPowerup->initial_value > 0) {
        TimerString(pPowerup->value, s, 0, 0, 1);
        if (pDraw) {
            DRPixelmapText(gBack_screen,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx,
                pY + gCurrent_graf_data->power_up_space_between_icon_text_dy,
                &gFonts[2],
                s,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx + 30);
        }
        if (pUpdate_x) {
            return pX + DRTextWidth(&gFonts[2], s);
        }
    } else if (pPowerup->type == ePowerup_timed) {

        timer = pPowerup->lose_time - pTime;
        if (timer < 0) {
            timer = 0;
        }
        TimerString(timer, s, 0, 0, 1);
        if (pDraw) {
            DRPixelmapText(gBack_screen,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx,
                pY + gCurrent_graf_data->power_up_space_between_icon_text_dy,
                &gFonts[1],
                s,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx + 30);
        }
        if (pUpdate_x) {
            return pX + DRTextWidth(&gFonts[1], s);
        }
    } else if (pPowerup->current_value > 0) {

        sprintf(s, "%d", pPowerup->current_value);
        if (pDraw) {
            DRPixelmapText(gBack_screen,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx,
                pY + gCurrent_graf_data->power_up_space_between_icon_text_dy,
                &gFonts[2],
                s,
                pX + gCurrent_graf_data->power_up_space_between_icon_text_dx + 30);
        }
        if (pUpdate_x) {
            return pX + DRTextWidth(&gFonts[2], s);
        }
    }
    return 0;
}

void C2_HOOK_FASTCALL DrawPowerupIcons2(tU32 pTime, tHeadup_icon* pHeadup_icons, int* pCount_icons, int pMax_count_icons, int pX, int pY) {
    int i;
    int y;
    tPowerup *the_powerup;
    tHeadup_icon *the_icon;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_icon_y_pitch, 0x29c);

    y =  pY;
    for (i = 0; i < *pCount_icons && i < pMax_count_icons; i++) {
        the_icon = &pHeadup_icons[i];
        the_powerup = the_icon->powerup;
        if (the_powerup->icon == NULL) {
            continue;
        }
        y += gCurrent_graf_data->power_up_icon_y_pitch;
        if (the_icon->fizzle_stage < 4) {
            if (the_icon->fizzle_direction < 0) {
                the_icon->fizzle_stage = 3 - (pTime - the_icon->fizzle_start) / 150;
            } else {
                the_icon->fizzle_stage = (pTime - the_icon->fizzle_start) / 150;
            }
            if (the_icon->fizzle_stage > 4) {
                the_icon->fizzle_stage = 4;
            } else if (the_icon->fizzle_stage < 0) {
                memmove(the_icon, &the_icon[1], sizeof(tHeadup_icon) * (*pCount_icons - i - 1));
                *pCount_icons -= 1;
                continue;
            }
        }
        DrawSinglePowerupIcon(1, pTime, the_icon->powerup, the_icon, pX, y, 0);
    }
}

void C2_HOOK_FASTCALL DrawKeyPowerups(tU32 pTime) {
    int x;
    int i;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_x, 0x2a4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_y, 0x2a8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_y, 0x2ac);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_bottom, 0x2b0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_x, 0x2b4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_spacing_x, 0x2b8);

    if (gInventory_cycling) {
        if (gInventory_selected >= 0) {
            if (GetTotalTime() >= gInventory_timeout) {
                gInventory_timeout = 0;
                gInventory_cycling = 0;
            }
            x = gCurrent_graf_data->keyboard_powerup_x;
            i = gInventory_selected;
            do {
                if (gInventory[i] >= 0) {
                    x = DrawSinglePowerupIcon(0, pTime, &gPowerup_array[gInventory[i]], NULL,
                        x, gCurrent_graf_data->keyboard_powerup_y, 1) + gCurrent_graf_data->keyboard_powerup_spacing_x;
                }
                i += 1;
                if (i >= CARPOCALYPSE2_ASIZE(gInventory)) {
                    i = 0;
                }
            } while (i != gInventory_selected);
            if (x >= 0) {
                DimRectangle(gBack_screen, gCurrent_graf_data->keyboard_powerup_bg_x, gCurrent_graf_data->keyboard_powerup_bg_y,
                    x, gCurrent_graf_data->keyboard_powerup_bg_bottom, 1);
                i = gInventory_selected;
                x = gCurrent_graf_data->keyboard_powerup_x;
                DRPixelmapText(gBack_screen,
                    gCurrent_graf_data->keyboard_powerup_bg_x + 1, gCurrent_graf_data->keyboard_powerup_bg_bottom - 9,
                    &gFonts[2],
                    gPowerup_array[gInventory[gInventory_selected]].message,
                    gCurrent_graf_data->keyboard_powerup_bg_x + 100);
                do {
                    if (gInventory[i] >= 0 && gInventory_cycling) {
                        x = DrawSinglePowerupIcon(1, pTime, &gPowerup_array[gInventory[i]], NULL,
                            x, gCurrent_graf_data->keyboard_powerup_y, 1) + gCurrent_graf_data->keyboard_powerup_spacing_x;
                    }
                    i += 1;
                    if (i >= CARPOCALYPSE2_ASIZE(gInventory)) {
                        i = 0;
                    }
                } while (i != gInventory_selected);
            }
        }
    } else {
        if (gInventory_selected >= 0) {
            DrawSinglePowerupIcon(1, pTime, gPowerup_array + gInventory[gInventory_selected], NULL,
            gCurrent_graf_data->keyboard_powerup_x, gCurrent_graf_data->keyboard_powerup_y, 0);
        }
    }
}

// C2PORT-NONMATCHING: best 46.45% | difftest PASS 2000 trials (spec difftest/specs/004dad00_DrawPowerups.json) | reason VC5 register and inline-copy layout
// FUNCTION: CARMA2_HW 0x004dad00
void C2_HOOK_FASTCALL DrawPowerups(tU32 pTime) {
    int i;
    int x;
    int y;
    tPowerup* the_powerup;
    tHeadup_icon* the_icon;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_icon_x, 0x290);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_icon_y, 0x298);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, power_up_icon_y_pitch, 0x29c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_x, 0x2a4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_y, 0x2a8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_y, 0x2ac);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_bottom, 0x2b0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_bg_x, 0x2b4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tGraf_data, keyboard_powerup_spacing_x, 0x2b8);

    y = gCurrent_graf_data->power_up_icon_y;
    for (i = 0; i < gNumber_of_icons && i < 5; i++) {
        the_icon = &gPickedup_powerups[i];
        the_powerup = the_icon->powerup;
        if (the_powerup->icon != NULL) {
            y += gCurrent_graf_data->power_up_icon_y_pitch;
            if (the_icon->fizzle_stage < 4) {
                if (the_icon->fizzle_direction < 0) {
                    the_icon->fizzle_stage = 3 - (pTime - the_icon->fizzle_start) / 150;
                } else {
                    the_icon->fizzle_stage = (pTime - the_icon->fizzle_start) / 150;
                }
                if (the_icon->fizzle_stage > 4) {
                    the_icon->fizzle_stage = 4;
                } else if (the_icon->fizzle_stage < 0) {
                    memmove(the_icon, &the_icon[1], sizeof(tHeadup_icon) * (gNumber_of_icons - i - 1));
                    gNumber_of_icons--;
                    continue;
                }
            }
            DrawSinglePowerupIcon(1, pTime, the_icon->powerup, the_icon, gCurrent_graf_data->power_up_icon_x, y, 0);
        }
    }

    if (gInventory_cycling) {
        if (gInventory_selected >= 0) {
            if (GetTotalTime() >= gInventory_timeout) {
                gInventory_timeout = 0;
                gInventory_cycling = 0;
            }
            x = gCurrent_graf_data->keyboard_powerup_x;
            i = gInventory_selected;
            do {
                if (gInventory[i] >= 0) {
                    x = DrawSinglePowerupIcon(0, pTime, &gPowerup_array[gInventory[i]], NULL,
                        x, gCurrent_graf_data->keyboard_powerup_y, 1) + gCurrent_graf_data->keyboard_powerup_spacing_x;
                }
                i++;
                if (i >= CARPOCALYPSE2_ASIZE(gInventory)) {
                    i = 0;
                }
            } while (i != gInventory_selected);
            if (x >= 0) {
                DimRectangle(gBack_screen, gCurrent_graf_data->keyboard_powerup_bg_x, gCurrent_graf_data->keyboard_powerup_bg_y,
                    x, gCurrent_graf_data->keyboard_powerup_bg_bottom, 1);
                i = gInventory_selected;
                x = gCurrent_graf_data->keyboard_powerup_x;
                DRPixelmapText(gBack_screen,
                    gCurrent_graf_data->keyboard_powerup_bg_x + 1, gCurrent_graf_data->keyboard_powerup_bg_bottom - 9,
                    &gFonts[2],
                    gPowerup_array[gInventory[gInventory_selected]].message,
                    gCurrent_graf_data->keyboard_powerup_bg_x + 100);
                do {
                    if (gInventory[i] >= 0 && gInventory_cycling) {
                        x = DrawSinglePowerupIcon(1, pTime, &gPowerup_array[gInventory[i]], NULL,
                            x, gCurrent_graf_data->keyboard_powerup_y, 1) + gCurrent_graf_data->keyboard_powerup_spacing_x;
                    }
                    i++;
                    if (i >= CARPOCALYPSE2_ASIZE(gInventory)) {
                        i = 0;
                    }
                } while (i != gInventory_selected);
            }
        }
    } else if (gInventory_selected >= 0) {
        DrawSinglePowerupIcon(1, pTime, gPowerup_array + gInventory[gInventory_selected], NULL,
            gCurrent_graf_data->keyboard_powerup_x, gCurrent_graf_data->keyboard_powerup_y, 0);
    }
}

// FUNCTION: CARMA2_HW 0x004e0710
void C2_HOOK_FASTCALL SetTailRenderStyleFaces(void) {
    tPhysics_object* node;

    for (node = gMutant_tail_first_collision_info; node != NULL; node = node->child) {
        node->actor->render_style = BR_RSTYLE_FACES;
    }
}

// FUNCTION: CARMA2_HW 0x004e0730
void C2_HOOK_FASTCALL HideTail(void) {
    tPhysics_object* node;

    for (node = gMutant_tail_first_collision_info; node != NULL; node = node->child) {
        node->actor->render_style = BR_RSTYLE_NONE;
    }
}

// FUNCTION: CARMA2_HW 0x004e0750
void C2_HOOK_FASTCALL MayQueuePowerupRespawn(int pPowerup_index, br_actor* actor) {
    int i;

    if (actor != NULL && gRace_powerup_respawn_bools[pPowerup_index] && (gNet_mode == eNet_mode_none || gCurrent_net_game->options.powerup_respawn)) {
for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gRespawn_powerups); i++) {
            if (gRespawn_powerups[i].actor != NULL) {
                gRespawn_powerups[i].actor = actor;
                gRespawn_powerups[i].index = pPowerup_index;;
                gRespawn_powerups[i].respawn_time = GetTotalTime() + gPickup_respawn_max_extra_time_ms / 2 + gPickup_respawn_min_time_ms;
                break;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004e07e0
void C2_HOOK_FASTCALL FUN_004e07e0(char* pArg, int pUnused) {

    pArg[0x20] = 1;
}
