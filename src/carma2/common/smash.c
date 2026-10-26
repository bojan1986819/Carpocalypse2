#include "smash.h"
#include "smashing.h"
#include "world.h"
#include "physics.h"
#include "globvrpb.h"
#include "car.h"
#include "crush.h"
#include "finteray.h"
#include "opponent.h"

#include "utility.h"
#include "globvars.h"
#include "carpocalypse2_types.h"
#include "carpocalypse2_macros.h"
// GLOBAL: CARMA2_HW 0x006a55c8
tExplosion gExplosions[50];
// FUNCTION: CARMA2_HW 0x004ea880

void C2_HOOK_FASTCALL InitExplosions(void) {
    int i;
    int capacity;
    tExplosion* explosion;

    capacity = (int)CARPOCALYPSE2_ASIZE(gExplosions);
    for (i = 0, explosion = gExplosions; i < capacity; explosion++, i++) {

        explosion->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);

        explosion->actor->model = BrModelAllocate(NULL, 4, 2);
        explosion->actor->model->faces[0].vertices[0] = 0;
        explosion->actor->model->faces[0].vertices[1] = 2;
        explosion->actor->model->faces[0].vertices[2] = 1;
        explosion->actor->model->faces[0].material = NULL;
        explosion->actor->model->faces[0].smoothing = 1;
        explosion->actor->model->faces[1].vertices[0] = 0;
        explosion->actor->model->faces[1].vertices[1] = 3;
        explosion->actor->model->faces[1].vertices[2] = 2;
        explosion->actor->model->faces[1].material = NULL;
        explosion->actor->model->faces[1].smoothing = 1;
        BrVector3Set(&explosion->actor->model->vertices[0].p, -.5f, -.5f, 0.f);
        BrVector2Set(&explosion->actor->model->vertices[0].map, 0.f, 1.f);
        BrVector3Set(&explosion->actor->model->vertices[1].p, -.5f, .5f, 0.f);
        BrVector2Set(&explosion->actor->model->vertices[1].map, 0.f, 0.f);
        BrVector3Set(&explosion->actor->model->vertices[2].p, .5f, .5f, 0.f);
        BrVector2Set(&explosion->actor->model->vertices[2].map, 1.f, 0.f);
        BrVector3Set(&explosion->actor->model->vertices[3].p, .5f, -.5f, 0.f);
        BrVector2Set(&explosion->actor->model->vertices[3].map, 1.f, 1.f);
        BrModelAdd(explosion->actor->model);

        explosion->actor->material = BrMaterialAllocate("BANG!");
        explosion->actor->material->flags &= ~BR_MATF_LIGHT;
        explosion->actor->material->flags |= BR_MATF_ALWAYS_VISIBLE;
        explosion->actor->material->colour_map = gBack_screen;
        GlorifyMaterial(&explosion->actor->material, 1, kRendererShadingType_AmbientOnly);
        BrMaterialAdd(explosion->actor->material);
        explosion->actor->render_style = BR_RSTYLE_FACES;

        explosion->start = 0;
    }
}

// ResetExplosions

// RemoveAllExplosionettes

// MungeExplosions

// CopyExplosionData

// DoExplosionette

// Explode

// RemoveExplosions

// CalcRandomPointBetween

// ApplyInitialMovement

// ApplyTimeSpec

// FindShrapnelFragment

// SetShrapnelFragment

// ShatterObject

// MungeInternalCarGlass

// ActuallySmashPart

// MakeSmashSound

// MungeSmokeFire

// Random34Matrix

// SetInitPosition

// DoGhostPartShrapnel

// DoNonCarShrapnel

// DoShrapnel

// DoAward

// DoSpillage

// DoSpecial

// GiveNonCarsAShove

// QueueDelayedSideEffect

// DoDelayedNonCar

// DoDelayedSmash

// MungeDelayedSideEffects

// ActivateNonCarSideEffects

// ActivateSmashSideEffects

// DoSideEffects

// DoVariableEffects

// DoConnotations

// DoPowerupRespawnSmash

// DoConnotationsMultiLevel

// SmashItIntoVerySmallPiecesIndeed

// InitSmashQueue

// FlushPowerupQueue

// FlushSmashQueue

// QueueSmashIfNecessary3

// QueueSmashIfNecessary2

// AllocateSmashLevels

// ReadSmashSounds

// ReadShrapnelMovementSpec

// ReadShrapnelInitialPosition

// ReadShrapnelTimeInfo

// NoteChildren

// ReadSmokeFireChance

// LoadActorFromRace

// ReadShrapnelSpec

// ReadSpillData

// ReadExplosion

// FreeExplosion

// ReadSpecialEffectsSpec

// ReadBounds

// ReadNoncarSideEffects

// ReadSmashSideEffects

// ReadSideEffects

// ReadLightingInformation

// ReadAward

// ReadVariableChanges

// ReadConnotations

// ReadSmashLevels

// ReadSmashableData

// ActuallyRepairSmash

// KillFragment

// AddRepairAnimationToQueue

// RepairSmash

// TotallyRepairSmash

// InitGlassFragments

// InitDecals

// RemoveAllDecals

// ReadPowerupSmash

// FUNCTION: CARMA2_HW 0x004efe00
void C2_HOOK_FASTCALL InitSmashing(void) {
    int i;

    for (i = 0; i < 200; i++) {
        tSmash_vertex* smash_vertex;

        smash_vertex = &gSmash_glass_fragments[i];
        smash_vertex->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        smash_vertex->actor->model = BrModelAllocate(NULL, 3, 1);
        smash_vertex->actor->model->faces[0].vertices[0] = 0;
        smash_vertex->actor->model->faces[0].vertices[1] = 1;
        smash_vertex->actor->model->faces[0].vertices[2] = 2;
        smash_vertex->actor->model->flags |= BR_MODF_UPDATEABLE;
        BrModelAdd(smash_vertex->actor->model);
        smash_vertex->model = smash_vertex->actor->model;
        smash_vertex->actor->render_style = BR_RSTYLE_FACES;
        smash_vertex->end_time = 0;
    }
    for (i = 0; i < 50; i++) {
        tDecal* smash_quad;

        smash_quad = &gDecals[i];
        smash_quad->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        smash_quad->actor->model = BrModelAllocate(NULL, 4, 2);
        smash_quad->actor->model->faces[0].vertices[0] = 0;
        smash_quad->actor->model->faces[0].vertices[1] = 1;
        smash_quad->actor->model->faces[0].vertices[2] = 3;
        smash_quad->actor->model->faces[1].vertices[0] = 0;
        smash_quad->actor->model->faces[1].vertices[1] = 3;
        smash_quad->actor->model->faces[1].vertices[2] = 2;
        smash_quad->actor->model->flags |= BR_MODF_UPDATEABLE;
        BrModelAdd(smash_quad->actor->model);
        smash_quad->actor->render_style = BR_RSTYLE_FACES;
        smash_quad->time = 0;
    }
}

// VerySimpleRotate

// DoFragMovement

// MungeGlassFragments2

// MungeAnimationRepairs

// MungeGlassFragments

// KillAllGlassFragments

// FUNCTION: CARMA2_HW 0x004f02b0
void C2_HOOK_FASTCALL CleanUpSmashStuff(void) {
    int i;

    MungeGlassFragments2(1);
    i = CARPOCALYPSE2_ASIZE(gDecals);
    do {
        tDecal* decal = &gDecals[CARPOCALYPSE2_ASIZE(gDecals) - i];
        if (decal->time != 0) {
            BrActorRemove(decal->actor);
        }
        decal->time = 0;
        i--;
    } while (i != 0);
    i = CARPOCALYPSE2_ASIZE(gExplosions);
    do {
        if (gExplosions[CARPOCALYPSE2_ASIZE(gExplosions) - i].actor->parent != NULL) {
            BrActorRemove(gExplosions[CARPOCALYPSE2_ASIZE(gExplosions) - i].actor);
        }
        i--;
    } while (i != 0);
}

// FUNCTION: CARMA2_HW 0x004f0310
void C2_HOOK_FASTCALL DisposeSmashData(tU8* pSmash) {
    int i;
    int offset;

    if (*(int*)(pSmash + 0x3c) != 0) {
        i = 0;
        if (*(tU16*)(pSmash + 0x38) > 0) {
            offset = 0;
            do {
                if (*(int*)(*(int*)(pSmash + 0x3c) + (offset + 0x70)) != 0) {
                    BrMemFree(*(void**)(*(int*)(pSmash + 0x3c) + (offset + 0x70)));
                }
                if (*(int*)(*(int*)(pSmash + 0x3c) + (offset + 0x78)) != 0) {
                    BrMemFree(*(void**)(*(int*)(pSmash + 0x3c) + (offset + 0x78)));
                }
                if (*(int*)(*(int*)(pSmash + 0x3c) + (offset + 0x50)) != 0) {
                    BrMemFree(*(void**)(*(int*)(pSmash + 0x3c) + (offset + 0x50)));
                }
                i++;
                offset += 0x7c;
            } while (i < *(tU16*)(pSmash + 0x38));
        }
        BrMemFree(*(void**)(pSmash + 0x3c));
    }
}

// DoBackwardShrapnel

// InitRepairAnimations

// SplondificatalizeIdentifier

// ReadSmashReplace

// ReadSmashTexture

// ReadSmashableEnvironment

// DisposeSmashEnvNonCars

// CloneMemory

// CloneEnvSmash

// FreeConnotations

// FreeEnvSmash

// DisposeSmashableEnvironment

// MungeSmashMaterialNames

// ExtrapolateTextureCoordinates

// ExtrapolateGeometryCoordinates2

// ExtrapolateGeometryCoordinates

// ExtractDeTiledFaces

// AddDecal

// InsertQuad

// SpawnSmashAndMungeFaces

// DistanceFromFaceND

// PointInFace

// FindFaceWithPoint

// ProcessTexture

// ProcessMesh

// ProcessModel

// BashModel

// SendNetSmash

// SendAllSmashes

// HitPowerup

// SmashEnvironment

// BashObject

// NonCarSnapSmashCallback

// SmashFaceMustBeUpdateable

// MungeMaterialCB

// MungeMaterial

// ReinitSmashing

// LinkSmashies

// RepairModels

// RepairAllSmashes

// SmashEverythingCB

// SmashEverything

// DetermineTilitudinality

// SetSmashableModel

// LoadWastedExplosion

// ChangeSmashModel

// TurnOnSmashModel

// TurnOffSmashModel

// SmashRemoveSomeFaces

// SmashRestoreSomeFaces

// TurnOnSmashDecal

// TurnOffSmashDecal

// ChangeSmashTexture

// C2PORT-NONMATCHING: best 75.25% | difftest PASS 2000 trials (spec difftest/specs/004f60b0_QueueNetSmash.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x004f60b0
void C2_HOOK_FASTCALL QueueNetSmash(tQueued_net_smash* pSmash) {
    int i;
    int is_host;
    tQueued_net_smash* pQueued_smash;
    tQueued_net_smash* pHost_smash;

    if (gPHIL_last_physics_tick == 0 || gPHIL_last_physics_tick >= pSmash->field_0x4) {
        DoNetSmash(pSmash);
        return;
    }

    i = 0;
    pQueued_smash = gQueued_net_smashes;
    do {
        if (pQueued_smash->field_0x4 == 0) {
            goto queue_net_smash_found;
        }
        i++;
        pQueued_smash++;
    } while (i < 50);
    return;
queue_net_smash_found:

    is_host = gNet_mode == eNet_mode_host;
    *pQueued_smash = *pSmash;

    if (is_host && gCount_host_smashes < 1000) {
        pHost_smash = (tQueued_net_smash*)((tU8*)gNet_host_smashes + gCount_host_smashes * 0x38);
        gCount_host_smashes++;
        *pHost_smash = *pSmash;
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 78.51% | difftest PASS 2000 trials (spec difftest/specs/004f6130_DoNetSmash.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x004f6130
void C2_HOOK_FASTCALL DoNetSmash(tQueued_net_smash* pSmash) {
    int owner_id;
    int face_count;
    int i;
    tCar_spec* car;
    tPhysics_object* object;
    tFace_ref* face;
    struct {
        tBounds bounds;
        br_matrix34 matrix;
        tFace_ref faces[20];
    } locals;
    const float* point;

    owner_id = pSmash->vehicle_id;
    if (owner_id != -1) {
        if (gNet_mode) {
            if (owner_id < gNumber_of_net_players) {
                car = gNet_players[owner_id].car;
            } else {
                car = NULL;
            }
        } else if ((owner_id & 0xffffff00) == 0) {
            car = &gProgram_state.current_car;
        } else {
            car = GetCarSpec((tVehicle_type)(owner_id >> 8), owner_id & 0xff);
        }
        if (car != NULL) {
            object = car->collision_info;
        } else {
            object = NULL;
        }
    } else {
        object = NULL;
    }
    if (object == NULL) {
        return;
    }

    locals.bounds.mat = &locals.matrix;
    BrMatrix34Identity(&locals.matrix);
    point = pSmash->position.v;
    locals.matrix.m[3][0] = point[0];
    locals.matrix.m[3][1] = point[1];
    locals.matrix.m[3][2] = point[2];
    locals.bounds.original_bounds.min.v[0] = -0.05f;
    locals.bounds.original_bounds.min.v[1] = -0.05f;
    locals.bounds.original_bounds.min.v[2] = -0.05f;
    locals.bounds.original_bounds.max.v[0] = 0.05f;
    locals.bounds.original_bounds.max.v[1] = 0.05f;
    locals.bounds.original_bounds.max.v[2] = 0.05f;
    FillInBounds(&locals.bounds);
    face_count = FindFacesInBox(&locals.bounds, locals.faces, 20, &gWorld_callbacks);
    face = locals.faces;
    i = 0;
    if (face_count > 0) {
        while (i < face_count) {
            if (FUN_004f0d50(point, face->normal.v, face->d,
                    face->v[0].v, face->v[1].v, face->v[2].v)) {
                break;
            }
            i++;
            face++;
        }
        if (i < face_count) {
            gINT_006a8294 = pSmash->flags & 1;
            SmashEnvironment(object, (undefined4*)face, pSmash->smash_strength,
                    (br_vector3*)point, &pSmash->field_0x1c, &pSmash->field_0x28,
                    (pSmash->flags >> 1) & 1, 1);
            FlushSmashQueue(0);
            gINT_006a8294 = 0;
        }
    }
}
#pragma auto_inline(on)

// ReceivedSmashEnvironment

// DoQueuedNetSmashes

// FindTheBastard

// ReceivedSmashShraps

// MungeSmashEdgeTriggers
