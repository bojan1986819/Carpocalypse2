#include "replay.h"

#include "car.h"
#include "controls.h"
#include "displays.h"
#include "drone.h"
#include "finteray.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "input.h"
#include "loading.h"
#include "main.h"
#include "mainloop.h"
#include "movie.h"
#include "netgame.h"
#include "opponent.h"
#include "oil.h"
#include "pedestrn.h"
#include "piping.h"
#include "platform.h"
#include "powerups.h"
#include "replay_callbacks.h"
#include "spark.h"
#include "structur.h"
#include "sound.h"
#include "utility.h"
#include "video.h"
#include "world.h"
#include "grafdata.h"

#include "c2_string.h"
#include "s3/internal/audio.h"

#include "carpocalypse2_macros.h"
#include "replay.h"
// GLOBAL: CARMA2_HW 0x00679278
int gAction_replay_manual_camera_target_type;

// GLOBAL: CARMA2_HW 0x0067927c
br_vector3 gAction_replay_manual_camera_target_position;

// GLOBAL: CARMA2_HW 0x00679288
tCar_spec* gAction_replay_manual_camera_target_car;

// GLOBAL: CARMA2_HW 0x0067928c
tPedestrian* gAction_replay_manual_camera_target_pedestrian;

// GLOBAL: CARMA2_HW 0x00679290
br_actor* gAction_replay_manual_camera_target_character_actor;

// GLOBAL: CARMA2_HW 0x0058f600
tU8 gCamera_type_allowed_replay[9] = {
    1, 1, 1, 1, 1, 0, 0, 1, 1,
};

// GLOBAL: CARMA2_HW 0x0058f610
tU8 gCamera_type_allowed_gameplay[9] = {
    1, 0, 0, 0, 1, 0, 0, 1, 1,
};

// GLOBAL: CARMA2_HW 0x006a23c4
int gSingle_frame_mode;

// GLOBAL: CARMA2_HW 0x006a23d4
tU32 gLast_synch_time;

// GLOBAL: CARMA2_HW 0x006a2374
int gKey_down;

// GLOBAL: CARMA2_HW 0x006a2380
tU32 gTime_of_last_zappy_headups;

// GLOBAL: CARMA2_HW 0x006a2388
br_pixelmap* gReplay_pixelmaps[11];

// GLOBAL: CARMA2_HW 0x0065fbb0
const char* gReplay_pixie_names[11] = {
    "REPLAY.PIX",
    "RBUTTONS.PIX",
    "REWSTART.PIX",
    "REW.PIX",
    "REVPLAY.PIX",
    "PAUSE.PIX",
    "PLAY.PIX",
    "FFWD.PIX",
    "FWDEND.PIX",
    "CAMERA.PIX",
    "RECORD.PIX",
};

// GLOBAL: CARMA2_HW 0x0065fbe0
int gReplay_progress_bar_left[2] = { 240, 443 };

// GLOBAL: CARMA2_HW 0x0065fbe8
int gReplay_progress_bar_right[2] = { 306, 612 };

// GLOBAL: CARMA2_HW 0x0065fbf0
int gReplay_progress_bar_y[2] = { 184, 446 };

// GLOBAL: CARMA2_HW 0x006a2360
tU32 gReplay_progress_start_time;

// GLOBAL: CARMA2_HW 0x006a2364
tU32 gReplay_progress_end_time;

// GLOBAL: CARMA2_HW 0x006a2368
int gReplay_show_camera_icon;

// GLOBAL: CARMA2_HW 0x006a2378
float gUNK_006a2378;

// GLOBAL: CARMA2_HW 0x006a237c
tU32 gUNK_006a237c;

// GLOBAL: CARMA2_HW 0x006a23c8
tU32 gReplay_camera_mode_change_time;

// GLOBAL: CARMA2_HW 0x0058adb8
const float gReplay_slow_rate_limit = -1.0f;

// GLOBAL: CARMA2_HW 0x0058adbc
const float gReplay_fast_rate_limit = 1.0f;

// GLOBAL: CARMA2_HW 0x006a23cc
int gUNK_006a23cc;

// GLOBAL: CARMA2_HW 0x006a23d0
tU32 gReplay_rate_adjust_time;

// GLOBAL: CARMA2_HW 0x0065fbf8
const int gReplay_control_key_codes[9] = {
    0x58, 0x55, 0x3b, 0x6a, 0x3a, 0x57, 0x5a, 0x4c, 0x4b,
};

// GLOBAL: CARMA2_HW 0x0065fc20
const tReplayControlButtonBounds gReplay_control_button_bounds[2][9] = {
    {
        { 268, 156, 278, 174 }, { 279, 150, 289, 160 }, { 279, 170, 289, 180 },
        { 281, 160, 298, 170 }, { 290, 170, 300, 180 }, { 290, 150, 300, 160 },
        { 301, 156, 311, 174 }, { 239, 168, 251, 180 }, { 254, 168, 266, 180 },
    },
    {
        { 519, 395, 543, 419 }, { 542, 372, 566, 396 }, { 545, 418, 569, 442 },
        { 555, 395, 579, 419 }, { 566, 418, 590, 442 }, { 569, 372, 593, 396 },
        { 595, 395, 619, 419 }, { 447, 415, 471, 439 }, { 481, 417, 505, 441 },
    },
};

// GLOBAL: CARMA2_HW 0x0058adc0
const volatile float gReplay_rate_zero = 0.0f;

// GLOBAL: CARMA2_HW 0x0058adc8
const double gReplay_forward_rate_adjust_limit = 1.2;

// GLOBAL: CARMA2_HW 0x0058add0
const double gReplay_forward_rate_adjust_step = -0.002;

// GLOBAL: CARMA2_HW 0x0058add8
const float gReplay_forward_rate_max = 8.0f;

// GLOBAL: CARMA2_HW 0x0058ade0
const double gReplay_reverse_rate_adjust_limit = -1.2;

// GLOBAL: CARMA2_HW 0x0058ade8
const double gReplay_reverse_rate_adjust_step = 0.002;

// GLOBAL: CARMA2_HW 0x0058adf0
const float gReplay_reverse_rate_min = -8.0f;

// GLOBAL: CARMA2_HW 0x00589330
const br_scalar gFLOAT_00589330 = 0.0f;

// GLOBAL: CARMA2_HW 0x006768fc
int gReplay_play_flag;

// GLOBAL: CARMA2_HW 0x0058f620
int gAR_camera_type = 2;

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0040e790
void C2_HOOK_FASTCALL SetCameraType(tActionReplayCameraMode pCamPos) {

    gAction_replay_camera_mode = pCamPos;
    switch (pCamPos) {
    case kActionReplayCameraMode_Standard:
    case kActionReplayCameraMode_Rigid:
    case kActionReplayCameraMode_Reversing:
        InitialiseExternalCamera();
        break;
    case kActionReplayCameraMode_Manual:
        gAction_replay_manual_camera_target_type = 0;
        break;
    default:
        break;
    }
    MungeCarMaterials(&gProgram_state.current_car, gAction_replay_camera_mode == kActionReplayCameraMode_Internal);
}
#pragma auto_inline(on)

int C2_HOOK_FASTCALL IsCameraTypeAllowed(tActionReplayCameraMode pCamera_type) {

    if (gAction_replay_mode && !gCamera_type_allowed_replay[pCamera_type]) {
        return 0;
    }
    if (!gAction_replay_mode && !gCamera_type_allowed_gameplay[pCamera_type]) {
        return 0;
    }
    if (pCamera_type >= 9) {
        return 0;
    }
    if (pCamera_type == kActionReplayCameraMode_Drone && !OKToViewDrones()) {
        return 0;
    }
    if (pCamera_type == kActionReplayCameraMode_Peds && gPed_count == 0) {
        return 0;
    }
    return 1;
}

void C2_HOOK_FASTCALL CheckCameraHither(void) {
    br_camera *cam;
    // GLOBAL: CARMA2_HW 0x0067931c
    static int old_hither;

    cam = gCamera->type_data;
    if (TestForNan(&cam->hither_z)) {
        cam->hither_z = (float)old_hither;
    }
    old_hither = (int)cam->hither_z;
}

void C2_HOOK_FASTCALL NormalPositionExternalCamera(tCar_spec* pCar, tU32 pTime_difference) {

    GeneralisedPositionExternalCamera(pCar, &pCar->car_master_actor->t.t.mat,
        &pCar->pos, pCar->speed, pCar->speedo_speed, &pCar->direction, &pCar->collision_info->omega, pTime_difference);
}

void C2_HOOK_FASTCALL SetPanningFieldOfView(void) {
    br_camera* camera_ptr;

    camera_ptr = gCamera->type_data;
    if (gPanning_camera_angle == 0) {
        gPanning_camera_angle = (br_angle)(BrDegreeToAngle(gCamera_angle) * 0.7);
    }
    camera_ptr->field_of_view = gPanning_camera_angle;
}

void C2_HOOK_FASTCALL PositionPedCam(tPed_character_instance* pPed_character, tU32 pTime) {

    if (pPed_character == NULL) {
        ChangeCameraType();
    } else {
        br_matrix34* mat;

        mat = GetCharacterMatrixPtr(pPed_character);
        GeneralisedPositionExternalCamera(NULL, mat, (br_vector3*)mat->m[3],
            0.f, 0.f, &pPed_character->field_0xc0, &gZero_v__car, pTime);
    }
}

void C2_HOOK_FASTCALL PositionDroneCam(tU32 pTime_difference) {

    if (OKToViewDrones()) {
        br_matrix34* mat;
        br_vector3* dir;

        mat = GetCurrentViewDroneMat();
        dir = GetCurrentViewDroneDirection();
        GeneralisedPositionExternalCamera(NULL, mat, (br_vector3*)mat->m[3], 0.f, 0.f, dir, &gZero_v__car, pTime_difference);
    }
}

// FUNCTION: CARMA2_HW 0x0040ea30
void C2_HOOK_FASTCALL PositionExternalCamera(tCar_spec* pCar_spec, tU32 pTime) {
    br_camera* camera;
    // GLOBAL: CARMA2_HW 0x0058f62c
    static tActionReplayCameraMode old_camera_mode = kActionReplayCameraMode_Invalid;

    camera = gCamera->type_data;
    CheckCameraHither();
    if (!IsCameraTypeAllowed(gAction_replay_camera_mode)) {
        if (KeyIsDown(8)) {
            gAction_replay_camera_mode--;
            if (gAction_replay_camera_mode >= 9 || gAction_replay_camera_mode < 0) {
                gAction_replay_camera_mode = 8;
            }
        } else {
            gAction_replay_camera_mode++;
            if (gAction_replay_camera_mode >= 9) {
                gAction_replay_camera_mode = 0;
            }
        }
        if (!IsCameraTypeAllowed(gAction_replay_camera_mode)) {
            ChangeCameraType();
        }
        if (gAction_replay_mode) {
            gAR_camera_type = gAction_replay_camera_mode;
        } else {
            gCamera_type = gAction_replay_camera_mode;
        }
        SetCameraType(gAction_replay_camera_mode);
    }
    {
        // GLOBAL: CARMA2_HW 0x00679298
        static tU32 time_of_death;

        // GLOBAL: CARMA2_HW 0x00679320
        static tU32 headup_timer;
        char s[256];

        if (gNet_mode != eNet_mode_none
                && (gCurrent_net_game->type == eNet_game_type_4
                        || gCurrent_net_game->type == eNet_game_type_fight_to_death)) {
            if (!gRace_finished) {
                time_of_death = 0;
                gOpponent_viewing_mode = 0;
            } else if (time_of_death == 0) {
                time_of_death = GetRaceTime();
            } else if (time_of_death + 10000 <= GetRaceTime()) {
                if (gOpponent_viewing_mode == 0) {
                    gOpponent_viewing_mode = 1;
                    gNet_player_to_view_index = -2;
                    ViewNetPlayer();
                }
                if (gNet_player_to_view_index >= gNumber_of_net_players) {
                    gNet_player_to_view_index = -2;
                    ViewNetPlayer();
                }
                if (gNet_player_to_view_index < 0 && GetRaceLeader() != gCar_to_view) {
                    gNet_player_to_view_index = -2;
                    ViewNetPlayer();
                }
                if ((headup_timer + 1000 < GetRaceTime() || headup_timer > GetRaceTime()) && gRace_over_reason == eRace_not_over_yet) {
                    strcpy(s, GetMiscString(eMiscString_watching));
                    strcat(s, " ");
                    if (gNet_player_to_view_index < 0) {
                        strcat(s, GetMiscString(eMiscString_race_leader));
                    } else {
                        strcat(s, gNet_players[gNet_player_to_view_index].player_name);
                    }
                    headup_timer = GetRaceTime();
                    NewTextHeadupSlot(6, 0, 500, -4, s);
                }
            }
        }
    }
    if (!gProgram_state.cockpit_on) {
        tCar_spec* c;

        if (gOpponent_viewing_mode && gAction_replay_mode) {
            c = &gProgram_state.current_car;
        } else {
            c = gCar_to_view;
        }
        if (c->car_master_actor->t.t.translate.t.v[0] <= 500.f) {
            if (old_camera_mode != gAction_replay_camera_mode) {
                camera->field_of_view = BrDegreeToAngle(gCamera_angle);
            }
            camera->hither_z = gCamera_hither;
            switch (gAction_replay_camera_mode) {
            case kActionReplayCameraMode_Standard:
            case kActionReplayCameraMode_Rigid:
            case kActionReplayCameraMode_Reversing:
                NormalPositionExternalCamera(c, pTime);
                break;
            case kActionReplayCameraMode_ActionTracking:
                CheckDisablePlingMaterials(c);
                SetPanningFieldOfView();
                if (!IncidentCam(c, pTime)) {
                    if (old_camera_mode != gAction_replay_camera_mode) {
                        SetUpPanningCamera(c);
                    }
                    PanningExternalCamera(c, pTime);
                    EnablePlingMaterials();
                }
                break;
            case kActionReplayCameraMode_Panning:
                CheckDisablePlingMaterials(c);
                SetPanningFieldOfView();
                if (old_camera_mode != kActionReplayCameraMode_Panning) {
                    SetUpPanningCamera(c);
                }
                PanningExternalCamera(c, pTime);
                EnablePlingMaterials();
                break;
            case kActionReplayCameraMode_Manual:
                FrozenCamera(c, pTime);
                break;
            case kActionReplayCameraMode_Peds:
                PositionPedCam(GetTestPed(), pTime);
                break;
            case kActionReplayCameraMode_Drone:
                PositionDroneCam(pTime);
                break;
            case kActionReplayCameraMode_Internal:
                camera->hither_z = gCamera_cockpit_hither;
                PositionCarMountedCamera(c, pTime);
                break;
#ifdef CARPOCALYPSE2_FIX_BUGS
            default:
                abort();
#endif
            }
            old_camera_mode = gAction_replay_camera_mode;
        }
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00403c70
void C2_HOOK_FASTCALL FUN_00403c70(tARDoActionReplay_step_callback* pStep_callback, tARDoActionReplay_frame_callback* pFrame_start_callback, tARDoActionReplay_frame_callback* pFrame_end_callback) {
    float saved_replay_rate;

    saved_replay_rate = gReplay_rate;
    gDisable_sound = 1;
    gReplay_rate = -100.0f;
    ARDoActionReplay(-2147483647, pStep_callback, pFrame_start_callback, pFrame_end_callback);
    gDisable_sound = 0;
    gReplay_rate = saved_replay_rate;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00403cc0
void C2_HOOK_FASTCALL FUN_00403cc0(int pUnused, tARDoActionReplay_step_callback* pStep_callback, tARDoActionReplay_frame_callback* pFrame_start_callback, tARDoActionReplay_frame_callback* pFrame_end_callback) {
    if (gReplay_rate != gFLOAT_00589330) {
        ARDoActionReplay((int)gReplay_rate, pStep_callback, pFrame_start_callback, pFrame_end_callback);
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00403d40
void C2_HOOK_FASTCALL ARMainLoopStart(void) {

    gAction_replay_mode = 0;
    gLast_replay_frame_time = PDGetTotalTime();
}

// FUNCTION: CARMA2_HW 0x00403d30
void C2_HOOK_FASTCALL ARService(void) {

}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00403d00
int C2_HOOK_FASTCALL FUN_00403d00(void) {
    return S3RegisterSampleFilters(thunk_FUN_00564d4a, thunk_FUN_00564d4a);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00403d10
void C2_HOOK_FASTCALL thunk_FUN_00564d4a(int pUnused, int pTag) {
    FUN_00564d4a(pUnused, pTag);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00403d20
int C2_HOOK_FASTCALL FUN_00403d20(void) {
    return S3RegisterSampleFilters(NULL, NULL);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004e71c0
void C2_HOOK_FASTCALL TurnOnActionReplay(void) {
    PedNoOp2();
    if (gMap_view == 2) {
        ToggleMap();
    }
    if (gNet_mode == 2) {
        SendGameplayToAllPlayers(eNet_gameplay_host_paused, 0, 0, 0, 0);
    }
    gUNK_006a237c = PDGetTotalTime();
    gReplay_progress_start_time = FindPipeSessionZeroValue();
    LoadInterfaceStuff(1);
    StartMouseCursor();
    gKey_down = 0x34;
    gUNK_006a2378 = 0;
    gReplay_camera_mode_change_time = PDGetTotalTime();
    gReplay_progress_end_time = GetTotalTime();
    if (gRace_finished == 0) {
        SaveCameraPosition(0);
    }
    gCamera_type = gAction_replay_camera_mode;
    SetCameraType(gAR_camera_type);
    InitReplaySplashes();
}

// FUNCTION: CARMA2_HW 0x004e7270
void C2_HOOK_FASTCALL TurnOffActionReplay(void) {
    UnlockInterfaceStuff();
    AssertThisTimeAsCurrentTime();
    if (gRace_finished == 0) {
        RestoreCameraPosition(0);
    }
    if (gNet_mode == 2) {
        SendGameplayToAllPlayers(eNet_gameplay_host_unpaused, 0, 0, 0, 0);
    }
    gAR_camera_type = gAction_replay_camera_mode;
    SetCameraType(gCamera_type);
    FUN_004d6da0();
    EndMouseCursor();
}

// FUNCTION: CARMA2_HW 0x004e72d0
void C2_HOOK_FASTCALL AfterActionReplay(void) {

    ForceRebuildActiveCarList();
}

// FUNCTION: CARMA2_HW 0x004e6900
void C2_HOOK_FASTCALL DoZappyActionReplayHeadups(int pFrame_number) {

    tU32 time = PDGetTotalTime();

    if (abs(pFrame_number) > 0x2710 &&
        time - gTime_of_last_zappy_headups > 0x32) {
        ActualActionReplayHeadups(pFrame_number);
        gTime_of_last_zappy_headups = time;
        PDScreenBufferSwap(0);
        RemoveTransientBitmaps(1);
    }
}

// FUNCTION: CARMA2_HW 0x004e6950
void C2_HOOK_FASTCALL PreProcess(int pFrame_period) {
    UpdateFramePeriod(&gCamera_period);
    if (ARGetReplayRate() < 0.0f) {
        ((void (C2_HOOK_FASTCALL *)(int))MungePedestrians)(pFrame_period);
    }
}

// FUNCTION: CARMA2_HW 0x004e6980
void C2_HOOK_FASTCALL PostProcess(int pFrame_period) {
    ProcessOilSpills((tU32)pFrame_period);
    if (ARGetReplayRate() >= 0.0f) {
        ((void (C2_HOOK_FASTCALL *)(int))MungePedestrians)(pFrame_period);
    }
    MungePowerupStuff(pFrame_period);
}

// FUNCTION: CARMA2_HW 0x004e72e0
void C2_HOOK_FASTCALL ToggleReplay(int* pArg1, int* pArg2) {

    if (gAction_replay_mode) {
        RenderAFrame(1);
        RenderAFrame(1);
    }
    if (!ARToggleReplay(TurnOnActionReplay, TurnOffActionReplay, AfterActionReplay,
            DoZappyActionReplayHeadups, PreProcess, PostProcess, pArg1, pArg2)) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(eMiscString_action_replay_unavailable));
    }
}

// FUNCTION: CARMA2_HW 0x004c6bf0
void C2_HOOK_FASTCALL InitialiseActionReplay(void) {

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gReplay_callbacks) != 70);

    ARInitialise(!gAusterity_mode && gNet_mode == eNet_mode_none, CARPOCALYPSE2_ASIZE(gReplay_callbacks), gReplay_callbacks);
    if (!gAusterity_mode && gNet_mode == eNet_mode_none) {
        gCrush_space = BrMemAllocate(0x4000, kMem_pipe_model_geometry);
        gSmudge_space = (tPipe_smudge_data*)gCrush_space;
        return;
    }
    gCrush_space = NULL;
    gSmudge_space = NULL;
}

// FUNCTION: CARMA2_HW 0x004c6c60
void C2_HOOK_FASTCALL DisposeActionReplay(void) {
    DisposePiping();
    if (gCrush_space != NULL) {
        BrMemFree(gCrush_space);
        gCrush_space = NULL;
    }
}

// FUNCTION: CARMA2_HW 0x004e6ff0
void C2_HOOK_FASTCALL CheckReplayTurnOn(void) {

    if (!gAction_replay_mode) {
        if (!KeyIsDown(58) || gEntering_message) {
            gKey_down = -1;
        } else if (gKey_down == -1) {
            ToggleReplay(NULL, NULL);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004e68e0
void C2_HOOK_FASTCALL DoActionReplay(tU32 pFrame_period) {

    gTime_of_last_zappy_headups = 0;
    FUN_00403cc0(pFrame_period, DoZappyActionReplayHeadups, PreProcess, PostProcess);
}

// FUNCTION: CARMA2_HW 0x004e69b0
void C2_HOOK_FASTCALL PollActionReplayControls(tU32 *pFrame_period, tU32* pAverage_frame_period) {
    float initial_replay_rate;
    tU32 current_time;
    int previous_key;
    int i;
    int mouse_x;
    int mouse_y;

    EdgeTriggerModeOff();
    current_time = PDGetTotalTime();
    initial_replay_rate = ARGetReplayRate();
    previous_key = gKey_down;
    gKey_down = PDAnyKeyDown();
    if ((previous_key == -1 || previous_key == 4) && KeyIsDown(0x3a)) {
        DRS3StopAllOutletSoundsExceptCDA();
        if (gAction_replay_mode) {
            RenderAFrame(1);
            RenderAFrame(1);
        }
        if (ARToggleReplay(TurnOnActionReplay, TurnOffActionReplay,
                AfterActionReplay, DoZappyActionReplayHeadups,
                PreProcess, PostProcess, (int*)pFrame_period, (int*)pAverage_frame_period)) {
            return;
        }
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(0x25));
        return;
    }

    if (gKey_down == -1) {
        if ((previous_key == -1 || previous_key == 4 || previous_key == 0x55
                || previous_key == 0x57 || previous_key == 0x4c)
                && EitherMouseButtonDown()) {
            GetMousePosition(&mouse_x, &mouse_y);
            for (i = 0; i < 9; i++) {
                const tReplayControlButtonBounds* bounds;

                bounds = &gReplay_control_button_bounds[gReal_graf_data_index][i];
                if (mouse_x >= bounds->left && mouse_y >= bounds->top
                        && mouse_x < bounds->right && mouse_y < bounds->bottom) {
                    gKey_down = gReplay_control_key_codes[i];
                    break;
                }
            }
            if (i == 9 && !gUNK_006a23cc) {
                PickActionReplayManualCameraTarget(mouse_x, mouse_y, 0.0f);
            }
            gUNK_006a23cc = 1;
        } else {
            gUNK_006a23cc = 0;
        }
    } else {
        gMouse_in_use = 0;
    }

    if (gKey_down == 0x4b && (previous_key == -1 || previous_key == 4)) {
        MovieRecordButtonPressed();
    } else if (gKey_down == 0x4c) {
        gReplay_show_camera_icon = 1;
        if (previous_key != 0x4c) {
            ChangeCameraType();
            gReplay_camera_mode_change_time = PDGetTotalTime();
        }
    } else {
        gReplay_show_camera_icon = 0;
    }

    if ((gKey_down == 0x56 || gKey_down == 0x6a)
            && (previous_key == -1 || previous_key == 4)) {
        ARSetReplayPlayFlag(!ARGetReplayPlayFlag());
    } else if ((gKey_down == 0x51 || gKey_down == 0x32)
            && (previous_key == -1 || previous_key == 4)) {
        FUN_004023c0((float)-ARGetReplayDirection());
        if (ARGetReplayPlayFlag()) {
            ARSetReplayPlayFlag(0);
        }
    }

    if (gKey_down == 0x52 && (previous_key == -1 || previous_key == 4)) {
        ARSetReplayRate(-1.0f);
        FUN_004023c0(-1.0f);
        gSingle_frame_mode = 1;
    } else if (gKey_down == 0x54 && (previous_key == -1 || previous_key == 4)) {
        ARSetReplayRate(1.0f);
        FUN_004023c0(1.0f);
        gSingle_frame_mode = 1;
    } else if (gKey_down == 0x55 || gKey_down == 0x44) {
        if (ARGetReplayRate() >= gReplay_reverse_rate_adjust_limit) {
            ARSetReplayRate(-1.2f);
        }
        if (gReplay_rate_adjust_time != 0) {
            ARSetReplayRate((float)(ARGetReplayRate()
                - (double)(current_time - gReplay_rate_adjust_time) * gReplay_reverse_rate_adjust_step));
        }
        if (ARGetReplayRate() < gReplay_reverse_rate_min) {
            ARSetReplayRate(-8.0f);
        }
    } else if (gKey_down == 0x57 || gKey_down == 0x45) {
        if (ARGetReplayRate() < gReplay_forward_rate_adjust_limit) {
            ARSetReplayRate(1.2f);
        }
        if (gReplay_rate_adjust_time != 0) {
            ARSetReplayRate((float)(ARGetReplayRate()
                - (double)(current_time - gReplay_rate_adjust_time) * gReplay_forward_rate_adjust_step));
        }
        if (ARGetReplayRate() > gReplay_forward_rate_max) {
            ARSetReplayRate(8.0f);
        }
    } else if (gKey_down == 0x3b) {
        ARSetReplayRate(-1.0f);
        FUN_004023c0(-1.0f);
        ARSetReplayPlayFlag(0);
    } else if (gKey_down == 0x3a) {
        ARSetReplayRate(1.0f);
        FUN_004023c0(1.0f);
        ARSetReplayPlayFlag(0);
    } else if (ARGetReplayPlayFlag()) {
        ARSetReplayRate(0.0f);
    } else {
        ARSetReplayRate((float)ARGetReplayDirection());
    }

    if ((gKey_down == 0x58 || gKey_down == 0x42)
            && (previous_key == -1 || previous_key == 4)) {
        gTime_of_last_zappy_headups = 0;
        if (gNoTransients) {
            gRecording_mouse_disabled = 1;
        }
        RenderAFrame(1);
        RenderAFrame(1);
        FUN_00403c70(DoZappyActionReplayHeadups, PreProcess, PostProcess);
        if (gNoTransients) {
            gRecording_mouse_disabled = 0;
        }
        ARSetReplayRate(1.0f);
        MungeCarGraphics(gFrame_period);
        GrooveThoseDelics();
        ARSetReplayRate(0.0f);
        FUN_004023c0(1.0f);
        ARSetReplayPlayFlag(1);
    } else if ((gKey_down == 0x5a || gKey_down == 0x43)
            && (previous_key == -1 || previous_key == 4)) {
        gTime_of_last_zappy_headups = 0;
        if (gNoTransients) {
            gRecording_mouse_disabled = 1;
        }
        RenderAFrame(1);
        RenderAFrame(1);
        FUN_00403210(DoZappyActionReplayHeadups, PreProcess, PostProcess);
        if (gNoTransients && !FUN_004e2100()) {
            gRecording_mouse_disabled = 0;
        }
        ARSetReplayRate(-1.0f);
        MungeCarGraphics(gFrame_period);
        GrooveThoseDelics();
        ARSetReplayRate(0.0f);
        FUN_004023c0(-1.0f);
        ARSetReplayPlayFlag(1);
    }

    if (gUNK_006a2378 != gReplay_rate_zero) {
        ARSetReplayRate(gUNK_006a2378);
        gUNK_006a2378 = 0.0f;
    }
    if (ARGetReplayRate() * initial_replay_rate < gReplay_rate_zero) {
        gUNK_006a2378 = ARGetReplayRate();
        ARSetReplayRate(0.0f);
    } else {
        gUNK_006a2378 = 0.0f;
    }
    if (initial_replay_rate != gReplay_rate_zero) {
        gFrame_period = (tU32)(ARGetReplayRate() / initial_replay_rate * (float)gFrame_period);
    }
    if (fabs((double)ARGetReplayRate()) < gReplay_forward_rate_adjust_limit) {
        gReplay_rate_adjust_time = 0;
    } else {
        gReplay_rate_adjust_time = current_time;
    }
    if (initial_replay_rate <= gReplay_rate_zero && ARGetReplayRate() > gReplay_rate_zero) {
        FUN_00403d20();
        return;
    }
    if (initial_replay_rate >= gReplay_rate_zero && ARGetReplayRate() < gReplay_rate_zero) {
        FUN_00403d00();
    }
}

// FUNCTION: CARMA2_HW 0x00402360
int C2_HOOK_FASTCALL ARReplayIsReallyPaused(void) {

    return gReplay_rate == gFLOAT_00589330;
}

// FUNCTION: CARMA2_HW 0x004c80b0
void C2_HOOK_FASTCALL PipeSingleGrooveStop(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {

    gUNK_0069410c_u32 = *(tU32*)&pPath_resumption;
    ARDoSingleVariedSession(0xd, pGroove_index, 5,
        4, 0, pObject_interrupt,
        4, 4, pObject_interrupt,
        4, 8, *(tU32*)&pPath_resumption,
        4, 0xc, *(tU32*)&pPath_resumption,
        0x30, 0x10, pMatrix);
}

// ActualActionReplayHeadups

// FUNCTION: CARMA2_HW 0x004e6270
void C2_HOOK_FASTCALL DoActionReplayHeadups(void) {

    ActualActionReplayHeadups(0);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004023a0
void C2_HOOK_FASTCALL ARSetReplayRate(float rate) {
    gReplay_rate = rate;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004022f0
void C2_HOOK_FASTCALL ARSetReplayPlayFlag(int pFlag) {

    gReplay_play_flag = pFlag;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004022e0
void C2_HOOK_FASTCALL ARSetLastReplayFrameTime(tU32 pTime) {

    gLast_replay_frame_time = pTime;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00402380
int C2_HOOK_FASTCALL ARGetReplayPlayFlag(void) {

    return gReplay_play_flag;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004e7090
void C2_HOOK_FASTCALL InitializeActionReplayPix(void) {
    int i;

    for (i = 0; i < 11; i++) {
        gReplay_pixelmaps[i] = LoadPixelmap(gReplay_pixie_names[i]);
        BRPM_convert(gReplay_pixelmaps[i], gBack_screen->type);
        BrMapAdd(gReplay_pixelmaps[i]);
    }
}

// FUNCTION: CARMA2_HW 0x004e70d0
void C2_HOOK_FASTCALL DisposeReplayPixelmaps(void) {
    int i;

    for (i = 0; i < 11; i++) {
        if (gReplay_pixelmaps[i] != NULL) {
            BrMapRemove(gReplay_pixelmaps[i]);
            BrPixelmapFree(gReplay_pixelmaps[i]);
            gReplay_pixelmaps[i] = NULL;
        }
    }
}

// FUNCTION: CARMA2_HW 0x004e7110
void C2_HOOK_FASTCALL SynchronizeActionReplay(void) {

    while (ARGetReplayRate() != 0.f && (float)(PDGetTotalTime() - gLast_synch_time) < (float)gFrame_period / fabsf(ARGetReplayRate())) {
        ServiceGameInRace();
    }
    gLast_synch_time = PDGetTotalTime();
    if (gSingle_frame_mode) {
        ARSetReplayRate(0.f);
        gSingle_frame_mode = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004e6260
void C2_HOOK_FASTCALL DoActionReplayPostSwap(void) {

    RemoveTransientBitmaps(1);
}
// ActualActionReplayHeadups

// DoActionReplayPostSwap

// DoZappyActionReplayHeadups

// DoActionReplayHeadups

// PreProcess

// PostProcess

// DoActionReplay

// MoveToEndOfReplay

// MoveToStartOfReplay

// PollActionReplayControls

// CheckReplayTurnOn

// InitializeActionReplayPix

// FreeActionReplayPix

// SynchronizeActionReplay

// TurnOnActionReplay

// TurnOffActionReplay

// AfterActionReplay

// ToggleReplay

// DisplayMovieStatusPanel

// ApplyTime

// ARSetPause

