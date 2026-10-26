#ifndef CARPOCALYPSE2_MATCHING
#include "loadprof.h"
#endif
#include "mainloop.h"

#include "car.h"
#include "controls.h"
#include "crush.h"
#include "depth.h"
#include "displays.h"
#include "drmem.h"
#include "drone.h"
#include "errors.h"
#include "explosions.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "graphics.h"
#include "globvrpb.h"
#include "input.h"
#include "joystick.h"
#include "loading.h"
#include "main.h"
#include "mainmenu.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pedestrn.h"
#include "physics.h"
#include "piping.h"
#include "powerups.h"
#include "racemem.h"
#include "replay.h"
#include "skidmark.h"
#include "smashing.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "timers.h"
#include "trig.h"
#include "utility.h"
#include "world.h"

#include "carpocalypse2_macros.h"
#ifdef __vita__
#include "sdl3/vita_perf.h"
#endif
#include "mainloop.h"

#ifdef CARPOCALYPSE2_MATCHING
#include <windows.h>

extern br_size_t gPage_size;
extern SYSTEM_INFO gSystem_info;
#endif
// GLOBAL: CARMA2_HW 0x0068c814
tU32 gOld_camera_time;

// GLOBAL: CARMA2_HW 0x0068c820
tU32 gActual_last_tick_count;

// GLOBAL: CARMA2_HW 0x0068c824
tU32 gAverage_frame_period;

// GLOBAL: CARMA2_HW 0x0068c770
tU32 gLast_net_auto_increase_credits;

// GLOBAL: CARMA2_HW 0x0068c764
tU32 gLast_credit_headup__mainloop;

// GLOBAL: CARMA2_HW 0x0068c76c
int gLast_time_headup;

// GLOBAL: CARMA2_HW 0x0068c808
int gTime_bonus_headup;

// GLOBAL: CARMA2_HW 0x0068c810
int gRace_bonus_headup;

// GLOBAL: CARMA2_HW 0x0068c788
tU32 gTime_bonus_start;

// GLOBAL: CARMA2_HW 0x0068c784
tTime_bonus_state gTime_bonus_state;

// GLOBAL: CARMA2_HW 0x0068c768
int gWasted_flash_state;

// GLOBAL: CARMA2_HW 0x0068c730
tU32 gWasted_last_flash;

// GLOBAL: CARMA2_HW 0x0068c778
float gFLOAT_0068c778;

// GLOBAL: CARMA2_HW 0x0068c80c
int gCredits_post_race;

// GLOBAL: CARMA2_HW 0x0068c738
br_vector3 gPrev_camera_position;

// GLOBAL: CARMA2_HW 0x0074abf0
tU32 gCamera_period;

// GLOBAL: CARMA2_HW 0x0068c834
tU32 gPrev_task_switched_message;

// GLOBAL: CARMA2_HW 0x0068c838
int gWindow_inactive;

// GLOBAL: CARMA2_HW 0x0068c774
int gTotal_bonus_post_race;

// GLOBAL: CARMA2_HW 0x0068c760
int gQueued_wasted_messages_count;

// GLOBAL: CARMA2_HW 0x0068c77c
tU32 gLast_wasted_message_start;

// GLOBAL: CARMA2_HW 0x0068c748
int gQueued_wasted_messages[5];

// GLOBAL: CARMA2_HW 0x0068c75c
int gCompletion_bonus_post_race;

// GLOBAL: CARMA2_HW 0x0068c780
int gTime_bonus;


// GLOBAL: CARMA2_HW 0x00761ef0
int gCredits_per_second_time_bonus[3];  /* FIXME: rename to gPoints_per_second */

// GLOBAL: CARMA2_HW 0x0074b740
int gAuto_increase_credits_dt[3];

// GLOBAL: CARMA2_HW 0x0074b720
tU32 gNet_auto_increase_credits_dt[8];

// GLOBAL: CARMA2_HW 0x0079eb00
char gHeadup_oppo_ped_text[64];

#ifndef CARPOCALYPSE2_MATCHING
/* Opt-in snapshots for long-play traffic/non-car motion comparisons. */
static void LogNonVehiclePhysics(void) {
    static tU32 last_time;
    static int have_time;
    tU32 now = gTime_stamp_for_this_munging;
    tPhysics_object* object;
    static tU32 wall_start;
    static tU32 frame_count;
    tU32 wall_now;
    tU32 wall_elapsed;
    int passive_peds = 0;
    int active_peds = 0;
    int i;
    if (getenv("C2_OBJLOG") == NULL) return;
    wall_now = PDGetTotalTime();
    if (!have_time) wall_start = wall_now;
    frame_count++;
    if (have_time && now - last_time < 1000) return;
    wall_elapsed = wall_now - wall_start;
    dr_dprintf("PEDTIMING frames=%u wall_ms=%u loop_fps=%.3f engine_frame_ms=%u",
        frame_count, wall_elapsed, wall_elapsed != 0 ? frame_count * 1000.0 / wall_elapsed : 0.0,
        gFrame_period_for_this_munging);
    frame_count = 0;
    wall_start = wall_now;
    for (i = 0; i < gPed_count; i++) {
        tPedestrian* ped = &gPedestrian_array[i];
        tPed_character_instance* character = ped->character;
        tPHIL_queued_header* info;
        if (character == NULL || !(character->field_0x14 & 1) || (character->field_0x14 & 4)
                || character->field_0x4 < 0 || ped->hit_points <= 0
                || (ped->action != ePed_action_walking && ped->action != ePed_action_running)) continue;
        object = character->personality->form->simple_physicing[character->field_0x5].collision_info;
        info = object != NULL ? object->field_0x240 : NULL;
        if (info != NULL && info->field_0x8 == 1) passive_peds++;
        if (info != NULL && info->field_0x8 == 2) active_peds++;
    }
    dr_dprintf("PEDOWNERS passive=%d active=%d", passive_peds, active_peds);
    have_time = 1;
    last_time = now;
    dr_dprintf("OBJFRAME time=%u tick=%u frame_ms=%u gravity=%.6g", now,
        gPHIL_last_physics_tick, gFrame_period_for_this_munging, gGravity_multiplier);
    for (object = gList_collision_infos; object != NULL; object = object->next) {
        tPHIL_queued_header* info = object->field_0x240;
        br_matrix34* mat = &object->transform_matrix;
        if (object->flags_0x238 == 1 || object->actor == NULL) continue;
        dr_dprintf("OBJLOG %p type=%u owner=%p state=%d halted=%u flags=%x M=%.7g I=%.7g,%.7g,%.7g v=%.7g,%.7g,%.7g omega=%.7g,%.7g,%.7g pos=%.7g,%.7g,%.7g basis=%.7g,%.7g,%.7g grav=%.7g drag=%.7g", object,
            object->flags_0x238, object->owner, info != NULL ? info->field_0x8 : -1,
            object->disable_move_rotate, object->flags, object->M, object->I.v[0], object->I.v[1], object->I.v[2],
            object->v.v[0], object->v.v[1], object->v.v[2], object->omega.v[0], object->omega.v[1], object->omega.v[2],
            mat->m[3][0], mat->m[3][1], mat->m[3][2],
            BrVector3LengthSquared((br_vector3*)mat->m[0]), BrVector3LengthSquared((br_vector3*)mat->m[1]), BrVector3LengthSquared((br_vector3*)mat->m[2]),
            info != NULL ? info->field_0x10 : 0.f, info != NULL ? info->field_0xc : 0.f);
    }
    for (i = 0; i < gCount_drones; ++i) {
        tDrone_spec* drone = &gDrone_specs[i];
        if (drone->actor == NULL || drone->current_state == 5) continue;
        dr_dprintf("DRONELOG id=%d object=%p state=%d last=%u dt=%.7g path_v=%.7g,%.7g,%.7g pos=%.7g,%.7g,%.7g", drone->id,
            &drone->collision_info, drone->current_state, drone->time_last_munge, gDrone_delta_time,
            drone->field_0x18.v[0], drone->field_0x18.v[1], drone->field_0x18.v[2],
            drone->actor->t.t.mat.m[3][0], drone->actor->t.t.mat.m[3][1], drone->actor->t.t.mat.m[3][2]);
    }
}

/* Test harness: C2_TEST_SMASH=n puts the player car ~2 units in front of smashable race target n,
 * facing it, two seconds into the race (hold accelerate with C2_TEST_KEYS to ram it). */
static float gC2_test_smash_height = .3f;
/* C2_TEST_SMASH=m:<prefix>: first track face whose material identifier starts with <prefix> */
static int gC2_test_face_skip; /* matches to skip (C2_TEST_SMASH=m:prefix,dist,height,index) */
static int TestFindMaterialFace(br_actor* pActor, const char* pPrefix, br_vector3* pCentre, br_vector3* pNormal) {
    br_actor* child;
    if (pActor->model != NULL && pActor->model->faces != NULL) {
        br_model* model = pActor->model;
        int f;
        for (f = 0; f < model->nfaces; f++) {
            br_material* mat = model->faces[f].material;
            if (mat != NULL && mat->identifier != NULL && strncmp(mat->identifier, pPrefix, strlen(pPrefix)) == 0) {
                br_matrix34 to_world;
                if (gC2_test_face_skip > 0) {
                    /* skip the rest of this model: count one match per model */
                    gC2_test_face_skip--;
                    break;
                }
                br_vector3 c, a, b, n;
                br_vertex* v = model->vertices;
                br_face* face = &model->faces[f];
                BrVector3Add(&c, &v[face->vertices[0]].p, &v[face->vertices[1]].p);
                BrVector3Accumulate(&c, &v[face->vertices[2]].p);
                BrVector3InvScale(&c, &c, 3.f);
                BrVector3Sub(&a, &v[face->vertices[1]].p, &v[face->vertices[0]].p);
                BrVector3Sub(&b, &v[face->vertices[2]].p, &v[face->vertices[0]].p);
                BrVector3Cross(&n, &a, &b);
                BrActorToActorMatrix34(&to_world, pActor, gUniverse_actor);
                BrMatrix34ApplyP(pCentre, &c, &to_world);
                BrMatrix34ApplyV(pNormal, &n, &to_world);
                dr_dprintf("C2_TEST_SMASH material face %s actor=%s centre=%.2f,%.2f,%.2f normal=%.2f,%.2f,%.2f",
                    mat->identifier, pActor->identifier ? pActor->identifier : "?",
                    pCentre->v[0], pCentre->v[1], pCentre->v[2], pNormal->v[0], pNormal->v[1], pNormal->v[2]);
                return 1;
            }
        }
    }
    /* prepared track models keep only groups: group->stored is the material's stored state */
    if (pActor->model != NULL && pActor->model->prepared != NULL) {
        v11model* prepared = pActor->model->prepared;
        int g, i;
        for (g = 0; g < prepared->ngroups; g++) {
            v11group* group = &prepared->groups[g];
            for (i = 0; i < gCount_track_smashable_environment_specs; i++) {
                tSmashable_item_spec* spec = &gTrack_smashable_environment_specs[i];
                br_material* mat;
                if (spec->trigger_type != kSmashableTrigger_Material) continue;
                mat = spec->trigger_object.material;
                if (mat == NULL || mat->identifier == NULL || strncmp(mat->identifier, pPrefix, strlen(pPrefix)) != 0) continue;
                if (group->stored == NULL || group->stored != mat->stored || group->nfaces == 0) continue;
                if (gC2_test_face_skip-- > 0) continue;
                {
                    br_matrix34 to_world;
                    br_vector3 c;
                    v11face* face = &group->faces[0];
                    BrVector3Add(&c, &group->vertices[face->vertices[0]].p, &group->vertices[face->vertices[1]].p);
                    BrVector3Accumulate(&c, &group->vertices[face->vertices[2]].p);
                    BrVector3InvScale(&c, &c, 3.f);
                    BrActorToActorMatrix34(&to_world, pActor, gUniverse_actor);
                    BrMatrix34ApplyP(pCentre, &c, &to_world);
                    BrMatrix34ApplyV(pNormal, (br_vector3*)&face->eqn, &to_world);
                    dr_dprintf("C2_TEST_SMASH prepared face %s actor=%s faces=%d centre=%.2f,%.2f,%.2f normal=%.2f,%.2f,%.2f",
                        mat->identifier, pActor->identifier ? pActor->identifier : "?", group->nfaces,
                        pCentre->v[0], pCentre->v[1], pCentre->v[2], pNormal->v[0], pNormal->v[1], pNormal->v[2]);
                    return 1;
                }
            }
        }
    }
    for (child = pActor->children; child != NULL; child = child->next) {
        if (TestFindMaterialFace(child, pPrefix, pCentre, pNormal)) return 1;
    }
    return 0;
}
static void TestPlaceAtSmashTarget(void) {
    static int done;
    const char* e;
    int n;
    br_matrix34 target_mat, m;
    br_vector3 t, d;
    tCar_spec* car = &gProgram_state.current_car;

    static tU32 go_time;
    static char spec_buf[128];
    static int stage;
    if (done || (e = getenv("C2_TEST_SMASH")) == NULL) return;
    if (gCountdown != 0 || go_time == 0) { if (gCountdown == 0) go_time = PDGetTotalTime(); else go_time = 0; return; }
    /* "first;second": the second placement follows 6 s after the first */
    if (PDGetTotalTime() - go_time < (stage == 0 ? 500u : 6500u)) return;
    {
        const char* semi = strchr(e, ';');
        if (stage == 0) {
            size_t len = semi != NULL ? (size_t)(semi - e) : strlen(e);
            if (len >= sizeof(spec_buf)) len = sizeof(spec_buf) - 1;
            memcpy(spec_buf, e, len);
            spec_buf[len] = 0;
            stage = 1;
            done = semi == NULL;
        } else {
            strncpy(spec_buf, semi + 1, sizeof(spec_buf) - 1);
            done = 1;
        }
        e = spec_buf;
    }
    n = atoi(e);
    dr_dprintf("C2_TEST_SMASH targets=%d", gCount_smashable_race_targets);
    for (n = 0; n < gCount_smashable_race_targets; n++) {
        br_actor* a = gSmashable_race_targets[n].actor;
        BrActorToActorMatrix34(&target_mat, a, gUniverse_actor);
        dr_dprintf("C2_TEST_SMASH target %d actor=%s model=%s pos=%.2f,%.2f,%.2f", n,
            a->identifier ? a->identifier : "?", a->model && a->model->identifier ? a->model->identifier : "?",
            target_mat.m[3][0], target_mat.m[3][1], target_mat.m[3][2]);
    }
    if (e[0] == 'm' && e[1] == ':') {
        /* C2_TEST_SMASH=m:<prefix>[,dist,height]: approach the face along its normal */
        char prefix[32];
        br_vector3 normal;
        float dist = 5.f, height = .3f, len;
        const char* c = strchr(e, ',');
        size_t plen = c != NULL ? (size_t)(c - (e + 2)) : strlen(e + 2);
        if (plen >= sizeof(prefix)) plen = sizeof(prefix) - 1;
        memcpy(prefix, e + 2, plen);
        prefix[plen] = '\0';
        gC2_test_face_skip = 0;
        if (c != NULL) sscanf(c + 1, "%f,%f,%d", &dist, &height, &gC2_test_face_skip);
        if (!TestFindMaterialFace(gUniverse_actor, prefix, &t, &normal)) {
            int i;
            dr_dprintf("C2_TEST_SMASH no face with material %s", prefix);
            for (i = 0; i < gCount_track_smashable_environment_specs; i++) {
                tSmashable_item_spec* spec = &gTrack_smashable_environment_specs[i];
                if (spec->trigger_type == kSmashableTrigger_Material && spec->trigger_object.material != NULL)
                    dr_dprintf("C2_TEST_SMASH spec %d material=%.11s stored=%p", i,
                        spec->trigger_object.material->identifier ? spec->trigger_object.material->identifier : "?",
                        (void*)spec->trigger_object.material->stored);
            }
            return;
        }
        len = sqrtf(normal.v[0] * normal.v[0] + normal.v[2] * normal.v[2]);
        if (len < 1e-6f) return;
        gC2_test_smash_height = height;
        BrVector3Set(&d, normal.v[0] / len * dist / 2.f, 0.f, normal.v[2] / len * dist / 2.f);
        t.v[1] -= .3f; /* face centre is above the ground */
        goto place;
    }
    n = atoi(e);
    if (n < 0 || n >= gCount_smashable_race_targets) return;
    BrActorToActorMatrix34(&target_mat, gSmashable_race_targets[n].actor, gUniverse_actor);
    BrVector3Copy(&t, (br_vector3*)target_mat.m[3]);
    { /* C2_TEST_SMASH=n[,dx,dz[,dist]]: approach direction (default +x) */
        float dx = 1.f, dz = 0.f, dist = 2.f, height = .3f, len;
        const char* c = strchr(e, ',');
        if (c != NULL) sscanf(c + 1, "%f,%f,%f,%f", &dx, &dz, &dist, &height);
        gC2_test_smash_height = height;
        len = sqrtf(dx * dx + dz * dz);
        if (len < 1e-3f) { dx = 1.f; dz = 0.f; len = 1.f; }
        BrVector3Set(&d, dx / len * dist / 2.f, 0.f, dz / len * dist / 2.f);
    }
place:
    BrMatrix34Identity(&m);
    /* car forward is -z: z axis = d */
    {
        float l = sqrtf(d.v[0] * d.v[0] + d.v[2] * d.v[2]);
        m.m[2][0] = d.v[0] / l; m.m[2][1] = 0.f; m.m[2][2] = d.v[2] / l;
        m.m[0][0] = d.v[2] / l; m.m[0][1] = 0.f; m.m[0][2] = -d.v[0] / l;
    }
    m.m[3][0] = t.v[0] + 2.f * d.v[0];
    m.m[3][1] = t.v[1] + gC2_test_smash_height;
    m.m[3][2] = t.v[2] + 2.f * d.v[2];
    for (n = 0; n < (int)CARPOCALYPSE2_ASIZE(car->last_safe_positions); n++) {
        BrMatrix34Copy(&car->last_safe_positions[n], &m);
    }
    FlipUpCar(car);
    dr_dprintf("C2_TEST_SMASH placed car at %.2f,%.2f,%.2f", car->car_master_actor->t.t.translate.t.v[0],
        car->car_master_actor->t.t.translate.t.v[1], car->car_master_actor->t.t.translate.t.v[2]);
}

/* First twelve seconds only, for countdown/start-grid investigations. */
static void LogRaceStart(void) {
    static tU32 previous_start, previous_sample;
    static int have_start, previous_countdown;
    tU32 now, age;
    tCar_spec* player = &gProgram_state.current_car;
    int i;
    if (getenv("C2_OBJLOG") == NULL) return;
    now = PDGetTotalTime();
    age = now - gRace_start;
    if (age > 12000) return;
    if (have_start && previous_start == gRace_start
            && now - previous_sample < 500 && previous_countdown == gCountdown) return;
    have_start = 1; previous_start = gRace_start;
    previous_sample = now; previous_countdown = gCountdown;
    dr_dprintf("RACESTART age=%u wall=%u ai=%u frame=%u count=%d ack=%d jump=%d",
        age, now, gTime_stamp_for_this_munging, gFrame_period_for_this_munging,
        gCountdown, gAcknowledged_start, gStart_jumped);
    dr_dprintf("RACEPLAYER acc_key=%d up=%d down=%d gear=%d acc=%.7g brake=%.7g",
        player->keys.acc, player->keys.change_up, player->keys.change_down,
        player->gear, player->acc_force, player->brake_force);
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; ++i) {
        tOpponent_spec* opponent = &gProgram_state.AI_vehicles.opponents[i];
        tCar_spec* car = opponent->car_spec;
        tPhysics_object* body;
        if (car == NULL || car->collision_info == NULL || car->car_master_actor == NULL) continue;
        body = car->collision_info;
        dr_dprintf("RACEGRID slot=%d id=%d name=%.24s rank=%d stun=%u objective=%d physics=%d cheat=%d",
            i, opponent->index, car->driver_name, gOpponents[opponent->index].strength_rating,
            opponent->stun_time_ends, opponent->current_objective, opponent->physics_me, opponent->cheating);
        dr_dprintf("RACEFORCE slot=%d acc=%.7g brake=%.7g M=%.7g v=%.7g,%.7g,%.7g pos=%.7g,%.7g,%.7g",
            i, car->acc_force, car->brake_force, body->M, body->v.v[0], body->v.v[1], body->v.v[2],
            car->car_master_actor->t.t.translate.t.v[0], car->car_master_actor->t.t.translate.t.v[1],
            car->car_master_actor->t.t.translate.t.v[2]);
    }
}

#endif

// C2PORT-NONMATCHING: best 32.67% | difftest PASS 2000 trials (spec difftest/specs/00401170_MungeAIWorld.json) | reason timer-stack scheduling and register allocation
// C2PORT-NONMATCHING: best 32.67% | difftest PASS 2000 trials (spec difftest/specs/00401170_MungeAIWorld.json) | reason timer-stack scheduling and register allocation
// FUNCTION: CARMA2_HW 0x00401170
void C2_HOOK_FASTCALL MungeAIWorld(tU32 pFrame_period) {

    gTime_stamp_for_this_munging = GetTotalTime();
    if (gTime_stamp_for_this_munging == 0) {
        gTime_stamp_for_this_munging = PDGetTotalTime();
    }
    gDrone_delta_time = (float)pFrame_period * 0.001f;
    gFrame_period_for_this_munging = pFrame_period;
    Timers_Push(TIMER_DRN);
    DoDronePerGameFrameStuff();
#ifndef CARPOCALYPSE2_MATCHING
    LogNonVehiclePhysics();
#endif
    Timers_Pop(TIMER_DRN);
    Timers_Push(TIMER_OPP);
    MungeOpponents();
#ifndef CARPOCALYPSE2_MATCHING
    LogRaceStart();
    TestPlaceAtSmashTarget();
    {
        /* heap-corruption tripwire (drmem.c), about once a second of game time */
        extern void C2_HOOK_FASTCALL C2CheckRegistriesAt(const char* pTitle);
        static tU32 last_registry_check;
        if (gTime_stamp_for_this_munging - last_registry_check >= 1000) {
            last_registry_check = gTime_stamp_for_this_munging;
            C2CheckRegistriesAt("race frame");
        }
    }
    if (getenv("C2_TEST_NAPALM") != NULL) {
        /* harness: fire the ped flamethrower power-up's napalm bolt every 2 s (MungeNapalm check) */
        extern int C2_HOOK_FASTCALL NapalmPedestrianNearCar(tCar_spec* car);
        static tU32 last_napalm;
        if (gTime_stamp_for_this_munging - last_napalm >= 2000) {
            last_napalm = gTime_stamp_for_this_munging;
            dr_dprintf("TEST NAPALM fired=%d", NapalmPedestrianNearCar(&gProgram_state.current_car));
        }
    }
    if (getenv("C2_TEST_MAPVIEW") != NULL) {
        /* harness: switch to the full-screen map once, C2_TEST_MAPVIEW seconds into the race */
        static int map_toggled;
        if (!map_toggled && gTime_stamp_for_this_munging - gRace_start >= (tU32)atoi(getenv("C2_TEST_MAPVIEW")) * 1000) {
            map_toggled = 1;
            ToggleMap();
        }
    }
#endif
    Timers_Pop(TIMER_OPP);
    gFirst_drone_processing = 0;
    ProcessShitMines(gTime_stamp_for_this_munging);
}

// FUNCTION: CARMA2_HW 0x004940e0
void C2_HOOK_FASTCALL CalculateCameraStuff(tU32 pCamera_period) {
    br_camera* camera;
    br_vector3 delta_camera;

    gOur_pos = &gPlayer_car_master_actor->t.t.translate.t;
    PositionExternalCamera(&gProgram_state.current_car, pCamera_period);
    BrActorToActorMatrix34(&gCamera_to_world, gCamera, gUniverse_actor);
    BrActorToActorMatrix34(&gRearview_camera_to_world, gRearview_camera, gUniverse_actor);
    gCamera_to_horiz_angle = FastScalarArcTan2(gCamera_to_world.m[2][1], gCamera_to_world.m[1][1]);

    camera = gCamera->type_data;
    gYon_squared = gYon_multiplier * gYon_multiplier * camera->yon_z * camera->yon_z;
    BrVector3Sub(&delta_camera, (br_vector3*)gCamera_to_world.m[3], &gPrev_camera_position);
    if (BrVector3LengthSquared(&delta_camera) > 400.0) {
        ResetPedNearness();
    }
    BrVector3Copy(&gPrev_camera_position, (br_vector3*)gCamera_to_world.m[3]);
}

// FUNCTION: CARMA2_HW 0x00494210
int C2_HOOK_FAKE_THISCALL FUN_00494210(int pStatus, undefined4 pArg2, undefined4 pArg3, undefined4 pArg4, int pFlag) {
    (void)pArg2;
    (void)pArg3;
    (void)pArg4;
    if (pFlag != 0) {
        return -1;
    }
    switch (pStatus) {
    case 1:
        PreloadBunchOfFlics(4);
        return pStatus;
    case 2:
        PreloadBunchOfFlics(5);
        return pStatus;
    case 3:
        PreloadBunchOfFlics(1);
        return pStatus;
    case 4:
        PreloadBunchOfFlics(3);
        return pStatus;
    case 5:
        PreloadBunchOfFlics(2);
        return pStatus;
    case 7:
        PreloadBunchOfFlics(7);
    }
    return pStatus;
}

void C2_HOOK_FASTCALL CheckActiveCarList(void) {
    int i;
    int new_count = 0;

    for (i = 0; i < gNum_active_cars; i++) {
        tCar_spec *c;

        c = gActive_car_list[i];
        if (c != NULL && c->driver > 5) {
            if (i != new_count) {
                gActive_car_list[new_count] = c;
            }
            new_count += 1;
        }
    }
    gNum_active_cars = new_count;
}

void C2_HOOK_FASTCALL CalculateFrameRate(void) {
    // GLOBAL: CARMA2_HW 0x0068c734
    static  tU32 last_time;
    tU32 new_time;
    // GLOBAL: CARMA2_HW 0x0068c790
    static int last_rates[30];
    int i;
    int new_rate;

    new_time = PDGetTotalTime();
    if (new_time != last_time) {
        new_rate = 10000 / (new_time - last_time);
        last_rates[CARPOCALYPSE2_ASIZE(last_rates) - 1] = new_rate;
        gFrame_rate = new_rate;
        for (i = 0; i < CARPOCALYPSE2_ASIZE(last_rates); i++) {
            gFrame_rate += last_rates[i];
        }
        gFrame_rate /= CARPOCALYPSE2_ASIZE(last_rates) + 1;
        for (i = 0; i < CARPOCALYPSE2_ASIZE(last_rates) - 1; i++) {
            last_rates[i] = last_rates[i + 1];
        }
        last_time = new_time;
    }
}

// FUNCTION: CARMA2_HW 0x00492840
void C2_HOOK_FASTCALL FUN_00492840(void* pMemory, br_size_t pSize) {
#ifdef CARPOCALYPSE2_MATCHING
    br_size_t offset;

    if (gPage_size == 0) {
        GetSystemInfo(&gSystem_info);
        gPage_size = gSystem_info.dwOemId;
    }
    for (offset = gPage_size << 4; offset < pSize; offset += gPage_size) {
        *((volatile br_uint_8*)pMemory + (offset - 16 * gPage_size)) = *((volatile br_uint_8*)pMemory + (offset - 16 * gPage_size));
        *((volatile br_uint_8*)pMemory + offset) = *((volatile br_uint_8*)pMemory + offset);
    }
#else
    (void)pMemory;
    (void)pSize;
#endif
}

// FUNCTION: CARMA2_HW 0x00492980
tRace_result C2_HOOK_FASTCALL MainGameLoop(void) {
    int tried_to_allocate_AR;
    tRace_result result;

    tried_to_allocate_AR = 0;
    gCar_to_view = &gProgram_state.current_car;
    gCar_to_view_id = 0;
    gOld_camera_time = 0;
    gLast_net_auto_increase_credits = 0;
    gLast_credit_headup__mainloop = 0;
    gLast_time_headup = gTimer;
    gRace_bonus_headup = -1;
    gTime_bonus_headup = -1;
    gTime_bonus_start = 0;
    gTime_bonus_state = 0;
    ARMainLoopStart();
    gActual_last_tick_count = PDGetTotalTime();
    gFrame_period = 100;
    gQueued_wasted_messages_count = 0;
    gWasted_flash_state = 0;
    gWasted_last_flash = 0;
    gLast_tick_count = gActual_last_tick_count;
    RevertPalette();
    gHost_abandon_game = 0;
    gNo_races_yet = 0;
    gRace_over_reason = eRace_not_over_yet;
    gFLOAT_0068c778 = 0.f;
    gNo_credits_APO_restore = 0;
    gReceived_game_scores = 0;
    if (gNet_mode == eNet_mode_none) {
        gProgram_state.field_0x2c = 1;
    }
    gCredits_post_race = -1;
    ShowSpecialVolumesIfRequ();
    gProgram_state.racing = 1;
    if (gNet_mode == eNet_mode_host) {
        gCurrent_net_game->status.stage = eNet_game_playing;
    }
    NetPlayerStatusChanged(ePlayer_status_racing);
    GetAverageGridPosition(&gCurrent_race);
    ForceRebuildActiveCarList();
    PrintMemoryDump(0, "ABOUT TO ENTER MAINLOOP");
#ifndef CARPOCALYPSE2_MATCHING
    C2LoadEnd();
#endif
    UpdateFramePeriod(&gCamera_period);
    BrVector3Set(&gPrev_camera_position, 1000.f, 1000.f, 1000.f);
    CalculateCameraStuff(gCamera_period);

    /* Only present in win32 version */
    PDPageInProcessMemory();

    gRace_start = PDGetTotalTime();
    Timers_Init();

    if (gWait_for_it != 0) {
        gWait_for_it += GetTotalTime();
    }
    ClearEntireScreen();
    ResetPalette();
    for (;;) {
        int window_inactive;
        tU32 frame_start_time;

        Timers_StartFrame();
        EdgeTriggerModeOff();
        if (!gAction_replay_mode) {
            UpdateFramePeriod(&gCamera_period);
        }
        frame_start_time = GetTotalTime();
#ifdef __vita__
        C2VitaPerfReset();
#endif
        CyclePollKeys();
        InitSmashQueue();
        CheckSystemKeys(1);
        InitialiseExtraRenders();
        NetReceiveAndProcessMessages();

        if (gHost_abandon_game || gProgram_state.prog_status == eProg_idling) {
            break;
        }
        if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox && gMap_view == 2) {
            ToggleMap();
        }
        ResetGrooveFlags();
        ServiceGameInRace();
        EnterUserMessage();
        ARService();
        ResetLollipopQueue();
        MungePalette();

        window_inactive = PDIsWindowInactive();
        if (window_inactive) {

            if ((tU32)PDGetTotalTime() > gPrev_task_switched_message + 1000) {
                gPrev_task_switched_message = GetTotalTime();
                NewTextHeadupSlot(4, 0, 500, -4, GetMiscString(eMiscString_task_switched));
            }
        } else {
            if (gWindow_inactive) {
                ClearHeadupSlot(4);
                ClearQueuedHeadups();
            }
        }
        gWindow_inactive = window_inactive;

        if (!gAction_replay_mode) {
#ifdef __vita__
            C2VitaPerfBegin(C2VP_AI);
#endif
            MungeAIWorld(gFrame_period);
#ifdef __vita__
            C2VitaPerfEnd(C2VP_AI);
#endif
            PollCarControls(gFrame_period);
        }

        if (!gAction_replay_mode) {
            CheckActiveCarList();

            Timers_Push(TIMER_PED);
#ifdef __vita__
            C2VitaPerfBegin(C2VP_PED);
#endif
            MungePedestrians();
#ifdef __vita__
            C2VitaPerfEnd(C2VP_PED);
#endif
            Timers_Pop(TIMER_PED);

            ControlOurCar(gFrame_period);

            Timers_Push(TIMER_PHS);
#ifdef __vita__
            C2VitaPerfBegin(C2VP_PHYSICS);
#endif
            ApplyPhysicsToCars(gLast_tick_count - gRace_start, gFrame_period);
#ifdef __vita__
            C2VitaPerfEnd(C2VP_PHYSICS);
#endif
            Timers_Pop(TIMER_PHS);

            MungeSomeOtherCarGraphics();
            PipeCarPositions();

            Timers_Push(TIMER_CRS);
            CrushBendFlapRend();
            Timers_Pop(TIMER_CRS);

            DoNetGameManagement();
            CheckRecoveryOfCars(gFrame_period + gLast_tick_count - gRace_start);
            NetSendMessageStacks();
        } else {
            DoActionReplay(gFrame_period);
        }
        PollCameraControls(gCamera_period);
        MungeEngineNoise();
        CalculateCameraStuff(gCamera_period);
        MungeNapalm();
        if (!gAction_replay_mode) {
            DoPowerupPeriodics(gFrame_period);
        }
        if (!gAction_replay_mode) {
            CheckCheckpoints();
            MungeEnvironmentalSound();
        }
        ChangingView();
        MungeCarGraphics(gFrame_period);
        FunkThoseTronics();
        GrooveThoseDelics();
        DoWheelDamage(gFrame_period);
        CalculateFrameRate();
        CameraBugFix(&gProgram_state.current_car, gCamera_period);
        if (!gAction_replay_mode) {
            /* MungeHeadups() inlined to match retail codegen */
            {
            char the_text[256];
            int net_credits;
            int effective_timer;
            int bonus;
            tU32 the_time;
            // GLOBAL: CARMA2_HW 0x0068c818
            static tU32 last_rattle_time;

            ClearHeadupSlot(3);
            MungeJoystickHeadups();
            net_credits = gProgram_state.credits;
            if (fabsf((float)gProgram_state.credits - (float)gLast_credit_headup__mainloop) / (float)gFrame_rate > 1.2f) {
                if (gProgram_state.credits - gLast_credit_headup__mainloop > 0) {
                    net_credits = (int)((float)gLast_credit_headup__mainloop + ((float)gProgram_state.credits - (float)gLast_credit_headup__mainloop + 1000.f) * (float)gFrame_period * 1.2f / 1000.f);
                } else {
                    net_credits = (int)((float)gLast_credit_headup__mainloop - ((float)gLast_credit_headup__mainloop - (float)gProgram_state.credits + 1000.f) * (float)gFrame_period * 1.2f / 1000.f);
                }
            }

            if (gNet_mode != eNet_mode_none
                    && gCountdown == 0
                    && gNet_auto_increase_credits_dt[gCurrent_net_game->type] != 0
                    && net_credits < gRecovery_cost.initial_network[gCurrent_net_game->type]
                    && PDGetTotalTime() - gLast_net_auto_increase_credits >= gNet_auto_increase_credits_dt[gCurrent_net_game->type]) {

                gLast_net_auto_increase_credits = PDGetTotalTime();
                gProgram_state.credits += 50;

        #define GET_RECOVERY_COST() ((gNet_mode == eNet_mode_none) ? \
            gRecovery_cost.initial[gProgram_state.skill_level] : \
            gRecovery_cost.initial_network[gCurrent_net_game->type])

                if (net_credits + 50 > GET_RECOVERY_COST()) {
                    gProgram_state.credits += (int)(GET_RECOVERY_COST() - net_credits - 50.f);
                    net_credits = (int)GET_RECOVERY_COST();
                } else {
                    net_credits += 50;
                }
            }
            gLast_credit_headup__mainloop = net_credits;

            if (gCountdown != 0) {
                int new_countdown;

                new_countdown = (int)(7.5 - GetRaceTime() / 1000.0);
                if (new_countdown < 0) {
                    new_countdown = 0;
                }
                if (gCountdown != new_countdown && new_countdown <= 5) {
                    gCountdown = new_countdown;
                    NewImageHeadupSlot(5, 0, 800, 4 + new_countdown);
                    DRS3StartSound(gPedestrians_outlet, eSoundId_Countdown_Start + gCountdown);
                    if (new_countdown == 0) {
                        MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo();
                    }
                }
            }
            if (fabsf((float)gTimer - (float)gLast_time_headup) / (float)gFrame_period <= 10.0) {
                effective_timer = gTimer;
            } else if ((int)gTimer < gLast_time_headup) {
                effective_timer = gTimer;
            } else {
                effective_timer = (int)((float)gLast_time_headup + 10.f * (float)gFrame_period);
            }
            gLast_time_headup = effective_timer;

            if (gNet_mode == eNet_mode_none) {
                if (net_credits < 0) {
                    sprintf(the_text, "\xf8%d\xfa %s", -net_credits, GetMiscString(eMiscString_credits_lost));
                    ChangeHeadupText(gCredits_won_headup, the_text);
                } else {
                    sprintf(the_text, "\xf8%d\xfa %s", net_credits, GetMiscString(net_credits >= 100000 ? eMiscString_credits_abbrev : eMiscString_credits_gained));
                    ChangeHeadupText(gCredits_won_headup, the_text);
                }
                if (gCurrent_race.race_spec->race_type == kRaceType_Carma1) {
                    sprintf(the_text, "\xf8%d\xfa/\xf8%d\xfa %s", gCount_killed_peds, gPed_count, GetMiscString(eMiscString_kills));
                    ChangeHeadupText(gPed_kill_count_headup, the_text);
                } else if (gCurrent_race.race_spec->race_type == kRaceType_Peds) {
                    sprintf(the_text, "\xf8%d\xfa/\xf8%d\xfa %s", gCount_killed_peds, gTotal_count_smash_peds, GetMiscString(eMiscString_kills));
                    ChangeHeadupText(gPed_kill_count_headup, the_text);
                }
                if (gCurrent_race.race_spec->race_type == kRaceType_Carma1
                        || gCurrent_race.race_spec->race_type == kRaceType_Cars) {

                    if (gQueued_wasted_messages_count != 0 && GetTotalTime() > gLast_wasted_message_start + 5000) {
                        LoseOldestWastedMessage();
                    }

                    if (gQueued_wasted_messages_count == 0) {
                        int count_oppos;

                        if (gCurrent_race.race_spec->race_type == kRaceType_Carma1) {
                            count_oppos = GetCarCount(eVehicle_opponent);
                        } else {
                            count_oppos = gCount_opponents;
                        }
                        sprintf(the_text, "%s \xf8%d\xfa/\xf8%d",
                            GetMiscString(eMiscString_wasted),
                            count_oppos - NumberOfOpponentsLeft(),
                            count_oppos);
                    } else {
                        if (Flash(150, &gWasted_last_flash, &gWasted_flash_state)) {
                            sprintf(the_text, "\xf9%s %s",
                                gOpponents[gQueued_wasted_messages[0]].abbrev_name,
                                GetMiscString(eMiscString_wasted));
                        } else {
                            sprintf(the_text, " ");
                        }
                    }
                    ChangeHeadupText(gCar_kill_count_headup, the_text);
                }

                TimerString(effective_timer, the_text, 1, 0, 0);
                ChangeHeadupText(gTimer_headup, the_text);

                if (gHeadup_detail_level == 1 || gHeadup_detail_level == 4) {
                    sprintf(gHeadup_oppo_ped_text, "%i  %i  %i",
                        GetCarCount(eVehicle_opponent) - NumberOfOpponentsLeft(),
                        net_credits,
                        gCount_killed_peds);
                }
                if (gCurrent_race.race_spec->race_type == kRaceType_Carma1
                        || gCurrent_race.race_spec->race_type == kRaceType_Checkpoints) {

                    sprintf(the_text, "%s \xf8%d\xfa/\xf8%d \xfa%s \xf8%d\xfa/\xf8%d",
                        GetMiscString(eMiscString_cp), gCheckpoint, gCheckpoint_count,
                        GetMiscString(eMiscString_lap), gLap,gTotal_laps);
                    ChangeHeadupText(gLaps_headup, the_text);
                }
                if (gCurrent_race.race_spec->race_type > 3) { /* kRaceType_Smash or kRaceType_SmashNPed */
                    sprintf(the_text, "%s \xf8%d\xfa/\xf8%d",
                        GetMiscString(eMiscString_targets),
                        GetRuntimeVariable(gCurrent_race.race_spec->options.smash.var_smash_number),
                        gCurrent_race.race_spec->options.smash.var_smash_target);
                    ChangeHeadupText(gLaps_headup, the_text);
                }

                the_time = GetTotalTime() - gTime_bonus_start;

                switch (gTime_bonus_state) {
                case eTime_bonus_initial_pause:
                    if (the_time >= 500) {
                        bonus = gCompletion_bonus_post_race;
                        sprintf(the_text, "%s %d", GetMiscString(eMiscString_completion_bonus_colon), bonus);
                        DRS3StartSound(gPedestrians_outlet, eSoundId_Clap);
                        ChangeHeadupText(gRace_bonus_headup, the_text);
                        gProgram_state.credits += bonus;
                        gTime_bonus_state = eTime_bonus_race_bonus;
                        gTime_bonus_start = GetTotalTime();
                    }
                    gRace_finished = 10000;
                    break;
                case eTime_bonus_race_bonus:
                    if (the_time >= 2000) {
                        gTime_bonus_state = eTime_bonus_tb_up;
                        gTime_bonus_start = GetTotalTime();
                        last_rattle_time = 0;
                    }
                    gRace_finished = 10000;
                    break;
                case eTime_bonus_tb_up:
                    if (gTimer != 0) {
                        if (the_time - last_rattle_time > 15) {
                            int previous_gtimer;

                            previous_gtimer = gTimer;
                            gTimer -= 1000 * ((the_time - last_rattle_time) / 15);
                            if (gTimer < 0) {
                                gTimer = 0;
                            }
                            gTime_bonus += (previous_gtimer - gTimer) / 1000 * gCredits_per_second_time_bonus[gProgram_state.skill_level];
                            last_rattle_time += 15 * ((the_time - last_rattle_time) / 15);
                        }
                        sprintf(the_text, "%s %d", GetMiscString(eMiscString_time_bonus_colon), gTime_bonus);
                        ChangeHeadupText(gTime_bonus_headup, the_text);
                    } else {
                        gTime_bonus_state = eTime_bonus_tb_pause;
                        gTime_bonus_start = GetTotalTime();
                        last_rattle_time = 0;
                    }
                    gRace_finished = 10000;
                    break;
                case eTime_bonus_tb_pause:
                    if (the_time >= 1000) {
                        gTime_bonus_state = eTime_bonus_tb_down;
                        gTime_bonus_start = GetTotalTime();
                        last_rattle_time = 0;
                    }
                    gRace_finished = 10000;
                    break;
                case eTime_bonus_tb_down:
                    if (gTime_bonus != 0) {
                        if (the_time - last_rattle_time > 15) {
                            bonus = gTime_bonus;
                            gTime_bonus -= (the_time - last_rattle_time) * gCredits_per_second_time_bonus[gProgram_state.skill_level] / 15;
                            if (gTime_bonus < 0) {
                                gTime_bonus = 0;
                            }
                            gProgram_state.credits += bonus - gTime_bonus;
                            last_rattle_time += 15 * ((the_time - last_rattle_time) / 15);
                        }
                        sprintf(the_text, "%s %d", GetMiscString(eMiscString_time_bonus_colon), gTime_bonus);
                        ChangeHeadupText(gTime_bonus_headup, the_text);
                    } else {
                        gTime_bonus_state = eTime_bonus_end_pause;
                        gTime_bonus_start = GetTotalTime();
                    }
                    gRace_finished = 10000;
                    break;
                case eTime_bonus_end_pause:
                    if (the_time >= 2000 && gRace_finished > 1) {
                        gRace_finished = 1;
                    }
                    break;
                default:
                    break;
                }
            } else {
                DoNetworkHeadups(net_credits);
            }
        }
            ProcessOilSpills(gFrame_period);
            MungeSmashEdgeTriggers(GetTotalTime());
        }
        MungeDelayedSideEffects();
        MungeShrapnel(gFrame_period);
        if (!gAction_replay_mode) {
            MungePowerupStuff(gFrame_period);
        }
        ChangeDepthEffect();
        ServiceGameInRace();
        EnterUserMessage();
        SkidsPerFrame();
        FlushSmashQueue(0);
        MungeGlassFragments();
        AnimateSky();
        MungeExplosions();
        LastChanceForPedEffects();
        DoLODCarModels();
        MungeLightingEffects();

        if (gWait_for_it != 0) {
            if (GetTotalTime() >= gWait_for_it) {
                gWait_for_it = 0;
            }
        }
        if (gWait_for_it == 0) {
            Timers_Force(TIMER_RQQ);
#ifdef __vita__
            C2VitaPerfBegin(C2VP_RENDER);
#endif
            RenderAFrame(1);
#ifdef __vita__
            C2VitaPerfEnd(C2VP_RENDER);
#endif
            Timers_Force(TIMER_OQQ);
        }
        DoComplexCarModels();
        CheckReplayTurnOn();
        if (!gRecover_car
                && gProgram_state.prog_status == eProg_game_ongoing
                && gPalette_fade_time == 0
                && (gNet_mode == eNet_mode_none
                        || !gAction_replay_mode
                        || gProgram_state.current_car.car_master_actor->t.t.translate.t.v[0] < 500.f)) {

            EnsureRenderPalette();
            EnsurePaletteUp();
        }

        if (KeyIsDown(0) && !gEntering_message) {

            WaitForNoKeys();
            if (!gAction_replay_mode) {
                tU32 start_menu_time;

                if (gMap_view == 2) {
                    ToggleMap();
                }
                start_menu_time = PDGetTotalTime();
                FadePaletteDown();
                ClearEntireScreen();
                GoingToInterfaceFromRace();
                DoOptionsMenu();
                GoingBackToRaceFromInterface();
                AddLostTime(PDGetTotalTime() - start_menu_time);
            } else {
                ToggleReplay(NULL, NULL);
            }
        }
        if (!gAction_replay_mode) {
            CheckTimer();
        } else {
            PollActionReplayControls(&gFrame_period, &gAverage_frame_period);
        }
        if (!gAction_replay_mode && gKnobbled_frame_period != 0) {
            while (GetTotalTime() - frame_start_time < gKnobbled_frame_period) {
                /* brr */
                /* FIXME: replace with udelay */
            }
        }
        if (!tried_to_allocate_AR) {
            tried_to_allocate_AR = 1;
            PrintMemoryDump(0, "JUST RENDERED 1ST STUFF");
            InitialiseActionReplay();
            PrintMemoryDump(0, "JUST ALLOCATED ACTION REPLAY BUFFER");
        }
        if (gNet_mode == eNet_mode_client && gAbandon_game) {
            gProgram_state.prog_status = eProg_idling;
            gAbandon_game = 0;
        }
        Timers_EndFrame();
#ifdef __vita__
        C2VitaPerfFinish();
#endif

        if (gProgram_state.prog_status != eProg_game_ongoing) {
            break;
        }
        if (MungeRaceFinished()) {
            break;
        }
        if (gAbandon_game || gHost_abandon_game) {
            break;
        }
    }
    if (gCredits_post_race >= 0) {
        gProgram_state.credits = gCredits_post_race + gTotal_bonus_post_race;
    }
    PHILDisable();
    PrintMemoryDump(0, "JUST EXITED MAINLOOP");
    FadePaletteDown();
    ClearEntireScreen();
    SuspendPendingFlic();
    RevertPalette();
    if (gMap_view == 2) {
        ToggleMap();
    }
    EnsureSpecialVolumesHidden();
    FadePaletteDown();
    NetPlayerStatusChanged(ePlayer_status_loading);
    if (gAdditional_actors != NULL && gAdditional_actors->children != NULL) {
        AutoSaveAdditionalStuff();
    }
    if (gProgram_state.prog_status == eProg_game_ongoing) {
        ResetCarScreens();
    }
    if (gAbandon_game && gNet_mode == eNet_mode_host && gRace_over_reason < 8 ) { /* FIXME: 8 == eRace_over_count */
        NetFinishRace(gCurrent_net_game, eRace_over_abandoned);
    }
    if (gAction_replay_mode) {
        ToggleReplay(NULL, NULL);
    }
    if (gHost_abandon_game || gProgram_state.prog_status != eProg_game_ongoing) {
        result = eRace_game_abandonned;
    } else if (gAbandon_game == 1) {
        result = eRace_aborted;
    } else if (gRace_over_reason == eRace_over_out_of_time || gRace_over_reason == eRace_over_6 || gRace_over_reason == eRace_over_7) {
        result = eRace_timed_out;
    } else {
        result = eRace_completed;
    }
    if (result < eRace_completed) {
        gProgram_state.redo_race_index = gProgram_state.current_race_index;
    } else {
        gProgram_state.redo_race_index = -1;
    }
    if (gNet_mode != eNet_mode_none
            && result == eRace_game_abandonned
            && !gAbandon_game
            && gProgram_state.prog_status != eProg_idling) {

        gProgram_state.prog_status = eProg_game_starting;
    }
    gAbandon_game = 0;
    gStart_race_sent = 0;
    gInitialised_grid = 0;
    gHost_abandon_game = 0;
    DRS3StopAllOutletSoundsExceptCDA();
    if (gNet_mode != eNet_mode_none) {
        int i;

        for (i = 0; i < gNumber_of_net_players; i++) {
            StopCarBeingIt(gNet_players[i].car);
        }
    }
    gProgram_state.racing = 0;
    if (gNet_mode == eNet_mode_host) {
        gCurrent_net_game->status.stage = eNet_game_stage0;
    }
    WaitForNoKeys();
    return result;
}

// FUNCTION: CARMA2_HW 0x00492830
tU32 C2_HOOK_FASTCALL GetLastTickCount(void) {
    return gLast_tick_count;
}

// FUNCTION: CARMA2_HW 0x00492950
tRace_result C2_HOOK_FASTCALL DoRace(void) {
    tRace_result result;

    if (PercentageChance(50)) {
        StartMusicTrack(9998);
    } else {
        StartMusicTrack(9997);
    }
    gRace_start = PDGetTotalTime();
    result = MainGameLoop();
    return result;
}

// FUNCTION: CARMA2_HW 0x00492680
void C2_HOOK_FASTCALL UpdateFramePeriod(tU32* pCamera_period) {
    tU32 new_tick_count;
    tU32 new_camera_tick_count;
    int error;

    if (gAction_replay_mode) {
        tU32 last_tick_count;

        last_tick_count = gLast_tick_count;
        gLast_tick_count = gLast_replay_frame_time;
        gFrame_period = abs((int)(gLast_replay_frame_time - last_tick_count));
        new_camera_tick_count = PDGetTotalTime();
        new_tick_count = GetTotalTime();
        *pCamera_period = (gOld_camera_time != 0) ? (new_camera_tick_count - gOld_camera_time) : 0;
        gOld_camera_time = new_camera_tick_count;
        if (gFrame_period != 0) {
            *pCamera_period = gFrame_period;
        }
    } else {
        int new_period;
        new_tick_count = PDGetTotalTime();
        gLast_tick_count += gFrame_period;
        error = new_tick_count - gLast_tick_count;
        gFrame_period = new_tick_count - gActual_last_tick_count;
        if (gFrame_period > 500 && new_tick_count - gRace_start > 2000) {
            gFrame_period = gAverage_frame_period / 10;
            gLast_tick_count = new_tick_count;
            if (gNet_mode != eNet_mode_none) {
                switch (gNet_mode) {
                case eNet_mode_host:
                    break;
                case eNet_mode_client:
                    gProgram_state.current_car.collision_info->field_0x49c = 0;
                    break;
                }
            }
        }
        gAverage_frame_period = 9 * gAverage_frame_period / 10 + gFrame_period;
        if (new_tick_count - gRace_start > 2000) {
            gFrame_period = gAverage_frame_period / 10;
        }
        new_period = gFrame_period + error;
        if (new_period <= 0) {
            gLast_tick_count = new_tick_count;
        } else {
            gFrame_period = new_period;
        }
        *pCamera_period = gFrame_period;
        gActual_last_tick_count = new_tick_count;
    }
    if (gFrame_period < 10) {
        gFrame_period = 10;
        gLast_tick_count = new_tick_count;
    } else if (gFrame_period > 1000) {
        gFrame_period = 1000;
        gLast_tick_count = new_tick_count;
    }
    if (*pCamera_period < 10) {
        *pCamera_period = 10;
    } else if (*pCamera_period > 1000 && 0) {
        *pCamera_period = 1000;
    }
}


void C2_HOOK_FASTCALL CheckTimer(void) {
    tS32 time_in_seconds;
    tS32 time_left;
    // GLOBAL: CARMA2_HW 0x0068c828
    static tU32 last_time_in_seconds;

    if (!gFreeze_timer && !gCountdown && !gRace_finished) {
        if (gFrame_period < gTimer) {
            if (gNet_mode == eNet_mode_none) {
                gTimer -= gFrame_period;
            }
            time_left = gTimer + 500;
            time_in_seconds = time_left / 1000;
            if (time_in_seconds != last_time_in_seconds && time_in_seconds <= 10) {
                DRS3StartSound(gIndexed_outlets[0], eSoundId_warning_buzzer_noise);
            }
            last_time_in_seconds = time_in_seconds;
        } else {
            gTimer = 0;
            RaceCompleted(eRace_over_out_of_time);
        }
    }
}

int C2_HOOK_FASTCALL MungeRaceFinished(void) {

    if (!gRace_finished || gAction_replay_mode || (gNet_mode != eNet_mode_none && gRace_over_reason == eRace_not_over_yet)) {
        return 0;
    }
    if (gNet_mode == eNet_mode_none && gRace_over_reason < 4 && gCredits_post_race < 0) {
        int completion_bonus;

        gCredits_post_race = gProgram_state.credits;
        if (gCurrent_race.race_spec->race_type == kRaceType_Carma1) {
            completion_bonus = gCurrent_race.completion_bonus[gProgram_state.skill_level];
        } else {
            /* FIXME: is it int completion_bonus[4][3] instead?*/
            switch (gRace_over_reason) {
            case eRace_over_0:
                completion_bonus = gCurrent_race.completion_bonus[gProgram_state.skill_level];
                break;
            case eRace_over_1:
                completion_bonus = gCurrent_race.completion_bonus_peds[gProgram_state.skill_level];
                break;
            case eRace_over_2:
                completion_bonus = gCurrent_race.completion_bonus_opponents[gProgram_state.skill_level];
                break;
            case eRace_over_3:
                /* FIXME: what completion_bonus type is this? */
                completion_bonus = gCurrent_race.completion_bonus_field_0x3c[gProgram_state.skill_level];
                break;
            default:
                CARPOCALYPSE2_UNREACHABLE();
                break;
            }
        }
        gCompletion_bonus_post_race = completion_bonus;
        gTotal_bonus_post_race = gCredits_per_second_time_bonus[gProgram_state.skill_level] * (gTimer / 1000)
                + gCompletion_bonus_post_race;
        if (!GetRuntimeVariable(99)) {
            NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(eMiscString_game_auto_saved));
        }
    }

    if (gRace_finished > gFrame_period) {
        gRace_finished -= gFrame_period;
    } else {
        if (gTimer == 0 || gNet_mode != eNet_mode_none || gRace_over_reason == eRace_over_6 || GetRuntimeVariable(99) != 0) {
            gRace_finished = 0;
            return 1;
        }
        gRace_finished = 15 * (gTimer + 300);
        gRace_bonus_headup = NewTextHeadupSlot(9, 0, 0, -4, "");
        gTime_bonus_headup = NewTextHeadupSlot(10, 0, 0, -4, "");
        gTime_bonus = 0;
        gTime_bonus_start = GetTotalTime();
        gTime_bonus_state = eTime_bonus_initial_pause;
    }
    return PDKeyDown(51) || PDKeyDown(52);
}

void C2_HOOK_FASTCALL LoseOldestWastedMessage(void) {
    int i;

    for (i = 1; i < gQueued_wasted_messages_count; i++) {
        gQueued_wasted_messages[i - 1] = gQueued_wasted_messages[i];
    }
    gQueued_wasted_messages_count -= 1;
    gLast_wasted_message_start = GetTotalTime();
}
// CalculateFrameRate

// LoseOldestWastedMassage

// FUNCTION: CARMA2_HW 0x00492620
void C2_HOOK_FASTCALL QueueWastedMessage(int pIndex) {

    if (gQueued_wasted_messages_count == CARPOCALYPSE2_ASIZE(gQueued_wasted_messages)) {
        int i;

        for (i = 1; i < gQueued_wasted_messages_count; i++) {
            gQueued_wasted_messages[i - 1] = gQueued_wasted_messages[i];
        }
        gQueued_wasted_messages_count = CARPOCALYPSE2_ASIZE(gQueued_wasted_messages) - 1;
        gLast_wasted_message_start = GetTotalTime();
    }
    if (gQueued_wasted_messages_count == 0) {
        gLast_wasted_message_start = GetTotalTime();
    }
    gQueued_wasted_messages[gQueued_wasted_messages_count] = pIndex;
    gQueued_wasted_messages_count++;
}

// MungeHeadups

// UpdateFramePeriod

// CheckTimer

// MungeRaceFinished

// CalculateCameraStuff

// CheckActiveCarList

// MainGameLoop

// DoRace


/* Rebuild after the Timers_Pop stack index correction. */
