#include <stdlib.h>
#include "car.h"
#ifndef CARPOCALYPSE2_MATCHING
#include "errors.h"
#endif

#include "brucetrk.h"
#include "compress.h"
#include "controls.h"
#include "crush.h"

extern int gUnknown_006b9c60[100];
#include "depth.h"
#include "displays.h"
#include "drone.h"
#include "finteray.h"
#include "flap.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "input.h"
#include "loading.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pedestrn.h"
#include "physics.h"
#include "piping.h"
#include "platform.h"
#include "powerup.h"
#include "powerups.h"
#include "pratcam.h"
#include "racemem.h"
#include "raycast.h"
#include "replay.h"
#include "skidmark.h"
#include "smashing.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"
#include "world.h"

#include "brender/brender.h"
#include "brender/br_inline_funcs.h"
#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"

#include "c2_stdlib.h"
#include "c2_string.h"

#include "c2_math.h"
#include "car.h"
#include "joystick.h"

extern tU8 gNon_car_spec_indices[100];

// GLOBAL: CARMA2_HW 0x005895f0
double gZero = 1.0;

// GLOBAL: CARMA2_HW 0x00589508
const double gBrAngleToRadiansScale = 0.00009587379924285257;

// GLOBAL: CARMA2_HW 0x005895a8
const double gManualCameraPickScreenScale = 0.004166666666666667;

// GLOBAL: CARMA2_HW 0x005895f8
float gOne = 1.0f;

// GLOBAL: CARMA2_HW 0x0058f6a4
int gCar_to_car_collisions = 1;

// GLOBAL: CARMA2_HW 0x005896a8
float gF1000 = 1000.0f;

// GLOBAL: CARMA2_HW 0x00589694
float gFneg1000 = -1000.0f;

// GLOBAL: CARMA2_HW 0x005897fc
const br_scalar gFloat_005897fc = 0.1725f;

// GLOBAL: CARMA2_HW 0x00589800
const br_scalar gFloat_00589800 = -0.1725f;

// GLOBAL: CARMA2_HW 0x00589650
const br_scalar gFloat_00589650 = 0.5f;

// GLOBAL: CARMA2_HW 0x005896d0
const br_scalar gFloat_005896d0 = 200.0f;

// GLOBAL: CARMA2_HW 0x00589858
const double gDouble_00589858 = 1.2;

// GLOBAL: CARMA2_HW 0x00589860
const double gDouble_00589860 = 0.9;

// GLOBAL: CARMA2_HW 0x00589868
const br_scalar gFloat_00589868 = 0.2f;

// GLOBAL: CARMA2_HW 0x00589870
const double gDouble_00589870 = -0.7;

// GLOBAL: CARMA2_HW 0x00589878
const double gDouble_00589878 = 1.4285714285714286;

// GLOBAL: CARMA2_HW 0x00589880
const br_scalar gFloat_00589880 = 1.5f;

// GLOBAL: CARMA2_HW 0x00589888
const double gDouble_00589888 = 0.003;

// GLOBAL: CARMA2_HW 0x0058989c
const br_scalar gFlyCarPitchRate = 3.0f;

// GLOBAL: CARMA2_HW 0x00589918
const br_scalar gFlyCarReversePitchRate = -3.0f;

// GLOBAL: CARMA2_HW 0x0058991c
const br_scalar gFlyCarAccelerationRate = -10.0f;

// GLOBAL: CARMA2_HW 0x00589688
const double gFlyCarAngleDamping = 0.1449275362318841;

// GLOBAL: CARMA2_HW 0x005896b8
const br_scalar gFlyCarAccelerationRate2 = 10.0f;

// GLOBAL: CARMA2_HW 0x00589790
const double gDouble_00589790 = 1.4492753623188406;

// GLOBAL: CARMA2_HW 0x00589738
const double gDouble_00589738 = 0.05;

// GLOBAL: CARMA2_HW 0x005897a0
const double gDouble_005897a0 = -0.005;

// GLOBAL: CARMA2_HW 0x00589798
const double gDouble_00589798 = 0.057971014492753624;

// GLOBAL: CARMA2_HW 0x005897a8
const double gDouble_005897a8 = -0.05;

// GLOBAL: CARMA2_HW 0x005897b0
const double gDouble_005897b0 = 0.005;

// GLOBAL: CARMA2_HW 0x005895b0
const br_scalar gFloat_005895b0 = 500.0f;

// GLOBAL: CARMA2_HW 0x00589628
const double gDouble_00589628 = 6.9;

// GLOBAL: CARMA2_HW 0x00589638
const double gDouble_00589638 = 0.5;

// GLOBAL: CARMA2_HW 0x00589668
const br_scalar gFloat_00589668 = -1.0f;

// GLOBAL: CARMA2_HW 0x005896d8
const br_scalar gFloat_005896d8 = 7.0f;

// GLOBAL: CARMA2_HW 0x0058969c
const br_scalar gFloat_0058969c = 0.0005f;

// GLOBAL: CARMA2_HW 0x00589740
const double gDouble_00589740 = -0.01;

// GLOBAL: CARMA2_HW 0x00589808
const br_scalar gFloat_00589808 = 0.02f;

// GLOBAL: CARMA2_HW 0x00589810
const double gDouble_00589810 = 0.0000002384185791015625;

// GLOBAL: CARMA2_HW 0x00589818
const double gDouble_00589818 = 0.0001;

// GLOBAL: CARMA2_HW 0x00589820
const double gDouble_00589820 = 0.999;

// GLOBAL: CARMA2_HW 0x00589828
const double gDouble_00589828 = 0.98;

// GLOBAL: CARMA2_HW 0x00589830
const br_scalar gFloat_00589830 = 0.707107f;

// GLOBAL: CARMA2_HW 0x00589834
const br_scalar gFloat_00589834 = -0.707107f;

// GLOBAL: CARMA2_HW 0x00589838
const double gDouble_00589838 = 0.0009;

// GLOBAL: CARMA2_HW 0x00589840
const br_scalar gFloat_00589840 = -2.8985507f;

// GLOBAL: CARMA2_HW 0x00589844
const br_scalar gFloat_00589844 = 0.06f;

// GLOBAL: CARMA2_HW 0x00589848
const br_scalar gFloat_00589848 = 0.001f;

// GLOBAL: CARMA2_HW 0x0058984c
const br_scalar gFloat_0058984c = 0.0002f;

// GLOBAL: CARMA2_HW 0x00661678
const br_vector3 gVector3_00661678 = { { 0.f, 1.f, 0.f } };

// GLOBAL: CARMA2_HW 0x005895c0
const br_scalar gFlyCarZero = 0.0f;

// GLOBAL: CARMA2_HW 0x00589920
const double gFlyCarRadiansToAngle = 10430.37835047045;

// GLOBAL: CARMA2_HW 0x00589928
const br_scalar gFlyCarCollisionStep = -0.05f;

// GLOBAL: CARMA2_HW 0x0058992c
const br_scalar gFlyCarCollisionStep2 = -0.05f;

// GLOBAL: CARMA2_HW 0x00589930
const double gFlyCarRadiansToAngleNeg = -10430.37835047045;

// GLOBAL: CARMA2_HW 0x0058f704
int gFlyCarMode = 2;

// GLOBAL: CARMA2_HW 0x00679414
br_scalar gFlyCarAngularVelocity;

extern br_scalar gFloat_005896d4;

// GLOBAL: CARMA2_HW 0x0079ecc0
tPhysics_object* gDrivable_on_list[50];

// GLOBAL: CARMA2_HW 0x0079ed88
tU32 gDrivable_on_count;

typedef void (C2_HOOK_FAKE_THISCALL * tControl_car_fn)(tCar_spec* pCar_spec, undefined4 pArg2, float pT);

// GLOBAL: CARMA2_HW 0x0074a5f4
int gOver_shoot;

// GLOBAL: CARMA2_HW 0x00589a38
const double gDOUBLE_00589a38 = 0.14492753623188406;

// GLOBAL: CARMA2_HW 0x00589af8
const double gDOUBLE_00589af8 = 6.9;

// GLOBAL: CARMA2_HW 0x00589b00
const double gDOUBLE_00589b00 = 0.0054931640625;

// GLOBAL: CARMA2_HW 0x00589b08
const br_scalar gFLOAT_00589b08 = 180.0f;

// GLOBAL: CARMA2_HW 0x00589b0c
const br_scalar gFLOAT_00589b0c = 360.0f;

// GLOBAL: CARMA2_HW 0x00589b10
const br_scalar gFLOAT_00589b10 = 0.03f;

// GLOBAL: CARMA2_HW 0x0074c7c0
tCar_spec* gActive_car_list[139]; /* FIXME: uncertain about array length */

// GLOBAL: CARMA2_HW 0x0079ef2c
int gNum_cars_and_non_cars;

// GLOBAL: CARMA2_HW 0x0074c9ec
int gNum_active_cars;

// GLOBAL: CARMA2_HW 0x006793f4
static tU32 last_frame_start;

// GLOBAL: CARMA2_HW 0x006793a0
int gFreeze_mechanics;

// GLOBAL: CARMA2_HW 0x0079eda0
tNon_car_spec* gActive_non_car_list[99];

// GLOBAL: CARMA2_HW 0x006793dc
int gNum_active_non_cars;

// GLOBAL: CARMA2_HW 0x00679360
br_scalar gMin_world_y;

// GLOBAL: CARMA2_HW 0x006793e4
tPhysics_object* gUnknown_car_collision_info;

// GLOBAL: CARMA2_HW 0x00679268
br_vector3 gAverage_grid_position;


// GLOBAL: CARMA2_HW 0x0067939c
int gTesting_car_for_sensible_place;

// GLOBAL: CARMA2_HW 0x0065cf78
tWorld_callbacks gWorld_callbacks = {
    ProcessForcesCallback,
    ProcessJointForcesCallback,
    NewFacesListCallback,
    FindFacesInBox,
    PullActorFromWorld,
    StopGroovidelic,
    GetFrictionFromFace,
    NULL,
};


// GLOBAL: CARMA2_HW 0x0058f6e0
tPhysics_callbacks gCar_physics_callbacks = {
    &gWorld_callbacks,
    APTCPreCollision,
    APTCPostCollision,
    APTCChangedObjects,
    APTCActiveHalted,
    APTCPassiveActivated,
    NULL,
};

// GLOBAL: CARMA2_HW 0x006940b0
int gFace_count;

// GLOBAL: CARMA2_HW 0x00744820
tFace_ref gFace_list__car[300];

// GLOBAL: CARMA2_HW 0x00679294
int gCamera_mode;

// GLOBAL: CARMA2_HW 0x006792b8
int gCamera_frozen;

// GLOBAL: CARMA2_HW 0x006792bc
int gOpponent_viewing_mode;

// GLOBAL: CARMA2_HW 0x0058f5fc
int gNet_player_to_view_index = -1;

// GLOBAL: CARMA2_HW 0x00679304
br_angle gPanning_camera_angle;

// GLOBAL: CARMA2_HW 0x00679310
br_scalar gManual_camera_yaw;

// GLOBAL: CARMA2_HW 0x00679314
br_scalar gManual_camera_dolly;

// GLOBAL: CARMA2_HW 0x00679318
br_scalar gManual_camera_vertical;

// GLOBAL: CARMA2_HW 0x006792c8
tSave_camera gSave_camera[2];

// GLOBAL: CARMA2_HW 0x00655f40
br_scalar gCamera_zoom = 0.2f;

// GLOBAL: CARMA2_HW 0x0079efa4
tCamera_key_flags gCamera_key_flags;

// GLOBAL: CARMA2_HW 0x006792f4
undefined2 gUNK_006792f4;

// GLOBAL: CARMA2_HW 0x006792f8
int gUNK_006792f8;

// GLOBAL: CARMA2_HW 0x006792e8
br_vector3 gCamera_pos_before_collide;

// GLOBAL: CARMA2_HW 0x006792e0
br_angle gOld_yaw__car;

// GLOBAL: CARMA2_HW 0x006793e0
int gCamera_has_collided;

// GLOBAL: CARMA2_HW 0x006792e4
br_angle gOld_zoom;

// GLOBAL: CARMA2_HW 0x00679364
int gInTheSea;

// GLOBAL: CARMA2_HW 0x006793d4
int gDouble_pling_water;

// GLOBAL: CARMA2_HW 0x006793a8
tU32 gWild_start;

// GLOBAL: CARMA2_HW 0x006793ac
tU32 gQuite_wild_end;

// GLOBAL: CARMA2_HW 0x006793b0
tU32 gQuite_wild_start;

// GLOBAL: CARMA2_HW 0x006793b4
tU32 gOn_me_wheels_start;

// GLOBAL: CARMA2_HW 0x006793b8
tU32 gWoz_upside_down_at_all;

// GLOBAL: CARMA2_HW 0x00676854
int gStop_opponents_moving = 0;

// GLOBAL: CARMA2_HW 0x006793c0
int gSkid_tag[2];

// GLOBAL: CARMA2_HW 0x006793c8
tCar_spec* gLast_car_to_skid[2];

// GLOBAL: CARMA2_HW 0x00679390
br_vector3 gCar_to_view_original_v;

// GLOBAL: CARMA2_HW 0x006793a4
tU32 gLast_cunning_stunt;

// GLOBAL: CARMA2_HW 0x006792b4
br_actor* gPed_actor;

// GLOBAL: CARMA2_HW 0x0058f5e8
int gCamera_incident_flag = 1;

// GLOBAL: CARMA2_HW 0x0058f630
int gCamera_incident_mode = 3;

// GLOBAL: CARMA2_HW 0x0058f634
int gCamera_incident_direction = 1;

// GLOBAL: CARMA2_HW 0x006792a0
tU32 gIncident_car_data[4];

// GLOBAL: CARMA2_HW 0x006792b0
br_scalar gIncident_towards;

// GLOBAL: CARMA2_HW 0x0067932c
tS32 gIncident_time;

// GLOBAL: CARMA2_HW 0x00679330
tS32 gIncident_camera_stack;

// GLOBAL: CARMA2_HW 0x0067935c
int gCrash_noise_volume;

// GLOBAL: CARMA2_HW 0x00679410
int gCrash_noise_tag;

// GLOBAL: CARMA2_HW 0x006792c0
tU32 gSwitch_time;

// GLOBAL: CARMA2_HW 0x00589440
const br_scalar gFLOAT_00589440 = 1.0f;

// GLOBAL: CARMA2_HW 0x00589410
const br_scalar gManual_camera_turn_step = 0.00025f;

// GLOBAL: CARMA2_HW 0x00589414
const br_scalar gManual_camera_reverse_step = -0.00025f;

// GLOBAL: CARMA2_HW 0x00589418
const br_scalar gFLOAT_00589418 = 0.0f;

// GLOBAL: CARMA2_HW 0x0058941c
const br_scalar gManual_camera_min_height = -10.0f;

// GLOBAL: CARMA2_HW 0x00589428
const double gHalf_00589428 = 0.5;

// GLOBAL: CARMA2_HW 0x00589430
const br_scalar gFLOAT_00589430 = 2.384185791015625e-7f;

// GLOBAL: CARMA2_HW 0x00589438
const double gOne_00589438 = 1.0;

// GLOBAL: CARMA2_HW 0x00589448
const double gRadiansToAngle_00589448 = 10430.378350470453;

// GLOBAL: CARMA2_HW 0x00589478
const double gDOUBLE_00589478 = 102.91955471539592;

// GLOBAL: CARMA2_HW 0x0058948c
const br_scalar gFLOAT_0058948c = 25.0f;

// GLOBAL: CARMA2_HW 0x005894a8
const double gDOUBLE_005894a8 = 205.83910943079184;

// GLOBAL: CARMA2_HW 0x00589400
const double gDegreesToAngle_00589400 = 182.04444444444445;

// GLOBAL: CARMA2_HW 0x00589460
const double gSwing_forward_speed_threshold = 0.0001;

// GLOBAL: CARMA2_HW 0x005894f0
const double gSwing_reverse_speed_threshold = 0.0006;

// GLOBAL: CARMA2_HW 0x00589510
const float gSwing_milliseconds_to_seconds = 0.001f;

// GLOBAL: CARMA2_HW 0x00589528
const double gSwing_fast_acceleration = 0.06;

// GLOBAL: CARMA2_HW 0x00589530
const double gSwing_slow_acceleration = 0.04;

// GLOBAL: CARMA2_HW 0x00589538
const double gSwing_reverse_acceleration_time = 200.0;

// GLOBAL: CARMA2_HW 0x00589540
const float gSwing_negative_min_angular_speed = -0.0872664600610733f;

// GLOBAL: CARMA2_HW 0x00589548
const double gSwing_initial_acceleration_time = 50.0;

// GLOBAL: CARMA2_HW 0x00589550
const double gSwing_turn_dot_threshold = -0.7;

// GLOBAL: CARMA2_HW 0x00589558
const double gSwing_turn_speed_threshold = 0.8;

// GLOBAL: CARMA2_HW 0x00589560
const double gSwing_fast_max_degrees = 40.0;

// GLOBAL: CARMA2_HW 0x00589568
const double gSwing_slow_max_degrees = 10.0;

// GLOBAL: CARMA2_HW 0x0058f6b0
const float gCar_simplification_factor[2][5] = {
    { 20.0f, 3.0f, 1.5f, 0.75f, 0.0f },
    { 50.0f, 5.0f, 2.5f, 1.50f, 0.0f }
};

// GLOBAL: CARMA2_HW 0x0058f660
tControl_car_fn ControlCar[6] = {
    ControlCar1,
    ControlCar2,
    ControlCar3,
    ControlCar4,
    ControlCar5,
    NULL,
};

// GLOBAL: CARMA2_HW 0x0058f678
int gControl__car = 3;

// GLOBAL: CARMA2_HW 0x0058f680
const int gCrash_noise_sound_ids[5] = {
    5000,
    5001,
    5002,
    5003,
    5004,
};

// GLOBAL: CARMA2_HW 0x0058f698
const int gINT_0058f698[3] = {
    5010,
    5011,
    5012,
};

// GLOBAL: CARMA2_HW 0x0058f6a8
float gFloat_0058f6a8 = 0.2f;

// GLOBAL: CARMA2_HW 0x00679400
int gINT_00679400;

// GLOBAL: CARMA2_HW 0x00679404
int gINT_00679404;

// GLOBAL: CARMA2_HW 0x00763500
int gCunning_stunt_bonus[3];

// GLOBAL: CARMA2_HW 0x0074d1c0
float gArmour_starting_value[100];

// GLOBAL: CARMA2_HW 0x00761f60
float gPower_starting_value[100];

// GLOBAL: CARMA2_HW 0x00761d40
float gOffensive_starting_value[100];

// GLOBAL: CARMA2_HW 0x0075ba20
tFloat_bunch_info gCar_softness;

// GLOBAL: CARMA2_HW 0x0074d600
tFloat_bunch_info gCar_car_damage_multiplier;

// GLOBAL: CARMA2_HW 0x0068b8d0
br_vector3 gZero_v__car;  // FIXME: make const?

static int CheckForWall(br_vector3* start, br_vector3* end) {
    br_vector3 dir;
    br_material* material;
    br_vector3 normal;
    br_scalar d;

    BrVector3Sub(&dir, end, start);
    FindFace(start, &dir, &normal, &d, &material);
    return d <= 1.f;
}

// FUNCTION: CARMA2_HW 0x004105f0
void C2_HOOK_FASTCALL SetUpPanningCamera(tCar_spec* c) {
    br_vector3 pos;
    br_vector3 perp;
    br_vector3 dir;
    br_vector3 normal;
    br_scalar d;
    br_material* material;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 left;
    br_vector3 right;
    br_vector3 car_centre;
    br_scalar ts;
    tU32 time;
    tU32 time_step;
    tU32 t;
    tU32 t2;
    br_matrix34* m1;

    m1 = &c->car_master_actor->t.t.mat;
    gCamera_incident_flag = 0;
    ScanCarsPositions(c, &c->pos, 411.678222f, -1, 5000, &pos, &time);
    BrVector3Sub(&dir, &pos, &c->pos);
    if (GetTotalTime() < time) {
        time_step = time - GetTotalTime();
    } else {
        time_step = GetTotalTime() - time;
    }
    time_step *= SRandomBetween(0.8f, 1.5f);
    if (BrVector3LengthSquared(&dir) < 0.01 || time == 0) {
        BrVector3Negate(&dir, (br_vector3*)&m1->m[2][0]);
        BrVector3Copy(&car_centre, &c->pos);
        time_step = 0;
    } else {
        ScanCarsPositions(c, &c->pos, 102.919556f, -1, time_step / 2, &car_centre, &t);
        if (t == 0) {
            BrVector3Copy(&car_centre, &c->pos);
        }
    }
    BrVector3SetFloat(&tv, 0.f, 1.f, 0.f);
    BrVector3Cross(&perp, &tv, &dir);
    ts = BrVector3Length(&perp);
    if (ts < 0.1f) {
        return;
    }
    ts = 2.f / ts;
    ts *= SRandomBetween(0.33333334f, 1.f);
    BrVector3Scale(&perp, &perp, ts);
    ts = SRandomBetween(0.33333334f, 1.f) * 2.f;
    BrVector3Set(&tv, 0.f, ts, 0.f);
    BrVector3Add(&tv, &car_centre, &tv);
    BrVector3Add(&left, &tv, &perp);
    BrVector3Sub(&right, &tv, &perp);
    CollideCamera2(&car_centre, &left, NULL, 1, NULL);
    CollideCamera2(&car_centre, &right, NULL, 1, NULL);
    BrVector3Sub(&tv, &left, &car_centre);
    BrVector3Sub(&tv2, &right, &car_centre);
    if (BrVector3LengthSquared(&tv) + SRandomPosNeg(.01f) > BrVector3LengthSquared(&tv2)) {
        BrVector3Copy(&gCamera->t.t.translate.t, &left);
    } else {
        BrVector3Copy(&gCamera->t.t.translate.t, &right);
    }
    if (time != 0) {
        if (CheckForWall(&c->pos, &gCamera->t.t.translate.t)) {
            ScanCarsPositions(c, &c->pos, 10000.f, -1, 1000, &tv, &t2);
            CollideCamera2(&tv, &gCamera->t.t.translate.t, NULL, 1, NULL);
        }
    }
    if (time != 0) {
        if (CheckForWall(&pos, &gCamera->t.t.translate.t)) {
            time_step = time_step / 16;
            BrVector3Copy(&tv, &car_centre);
            do {
                ScanCarsPositions(c, &tv, 10000.f, abs((tS32)t - (tS32)GetTotalTime()), time_step, &tv2, &t2);
                t += (tS32)(ARGetReplayDirection() ? 1 : -1) * time_step;
                BrVector3Copy(&tv, &tv2);
            } while (!CheckForWall(&tv, &gCamera->t.t.translate.t) && t < GetTotalTime() + 5000);
            gSwitch_time = t;
            return;
        }
    }
    if (time == 0) {
        time = 5000;
    }
    gSwitch_time = time;
}

// FUNCTION: CARMA2_HW 0x004122b0
int C2_HOOK_FASTCALL CollideCamera2(br_vector3* car_pos, br_vector3* cam_pos, br_vector3* old_camera_pos, int manual_move, tPhysics_object *collision_info) {
    int i;
    br_vector3 cam_hither_pos;
    br_vector3 car_coll_pos;
    br_vector3 dir;
    br_scalar ts;
    br_scalar dist;
    br_scalar hither;
    tBounds bnds;
    br_actor* actor;
    br_matrix34 mat;
    br_vector3 tv;
    tFace_ref face_list[6];
    br_scalar face_dots[6];
    int count_min_dots;
    int index_min_dots[3];
    int loop_done;
    int count;
    br_vector3 nor;
    br_vector3 delta;

    hither = ((br_camera*)gCamera->type_data)->hither_z * 3.0f;
    gCamera_has_collided = 0;
    loop_done = 0;
    for (;;) {
        br_vector3 new_cam_pos;
        br_vector3 tv2;
        br_vector3 p1;
        br_vector3 p2;
        br_vector3 nor_mod;
        br_material* material;
        br_scalar cos_coll;
        br_scalar delta_hither;
        br_scalar nor_factor;
        int i;

        BrVector3Sub(&delta, cam_pos, car_pos);
        dist = BrVector3Length(&delta);
        BrVector3Scale(&delta, &delta, 1.25f);
        ActorFindFace(car_pos, &delta, gTrack_actor, &nor, &ts, &material, &actor);
        if (ts >= 1.0) {
            BrVector3Copy(&new_cam_pos, cam_pos);
        } else {
            gCamera_has_collided = 1;
            BrVector3Scale(&tv, &delta, ts);
            BrVector3Add(&new_cam_pos, car_pos, &tv);
        }
        BrVector3Normalise(&dir, &delta);

        BrVector3Scale(&tv2, &dir, -hither);
        BrVector3Add(&cam_hither_pos, &new_cam_pos, &tv2);
        BrVector3Add(&car_coll_pos, car_pos, &delta);

        for (i = 0; i < gNum_active_non_cars; i++) {
            tNon_car_spec* non_car = gActive_non_car_list[i];

            if (non_car->flags & 0x80000) {
                br_scalar factor;
                br_vector3 temp_normal;

                DrMatrix34ApplyLPInverse(&p1, &cam_hither_pos, &non_car->collision_info->actor->t.t.mat);
                DrMatrix34ApplyLPInverse(&p2, &car_coll_pos, &non_car->collision_info->actor->t.t.mat);
                if (ShapeRayCast(&p1, &p2, non_car->collision_info->shape, &new_cam_pos, &factor, &temp_normal)) {
                    gCamera_has_collided = 1;
                    DrMatrix34ApplyLPInverse(&p1, car_pos, &non_car->collision_info->actor->t.t.mat);
                    DrMatrix34ApplyLPInverse(&p2, &car_coll_pos, &non_car->collision_info->actor->t.t.mat);
                    if (ShapeRayCast(&p1, &p2, non_car->collision_info->shape, &new_cam_pos, &factor, &temp_normal) && factor < ts) {
                        BrVector3Copy(&nor, &temp_normal);
                        actor = NULL;
                        BrMatrix34ApplyP(&tv, &new_cam_pos, &non_car->collision_info->actor->t.t.mat);
                        BrVector3Copy(&new_cam_pos, &tv);
                    }
                }
            }
        }
        if (ts > 1.f) {
            break;
        }
        cos_coll = BrVector3Dot(&nor, &delta);
        if (cos_coll <= 0.f) {
            cos_coll = -cos_coll;
        } else {
            BrVector3Negate(&nor, &nor);
        }
        if (loop_done) {
            break;
        }
        delta_hither = hither + cos_coll / 1.25f - ts * cos_coll;
        if (delta_hither <= 0.f) {
            break;
        }
        BrVector3Copy(&nor_mod, &nor);
        nor_mod.v[1] += nor.v[1] >= 0.7f ? 3.f : -3.f;
        nor_factor = fabsf(nor.v[1]) * delta_hither / BrVector3Length(&nor_mod);
        BrVector3Scale(&tv, &nor_mod, nor_factor);
        BrVector3Accumulate(cam_pos, &tv);

        BrVector3Sub(&delta, cam_pos, car_pos);
        BrVector3Normalise(&delta, &delta);
        BrVector3Scale(&delta, &delta, dist);
        BrVector3Add(cam_pos, car_pos, &delta);
        loop_done = 1;
    }
    if (ts <= 1.f) {
        br_scalar dt;
        br_scalar l;

        gCamera_has_collided = 1;
        if (actor != NULL && actor->identifier != NULL && actor->identifier[0] == '&') {
            br_vector3 tv;

            BrMatrix34ApplyV(&tv, &nor, &actor->t.t.mat);
            BrVector3Copy(&nor, &tv);
        }
        dt = -(hither / BrVector3Dot(&nor, &delta));
        ts -= dt;
        if (ts * dist < 1.f) {
            ts += dt - 0.001f;
            if (ts >= 1.f / dist) {
                ts = 1.f / dist;
            }
        }
        if (ts > .8f) {
            ts = .8f;
        }
        BrVector3Scale(&delta, &delta, ts);
        l = BrVector3LengthSquared(&delta);
        BrVector3Add(cam_pos, car_pos, &delta);
        if ((float)sqrt(l) < gMin_camera_car_distance && !loop_done) {
            br_scalar a;
            br_scalar b;
            br_scalar discr;

            BrVector3Scale(&tv, &nor, -nor.v[1]);
            tv.v[1] += 1.f;
            if (gProgram_state.current_car.car_master_actor->t.t.mat.m[1][1] < 0.f) {
                BrVector3Negate(&tv, &tv);
            }
            a = BrVector3LengthSquared(&tv);
            b = BrVector3Dot(&tv, &delta);
            discr = CARPOCALYPSE2_SQR(b) - 4.f * a * (l - CARPOCALYPSE2_SQR(gMin_camera_car_distance));
            if (discr >= 0.f && a != 0.f) {
                br_material* material;
                br_vector3 tv2;
                br_vector3 tv3;
                br_scalar f;
                br_scalar dot;

                f = (sqrtf(discr) - b) / (2 * a);
                BrVector3Scale(&tv, &tv, f);
                FindFace(cam_pos, &tv, &nor, &ts, &material);
                if (ts < 1.f) {
                    BrVector3Scale(&tv, &tv, ts);
                }
                BrVector3Copy(&tv2, &delta);
                tv2.v[1] = 0.f;
                BrVector3Normalise(&tv3, &tv2);
                BrVector3Accumulate(&delta, &tv);
                dot = BrVector3Dot(&tv3, &delta);
                if (dot < .03f && !gAction_replay_mode) {
                    br_scalar dot2;

                    BrVector3Normalise(&tv, &tv);
                    dot2 = BrVector3Dot(&tv3, &tv);
                    if (dot2 < -.03f) {
                        a = (.03f - dot) / dot2;
                        BrVector3Scale(&tv, &tv, a);
                        BrVector3Accumulate(&delta, &tv);
                    }
                }
            }
            BrVector3Add(cam_pos, car_pos, &delta);
        }
    }
    bnds.mat = &mat;
    BrMatrix34Identity(&mat);
    BrVector3Set(&tv, hither, hither, hither);
    BrVector3Sub(&bnds.original_bounds.min, cam_pos, &tv);
    BrVector3Add(&bnds.original_bounds.max, cam_pos, &tv);
    count = FindFacesInBox(&bnds, face_list, CARPOCALYPSE2_ASIZE(face_list), NULL);
    for (i = 0; i < count; i++) {
        tFace_ref* fr = &face_list[i];
        if ((fr->material->flags & BR_MATF_TWO_SIDED) && BrVector3Dot(&fr->normal, &delta) > 0.f) {
            BrVector3Negate(&fr->normal, &fr->normal);
        }
        BrVector3Sub(&tv, cam_pos, &fr->v[0]);
        face_dots[i] = BrVector3Dot(&tv, &fr->normal);
    }

    for (i = 0; i < CARPOCALYPSE2_ASIZE(index_min_dots); i++) {
        int j;
        float min_dot = 100.f;
        int index_min_dot;

        for (j = 0; j < count; j++) {
            if (face_dots[j] > 0.f && face_dots[j] < min_dot) {
                min_dot = face_dots[j];
                index_min_dot = j;
            }
        }
        if (min_dot == 100.f) {
            break;
        }
        index_min_dots[i] = index_min_dot;
        face_dots[index_min_dot] = -100.f;
    }
    count_min_dots = i;
    if (count_min_dots >= 1) {
        br_scalar d;

        BrVector3Sub(&tv, cam_pos, &face_list[index_min_dots[0]].v[0]);
        d = BrVector3Dot(&face_list[index_min_dots[0]].normal, &tv);
        if (d < hither) {
            d = hither - d;
            BrVector3Scale(&tv, &face_list[index_min_dots[0]].normal, d);
            BrVector3Accumulate(cam_pos, &tv);
        }
        if (count_min_dots >= 2) {
            int o_i = 1;

            d = BrVector3Dot(&face_list[index_min_dots[0]].normal, &face_list[index_min_dots[1]].normal);
            if (d > .95f && count_min_dots >= 3) {
                o_i = 2;
                d = BrVector3Dot(&face_list[index_min_dots[0]].normal, &face_list[index_min_dots[2]].normal);
            }
            if (d <= .95f) {
                br_vector3 tv;

                BrVector3Sub(&tv, cam_pos, &face_list[index_min_dots[o_i]].v[0]);
                d = BrVector3Dot(&face_list[index_min_dots[o_i]].normal, &tv);
                if (d < hither) {
                    br_scalar a;

                    a = BrVector3Dot(&face_list[index_min_dots[0]].normal, &face_list[index_min_dots[o_i]].normal);
                    BrVector3Scale(&tv, &face_list[index_min_dots[0]].normal, a);
                    BrVector3Sub(&face_list[index_min_dots[o_i]].normal, &face_list[index_min_dots[o_i]].normal, &tv);
                    BrVector3Scale(&tv, &face_list[index_min_dots[0]].normal, hither - d);
                    BrVector3Accumulate(cam_pos, &tv);
                }
            }
            if (count_min_dots >= 3) {
                br_vector3 tv;
                br_scalar d;

                BrVector3Sub(&tv, cam_pos, &face_list[index_min_dots[o_i]].v[0]);
                d = BrVector3Dot(&face_list[index_min_dots[o_i]].normal, &tv);
                if (d < hither && d >= 0.f) {
                    BrVector3Cross(&tv, &face_list[index_min_dots[0]].normal, &face_list[index_min_dots[o_i]].normal);
                    if (BrVector3Dot(&tv, &face_list[index_min_dots[2]].normal) < 0.f) {
                        BrVector3Negate(&tv, &tv);
                    }
                    if (d > .5f) {
                        br_scalar a;

                        a = (hither - d) / (BrVector3Dot(&face_list[index_min_dots[2]].normal, &tv));
                        BrVector3Scale(&tv, &tv, a);
                        BrVector3Accumulate(cam_pos, &tv);
                    }
                }
            }
        }
        if (collision_info != NULL && gAction_replay_camera_mode != kActionReplayCameraMode_Manual) {
#ifndef CARPOCALYPSE2_MATCHING
            /* port: each pass snaps the camera onto the car shape and steps it out by exactly one
             * hither distance, so the next ray ends on the surface at t == 1. With x87 precision the
             * original sees t > 1 and stops; with SSE single precision it can stay a hit forever. */
            int guard = 0;
            for (; guard < 16; guard++) {
#else
            for (;;) {
#endif
                br_vector3 p1;
                br_vector3 p2;
                br_vector3 new_cam_pos;
                br_vector3 temp_normal;
                br_vector3 pos_screen;
                int num_collisions;

                BrVector3Sub(&delta, cam_pos, car_pos);
                BrVector3Normalise(&delta, &delta);
                BrVector3Scale(&delta, &delta, hither);
                BrVector3Sub(&pos_screen, cam_pos, &delta);
                DrMatrix34ApplyLPInverse(&p1, cam_pos, &collision_info->actor->t.t.mat);
                DrMatrix34ApplyLPInverse(&p2, &pos_screen, &collision_info->actor->t.t.mat);
                num_collisions = ShapeRayCast(&p1, &p2, collision_info->shape, &new_cam_pos, &ts, &temp_normal);
                if (num_collisions == 0) {
                    break;
                }
                if (num_collisions > 0) {
                    BrMatrix34ApplyP(cam_pos, &new_cam_pos, &collision_info->actor->t.t.mat);
                }
                BrVector3Accumulate(cam_pos, &delta);
            }
        }
    }
    return loop_done;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0041f4a0
void C2_HOOK_FASTCALL PutOpponentsInNeutral(void) {
    gStop_opponents_moving = !gStop_opponents_moving;
    if (gStop_opponents_moving) {
        NewTextHeadupSlot(4, 0, 3000, -4, "Opponents in neutral");
    } else {
        NewTextHeadupSlot(4, 0, 3000, -4, "Back in gear");
    }
}

// C2PORT-NONMATCHING: best 46.34% | difftest PASS 2000 trials (spec difftest/specs/0041f4f0_FlyCar.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x0041f4f0
void C2_HOOK_FAKE_THISCALL FlyCar(tCar_spec* c, undefined4 pArg2, br_scalar dt) {
    br_matrix34* mat;
    br_vector3 position;
    br_vector3 ray_direction;
    br_vector3 normal;
    br_vector3 normal2;
    br_scalar ray_distance;
    br_scalar ray_distance2;
    br_scalar fly_rate;
    br_material* material;
    br_material* material2;
    int key_0;
    register int reverse_key;
    register int previous_mode;
    register int axis;
    int i;

    CARPOCALYPSE2_THISCALL_UNUSED(pArg2);

    mat = &c->collision_info->actor->t.t.mat;
    key_0 = PDKeyDown(0);
    if (key_0 == 0 && PDKeyDown(1) != 0) {
        reverse_key = 1;
    } else {
        reverse_key = 0;
    }
    previous_mode = gFlyCarMode;

    if ((*(tU32*)&gProgram_state.current_car.keys & 0x40000) == 0
            && gProgram_state.current_car.joystick.acc < 1
            && (*(tU32*)&gProgram_state.current_car.keys & 0x80000) == 0
            && gProgram_state.current_car.joystick.dec < 1) {
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x200000) != 0 && key_0 != 0) {
            fly_rate = dt * gFlyCarPitchRate;
            gFlyCarMode = 1;
            goto apply_fly_rate;
        }
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x400000) != 0 && key_0 != 0) {
            fly_rate = dt * gFlyCarReversePitchRate;
            gFlyCarMode = 1;
            goto apply_fly_rate;
        }
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x10000) != 0 && reverse_key) {
            fly_rate = dt * gFlyCarReversePitchRate;
            gFlyCarMode = 0;
            goto apply_fly_rate;
        }
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x20000) != 0 && reverse_key) {
            fly_rate = dt * gFlyCarPitchRate;
            gFlyCarMode = 0;
            goto apply_fly_rate;
        }
        if (dt * gFlyCarPitchRate <= gFlyCarAngularVelocity
                || gFlyCarAngularVelocity <= dt * gFlyCarReversePitchRate) {
            fly_rate = (gFlyCarAngularVelocity / fabsf(gFlyCarAngularVelocity)) * dt * gFlyCarPitchRate;
            goto apply_fly_delta;
        } else {
            gFlyCarAngularVelocity = 0.0f;
            fly_rate = 0.0f;
        }
    } else {
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x40000) != 0
                || gProgram_state.current_car.joystick.acc >= 1) {
            fly_rate = dt * gFlyCarAccelerationRate;
        } else {
            fly_rate = dt * gFlyCarAccelerationRate2;
        }
        gFlyCarMode = 2;
        goto apply_fly_rate;
    }
    goto apply_fly_delta;

apply_fly_rate:
    fly_rate *= (br_scalar)gFlyCarAngleDamping;
apply_fly_delta:
    gFlyCarAngularVelocity = gFlyCarAngularVelocity - fly_rate;

    if (gFlyCarMode != previous_mode) {
        gFlyCarAngularVelocity = 0.0f;
    }
    if ((*(tU32*)&gProgram_state.current_car.keys & 0x100000) != 0) {
        gFlyCarAngularVelocity = 0.0f;
    }

    axis = key_0 != 0 ? 2 : 1;
    if (key_0 == 0) {
        c->collision_info->omega.v[2] = 0.0f;
    } else {
        c->collision_info->omega.v[1] = 0.0f;
    }
    if (((*(tU32*)&gProgram_state.current_car.keys & 0x10000) == 0
            && gProgram_state.current_car.joystick.left < 1) || reverse_key) {
        if (((*(tU32*)&gProgram_state.current_car.keys & 0x20000) == 0
                && gProgram_state.current_car.joystick.right < 1) || reverse_key) {
            c->collision_info->omega.v[axis] = 0.0f;
        } else {
            c->collision_info->omega.v[axis] -= dt;
        }
    } else {
        c->collision_info->omega.v[axis] += dt;
    }
    if ((*(tU32*)&gProgram_state.current_car.keys & 0x200000) == 0 || key_0 != 0) {
        if ((*(tU32*)&gProgram_state.current_car.keys & 0x400000) == 0 || key_0 != 0) {
            c->collision_info->omega.v[0] = 0.0f;
        } else {
            c->collision_info->omega.v[0] -= dt;
        }
    } else {
        c->collision_info->omega.v[0] += dt;
    }

    if (c->collision_info->omega.v[0] != gFlyCarZero) {
        BrMatrix34PreRotateX(mat, (br_angle)(int)(c->collision_info->omega.v[0] * dt * gFlyCarRadiansToAngle));
    }
    if (c->collision_info->omega.v[1] != gFlyCarZero) {
        BrVector3Copy(&position, (br_vector3*)mat->m[3]);
        mat->m[3][0] -= position.v[0];
        mat->m[3][1] -= position.v[1];
        mat->m[3][2] -= position.v[2];
        BrMatrix34PostRotateY(mat, (br_angle)(int)(c->collision_info->omega.v[1] * dt * gFlyCarRadiansToAngle));
        mat->m[3][0] += position.v[0];
        mat->m[3][1] += position.v[1];
        mat->m[3][2] += position.v[2];
    }
    if (c->collision_info->omega.v[2] != gFlyCarZero) {
        BrMatrix34PreRotateZ(mat, (br_angle)(int)(c->collision_info->omega.v[2] * dt * gFlyCarRadiansToAngle));
    }

    c->collision_info->v.v[0] = -gFlyCarAngularVelocity * mat->m[gFlyCarMode][0];
    c->collision_info->v.v[1] = -gFlyCarAngularVelocity * mat->m[gFlyCarMode][1];
    c->collision_info->v.v[2] = -gFlyCarAngularVelocity * mat->m[gFlyCarMode][2];
    mat->m[3][0] += c->collision_info->v.v[0] * dt;
    mat->m[3][1] += c->collision_info->v.v[1] * dt;
    mat->m[3][2] += c->collision_info->v.v[2] * dt;

    if ((*(tU32*)&gProgram_state.current_car.keys & 0x800000) != 0 && key_0 == 0) {
        br_angle yaw;

        BrVector3Copy(&position, (br_vector3*)mat->m[3]);
        yaw = (br_angle)(0x3ffc - (int)(atan2(mat->m[2][2], mat->m[2][0]) * gFlyCarRadiansToAngle));
        BrMatrix34RotateY(mat, yaw);
        mat->m[3][0] = position.v[0];
        mat->m[3][1] = position.v[1];
        mat->m[3][2] = position.v[2];

        BrVector3SetFloat(&ray_direction, 0.0f, -10.0f, 0.0f);
        BrVector3Copy(&position, (br_vector3*)mat->m[3]);
        ActorFindFace(&position, &ray_direction, gTrack_actor, &normal, &ray_distance, &material, NULL);
        position.v[1] -= gFlyCarAccelerationRate;
        ActorFindFace(&position, &ray_direction, gTrack_actor, &normal2, &ray_distance2, &material2, NULL);
        if (ray_distance2 <= gOne) {
            BrVector3SetFloat(&ray_direction, 0.0f, -0.501f, 0.0f);
            i = 0;
            do {
                ActorFindFace(&position, &ray_direction, gTrack_actor, &normal2, &ray_distance2, &material, NULL);
                if (ray_distance2 <= gOne) {
                    break;
                }
                i++;
            } while (i < 20);
            ray_distance2 = ray_distance2 * gFlyCarCollisionStep - (float)(i + 1) * gFlyCarCollisionStep2;
        }
        if (ray_distance2 < ray_distance) {
            ray_distance = -ray_distance2;
        }
        if (fabsf(ray_distance) <= gOne) {
            br_scalar dot;

            mat->m[3][1] -= ray_distance * gFlyCarAccelerationRate2;
            dot = mat->m[2][0] * normal.v[0] + mat->m[2][2] * normal.v[2];
            dot = dot + mat->m[2][1] * normal.v[1];
            BrMatrix34PreRotateX(mat, (br_angle)(int)(atan(dot) * gFlyCarRadiansToAngle));
            dot = mat->m[0][0] * normal.v[0] + mat->m[0][1] * normal.v[1];
            dot = dot + mat->m[0][2] * normal.v[2];
            BrMatrix34PreRotateZ(mat, (br_angle)(int)(atan(dot) * gFlyCarRadiansToAngleNeg));
        }
    } else if ((*(tU32*)&gProgram_state.current_car.keys & 0x800000) != 0) {
        BrMatrix34PreRotateX(mat, (br_angle)(int)(atan(mat->m[2][1]) * gFlyCarRadiansToAngle));
        BrMatrix34PreRotateZ(mat, (br_angle)(int)(atan(mat->m[0][1]) * gFlyCarRadiansToAngleNeg));
    }

    c->direction.v[0] = -mat->m[2][0];
    c->direction.v[1] = -mat->m[2][1];
    c->direction.v[2] = -mat->m[2][2];
    BrMatrix34ApplyP(&c->pos, &c->collision_info->cmpos, mat);
    UpdateChildTransforms(c->collision_info, NULL);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x0041fbe0
float C2_HOOK_FASTCALL GetCarOverallBoundsMinY(tCar_spec* pCar) {
    float rear_min;
    float front_min;
    float min_value;

    rear_min = (pCar->wpos[0].v[1] - pCar->susp_height[0]) * (1.0f / WORLD_SCALE);
    front_min = (pCar->wpos[2].v[1] - pCar->susp_height[1]) * (1.0f / WORLD_SCALE);
    min_value = pCar->collision_info->bb1.min.v[1];

    min_value = MIN(min_value, rear_min);
    min_value = MIN(min_value, front_min);
    return min_value;
}

// C2PORT-NONMATCHING: best 41.6% | difftest PASS 2000 trials (spec 0041fc60_SetCarSuspGiveAndHeight.json) | reason formulas and writes match; remaining diff is VC5 x87 scheduling
// FUNCTION: CARMA2_HW 0x0041fc60
void C2_HOOK_FAKE_THISCALL SetCarSuspGiveAndHeight(tCar_spec* pCar, float pFront_give_factor, float pRear_give_factor, float pDamping_factor, float pExtra_front_height, float pExtra_rear_height) {
    float ratio;
    float front_give;
    float rear_give;
    float damping;
    float rear_min;
    float front_min;
    float min_value;

#define UNK_SUSPENION_FACTOR 5.0f

    front_give = pCar->susp_give * pFront_give_factor * WORLD_SCALE;
    rear_give = pCar->steerable_suspension_give * pRear_give_factor * WORLD_SCALE;
    damping = pCar->damping * pDamping_factor;
    ratio = fabsf((pCar->wpos[0].v[2] - pCar->centre_of_mass_world_scale.v[2]) / (pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2]));
    pCar->sk[0] = (pCar->collision_info->M / (ratio + 1.0f)) * UNK_SUSPENION_FACTOR / rear_give;
    pCar->sb[0] = (pCar->collision_info->M / (ratio + 1.0f)) * sqrtf(UNK_SUSPENION_FACTOR) / sqrtf(rear_give);
    ratio = 1.0f / ratio;
    pCar->sk[1] = (pCar->collision_info->M / (ratio + 1.0f)) * UNK_SUSPENION_FACTOR / front_give;
    pCar->sb[1] = (pCar->collision_info->M / (ratio + 1.0f)) * sqrtf(UNK_SUSPENION_FACTOR) / sqrtf(front_give);

    pCar->sb[0] *= damping;
    pCar->sb[1] *= damping;
    pCar->susp_height[0] = pCar->ride_height + rear_give + pExtra_rear_height;
    pCar->susp_height[1] = pCar->ride_height + front_give + pExtra_front_height;

    rear_min = (pCar->wpos[0].v[1] - pCar->susp_height[0]) / WORLD_SCALE;
    front_min = (pCar->wpos[2].v[1] - pCar->susp_height[1]) / WORLD_SCALE;
    min_value = pCar->collision_info->bb1.min.v[1];
    min_value = MIN(min_value, rear_min);
    min_value = MIN(min_value, front_min);
    pCar->collision_info->bb2.min.v[1] = min_value;

#undef UNK_SUSPENION_FACTOR
}

// FUNCTION: CARMA2_HW 0x0041fe50
int C2_HOOK_FASTCALL TestForCarInSensiblePlace(tCar_spec *pCar_spec, br_vector3 *pVec3) {
    int r;

    if (!gProgram_state.racing) {
        return 1;
    }
    gTesting_car_for_sensible_place = 1;
    r = TestForObjectInSensiblePlace(pCar_spec->collision_info,
        gList_collision_infos,
        pVec3,
        &gWorld_callbacks);
    gTesting_car_for_sensible_place = 0;
    return r;
}

// C2PORT-NONMATCHING: best 78.57% | difftest PASS 2000 trials (spec difftest/specs/0041fe90_RemoveNonCarFromActiveList.json) | reason VC5 loop unrolling and branch layout
// FUNCTION: CARMA2_HW 0x0041fe90
void C2_HOOK_FASTCALL RemoveNonCarFromActiveList(br_actor* pActor) {
    tNon_car_spec* non_car;
    tNon_car_spec** non_car_list;
    int active_count;
    int i;

    non_car = pActor->type_data;
    active_count = gNum_active_non_cars;
    i = 0;
    if (active_count <= 0) {
        return;
    }
    non_car_list = gActive_non_car_list;
search_for_non_car:
    if (*non_car_list == non_car) {
        goto found_non_car;
    }
    i++;
    non_car_list++;
    if (i < active_count) {
        goto search_for_non_car;
    }
    return;

found_non_car:
    gNum_active_non_cars--;
    PHILRemoveObject(non_car->collision_info);
    for (; i < gNum_active_non_cars; i++) {
        gActive_non_car_list[i] = gActive_non_car_list[i + 1];
    }
    non_car->driver = eDriver_non_car_unused_slot;
    if (gCar_to_view == (tCar_spec*)non_car) {
        FUN_0040e530(NULL);
    }
}

// FUNCTION: CARMA2_HW 0x004104b0
void C2_HOOK_FASTCALL PanningExternalCamera(tCar_spec* c, tU32 pTime) {
    float fVar1;
    br_vector3 tv;
    br_vector3 dir;
    br_vector3 normal;
    br_vector3* pCar_position;
    br_matrix34* m2;
    br_material* material;
    tU32 total_time;
    int bVar6;

    m2 = &gCamera->t.t.mat;
    BrVector3Sub(&tv, (br_vector3*)m2->m[3], &c->pos);
    fVar1 = BrVector3LengthSquared(&tv);

    if (fVar1 <= gDOUBLE_00589478) {
        if (gSwitch_time == 0) {
            gCamera_incident_flag = 1;
            goto point_camera_at_car;
        }
        if (ARReplayPlaying() != 0) {
            total_time = GetTotalTime();
            bVar6 = total_time >= gSwitch_time;
        } else {
            total_time = GetTotalTime();
            bVar6 = gSwitch_time >= total_time;
        }
        if (!bVar6) {
            gCamera_incident_flag = 1;
            goto point_camera_at_car;
        }
    }

    if (gCamera_incident_flag != 0 || fVar1 > gDOUBLE_005894a8) {
        if (fVar1 <= gFLOAT_0058948c) {
            pCar_position = &c->pos;
            BrVector3Sub(&dir, (br_vector3*)gCamera->t.t.mat.m[3], pCar_position);
            FindFace(pCar_position, &dir, &normal, &fVar1, &material);
            if (fVar1 > gFLOAT_00589440) {
                goto point_camera_at_car;
            }
        }
        SetUpPanningCamera(c);
    }

point_camera_at_car:
    PointCameraAtCar(&c->pos, &gCamera->t.t.mat, 1.0f);
}

// FUNCTION: CARMA2_HW 0x004121b0
void C2_HOOK_FASTCALL PangToCamera(br_vector3* pPos, br_matrix34* pMat)
{
    br_camera* camera;
    br_scalar dx, dz, d, nx, nz;
    br_angle pitch;

    camera = (br_camera*)gCamera->type_data;

    dx = pPos->v[0] - pMat->m[3][0];
    dz = pPos->v[2] - pMat->m[3][2];
    d = sqrt(dx * dx + dz * dz);

    if (d >= BR_SCALAR_EPSILON * 2)
    {
        nx = dx / d;
        nz = dz / d;
    }
    else
    {
        nx = 1.0f;
        nz = 0.0f;
    }

    pMat->m[0][0] = -nz;
    pMat->m[0][1] = 0.0f;
    pMat->m[0][2] = -nx;
    pMat->m[1][0] = 0.0f;
    pMat->m[1][1] = 1.0f;
    pMat->m[1][2] = 0.0f;
    pMat->m[2][0] = -nx;
    pMat->m[2][1] = 0.0f;
    pMat->m[2][2] = -nz;

    pitch = BR_ATAN2(pMat->m[3][1] - pPos->v[1],
        sqrt((pMat->m[3][0] - pPos->v[2]) * (pMat->m[3][0] - pPos->v[2]) +
             (pMat->m[3][2] - pPos->v[0]) * (pMat->m[3][2] - pPos->v[0])));

    BrMatrix34PostRotateY(pMat, (br_angle)(camera->field_of_view / 5) - pitch);
}

// FUNCTION: CARMA2_HW 0x0040f790
int C2_HOOK_FASTCALL IncidentCam(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_vector3 old_cam_pos;
    br_vector3 cam1;
    br_vector3 camP;
    br_vector3 tv;
    br_vector3 perp;
    br_vector3 vertical;
    br_vector3 scan_pos;
    br_vector3 left;
    br_vector3 right;
    br_vector3 scaled;
    br_vector3 dir;
    br_vector3 nor;
    br_material* material;
    br_scalar d;
    br_scalar ts;
    tU32 time_returned;
    tU32 scan_data[4];
    tS32 mode2;
    br_scalar towards2;
    tS32 time2;
    int scan_index = 0;
    tS32 scan_time;
    tCar_spec* car2;
    br_actor* ped_actor;
    br_actor* murderer;
    tU8 col_x;
    tU8 col_z;
    int removed = 0;

    m2 = &gCamera->t.t.mat;
    gPed_actor = NULL;
    if (gCamera_incident_mode == 3) {
        if (!GetPipeCarStateAtTime(-1, &gCamera_incident_mode, &gIncident_towards, gIncident_car_data, &gIncident_time)) {
            gCamera_incident_mode = 3;
        } else {
            if (gIncident_time < 0 && ARGetReplayDirection() >= 0) {
                gCamera_incident_mode = 3;
            } else {
                scan_time = abs(gIncident_time);
                if (scan_time > 2500) {
                    gCamera_incident_mode = 3;
                } else {
                    if (GetPipeCarStateAtTime(scan_time, &mode2, &towards2, scan_data, &time2)) {
                        do {
                            scan_index++;
                            if (scan_index > 10) {
                                break;
                            }
                            if (abs(time2) > 3500) {
                                break;
                            }
                            if (mode2 < gCamera_incident_mode
                                || (mode2 == gCamera_incident_mode && gIncident_towards <= towards2)) {
                                gCamera_incident_mode = mode2;
                                gIncident_car_data[0] = scan_data[0];
                                gIncident_car_data[1] = scan_data[1];
                                gIncident_car_data[2] = scan_data[2];
                                gIncident_car_data[3] = scan_data[3];
                                gIncident_towards = towards2;
                                gIncident_time = time2;
                            }
                        } while (GetPipeCarStateAtTime(abs(time2), &mode2, &towards2, scan_data, &time2));
                    }
                    if (abs(gIncident_time) > 2500) {
                        gCamera_incident_mode = 3;
                    } else if (gCamera_incident_mode == 2 && gIncident_towards < 0.1f) {
                        gCamera_incident_mode = 3;
                    } else {
                        if (gCamera_incident_mode == 2) {
                            ScanCarsPositions(c, &c->pos, 100000.f, -1, abs(time2), &scan_pos, &time_returned);
                            if (time_returned != 0 && Vector3DistanceSquared(&scan_pos, &c->pos) <= 102.91955471539592) {
                                BrVector3Sub(&tv, &scan_pos, (br_vector3*)gIncident_car_data);
                                tv.v[1] = scan_pos.v[1] + 2.f - ((br_vector3*)gIncident_car_data)->v[1];
                                BrVector3Normalise(&tv, &tv);
                                BrVector3Scale(&tv, &tv, 2.f);
                                BrVector3Set(&vertical, 0.f, 1.f, 0.f);
                                BrVector3Cross(&perp, &vertical, &tv);
                                BrVector3Add(&left, &scan_pos, &tv);
                                BrVector3Add(&left, &left, &perp);
                                left.v[1] += 2.f;
                                BrVector3Add(&right, &scan_pos, &tv);
                                BrVector3Sub(&right, &right, &perp);
                                right.v[1] += 2.f;
                                CollideCamera2(&scan_pos, &left, NULL, 1, NULL);
                                CollideCamera2(&scan_pos, &right, NULL, 1, NULL);
                                if (Vector3DistanceSquared(&left, &scan_pos) > Vector3DistanceSquared(&right, &scan_pos)) {
                                    BrVector3Copy(&gCamera->t.t.translate.t, &left);
                                } else {
                                    BrVector3Copy(&gCamera->t.t.translate.t, &right);
                                }
                            } else {
                                gCamera_incident_mode = 3;
                            }
                        }
                        gIncident_time += (tS32)GetTotalTime();
                    }
                }
            }
        }
    } else {
        gCamera_incident_flag = 1;
    }
    switch (gCamera_incident_mode) {
        case 0:
            BrVector3Copy(&old_cam_pos, &gCamera->t.t.translate.t);
            ped_actor = (br_actor*)gIncident_car_data[0];
            murderer = (br_actor*)gIncident_car_data[1];
            gPed_actor = ped_actor;
            XZToColumnXZ(&col_x, &col_z, murderer->t.t.translate.t.v[0], murderer->t.t.translate.t.v[2], &gProgram_state.track_spec);
            if (gProgram_state.track_spec.columns[col_z][col_x].actor_0x0 == murderer->parent) {
                BrActorRemove(murderer);
                removed = 1;
            }
            BrVector3Copy(&cam1, &ped_actor->t.t.translate.t);
            gIncident_car_data[1] = (tU32)c->car_master_actor;
            if (c->car_master_actor != NULL) {
                BrVector3Copy(&camP, &c->pos);
            } else if (c->car_master_actor->model != NULL) {
                BrVector3Sub(&scaled, &c->car_master_actor->model->bounds.max, &c->car_master_actor->model->bounds.min);
                BrVector3Scale(&scaled, &scaled, 0.5f);
                BrMatrix34ApplyP(&camP, &scaled, &c->car_master_actor->t.t.mat);
            } else {
                BrVector3Copy(&camP, &c->car_master_actor->t.t.translate.t);
            }
            if ((tU32)GetTotalTime() < (tU32)gIncident_time || ARGetReplayDirection() != 1) {
                BrVector3Sub(&tv, &cam1, &camP);
                tv.v[1] = 0.f;
                BrVector3Normalise(&tv, &tv);
                BrVector3Set(&vertical, 0.f, 0.4f, 0.f);
                BrVector3Cross(&perp, &tv, &vertical);
                if (gCamera_incident_direction) {
                    BrVector3Negate(&perp, &perp);
                }
                if (ARReplayPlaying()) {
                    BrVector3Accumulate(&perp, &tv);
                }
                BrVector3Add(&gCamera->t.t.translate.t, &cam1, &perp);
                gCamera->t.t.translate.t.v[1] += 0.25f;
                CollideCamera2(&cam1, &gCamera->t.t.translate.t, NULL, 1, NULL);
            }
            PangToCamera(&cam1, m2);
            ts = Vector3DistanceSquared(&gCamera->t.t.translate.t, &((br_actor*)gIncident_car_data[1])->t.t.translate.t);
            if (abs((tS32)GetTotalTime() - gIncident_time) > 2500) {
                gCamera_incident_mode = 3;
            }
            if (ARReplayPlaying() ? (tU32)(gIncident_time + 300) < GetTotalTime() : (tU32)GetTotalTime() < (tU32)(gIncident_time - 600)) {
                if (ts > 25.f) {
                    gCamera_incident_mode = 3;
                } else {
                    BrVector3Sub(&dir, &gCamera->t.t.translate.t, &((br_actor*)gIncident_car_data[1])->t.t.translate.t);
                    FindFace(&((br_actor*)gIncident_car_data[1])->t.t.translate.t, &dir, &nor, &d, &material);
                    if (d <= 1.f) {
                        gCamera_incident_mode = 3;
                    }
                }
            }
            if (removed) {
                XZToColumnXZ(&col_x, &col_z, ((br_actor*)gIncident_car_data[1])->t.t.translate.t.v[0], ((br_actor*)gIncident_car_data[1])->t.t.translate.t.v[2], &gProgram_state.track_spec);
                BrActorAdd(&gProgram_state.track_spec.columns[col_z][col_x].actor_0x0, (br_actor*)gIncident_car_data[1]);
            }
            BrVector3Sub(&tv, &cam1, &gCamera->t.t.translate.t);
            if (tv.v[0] * tv.v[0] + tv.v[2] * tv.v[2] < 0.0225) {
                BrVector3Copy(&gCamera->t.t.translate.t, &old_cam_pos);
                gPed_actor = NULL;
                return 0;
            }
            break;
        case 1:
            car2 = (tCar_spec*)gIncident_car_data[0];
            BrVector3Sub(&tv, &car2->pos, &c->pos);
            tv.v[1] = 0.f;
            BrVector3Normalise(&tv, &tv);
            BrVector3Scale(&tv, &tv, 2.f);
            BrVector3Add(&gCamera->t.t.translate.t, &car2->pos, &tv);
            gCamera->t.t.translate.t.v[1] += 1.f;
            CollideCamera2(&car2->pos, &gCamera->t.t.translate.t, NULL, 1, NULL);
            PangToCamera(&car2->pos, m2);
            ts = Vector3DistanceSquared(&gCamera->t.t.translate.t, &c->pos);
            if (fabsf((float)((tS32)GetTotalTime() - gIncident_time)) > 2500.f) {
                gCamera_incident_mode = 3;
            }
            if (ARReplayPlaying() ? (tU32)gIncident_time < GetTotalTime() : GetTotalTime() < (tU32)gIncident_time) {
                if (ts > 25.f) {
                    gCamera_incident_mode = 3;
                } else {
                    BrVector3Sub(&dir, &gCamera->t.t.translate.t, &c->pos);
                    FindFace(&c->pos, &dir, &nor, &d, &material);
                    if (d <= 1.f) {
                        gCamera_incident_mode = 3;
                    }
                }
            }
            break;
        case 2:
            PangToCamera(&c->pos, m2);
            ts = Vector3DistanceSquared(&gCamera->t.t.translate.t, &c->pos);
            if (fabsf((float)((tS32)GetTotalTime() - gIncident_time)) > 2500.f) {
                gCamera_incident_mode = 3;
            }
            if (ARReplayPlaying() ? (tU32)gIncident_time < GetTotalTime() : GetTotalTime() < (tU32)gIncident_time) {
                if (ts > 25.f) {
                    gCamera_incident_mode = 3;
                } else {
                    BrVector3Sub(&dir, &gCamera->t.t.translate.t, &c->pos);
                    FindFace(&c->pos, &dir, &nor, &d, &material);
                    if (d <= 1.f) {
                        gCamera_incident_mode = 3;
                    }
                }
            }
            break;
        default:
            gCamera_incident_mode = 3;
            break;
        }
        if (gCamera_incident_mode != 3) {
            return 1;
        }
        if (gIncident_camera_stack > 1) {
            SetUpPanningCamera(c);
            return 0;
        }
        gIncident_camera_stack++;
        if (IncidentCam(c, pTime)) {
            gIncident_camera_stack--;
            return 1;
        }
        gIncident_camera_stack--;
        return 0;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004ff530
void C2_HOOK_FASTCALL ResetCarSpecialVolume(tPhysics_object* pCollision_info) {
    br_vector3 cast_v;
    br_vector3 norm;
    br_scalar t;
    int id_len;
    char* mat_id;
    tSpecial_volume* new_special_volume;
    br_material* material;

    new_special_volume = NULL;
    BrVector3Set(&cast_v, 0.f, 200.f, 0.f);
    DisablePlingMaterials();
    FindFace(&pCollision_info->actor->t.t.translate.t, &cast_v, &norm, &t, &material);
    EnablePlingMaterials();
    if (t < 100.0f && material != NULL) {
        mat_id = material->identifier;
        if (mat_id != NULL) {
            id_len = strlen(mat_id);
            if (id_len != 0 && (mat_id[0] == '!' || mat_id[0] == '#')) {
                new_special_volume = GetDefaultSpecialVolumeForWater();
            }
        }
    }
    pCollision_info->auto_special_volume = new_special_volume;
    pCollision_info->water_depth_factor = 1.0f;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004175e0
void C2_HOOK_FAKE_THISCALL ControlCar4(tCar_spec* c, undefined4 pArg2, br_scalar dt) {

    CARPOCALYPSE2_THISCALL_UNUSED(pArg2);
#ifndef CARPOCALYPSE2_MATCHING
    if (getenv("C2_CARLOG") != NULL) {
        dr_dprintf("CARLOG curv %.4f max %.4f v %.3f %.3f %.3f om %.3f %.3f %.3f M %.3g L%d R%d A%d", c->curvature, c->maxcurve,
            c->collision_info->v.v[0], c->collision_info->v.v[1], c->collision_info->v.v[2],
            c->collision_info->omega.v[0], c->collision_info->omega.v[1], c->collision_info->omega.v[2], c->collision_info->M,
            c->keys.left, c->keys.right, c->keys.acc);
    }
#endif

    if (c->keys.left) {
        if (c->turn_speed < 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->collision_info->velocity_car_space.v[2] > 0.f) {
            c->turn_speed += .25 * dt;
        } else if ((c->curvature >= 0.f && c->collision_info->omega.v[1] >= -.001) || c->turn_speed != 0.f) {
            c->turn_speed += 25.f * dt * (0.05 / (5. + WORLD_SCALE * BrVector3Length(&c->collision_info->v) )) * .25;
        } else {
            c->turn_speed = 25.f * dt * (.05 / (5. + WORLD_SCALE * BrVector3Length(&c->collision_info->v)));
            if (c->collision_info->omega.v[1] < -.01) {
                c->turn_speed -= .25 * dt * c->collision_info->omega.v[1];
            }
        }
    }
    if (c->keys.right) {
        if (c->turn_speed > 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->collision_info->velocity_car_space.v[2] > 0.f) {
            c->turn_speed -= .25 * dt;
        } else if ((c->curvature <= 0.f && c->collision_info->omega.v[1] <= .001) || c->turn_speed != 0.f) {
            c->turn_speed -= 25.f * dt * (.05 / (5. + WORLD_SCALE * BrVector3Length(&c->collision_info->v))) * .25;
        } else {
            c->turn_speed = -25.f * dt * (.05 / (5. + WORLD_SCALE * BrVector3Length(&c->collision_info->v)));
            if (c->collision_info->omega.v[1] < -.01) {
                c->turn_speed -= .25 * dt * c->collision_info->omega.v[1];
            }
        }
    }
    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.f;
    } else if (fabsf(c->turn_speed) < fabsf(2.f * dt * c->curvature) && c->curvature * c->turn_speed < 0.f) {
        c->turn_speed = -(2.f * dt * c->curvature);
    }
    c->curvature += c->turn_speed;
    if (c->joystick.left > 0) {
        c->curvature = (float)c->joystick.left * (float)c->joystick.left / 4294967300.f * c->maxcurve;
    } else if (c->joystick.right >= 0) {
        c->curvature = -((float)c->joystick.right * (float)c->joystick.right / 4294967300.f) * c->maxcurve;
    }
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

__inline void C2_HOOK_FASTCALL RememberSafePosition(tCar_spec* car, tU32 pTime_difference) {
    // GLOBAL: CARMA2_HW 0x006793ec
    static tU32 time_count = 0;
    int i;
    br_vector3 r;

    time_count += pTime_difference;
    if (time_count < 5000) {
        return;
    }
    time_count = 4000;
    if (car->disabled) {
        return;
    }
    if (car->car_crush_spec != NULL && car->car_crush_spec->field_0x4b8) {
        return;
    }

    CARPOCALYPSE2_BUG_ON(CARPOCALYPSE2_ASIZE(car->oldd) != 4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, susp_height, 0x1218);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, oldd, 0x1264);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(car->oldd); i++) {
        if (car->oldd[i] >= car->susp_height[i / 2]) {
            return;
        }
    }
    if ((car->collision_info->last_special_volume == NULL || car->collision_info->last_special_volume->gravity_multiplier == gZero)
        && gFriction_materials[car->material_index[0]].tyre_road_friction >= 0.1
        && gFriction_materials[car->material_index[1]].tyre_road_friction >= 0.1
        && gFriction_materials[car->material_index[2]].tyre_road_friction >= 0.1
        && gFriction_materials[car->material_index[3]].tyre_road_friction >= 0.1
        && !car->field_0x195c
        && car->car_master_actor->t.t.mat.m[1][1] >= 0.8f) {

        CARPOCALYPSE2_BUG_ON(CARPOCALYPSE2_ASIZE(car->last_safe_positions) != 20);
        /* Only check last 5 positions */
        for (i = 0; i < 4; i++) {
            BrVector3Sub(&r, &car->car_master_actor->t.t.translate.t, (br_vector3*)car->last_safe_positions[i].m[3]);

            if (BrVector3LengthSquared(&r) < 8.4015961f) {
                return;
            }
        }
        for (i = CARPOCALYPSE2_ASIZE(car->last_safe_positions) - 2; i > 0; i--) {
            BrMatrix34Copy(&car->last_safe_positions[i], &car->last_safe_positions[i - 1]);
        }
        BrMatrix34Copy(&car->last_safe_positions[0], &car->car_master_actor->t.t.mat);
        time_count = 0;
    }
}

// FUNCTION: CARMA2_HW 0x00414cb0
void C2_HOOK_FASTCALL ControlOurCar(tU32 pTime_difference) {
    // GLOBAL: CARMA2_HW 0x0058f6d8
    static int last_key_down = 1;
    tCar_spec* car;
    tU32 time;
    br_vector3 minus_k;
    br_vector3 tmp;
    br_vector3 delta_omega;

    car = &gProgram_state.current_car;

    if (car->keys.change_down) {
        if (!last_key_down) {
            last_key_down = 1;

            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, number_of_wheels_on_ground, 0x12e8);
            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, oldd, 0x1264);
            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, susp_height, 0x1218);
            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, frame_collision_flag, 0x64);
            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, disable_move_rotate, 0xec);
            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, omega, 0x74);

            if (car->number_of_wheels_on_ground <= 2
                    && BrVector3LengthSquared(&car->collision_info->v) < .04
                    && car->oldd[0] == car->susp_height[0]
                    && car->oldd[1] == car->susp_height[1]
                    && (car->frame_collision_flag || car->collision_info->disable_move_rotate)) {

                tmp.v[0] = -car->collision_info->transform_matrix.m[1][2];
                tmp.v[1] = 0.f;
                tmp.v[2] = car->collision_info->transform_matrix.m[1][0];
                BrVector3Normalise(&tmp, &tmp);
                BrMatrix34TApplyV(&delta_omega, &tmp, &car->collision_info->transform_matrix);
                BrVector3Accumulate(&car->collision_info->omega, &delta_omega);
                car->collision_info->disable_move_rotate = 0;
            }
        }
    } else {
        last_key_down = 0;
    }
    if (gNet_mode != eNet_mode_none) {
        int i;

        for (i = 0; i < gNumber_of_net_players; i++) {

            if (i != gThis_net_player_index) {
                ControlCar[gControl__car](gNet_players[i].car CARPOCALYPSE2_THISCALL_EDX, 0.001f * pTime_difference);
            }
        }
    }
    if (gCar_flying) {
        FlyCar(gCar_to_view CARPOCALYPSE2_THISCALL_EDX, pTime_difference / 1000.f);
        return;
    }

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, end_steering_damage_effect, 0x154c);

    time = GetTotalTime();
    if (car->damage_units[eDamage_steering].damage_level > 40) {
        if (car->end_steering_damage_effect != 0) {
            if (car->end_steering_damage_effect > time || car->damage_units[eDamage_steering].damage_level == 99) {
                car->keys.left = car->false_key_left;
                car->keys.right = car->false_key_right;
            } else {
                car->end_steering_damage_effect = 0;
            }
        } else {
            float ts;

            ts = pTime_difference * (car->damage_units[eDamage_steering].damage_level - 40) * 0.0045;
            if (PercentageChance((int)ts) && fabsf(car->collision_info->velocity_car_space.v[2]) > 1. / (10000. * WORLD_SCALE)) {
                if (car->keys.left || car->keys.right) {
                    car->false_key_left = !car->keys.left;
                    car->false_key_right = !car->keys.right;
                } else {
                    if (PercentageChance(50)) {
                        car->false_key_left = 1;
                    } else {
                        car->false_key_right = 1;
                    }
                }
                ts = (float)(25 * (car->damage_units[eDamage_steering].damage_level - 40));
                car->end_steering_damage_effect = (tU32)(time + FRandomBetween(0.0f, ts));
            }
        }
    }
    if (car->damage_units[eDamage_transmission].damage_level > 40) {
        if (car->end_trans_damage_effect != 0) {
            if (car->end_trans_damage_effect > time || car->damage_units[eDamage_transmission].damage_level == 99) {
                car->gear = 0;
                car->just_changed_gear = 1;
            } else {
                car->end_trans_damage_effect = 0;
            }
        } else {
            float ts;

            ts = pTime_difference * (car->damage_units[eDamage_transmission].damage_level - 40) * 0.006;
            if (PercentageChance((int)ts)) {
                ts = (float)(50 * (car->damage_units[eDamage_transmission].damage_level - 40));
                car->end_trans_damage_effect = (tU32)(time + FRandomBetween(0.f, ts));
            }
        }
    }
    ControlCar[gControl__car](car CARPOCALYPSE2_THISCALL_EDX, pTime_difference / 1000.0f);
    RememberSafePosition(car, pTime_difference);
    if (gCamera_reset) {
        BrVector3SetFloat(&minus_k, 0.0f, 0.0f, -1.0f);
        gCamera_sign = 0;
        BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
    }
}

// FUNCTION: CARMA2_HW 0x00414510
void C2_HOOK_FASTCALL SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index) {
    int place_on_grid;
    int i;
    int start_i;
    int j;
    br_actor* car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    tCar_spec* car;
    br_vector3 grid_offset;
    br_vector3 dist;
    br_vector3 real_pos;
    br_matrix34 initial_yaw_matrix;

    initial_yaw = 0;
    car_actor = pThe_race->opponent_list[pCar_index].car_spec->car_master_actor;
    car = pThe_race->opponent_list[pCar_index].car_spec;
    BrMatrix34Identity(&car_actor->t.t.mat);
    place_on_grid = 1;
    if (gNet_mode != eNet_mode_none && !gCurrent_net_game->options.grid_start && pThe_race->count_network_start_points != 0) {
        start_i = i = IRandomBetween(0, pThe_race->count_network_start_points - 1);
        do {
            PossibleService();
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (j != pCar_index) {
                    BrVector3Copy(&real_pos, &pThe_race->opponent_list[j].car_spec->car_master_actor->t.t.translate.t);
                    if (real_pos.v[0] > 500.f) {
                        BrVector3Sub(&real_pos, &real_pos, &gInitial_position);
                    }
                    BrVector3Sub(&dist, &real_pos, &pThe_race->net_starts[i].pos);
                    if (BrVector3LengthSquared(&dist) < 16.f) {
                        break;
                    }
                }
            }
            if (j == gNumber_of_net_players) {
                BrVector3Copy(&car_actor->t.t.translate.t, &pThe_race->net_starts[i].pos);
                initial_yaw = BrDegreeToAngle(pThe_race->net_starts[i].yaw);
                place_on_grid = 0;
            }
            i++;
            if (i == pThe_race->count_network_start_points) {
                i = 0;
            }
        } while (i != start_i);
    }
    if (gNet_mode == eNet_mode_none && pGrid_index < 0) {
        BrVector3Copy(&car_actor->t.t.translate.t, &pThe_race->net_starts[-pGrid_index - 1].pos);
        initial_yaw = BrDegreeToAngle(pThe_race->net_starts[-pGrid_index - 1].yaw);
        place_on_grid = 0;
    }
    if (place_on_grid) {
        initial_yaw = BrDegreeToAngle(pThe_race->initial_yaw);
        BrMatrix34RotateY(&initial_yaw_matrix, initial_yaw);
        grid_offset.v[0] = -(br_scalar)(pGrid_index % 2);
        grid_offset.v[1] = 0.0f;
        grid_offset.v[2] = 2.0f * (br_scalar)(pGrid_index / 2) + 0.4f * (br_scalar)(pGrid_index % 2);
        BrMatrix34ApplyV(&car_actor->t.t.translate.t, &grid_offset, &initial_yaw_matrix);
        BrVector3Accumulate(&car_actor->t.t.translate.t, &pThe_race->initial_position);
    }
    if (gTrack_actor != NULL) {
        FindBestY(
            &car_actor->t.t.translate.t,
            gTrack_actor,
            10.0f,
            &nearest_y_above,
            &nearest_y_below,
            &above_model,
            &below_model,
            &above_face_index,
            &below_face_index);
        if (nearest_y_above != 30000.0f) {
            car_actor->t.t.translate.t.v[1] = nearest_y_above;
        } else if (nearest_y_below != -30000.0f) {
            car_actor->t.t.translate.t.v[1] = nearest_y_below;
        } else {
            car_actor->t.t.translate.t.v[1] = 0.0f;
        }
    }
    BrMatrix34PreRotateY(&car_actor->t.t.mat, initial_yaw);
    if (gNet_mode != eNet_mode_none) {
        tCompressed_matrix3 compressed;
        int inactive;

        CompressMatrix34(&compressed, &inactive, &car_actor->t.t.mat);
        ExpandMatrix34(&car_actor->t.t.mat, &compressed, inactive);
        BrMatrix34Copy(
            &gNet_players[pThe_race->opponent_list[pCar_index].net_player_index].initial_position,
            &car->car_master_actor->t.t.mat);
    }
    if (gNet_mode != eNet_mode_none && car->disabled && car_actor->t.t.translate.t.v[0] < 500.0f) {
        DisableCar(car);
    }
}

// FUNCTION: CARMA2_HW 0x00413f70
void C2_HOOK_FASTCALL InitialiseCar2(tCar_spec* pCar, int pClear_disabled_flag) {
    int index;
    int j;
    br_matrix34 safe_position;
    tNet_game_player_info* net_player;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x261, 0x261);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, message_time, 0x268);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x49c, 0x49c);

    PossibleService();
    if (pCar->disabled && pClear_disabled_flag) {
        if (gNet_mode == eNet_mode_none) {
            EnableCar(pCar);
        } else {
            net_player = NetPlayerFromCar(pCar);
            if (net_player->player_status == ePlayer_status_ready || net_player->player_status == ePlayer_status_racing) {
                EnableCar(pCar);
            }
        }
    }
    InitCarSkidStuff(pCar);
    pCar->car_model_actor->render_style = BR_RSTYLE_DEFAULT;
    SwitchCarModels(pCar, 0);
    pCar->collision_info->last_special_volume = NULL;
    pCar->collision_info->auto_special_volume = NULL;
    pCar->field_0xc8 = NULL;
    if (pCar != NULL && pCar->driver == eDriver_local_human) {
        ResetRecoveryVouchers();
    }
    BrVector3SetFloat(&pCar->collision_info->v, 0.f, 0.f, 0.f);
    BrVector3SetFloat(&pCar->collision_info->omega, 0.f, 0.f, 0.f);
    pCar->curvature = 0.f;
    pCar->field_0x1260 = 0.f;
    BrMatrix34Copy(&safe_position, &pCar->car_master_actor->t.t.mat);
    if (safe_position.m[3][0] > 500.0f) {
        BrVector3Sub((br_vector3*)safe_position.m[3], (br_vector3*)safe_position.m[3], &gInitial_position);
    }
    BrMatrix34Copy(&pCar->old_frame_mat, &safe_position);
    BrMatrix34Copy(&pCar->collision_info->transform_matrix, &safe_position);
    BrMatrix34ApplyP(&pCar->pos, &pCar->collision_info->cmpos, &pCar->collision_info->transform_matrix);
    for (j = 0; j < CARPOCALYPSE2_ASIZE(pCar->oldd); j++) {
        pCar->oldd[j] = pCar->ride_height;
    }
    pCar->gear = 0;
    pCar->revs = 0.f;
    pCar->traction_control = 1;
    BrVector3Negate(&pCar->direction, (br_vector3*)pCar->car_master_actor->t.t.mat.m[2]);
    for (j = 0; j < CARPOCALYPSE2_ASIZE(pCar->last_safe_positions); j++) {
        BrMatrix34Copy(&pCar->last_safe_positions[j], &safe_position);
    }
    pCar->collision_info->field_0x261 = 0;
    pCar->collision_info->message_time = 0;
    pCar->dt = -1.f;
    pCar->collision_info->field_0x49c = 1;
    pCar->time_to_recover = 0;
    pCar->repair_time  = 0;
    pCar->collision_info->water_d  = 10000.f;

    switch (pCar->driver) {
    case eDriver_oppo:
        index = 0;
        for (j = 0; j < gCurrent_race.number_of_racers; j++) {
            if (gCurrent_race.opponent_list[j].car_spec != NULL
                    && gCurrent_race.opponent_list[j].car_spec->driver == eDriver_oppo) {
                if (pCar == gCurrent_race.opponent_list[j].car_spec) {
                    pCar->car_ID = 0x200 + index;
                }
                index++;
            }
        }
        break;
    case eDriver_net_human:
        index = 0;
        for (j = 0; j < gCurrent_race.number_of_racers; j++) {
            if (gCurrent_race.opponent_list[j].car_spec != NULL
                    && gCurrent_race.opponent_list[j].car_spec->driver == eDriver_net_human) {
                if (pCar == gCurrent_race.opponent_list[j].car_spec) {
                    pCar->car_ID = 0x100 + index;
                }
                index++;
            }
        }
        break;
    case eDriver_local_human:
        pCar->car_ID = 0;
        break;
    default:
        break;
    }
    PossibleService();
    pCar->collision_info->box_face_ref = gFace_num__car - 2;
    pCar->collision_info->box_face_end = 0;
    pCar->collision_info->box_face_start = 0;
    pCar->collision_info->disable_move_rotate = 0;
    pCar->end_steering_damage_effect = 0;
    pCar->end_trans_damage_effect = 0;
    pCar->wheel_dam_offset[0] = 0.f;
    pCar->wheel_dam_offset[1] = 0.f;
    pCar->wheel_dam_offset[2] = 0.f;
    pCar->wheel_dam_offset[3] = 0.f;
    pCar->shadow_intersection_flags = 0;
    pCar->underwater_ability = 0;

    if (gNet_mode == eNet_mode_none) {
        net_player = NULL;
    } else {
        net_player = NetPlayerFromCar(pCar);
    }
    if (net_player != NULL && net_player->field_0x80) {
        pCar->invulnerable_no_crushage = 1;
        pCar->invulnerable_no_damage = 1;
        pCar->invulnerable_no_wastage = 1;
    } else {
        pCar->invulnerable_no_crushage = 0;
        pCar->invulnerable_no_damage = 0;
        pCar->invulnerable_no_wastage = 0;
    }
    pCar->wall_climber_mode = 0;
    pCar->grip_multiplier = 1.f;
    pCar->damage_multiplier = 1.f;
    pCar->field_0x4c8 = 1.f;
    pCar->field_0x4d4 = 1.f;
    pCar->bounce_rate = 0.f;
    pCar->bounce_amount = 0.f;
    pCar->knackered = 0;
    pCar->collision_info->last_special_volume = NULL;
    pCar->collision_info->auto_special_volume = NULL;
    RemoveFromCloakingList(pCar);
    TurnOffCloaking(NULL, pCar);
    if (pCar == NULL || pCar->driver != eDriver_local_human) {
        pCar->joystick.left = -1;
        pCar->joystick.right = -1;
    }
    TotallyRepairACar(pCar);
    SetCarSuspGiveAndHeight(pCar, 1.f, 1.f, 1.f, 0.f, 0.f);
    for (j = 0; j < CARPOCALYPSE2_ASIZE(pCar->powerups); j++) {
        pCar->powerups[j] = 0;
    }
    if (gNet_mode != eNet_mode_none && (net_player == NULL || !net_player->field_0x80)) {
        for (j = 0; j < (int)CARPOCALYPSE2_ASIZE(pCar->power_up_levels); j++) {
            if (gNet_mode != eNet_mode_none) {
                pCar->power_up_levels[j] = gInitial_APO[j].initial_network[gCurrent_net_game->type];
            } else {
                pCar->power_up_levels[j] = gInitial_APO[j].initial[gProgram_state.skill_level];
            }
            if (gNet_mode != eNet_mode_none) {
                pCar->power_up_slots[j] = gInitial_APO_potential[j].initial_network[gCurrent_net_game->type];
            } else {
                pCar->power_up_slots[j] = gInitial_APO_potential[j].initial[gProgram_state.skill_level];
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x00414400
void C2_HOOK_FASTCALL InitialiseCar(tCar_spec* pCar) {

    InitialiseCar2(pCar, 1);
}

// FUNCTION: CARMA2_HW 0x00414410
void C2_HOOK_FASTCALL InitialiseCarsEtc(tRace_info* pThe_race) {
    int i;
    int cat;
    int car_count;
    tCar_spec* car;
    br_bounds bnds;

    BrVector3Copy(&gProgram_state.initial_position, &pThe_race->initial_position);
    gProgram_state.initial_yaw = pThe_race->initial_yaw;
    BrActorToBounds(&bnds, gProgram_state.track_spec.the_actor);
    gMin_world_y = bnds.min.v[1];
    gNum_active_non_cars = 0;
    for (cat = eVehicle_self; cat <= eVehicle_not_really; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            PossibleService();
            if (cat == eVehicle_self) {
                car = &gProgram_state.current_car;
            } else {
                car = GetCarSpec(cat, i);
            }
            if (cat != eVehicle_not_really) {
                InitialiseCar(car);
            }
        }
    }
    gCamera_yaw = 0;
    if (gAction_replay_camera_mode == kActionReplayCameraMode_Manual) {
        gCamera_type = 0;
        gAction_replay_camera_mode = kActionReplayCameraMode_Standard;
    }
    InitialiseExternalCamera();
    if (gUnknown_car_collision_info != NULL && gProgram_state.current_car.collision_info != gUnknown_car_collision_info->parent) {
        AddCollisionInfoChild(gProgram_state.current_car.collision_info, gUnknown_car_collision_info);
    }
}

// FUNCTION: CARMA2_HW 0x004148d0
void C2_HOOK_FASTCALL SetInitialPositions(tRace_info* pThe_race) {
    int i;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tRace_info, number_of_racers, 0x90);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tRace_info, opponent_list, 0xce4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpp_spec, ranking, 0x8);
    C2_HOOK_BUG_ON(sizeof(tOpp_spec) != 16);

    for (i = 0; i < pThe_race->number_of_racers; i++) {
        int grid_index = pThe_race->opponent_list[i].ranking;

        if (grid_index >= 0) {
            grid_index = i;
        }
        SetInitialPosition(pThe_race, i, grid_index);
    }
}


// FUNCTION: CARMA2_HW 0x00413580
void C2_HOOK_FASTCALL InitialiseExternalCamera(void) {
    br_scalar ts;
    tCar_spec* c;
    br_angle yaw;

    c = gCar_to_view;
    if (!gProgram_state.racing) {
        c = &gProgram_state.current_car;
    }
    gCamera_height = c->pos.v[1];
    BrVector3Set(&gView_direction, c->direction.v[0], 0.0f, c->direction.v[2]);
    BrVector3Normalise(&gView_direction, &gView_direction);
    ts = -BrVector3Dot(&gView_direction, (br_vector3*)c->car_master_actor->t.t.mat.m[2]);
    gCamera_sign = ts < 0;
    gCamera_mode = 0;
    if (gCamera_sign) {
        yaw = -gCamera_yaw;
    } else {
        yaw = gCamera_yaw;
    }
    DrVector3RotateY(&gView_direction, yaw);
    gMin_camera_car_distance = 0.6f;
    gCamera_frozen = 0;
    gCamera_mode = -2;
    if (gCountdown && (gNet_mode == eNet_mode_none || gCurrent_net_game->options.grid_start) && gCountdown > 4) {
        gCamera_height += 10.f;
    }
}

void C2_HOOK_FASTCALL SetAmbientPratCam(tCar_spec* pCar) {
    br_scalar vcs_x;
    br_scalar vcs_y;
    br_scalar vcs_z;
    br_scalar abs_vcs_x;
    br_scalar abs_vcs_y;
    br_scalar abs_vcs_z;
    br_scalar abs_omega_x;
    br_scalar abs_omega_y;
    br_scalar abs_omega_z;
    tU32 the_time;
    // GLOBAL: CARMA2_HW 0x00679370
    static tU32 last_time_on_ground;

    if (gRace_finished) {
        return;
    }
    the_time = GetTotalTime();
    if (pCar->number_of_wheels_on_ground != 0) {
        last_time_on_ground = the_time;
    }
    vcs_x = pCar->collision_info->velocity_car_space.v[0] / 1000.f;
    vcs_y = pCar->collision_info->velocity_car_space.v[1] / 1000.f;
    vcs_z = pCar->collision_info->velocity_car_space.v[2] / 1000.f;
    abs_vcs_x = fabsf(vcs_x);
    abs_vcs_y = fabsf(vcs_y);
    abs_vcs_z = fabsf(vcs_z);
    abs_omega_x = fabsf(pCar->collision_info->omega.v[0]);
    abs_omega_y = fabsf(pCar->collision_info->omega.v[1]);
    abs_omega_z = fabsf(pCar->collision_info->omega.v[2]);

    if (abs_omega_x > 4.5f || abs_omega_z > 4.5f) {
        ChangeAmbientPratcam(9);
    } else if (abs_omega_y > 4.5f) {
        ChangeAmbientPratcam(12);
    } else if (abs_omega_x > 3.f || abs_omega_z > 3.f) {
        ChangeAmbientPratcam(8);
    } else if (abs_omega_y > 3.f) {
        ChangeAmbientPratcam(11);
    } else if (pCar->car_master_actor->t.t.mat.m[1][1] < 0.1f) {
        ChangeAmbientPratcam(44);
    } else if (abs_vcs_y > abs_vcs_z && abs_vcs_y > abs_vcs_x && vcs_y < -.004f) {
        ChangeAmbientPratcam(6);
    } else if (the_time - last_time_on_ground > 500) {
        ChangeAmbientPratcam(5);
    } else if (abs_vcs_x > abs_vcs_z && vcs_x > .001f) {
        ChangeAmbientPratcam(26);
    } else if (abs_vcs_x > abs_vcs_z && vcs_x < -.001f) {
        ChangeAmbientPratcam(25);
    } else if (abs_omega_x > 1.5f || abs_omega_z > 1.5f) {
        ChangeAmbientPratcam(7);
    } else if (abs_omega_y > 1.5f) {
        ChangeAmbientPratcam(10);
    } else if (abs_vcs_z > .01f) {
        ChangeAmbientPratcam(3);
    } else if (abs_vcs_z > .004f) {
        ChangeAmbientPratcam(2);
    } else if (abs_vcs_z > .0015f) {
        ChangeAmbientPratcam(1);
    } else {
        ChangeAmbientPratcam(0);
    }
}

// FUNCTION: CARMA2_HW 0x0041e660
void C2_HOOK_FASTCALL MungeCarGraphics(tU32 pFrame_period) {
    tU32 the_time;
    int car;
    int cat;

    if (gNet_mode != eNet_mode_none
            && gCurrent_net_game->type == eNet_game_type_foxy
            && gThis_net_player_index == gIt_or_fox) {
        gProgram_state.current_car.power_up_levels[1] = 0;
    }
    SetAmbientPratCam(&gProgram_state.current_car);

    the_time = PDGetTotalTime();
    for (cat = eVehicle_self; cat <= eVehicle_net_player; cat++) {
        int car_count;

        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (car = 0; car < car_count; car++) {
            tCar_spec* the_car;

            if (cat == eVehicle_self) {
                the_car = &gProgram_state.current_car;
            } else {
                the_car = GetCarSpec(cat, car);
            }
            if (!(the_car != NULL && the_car->driver == eDriver_local_human) && PointOutOfSight(&the_car->pos CARPOCALYPSE2_THISCALL_EDX, gYon_squared)) {
                the_car->car_master_actor->render_style = BR_RSTYLE_NONE;
            } else {
                the_car->car_master_actor->render_style = BR_RSTYLE_DEFAULT;
            }
        }
    }
    if (!(gCar_to_view != NULL && gCar_to_view->driver == eDriver_oppo)) {
        gCar_to_view->car_master_actor->render_style = BR_RSTYLE_DEFAULT;
    }

    for (car = 0; car < gNum_active_cars; car++) {
        tCar_spec* the_car;

        the_car = gActive_car_list[car];
        if (the_car->car_master_actor->render_style != BR_RSTYLE_NONE && the_car != NULL && the_car->driver >= eDriver_oppo) {
            br_scalar car_x;
            br_scalar car_z;
            int oily_count;
            int i;

            the_car->shadow_intersection_flags = 0;
            car_x = the_car->car_master_actor->t.t.translate.t.v[0];
            car_z = the_car->car_master_actor->t.t.translate.t.v[2];
            oily_count = GetOilSpillCount();

            for (i = 0; i < oily_count; i++) {
                br_actor* oily_actor;
                br_scalar oily_size;

                GetOilSpillDetails(i, &oily_actor, &oily_size);
                if (oily_actor != NULL) {
                    br_scalar car_radius;

                    car_radius = the_car->collision_info->shape->common.bb.max.v[2] / WORLD_SCALE * 1.5f;
                    if (oily_actor->t.t.translate.t.v[0] - oily_size < car_x + car_radius
                            && oily_actor->t.t.translate.t.v[0] + oily_size > car_x - car_radius
                            && oily_actor->t.t.translate.t.v[2] - oily_size < car_z + car_radius
                            && oily_actor->t.t.translate.t.v[2] + oily_size > car_z - car_radius) {
                        the_car->shadow_intersection_flags |= 1 << i;
                    }
                }
            }
            if (the_car->driver < eDriver_net_human && (!gAction_replay_mode || !ARReplayIsReallyPaused())) {
                if (gCountdown) {
                    float sine_angle;
                    float raw_revs;
                    float rev_reducer;

                    sine_angle = FRandomBetween(.4f, 1.6f) * ((float)GetTotalTime() / ((float)gCountdown * 100.f));
                    sine_angle = frac(sine_angle) * 360.0f;
                    sine_angle = FastScalarSin((int)sine_angle);
                    raw_revs = the_car->red_line * fabsf(sine_angle);
                    rev_reducer = (11.f - (float)(gCountdown)) / 10.f;
                    the_car->revs = rev_reducer * raw_revs;
                } else {
                    the_car->revs = (the_car->speedo_speed / 0.003f - (float)(int)(the_car->speedo_speed / 0.003))
                                    * (float)(the_car->red_line - 800)
                                    + 800.f;
                }
            }
            for (i = 0; i < the_car->number_of_steerable_wheels; i++) {
                ControlBoundFunkGroove(the_car->steering_ref[i] CARPOCALYPSE2_THISCALL_EDX, the_car->steering_angle);
            }
            for (i = 0; i < CARPOCALYPSE2_ASIZE(the_car->rf_sus_ref); i++) {
                ControlBoundFunkGroove(the_car->rf_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, the_car->rf_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, -the_car->lf_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, the_car->lf_sus_position);
                }
            }
            for (i = 0; i < CARPOCALYPSE2_ASIZE(the_car->rr_sus_ref); i++) {
                ControlBoundFunkGroove(the_car->rr_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, the_car->rr_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, -the_car->lr_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i] CARPOCALYPSE2_THISCALL_EDX, the_car->lr_sus_position);
                }
            }
            if (!gAction_replay_mode || !ARReplayIsReallyPaused()) {
                float wheel_speed;

                wheel_speed = -(the_car->speedo_speed / the_car->non_driven_wheels_circum * (float)gFrame_period);
                if (gAction_replay_mode && ARGetReplayDirection() < 0) {
                    wheel_speed = -wheel_speed;
                }
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_1 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_2 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_3 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_4 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                if (the_car->driver >= eDriver_net_human) {
                    if (the_car->gear != 0) {
                        wheel_speed = -(the_car->revs
                                        * the_car->speed_revs_ratio
                                        * (float)the_car->gear
                                        * (float)gFrame_period
                                        / (1000.f * WORLD_SCALE))
                                        / the_car->driven_wheels_circum;
                    } else if (the_car->keys.brake) {
                        wheel_speed = 0.0;
                    } else {
                        wheel_speed = -(the_car->speedo_speed / the_car->driven_wheels_circum * (float)gFrame_period);
                    }
                    if (gAction_replay_mode && ARGetReplayDirection() < 0) {
                        wheel_speed = -wheel_speed;
                    }
                }
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_1 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_2 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_3 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_4 CARPOCALYPSE2_THISCALL_EDX, wheel_speed);
            }
            if (gAction_replay_mode) {
                MungeSpecialVolume(the_car->collision_info);
            } else if (the_car->driver == eDriver_local_human && the_car->collision_info->M < 5.f) {
                br_scalar abs_omega_x;
                br_scalar abs_omega_y;
                br_scalar abs_omega_z;
                int spinning_wildly;

                abs_omega_x = (WORLD_SCALE * WORLD_SCALE * the_car->collision_info->I.v[0] + 3.30f) / 2.f * fabsf(the_car->collision_info->omega.v[0]);
                abs_omega_y = (WORLD_SCALE * WORLD_SCALE * the_car->collision_info->I.v[1] + 3.57f) / 2.f * fabsf(the_car->collision_info->omega.v[1]);
                abs_omega_z = (WORLD_SCALE * WORLD_SCALE * the_car->collision_info->I.v[2] + 0.44f) / 2.f * fabsf(the_car->collision_info->omega.v[2]);
                spinning_wildly = abs_omega_x > 26.4f || abs_omega_y > 49.98f || abs_omega_z > 3.52f;
                if (spinning_wildly && the_time - gLast_cunning_stunt > 10000) {
                    if (gWild_start == 0
                            || (the_car->collision_info->last_special_volume != NULL && the_car->collision_info->last_special_volume->gravity_multiplier != 1.f)) {
                        gWild_start = the_time;
                    } else if (the_time - gWild_start >= 500) {
                        DoFancyHeadup(18);
                        EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                        gLast_cunning_stunt = the_time;
                        gOn_me_wheels_start = 0;
                        gQuite_wild_end = 0;
                        gQuite_wild_start = 0;
                        gWoz_upside_down_at_all = 0;
                    }
                } else {
                    int spinning_mildly;

                    gWild_start = 0;
                    spinning_mildly = abs_omega_x > 1.65f || abs_omega_z > .22f;
                    if (the_car->number_of_wheels_on_ground < 4) {
                        gOn_me_wheels_start = 0;
                        if (the_car->number_of_wheels_on_ground == 0 && spinning_mildly) {
                            if (gQuite_wild_start == 0) {
                                gQuite_wild_start = the_time;
                            }
                            if (the_car->car_master_actor->t.t.mat.m[1][1] < -.8f) {
                                gWoz_upside_down_at_all = the_time;
                            }
                        } else {
                            gQuite_wild_end = the_time;
                        }
                    } else {
                        if (gQuite_wild_end == 0) {
                            gQuite_wild_end = the_time;
                        }
                        if (gQuite_wild_start == 0
                                || the_time - gLast_cunning_stunt <= 10000) {
                            gWild_start = 0;
                        } else if (gQuite_wild_end - gQuite_wild_start >= 2000
                                && gQuite_wild_start <= gWoz_upside_down_at_all
                                && gWoz_upside_down_at_all <= gQuite_wild_end) {

                            if (gOn_me_wheels_start != 0) {
                                if (the_time - gOn_me_wheels_start > 500
                                        && !(the_car->collision_info->last_special_volume != NULL && the_car->collision_info->last_special_volume->gravity_multiplier != 1.f)) {
                                    DoFancyHeadup(18);
                                    EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                                    gLast_cunning_stunt = PDGetTotalTime();
                                    gQuite_wild_start = 0;
                                    gQuite_wild_end = 0;
                                    gOn_me_wheels_start = 0;
                                    gWoz_upside_down_at_all = 0;
                                } else {
                                    gWild_start = 0;
                                }
                            } else if (the_time - gQuite_wild_end < 300) {
                                gOn_me_wheels_start = the_time;
                                gWild_start = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x0041f280
intptr_t C2_HOOK_CDECL TurnOffNonGroovers(br_actor* pActor, void* pData) {
    tUser_crush_data *crush_data = pActor->user;

    if (crush_data != NULL && crush_data->groove == NULL) {
        pActor->type = BR_ACTOR_NONE;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0041f110
void C2_HOOK_FASTCALL DoLODCarModels(void) {
    int i;
    int j;
    int variant;
    tCar_spec* car;
    br_vector3 tv;
    float level;

    for (i = 0; i < gNum_active_cars; i++) {
        car = gActive_car_list[i];

        if (car != NULL && car->driver > 5 && car->car_master_actor->render_style != BR_RSTYLE_NONE) {
            BrVector3Sub(&tv, &car->car_master_actor->t.t.translate.t, (br_vector3*)gCamera_to_world.m[3]);
            level = gCar_simplification_factor[gGraf_spec_index][gCar_simplification_level] >= 0.001f ? BrVector3LengthSquared(&tv) / gCar_simplification_factor[gGraf_spec_index][gCar_simplification_level] : BR_SCALAR_MAX;

            for (j = car->count_detail_levels - 1; j > 0; j--) {
                if (car->detail_levels[j] < level) {
                    break;
                }
            }
            if (j > 0) {
                variant = j;
                DRActorEnumRecurse(car->car_model_actor, SwitchCarModel, &variant);
                car->field_0xe18 = variant;
            } else if (!gAction_replay_mode && car->use_shell_model && car->shell_model != NULL) {
                DRActorEnumRecurse(car->car_model_actor, TurnOffNonGroovers, NULL);
                car->car_model_actor->type = BR_ACTOR_MODEL;
                car->car_model_actor->model = car->shell_model;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x0041f2a0
void C2_HOOK_FASTCALL DoComplexCarModels(void) {
    int i;
    tCar_spec* car;

    for (i = 0; i < gNum_active_cars; i++) {
        car = gActive_car_list[i];
        if (car != NULL && car->driver > 5) {
            SwitchCarModels(car, 0);
        }
    }
}

// FUNCTION: CARMA2_HW 0x0041f300
void C2_HOOK_FASTCALL ResetCarScreens(void) {
    int i;
    int j;

    for (i = 0; i <= 3; i++) {
        int count = (i == 0) ? 1 : GetCarCount(i);
        for (j = 0; j < count; j++) {
            tCar_spec* spec = (i == 0) ? &gProgram_state.current_car : GetCarSpec(i, j);
            spec->collision_info->last_special_volume = NULL;
        }
    }
    MungeCarGraphics(gFrame_period);
}

// FUNCTION: CARMA2_HW 0x0041f360
void C2_HOOK_FASTCALL ToggleCarToCarCollisions(void) {
    gCar_to_car_collisions = gCar_to_car_collisions == 0;
    if (gCar_to_car_collisions != 0) {
        NewTextHeadupSlot(4, 0, 3000, -4, "Car Car Collisions");
    } else {
        NewTextHeadupSlot(4, 0, 3000, -4, "Ghost Cars");
    }
}

// FUNCTION: CARMA2_HW 0x0040f760
void C2_HOOK_FASTCALL CameraBugFix(tCar_spec* c, tU32 pTime) {

    if (gAction_replay_mode
            && (gAction_replay_camera_mode == kActionReplayCameraMode_ActionTracking || gAction_replay_camera_mode == kActionReplayCameraMode_Panning)
            && gPed_actor != NULL
            && !gProgram_state.cockpit_on) {
        IncidentCam(c, pTime);
    }
}

void C2_HOOK_FASTCALL SetTextureBits(tCar_spec* pCar) {

    pCar->field_0x18cc = 0;
    if (pCar->keys.brake || (pCar->brake_force != 0.f && fabsf(pCar->collision_info->velocity_car_space.v[2]) > 7.2463765e-05f)) {
        pCar->field_0x18cc |= 0x4;
    }
    if (pCar->gear < 0 || (!(pCar != NULL && pCar->driver == eDriver_local_human) && pCar->collision_info->velocity_car_space.v[2] > 0.f)) {
        pCar->field_0x18cc |= 0x8;
    }
}

// FUNCTION: CARMA2_HW 0x0041e5a0
void C2_HOOK_FASTCALL MungeSomeOtherCarGraphics(void) {
    int i;

    for (i = 0; i < gNum_active_cars; i++) {
        tCar_spec* car;

        car = gActive_car_list[i];
        if (car->car_master_actor->render_style != BR_RSTYLE_NONE && !gAction_replay_mode) {
            SetTextureBits(car);
        }
    }
}

// FUNCTION: CARMA2_HW 0x00413780
void C2_HOOK_FASTCALL GetAverageGridPosition(tRace_info* pThe_race) {

    BrVector3SetFloat(&gAverage_grid_position, 0.0f, 0.0f, 0.0f);
    if (pThe_race->number_of_racers <= 2) {
        BrVector3Copy(&gAverage_grid_position, &gProgram_state.current_car.pos);
    } else {
        int i;
        br_scalar total_cars;

        total_cars = 0.0f;
        for (i = 0; i < pThe_race->number_of_racers; i++) {
            tCar_spec* car;

            car = pThe_race->opponent_list[i].car_spec;
            BrVector3Accumulate(&gAverage_grid_position, &car->pos);
            total_cars += 1.0f;
        }
        BrVector3InvScale(&gAverage_grid_position, &gAverage_grid_position, total_cars);
    }
}

// C2PORT-NONMATCHING: best 45.73% | difftest PASS 2000 trials (spec difftest/specs/00413860_PickActionReplayManualCameraTarget.json) | reason x87-order, regalloc-loop
// FUNCTION: CARMA2_HW 0x00413860
void C2_HOOK_FASTCALL PickActionReplayManualCameraTarget(int pCursor_x, int pCursor_y, float pArg3) {
    br_actor* camera_actor;
    br_camera* camera_data;
    br_vector3 ray_direction;
    br_vector3 face_normal;
    br_material* face_material;
    br_actor* face_actor;
    float horizontal_offset;
    float vertical_offset;
    br_vector3* camera_position;
    tCar_spec* nearest_car;
    tPedestrian* nearest_pedestrian;
    br_actor* nearest_character_actor;
    tPedestrian* pedestrian;
    int category;
    int car_count;
    int car_index;
    int pedestrian_index;

    camera_actor = gCamera;
    camera_data = (br_camera*)camera_actor->type_data;
    if (gAction_replay_camera_mode != kActionReplayCameraMode_Manual || pArg3 != 0.0f) {
        return;
    }
    ray_direction.v[0] = -camera_actor->t.t.mat.m[2][0] * camera_data->yon_z;
    ray_direction.v[1] = -camera_actor->t.t.mat.m[2][1] * camera_data->yon_z;
    ray_direction.v[2] = -camera_actor->t.t.mat.m[2][2] * camera_data->yon_z;
    vertical_offset = (float)((240 - pCursor_y) * gManualCameraPickScreenScale
        * tan((camera_data->field_of_view >> 1) * gBrAngleToRadiansScale) * camera_data->yon_z);
    pArg3 = (float)(tan((camera_data->field_of_view >> 1) * gBrAngleToRadiansScale) * camera_data->yon_z);
    horizontal_offset = (float)((pCursor_x - 320) * gManualCameraPickScreenScale * pArg3);
    ray_direction.v[0] = camera_actor->t.t.mat.m[0][0] * horizontal_offset
        + (camera_actor->t.t.mat.m[1][0] * vertical_offset + ray_direction.v[0]);
    ray_direction.v[1] = camera_actor->t.t.mat.m[0][1] * horizontal_offset
        + (camera_actor->t.t.mat.m[1][1] * vertical_offset + ray_direction.v[1]);
    ray_direction.v[2] = camera_actor->t.t.mat.m[0][2] * horizontal_offset
        + (camera_actor->t.t.mat.m[1][2] * vertical_offset + ray_direction.v[2]);

    ActorFindFace(&camera_actor->t.t.translate.t, &ray_direction, gTrack_actor, &face_normal, &pArg3, &face_material, &face_actor);
    if (pArg3 < gFLOAT_00589440) {
        ray_direction.v[0] *= pArg3;
        ray_direction.v[1] *= pArg3;
        ray_direction.v[2] *= pArg3;
    }
    camera_position = &gCamera->t.t.translate.t;
    gAction_replay_manual_camera_target_position.v[0] = gCamera->t.t.translate.t.v[0] + ray_direction.v[0];
    gAction_replay_manual_camera_target_position.v[1] = gCamera->t.t.translate.t.v[1] + ray_direction.v[1];
    gAction_replay_manual_camera_target_position.v[2] = gCamera->t.t.translate.t.v[2] + ray_direction.v[2];
    gAction_replay_manual_camera_target_type = 1;
    pArg3 = gFLOAT_00589440;
    nearest_car = NULL;
    category = 0;
    do {
        car_count = category == eVehicle_self ? 1 : GetCarCount(category);
        car_index = 0;
        if (car_count > 0) {
            do {
                tCar_spec* car;
                br_vector3 local_origin;
                br_vector3 local_direction;
                br_vector3 local_end;
                br_vector3 hit_point;

                car = category == eVehicle_self ? &gProgram_state.current_car : GetCarSpec(category, car_index);
                DrMatrix34ApplyLPInverse(&local_origin, camera_position, &car->car_master_actor->t.t.mat);
                BrMatrix34TApplyV(&local_direction, &ray_direction, &car->car_master_actor->t.t.mat);
                BrVector3Add(&local_end, &local_origin, &local_direction);
                FUN_00460bd0(&local_origin, &local_end, &car->collision_info->bb2, &hit_point, &pArg3);
                if (pArg3 < gFLOAT_00589440) {
                    ray_direction.v[0] *= pArg3;
                    ray_direction.v[1] *= pArg3;
                    ray_direction.v[2] *= pArg3;
                    pArg3 = gFLOAT_00589440;
                    nearest_car = car;
                }
                car_index++;
            } while (car_index < car_count);
        }
        category++;
    } while (category < 5);
    gAction_replay_manual_camera_target_car = nearest_car;
    if (nearest_car != NULL) {
        gAction_replay_manual_camera_target_type = 2;
    }

    nearest_pedestrian = NULL;
    nearest_character_actor = NULL;
    pedestrian = gPedestrian_array;
    pedestrian_index = 0;
    if (gPed_count > 0) {
        do {
            if ((pedestrian->flags & 1) != 0) {
                br_actor* character_actor;
                br_vector3 local_origin;
                br_vector3 local_direction;
                br_vector3 local_end;
                br_vector3 hit_point;

                character_actor = GetCharacterActorPtr(pedestrian->character);
                DrMatrix34ApplyLPInverse(&local_origin, camera_position, &character_actor->t.t.mat);
                BrMatrix34TApplyV(&local_direction, &ray_direction, &character_actor->t.t.mat);
                BrVector3Add(&local_end, &local_origin, &local_direction);
                FUN_00460bd0(&local_origin, &local_end, &pedestrian->character->personality->bb, &hit_point, &pArg3);
                if (pArg3 < gFLOAT_00589440) {
                    ray_direction.v[0] *= pArg3;
                    ray_direction.v[1] *= pArg3;
                    ray_direction.v[2] *= pArg3;
                    pArg3 = gFLOAT_00589440;
                    nearest_pedestrian = pedestrian;
                    nearest_character_actor = character_actor;
                }
            }
            pedestrian_index++;
            pedestrian++;
        } while (pedestrian_index < gPed_count);
    }
    gAction_replay_manual_camera_target_pedestrian = nearest_pedestrian;
    if (nearest_pedestrian != NULL) {
        gAction_replay_manual_camera_target_type = 3;
        gAction_replay_manual_camera_target_character_actor = nearest_character_actor;
    }
}

// FUNCTION: CARMA2_HW 0x00413d00
void C2_HOOK_FASTCALL AddEngineDamage(int pAmount) {

    if (pAmount > 0) {
        gProgram_state.current_car.damage_units[eDamage_engine].damage_level += pAmount;
        if (gProgram_state.current_car.damage_units[eDamage_engine].damage_level >= 100) {
            gProgram_state.current_car.damage_units[eDamage_engine].damage_level = 99;
        }
    }
    SortOutSmoke(&gProgram_state.current_car);
}

// FUNCTION: CARMA2_HW 0x004207e0
void C2_HOOK_FASTCALL AdjustNonCar(br_actor* pActor, const br_matrix34* pMatrix) {
    tU8 column_x;
    tU8 column_z;

    BrMatrix34Copy(&pActor->t.t.mat, pMatrix);
    if (pActor->parent != gNon_track_actor) {
        XZToColumnXZ(&column_x, &column_z, pActor->t.t.mat.m[3][0], pActor->t.t.mat.m[3][2], &gProgram_state.track_spec);
        if (gProgram_state.track_spec.columns[column_z][column_x].actor_0x0 != pActor->parent && gProgram_state.track_spec.columns[column_z][column_x].actor_0x0 != NULL) {
            BrActorRemove(pActor);
            BrActorAdd(gProgram_state.track_spec.columns[column_z][column_x].actor_0x0, pActor);
        }
    }
}

// FUNCTION: CARMA2_HW 0x00420880
int C2_HOOK_FASTCALL GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs) {

    if (pCar->collision_info->box_face_ref == gFace_num__car
        || (pCar->collision_info->box_face_ref == gFace_num__car - 1 && gFace_count < pCar->collision_info->box_face_start)) {
        *pFace_refs = &gFace_list__car[pCar->collision_info->box_face_start];
        return pCar->collision_info->box_face_end - pCar->collision_info->box_face_start;
    }
    return 0;
}


void C2_HOOK_FASTCALL FUN_0041deb0(tCar_spec* pCar, const br_vector3* pPosition, const br_vector3* pImpulse, tCar_spec* pOtherCar, undefined4 pArg5, const br_vector3* pPoints, undefined4 pArg7);

// C2PORT-NONMATCHING: best 21.78% | world-smash argument prefix PASS 2003 cases per build (difftest/world_smash_impact_test.py); earlier full-function fixture omits world-smash path | reason remaining decomp/codegen differences
// FUNCTION: CARMA2_HW 0x0041c1b0
int C2_HOOK_FASTCALL ProcessForcesCallback(void* arg1, float* arg2, int arg3)

{
    typedef unsigned int uint;
    typedef unsigned char undefined1;
    typedef unsigned char byte;

  int iVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  char cVar7;
  char *pcVar8;
  float fVar9;
  int iVar10;
  undefined4 uVar11;
  register int iVar12;
  uint uVar13;
  float *pfVar14;
  int iVar15;
  float *pfVar16;
  float *pfVar17;
  float *pfVar18;
  int *piVar19;
  char *pcVar20;
  int iVar21;
  int *piVar22;
  int *piVar23;
  double fVar24;
  undefined1 *puVar25;
  undefined4 *puVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  int local_194;
  int local_190;
  float *local_170;
  float *local_16c;
  float *local_168;
  float *local_158;
  float *local_154;
  float *local_11c;
  float *local_100;
  int local_e4;
  /* port: the original's float locals form one contiguous frame (0x19c..0x0 below the
   * frame top); this code addresses vectors across neighbouring locals (&local_c8, &local_70,
   * &local_bc[3], &local_80 - 1, ...), so they keep that exact layout here */
  float pfc_frame[103];
#define local_19c (pfc_frame[0])
#define local_18c (pfc_frame[4])
#define local_188 (pfc_frame[5])
#define local_184 (pfc_frame[6])
#define local_180 (pfc_frame[7])
#define local_17c (pfc_frame[8])
#define local_178 (pfc_frame[9])
#define local_174 (pfc_frame[10])
#define local_164 (pfc_frame[14])
#define local_160 (pfc_frame[15])
#define local_15c (pfc_frame[16])
#define local_150 (pfc_frame[19])
#define local_14c (pfc_frame[20])
#define local_148 (pfc_frame[21])
#define local_144 (pfc_frame[22])
#define local_140 (pfc_frame[23])
#define local_13c (pfc_frame[24])
#define local_138 (pfc_frame[25])
#define local_134 (pfc_frame[26])
#define local_130 (pfc_frame[27])
#define local_12c (pfc_frame[28])
#define local_128 (pfc_frame[29])
#define local_124 (pfc_frame[30])
#define local_120 (pfc_frame[31])
#define local_118 (pfc_frame[33])
#define local_114 (pfc_frame[34])
#define local_110 (pfc_frame[35])
#define local_10c (pfc_frame[36])
#define local_108 (pfc_frame[37])
#define local_104 (pfc_frame[38])
#define local_fc (pfc_frame[40])
#define local_f8 (pfc_frame[41])
#define local_f4 (pfc_frame[42])
#define local_f0 ((undefined1*)&pfc_frame[43])
#define local_e0 (pfc_frame[47])
#define local_dc (pfc_frame[48])
#define local_d8 (pfc_frame[49])
#define local_d4 (pfc_frame[50])
#define local_d0 (pfc_frame[51])
#define local_cc (pfc_frame[52])
#define local_c8 (*(undefined4*)&pfc_frame[53])
#define local_c4 (*(undefined4*)&pfc_frame[54])
#define local_c0 (*(undefined4*)&pfc_frame[55])
#define local_bc (&pfc_frame[56])
#define local_ac (pfc_frame[60])
#define local_a8 (pfc_frame[61])
#define local_a4 (pfc_frame[62])
#define local_a0 (pfc_frame[63])
#define local_9c (&pfc_frame[64])
#define local_80 (pfc_frame[71])
#define local_7c (pfc_frame[72])
#define local_78 (pfc_frame[73])
#define local_74 (pfc_frame[74])
#define local_70 (pfc_frame[75])
#define local_6c (&pfc_frame[76])
#define local_50 (&pfc_frame[83])
  float *vector_a;
  float *vector_b;
  int collision_index;
  int index_a;
  int index_b;
  float edge_delta;
  int collision_object_a;
  int collision_object_b;
  tCar_spec* force_car;
  br_vector3* force_position;
  int param_1;
  float* param_2;
  unsigned int param_3;
  param_1 = (int)arg1;
  param_2 = arg2;
  param_3 = (unsigned int)arg3;
  local_19c = -1.0;
  local_194 = 0;
  collision_index = 0;
  iVar12 = 0;
  local_100 = param_2;
  local_e4 = param_1;
  if (0 < (int)param_3) {
    local_16c = local_50;
    piVar19 = (int *)(param_1 + 0xc);
    do {
      if (gFloat_00589808 < *param_2) {
        local_11c = param_2;
        if ((piVar19[-1] == 0) && (piVar23 = (int *)*piVar19, piVar23 != (int *)0x0)) {
          if ((*piVar23 == 0) || (pcVar8 = *(char **)(*piVar23 + 4), pcVar8 == (char *)0x0)) {
LAB_0041c24e:
            if ((piVar23[0x12] != 0) &&
               (pcVar8 = *(char **)(piVar23[0x12] + 0x14), pcVar8 != (char *)0x0)) {
              iVar12 = -1;
              pcVar20 = pcVar8;
              do {
                if (iVar12 == 0) break;
                iVar12 = iVar12 + -1;
                cVar7 = *pcVar20;
                pcVar20 = pcVar20 + 1;
              } while (cVar7 != '\0');
              if (((iVar12 == -0xd) && (pcVar8[5] == '|')) || (pcVar8[1] == -0x5d))
              goto LAB_0041c285;
            }
            goto LAB_0041c87c;
          }
          iVar12 = -1;
          pcVar20 = pcVar8;
          do {
            if (iVar12 == 0) break;
            iVar12 = iVar12 + -1;
            cVar7 = *pcVar20;
            pcVar20 = pcVar20 + 1;
          } while (cVar7 != '\0');
          if ((iVar12 != -0xd) || (pcVar8[5] != '|')) goto LAB_0041c24e;
LAB_0041c285:
          /* Retail 0x41c285..0x41c2d3 loads float slots directly (fld), not integer conversions. */
          local_e0 = *param_2 * ((float*)piVar19)[8];
          local_dc = *param_2 * ((float*)piVar19)[9];
          iVar12 = piVar19[-2];
          local_d8 = ((float*)piVar19)[10] * *param_2;
          local_138 = *(float *)(iVar12 + 0x14) + ((float*)piVar19)[2];
          local_134 = *(float *)(iVar12 + 0x18) + ((float*)piVar19)[3];
          local_130 = *(float *)(iVar12 + 0x1c) + ((float*)piVar19)[4];
          if (piVar19[0x14] == 0) {
            local_12c = ((float*)piVar19)[5];
            local_128 = ((float*)piVar19)[6];
            local_124 = ((float*)piVar19)[7];
            pfVar18 = (float *)(piVar23 + 0xd);
            fVar24 = (double)DistanceFromFaceND(&local_12c,pfVar18,((float*)piVar23)[0x11]);
            param_2 = local_11c;
            if (fabs(fVar24) <= (double)gDouble_00589638) {
              fVar3 = (float)-fVar24;
              local_bc[0] = (float)(-fVar24 * (double)*pfVar18 + (double)local_12c);
              local_bc[1] = ((float*)piVar23)[0xe] * fVar3 + local_128;
              local_bc[2] = ((float*)piVar23)[0xf] * fVar3 + local_124;
              uVar13 = (uint)(fabs(*pfVar18) < fabs(((float*)piVar23)[0xe]));
              if (fabs(pfVar18[uVar13]) < fabs(((float*)piVar23)[0xf])) {
                uVar13 = 2;
              }
              if (uVar13 == 0) {
                index_a = 1;
LAB_0041c403:
                index_b = 2;
              }
              else {
                if (uVar13 == 1) {
                  index_a = 0;
                  goto LAB_0041c403;
                }
                if (uVar13 == 2) {
                  index_a = 0;
                  index_b = 1;
                }
              }
              fVar3 = ((float*)piVar23)[index_a + 1];
              edge_delta = ((float*)piVar23)[index_a + 4] - fVar3;
              fVar4 = ((float*)piVar23)[index_b + 1];
              local_120 = ((float*)piVar23)[index_b + 4] - fVar4;
              fVar5 = ((float*)piVar23)[index_a + 7] - fVar3;
              fVar3 = local_bc[index_a] - fVar3;
              local_180 = local_bc[index_b] - fVar4;
              if ((float)gDouble_00589810 < fabs(edge_delta)) {
                fVar6 = (((float*)piVar23)[index_b + 7] - fVar4) * edge_delta -
                        local_120 * fVar5;
                if (fVar6 == gFlyCarZero) goto LAB_0041c54a;
                fVar6 = (local_180 * edge_delta - fVar3 * local_120) / fVar6;
                fVar3 = (fVar3 - fVar6 * fVar5) / edge_delta;
              }
              else {
                fVar6 = fVar3 / fVar5;
                fVar3 = (local_180 - fVar6 * (((float*)piVar23)[index_b + 7] - fVar4)) /
                        local_120;
              }
              if ((((float)gDouble_00589818 <= fVar3) && ((float)gDouble_00589818 <= fVar6)) &&
                 (fVar3 + fVar6 <= (float)gDouble_00589820)) goto LAB_0041c72e;
            }
LAB_0041c54a:
            iVar12 = *piVar19;
            fVar4 = local_12c - *(float *)(iVar12 + 4);
            fVar3 = local_128 - *(float *)(iVar12 + 8);
            fVar5 = local_12c - *(float *)(iVar12 + 0x10);
            fVar9 = local_128 - *(float *)(iVar12 + 0x14);
            local_120 = local_124 - *(float *)(iVar12 + 0xc);
            fVar6 = local_124 - *(float *)(iVar12 + 0x18);
            fVar3 = local_120 * local_120 + fVar3 * fVar3 + fVar4 * fVar4;
            local_12c = local_12c - *(float *)(iVar12 + 0x1c);
            local_128 = local_128 - *(float *)(iVar12 + 0x20);
            fVar4 = fVar6 * fVar6 + fVar9 * fVar9 + fVar5 * fVar5;
            local_124 = local_124 - *(float *)(iVar12 + 0x24);
            fVar5 = local_124 * local_124 + local_128 * local_128 + local_12c * local_12c;
            if ((fVar4 <= fVar3) || (fVar5 <= fVar3)) {
              if ((fVar3 <= fVar4) || (fVar5 <= fVar4)) {
                iVar10 = 2;
                iVar15 = 1;
                iVar21 = 0;
              }
              else {
                iVar10 = 1;
                iVar15 = 0;
                iVar21 = 2;
              }
            }
            else {
              iVar10 = 0;
              iVar15 = 1;
              iVar21 = 2;
            }
            iVar1 = iVar12 + iVar10 * 0xc;
            iVar2 = iVar12 + iVar21 * 0xc;
            local_12c = *(float *)(iVar1 + 4) * (float)gDouble_00589828 -
                        (*(float *)(iVar12 + 4 + iVar15 * 0xc) + *(float *)(iVar2 + 4)) *
                        (float)gDouble_00589740;
            local_128 = *(float *)(iVar1 + 8) * (float)gDouble_00589828 -
                        (*(float *)(iVar2 + 8) + *(float *)(iVar12 + iVar15 * 0xc + 8)) *
                        (float)gDouble_00589740;
            local_124 = *(float *)(iVar12 + (iVar10 + 1) * 0xc) * (float)gDouble_00589828 -
                        (*(float *)(iVar12 + (iVar21 + 1) * 0xc) +
                        *(float *)(iVar12 + (iVar15 + 1) * 0xc)) * (float)gDouble_00589740;
          }
          else {
            BrMatrix34ApplyP((br_vector3*)&local_12c, (const br_vector3*)&local_138, (const br_matrix34*)((char*)piVar19[-2] + 0x8c));
          }
LAB_0041c72e:
          iVar12 = piVar19[-2];
          local_118 = *(float *)(iVar12 + 0x1a8) +
                      (*(float *)(iVar12 + 0x78) * local_130 - *(float *)(iVar12 + 0x7c) * local_134
                      );
          local_114 = *(float *)(iVar12 + 0x1ac) +
                      (*(float *)(iVar12 + 0x7c) * local_138 - *(float *)(iVar12 + 0x74) * local_130
                      );
          local_110 = *(float *)(iVar12 + 0x1b0) +
                      (*(float *)(iVar12 + 0x74) * local_134 - *(float *)(iVar12 + 0x78) * local_138
                      );
          BrMatrix34ApplyV((br_vector3*)&local_c8, (const br_vector3*)&local_118, (const br_matrix34*)((char*)iVar12 + 0x8c));
          BrMatrix34ApplyV((br_vector3*)&local_d4, (const br_vector3*)&local_e0, (const br_matrix34*)((char*)iVar12 + 0x8c));
          fVar24 = (double)SmashEnvironment((tPhysics_object*)piVar19[-2],(undefined4*)*piVar19,*param_2,(br_vector3*)&local_12c,
                                             (br_vector3*)&local_c8,(br_vector3*)&local_d4,1,0);
          if ((fVar24 != (double)gFloat_00589668) && (local_19c == gFloat_00589668)) {
            local_19c = (float)fVar24;
          }
          if ((local_19c == gFloat_00589668) || (0x13 < local_194)) goto LAB_0041cb2a;
LAB_0041cb10:
          *(int*)local_16c = collision_index;
          local_194 = local_194 + 1;
          local_16c = local_16c + 1;
        }
        else {
LAB_0041c87c:
          if (piVar19[-1] == 0) goto LAB_0041cb2a;
          piVar23 = (int *)piVar19[-2];
          piVar22 = (int *)0x0;
          if (((*piVar23 == 0) || (pcVar8 = *(char **)(*piVar23 + 0x14), pcVar8 == (char *)0x0)) ||
             ((*pcVar8 != '&' ||
              (gUnknown_006b9c60[(int)pcVar8[2] + pcVar8[1] * 10] == 0)))) {
            piVar23 = (int *)piVar19[-1];
            if (((*piVar23 != 0) && (pcVar8 = *(char **)(*piVar23 + 0x14), pcVar8 != (char *)0x0))
               && ((*pcVar8 == '&' &&
                   (gUnknown_006b9c60[(int)pcVar8[2] + pcVar8[1] * 10] != 0)))) {
              local_e0 = *param_2 * (float)piVar19[0xb];
              local_dc = *param_2 * (float)piVar19[0xc];
              local_d8 = (float)piVar19[0xd] * *param_2;
              fVar3 = (float)piVar19[6];
              local_138 = (float)piVar19[5] + (float)piVar23[5];
              fVar4 = (float)piVar19[7];
              goto LAB_0041c95f;
            }
          }
          else {
            local_e0 = *param_2 * (float)piVar19[8];
            local_dc = *param_2 * (float)piVar19[9];
            local_d8 = (float)piVar19[10] * *param_2;
            fVar3 = (float)piVar19[3];
            local_138 = (float)piVar19[2] + (float)piVar23[5];
            fVar4 = (float)piVar19[4];
LAB_0041c95f:
            local_134 = fVar3 + (float)piVar23[6];
            local_130 = fVar4 + (float)piVar23[7];
            piVar22 = piVar23;
          }
          if (piVar22 != (int *)0x0) {
            local_118 = (float)piVar22[0x6a] +
                        ((float)piVar22[0x1e] * local_130 - (float)piVar22[0x1f] * local_134);
            local_114 = (float)piVar22[0x6b] +
                        ((float)piVar22[0x1f] * local_138 - (float)piVar22[0x1d] * local_130);
            local_110 = (float)piVar22[0x6c] +
                        ((float)piVar22[0x1d] * local_134 - (float)piVar22[0x1e] * local_138);
            BrMatrix34ApplyV((br_vector3*)&local_c8, (const br_vector3*)&local_118, (const br_matrix34*)((char*)piVar22 + 0x8c));
            BrMatrix34ApplyV((br_vector3*)&local_d4, (const br_vector3*)&local_e0, (const br_matrix34*)((char*)piVar22 + 0x8c));
            BrMatrix34ApplyP((br_vector3*)&local_12c, (const br_vector3*)&local_138, (const br_matrix34*)((char*)piVar22 + 0x8c));
            local_d4 = local_d4 * (float)gDouble_00589628;
            local_d0 = local_d0 * (float)gDouble_00589628;
            local_cc = local_cc * (float)gDouble_00589628;
            fVar24 = (double)BashObject((tPhysics_object*)piVar22,(br_actor*)*piVar22,*param_2,(br_vector3*)&local_12c,
                                         (br_vector3*)&local_c8,(br_vector3*)&local_d4,0,0);
            if ((fVar24 != (double)gFloat_00589668) && (local_19c == gFloat_00589668)) {
              local_19c = (float)fVar24;
            }
            if ((local_19c != gFloat_00589668) && (local_194 < 0x14)) goto LAB_0041cb10;
          }
        }
      }
LAB_0041cb2a:
      collision_index++;
      piVar19 = piVar19 + 0x18;
      param_2 = param_2 + 1;
      iVar12 = local_194;
      local_11c = param_2;
    } while (collision_index < (int)param_3);
  }
  FlushSmashQueue(1);
  if (iVar12 != 0) {
    FUN_004ba550(param_1,(float*)local_100,(int)param_3,(int*)local_50,local_194,local_19c);
    return 0;
  }
  iVar12 = TrySplitCarOnVerticalForce((tPhysics_joint_data*)param_1,(float*)local_100,(int)param_3);
  if ((iVar12 == 0) && (iVar12 = FullyDetachSnappedBits((struct tCollision_contact*)param_1,(float*)local_100,(int)param_3), iVar12 == 0)) {
    if (0 < (int)param_3) {
      pfVar18 = local_50;
      for (uVar13 = param_3 >> 2; uVar13 != 0; uVar13 = uVar13 - 1) {
        *pfVar18 = 0.0;
        pfVar18 = pfVar18 + 1;
      }
      for (uVar13 = param_3 & 3; uVar13 != 0; uVar13 = uVar13 - 1) {
        *(undefined1 *)pfVar18 = 0;
        pfVar18 = (float *)((int)pfVar18 + 1);
      }
    }
    local_190 = 0;
    if (0 < (int)param_3) {
      do {
        if (*(char *)((int)local_50 + local_190) == '\0') {
          local_11c = (float *)0x0;
          local_17c = local_100[local_190] * *(float *)(local_190 * 0x60 + 0x14 + local_e4);
          iVar12 = local_190 * 0x60 + local_e4;
          local_178 = local_100[local_190] * *(float *)(iVar12 + 0x18);
          local_174 = local_100[local_190] * *(float *)(iVar12 + 0x1c);
          local_150 = local_100[local_190] * *(float *)(iVar12 + 0x2c);
          iVar10 = *(int *)(iVar12 + 4);
          iVar15 = *(int *)(iVar12 + 8);
          local_14c = *(float *)(iVar12 + 0x30) * local_100[local_190];
          local_148 = *(float *)(iVar12 + 0x34) * local_100[local_190];
          local_bc[3] = *(float *)(iVar12 + 0x14) + *(float *)(iVar10 + 0x14);
          local_ac = *(float *)(iVar12 + 0x18) + *(float *)(iVar10 + 0x18);
          local_a8 = *(float *)(iVar10 + 0x1c) + *(float *)(iVar12 + 0x1c);
          if (iVar15 != 0) {
            local_164 = local_100[local_190] * *(float *)(iVar12 + 0x20);
            local_160 = local_100[local_190] * *(float *)(iVar12 + 0x24);
            local_15c = local_100[local_190] * *(float *)(iVar12 + 0x28);
            local_104 = -local_100[local_190];
            local_10c = *(float *)(iVar12 + 0x38) * local_104;
            local_108 = *(float *)(iVar12 + 0x3c) * local_104;
            local_104 = *(float *)(iVar12 + 0x40) * local_104;
            local_80 = *(float *)(iVar15 + 0x14) + *(float *)(iVar12 + 0x20);
            local_7c = *(float *)(iVar15 + 0x18) + *(float *)(iVar12 + 0x24);
            local_78 = *(float *)(iVar15 + 0x1c) + *(float *)(iVar12 + 0x28);
          }
          local_19c = local_100[local_190];
          *(undefined1 *)((int)local_50 + local_190) = 1;
          local_180 = (float)(local_190 + 1);
          local_194 = 1;
          if ((int)local_180 < (int)param_3) {
            local_16c = &local_70;
            local_168 = local_6c;
            vector_a = &local_a0;
            local_170 = &local_74;
            vector_b = local_9c;
            pfVar18 = local_100 + (int)local_180;
            pfVar14 = (float *)((int)local_180 * 0x60 + 0x3c + local_e4);
            pfVar16 = &local_a4;
            do {
              if (local_194 == 4) break;
              pfVar17 = pfVar16;
              if (*(char *)((int)local_50 + (int)local_180) == '\0') {
                collision_object_a = *(int*)(pfVar14 - 0xe);
                if (((collision_object_a != *(int *)(iVar12 + 4)) ||
                    (collision_object_b = *(int*)(pfVar14 - 0xd), collision_object_b != *(int *)(iVar12 + 8))) ||
                   (*(float *)(iVar12 + 0x34) * pfVar14[-2] +
                    pfVar14[-3] * *(float *)(iVar12 + 0x30) +
                    *(float *)(iVar12 + 0x2c) * pfVar14[-4] <= gFloat_00589830)) {
                  collision_object_b = *(int*)(pfVar14 - 0xd);
                  if (((collision_object_b == *(int *)(iVar12 + 4)) && (collision_object_a == *(int *)(iVar12 + 8))) &&
                     (*pfVar14 * *(float *)(iVar12 + 0x30) +
                      pfVar14[-1] * *(float *)(iVar12 + 0x2c) +
                      *(float *)(iVar12 + 0x34) * pfVar14[1] < gFloat_00589834)) {
                    if (*pfVar18 != gFlyCarZero) {
                      local_17c = *pfVar18 * pfVar14[-7] + local_17c;
                      local_178 = *pfVar18 * pfVar14[-6] + local_178;
                      fVar5 = -*pfVar18;
                      local_174 = *pfVar18 * pfVar14[-5] + local_174;
                      local_150 = pfVar14[-1] * fVar5 + local_150;
                      local_14c = *pfVar14 * fVar5 + local_14c;
                      local_148 = pfVar14[1] * fVar5 + local_148;
                      local_164 = pfVar14[-10] * *pfVar18 + local_164;
                      local_160 = pfVar14[-9] * *pfVar18 + local_160;
                      local_15c = *pfVar18 * pfVar14[-8] + local_15c;
                      local_144 = *pfVar18 * pfVar14[-4];
                      local_140 = pfVar14[-3] * *pfVar18;
                      local_13c = *pfVar18 * pfVar14[-2];
                      local_10c = local_144 + local_10c;
                      local_19c = local_19c + *pfVar18;
                      local_108 = local_140 + local_108;
                      local_104 = local_13c + local_104;
                    }
                    pfVar17 = pfVar16 + 3;
                    local_194 = local_194 + 1;
                    *pfVar16 = *(float *)(collision_object_b + 0x14) + pfVar14[-7];
                    fVar5 = *(float *)(collision_object_b + 0x18);
                    fVar6 = pfVar14[-6];
                    *(undefined1 *)((int)local_50 + (int)local_180) = 1;
                    *vector_a = fVar5 + fVar6;
                    *vector_b = *(float *)(collision_object_b + 0x1c) + pfVar14[-5];
                    *local_170 = *(float *)(collision_object_a + 0x14) + pfVar14[-10];
                    *local_16c = *(float *)(collision_object_a + 0x18) + pfVar14[-9];
                    *local_168 = *(float *)(collision_object_a + 0x1c) + pfVar14[-8];
                    local_170 = local_170 + 3;
                    local_16c = local_16c + 3;
                    local_168 = local_168 + 3;
                    vector_a = vector_a + 3;
                    vector_b = vector_b + 3;
                  }
                }
                else {
                  if (*pfVar18 != gFlyCarZero) {
                    local_17c = pfVar14[-10] * *pfVar18 + local_17c;
                    local_178 = pfVar14[-9] * *pfVar18 + local_178;
                    local_174 = pfVar14[-8] * *pfVar18 + local_174;
                    local_144 = *pfVar18 * pfVar14[-4];
                    local_140 = *pfVar18 * pfVar14[-3];
                    local_150 = local_144 + local_150;
                    local_14c = local_140 + local_14c;
                    local_13c = *pfVar18 * pfVar14[-2];
                    local_19c = local_19c + *pfVar18;
                    local_148 = local_13c + local_148;
                  }
                  *pfVar16 = *(float *)(collision_object_a + 0x14) + pfVar14[-10];
                  *vector_a = *(float *)(collision_object_a + 0x18) + pfVar14[-9];
                  *vector_b = *(float *)(collision_object_a + 0x1c) + pfVar14[-8];
                  if (collision_object_b != 0) {
                    if (*pfVar18 != gFlyCarZero) {
                      local_164 = *pfVar18 * pfVar14[-7] + local_164;
                      local_160 = *pfVar18 * pfVar14[-6] + local_160;
                      local_13c = -*pfVar18;
                      local_15c = pfVar14[-5] * *pfVar18 + local_15c;
                      local_144 = pfVar14[-1] * local_13c;
                      local_140 = *pfVar14 * local_13c;
                      local_10c = local_144 + local_10c;
                      local_108 = local_140 + local_108;
                      local_13c = pfVar14[1] * local_13c;
                      local_104 = local_13c + local_104;
                    }
                    *local_170 = *(float *)(collision_object_b + 0x14) + pfVar14[-7];
                    *local_16c = *(float *)(collision_object_b + 0x18) + pfVar14[-6];
                    *local_168 = *(float *)(collision_object_b + 0x1c) + pfVar14[-5];
                  }
                  local_194 = local_194 + 1;
                  *(undefined1 *)((int)local_50 + (int)local_180) = 1;
                  pfVar17 = pfVar16 + 3;
                  local_170 = local_170 + 3;
                  local_16c = local_16c + 3;
                  local_168 = local_168 + 3;
                  vector_a = vector_a + 3;
                  vector_b = vector_b + 3;
                }
              }
              local_180 = (float)((int)local_180 + 1);
              pfVar18 = pfVar18 + 1;
              pfVar14 = pfVar14 + 0x18;
              pfVar16 = pfVar17;
            } while ((int)local_180 < (int)param_3);
          }
          iVar10 = *(int *)(iVar12 + 4);
          iVar15 = *(int *)(iVar12 + 8);
          local_17c = local_17c / local_19c + *(float *)(iVar10 + 0x14);
          local_178 = local_178 / local_19c + *(float *)(iVar10 + 0x18);
          local_174 = local_174 / local_19c + *(float *)(iVar10 + 0x1c);
          if (iVar15 != 0) {
            local_164 = *(float *)(iVar15 + 0x14) + local_164 / local_19c;
            local_160 = *(float *)(iVar15 + 0x18) + local_160 / local_19c;
            local_15c = *(float *)(iVar15 + 0x1c) + local_15c / local_19c;
          }
          if (((iVar10 == 0) || (*(int *)(iVar10 + 0x23c) == 0)) ||
             ((*(char *)(iVar10 + 0x238) != '\x01' ||
              (*(int *)(*(int *)(iVar10 + 0x23c) + 0xc) != 8)))) {
            if ((((iVar15 != 0) && (*(int *)(iVar15 + 0x23c) != 0)) &&
                (*(char *)(iVar15 + 0x238) == '\x01')) &&
               ((*(int *)(*(int *)(iVar15 + 0x23c) + 0xc) == 8 && (gFloat_00589808 < local_19c)))) {
              pfVar18 = &local_10c;
              force_car = (tCar_spec*)*(int*)(iVar15 + 0x23c);
              force_position = (br_vector3*)&local_164;
              goto LAB_0041d351;
            }
          }
          else if (gFloat_00589808 < local_19c) {
            pfVar18 = &local_150;
            force_car = (tCar_spec*)*(int*)(iVar10 + 0x23c);
            force_position = (br_vector3*)&local_17c;
LAB_0041d351:
            FUN_0045c910(force_car,force_position,pfVar18,local_19c);
          }
          puVar26 = *(undefined4 **)(iVar12 + 4);
          pfVar18 = local_11c;
          if (((puVar26 != (undefined4 *)0x0) && (puVar26[0x8f] != 0)) &&
             ((*(char *)(puVar26 + 0x8e) == '\x01' && (gFloat_00589808 < local_19c)))) {
            iVar10 = *(int *)(iVar12 + 8);
            if (((iVar10 == 0) || (*(int *)(iVar10 + 0x23c) == 0)) ||
               (*(char *)(iVar10 + 0x238) != '\x01')) {
              FUN_0041deb0((tCar_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c), &local_17c, &local_150, NULL, local_194, (br_vector3*)&local_bc[3], *puVar26);
              pfVar18 = local_11c;
              if ((*(int *)(iVar12 + 8) == 0) && (*(int *)(iVar12 + 0xc) != 0)) {
                if (*(float *)(*(int *)(iVar12 + 4) + 0x6c) < gFloat_00589840) {
                  local_150 = local_150 + local_150;
                  local_14c = local_14c + local_14c;
                  local_148 = local_148 + local_148;
                }
                CrashEarnings((tCar_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c), NULL);
                iVar10 = *(int *)(iVar12 + 4);
                pfVar18 = local_11c;
                if ((float)gDouble_00589838 <
                    *(float *)(iVar10 + 0x70) * *(float *)(iVar10 + 0x70) +
                    *(float *)(iVar10 + 0x6c) * *(float *)(iVar10 + 0x6c) +
                    *(float *)(iVar10 + 0x68) * *(float *)(iVar10 + 0x68)) {
                  local_144 = *(float *)(iVar10 + 0x1a8) +
                              (*(float *)(iVar10 + 0x78) * local_174 -
                              *(float *)(iVar10 + 0x7c) * local_178);
                  local_140 = *(float *)(iVar10 + 0x1ac) +
                              (*(float *)(iVar10 + 0x7c) * local_17c -
                              *(float *)(iVar10 + 0x74) * local_174);
                  local_13c = *(float *)(iVar10 + 0x1b0) +
                              (*(float *)(iVar10 + 0x74) * local_178 -
                              *(float *)(iVar10 + 0x78) * local_17c);
                  BrMatrix34ApplyV((br_vector3*)&local_144, (const br_vector3*)&local_fc, (const br_matrix34*)((char*)iVar10 + 0x8c));
                  BrMatrix34ApplyV((br_vector3*)&local_150, (const br_vector3*)&local_18c, (const br_matrix34*)((char*)iVar10 + 0x8c));
                  BrMatrix34ApplyP((br_vector3*)&local_17c, (const br_vector3*)local_f0, (const br_matrix34*)((char*)iVar10 + 0x8c));
                  local_18c = local_18c * (float)gDouble_00589628;
                  local_188 = local_188 * (float)gDouble_00589628;
                  local_184 = local_184 * (float)gDouble_00589628;
                  iVar10 = **(char **)(**(int **)(iVar12 + 0xc) + 4) + -0x2f;
                  if ((iVar10 < 0) || (9 < iVar10)) {
                    iVar10 = 0;
                  }
                  pfVar18 = (float *)0x1;
                  iVar15 = *(int *)(*(int *)(iVar12 + 4) + 0x23c);
                  if (((iVar15 != 0) && (5 < *(int *)(iVar15 + 0xc))) ||
                     ((*(uint *)(iVar15 + 0x100) & 0x200000) != 0)) {
                    CreateSparks((const br_vector3*)local_f0,(br_vector3*)&local_fc,(const br_vector3*)&local_18c,
                                 gFriction_materials[iVar10].sparkiness,(tCar_spec*)iVar15);
                  }
                  CrashNoise((br_vector3*)&local_18c,(br_vector3*)local_f0,iVar10);
                  iVar15 = (int)((int)(sqrt(((float*)local_f0)[0] * ((float*)local_f0)[0] + ((float*)local_f0)[1] * ((float*)local_f0)[1] + ((float*)local_f0)[2] * ((float*)local_f0)[2]) * gFloat_005896d8) * (float)gDouble_00589628);
                  if (gFriction_materials[iVar10].scrape_noise_index != -1) {
                    if (((gINT_00679400 == 0) || (iVar10 = DRS3SoundStillPlaying(gINT_00679400), iVar10 == 0)) &&
                       (0x1e < iVar15)) {
                      uVar28 = 0x10000;
                      local_c8 = 0;
                      local_c4 = 0;
                      local_c0 = 0;
                      uVar11 = IRandomBetween(0xc000,0x14000);
                      puVar26 = &local_c8;
                      puVar25 = local_f0;
                      uVar27 = 1;
                      iVar10 = iVar15;
                      iVar21 = IRandomBetween(0,2);
                      gINT_00679400 = DRS3StartSound3D(gCar_outlet,
                                                      gINT_0058f698[iVar21],
                                                      puVar25,puVar26,uVar27,iVar10,uVar11,uVar28);
                      gINT_00679404 = iVar15;
                    }
                    else if (gINT_00679404 < iVar15) {
                      DRS3ChangeVolume(gINT_00679400,iVar15);
                      gINT_00679404 = iVar15;
                    }
                  }
                }
              }
            }
            else {
              FUN_0041deb0((tCar_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c), &local_17c, &local_150, (tCar_spec*)*(int*)(iVar10 + 0x23c), local_194, (br_vector3*)&local_bc[3], *puVar26);
              FUN_0041deb0((tCar_spec*)*(int*)(*(int*)(iVar12 + 8) + 0x23c), &local_164, &local_10c, (tCar_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c), local_194, (const br_vector3*)((char*)&local_80 - sizeof(float)), **(undefined4 **)(iVar12 + 8));
              CrashEarnings((tCar_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c), (tCar_spec*)*(int*)(*(int*)(iVar12 + 8) + 0x23c));
              iVar10 = *(int *)(iVar12 + 4);
              if (((float)gDouble_00589838 <
                   *(float *)(iVar10 + 0x70) * *(float *)(iVar10 + 0x70) +
                   *(float *)(iVar10 + 0x6c) * *(float *)(iVar10 + 0x6c) +
                   *(float *)(iVar10 + 0x68) * *(float *)(iVar10 + 0x68)) ||
                 (iVar10 = *(int *)(iVar12 + 8), pfVar18 = local_11c,
                 (float)gDouble_00589838 <
                 *(float *)(iVar10 + 0x70) * *(float *)(iVar10 + 0x70) +
                 *(float *)(iVar10 + 0x6c) * *(float *)(iVar10 + 0x6c) +
                 *(float *)(iVar10 + 0x68) * *(float *)(iVar10 + 0x68))) {
                iVar10 = *(int *)(iVar12 + 8);
                local_fc = (*(float *)(iVar10 + 0x68) + *(float *)(iVar10 + 0x68)) * gFloat_00589650;
                local_f8 = (*(float *)(iVar10 + 0x6c) + *(float *)(iVar10 + 0x6c)) * gFloat_00589650;
                local_f4 = (*(float *)(iVar10 + 0x70) + *(float *)(iVar10 + 0x70)) * gFloat_00589650;
                /* Retail 0x41d491..0x41d515: object1-local force/point -> world. */
                BrMatrix34ApplyV((br_vector3*)&local_18c, (const br_vector3*)&local_150,
                    &(*(tPhysics_object**)(iVar12 + 4))->transform_matrix);
                BrMatrix34ApplyP((br_vector3*)local_f0, (const br_vector3*)&local_17c,
                    &(*(tPhysics_object**)(iVar12 + 4))->transform_matrix);
                local_18c = local_18c * (float)gDouble_00589628;
                local_188 = local_188 * (float)gDouble_00589628;
                local_184 = local_184 * (float)gDouble_00589628;
                CreateSparkShower((br_vector3*)local_f0,(br_vector3*)&local_fc,(br_vector3*)&local_18c,
                                  (tNon_car_spec*)*(int*)(*(int*)(iVar12 + 4) + 0x23c),
                                  (tNon_car_spec*)*(int*)(*(int*)(iVar12 + 8) + 0x23c));
                CrashNoise((br_vector3*)&local_18c,(br_vector3*)local_f0,0);
                pfVar18 = (float *)1;
              }
            }
          }
          if (gFloat_0058969c < local_19c) {
            iVar10 = *(int *)(iVar12 + 4);
            if (((((iVar10 == 0) || (*(int *)(iVar10 + 0x23c) == 0)) ||
                 (*(byte *)(iVar10 + 0x238) < 2)) || (4 < *(byte *)(iVar10 + 0x238))) ||
               (((iVar15 = *(int *)(iVar12 + 8), iVar15 != 0 && (*(int *)(iVar15 + 0x23c) != 0)) &&
                ((1 < *(byte *)(iVar15 + 0x238) && (*(byte *)(iVar15 + 0x238) < 5)))))) {
              iVar15 = *(int *)(iVar12 + 8);
              if (((((iVar15 != 0) && (*(int *)(iVar15 + 0x23c) != 0)) &&
                   (1 < *(byte *)(iVar15 + 0x238))) && (*(byte *)(iVar15 + 0x238) < 5)) &&
                 ((((iVar10 == 0 || (*(int *)(iVar10 + 0x23c) == 0)) ||
                   (*(byte *)(iVar10 + 0x238) < 2)) || (4 < *(byte *)(iVar10 + 0x238))))) {
                BrMatrix34ApplyP((br_vector3*)&local_17c, (const br_vector3*)local_f0, (const br_matrix34*)((char*)iVar15 + 0x8c));
                FUN_004d2320((tPhysics_object*)iVar15,(tPhysics_object*)iVar10,(void*)*(undefined4*)(iVar12 + 0xc),
                             (undefined4)local_100[local_190],(br_vector3*)local_f0);
              }
            }
            else {
              BrMatrix34ApplyP((br_vector3*)&local_17c, (const br_vector3*)local_f0, (const br_matrix34*)((char*)iVar10 + 0x8c));
              FUN_004d2320((tPhysics_object*)iVar10,(tPhysics_object*)iVar15,(void*)*(undefined4*)(iVar12 + 0xc),
                           (undefined4)local_100[local_190],(br_vector3*)local_f0);
            }
          }
          if (gFloat_00589844 < local_19c) {
            if ((*(int *)(iVar12 + 4) != 0) && (*(char *)(*(int *)(iVar12 + 4) + 0x238) == '\x10'))
            {
              FUN_0042d1e0((tPhysics_object*)*(int*)(iVar12 + 4),local_194,(br_vector3*)&local_bc[3],(br_vector3*)&local_150);
              BrMatrix34ApplyV((br_vector3*)&local_150, (const br_vector3*)&local_18c, (const br_matrix34*)((char*)iVar10 + 0x8c));
              BrMatrix34ApplyP((br_vector3*)&local_17c, (const br_vector3*)local_f0, (const br_matrix34*)((char*)iVar10 + 0x8c));
              local_18c = local_18c * (float)gDouble_00589628;
              local_188 = local_188 * (float)gDouble_00589628;
              local_184 = local_184 * (float)gDouble_00589628;
              pfVar18 = (float *)0x1;
              CrashNoise((br_vector3*)&local_18c,(br_vector3*)local_f0,0);
            }
            if ((*(int *)(iVar12 + 8) != 0) && (*(char *)(*(int *)(iVar12 + 8) + 0x238) == '\x10'))
            {
              FUN_0042d1e0((tPhysics_object*)*(int*)(iVar12 + 8),local_194,(br_vector3*)&local_80,(br_vector3*)&local_10c);
              BrMatrix34ApplyV((br_vector3*)&local_150, (const br_vector3*)&local_18c, (const br_matrix34*)((char*)iVar15 + 0x8c));
              BrMatrix34ApplyP((br_vector3*)&local_17c, (const br_vector3*)local_f0, (const br_matrix34*)((char*)iVar15 + 0x8c));
              local_18c = local_18c * (float)gDouble_00589628;
              local_188 = local_188 * (float)gDouble_00589628;
              local_184 = local_184 * (float)gDouble_00589628;
              pfVar18 = (float *)0x1;
              CrashNoise((br_vector3*)&local_18c,(br_vector3*)local_f0,0);
            }
          }
          if ((gFloat_00589848 < local_19c) &&
             (((*(int *)(iVar12 + 4) != 0 && (*(char *)(*(int *)(iVar12 + 4) + 0x238) == '@')) ||
              ((*(int *)(iVar12 + 8) != 0 && (*(char *)(*(int *)(iVar12 + 8) + 0x238) == '@')))))) {
            CollideShitMine((tPhysics_object*)*(int*)(iVar12 + 4),(tPhysics_object*)*(int*)(iVar12 + 8),local_19c);
          }
          if (((gPinball_factor != gFlyCarZero) && (*(int *)(iVar12 + 8) == 0)) &&
             (gFloat_0058984c < local_19c)) {
            if (pfVar18 == (float *)0x0) {
              BrMatrix34ApplyV((br_vector3*)&local_18c, (const br_vector3*)&local_150, (const br_matrix34*)((char*)iVar10 + 0x8c));
              local_18c = local_18c * (float)gDouble_00589628;
              local_188 = local_188 * (float)gDouble_00589628;
              local_184 = local_184 * (float)gDouble_00589628;
            }
            iVar10 = *(int *)(iVar12 + 4);
            fVar6 = ((local_18c * gPinball_factor) / *(float *)(iVar10 + 4)) * gFloat_0058f6a8;
            fVar3 = ((local_188 * gPinball_factor) / *(float *)(iVar10 + 4)) * gFloat_0058f6a8;
            fVar4 = ((local_184 * gPinball_factor) / *(float *)(iVar10 + 4)) * gFloat_0058f6a8;
            fVar5 = fVar4 * fVar4 + fVar3 * fVar3 + fVar6 * fVar6;
            if (gFloat_005895b0 < fVar5) {
              fVar5 = gFloat_005895b0 / fVar5;
              fVar6 = fVar6 * fVar5;
              fVar3 = fVar3 * fVar5;
              fVar4 = fVar4 * fVar5;
            }
            *(float *)(iVar10 + 0x68) = fVar6 + *(float *)(iVar10 + 0x68);
            *(float *)(*(int *)(iVar12 + 4) + 0x6c) =
                 fVar3 + *(float *)(*(int *)(iVar12 + 4) + 0x6c);
            fVar5 = gOne;
            *(float *)(*(int *)(iVar12 + 4) + 0x70) =
                 fVar4 + *(float *)(*(int *)(iVar12 + 4) + 0x70);
            if (fVar5 < fVar4 * fVar4 + fVar3 * fVar3 + fVar6 * fVar6) {
              piVar19 = *(int **)(iVar12 + 4);
              if (((piVar19 == (int *)0x0) || (piVar19[0x8f] == 0)) ||
                 (((char)piVar19[0x8e] != '\x01' || (*(int *)(piVar19[0x8f] + 0xc) != 8)))) {
                DRS3StartSound3D(gCar_outlet,0x2333,*piVar19 + 0x50,&gZero_v__car,1,0xff,0x10000,
                                 0x10000);
              }
              else {
                DRS3StartSound(gCar_outlet,0x2333);
              }
            }
          }
        }
        local_190 = local_190 + 1;
      } while (local_190 < (int)param_3);
    }
  }
  return 0;
#undef local_19c
#undef local_18c
#undef local_188
#undef local_184
#undef local_180
#undef local_17c
#undef local_178
#undef local_174
#undef local_164
#undef local_160
#undef local_15c
#undef local_150
#undef local_14c
#undef local_148
#undef local_144
#undef local_140
#undef local_13c
#undef local_138
#undef local_134
#undef local_130
#undef local_12c
#undef local_128
#undef local_124
#undef local_120
#undef local_118
#undef local_114
#undef local_110
#undef local_10c
#undef local_108
#undef local_104
#undef local_fc
#undef local_f8
#undef local_f4
#undef local_f0
#undef local_e0
#undef local_dc
#undef local_d8
#undef local_d4
#undef local_d0
#undef local_cc
#undef local_c8
#undef local_c4
#undef local_c0
#undef local_bc
#undef local_ac
#undef local_a8
#undef local_a4
#undef local_a0
#undef local_9c
#undef local_80
#undef local_7c
#undef local_78
#undef local_74
#undef local_70
#undef local_6c
#undef local_50
}

// FUNCTION: CARMA2_HW 0x0041deb0
void C2_HOOK_FASTCALL FUN_0041deb0(tCar_spec* pCar, const br_vector3* pPosition, const br_vector3* pImpulse, tCar_spec* pOtherCar, undefined4 pArg5, const br_vector3* pPoints, undefined4 pArg7) {
    float fVar1;
    float fVar2;
    int iVar3;
    register int five = 5;
    register int seven;
    struct {
        float impulse[3];
        float force[3];
        float output[3];
        br_matrix34 matrix;
    } local;

    (void)pArg5;
    (void)pArg7;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, field_0xc8, 0xc8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, damage_magnitude_accumulator, 0x133c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, invulnerable_no_damage, 0x1b8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, knackered, 0x1d4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, power_up_levels, 0x18a8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, softness_factor, 0x18d0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, time_last_hit, 0x13e4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, field_0x4c8, 0x4c8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, damage_multiplier, 0x4cc);

    if (pOtherCar != NULL) {
        if (pOtherCar->field_0xc8 == NULL || pOtherCar->driver > five) {
            pOtherCar->field_0xc8 = (undefined*)pCar;
        }
        if (pCar->field_0xc8 != NULL) {
            if (pCar == NULL) {
                return;
            }
            if (pCar->driver > five) {
                goto after_link;
            }
        }
        pCar->field_0xc8 = (undefined*)pOtherCar;
    }
after_link:
    if (pCar == NULL || pCar->driver <= five) {
        return;
    }

    if (gNet_mode == eNet_mode_none) {
        fVar1 = gCar_softness.initial[gProgram_state.skill_level];
    } else {
        fVar1 = gCar_softness.initial_network[gCurrent_net_game->type];
    }
    if (pCar->driver == 6) {
        local.force[0] = pCar->pos.v[0] - gCamera_to_world.m[3][0];
        local.force[1] = pCar->pos.v[1] - gCamera_to_world.m[3][1];
        fVar2 = pCar->pos.v[2] - gCamera_to_world.m[3][2];
        if (fVar2 * fVar2 + local.force[1] * local.force[1] + local.force[0] * local.force[0] > gFloat_005896d0) {
            return;
        }
    }
    seven = 7;
    if (pOtherCar == NULL) {
        if (pCar->driver == 6) {
            fVar1 = pCar->softness_factor * fVar1;
            if (5000 < PDGetTotalTime() - pCar->time_last_hit) {
                fVar1 *= gFloat_00589868;
            }
        }
    } else {
        if (pOtherCar->driver > five) {
            MarkPlayerInteraction(pCar, pOtherCar);
        }
        if (pCar->driver >= seven) {
            fVar1 = (float)(gArmour_starting_value[pCar->power_up_levels[0]] * gDouble_00589858 * fVar1);
        }
        iVar3 = pOtherCar->driver;
        if (iVar3 >= seven) {
            if (gNet_mode == eNet_mode_none) {
                fVar2 = gCar_car_damage_multiplier.initial[gProgram_state.skill_level];
            } else {
                fVar2 = gCar_car_damage_multiplier.initial_network[gCurrent_net_game->type];
            }
            fVar1 = (float)(fVar1 * fVar2 * gOffensive_starting_value[pOtherCar->power_up_levels[2]] * pOtherCar->damage_multiplier * gDouble_00589860);
        }
        if (pCar->driver == 6 && iVar3 == 6) {
            fVar1 *= gFloat_00589868;
        }
        if (iVar3 < 6) {
            pOtherCar = NULL;
        } else {
            fVar1 = (float)(fVar1 / ((pOtherCar->softness_factor - gDouble_00589870) * gDouble_00589878));
        }
    }

    fVar1 *= gFloat_00589868;
    local.impulse[0] = pImpulse->v[0] * fVar1;
    local.impulse[1] = pImpulse->v[1] * fVar1;
    local.impulse[2] = pImpulse->v[2] * fVar1;
    fVar1 = local.impulse[2] * local.impulse[2] + local.impulse[1] * local.impulse[1] + local.impulse[0] * local.impulse[0];
    iVar3 = pCar->driver;
    if (iVar3 <= five || pCar->invulnerable_no_damage == 0) {
        pCar->damage_magnitude_accumulator += fVar1;
    }
    five = 8;
    if (iVar3 < seven) {
        local.force[0] = local.impulse[0] * gFloat_00589880;
        local.force[1] = local.impulse[1] * gFloat_00589880;
        local.force[2] = local.impulse[2] * gFloat_00589880;
    } else {
        if (pCar->field_0x4c8 != gZero) {
            local.impulse[0] /= pCar->field_0x4c8;
            local.impulse[1] /= pCar->field_0x4c8;
            local.impulse[2] /= pCar->field_0x4c8;
        }
        local.force[0] = local.impulse[0] * gFloat_00589650;
        local.force[1] = local.impulse[1] * gFloat_00589650;
        local.force[2] = local.impulse[2] * gFloat_00589650;
        if (iVar3 == five) {
            FUN_0043faa0((br_vector3*)&local.impulse[0]);
        }
    }
    if (gNet_mode == 2 && gCurrent_net_game->type == 7 && pOtherCar != NULL
        && pCar->driver >= seven && pOtherCar->driver >= seven) {
        if (pCar == gNet_players[gIt_or_fox].car) {
            if (pOtherCar->knackered == 0) {
                FUN_0049a990(NetPlayerFromCar(pOtherCar));
            }
        } else if (pOtherCar == gNet_players[gIt_or_fox].car && pCar->knackered == 0) {
            FUN_0049a990(NetPlayerFromCar(pCar));
        }
    }
    if (gNet_mode != 3 || pOtherCar == NULL) {
        DamageCarInDirection(pCar, (float*)pPosition, &local.impulse[0], pOtherCar != NULL);
    }
    FUN_0042d400(pCar, *(int*)&fVar1, pPoints, (br_vector3*)&local.force[0], gPHIL_last_physics_tick);
    if (pOtherCar != NULL && pOtherCar->driver == five && fVar1 > gDouble_00589888) {
        FUN_004c8540(pCar, (br_vector3*)pPosition, *(int*)&fVar1);
    }
    if (pOtherCar == NULL && pCar->driver == five && fVar1 > gDouble_00589888) {
        BrMatrix34Copy(&local.matrix, &pCar->car_master_actor->t.t.mat);
        local.matrix.m[3][0] *= gFloat_005896d4;
        local.matrix.m[3][1] *= gFloat_005896d4;
        local.matrix.m[3][2] *= gFloat_005896d4;
        BrMatrix34ApplyP((br_vector3*)&local.output[0], pPosition, &local.matrix);
        PipeNoOp(fVar1);
    }
}

// FUNCTION: CARMA2_HW 0x0041e310
int C2_HOOK_FASTCALL ProcessJointForcesCallback(undefined4 param_1, undefined4 param_2, undefined4 param_3) {

    return 0;
}

// FUNCTION: CARMA2_HW 0x0041e320
int C2_HOOK_FASTCALL FUN_0041e320(void) {

    return 1;
}

static br_scalar Dot3(const br_vector3* a, const br_vector3* b) {
    return a->v[0] * b->v[0] + a->v[1] * b->v[1] + a->v[2] * b->v[2];
}

/* declared locally so the shared headers stay unmodified (codegen stability) */
void C2_HOOK_FASTCALL FreezeCamera(void);
void C2_HOOK_FASTCALL AddSplashToPipingSession(tPhysics_object* pCollision);
void C2_HOOK_FASTCALL AddExtendedSplashToPipingSession(tPhysics_object* pCollision, void* pArg2);
void C2_HOOK_FASTCALL PedPreCollisionStuff(void);
#define ePipe_chunk_splash 20
#define ePipe_chunk_extended_splash 59

// FUNCTION: CARMA2_HW 0x00414910
void C2_HOOK_FASTCALL NewFacesListCallback(tPhysics_object* pCollision, undefined4* arg2) {
    tFace_ref* pFace;
    br_bounds3 current_bounds;
    br_scalar old_d;
    float prop7;

    pFace = (tFace_ref*)arg2;
    old_d = pCollision->water_d;
    if (pCollision->owner == NULL) {
        return;
    }
    prop7 = PHILGetObjectProperty(pCollision, 7);
    if (pCollision->flags_0x238 != 1 && prop7 == 0.f) {
        return;
    }
    if (pCollision != NULL
        && pCollision->owner != NULL
        && pCollision->flags_0x238 == 1
        && ((tCar_spec*)pCollision->owner)->driver == eDriver_local_human
        && pCollision->water_d != 10000.f
        && gDouble_pling_water
        && BrVector3Dot(&pCollision->water_normal, &pCollision->field_0xf4.max) - pCollision->water_d <= 0.f) {
        gInTheSea = 1;
        FreezeCamera();
    }
    if (pFace != NULL && fabsf(pFace->normal.v[1]) > 0.9f) {
        BrVector3Copy(&pCollision->water_normal, &pFace->normal);
        if (pCollision->water_normal.v[1] < 0.f) {
            BrVector3Negate(&pCollision->water_normal, &pCollision->water_normal);
        }
        pCollision->water_d = (((pFace->v[0].v[2]) * (pCollision->water_normal.v[2])) + ((pFace->v[0].v[1]) * (pCollision->water_normal.v[1]))) + ((pFace->v[0].v[0]) * (pCollision->water_normal.v[0]));
        if (pCollision != NULL
            && pCollision->owner != NULL
            && pCollision->flags_0x238 == 1
            && ((tCar_spec*)pCollision->owner)->driver == eDriver_local_human) {
            if (pFace->material->identifier[1] == '!') {
                if (BrVector3Dot(&pCollision->field_0xf4.min, &pCollision->water_normal) - pCollision->water_d < 0.f) {
                    GetNewBoundingBox(&current_bounds, &pCollision->bb1, &pCollision->actor->t.t.mat);
                    if ((((pCollision->water_normal.v[0]) * (current_bounds.min.v[0])) + ((pCollision->water_normal.v[1]) * (current_bounds.min.v[1]))) + ((pCollision->water_normal.v[2]) * (current_bounds.min.v[2])) - pCollision->water_d < 0.f) {
                        gInTheSea = 1;
                        FreezeCamera();
                    }
                }
                gDouble_pling_water = 1;
            } else {
                gDouble_pling_water = 0;
            }
        }
    } else {
        pCollision->water_d = 10000.f;
        if (pCollision != NULL
            && pCollision->owner != NULL
            && pCollision->flags_0x238 == 1
            && ((tCar_spec*)pCollision->owner)->driver == eDriver_local_human) {
            gInTheSea = gInTheSea == 1 ? 2 : 0;
        }
    }
    if (fabs(old_d - pCollision->water_d) > 1e-05) {
        if (PHILGetObjectProperty(pCollision, 6) != 0.f) {
            ARStartPipingSession(ePipe_chunk_extended_splash);
            AddExtendedSplashToPipingSession(pCollision, pCollision);
            AREndPipingSession();
        }
        if (pCollision != NULL && pCollision->owner != NULL && pCollision->flags_0x238 == 1) {
            tCar_spec* owner = (tCar_spec*)pCollision->owner;
            if (owner != NULL && owner->driver > 5) {
                ARStartPipingSession(ePipe_chunk_splash);
                AddSplashToPipingSession(pCollision);
                AREndPipingSession();
                return;
            }
            ARStartPipingSession(ePipe_chunk_extended_splash);
            AddExtendedSplashToPipingSession(pCollision, pCollision->actor);
            AREndPipingSession();
            return;
        }
        if (pCollision != NULL && pCollision->owner != NULL && pCollision->flags_0x238 >= 2 && pCollision->flags_0x238 <= 4) {
            ARStartPipingSession(ePipe_chunk_extended_splash);
            AddExtendedSplashToPipingSession(pCollision, pCollision->owner);
            AREndPipingSession();
        }
    }
}

// C2PORT-NONMATCHING: best 65.39% | difftest PASS 2000 trials (spec difftest/specs/0041ff20_DoPullActorFromWorld.json) | reason VC5-register-stack-layout
// FUNCTION: CARMA2_HW 0x0041ff20
tNon_car_spec* C2_HOOK_FASTCALL DoPullActorFromWorld(register br_actor* actor) {
    register tNon_car_spec* non_car;
    register int index;
    register int zero;
    char spec_index;

    if (actor->type_data != NULL) {
        return NULL;
    }
    zero = 0;
    index = (char)actor->identifier[2] - 0x210 + (char)actor->identifier[1] * 10;
    non_car = (tNon_car_spec*)zero;
    spec_index = (char)gNon_car_spec_indices[index];
    if (spec_index != zero) {
        non_car = &gProgram_state.non_cars[spec_index + 0x27];
    }
    if (non_car == (tNon_car_spec*)zero) {
        goto fail;
    }
    if (non_car->driver == eDriver_4) {
        tNon_car_spec* free_non_car;
        tPhysics_object* free_collision_info;
        int i;

        free_non_car = gProgram_state.non_cars;
        for (i = 0; i < 0x28; i++) {
            if (free_non_car->driver == eDriver_non_car_unused_slot) {
                break;
            }
            free_non_car++;
        }
        if (i == 0x28) {
            non_car = (tNon_car_spec*)zero;
        } else {
            free_collision_info = free_non_car->collision_info;
            memcpy(free_collision_info, gProgram_state.non_cars[(char)gNon_car_spec_indices[index] + 0x27].collision_info, sizeof(*free_collision_info));
            memcpy(free_non_car, &gProgram_state.non_cars[(char)gNon_car_spec_indices[index] + 0x27], sizeof(*free_non_car));
            free_non_car->collision_info = free_collision_info;
            non_car = free_non_car;
        }
    }
    if (non_car == (tNon_car_spec*)zero) {
fail:
        actor->identifier[1] = (char)0x78;
        return NULL;
    }

    actor->type_data = non_car;
    non_car->driver = eDriver_4;
    non_car->collision_info->disable_move_rotate = 1;
    if (actor->parent == gAdditional_actors) {
        non_car->car_ID = -1;
    } else {
        non_car->car_ID = (char)actor->identifier[7] - 0x14d0
            + ((short)actor->identifier[6] + (char)actor->identifier[5] * 10) * 10;
    }
    BrActorRemove(actor);
    BrActorAdd(gNon_track_actor, actor);
    non_car->actor = actor;
    gActive_non_car_list[gNum_active_non_cars] = non_car;
    gNum_active_non_cars++;
    gActive_car_list[gNum_cars_and_non_cars] = (tCar_spec*)non_car;
    gNum_cars_and_non_cars++;

    if (gTesting_car_for_sensible_place != 0) {
        GetNewBoundingBox(&non_car->collision_info->field_0xf4, &non_car->collision_info->bb2, &actor->t.t.mat);
        memcpy(&non_car->collision_info->field_0x10c, &non_car->collision_info->field_0xf4, sizeof(br_bounds3));
    }
    BrMatrix34Copy(&non_car->collision_info->transform_matrix, &actor->t.t.mat);
    non_car->collision_info->box_face_ref = (tU16)gFace_num__car - 2;
    non_car->collision_info->disable_move_rotate = 1;
    non_car->collision_info->actor = non_car->actor;
    *(tU32*)((tU8*)non_car + 4) = zero;
    BrVector3SetFloat(&non_car->collision_info->v, (br_scalar)zero, (br_scalar)zero, (br_scalar)zero);
    BrVector3SetFloat(&non_car->collision_info->omega, (br_scalar)zero, (br_scalar)zero, (br_scalar)zero);
    BrVector3SetFloat(&non_car->collision_info->rotate_omega, (br_scalar)zero, (br_scalar)zero, (br_scalar)zero);
    *(tU32*)((tU8*)non_car + 0x68) = zero;
    *(tU32*)((tU8*)non_car + 0xc8) = zero;

    if (actor->identifier[3] == '!' || non_car->snap_off_cosine >= gOne) {
        actor->identifier[3] = '!';
        non_car->collision_info->field_0xed = 1;
        non_car->collision_info->M = non_car->free_mass;
        *(tU32*)((tU8*)non_car + 0x14) = zero;
        if (non_car->snap_off_cosine < gOne) {
            non_car->collision_info->physics_joint2 = (tPhysics_joint*)zero;
        }
        non_car->collision_info->field_0x194 = zero;
        non_car->collision_info->field_0x198 = zero;
    } else {
        non_car->collision_info->M = non_car->attached_mass;
        *(float*)((tU8*)non_car + 0x14) = non_car->min_torque_squared;
        non_car->collision_info->field_0xed = zero;
        non_car->collision_info->physics_joint2 = non_car->field_0xf0;
        non_car->collision_info->field_0x194 = (tU32)(uintptr_t)FUN_00420360;
        non_car->collision_info->field_0x198 = (tU32)(uintptr_t)FUN_00420310;
        if (non_car->collision_info->physics_joint2 != (tPhysics_joint*)zero) {
            BrMatrix34ApplyP(&non_car->collision_info->field_0x17c,
                (br_vector3*)((tU8*)non_car->collision_info->physics_joint2 + 8), &non_car->collision_info->transform_matrix);
        }
    }
    non_car->collision_info->I.v[0] = non_car->I_over_M.v[0] * non_car->collision_info->M;
    non_car->collision_info->I.v[1] = non_car->I_over_M.v[1] * non_car->collision_info->M;
    non_car->collision_info->I.v[2] = non_car->I_over_M.v[2] * non_car->collision_info->M;
    *(tU32*)((tU8*)non_car + 0xc4) = 0xbf800000;
    non_car->collision_info->water_d = 10000.f;
    non_car->collision_info->field_0x261 = zero;
    non_car->collision_info->message_time = zero;
    non_car->field_0xf8 = 0;
    non_car->collision_info->field_0x49c = 1;
    non_car->flags &= 0xffef0000;
    BrVector3SetFloat(&non_car->collision_info->field_0x54, (br_scalar)zero, -0.0579710156f, (br_scalar)zero);
    ResetCarSpecialVolume(non_car->collision_info);
    BrMatrix34Copy(&non_car->collision_info->transform_matrix, &actor->t.t.mat);
    if (non_car->car_ID != -1) {
        PHILSendDetachBit(non_car->car_ID, actor);
    }
    PHILQueueDetachBit(non_car->collision_info, (void*)zero, (br_vector3*)zero, (br_vector3*)zero);
    return non_car;
}

// FUNCTION: CARMA2_HW 0x0041ff00
tNon_car_spec* C2_HOOK_FASTCALL PullActorFromWorld(br_actor* actor) {

    if (!gCrush_deferred && !gTesting_car_for_sensible_place) {
        return NULL;
    }
    return DoPullActorFromWorld(actor);
}

// FUNCTION: CARMA2_HW 0x004b5970
float C2_HOOK_FASTCALL GetFrictionFromFace(void *arg1) {
    void *ptr = *(void **)arg1;
    char code = *(char *)*(void **)((char *)ptr + 4);
    int idx = code - 0x2f;
    if (idx < 0 || idx >= 11) {
        idx = 0;
    }
    return gFriction_materials[idx].car_wall_friction;
}

// FUNCTION: CARMA2_HW 0x00417de0
void C2_HOOK_FAKE_THISCALL ControlCar1(tCar_spec* c, undefined4 arg2, br_scalar dt) {
    br_scalar mag;

    CARPOCALYPSE2_THISCALL_UNUSED(arg2);

    c->acc_force = 0.0f;

    if (c->keys.acc) {
        c->acc_force = c->collision_info->M * 7.0f;
    }
    if (c->keys.dec) {
        c->acc_force = -(c->collision_info->M * 7.0f);
    }

    if (c->keys.left) {
        if (c->curvature < 0.0f) {
            c->curvature -= dt * -0.25;
        } else {
            mag = BrVector3Length(&c->collision_info->v);
            c->curvature += 25.f * dt * (0.05f / (mag + 5.0f));
        }
    }

    if (c->keys.right) {
        if (c->curvature > 0.0f) {
            c->curvature -= dt * 0.25;
        } else {
            mag = BrVector3Length(&c->collision_info->v);
            c->curvature -= 25.f * dt * (0.05f / (mag + 5.0f));
        }
    }

    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// FUNCTION: CARMA2_HW 0x00417180
void C2_HOOK_FAKE_THISCALL ControlCar2(tCar_spec* c, undefined4 arg2, br_scalar dt) {
    br_scalar mag;

    CARPOCALYPSE2_THISCALL_UNUSED(arg2);

    c->acc_force = 0.0f;

    if (c->keys.acc) {
        c->acc_force = c->collision_info->M * 7.0f;
    }
    if (c->keys.dec) {
        c->acc_force = -(c->collision_info->M * 7.0f);
    }

    if (c->keys.left) {
        if (c->turn_speed < 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature < 0.0f) {
            c->turn_speed -= dt * -0.125;
        } else {
            mag = BrVector3Length(&c->collision_info->v);
            c->turn_speed -= -0.5f * (25.f * dt * (0.05f / (mag + 5.0f)));
        }
    }

    if (c->keys.right) {
        if (c->turn_speed > 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature > 0.0f) {
            c->turn_speed -= dt * 0.125;
        } else {
            mag = BrVector3Length(&c->collision_info->v);
            c->turn_speed -= 0.5f * (25.f * dt * (0.05f / (mag + 5.0f)));
        }
    }

    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.0f;
    }

    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// FUNCTION: CARMA2_HW 0x004173b0
void C2_HOOK_FAKE_THISCALL ControlCar3(tCar_spec* c, undefined4 arg2, br_scalar dt) {
    br_scalar mag;

    CARPOCALYPSE2_THISCALL_UNUSED(arg2);

    if (c->keys.left) {
        if (c->turn_speed < 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature >= 0.0f && c->collision_info->omega.v[1] >= 0.0f) {
            mag = BrVector3Length(&c->collision_info->v);
            c->turn_speed -= 25.f * dt * (0.05f / (mag + 5.0f)) * -0.5f * 0.75;
        } else {
            c->turn_speed -= dt * -0.375;
        }
    }

    if (c->keys.right) {
        if (c->turn_speed > 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature <= 0.0f && c->collision_info->omega.v[1] <= 0.0f) {
            mag = BrVector3Length(&c->collision_info->v);
            c->turn_speed -= 25.f * dt * (0.05f / (mag + 5.0f)) * 0.5f * 0.75;
        } else {
            c->turn_speed -= dt * 0.375;
        }
    }

    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.0f;
    }

    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// C2PORT-NONMATCHING: best 72.95% | difftest PASS 2000 trials (spec difftest/specs/00417a20_ControlCar5.json) | reason VC5 x87 schedule and stack/register layout
// FUNCTION: CARMA2_HW 0x00417a20
void C2_HOOK_FAKE_THISCALL ControlCar5(tCar_spec* c, undefined4 arg2, br_scalar dt) {
    unsigned int input;
    br_scalar mag;
    tCar_spec* volatile saved_car;

    CARPOCALYPSE2_THISCALL_UNUSED(arg2);
    saved_car = c;

    input = *(unsigned int*)&c->keys;
    c->acc_force = 0.0f;

    if (c->keys.acc) {
        c->acc_force = c->collision_info->M * 7.0f;
    }
    if (c->keys.dec) {
        c->acc_force = -(c->collision_info->M * 7.0f);
    }

    if (c->keys.left) {
        if (c->turn_speed < 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature < 0.0f) {
            c->turn_speed += dt * 0.0625;
        } else {
            mag = (float)sqrt(BR_ADD(BR_ADD(BR_SQR(c->collision_info->v.v[0]), BR_SQR(c->collision_info->v.v[1])), BR_SQR(c->collision_info->v.v[2])));
            c->turn_speed += BR_MUL(dt, 25.0f) * (0.05f / (mag + 5.0f)) * 0.25f;
        }
    }

    if (c->keys.right) {
        if (c->turn_speed > 0.0f) {
            c->turn_speed = 0.0f;
        }
        if (c->curvature > 0.0f) {
            c->turn_speed -= dt * 0.0625;
        } else {
            mag = (float)sqrt(BR_ADD(BR_ADD(BR_SQR(c->collision_info->v.v[0]), BR_SQR(c->collision_info->v.v[1])), BR_SQR(c->collision_info->v.v[2])));
            c->turn_speed -= BR_MUL(dt, 25.0f) * (0.05f / (mag + 5.0f)) * 0.25f;
        }
    }

    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.0f;
        if (c->curvature < 0.0f) {
            if (!c->keys.holdw) {
                mag = (float)sqrt(BR_ADD(BR_ADD(BR_SQR(c->collision_info->v.v[0]), BR_SQR(c->collision_info->v.v[1])), BR_SQR(c->collision_info->v.v[2])));
                c->curvature += BR_MUL(dt, 25.0f) * (0.05f / (mag + 5.0f)) * 2.0f;
                if (c->curvature > 0.0f) {
                    c->curvature = 0.0f;
                }
            }
        } else {
            if (!c->keys.holdw) {
                mag = (float)sqrt(BR_ADD(BR_ADD(BR_SQR(c->collision_info->v.v[0]), BR_SQR(c->collision_info->v.v[1])), BR_SQR(c->collision_info->v.v[2])));
                c->curvature -= BR_MUL(dt, 25.0f) * (0.05f / (mag + 5.0f)) * 2.0f;
                if (c->curvature < 0.0f) {
                    c->curvature = 0.0f;
                }
            }
        }
    }

    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }

    input |= 0x10000;
    *(unsigned int*)&c->keys = input;
}

void C2_HOOK_FASTCALL DrVector3RotateY(br_vector3* v, br_angle t) {
    br_scalar c;
    br_scalar s;
    br_scalar ts;

    c = cosf(BrAngleToRadian(t));
    s = sinf(BrAngleToRadian(t));
    ts = v->v[0] * c + v->v[2] * s;
    v->v[2] = v->v[2] * c - v->v[0] * s;
    v->v[0] = ts;
}

// FUNCTION: CARMA2_HW 0x004f8dc0
intptr_t C2_HOOK_CDECL ActorFunks(br_actor* pActor, void* pContext) {
    tFunk_index_cbfn* funk_index_callback;
    tUser_crush_data* user_crush_data;
    tCar_crush_buffer_entry* crush_data;
    int i;
    int funk_index;

    user_crush_data = pActor->user;
    funk_index_callback = pContext;

    if (user_crush_data != NULL) {
        crush_data = user_crush_data->crush_data;
        if (crush_data != NULL) {
            if (crush_data->smashables != NULL) {
                for (i = 0; i < crush_data->count_smashables; i++) {
                    funk_index = crush_data->smashables[i].funk;
                    if (funk_index >= 0) {
                        funk_index_callback(funk_index);
                    }
                }
            }
        }
    }
    return 0;
}

// GLOBAL: CARMA2_HW 0x006a82a8
tCar_spec* gRecalc_mass_car;

void C2_HOOK_FASTCALL MasterDisableFunkotronic(int pFunk_index);

// FUNCTION: CARMA2_HW 0x004f8ff0
int C2_HOOK_FASTCALL SetCarMaterialToSolidGreenCB(br_material* pMaterial) {
    tCar_spec* car = gRecalc_mass_car;
    br_pixelmap* pix = car->field_0x1960;

    if (pMaterial->colour_map != pix) {
        pMaterial->colour_map = pix;
        BrMaterialUpdate(pMaterial, BR_MATU_ALL);
        return 1;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004f9020
void C2_HOOK_FASTCALL RecalculateCarMassMomentOfInertia(tCar_spec* car) {
    int local_c;
    int j;
    tCar_bit_spec* bit;
    tCar_crush_spec* spec;
    br_actor* actor;
    tUser_crush_data* user;
    tCar_crush_buffer_entry* crush;
    br_material* solid_mat;

    spec = car->car_crush_spec;
    DRActorEnumRecurse(car->car_model_actor, ActorFunks, (void*)MasterDisableFunkotronic);
    local_c = 0;
    if (spec->field_0x270 > 0) {
        bit = &spec->field_0x274[0];
        do {
            actor = bit->field_0x0;
            user = (tUser_crush_data*)actor->user;
            if (user != NULL) {
                crush = user->crush_data;
                if (crush != NULL) {
                    if (crush->smashables != NULL) {
                        j = 0;
                        if (crush->count_smashables != 0) {
                            do {
                                if (crush->smashables[j].funk >= 0) {
                                    MasterDisableFunkotronic(crush->smashables[j].funk);
                                }
                                j++;
                            } while (j < crush->count_smashables);
                        }
                    }
                }
            }
            local_c++;
            bit++;
        } while (local_c < spec->field_0x270);
    }
    local_c = 0;
    if (spec->field_0x2b0 > 0) {
        bit = &spec->field_0x2b4[0];
        do {
            actor = bit->field_0x0;
            user = (tUser_crush_data*)actor->user;
            if (user != NULL) {
                crush = user->crush_data;
                if (crush != NULL) {
                    if (crush->smashables != NULL) {
                        j = 0;
                        if (crush->count_smashables != 0) {
                            do {
                                if (crush->smashables[j].funk >= 0) {
                                    MasterDisableFunkotronic(crush->smashables[j].funk);
                                }
                                j++;
                            } while (j < crush->count_smashables);
                        }
                    }
                }
            }
            local_c++;
            bit++;
        } while (local_c < spec->field_0x2b0);
    }
    solid_mat = BrMaterialFind("solidgrn");
    if (solid_mat != NULL && solid_mat->colour_map != NULL) {
        gRecalc_mass_car = car;
        car->field_0x1960 = solid_mat->colour_map;
        ForEveryCarMaterial(car, SetCarMaterialToSolidGreenCB, 1);
    }
}

// C2PORT-NONMATCHING: best 44.00% | difftest PASS 2000 trials (spec difftest/specs/004f9180_CarModelColorCycleRender.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x004f9180
void C2_HOOK_CDECL CarModelColorCycleRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    int i;
    int j;
    int cycle;
    int colour;

    if (gAFE_color_cycle != 0) {
        cycle = (int)GetTotalTime() >> 1;

        if (model != NULL) {
            for (i = 0; i < model->prepared->ngroups; i++) {
                v11group* group = &model->prepared->groups[i];

                for (j = 0; j < group->nvertices; j++) {
                    br_vertex* vertex = &model->vertices[group->vertex_user[j]];

                    if (vertex != NULL) {
                        colour = cycle & 0x3fff;
                        if (colour >= 0x100) {
                            colour = -1 - (char)colour;
                        }
                        vertex->red = (br_uint_8)colour;

                        colour = cycle + 0x100 & 0x3ff;
                        if (colour >= 0x100) {
                            colour = 0x3ff - colour;
                        }
                        vertex->grn = (br_uint_8)colour;

                        colour = cycle - 0x200 & 0x3ff;
                        if (colour >= 0x100) {
                            colour = 0x3ff - colour;
                        }
                        vertex->blu = (br_uint_8)colour;
                    }
                }
            }
        }

        if ((model->flags & 0x80) != 0) {
            BrModelUpdate(model, 0x40);
        }
    }
    BrZsModelRender(actor, model, material, (br_order_table*)render_data, style, BrOnScreenCheck(&model->bounds), 0);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0047b2b0
void C2_HOOK_FASTCALL MasterDisableFunkotronic(int pFunk_index) {

    gFunkotronics_array[pFunk_index].flags |= 0x2;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x0047b2e0
void C2_HOOK_FASTCALL MasterEnableFunkotronic(int pFunk_index) {

    gFunkotronics_array[pFunk_index].flags &= ~0x2;
}

// FUNCTION: CARMA2_HW 0x004f9760
int C2_HOOK_FASTCALL RestorePixelmap(br_material* pMaterial) {

    if (pMaterial->colour_map != (br_pixelmap*)pMaterial->user) {
        pMaterial->colour_map = (br_pixelmap*)pMaterial->user;
        BrMaterialUpdate(pMaterial, BR_MATU_ALL);
        return 1;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0040e700
void C2_HOOK_FASTCALL MungeCarMaterials(tCar_spec* pCar, int pInternal_cam) {
    int i;
    tCarCockpitMaterial* cockpit_material;

    for (i = 0, cockpit_material = &pCar->window_materials[0]; i < pCar->count_window_materials; i++, cockpit_material++) {
        int two_sided_material = pInternal_cam;

        if (pInternal_cam) {
            int j;

            for (j = 0; j < cockpit_material->count_maps; j++) {
                if (cockpit_material->material->colour_map == cockpit_material->maps[j]) {
                    two_sided_material = 0;
                }
            }
        }
        if (two_sided_material) {
            cockpit_material->material->flags &= ~BR_MATF_TWO_SIDED;
        }
        if (!two_sided_material) {
            cockpit_material->material->flags |= BR_MATF_TWO_SIDED;
        }
        BrMaterialUpdate(cockpit_material->material, BR_MATU_RENDERING);
    }
}

// FUNCTION: CARMA2_HW 0x0040f510
void C2_HOOK_FASTCALL CheckDisablePlingMaterials(tCar_spec* pCar) {
    br_matrix34* mat;
    br_scalar height;
    int i;

    height = 0.f;
    if (pCar->collision_info->water_d != 10000.f) {
        mat = &pCar->car_master_actor->t.t.mat;
        for (i = 0; i < 3; i++) {
            if (mat->m[i][1] > 0.f) {
                height += pCar->collision_info->bb2.max.v[i] * mat->m[i][1];
            } else {
                height += pCar->collision_info->bb2.min.v[i] * mat->m[i][1];
            }
        }
        if (mat->m[3][1] / WORLD_SCALE + height < pCar->collision_info->water_d) {
            DisablePlingMaterials();
        }
    } else {
        DisablePlingMaterials();
    }
}

// FUNCTION: CARMA2_HW 0x0040f590
void C2_HOOK_FASTCALL PositionCarMountedCamera(tCar_spec* pCar, tU32 pTime) {
    // GLOBAL: CARMA2_HW 0x00679300
    static float swing_rate;
    // GLOBAL: CARMA2_HW 0x0058f624
    static float swing_frac = 1.f;
    br_vector3 offset;
    float weight;

    if (gCamera_key_flags.field_0x0_bit1 && !gCamera_key_flags.field_0x0_bit0 && swing_rate >= 0.f) {
        swing_rate = -0.0002f;
    } else if (gCamera_key_flags.field_0x0_bit2 && !gCamera_key_flags.field_0x0_bit0 && swing_rate <= 0.f) {
        swing_rate = 0.0002f;
    }

    swing_frac += (double)pTime * swing_rate;
    if (swing_frac > 1.f) {
        swing_rate = 0.f;
        swing_frac = 1.f;
    } else if (swing_frac < 0.f) {
        swing_rate = 0.f;
        swing_frac = 0.f;
    }

    weight = 1.f - swing_frac;
    offset.v[0] = pCar->camera_bumper_position.v[0] * weight + pCar->camera_cockpit_position.v[0] * swing_frac;
    offset.v[1] = pCar->camera_bumper_position.v[1] * weight + pCar->camera_cockpit_position.v[1] * swing_frac;
    offset.v[2] = pCar->camera_bumper_position.v[2] * weight + pCar->camera_cockpit_position.v[2] * swing_frac;

    BrMatrix34Copy(&gCamera->t.t.mat, &pCar->car_master_actor->t.t.mat);
    BrMatrix34PreTranslate(&gCamera->t.t.mat, offset.v[0], offset.v[1], offset.v[2]);

    if (swing_rate < 0.f) {
        swing_rate -= 0.0001;
    }
    if (swing_rate > 0.f) {
        swing_rate += 0.0001;
    }

    CollideCamera2(&pCar->pos, &gCamera->t.t.translate.t, NULL, 0, NULL);
}

// FUNCTION: CARMA2_HW 0x0040f4a0
tCar_spec* C2_HOOK_FASTCALL GetRaceLeader(void) {
    int i;
    int best;
    tCar_spec* leader;

    leader = gNet_players[0].car;
    best = gNet_players[0].last_score_index;
    if (gCurrent_net_game->type == eNet_game_type_foxy) {
        i = gIt_or_fox;
        if (i >= 0 && i < gNumber_of_net_players) {
            return gNet_players[i].car;
        }
    }
    if (gNumber_of_net_players > 1) {
        for (i = 1; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].last_score_index < best) {
                best = gNet_players[i].last_score_index;
                leader = gNet_players[i].car;
            }
        }
    }
    return leader;
}

// FUNCTION: CARMA2_HW 0x00410be0
void C2_HOOK_FASTCALL SaveCameraPosition(int i) {

    if (gSave_camera[i].saved != 1) {
        gSave_camera[i].zoom = gCamera_zoom;
        gSave_camera[i].yaw = gCamera_yaw;
        gSave_camera[i].saved = 1;
    }
}

// FUNCTION: CARMA2_HW 0x00410c20
void C2_HOOK_FASTCALL RestoreCameraPosition(int i) {

    if (gSave_camera[i].saved != 0) {
        gCamera_zoom = gSave_camera[i].zoom;
        gCamera_yaw = gSave_camera[i].yaw;
        gSave_camera[i].saved = 0;
    }
}

void C2_HOOK_FASTCALL DoCameraControls(tCamera_key_flags *pCamera_controls, tU32 pTime_difference) {
    int flag;
    int swirl_mode;
    int up_and_down_mode;
    int going_down;
    // GLOBAL: CARMA2_HW 0x0067930c
    static int last_swirl_mode;

    flag = 0;
    swirl_mode = !GetRuntimeVariable(99) && gRace_finished && !gAction_replay_mode && (gCar_to_view == &gProgram_state.current_car || gCar_to_view->knackered);
    up_and_down_mode = swirl_mode && !gCamera_has_collided;
    going_down = gCamera_zoom > 1.0;
    if (last_swirl_mode != swirl_mode) {
        if (swirl_mode) {
            SaveCameraPosition(0);
        } else {
            RestoreCameraPosition(0);
        }
        last_swirl_mode = swirl_mode;
    }
    if (gMap_view != 2 && !gProgram_state.cockpit_on && (!gAction_replay_mode || gAction_replay_camera_mode < kActionReplayCameraMode_Panning)) {
        if (pCamera_controls->field_0x0_bit2 || (up_and_down_mode && !going_down)) {
            gCamera_zoom += (float)pTime_difference * 5.f / 10000.f / (float)(2 * swirl_mode + 1);
            if (gCamera_zoom > 2.f) {
                gCamera_zoom = 2.f;
            }
            if (up_and_down_mode && gCamera_zoom > 1.f) {
                gCamera_zoom = 1.f;
            }
        }
        if (pCamera_controls->field_0x0_bit1 || (up_and_down_mode && going_down)) {
            float zoom;

            gCamera_zoom -= (float)pTime_difference * 5.f / 10000.f / (float)(2 * swirl_mode + 1);
            if (gAction_replay_camera_mode == kActionReplayCameraMode_Peds) {
                zoom = 0.001f;
            } else {
                zoom = 0.1f;
            }
            if (gCamera_zoom < zoom) {
                gCamera_zoom = zoom;
            }
            if (up_and_down_mode && gCamera_zoom < 1.0f) {
                gCamera_zoom = 1.0f;
            }
        }
        if (swirl_mode && gProgram_state.current_car.speedo_speed < 0.001449275362318841) {
            pCamera_controls->field_0x0_bit4 = 0;
            pCamera_controls->field_0x0_bit3 = 1;
        }

        if (gCamera_sign ? pCamera_controls->field_0x0_bit3 : pCamera_controls->field_0x0_bit4) {
            if (!gCamera_reset) {
                gCamera_yaw += BrDegreeToAngle((float)pTime_difference * 0.05f);
            }
            flag = 1;
        }
        if (gCamera_sign ? pCamera_controls->field_0x0_bit4 : pCamera_controls->field_0x0_bit3) {
            if (!gCamera_reset) {
                gCamera_yaw -= BrDegreeToAngle((float)pTime_difference * 0.05f);
            }
            if (flag) {
                gCamera_yaw = 0;
                gCamera_reset = 1;
            }
        } else {
            if (!flag) {
                gCamera_reset = 0;
            }
        }
    }
}

void C2_HOOK_FASTCALL MoveWithWheels(tCar_spec* pCar, br_vector3* pDir, int pManual_swing) {
    br_angle yaw;
    br_angle theta;
    // GLOBAL: CARMA2_HW 0x0058f638
    static int move_with_wheels = 1;


    if (pCar != NULL && pCar->speed <= 0.0001f && !gCamera_mode) {
        if (pManual_swing) {
            if (gCamera_yaw > BrDegreeToAngle(180)) {
                yaw = gCamera_yaw - BrDegreeToAngle(180);
            } else {
                yaw = gCamera_yaw;
            }
            if (yaw > BrDegreeToAngle(45) && yaw < BrDegreeToAngle(135)) {
                if (move_with_wheels) {
                    theta = BrRadianToAngle(atan2f(pCar->wpos[0].v[2] * pCar->curvature, 1.f));
                    gCamera_yaw += (-2 * gCamera_sign + 1) * theta;
                    move_with_wheels = 0;
                }
            } else {
                if (!move_with_wheels) {
                    theta = BrRadianToAngle(atan2f(pCar->wpos[0].v[2] * pCar->curvature, 1.f));
                    gCamera_yaw -= (-2 * gCamera_sign + 1) * theta;
                    move_with_wheels = 1;
                }
            }
        }
        if (move_with_wheels) {
            if (!gCar_flying && gAction_replay_camera_mode != kActionReplayCameraMode_Rigid) {
                theta = BrRadianToAngle(atan2f(pCar->wpos[0].v[2] * pCar->curvature, 1.f));
                DrVector3RotateY(pDir, theta);
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x00410c60
void C2_HOOK_FASTCALL GeneralisedPositionExternalCamera(tCar_spec* pCar, br_matrix34* pMat, br_vector3* pPos, float pSpeed, float pSpeedo_speed, br_vector3* pDirection, br_vector3* pOmega, tU32 pTime_difference) {
    br_vector3 old_camera_pos;
    br_matrix34* m1;
    int swoop;

    m1 = &gCamera->t.t.mat;
    swoop = gCountdown && gCamera_height > pPos->v[1] + 0.001f;
    BrVector3Copy(&old_camera_pos, &gCamera->t.t.translate.t);
    if (!gProgram_state.cockpit_on) {
        int manual_swing;
        float d;
        float l;
        float height_inc;
        float time;

        DoCameraControls(&gCamera_key_flags, pTime_difference);
        manual_swing = gCamera_key_flags.field_0x0_bit4 || gCamera_key_flags.field_0x0_bit3 || swoop;
        if (swoop) {
            gCamera_yaw = 0;
        }
        if (fabsf(pSpeedo_speed) > 0.0006f && gCamera_mode > 0) {
            gCamera_mode = -1;
            if (gAction_replay_camera_mode == kActionReplayCameraMode_Standard) {
                gCamera_sign = 0;
            } else if (BrVector3Dot((br_vector3*)pMat->m[2], pDirection) <= 0.f) {
                gCamera_sign = 0;
            } else {
                gCamera_sign = 1;
            }
        }
        if (pCar != NULL && pCar->frame_collision_flag && gCamera_mode != -2
                && (!gAction_replay_mode || ARReplayForwards())) {
            gCamera_mode = 1;
        }
        if (gCar_flying || gCamera_reset
                || gCamera_mode == -2 || gAction_replay_camera_mode == kActionReplayCameraMode_Rigid) {
            gCamera_mode = 0;
        }
        d = sqrtf(gCamera_zoom) + 4.f / WORLD_SCALE;
        if (!gCamera_mode || gCamera_mode == -1) {
            br_vector3 vn;
            br_vector3 a;

            if (gAction_replay_camera_mode == kActionReplayCameraMode_Rigid) {
                BrVector3Negate(&vn, (br_vector3 *) pMat->m[2]);
            } else {
                BrVector3Copy(&vn, pDirection);
            }
            MoveWithWheels(pCar, &vn, manual_swing);
            vn.v[1] = 0.0f;
            BrVector3Normalise(&vn, &vn);
            vn.v[1] = 0.0f;
            if (gCar_flying || gAction_replay_camera_mode == kActionReplayCameraMode_Rigid) {
                gCamera_sign = 0;
            }
            if (!gAction_replay_mode || !ARReplayIsReallyPaused()) {
                SwingCamera(pMat, m1, &vn, pOmega, pSpeed, pSpeedo_speed, pTime_difference, pCar);
            } else {
                SwingCamera(pMat, m1, &vn, pOmega, pSpeed, pSpeedo_speed, 0, pCar);
            }
            BrVector3Scale(&a, &vn, d);
            BrVector3Sub(&gCamera->t.t.translate.t, pPos, &a);
            BrVector3Copy(&gView_direction, &vn);
        } else {
            gUNK_006792f4 = 0;
            gUNK_006792f8 = 0;
        }
        if (gCamera_mode == 1) {
            br_vector3 vn;
            br_vector3 a;
            br_scalar dist;
            br_scalar l;

            BrVector3Sub(&a, pPos, &old_camera_pos);
            BrVector3Copy(&old_camera_pos, &gCamera_pos_before_collide);
            a.v[1] = 0.0f;
            if (manual_swing) {
                DrVector3RotateY(&a, (gCamera_sign == 0 ? 1 : -1) * (gCamera_yaw - gOld_yaw__car));
                gCamera_yaw = gOld_yaw__car;
            }
            BrVector3Normalise(&vn, &a);
            if (gAction_replay_camera_mode != kActionReplayCameraMode_Standard || manual_swing
                    || BrVector3Dot((br_vector3*)pMat->m[2], &vn) < BrVector3Dot((br_vector3*)pMat->m[2], &gView_direction)) {
                BrVector3Copy(&gView_direction, &vn);
            }
            BrVector3Scale(&vn, &vn, -d);
            BrVector3Accumulate(&a, &vn);
            dist = BrVector3Length(&a);
            l = (float)pTime_difference / 1000.0f * (dist + 1.0f) / dist;
            if (gAction_replay_camera_mode != kActionReplayCameraMode_Standard
                    && l < 1.0f && BrVector3Dot(&a, &vn) > 0.0f) {
                BrVector3Scale(&a, &a, l - 1.f);
                BrVector3Accumulate(&vn, &a);
            }
            BrVector3Add(&gCamera->t.t.translate.t, pPos, &vn);
        }

        height_inc = gCamera_zoom * gCamera_zoom + 0.3f;
        time = pTime_difference / 1000.f;
        if (!gCamera_frozen && (!gAction_replay_mode || !ARReplayIsReallyPaused())) {
            if (pTime_difference >= 5000) {
                gCamera_height = pPos->v[1];
            } else if (swoop) {
                if (time > 0.2f) {
                    time = 0.2f;
                }
                gCamera_height -= 5.0f * time;
                if (gCamera_height < pPos->v[1]) {
                    gCamera_height = pPos->v[1];
                }
            } else {
                gCamera_height += 5.0f * time * pPos->v[1];
                gCamera_height /= 5.0f * time + 1.0f;
            }
        }
        l = pDirection->v[1] * d;
        if (l > 0) {
            br_scalar new_height = pPos->v[1] - l - height_inc / 2.0f;
            if (new_height > gCamera_height) {
                gCamera_height = new_height;
            }
        }

        gCamera->t.t.translate.t.v[1] = height_inc + gCamera_height;
        BrVector3Copy(&gCamera_pos_before_collide, &gCamera->t.t.translate.t);
        CollideCameraWithOtherCars(pPos, &gCamera->t.t.translate.t);
        CollideCamera2(pPos, &gCamera->t.t.translate.t, &old_camera_pos,
            manual_swing || gCamera_key_flags.field_0x0_bit1 || gCamera_key_flags.field_0x0_bit2, pCar != NULL ? pCar->collision_info : NULL);
        if (gCamera_has_collided && swoop) {
            gCamera_height = pPos->v[1];
        }
        PointCameraAtCar(pPos, m1, 1.f);
    }
    gOld_yaw__car = gCamera_yaw;
    gOld_zoom = (int)gCamera_zoom;
}

// C2PORT-NONMATCHING: best 64.21% | difftest PASS 2000 trials (spec difftest/specs/0040ef90_FrozenCamera.json) | reason x87-order, stack-layout
// FUNCTION: CARMA2_HW 0x0040ef90
void C2_HOOK_FASTCALL FrozenCamera(tCar_spec* pCar, tU32 pTime) {
    float time;
    float target_height;
    br_vector3* target_position;
    br_vector3 old_camera_position;

    if (gCamera_key_flags.field_0x0_bit3 && gCamera_key_flags.field_0x0_bit4) {
        InitialiseExternalCamera();
        GeneralisedPositionExternalCamera(pCar, &pCar->car_master_actor->t.t.mat, &pCar->pos,
            pCar->speed, pCar->speedo_speed, &pCar->direction, &pCar->collision_info->omega, pTime);
        gAction_replay_manual_camera_target_type = 0;
    } else if (gCamera_key_flags.field_0x0_bit3) {
        gManual_camera_yaw -= gManual_camera_turn_step;
    } else if (gCamera_key_flags.field_0x0_bit4) {
        gManual_camera_yaw -= gManual_camera_reverse_step;
    } else {
        gManual_camera_yaw = 0.0f;
    }

    if (!gCamera_key_flags.field_0x0_bit0) {
        gManual_camera_vertical = 0.0f;
        if (!gCamera_key_flags.field_0x0_bit1 && !gCamera_key_flags.field_0x0_bit2) {
            gManual_camera_dolly = 0.0f;
        } else {
            if (gCamera_key_flags.field_0x0_bit1) {
                gManual_camera_dolly -= gManual_camera_reverse_step;
            }
            if (gCamera_key_flags.field_0x0_bit2) {
                gManual_camera_dolly -= gManual_camera_turn_step;
            }
        }
    } else {
        gManual_camera_dolly = 0.0f;
        if (!gCamera_key_flags.field_0x0_bit1 && !gCamera_key_flags.field_0x0_bit2) {
            gManual_camera_vertical = 0.0f;
        } else {
            if (gCamera_key_flags.field_0x0_bit1) {
                gManual_camera_vertical -= gManual_camera_reverse_step;
                if (!(gCamera->t.t.translate.t.v[1] <= pCar->collision_info->transform_matrix.m[3][1]
                        - gManual_camera_min_height)) {
                    gManual_camera_vertical = 0.0f;
                }
            }
            if (gCamera_key_flags.field_0x0_bit2) {
                gManual_camera_vertical -= gManual_camera_turn_step;
            }
        }
    }

    old_camera_position.v[0] = gCamera->t.t.translate.t.v[0];
    time = (float)pTime;
    old_camera_position.v[1] = gCamera->t.t.translate.t.v[1];
    target_height = 0.2f;
    old_camera_position.v[2] = gCamera->t.t.translate.t.v[2];
    gCamera->t.t.translate.t.v[1] += gManual_camera_vertical * time;
    target_position = (br_vector3*)pTime;
    switch (gAction_replay_manual_camera_target_type) {
    case 0:
        target_height = pCar->collision_info->bb1.max.v[2] - pCar->collision_info->bb1.min.v[2];
        target_position = &pCar->pos;
        break;
    case 1:
        target_position = &gAction_replay_manual_camera_target_position;
        break;
    case 2:
        BrMatrix34ApplyP(&gAction_replay_manual_camera_target_position,
            &gAction_replay_manual_camera_target_car->collision_info->cmpos,
            &gAction_replay_manual_camera_target_car->car_master_actor->t.t.mat);
        target_position = &gAction_replay_manual_camera_target_position;
        break;
    case 3:
        target_position = &gAction_replay_manual_camera_target_position;
        if ((gAction_replay_manual_camera_target_pedestrian->flags & 1) != 0) {
            tPed_personality* personality;
            br_vector3 target_center;

            personality = gAction_replay_manual_camera_target_pedestrian->character->personality;
            target_center.v[0] = (float)((personality->bb.max.v[0] + personality->bb.min.v[0]) * gHalf_00589428);
            target_center.v[1] = (float)((personality->bb.max.v[1] + personality->bb.min.v[1]) * gHalf_00589428);
            target_center.v[2] = (float)((personality->bb.max.v[2] + personality->bb.min.v[2]) * gHalf_00589428);
            BrMatrix34ApplyP(&gAction_replay_manual_camera_target_position, &target_center,
                &gAction_replay_manual_camera_target_character_actor->t.t.mat);
        }
        break;
    }

    {
        float target_distance;
        float move_distance;
        float camera_forward_x;
        float camera_forward_z;
        float forward_length;
        float camera_forward_scale;
        float camera_forward_depth;

        camera_forward_x = gCamera->t.t.translate.t.v[0] - target_position->v[0];
        camera_forward_z = gCamera->t.t.translate.t.v[2] - target_position->v[2];
        target_distance = (float)sqrt(camera_forward_z * camera_forward_z + camera_forward_x * camera_forward_x);
        if (gManual_camera_dolly != 0.0f) {
            camera_forward_x = gCamera->t.t.mat.m[2][0];
            camera_forward_z = gCamera->t.t.mat.m[2][2];
            forward_length = (float)sqrt(camera_forward_z * camera_forward_z + camera_forward_x * camera_forward_x);
            camera_forward_depth = 0.0f;
            camera_forward_scale = 1.0f;
            if (gFLOAT_00589430 < forward_length) {
                forward_length = (float)(gOne_00589438 / forward_length);
                camera_forward_depth = forward_length * camera_forward_z;
                camera_forward_scale = forward_length * camera_forward_x;
            }
            move_distance = gManual_camera_dolly * time;
            if (target_distance - move_distance < target_height) {
                move_distance = target_distance - target_height;
            }
            move_distance = -move_distance;
            gCamera->t.t.translate.t.v[0] = camera_forward_scale * move_distance + gCamera->t.t.translate.t.v[0];
            gCamera->t.t.translate.t.v[1] += gFLOAT_00589418 * move_distance;
            gCamera->t.t.translate.t.v[2] = camera_forward_depth * move_distance + gCamera->t.t.translate.t.v[2];
        }
        if (gManual_camera_yaw != 0.0f) {
            gCamera->t.t.translate.t.v[0] -= target_position->v[0];
            gCamera->t.t.translate.t.v[1] -= target_position->v[1];
            gCamera->t.t.translate.t.v[2] -= target_position->v[2];
            BrMatrix34PostRotateY(&gCamera->t.t.mat, (br_angle)(gManual_camera_yaw * time
                / target_distance * gRadiansToAngle_00589448));
            gCamera->t.t.translate.t.v[0] += target_position->v[0];
            gCamera->t.t.translate.t.v[1] += target_position->v[1];
            gCamera->t.t.translate.t.v[2] += target_position->v[2];
        }
    }
    CollideCameraWithOtherCars(target_position, &gCamera->t.t.translate.t);
    CollideCamera2(&old_camera_position, &gCamera->t.t.translate.t, NULL, 1, pCar->collision_info);
    if (gCamera_has_collided != 0) {
        gManual_camera_yaw = 0.0f;
        gManual_camera_dolly = 0.0f;
        gManual_camera_vertical = 0.0f;
        gCamera->t.t.translate.t = old_camera_position;
    }
    PointCameraAtCar(target_position, &gCamera->t.t.mat, 1.0f);
}

// FUNCTION: CARMA2_HW 0x00411980
void C2_HOOK_FASTCALL SwingCamera(br_matrix34* pM1, br_matrix34* pM2, br_vector3* pVn, br_vector3* pOmega, float pSpeed, float pSpeedo_speed, tU32 pTime_difference, tCar_spec* pCar) {
    br_angle yaw;
    br_scalar cos_dtheta;
    br_scalar sign;
    br_scalar old_dot;
    int manual_swing;
    br_scalar v16;
    br_scalar v17;
    br_scalar v18;
    br_scalar abs_v18;
    br_angle v8;
    br_angle v9;
    // GLOBAL: CARMA2_HW 0x0058f63c
    static int elapsed_time = -1;
    // GLOBAL: CARMA2_HW 0x00679338
    static br_vector3 old_vn;

    manual_swing = gOld_yaw__car != gCamera_yaw;
    if (elapsed_time > 500) {
        elapsed_time = -1;
    }
    if (elapsed_time >= 0) {
        elapsed_time += pTime_difference;
    }
    sign = -BR_MAC3(pM1->m[2][0], pVn->v[0], pM1->m[2][1], pVn->v[1], pVn->v[2], pM1->m[2][2]);
    old_dot = BrVector3Dot(pVn, &old_vn);

    if ((sign < gFLOAT_00589418) != gCamera_sign) {
        if (old_dot <= gFLOAT_00589418 || elapsed_time >= 0) {
            if (gAction_replay_camera_mode != kActionReplayCameraMode_Standard || gCamera_sign) {
                if (elapsed_time < 0) {
                    elapsed_time = 0;
                }
                if (elapsed_time >= 500 || sign > gFLOAT_00589418) {
                    elapsed_time = 500;
                    if (fabs(pSpeedo_speed) > (sign > gFLOAT_00589418 ? gSwing_forward_speed_threshold : gSwing_reverse_speed_threshold) && !gCar_flying) {
                        gCamera_sign = !gCamera_sign;
                        gUNK_006792f4 = (br_angle)(long)(BR_MUL(BR_MUL(gUNK_006792f8 ? gSwing_fast_acceleration : gSwing_slow_acceleration, gSwing_reverse_acceleration_time), gDegreesToAngle_00589400));
                        if (gCamera_yaw > BR_ANGLE_DEG(180)) {
                            yaw = gCamera_yaw - BR_ANGLE_DEG(180);
                        } else {
                            yaw = gCamera_yaw;
                        }
                        if ((br_angle)(gCamera_yaw + BR_ANGLE_DEG(90)) > BR_ANGLE_DEG(180)) {
                            gCamera_yaw = BR_ANGLE_DEG(180) - gCamera_yaw;
                        } else if (yaw > BR_ANGLE_DEG(45) && yaw < BR_ANGLE_DEG(135)) {
                            gCamera_yaw = BR_ANGLE_DEG(180) - gCamera_yaw;
                        }
                        goto direction_ready;
                    }
                }
            }
            BrVector3Negate(pVn, pVn);
        } else {
            gCamera_sign = !gCamera_sign;
            if (gCamera_yaw > BR_ANGLE_DEG(180)) {
                yaw = gCamera_yaw - BR_ANGLE_DEG(180);
            } else {
                yaw = gCamera_yaw;
            }
            if (yaw > BR_ANGLE_DEG(45) && yaw < BR_ANGLE_DEG(135)) {
                gCamera_yaw = -gCamera_yaw;
            }
        }
    } else {
        elapsed_time = -1;
    }
direction_ready:
    BrVector3Copy(&old_vn, pVn);
    if (gCamera_sign) {
        yaw = -gCamera_yaw;
    } else {
        yaw = gCamera_yaw;
    }
    if (!gCar_flying) {
        br_scalar c;
        br_scalar s;
        br_scalar x;
        br_scalar z;

        c = cos(yaw * gBrAngleToRadiansScale);
        s = sin(yaw * gBrAngleToRadiansScale);
        x = BR_ADD(BR_MUL(pVn->v[0], c), BR_MUL(pVn->v[2], s));
        z = BR_SUB(BR_MUL(pVn->v[2], c), BR_MUL(pVn->v[0], s));
        pVn->v[0] = x;
        pVn->v[2] = z;
    }
    v16 = BR_ADD(BR_MUL(pVn->v[0], gView_direction.v[0]), BR_MUL(pVn->v[2], gView_direction.v[2]));
    v17 = BR_SUB(BR_MUL(pVn->v[0], gView_direction.v[2]), BR_MUL(pVn->v[2], gView_direction.v[0]));

    if (v16 < gHalf_00589428 && gCamera_yaw == 0) {
        gUNK_006792f8 = 1;
    }

    v18 = BR_MAC3(pM1->m[0][1], pOmega->v[0], pM1->m[1][1], pOmega->v[1], pM1->m[2][1], pOmega->v[2]);
    abs_v18 = fabsf(v18);
    v8 = (br_angle)(long)(pTime_difference * (abs_v18 - gSwing_negative_min_angular_speed) * gSwing_milliseconds_to_seconds * gRadiansToAngle_00589448);

    if (!(gUNK_006792f4 == 0 && v16 > gFLOAT_00589418 && (v9 = (br_angle)(long)(asin(fabsf(v17)) * gRadiansToAngle_00589448)) < v8) && !gCar_flying && !manual_swing) {
        br_angle omega;

        if (gUNK_006792f4 == 0) {
            gUNK_006792f4 = (br_angle)(long)(BR_MUL(BR_MUL(gUNK_006792f8 ? gSwing_fast_acceleration : gSwing_slow_acceleration, gSwing_initial_acceleration_time), gDegreesToAngle_00589400));
        }
        omega = pTime_difference * gUNK_006792f4 / 100;
        if (omega < v8) {
            omega = v8;
        }
        cos_dtheta = cos(omega * gBrAngleToRadiansScale);
        if (v16 < cos_dtheta) {
            double ts;

            if (v16 < gSwing_turn_dot_threshold && abs_v18 > gSwing_turn_speed_threshold && v18 * v17 > gFLOAT_00589418) {
                omega = -omega;
            }
            ts = omega * gBrAngleToRadiansScale;
            if (v17 > gFLOAT_00589418) {
                pVn->v[0] = sin(ts) * gView_direction.v[2] + cos(ts) * gView_direction.v[0];
                pVn->v[2] = cos(ts) * gView_direction.v[2] - sin(ts) * gView_direction.v[0];
            } else {
                pVn->v[0] = cos(ts) * gView_direction.v[0] - sin(ts) * gView_direction.v[2];
                pVn->v[2] = sin(ts) * gView_direction.v[0] + cos(ts) * gView_direction.v[2];
            }
            gUNK_006792f4 += (br_angle)(long)(BR_MUL(BR_MUL((tS32)pTime_difference, gUNK_006792f8 ? gSwing_fast_acceleration : gSwing_slow_acceleration), gDegreesToAngle_00589400));

            if (gUNK_006792f4 > (br_angle)(long)(BR_MUL(gUNK_006792f8 ? gSwing_fast_max_degrees : gSwing_slow_max_degrees, gDegreesToAngle_00589400))) {
                gUNK_006792f4 = (br_angle)(long)(BR_MUL(gUNK_006792f8 ? gSwing_fast_max_degrees : gSwing_slow_max_degrees, gDegreesToAngle_00589400));
            }
            return;
        }
    }
    gUNK_006792f4 = 0;
    gCamera_mode = 0;
    gUNK_006792f8 = 0;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00413570
int C2_HOOK_FASTCALL CollideCameraWithOtherCars(br_vector3* pPos, br_vector3* pCamera_pos) {

    return 0;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00411fc0
void C2_HOOK_FASTCALL PointCameraAtCar(br_vector3* pPos, br_matrix34* pMat, float pFov_factor) {
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar dist;
    br_scalar frac;
    br_angle theta;
    br_vector3* pos;
    br_camera* camera_ptr;
    int swoop;

    camera_ptr = gCamera->type_data;
    theta = (br_angle)(pFov_factor * camera_ptr->field_of_view / 5.f);
    swoop = gCountdown && gCamera_height > pPos->v[1] + 0.01f;
    if (swoop) {
        BrVector3Sub(&tv, &gAverage_grid_position, pPos);
        frac = (gCamera_height - pPos->v[1]) / 10.0f;
        BrVector3Scale(&tv, &tv, frac);
        BrVector3Add(&tv, pPos, &tv);
        pos = &tv;
        theta = (br_angle)((1.0f - frac) * (float)theta);
    } else {
        pos = pPos;
    }
    BrVector3Sub(&vn, pPos, (br_vector3*)pMat->m[3]);
    vn.v[1] = 0.f;
    BrVector3Normalise(&vn, &vn);
    pMat->m[0][0] = -vn.v[2];
    pMat->m[0][1] = 0.0f;
    pMat->m[0][2] = vn.v[0];
    pMat->m[1][0] = 0.0f;
    pMat->m[1][1] = 1.0f;
    pMat->m[1][2] = 0.0f;
    pMat->m[2][0] = -vn.v[0];
    pMat->m[2][1] = 0.0f;
    pMat->m[2][2] = -vn.v[2];
    BrVector3Sub(&tv2, pos, (br_vector3*)pMat->m[3]);
    dist = BrVector3Dot(&tv2, &vn);
    BrMatrix34PreRotateX(pMat, theta - BrRadianToAngle(atan2f(pMat->m[3][1] - pos->v[1], dist)));
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00414ca0
int C2_HOOK_FASTCALL IsCarInTheSea(void) {

    return gInTheSea;
}

// C2PORT-NONMATCHING: best 62.88% | difftest PASS 2000 trials (spec difftest/specs/0043aa80_FUN_0043aa80.json) | reason VC5 register and x87 scheduling
// FUNCTION: CARMA2_HW 0x0043aa80
void C2_HOOK_FASTCALL FUN_0043aa80(tCar_spec* pCar, void* pArg2) {
    int i;
    br_actor** wheel_actor;
    br_vector3 vector;
    br_vector3 new_position;

    wheel_actor = pCar->wheel_actors;
    i = 0;
    do {
        br_actor* actor;
        tGroovidelic_spec* groove;

        actor = wheel_actor[6];
        if (actor == NULL) {
            actor = wheel_actor[0];
        }
        if (actor != NULL) {
            groove = ActorsGroove(actor);
            if (groove != NULL && groove->path_type == eGroove_path_straight) {
                br_vector3* centre;
                br_vector3* crush_position;

                centre = &groove->path_data.straight_info.centre;
                crush_position = (br_vector3*)((tU8*)pCar->car_crush_spec->field_0x1ec + i);
                BrVector3Negate(&vector, centre);
                BrVector3Copy(centre, crush_position);
                FUN_00434f60(centre, pArg2);
                centre->v[1] = crush_position->v[1];
                BrVector3Add(&vector, centre, &vector);
                PipeSingleVector3(centre, &vector);
            }
        }
        i += 0xc;
        wheel_actor++;
    } while (i < 0x48);

    for (i = 0; i < CARPOCALYPSE2_ASIZE(pCar->wpos); i++) {
        new_position.v[0] = pCar->car_crush_spec->field_0x234[i].v[0] * gDOUBLE_00589a38;
        new_position.v[1] = pCar->car_crush_spec->field_0x234[i].v[1] * gDOUBLE_00589a38;
        new_position.v[2] = pCar->car_crush_spec->field_0x234[i].v[2] * gDOUBLE_00589a38;
        FUN_00434f60(&new_position, pArg2);
        pCar->wpos[i].v[0] = new_position.v[0] * gDOUBLE_00589af8;
        pCar->wpos[i].v[1] = new_position.v[1] * gDOUBLE_00589af8;
        pCar->wpos[i].v[2] = new_position.v[2] * gDOUBLE_00589af8;
    }

    SetCarSuspGiveAndHeight(pCar, 1.f, 1.f, 1.f, 0.f, 0.f);
    {
        tU8* model_data;
        float angle;

        model_data = *(tU8**)((tU8*)pArg2 + 0x34);
        if (*(float*)(model_data + 0x20) == pCar->car_crush_spec->bend_z_min) {
            angle = *(tU16*)(model_data + 0x3e) * gDOUBLE_00589b00;
            if (angle >= gFLOAT_00589b08) {
                angle -= gFLOAT_00589b0c;
            }
            pCar->field_0x1260 = angle * gFLOAT_00589b10;
        }
    }
}

// FUNCTION: CARMA2_HW 0x0043b840
float C2_HOOK_FASTCALL RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection) {

    return ComputeCarImpact(
        VEHICLE_TYPE_FROM_ID(pCar_ID) == eVehicle_self
            ? &gProgram_state.current_car
            : GetCarSpec(VEHICLE_TYPE_FROM_ID(pCar_ID), VEHICLE_INDEX_FROM_ID(pCar_ID)),
        pFrame_period,
        pTotal_deflection);
}

// FUNCTION: CARMA2_HW 0x0041e580
void C2_HOOK_FASTCALL CancelPendingCunningStunt(void) {

    gQuite_wild_end = 0;
    gQuite_wild_start = 0;
    gOn_me_wheels_start = 0;
    gWoz_upside_down_at_all = 0;
    gWild_start = 0;
}

// FUNCTION: CARMA2_HW 0x004157e0
void C2_HOOK_FASTCALL CalcGraphicalWheelStuff(tCar_spec* pCar) {

    pCar->steering_angle = BrRadianToDegree(atanf((pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * (pCar->field_0x1260 + pCar->curvature)));
    pCar->lr_sus_position = (pCar->wpos[0].v[1] - pCar->oldd[0]) / WORLD_SCALE;
    pCar->rr_sus_position = (pCar->wpos[1].v[1] - pCar->oldd[1]) / WORLD_SCALE;
    pCar->lf_sus_position = (pCar->wpos[2].v[1] - pCar->oldd[2]) / WORLD_SCALE;
    pCar->rf_sus_position = (pCar->wpos[3].v[1] - pCar->oldd[3]) / WORLD_SCALE;
    PipeSingleGraphicalWheelStuff(pCar);
}

// FUNCTION: CARMA2_HW 0x00416500
void C2_HOOK_FASTCALL FinishCars(tU32 pLast_frame_time, tU32 pTime) {
    int i;

    if (gCar_flying) {
        BrMatrix34Copy(&gCar_to_view->collision_info->actor->t.t.mat,
            &gCar_to_view->collision_info->transform_matrix);
    }
    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        tCar_spec* car;
        float original_speed;
        br_vector3 minus_k;

        car = gActive_car_list[i];
        BrMatrix34ApplyP(&car->pos, &car->collision_info->cmpos, &car->car_master_actor->t.t.mat);
        original_speed = car->speed;
        car->speed = BR_LENGTH2(car->collision_info->v.v[0], car->collision_info->v.v[2]) / 1000.f;
        BrVector3Negate(&minus_k, (br_vector3*)&car->car_master_actor->t.t.mat.m[2]);
        if (original_speed < 0.0001f && car->speed > 0.0001f) {
            car->speed = .0001f;
        }
        if (car->speed > .0001f) {
            BrVector3Normalise(&car->direction, &car->collision_info->v);
        } else {
            float ts;
            if (BrVector3Dot(&car->direction, &minus_k) < 0.f ) {
                ts = 1.f;
            } else {
                ts = -1.f;
            }
            BrVector3SetFloat(&minus_k, 0.f, 0.f, ts);
            BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
        }
        if (car != NULL && car->driver > 5) {
            int wheel;

            car->speedo_speed = BrVector3Dot(&minus_k, &car->collision_info->v) / 1000.f;
            CalcGraphicalWheelStuff(car);

            for (wheel = 0; wheel < 4; wheel++) {
                if (car->oldd[wheel] < car->susp_height[wheel / 2] && gCurrent_race.material_modifiers[car->material_index[wheel]].smoke_type >= 2 && !car->collision_info->disable_move_rotate) {
                    GenerateContinuousSmoke(car, wheel, pTime);
                }
            }
        }
    }
    if (pLast_frame_time < gPHIL_last_physics_tick && gCar_to_view->speed > .0001f) {
        br_vector3 tv;
        br_scalar dt;

        dt = (gPHIL_last_physics_tick - pLast_frame_time) / 40.f;
        BrVector3Sub(&tv, &gCar_to_view_original_v, &gCar_to_view->collision_info->v);
        BrVector3Scale(&tv, &tv, dt);
        BrVector3Accumulate(&tv, &gCar_to_view->collision_info->v);
        BrVector3Normalise(&gCar_to_view->direction, &tv);
    }
}

extern void C2_HOOK_FASTCALL AddNonCarActorToPipingSession(int pCar_ID, br_actor* pActor);

// FUNCTION: CARMA2_HW 0x004207a0
int C2_HOOK_FASTCALL PipeNonCarObject(tPhysics_object* pCollision_info, void* pUser_data) {

    if (pCollision_info->disable_move_rotate == 0 && pCollision_info->actor != NULL && pCollision_info->actor->type_data != NULL) {
        AddNonCarActorToPipingSession((tU16)((tNon_car_spec*)pCollision_info->actor->type_data)->car_ID, pCollision_info->actor);
    }
    ((tNon_car_spec*)pCollision_info->owner)->field_0xfc = pCollision_info->disable_move_rotate;
    return 0;
}

void C2_HOOK_FASTCALL PipeNonCars(void) {
    int i;

    ARStartPipingSession(ePipe_chunk_non_car);
    for (i = 0; i < gNum_active_non_cars; i++) {
        tNon_car_spec* non_car;

        non_car = gActive_non_car_list[i];
        if (non_car->car_ID != -1) {
            PhysicsObjectRecurse(non_car->collision_info, PipeNonCarObject, NULL);
        }
    }
    AREndPipingSession();
}

// FUNCTION: CARMA2_HW 0x00420310
int C2_HOOK_FASTCALL FUN_00420310(tPhysics_object* pObject) {
    pObject->field_0xed = 1;
    pObject->field_0x194 = 0;
    pObject->field_0x198 = 0;
    if (*(float*)((tU8*)pObject->owner + 0xd8) < gOne) {
        pObject->physics_joint2 = NULL;
    }
    pObject->field_0x218 = 0;
    pObject->v.v[0] = 0.0f;
    pObject->v.v[1] = 0.0f;
    pObject->v.v[2] = 0.0f;
    return 0;
}

// FUNCTION: CARMA2_HW 0x00420360
int C2_HOOK_FASTCALL FUN_00420360(tPhysics_object* pObject) {
    tNon_car_spec* non_car;

    non_car = (tNon_car_spec*)pObject->owner;
    if (*(br_scalar*)((tU8*)pObject->actor + 0x3c) < non_car->snap_off_cosine) {
        pObject->field_0xed = 1;
        pObject->field_0x194 = 0;
        pObject->field_0x198 = 0;
        if (non_car->snap_off_cosine < gOne) {
            pObject->physics_joint2 = NULL;
        }
        pObject->field_0x218 = 0;
        pObject->v.v[0] = 0.f;
        pObject->v.v[1] = 0.f;
        pObject->v.v[2] = 0.f;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004203c0
void C2_HOOK_FASTCALL CheckForDeAttachmentOfNonCars(tU32 pTime) {
    // GLOBAL: CARMA2_HW 0x00679418
    static tU32 total_time;
    int new_count;

    new_count = 0;
    if (gNum_active_non_cars != 0) {
        PipeNonCars();
        total_time += pTime;
        if (total_time >= 1000) {
            int i;

            total_time = 0;
            for (i = 0; i < gNum_active_non_cars; i++) {
                tNon_car_spec* non_car;

                non_car = gActive_non_car_list[i];

                if (non_car->actor->t.t.translate.t.v[1] < gMin_world_y - 10.f) {
                    non_car->collision_info->disable_move_rotate = 1;
                }
                if (TestForNan(&non_car->actor->t.t.translate.t.v[1])) {
                    BrVector3Set(&non_car->collision_info->omega, 0.f, 0.f, 0.f);
                    BrMatrix34Identity(&non_car->actor->t.t.mat);
                    BrVector3Set(&non_car->actor->t.t.translate.t, 2000.f, 0.f, 0.f);
                    non_car->collision_info->disable_move_rotate = 1;
                }
                gActive_non_car_list[new_count] = non_car;
                if (non_car->collision_info->disable_move_rotate && non_car->driver == eDriver_4) {
                    int j;
                    int drop;
                    br_actor *non_car_actor;

                    drop = 1;
                    non_car_actor = non_car->actor;
                    for (j = 0; j < gNum_cars_and_non_cars; j++) {
                        tCar_spec *car;

                        car = gActive_car_list[j];
                        if (car != (tCar_spec*)non_car && !car->collision_info->disable_move_rotate) {
                            br_matrix34 mat;
                            br_bounds3 bb;

                            BrMatrix34Mul(&mat, &non_car_actor->t.t.mat, &car->collision_info->field_0x144);
                            GetNewBoundingBox(&bb, &non_car_actor->model->bounds, &mat);
                            if (!(bb.min.v[0] > car->collision_info->field_0x124.max.v[0]
                                  || bb.min.v[1] > car->collision_info->field_0x124.max.v[1]
                                  || bb.min.v[2] > car->collision_info->field_0x124.max.v[2]
                                  || car->collision_info->field_0x124.min.v[0] > bb.max.v[0]
                                  || car->collision_info->field_0x124.min.v[1] > bb.max.v[1]
                                  || car->collision_info->field_0x124.min.v[2] > bb.max.v[2])) {
                                drop = 0;
                                break;
                            }
                        }
                    }
                    if (drop) {
                        C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x17c, 0x17c);

                        if (non_car->flags & 0x10000) {
                            if (Vector3DistanceSquared(&non_car->collision_info->field_0x17c,
                                    (br_vector3*)non_car->collision_info->transform_matrix.m[3]) > CARPOCALYPSE2_SQR(.005f)) {
                                drop = 0;
                            } else {
                                BrVector3Copy((br_vector3 *) &non_car->collision_info->transform_matrix.m[3],
                                    &non_car->collision_info->field_0x17c);
                            }
                        }
                        if ((tCar_spec*)non_car != gCar_to_view) {
                            BrActorRemove(non_car_actor);
                            ClearSplashes(non_car->collision_info);
                            new_count -= 1;
                            non_car->driver = eDriver_non_car_unused_slot;
                            if (non_car->car_ID != -1) {
                                tU8 col_x;
                                tU8 col_z;
                                br_actor *parent;

                                XZToColumnXZ(&col_x, &col_z, non_car_actor->t.t.mat.m[3][0],
                                    non_car_actor->t.t.mat.m[3][2], &gProgram_state.track_spec);
                                parent = gTrack_actor;
                                if (gProgram_state.track_spec.columns[col_z][col_x].actor_0x0 != NULL) {
                                    parent = gProgram_state.track_spec.columns[col_z][col_x].actor_0x0;
                                }
                                BrActorAdd(parent, non_car_actor);
                            } else if (gAdditional_actors != NULL) {
                                BrActorAdd(gAdditional_actors, non_car_actor);
                            }
                            non_car_actor->type_data = NULL;
                            PHILRemoveObject(non_car->collision_info);
                        }
                    }
                }
                new_count += 1;
            }
            gNum_active_non_cars = new_count;
        }
    }
}


__inline void C2_HOOK_FASTCALL StopSkid(tCar_spec* pC) {

    if (gLast_car_to_skid[0] == pC) {
        DRS3StopSound(gSkid_tag[0]);
    }
    if (gLast_car_to_skid[1] == pC) {
        DRS3StopSound(gSkid_tag[1]);
    }
}

// FUNCTION: CARMA2_HW 0x00415890
#ifdef __vita__
/* pre-collision breakdown for the physics spike log */
unsigned long long C2VitaPreFacesNS, C2VitaPreCarNS, C2VitaPreNonCarNS;
unsigned C2VitaPreFacesCalls, C2VitaPreCarCalls, C2VitaPreNonCarCalls;
extern unsigned long long C2VitaGLTicksNS(void);
#define C2_PRE_T(acc, cnt, stmt) do { unsigned long long c2_t = C2VitaGLTicksNS(); stmt; acc += C2VitaGLTicksNS() - c2_t; ++cnt; } while (0)
#else
#define C2_PRE_T(acc, cnt, stmt) stmt
#endif
void C2_HOOK_FASTCALL APTCPreCollision(void) {
    tPhysics_object* obj;
    tCar_spec* car;
    tNon_car_spec* non_car;
    tPhysics_object* node;
    void** wp;
    int i;
    float x;
    float f;

    gDrivable_on_count = 0;
    for (obj = gList_collision_infos; obj != NULL && gDrivable_on_count < 50; obj = obj->next) {
        if (obj->drivable_on) {
            gDrivable_on_list[gDrivable_on_count] = obj;
            gDrivable_on_count += 1;
        }
    }

    gCar_to_view_original_v.v[0] = gCar_to_view->collision_info->v.v[0];
    gCar_to_view_original_v.v[1] = gCar_to_view->collision_info->v.v[1];
    gCar_to_view_original_v.v[2] = gCar_to_view->collision_info->v.v[2];

    i = 0;
    if (i < gNum_active_cars) {
        wp = (void**)gActive_car_list;
        do {
            if (*wp != NULL && ((tCar_spec*)*wp)->driver > 5) {
                car = (tCar_spec*)*wp;
                BrMatrix34Copy(&car->car_master_actor->t.t.mat,
                               &car->collision_info->transform_matrix);
                if (car->collision_info->field_0x261 == 0x10
                        && car->collision_info->message_time == gPHIL_last_physics_tick) {
                    if (car == NULL || car->driver != eDriver_local_human) {
                        car->curvature = (float)car->collision_info->field_0x278 * car->maxcurve * 0.000030518509f;
                    }
                    if (gNet_mode == eNet_mode_host) {
                        car->collision_info->field_0xf0 = 1;
                    }
                    SetCollisionFlagsAndStuff(car);
                }
                if (car->disabled == 0) {
                    if (!(car->collision_info->disable_move_rotate != 0
                            && gPalette_fade_time != 0
                            && car != NULL
                            && car->driver == eDriver_local_human)) {
                        if (car->collision_info->box_face_ref != gFace_num__car
                                && (car->collision_info->box_face_ref != gFace_num__car - 1
                                    || car->collision_info->box_face_start <= gFace_count)) {
                            C2_PRE_T(C2VitaPreFacesNS, C2VitaPreFacesCalls, GetFacesInBox(car->collision_info, &gWorld_callbacks));
                        }
                        if (car->dt != 0.f) {
                            C2_PRE_T(C2VitaPreCarNS, C2VitaPreCarCalls, MoveAndCollideCar(car, 0.04f));
                        }
                        if (car->collision_info->child != NULL) {
                            PhysicsObjectMoveVelocityList(car->collision_info->child);
                            for (node = car->collision_info->child; node != NULL; node = node->next) {
                                AddDrag(car, node, 0.04f);
                                if (node->child != NULL) {
                                    DragChildren(car, node->child);
                                }
                            }
                            PositionChildren(car->collision_info);
                        }
                    }
                }
            }
            i++;
            wp++;
        } while (i < gNum_active_cars);
    }

    i = 0;
    if (i < gNum_active_non_cars) {
        wp = (void**)gActive_non_car_list;
        do {
            non_car = (tNon_car_spec*)*wp;
            if (non_car->collision_info->field_0x261 != 0
                    && non_car->collision_info->message_time == gPHIL_last_physics_tick) {
                SetCollisionFlagsAndStuff((tCar_spec*)non_car);
            }
            if (non_car->flags & 0x10000) {
                if (non_car->field_0xf8 != 0 && non_car->field_0xf8 < GetRaceTime()) {
                    non_car->field_0xf8 = 0;
                    non_car->flags = (non_car->flags & 0xffff0100) | 0x100;
                    non_car->collision_info->disable_move_rotate = 0;
                }
            }
            if (non_car->collision_info->disable_move_rotate == 0
                    || (non_car->flags & 0xff) != 0
                    || (non_car->flags & 0xff00) != 0) {
                non_car->collision_info->disable_move_rotate = 0;
                if (non_car->dt != 0.f) {
                    C2_PRE_T(C2VitaPreNonCarNS, C2VitaPreNonCarCalls, MoveAndCollideNonCar(non_car, 0.04f));
                }
                if (non_car->collision_info->child != NULL) {
                    PhysicsObjectMoveVelocityList(non_car->collision_info->child);
                    PositionChildren(non_car->collision_info);
                }
            }
            i++;
            wp++;
        } while (i < gNum_active_non_cars);
    }

    i = 0;
    if (i < gNum_active_cars) {
        wp = (void**)gActive_car_list;
        do {
            car = (tCar_spec*)*wp;
            if (car->field_0x4c8 != gZero) {
                x = car->field_0x4c8;
                f = car->collision_info->M; f *= x; car->collision_info->M = f;
                f = car->collision_info->I.v[0]; f *= x; car->collision_info->I.v[0] = f;
                f = car->collision_info->I.v[1]; f *= x; car->collision_info->I.v[1] = f;
                f = car->collision_info->I.v[2]; f *= x; car->collision_info->I.v[2] = f;
            }
            i++;
            wp++;
        } while (i < gNum_active_cars);
    }

    if (gCar_flying) {
        tCar_spec* c = gCar_to_view;

        c->collision_info->transform_matrix.m[3][1] -= gFneg1000;
        BrMatrix34Copy(&c->car_master_actor->t.t.mat,
                       &c->collision_info->transform_matrix);
        PositionChildren(c->collision_info);
        SetCollisionInfoChildsDoNothing(c->collision_info, 1);
    }
    MinePreCollisionStuff();
    PedPreCollisionStuff();
    DronePreCollisionStuff();
}

#pragma auto_inline(off)

// FUNCTION: CARMA2_HW 0x00415c40
void C2_HOOK_FASTCALL SetCollisionFlagsAndStuff(tCar_spec* pCar) {
    float temp;
    int i;

    if (gNet_mode == eNet_mode_host
            && pCar->collision_info->field_0x49c > pCar->collision_info->field_0x274) {
        pCar->collision_info->field_0x261 = 0;
        pCar->dt = -1.f;
        return;
    }

    switch (pCar->collision_info->field_0xf0) {
    case 0:
        SelectCrushNetwork(&pCar->collision_info->field_0x294, pCar->collision_info, 2);
        break;
    case 1:
        SelectCrushNetwork(&pCar->collision_info->field_0x294, pCar->collision_info, 1);
        break;
    case 2:
        SelectCrushNetwork(&pCar->collision_info->field_0x294, pCar->collision_info, 1);
        break;
    }

    if (pCar != NULL && pCar->driver > 5) {
        for (i = 0; i < 4; i++) {
            pCar->oldd[i] = (float)pCar->collision_info->field_0x26c[i]
                    * pCar->susp_height[i >> 1] * (1.f / 255.f);
        }

        if (pCar->driver == eDriver_net_human) {
            pCar->revs = (float)pCar->collision_info->field_0x27a;
        }

        if (pCar->driver >= eDriver_net_human
                && pCar->collision_info->field_0x27c > pCar->repair_time) {
            if (pCar->collision_info->field_0x27c - pCar->repair_time >= 100000) {
                TotallyRepairACar(pCar);
                pCar->repair_time = pCar->collision_info->field_0x27c;
            } else {
                ComputeCarImpact(pCar, pCar->collision_info->field_0x27c - pCar->repair_time, &temp);
            }
            for (i = 0; i < 12; i++) {
                pCar->damage_units[i].damage_level = (int)pCar->collision_info->field_0x280[i];
            }
            SetSmokeLastDamageLevel(pCar);
            StopCarSmoking(pCar);
        } else {
            for (i = 0; i < 12; i++) {
                pCar->damage_units[i].damage_level = (int)pCar->collision_info->field_0x280[i];
            }
            SortOutSmoke(pCar);
        }

        if (pCar->collision_info->disable_move_rotate != 0) {
            if (BrVector3LengthSquared(&pCar->collision_info->v) != 0.f) {
                pCar->collision_info->disable_move_rotate = 0;
            }
        }

        if (pCar->driver >= eDriver_net_human
                && (pCar->collision_info->field_0x292 & 1) != 0
                && (pCar->collision_info->field_0x290 != pCar->car_crush_spec->field_0x24
                        || pCar->driver == eDriver_local_human)) {
            if ((pCar->collision_info->field_0x292 & 2) != 0) {
                WeldCarByNetwork(pCar);
            } else if (pCar->car_crush_spec->field_0x144 != 0) {
                CrushCarByNetwork(pCar);
            }

            if ((pCar->collision_info->field_0x292 & 4) != 0) {
                WeldCarPartial(pCar);
            } else if (pCar->car_crush_spec->field_0x4b8 != 0) {
                WeldCar(pCar);
            }

            UpdateCrushVertices(pCar, (tU8*)&pCar->collision_info->field_0x2fc);
        }

        if (pCar->driver != eDriver_local_human) {
            for (i = 0; i < 4; i++) {
                pCar->wheel_dam_offset[i] = pCar->collision_info->field_0x48c[i];
            }
        }

        GetFacesInBox(pCar->collision_info, &gWorld_callbacks);
    }

    pCar->collision_info->field_0x261 = 0;
    if (gNet_mode == eNet_mode_client) {
        pCar->collision_info->field_0x49c = pCar->collision_info->field_0x274;
    }
}

// FUNCTION: CARMA2_HW 0x00415ef0
void C2_HOOK_FASTCALL AddDrag(tCar_spec* pCar, tPhysics_object* pObject, br_scalar pDt) {
    br_scalar drag;
    br_scalar k;
    br_vector3 v;

    drag = pDt * 0.0005f;
    if (pObject->last_special_volume != NULL) {
        k = pObject->last_special_volume->viscosity_multiplier;
        if (pCar->underwater_ability != 0) {
            k *= 0.6;
        }
        drag *= k;
        drag *= pObject->water_depth_factor;
    }

    k = BrVector3Length(&pObject->v) * drag * 6.9 / pObject->M;
    v.v[0] = pObject->v.v[0] * k;
    v.v[1] = pObject->v.v[1] * k;
    v.v[2] = pObject->v.v[2] * k;
    DRVector3Diminish(&pObject->v, &v);

    k = BrVector3Length(&pObject->omega) * drag;
    v.v[0] = pObject->omega.v[0] * k;
    v.v[1] = pObject->omega.v[1] * k;
    v.v[2] = pObject->omega.v[2] * k;
    ApplyWaterOmegaBrake(pObject, &v);
}

// FUNCTION: CARMA2_HW 0x00416030
void C2_HOOK_FASTCALL DragChildren(tCar_spec* pCar, tPhysics_object* pChild) {

    while (pChild != NULL) {
        AddDrag(pCar, pChild, 0.04f);
        if (pChild->child != NULL) {
            DragChildren(pCar, pChild->child);
        }
        pChild = pChild->next;
    }
}

#ifndef CARPOCALYPSE2_MATCHING
/* port diagnostics: C2_PHYSLOG=1 logs MoveAndCollideCar calls over 1 ms (DIAGNOST.TXT) */
static int gC2_physlog = -1;
static int gC2_calcforce_us;
#define C2_TIMED_CALCFORCE(stmt) do { int c2t = PDGetMicroseconds(); stmt; gC2_calcforce_us += PDGetMicroseconds() - c2t; } while (0)
static void C2_HOOK_FASTCALL MoveAndCollideCar_body(tCar_spec* pCar, br_scalar pDt);
void C2_HOOK_FASTCALL MoveAndCollideCar(tCar_spec* pCar, br_scalar pDt) {
    int t0;
    if (gC2_physlog < 0) gC2_physlog = getenv("C2_PHYSLOG") != NULL;
    if (!gC2_physlog) { MoveAndCollideCar_body(pCar, pDt); return; }
    gC2_calcforce_us = 0;
    t0 = PDGetMicroseconds();
    MoveAndCollideCar_body(pCar, pDt);
    t0 = PDGetMicroseconds() - t0;
    if (t0 > 150) dr_dprintf("PHYSLOG MoveAndCollideCar car=%p driver=%d us=%d calcforce_us=%d", (void*)pCar, (int)pCar->driver, t0, gC2_calcforce_us);
}
#define C2_MOVE_AND_COLLIDE_CAR_BODY MoveAndCollideCar_body
#else
#define C2_TIMED_CALCFORCE(stmt) stmt
#define C2_MOVE_AND_COLLIDE_CAR_BODY MoveAndCollideCar
#endif

// FUNCTION: CARMA2_HW 0x004168c0
void C2_HOOK_FASTCALL C2_MOVE_AND_COLLIDE_CAR_BODY(tCar_spec* pCar, br_scalar pDt) {
    br_scalar wheel_spin_force;
    br_scalar ts;
    int new_gear;
    br_vector3 v;
    br_scalar scale;
    br_scalar k;
    int i;

    if (pCar->dt >= 0.f) {
        pDt = pCar->dt;
    }
    if (pDt == 0.f) {
        return;
    }
    if (gCar_flying && pCar == gCar_to_view) {
        return;
    }

    pCar->new_skidding = 0;
    if (pCar->collision_info->water_d != 10000.f) {
        TestAutoSpecialVolume(pCar->collision_info);
    }
    MungeSpecialVolume(pCar->collision_info);
    BrVector3Scale((br_vector3*)&pCar->car_master_actor->t.t.mat.m[3],
        (br_vector3*)&pCar->car_master_actor->t.t.mat.m[3], WORLD_SCALE);

    if (pCar->field_0x18c8 != 0) {
        if (pCar->collision_info->disable_move_rotate == 0) {
            k = pDt * 10.f;
            if (pCar->collision_info->last_special_volume != NULL) {
                k *= pCar->collision_info->last_special_volume->gravity_multiplier;
                pCar->collision_info->v.v[1] = pCar->collision_info->v.v[1] - k * (1.0 / 6.9);
            } else {
                pCar->collision_info->v.v[1] = pCar->collision_info->v.v[1] - k * (1.0 / 6.9);
            }
        }
        if (pCar->field_0x18c8 == 2) {
            BrMatrix34TApplyV(&v, (br_vector3*)&gCamera->t.t.mat,
                &pCar->collision_info->actor->t.t.mat);
            scale = BR_LENGTH3(v.v[0], v.v[1], v.v[2]);
            if (scale > BR_SCALAR_EPSILON * 2) {
                scale = gZero / scale;
                v.v[0] = v.v[0] * scale;
                v.v[1] = v.v[1] * scale;
                v.v[2] = v.v[2] * scale;
            } else {
                v.v[0] = 1.f;
                v.v[1] = 0.f;
                v.v[2] = 0.f;
            }
            if (pCar->keys.dec) {
                pCar->collision_info->omega.v[0] += v.v[0];
                pCar->collision_info->omega.v[1] += v.v[1];
                pCar->collision_info->omega.v[2] += v.v[2];
            }
            if (pCar->keys.acc) {
                pCar->collision_info->omega.v[0] -= v.v[0];
                pCar->collision_info->omega.v[1] -= v.v[1];
                pCar->collision_info->omega.v[2] -= v.v[2];
            }
            scale = BR_LENGTH3(pCar->collision_info->velocity_car_space.v[0],
                pCar->collision_info->velocity_car_space.v[1],
                pCar->collision_info->velocity_car_space.v[2]);
            if (scale > BR_SCALAR_EPSILON * 2) {
                scale = gZero / scale;
                ts = pCar->collision_info->velocity_car_space.v[0];
                v.v[0] = ts * scale;
                ts = pCar->collision_info->velocity_car_space.v[1];
                v.v[1] = ts * scale;
                ts = pCar->collision_info->velocity_car_space.v[2];
                v.v[2] = ts * scale;
            } else {
                v.v[0] = 1.f;
                v.v[1] = 0.f;
                v.v[2] = 0.f;
            }
            if (pCar->keys.right) {
                pCar->collision_info->omega.v[0] += v.v[0];
                pCar->collision_info->omega.v[1] += v.v[1];
                pCar->collision_info->omega.v[2] += v.v[2];
            }
            if (pCar->keys.left) {
                pCar->collision_info->omega.v[0] -= v.v[0];
                pCar->collision_info->omega.v[1] -= v.v[1];
                pCar->collision_info->omega.v[2] -= v.v[2];
            }
            if (pCar->keys.brake) {
                pCar->collision_info->omega.v[0] = 0.f;
                pCar->collision_info->omega.v[1] = 0.f;
                pCar->collision_info->omega.v[2] = 0.f;
            }
        }
    } else if (pCar == NULL || pCar->driver < eDriver_net_human) {
        C2_TIMED_CALCFORCE(CalcForce(pCar, pDt));
    } else {
        if (gCar_flying && pCar->driver == eDriver_local_human) {
            pCar->acc_force = 0.f;
        } else {
            CalcEngineForce(pCar, pDt);
        }
        C2_TIMED_CALCFORCE(CalcForce(pCar, pDt));

        ts = -BrVector3Dot(&pCar->collision_info->v, (br_vector3*)&pCar->car_master_actor->t.t.mat.m[2]) * 6.9;
        pCar->target_revs = (pCar->gear == 0) ? 0.f : (float)(ts / pCar->speed_revs_ratio / (double)pCar->gear);
        if (pCar->target_revs < 0.f) {
            pCar->target_revs = 0.f;
            pCar->gear = 0;
        }
        if (!pCar->number_of_wheels_on_ground || ((pCar->wheel_slip & 2) + 1) != 0 || !pCar->gear) {
            if (!pCar->number_of_wheels_on_ground) {
                wheel_spin_force = pCar->torque * pCar->force_torque_ratio;
            } else {
                wheel_spin_force = pCar->torque * pCar->force_torque_ratio
                    - pCar->gear * pCar->acc_force;
            }
            if (pCar->gear == 0) {
                if (pCar->revs > gF1000 && !pCar->keys.brake
                        && (pCar->keys.acc || pCar->joystick.acc > 0)
                        && gCountdown == 0) {
                    new_gear = pCar->keys.backwards ? -1 : 1;
                    pCar->gear = new_gear;
                    pCar->target_revs = ts / pCar->speed_revs_ratio / (double)new_gear;
                }
                wheel_spin_force = pCar->torque * pCar->force_torque_ratio;
            } else if (pCar->gear < 2 && (pCar->keys.dec || pCar->joystick.dec > 0)
                    && fabsf(ts) < 1.f && pCar->revs > gF1000) {
                pCar->gear = -pCar->gear;
            }
            pCar->revs = pCar->revs
                - wheel_spin_force / pCar->force_torque_ratio * pDt * -5000.;
            if (pCar->traction_control && wheel_spin_force > 0.f) {
                if (pCar->revs > pCar->target_revs && pCar->gear != 0
                        && pCar->target_revs > gF1000) {
                    pCar->revs = pCar->target_revs;
                }
            }
            if (pCar->revs <= 0.f) {
                pCar->revs = 0.f;
            }
        }
        while ((pCar->wheel_slip & 2) == 0 && pCar->target_revs > 6000.f
                && pCar->gear < pCar->max_gear && pCar->gear > 0
                && !pCar->just_changed_gear) {
            pCar->target_revs = pCar->target_revs * pCar->gear / (pCar->gear + 1);
            pCar->gear++;
        }
        while (pCar->gear > 1 && pCar->target_revs < 3000.f
                && !pCar->just_changed_gear) {
            pCar->target_revs = pCar->target_revs * pCar->gear / (pCar->gear - 1);
            pCar->gear--;
        }
        if (pCar->revs < 200.f && pCar->target_revs < 200.f && pCar->gear <= 1
                && !pCar->keys.acc && pCar->joystick.acc <= 0
                && !pCar->just_changed_gear) {
            pCar->gear = 0;
        }
        if (pCar->just_changed_gear && pCar->revs < 6000.f && pCar->revs > 200.f
                && (pCar->gear < 2 || pCar->revs >= 3000.f)) {
            pCar->just_changed_gear = 0;
        }
        if (pCar->revs >= 6000.f && (pCar->keys.acc || pCar->joystick.acc > 0)) {
            pCar->just_changed_gear = 0;
        }
    }

    BrVector3InvScale((br_vector3*)&pCar->car_master_actor->t.t.mat.m[3],
        (br_vector3*)&pCar->car_master_actor->t.t.mat.m[3], WORLD_SCALE);
    BrMatrix34ApplyP(&pCar->pos, &pCar->collision_info->cmpos,
        &pCar->collision_info->transform_matrix);
    for (i = 0; i < 4; i++) {
        SkidMark(pCar, i);
    }
}

#undef C2_MOVE_AND_COLLIDE_CAR_BODY

void C2_HOOK_FASTCALL ApplyNonCarTumble(tPhysics_object* pObject, br_vector3* pV, br_scalar pTumbleFactor, br_scalar pTumbleThreshold);

// C2PORT-NONMATCHING: best 49.88% | difftest PASS 2000 trials (spec difftest/specs/00418250_MoveNonCar.json) | reason vc5-x87-scheduling-register-allocation
// FUNCTION: CARMA2_HW 0x00418250
void C2_HOOK_FASTCALL MoveNonCar(tNon_car_spec* pNon_car, br_scalar pDt) {
    tSpecial_volume* special_volume;
    br_vector3 world_hinge_axis;
    br_vector3 drag;
    br_scalar gravity;
    br_scalar drag_scale;
    br_scalar scale;
    br_scalar along_axis;
    br_scalar velocity_difference;
    br_scalar resistance;
    tU32 flags;
    tU8 pushes;
    tPhysics_object* parent;
    special_volume = pNon_car->collision_info->last_special_volume;
    pNon_car->flags |= 0x100000;
    if (gCar_flying == 0 || gCar_to_view != (tCar_spec*)pNon_car) {
        if (pNon_car->actor->identifier[3] != '!') {
            pNon_car->actor->identifier[3] = '&';
            if (pNon_car->collision_info->physics_joint2 == NULL) {
                pNon_car->collision_info->actor->identifier[3] = '!';
                pNon_car->collision_info->omega.v[0] = 0.f;
                pNon_car->collision_info->omega.v[1] = 0.f;
                pNon_car->collision_info->omega.v[2] = 0.f;
                pNon_car->collision_info->v.v[0] = 0.f;
                pNon_car->collision_info->v.v[1] = 0.f;
                pNon_car->collision_info->v.v[2] = 0.f;
                pNon_car->collision_info->M = pNon_car->free_mass;
                pNon_car->collision_info->field_0xed = 1;
                pNon_car->collision_info->I.v[0] = pNon_car->I_over_M.v[0] * pNon_car->collision_info->M;
                pNon_car->collision_info->I.v[1] = pNon_car->I_over_M.v[1] * pNon_car->collision_info->M;
                pNon_car->collision_info->I.v[2] = pNon_car->I_over_M.v[2] * pNon_car->collision_info->M;
                parent = pNon_car->collision_info->field183_0x1d8;
                if (parent != NULL && parent->owner != NULL && parent->flags_0x238 == 1 && parent->owner != pNon_car) {
                    PushObject(pNon_car->collision_info, &pNon_car->pos, &pNon_car->collision_info->v, (br_vector3*)&gVector3_00661678);
                }
            } else {
                BrVector3SetFloat(&pNon_car->collision_info->v, 0.f, 0.f, 0.f);
                BrVector3SetFloat(&pNon_car->collision_info->omega, 0.f, 0.f, 0.f);
                BrVector3SetFloat(&pNon_car->collision_info->rotate_omega, 0.f, 0.f, 0.f);
                pNon_car->collision_info->disable_move_rotate = 1;
            }
        }
        if (pNon_car->actor->identifier[3] == '!') {
            if (special_volume == NULL) {
                gravity = (br_scalar)(pDt * gDouble_00589790);
            } else {
                gravity = (br_scalar)(pDt * gFlyCarAccelerationRate2 * special_volume->gravity_multiplier * gFlyCarAngleDamping);
            }
            pNon_car->collision_info->field_0x54.v[0] = 0.f;
            pNon_car->collision_info->field_0x54.v[1] = -gravity;
            pNon_car->collision_info->field_0x54.v[2] = 0.f;
            pNon_car->collision_info->v.v[1] -= gravity;

            scale = BrVector3Length(&pNon_car->collision_info->v) * 6.9;
            if (special_volume != NULL) {
                scale *= special_volume->viscosity_multiplier;
            }
            drag_scale = pDt * 0.0005f;
            scale = (br_scalar)(scale * drag_scale / pNon_car->collision_info->M);
            drag.v[0] = pNon_car->collision_info->v.v[0] * scale;
            drag.v[1] = pNon_car->collision_info->v.v[1] * scale;
            drag.v[2] = pNon_car->collision_info->v.v[2] * scale;
            DRVector3Diminish(&pNon_car->collision_info->v, &drag);

            scale = BrVector3Length(&pNon_car->collision_info->omega);
            if (special_volume != NULL) {
                scale *= special_volume->viscosity_multiplier;
            }
            scale *= drag_scale;
            drag.v[0] = pNon_car->collision_info->omega.v[0] * scale;
            drag.v[1] = pNon_car->collision_info->omega.v[1] * scale;
            drag.v[2] = pNon_car->collision_info->omega.v[2] * scale;
            ApplyWaterOmegaBrake(pNon_car->collision_info, &drag);
            if (pNon_car->tumble_factor != gFlyCarZero) {
                ApplyNonCarTumble(pNon_car->collision_info, &pNon_car->collision_info->v, pNon_car->tumble_factor, pNon_car->tumble_threshold);
            }
            if ((pNon_car->flags & 0x10000) != 0) {
                BrMatrix34ApplyV(&world_hinge_axis, &pNon_car->field_0xf0->hinge_axis, &pNon_car->collision_info->transform_matrix);
                along_axis = pNon_car->field_0xf0->hinge_axis.v[1] * pNon_car->collision_info->velocity_car_space.v[1]
                    + pNon_car->field_0xf0->hinge_axis.v[0] * pNon_car->collision_info->velocity_car_space.v[0]
                    + pNon_car->field_0xf0->hinge_axis.v[2] * pNon_car->collision_info->velocity_car_space.v[2];
                velocity_difference = along_axis - (pNon_car->collision_info->field_0x1b4.v[0] * world_hinge_axis.v[0]
                    + pNon_car->collision_info->field_0x1b4.v[2] * world_hinge_axis.v[2]
                    + pNon_car->collision_info->field_0x1b4.v[1] * world_hinge_axis.v[1]);
                resistance = pNon_car->translation_parameters->forward_resistance;
                pDt = 0.f;
                flags = pNon_car->flags;
                if ((flags & 0x20000) != 0 && (pNon_car->collision_info->collision_flag & 2) != 0
                        && (flags & 0xff) == 0 && ((flags >> 8) & 0xff) == 0
                        && pNon_car->field_0xf8 == 0 && gNet_mode != eNet_mode_client) {
                    pNon_car->flags = (flags & ~0xffu) | 1;
                }
                flags = pNon_car->flags;
                if ((flags & 0xff) == 0 && (pNon_car->collision_info->disable_move_rotate != 0 || pNon_car->field_0xf8 == 0)) {
                    pushes = (tU8)(flags >> 8);
                    if (pushes != 0 || (pNon_car->collision_info->disable_move_rotate == 0 && pNon_car->field_0xf8 == 0)) {
                        resistance = pNon_car->translation_parameters->reverse_resistance;
                        pDt = (br_scalar)(-pNon_car->translation_parameters->reverse_acceleration * gDouble_00589798);
                        if (gDouble_00589738 < velocity_difference && gDouble_005897a0 < along_axis) {
                            if (pushes < pNon_car->number_of_pushes && (pNon_car->collision_info->collision_flag & 2) != 0) {
                                pNon_car->flags = flags + 0x100;
                            } else {
                                pNon_car->flags = flags & 0xffff00ff;
                                pNon_car->collision_info->disable_move_rotate = 1;
                                BrVector3SetFloat(&pNon_car->collision_info->v, 0.f, 0.f, 0.f);
                                BrVector3SetFloat(&pNon_car->collision_info->omega, 0.f, 0.f, 0.f);
                                pDt = 0.f;
                            }
                        }
                    }
                } else {
                    pDt = (br_scalar)(pNon_car->translation_parameters->forward_acceleration * gDouble_00589798);
                    if (velocity_difference < gDouble_005897a8 && along_axis < gDouble_005897b0) {
                        if (pNon_car->field_0xf8 == 0 || (flags & 0xff) == 1) {
                            pNon_car->field_0xf8 = GetRaceTime() + pNon_car->translation_parameters->pause_at_top;
                        }
                        flags = pNon_car->flags;
                        if ((flags & 0xff) < pNon_car->number_of_pushes && (pNon_car->collision_info->collision_flag & 2) != 0) {
                            if ((flags & 0xff) != 0) {
                                pNon_car->flags = flags + 1;
                            }
                        } else {
                            pNon_car->flags = flags & 0xffffff00;
                            pNon_car->collision_info->disable_move_rotate = 1;
                            BrVector3SetFloat(&pNon_car->collision_info->v, 0.f, 0.f, 0.f);
                            BrVector3SetFloat(&pNon_car->collision_info->omega, 0.f, 0.f, 0.f);
                            pDt = 0.f;
                        }
                    }
                }
                pNon_car->collision_info->v.v[0] = (pNon_car->field_0xf0->hinge_axis.v[0] * pDt + pNon_car->collision_info->v.v[0]) * resistance;
                pNon_car->collision_info->v.v[1] = (pNon_car->field_0xf0->hinge_axis.v[1] * pDt + pNon_car->collision_info->v.v[1]) * resistance;
                pNon_car->collision_info->v.v[2] = (pNon_car->field_0xf0->hinge_axis.v[2] * pDt + pNon_car->collision_info->v.v[2]) * resistance;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x00417030
void C2_HOOK_FASTCALL MoveAndCollideNonCar(tNon_car_spec* pNon_car, br_scalar pDt) {

    if (pNon_car->collision_info->water_d != 10000.f) {
        TestAutoSpecialVolume(pNon_car->collision_info);
    }
    MungeSpecialVolume(pNon_car->collision_info);
    if (pNon_car->dt >= 0.f) {
        pDt = pNon_car->dt;
    }
    MoveNonCar(pNon_car, pDt);
    BrMatrix34ApplyP(&pNon_car->pos, &pNon_car->centre_of_mass_world_scale, &pNon_car->actor->t.t.mat);
    pNon_car->pos.v[0] *= 1.f / WORLD_SCALE;
    pNon_car->pos.v[1] *= 1.f / WORLD_SCALE;
    pNon_car->pos.v[2] *= 1.f / WORLD_SCALE;
}

// FUNCTION: CARMA2_HW 0x004c2060
void C2_HOOK_FASTCALL GetFacesInBox(tPhysics_object* pCollision, tWorld_callbacks* pWorld_callbacks) {
    tBounds bnds;
    br_bounds xformed_bounds;
    br_bounds tmp_bounds;
    br_vector3 vel;
    br_matrix34 mat1;
    br_matrix34 mat2;
    br_matrix34 mat3;
    br_matrix34 mat4;
    int i;
    int flags;

    if (pCollision->flags & 0x40) {
        pCollision->box_face_start = pCollision->box_face_end;
        return;
    }
    if ((pCollision->flags & 0x2) && pCollision->parent != NULL) {
        pCollision->box_face_start = pCollision->parent->box_face_start;
        pCollision->box_face_end = pCollision->parent->box_face_end;
        pCollision->box_face_ref = pCollision->parent->box_face_ref;
        return;
    }
    if (pCollision->flags & 0x1) {
        if (pCollision->box_face_ref == gFace_num__car
                || (pCollision->box_face_ref == gFace_num__car - 1
                    && pCollision->box_face_start > gFace_count)) {
            if (pCollision->field_0x10c.min.v[0] > pCollision->field_0x124.min.v[0]
                    && pCollision->field_0x10c.min.v[1] > pCollision->field_0x124.min.v[1]
                    && pCollision->field_0x10c.min.v[2] > pCollision->field_0x124.min.v[2]
                    && pCollision->field_0x10c.max.v[0] < pCollision->field_0x124.max.v[0]
                    && pCollision->field_0x10c.max.v[1] < pCollision->field_0x124.max.v[1]
                    && pCollision->field_0x10c.max.v[2] < pCollision->field_0x124.max.v[2]) {
                return;
            }
        }
        BrVector3Scale(&vel, &pCollision->v, 0.12f);
        for (i = 0; i < 3; i++) {
            bnds.original_bounds.min.v[i] = pCollision->field_0x10c.min.v[i] - 0.15;
            bnds.original_bounds.max.v[i] = pCollision->field_0x10c.max.v[i] + 0.15;
            if (vel.v[i] < 0.f) {
                bnds.original_bounds.min.v[i] += vel.v[i];
            } else {
                bnds.original_bounds.max.v[i] += vel.v[i];
            }
            pCollision->field_0x124.min.v[i] = bnds.original_bounds.min.v[i] + 0.005;
            pCollision->field_0x124.max.v[i] = bnds.original_bounds.max.v[i] - 0.005;
        }
        bnds.mat = &mat3;
        BrMatrix34Identity(&mat3);
    } else {
        br_matrix34* actor_mat = &pCollision->actor->t.t.mat;
        br_matrix34* trans_mat = &pCollision->transform_matrix;

        if (pCollision->box_face_ref == gFace_num__car
                || (pCollision->box_face_ref == gFace_num__car - 1
                    && pCollision->box_face_start > gFace_count)) {
            BrMatrix34Mul(&mat1, actor_mat, &pCollision->field_0x144);
            GetNewBoundingBox(&xformed_bounds, &pCollision->bb2, &mat1);
            if (xformed_bounds.max.v[0] < pCollision->field_0x124.max.v[0]
                    && xformed_bounds.max.v[1] < pCollision->field_0x124.max.v[1]
                    && xformed_bounds.max.v[2] < pCollision->field_0x124.max.v[2]
                    && xformed_bounds.min.v[0] > pCollision->field_0x124.min.v[0]
                    && xformed_bounds.min.v[1] > pCollision->field_0x124.min.v[1]
                    && xformed_bounds.min.v[2] > pCollision->field_0x124.min.v[2]) {
                return;
            }
        }
        BrMatrix34LPInverse(&mat3, actor_mat);
        BrMatrix34Mul(&mat2, trans_mat, &mat3);
        GetNewBoundingBox(&bnds.original_bounds, &pCollision->bb2, &mat2);
        for (i = 0; i < 3; i++) {
            bnds.original_bounds.min.v[i] = bnds.original_bounds.min.v[i] < pCollision->bb2.min.v[i]
                ? bnds.original_bounds.min.v[i] : pCollision->bb2.min.v[i];
            bnds.original_bounds.max.v[i] = bnds.original_bounds.max.v[i] > pCollision->bb2.max.v[i]
                ? bnds.original_bounds.max.v[i] : pCollision->bb2.max.v[i];
            bnds.original_bounds.min.v[i] -= 0.0025f;
            bnds.original_bounds.max.v[i] += 0.0025f;
        }
        BrMatrix34Mul(&mat1, &mat2, &mat2);
        BrMatrix34Mul(&mat4, &mat1, &mat2);
        BrMatrix34LPInverse(&mat1, &mat4);
        GetNewBoundingBox(&tmp_bounds, &pCollision->bb2, &mat1);
        for (i = 0; i < 3; i++) {
            bnds.original_bounds.min.v[i] = bnds.original_bounds.min.v[i] < tmp_bounds.min.v[i]
                ? bnds.original_bounds.min.v[i] : tmp_bounds.min.v[i];
            bnds.original_bounds.max.v[i] = bnds.original_bounds.max.v[i] > tmp_bounds.max.v[i]
                ? bnds.original_bounds.max.v[i] : tmp_bounds.max.v[i];
            bnds.original_bounds.min.v[i] -= 0.02f;
            bnds.original_bounds.max.v[i] += 0.02f;
        }
        pCollision->field_0x124 = bnds.original_bounds;
        BrMatrix34Copy(&pCollision->field_0x144, &mat3);
        bnds.mat = actor_mat;
    }
    pCollision->box_face_start = gFace_count;
    gPling_face = NULL;
    flags = pCollision->flags;
    gActorBoxPick_StopGroovidelics = (tU8)(~flags) >> 7;
    if (pWorld_callbacks != NULL && pWorld_callbacks->find_faces_in_box != NULL) {
        gFace_count += pWorld_callbacks->find_faces_in_box(&bnds, &gFace_list__car[gFace_count], 300 - gFace_count, pWorld_callbacks);
        if (gFace_count >= 300) {
            pCollision->box_face_start = 0;
            gFace_count = pWorld_callbacks->find_faces_in_box(&bnds, gFace_list__car, 300, pWorld_callbacks);
            gFace_num__car++;
        }
    }
    pCollision->box_face_end = gFace_count;
    pCollision->box_face_ref = gFace_num__car;
    gActorBoxPick_StopGroovidelics = 1;
    if (pWorld_callbacks != NULL && pWorld_callbacks->new_face_list != NULL) {
        pWorld_callbacks->new_face_list(pCollision, gPling_face);
    }
    if (pCollision->flags & 0x80) {
        pCollision->box_face_end = pCollision->box_face_start;
    }
}

#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00416070
void C2_HOOK_FASTCALL APTCPostCollision(void) {
    void** wp;
    tCar_spec* car;
    int i;

    if (gCar_flying) {
        tCar_spec* c = gCar_to_view;
        c->collision_info->transform_matrix.m[3][1] -= gF1000;

        BrMatrix34Copy(&c->car_master_actor->t.t.mat,
                       &c->collision_info->transform_matrix);
        PositionChildren(c->collision_info);
        SetCollisionInfoChildsDoNothing(c->collision_info, 0);
    }

    i = 0;
    if (i < gNum_active_cars) {
        wp = (void**)gActive_car_list;
        do {
            float x;
            float fac;
            float f;

            car = (tCar_spec*)*wp;
            if (car->field_0x4c8 != gZero) {
                x = car->field_0x4c8;
                fac = (float)(gZero / x);
                f = car->collision_info->M; f *= fac; car->collision_info->M = f;
                f = car->collision_info->I.v[0]; f *= fac; car->collision_info->I.v[0] = f;
                f = car->collision_info->I.v[1]; f *= fac; car->collision_info->I.v[1] = f;
                f = car->collision_info->I.v[2]; f *= fac; car->collision_info->I.v[2] = f;
            }
            if (((tCar_spec*)*wp)->number_of_wheels_on_ground != 0) {
                SetCollisionInfoChildsDoNothing(((tCar_spec*)*wp)->collision_info, 0);
            }
            i++;
            wp++;
        } while (i < gNum_active_cars);
    }

    i = 0;
    if (i < gNum_cars_and_non_cars) {
        wp = (void**)gActive_car_list;
        do {
            car = (tCar_spec*)*wp;
            car->frame_collision_flag |= (int)(signed char)car->collision_info->collision_flag;
            i++;
            wp++;
        } while (i < gNum_cars_and_non_cars);
    }

    UpdateCrushTimers();
    if (gNet_mode) {
        UpdateCrushPainList();
    }
    APTCPostCollisionTree(gList_collision_infos);
}

// FUNCTION: CARMA2_HW 0x004161a0
void C2_HOOK_FASTCALL APTCPostCollisionTree(tPhysics_object* node) {
    tPhysics_object* child;
    void* owner;
    void* unk;

    while (node != NULL) {
        if (gNet_mode == eNet_mode_host) {
            int need = ((node->collision_flag & 2) != 0) || ((node->flags & 0x1000) != 0);

            for (child = node->child; child != NULL; child = child->next) {
                need |= child->collision_flag & 2;
            }
            if (need) {
                node->field_0x49c = gPHIL_last_physics_tick + 0x28;
                owner = node->owner;
                if (owner != NULL && node->flags_0x238 == 1 && *(int*)((char*)owner + 0xc) > 5) {
                    unk = *(void**)((char*)owner + 0x18d4);
                    if (unk != NULL) {
                        MarkNetworkShapeStatusesDirty(*(tCar_crush_network_shapes**)((char*)unk + 0x20));
                    }
                }
            }
        }
        node->disable_move_rotate &= 0xfd;
        APTCPostCollisionTree(node->child);
        node = node->next;
    }
}

// FUNCTION: CARMA2_HW 0x00416300
void C2_HOOK_FASTCALL APTCChangedObjects(tPhysics_object* pObject, undefined4 pArg2) {

    if (pObject != NULL && pObject->owner != NULL && pObject->flags_0x238 == 2) {
        BonerPedMovedByPhysics((tPed_character_instance*)pObject->owner, pArg2);
    }
    if (pObject != NULL && pObject->flags_0x238 == 0x10) {
        MoveMyDroneBaby(pObject, pArg2);
    }
}

// FUNCTION: CARMA2_HW 0x00416270
int C2_HOOK_FASTCALL APTCActiveHalted(tPhysics_object* pObject) {

    if (pObject != NULL && pObject->owner != NULL && pObject->flags_0x238 >= 2 && pObject->flags_0x238 <= 4) {
        return BonerActiveHalted((tPed_character_instance*)pObject->owner);
    }
    if (pObject != NULL && pObject->flags_0x238 == 0x10) {
        return MyDroneHathHalteth(pObject);
    }
    return 1;
}

// FUNCTION: CARMA2_HW 0x004162b0
int C2_HOOK_FASTCALL APTCPassiveActivated(tPhysics_object* pObject, tPhysics_object* pArg2) {

    if (pObject != NULL && pObject->owner != NULL && pObject->flags_0x238 >= 2 && pObject->flags_0x238 <= 4) {
        return BonerPassiveCollision((tPed_character_instance*)pObject->owner, pArg2);
    }
    if (pObject != NULL && pObject->flags_0x238 == 0x10) {
        return MyDroneHathCollideth(pObject, pArg2);
    }
    return 1;
}

void C2_HOOK_FASTCALL GetNonCars(void) {
    int i;
    int j;

    gNum_cars_and_non_cars = gNum_active_non_cars + gNum_active_cars;
    for (i = gNum_active_cars, j = 0; i < gNum_cars_and_non_cars; i++, j++) {
        gActive_car_list[i] = (tCar_spec*)gActive_non_car_list[j];
    }
}

// FUNCTION: CARMA2_HW 0x00416340
#ifdef __vita__
static void C2VitaPhysicsSpike(unsigned long long start, unsigned long long finish_start, tU32 pFrame_period);
#endif
void C2_HOOK_FASTCALL ApplyPhysicsToCars(tU32 pLast_tick_time, tU32 pFrame_period) {
    int i;
#ifdef __vita__
    extern unsigned long long C2VitaGLTicksNS(void);
    extern unsigned long long C2VitaPhysCollideNS, C2VitaPhysChangedNS, C2VitaModelUpdateNS;
    extern unsigned long long C2VitaPhysPreNS, C2VitaPhysMungeNS, C2VitaPhysPostNS, C2VitaPhysFlushNS;
    extern unsigned long long C2VitaPhysCarsNS, C2VitaPhysPhilNS;
    extern unsigned C2VitaPhysIterations, C2VitaModelUpdateCount;
    unsigned long long c2_start = C2VitaGLTicksNS(), c2_finish, c2_phil;
    C2VitaPhysCollideNS = C2VitaPhysChangedNS = C2VitaModelUpdateNS = 0;
    C2VitaPhysPreNS = C2VitaPhysMungeNS = C2VitaPhysPostNS = C2VitaPhysFlushNS = 0;
    {
        extern unsigned long long C2VitaPreFacesNS, C2VitaPreCarNS, C2VitaPreNonCarNS;
        extern unsigned C2VitaPreFacesCalls, C2VitaPreCarCalls, C2VitaPreNonCarCalls;
    C2VitaPreFacesNS = C2VitaPreCarNS = C2VitaPreNonCarNS = 0;
    C2VitaPreFacesCalls = C2VitaPreCarCalls = C2VitaPreNonCarCalls = 0;
    }
    C2VitaPhysIterations = C2VitaModelUpdateCount = 0;
#endif

    if (gFreeze_mechanics) {
        return;
    }
    if (gNet_mode == eNet_mode_client) {
        ForceRebuildActiveCarList();
    }
    GetNonCars();

    last_frame_start = pLast_tick_time;
    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        tCar_spec* car;

        car = gActive_car_list[i];
        car->frame_collision_flag = gOver_shoot && car->collision_info->collision_flag;
        if (car != NULL && car->driver > 5) {
            RecordLastDamage(car);
            if (car->driver == eDriver_oppo && gStop_opponents_moving) {
                car->acc_force = 0.f;
                car->brake_force = 0.f;
                car->keys.acc = 0;
                car->keys.dec = 0;
                car->joystick.acc = -1;
                car->joystick.dec = -1;
            }
            if (!car->wheel_slip) {
                StopSkid(car);
            }
            if (car->driver == eDriver_net_human && car->collision_info->message_time < pLast_tick_time - 1000) {
                car->keys.acc = 0;
                car->keys.dec = 0;
                car->joystick.acc = -1;
                car->joystick.dec = -1;
                car->keys.horn = 0;
            }
            SetSmokeLastDamageLevel(car);
        }
    }
#ifdef __vita__
    c2_phil = C2VitaGLTicksNS();
    C2VitaPhysCarsNS = c2_phil - c2_start;
#endif
    PHILDoPhysics(&gCar_physics_callbacks, pLast_tick_time, pFrame_period);
#ifdef __vita__
    C2VitaPhysPhilNS = C2VitaGLTicksNS() - c2_phil;
#endif
    if (TimeToSendData()) {
        SendCarData(gPHIL_last_physics_tick);
        SendMines(gPHIL_last_physics_tick);
    }
#ifdef __vita__
    c2_finish = C2VitaGLTicksNS();
#endif
    FinishCars(pLast_tick_time + pFrame_period, pFrame_period);
    CheckForDeAttachmentOfNonCars(pFrame_period);
#ifdef __vita__
    C2VitaPhysicsSpike(c2_start, c2_finish, pFrame_period);
#endif
}
#ifdef __vita__
/* One line per physics frame over 25 ms (rare): where the time went. */
unsigned long long C2VitaPhysCarsNS, C2VitaPhysPhilNS;
static void C2VitaPhysicsSpike(unsigned long long start, unsigned long long finish_start, tU32 pFrame_period) {
    extern unsigned long long C2VitaGLTicksNS(void);
    extern void C2VitaLog(const char* format, ...);
    extern unsigned long long C2VitaPhysCollideNS, C2VitaPhysChangedNS, C2VitaModelUpdateNS;
    extern unsigned C2VitaPhysIterations, C2VitaModelUpdateCount;
    extern unsigned long long C2VitaPhysPreNS, C2VitaPhysMungeNS, C2VitaPhysPostNS, C2VitaPhysFlushNS;
    static unsigned logged;
    unsigned long long end = C2VitaGLTicksNS();
    if (end - start < 25000000ull || logged >= 400) return;
    ++logged;
    extern unsigned long long C2VitaPreFacesNS, C2VitaPreCarNS, C2VitaPreNonCarNS;
    extern unsigned C2VitaPreFacesCalls, C2VitaPreCarCalls, C2VitaPreNonCarCalls;
    C2VitaLog("VITA PHYS PRE faces_us=%llu faces=%u carcollide_us=%llu carcollides=%u noncarcollide_us=%llu noncarcollides=%u active_noncars=%d\n",
        C2VitaPreFacesNS / 1000, C2VitaPreFacesCalls, C2VitaPreCarNS / 1000, C2VitaPreCarCalls,
        C2VitaPreNonCarNS / 1000, C2VitaPreNonCarCalls, gNum_active_non_cars);
    C2VitaLog("VITA PHYS SPIKE total_us=%llu ticks=%u cars_us=%llu phil_us=%llu pre_us=%llu munge_us=%llu collide_us=%llu post_us=%llu flush_us=%llu changed_us=%llu finish_us=%llu modelupdate_us=%llu modelupdates=%u frame_period=%u objects=%d\n",
        (end - start) / 1000, C2VitaPhysIterations, C2VitaPhysCarsNS / 1000, C2VitaPhysPhilNS / 1000,
        C2VitaPhysPreNS / 1000, C2VitaPhysMungeNS / 1000, C2VitaPhysCollideNS / 1000, C2VitaPhysPostNS / 1000,
        C2VitaPhysFlushNS / 1000, C2VitaPhysChangedNS / 1000,
        (end - finish_start) / 1000, C2VitaModelUpdateNS / 1000, C2VitaModelUpdateCount, (unsigned)pFrame_period,
        gNum_cars_and_non_cars);
}
#endif

// FUNCTION: CARMA2_HW 0x004170c0
void C2_HOOK_FASTCALL CycleControlCar(void) {
    gControl__car++;
    if (ControlCar[gControl__car] == NULL) {
        gControl__car = 0;
    }
    switch (gControl__car) {
    case 0:
        NewTextHeadupSlot(4, 0, 500, -1, "Original Controls");
        return;
    case 1:
        NewTextHeadupSlot(4, 0, 500, -1, "Accelerated steering");
        return;
    case 2:
        NewTextHeadupSlot(4, 0, 500, -1, "0.75 Accelerated");
        return;
    case 3:
        NewTextHeadupSlot(4, 0, 500, -1, "0.5 Accelerated");
        return;
    default:
        NewTextHeadupSlot(4, 0, 500, -1, "New controls");
        return;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00417fb0
void C2_HOOK_FASTCALL ApplyNonCarTumble(tPhysics_object* pObject, br_vector3* pV, br_scalar pTumbleFactor, br_scalar pTumbleThreshold) {
    br_vector3 cross;
    br_vector3 out;
    br_vector3 size;
    br_scalar len2;
    br_scalar speed;
    br_scalar over;
    br_scalar scale;
    br_scalar k;
    br_scalar nx;
    br_scalar ny;
    br_scalar nz;
    br_scalar new_len2;
    int i;

    if (pObject->collision_flag == 0 && pObject->disable_move_rotate == 0) {
        return;
    }

    len2 = BrVector3LengthSquared(pV);
    if (len2 < pTumbleThreshold * pTumbleThreshold) {
        return;
    }

    DRVector3SafeCross(&cross, &gFace_list__car[pObject->box_face_start].normal, pV);

    scale = BR_LENGTH3(cross.v[0], cross.v[1], cross.v[2]);
    if (scale > BR_SCALAR_EPSILON * 2) {
        scale = gZero / scale;
        cross.v[0] = cross.v[0] * scale;
        cross.v[1] = cross.v[1] * scale;
        cross.v[2] = cross.v[2] * scale;
    } else {
        cross.v[0] = 1.f;
        cross.v[1] = 0.f;
        cross.v[2] = 0.f;
    }

    BrMatrix34TApplyV(&out, &cross, &pObject->actor->t.t.mat);

    if (out.v[1] * pObject->omega.v[1] + out.v[2] * pObject->omega.v[2]
            + out.v[0] * pObject->omega.v[0] > 1.5) {
        return;
    }

    speed = sqrt(len2);
    BrVector3Sub(&size, &pObject->bb1.max, &pObject->bb1.min);
    over = speed - pTumbleThreshold;

    for (i = 0; i < 3; i++) {
        out.v[i] = out.v[i] * (0.5f * size.v[i]) / pObject->I.v[i];
    }

    k = 0.1f * pTumbleFactor * over * pObject->M;
    nx = out.v[0] * k + pObject->omega.v[0];
    ny = out.v[1] * k + pObject->omega.v[1];
    nz = out.v[2] * k + pObject->omega.v[2];

    new_len2 = nx * nx + ny * ny + nz * nz;
    pObject->omega.v[0] = nx;
    pObject->omega.v[1] = ny;
    pObject->omega.v[2] = nz;
    if (new_len2 > 400.f) {
        scale = sqrt(400.f / new_len2);
        pObject->omega.v[0] = nx * scale;
        pObject->omega.v[1] = ny * scale;
        pObject->omega.v[2] = nz * scale;
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00418230
void C2_HOOK_FASTCALL MakeLiftGoUp(tNon_car_spec* pNon_car) {

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNon_car_spec, flags, 0x100);

    if (gNet_mode != eNet_mode_client) {
        pNon_car->flags &= ~0xff;
        pNon_car->flags |= 0x1;
    }
}
// FUNCTION: CARMA2_HW 0x00413cc0
void C2_HOOK_FASTCALL DamageUnit(tCar_spec* pCar, tDamage_type pDamage_type, int pMagnitude) {
    tDamage_unit* damage_unit;

    if (pMagnitude > 0) {
        damage_unit = &pCar->damage_units[pDamage_type];
        damage_unit->damage_level += pMagnitude;
        if (damage_unit->damage_level >= 100) {
            damage_unit->damage_level = 99;
        }
    }
}

// SwitchCarModel

// FUNCTION: CARMA2_HW 0x00413f40
void C2_HOOK_FASTCALL SwitchCarModels(tCar_spec* pCar, int pIndex) {
#ifndef CARPOCALYPSE2_MATCHING
    DRActorEnumRecurse(pCar->car_model_actor, SwitchCarModel, &pIndex);
    pCar->field_0xe18 = pIndex;
#else
    DRActorEnumRecurse(pCar->car_model_actor, SwitchCarModel, &pIndex);
    pCar->field_0xe18 = pIndex;
#endif
}


// InitialiseCar2

// InitialiseCar

// InitialiseCarsEtc

// SetInitialPosition

// SetInitialPositions

// InitialiseNonCar

// NewFaceListCallBack

// IsCarInTheSea

// RememberSafePosition

// ControlNetCars

// ControlOurCar

// FUNCTION: CARMA2_HW 0x00415300
void C2_HOOK_FASTCALL CalcEngineForce(tCar_spec* pCar, br_scalar pDt) {
    br_scalar speed;
    br_scalar factor;
    tS32 tmp_joy;

    speed = -pCar->collision_info->velocity_car_space.v[2] * 6.9;
    pCar->acc_force = 0.f;
    if (pCar->revs == 0.f) {
        pCar->gear = 0;
    }

    if ((!pCar->keys.backwards && pCar->gear < 0) ||
        (pCar->gear == 0 && speed < -0.5 && !pCar->keys.backwards)) {
        pCar->keys.backwards = !pCar->keys.backwards;
        pCar->keys.acc ^= pCar->keys.dec;
        pCar->keys.dec ^= pCar->keys.acc;
        pCar->keys.acc ^= pCar->keys.dec;
        tmp_joy = pCar->joystick.acc;
        pCar->joystick.acc = pCar->joystick.dec;
        pCar->joystick.dec = tmp_joy;
    }
    if ((pCar->keys.backwards && pCar->gear > 0) ||
        (pCar->gear == 0 && speed > 0.5 && pCar->keys.backwards)) {
        pCar->keys.backwards = !pCar->keys.backwards;
        pCar->keys.acc ^= pCar->keys.dec;
        pCar->keys.dec ^= pCar->keys.acc;
        pCar->keys.acc ^= pCar->keys.dec;
        tmp_joy = pCar->joystick.acc;
        pCar->joystick.acc = pCar->joystick.dec;
        pCar->joystick.dec = tmp_joy;
    }
    if (pCar->gear == 0 && !pCar->keys.acc && pCar->joystick.acc <= 0 &&
        (pCar->keys.dec || pCar->joystick.dec > 0) && !pCar->keys.backwards &&
        fabsf(speed) < 1.f) {
        pCar->keys.backwards = 1;
        pCar->keys.acc = pCar->keys.dec;
        pCar->keys.dec = 0;
        tmp_joy = pCar->joystick.acc;
        pCar->joystick.acc = pCar->joystick.dec;
        pCar->joystick.dec = tmp_joy;
    }

    pCar->torque = pCar->revs * pCar->revs * -1e-08f - 0.2;
    if (pCar->keys.acc || pCar->joystick.acc >= 0) {
        if (fabsf(pCar->curvature) > pCar->maxcurve * 0.5f && pCar->gear < 2 &&
            pCar->gear != 0 && pCar->traction_control && pCar->joystick.acc < 0) {
            factor = 0.7f;
        } else if (pCar->joystick.acc >= 0) {
            factor = pCar->joystick.acc * (1.0 / 54613.0);
        } else {
            factor = 1.2f;
        }
        factor = factor * pCar->field_0x4d4;
        factor = factor * gPower_starting_value[pCar->power_up_levels[1]];
        if (pCar->damage_units[0].damage_level > 10) {
            factor = factor * (1.f - (pCar->damage_units[0].damage_level - 10) * 0.01f);
        }
        pCar->torque = pCar->torque + factor;
    } else {
        pCar->traction_control = 1;
    }

    if (pCar->keys.dec || (pCar->keys.acc && pCar->gear == 0) ||
        pCar->joystick.dec > 0 || (pCar->joystick.acc > 0 && pCar->gear == 0)) {
        if (pCar->joystick.dec > 0) {
            br_scalar bi = pCar->brake_increase;
            br_scalar q = (pCar->joystick.dec) / 65536;
            pCar->brake_force = pCar->initial_brake + bi * q;
        }
        if (pCar->brake_force == 0.f) {
            pCar->brake_force = pCar->initial_brake;
        } else {
            br_scalar bi = pCar->brake_increase;
            pCar->brake_force = pCar->brake_force + bi * pDt;
            if (pCar->brake_force > pCar->initial_brake + pCar->brake_increase) {
                pCar->brake_force = pCar->initial_brake + pCar->brake_increase;
            }
        }
    } else {
        pCar->brake_force = 0.f;
    }

    if (pCar->gear == 0) {
        return;
    }
    pCar->acc_force = pCar->force_torque_ratio * pCar->torque / (float)pCar->gear;
    if (pCar->brake_force != 0.f) {
        pCar->revs = pCar->target_revs;
    } else if (pCar->revs - 1.f > pCar->target_revs ||
               pCar->revs + 1.f < pCar->target_revs) {
        br_scalar lt = gZero / (pCar->speed_revs_ratio * pCar->collision_info->M) / (float)pCar->gear;
        pCar->acc_force = pCar->acc_force +
            (pCar->torque * pDt * 5000.0 + (pCar->revs - pCar->target_revs)) /
            ((lt +
              (float)pCar->gear * (gZero / (pCar->force_torque_ratio * 0.0002))) * pDt);
    }
}

// PrepareCars

// CalcGraphicalWheelStuff

// FinishCars

// GetNonCars

// GetNetPos

// MungeCarsMass

// AddDrag

// DragChildren

// DoCarStuff

// DoNonCarStuff

// RemoveFlyingCar

// RestoreFlyingCar

// GetDrivableOnList

// APTCPreCollision

// SetCollisionFlagsAndStuff

// APTCPostCollision

// APTCActiveHalted

// APTCPassiveActivated

// APTCChangedObjects

// ApplyPhysicsToCars

// MoveAndCollideCar

// MoveAndCollideNonCar

// ControlCar2

// ControlCar3

// ControlCar4

// ControlCar5

// ControlCar1

// SteeringSelfCentre

// NonCarSnapOff

// TestNonCarSnapOff

// TumbleObjectWithV

// TumbleObject

// MakeLiftGoUp

// NonCarCalcForce

// DoBumpiness

// SmashFacesWithWheels

// ConditionallyNoteSkid

// C2PORT-NONMATCHING: best 30.15% | difftest PASS 2000 trials (spec difftest/specs/0041bd40_ConditionallyNoteSkid.json) | reason x87-order and register allocation
// FUNCTION: CARMA2_HW 0x0041bd40
void C2_HOOK_FASTCALL ConditionallyNoteSkid(tCar_spec* pCar, tFace_ref** faces, int pIndex) {
    tFace_ref* face;
    br_material* material;
    br_vector3 local_pos;
    br_vector3* special_start;
    int mask;
    br_scalar plane_offset;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, wpos, 0x1224);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, oldd, 0x1264);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, new_skidding, 0x159c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, special_start, 0x15ac);

    face = faces[pIndex];
    if (face != NULL) {
        material = face->material;
        if (material != NULL) {
            if (material->colour_map != NULL && ((tU8*)material->colour_map)[0x2c] == 0x12) {
                return;
            }
            if (material->identifier != NULL
                    && strlen(material->identifier) == 11
                    && material->identifier[5] == '|') {
                return;
            }
        }
    }

    mask = 1 << pIndex;
    if ((mask & pCar->new_skidding) != 0) {
        return;
    }
    pCar->new_skidding |= mask;

    special_start = &pCar->special_start[pIndex];
    if ((pIndex & 1) != 0) {
        local_pos.v[0] = (pCar->wpos[pIndex].v[0] - gFloat_005897fc) * gFloat_005896d4;
    } else {
        local_pos.v[0] = (pCar->wpos[pIndex].v[0] - gFloat_00589800) * gFloat_005896d4;
    }
    local_pos.v[1] = (pCar->wpos[pIndex].v[1] - pCar->oldd[pIndex]) * gFloat_005896d4;
    local_pos.v[2] = pCar->wpos[pIndex].v[2] * gFloat_005896d4;
    BrMatrix34ApplyP(special_start, &local_pos, &pCar->collision_info->transform_matrix);

    face = faces[pIndex];
    plane_offset = (face->v[0].v[0] - special_start->v[0]) * face->normal.v[0];
    plane_offset += (face->v[0].v[1] - special_start->v[1]) * face->normal.v[1];
    plane_offset += (face->v[0].v[2] - special_start->v[2]) * face->normal.v[2];
    special_start->v[0] = face->normal.v[0] * plane_offset + special_start->v[0];
    special_start->v[1] = face->normal.v[1] * plane_offset + special_start->v[1];
    special_start->v[2] = face->normal.v[2] * plane_offset + special_start->v[2];
}

// NudgeObject

/* MSVC5 has no __declspec(noinline); keep CalcForce's helper calls out-of-line. */
#pragma inline_depth(0)

// FUNCTION: CARMA2_HW 0x00418850
void C2_HOOK_FASTCALL CalcForce(tCar_spec* pCar, br_scalar pDt) {
    int i;
    int j;
    int normnum;
    int vol_mod;
    int count;
    int best_index;
    int ffb_force;
    br_scalar force[4];
    br_scalar d[4];
    tFace_ref* collected[4];
    br_scalar dd[4];
    br_scalar rt[4];
    br_scalar val[4];
    br_scalar best;
    br_scalar wheelratio;
    br_scalar k;
    br_scalar ts;
    br_scalar ts2;
    br_scalar ts3;
    br_scalar ts4;
    br_scalar maxfl;
    br_scalar maxfr;
    br_scalar friction_number;
    br_scalar deltaomega;
    br_scalar lateral_speed;
    br_scalar yaw_inertia;
    br_vector3 velocity_car_space_ws;
    br_scalar v98;
    br_scalar v99;
    br_scalar v87;
    br_scalar v106;
    br_scalar v108;
    br_scalar v109;
    br_scalar v116;
    br_scalar v125;
    br_scalar v128;
    br_scalar v129;
    br_scalar v134;
    br_scalar v135;
    br_scalar pV;
    br_scalar fl_oil_factor;
    br_scalar fr_oil_factor;
    br_scalar rl_oil_factor;
    br_scalar rr_oil_factor;
    int delta;
    br_vector3 b;
    br_vector3 f;
    br_vector3 B;
    br_vector3 v;
    br_vector3 tv;
    br_vector3 tmp;
    br_vector3 ray_dir;
    br_vector3 normal;
    br_vector3 nor2;
    br_vector3 vplane;
    br_vector3 rightplane;
    br_vector3 a;
    br_vector3 v103;
    br_vector3 v136;
    br_vector3 v123;
    br_vector3 norm[4];
    br_vector3 ray_pos[4];
    br_vector3 contact[4];
    br_vector3 wheel_pos[4];
    br_bounds bounds;
    tFace_ref* faces[4];
    tPhysics_object* objs[4];
    tPhysics_object* pObj;
    tPhysics_object** pList;
    tFace_ref* pFace;
    br_matrix34* mat;
    tSpecial_volume* vol;
    tMaterial_modifiers* mat_list;
    br_scalar* pd;
    static br_scalar stop_timer;
    static br_scalar slide_dist;

    B.v[0] = 0.f;
    B.v[1] = 0.f;
    B.v[2] = 0.f;
    pCar->curvature = pCar->curvature + pCar->field_0x1260;
    pCar->field_0x195c = 0;
    vol = pCar->collision_info->last_special_volume;
    wheelratio = (pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2])
        / (pCar->wpos[0].v[2] - pCar->centre_of_mass_world_scale.v[2]);
    faces[0] = NULL;
    faces[1] = NULL;
    faces[2] = NULL;
    faces[3] = NULL;
    normnum = 0;
    objs[0] = NULL;
    objs[1] = NULL;
    objs[2] = NULL;
    objs[3] = NULL;
    vol_mod = 0;
    f.v[0] = 0.f;
    f.v[1] = 0.f;
    f.v[2] = 0.f;
    mat = &pCar->car_master_actor->t.t.mat;
    b.v[0] = -mat->m[1][0];
    b.v[1] = -mat->m[1][1];
    b.v[2] = -mat->m[1][2];
    v.v[0] = pCar->collision_info->v.v[0] * 6.9;
    v.v[1] = pCar->collision_info->v.v[1] * 6.9;
    v.v[2] = pCar->collision_info->v.v[2] * 6.9;
    pCar->road_normal.v[0] = 0.f;
    pCar->road_normal.v[1] = 0.f;
    pCar->road_normal.v[2] = 0.f;
    for (i = 0; i < 4; i++) {
        BrMatrix34ApplyP(&wheel_pos[i], &pCar->wpos[i], mat);
    }
    k = (pCar->susp_height[0] < pCar->susp_height[1] ? pCar->susp_height[0] : pCar->susp_height[1]) - (-0.5);
    d[0] = 2.f;
    d[1] = 2.f;
    BrVector3Scale(&tmp, &b, k);
    d[2] = 2.f;
    d[3] = 2.f;
    for (i = 0; i < 4; i++) {
        BrVector3InvScale(&ray_pos[i], &wheel_pos[i], WORLD_SCALE);
    }
    BrVector3InvScale(&ray_dir, &tmp, WORLD_SCALE);
    for (i = pCar->collision_info->box_face_start, pFace = &gFace_list__car[i];
            i < pCar->collision_info->box_face_end;
            i++, pFace++) {
        if (gUNK_006793d0 != 0 && (pFace->flags & 0x80) != 0) {
            continue;
        }
        MultiRayCheckSingleFace(4, pFace, ray_pos, &ray_dir, &normal, rt);
        for (j = 0; j < 4; j++) {
            if (rt[j] < d[j]) {
                faces[j] = pFace;
                d[j] = rt[j];
                BrVector3Copy(&pCar->nor[j], &normal);
            }
        }
    }
    bounds.max = ray_pos[0];
    bounds.min = ray_pos[0];
    for (i = 1; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (ray_pos[i].v[j] < bounds.min.v[j]) {
                bounds.min.v[j] = ray_pos[i].v[j];
            }
            if (bounds.max.v[j] < ray_pos[i].v[j]) {
                bounds.max.v[j] = ray_pos[i].v[j];
            }
        }
    }
    for (j = 0; j < 3; j++) {
        if (ray_dir.v[j] < 0.f) {
            bounds.min.v[j] = bounds.min.v[j] + ray_dir.v[j];
        } else {
            bounds.max.v[j] = bounds.max.v[j] + ray_dir.v[j];
        }
    }
    for (i = 0, pList = gDrivable_on_list; i < (int)gDrivable_on_count; i++, pList++) {
        pObj = *pList;
        if (!BoundsOverlapTest__finteray(&bounds, &pObj->field_0xf4)) {
            continue;
        }
        BrMatrix34TApplyV(&ray_dir, &ray_dir, &pObj->transform_matrix);
        if (pObj->shape == NULL) {
            continue;
        }
        for (;;) {
            BrMatrix34ApplyP(&pObj->cmpos, &pObj->cmpos, &pObj->transform_matrix);
            for (j = 0; j < 4; j++) {
                DrMatrix34ApplyLPInverse(&tmp, &ray_pos[j], &pObj->transform_matrix);
                ShapeRayCast(&tmp, &ray_dir, (const tPhysics_shape*)pObj->shape, &pObj->cmpos, &ts, &normal);
                if (ts < d[j]) {
                    normal.v[0] = -normal.v[0];
                    normal.v[1] = -normal.v[1];
                    normal.v[2] = -normal.v[2];
                    BrMatrix34ApplyV(&normal, &normal, &pObj->transform_matrix);
                    d[j] = ts;
                    faces[j] = NULL;
                    objs[j] = pObj;
                }
            }
            if (pObj->next == NULL) {
                break;
            }
            pObj = (tPhysics_object*)pObj->next;
        }
    }
    if (pCar->collision_info->last_special_volume != NULL
        && pCar->collision_info->last_special_volume->material_modifier_index != 0) {
        vol_mod = pCar->collision_info->last_special_volume->material_modifier_index;
    }
    mat_list = gFriction_materials;
    for (i = 0, j = 0; j < 0x30; i++, j += 0xc) {
        pd = &d[i];
        *pd = *pd * k;
        ts = *pd;
        if (faces[i] != NULL
                && faces[i]->material != NULL
                && faces[i]->material->identifier != NULL) {
            pCar->material_index[i] = vol_mod != 0
                ? vol_mod
                : (signed char)faces[i]->material->identifier[0] - '/';
            if (pCar->material_index[i] < 0 || pCar->material_index[i] > 0xb) {
                pCar->material_index[i] = 0;
            }
            if (strlen(faces[i]->material->identifier) == 0xb
                    && faces[i]->material->identifier[5] == '|') {
                pCar->oil_remaining[i] = 0.f;
            }
        } else {
            pCar->material_index[i] = 0;
        }
        BrMatrix34TApplyV(&norm[i], &pCar->nor[i], mat);
        if (mat_list[pCar->material_index[i]].bumpiness != 0.f) {
            br_scalar tv0 = pCar->nor[i].v[0] * *pd;
            br_scalar tv2 = pCar->nor[i].v[2] * *pd;
            {
                int x = abs((int)(512.f * (tv0 + wheel_pos[i].v[0])));
                int y = abs((int)(512.f * (wheel_pos[i].v[2] + tv2)));
                x = x % 2048;
                y = y % 2048;
                if (x > 1024) {
                    x = 2048 - x;
                }
                if (y > 1024) {
                    y = 2048 - y;
                }
                if (x + y > 1024) {
                    delta = 2048 - x - y;
                } else {
                    delta = x + y;
                }
                delta = delta - 400;
                if (delta < 0) {
                    delta = 0;
                }
            }
            d[i] = delta * mat_list[pCar->material_index[i]].bumpiness / 42400.f
                * norm[i].v[1] + *pd;
        }
        if (*pd < -0.5f
                || pCar->wheel_dam_offset[i ^ 2] * 6.9 + pCar->susp_height[i / 2] < *pd) {
            force[i] = 0.f;
            d[i] = pCar->susp_height[i / 2];
        } else {
            BrVector3Accumulate(&pCar->road_normal, &norm[i]);
            normnum++;
            d[i] = d[i] - pCar->wheel_dam_offset[i ^ 2] * 6.9;
            force[i] = (pCar->susp_height[i / 2] - d[i]) * pCar->sk[i / 2];
            force[i] = force[i] - (d[i] - pCar->oldd[i]) / pDt * pCar->sb[i / 2];
            if (pCar->susp_height[i / 2] == pCar->oldd[i]
                && pCar->nor[i].v[0] * v.v[0] + pCar->nor[i].v[1] * v.v[1] + pCar->nor[i].v[2] * v.v[2] > -0.01f
                && pCar->collision_info->M * 5.f < force[i]) {
                d[i] = pCar->susp_height[i / 2];
                force[i] = pCar->collision_info->M * 5.f;
            }
            if (force[i] < 0.f) {
                force[i] = 0.f;
            }
            B.v[1] = force[i] + B.v[1];
            f.v[0] = f.v[0] - (pCar->wpos[i].v[2] - pCar->centre_of_mass_world_scale.v[2]) * force[i];
            f.v[2] = (pCar->wpos[i].v[0] - pCar->centre_of_mass_world_scale.v[0]) * force[i] + f.v[2];
        }
        dd[i] = pCar->oldd[i] - d[i];
        pCar->oldd[i] = d[i];
        if (objs[i] == NULL || force[i] == 0.f) {
            continue;
        }
        pCar->field_0x195c = (unsigned int)objs[i];
        if (objs[i]->disable_move_rotate != 0
                && objs[i]->owner != NULL
                && objs[i]->flags_0x238 == 1
                && ((tNon_car_spec*)objs[i]->owner)->driver <= 5
                && (((tNon_car_spec*)objs[i]->owner)->flags & 0x10000) != 0) {
            continue;
        }
        objs[i]->disable_move_rotate = 0;
        ts = pCar->collision_info->M * pDt / (force[i] / 6.9);
        tmp.v[0] = ts * b.v[0];
        tmp.v[1] = ts * b.v[1];
        tmp.v[2] = ts * b.v[2];
        objs[i]->v.v[0] = tmp.v[0] + objs[i]->v.v[0];
        objs[i]->v.v[1] = tmp.v[1] + objs[i]->v.v[1];
        objs[i]->v.v[2] = tmp.v[2] + objs[i]->v.v[2];
        if (gUNK_006793d0 == 2) {
            pCar->collision_info->field_0x49c = (int)&gZero;
        }
    }
    if (objs[0] != NULL
            && objs[0] == objs[1]
            && objs[0] == objs[2]
            && objs[0] == objs[3]
            && objs[0]->owner != NULL
            && objs[0]->flags_0x238 == 1
            && ((tNon_car_spec*)objs[0]->owner)->driver <= 5
            && (((tNon_car_spec*)objs[0]->owner)->flags & 0x10000) != 0
            && (((tNon_car_spec*)objs[0]->owner)->flags & 0x40000) != 0
            && (((tNon_car_spec*)objs[0]->owner)->flags & 0xff) == 0
            && (((tNon_car_spec*)objs[0]->owner)->flags & 0xff00) == 0
            && ((tNon_car_spec*)objs[0]->owner)->field_0xf8 == 0
            && gUNK_006793d0 != 3) {
        ((tNon_car_spec*)objs[0]->owner)->flags = (((tNon_car_spec*)objs[0]->owner)->flags & 1) | 1;
    }
    if (pCar->collision_info->disable_move_rotate != 0 && normnum == 0) {
        goto steer_centre;
    }
    if (pCar->collision_info->disable_move_rotate != 0) {
        SetCollisionInfoDoNothingChain(pCar->collision_info, 0);
    }
    if (pCar == NULL || pCar->driver <= 5 || pCar->wall_climber_mode == 0
            || (pCar->road_normal.v[0] == 0.f && pCar->road_normal.v[1] == 0.f && pCar->road_normal.v[2] == 0.f)) {
        friction_number = pCar->collision_info->M * 10.f;
        if (vol != NULL) {
            friction_number = (1.f - vol->gravity_multiplier) * pCar->collision_info->water_depth_factor;
            if (pCar->underwater_ability != 0) {
                friction_number = friction_number * 0.6f;
            }
            friction_number = (1.f - friction_number) * pCar->collision_info->M;
        } else {
            friction_number = pCar->collision_info->M;
        }
        friction_number = friction_number * gGravity_multiplier * 10.f;
        /* retail 0x41bcbd..0x41bcea: the per-step gravity velocity the physics solver adds (field_0x54);
         * it follows water depth and the gravity multiplier */
        BrVector3SetFloat(&pCar->collision_info->field_0x54, 0.f, -(friction_number / pCar->collision_info->M * pDt) * (1.f / WORLD_SCALE), 0.f);
        B.v[0] = B.v[0] - mat->m[0][1] * friction_number;
        B.v[1] = B.v[1] - mat->m[1][1] * friction_number;
        B.v[2] = B.v[2] - mat->m[2][1] * friction_number;
    } else {
        BrVector3Normalise(&tmp, &pCar->road_normal);
        BrVector3Scale(&tmp, &tmp, -(pCar->collision_info->M * 10.0f));
        BrVector3Accumulate(&B, &tmp);
    }
steer_centre:
    if (pCar != NULL && pCar->driver >= 7) {
        if (pCar->curvature > pCar->maxcurve) {
            pCar->curvature = pCar->maxcurve;
        }
        if (pCar->curvature < -pCar->maxcurve) {
            pCar->curvature = -pCar->maxcurve;
        }
        if (!(pCar->keys.left || pCar->keys.right || pCar->keys.holdw)
            && (gUNK_0074cf98 != 0
                || (pCar->joystick.left <= 0 && pCar->joystick.right <= 0))
            && (pCar->oldd[2] < pCar->susp_height[1] || pCar->oldd[3] < pCar->susp_height[1])) {
            ts = -((pCar->collision_info->omega.v[2] * pCar->road_normal.v[2]
                        + pCar->collision_info->omega.v[1] * pCar->road_normal.v[1]
                        + pCar->collision_info->omega.v[0] * pCar->road_normal.v[0])
                * (pDt / (pCar->wpos[0].v[2] - pCar->wpos[2].v[2])));
            ts2 = -(pCar->curvature * pDt);
            /* retail 0x4197c0..0x4197f4: the curvature decay ts2 is used only when it is at least as
             * large as the yaw-based correction ts and has the same sign */
            if (fabs(ts2) >= fabs(ts) && ts * ts2 >= 0.f) {
                ts = ts2;
            }
            if (gKey_mapping[47] < 143 || gKey_mapping[48] < 143) {
                pCar->curvature += ts;
                if (pCar->curvature * ts > 0.f) {
                    pCar->curvature = 0.f;
                }
            }
            ffb_force = (int)(ts * 240000.f);
        } else {
            ffb_force = 0;
        }
        if (pCar->driver == eDriver_local_human) {
            SetJoystickFFBSteeringForce(ffb_force - (int)((dd[2] - dd[3]) * -30000.f));
        }
    }
    if (normnum != 0) {
        count = 0;
        best_index = -1;
        best = 0.f;
        for (i = 0; i < 4; i++) {
            pFace = faces[i];
            if (pFace == NULL || pFace->material == NULL
                    || pFace->material->identifier == NULL) {
                continue;
            }
            if (strlen(pFace->material->identifier) != 0xb
                    || pFace->material->identifier[5] != '|') {
                continue;
            }
            for (j = 0; j < count; j++) {
                if (collected[j] == pFace) {
                    break;
                }
            }
            if (j != count) {
                continue;
            }
            contact[count].v[0] = -pCar->oldd[i] * mat->m[1][0];
            contact[count].v[1] = -pCar->oldd[i] * mat->m[1][1];
            contact[count].v[2] = -pCar->oldd[i] * mat->m[1][2];
            val[count] = force[i] * 0.04f;
            contact[count].v[0] += wheel_pos[i].v[0];
            contact[count].v[1] += wheel_pos[i].v[1];
            contact[count].v[2] += wheel_pos[i].v[2];
            if (val[count] > best) {
                best = val[count];
                best_index = count;
            }
            collected[count] = pFace;
            count++;
        }
        if (best_index >= 0) {
            BrVector3InvScale(&contact[best_index], &contact[best_index], WORLD_SCALE);
            tv.v[0] = -val[best_index] * mat->m[1][0];
            tv.v[1] = -val[best_index] * mat->m[1][1];
            tv.v[2] = -val[best_index] * mat->m[1][2];
            SmashEnvironment(pCar->collision_info,
                (undefined4*)collected[best_index],
                val[best_index], &contact[best_index],
                &pCar->collision_info->v, &tv, 0, 0);
        }
        FlushSmashQueue(1);
        BrVector3NormaliseQuick(&pCar->road_normal, &pCar->road_normal);
        /* retail 0x419b16..0x419bae: from here on CalcForce works with the car-space velocity in
         * world-scale units (collision_info->velocity_car_space * 6.9, esp+0xb0) */
        BrVector3Scale(&velocity_car_space_ws, &pCar->collision_info->velocity_car_space, WORLD_SCALE);
        friction_number = pCar->road_normal.v[1] * mat->m[1][1]
            + pCar->road_normal.v[2] * mat->m[2][1]
            + pCar->road_normal.v[0] * mat->m[0][1];
        if (pCar->driver > 5 && pCar->wall_climber_mode != 0) {
            friction_number = 1.f;
        }
        friction_number = mat_list[pCar->material_index[0]].down_force * friction_number;
        if (friction_number > 0.f) {
            friction_number = fabs(velocity_car_space_ws.v[2])
                * pCar->collision_info->M * 10.0 * friction_number / pCar->downforce_to_weight;
            if (pCar->collision_info->M * 10.0 < friction_number) {
                friction_number = pCar->collision_info->M * 10.0;
            }
            if (pCar->number_of_wheels_on_ground == 4
                && pCar->oldd[2] == pCar->susp_height[1]
                && pCar->oldd[3] == pCar->susp_height[1]) {
                br_vector3 pos;
                br_vector3 delta;
                br_material* material;

                pos.v[0] = (mat->m[3][0] + mat->m[2][0] * pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                pos.v[1] = (mat->m[3][1] + mat->m[2][1] * pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                pos.v[2] = (mat->m[3][2] + mat->m[2][2] * pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                delta.v[0] = b.v[0] * (pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                delta.v[1] = b.v[1] * (pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                delta.v[2] = b.v[2] * (pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * (1.f / WORLD_SCALE);
                ActorFindFace(&pos, &delta, gTrack_actor, &nor2, &ts, &material, NULL);
                if (ts > 1.f) {
                    pCar->field_0x12a8 = 1;
                }
            }
            if (pCar->field_0x12a8 != 0
                && (pCar->oldd[2] < pCar->susp_height[1] || pCar->oldd[3] < pCar->susp_height[1])) {
                pCar->field_0x12a8 = 0;
            }
            if (pCar->field_0x12a8 != 0) {
                friction_number = friction_number
                    * ((pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2])
                        / (pCar->wpos[2].v[2] - pCar->wpos[0].v[2]));
                f.v[0] = f.v[0]
                    + (pCar->wpos[0].v[2] - pCar->centre_of_mass_world_scale.v[2]) * friction_number;
            }
            B.v[1] = B.v[1] - friction_number;
        }
        ts2 = BrVector3Dot(&velocity_car_space_ws, &pCar->road_normal);
        vplane.v[0] = velocity_car_space_ws.v[0] - ts2 * pCar->road_normal.v[0];
        vplane.v[1] = velocity_car_space_ws.v[1] - ts2 * pCar->road_normal.v[1];
        vplane.v[2] = velocity_car_space_ws.v[2] - ts2 * pCar->road_normal.v[2];
        if (vplane.v[2] < 0.f) {
            ts = 1.f;
        } else {
            ts = -1.f;
        }
        ts2 = BrVector3Length(&vplane);
        deltaomega = ts2 * pCar->curvature * ts;
        deltaomega = deltaomega - BrVector3Dot(&pCar->collision_info->omega, &pCar->road_normal);
        BrVector3Set(&v103, pCar->road_normal.v[1], -pCar->road_normal.v[0], 0.f);
        BrVector3Normalise(&v103, &v103);
        /* retail 0x41a02a..0x41a053: C2 keeps the yaw inertia in physics units; CalcForce scales it by
         * 47.61 (= WORLD_SCALE^2) and keeps the product (esp+0x40) for the grip-limit block below */
        yaw_inertia = pCar->collision_info->I.v[1] * 47.61f;
        friction_number = yaw_inertia / pDt * deltaomega;
        ts = friction_number / (pCar->wpos[2].v[2] - pCar->wpos[0].v[2]);
        v108 = ts;
        v109 = -ts;
        BrVector3Set(&rightplane, 0.f, pCar->road_normal.v[2], -pCar->road_normal.v[1]);
        BrVector3Normalise(&rightplane, &rightplane);
        v99 = pCar->acc_force;
        friction_number = BrVector3Dot(&rightplane, &vplane);
        /* retail keeps this dot product in its own slot (esp+0xbc); v108 (esp+0x44) still holds the
         * yaw correction ts from above, which the friction force is added to below */
        lateral_speed = BrVector3Dot(&v103, &vplane);
        ts2 = fabs(lateral_speed);
        friction_number = (pCar->wpos[0].v[2] - pCar->centre_of_mass_world_scale.v[2]) * friction_number * fabs(pCar->curvature);
        if (pCar->curvature <= 0.f) {
            friction_number = lateral_speed - friction_number;
        } else {
            friction_number = lateral_speed + friction_number;
        }
        friction_number = -(pCar->collision_info->M / pDt * friction_number);
        friction_number = friction_number - BrVector3Dot(&B, &v103);
        friction_number = friction_number / (1.0 - wheelratio);
        v108 = friction_number + v108;
        v109 = -wheelratio * friction_number + v109;
        friction_number = (pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * v108;
        v98 = friction_number * pCar->curvature;
        friction_number = BrVector3Dot(&velocity_car_space_ws, &rightplane) * pCar->collision_info->M / pDt;
        v129 = BrVector3Dot(&rightplane, &B) + friction_number;
        v128 = pCar->mu.v[0] * pCar->brake_force / (pCar->mu.v[1] / pCar->traction_multiplier + pCar->mu.v[0]);
        v125 = pCar->brake_force - v128;
        ts = (pCar->damage_units[7].damage_level + pCar->damage_units[6].damage_level) / 2;
        if (ts > 20.f) {
            v128 = (1.f - (ts - 20.f) / 80.f) * (1.f - (ts - 20.f) / 80.f) * v128;
        }
        ts = (pCar->damage_units[5].damage_level + pCar->damage_units[4].damage_level) / 2;
        if (ts > 20.f) {
            v125 = (1.f - (ts - 20.f) / 80.f) * (1.f - (ts - 20.f) / 80.f) * v125;
        }
        ts2 = (force[1] + force[0]) * pCar->rolling_resistance + v128;
        v106 = (force[2] + force[3]) * pCar->steerable_rolling_resistance + v125;
        v128 = pCar->wpos[0].v[2] - pCar->wpos[2].v[2];
        v128 = sqrt(v128 * v128 * pCar->curvature * pCar->curvature + 1.0);
        v106 = v106 / v128;
        v134 = v106 + ts2;
        if (fabs(v129) < fabs(v134)) {
            ts2 = v129 / v134 * ts2;
            v106 = v129 / v134 * v106;
        }
        if ((v106 + ts2) * v129 < 0.f) {
            ts2 = -ts2;
            v106 = -v106;
        }
        v129 = v129 - (ts2 + v106);
        v99 = v99 - ts2;
        if (pCar->keys.brake
                && pCar->damage_units[7].damage_level < 60
                && pCar->damage_units[6].damage_level < 60) {
            v99 = v99 - v129;
            pCar->gear = 0;
        }
        v99 = v99 / pCar->traction_multiplier;
        v135 = sqrt(v99 * v99 + v109 * v109) / 2.0;
        GetOilFrictionFactors(pCar, &fl_oil_factor, &fr_oil_factor, &rl_oil_factor, &rr_oil_factor);
        for (i = 0; i < 4; i++) {
            if (pCar->oil_remaining[i] != 0.f) {
                ConditionallyNoteSkid(pCar, faces, i);
            }
        }
        if (pCar->driver <= 5) {
            v116 = 1.f;
        } else {
            v116 = pCar->grip_multiplier;
        }
        BrVector3Sub(&a, &pCar->wpos[0], &pCar->centre_of_mass_world_scale);
        BrVector3Cross(&a, &pCar->collision_info->omega, &a);
        BrVector3Accumulate(&a, &velocity_car_space_ws);
        if (pCar != NULL && pCar->driver >= eDriver_net_human
            && (((pCar->keys.left || pCar->joystick.left > 0x8000) && pCar->curvature > 0.f && deltaomega > 0.1 && a.v[0] > 0.f)
                || ((pCar->keys.right || pCar->joystick.right > 0x8000) && pCar->curvature < 0.f && deltaomega < 0.1 && a.v[0] < 0.f))
            && ts > 0.f) {
            friction_number = pCar->mu.v[0];
        } else {
            friction_number = pCar->mu.v[2];
            ts2 = fabs(a.v[0]) / 10.f;
            if (ts2 > 1.f) {
                ts2 = 1.f;
            }
            friction_number = (pCar->mu.v[2] - pCar->mu.v[0]) * ts2 + friction_number;
        }
        maxfl = sqrt(force[0]) * friction_number * (rl_oil_factor * v116) * mat_list[pCar->material_index[0]].tyre_road_friction;
        maxfr = sqrt(force[1]) * friction_number * (rr_oil_factor * v116) * mat_list[pCar->material_index[1]].tyre_road_friction;
        pCar->max_force_rear = maxfr + maxfl;
        if (rl_oil_factor == 1.f && rr_oil_factor == 1.f && pCar->traction_control
                && v135 * 2.f > pCar->max_force_rear && pCar->acc_force > 0.f
                && (pCar == NULL || pCar->driver < eDriver_net_human
                    || (pCar->target_revs > 1000.f && pCar->gear > 0))) {
            ts2 = v99;
            if (v99 * v99 <= v135 * v135 * 4.f) {
                v87 = sqrt(v135 * v135 * 4.f - v99 * v99);
            } else {
                v87 = 0.f;
            }
            if (pCar->max_force_rear <= v87) {
                pCar->torque = -(pCar->revs * pCar->revs / 100000000.f) - 0.1f;
            } else {
                float v177 = sqrt(pCar->max_force_rear * pCar->max_force_rear - v87 * v87);
                ts3 = ts2 < 0.f ? -1.f : 1.f;
                ts4 = (ts2 - ts3 * v177) * 0.99f;
                if (fabs(ts2) > fabs(ts4)) {
                    ts2 = ts4;
                }
            }
            v99 = v99 - ts2;
            v135 = sqrt(v99 * v99 + v109 * v109) / 2.f;
        } else if (pCar != NULL && pCar->driver >= eDriver_net_human && pCar->gear > 0
                && pCar->revs > pCar->target_revs && !pCar->traction_control) {
            if (!pCar->keys.change_down) {
                pCar->traction_control = 1;
            }
            friction_number = 1.f - (pCar->revs - pCar->target_revs) / (float)(400 * pCar->gear);
            if (friction_number < 0.4f) {
                friction_number = 0.4f;
            }
            maxfl = friction_number * maxfl;
            maxfr = friction_number * maxfr;
        }
        if (fabs(v109) > maxfr + maxfl && maxfr + maxfl > 0.1f) {
            /* retail 0x41aa36..0x41abb7: scale the rear forces down to the grip limit, rebalance the
             * front lateral force so the yaw/lateral motion stays consistent, then undo the scale.
             * The scale lives in its own temporary; v106 (front rolling/brake force) is used below. */
            v87 = (maxfr + maxfl) / fabs(v109) * pDt;
            v109 = v87 * v109;
            v99 = pCar->traction_multiplier * v87 * v99;
            friction_number = -((pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2]) * v108
                    * ((pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2]) * v108) / yaw_inertia
                + (v98 * v98 + v108 * v108) / pCar->collision_info->M);
            ts2 = (BrVector3Dot(&v103, &vplane) + v109 / pCar->collision_info->M) * v108;
            ts2 = BrVector3Dot(&rightplane, &vplane) * v98 + ts2;
            ts2 = BrVector3Dot(&pCar->collision_info->omega, &pCar->road_normal) * (pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2]) * v108 + ts2;
            ts2 = (pCar->wpos[0].v[2] - pCar->centre_of_mass_world_scale.v[2]) * (pCar->wpos[2].v[2] - pCar->centre_of_mass_world_scale.v[2])
                * v109 / yaw_inertia * v108 + ts2;
            if (fabs(friction_number) > 0.1) {
                friction_number = ts2 / (friction_number * pDt);
                v108 = friction_number * v108;
                v98 = friction_number * v98;
            }
            v109 = v109 / v87;
            v99 = v99 / (pCar->traction_multiplier * v87);
        }
        v98 = v98 - v106;
        v108 = (pCar->wpos[0].v[2] - pCar->wpos[2].v[2]) * pCar->curvature * v106 + v108;
        if (v135 > 0.0001f) {
            v109 = v109 / (v135 * 2.f);
            v99 = v99 / (v135 * 2.0);
        }
        v99 = pCar->traction_multiplier * v99;
        force[0] = v135;
        force[1] = v135;
        pCar->wheel_slip = 0;
        switch ((force[0] > maxfl) + 2 * (force[1] > maxfr)) {
        case 0:
            slide_dist = 0.f;
            break;
        case 1:
            force[0] = pCar->friction_slipping_reduction * maxfl;
            force[1] = v135 - force[0] + force[1];
            if (force[1] <= maxfr) {
                slide_dist = 0.f;
            } else {
                if (maxfr > 0.1f) {
                    pV = (force[1] - maxfr) / maxfr;
                    if (&gProgram_state.current_car == pCar) {
                        ts2 = 20.f;
                    } else {
                        ts2 = 60.f;
                    }
                    if (ts2 <= pV) {
                        ConditionallyNoteSkid(pCar, faces, 1);
                    }
                    SkidNoise(pCar, 1, pV, pCar->material_index[1]);
                }
                force[1] = pCar->friction_slipping_reduction * maxfr;
                pCar->wheel_slip |= 2;
            }
            break;
        case 2:
            force[1] = pCar->friction_slipping_reduction * maxfr;
            force[0] = v135 - force[1] + force[0];
            if (force[0] <= maxfl) {
                slide_dist = 0.f;
            } else {
                if (maxfl > 0.1f) {
                    pV = (force[0] - maxfl) / maxfl;
                    if (&gProgram_state.current_car == pCar) {
                        ts2 = 20.f;
                    } else {
                        ts2 = 60.f;
                    }
                    if (ts2 <= pV) {
                        ConditionallyNoteSkid(pCar, faces, 0);
                    }
                    SkidNoise(pCar, 0, pV, pCar->material_index[0]);
                }
                force[0] = pCar->friction_slipping_reduction * maxfl;
                pCar->wheel_slip |= 2;
            }
            break;
        case 3:
            force[0] = pCar->friction_slipping_reduction * maxfl;
            force[1] = pCar->friction_slipping_reduction * maxfr;
            pCar->wheel_slip |= 2;
            pV = (v135 * 2.0 - maxfl - maxfr) / (maxfr + maxfl);
            if (&gProgram_state.current_car == pCar) {
                ts2 = 20.f;
            } else {
                ts2 = 60.f;
            }
            if (ts2 <= pV) {
                if (maxfl > 0.1f) {
                    ConditionallyNoteSkid(pCar, faces, 0);
                }
                if (maxfr > 0.1f) {
                    ConditionallyNoteSkid(pCar, faces, 1);
                }
            }
            if (IRandomBetween(0, 1)) {
                if (maxfl > 0.1f) {
                    SkidNoise(pCar, 0, pV, pCar->material_index[0]);
                }
            } else {
                if (maxfr > 0.1f) {
                    SkidNoise(pCar, 1, pV, pCar->material_index[1]);
                }
            }
            break;
        }
        v135 = sqrt(v108 * v108 + v98 * v98) / 2.0;
        if (v135 > 0.0001f) {
            v108 = v108 / (v135 * 2.0);
            v98 = v98 / (v135 * 2.0);
        }
        maxfl = sqrt(force[2]) * pCar->mu.v[1] * (fl_oil_factor * v116) * mat_list[pCar->material_index[2]].tyre_road_friction;
        maxfr = sqrt(force[3]) * pCar->mu.v[1] * (fr_oil_factor * v116) * mat_list[pCar->material_index[3]].tyre_road_friction;
        if (pCar != NULL && pCar->driver == eDriver_local_human) {
            SetJoystickFFBForce((int)(225.0f * (maxfl * force[2] + maxfr * force[3]) / (1.0f + 750.0f * pCar->speed)));
        }
        pCar->max_force_front = maxfr + maxfl;
        force[2] = v135;
        force[3] = v135;
        switch ((v135 > maxfl) + 2 * (v135 > maxfr)) {
        case 1:
            force[2] = pCar->friction_slipping_reduction * maxfl;
            force[3] = v135 - force[2] + force[3];
            if (force[3] > maxfr) {
                if (maxfr > 0.1f) {
                    pV = (force[3] - maxfr) / maxfr;
                    if (&gProgram_state.current_car == pCar) {
                        ts2 = 20.f;
                    } else {
                        ts2 = 60.f;
                    }
                    if (ts2 <= pV) {
                        ConditionallyNoteSkid(pCar, faces, 3);
                    }
                    SkidNoise(pCar, 3, pV, pCar->material_index[3]);
                }
                force[3] = pCar->friction_slipping_reduction * maxfr;
                pCar->wheel_slip |= 1;
            }
            break;
        case 2:
            force[3] = pCar->friction_slipping_reduction * maxfr;
            force[2] = v135 - force[3] + force[2];
            if (force[2] > maxfl) {
                if (maxfl > 0.1f) {
                    pV = (force[2] - maxfl) / maxfl;
                    if (&gProgram_state.current_car == pCar) {
                        ts2 = 20.f;
                    } else {
                        ts2 = 60.f;
                    }
                    if (ts2 <= pV) {
                        ConditionallyNoteSkid(pCar, faces, 2);
                    }
                    SkidNoise(pCar, 2, pV, pCar->material_index[2]);
                }
                force[2] = pCar->friction_slipping_reduction * maxfl;
                pCar->wheel_slip |= 1;
            }
            break;
        case 3:
            force[2] = pCar->friction_slipping_reduction * maxfl;
            force[3] = pCar->friction_slipping_reduction * maxfr;
            pCar->wheel_slip |= 1;
            pV = (v135 * 2.0 - maxfl - maxfr) / (maxfr + maxfl);
            if (&gProgram_state.current_car == pCar) {
                ts2 = 20.f;
            } else {
                ts2 = 60.f;
            }
            if (ts2 <= pV) {
                if (maxfl > 0.1f) {
                    ConditionallyNoteSkid(pCar, faces, 2);
                }
                if (maxfr > 0.1f) {
                    ConditionallyNoteSkid(pCar, faces, 3);
                }
            }
            if (IRandomBetween(0, 1)) {
                if (maxfl > 0.1f) {
                    SkidNoise(pCar, 2, pV, pCar->material_index[2]);
                }
            } else {
                if (maxfr > 0.1f) {
                    SkidNoise(pCar, 3, pV, pCar->material_index[3]);
                }
            }
            break;
        }
        BrVector3Scale(&v136, &rightplane, v99);
        BrVector3Scale(&a, &v103, v109);
        BrVector3Accumulate(&v136, &a);
        BrVector3Scale(&v123, &rightplane, v98);
        BrVector3Scale(&a, &v103, v108);
        BrVector3Accumulate(&v123, &a);
        for (i = 0; i < 4; i++) {
            rightplane = pCar->wpos[i];
            rightplane.v[1] = rightplane.v[1] - pCar->oldd[i];
            BrVector3Sub(&rightplane, &rightplane, &pCar->centre_of_mass_world_scale);
            if (i < 2) {
                BrVector3Scale(&b, &v136, force[i]);
            } else {
                BrVector3Scale(&b, &v123, force[i]);
            }
            BrVector3Accumulate(&B, &b);
            BrVector3Cross(&a, &rightplane, &b);
            BrVector3Accumulate(&f, &a);
        }
    } else {
        pCar->max_force_front = 0.f;
        pCar->max_force_rear = 0.f;
        if (gLast_car_to_skid[0] == pCar) {
            DRS3StopSound(gSkid_tag[0]);
        }
        if (gLast_car_to_skid[1] == pCar) {
            DRS3StopSound(gSkid_tag[1]);
        }
    }
    pCar->number_of_wheels_on_ground = normnum;
    BrMatrix34ApplyV(&b, &B, mat);
    BrVector3Scale(&rightplane, &f, pDt);
    PhysicsObjectApplyTorque(pCar->collision_info, &rightplane);
    BrVector3Scale(&rightplane, &b, pDt / (pCar->collision_info->M * WORLD_SCALE));
    BrVector3Accumulate(&pCar->collision_info->v, &rightplane);
    if (pCar->speed < 0.0001f
        && ((!pCar->keys.acc && pCar->joystick.acc <= 0) || !pCar->gear)
        && !pCar->keys.dec
        && pCar->joystick.dec <= 0
        && pCar->bounce_rate == 0.f
        && BrVector3Length(&pCar->collision_info->omega) < 0.05f) {
        if (vol != NULL) {
            ts2 = pCar->driver > 5 && pCar->underwater_ability != 0
                ? 1.0 - (1.0 - vol->gravity_multiplier) * 0.6
                : vol->gravity_multiplier;
            friction_number = BrVector3Length(&b) / ts2 / gGravity_multiplier;
        } else {
            friction_number = BrVector3Length(&b);
        }
        if (pCar->collision_info->M > friction_number || (pCar->keys.brake && normnum >= 3)) {
            if (stop_timer == 100.f) {
                stop_timer = 0.f;
            }
            if (stop_timer > 0.5f) {
                BrVector3SetFloat(&pCar->collision_info->v, 0.f, 0.f, 0.f);
                BrVector3SetFloat(&pCar->collision_info->omega, 0.f, 0.f, 0.f);
                stop_timer = 0.5f;
            }
        }
    }
    stop_timer = pDt + stop_timer;
    if (stop_timer > 1.f) {
        stop_timer = 100.f;
    }
    AddDrag(pCar, (tPhysics_object*)pCar->collision_info, pDt);
    if (pCar->driver >= eDriver_net_human) {
        pCar->acc_force = -(v136.v[2] * force[0]) - v136.v[2] * force[1];
    }
    pCar->curvature = pCar->curvature - pCar->field_0x1260;
}

#pragma inline_depth(1)

// DoRevs

// ScrapeNoise

// SkidNoise

// FUNCTION: CARMA2_HW 0x0041bed0
void C2_HOOK_FASTCALL SkidNoise(tCar_spec* pCar, int pSide, br_scalar pV, int pMaterial_index) {
    br_vector3 pos;
    br_vector3 wv;
    br_vector3 world_pos;
    br_vector3 wvw;
    br_vector3 tv;
    br_scalar ts;
    static tS3_volume last_skid_vol[2];
    int i;

    i = IRandomBetween(0, 1);
    if (pV < 5.0f) {
        return;
    }
    if (gCurrent_race.material_modifiers[pMaterial_index].tyre_noise_index == -1) {
        return;
    }
    if (IRandomBetween(0, 4) != 0) {
        return;
    }

    last_skid_vol[i] = pV * 10.0f;
    pos.v[0] = pCar->wpos[pSide].v[0];
    pos.v[1] = pCar->wpos[pSide].v[1] - pCar->oldd[pSide];
    pos.v[2] = pCar->wpos[pSide].v[2];
    BrMatrix34ApplyP(&world_pos, &pos, &pCar->car_master_actor->t.t.mat);
    BrVector3InvScale(&world_pos, &world_pos, WORLD_SCALE);
    if (!DRS3SoundStillPlaying(gSkid_tag[i])
            || (pCar != NULL && pCar->driver == eDriver_local_human && gLast_car_to_skid[i] != pCar)) {
        gSkid_tag[i] = DRS3StartSound3D(gXXX_outlet,
            IRandomBetween(0, 4) + 9000,
            &world_pos,
            &pCar->vel,
            1,
            last_skid_vol[i],
            IRandomBetween(49152, 81920),
            0x10000);
        gLast_car_to_skid[i] = pCar;
    }
    if (gCurrent_race.material_modifiers[pMaterial_index].smoke_type == 1) {
        BrVector3Cross(&wv, &pCar->collision_info->omega, &pos);
        BrVector3Scale(&tv, &pCar->collision_info->velocity_car_space, WORLD_SCALE);
        BrVector3Add(&wv, &wv, &tv);
        ts = -(BrVector3Dot(&wv, &pCar->road_normal));
        BrVector3Scale(&wvw, &pCar->road_normal, ts);
        BrVector3Accumulate(&wv, &wvw);
        BrMatrix34ApplyV(&wvw, &wv, &pCar->car_master_actor->t.t.mat);
        CreatePuffOfSmoke(&world_pos, &wvw, pV / 25.0f, 1.0f, 4);
    }
}

// StopSkid

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0041ddc0
void C2_HOOK_FASTCALL CrashNoise(const br_vector3* pImpact_velocity, const br_vector3* pPosition, int pMaterial_index) {
    int volume;

    volume = (int)(BrVector3Length(pImpact_velocity) * 60.0);
    if (gCurrent_race.material_modifiers[pMaterial_index].crash_noise_index != -1) {
        br_vector3 zero;

        if (volume > 255) {
            volume = 255;
        }
        if (gCrash_noise_tag != 0 && DRS3SoundStillPlaying(gCrash_noise_tag) != 0) {
            return;
        }
        if (volume > 30) {
            gCrash_noise_volume = volume;
            zero.v[0] = 0.0f;
            zero.v[1] = 0.0f;
            zero.v[2] = 0.0f;
            gCrash_noise_tag = DRS3StartSound3D(gXXX_outlet,
                gCrash_noise_sound_ids[IRandomBetween(0, 4)], pPosition, &zero, 1, volume,
                IRandomBetween(0xc000, 0x14000), 0x10000);
        }
    }
}
#pragma auto_inline(on)

// CrushAndDamageCar

// PointInFaceByQuiteABitActually

// DoEnvironmentSmashes

// ProcessForcesCallBack

// ProcessJointForcesCallBack

// MultiFindFloorInBoxM

// MultiRayCastOnObjects

// MultiFindFloorInBoxBU

// FindFace

// findfloor

// FindFloorInBoxBU

// CancelPendingCunningStunt

// SetAmbientPratCam

// SetTextureBits

// MungeSomeOtherCarGraphics

// MungeCarGraphics

// TurnOffNonGroovers

// DoLODCarModels

// DoComplexCarModels

// ResetCarScreens

// FlyCar

// GetCarOverallBoundsMinY

// SetCarSuspGiveAndHeight

// TestForCarInSensiblePlace

// PullActorFromWorld

// DoPullActorFromWorld

// PipeNonCarObject

// PipeNonCars

// CheckForDeAttachmentOfNonCars

// AdjustNonCar

// GetPrecalculatedFacesUnderCar

// TurnOnNonCar

// TurnOffNonCar

