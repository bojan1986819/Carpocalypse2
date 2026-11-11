#include "crush1.h"
#include "crush.h"
#include "physics.h"
#include "globvars.h"
#include "compress.h"
#include "smash.h"
#include "spark.h"
#include "utility.h"
#include "piping.h"
#include "world.h"
#include "car.h"
#include "errors.h"
#include "carpocalypse2_macros.h"

#include "loading1.h"
#include "platform.h"

#include <brender/brender.h>

#include "c2_string.h"
// GLOBAL: CARMA2_HW 0x00679698
br_scalar gDistortion_factor;

// GLOBAL: CARMA2_HW 0x006796b0
br_scalar gForce_to_movement_factor;

// GLOBAL: CARMA2_HW 0x00679550
br_scalar gMax_detach_time_ms;

// GLOBAL: CARMA2_HW 0x006796b8
br_scalar gMin_crush_force;

// GLOBAL: CARMA2_HW 0x0067944c
br_scalar gNormal_force_to_detach;

// GLOBAL: CARMA2_HW 0x0067a18c
br_scalar gMin_bend_angle;

// GLOBAL: CARMA2_HW 0x0067b7b0
int gMin_bend_damage;

// GLOBAL: CARMA2_HW 0x0067b7b4
br_scalar gFlap_inertia_fudge_biscuit;

// GLOBAL: CARMA2_HW 0x0067b7d0
int gMax_split_damage;

// GLOBAL: CARMA2_HW 0x0067bac8
br_scalar gMin_force_to_split_XZ_per_tonne;

// GLOBAL: CARMA2_HW 0x0067bacc
br_scalar gMax_crush_dist_sq;

// GLOBAL: CARMA2_HW 0x0067bd5c
br_scalar gMin_force_to_split_Y_per_tonne;

// GLOBAL: CARMA2_HW 0x0067bd64
int gMin_split_damage;

// GLOBAL: CARMA2_HW 0x0067bde8
br_scalar gMin_bend_force;

// GLOBAL: CARMA2_HW 0x0067bdec
br_scalar gMax_crush_force;

// GLOBAL: CARMA2_HW 0x0067bdf4
int gMax_bend_damage;

// GLOBAL: CARMA2_HW 0x0067bdf8
br_scalar gTorque_to_snap_per_tonne;

// GLOBAL: CARMA2_HW 0x0067be00
br_scalar gChance_of_bending;

// GLOBAL: CARMA2_HW 0x0067be04
br_scalar gChance_of_inverse_buckle;

// GLOBAL: CARMA2_HW 0x0067be78
br_vector3 gBatty_gravity;
// FUNCTION: CARMA2_HW 0x00429bb0

void C2_HOOK_FASTCALL ReadCrushSettings(FILE* pF) {
    char s[256];
    char *result;

    while (1) {
        result = GetALineAndDontArgue(pF, s);
        if (result == NULL) {
            break;
        }
        if (strcmp("START OF CRUSH SETTINGS", s) == 0) {
            break;
        }
    }
    if (result == NULL) {
        PDFatalError("Can't find start of CRUSH SETTINGS in .TXT file");
    }
    if (GetAnInt(pF) != 4) {
        PDFatalError("Wrong version of CRUSH SETTINGS");
    }
    /* CRUSHING */
    gDistortion_factor = GetAScalar(pF);
    gMin_crush_force = GetAScalar(pF);
    gMax_crush_force = GetAScalar(pF);
    gForce_to_movement_factor = GetAScalar(pF);
    gMax_crush_dist_sq = GetAScalar(pF);

    /* SPLITTING */
    gMin_force_to_split_XZ_per_tonne = GetAScalar(pF);
    gMin_force_to_split_Y_per_tonne = GetAScalar(pF);
    gMin_split_damage = GetAnInt(pF);
    gMax_split_damage = GetAnInt(pF);

    /* BUCKLING */
    gChance_of_inverse_buckle = GetAScalar(pF);

    /* FLAPPING AND JOINT SNAPPING */
    gFlap_inertia_fudge_biscuit = GetAScalar(pF);
    gBatty_gravity.v[1] = GetAScalar(pF);
    gTorque_to_snap_per_tonne = GetAScalar(pF);

    /* DETACHING */
    gMax_detach_time_ms = GetAScalar(pF);
    gNormal_force_to_detach = GetAScalar(pF);

    /* BENDING */
    gMin_bend_force = GetAScalar(pF);
    gChance_of_bending = GetAScalar(pF);
    gMin_bend_angle = GetAScalar(pF);
    gMin_bend_damage = GetAnInt(pF);
    gMax_bend_damage = GetAnInt(pF);
    while (1) {
        result = GetALineAndDontArgue(pF, s);
        if (result == NULL) {
            break;
        }
        if (strcmp("END OF CRUSH SETTINGS", s) == 0) {
            break;
        }
    }
    if (result == NULL) {
        PDFatalError("Can't find end of CRUSH SETTINGS in .TXT file");
    }
}

// ClearCrushLists

// RemoveCarFromCrushLists

// FUNCTION: CARMA2_HW 0x00429fa0
void C2_HOOK_FASTCALL InitCrushSystems(void) {
#ifdef CARPOCALYPSE2_MATCHING
    int i;

    memset(gCrush_array_0067a190, 0, sizeof(gCrush_array_0067a190));
    gDAT_00679440 = 0;
    gDetached_bit_crush_info_buffer.capacity = 16;
    gDetached_bit_crush_info_buffer.crush_infos = BrMemAllocate(gDetached_bit_crush_info_buffer.capacity * sizeof(tCrush_info), kMem_crush_data);
    gDetached_bit_collision_infos = BrMemAllocate(gDetached_bit_crush_info_buffer.capacity * sizeof(tPhysics_object), kMem_crush_data);
    for (i = 0; i < gDetached_bit_crush_info_buffer.capacity; i++) {
        gDetached_bit_crush_info_buffer.crush_infos[i].field_0xdc = 1.f;
        gDetached_bit_crush_info_buffer.crush_infos[i].field_0xe0 = 0.28985506f;
        gDetached_bit_crush_info_buffer.crush_infos[i].collision_object = &gDetached_bit_collision_infos[i];
        gDetached_bit_collision_infos[i].shape = (tPhysics_shape*)AllocateShapePolyhedron(16, kMem_crush_data);
    }
    gDetached_bit_driver = eDriver_detached_bit;
    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gTrack_crush_joints); i++) {
        gTrack_crush_joints[i] = AllocatePhysicsJoint(3, kMem_crush_data);
    }
    gSplit_car_crush_info_buffer.capacity = 3;
    gSplit_car_crush_info_buffer.crush_infos = BrMemAllocate(gSplit_car_crush_info_buffer.capacity * sizeof(tCrush_info), kMem_crush_data);
    gSplit_car_collision_infos = BrMemAllocate(gSplit_car_crush_info_buffer.capacity * sizeof(tPhysics_object), kMem_crush_data);
    for (i = 0; i < gSplit_car_crush_info_buffer.capacity; i++) {
        int j;
        tPhysics_shape* shape = NULL;

        gSplit_car_crush_info_buffer.crush_infos[i].field_0xdc = 0.f;
        gSplit_car_crush_info_buffer.crush_infos[i].collision_object = &gSplit_car_collision_infos[i];
        for (j = 0; j < 4; j++) {
            tPhysics_shape* polyhedron = (tPhysics_shape*)AllocateShapePolyhedron(16, kMem_crush_data);

            if (shape != NULL) {
                shape->common.next = polyhedron;
            } else {
                gSplit_car_collision_infos[i].shape = polyhedron;
            }
            shape = polyhedron;
        }
        if (shape != NULL) {
            shape->common.next = NULL;
        }
    }
    gSplit_car_driver = eDriver_split_car;
    gGonad_sphere_collision_shape = (tPhysics_shape*)AllocateShapeSphere(kMem_crush_data);
#else
    int i;
    int j;
    tCrush_info* crush_infos;
    tCrush_info* crush_info;
    tPhysics_object* collision_infos;
    tPhysics_object* collision_info;
    tPhysics_joint** joint;
    tCollision_shape_polyhedron* shape;

    for (i = 0; i < CARPOCALYPSE2_ASIZE(gCrush_array_0067a190); i++) {
        gCrush_array_0067a190[i] = 0;
    }
    gDAT_00679440 = 0;

    gDetached_bit_crush_info_buffer.capacity = 16;
    gDetached_bit_crush_info_buffer.crush_infos = BrMemAllocate(16 * sizeof(tCrush_info), kMem_crush_data);
    gDetached_bit_collision_infos = BrMemAllocate(gDetached_bit_crush_info_buffer.capacity * sizeof(tPhysics_object), kMem_crush_data);
    crush_infos = gDetached_bit_crush_info_buffer.crush_infos;
    collision_infos = gDetached_bit_collision_infos;
    crush_info = crush_infos;
    collision_info = collision_infos;
    for (i = 0; i < gDetached_bit_crush_info_buffer.capacity; i++) {
        crush_info->field_0xdc = 1.0f;
        crush_info->field_0xe0 = 0.28985506296157837f;
        crush_info->collision_object = collision_info;
        crush_info->collision_object->shape = (tPhysics_shape*)AllocateShapePolyhedron(16, kMem_crush_data);
        crush_info++;
        collision_info++;
    }

    gDetached_bit_driver = 2;
    joint = &gTrack_crush_joints[0];
    do {
        *joint = AllocatePhysicsJoint(3, kMem_crush_data);
        joint++;
    } while ((int)joint < (int)&gTrack_crush_joints[CARPOCALYPSE2_ASIZE(gTrack_crush_joints)]);

    gSplit_car_crush_info_buffer.capacity = 3;
    gSplit_car_crush_info_buffer.crush_infos = BrMemAllocate(3 * sizeof(tCrush_info), kMem_crush_data);
    gSplit_car_collision_infos = BrMemAllocate(gSplit_car_crush_info_buffer.capacity * sizeof(tPhysics_object), kMem_crush_data);
    crush_infos = gSplit_car_crush_info_buffer.crush_infos;
    collision_infos = gSplit_car_collision_infos;
    crush_info = crush_infos;
    collision_info = collision_infos;
    for (i = 0; i < gSplit_car_crush_info_buffer.capacity; i++) {
        crush_info->field_0xdc = 0.0f;
        crush_info->collision_object = collision_info;
        shape = NULL;
        for (j = 0; j < 4; j++) {
            tCollision_shape_polyhedron* new_shape;

            new_shape = AllocateShapePolyhedron(16, kMem_crush_data);
            if (shape != NULL) {
                shape->common.next = (tPhysics_shape*)new_shape;
            } else {
                collision_info->shape = (tPhysics_shape*)new_shape;
            }
            shape = new_shape;
        }
        if (shape != NULL) {
            shape->common.next = NULL;
        }
        crush_info++;
        collision_info++;
    }

    gSplit_car_driver = 3;
    gGonad_sphere_collision_shape = (tPhysics_shape*)AllocateShapeSphere(kMem_crush_data);
#endif
}

// ResetCrushSystems

// ReadCrushData

// CreateDefaultCrushDataCB

// CreateDefaultCrushData

// ReadMasterCrushData

// LoadCrushDataFile

// LinkCrushDataToActor

// LinkCrushDataToActors

// AccumulateSquashVertices

// InitModelMasterCrushData

// InitNetworkShapesStuff

// IncreasingCompare

// DecreasingCompare

// CrushLimitNumber

// SoftnessOfNearestPointCB

// DRActorEnumRecurseWithTranslation

// SoftnessOfNearestPoint

// SetUpShapeLimitingStuff

// CopyShapePolyhedron

// InitShapeStuff

// InitPhysMasterCrushData

// CheckHingePointOrder

// FindNearestVertex

// DistanceToBounds

// FindNearestParentVertex

// SetUpSemiDetachJointStuff

// InitPhysCrushDataCB

// InitPhysCrushData

// CalculateReferencePoints

// InitModelCrushDataCB

// InitModelCrushData

// PointEdgeDistSq

// SetFlapCheckVertices

// InitModelVertexData

// InitVertexDataCB

// InitVertexData

// CheckWheelPositions

// InitPhysModCrushData

// FUNCTION: CARMA2_HW 0x0042c420
void C2_HOOK_FASTCALL DisposeMasterCrushData(tCar_crush_spec* pCar_crush, tCar_spec* pCar) {
#ifndef CARPOCALYPSE2_MATCHING
    int i;

    if (pCar_crush == NULL) {
        return;
    }
    if (pCar_crush->actor != NULL) {
        BrActorFree(pCar_crush->actor);
    }
    if (pCar_crush->field_0x1cc != NULL) {
        BrMemFree(pCar_crush->field_0x1cc);
    }
    for (i = 0; i < pCar_crush->count_shapes; i++) {
        BrMemFree(pCar_crush->field_0x4[i].field_0x18);
        BrMemFree(pCar_crush->field_0x4[i].field_0x0);
        BrMemFree(pCar_crush->field_0x4[i].field_0x4);
    }
    BrMemFree(pCar_crush->field_0x4);
    if (pCar->collision_info != NULL) {
        pCar->collision_info->shape = pCar_crush->field_0xc;
    }
    if (pCar_crush->network_stuff != NULL) {
        BrMemFree(pCar_crush->network_stuff);
    }
    for (i = 0; i < 4; i++) {
        BrMemFree(pCar_crush->field_0x564[i]);
    }
    BrMemFree(pCar_crush);
#else
    int i;

    if (pCar_crush == NULL) {
        return;
    }
    BrActorFree(pCar_crush->actor);
    if (pCar_crush->field_0x1cc != NULL) {
        BrMemFree(pCar_crush->field_0x1cc);
    }
    for (i = 0; i < pCar_crush->count_shapes; i++) {
        BrMemFree(pCar_crush->field_0x4[i].field_0x18);
        BrMemFree(pCar_crush->field_0x4[i].field_0x0);
        BrMemFree(pCar_crush->field_0x4[i].field_0x4);
    }
    BrMemFree(pCar_crush->field_0x4);
    pCar->collision_info->shape = pCar_crush->field_0xc;
    if (pCar_crush->network_stuff != NULL) {
        BrMemFree(pCar_crush->network_stuff);
    }
    for (i = 0; i < 4; i++) {
        BrMemFree(pCar_crush->field_0x564[i]);
    }
    BrMemFree(pCar_crush);
#endif
}

// FUNCTION: CARMA2_HW 0x0042c4f0
void C2_HOOK_FASTCALL FUN_0042c4f0(tU8* pData) {
    void* data;

    if (pData != NULL) {
        data = *(void**)(pData + 0x30);
        if (data != NULL) {
            BrMemFree(data);
        }
        data = *(void**)(pData + 0x34);
        if (data != NULL) {
            BrMemFree(data);
        }
        DisposeSmashData(pData);
        BrMemFree(pData);
    }
}

// FUNCTION: CARMA2_HW 0x0042c530
void C2_HOOK_FASTCALL FreeIfNotNull(void* pPtr) {

    if (pPtr != NULL) {
        BrMemFree(pPtr);
    }
}

// DisposeCrushData

// DisposeVertexData

// BuckleTheBugger

// OutsideEdgeLine

// PointTriangleDistSq

// PointRectDistSq

// PointSimplexDistSq

// DistSq

// LimitImpactScale

// GetImpactScale

// TooFarToCrush

// NearestAxis

// CrushModel

// PossibleShockSmash

// CrushModels

// CrushPoint

// ClipPointToLimits

// CrushCollisionShapes

// SimpleCrushCarModelsCB

// SimpleCrushCar

// CrushCar

// SendCrush

// CompressCrush

// ExpandCrush

// RemoveDuplicatePoints

// GLOBAL: CARMA2_HW 0x00589a70
const br_scalar gFloat_00589a70 = 0.005f;
// GLOBAL: CARMA2_HW 0x00589a78
const br_scalar gFloat_00589a78 = 47.6099967956543f;

extern br_scalar gMax_crush_dist_sq;
extern br_scalar gCrushZero;
extern br_scalar gCrushSqrtHalf;
extern const br_scalar gFLOAT_00589a54;
extern br_scalar gFLOAT_00589a5c;
extern const double gDOUBLE_00589a60;
int C2_HOOK_FASTCALL TooFarToCrush(const br_model* pModel, int pCount, const br_vector3* pVertices, float pMax_dist_sq);
void C2_HOOK_FASTCALL FUN_004c7f40(br_model* pModel, br_vector3* pSrc, const float* pScalars);

// C2PORT-NONMATCHING: best 48.93% | difftest PASS 2000 trials (spec difftest/specs/0042c540_SimpleCrushModel.json) | reason x87-order-stack-layout
// FUNCTION: CARMA2_HW 0x0042c540
void C2_HOOK_FASTCALL SimpleCrushModel(br_model* pModel, int pCount, float* pPoints, float* pDir, float* pBounds) {
    float unique[12];
    float weights[1024];
    float* pCur;
    float* pNext;
    float* pOut;
    float* pVert;
    int remaining;
    int count;
    int axis;
    int found;
    int k;
    float len;
    float scale;
    float norm_x;
    float norm_y;
    float bend;
    float a;
    float b;
    float dist_sq;
    tU16 v;

    if (pModel == NULL) {
        return;
    }
    if (pModel->nvertices > 0x400) {
        PDFatalError("SimpleCrushModel can't cope with that many vertices");
    }
    if (pCount <= 0) {
        return;
    }
    if (pCount > 4) {
        pCount = 4;
    }

    count = 1;
    unique[0] = pPoints[0];
    unique[1] = pPoints[1];
    unique[2] = pPoints[2];
    if (pCount > 1) {
        remaining = pCount - 1;
        pCur = pPoints;
        pOut = unique + 4;
        do {
            pNext = pCur + 3;
            found = 0;
            for (k = 0; k < count; k++) {
                float* pU = unique + k * 3;
                if ((float)fabs(pU[0] - pNext[0]) < gFloat_00589a70 && (float)fabs(pU[1] - pNext[1]) < gFloat_00589a70 &&
                    (float)fabs(pU[2] - pNext[2]) < gFloat_00589a70) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                pOut[-1] = pNext[0];
                pOut[0] = pNext[1];
                pOut[1] = pNext[2];
                pOut += 3;
                count++;
            }
            remaining--;
            pCur = pNext;
        } while (remaining != 0);
    }

    if (TooFarToCrush(pModel, count, (br_vector3*)unique, gMax_crush_dist_sq) != 0) {
        return;
    }

    len = (float)sqrt(pDir[0] * pDir[0] + pDir[2] * pDir[2] + pDir[1] * pDir[1]);
    if (len <= gFLOAT_00589a5c) {
        norm_y = gCrushZero;
        norm_x = gFLOAT_00589a54;
    } else {
        scale = (float)gDOUBLE_00589a60 / len;
        norm_y = pDir[1] * scale;
        norm_x = pDir[0] * scale;
    }

    if ((float)fabs(norm_x) <= gCrushSqrtHalf) {
        axis = 1;
        if ((float)fabs(norm_y) <= gCrushSqrtHalf) {
            axis = 2;
        }
    } else {
        axis = 0;
    }

    v = 0;
    if (pModel->nvertices != 0) {
        do {
            pVert = (float*)((tU8*)pModel->vertices + (tU32)v * 0x28);
            dist_sq = CrushShapeDistSq(pVert, count, unique);
            if (dist_sq <= gMax_crush_dist_sq) {
                bend = gFLOAT_00589a54 - (float)sqrt(dist_sq * gFloat_00589a78);
            } else {
                bend = gCrushZero;
            }
            weights[v] = bend;
            if (bend != gCrushZero) {
                a = (pBounds[axis] - pVert[axis]) / pDir[axis];
                b = (pBounds[axis + 3] - pVert[axis]) / pDir[axis];
                if (b <= a) {
                    a = b;
                }
                if (gCrushZero < a) {
                    if (a < bend) {
                        weights[v] = a;
                    }
                } else {
                    weights[v] = gCrushZero;
                }
                if (weights[v] != gCrushZero) {
                    pVert[0] = pVert[0] + weights[v] * pDir[0];
                    pVert[1] = pDir[1] * weights[v] + pVert[1];
                    pVert[2] = pDir[2] * weights[v] + pVert[2];
                }
            }
            v++;
        } while (v < pModel->nvertices);
    }

    FUN_004c7f40(pModel, (br_vector3*)pDir, weights);

    if ((pModel->flags & 0x20) != 0 && pModel->user != NULL) {
        *(int*)pModel->user = 1;
        return;
    }
    pModel->custom = JitModelUpdate;
    pModel->flags = pModel->flags | 0x20;
}


// CrushDrone

// CrushDroneObject

// DoDroneCrushing

// AddDroneCrush

// QueueDroneCrush

// DoCrushing

// SetCrush

// QueueCarCrush

// SetBitForDetachment

// SetFlappage

// GetUniqueID

// InitializeCollisionInfo

// RemoveBit

// RemoveNonCarChildren

// RemoveNonCar

// GetNonCar

// GetNonCarObject

// SetRelationsDoingNothingFlags

// SetUpDetachedBitObject

// SetUpSemiDetachedBitJoint

// BitIsInBentPartOfCar

// SetUpFlapJoint

// MakeModelMaterialsDoubleSided

// CheckFlapAngles

// MakeModelMaterialsSingleSided

// ReAttachBit

// CentreModelOnPoint

// SemiDetachBit

// GetSDBJointPosAndBounds

// SendSemiDetachBit

// SendDetachBit

// SendAllSemiDetachedBits

// AddNonCar

// DetachBit

// DoDetaching

// FlapBit

// FlapBits

// StartFlapping

// MungeDetachLists

// SendReAttachBit

// StopFlapping

// DoFlapping

// ToggleFlapsCB2

// ToggleFlapsCB

// SendToggleFlaps

// ToggleFlaps

// BitObjectIsSufficientlyOutsideCarObjectToDetach

// FullyDetachBit

// SendFullyDetachBit

// DoFullyDetaching

// CalculateCollisionObjectStuffForSplitHalves

// SetUpCollisionInfoStuffForSplitHalf

// SetFace

// AddSplitVertexIndex

// CreateSplitVertex

// GetSplitVertex

// SplitModel

// CreateSplitPoints

// SplitCollisionShape

// SplitCarCollisionShapes

// QuantizeWithinBounds

// SaveVitalBitsOfObject

// MakeCarModelsMaterialsDoubleSided

// SwapCarShapes

// IsAWheelOrPivotActor

// MakeCarModelsMaterialsSingleSided

// IsOnNonCarHalf

// SplitCar

// QuantizePlane

// WithinBounds

// SplitCheck

// FindForceEdge

// DoXZSplitting

// DoYSplitting

// DoSplitting

// DoJointSnapping

// PointIsOutsideShape

// PointIsOutsideLimits

// ClipPointToPolyhedron

// QuantizeVector3

// MoveCarToSensiblePlace

// SwapShapesIfPossible

// DoShapeSwapping

// BendPoint

// EdgeCrossesZPlane

// BendCollisionShape

// BendCollisionShapes

// BendModel

// BendModels

// BrAngleToSignedDegree

// BendWheels

// BendActor

// BendOtherThings

// ReAttachOrFullyDetachDifficultBits

// BendCar

// QuantizeAngle

// BendCarOneForce

// DoBending

// JitModelUpdate

// SetModelForUpdate

// RandomlyXZSplitCar

// RandomlyBendCar

// TotallySpamTheCar

// DRActorEnumRecurseUltraPlus

// FireOffBitsCB

// FireSomeBitsOffCar

// DoSpams

// CrushBendFlapRend

// SphericizeModel

// SphericizePhysics

// SphericizeCar

// BattenDownTheHatches

// BattenDownTheObjects

// CompletelyUnBendCollisionShape

// CompletelyUnBendCollisionShapes

// CompletelyUnBendWheels

// CompletelyUnBendOtherThings

// EnableGroovers

// DoCompletelyUnBentThings

// TotallyRepairCarCollisionShapes

// TotallyRepairModel

// TotallyRepairModels

// TotallyRepairObject

// SendReAttachAllBits

intptr_t C2_HOOK_CDECL TotallyRepairCarCB(br_actor* pActor, void* data) {
    tCar_spec* pCar_spec = data;
    tUser_crush_data* user_crush_data;
    int i;

    user_crush_data = pActor->user;
    if (user_crush_data == NULL || user_crush_data->crush_data == NULL) {
        return 0;
    }
    for (i = 0; i < pCar_spec->count_detail_levels; i++) {
        br_model* model = user_crush_data->models[i];
        if (model != NULL && model->user != NULL && model->nvertices != 0) {
            tUser_detail_level_model* model_user_data = model->user;
            if (model_user_data->field_0x4 != NULL) {
                int j;
                for (j = 0; j < model->nvertices; j++) {
                    ExpandVector3(&model->vertices[j].p, &model_user_data->field_0x4[j].p, -10.f, 10.f);
                    model_user_data->field_0x4[j].flags = 0;
                }
            }
        }
    }
    return 0;
}

extern int gUNK_0075bc24;

// C2PORT-NONMATCHING: best 41.20% | difftest PASS 2000 trials (spec difftest/specs/00439510_TotallyRepairACar.json) | reason VC5 register allocation and loop codegen
// FUNCTION: CARMA2_HW 0x00439510
void C2_HOOK_FASTCALL TotallyRepairACar(tCar_spec* pCar_spec) {
    tCar_crush_spec* crush_spec;
    br_vector3 vector;
    int i;
    int shape_index;
    register int point_index;
    register int point_offset;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, damage_units, 0x4d8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCar_spec, repair_time, 0x18a4);

    StopCarSmokingInstantly(pCar_spec);
    if (ARIsActionReplayAvailable() != 0) {
        PipeInstantUnSmudge(pCar_spec);
    }
    pCar_spec->repair_time += 100000;
    for (i = 0; i < 12; i++) {
        pCar_spec->damage_units[i].damage_level = 0;
        pCar_spec->damage_units[i].last_level = 0;
        pCar_spec->damage_units[i].smoke_last_level = 0;
    }

    if (pCar_spec->use_shell_model == 0 && pCar_spec->car_crush_spec != NULL) {
        crush_spec = pCar_spec->car_crush_spec;
        if (crush_spec->field_0x4b8 != 0) {
            WeldCar(pCar_spec);
        }
        for (i = 0; i < crush_spec->field_0x2b0; i++) {
            ReAttachBit(pCar_spec,
                crush_spec->field_0x2b4[i].field_0x4,
                crush_spec->field_0x2b4[i].field_0x0,
                NULL);
        }
        crush_spec->field_0x2b0 = 0;
        for (i = 0; i < crush_spec->field_0x270; i++) {
            ReAttachBit(pCar_spec,
                crush_spec->field_0x274[i].field_0x4,
                crush_spec->field_0x274[i].field_0x0,
                crush_spec->field_0x274[i].field_0x8);
        }
        crush_spec->field_0x270 = 0;

        DRActorEnumRecurse(pCar_spec->car_model_actor, MakeCarModelsMaterialsSingleSided, pCar_spec);
        PipeSingleDSModel(0, pCar_spec);
        if (crush_spec->field_0x144 != 0) {
            int shape_offset;

            crush_spec->field_0x144 = 0;
            shape_offset = 0;
            for (shape_index = 0; shape_index < crush_spec->count_shapes; shape_index++) {
                tCar_crush_shape_info* shape = (tCar_crush_shape_info*)((char*)crush_spec->field_0x4 + shape_offset);

                shape->count_points = shape->original_count_points;
                point_offset = 0;
                for (point_index = 0; point_index < shape->count_points; point_index++) {
                    tCar_crush_reordered_shape_info* point = (tCar_crush_reordered_shape_info*)((char*)shape->field_0x18 + point_offset);
                    point->field_0x18 = point->field_0x2c;
                    point_offset += sizeof(*point);
                }
                shape_offset += sizeof(*shape);
            }
            crush_spec->expand_bounding_box = 3;
            {
                br_actor** wheel_actor = pCar_spec->wheel_actors;
                for (i = 0; i < 0x48; i += 0xc, wheel_actor++) {
                    br_actor* actor;
                    tGroovidelic_spec* groove;

                    actor = wheel_actor[6];
                    if (actor == NULL) {
                        actor = wheel_actor[0];
                    }
                    if (actor != NULL) {
                        groove = ActorsGroove(actor);
                        if (groove != NULL && groove->path_type == eGroove_path_straight) {
                            BrVector3Negate(&vector, &groove->path_data.straight_info.centre);
                            BrVector3Copy(&groove->path_data.straight_info.centre,
                                (br_vector3*)((char*)crush_spec->field_0x1ec + i));
                            BrVector3Add(&vector, &groove->path_data.straight_info.centre, &vector);
                            PipeSingleVector3(&groove->path_data.straight_info.centre, &vector);
                        }
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                pCar_spec->wpos[i] = crush_spec->field_0x234[i];
            }
            SetCarSuspGiveAndHeight(pCar_spec, 1.f, 1.f, 1.f, 0.f, 0.f);
            pCar_spec->field_0x1260 = 0.f;
            BrVector3Copy(&pCar_spec->collision_info->cmpos, &crush_spec->field_0x264);
            DRActorEnumRecurse(pCar_spec->car_model_actor, EnableGroovers, NULL);
        }

        ARStartPipingSession(ePipe_model_mash);
        PhysicsObjectRecurse(pCar_spec->collision_info, TotallyRepairObject, pCar_spec);
        AREndPipingSession();
        dr_dprintf("TotallyRepairACar() - Point 7");
        TotallyRepairCarCollisionShapes(pCar_spec);
        dr_dprintf("TotallyRepairACar() - Point 8");
        PhysicsObjectRecurse(pCar_spec->collision_info, BattenDownTheObjects, pCar_spec);
        dr_dprintf("TotallyRepairACar() - Point 9");
        if (gUNK_0075bc24 != 2) {
            dr_dprintf("TotallyRepairACar() - Point 10");
            SwapShapesIfPossible(pCar_spec);
        }
        dr_dprintf("TotallyRepairACar() - Point 11");
        pCar_spec->use_shell_model = 1;
        RemoveCarFromCrushLists(pCar_spec);
        dr_dprintf("TotallyRepairACar() - Point 12");
    }
}

// TotallyRepairCar

// AddTotalDeflectionOfModel

// AddTotalDeflection

// RepairModel

// RepairModels

// UnBendModel

// UnBendModels

// UnBendActor

// UnBendCollisionShape

// UnBendCollisionShapes

// UnBendWheels

// UnBendOtherThings

// NormalizedAngle

// CalculateAmountRepairedFraction

// UnBendCar

// CompletelyUnBendCar

// ReAttachBits

// RepairObject

// RepairCarCollisionShapes

// RepairCar2

// RepairCar

// RestoreVitalBitsOfObject

// WeldCar

// ARCompressCrush

// ARApplyCrush

// ARUndoCrush

// ARCompressModelMash

// ARApplyModelMash

// ARUndoModelMash

// ARApplyFlap

// ARApplySplitWeld

// ARUndoSplitWeld

// ReceivedCrush

// FindBitWithID

// AttachedBitFromID

// ReceivedSemiDetachBit

// ReceivedDetachBit

// SemiDetachedBitObjectFromID

// ReceivedFullyDetachBit

// DetachedBitFromID

// SemiDetachedBitFromID

// ReceivedReAttachBit

// ReceivedReAttachAllBits

// DoQueuedNetDetaching

// DBNonCarOwner

// SendDBNonCar

// ReceivedDBNonCar

// SendAllDBNonCars

// ReceivedDBNonCarPosition

// ReceivedToggleFlaps

// SHNonCarOwner

// SendSHNonCar

// SendAllSHNonCars

// ReceivedSHNonCar

// GetNetShapeDataLengthFromShape

// GetNetShapeDataLengthFromData

// EncodeNetShapeData

// DecodeNetShapeData

// CopyBendInMessage

// Vector4Equal

// CompressPlane

// ExpandPlane

// CopySplitInMessage

// SetAllShapeStatusFlags
