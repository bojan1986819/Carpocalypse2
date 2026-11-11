#include "oil.h"

#include "finteray.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "loading.h"
#include "network.h"
#include "piping.h"
#include "utility.h"

#include <brender/brender.h>

#include "carpocalypse2_macros.h"
#include "oil.h"

// STUB: CARMA2_HW 0x004a6a10
// GLOBAL: CARMA2_HW 0x0065a398
const char* gOil_pixie_names[1] = {
    "OIL.PIX",
};

// GLOBAL: CARMA2_HW 0x00690c88
br_pixelmap* gOil_pixies[1];

// GLOBAL: CARMA2_HW 0x00690c90
tOil_spill_info gOily_spills[32];

// GLOBAL: CARMA2_HW 0x0074a68c
br_material* oily_material;

// GLOBAL: CARMA2_HW 0x00691710
int gNext_oil_pixie;

// FUNCTION: CARMA2_HW 0x004a6c50
void C2_HOOK_FASTCALL ResetOilSpills(void) {
    int i;

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
        gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        gOily_spills[i].car = NULL;
        gOily_spills[i].car_actor = NULL;
        gOily_spills[i].stop_time = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004a6c80
void C2_HOOK_FASTCALL QueueOilSpill2(
    tCar_spec* pCar,
    br_actor* pActor,
    br_model* pModel,
    int pSpill_time_offset,
    float pFull_size_min,
    float pFull_size_max,
    float pGrow_size_min,
    float pGrow_size_max,
    float pSize,
    float pField_0x48,
    float pField_0x4c,
    float pField_0x40,
    float pField_0x44,
    br_material* pMaterial,
    int pField_0x50)
{
    int i;
    int slot;
    int oldest;
    tU32 time_low;
    tU32 time_high;
    tU32 smallest_spill_time;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, car, 0xc);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, spill_time, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, current_size, 0x1c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x20, 0x20);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, grow_rate, 0x24);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x40, 0x40);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x44, 0x44);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x48, 0x48);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x4c, 0x4c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, field_0x50, 0x50);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(br_actor, material, 0x1c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(br_actor, render_style, 0x20);

    time_low = GetTotalTime();
    time_high = GetTotalTime();

    if (pCar != NULL) {
for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
            if (gOily_spills[i].car_actor == pActor
                    && time_low < gOily_spills[i].spill_time + 5000) {
                return;
            }
        }
    }

    slot = -1;
    oldest = 0;
    smallest_spill_time = time_high;
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
        if (gOily_spills[i].car_actor == NULL) {
            slot = i;
            break;
        }
        if (gOily_spills[i].spill_time < smallest_spill_time) {
            smallest_spill_time = gOily_spills[i].spill_time;
            oldest = i;
        }
    }
    if (slot < 0) {
        slot = oldest;
    }

    gOily_spills[slot].actor->material = pMaterial;
    gOily_spills[slot].car = pCar;
    gOily_spills[slot].car_actor = pActor;
    gOily_spills[slot].car_model = pModel;
    gOily_spills[slot].spill_time = time_low + pSpill_time_offset;
    gOily_spills[slot].field_0x20 = pSize;
    gOily_spills[slot].full_size = SRandomBetween(pFull_size_min, pFull_size_max);
    gOily_spills[slot].grow_rate = SRandomBetween(pGrow_size_min, pGrow_size_max);
    gOily_spills[slot].current_size = pSize;
    gOily_spills[slot].field_0x40 = pField_0x40;
    gOily_spills[slot].field_0x44 = pField_0x44;
    gOily_spills[slot].field_0x48 = pField_0x48;
    gOily_spills[slot].field_0x4c = pField_0x4c;
    gOily_spills[slot].field_0x50 = pField_0x50;
    /* Queued spills stay hidden until SetInitialOilStuff prepares the model. */
    gOily_spills[slot].actor->render_style = BR_RSTYLE_NONE;
}

// FUNCTION: CARMA2_HW 0x004a6dd0
void C2_HOOK_FASTCALL QueueOilSpill(tCar_spec* pCar) {
    QueueOilSpill2(
        pCar,
        pCar->car_master_actor,
        pCar->car_actor->model,
        500,
        0.35f,
        0.6f,
        3.0e-5f,
        0.0001f,
        0.1f,
        1.5f,
        2.5f,
        0.01f,
        0.15f,
        oily_material,
        0);
}

// FUNCTION: CARMA2_HW 0x004a78e0
void C2_HOOK_FASTCALL FUN_004a78e0(tNet_message_chunk* pChunk) {
    tCar_spec* car;

    car = NetCarFromPlayerID(pChunk->oil_spill.player);
    if (car != NULL) {
        QueueOilSpill2(
            car,
            car->car_master_actor,
            car->car_actor->model,
            500,
            0.35f,
            0.6f,
            3.0e-5f,
            0.0001f,
            0.1f,
            1.5f,
            2.5f,
            0.01f,
            0.15f,
            oily_material,
            0);
    }
}

// FUNCTION: CARMA2_HW 0x004a7460
void C2_HOOK_FASTCALL SetInitialOilStuff(tOil_spill_info* pOil, br_model* pModel) {
    /* Resize the spill in the ground plane without changing vertex heights. */
    pModel->vertices[0].p.v[0] = -pOil->field_0x20;
    pModel->vertices[0].p.v[2] = -pOil->field_0x20;
    pModel->vertices[1].p.v[0] = pOil->field_0x20;
    pModel->vertices[1].p.v[2] = -pOil->field_0x20;
    pModel->vertices[2].p.v[0] = pOil->field_0x20;
    pModel->vertices[2].p.v[2] = pOil->field_0x20;
    pModel->vertices[3].p.v[0] = -pOil->field_0x20;
    pModel->vertices[3].p.v[2] = pOil->field_0x20;
    pOil->actor->render_style = BR_RSTYLE_FACES;
    BrMaterialUpdate(pOil->actor->material, BR_MATU_ALL);
    BrModelUpdate(pModel, BR_MODU_ALL);
}

void C2_HOOK_FASTCALL MungeOilsHeightAeGround(tOil_spill_info* pThe_spill) {

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, actor, 0x0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, position, 0x34);

    BrVector3Copy(&pThe_spill->actor->t.t.translate.t, &pThe_spill->position);
}

int C2_HOOK_FASTCALL OKToSpillOil(tOil_spill_info* pOil) {
    br_scalar distance;
    br_scalar mr_dotty;
    br_vector3 pos;
    br_vector3 v;
    br_vector3 ray_pos;
    br_vector3 ray_dir;
    br_vector3 normal;
    tCar_spec* car;
    int i;
    int face_count;
    int found_one;
    br_angle angle_to_rotate_by;
    tBounds kev_bounds;
    tFace_ref the_list[10];
    tFace_ref* face_ref;

    car = pOil->car;
    if (car != NULL && car->driver >= 7 && car->damage_units[eDamage_engine].damage_level < 99 && car->damage_units[eDamage_transmission].damage_level < 99) {
        return 0;
    }
    angle_to_rotate_by = IRandomBetween(0, 0xffff);
    BrVector3Set(&kev_bounds.original_bounds.min,
        -pOil->full_size,
        (float)(pOil->car_model->bounds.min.v[1] - 1.0 / 6.9),
        -pOil->full_size);
    BrVector3Set(&kev_bounds.original_bounds.max,
        pOil->full_size,
        pOil->car_model->bounds.max.v[1],
        pOil->full_size);
    BrMatrix34PreRotateY(&pOil->actor->t.t.mat, angle_to_rotate_by);
    kev_bounds.mat = &pOil->car_actor->t.t.mat;
    face_count = FindFacesInBox(&kev_bounds, the_list, CARPOCALYPSE2_ASIZE(the_list), NULL);
    BrVector3Set(&v, .0f, .2f, .0f);
    BrVector3Add(&ray_pos, &pOil->car_actor->t.t.translate.t, &v);
    BrVector3Set(&ray_dir, 0.f, kev_bounds.original_bounds.min.v[1] - kev_bounds.original_bounds.max.v[1], 0.f);
    {
        br_scalar scale;

        scale = (br_scalar)sqrt(BR_SQR(ray_dir.v[1]));
        if (scale > 2 * BR_SCALAR_EPSILON) {
            scale = (br_scalar)(1.0 / scale);
            BrVector3Set(&ray_dir, 0.f, BR_MUL(ray_dir.v[1], scale), 0.f);
        } else {
            BrVector3Set(&ray_dir, 1.f, 0.f, 0.f);
        }
    }
    if (face_count == 0) {
        return 0;
    }
    found_one = 0;
    for (i = 0; i < face_count; i++) {
        face_ref = &the_list[i];
        if (!found_one) {
            CheckSingleFace(face_ref, &ray_pos, &ray_dir, &normal, &distance, &pos);
            if (distance < 100.f) {
                found_one = 1;
                BrVector3Copy(&pOil->actor->t.t.look_up.up, &normal);
                BrVector3Set(&v, 0.f, 0.f, 1.f);
                BrVector3Cross(&pOil->actor->t.t.look_up.look, &normal, &v);
                BrVector3Set(&v, -1.f, 0.f, 0.f);
                BrVector3Cross((br_vector3*)pOil->actor->t.t.mat.m[2], &normal, &v);
                /* Original uses the last two hit coordinates and ray origin X. */
                pOil->position.v[0] = pos.v[1];
                pOil->position.v[1] = pos.v[2];
                pOil->position.v[2] = ray_pos.v[0];
                BrMatrix34PreRotateY(&pOil->actor->t.t.mat, angle_to_rotate_by);
            }
        }
    }
    if (!found_one || normal.v[1] < .97f) {
        return 0;
    }
    for (i = 0; i < face_count; i++) {
        face_ref = &the_list[i];
        mr_dotty = BrVector3Dot(&face_ref->normal, &normal);
        if (mr_dotty < .98f && (mr_dotty > .8f || !NormalSideOfPlane(&pOil->actor->t.t.translate.t, &face_ref->normal, face_ref->d))) {
            return 0;
        }
    }
    return 1;
}

void C2_HOOK_FASTCALL MungeOilsHeightAboveGround(tOil_spill_info* pOil) {

    /* struct assignment (not BrVector3Copy): VC5 computes both addresses once, as the original */
    pOil->actor->t.t.translate.t = pOil->position;
}

// FUNCTION: CARMA2_HW 0x004a6e50
void C2_HOOK_FASTCALL ProcessOilSpills(tU32 pFrame_period) {
    int i;
    tU32 time;
    br_scalar this_size;
    br_vector3 v;
    tNet_message* message;
    tOil_spill_info* oil;
    br_model* the_model;
    br_scalar distance;
    br_scalar mr_dotty;
    br_vector3 pos;
    br_vector3 ray_pos;
    br_vector3 ray_dir;
    br_vector3 normal;
    tCar_spec* car;
    int face_i;
    int face_count;
    int found_one;
    br_angle angle_to_rotate_by;
    tBounds kev_bounds;
    tFace_ref the_list[10];
    tFace_ref* face_ref;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOil_spill_info, position, 0x34);
    /* VC5 cannot use anonymous-union member addresses in constant expressions. */
#ifndef CARPOCALYPSE2_MATCHING
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message, contents.oil_spill.player, 0x1c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message, contents.oil_spill.full_size, 0x20);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message, contents.oil_spill.grow_rate, 0x24);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message, contents.oil_spill.current_size, 0x28);
#endif

    time = GetTotalTime();
    i = 0;
    oil = gOily_spills;
    do {
        if (oil->car_actor == NULL) {
            goto hide_spill;
        }
        the_model = oil->actor->model;
        if (oil->actor->render_style != BR_RSTYLE_NONE
                || time < oil->spill_time
                || (oil->car != NULL
                    && (!(fabsf(oil->car->collision_info->v.v[0]) < (float)(1.0 / 6.9 / 100.0))
                        || !(fabsf(oil->car->collision_info->v.v[1]) < (float)(1.0 / 6.9 / 100.0))
                        || !(fabsf(oil->car->collision_info->v.v[2]) < (float)(1.0 / 6.9 / 100.0))))) {
            goto grow_spill;
        }
        if (gAction_replay_mode) {
            SetInitialOilStuff(oil, the_model);
            goto position_spill;
        }
        car = oil->car;
        if (car != NULL && car->driver >= 7 && car->damage_units[eDamage_engine].damage_level < 99 && car->damage_units[eDamage_transmission].damage_level < 99) {
            goto spill_failed;
        }
        angle_to_rotate_by = IRandomBetween(0, 0xffff);
        BrVector3Set(&kev_bounds.original_bounds.min,
            -oil->full_size,
            (float)(oil->car_model->bounds.min.v[1] - 1.0 / 6.9),
            -oil->full_size);
        BrVector3Set(&kev_bounds.original_bounds.max,
            oil->full_size,
            oil->car_model->bounds.max.v[1],
            oil->full_size);
        BrMatrix34PreRotateY(&oil->actor->t.t.mat, angle_to_rotate_by);
        kev_bounds.mat = &oil->car_actor->t.t.mat;
        face_count = FindFacesInBox(&kev_bounds, the_list, CARPOCALYPSE2_ASIZE(the_list), NULL);
        ray_pos = oil->car_actor->t.t.translate.t;
        ray_pos.v[1] -= -0.2f;
        BrVector3Set(&ray_dir, 0.f, kev_bounds.original_bounds.min.v[1] - kev_bounds.original_bounds.max.v[1], 0.f);
        {
            br_scalar scale;

            scale = (br_scalar)sqrt(BR_SQR(ray_dir.v[1]));
            if (scale > 2 * BR_SCALAR_EPSILON) {
                scale = (br_scalar)(1.0 / scale);
                BrVector3Set(&ray_dir, 0.f, BR_MUL(ray_dir.v[1], scale), 0.f);
            } else {
                BrVector3Set(&ray_dir, 1.f, 0.f, 0.f);
            }
        }
        if (face_count == 0) {
            goto spill_failed;
        }
        found_one = 0;
        for (face_i = 0; face_i < face_count; face_i++) {
            face_ref = &the_list[face_i];
            if (!found_one) {
                CheckSingleFace(face_ref, &ray_pos, &ray_dir, &normal, &distance, &pos);
                if (distance < 100.f) {
                    found_one = 1;
                    BrVector3Copy(&oil->actor->t.t.look_up.up, &normal);
                    oil->actor->t.t.mat.m[0][0] = normal.v[1];
                    oil->actor->t.t.mat.m[0][1] = -normal.v[0];
                    oil->actor->t.t.mat.m[0][2] = 0.f;
                    oil->actor->t.t.mat.m[2][0] = 0.f;
                    oil->actor->t.t.mat.m[2][1] = -normal.v[2];
                    oil->actor->t.t.mat.m[2][2] = normal.v[1];
                    /* Original uses the last two hit coordinates and ray origin X. */
                    oil->position.v[0] = pos.v[1];
                    oil->position.v[1] = pos.v[2];
                    oil->position.v[2] = ray_pos.v[0];
                    BrMatrix34PreRotateY(&oil->actor->t.t.mat, angle_to_rotate_by);
                }
            }
        }
        if (!found_one || normal.v[1] < .97f) {
            goto spill_failed;
        }
        for (face_i = 0; face_i < face_count; face_i++) {
            face_ref = &the_list[face_i];
            mr_dotty = BrVector3Dot(&face_ref->normal, &normal);
            if (mr_dotty < .98f && (mr_dotty > .8f || !NormalSideOfPlane(&oil->actor->t.t.translate.t, &face_ref->normal, face_ref->d))) {
                goto spill_failed;
            }
        }
        oil->spill_time = time;
        if (gNext_oil_pixie > 0) { /* CARPOCALYPSE2_ASIZE(gOil_pixies) */
            gNext_oil_pixie = 0;
        }
        if (oil->car == NULL) {
            oil->original_pos = oil->car_actor->t.t.translate.t;
        } else {
            oil->original_pos = oil->car->pos;
        }
        PipeSingleOilSpill(i,
            &oil->actor->t.t.mat,
            oil->full_size,
            oil->grow_rate,
            oil->spill_time,
            oil->stop_time,
            oil->car,
            &oil->original_pos,
            oil->actor->material->colour_map);
        oil->stop_time = 0;
        SetInitialOilStuff(oil, the_model);
        if (gNet_mode == eNet_mode_host && oil->car != NULL) {
            message = NetBuildGuaranteedMessage(31, 0);
            message->contents.oil_spill.player = NetPlayerFromCar(oil->car)->ID;
            message->contents.oil_spill.full_size = oil->full_size;
            message->contents.oil_spill.grow_rate = oil->grow_rate;
            message->contents.oil_spill.current_size = oil->current_size;
            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
        }
        goto position_spill;

spill_failed:
        oil->car = NULL;
        oil->car_actor = NULL;
        goto position_spill;

grow_spill:
        if (oil->actor->render_style != BR_RSTYLE_FACES
                || (oil->stop_time != 0 && time >= oil->stop_time)) {
            goto position_spill;
        }
        if (oil->car != NULL) {
            BrVector3Sub(&v, &oil->original_pos, &oil->car->pos);
            if (oil->stop_time == 0 && BrVector3LengthSquared(&v) > 0.2f) {
                oil->stop_time = time;
                goto next_spill;
            }
        }
        this_size = oil->field_0x20 + (tS32)(time - oil->spill_time) * oil->grow_rate;
        if (this_size >= oil->field_0x20) {
            oil->actor->render_style = BR_RSTYLE_FACES;
            if (this_size <= oil->full_size) {
                the_model->vertices[0].p.v[0] = -this_size;
                the_model->vertices[0].p.v[2] = -this_size;
                the_model->vertices[1].p.v[0] =  this_size;
                the_model->vertices[1].p.v[2] = -this_size;
                the_model->vertices[2].p.v[0] =  this_size;
                the_model->vertices[2].p.v[2] =  this_size;
                the_model->vertices[3].p.v[0] = -this_size;
                the_model->vertices[3].p.v[2] =  this_size;
                oil->current_size = this_size;
            } else {
                the_model->vertices[0].p.v[0] = -oil->full_size;
                the_model->vertices[0].p.v[2] = -oil->full_size;
                the_model->vertices[1].p.v[0] =  oil->full_size;
                the_model->vertices[1].p.v[2] = -oil->full_size;
                the_model->vertices[2].p.v[0] =  oil->full_size;
                the_model->vertices[2].p.v[2] =  oil->full_size;
                the_model->vertices[3].p.v[0] = -oil->full_size;
                the_model->vertices[3].p.v[2] =  oil->full_size;
                oil->current_size = oil->full_size;
            }
            BrModelUpdate(the_model, BR_MODU_ALL);
        } else {
            oil->actor->render_style = BR_RSTYLE_NONE;
        }
        goto position_spill;

hide_spill:
        oil->actor->render_style = BR_RSTYLE_NONE;
position_spill:
        if (oil->actor->render_style == BR_RSTYLE_FACES) {
            oil->actor->t.t.translate.t = oil->position;
        }
next_spill:
        i++;
        oil++;
    } while (oil < gOily_spills + CARPOCALYPSE2_ASIZE(gOily_spills));
}

// FUNCTION: CARMA2_HW 0x004a7860
void C2_HOOK_FAKE_THISCALL InitOilSpillSlot(int pIndex, const br_matrix34* pMatrix, float pFull_size, float pGrow_rate, tU32 pSpill_time, tU32 pUnused, tCar_spec* pCar, const br_vector3* pPos, tU32 pUnused2) {
    BrMatrix34Copy(&gOily_spills[pIndex].actor->t.t.mat, pMatrix);
    gOily_spills[pIndex].full_size = pFull_size;
    gOily_spills[pIndex].grow_rate = pGrow_rate;
    gOily_spills[pIndex].spill_time = pSpill_time;
    gOily_spills[pIndex].stop_time = 0;
    gOily_spills[pIndex].car = pCar;
    gOily_spills[pIndex].original_pos.v[0] = pPos->v[0];
    gOily_spills[pIndex].original_pos.v[1] = pPos->v[1];
    gOily_spills[pIndex].original_pos.v[2] = pPos->v[2];
    gOily_spills[pIndex].actor->render_style = 1;
}

// FUNCTION: CARMA2_HW 0x004a74e0
int C2_HOOK_FASTCALL GetOilSpillCount(void) {

    return CARPOCALYPSE2_ASIZE(gOily_spills);
}

// FUNCTION: CARMA2_HW 0x004a74f0
void C2_HOOK_FASTCALL GetOilSpillDetails(int pIndex, br_actor** pActor, br_scalar* pSize) {
    if (gOily_spills[pIndex].car_actor != NULL) {
        *pActor = gOily_spills[pIndex].actor;
        *pSize = gOily_spills[pIndex].full_size;
    } else {
        *pActor = NULL;
    }
}

// C2PORT-NONMATCHING: best 55.12% | difftest PASS 2000 trials (spec difftest/specs/004a7530_GetOilFrictionFactors.json) | reason register-allocation,stack-layout
// FUNCTION: CARMA2_HW 0x004a7530
void C2_HOOK_FASTCALL GetOilFrictionFactors(tCar_spec* pCar, br_scalar* pFl, br_scalar* pFr, br_scalar* pRl, br_scalar* pRr) {
    int i;

    *pFl = 1.f;
    *pFr = 1.f;
    *pRl = 1.f;
    *pRr = 1.f;

    if (pCar != NULL && pCar->driver > 5) {
        if (pCar->shadow_intersection_flags != 0) {
            for (i = 0; i < CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
                tOil_spill_info* oil;

                oil = &gOily_spills[i];
                if (oil->field_0x4c != 0.f && (pCar->shadow_intersection_flags & (1 << i)) != 0) {
                    br_vector3 point;
                    br_matrix34* mat;
                    br_scalar random_min;
                    br_scalar random_max;
                    br_scalar factor_min;
                    br_scalar factor_max;

                    mat = &pCar->car_master_actor->t.t.mat;
                    random_min = oil->field_0x48;
                    random_max = oil->field_0x4c;
                    factor_min = oil->field_0x40;
                    factor_max = oil->field_0x44;

#define CHECK_OILY_WHEEL(wpos_index, oil_index) do { \
                    BrMatrix34ApplyP(&point, &pCar->wpos[wpos_index], mat); \
                    if (PointInSpill(&point, i)) { \
                        pCar->oil_remaining[oil_index] = SRandomBetween(random_min, random_max); \
                        pCar->maybe_blood_remaining[oil_index] = SRandomBetween(factor_min, factor_max); \
                        pCar->field_0x16fc[oil_index] = (br_material*)oil->field_0x50; \
                    } \
                } while (0)

                    CHECK_OILY_WHEEL(2, 2);
                    CHECK_OILY_WHEEL(3, 3);
                    CHECK_OILY_WHEEL(0, 0);
                    CHECK_OILY_WHEEL(1, 1);

#undef CHECK_OILY_WHEEL
                }
            }
        }
        if (pCar->oil_remaining[2] != 0.f) {
            *pFl = pCar->maybe_blood_remaining[2];
        }
        if (pCar->oil_remaining[3] != 0.f) {
            *pFr = pCar->maybe_blood_remaining[3];
        }
        if (pCar->oil_remaining[0] != 0.f) {
            *pRl = pCar->maybe_blood_remaining[0];
        }
        if (pCar->oil_remaining[1] != 0.f) {
            *pRr = pCar->maybe_blood_remaining[1];
        }
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 68.09% | difftest PASS 2000 trials (spec difftest/specs/004a77d0_PointInSpill.json) | reason x87-scheduling
// FUNCTION: CARMA2_HW 0x004a77d0
int C2_HOOK_FASTCALL PointInSpill(br_vector3* pV, int pSpill) {
    br_actor* actor;
    float diff;

    actor = gOily_spills[pSpill].actor;
    diff = pV->v[0] / WORLD_SCALE - actor->t.t.translate.t.v[0];
    if (diff * diff < gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f
            && (diff = pV->v[2] / WORLD_SCALE - actor->t.t.translate.t.v[2], diff * diff < gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f)
            && fabsf(pV->v[1] / WORLD_SCALE - actor->t.t.translate.t.v[1]) < 0.1f) {
        return 1;
    }
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004a7940
void C2_HOOK_FASTCALL FUN_004a7940(tCar_spec* pCar) {
    int i;

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
        if (gOily_spills[i].car_actor != NULL && gOily_spills[i].car == pCar) {
            gOily_spills[i].car = NULL;
            gOily_spills[i].car_actor = NULL;
        }
    }
}

// FUNCTION: CARMA2_HW 0x004a6e20
void C2_HOOK_FASTCALL MungeIndexedOilsHeightAboveGround(int pIndex) {

    MungeOilsHeightAboveGround(&gOily_spills[pIndex]);
}
// FUNCTION: CARMA2_HW 0x004a6a10
void C2_HOOK_FASTCALL InitOilSpills(void) {
    int i;
    br_model* the_model;

    C2_HOOK_BUG_ON(sizeof(tOil_spill_info) != 0x54);

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gOil_pixie_names); i++) {
        gOil_pixies[i] = LoadPixelmap(gOil_pixie_names[i]);
        BrMapAdd(gOil_pixies[i]);
    }

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gOily_spills); i++) {
        oily_material = BrMaterialAllocate(NULL);
        BrMaterialAdd(oily_material);
        oily_material->flags |= BR_MATF_LIGHT;
        oily_material->flags |= BR_MATF_PERSPECTIVE;
        oily_material->flags |= BR_MATF_SMOOTH;
        oily_material->ka = 0.99f;
        oily_material->kd = 0.0f;
        oily_material->ks = 0.0f;
        oily_material->power = 0.0f;
        oily_material->index_base = 0;
        oily_material->index_range = 0;
        oily_material->colour_map = gOil_pixies[0];
        BrMatrix23Identity(&oily_material->map_transform);
        oily_material->index_shade = BrTableFind("IDENTITY.TAB");
        GlorifyMaterial(&oily_material, 1, kRendererShadingType_Specular);
        BrMaterialUpdate(oily_material, BR_MATU_ALL);
        the_model = BrModelAllocate(NULL, 4, 2);
        the_model->flags |= BR_MODF_KEEP_ORIGINAL;

        the_model->faces[0].vertices[0] = 2;
        the_model->faces[0].vertices[1] = 1;
        the_model->faces[0].vertices[2] = 0;
        the_model->faces[0].material = NULL;
        the_model->faces[0].smoothing = 1;

        the_model->faces[1].vertices[0] = 3;
        the_model->faces[1].vertices[1] = 2;
        the_model->faces[1].vertices[2] = 0;
        the_model->faces[1].material = NULL;
        the_model->faces[1].smoothing = 1;

        BrVector3Set(&the_model->vertices[0].p, -1.f, 0.f, -1.f);
        BrVector2Set(&the_model->vertices[0].map, 0.f, 1.f);
        BrVector3Set(&the_model->vertices[1].p, 1.f, 0.f, 1.f);
        BrVector2Set(&the_model->vertices[1].map, 0.f, 0.f);
        BrVector3Set(&the_model->vertices[2].p, 1.f, 0.f, -1.f);
        BrVector2Set(&the_model->vertices[2].map, 1.f, 0.f);
        BrVector3Set(&the_model->vertices[3].p, -1.f, 0.f, 1.f);
        BrVector2Set(&the_model->vertices[3].map, 1.f, 1.f);
        gOily_spills[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gOily_spills[i].actor->model = the_model;
        gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        gOily_spills[i].actor->material = oily_material;
        BrActorAdd(gOther_selfs[2], gOily_spills[i].actor);
    }
}

// ResetOilSpills

// QueueOilSpill2

// QueueOilSpill

// OKToSpillOil

// MungeOilsHeightAboveGround

// MungeIndexedOilsHeightAboveGround

// SetInitialOilStuff

// ProcessOilSpills

// GetOilSpillCount

// GetOilSpillDetails

// PointInSpill

// GetOilFrictionFactors

// AdjustOilSpill

// ReceivedOilSpill

// RemoveCarsOilSlicks

