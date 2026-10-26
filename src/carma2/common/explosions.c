#include "explosions.h"

#include "animation.h"
#include "errors.h"
#include "globvars.h"
#include "graphics.h"
#include "loading.h"
#include "piping.h"
#include "smashing.h"
#include "utility.h"
#include "world.h"

#include "brender/brender.h"

#include "c2_hooks.h"
#include "c2_string.h"

#include "carpocalypse2_macros.h"
// GLOBAL: CARMA2_HW 0x006a55c8
extern tExplosion gExplosions[50];
extern const double gDouble_0058aeb8;
// GLOBAL: CARMA2_HW 0x006a6d40
// GLOBAL: CARMA2_HW 0x006a6d40
tSmash_explosion gSmash_explosions[20];

// FUNCTION: CARMA2_HW 0x004eaac0
void C2_HOOK_FASTCALL ResetExplosions(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gExplosions) != 50);
    C2_HOOK_BUG_ON(sizeof(gExplosions[0]) != 0x78);
    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gSmash_explosions) != 20);
    C2_HOOK_BUG_ON(sizeof(gSmash_explosions[0]) != 0xc8);

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gExplosions); i++) {
        gExplosions[i].start = 0;
        gExplosions[i].finished = 0;
    }
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gSmash_explosions); i++) {
        gSmash_explosions[i].active = 0;
    }
}

// FUNCTION: CARMA2_HW 0x004f5ec0
void C2_HOOK_FASTCALL ReadExplosionInfo(FILE* pF, int* pChance_explosion, int* pExplosion_sound_id, tExplosion_animation* pExplosion_groups) {
    tPath_name the_path;
    char* name;
    char s[256];

    /* Wasted explosion settings */

    /* Percentage chance of car exploding */
    *pChance_explosion = GetAnInt(pF);
    /* Sound ID */
    *pExplosion_sound_id = GetAnInt(pF);

    PathCat(the_path, gApplication_path, "COMMON");
    /* Name of pixelmap file */
    GetALineAndDontArgue(pF, s);
    name = strtok(s, "\t ,/");
    name[strlen(name) - 4] = '\0';
    PathCat(the_path, the_path, name);
    LoadAllImagesInDirectory(&gMisc_storage_space, the_path);
    ReadExplosion(pF, pExplosion_groups);
}

// FUNCTION: CARMA2_HW 0x004f5f60
void C2_HOOK_FAKE_THISCALL FUN_004f5f60(void* pThis, undefined4 pArg2, undefined4 pValue) {

    CARPOCALYPSE2_THISCALL_UNUSED(pArg2);
    *(undefined4*)((char*)pThis + 0x18) = pValue;
}

int C2_HOOK_FAKE_THISCALL PointOutOfSightNotAR(const br_vector3* pPoint, undefined4 pArg2, float pMax_distance) {

    if (gAction_replay_mode) {
        return 1;
    }
    return PointOutOfSight(pPoint, pArg2, pMax_distance);
}

// FUNCTION: CARMA2_HW 0x004eaaf0
void C2_HOOK_FASTCALL MungeExplosions(void) {
    int i;
    tU32 now;

    now = GetTotalTime();
    for (i = 0; i < CARPOCALYPSE2_ASIZE(gExplosions); i++) {
        tExplosion* explosion = &gExplosions[i];
        int frame;
        undefined prev_frame;
        br_vector3 tv;

        if (explosion->start == 0) {
            continue;
        }
        frame = (now - explosion->start) / explosion->period;
        prev_frame = explosion->field_0x8;
        if (now < explosion->start) {
            if (explosion->actor->parent != NULL) {
                BrActorRemove(explosion->actor);
            }
            continue;
        }
        if (frame >= explosion->count_frames) {
            if (explosion->actor->parent != NULL) {
                BrActorRemove(explosion->actor);
            }
            explosion->finished = 1;
            continue;
        }
        if (PointOutOfSightNotAR(&explosion->collision_actor->t.t.translate.t CARPOCALYPSE2_THISCALL_EDX, 0.f)) {
            if (explosion->actor->parent != NULL) {
                BrActorRemove(explosion->actor);
            }
            continue;
        }
        explosion->field_0x8 = frame;
        if (explosion->actor->parent == NULL) {
            BrActorAdd(gNon_track_actor, explosion->actor);
        }
        if (frame != prev_frame) {
            explosion->actor->material->colour_map = explosion->frames[frame].map;
            BrMaterialUpdate(explosion->actor->material, BR_MATU_ALL);
        }
        if (explosion->actor->material->colour_map == NULL) {
            if (explosion->actor->parent != NULL) {
                BrActorRemove(explosion->actor);
            }
            continue;
        }
        explosion->actor->t.t.mat = gCamera->t.t.mat;
        BrMatrix34ApplyV(&tv, &explosion->field_0x1c, &explosion->collision_actor->t.t.mat);
        BrVector3Accumulate(&explosion->actor->t.t.translate.t, &tv);
        BrMatrix34PreScale(&explosion->actor->t.t.mat,
            (float)explosion->actor->material->colour_map->width * explosion->field_0x14 / 128.f,
            (float)explosion->actor->material->colour_map->height * explosion->field_0x14 / 128.f,
            1.f);
        BrMatrix34PreRotateZ(&explosion->actor->t.t.mat, explosion->angle);
    }
}

// GLOBAL: CARMA2_HW 0x006a82a0
int gNext_explosion_slot;

// FUNCTION: CARMA2_HW 0x004c8920
void C2_HOOK_FASTCALL CreateExplosionBillboard(int pIndex, tExplosion* pExplosion) {

    ARDoSingleVariedSession(0x1f, pIndex, 1, 0x78, 0, pExplosion);
}

// C2PORT-NONMATCHING: best 59.74% | difftest PASS 2000 trials (spec difftest/specs/004c8980_FUN_004c8980.json) | reason regalloc-memcpy
// FUNCTION: CARMA2_HW 0x004c8980
void C2_HOOK_FASTCALL FUN_004c8980(uintptr_t pOwner, tU32 pCount, const void* pData, undefined4 pValue) {
    tU32 uVar3;
    undefined4* piVar1;

    piVar1 = gCrush_space;
    if (piVar1 != NULL) {
        piVar1[1] = pCount;
        piVar1[0] = pValue;
        uVar3 = pCount * 8;
        memcpy(piVar1 + 2, pData, uVar3);
        uVar3 += 8;
        if (uVar3 > 0x4000) {
            FatalError(kFatalError_ChunkIsTooLargeToPipe);
        }
        FUN_00402ab0(0x22, pOwner, piVar1, uVar3);
    }
}

// FUNCTION: CARMA2_HW 0x004c89f0
void C2_HOOK_FASTCALL FUN_004c89f0(uintptr_t pOwner) {

    return FUN_00402ab0(0x23, pOwner, &gZero_vector__smash, sizeof(gZero_vector__smash.v[0]));
}

// FUNCTION: CARMA2_HW 0x004eace0
void C2_HOOK_FASTCALL FUN_004eace0(int pIndex, tExplosion* pExplosion) {

    gExplosions[pIndex] = *pExplosion;
}

// C2PORT-NONMATCHING: best 18.47% | difftest PASS 2000 trials (spec difftest/specs/004ead00_Explode.json) | reason compiler codegen after behavior fixes
// FUNCTION: CARMA2_HW 0x004ead00
void C2_HOOK_FASTCALL Explode(tExplosion_animation* pExplosion_animation, br_actor* pActor, br_bounds3* pBounds, br_vector3* pPos) {
    int i;
    int j;
    int k;
    int count;
    tU32 now;
    tExplosion_animation_group* group;
    float min_sx;
    float max_sx;
    float min_sy;
    float max_sy;
    float min_sz;
    float max_sz;
    float rx;
    float ry;
    float rz;
    float px;
    float py;
    float pz;
    float scaling;
    int frametime;
    int start_delay;
    int angle;
    tExplosion* slot;
    int slot_idx;

    now = GetTotalTime();
    if (pActor == NULL) {
        return;
    }
    for (i = 0; i < pExplosion_animation->count_groups; i++) {
        group = &pExplosion_animation->groups[i];
        count = IRandomBetween(group->min_count, group->max_count);
        min_sx = group->min_xyz_factor.v[0] * pBounds->max.v[0];
        max_sx = group->max_xyz_factor.v[0] * pBounds->max.v[0];
        min_sy = group->min_xyz_factor.v[1] * pBounds->max.v[1];
        max_sy = group->max_xyz_factor.v[1] * pBounds->max.v[1];
        min_sz = group->min_xyz_factor.v[2] * pBounds->max.v[2];
        max_sz = group->max_xyz_factor.v[2] * pBounds->max.v[2];
        if (count <= 0) {
            continue;
        }
        for (j = 0; j < count; j++) {
            rx = FRandomBetween(min_sx, max_sx);
            if (!PercentageChance(50)) {
                rx = -rx;
            }
            ry = FRandomBetween(min_sy, max_sy);
            if (!PercentageChance(50)) {
                ry = -ry;
            }
            rz = FRandomBetween(min_sz, max_sz);
            if (!PercentageChance(50)) {
                rz = -rz;
            }
            if (pPos != NULL) {
                px = rx + pPos->v[0] + group->offset.v[0];
                py = ry + pPos->v[1] + group->offset.v[1];
                pz = rz + pPos->v[2] + group->offset.v[2];
            } else {
                px = rx + group->offset.v[0];
                py = ry + group->offset.v[1];
                pz = rz + group->offset.v[2];
            }
            if (group->rotate_modus != kExplosionRotateModus_NoRotate) {
                angle = IRandomBetween(0, 0xffff);
            } else {
                angle = 0;
            }
            scaling = FRandomBetween(group->min_scaling_factor, group->max_scaling_factor);
            frametime = IRandomBetween(group->min_frametime_ms, group->max_frametime_ms);
            start_delay = IRandomBetween(group->min_start_delay_ms, group->max_start_delay_ms);
            slot = NULL;
            for (k = 0; k < (int)CARPOCALYPSE2_ASIZE(gExplosions); k++) {
                if (gExplosions[k].start == 0) {
                    slot = &gExplosions[k];
                    slot_idx = 1;
                    break;
                }
            }
            if (slot == NULL) {
                for (k = 0; k < (int)CARPOCALYPSE2_ASIZE(gExplosions); k++) {
                    gNext_explosion_slot++;
                    if (gNext_explosion_slot >= (int)CARPOCALYPSE2_ASIZE(gExplosions)) {
                        gNext_explosion_slot = 0;
                    }
                    slot = &gExplosions[gNext_explosion_slot];
                    if (slot->finished != 0) {
                        break;
                    }
                }
                if (k >= (int)CARPOCALYPSE2_ASIZE(gExplosions)) {
                    return;
                }
            }
            slot->start = now + start_delay;
            slot->period = frametime;
            slot->count_frames = group->count_frames;
            slot->finished = 0;
            slot->collision_actor = pActor;
            slot->field_0x14 = scaling * gDouble_0058aeb8;
            slot->field_0x8 = 0xff;
            slot->angle = angle;
            slot->field_0x1c.v[0] = px;
            slot->field_0x1c.v[1] = py;
            slot->field_0x1c.v[2] = pz;
            for (k = 0; k < group->count_frames; k++) {
                slot->frames[k].map = group->frames[k].map;
                slot->frames[k].opacity = group->frames[k].opacity;
            }
            CreateExplosionBillboard((int)(slot - gExplosions), slot);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004eb120
void C2_HOOK_FASTCALL FUN_004eb120(tSmash_vertex* pFragment, tSmashable_initial_speed_spec* pInitial_speed, br_vector3* pDirection, float pScale, int* pTime_range, tU32 pTime, br_vector3* pRandom_direction, br_vector3* pArg7, br_vector3* pArg8) {
    ApplyInitialMovement((undefined4*)pInitial_speed, pDirection, &pFragment->v, &pFragment->omega, pScale, pRandom_direction, pArg7, pArg8);
    pFragment->field_0xc = pTime;
    pFragment->time_last_move = pTime;
    pFragment->end_time = pTime + pTime_range[1];
    pFragment->field_0x10 = pTime + IRandomBetween(pTime_range[0], pTime_range[1]);
}

// FUNCTION: CARMA2_HW 0x004f5f80
void C2_HOOK_FASTCALL FUN_004f5f80(char* pArg) {

    pArg[0x20] = 1;
}
// FUNCTION: CARMA2_HW 0x004f5f90
void C2_HOOK_FASTCALL FUN_004f5f90(br_model* pModel, int pCount) {

    pModel->nfaces -= pCount;
    DRModelUpdateAndKevificateMaterials(pModel, BR_MODU_ALL);
}

// FUNCTION: CARMA2_HW 0x004f5fa0
void C2_HOOK_FASTCALL AddFacesToModel(br_model* pModel, int pFace_count, const tU16* pFace_data, br_material* pMaterial) {
    br_face* face;
    const tU16* face_data;
    int i;

    face = &pModel->faces[pModel->nfaces];
    if (pFace_count > 0) {
        face_data = pFace_data;
        i = pFace_count;
        do {
            face->material = pMaterial;
            face->vertices[0] = face_data[0];
            face->vertices[1] = face_data[1];
            face->vertices[2] = face_data[2];
            face->smoothing = face_data[3];
            face->flags = 0;
            face++;
            face_data += 4;
            i--;
        } while (i != 0);
    }
    pModel->nfaces += (br_int_16)pFace_count;
    DRModelUpdateAndKevificateMaterials(pModel, BR_MODU_ALL);
}

// FUNCTION: CARMA2_HW 0x004eb020
void C2_HOOK_FASTCALL RemoveExplosionsForCar(tCar_spec* pCar) {
    int i;
    tExplosion* explosion;

    explosion = gExplosions;
    i = 50;
    do {
        if (explosion->start != 0 && explosion->collision_actor == pCar->collision_info->actor) {
            if (explosion->actor->parent != NULL) {
                BrActorRemove(explosion->actor);
            }
            explosion->finished = 1;
        }
        explosion++;
        i--;
    } while (i != 0);
}

