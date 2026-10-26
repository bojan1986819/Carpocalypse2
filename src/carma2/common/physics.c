#include "physics.h"
#ifndef CARPOCALYPSE2_MATCHING
#include "pedestrn.h"
#endif
#include "crush.h"

#include "car.h"
#include "errors.h"
#include "globvars.h"
#include "globvrpb.h"
#include "loading.h"
#include "netgame.h"
#include "opponent.h"
#include "piping.h"
#include "platform.h"
#include "powerups.h"
#include "raycast.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"
#include "finteray.h"

#include <brender/brender.h>

extern const br_scalar gFLOAT_005899a8;
extern const double gDouble_005899b0;

extern br_quat* C2_HOOK_CDECL BrQuatNormalise(br_quat* q, const br_quat* qq);
extern br_matrix34* C2_HOOK_CDECL BrQuatToMatrix34(br_matrix34* mat, const br_quat* q);

#include <stdarg.h>

extern const br_scalar gFLOAT_005899a0;
extern const br_scalar gFLOAT_005899a4;

#ifdef CARPOCALYPSE2_MATCHING
#if defined(_MSC_VER)
#define C2_MATCHING_INLINE __inline
#else
#define C2_MATCHING_INLINE inline
#endif
#else
#define C2_MATCHING_INLINE
#endif
#include "c2_stdlib.h"
#include "c2_string.h"

#include "carpocalypse2_macros.h"
#include "c2_math.h"

extern const char gEmpty_string_00676920[];

extern const br_scalar gFLOAT_0058a3f8;
extern const br_scalar gFLOAT_0058a40c;
extern const br_scalar gFLOAT_0058a8b8;
extern const double gDOUBLE_0058a8c0;

// GLOBAL: CARMA2_HW 0x00589954
const br_scalar gFLOAT_00589954 = 0.0f;

// GLOBAL: CARMA2_HW 0x0058a414
const br_scalar gFLOAT_0058a414 = 39.2699f;

// GLOBAL: CARMA2_HW 0x0058a420
const br_scalar gFLOAT_0058a420 = 78.5398f;

#define PHYSICS_BUFFER_PART_SIZE 40000
#define PHYSICS_BUFFER_OTHER_SIZE 50000
#include "physics.h"

// count

// DrMatrix34ApplyLPInverse

// AddObjectWallForce

// AddObjectObjectForce

// RayCastThruFaceList

// RayCastThruFaceList2

// FaceOffCount2

// FaceOffCount

// FaceOnCount

// EliminateFaces

// GetShapePoint

// GetObjectObjectEdgeIntersections

// GetFaceEdgeIntersection

// GetSphereToFacePoints

// CheckSingleEdgeWithSphere

// GetSphereToEdgePoints

// GetCornerToFacePoints

// CylinderCheck

// CheckTwoEdges

// WorldPointCollision

// CheckSingleEdgeWithObjectEdges

// GetEdgeToEdgePoints

// SetUpWorkspace

// GetObjectWallForceInfo

// GetObjectCornerToObjectFacePoints

// BoundsOverlapTest

// GetObjectObjectEdgePoints

// GetSphereNonSphereForceInfo

// GetSphereSphereForceInfo

// GetSphereShapeForceInfo

// GetNonSphereNonSphereForceInfo

// GetNonSphereShapeForceInfo

// GetObjectObjectForceInfo

// AddOldForces

// TestShapeEdgeIntersection

// TestObjectOverlap

// TestObjectOverlapIgnoreCloseRelatives

// TestObjectOverlapRecurse

// TestForObjectInSensiblePlace

// HierarchyCalcEnergy

// CheckCollisionList

// MakeJointVelocitiesVector

// ApplySingleForce

// ApplyJointForces

// SkipForces

// ScaleUpObjectMI

// CheckForTwang

// CreateJointForce

// CreateJointTorque

// AllocateObjectWorkspace

// RedoQuickHingeVelocities

// GetJointForces

// GetUniversalRotationAngle

// GetObjectsParent

// GetJointLimits

// GetListJointLimits

// GetGroupJointLimits

// LimitOmega

// CapOmega

// ListLimitOmega

// GroupLimitOmega

// PositionWorldJointedObject

// TranslateAndRotatePartialObject

// TranslateAndRotateJointedObject

// CountJointForces

// MoveJointedObject

// TellPhysicsJointsHaveChanged

// CombineBounds

// GetWorldBoundingBoxForObjects

// GetWorldBoundingBoxForObject

// MatricesAreIdentical

// InternalPrepareObject

// PrepareObject

// PrepareForCollisions

// CheckPointVelocities

// WriteNewPositions

// SetListDoingNothingFlag

// SetGroupDoingNothingFlag

// SetHierarchyDoingNothingFlag

// CheckDoingNothingFlag

// SetGroup

// GroupTogether

// GetPreCalculatedJointForces

// MakeObjectListDoSomething

// CheckForObjectTouchingAnotherObject

// CheckForObjectListTouchingAnotherObject

// CheckForObjectHierachyTouchingAnotherObject

// CheckForObjectHierachyTouchingObjectList

// MakeObjectDoSomething

// GetReducedList

// ReduceReducedList

// SetObjectTrialPositionType

// FillInTrialWorkspace

// FlagsAreGo

// CollideObjectWithReducedObjectList

// AddObjectToReducedList

// ReducedFindObjectCollisions

// FindObjectCollisions

// DoRigidJoints

// DoInterGroupCollisions

// ReduceEffectOfForces

// DoCollisions

// AddDoubleTorqueToMatrix

// DRMatrix33Inverse

// SetUpQuickHingeData

// InitQuickHinge

// CalcQuickHingeData

// SetUpForce

// SetUpTorqueHingePartners

// SetUpHingePartners

// MakeMatrix

// RestoreListOmega

// RestoreGroupOmegas

// MakeVelocitiesVector2

// MakeVelocitiesVector

// NewAddFrictionObjectObject

// NewAddFriction

// ApplyForces

// DoJointFriction

// ListDoJointFriction

// PostAddFriction

// RotateObjectSecondOrder

// RotateObjectFirstOrder

// RotateObject

// TranslateObject

// GetFacesNearObject

// ResetObjectAndChildren

// ResetObjectList

// RepositionObjectGroup

// SimpleRotate

// InterpolateSingleObject

// InterpolateObjects

// ApplyGravitySingleObject

// ApplyGravity

// SetGravityDeltaV

// TrialPositionChildren

// InternalPositionChildren

// PositionChildren

// ApplyTorque

// ApplyDiminishingTorque

// OrderObjectListOnX

// SwapPair

// CheckObjectsPostionInList

// TestForWrongAnswer

// RQIterativeImprovement

// RQExpand

// RQSingleExpand

// RQDcmpV

// MakeIdentity

// SparseRQDcmpV

// MakeMatrixFromIndices

// HasBeenDoneBefore

// RemoveRQSingularities

// AddToMatrix

// RQRemovedOneTooManyForces

// GetInverseM

// RQMatrixSolveNoNegatives

// RQMatrixSolveWithNegatives

// MatrixProcess

// RqBackSub

// GetPhysicsScratchPointer

// IncreasePhysicsScratchPointer

// ClearPhysicsScratchSpace

// AllocateShapeSphere

// AllocateShapePolyhedron

// AllocateShapeWireFrame

// AllocateShapeBox

// GetJointSize

// AllocatePhysicsJoint

// ClonePhysicsJoint

// FreeShape
typedef enum {
    ePlaneSide_positive = 0,
    ePlaneSide_negative = 1,
    ePlaneSide_zero = 2,
} tPlane_side;

typedef int C2_HOOK_FASTCALL tPhysicsObject_cbfn(tPhysics_object* pObject, void* pContext);
// GLOBAL: CARMA2_HW 0x006940c0
tPhysicsError_cbfn* gPhysics_error_cb;

// GLOBAL: CARMA2_HW 0x00744818
void* gPhysics_buffer1_ptr;

// GLOBAL: CARMA2_HW 0x00744814
void* gPhysics_buffer2_ptr;

// GLOBAL: CARMA2_HW 0x0074a5e0
void* gPhysics_buffer3_ptr;

// GLOBAL: CARMA2_HW 0x006940bc
void* gPhysics_other_buffer_cursor;

// GLOBAL: CARMA2_HW 0x006940c8
int gPhysics_other_buffer_capacity;

// GLOBAL: CARMA2_HW 0x006940c4
void* gPhysics_other_buffer;

// GLOBAL: CARMA2_HW 0x006a0adc
int gCollision_info_uid_counter;

extern const float gFLOAT_0058ac00;
// GLOBAL: CARMA2_HW 0x0058ac04
const float gFLOAT_0058ac04 = 0.0833333358168602f;

// GLOBAL: CARMA2_HW 0x0065d010
int gFace_num__car = 1;

// GLOBAL: CARMA2_HW 0x00679420
br_vector3 gPhysics_reference_normal_comparison;

// GLOBAL: CARMA2_HW 0x0067942c
tCollision_shape_polyhedron_data* gPolyhedron_to_sort;

// GLOBAL: CARMA2_HW 0x0079ecbc
tPhysics_object* gSensible_place_blocker;

// GLOBAL: CARMA2_HW 0x006923f0
tPHIL_queued_objects gPhil_queued_objects;

// GLOBAL: CARMA2_HW 0x006923e0
int gPHIL_count_list_collision_infos;

// GLOBAL: CARMA2_HW 0x00692dc0
int gPHIL_count_queued_objects;

// GLOBAL: CARMA2_HW 0x00692dbc
int gPHIL_count_queued_objects_for_removal;

// GLOBAL: CARMA2_HW 0x0074a5f0
tPhysics_object* gList_collision_infos;

// GLOBAL: CARMA2_HW 0x0074a5e8
int gPHIL_munging_objects;

// GLOBAL: CARMA2_HW 0x0074a5f8
int gCrush_deferred;

// GLOBAL: CARMA2_HW 0x006923e4
int gPHIL_object_added;

// GLOBAL: CARMA2_HW 0x0074a5ec
tU32 gPHIL_last_physics_tick;

// GLOBAL: CARMA2_HW 0x0074a5e4
int gPHIL_mechanics_time_sync;

// GLOBAL: CARMA2_HW 0x00692dcc
int gPrepared_objects;

// GLOBAL: CARMA2_HW 0x006940a0
int gINT_006940a0;

// GLOBAL: CARMA2_HW 0x006940a4
int gINT_006940a4;

// GLOBAL: CARMA2_HW 0x0065d008
br_scalar gFLOAT_0065d008 = 1.f;

// GLOBAL: CARMA2_HW 0x0065d00c
br_scalar gFLOAT_0065d00c = 1.f;

// GLOBAL: CARMA2_HW 0x00692dd0
br_scalar gFLOAT_00692dd0;

// GLOBAL: CARMA2_HW 0x00692dd4
br_scalar gFLOAT_00692dd4;

// GLOBAL: CARMA2_HW 0x0058a788
const double gDOUBLE_0058a788 = 0.0025;

// GLOBAL: CARMA2_HW 0x0058a790
const double gDOUBLE_0058a790 = -0.0025;

// GLOBAL: CARMA2_HW 0x0058a798
const double gDOUBLE_0058a798 = 0.01;

// GLOBAL: CARMA2_HW 0x0058a7c8
const br_scalar gFLOAT_0058a7c8 = 25.0f;

// GLOBAL: CARMA2_HW 0x0058a7d0
const double gDOUBLE_0058a7d0 = -0.5;

// GLOBAL: CARMA2_HW 0x0058a7e8
const br_scalar gFLOAT_0058a7e8 = 100.0f;

// GLOBAL: CARMA2_HW 0x0065d014
int gINT_0065d014 = 1;

// GLOBAL: CARMA2_HW 0x0058a7d8
const double gDOUBLE_0058a7d8 = -1.001;

// GLOBAL: CARMA2_HW 0x0058a7e0
const double gDOUBLE_0058a7e0 = 25.0;

// GLOBAL: CARMA2_HW 0x0058a830
const double gDOUBLE_0058a830 = 0.1;

// GLOBAL: CARMA2_HW 0x0058a838
const double gDOUBLE_0058a838 = -0.1;

// GLOBAL: CARMA2_HW 0x0058a840
const double gDOUBLE_0058a840 = 0.0099;

// GLOBAL: CARMA2_HW 0x0058a824
const br_scalar gFLOAT_0058a824 = 0.0016f;

// GLOBAL: CARMA2_HW 0x0058a828
const br_scalar gFLOAT_0058a828 = 2.4649f;

// GLOBAL: CARMA2_HW 0x0058a82c
const br_scalar gFLOAT_0058a82c = 1.27388525f;

// GLOBAL: CARMA2_HW 0x0058a85c
const br_scalar gFLOAT_0058a85c = 0.636942625f;

// GLOBAL: CARMA2_HW 0x0058a860
const double gDOUBLE_0058a860 = 0.0000001;

// GLOBAL: CARMA2_HW 0x0058a868
const br_scalar gFLOAT_0058a868 = 0.008f;

// GLOBAL: CARMA2_HW 0x0058a86c
const br_scalar gFLOAT_0058a86c = 31.2499981f;

// GLOBAL: CARMA2_HW 0x0058a770
const double gDOUBLE_0058a770 = 10430.378350470453;

// GLOBAL: CARMA2_HW 0x0058a780
const double gDOUBLE_0058a780 = 1.0;

// GLOBAL: CARMA2_HW 0x0058a7f0
const double gDOUBLE_0058a7f0 = 0.5;

// GLOBAL: CARMA2_HW 0x0058a7f8
const br_scalar gFLOAT_0058a7f8 = 0.04f;

// GLOBAL: CARMA2_HW 0x0058a800
const br_scalar gFLOAT_0058a800 = 1.0f;

// GLOBAL: CARMA2_HW 0x0058a804
const br_scalar gFLOAT_0058a804 = 0.1f;

// GLOBAL: CARMA2_HW 0x0058a7fc
br_scalar gFLOAT_0058a7fc = 2.384185791015625e-7f;

// GLOBAL: CARMA2_HW 0x0058a768
br_scalar gFLOAT_0058a768;

// GLOBAL: CARMA2_HW 0x00692ddc
int gINT_00692ddc;

// GLOBAL: CARMA2_HW 0x00692dd8
int gINT_00692dd8;


// GLOBAL: CARMA2_HW 0x00692de0
tCollision_contact gCollision_contacts[50];

// GLOBAL: CARMA2_HW 0x006940a8
tPhysics_object** gReduced_object_list;

// GLOBAL: CARMA2_HW 0x006940ac
int gINT_006940ac;

// GLOBAL: CARMA2_HW 0x00692dc8
int gINT_00692dc8;

// GLOBAL: CARMA2_HW 0x006923ec
tPhysics_object* gPHIL_queued_objects_for_removal[1];

// GLOBAL: CARMA2_HW 0x00692db8
tWorld_callback_active_passive_cbfn* gPHIL_original_activate_passive;

// GLOBAL: CARMA2_HW 0x006923e8
tPhysics_callbacks* gPHIL_callbacks;

// FUNCTION: CARMA2_HW 0x004c6410
void C2_HOOK_FASTCALL PHILRemoveFromParentChain(tPhysics_object* pObject) {
    tPhysics_object* parent;
    tPhysics_object* walk;

    parent = pObject->parent;
    walk = parent->child;
    if (walk == pObject) {
        parent->child = pObject->next;
    } else {
        while (walk->next != pObject) {
            walk = walk->next;
        }
        walk->next = pObject->next;
    }
    pObject->parent = NULL;
    pObject->next = NULL;
}

// FUNCTION: CARMA2_HW 0x004b9750
void C2_HOOK_FASTCALL PHILDetachObjectState(tPhysics_object* pObject) {

    pObject->field_0x218 = 0;
    ((void (C2_HOOK_FASTCALL *)(tPhysics_object*, float))MoveJointedObject)(pObject, 0.f);
    gINT_00692dc8 = 1;
}

// FUNCTION: CARMA2_HW 0x004b62e0
int C2_HOOK_FASTCALL PHILQueueDetachBit(tPhysics_object* pObject, void* pA, const br_vector3* pB, const br_vector3* pC) {
    tQueued_object_info* object_info;
    int result;

    if (gPHIL_enabled) {
        return 0;
    }
    if (gPHIL_munging_objects) {
        gPHIL_object_added = 1;
    }
    result = PHILAddObject(pObject);
    if (result == 0) {
        if (pA != NULL && !gPHIL_enabled) {
            if (pObject->field_0x239 == 2) {
                tQueued_object_info* info = pObject->field_0x240;
                memcpy(&info->field_0x30, pA, sizeof(br_matrix34));
                info->flags |= 0x4;
            } else if (pObject->field_0x240 != NULL) {
                object_info = pObject->field_0x240;
                if (object_info->field_0x8 != 1) {
                    gPHIL_object_added = 0;
                    return 4;
                }
                memcpy(&object_info->object->actor->t.t.mat, pA, sizeof(br_matrix34));
            } else {
                gPHIL_object_added = 0;
                return 3;
            }
        }
        result = PHILMakeObjectActive(pObject, pB, pC, 0);
    }
    gPHIL_object_added = 0;
    return result;
}

// FUNCTION: CARMA2_HW 0x004c80f0
void C2_HOOK_FASTCALL PHILSendDetachBit(int pFlags, void* pBit) {

    ARDoSingleVariedSession(0xe, pFlags, 2, 4, 0, (uintptr_t)pBit, 0x30, 4, (uintptr_t)((tU8*)pBit + 0x2c));
}


// FUNCTION: CARMA2_HW 0x004b5990
void C2_HOOK_FASTCALL DoPhysicsError(tPhysicsError pError, const char* pMessage) {
    char s[256];

    switch(pError) {
    case ePhysicsError_UnknownShapeType:
        strcpy(s, "Unknown shape type");
        break;
    case ePhysicsError_WireFrameHasNoPoints:
        strcpy(s, "Wire frame shape has no points");
        break;
    case ePhysicsError_PolyhedronHasNoPoints:
        strcpy(s, "Polyhedron shape has no points");
        break;
    case ePhysicsError_UnknownHingeType:
        strcpy(s, "Unknown hinge type");
    default:
        sprintf(s, "%d ", pError);
        break;
    }
    FatalError(1000 + pError, s);
}

// FUNCTION: CARMA2_HW 0x004b5aa0
void C2_HOOK_FASTCALL SetUpTestObjects(void) {

}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c61e0
void C2_HOOK_FASTCALL PhysicsNoOp(void) {
}

// FUNCTION: CARMA2_HW 0x004c61f0
void C2_HOOK_FASTCALL PhysicsSetErrorHandler(tPhysicsError_cbfn *pError_cbfn) {

    gPhysics_error_cb = pError_cbfn;
}

// FUNCTION: CARMA2_HW 0x004c61c0
void C2_NORETURN C2_HOOK_FASTCALL PhysicsError(tPhysicsError pError, const char* pMessage) {

    if (gPhysics_error_cb != NULL) {
        gPhysics_error_cb(pError, pMessage);
    }

    exit(pError);
}

// FUNCTION: CARMA2_HW 0x004b9650
int C2_HOOK_FASTCALL FUN_004b9650(tPhysics_object* pObject, tPhysics_object* pParent_context) {
    int result;
    volatile int pObject_value;
    tPhysics_joint* joint;
    tPhysics_object* child;
    tPhysics_object* child_parent_context;

    result = 0;
    pObject_value = (int)pObject;
    joint = pObject->physics_joint1;
    pObject->field_0x230 = pParent_context;
    pObject->field_0x1e4 = 0;
    if (joint != NULL && (pObject->flags & 0x400) == 0) {
        switch (joint->type) {
        case eJoint_none:
            break;
        case eJoint_hinge:
            result = 5;
            break;
        case eJoint_universal:
            result = 4;
            break;
        case eJoint_ball_n_socket:
            result = 3;
            break;
        case eJoint_quick_hinge:
            if (pParent_context->field_0x1e4 == 0) {
                result = 2;
                pObject->field_0x1e4 = -1;
                pParent_context->field_0x1e4 = -1;
                break;
            }
            joint->type = eJoint_hinge;
            result = 5;
            break;
        default:
            ((void (C2_HOOK_FASTCALL *)(tPhysicsError, const char*))PhysicsError)(8, gEmpty_string_00676920);
            result = pObject_value;
            break;
        }
    }
    if (pObject->physics_joint2 != NULL) {
        switch (pObject->physics_joint2->type) {
        case eJoint_universal:
            result += 4;
            break;
        case eJoint_ball_n_socket:
            result += 3;
            break;
        case eJoint_translation:
            result += 2;
            break;
        }
    }
    child_parent_context = (pObject->flags & 0x400) != 0 ? pParent_context : pObject;
    for (child = pObject->child; child != NULL; child = child->next) {
        result += FUN_004b9650(child, child_parent_context);
    }
    return result;
}

// C2PORT-NONMATCHING: best 43.90% | difftest PASS 2000 trials (spec difftest/specs/004c6510_InitPhysicsWorkspace.json) | reason VC5 reorders stores; callback literal pointer is relocated in traces
// FUNCTION: CARMA2_HW 0x004c6510
void C2_HOOK_FASTCALL InitPhysicsWorkspace(tU8* pBuffer, int pSize) {

    gPhysics_buffer1_ptr = pBuffer + 0 * PHYSICS_BUFFER_PART_SIZE;
    gPhysics_buffer2_ptr = pBuffer + 1 * PHYSICS_BUFFER_PART_SIZE;
    gPhysics_buffer3_ptr = pBuffer + 2 * PHYSICS_BUFFER_PART_SIZE;

    if (pSize - 3 * PHYSICS_BUFFER_PART_SIZE < PHYSICS_BUFFER_OTHER_SIZE) {
        PhysicsError(ePhysicsError_InsufficientSizedBuffer, "Physics workspace too small");
    }
    gPhysics_other_buffer_capacity = pSize - 3 * PHYSICS_BUFFER_PART_SIZE;
    gPhysics_other_buffer = pBuffer + 3 * PHYSICS_BUFFER_PART_SIZE;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004b5cc0
int C2_HOOK_FASTCALL PHILInit(void) {
    int i;

    C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers) != 100);
    C2_HOOK_BUG_ON(sizeof(gPhil_queued_objects.headers[0]) != 0x18);

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers); i++) {
        gPhil_queued_objects.headers[i].collision_info = NULL;
    }
    gPHIL_count_list_collision_infos = 0;
    gPHIL_count_queued_objects = 0;
    gPHIL_count_queued_objects_for_removal = 0;
    gList_collision_infos = NULL;
    gPHIL_munging_objects = 0;
    gCrush_deferred = 0;
    gPHIL_object_added = 0;
    gPHIL_last_physics_tick = 0;
    gPHIL_mechanics_time_sync = 1;
    gPHIL_enabled = 0;
    return 0;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 25.67% | difftest PASS 2000 trials (spec difftest/specs/004c4f40_FUN_004c4f40.json) | reason x87-order/stack-layout
// FUNCTION: CARMA2_HW 0x004c4f40
void C2_HOOK_FASTCALL FUN_004c4f40(void* pA, undefined4 pArg1, int pCount, void* pB, void* pTableArg) {
    const tU8* pbVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar7;
    float fVar8;
    float fVar9;
    float fVar10;
    float fVar11;
    tU8 bVar5;
    int bVar6;
    int iVar12;
    float* pfVar13;
    float* pfVar14;
    float* pfVar15;
    int iVar16;
    float* pfVar17;
    int iVar18;
    int iVar19;
    const float* pSource = (const float*)(uintptr_t)pArg1;
    const tU8* pTable = (const tU8*)pTableArg;
    int local_1b4;
    float* local_1b0;
    float* local_1a4;
    float local_190[101];

    iVar12 = 0;
    if (-1 < pCount) {
        bVar5 = pTable[pCount];
        pfVar13 = local_190;
        do {
            pbVar1 = pTable + iVar12;
            iVar12++;
            *pfVar13 = pSource[(tU32)*pbVar1 + (tU32)bVar5 * 100];
            pfVar13++;
        } while (iVar12 <= pCount);
    }
    iVar12 = 0;
    pfVar13 = (float*)pB;
    if (0 < pCount) {
        do {
            pfVar17 = local_190;
            pfVar14 = pfVar13;
            iVar16 = pCount;
            fVar8 = gFLOAT_0058a8b8;
            do {
                fVar2 = *pfVar14;
                fVar3 = *pfVar17;
                pfVar14++;
                pfVar17++;
                iVar16--;
                fVar8 = fVar2 * fVar3 + fVar8;
            } while (iVar16 != 0);
            iVar16 = iVar12 + pCount * 100;
            iVar12++;
            ((float*)pA)[iVar16] = fVar8;
            pfVar13 += 100;
        } while (iVar12 < pCount);
    }
    ((float*)pB)[pCount * 0x65] = 1.0f;
    if (0 < pCount) {
        pfVar13 = (float*)pB + pCount;
        pfVar17 = (float*)pB + pCount * 100;
        iVar12 = pCount;
        do {
            *pfVar13 = 0.0f;
            *pfVar17 = 0.0f;
            pfVar13 += 100;
            pfVar17++;
            iVar12--;
        } while (iVar12 != 0);
        if (0 < pCount) {
            local_1a4 = (float*)pB;
            iVar16 = (int)((float*)pB + pCount * 100) - (int)pB;
            local_1b0 = (float*)pA + pCount;
            pfVar13 = local_190;
            local_1b4 = 400;
            iVar12 = 1;
            pfVar17 = (float*)pA;
            do {
                fVar8 = fabsf(*pfVar13);
                *local_1b0 = 0.0f;
                if ((double)fVar8 != gDOUBLE_0058a8c0) {
                    fVar2 = fabsf(*pfVar17);
                    if (fVar2 < fVar8) {
                        fVar2 = fVar8;
                    }
                    fVar8 = *pfVar13 / fVar2;
                    fVar3 = *pfVar17 / fVar2;
                    *pfVar13 = fVar8;
                    fVar9 = fabsf(sqrtf(fVar3 * fVar3 + fVar8 * fVar8));
                    if ((double)fVar3 <= gDOUBLE_0058a8c0) {
                        fVar9 = -fVar9;
                    }
                    iVar19 = pCount + 1;
                    fVar3 = fVar3 + fVar9;
                    fVar10 = fVar8 / (fVar3 * fVar9);
                    fVar11 = fVar3 / (fVar3 * fVar9);
                    pfVar14 = local_1a4;
                    iVar18 = iVar19;
                    if (0 < iVar19) {
                        do {
                            fVar4 = fVar8 * *(float*)(iVar16 + (int)pfVar14) + fVar3 * *pfVar14;
                            iVar18--;
                            *(float*)(iVar16 - 4 + (int)(pfVar14 + 1)) = *(float*)(iVar16 + (int)pfVar14) - fVar10 * fVar4;
                            *pfVar14 = *pfVar14 - fVar11 * fVar4;
                            pfVar14++;
                        } while (iVar18 != 0);
                    }
                    if (iVar12 < iVar19) {
                        iVar19 -= iVar12;
                        pfVar15 = (float*)((int)pA + (local_1b4 - (int)local_190) + (int)pfVar13);
                        pfVar14 = pfVar13;
                        do {
                            pfVar14++;
                            fVar7 = fVar3 * *pfVar15 + fVar8 * *pfVar14;
                            iVar19--;
                            fVar4 = *pfVar15;
                            *pfVar14 = *pfVar14 - fVar10 * fVar7;
                            *pfVar15 = fVar4 - fVar11 * fVar7;
                            pfVar15 += 100;
                        } while (iVar19 != 0);
                    }
                    *pfVar17 = -(fVar9 * fVar2);
                }
                local_1b0 += 100;
                iVar16 -= 400;
                local_1b4 += 400;
                pfVar13++;
                local_1a4 += 100;
                pfVar17 += 0x65;
                bVar6 = iVar12 < pCount;
                iVar12++;
            } while (bVar6);
        }
    }
    ((float*)pA)[pCount * 0x65] = local_190[pCount];
}

// C2PORT-NONMATCHING: best 22.96% | difftest PASS 2000 trials (spec difftest/specs/004c4850_FUN_004c4850.json) | reason stack-layout-x87-order
// FUNCTION: CARMA2_HW 0x004c4850
void C2_HOOK_FASTCALL FUN_004c4850(float* param_1, int param_2, int param_3, int param_4, float* param_5, int param_6) {
    typedef unsigned char byte;
    typedef unsigned int uint;
  float local_190 [100];
  byte *pbVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  byte bVar8;
  int iVar9;
  float *pfVar10;
  float *pfVar11;
  float *pfVar12;
  float *pfVar13;
  float *pfVar14;
  int iVar15;
  int local_1bc;
  int local_1b8_offset;
  float *local_1b8;
  float local_1b4;
  float *local_1b0;
  float *local_1ac;
  int local_1a0;
  float *local_19c;
  float *local_198;

  local_1bc = param_3;
  if (param_3 < param_4) {
    local_1b8_offset = param_3 * 100;
    local_1ac = param_1 + param_3;
    do {
      bVar8 = *(byte *)(local_1bc + param_6);
      iVar9 = 0;
      if (0 < param_3) {
        local_1b0 = local_1ac;
        pfVar14 = local_190;
        do {
          pbVar1 = (byte *)(iVar9 + param_6);
          iVar9 = iVar9 + 1;
          fVar2 = *(float *)((uint)bVar8 * 400 + param_2 + (uint)*pbVar1 * 4);
          *pfVar14 = fVar2;
          *local_1b0 = fVar2;
          local_1b0 = local_1b0 + 100;
          pfVar14 = pfVar14 + 1;
        } while (iVar9 < param_3);
      }
      iVar9 = 0;
      pfVar14 = param_5;
      if (0 < param_3) {
        do {
          pfVar11 = local_190;
          pfVar13 = pfVar14;
          iVar15 = param_3;
          fVar2 = gFLOAT_0058a8b8;
          do {
            fVar3 = *pfVar11;
            fVar4 = *pfVar13;
            pfVar13 = pfVar13 + 1;
            pfVar11 = pfVar11 + 1;
            iVar15 = iVar15 + -1;
            fVar2 = fVar3 * fVar4 + fVar2;
          } while (iVar15 != 0);
          iVar15 = local_1b8_offset + iVar9;
          iVar9 = iVar9 + 1;
          param_1[iVar15] = fVar2;
          pfVar14 = pfVar14 + 100;
        } while (iVar9 < param_3);
      }
      local_1bc = local_1bc + 1;
      local_1ac = local_1ac + 1;
      local_1b8_offset = local_1b8_offset + 100;
    } while (local_1bc < param_4);
  }
  if (param_3 < param_4) {
    pfVar14 = param_5 + param_3 * 0x65;
    local_1b8 = param_5 + param_3;
    local_1ac = param_5 + param_3 * 100;
    iVar9 = param_3;
    local_198 = pfVar14;
    do {
      bVar8 = *(byte *)(iVar9 + param_6);
      pfVar11 = pfVar14;
      iVar15 = param_3;
      do {
        pbVar1 = (byte *)(iVar15 + param_6);
        iVar15 = iVar15 + 1;
        *(undefined4 *)((int)param_1 + (-4 - (int)param_5) + (int)(pfVar11 + 1)) =
             *(undefined4 *)((uint)bVar8 * 400 + param_2 + (uint)*pbVar1 * 4);
        *pfVar11 = 0.0;
        pfVar11 = pfVar11 + 1;
      } while (iVar15 < param_4);
      pfVar11 = local_1ac;
      pfVar13 = local_1b8;
      iVar15 = param_3;
      if (0 < param_3) {
        do {
          *pfVar11 = 0.0;
          *pfVar13 = 0.0;
          iVar15 = iVar15 + -1;
          pfVar11 = pfVar11 + 1;
          pfVar13 = pfVar13 + 100;
        } while (iVar15 != 0);
      }
      *local_198 = 1.0;
      iVar9 = iVar9 + 1;
      pfVar14 = pfVar14 + 100;
      local_1ac = local_1ac + 100;
      local_1b8 = local_1b8 + 1;
      local_198 = local_198 + 0x65;
    } while (iVar9 < param_4);
  }
  local_1ac = (float *)0x0;
  if (0 < param_3) {
    local_19c = param_1 + param_3 + 100;
    local_198 = (float *)0x64;
    pfVar14 = param_1;
    local_1b0 = param_1;
    do {
      fVar2 = pfVar14[(int)local_1ac];
      local_1b4 = 0.0;
      if (param_3 < param_4) {
        pfVar11 = pfVar14 + param_3;
        iVar9 = param_4 - param_3;
        do {
          fVar3 = fabsf(*pfVar11);
          if (fabsf(*pfVar11) < local_1b4) {
            fVar3 = local_1b4;
          }
          pfVar11 = pfVar11 + 1;
          iVar9 = iVar9 + -1;
          local_1b4 = fVar3;
        } while (iVar9 != 0);
      }
      if (local_1b4 != (float)gDOUBLE_0058a8c0) {
        fVar3 = fabsf(fVar2);
        if (fVar3 < local_1b4) {
          fVar3 = local_1b4;
        }
        if (param_3 < param_4) {
          iVar9 = param_4 - param_3;
          pfVar11 = pfVar14 + param_3;
          do {
            iVar9 = iVar9 + -1;
            *pfVar11 = *pfVar11 / fVar3;
            pfVar11 = pfVar11 + 1;
          } while (iVar9 != 0);
        }
        fVar2 = fVar2 / fVar3;
        fVar4 = gFLOAT_0058a8b8;
        if (param_3 < param_4) {
          pfVar11 = pfVar14 + param_3;
          iVar9 = param_4 - param_3;
          do {
            fVar5 = *pfVar11;
            pfVar11 = pfVar11 + 1;
            iVar9 = iVar9 + -1;
            fVar4 = fVar5 * fVar5 + fVar4;
          } while (iVar9 != 0);
        }
        fVar4 = fabsf(sqrtf(fVar2 * fVar2 + fVar4));
        if (fVar2 <= (float)gDOUBLE_0058a8c0) {
          fVar4 = -fVar4;
        }
        fVar2 = fVar2 + fVar4;
        iVar9 = 0;
        if (0 < param_4) {
          pfVar11 = (float *)(((int)param_5 - (int)param_1) + (int)pfVar14);
          do {
            fVar5 = fVar2 * *pfVar11;
            if (param_3 < param_4) {
              pfVar10 = pfVar14 + param_3;
              pfVar13 = param_5 + iVar9 + param_3 * 100;
              iVar15 = param_4 - param_3;
              do {
                fVar6 = *pfVar13;
                fVar7 = *pfVar10;
                pfVar13 = pfVar13 + 100;
                pfVar10 = pfVar10 + 1;
                iVar15 = iVar15 + -1;
                fVar5 = fVar6 * fVar7 + fVar5;
              } while (iVar15 != 0);
            }
            fVar5 = fVar5 / (fVar2 * fVar4);
            if (param_3 < param_4) {
              pfVar13 = pfVar14 + param_3;
              iVar15 = param_4 - param_3;
              pfVar10 = param_5 + iVar9 + param_3 * 100;
              do {
                fVar6 = *pfVar13;
                pfVar13 = pfVar13 + 1;
                iVar15 = iVar15 + -1;
                *pfVar10 = *pfVar10 - fVar5 * fVar6;
                pfVar10 = pfVar10 + 100;
              } while (iVar15 != 0);
            }
            iVar9 = iVar9 + 1;
            *pfVar11 = *pfVar11 - fVar2 * fVar5;
            pfVar11 = pfVar11 + 1;
          } while (iVar9 < param_4);
        }
        if ((int)local_1ac + 1 < param_4) {
          local_1a0 = param_4 - ((int)local_1ac + 1);
          pfVar11 = param_1 + (int)local_1ac + (int)local_198;
          pfVar13 = local_19c;
          do {
            fVar5 = fVar2 * *pfVar11;
            if (param_3 < param_4) {
              pfVar10 = pfVar14 + param_3;
              iVar9 = param_4 - param_3;
              pfVar12 = pfVar13;
              do {
                fVar6 = *pfVar12;
                fVar7 = *pfVar10;
                pfVar12 = pfVar12 + 1;
                pfVar10 = pfVar10 + 1;
                iVar9 = iVar9 + -1;
                fVar5 = fVar6 * fVar7 + fVar5;
              } while (iVar9 != 0);
            }
            fVar5 = fVar5 / (fVar2 * fVar4);
            if (param_3 < param_4) {
              pfVar10 = pfVar14 + param_3;
              iVar9 = param_4 - param_3;
              pfVar12 = pfVar13;
              do {
                fVar6 = *pfVar10;
                pfVar10 = pfVar10 + 1;
                iVar9 = iVar9 + -1;
                *pfVar12 = *pfVar12 - fVar5 * fVar6;
                pfVar12 = pfVar12 + 1;
              } while (iVar9 != 0);
            }
            pfVar13 = pfVar13 + 100;
            local_1a0 = local_1a0 + -1;
            *pfVar11 = *pfVar11 - fVar2 * fVar5;
            pfVar11 = pfVar11 + 100;
          } while (local_1a0 != 0);
        }
        *local_1b0 = -(fVar4 * fVar3);
        if (param_3 < param_4) {
          pfVar11 = pfVar14 + param_3;
          for (iVar9 = param_4 - param_3; iVar9 != 0; iVar9 = iVar9 + -1) {
            *pfVar11 = 0.0;
            pfVar11 = pfVar11 + 1;
          }
        }
      }
      local_1ac = (float *)((int)local_1ac + 1);
      pfVar14 = pfVar14 + 100;
      local_198 = (float *)((int)local_198 + 100);
      local_19c = local_19c + 100;
      local_1b0 = local_1b0 + 0x65;
    } while ((int)local_1ac < param_3);
  }
  if (param_3 < param_4) {
    local_1bc = param_3 * 100;
    pfVar14 = param_1 + param_3 * 0x65;
    do {
      local_1b4 = 0.0;
      if (param_3 < param_4) {
        iVar9 = param_4 - param_3;
        pfVar11 = pfVar14;
        do {
          fVar2 = fabsf(*pfVar11);
          if (fabsf(*pfVar11) < local_1b4) {
            fVar2 = local_1b4;
          }
          pfVar11 = pfVar11 + 1;
          iVar9 = iVar9 + -1;
          local_1b4 = fVar2;
        } while (iVar9 != 0);
      }
      if (local_1b4 != gFLOAT_0058a8b8) {
        if (param_3 < param_4) {
          iVar9 = param_4 - param_3;
          pfVar11 = pfVar14;
          do {
            iVar9 = iVar9 + -1;
            *pfVar11 = *pfVar11 / local_1b4;
            pfVar11 = pfVar11 + 1;
          } while (iVar9 != 0);
        }
        fVar2 = gFLOAT_0058a8b8;
        if (param_3 < param_4) {
          iVar9 = param_4 - param_3;
          pfVar11 = pfVar14;
          do {
            fVar3 = *pfVar11;
            pfVar11 = pfVar11 + 1;
            iVar9 = iVar9 + -1;
            fVar2 = fVar3 * fVar3 + fVar2;
          } while (iVar9 != 0);
        }
        fVar2 = fabsf(sqrtf(fVar2));
        if (*pfVar14 <= (float)gDOUBLE_0058a8c0) {
          fVar2 = -fVar2;
        }
        fVar3 = fVar2 * (fVar2 + *pfVar14);
        *pfVar14 = fVar2 + *pfVar14;
        iVar9 = 0;
        if (0 < param_4) {
          do {
            fVar4 = gFLOAT_0058a8b8;
            if (param_3 < param_4) {
              pfVar11 = param_5 + local_1bc + iVar9;
              iVar15 = param_4 - param_3;
              pfVar13 = pfVar14;
              do {
                fVar5 = *pfVar13;
                fVar6 = *pfVar11;
                pfVar13 = pfVar13 + 1;
                pfVar11 = pfVar11 + 100;
                iVar15 = iVar15 + -1;
                fVar4 = fVar5 * fVar6 + fVar4;
              } while (iVar15 != 0);
            }
            if (param_3 < param_4) {
              iVar15 = param_4 - param_3;
              pfVar11 = param_5 + iVar9 + local_1bc;
              pfVar13 = pfVar14;
              do {
                fVar5 = *pfVar13;
                pfVar13 = pfVar13 + 1;
                iVar15 = iVar15 + -1;
                *pfVar11 = *pfVar11 - (fVar4 / fVar3) * fVar5;
                pfVar11 = pfVar11 + 100;
              } while (iVar15 != 0);
            }
            iVar9 = iVar9 + 1;
          } while (iVar9 < param_4);
        }
        if (param_3 + 1 < param_4) {
          pfVar11 = param_1 + local_1bc + param_3 + 100;
          local_1a0 = param_4 - (param_3 + 1);
          do {
            fVar4 = gFLOAT_0058a8b8;
            if (param_3 < param_4) {
              iVar9 = param_4 - param_3;
              pfVar13 = pfVar14;
              pfVar10 = pfVar11;
              do {
                fVar5 = *pfVar10;
                fVar6 = *pfVar13;
                pfVar13 = pfVar13 + 1;
                pfVar10 = pfVar10 + 1;
                iVar9 = iVar9 + -1;
                fVar4 = fVar5 * fVar6 + fVar4;
              } while (iVar9 != 0);
            }
            if (param_3 < param_4) {
              iVar9 = param_4 - param_3;
              pfVar13 = pfVar11;
              pfVar10 = pfVar14;
              do {
                fVar5 = *pfVar10;
                pfVar10 = pfVar10 + 1;
                iVar9 = iVar9 + -1;
                *pfVar13 = *pfVar13 - (fVar4 / fVar3) * fVar5;
                pfVar13 = pfVar13 + 1;
              } while (iVar9 != 0);
            }
            pfVar11 = pfVar11 + 100;
            local_1a0 = local_1a0 + -1;
          } while (local_1a0 != 0);
        }
        iVar9 = param_3 + 1;
        *pfVar14 = -(fVar2 * local_1b4);
        if (iVar9 < param_4) {
          pfVar11 = param_1 + local_1bc + iVar9;
          for (iVar15 = param_4 - iVar9; iVar15 != 0; iVar15 = iVar15 + -1) {
            *pfVar11 = 0.0;
            pfVar11 = pfVar11 + 1;
          }
        }
      }
      param_3 = param_3 + 1;
      pfVar14 = pfVar14 + 0x65;
      local_1bc = local_1bc + 100;
    } while (param_3 < param_4);
  }
  return;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 39.53% | difftest PASS 2000 trials (spec difftest/specs/004c5220_FUN_004c5220.json) | reason pointer-arith-codegen
// FUNCTION: CARMA2_HW 0x004c5220
void C2_HOOK_FASTCALL FUN_004c5220(int pParam1, int pParam2, undefined4 pArg1, undefined4* pArg2, int* pArg3, tU8* pArg4, int pArg5, int** pArg6, int pCount) {
    int iVar2;
    int iVar3;
    int iVar5;
    int iVar6;
    int iVar7;
    int local_14;
    int local_18;
    int local_c;
    int* param_9_val;
    int* param_8;
    int** param_9;
    undefined4* puVar4;
    undefined4* puVar8;
    undefined4* puVar9;
    undefined4* puVar10;

    iVar5 = 0;
    if (0 < pCount) {
        local_c = pCount;
        param_9 = pArg6;
        do {
            param_9_val = (int*)*param_9;
            iVar2 = *param_9_val;
            if (0 < iVar2) {
                param_8 = (int*)(pParam2 + iVar5 * 4);
                local_18 = 0;
                puVar4 = (undefined4*)(iVar5 * 400 + pParam2);
                puVar9 = (undefined4*)(pParam2 + iVar5 * 0x194);
                local_14 = iVar2;
                do {
                    iVar3 = pParam1 - pParam2;
                    iVar6 = iVar2;
                    iVar7 = local_18;
                    puVar8 = puVar9;
                    do {
                        iVar7 += 4;
                        iVar6--;
                        *(undefined4*)((char*)puVar8 + iVar3) = *(undefined4*)((char*)*(int*)((char*)param_9_val + 0x18) + iVar7 - 4);
                        *puVar8 = *(undefined4*)((char*)*(int*)((char*)param_9_val + 0x14) + iVar7 - 4);
                        puVar8++;
                    } while (iVar6 != 0);
                    iVar6 = iVar5;
                    puVar8 = puVar4;
                    puVar10 = (undefined4*)param_8;
                    if (0 < iVar5) {
                        do {
                            *(undefined4*)((char*)puVar10 + iVar3) = 0;
                            *(undefined4*)((char*)puVar8 + iVar3) = 0;
                            *puVar10 = 0;
                            *puVar8 = 0;
                            iVar6--;
                            puVar8++;
                            puVar10 += 100;
                        } while (iVar6 != 0);
                    }
                    param_8 = param_8 + 1;
                    local_18 = local_18 + iVar2 * 4;
                    puVar9 += 100;
                    puVar4 += 100;
                    local_14--;
                } while (local_14 != 0);
            }
            iVar5 += iVar2;
            param_9 = param_9 + 1;
            local_c--;
        } while (local_c != 0);
    }
    if (pArg5 - iVar5 == 1) {
        FUN_004c4f40(pParam1, pArg1, iVar5, (void*)pParam2, pArg4);
    } else if (iVar5 < pArg5) {
        FUN_004c4850((float*)(uintptr_t)pParam1, pArg1, iVar5, pArg5, (float*)pParam2, (int)(uintptr_t)pArg4);
    }
    iVar5 = 0;
    if (0 < pArg5) {
        do {
            *pArg2 = pArg3[pArg4[iVar5]];
            iVar5++;
            pArg2++;
        } while (iVar5 < pArg5);
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c5e00
tCollision_shape_box* C2_HOOK_FASTCALL AllocateBoxCollisionShape(int pType) {
    tCollision_shape_box* result;

    C2_HOOK_BUG_ON(sizeof(tCollision_shape_box) != 56);
    result = BrMemAllocate(sizeof(tCollision_shape_box), pType);
    result->common.type = kCollisionShapeType_Box;
    return result;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c5d10
tCollision_shape_sphere* C2_HOOK_FASTCALL AllocateShapeSphere(int pType) {
    tCollision_shape_sphere* result;

    C2_HOOK_BUG_ON(sizeof(tCollision_shape_sphere) != 76);
    result = BrMemAllocate(sizeof(tCollision_shape_sphere), pType);
    result->common.type = kCollisionShapeType_Sphere;
    return result;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c5d30
tCollision_shape_polyhedron* C2_HOOK_FASTCALL AllocateShapeWireframePolyhedron(int pCount_points, int pType) {
    tCollision_shape_polyhedron* result;

#ifdef __vita__
    result = BrMemAllocate(sizeof(*result) + pCount_points *
        (sizeof(br_vector3) + sizeof(tPolyhedron_edge_indexes) + sizeof(br_vector4))
        + 2 * sizeof(br_vector4) + 3, pType);
#else
    result = BrMemAllocate(pCount_points * 30 + 0x70, pType);
#endif
    result->polyhedron.points = (br_vector3*)((tU8*)result + sizeof(tCollision_shape_polyhedron));
    result->polyhedron.count_points = pCount_points;
    result->polyhedron.edges = (tPolyhedron_edge_indexes*)(result->polyhedron.points + pCount_points);
    result->polyhedron.planes = (br_vector4*)((tU8*)result->polyhedron.edges + pCount_points * sizeof(tPolyhedron_edge_indexes));
#ifdef __vita__
    /* An odd number of byte-sized edge pairs leaves VFP data halfword aligned. */
    result->polyhedron.planes = (br_vector4*)(((uintptr_t)result->polyhedron.planes + 3) & ~(uintptr_t)3);
#endif
    result->polyhedron.count_edges = 0;
    result->polyhedron.count_planes = 0;
    result->common.type = kCollisionShapeType_Wireframe_Polyhedron;
    return result;
}

// FUNCTION: CARMA2_HW 0x004c5d70
tCollision_shape_polyhedron* C2_HOOK_FASTCALL AllocateShapePolyhedron(int pCount_points, br_uint_8 pType) {
    tCollision_shape_polyhedron* result;
    tU8* raw_memory;

    C2_HOOK_BUG_ON(sizeof(tCollision_shape_polyhedron) != 80);
    C2_HOOK_BUG_ON(sizeof(tPolyhedron_edge_indexes) != 2);
#ifdef __vita__
    result = BrMemAllocate(sizeof(*result) + pCount_points * sizeof(br_vector3)
        + (pCount_points - 2) * (3 * sizeof(tPolyhedron_edge_indexes) + 2 * sizeof(br_vector4)) + 3, pType);
#else
    result = BrMemAllocate(sizeof(tCollision_shape_polyhedron) * pCount_points * sizeof(br_vector3) + (pCount_points + -2) * 38, pType); /* FIXME: 38? */
#endif
    raw_memory = (tU8*)result;

    raw_memory += sizeof(tCollision_shape_polyhedron);

    result->polyhedron.points = (br_vector3*)raw_memory;
    raw_memory += pCount_points * sizeof(br_vector3);

    result->polyhedron.edges = (tPolyhedron_edge_indexes*)raw_memory;
    raw_memory += (pCount_points - 2) * 3 * sizeof(tPolyhedron_edge_indexes);

#ifdef __vita__
    raw_memory = (tU8*)(((uintptr_t)raw_memory + 3) & ~(uintptr_t)3);
#endif
    result->polyhedron.planes = (br_vector4*)raw_memory;

    result->polyhedron.count_points = 0;
    result->polyhedron.count_edges = 0;
    result->polyhedron.count_planes = 0;
    result->common.type = kCollisionShapeType_Polyhedron;
    return result;
}

// FUNCTION: CARMA2_HW 0x004c5dc0
tCollision_shape_wireframe* C2_HOOK_FASTCALL AllocateWireFrameCollisionShape(int pCount_points, int pCount_lines, br_uint_8 pType) {
    tCollision_shape_wireframe* result;
    tU8* raw_memory;

    C2_HOOK_BUG_ON(sizeof(tCollision_shape_wireframe) != 72);
    C2_HOOK_BUG_ON(sizeof(tPolyhedron_edge_indexes) != 2);
    result = BrMemAllocate(sizeof(tCollision_shape_wireframe) + pCount_points * sizeof(br_vector3) + pCount_lines * sizeof(tPolyhedron_edge_indexes), pType);
    raw_memory = (tU8*)result;

    raw_memory += sizeof(tCollision_shape_wireframe);

    result->wireframe.points = (br_vector3*)raw_memory;
    raw_memory += pCount_points * sizeof(br_vector3);

    result->wireframe.lines = (tPolyhedron_edge_indexes*)raw_memory;
    result->wireframe.count_lines = pCount_lines;
    result->wireframe.count_points = pCount_points;
    result->common.type = kCollisionShapeType_Wireframe;
    return result;
}

// C2PORT-NONMATCHING: best 29.31% | difftest PASS 2000 trials (spec difftest/specs/00488b00_ReadMechanicsShapes.json) | reason parser stack layout and temporary output scheduling
// FUNCTION: CARMA2_HW 0x00488b00
void C2_HOOK_FASTCALL ReadMechanicsShapes(tPhysics_shape** pShape, FILE* pF) {
    int i;
    int count;

    /* Number of 'Bounding shapes' entries. */
    count = GetAnInt(pF);

    for (i = 0; i < count; i++) {
        char s[256];
        tPhysics_shape* shape;

        GetAString(pF, s);
        if (DRStricmp(s, "box") == 0) {

            shape = (tPhysics_shape*)AllocateBoxCollisionShape(kMem_collision_shape);
            GetThreeFloats(pF,
                &shape->box.common.bb.min.v[0],
                &shape->box.common.bb.min.v[1],
                &shape->box.common.bb.min.v[2]);
            GetThreeFloats(pF,
                &shape->box.common.bb.max.v[0],
                &shape->box.common.bb.max.v[1],
                &shape->box.common.bb.max.v[2]);
        } else if (DRStricmp(s, "polyhedron") == 0) {
            int count_points;
            int j;

            /* number of points */
            count_points = GetAnInt(pF);
            if (count_points > 32) { /* FIXME: magic number */
                sprintf(s, "Ow! Physics shape is mad (having %d points is silly)", count_points);
                PDFatalError(s);
            }
            shape = (tPhysics_shape*)AllocateShapePolyhedron(count_points, kMem_collision_shape);

            shape->polyhedron.polyhedron.count_points = count_points;
            for (j = 0; j < count_points; j++) {
                GetThreeFloats(pF,
                    &shape->polyhedron.polyhedron.points[j].v[0],
                    &shape->polyhedron.polyhedron.points[j].v[1],
                    &shape->polyhedron.polyhedron.points[j].v[2]);
            }
        } else if (DRStricmp(s, "sphere") == 0) {

            shape = (tPhysics_shape*)AllocateShapeSphere(kMem_collision_shape);

            shape->sphere.sphere.radius = GetAScalar(pF);
            GetThreeFloats(pF,
                &shape->sphere.sphere.center.v[0],
                &shape->sphere.sphere.center.v[1],
                &shape->sphere.sphere.center.v[2]);
        } else if (DRStricmp(s, "wire") == 0) {
            br_vector3 points[64];
            int count_points;
            int count_lines;
            int j;

            count_points = GetAnInt(pF);
            if (count_points > CARPOCALYPSE2_ASIZE(points)) {
                FatalError(kFatalError_TooManyExtraPointsForCarIndex_S, " Too many points in wire frame shape ");
            }
            for (j = 0; j < count_points; j++) {
                GetThreeFloats(pF, &points[j].v[0], &points[j].v[1], &points[j].v[2]);
            }
            count_lines = GetAnInt(pF);
            shape = (tPhysics_shape*)AllocateWireFrameCollisionShape(count_points, count_lines, kMem_collision_shape);

            for (j = 0; j < count_points; j++) {
                BrVector3Copy(&shape->wireframe.wireframe.points[j], &points[j]);
            }
            for (j = 0; j < count_lines; j++) {
                int index1, index2;

                GetPairOfInts(pF, &index1, &index2);
                shape->wireframe.wireframe.lines[j].index1 = index1;
                shape->wireframe.wireframe.lines[j].index2 = index2;
            }
        } else {
            FatalError(kFatalError_ShapeDataIsWrong);
        }
        FillInShape(shape);
        *pShape = shape;
        pShape = &shape->common.next;
    }
}

// FUNCTION: CARMA2_HW 0x004c60a0
void C2_HOOK_FASTCALL UpdateCollisionObject(tPhysics_object* pCollision_info) {
    tPhysics_shape* shape;
    tPhysics_shape* current;

    shape = pCollision_info->shape;
    if (shape == NULL) {
        return;
    }
    pCollision_info->bb1 = shape->common.bb;

    for (current = shape->common.next; current != NULL; current = current->common.next) {
        int i;

        for (i = 0; i < 3; i++) {
            pCollision_info->bb1.min.v[i] = pCollision_info->bb1.min.v[i] < current->common.bb.min.v[i]
                ? pCollision_info->bb1.min.v[i] : current->common.bb.min.v[i];
            pCollision_info->bb1.max.v[i] = pCollision_info->bb1.max.v[i] > current->common.bb.max.v[i]
                ? pCollision_info->bb1.max.v[i] : current->common.bb.max.v[i];
        }
    }
    pCollision_info->bb2 = pCollision_info->bb1;

    pCollision_info->radius_squared = MAX(CARPOCALYPSE2_SQR(pCollision_info->bb2.min.v[0]), CARPOCALYPSE2_SQR(pCollision_info->bb2.max.v[0]))
            + MAX(CARPOCALYPSE2_SQR(pCollision_info->bb2.min.v[1]), CARPOCALYPSE2_SQR(pCollision_info->bb2.max.v[1]))
            + MAX(CARPOCALYPSE2_SQR(pCollision_info->bb2.min.v[2]), CARPOCALYPSE2_SQR(pCollision_info->bb2.max.v[2]));
    pCollision_info->radius = sqrtf(pCollision_info->radius_squared);
}

// C2PORT-NONMATCHING: best 65.75% | difftest PASS 2000 trials (spec difftest/specs/00420ef0_ArePointsColinear.json) | reason x87-cross-product-order
// FUNCTION: CARMA2_HW 0x00420ef0
int C2_HOOK_FASTCALL ArePointsColinear(const br_vector3* pV1, const br_vector3* pV2, const br_vector3* pV3) {
    br_scalar dz31;
    br_scalar dy31;
    br_scalar dy21;
    br_scalar dz21;
    br_scalar dx21;
    br_scalar dx31;
    br_scalar cross;

    dz31 = pV3->v[2] - pV1->v[2];
    dy31 = pV3->v[1] - pV1->v[1];
    dy21 = pV2->v[1] - pV1->v[1];
    dz21 = pV2->v[2] - pV1->v[2];
    cross = dz31 * dy21 - dy31 * dz21;
    if (fabsf(cross) >= 1.0e-6f) {
        goto not_collinear;
    }
    dx21 = pV2->v[0] - pV1->v[0];
    dx31 = pV3->v[0] - pV1->v[0];
    cross = dx31 * dz21 - dx21 * dz31;
    if (fabsf(cross) >= 1.0e-6f) {
        goto not_collinear;
    }
    cross = dx21 * dy31 - dx31 * dy21;
    if (fabsf(cross) >= 1.0e-6f) {
        goto not_collinear;
    }
    return 1;
not_collinear:
    return 0;
}

static br_scalar C2_HOOK_FASTCALL PlaneValue(const br_vector4* pPlane, const br_vector3* pP) {

    return BrVector3Dot(pPlane, pP) + pPlane->v[3];
}

// C2PORT-NONMATCHING: best 28.94% | difftest PASS 2000 trials (spec difftest/specs/004211f0_ProcessTetrahedronPolyhedronCollisionShape.json) | reason x87-geometry-codegen
// FUNCTION: CARMA2_HW 0x004211f0
tPhysicsError C2_HOOK_FASTCALL ProcessTetrahedronPolyhedronCollisionShape(tCollision_shape_polyhedron_data* pPolyhedron, tPolyhedron_edge_indexes* pEdges) {
    int first_non_colinear;
    int i;
    br_vector3 tv2;
    br_vector3 tv1;

    first_non_colinear = 0;
    while (ArePointsColinear(&pPolyhedron->points[first_non_colinear + 0], &pPolyhedron->points[first_non_colinear + 1], &pPolyhedron->points[first_non_colinear + 2])) {
        first_non_colinear++;
        if (first_non_colinear == pPolyhedron->count_points - 3) {
            return ePhysicsError_UnknownShapeType;
        }
    }

    if (first_non_colinear != 0) {
        BrVector3Copy(&pPolyhedron->points[0], &pPolyhedron->points[first_non_colinear + 0]);
        BrVector3Copy(&pPolyhedron->points[1], &pPolyhedron->points[first_non_colinear + 1]);
        BrVector3Copy(&pPolyhedron->points[2], &pPolyhedron->points[first_non_colinear + 2]);
    }
    pPolyhedron->edges[0].index1 = 0;
    pPolyhedron->edges[0].index2 = 1;
    pPolyhedron->edges[1].index1 = 1;
    pPolyhedron->edges[1].index2 = 2;
    pPolyhedron->edges[2].index1 = 0;
    pPolyhedron->edges[2].index2 = 2;

    /* PlaneEquation */
    BrVector3Sub(&tv1, &pPolyhedron->points[2], &pPolyhedron->points[1]);
    BrVector3Sub(&tv2, &pPolyhedron->points[1], &pPolyhedron->points[0]);
    BrVector3Cross(&pPolyhedron->planes[0], &tv2, &tv1);
    BrVector3Normalise(&pPolyhedron->planes[0], &pPolyhedron->planes[0]);
    pPolyhedron->planes[0].v[3] = -BrVector3Dot(&pPolyhedron->planes[0], &pPolyhedron->points[0]);

    pEdges[2].index1 = 0;
    pEdges[1].index1 = 0;
    pEdges[0].index1 = 0;

    for (i = 0; i < pPolyhedron->count_points; i++) {

        if (fabsf(PlaneValue(&pPolyhedron->planes[0], &pPolyhedron->points[i])) > 1e-6f) {
            break;
        }
    }
    if (i == pPolyhedron->count_points) {
        return 7;
    }
    BrVector3Copy(&pPolyhedron->points[3], &pPolyhedron->points[i]);
    /* Points inside the polyhedron must have positive plane value */
    if (PlaneValue(&pPolyhedron->planes[0], &pPolyhedron->points[i]) < 1e-6f) {
        BrVector4Negate(&pPolyhedron->planes[0], &pPolyhedron->planes[0]);
    }
    for (i = 0; i < 3; i++) {
        int o;
        int o_i;

        o = i + 1;
        o_i = o % 3;

        pPolyhedron->edges[3 + i].index1 = i;
        pPolyhedron->edges[3 + i].index2 = 3;

        /* PlaneEquation */
        BrVector3Sub(&tv1, &pPolyhedron->points[3], &pPolyhedron->points[i]);
        BrVector3Sub(&tv2, &pPolyhedron->points[o_i], &pPolyhedron->points[i]);
        BrVector3Cross(&pPolyhedron->planes[o], &tv1, &tv2);
        BrVector3Normalise(&pPolyhedron->planes[o], &pPolyhedron->planes[o]);
        pPolyhedron->planes[o].v[3] = -BrVector3Dot(&pPolyhedron->planes[o], &pPolyhedron->points[3]);

        pEdges[i].index2 = o;
        pEdges[3 + i].index1 = o;
        pEdges[3 + i].index2 = 1 + (o + 1) % 3;
    }

    for (i = 0; i < 3; i++) {

        if (PlaneValue(&pPolyhedron->planes[1 + i], &pPolyhedron->points[(2 + i) % 3]) < 1e-6f) {
            BrVector4Negate(&pPolyhedron->planes[1 + i], &pPolyhedron->planes[1 + i]);
        }
    }

    pPolyhedron->count_points = 4;
    pPolyhedron->count_planes = 4;
    pPolyhedron->count_edges = 6;
    return ePhysicsError_Ok;
}

static tPlane_side calculate_plane_side(br_vector4* pPlane, br_vector3* pPoint) {
    br_scalar s;

    s = PlaneValue(pPlane, pPoint);
    if (s < -1e-6f) {
        return ePlaneSide_negative;
    } else if (s >= 1e-6f) {
        return ePlaneSide_positive;
    } else {
        return ePlaneSide_zero;
    }
}

// FUNCTION: CARMA2_HW 0x00421fd0
tCollision_shape_polyhedron_data* C2_HOOK_FASTCALL FUN_00421fd0(int pCount_points) {
    tCollision_shape_polyhedron_data* result;
    int count_minus_two;
    int count_plus_two;
    int count_edges;

    count_minus_two = pCount_points - 2;
    count_plus_two = pCount_points + 2;
    count_edges = count_minus_two * 3;
#ifdef __vita__
    result = BrMemAllocate(sizeof(*result) + pCount_points * sizeof(br_vector3)
        + count_edges * sizeof(tPolyhedron_edge_indexes) + 2 * count_minus_two * sizeof(br_vector4) + 3, 0x80);
#else
    result = BrMemAllocate(2 * (count_edges + count_minus_two * 16 + count_plus_two * 6), 0x80);
#endif
    if (result == NULL) {
        return NULL;
    }
    result->points = (br_vector3*)((tU8*)result + sizeof(*result));
    result->count_points = 0;
    result->count_edges = 0;
    result->edges = (tPolyhedron_edge_indexes*)(result->points + pCount_points);
    result->count_planes = 0;
    result->planes = (br_vector4*)(result->edges + count_edges);
#ifdef __vita__
    result->planes = (br_vector4*)(((uintptr_t)result->planes + 3) & ~(uintptr_t)3);
#endif
    return result;
}

// C2PORT-NONMATCHING: best 14.4% | difftest PASS 2000 trials (spec 00421790_PolyhedronCollisionShape_AddPoint.json) | reason x87 arithmetic scheduling
// FUNCTION: CARMA2_HW 0x00421790
int C2_HOOK_FASTCALL PolyhedronCollisionShape_AddPoint(tCollision_shape_polyhedron_data* pPolyhedron, br_vector3* pVertex, tU8 pVertex_index, tPolyhedron_edge_indexes* pEdge_to_plane_indices) {
    tPlane_side plane_side_buffer[596];
    tPolyhedron_edge_indexes new_edge_to_plane_indices[894];
    tPolyhedron_edge_indexes new_edges[894];
    br_vector4 new_planes[596];
    int positive_side_edges[596];
    int all_positive;
    int i;
    int j;
    int count_new_planes;
    int count_new_edges;
    int final_count_edges;
    int final_count_planes;

    all_positive = 1;
    for (i = 0; i < pPolyhedron->count_planes; i++) {
        plane_side_buffer[i] = calculate_plane_side(&pPolyhedron->planes[i], pVertex);
        all_positive &= plane_side_buffer[i] != ePlaneSide_negative;
    }
    /* If all positive ==> point is inside polyhedron */
    if (all_positive) {
        return 0;
    }

    count_new_planes = 0;
    count_new_edges = 0;

    for (i = 0; i < pPolyhedron->count_edges; i++) {
        tU8 plane1;
        tPlane_side side1;
        tU8 plane2;
        tPlane_side side2;

        plane1 = pEdge_to_plane_indices[i].index1;
        side1 = plane_side_buffer[plane1];
        plane2 = pEdge_to_plane_indices[i].index2;
        side2 = plane_side_buffer[plane2];
        positive_side_edges[i] = side1 == ePlaneSide_positive || side2 == ePlaneSide_positive;
        if ((side1 ^ side2) & ePlaneSide_negative) {
            /* Exactly one plane of edge has vertex on negative side */
            if (!(side1 & ePlaneSide_zero) && !(side2 & ePlaneSide_zero)) {
                br_vector3* v1,* v2;
                br_vector3 tv1, tv2;
                br_vector4 *new_plane;
                /* vertex does not lie on edge */
                v1 = &pPolyhedron->points[pPolyhedron->edges[i].index1];
                v2 = &pPolyhedron->points[pPolyhedron->edges[i].index2];
                BrVector3Sub(&tv1, pVertex, v1);
                BrVector3Sub(&tv2, v2, v1);
                new_plane = &new_planes[count_new_planes];
                BrVector3Cross(new_plane, &tv1, &tv2);
                BrVector3Normalise(new_plane, new_plane);
                if (BrVector3Dot(&pPolyhedron->planes[plane1], &pPolyhedron->planes[plane1]) * BrVector3Dot(new_plane, &pPolyhedron->planes[plane2])
                        - BrVector3Dot(&pPolyhedron->planes[plane1], &pPolyhedron->planes[plane2]) * BrVector3Dot(new_plane, &pPolyhedron->planes[plane1]) < 0.f) {
                    BrVector4Negate(new_plane, new_plane);
                }
                new_plane->v[3] = -BrVector3Dot(new_plane, pVertex);
                if (side1 == ePlaneSide_positive) {
                    pEdge_to_plane_indices[i].index2 = count_new_planes + pPolyhedron->count_planes;
                } else {
                    pEdge_to_plane_indices[i].index1 = count_new_planes + pPolyhedron->count_planes;
                }

                for (j = 0; j < count_new_edges; j++) {
                    if (new_edges[j].index1 == pPolyhedron->edges[i].index1 && new_edges[j].index2 == pVertex_index) {
                        new_edge_to_plane_indices[j].index2 = count_new_planes + pPolyhedron->count_planes;
                        break;
                    }
                }
                if (j >= count_new_edges) {
                    new_edges[count_new_edges].index1 = pPolyhedron->edges[i].index1;
                    new_edges[count_new_edges].index2 = pVertex_index;
                    new_edge_to_plane_indices[count_new_edges].index1 = count_new_planes + pPolyhedron->count_planes;
                    count_new_edges += 1;
                }

                for (j = 0; j < count_new_edges; j++) {
                    if (new_edges[j].index1 == pPolyhedron->edges[i].index2 && new_edges[j].index2 == pVertex_index) {
                        new_edge_to_plane_indices[j].index2 = count_new_planes + pPolyhedron->count_planes;
                        count_new_planes += 1;
                        break;
                    }
                }
                if (j >= count_new_edges) {
                    new_edges[count_new_edges].index1 = pPolyhedron->edges[i].index2;
                    new_edges[count_new_edges].index2 = pVertex_index;
                    new_edge_to_plane_indices[count_new_edges].index1 = count_new_planes + pPolyhedron->count_planes;
                    count_new_edges += 1;
                    count_new_planes += 1;
                }
            } else {
                if (!(side1 & ePlaneSide_zero)) {
                    plane1 = plane2;
                }

                for (j = 0; j < count_new_edges; j++) {
                    if (new_edges[j].index1 == pPolyhedron->edges[i].index1 && new_edges[j].index2 == pVertex_index) {
                        new_edge_to_plane_indices[j].index2 = plane1;
                        break;
                    }
                }
                if (j >= count_new_edges) {
                    new_edges[count_new_edges].index1 = pPolyhedron->edges[i].index1;
                    new_edges[count_new_edges].index2 = pVertex_index;
                    new_edge_to_plane_indices[count_new_edges].index1 = plane1;
                    count_new_edges += 1;
                }

                for (j = 0; j < count_new_edges; j++) {
                    if (new_edges[j].index1 == pPolyhedron->edges[i].index2 && new_edges[j].index2 == pVertex_index) {
                        new_edge_to_plane_indices[j].index2 = plane1;
                        break;
                    }
                }
                if (j >= count_new_edges) {
                    new_edges[count_new_edges].index1 = pPolyhedron->edges[i].index2;
                    new_edges[count_new_edges].index2 = pVertex_index;
                    new_edge_to_plane_indices[count_new_edges].index1 = plane1;
                    count_new_edges += 1;
                }

            }
        }
    }

    final_count_edges = 0;
    for (i = 0; i < pPolyhedron->count_edges; i++) {
        if (positive_side_edges[i]) {
            if (i != final_count_edges) {
                pPolyhedron->edges[final_count_edges] = pPolyhedron->edges[i];
                pEdge_to_plane_indices[final_count_edges] = pEdge_to_plane_indices[i];
            }
            final_count_edges += 1;
        }
    }
    for (i = 0; i < count_new_edges; i++) {
        pPolyhedron->edges[final_count_edges] = new_edges[i];
        pEdge_to_plane_indices[final_count_edges] = new_edge_to_plane_indices[i];
        final_count_edges += 1;
    }
    pPolyhedron->count_edges = final_count_edges;

    final_count_planes = 0;
    for (i = 0; i < pPolyhedron->count_planes; i++) {
        if (!(plane_side_buffer[i] & ePlaneSide_negative)) {
            if (i != final_count_planes) {
                BrVector4Copy(&pPolyhedron->planes[final_count_planes], &pPolyhedron->planes[i]);
                for (j = 0; j < pPolyhedron->count_edges; j++) {
                    if (pEdge_to_plane_indices[j].index1 == i) {
                        pEdge_to_plane_indices[j].index1 = final_count_planes;
                    } else if (pEdge_to_plane_indices[j].index2 == i) {
                        pEdge_to_plane_indices[j].index2 = final_count_planes;
                    }
                }
            }
            final_count_planes += 1;
        }
    }
    for (i = 0; i < count_new_planes; i++) {
        BrVector4Copy(&pPolyhedron->planes[final_count_planes], &new_planes[i]);
        for (j = 0; j < pPolyhedron->count_edges; j++) {
            if (pEdge_to_plane_indices[j].index1 == pPolyhedron->count_planes + i) {
                pEdge_to_plane_indices[j].index1 = final_count_planes;
            } else if (pEdge_to_plane_indices[j].index2 == pPolyhedron->count_planes + i) {
                pEdge_to_plane_indices[j].index2 = final_count_planes;
            }
        }
        final_count_planes += 1;
    }
    pPolyhedron->count_planes = final_count_planes;
    return 1;
}

// C2PORT-NONMATCHING: best 25.40% | difftest PASS 2000 trials (spec 00420d60_ComparePolyhedronPlaneToNormal.json) | reason x87 register scheduling
// FUNCTION: CARMA2_HW 0x00420d60
br_scalar C2_HOOK_FASTCALL ComparePolyhedronPlaneToNormal(const br_vector3* pV1, const br_vector3* pV2) {
    br_vector3 tv;

    BrVector3Cross(&tv, pV1, pV2);
    return BrVector3Dot(&tv, &gPhysics_reference_normal_comparison);
}

// FUNCTION: CARMA2_HW 0x00420dc0
int C2_HOOK_CDECL ComparePolyhedronPointIndicesToNormal(const int* pIndex1, const int* pIndex2) {
    br_vector3 tv1, tv2;
    br_scalar s;
    br_scalar s1, s2;

    BrVector3Sub(&tv1, &gPolyhedron_to_sort->points[*pIndex1], &gPolyhedron_to_sort->points[0]);
    BrVector3Sub(&tv2, &gPolyhedron_to_sort->points[*pIndex2], &gPolyhedron_to_sort->points[0]);
    /* Calculate norm2 of normal on vectors v1/v2 with orthonormal */
    s = ComparePolyhedronPlaneToNormal(&tv1, &tv2);
    if (s > 1e-6f) {
        return -1;
    }
    if (s < -1e-6f) {
        return 1;
    }
    s1 = BrVector3LengthSquared(&tv1);
    s2 = BrVector3LengthSquared(&tv2);
    if (s1 < s2) {
        return -1;
    } else if (s2 > s1) {
        return 1;
    }
    return 0;
}

// C2PORT-NONMATCHING: best 49.43% | difftest PASS 2000 trials (spec difftest/specs/00420910_AddPolyhedronCollisionShapePlanes.json) | reason compiler x87 scheduling and large stack allocation
// FUNCTION: CARMA2_HW 0x00420910
tPhysicsError C2_HOOK_FASTCALL AddPolyhedronCollisionShapePlanes(tCollision_shape_polyhedron_data* pPolyhedron) {
    int first_vertex;
    br_vector3 tv1;
    br_vector3 tv2;
    br_vector3 tv3;
    int i;
    int indices_buffer[301];
    br_vector3 point_buffer[300];
    int index_min;
    int new_count_points;
    int read_idx;
    int write_idx;

    for (first_vertex = 0; ArePointsColinear(&pPolyhedron->points[first_vertex + 0], &pPolyhedron->points[first_vertex + 1], &pPolyhedron->points[first_vertex + 2]); first_vertex++) {
        if (first_vertex + 1 == pPolyhedron->count_points - 3) {
            return ePhysicsError_UnknownShapeType;
        }
    }
    BrVector3Sub(&tv1, &pPolyhedron->points[first_vertex + 1], &pPolyhedron->points[first_vertex + 0]);
    BrVector3Sub(&tv2, &pPolyhedron->points[first_vertex + 2], &pPolyhedron->points[first_vertex + 0]);
    BrVector3Cross(&tv3, &tv1, &tv2);
    BrVector3Normalise(&gPhysics_reference_normal_comparison, &tv3);

    index_min = 0;
    for (i = 1; i < pPolyhedron->count_points; i++) {

        if (pPolyhedron->points[i].v[0] < pPolyhedron->points[index_min].v[0]
                || (pPolyhedron->points[i].v[0] == pPolyhedron->points[index_min].v[0]
                        && (pPolyhedron->points[i].v[1] < pPolyhedron->points[index_min].v[1]
                            || (pPolyhedron->points[i].v[1] == pPolyhedron->points[index_min].v[1]
                               && pPolyhedron->points[i].v[2] < pPolyhedron->points[index_min].v[2])))) {
            index_min = i;
        }
    }

    BrVector3Copy(&tv1, &pPolyhedron->points[0]);
    BrVector3Copy(&pPolyhedron->points[0], &pPolyhedron->points[index_min]);
    BrVector3Copy(&pPolyhedron->points[index_min], &tv1);
    for (i = 0; i < pPolyhedron->count_points; i++) {
        indices_buffer[i + 1] = i;
    }
    gPolyhedron_to_sort = pPolyhedron;
    qsort(&indices_buffer[2], pPolyhedron->count_points - 1, sizeof(indices_buffer[1]), (int(C2_HOOK_CDECL*)(const void*, const void*))ComparePolyhedronPointIndicesToNormal);
    indices_buffer[0] = indices_buffer[pPolyhedron->count_points];
    indices_buffer[1] = 0;

    new_count_points = 1;
    read_idx = 2;
    write_idx = 1;
    for (i = 1; i < pPolyhedron->count_points; ) {
        BrVector3Sub(&tv1, &pPolyhedron->points[indices_buffer[write_idx]], &pPolyhedron->points[indices_buffer[write_idx - 1]]);
        BrVector3Sub(&tv2, &pPolyhedron->points[indices_buffer[read_idx]], &pPolyhedron->points[indices_buffer[write_idx]]);

        if (ComparePolyhedronPlaneToNormal(&tv1, &tv2) <= 1e-6f) {
            new_count_points -= 1;
            write_idx -= 1;
        } else {
            write_idx += 1;
            new_count_points += 1;
            indices_buffer[write_idx] = indices_buffer[read_idx];
            read_idx += 1;
            i++;
        }
    }
    pPolyhedron->count_points = new_count_points;
    pPolyhedron->count_edges = new_count_points;
    pPolyhedron->count_planes = 1;
    for (i = 0; i < new_count_points; i++) {
        BrVector3Copy(&point_buffer[i], &pPolyhedron->points[indices_buffer[i]]);
    }
    for (i = 0; i < new_count_points; i++) {
        BrVector3Copy(&pPolyhedron->points[i], &point_buffer[i]);
        pPolyhedron->edges[i].index1 = i + 0;
        pPolyhedron->edges[i].index2 = i + 1;
    }
    pPolyhedron->edges[0].index2 = 0;
    BrVector3Copy(&pPolyhedron->planes[0], &gPhysics_reference_normal_comparison);
    pPolyhedron->planes[0].v[3] = BrVector3Dot(&pPolyhedron->planes[0], &pPolyhedron->points[0]);
    return ePhysicsError_Ok;
}

// FUNCTION: CARMA2_HW 0x004c5eb0
void C2_HOOK_FASTCALL GetBoundsFromPointList(const br_vector3* pVertices, int pCount_vertices, br_bounds3* pBounds) {
    int i;

    BrVector3Copy(&pBounds->min, &pVertices[0]);
    BrVector3Copy(&pBounds->max, &pVertices[0]);
    for (i = 1; i < pCount_vertices; i++) {
        int j;

        for (j = 0; j < 3; j++) {
            if (pVertices[i].v[j] < pBounds->min.v[j]) {
                pBounds->min.v[j] = pVertices[i].v[j];
            } else if (pVertices[i].v[j] > pBounds->max.v[j]) {
                pBounds->max.v[j] = pVertices[i].v[j];
            }
        }
    }
}

// C2PORT-NONMATCHING: best 23.81% | difftest PASS 2000 trials (spec difftest/specs/00420fb0_ConvexHull3D.json) | reason vc5-stack-copy-loop-codegen
// FUNCTION: CARMA2_HW 0x00420fb0
tPhysicsError C2_HOOK_FASTCALL ConvexHull3D(tCollision_shape_polyhedron_data* pPolyhedron) {
    int original_count_points;
    br_vector3 original_points[300];
    tPolyhedron_edge_indexes indices[894];

    if (pPolyhedron->count_points > CARPOCALYPSE2_ASIZE(original_points)) {
        pPolyhedron->count_points = CARPOCALYPSE2_ASIZE(original_points);
    }

    original_count_points = pPolyhedron->count_points;
    memcpy(original_points, pPolyhedron->points, original_count_points * sizeof(original_points[0]));
    switch (ProcessTetrahedronPolyhedronCollisionShape(pPolyhedron, indices)) {
    case ePhysicsError_Ok: {
        int new_count_points;
        int i;
        int j;

        new_count_points = 4;
        for (i = 0; i < original_count_points; i++) {

            if (PolyhedronCollisionShape_AddPoint(pPolyhedron, &original_points[i], new_count_points, indices)) {
                if (i != new_count_points) {
                    BrVector3Copy(&pPolyhedron->points[new_count_points], &original_points[i]);
                }
                new_count_points++;
            }
        }
        pPolyhedron->count_points = new_count_points;

        new_count_points = 0;
        for (i = 0; i < pPolyhedron->count_points; i++) {

            for (j = 0; j < pPolyhedron->count_edges; j++) {
                if (i == pPolyhedron->edges[j].index1 || i == pPolyhedron->edges[j].index2) {
                    break;
                }
            }
            if (j >= pPolyhedron->count_edges) {
                continue;
            }
            if (i != new_count_points) {
                BrVector3Copy(&pPolyhedron->points[new_count_points], &pPolyhedron->points[i]);
                for (j = 0; j < pPolyhedron->count_edges; j++) {
                    if (pPolyhedron->edges[j].index1 == i) {
                        pPolyhedron->edges[j].index1 = new_count_points;
                    } else if (pPolyhedron->edges[j].index2 == i) {
                        pPolyhedron->edges[j].index2 = new_count_points;
                    }
                }
            }
            new_count_points++;
        }
        pPolyhedron->count_points = new_count_points;
        return ePhysicsError_Ok;
    }
    default: {
        tPhysicsError err;

        pPolyhedron->count_points = original_count_points;
        memcpy(pPolyhedron->points, original_points, original_count_points * sizeof(original_points[0]));
        err = AddPolyhedronCollisionShapePlanes(pPolyhedron);
        if (err == ePhysicsError_Ok) {
            return ePhysicsError_WireFrameHasNoPoints;
        }
        return err;
    }
    case ePhysicsError_UnknownShapeType:
        return ePhysicsError_UnknownShapeType;
    }
}

// C2PORT-NONMATCHING: best 79.65% | difftest PASS 2000 trials (spec difftest/specs/00422030_FUN_00422030.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x00422030
int C2_HOOK_FASTCALL FUN_00422030(br_model* pModel, int pCount_vertices, int* pVertex_indices) {
    float* distances;
    int removed[512];
    int count_vertices;
    int distance_offset;
    int remove_count;
    int i;

    count_vertices = pModel->nvertices;
    if (count_vertices > 512) {
        count_vertices = 512;
    }
    if (count_vertices <= pCount_vertices) {
        return 3;
    }
    distances = BrMemAllocate(count_vertices * count_vertices / 2 * sizeof(float), 0x80);
    if (distances == NULL) {
        return 4;
    }

    distance_offset = 0;
    i = 0;
    if (count_vertices != 0) {
        int vertex_offset;

        vertex_offset = 0;
        do {
            if (vertex_offset > 0) {
                float* row;
                int j;
                int inner_offset;

                row = distances + distance_offset;
                distance_offset += i;
                j = i;
                inner_offset = 0;
                do {
                    row[0] =
                        (*(float*)((char*)pModel->vertices + vertex_offset + 8) - *(float*)((char*)pModel->vertices + inner_offset + 8)) * (*(float*)((char*)pModel->vertices + vertex_offset + 8) - *(float*)((char*)pModel->vertices + inner_offset + 8))
                        + (*(float*)((char*)pModel->vertices + vertex_offset + 4) - *(float*)((char*)pModel->vertices + inner_offset + 4)) * (*(float*)((char*)pModel->vertices + vertex_offset + 4) - *(float*)((char*)pModel->vertices + inner_offset + 4))
                        + (*(float*)((char*)pModel->vertices + vertex_offset) - *(float*)((char*)pModel->vertices + inner_offset)) * (*(float*)((char*)pModel->vertices + vertex_offset) - *(float*)((char*)pModel->vertices + inner_offset));
                    row++;
                    inner_offset += 0x28;
                    j--;
                } while (j != 0);
            }
            i++;
            vertex_offset += 0x28;
        } while (i < count_vertices);
    }

    memset(removed, 0, count_vertices * sizeof(removed[0]));
    remove_count = count_vertices - pCount_vertices;
    do {
        int index1;
        int index2;
        float min_distance;
        float sum1;
        float sum2;

        min_distance = 3.4028235e+38f;
        for (i = 0; i < count_vertices; i++) {
            if (removed[i] == 0) {
                int j;

                for (j = 0; j < i; j++) {
                    if (removed[j] == 0) {
                        int distance_index;
                        float distance;

                        if (j < i) {
                            distance_index = ((i - 1) * i >> 1) + j;
                        } else {
                            distance_index = ((j - 1) * j >> 1) + i;
                        }
                        distance = distances[distance_index];
                        if (distance < min_distance) {
                            index1 = i;
                            index2 = j;
                            min_distance = distance;
                        }
                    }
                }
            }
        }

        sum1 = gFLOAT_00589954;
        sum2 = gFLOAT_00589954;
        for (i = 0; i < count_vertices; i++) {
            if (removed[i] == 0 && i != index1 && i != index2) {
                int distance_index;

                if (i < index1) {
                    distance_index = ((index1 - 1) * index1 >> 1) + i;
                } else {
                    distance_index = ((i - 1) * i >> 1) + index1;
                }
                sum1 = sqrtf(distances[distance_index]) + sum1;

                if (i < index2) {
                    distance_index = ((index2 - 1) * index2 >> 1) + i;
                } else {
                    distance_index = ((i - 1) * i >> 1) + index2;
                }
                sum2 = sqrtf(distances[distance_index]) + sum2;
            }
        }
        if (sum1 >= sum2) {
            removed[index2] = 1;
        } else {
            removed[index1] = 1;
        }
        remove_count--;
    } while (remove_count != 0);

    for (i = 0; i < count_vertices; i++) {
        if (removed[i] == 0) {
            *pVertex_indices = i;
            pVertex_indices++;
        }
    }
    BrMemFree(distances);
    return 0;
}

// C2PORT-NONMATCHING: best 82.35% | difftest PASS 2000 trials (spec difftest/specs/004222c0_FUN_004222c0.json) | reason allocation-init-regalloc
// FUNCTION: CARMA2_HW 0x004222c0
int C2_HOOK_FASTCALL FUN_004222c0(int* pOut_point_count, int* pVertex_indices, br_model* pModel, int pMin_point_count, int pMax_point_count) {
    tCollision_shape_polyhedron_data* allocation_result;
    int result;
    int point_count;
    int count_points;
    int edge_capacity;
    tCollision_shape_polyhedron_data* polyhedron;
    int count_minus_two;
    int count_plus_two;
    int plane_capacity;

    polyhedron = NULL;
    if (pModel == NULL) {
        return 5;
    }
    if (pMax_point_count < 3) {
        return 6;
    }
    if (pMax_point_count > (int)pModel->nvertices) {
        pMax_point_count = pModel->nvertices;
    }
    if (pMin_point_count < 3) {
        pMin_point_count = 3;
    } else if (pMin_point_count > pMax_point_count) {
        pMin_point_count = pMax_point_count;
    }

#ifdef __vita__
    /* The retail compiler inlined the same packed allocation here. */
    allocation_result = FUN_00421fd0(pMax_point_count);
    polyhedron = allocation_result;
#else
    count_minus_two = pMax_point_count - 2;
    count_plus_two = pMax_point_count + 2;
    edge_capacity = count_minus_two * 3;
    plane_capacity = count_plus_two * 3;
    allocation_result = BrMemAllocate(
            2 * (count_minus_two * 16 + edge_capacity + plane_capacity * 2),
            0x80);
    if (allocation_result != NULL) {
        polyhedron = allocation_result;
        polyhedron->points = (br_vector3*)(polyhedron + 1);
        polyhedron->count_points = 0;
        polyhedron->edges = (tPolyhedron_edge_indexes*)(polyhedron->points + pMax_point_count);
        polyhedron->count_edges = 0;
        polyhedron->count_planes = 0;
        polyhedron->planes = (br_vector4*)(polyhedron->edges + edge_capacity);
    }
#endif
    if (polyhedron != NULL) {
        *pOut_point_count = pMax_point_count;
        point_count = pMax_point_count;
        result = FUN_00422400(polyhedron, pVertex_indices, pModel, point_count);
        count_points = polyhedron->count_points;
        if (count_points != 0) {
            while (1) {
                if (result != 0 && result != 2) {
                    return result;
                }
                point_count = point_count * pMin_point_count / count_points;
                if (count_points == pMin_point_count || point_count >= pMax_point_count) {
                    break;
                }
                *pOut_point_count = point_count;
                result = FUN_00422400(polyhedron, pVertex_indices, pModel, point_count);
                count_points = polyhedron->count_points;
                if (count_points == 0) {
                    return result;
                }
            }
            BrMemFree(polyhedron);
        }
        return result;
    }
    return 4;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00422400
tPhysicsError C2_HOOK_FASTCALL FUN_00422400(tCollision_shape_polyhedron_data* pPolyhedron, int* pVertex_indices, br_model* pModel, int pCount_vertices) {
    int i;
    int result;

    if (pCount_vertices < pModel->nvertices) {
        result = FUN_00422030(pModel, pCount_vertices, pVertex_indices);
        if (result != ePhysicsError_Ok) {
            return result;
        }
        pPolyhedron->count_points = pCount_vertices;
        if (pCount_vertices > 0) {
            for (i = 0; i < pCount_vertices; i++) {
                pPolyhedron->points[i].v[0] = pModel->vertices[pVertex_indices[i]].p.v[0];
                pPolyhedron->points[i].v[1] = pModel->vertices[pVertex_indices[i]].p.v[1];
                pPolyhedron->points[i].v[2] = pModel->vertices[pVertex_indices[i]].p.v[2];
            }
            return ConvexHull3D(pPolyhedron);
        }
    } else {
        pPolyhedron->count_points = pModel->nvertices;
        for (i = 0; i < pModel->nvertices; i++) {
            pVertex_indices[i] = i;
            pPolyhedron->points[i].v[0] = pModel->vertices[i].p.v[0];
            pPolyhedron->points[i].v[1] = pModel->vertices[i].p.v[1];
            pPolyhedron->points[i].v[2] = pModel->vertices[i].p.v[2];
        }
    }
    return ConvexHull3D(pPolyhedron);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00422500
int C2_HOOK_FASTCALL FUN_00422500(tCollision_shape_polyhedron_data* pPolyhedron, br_model* pModel, int pMin_point_count, int pMax_point_count, int* pVertex_indices) {
    int result;
    int count_points;
    int point_count;

    if (pModel == NULL) {
        return 5;
    }
    if (pMax_point_count < 3) {
        return 6;
    }
    if (pMax_point_count > (int)pModel->nvertices) {
        pMax_point_count = pModel->nvertices;
    }
    if (pMin_point_count < 3) {
        pMin_point_count = 3;
    } else if (pMin_point_count > pMax_point_count) {
        pMin_point_count = pMax_point_count;
    }
    point_count = pMax_point_count;
    result = FUN_00422400(pPolyhedron, pVertex_indices, pModel, point_count);
    count_points = pPolyhedron->count_points;
    while (count_points != 0
            && ((result == 0 || result == 2)
                && (point_count = point_count * pMin_point_count / count_points, count_points != pMin_point_count))
            && point_count < pMax_point_count) {
        result = FUN_00422400(pPolyhedron, pVertex_indices, pModel, point_count);
        count_points = pPolyhedron->count_points;
    }
    return result;
}

// C2PORT-NONMATCHING: best 63.31% | difftest PASS 2000 trials (spec difftest/specs/004c5f20_FillInShape.json) | reason VC5 switch and inline error-path layout
// FUNCTION: CARMA2_HW 0x004c5f20
void C2_HOOK_FASTCALL FillInShape(tPhysics_shape* pShape) {
    int i;
    tPhysicsError error;

    switch (pShape->common.type) {
    case kCollisionShapeType_Box:
        break;
    case kCollisionShapeType_Wireframe:
        if (pShape->wireframe.wireframe.count_points == 0) {
            if (gPhysics_error_cb != NULL) {
                gPhysics_error_cb(2, NULL);
            }
            _exit(2);
        }
        GetBoundsFromPointList(pShape->wireframe.wireframe.points, pShape->wireframe.wireframe.count_points, &pShape->common.bb);
        break;
    case kCollisionShapeType_Polyhedron:
        if (pShape->polyhedron.polyhedron.count_points < 4) {
            if (gPhysics_error_cb != NULL) {
                gPhysics_error_cb(3, NULL);
            }
            _exit(3);
        }
        error = ConvexHull3D(&pShape->polyhedron.polyhedron);
        if (error != ePhysicsError_Ok) {
            if (error != ePhysicsError_WireFrameHasNoPoints) {
                PDFatalError("This polyhedron is too wierd for me");
            }
            pShape->common.type = kCollisionShapeType_Wireframe_Polyhedron;
            GetBoundsFromPointList(pShape->polyhedron.polyhedron.points, pShape->polyhedron.polyhedron.count_points, &pShape->common.bb);
            return;
        }
        GetBoundsFromPointList(pShape->polyhedron.polyhedron.points, pShape->polyhedron.polyhedron.count_points, &pShape->common.bb);
        break;
    case kCollisionShapeType_Wireframe_Polyhedron:
        if (pShape->polyhedron.polyhedron.count_points < 3) {
            if (gPhysics_error_cb != NULL) {
                gPhysics_error_cb(4, NULL);
            }
            _exit(4);
        }
        AddPolyhedronCollisionShapePlanes(&pShape->polyhedron.polyhedron);
        GetBoundsFromPointList(pShape->polyhedron.polyhedron.points, pShape->polyhedron.polyhedron.count_points, &pShape->common.bb);
        break;
    case kCollisionShapeType_Sphere:
        BrVector3Copy(&pShape->common.bb.min, &pShape->sphere.sphere.center);
        BrVector3Copy(&pShape->common.bb.max, &pShape->sphere.sphere.center);
        for (i = 0; i < 3; i++) {
            pShape->common.bb.min.v[i] -= pShape->sphere.sphere.radius;
            pShape->common.bb.max.v[i] += pShape->sphere.sphere.radius;
        }
        pShape->sphere.sphere.radius_squared = CARPOCALYPSE2_SQR(pShape->sphere.sphere.radius);
        break;
    default:
        if (gPhysics_error_cb != NULL) {
            gPhysics_error_cb(1, NULL);
        }
        _exit(1);
        break;
    }
}
// FUNCTION: CARMA2_HW 0x004da400
tPhysics_object* C2_HOOK_FAKE_THISCALL MungeSphereObject(br_model* pModel, undefined4 pArg2, float pWeight) {
    tPhysics_object* collision_info;

    CARPOCALYPSE2_THISCALL_UNUSED(pArg2);

    C2_HOOK_BUG_ON(sizeof(tPhysics_object) != 1240);

    collision_info = BrMemAllocate(sizeof(tPhysics_object), kMem_collision_object);
    collision_info->shape = (tPhysics_shape*)AllocateShapeSphere(kMem_collision_shape);
    BrVector3Set(&collision_info->shape->sphere.sphere.center, 0.f, 0.f, 0.f);
    collision_info->shape->sphere.sphere.radius = (pModel->bounds.max.v[0] - pModel->bounds.min.v[0] + pModel->bounds.max.v[1] - pModel->bounds.min.v[1] + pModel->bounds.max.v[2] - pModel->bounds.min.v[2]) / 6.f;
    FillInShape(collision_info->shape);
    collision_info->uid = gCollision_info_uid_counter;
    gCollision_info_uid_counter++;
    UpdateCollisionObject(collision_info);
    collision_info->M = pWeight;
    BrVector3Set(&collision_info->I,
        CARPOCALYPSE2_SQR(collision_info->shape->sphere.sphere.radius) * pWeight / 6.f,
        CARPOCALYPSE2_SQR(collision_info->shape->sphere.sphere.radius) * pWeight / 6.f,
        CARPOCALYPSE2_SQR(collision_info->shape->sphere.sphere.radius) * pWeight / 6.f);
    BrVector3SetFloat(&collision_info->field_0x54, 0.f, 1/-17.25f, 0.f);
    collision_info->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    BrMatrix34Copy(&collision_info->actor->t.t.mat, &collision_info->transform_matrix);
    collision_info->actor->model = pModel;
    collision_info->box_face_ref = gFace_num__car - 2;
    return collision_info;
}

// FUNCTION: CARMA2_HW 0x004b8e10
void C2_HOOK_FASTCALL FUN_004b8e10(tPhysics_object* pObject1, tPhysics_object* pObject2, tPhysics_joint_data* pJoint_data, const br_vector3* pAxis) {
    br_vector3 axis_world;

    C2_HOOK_BUG_ON(sizeof(tPhysics_joint_data) != 0x60);
    C2_HOOK_BUG_ON(sizeof(tCollision_contact) != 0x60);

    pJoint_data->field_0x00 = 0;
    pJoint_data->object1 = pObject1;
    pJoint_data->object2 = pObject2;
    pJoint_data->anchor1.v[0] = pObject1->physics_joint1->field_0x08.v[0] - pObject1->cmpos.v[0];
    pJoint_data->anchor1.v[1] = pObject1->physics_joint1->field_0x08.v[1] - pObject1->cmpos.v[1];
    pJoint_data->anchor1.v[2] = pObject1->physics_joint1->field_0x08.v[2] - pObject1->cmpos.v[2];
    pJoint_data->anchor2.v[0] = pObject1->physics_joint1->field_0x14.v[0] - pObject2->cmpos.v[0];
    pJoint_data->anchor2.v[1] = pObject1->physics_joint1->field_0x14.v[1] - pObject2->cmpos.v[1];
    pJoint_data->anchor2.v[2] = pObject1->physics_joint1->field_0x14.v[2] - pObject2->cmpos.v[2];
    pJoint_data->axis1.v[0] = pAxis->v[0];
    pJoint_data->axis1.v[1] = pAxis->v[1];
    pJoint_data->axis1.v[2] = pAxis->v[2];
    BrMatrix34ApplyV(&axis_world, &pJoint_data->axis1, &pObject1->transform_matrix);
    BrMatrix34TApplyV(&pJoint_data->axis2, &axis_world, &pObject2->transform_matrix);
    pJoint_data->field_0x5c = 0xb;
}

// FUNCTION: CARMA2_HW 0x004c2a10
void C2_HOOK_FASTCALL TrialPositionChildren(tPhysics_object* pObject) {
    tPhysics_object* child;
    tPhysics_object* matrix_parent;
    int flags;
    br_vector3 parent_joint_pos;
    br_vector3 child_joint_pos;

    matrix_parent = pObject;
    flags = pObject->flags;
    child = pObject->child;
    if (flags & 0x400) {
        matrix_parent = pObject->field_0x230;
    }
    while (child != NULL) {
        if (!(pObject->flags & 0x400)) {
            BrMatrix34ApplyP(&parent_joint_pos, &child->physics_joint1->field_0x14, &pObject->actor->t.t.mat);
            BrMatrix34ApplyP(&child_joint_pos, &child->physics_joint1->field_0x08, &child->actor->t.t.mat);
            BrVector3Sub(&parent_joint_pos, &parent_joint_pos, &child_joint_pos);
            BrVector3Accumulate((br_vector3*)&child->actor->t.t.mat.m[3], &parent_joint_pos);
        } else {
            BrMatrix34Mul(&pObject->actor->t.t.mat, &pObject->field_0x1e8, &matrix_parent->actor->t.t.mat);
        }
        TrialPositionChildren(child);
        child = child->next;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c2b10
void C2_HOOK_FASTCALL PositionChildren(tPhysics_object *pCollision_info) {

    InternalPositionChildren(pCollision_info, pCollision_info);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c2b20
void C2_HOOK_FASTCALL InternalPositionChildren(tPhysics_object *pParent, tPhysics_object *pRoot) {
    tPhysics_object *child;

    if (!(pParent->flags & 0x400)) {
        pRoot = pParent;
    }
    for (child = pParent->child; child != NULL; child = child->next) {
        if (pParent->flags & 0x400) {
            BrMatrix34Mul(&pParent->transform_matrix, &pParent->field_0x1e8, &pRoot->transform_matrix);
        } else {
            br_vector3 tv1;
            br_vector3 tv2;
            BrMatrix34ApplyP(&tv1, &child->physics_joint1->field_0x14, &pParent->transform_matrix);
            BrMatrix34ApplyP(&tv2, &child->physics_joint1->field_0x08, &child->transform_matrix);
            BrVector3Sub(&tv1, &tv1, &tv2);
            BrVector3Accumulate((br_vector3*)&child->transform_matrix.m[3], &tv1);
        }
        InternalPositionChildren(child, pRoot);
    }
}

// FUNCTION: CARMA2_HW 0x004b63b0
int C2_HOOK_CDECL PHILSetObjectProperty(tPhysics_object *pCollision_info, int pParam, ...) {
    va_list va;
    tCollision_info_owner* owner;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCollision_info_owner, field_0x04, 0x04);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCollision_info_owner, field_0x08, 0x08);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCollision_info_owner, field_0x0c, 0x0c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCollision_info_owner, field_0x10, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tCollision_info_owner, field_0x14, 0x14);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x240, 0x240);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, box_face_ref, 0x178);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, flags, 0x19c);

    if (gPHIL_enabled) {
        return 0;
    }
    if (pCollision_info->field_0x239 != 2) {
    }
    owner = (tCollision_info_owner*)pCollision_info->field_0x240;
    if (owner == NULL) {
        return 3;
    }

    va_start(va, pParam);
    switch (pParam) {
        case 0:
            owner->field_0x10 = (float)va_arg(va, double);
            break;
        case 1: {
            int v = !!va_arg(va, int);
            if (v) {
                owner->field_0x04 |= 0x1;
                PositionChildren(pCollision_info);
            } else {
                owner->field_0x04 &= 0x1;
            }
            break;
        }
        case 2:
            owner->field_0x0c = (float)va_arg(va, double);
            break;
        case 3: {
            int v = va_arg(va, int) != 0;
            pCollision_info->flags = (pCollision_info->flags & ~(1 << 6)) | (v << 6);
            if (!v) {
                pCollision_info->box_face_ref = gFace_num__car - 2;
            }
            break;
        }
        case 4: {
            int v = va_arg(va, int) != 0;
            pCollision_info->flags = (pCollision_info->flags & ~(1 << 3)) | (v << 3);
            break;
        }
        case 5: {
            int v = va_arg(va, int) != 0;
            pCollision_info->flags = (pCollision_info->flags & ~(1 << 4)) | (v << 4);
            break;
        }
        case 6: {
            int v = va_arg(va, int) != 0;
            if (v) {
                owner->field_0x04 |= 0x4;
            } else {
                owner->field_0x04 &= 0x4;
            }
            break;
        }
        case 7: {
            int v = va_arg(va, int) != 0;
            if (v) {
                owner->field_0x04 |= 0x8;
                pCollision_info->water_d = 10000.f;
            } else {
                owner->field_0x04 &= 0x8;
            }
            break;
        }
        case 8:
            owner->field_0x14 = (float)va_arg(va, double);
            break;
        default:
            va_end(va);
            return 5;
    }
    va_end(va);
    return 0;
}

// FUNCTION: CARMA2_HW 0x004b6530
float C2_HOOK_FASTCALL PHILGetObjectProperty(tPhysics_object *pCollision_info, int pParam) {
    tCollision_info_owner* owner;

    owner = (tCollision_info_owner*)pCollision_info->field_0x240;
    if (gPHIL_enabled || owner == NULL) {
        return 0.f;
    }
    switch (pParam) {
    case 0:
        return owner->field_0x10;
    case 1:
        return (float)!!(owner->field_0x04 & 0x1);
    case 2:
        return owner->field_0x0c;
    case 3:
        return (float)!!(pCollision_info->flags & 0x40);
    case 4:
        return (float)!!(pCollision_info->flags & 0x8);
    case 5:
        return (float)!!(pCollision_info->flags & 0x10);
    case 6:
        return (float)!!(owner->field_0x04 & 0x4);
    case 7:
        return (float)!!(owner->field_0x04 & 0x8);
    case 8:
        return owner->field_0x14;
    default:
        return 5.f;
    }
}

// FUNCTION: CARMA2_HW 0x004b9ef0
void C2_HOOK_FASTCALL SetCollisionInfoChildsDoNothing(tPhysics_object *pCollision_info, tU32 pDisable) {
    tPhysics_object *sibling;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, field_0x1df, 0x1df);

    pCollision_info->disable_move_rotate = pDisable;
    pCollision_info->field_0x1df = 0;
    if (pCollision_info->child != NULL) {
        sibling = pCollision_info->child;
        while (sibling != NULL) {
            sibling->disable_move_rotate = pDisable;
            sibling->field_0x1df = 0;
            if (sibling->child != NULL) {
                SetCollisionInfoDoNothing(sibling->child, pDisable);
            }
            sibling = sibling->next;
        }
    }
}

// SetCollisionInfoDoNothing

// FUNCTION: CARMA2_HW 0x004c63d0
void C2_HOOK_FASTCALL AddCollisionInfoChild(tPhysics_object* pParent, tPhysics_object* pChild) {
    tPhysics_object* current;
    tPhysics_object* last;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, parent, 0x228);

    pChild->parent = pParent;
    last = pChild;
    for (current = pChild->next; current != NULL; current = current->next) {
        current->parent = pParent;
        last = current;
    }
    last->next = pParent->child;
    pParent->child = pChild;
}

// FUNCTION: CARMA2_HW 0x004c6470
int C2_HOOK_FASTCALL PhysicsObjectRecurse(tPhysics_object* pCollision_info, tEnumCollision_cbfn* pCallback, void* pUser_data) {
    int r;
    tPhysics_object* child;

    r = pCallback(pCollision_info, pUser_data);
    if (r != 0) {
        return r;
    }
    for (child = pCollision_info->child; child != NULL; child = child->next) {
        r = PhysicsObjectRecurse(child,pCallback, pUser_data);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00428dc0
int C2_HOOK_FASTCALL TestObjectOverlapRecurse(tPhysics_object* pCollision_1, tPhysics_object* pCollision_2, tPhysics_shape* pShape_1, tPhysics_shape* pShape_2) {
    tShape_overlap_work local;
    volatile br_vector3* corner_input;
    br_matrix34* pTransform2;
    const tPolyhedron_edge_indexes* edges;
    int edge_count;
    int i;

    corner_input = &local.vertices[0];
    switch (pShape_1->common.type) {
    case kCollisionShapeType_Polyhedron:
    case kCollisionShapeType_Wireframe_Polyhedron:
        pTransform2 = &pCollision_2->transform_matrix;
        i = 0;
        if (pShape_1->polyhedron.polyhedron.count_points > 0) {
            do {
                BrMatrix34ApplyP((br_vector3*)local.transformed, &pShape_1->polyhedron.polyhedron.points[i], &pCollision_1->transform_matrix);
                local.relative[0] = local.transformed[0] - pTransform2->m[3][0];
                local.relative[1] = local.transformed[1] - pTransform2->m[3][1];
                local.relative[2] = local.transformed[2] - pTransform2->m[3][2];
                BrMatrix34TApplyV(&local.vertices[i], (br_vector3*)local.relative, pTransform2);
                i++;
            } while (i < pShape_1->polyhedron.polyhedron.count_points);
        }
        edges = pShape_1->polyhedron.polyhedron.edges;
        edge_count = pShape_1->polyhedron.polyhedron.count_edges;
        break;
    case kCollisionShapeType_Box:
    case kCollisionShapeType_Wireframe:
    case kCollisionShapeType_Sphere:
        pTransform2 = &pCollision_2->transform_matrix;
        i = 0;
        do {
            corner_input = &local.vertices[i];
            corner_input->v[0] = (i & 2) ? pShape_1->common.bb.min.v[0] : pShape_1->common.bb.max.v[0];
            corner_input->v[1] = (i & 1) ? pShape_1->common.bb.min.v[1] : pShape_1->common.bb.max.v[1];
            corner_input->v[2] = (i & 4) ? pShape_1->common.bb.min.v[2] : pShape_1->common.bb.max.v[2];
            BrMatrix34ApplyP((br_vector3*)local.transformed, (const br_vector3*)corner_input, &pCollision_1->transform_matrix);
            local.relative[0] = local.transformed[0] - pTransform2->m[3][0];
            local.relative[1] = local.transformed[1] - pTransform2->m[3][1];
            local.relative[2] = local.transformed[2] - pTransform2->m[3][2];
            BrMatrix34TApplyV(&local.vertices[i], (br_vector3*)local.relative, pTransform2);
            i++;
        } while (i < 8);
        edges = gBox_edges_0058f7f0;
        edge_count = 12;
        break;
    default:
        return 0;
    }

    switch (pShape_2->common.type) {
    case kCollisionShapeType_Polyhedron:
    case kCollisionShapeType_Wireframe_Polyhedron:
        for (i = 0; i < edge_count; i++) {
            if (FUN_00461310(&local.vertices[edges[i].index1], &local.vertices[edges[i].index2],
                    &pShape_2->polyhedron.polyhedron, &local.hit_position, 0.005f, &local.factor)) {
                return 1;
            }
        }
        return 0;
    case kCollisionShapeType_Box:
    case kCollisionShapeType_Wireframe:
    case kCollisionShapeType_Sphere:
        local.expanded_bounds.min.v[0] = pShape_2->common.bb.min.v[0] - gFLOAT_005899a0;
        local.expanded_bounds.min.v[1] = pShape_2->common.bb.min.v[1] - gFLOAT_005899a0;
        local.expanded_bounds.min.v[2] = pShape_2->common.bb.min.v[2] - gFLOAT_005899a0;
        local.expanded_bounds.max.v[0] = pShape_2->common.bb.max.v[0] - gFLOAT_005899a4;
        local.expanded_bounds.max.v[1] = pShape_2->common.bb.max.v[1] - gFLOAT_005899a4;
        local.expanded_bounds.max.v[2] = pShape_2->common.bb.max.v[2] - gFLOAT_005899a4;
        for (i = 0; i < edge_count; i++) {
            if (FUN_00460bd0(&local.vertices[edges[i].index1], &local.vertices[edges[i].index2], &local.expanded_bounds, &local.hit_position, &local.factor)) {
                return 1;
            }
        }
        return 0;
    default:
        return 0;
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 49.71% | difftest PASS 2000 trials (spec difftest/specs/00429070_TestForObjectInSensiblePlace.json) | reason VC5 stack, register, and x87 scheduling
// FUNCTION: CARMA2_HW 0x00429070
int C2_HOOK_FASTCALL TestForObjectInSensiblePlace(tPhysics_object* pCollision_info_1, tPhysics_object* pCollision_info_2, br_vector3* pVec3, tWorld_callbacks* pCar_callbacks) {
    struct {
        tU8 field_0x00[0x10];
        br_vector3 normal;
        tU8 field_0x1c[0xc];
        tU8 contact_data[0x2f0];
    } local;
    tPhysics_object* candidate;
    tPhysics_object* child;
    tPhysics_shape* shape1;
    tPhysics_shape* shape2;
    float length;
    float scale;
    int contact_count;

    gSensible_place_blocker = NULL;
    GetNewBoundingBox((br_bounds*)&pCollision_info_1->field_0xf4, (br_bounds*)&pCollision_info_1->bb1, &pCollision_info_1->transform_matrix);
    GetFacesInBox(pCollision_info_1, pCar_callbacks);

    gSensible_place_collect_active = 1;
    contact_count = FUN_004228b0(pCollision_info_1, local.contact_data, 0);
    gSensible_place_collect_active = 0;
    if (contact_count != 0) {
        local.normal.v[0] = *(float*)&local.contact_data[0x2c];
        local.normal.v[1] = *(float*)&local.contact_data[0x30];
        local.normal.v[2] = *(float*)&local.contact_data[0x34];
        if (contact_count > 1) {
            int i;

            for (i = 1; i < contact_count; i++) {
                local.normal.v[0] += *(float*)&local.contact_data[0x8c + (i - 1) * 0x60];
                local.normal.v[1] += *(float*)&local.contact_data[0x90 + (i - 1) * 0x60];
                local.normal.v[2] += *(float*)&local.contact_data[0x94 + (i - 1) * 0x60];
            }
        }

        length = sqrtf(local.normal.v[0] * local.normal.v[0] + local.normal.v[1] * local.normal.v[1] + local.normal.v[2] * local.normal.v[2]);
        if (length <= gFLOAT_005899a8) {
            local.normal.v[0] = 1.f;
            local.normal.v[1] = 0.f;
            local.normal.v[2] = 0.f;
        } else {
            scale = (float)(gDouble_005899b0 / length);
            local.normal.v[0] *= scale;
            local.normal.v[1] *= scale;
            local.normal.v[2] *= scale;
        }
        BrMatrix34ApplyV(pVec3, &local.normal, &pCollision_info_1->transform_matrix);
        return 0;
    }

    candidate = pCollision_info_2;
    for (;;) {
        if (candidate == NULL) {
            return -1;
        }

        if (candidate != pCollision_info_1
                && pCollision_info_1->parent != candidate
                && candidate->parent != pCollision_info_1) {
            shape1 = pCollision_info_1->shape;
            shape2 = candidate->shape;
            if (CanPhysicsObjectsCollide(pCollision_info_1, candidate)
                    && BoundsOverlapTest__finteray((br_bounds*)&pCollision_info_1->field_0xf4, (br_bounds*)&candidate->field_0xf4)) {
                for (; shape1 != NULL; shape1 = shape1->common.next) {
                    for (; shape2 != NULL; shape2 = shape2->common.next) {
                        if (TestObjectOverlapRecurse(pCollision_info_1, candidate, shape1, shape2)
                                || TestObjectOverlapRecurse(candidate, pCollision_info_1, shape2, shape1)) {
                            goto collision_found;
                        }
                    }
                }
            }
        }
        for (child = candidate->child; child != NULL; child = child->next) {
            if (FUN_004293c0(pCollision_info_1, child)) {
                goto collision_found;
            }
        }
        candidate = candidate->next;
    }

collision_found:
    pVec3->v[0] = pCollision_info_1->field_0x10c.min.v[0] + pCollision_info_1->field_0x10c.max.v[0];
    pVec3->v[1] = pCollision_info_1->field_0x10c.min.v[1] + pCollision_info_1->field_0x10c.max.v[1];
    pVec3->v[2] = pCollision_info_1->field_0x10c.min.v[2] + pCollision_info_1->field_0x10c.max.v[2];
    pVec3->v[0] -= candidate->field_0x10c.min.v[0];
    pVec3->v[1] -= candidate->field_0x10c.min.v[1];
    pVec3->v[2] -= candidate->field_0x10c.min.v[2];
    pVec3->v[0] -= candidate->field_0x10c.max.v[0];
    pVec3->v[1] -= candidate->field_0x10c.max.v[1];
    pVec3->v[2] -= candidate->field_0x10c.max.v[2];

    length = sqrtf(pVec3->v[0] * pVec3->v[0] + pVec3->v[2] * pVec3->v[2] + pVec3->v[1] * pVec3->v[1]);
    if (length <= gFLOAT_005899a8) {
        pVec3->v[0] = 1.f;
        pVec3->v[1] = 0.f;
        pVec3->v[2] = 0.f;
    } else {
        scale = (float)(gDouble_005899b0 / length);
        pVec3->v[0] *= scale;
        pVec3->v[1] *= scale;
        pVec3->v[2] *= scale;
    }
    gSensible_place_blocker = candidate;
    return 0;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 66.67% | difftest PASS 2000 trials (spec difftest/specs/004293c0_FUN_004293c0.json) | reason VC5 register allocation and result-branch layout
// FUNCTION: CARMA2_HW 0x004293c0
int C2_HOOK_FASTCALL FUN_004293c0(tPhysics_object* pObject1, tPhysics_object* pObject2) {
    int collision;
    tPhysics_shape* shape1;
    tPhysics_shape* shape2;
    tPhysics_object* child;

    if (pObject2 != pObject1 && pObject1->parent != pObject2 && pObject2->parent != pObject1) {
        shape1 = pObject1->shape;
        shape2 = pObject2->shape;
        if (CanPhysicsObjectsCollide(pObject1, pObject2)
                && BoundsOverlapTest__finteray((br_bounds*)&pObject1->field_0xf4, (br_bounds*)&pObject2->field_0xf4)) {
            for (; shape1 != NULL; shape1 = shape1->common.next) {
                for (; shape2 != NULL; shape2 = shape2->common.next) {
                    if (TestObjectOverlapRecurse(pObject1, pObject2, shape1, shape2)
                            || TestObjectOverlapRecurse(pObject2, pObject1, shape2, shape1)) {
                        collision = 1;
                        goto collision_test_done;
                    }
                }
            }
        }
    }
    collision = 0;
collision_test_done:
    if (collision != 0) {
        return 1;
    } else {
        for (child = pObject2->child; child != NULL; child = child->next) {
            if (FUN_004293c0(pObject1, child)) {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 73.22% | difftest PASS 3000 trials (spec difftest/specs/00429490_FUN_00429490.json) | reason VC5 register allocation (loop vars/flags/spilled object pointers); stub arg4 &factor stack address compared only within tolerance, dead local count not observable
// FUNCTION: CARMA2_HW 0x00429490
void C2_HOOK_FASTCALL FUN_00429490(tPhysics_object* pObject) {
    tPhysics_object* object2;
    br_matrix34 relative_mat;
    br_matrix34 inverse_mat;
    br_vector3 corners1[8];
    br_vector3 corners2[8];
    br_vector3 point;
    br_vector3 delta;
    float factor;
    int count;
    int i;
    int bit_x;
    int bit_y;
    int bit_z;
    tCollision_shape_common* shape;
    int j;

    count = 0;
    for (; pObject != NULL; pObject = pObject->next) {
        for (object2 = pObject->next; object2 != NULL; object2 = object2->next) {
            BrMatrix34LPInverse(&inverse_mat, &object2->transform_matrix);
            BrMatrix34Mul(&relative_mat, &pObject->transform_matrix, &inverse_mat);
            if (pObject->shape->common.type != kCollisionShapeType_Box || object2->shape->common.type != kCollisionShapeType_Box) {
                continue;
            }
            for (i = 0; i < 8; i++) {
                shape = &pObject->shape->common;
                bit_x = i & 2;
                if (bit_x) {
                    point.v[0] = shape->bb.min.v[0];
                } else {
                    point.v[0] = shape->bb.max.v[0];
                }
                bit_y = i & 1;
                if (bit_y) {
                    point.v[1] = shape->bb.min.v[1];
                } else {
                    point.v[1] = shape->bb.max.v[1];
                }
                bit_z = i & 4;
                if (bit_z) {
                    point.v[2] = shape->bb.min.v[2];
                } else {
                    point.v[2] = shape->bb.max.v[2];
                }
                BrMatrix34ApplyP(&corners1[i], &point, &relative_mat);
                shape = &object2->shape->common;
                if (bit_x) {
                    point.v[0] = shape->bb.min.v[0];
                } else {
                    point.v[0] = shape->bb.max.v[0];
                }
                if (bit_y) {
                    point.v[1] = shape->bb.min.v[1];
                } else {
                    point.v[1] = shape->bb.max.v[1];
                }
                if (bit_z) {
                    point.v[2] = shape->bb.min.v[2];
                } else {
                    point.v[2] = shape->bb.max.v[2];
                }
                delta.v[0] = point.v[0] - relative_mat.m[3][0];
                delta.v[1] = point.v[1] - relative_mat.m[3][1];
                delta.v[2] = point.v[2] - relative_mat.m[3][2];
                BrMatrix34TApplyV(&corners2[i], &delta, &relative_mat);
            }
            for (j = 0; j < 12; j++) {
                if (FUN_00460bd0(&corners1[gBox_edges_0058f7f0[j].index1], &corners1[gBox_edges_0058f7f0[j].index2], &object2->shape->common.bb, &point, &factor)
                        || FUN_00460bd0(&corners2[gBox_edges_0058f7f0[j].index1], &corners2[gBox_edges_0058f7f0[j].index2], &pObject->shape->common.bb, &point, &factor)) {
                    count++;
                    break;
                }
            }
        }
    }
}

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x004da290
tPhysics_object* C2_HOOK_FAKE_THISCALL MungeBoxObject(br_model* pModel, undefined4 pArg2, float pMass) {
    tPhysics_object* box;

    C2_HOOK_BUG_ON(sizeof(tPhysics_object) != 0x4d8);

    box = BrMemAllocate(sizeof(tPhysics_object), kMem_collision_object);
    box->shape = (tPhysics_shape*)AllocateBoxCollisionShape(kMem_collision_shape);
    box->shape->box.common.bb = pModel->bounds;
    FillInShape(box->shape);
    box->uid = gCollision_info_uid_counter;
    gCollision_info_uid_counter += 1;
    UpdateCollisionObject(box);
    box->M = pMass;
    BrVector3Set(&box->I,
        (CARPOCALYPSE2_SQR((pModel->bounds.max.v[1] - pModel->bounds.min.v[1]) * gFLOAT_0058ac00) + CARPOCALYPSE2_SQR((pModel->bounds.max.v[2] - pModel->bounds.min.v[2]) * gFLOAT_0058ac00)) * pMass * gFLOAT_0058ac04,
        (CARPOCALYPSE2_SQR((pModel->bounds.max.v[0] - pModel->bounds.min.v[0]) * gFLOAT_0058ac00) + CARPOCALYPSE2_SQR((pModel->bounds.max.v[2] - pModel->bounds.min.v[2]) * gFLOAT_0058ac00)) * pMass * gFLOAT_0058ac04,
        (CARPOCALYPSE2_SQR((pModel->bounds.max.v[0] - pModel->bounds.min.v[0]) * gFLOAT_0058ac00) + CARPOCALYPSE2_SQR((pModel->bounds.max.v[1] - pModel->bounds.min.v[1]) * gFLOAT_0058ac00)) * pMass * gFLOAT_0058ac04);
    BrVector3SetFloat(&box->field_0x54, 0.f, 1/-17.25f, 0.f);
    box->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    BrMatrix34Copy(&box->actor->t.t.mat, &box->transform_matrix);
    box->actor->model = pModel;
    box->box_face_ref = gFace_num__car - 2;
    return box;
}
#pragma inline_depth()

// FUNCTION: CARMA2_HW 0x004c5e20
tPhysics_joint* C2_HOOK_FASTCALL AllocatePhysicsJoint(int pCount_limits, int pType) {

    C2_HOOK_BUG_ON(offsetof(tPhysics_joint, limits) != 0x54);
    C2_HOOK_BUG_ON(sizeof(tPhysics_joint_limit) != 0x20);

    return BrMemAllocate(offsetof(tPhysics_joint, limits) + pCount_limits * sizeof(tPhysics_joint_limit), pType);
}

// FUNCTION: CARMA2_HW 0x004c5e40
tPhysics_joint* C2_HOOK_FASTCALL ClonePhysicsJoint(const tPhysics_joint* pJoint, int pType) {
    size_t size;
    tPhysics_joint* clone;

    size = offsetof(tPhysics_joint, limits) + pJoint->count_limits * sizeof(tPhysics_joint_limit);
    clone = BrMemAllocate(size, pType);
    memcpy(clone, pJoint, size);
    return clone;
}

// FUNCTION: CARMA2_HW 0x004c6200
int C2_HOOK_FASTCALL FUN_004c6200(tPhysics_object* pObject, tPhysics_object* pBuffer, int pRemaining) {
    tPhysics_object* pOutput;

    pOutput = pBuffer;
    if (pObject != NULL) {
        do {
            if (pRemaining != 0) {
                int remaining_before_children;

                memcpy(pOutput++, pObject, sizeof(tPhysics_object));
                pRemaining -= 1;
                remaining_before_children = pRemaining;
                if (pObject->child != NULL) {
                    pRemaining = FUN_004c6200(pObject->child, pOutput, pRemaining);
                    pOutput += remaining_before_children - pRemaining;
                }
            }
            pObject = pObject->next;
        } while (pObject != NULL);
    }
    return pRemaining;
}

// FUNCTION: CARMA2_HW 0x004c62e0
void C2_HOOK_FASTCALL UpdateChildTransforms(tPhysics_object* pObject, br_matrix34* pParent_matrix) {
    br_matrix34 inverse;
    br_matrix34 tmp_matrix;
    br_matrix34 tmp_matrix2;
    tPhysics_object* child;

    child = pObject->child;
    if (pParent_matrix != NULL) {
        BrMatrix34Mul(&pObject->actor->t.t.mat, &pObject->transform_matrix, pParent_matrix);
    } else {
        BrMatrix34LPInverse(&inverse, &pObject->transform_matrix);
        BrMatrix34Mul(&tmp_matrix2, &inverse, &pObject->actor->t.t.mat);
        BrMatrix34LPNormalise(&tmp_matrix, &tmp_matrix2);
        pParent_matrix = &tmp_matrix;
    }
    if (child != NULL) {
        BrMatrix34LPInverse(&inverse, &pObject->transform_matrix);
        BrMatrix34Mul(&tmp_matrix, &inverse, &pObject->actor->t.t.mat);
        do {
            UpdateChildTransforms(child, pParent_matrix);
            child = child->next;
        } while (child != NULL);
    }
    BrMatrix34Copy(&pObject->transform_matrix, &pObject->actor->t.t.mat);
}

// FUNCTION: CARMA2_HW 0x004c63b0
void C2_HOOK_FASTCALL PhysicsAddObject(tPhysics_object* pParent, tPhysics_object* pChild) {

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, next, 0x220);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, child, 0x224);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, parent, 0x228);

    pChild->next = pParent->child;
    pParent->child = pChild;
    pChild->parent = pParent;
}

// FUNCTION: CARMA2_HW 0x004b5fd0
tPhysics_object* C2_HOOK_FASTCALL PHILGetFirstObject(void) {
    if (gPHIL_enabled) {
        return NULL;
    }
    return gList_collision_infos;
}

// FUNCTION: CARMA2_HW 0x004b5ff0
tPhysics_object* C2_HOOK_FASTCALL PHILGetNextObject(tPhysics_object* pCollision_info) {
    if (gPHIL_enabled) {
        return NULL;
    }
    return pCollision_info->next;
}

// FUNCTION: CARMA2_HW 0x004b7310
void C2_HOOK_FASTCALL FUN_004b7310(tPhysics_object* pObject) {
    tPhysics_object* parent;
    br_matrix34 inverse;
    br_matrix34 normalised;

    parent = pObject->parent;
    pObject->flags |= 0x400;
    while (parent != NULL && (parent->flags & 0x400) != 0) {
        parent = parent->parent;
    }
    BrMatrix34LPInverse(&inverse, &parent->transform_matrix);
    BrMatrix34LPNormalise(&normalised, &inverse);
    BrMatrix34Mul(&pObject->field_0x1e8, &pObject->transform_matrix, &normalised);
    FUN_004b73a0(pObject->child, &pObject->transform_matrix, &parent->transform_matrix);
}

// FUNCTION: CARMA2_HW 0x004b73a0
void C2_HOOK_FASTCALL FUN_004b73a0(tPhysics_object* pObject, const br_matrix34* pParent_matrix, const br_matrix34* pRoot_matrix) {
    br_matrix34 inverse;
    br_matrix34 normalised;
    br_vector3 transformed;
    tPhysics_joint* joint;
    int i;

    for (; pObject != NULL; pObject = pObject->next) {
        if ((pObject->flags & 0x400) != 0) {
            BrMatrix34LPInverse(&inverse, &pObject->parent->transform_matrix);
            BrMatrix34LPNormalise(&normalised, &inverse);
            BrMatrix34Mul(&inverse, &pObject->transform_matrix, &normalised);
            BrMatrix34Mul(&pObject->field_0x1e8, &inverse, &pObject->parent->field_0x1e8);
        }
        joint = pObject->physics_joint1;
        if (joint != NULL) {
            if ((pObject->flags & 0x400) != 0) {
                FUN_004b73a0(pObject->child, pParent_matrix, pRoot_matrix);
            } else {
                BrMatrix34ApplyP(&transformed, &joint->field_0x14, pParent_matrix);
                DrMatrix34ApplyLPInverse(&joint->field_0x14, &transformed, pRoot_matrix);
                if (joint->type == 1 || joint->type == 4) {
                    BrMatrix34ApplyV(&transformed, &joint->parent_bone_axis, pParent_matrix);
                    BrMatrix34TApplyV(&joint->parent_bone_axis, &transformed, pRoot_matrix);
                }
            }
            for (i = 0; i < joint->count_limits; i++) {
                BrMatrix34ApplyV(&transformed, &joint->limits[i].parent, pParent_matrix);
                BrMatrix34TApplyV(&joint->limits[i].parent, &transformed, pRoot_matrix);
            }
        }
    }
}

#pragma auto_inline(off)
int C2_HOOK_FASTCALL FUN_004b8350(tPhysics_object* pObject, tCollision_contact* pContacts);
// C2PORT-NONMATCHING: best 36.05% | difftest PASS 2000 trials (spec difftest/specs/004b7510_MoveJointedObject.json) | reason VC5 stack/register layout and x87 scheduling
// FUNCTION: CARMA2_HW 0x004b7510
void C2_HOOK_FAKE_THISCALL MoveJointedObject(tPhysics_object* pObject, undefined4 pArg2, float pDelta_time) {
    tPhysics_object* object;
    tCollision_contact* contacts;
    int* scratch;
    int* joint_data;
    int* impulse_data;
    float fVar1;
    float fVar2;
    float object_energy;
    float* inertia;
    volatile float energy;
    int joint_count;
    int contact_count;
    int scratch_size;
    int i;
    int contact_list[40];
    char matrix_buffer[400];
    int joint_dt[100];

    (void)pArg2;
    joint_count = FUN_004b9650(pObject, NULL);
    if (joint_count != 0) {
        scratch_size = 8 * joint_count * (joint_count + 12) + 0x1c;
        scratch = (int*)PhysicsScratchSpaceCheck(scratch_size);
        IncreasePhysicsScratchPointer(scratch_size);
        contacts = (tCollision_contact*)(scratch + 7);
        scratch[0] = joint_count;
        scratch[1] = (int)(uintptr_t)contacts;
        scratch[5] = (int)(uintptr_t)(contacts + joint_count);
        scratch[6] = scratch[5] + joint_count * joint_count * sizeof(int);
        contact_count = FUN_004b8350(pObject, contacts);
        pObject->field_0x218 = (undefined4)(uintptr_t)scratch;
        for (i = 0; i < contact_count; i++) {
            contact_list[i] = (int)(uintptr_t)((char*)contacts + i * sizeof(tCollision_contact));
        }
        impulse_data = (int*)(uintptr_t)scratch[6];
        joint_data = (int*)(uintptr_t)scratch[5];
        FUN_004bf490(impulse_data, contact_count, contact_list, contact_count);
        FUN_004b77b0((int*)matrix_buffer, contact_list, contact_count);
        FUN_004c53b0((float*)impulse_data, (float*)joint_data, (float*)matrix_buffer, (float*)joint_dt, contact_count);

        if (pDelta_time != gFLOAT_0058a768) {
            energy = gFLOAT_0058a768;
            for (object = pObject; object != NULL; object = object->next) {
                object_energy = gFLOAT_0058a768;
                inertia = object->I.v;
                i = 3;
                do {
                    object_energy += inertia[0] * inertia[27] * inertia[27]
                            + object->M * inertia[24] * inertia[24];
                    inertia++;
                    i--;
                } while (i != 0);
                object_energy += PhysicsObjectsWeightedSquareSum(object->child);
                energy = object_energy + energy;
            }
            for (i = 0; i < contact_count; i++) {
                FUN_004b7960((tPhysics_joint_data*)(uintptr_t)contact_list[i], *(float*)&joint_dt[i]);
            }
            energy = gFLOAT_0058a768;
            for (object = pObject; object != NULL; object = object->next) {
                object_energy = gFLOAT_0058a768;
                inertia = object->I.v;
                i = 3;
                do {
                    object_energy += inertia[0] * inertia[27] * inertia[27]
                            + object->M * inertia[24] * inertia[24];
                    inertia++;
                    i--;
                } while (i != 0);
                object_energy += PhysicsObjectsWeightedSquareSum(object->child);
                energy = object_energy + energy;
            }
            if (pDelta_time > gFLOAT_0058a768) {
                FUN_004b8ee0(pObject, *(undefined4*)&pDelta_time, pDelta_time);
            }
        }
        return;
    }

    if (pObject->child != NULL) {
        FUN_004b8ee0(pObject, *(undefined4*)&pDelta_time, pDelta_time);
        return;
    }
    fVar1 = pObject->v.v[1];
    fVar2 = pObject->v.v[2];
    pObject->actor->t.t.mat.m[3][0] += pObject->v.v[0] * pDelta_time;
    pObject->actor->t.t.mat.m[3][1] += fVar1 * pDelta_time;
    pObject->actor->t.t.mat.m[3][2] += fVar2 * pDelta_time;
    RotateObject(pObject, *(undefined4*)&pDelta_time, pDelta_time);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b8ee0
void C2_HOOK_FAKE_THISCALL FUN_004b8ee0(tPhysics_object* pObject, undefined4 pArg2, float pDelta_time) {
    tPhysics_object* changed_object;
    tPhysics_object* child;
    tPhysics_object* matrix_parent;
    br_vector3 child_joint_pos;
    br_vector3 parent_joint_pos;

    (void)pArg2;
    changed_object = NULL;
    FUN_004b9230(pObject, &changed_object, pDelta_time);
    if (changed_object == NULL || changed_object->field_0x230 == NULL) {
        return;
    }
    child = changed_object;
    ((void (C2_HOOK_FAKE_THISCALL *)(tPhysics_object*, undefined4, float))FUN_004b8fe0)(child, *(undefined4*)&pDelta_time, pDelta_time);
    matrix_parent = child->field_0x230;
    do {
        BrMatrix34ApplyP(&child_joint_pos, &child->physics_joint1->field_0x08, &child->actor->t.t.mat);
        BrMatrix34ApplyP(&parent_joint_pos, &child->physics_joint1->field_0x14, &matrix_parent->actor->t.t.mat);
        child_joint_pos.v[0] -= parent_joint_pos.v[0];
        child_joint_pos.v[1] -= parent_joint_pos.v[1];
        child_joint_pos.v[2] -= parent_joint_pos.v[2];
        child->parent->actor->t.t.mat.m[3][0] += child_joint_pos.v[0];
        child->parent->actor->t.t.mat.m[3][1] += child_joint_pos.v[1];
        child->parent->actor->t.t.mat.m[3][2] += child_joint_pos.v[2];
        child = matrix_parent;
        matrix_parent = child->field_0x230;
    } while (matrix_parent != NULL);
    TrialPositionChildren(pObject);
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 47.77% | difftest PASS 2000 trials (spec difftest/specs/004b8fe0_FUN_004b8fe0.json) | reason x87 order and register/stack allocation
// FUNCTION: CARMA2_HW 0x004b8fe0
void C2_HOOK_FAKE_THISCALL FUN_004b8fe0(tPhysics_object* pObject, undefined4 pArg2, float pDelta_time) {
    br_vector3 joint_axis;
    br_vector3 joint_position;
    br_vector3 old_translation;
    float axis_offset;
    float axis_speed;

    (void)pArg2;
    if (pObject->physics_joint2->type == eJoint_translation) {
        BrMatrix34ApplyV(&joint_axis, &pObject->physics_joint2->hinge_axis, &pObject->transform_matrix);
        old_translation.v[0] = pObject->actor->t.t.mat.m[3][0];
        old_translation.v[1] = pObject->actor->t.t.mat.m[3][1];
        old_translation.v[2] = pObject->actor->t.t.mat.m[3][2];
        axis_offset = old_translation.v[0] * joint_axis.v[0]
                    + old_translation.v[1] * joint_axis.v[1]
                    + old_translation.v[2] * joint_axis.v[2];
        BrMatrix34ApplyP(&joint_position, &pObject->physics_joint2->field_0x08, &pObject->actor->t.t.mat);
        pObject->actor->t.t.mat.m[3][0] += pObject->field_0x17c.v[0] - joint_position.v[0];
        pObject->actor->t.t.mat.m[3][1] += pObject->field_0x17c.v[1] - joint_position.v[1];
        pObject->actor->t.t.mat.m[3][2] += pObject->field_0x17c.v[2] - joint_position.v[2];
        axis_offset -= pObject->actor->t.t.mat.m[3][0] * joint_axis.v[0]
                     + pObject->actor->t.t.mat.m[3][1] * joint_axis.v[1]
                     + pObject->actor->t.t.mat.m[3][2] * joint_axis.v[2];
        pObject->actor->t.t.mat.m[3][0] += axis_offset * joint_axis.v[0];
        pObject->actor->t.t.mat.m[3][1] += axis_offset * joint_axis.v[1];
        pObject->actor->t.t.mat.m[3][2] += axis_offset * joint_axis.v[2];
        axis_speed = pObject->v.v[0] * joint_axis.v[0]
                   + pObject->v.v[1] * joint_axis.v[1]
                   + pObject->v.v[2] * joint_axis.v[2];
        pObject->v.v[0] = axis_speed * joint_axis.v[0];
        pObject->v.v[1] = axis_speed * joint_axis.v[1];
        pObject->v.v[2] = axis_speed * joint_axis.v[2];
        pObject->actor->t.t.mat.m[3][0] += pDelta_time * axis_speed * joint_axis.v[0];
        pObject->actor->t.t.mat.m[3][1] += pDelta_time * axis_speed * joint_axis.v[1];
        pObject->actor->t.t.mat.m[3][2] += pDelta_time * axis_speed * joint_axis.v[2];
    } else {
        BrMatrix34ApplyP(&joint_position, &pObject->physics_joint2->field_0x08, &pObject->actor->t.t.mat);
        pObject->actor->t.t.mat.m[3][0] += pObject->field_0x17c.v[0] - joint_position.v[0];
        pObject->actor->t.t.mat.m[3][1] += pObject->field_0x17c.v[1] - joint_position.v[1];
        pObject->actor->t.t.mat.m[3][2] += pObject->field_0x17c.v[2] - joint_position.v[2];
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 51.06% | difftest PASS 2000 trials (spec difftest/specs/004b9230_FUN_004b9230.json) | reason x87-order/regalloc/stack-layout
// FUNCTION: CARMA2_HW 0x004b9230
void C2_HOOK_FAKE_THISCALL FUN_004b9230(tPhysics_object* pObject, tPhysics_object** ppChanged_object, float pDelta_time) {
    tPhysics_object* parent;
    tPhysics_object* child;
    br_matrix34 inverse_parent_matrix;
    br_matrix34 relative_matrix;
    br_scalar angle;
    union {
        struct {
            br_vector3 inverse_position;
            br_vector3 relative_position;
            br_vector3 angular_velocity;
        } parent_motion;
        struct {
            br_vector3 parent_omega_world;
            br_vector3 parent_omega_local;
            br_vector3 scratch;
        } hinge_motion;
        struct {
            br_vector3 child_joint_position;
            br_vector3 parent_joint_position;
            br_vector3 scratch;
        } joint_positions;
    } vectors;

    if (pObject->physics_joint1 == NULL || pObject->physics_joint1->type == eJoint_none) {
        if (pObject->physics_joint2 != NULL && pObject->physics_joint2->type != eJoint_none) {
            RotateObject(pObject, (undefined4)(uintptr_t)ppChanged_object, pDelta_time);
            FUN_004b8fe0(pObject, (undefined4)(uintptr_t)ppChanged_object, pDelta_time);
        } else {
            pObject->actor->t.t.mat.m[3][0] += pObject->v.v[0] * pDelta_time;
            pObject->actor->t.t.mat.m[3][1] += pObject->v.v[1] * pDelta_time;
            pObject->actor->t.t.mat.m[3][2] += pObject->v.v[2] * pDelta_time;
            RotateObject(pObject, (undefined4)(uintptr_t)ppChanged_object, pDelta_time);
        }
    } else {
        parent = pObject->field_0x230;
        if (parent == NULL) {
            PhysicsError(11, gEmpty_string_00676920);
        }
        if (pObject->flags & 0x400) {
            BrMatrix34Mul(&pObject->actor->t.t.mat, &pObject->field_0x1e8, &parent->actor->t.t.mat);
            BrMatrix34TApplyV(&pObject->omega, &parent->rotate_omega, &pObject->field_0x1e8);
            pObject->rotate_omega = pObject->omega;
            DrMatrix34ApplyLPInverse(&vectors.parent_motion.inverse_position, &parent->cmpos, &pObject->field_0x1e8);
            BrVector3Sub(&vectors.parent_motion.relative_position, &pObject->cmpos, &vectors.parent_motion.inverse_position);
            BrVector3Cross(&vectors.parent_motion.angular_velocity, &pObject->omega, &vectors.parent_motion.relative_position);
            BrMatrix34ApplyV(&pObject->v, &vectors.parent_motion.angular_velocity, &pObject->transform_matrix);
            pObject->v.v[0] += parent->v.v[0];
            pObject->v.v[1] += parent->v.v[1];
            pObject->v.v[2] += parent->v.v[2];
            BrMatrix34TApplyV(&pObject->velocity_car_space, &pObject->v, &pObject->transform_matrix);
        } else {
            switch (pObject->physics_joint1->type) {
            case eJoint_hinge:
            case eJoint_quick_hinge:
                BrMatrix34ApplyV(&vectors.hinge_motion.parent_omega_world, &parent->rotate_omega, &parent->transform_matrix);
                BrMatrix34TApplyV(&vectors.hinge_motion.parent_omega_local, &vectors.hinge_motion.parent_omega_world, &pObject->transform_matrix);
                angle = ((pObject->omega.v[0] - vectors.hinge_motion.parent_omega_local.v[0]) * pObject->physics_joint1->hinge_axis.v[0]
                       + (pObject->omega.v[1] - vectors.hinge_motion.parent_omega_local.v[1]) * pObject->physics_joint1->hinge_axis.v[1]
                       + (pObject->omega.v[2] - vectors.hinge_motion.parent_omega_local.v[2]) * pObject->physics_joint1->hinge_axis.v[2]) * pDelta_time;
                BrMatrix34LPInverse(&inverse_parent_matrix, &parent->transform_matrix);
                BrMatrix34Mul(&relative_matrix, &pObject->transform_matrix, &inverse_parent_matrix);
                BrMatrix34Mul(&pObject->actor->t.t.mat, &relative_matrix, &parent->actor->t.t.mat);
                BrMatrix34PreRotate(&pObject->actor->t.t.mat, (br_angle)(angle * gDOUBLE_0058a770), &pObject->physics_joint1->hinge_axis);
                pObject->rotate_omega = pObject->omega;
                BrMatrix34ApplyV(&vectors.hinge_motion.scratch, &pObject->omega, &pObject->transform_matrix);
                BrMatrix34TApplyV(&pObject->omega, &vectors.hinge_motion.scratch, &pObject->actor->t.t.mat);
                break;
            case eJoint_universal:
                RotateObject(pObject, (undefined4)(uintptr_t)ppChanged_object, pDelta_time);
                break;
            case eJoint_ball_n_socket:
                RotateObject(pObject, *(undefined4*)&pDelta_time, pDelta_time);
                break;
            default:
                PhysicsError(5, NULL);
            }
            BrMatrix34ApplyP(&vectors.joint_positions.child_joint_position, &pObject->physics_joint1->field_0x08, &pObject->actor->t.t.mat);
            BrMatrix34ApplyP(&vectors.joint_positions.parent_joint_position, &pObject->physics_joint1->field_0x14, &parent->actor->t.t.mat);
            pObject->actor->t.t.mat.m[3][0] += vectors.joint_positions.parent_joint_position.v[0] - vectors.joint_positions.child_joint_position.v[0];
            pObject->actor->t.t.mat.m[3][1] += vectors.joint_positions.parent_joint_position.v[1] - vectors.joint_positions.child_joint_position.v[1];
            pObject->actor->t.t.mat.m[3][2] += vectors.joint_positions.parent_joint_position.v[2] - vectors.joint_positions.child_joint_position.v[2];
        }
    }
    for (child = pObject->child; child != NULL; child = child->next) {
        FUN_004b9230(child, ppChanged_object, pDelta_time);
    }
    if (pObject->physics_joint2 != NULL) {
        *ppChanged_object = pObject;
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 60.39% | difftest PASS 6000 trials (spec difftest/specs/004b99e0_InternalPrepareObject_disabled_root_bounds.json, difftest/specs/004b99e0_InternalPrepareObject_trial_pose_branch.json, difftest/specs/004b99e0_InternalPrepareObject_parented_modes.json) | reason physics call paths retain different register and stack layout
// FUNCTION: CARMA2_HW 0x004b99e0
void C2_HOOK_FASTCALL InternalPrepareObject(tPhysics_object* pObject) {
    tPhysics_object* child;
    tPhysics_object* parent;
    int i;
    int j;
    int differ;

    if (pObject->M == 0.f) {
        pObject->M = 0.01f;
        BrVector3SetFloat(&pObject->I, 0.01f, 0.01f, 0.01f);
    }
    pObject->flags &= ~0x1200;
    pObject->field_0x1e0[0] = pObject->disable_move_rotate;
    pObject->field_0x1de = 0;
    pObject->collision_flag = 0;
    pObject->field_0x1dc = 0;
    memset(&pObject->field_0x1e0[4], 0, 4);
    pObject->field_0x21c = 1.f;
    pObject->field183_0x1d8 = pObject;
    pObject->field_0x218 = 0;

    BrMatrix34TApplyV(&pObject->velocity_car_space, &pObject->v, &pObject->transform_matrix);
    pObject->field_0x1b4.v[0] = pObject->v.v[0];
    pObject->field_0x1b4.v[1] = pObject->v.v[1];
    pObject->field_0x1b4.v[2] = pObject->v.v[2];
    parent = pObject->parent;
    pObject->field_0x1cc.v[0] = pObject->omega.v[0];
    pObject->field_0x1cc.v[1] = pObject->omega.v[1];
    pObject->field_0x1cc.v[2] = pObject->omega.v[2];

    if (parent != NULL) {
        pObject->disable_move_rotate = parent->disable_move_rotate;
    }
    if (pObject->disable_move_rotate == 0 && pObject->field_0x1df != 0) {
        pObject->I.v[0] *= 50.f;
        pObject->I.v[1] *= 50.f;
        pObject->I.v[2] *= 50.f;
        pObject->field_0x21c *= 50.f;
    }

    if (pObject->disable_move_rotate != 0 && pObject->field_0xf0 != 2) {
        BrMatrix34Copy(&pObject->actor->t.t.mat, &pObject->transform_matrix);
        for (child = pObject->child; child != NULL; child = child->next) {
            InternalPrepareObject(child);
        }
        if (pObject->parent == NULL) {
            MergeBoundsIntoParent(pObject, NULL);
        }
        return;
    }

    if (pObject->field_0xf0 == 2) {
        float dx;
        float dy;
        float dz;
        float d2;

        pObject->field_0x230 = pObject->parent;
        dx = pObject->actor->t.t.mat.m[3][0] - pObject->transform_matrix.m[3][0];
        dy = pObject->actor->t.t.mat.m[3][1] - pObject->transform_matrix.m[3][1];
        dz = pObject->actor->t.t.mat.m[3][2] - pObject->transform_matrix.m[3][2];
        d2 = dx * dx + dy * dy + dz * dz;
        if (d2 > 1.f) {
            PhysicsWarning("Stupid trial position");
            BrMatrix34Copy(&pObject->transform_matrix, &pObject->actor->t.t.mat);
        }
        differ = 0;
        for (j = 3; j >= 1 && !differ; j--) {
            for (i = 0; i < 3; i++) {
                if (pObject->transform_matrix.m[j][i] != pObject->actor->t.t.mat.m[j][i]) {
                    pObject->disable_move_rotate = 0;
                    differ = 1;
                    break;
                }
            }
        }
        for (child = pObject->child; child != NULL; child = child->next) {
            InternalPrepareObject(child);
        }
        MergeBoundsIntoParent(pObject, NULL);
        return;
    }

    if (pObject->field_0xf0 == 1) {
        BrMatrix34Copy((br_matrix34*)pObject->field_0xbc, &pObject->actor->t.t.mat);
    } else {
        BrMatrix34LPNormalise(&pObject->actor->t.t.mat, &pObject->transform_matrix);
    }
    for (child = pObject->child; child != NULL; child = child->next) {
        InternalPrepareObject(child);
    }
    if (pObject->parent != NULL) {
        return;
    }
    MoveJointedObject(pObject, 0, 0.04f);
    MergeBoundsIntoParent(pObject, NULL);
}

/* Transform bb1 by both object transforms, then merge the object and its
 * children into the parent bounds. */
// FUNCTION: CARMA2_HW 0x004b9ce0
void C2_HOOK_FASTCALL MergeBoundsIntoParent(tPhysics_object* pObject, br_bounds3* pParent) {
    tPhysics_object* child;
    br_bounds3 local_box;
    float fVar1;
    int i;

    GetNewBoundingBox(&pObject->field_0xf4, &pObject->bb1, &pObject->transform_matrix);

    GetNewBoundingBox(&local_box, &pObject->bb1, &pObject->actor->t.t.mat);
    for (i = 0; i < 3; i++) {
        if (pObject->field_0xf4.min.v[i] > local_box.min.v[i]) {
            pObject->field_0xf4.min.v[i] = local_box.min.v[i];
        }
        if (pObject->field_0xf4.max.v[i] < local_box.max.v[i]) {
            pObject->field_0xf4.max.v[i] = local_box.max.v[i];
        }
        pObject->field_0xf4.min.v[i] -= gDOUBLE_0058a788;
        pObject->field_0xf4.max.v[i] -= gDOUBLE_0058a790;
    }

    pObject->field_0x10c = pObject->field_0xf4;

    for (child = pObject->child; child != NULL; child = child->next) {
        MergeBoundsIntoParent(child, &pObject->field_0x10c);
    }

    if (pParent != NULL) {
        for (i = 0; i < 3; i++) {
            if (pObject->field_0x10c.min.v[i] < pParent->min.v[i]) {
                pParent->min.v[i] = pObject->field_0x10c.min.v[i];
            }
            if (pObject->field_0x10c.max.v[i] > pParent->max.v[i]) {
                pParent->max.v[i] = pObject->field_0x10c.max.v[i];
            }
        }
        return;
    }

    fVar1 = pObject->field_0x10c.max.v[0] - pObject->transform_matrix.m[3][0];
    if (gFLOAT_00692dd0 <= fVar1) {
        gFLOAT_00692dd0 = fVar1;
    }
    fVar1 = pObject->field_0x10c.min.v[0] - pObject->transform_matrix.m[3][0];
    if (gFLOAT_00692dd4 >= fVar1) {
        gFLOAT_00692dd4 = fVar1;
    }
}

// FUNCTION: CARMA2_HW 0x004b9f40
void C2_HOOK_FASTCALL MakeObjectListDoSomething(tPhysics_object* pObject) {

    while (pObject != NULL) {
        if (pObject->disable_move_rotate) {
            pObject->disable_move_rotate = 0;
            pObject->box_face_ref = gFace_num__car - 2;
        }
        if (pObject->child != NULL) {
            MakeObjectListDoSomething(pObject->child);
        }
        pObject = pObject->next;
    }
}

// C2PORT-NONMATCHING: best 67.11% | difftest PASS 2000 trials (spec difftest/specs/004b9f90_CheckForObjectHierachyTouchingAnotherObject.json) | reason VC5 register allocation and loop layout
// FUNCTION: CARMA2_HW 0x004b9f90
int C2_HOOK_FASTCALL CheckForObjectHierachyTouchingAnotherObject(tPhysics_object* pObject, tPhysics_object* pList, tPhysics_object* pList_original) {
    tPhysics_object* item;
    tPhysics_object* child;
    tPhysics_object* root;

    if (gDOUBLE_0058a798 < pObject->field_0xf4.min.v[0] - pList->field_0xf4.max.v[0]
            || gDOUBLE_0058a798 < pObject->field_0xf4.min.v[1] - pList->field_0xf4.max.v[1]
            || gDOUBLE_0058a798 < pObject->field_0xf4.min.v[2] - pList->field_0xf4.max.v[2]
            || gDOUBLE_0058a798 < pList->field_0xf4.min.v[0] - pObject->field_0xf4.max.v[0]
            || gDOUBLE_0058a798 < pList->field_0xf4.min.v[1] - pObject->field_0xf4.max.v[1]
            || gDOUBLE_0058a798 < pList->field_0xf4.min.v[2] - pObject->field_0xf4.max.v[2]) {
        return 0;
    }
    if (pList->disable_move_rotate) {
        root = pList;
        while (root->parent != NULL) {
            if (root->physics_joint1 == NULL || root->physics_joint1->type == 0) {
                break;
            }
            root = root->parent;
        }
        root->disable_move_rotate = 0;
        root->box_face_ref = gFace_num__car - 2;
        if (root->child != NULL || root->physics_joint1 != NULL || root->physics_joint2 != NULL) {
            if (root->field_0x218 == 0) {
                MoveJointedObject(root, 0, 0.f);
            }
            for (child = root->child; child != NULL; child = child->next) {
                if (child->disable_move_rotate) {
                    child->disable_move_rotate = 0;
                    child->box_face_ref = gFace_num__car - 2;
                }
                if (child->child != NULL) {
                    MakeObjectListDoSomething(child->child);
                }
            }
        }
        for (item = pList_original; item != NULL; item = item->next) {
            if (item->disable_move_rotate) {
                CheckForObjectHierachyTouchingAnotherObject(root, item, pList_original);
                child = root->child;
                if (child != NULL && item->disable_move_rotate) {
                    while (child != NULL
                            && CheckForObjectHierachyTouchingAnotherObject(child, item, pList_original) == 0) {
                        child = child->next;
                    }
                }
                if (item->child != NULL) {
                    CheckForObjectHierachyTouchingObjectList(root, item->child, pList_original);
                }
            }
        }
    }
    return 1;
}

// C2PORT-NONMATCHING: best 54.44% | difftest PASS 2000 trials (spec difftest/specs/004b6090_PHILMakeObjectActive.json; NULL_QUEUE PASS 2000) | reason VC5 register and branch layout
// FUNCTION: CARMA2_HW 0x004b6090
int C2_HOOK_FASTCALL PHILMakeObjectActive(tPhysics_object* pObject, const br_vector3* pVel, const br_vector3* pOmega, int pArg4) {
    register tQueued_object_info* object_info;

    if (gPHIL_enabled) {
        return 0;
    }
    object_info = pObject->field_0x240;
    if (pObject->field_0x239 == 2) {
        object_info->field_0x8 = 2;
        if (pVel != NULL) {
            object_info->field_0x18.v[0] = pVel->v[0];
            object_info->field_0x18.v[1] = pVel->v[1];
            object_info->field_0x18.v[2] = pVel->v[2];
            object_info->flags |= 0x1;
        }
        if (pOmega != NULL) {
            object_info->field_0x24.v[0] = pOmega->v[0];
            object_info->field_0x24.v[1] = pOmega->v[1];
            object_info->field_0x24.v[2] = pOmega->v[2];
            object_info->flags |= 0x2;
        }
        if (pArg4) {
            object_info->field_0x4 |= 0x2;
        } else {
            object_info->field_0x4 &= ~0x2;
        }
        return 0;
    }
    if (object_info == NULL) {
        return 3;
    }
    object_info->field_0x8 = 2;
    pObject->flags &= ~0x20;
    if (pArg4) {
        object_info->field_0x4 |= 0x2;
    } else {
        object_info->field_0x4 &= ~0x2;
    }
    if (pVel != NULL) {
        pObject->v.v[0] = pVel->v[0];
        pObject->v.v[1] = pVel->v[1];
        pObject->v.v[2] = pVel->v[2];
    }
    if (pOmega != NULL) {
        pObject->omega.v[0] = pOmega->v[0];
        pObject->omega.v[1] = pOmega->v[1];
        pObject->omega.v[2] = pOmega->v[2];
    }
    ResetCarSpecialVolume(pObject);
    return 0;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 36.02% | difftest PASS 2000 trials (spec difftest/specs/004ba190_CheckForObjectHierachyTouchingObjectList.json) | reason duplicated-bounds-checks,register-allocation,stack-layout
// FUNCTION: CARMA2_HW 0x004ba190
void C2_HOOK_FASTCALL CheckForObjectHierachyTouchingObjectList(tPhysics_object* pObject, tPhysics_object* pList, tPhysics_object* pList_original) {
    tPhysics_object* root;
    tPhysics_object* child;
    tPhysics_object* list_child;
    tPhysics_object* item;

#define OBJECT_BOUNDS_OVERLAP(a, b) \
    ((a)->field_0xf4.min.v[0] - (b)->field_0xf4.max.v[0] <= gDOUBLE_0058a798 \
        && (a)->field_0xf4.min.v[1] - (b)->field_0xf4.max.v[1] <= gDOUBLE_0058a798 \
        && (a)->field_0xf4.min.v[2] - (b)->field_0xf4.max.v[2] <= gDOUBLE_0058a798 \
        && (b)->field_0xf4.min.v[0] - (a)->field_0xf4.max.v[0] <= gDOUBLE_0058a798 \
        && (b)->field_0xf4.min.v[1] - (a)->field_0xf4.max.v[1] <= gDOUBLE_0058a798 \
        && (b)->field_0xf4.min.v[2] - (a)->field_0xf4.max.v[2] <= gDOUBLE_0058a798)

    if (pList == NULL) {
        return;
    }
    item = pList;
    do {
            if (item->disable_move_rotate) {
                if (OBJECT_BOUNDS_OVERLAP(pObject, item)) {
                    root = item;
                    while (root->parent != NULL) {
                        if (root->physics_joint1 == NULL || root->physics_joint1->type == eJoint_none) {
                            break;
                        }
                        root = root->parent;
                    }
                    root->disable_move_rotate = 0;
                    root->box_face_ref = gFace_num__car - 2;
                    if (root->child != NULL || root->physics_joint1 != NULL || root->physics_joint2 != NULL) {
                        if (root->field_0x218 == 0) {
                            MoveJointedObject(root, 0, 0.f);
                        }
                        for (child = root->child; child != NULL; child = child->next) {
                            if (child->disable_move_rotate) {
                                child->disable_move_rotate = 0;
                                child->box_face_ref = gFace_num__car - 2;
                            }
                            if (child->child != NULL) {
                                MakeObjectListDoSomething(child->child);
                            }
                        }
                    }
                    CheckForObjectHierachyTouchingObjectList(root, pList_original, pList_original);
                }
                child = pObject->child;
                if (child != NULL && item->disable_move_rotate) {
                    for (; child != NULL; child = child->next) {
                        if (OBJECT_BOUNDS_OVERLAP(child, item)) {
                            root = item;
                            while (root->parent != NULL) {
                                if (root->physics_joint1 == NULL || root->physics_joint1->type == eJoint_none) {
                                    break;
                                }
                                root = root->parent;
                            }
                            root->box_face_ref = gFace_num__car - 2;
                            root->disable_move_rotate = 0;
                            if (root->child != NULL || root->physics_joint1 != NULL || root->physics_joint2 != NULL) {
                                if (root->field_0x218 == 0) {
                                    MoveJointedObject(root, 0, 0.f);
                                }
                                for (list_child = root->child; list_child != NULL; list_child = list_child->next) {
                                    if (list_child->disable_move_rotate) {
                                        list_child->disable_move_rotate = 0;
                                        list_child->box_face_ref = gFace_num__car - 2;
                                    }
                                    if (list_child->child != NULL) {
                                        MakeObjectListDoSomething(list_child->child);
                                    }
                                }
                            }
                            CheckForObjectHierachyTouchingObjectList(root, pList_original, pList_original);
                        }
                    }
                }
                if (item->child != NULL) {
                    CheckForObjectHierachyTouchingObjectList(pObject, item->child, pList_original);
                }
            }
        item = item->next;
    } while (item != NULL);
#undef OBJECT_BOUNDS_OVERLAP
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004b6210
int C2_HOOK_FASTCALL PHILAddActiveObject(tPhysics_object* pInfo, undefined4* pArg2, const br_vector3* pArg3, const br_vector3* pArg4) {
    int result;

    if (gPHIL_enabled) {
        return 0;
    }
    result = PHILAddObject(pInfo);
    if (result != 0) {
        return result;
    }
    if (pArg2 != NULL && !gPHIL_enabled) {
        if (pInfo->field_0x239 == 2) {
            tQueued_object_info* object_info = pInfo->field_0x240;
            object_info->field_0x30 = *(br_matrix34*)pArg2;
            object_info->flags |= 0x4;
        } else {
            tQueued_object_info* object_info = pInfo->field_0x240;
            if (object_info != NULL) {
                if (object_info->field_0x8 != 1) {
                    return 4;
                }
                object_info->object->actor->t.t.mat = *(br_matrix34*)pArg2;
            } else {
                return 3;
            }
        }
    }
    return PHILMakeObjectActive(pInfo, pArg3, pArg4, 0);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b62b0
int C2_HOOK_FASTCALL PHILAddObjectWithFlag(tPhysics_object* pObject) {
    int result;

    if (gPHIL_enabled) {
        return 0;
    }
    if (gPHIL_munging_objects) {
        gPHIL_object_added = 1;
    }
    result = PHILAddObject(pObject);
    gPHIL_object_added = 0;
    return result;
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL SwapPair(tPhysics_object* pObj1, tPhysics_object* pObj2) {
    tPhysics_object* obj1_prev;
    tPhysics_object* obj2_next;

    obj1_prev = pObj1->prev;
    obj2_next = pObj2->next;
    if (obj1_prev != NULL) {
        obj1_prev->next = pObj2;
    }
    if (obj2_next != NULL) {
        obj2_next->prev = pObj1;
    }
    pObj1->prev = pObj2;
    pObj2->prev = obj1_prev;
    pObj1->next = obj2_next;
    pObj2->next = pObj1;
}

void C2_HOOK_FASTCALL CheckObjectsPostionInList(tPhysics_object* pObject, tPhysics_object** pList) {

    if (pObject->prev != NULL && pObject->transform_matrix.m[3][0] < pObject->prev->transform_matrix.m[3][0]) {
        do {
            SwapPair(pObject->prev, pObject);
        } while (pObject->prev != NULL && pObject->transform_matrix.m[3][0] < pObject->prev->transform_matrix.m[3][0]);
        if (pObject->prev == NULL) {
            *pList = pObject;
        }
    } else if (pObject->next != NULL && pObject->transform_matrix.m[3][0] > pObject->next->transform_matrix.m[3][0]) {
        do {
            SwapPair(pObject, pObject->next);
        } while (pObject->next != NULL && pObject->transform_matrix.m[3][0] > pObject->next->transform_matrix.m[3][0]);
    }
}

void C2_HOOK_FASTCALL MakeObjectDoSomething(tPhysics_object* pObject, tPhysics_object* pList) {
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, parent, 0x228);

    if (pObject->disable_move_rotate) {
        while (pObject->parent != NULL && pObject->parent->physics_joint1 != NULL && pObject->parent->physics_joint1->type != eJoint_none) {
            pObject = pObject->parent;
        }
        pObject->box_face_ref = gFace_num__car - 2;
        pObject->disable_move_rotate = 0;
        if (pObject->child != NULL || pObject->physics_joint1 != NULL || pObject->physics_joint2 != NULL) {
            if (pObject->field_0x218 == 0) {
                MoveJointedObject(pObject CARPOCALYPSE2_THISCALL_EDX, 0.f);
            }
            MakeObjectListDoSomething(pObject->child);
        }
        CheckForObjectHierachyTouchingObjectList(pObject, pList, pList);
    }
}

void C2_HOOK_FASTCALL AddObjectToReducedList(tPhysics_object* pObject, tPhysics_object** pReduced_list) {

    pObject->field_0x234 = *pReduced_list;
    pReduced_list[0] = pObject;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 10.32% | difftest PASS 2000 trials (spec difftest/specs/004b9770_PrepareObject.json) | reason helper-inline and VC5 register allocation
// FUNCTION: CARMA2_HW 0x004b9770
void C2_HOOK_FASTCALL PrepareObject(tPhysics_object* pObject, tPhysics_object** pList) {

    InternalPrepareObject(pObject);
    gPrepared_objects = 1;
    CheckObjectsPostionInList(pObject, pList);
    if (gINT_006940ac) {
        MakeObjectDoSomething(pObject, *pList);
    }
    if (gReduced_object_list != NULL) {
        AddObjectToReducedList(pObject, gReduced_object_list);
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 64.93% | difftest PASS 2000 trials (spec difftest/specs/004be0e0_FUN_004be0e0.json) | reason VC5 register allocation and stack layout
// FUNCTION: CARMA2_HW 0x004be0e0
void C2_HOOK_FASTCALL FUN_004be0e0(tPhysics_object* pObject, tPhysics_object* pList) {
    tPhysics_object* item;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, disable_move_rotate, 0xec);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, parent, 0x228);

    if (!pObject->disable_move_rotate) {
        return;
    }
    while (pObject->parent != NULL && pObject->physics_joint1 != NULL && pObject->physics_joint1->type != eJoint_none) {
        pObject = pObject->parent;
    }
    pObject->disable_move_rotate = 0;
    pObject->box_face_ref = gFace_num__car - 2;
    if (pObject->child != NULL || pObject->physics_joint1 != NULL || pObject->physics_joint2 != NULL) {
        if (pObject->field_0x218 == 0) {
            MoveJointedObject(pObject CARPOCALYPSE2_THISCALL_EDX, 0.f);
        }
        {
            tPhysics_object* child;
            for (child = pObject->child; child != NULL; child = child->next) {
                if (child->disable_move_rotate) {
                    child->disable_move_rotate = 0;
                    child->box_face_ref = gFace_num__car - 2;
                }
                if (child->child != NULL) {
                    MakeObjectListDoSomething(child->child);
                }
            }
        }
    }

    for (item = pList; item != NULL; item = item->next) {
        if (!item->disable_move_rotate) {
            continue;
        }
        if (pObject->field_0xf4.min.v[0] - item->field_0xf4.max.v[0] <= gDOUBLE_0058a798
                && pObject->field_0xf4.min.v[1] - item->field_0xf4.max.v[1] <= gDOUBLE_0058a798
                && pObject->field_0xf4.min.v[2] - item->field_0xf4.max.v[2] <= gDOUBLE_0058a798
                && item->field_0xf4.min.v[0] - pObject->field_0xf4.max.v[0] <= gDOUBLE_0058a798
                && item->field_0xf4.min.v[1] - pObject->field_0xf4.max.v[1] <= gDOUBLE_0058a798
                && item->field_0xf4.min.v[2] - pObject->field_0xf4.max.v[2] <= gDOUBLE_0058a798) {
            FUN_004be0e0(item, pList);
        }
        {
            tPhysics_object* child;
            child = pObject->child;
            if (child != NULL && item->disable_move_rotate) {
                for (; child != NULL; child = child->next) {
                    if (child->field_0xf4.min.v[0] - item->field_0xf4.max.v[0] <= gDOUBLE_0058a798
                            && child->field_0xf4.min.v[1] - item->field_0xf4.max.v[1] <= gDOUBLE_0058a798
                            && child->field_0xf4.min.v[2] - item->field_0xf4.max.v[2] <= gDOUBLE_0058a798
                            && item->field_0xf4.min.v[0] - child->field_0xf4.max.v[0] <= gDOUBLE_0058a798
                            && item->field_0xf4.min.v[1] - child->field_0xf4.max.v[1] <= gDOUBLE_0058a798
                            && item->field_0xf4.min.v[2] - child->field_0xf4.max.v[2] <= gDOUBLE_0058a798) {
                        FUN_004be0e0(item, pList);
                        break;
                    }
                }
            }
        }
        if (item->child != NULL) {
            CheckForObjectHierachyTouchingObjectList(pObject, item->child, pList);
        }
    }
}

// C2PORT-NONMATCHING: best 98.99% | difftest PASS 2000 trials (spec difftest/specs/004b5d40_PHILAddObject.json) | reason global-layout-boundary
// FUNCTION: CARMA2_HW 0x004b5d40
int C2_HOOK_FASTCALL PHILAddObject(tPhysics_object* pObject) {
    tPHIL_queued_header* object_info;
    tPHIL_queued_header* current_object_info;

    object_info = NULL;
    if (gPHIL_enabled) {
        return 0;
    }
    if (gPHIL_munging_objects && !gPHIL_object_added) {
        C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tQueued_object_info, object, 0x0);
        C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tQueued_object_info, field_0x8, 0x8);
        C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tQueued_object_info, flags, 0x60);

        gPhil_queued_objects.objects[gPHIL_count_queued_objects].collision_info = pObject;
        gPhil_queued_objects.objects[gPHIL_count_queued_objects].field_0x8 = 1;
        gPhil_queued_objects.objects[gPHIL_count_queued_objects].flags = 0;
        pObject->field_0x240 = &gPhil_queued_objects.objects[gPHIL_count_queued_objects];
        pObject->field_0x239 = 2;
        gPHIL_count_queued_objects += 1;
        return 0;
    }
    for (current_object_info = &gPhil_queued_objects.headers[0];
            (intptr_t)current_object_info < (intptr_t)&gPhil_queued_objects.headers[CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers)];
            current_object_info++) {
        if (current_object_info->collision_info == NULL) {
            if (object_info == NULL) {
                object_info = current_object_info;
            }
        } else if (pObject == current_object_info->collision_info) {
            return 2;
        }
    }
    if (object_info != NULL) {
        memset(object_info, 0, sizeof(*object_info));
        object_info->collision_info = pObject;
        object_info->field_0x8 = 1;
        pObject->field_0x240 = object_info;
        pObject->flags |= 0x20;
        pObject->field_0x239 = 1;
        gPHIL_count_list_collision_infos += 1;
        if (gList_collision_infos != NULL) {
            pObject->next = gList_collision_infos->next;
            gList_collision_infos->next = pObject;
            pObject->prev = gList_collision_infos;
            if (pObject->next != NULL) {
                pObject->next->prev = pObject;
            }
        } else {
            gList_collision_infos = pObject;
            pObject->next = NULL;
            pObject->prev = NULL;
        }
        if (gCrush_deferred) {
            PrepareObject(pObject, &gList_collision_infos);
        }
        return 0;
    }
    return 1;
}

C2_MATCHING_INLINE void C2_HOOK_FASTCALL PHILMungeObjects(tPhysics_object* pObjects) {
    tPhysics_object* object;

    for (object = pObjects; object != NULL; object = object->next) {
        tPHIL_queued_header* object_info;
        tSpecial_volume* original_last_special_volume;
        float original_water_depth_factor;

#ifdef CARPOCALYPSE2_FIX_BUGS
        original_last_special_volume = NULL;
#endif

        object_info = object->field_0x240;
        if (!object->disable_move_rotate && object_info != NULL && object_info->field_0x8 == 2) {
            if (object_info->field_0x4 & 0x8) {
                original_water_depth_factor = object->water_depth_factor;
                original_last_special_volume = object->last_special_volume;
                if (object->water_d != 10000.f) {
                    TestAutoSpecialVolume(object);
                }
                MungeSpecialVolume(object);
            }
            if (object_info->field_0x10 != 0.f || (object->last_special_volume != NULL && (object_info->field_0x4 & 0x8))) {
                float gravity;

                gravity = object_info->field_0x10 != 0.f ? object_info->field_0x10 : 1.0;
                ProcessGravity(object_info, object, gravity);
            }
            if (object_info->field_0xc != 0.f || (object->last_special_volume != NULL && (object_info->field_0x4 & 0x8))) {
                float drag;

                drag = object_info->field_0xc != 0.f ? object_info->field_0xc : 1.0;
                ProcessDrag(object_info, object, drag);
            }
            if (object_info->field_0x4 & 0x8) {
                if (!(object->last_special_volume != NULL && object->last_special_volume->viscosity_multiplier > 2.0 && object->water_depth_factor == 1.0)
                        || (original_last_special_volume != NULL && original_last_special_volume->viscosity_multiplier > 2.0 && original_water_depth_factor >= 1.0)) {

                    if (!(object->last_special_volume != NULL && object->last_special_volume->viscosity_multiplier > 2.0)
                            && (original_last_special_volume != NULL && original_last_special_volume->viscosity_multiplier > 2.0)) {
                        ProcessDrag(object_info, object, gFLOAT_0065d00c);
                    }
                }
                else {
                    ProcessDrag(object_info, object, gFLOAT_0065d008);
                }
                if (object_info->field_0x14 != 0.f
                        && object->last_special_volume != NULL
                        && object->last_special_volume->viscosity_multiplier > 2.0
                        && object->water_depth_factor < 1.0) {
                    LevelOutOnSurface(object);
                }
            }
            if (object_info->field_0x4 & 0x1) {
                PositionChildren(object);
            }
        } else if (object_info != NULL && object_info->field_0x8 == 1) {
            MarkObjectAndChildrenAsPassive(object);
        }
    }
}

C2_MATCHING_INLINE void C2_HOOK_FASTCALL FlushQueuedAddsAndRemoves(void) {
    int i;

    for (i = 0; i < gPHIL_count_queued_objects; i++) {
        tPHIL_queued_object_info* queued_object_info;
        tPHIL_queued_header* object_info;

        queued_object_info = &gPhil_queued_objects.objects[i];
        if (queued_object_info->collision_info == NULL) {
            continue;
        }
        PHILAddObject(queued_object_info->collision_info);
        if (queued_object_info->flags & 0x4) {
            PHILSetPassiveObjectsMatrix(queued_object_info->collision_info, &queued_object_info->field_0x30);
        }
        if (queued_object_info->field_0x8 == 2) {
            br_vector3* omega;
            br_vector3* v;

            v = (queued_object_info->flags & 0x1) ? &queued_object_info->field_0x18 : NULL;
            omega = (queued_object_info->flags & 0x2) ? &queued_object_info->field_0x24 : NULL;
            PHILMakeObjectActive(queued_object_info->collision_info, v, omega, 0);
        }
        object_info = queued_object_info->collision_info->field_0x240;
        if (object_info != NULL) {
            object_info->field_0x4 &= ~0x2;
            object_info->field_0x4 |= queued_object_info->field_0x4 & ~0x2;
            object_info->field_0xc = queued_object_info->field_0xc;
            object_info->field_0x10 = queued_object_info->field_0x10;
            object_info->field_0x14 = queued_object_info->field_0x14;
        }
    }
    for (i = 0; i < gPHIL_count_queued_objects_for_removal; i++) {
        tPhysics_object* object;

        object = gPHIL_queued_objects_for_removal[i];
        if (object != NULL) {
            PHILRemoveObject(object);
        }
    }
}

C2_MATCHING_INLINE void C2_HOOK_FASTCALL PHILInterpolateObjects(tPhysics_object* pObjects, tU32 pTime) {
    tPhysics_object *object;
    float dt;

    dt = (gPHIL_last_physics_tick - pTime) * 0.001;
    if (!(dt <= .04 && dt >= .0f)) {
        dt = .0f;
    }
    gOver_shoot = dt > .0f;
    for (object = pObjects; object != NULL; object = object->next) {
        tPHIL_queued_object_info* object_info;

        object_info = object->field_0x240;
        if (object_info != NULL && object_info->field_0x8 == 2) {
            RepositionObjectGroup(object, -dt);
#ifndef CARPOCALYPSE2_MATCHING
            RefreshPedestrianAfterInterpolation(object);
#endif
        }
    }
}

#ifdef __vita__
/* spike breakdown for ApplyPhysicsToCars (car.c logs it) */
extern unsigned long long C2VitaGLTicksNS(void);
unsigned long long C2VitaPhysCollideNS, C2VitaPhysChangedNS;
unsigned long long C2VitaPhysPreNS, C2VitaPhysMungeNS, C2VitaPhysPostNS, C2VitaPhysFlushNS;
unsigned C2VitaPhysIterations;
#define C2_PHYS_T(acc, stmt) do { unsigned long long c2_t = C2VitaGLTicksNS(); stmt; acc += C2VitaGLTicksNS() - c2_t; } while (0)
#else
#define C2_PHYS_T(acc, stmt) stmt
#endif
// FUNCTION: CARMA2_HW 0x004b6630
void C2_HOOK_FASTCALL PHILDoPhysics(tPhysics_callbacks* pCallbacks, tU32 pLast_tick_time, tU32 pFrame_period) {
    tU32 now;
    int iteration;

    iteration = 0;
    if (gPHIL_enabled) {
        return;
    }
    now = pLast_tick_time + pFrame_period;
    gPHIL_original_activate_passive = pCallbacks->world_callbacks->activate_passive;
    pCallbacks->world_callbacks->activate_passive = PHILActivatePassive;
    gPHIL_callbacks = pCallbacks;
    if (gPHIL_last_physics_tick < pLast_tick_time) {
        gPHIL_last_physics_tick = 40 * (pLast_tick_time / 40);
    }
    if (now <= gPHIL_last_physics_tick) {
        ResetObjectList(gList_collision_infos);
        PHILInterpolateObjects(gList_collision_infos, now);
    } else {
        gCrush_deferred = 1;
        gPHIL_mechanics_time_sync = now - gPHIL_last_physics_tick;
        while (gPHIL_last_physics_tick < now && iteration < 5) {

            iteration += 1;
            if (pCallbacks->pre_collision != NULL) {
                C2_PHYS_T(C2VitaPhysPreNS, pCallbacks->pre_collision());
            }
            C2_PHYS_T(C2VitaPhysMungeNS, PHILMungeObjects(gList_collision_infos));
            gPHIL_munging_objects = 1;
#ifdef __vita__
            {
                unsigned long long c2_t0 = C2VitaGLTicksNS();
                ++C2VitaPhysIterations;
#endif
            if (gList_collision_infos != NULL) {
                DoCollisions(&gList_collision_infos, pCallbacks->world_callbacks);
            }
#ifdef __vita__
                C2VitaPhysCollideNS += C2VitaGLTicksNS() - c2_t0;
            }
#endif
            gPHIL_munging_objects = 0;
            if (pCallbacks->post_collision != NULL) {
                C2_PHYS_T(C2VitaPhysPostNS, pCallbacks->post_collision());
            }
            C2_PHYS_T(C2VitaPhysFlushNS, FlushQueuedAddsAndRemoves());
            gPHIL_last_physics_tick += 40;
            gPHIL_mechanics_time_sync -= 40;
            pLast_tick_time = gPHIL_mechanics_time_sync;
        }
        {
            tPhysics_object* object;
            float dt;

            dt = (gPHIL_last_physics_tick - now) * 0.001;
            if (!(dt <= .04 && dt >= .0f)) {
                dt = .0f;
            }
            gOver_shoot = dt > .0f;
            for (object = gList_collision_infos; object != NULL; object = object->next) {
                tPHIL_queued_object_info* object_info;

                object_info = object->field_0x240;
                if (object_info != NULL && object_info->field_0x8 == 2) {
                    RepositionObjectGroup(object, -dt);
                }
            }
        }
#ifdef __vita__
        {
            unsigned long long c2_t0 = C2VitaGLTicksNS();
#endif
        ChangedObjectsCallbacks(gList_collision_infos, pCallbacks, pFrame_period);
#ifdef __vita__
            C2VitaPhysChangedNS += C2VitaGLTicksNS() - c2_t0;
        }
#endif
        gPHIL_mechanics_time_sync = 1;
        gCrush_deferred = 0;
    }
    pCallbacks->world_callbacks->activate_passive = gPHIL_original_activate_passive;
}

// FUNCTION: CARMA2_HW 0x004978d0
int C2_HOOK_FASTCALL TimeToSendData(void) {
    // GLOBAL: CARMA2_HW 0x00659c30
    static tU32 last_physics_send_time = -1;

    if (gPHIL_last_physics_tick > last_physics_send_time && gPHIL_last_physics_tick < last_physics_send_time + 80) {
        return 0;
    }
    last_physics_send_time = gPHIL_last_physics_tick;
    return 1;
}

// FUNCTION: CARMA2_HW 0x004b71b0
void C2_HOOK_FASTCALL PHILActivatePassive(tPhysics_object* pObject, tPhysics_object* pActivator) {
    tCollision_info_owner* owner;

    owner = pObject->field_0x240;
    if (owner == NULL) {
        pObject->flags |= 0x1000;
        return;
    }
    if (owner->field_0x08 != 1) {
        pObject->flags |= 0x1000;
        return;
    }

    if (gPHIL_original_activate_passive != NULL) {
        gPHIL_original_activate_passive(pObject, pActivator);
    }

    if (gPHIL_callbacks->passive_activated != NULL) {
        /* retail 0x4b71df: the activator (edx) is passed on; ped collision damage needs it */
        if (gPHIL_callbacks->passive_activated(pObject, pActivator)) {
            PHILMakeObjectActive(pObject, NULL, NULL, 1);
        }
    } else {
        PHILMakeObjectActive(pObject, NULL, NULL, 1);
    }
}

// FUNCTION: CARMA2_HW 0x004b7220
void C2_HOOK_FASTCALL PHILSetObjectMatrix(tPhysics_object* pObject, br_matrix34* pMat) {

    BrMatrix34Copy(&pObject->transform_matrix, pMat);
    BrMatrix34Copy(&pObject->actor->t.t.mat, pMat);
}

// FUNCTION: CARMA2_HW 0x004b7250
float C2_HOOK_FASTCALL PhysicsObjectWeightedSquareSum(tPhysics_object* pObject) {
    float sum;
    int i;

    sum = gFLOAT_0058a768;
    for (i = 0; i < 3; i++) {
        sum += BR_MUL(pObject->M, BR_SQR(pObject->v.v[i])) + BR_MUL(pObject->I.v[i], BR_SQR(pObject->omega.v[i]));
    }
    return sum;
}

// FUNCTION: CARMA2_HW 0x004b7290
float C2_HOOK_FASTCALL PhysicsObjectsWeightedSquareSum(tPhysics_object* pObject) {
    float sum;
    float object_sum;

    sum = 0.0f;
    while (pObject != NULL) {
        object_sum = PhysicsObjectWeightedSquareSum(pObject);
        sum += object_sum;
        sum += PhysicsObjectsWeightedSquareSum(pObject->child);
        pObject = pObject->next;
    }
    return sum;
}
#pragma auto_inline(off)

void* C2_HOOK_FASTCALL PhysicsScratchSpaceCheck(int pSize);
void C2_HOOK_FASTCALL InitializeCollisionContact(tPhysics_object* pObject1, tPhysics_object* pObject2, tCollision_contact* pContact, br_vector3* pNormal);

// C2PORT-NONMATCHING: best 27.23% | difftest PASS 2000 trials (spec difftest/specs/004b8350_FUN_004b8350.json) | reason VC5 stack/register allocation
// FUNCTION: CARMA2_HW 0x004b8350
int C2_HOOK_FASTCALL FUN_004b8350(tPhysics_object* pObject, tCollision_contact* pContacts) {
    int count;
    int local_58;
    float local_54[9];
    float inverse[12];
    float fVar1;
    float fVar4;
    float fVar5;
    float* axis_component;
    tPhysics_object* other;
    tCollision_contact* contact;
    br_matrix34* object_matrix;
    int i;

    count = 0;
    if (pObject->physics_joint1 != NULL && pObject->physics_joint1->type != eJoint_none && (pObject->flags & 0x400) == 0) {
        other = pObject->field_0x230;
        if (pObject->physics_joint1->type == eJoint_quick_hinge) {
            InitializeCollisionContact(pObject, other, pContacts, &pObject->physics_joint1->hinge_axis2);
            pContacts++;
            InitializeCollisionContact(pObject, other, pContacts, &pObject->physics_joint1->hinge_axis3);
            pContacts++;
            count = 2;
        } else {
            axis_component = &local_54[0];
            local_58 = 3;
            count = 3;
            do {
                local_54[0] = 0.f;
                local_54[1] = 0.f;
                local_54[2] = 0.f;
                *axis_component = 1.f;
                InitializeCollisionContact(pObject, other, pContacts, (br_vector3*)local_54);
                pContacts++;
                axis_component++;
                local_58--;
            } while (local_58 != 0);
        }

        switch (pObject->physics_joint1->type) {
        case eJoint_hinge:
            object_matrix = &pObject->transform_matrix;
            contact = pContacts;
            contact->impulse = 0.f;
            contact->object1 = pObject;
            contact->object2 = other;
            contact->r1.v[0] = pObject->physics_joint1->field_0x08.v[0] - pObject->cmpos.v[0];
            contact->r1.v[1] = pObject->physics_joint1->field_0x08.v[1] - pObject->cmpos.v[1];
            contact->r1.v[2] = pObject->physics_joint1->field_0x08.v[2] - pObject->cmpos.v[2];
            contact->r2.v[0] = pObject->physics_joint1->field_0x14.v[0] - other->cmpos.v[0];
            contact->r2.v[1] = pObject->physics_joint1->field_0x14.v[1] - other->cmpos.v[1];
            contact->r2.v[2] = pObject->physics_joint1->field_0x14.v[2] - other->cmpos.v[2];
            contact->normal.v[0] = pObject->physics_joint1->hinge_axis2.v[0];
            contact->normal.v[1] = pObject->physics_joint1->hinge_axis2.v[1];
            contact->normal.v[2] = pObject->physics_joint1->hinge_axis2.v[2];
            BrMatrix34ApplyV((br_vector3*)local_54, &contact->normal, object_matrix);
            BrMatrix34TApplyV(&contact->field_0x38, (br_vector3*)local_54, &other->transform_matrix);
            contact->type = 0xb;
            pContacts++;

            contact = pContacts;
            contact->impulse = 0.f;
            contact->object1 = pObject;
            contact->object2 = other;
            contact->r1.v[0] = pObject->physics_joint1->field_0x08.v[0] - pObject->cmpos.v[0];
            contact->r1.v[1] = pObject->physics_joint1->field_0x08.v[1] - pObject->cmpos.v[1];
            contact->r1.v[2] = pObject->physics_joint1->field_0x08.v[2] - pObject->cmpos.v[2];
            contact->r2.v[0] = pObject->physics_joint1->field_0x14.v[0] - other->cmpos.v[0];
            contact->r2.v[1] = pObject->physics_joint1->field_0x14.v[1] - other->cmpos.v[1];
            contact->r2.v[2] = pObject->physics_joint1->field_0x14.v[2] - other->cmpos.v[2];
            contact->normal.v[0] = pObject->physics_joint1->hinge_axis3.v[0];
            contact->normal.v[1] = pObject->physics_joint1->hinge_axis3.v[1];
            contact->normal.v[2] = pObject->physics_joint1->hinge_axis3.v[2];
            BrMatrix34ApplyV((br_vector3*)local_54, &contact->normal, object_matrix);
            BrMatrix34TApplyV(&contact->field_0x38, (br_vector3*)local_54, &other->transform_matrix);
            contact->type = 0xb;
            pContacts++;
            count += 2;
            break;

        case eJoint_universal:
            object_matrix = &pObject->transform_matrix;
            contact = pContacts;
            contact->impulse = 0.f;
            contact->object1 = pObject;
            contact->object2 = other;
            contact->r1.v[0] = pObject->physics_joint1->field_0x08.v[0] - pObject->cmpos.v[0];
            contact->r1.v[1] = pObject->physics_joint1->field_0x08.v[1] - pObject->cmpos.v[1];
            contact->r1.v[2] = pObject->physics_joint1->field_0x08.v[2] - pObject->cmpos.v[2];
            contact->r2.v[0] = pObject->physics_joint1->field_0x14.v[0] - other->cmpos.v[0];
            contact->r2.v[1] = pObject->physics_joint1->field_0x14.v[1] - other->cmpos.v[1];
            contact->r2.v[2] = pObject->physics_joint1->field_0x14.v[2] - other->cmpos.v[2];
            contact->normal.v[0] = pObject->physics_joint1->hinge_axis2.v[0];
            contact->normal.v[1] = pObject->physics_joint1->hinge_axis2.v[1];
            contact->normal.v[2] = pObject->physics_joint1->hinge_axis2.v[2];
            BrMatrix34ApplyV((br_vector3*)local_54, &contact->normal, object_matrix);
            BrMatrix34TApplyV(&contact->field_0x38, (br_vector3*)local_54, &other->transform_matrix);
            contact->type = 0xb;
            pContacts++;
            count++;
            break;

        case eJoint_ball_n_socket:
            break;

        case eJoint_quick_hinge: {
            tPhysics_joint_data* joint_data;

            joint_data = (tPhysics_joint_data*)PhysicsScratchSpaceCheck(0xd0);
            IncreasePhysicsScratchPointer(0xd0);
            pObject->field_0x1e4 = (int)(uintptr_t)joint_data;
            other->field_0x1e4 = (int)(uintptr_t)((tU8*)joint_data + 0x68);
            *(int*)(uintptr_t)pObject->field_0x1e4 = 0;
            *(tPhysics_object**)(uintptr_t)(pObject->field_0x1e4 + 4) = other;
            *(int*)(uintptr_t)other->field_0x1e4 = 0;
            *(tPhysics_object**)(uintptr_t)(other->field_0x1e4 + 4) = pObject;

            BrMatrix34LPInverse((br_matrix34*)inverse, &pObject->transform_matrix);
            BrMatrix34Mul((br_matrix34*)(uintptr_t)(pObject->field_0x1e4 + 0x38), &other->transform_matrix, (br_matrix34*)inverse);
            BrMatrix34LPInverse((br_matrix34*)(uintptr_t)(other->field_0x1e4 + 0x38), (br_matrix34*)(uintptr_t)(pObject->field_0x1e4 + 0x38));
            *(int*)(uintptr_t)pObject->field_0x1e4 = 1;
            *(int*)(uintptr_t)other->field_0x1e4 = 1;
            FUN_004b8cf0(pObject);

            fVar1 = pObject->physics_joint1->parent_bone_axis.v[1] * other->omega.v[1]
                    + other->omega.v[0] * pObject->physics_joint1->parent_bone_axis.v[0]
                    + pObject->physics_joint1->parent_bone_axis.v[2] * other->omega.v[2];
            local_54[0] = other->omega.v[0] - pObject->physics_joint1->parent_bone_axis.v[0] * fVar1;
            local_54[1] = other->omega.v[1] - pObject->physics_joint1->parent_bone_axis.v[1] * fVar1;
            local_54[2] = other->omega.v[2] - pObject->physics_joint1->parent_bone_axis.v[2] * fVar1;
            BrMatrix34ApplyV((br_vector3*)&local_54[3], (br_vector3*)local_54, &other->transform_matrix);
            BrMatrix34TApplyV((br_vector3*)local_54, (br_vector3*)&local_54[3], &pObject->transform_matrix);

            fVar4 = pObject->physics_joint1->hinge_axis.v[1] * pObject->omega.v[1]
                    + pObject->physics_joint1->hinge_axis.v[2] * pObject->omega.v[2]
                    + pObject->physics_joint1->hinge_axis.v[0] * pObject->omega.v[0];
            local_54[0] += pObject->physics_joint1->hinge_axis.v[0] * fVar4;
            local_54[1] += pObject->physics_joint1->hinge_axis.v[1] * fVar4;
            local_54[2] += pObject->physics_joint1->hinge_axis.v[2] * fVar4;
            pObject->omega.v[0] = local_54[0];
            pObject->omega.v[1] = local_54[1];
            pObject->omega.v[2] = local_54[2];

            fVar1 = pObject->physics_joint1->field_0x14.v[0] - other->cmpos.v[0];
            fVar4 = pObject->physics_joint1->field_0x14.v[1] - other->cmpos.v[1];
            fVar5 = pObject->physics_joint1->field_0x14.v[2] - other->cmpos.v[2];
            local_54[3] = other->omega.v[1] * fVar5 - other->omega.v[2] * fVar4;
            local_54[4] = other->omega.v[2] * fVar1 - other->omega.v[0] * fVar5;
            local_54[5] = other->omega.v[0] * fVar4 - other->omega.v[1] * fVar1;
            BrMatrix34ApplyV((br_vector3*)local_54, (br_vector3*)&local_54[3], &other->transform_matrix);
            local_54[0] += other->v.v[0];
            local_54[1] += other->v.v[1];
            local_54[2] += other->v.v[2];
            BrMatrix34TApplyV((br_vector3*)&local_54[6], (br_vector3*)local_54, &pObject->transform_matrix);

            fVar1 = pObject->physics_joint1->field_0x08.v[0] - pObject->cmpos.v[0];
            fVar4 = pObject->physics_joint1->field_0x08.v[1] - pObject->cmpos.v[1];
            fVar5 = pObject->physics_joint1->field_0x08.v[2] - pObject->cmpos.v[2];
            local_54[3] = pObject->omega.v[1] * fVar5 - pObject->omega.v[2] * fVar4;
            local_54[4] = pObject->omega.v[2] * fVar1 - pObject->omega.v[0] * fVar5;
            local_54[5] = pObject->omega.v[0] * fVar4 - pObject->omega.v[1] * fVar1;
            local_54[6] -= local_54[3];
            local_54[7] -= local_54[4];
            local_54[8] -= local_54[5];
            local_54[2] = (pObject->physics_joint1->hinge_axis.v[1] * local_54[7]
                    + pObject->physics_joint1->hinge_axis.v[0] * local_54[6]
                    + pObject->physics_joint1->hinge_axis.v[2] * local_54[8])
                    - (pObject->physics_joint1->hinge_axis.v[1] * pObject->velocity_car_space.v[1]
                    + pObject->physics_joint1->hinge_axis.v[0] * pObject->velocity_car_space.v[0]
                    + pObject->physics_joint1->hinge_axis.v[2] * pObject->velocity_car_space.v[2]);
            local_54[0] = pObject->physics_joint1->hinge_axis.v[0] * local_54[2];
            local_54[1] = pObject->physics_joint1->hinge_axis.v[1] * local_54[2];
            local_54[2] = pObject->physics_joint1->hinge_axis.v[2] * local_54[2];
            pObject->velocity_car_space.v[0] += local_54[0];
            pObject->velocity_car_space.v[1] += local_54[1];
            pObject->velocity_car_space.v[2] += local_54[2];
            BrMatrix34ApplyV(&pObject->v, &pObject->velocity_car_space, &pObject->transform_matrix);
            break;
        }

        default:
            PhysicsError((tPhysicsError)5, NULL);
        }
    }

    if (pObject->physics_joint2 != NULL) {
        if (pObject->physics_joint2->type == eJoint_translation) {
            contact = pContacts;
            contact->impulse = 0.f;
            contact->object1 = pObject;
            contact->object2 = NULL;
            contact->field_0xc = NULL;
            contact->normal.v[0] = pObject->physics_joint2->hinge_axis2.v[0];
            contact->normal.v[1] = pObject->physics_joint2->hinge_axis2.v[1];
            contact->normal.v[2] = pObject->physics_joint2->hinge_axis2.v[2];
            contact->type = 10;
            pContacts++;
            count++;

            contact = pContacts;
            contact->impulse = 0.f;
            contact->object1 = pObject;
            contact->object2 = NULL;
            contact->field_0xc = NULL;
            contact->normal.v[0] = pObject->physics_joint2->hinge_axis3.v[0];
            contact->normal.v[1] = pObject->physics_joint2->hinge_axis3.v[1];
            contact->normal.v[2] = pObject->physics_joint2->hinge_axis3.v[2];
            contact->type = 10;
            pContacts++;
            count++;
        } else {
            count += 3;
            i = 0;
            do {
                contact = pContacts++;
                contact->impulse = 0.f;
                contact->object1 = pObject;
                contact->object2 = NULL;
                contact->field_0xc = NULL;
                contact->r1.v[0] = pObject->physics_joint2->field_0x08.v[0] - pObject->cmpos.v[0];
                contact->r1.v[1] = pObject->physics_joint2->field_0x08.v[1] - pObject->cmpos.v[1];
                contact->r1.v[2] = pObject->physics_joint2->field_0x08.v[2] - pObject->cmpos.v[2];
                contact->normal.v[0] = 0.f;
                contact->normal.v[1] = 0.f;
                contact->normal.v[2] = 0.f;
                contact->normal.v[i] = 1.f;
                contact->type = 10;
                i++;
            } while (i < 3);

            switch (pObject->physics_joint2->type) {
            case eJoint_hinge:
            case eJoint_ball_n_socket:
                break;

            case eJoint_universal:
                contact = pContacts;
                contact->impulse = 0.f;
                contact->object1 = pObject;
                contact->object2 = NULL;
                contact->field_0xc = NULL;
                contact->r1.v[0] = pObject->physics_joint2->field_0x08.v[0] - pObject->cmpos.v[0];
                contact->r1.v[1] = pObject->physics_joint2->field_0x08.v[1] - pObject->cmpos.v[1];
                contact->r1.v[2] = pObject->physics_joint2->field_0x08.v[2] - pObject->cmpos.v[2];
                contact->normal.v[0] = pObject->physics_joint2->hinge_axis2.v[0];
                contact->normal.v[1] = pObject->physics_joint2->hinge_axis2.v[1];
                contact->normal.v[2] = pObject->physics_joint2->hinge_axis2.v[2];
                contact->type = 0xb;
                pContacts++;
                count++;
                break;

            default:
                PhysicsError((tPhysicsError)5, NULL);
            }
        }
    }

    for (other = pObject->child; other != NULL; other = other->next) {
        int child_count;

        child_count = FUN_004b8350(other, pContacts);
        count += child_count;
        pContacts += child_count;
    }
    return count;
}

#pragma auto_inline(on)
#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b8c20
void C2_HOOK_FASTCALL InitializeCollisionContact(tPhysics_object* pObject1, tPhysics_object* pObject2, tCollision_contact* pContact, br_vector3* pNormal) {
    br_vector3 normal;

    pContact->impulse = 0.f;
    pContact->object1 = pObject1;
    pContact->object2 = pObject2;
    pContact->field_0xc = NULL;
    pContact->r1.v[0] = pObject1->physics_joint1->field_0x08.v[0] - pObject1->cmpos.v[0];
    pContact->r1.v[1] = pObject1->physics_joint1->field_0x08.v[1] - pObject1->cmpos.v[1];
    pContact->r1.v[2] = pObject1->physics_joint1->field_0x08.v[2] - pObject1->cmpos.v[2];
    pContact->r2.v[0] = pObject1->physics_joint1->field_0x14.v[0] - pObject2->cmpos.v[0];
    pContact->r2.v[1] = pObject1->physics_joint1->field_0x14.v[1] - pObject2->cmpos.v[1];
    pContact->r2.v[2] = pObject1->physics_joint1->field_0x14.v[2] - pObject2->cmpos.v[2];
    pContact->normal.v[0] = pNormal->v[0];
    pContact->normal.v[1] = pNormal->v[1];
    pContact->normal.v[2] = pNormal->v[2];
    BrMatrix34ApplyV(&normal, pNormal, &pObject1->transform_matrix);
    BrMatrix34TApplyV(&pContact->field_0x38, &normal, &pObject2->transform_matrix);
    pContact->type = 10;
}
#pragma auto_inline(on)

// GLOBAL: CARMA2_HW 0x006793d0
int gUNK_006793d0;

/* Local declarations: physics.c does not include finteray.h so that header can
 * stay untouched. Note CheckSingleFace takes normal before rt (differs from
 * finteray.h). */
void C2_HOOK_FASTCALL CheckSingleFace(tFace_ref* pFace, br_vector3* ray_pos, br_vector3* ray_dir, br_vector3* normal, br_scalar* rt, br_vector3* coll_pos);
void C2_HOOK_FASTCALL EnablePlingMaterials(void);
void C2_HOOK_FASTCALL DisablePlingMaterials(void);

// GLOBAL: CARMA2_HW 0x005896d4
br_scalar gFloat_005896d4 = 0.14492753148078918f;

// C2PORT-NONMATCHING: best 59.52% | difftest PASS 2000 trials (spec difftest/specs/0041e3f0_FUN_0041e3f0.json) | reason x87-scheduling
// FUNCTION: CARMA2_HW 0x0041e3f0
void C2_HOOK_FASTCALL FUN_0041e3f0(br_vector3* pRay_pos, br_vector3* pRay_dir, br_vector3* pHit_pos, br_scalar* pT, void* pRay_context) {
    br_vector3 scaled_ray_pos;
    br_vector3 scaled_ray_dir;

    scaled_ray_pos.v[0] = pRay_pos->v[0] * gFloat_005896d4;
    scaled_ray_dir.v[0] = pRay_dir->v[0] * gFloat_005896d4;
    scaled_ray_pos.v[1] = pRay_pos->v[1] * gFloat_005896d4;
    scaled_ray_pos.v[2] = pRay_pos->v[2] * gFloat_005896d4;
    scaled_ray_dir.v[1] = pRay_dir->v[1] * gFloat_005896d4;
    scaled_ray_dir.v[2] = pRay_dir->v[2] * gFloat_005896d4;
    RayCastInBox(&scaled_ray_pos, &scaled_ray_dir, pHit_pos, pT, *(tPhysics_object**)((tU8*)pRay_context + 8));
}

#pragma auto_inline(off)

// FUNCTION: CARMA2_HW 0x0041e480
int C2_HOOK_FASTCALL RayCastInBox(br_vector3* pRay_pos, br_vector3* pRay_dir, br_vector3* pHit_pos, br_scalar* pT, tPhysics_object* pObject) {
    br_vector3 normal;
    br_vector3 coll_pos;
    br_scalar t;
    int i;
    int best;

    *pT = 2.f;
    for (i = pObject->box_face_start; i < pObject->box_face_end; i++) {
        if (gUNK_006793d0 != 0 && (gFace_list__car[i].flags & 0x80) != 0) {
            continue;
        }
        CheckSingleFace(&gFace_list__car[i], pRay_pos, pRay_dir, &normal, &t, &coll_pos);
        if (t < *pT) {
            *pT = t;
            best = i;
            *pHit_pos = normal;
        }
    }
    if (*pT < 2.f) {
        int c;

        c = gFace_list__car[best].material->identifier[0] - '/';
        if (c >= 0 && c < 11) {
            return c;
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0041e570
void C2_HOOK_FASTCALL FUN_0041e570(void) {

}

#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004ff5d0
void C2_HOOK_FASTCALL TestAutoSpecialVolume(tPhysics_object* pObject) {
    br_scalar d;
    br_scalar d_neg;
    br_scalar d_pos;
    br_scalar t1;
    br_scalar t2;
    tSpecial_volume* def_vol;
    br_vector3 hit;
    br_vector3 world_min;
    br_vector3 q;
    br_vector3 dir;
    int k;

    d = 0.f;
    for (k = 0; k < 3; k++) {
        d += BR_MAC3(pObject->transform_matrix.m[k][0], pObject->water_normal.v[0],
                     pObject->transform_matrix.m[k][1], pObject->water_normal.v[1],
                     pObject->transform_matrix.m[k][2], pObject->water_normal.v[2])
             * pObject->bb2.min.v[k];
    }
    d += BR_MAC3(pObject->transform_matrix.m[3][0], pObject->water_normal.v[0],
                 pObject->transform_matrix.m[3][1], pObject->water_normal.v[1],
                 pObject->transform_matrix.m[3][2], pObject->water_normal.v[2]);
    d_pos = d;
    d_neg = d;
    for (k = 0; k < 3; k++) {
        t1 = BR_MAC3(pObject->transform_matrix.m[k][0], pObject->water_normal.v[0],
                     pObject->transform_matrix.m[k][1], pObject->water_normal.v[1],
                     pObject->transform_matrix.m[k][2], pObject->water_normal.v[2])
             * (pObject->bb2.max.v[k] - pObject->bb2.min.v[k]);
        if (t1 < 0.f) {
            d_neg += t1;
        } else {
            d_pos += t1;
        }
    }
    if (d_neg >= pObject->water_d) {
        pObject->auto_special_volume = NULL;
        pObject->water_depth_factor = 1.f;
        return;
    }
    if (pObject->water_d >= d_pos - 0.001) {
        pObject->water_depth_factor = 1.f;
    } else {
        pObject->water_depth_factor = (pObject->water_d - d_neg) / (d_pos - d_neg);
    }
    if (pObject->auto_special_volume != NULL) {
        return;
    }
    def_vol = gDefault_water_spec_vol_real;
    if (def_vol != NULL) {
        BrMatrix34ApplyP(&world_min, &pObject->bb2.min, &pObject->transform_matrix);
        BrVector3Copy(&q, &world_min);
        for (k = 0; k < 3; k++) {
            hit.v[0] = (pObject->bb2.max.v[k] - pObject->bb2.min.v[k]) * pObject->transform_matrix.m[k][0];
            hit.v[1] = (pObject->bb2.max.v[k] - pObject->bb2.min.v[k]) * pObject->transform_matrix.m[k][1];
            hit.v[2] = (pObject->bb2.max.v[k] - pObject->bb2.min.v[k]) * pObject->transform_matrix.m[k][2];
            if (BrVector3Dot(&pObject->water_normal, &hit) > 0.f) {
                BrVector3Add(&q, &q, &hit);
            } else {
                BrVector3Add(&world_min, &world_min, &hit);
            }
        }
        BrVector3Sub(&dir, &world_min, &q);
        DisablePlingMaterials();
        RayCastInBox(&q, &dir, &hit, &t1, pObject);
        EnablePlingMaterials();
        RayCastInBox(&q, &dir, &hit, &t2, pObject);
        if (t1 < t2) {
            pObject->auto_special_volume = def_vol;
            return;
        }
    }
    pObject->water_depth_factor = 1.f;
    pObject->auto_special_volume = NULL;
}

// FUNCTION: CARMA2_HW 0x004ff410
void C2_HOOK_FASTCALL MungeSpecialVolume(tPhysics_object* pObject) {
    tSpecial_volume* new_vol;
    tCar_spec* sv;

    new_vol = FindSpecialVolume(&pObject->pos, pObject->last_special_volume, 0);

    sv = NULL;
    if (pObject != NULL && pObject->owner != NULL && pObject->flags_0x238 == 1) {
        sv = (tCar_spec*)pObject->owner;
    } else {
        sv = NULL;
    }

    if (pObject->auto_special_volume != NULL) {
        if (new_vol == NULL || new_vol->gravity_multiplier == 1.0f) {
            if (pObject->water_d == 10000.0f && pObject->water_depth_factor != 1.0f) {
                pObject->auto_special_volume = NULL;
            } else {
                new_vol = pObject->auto_special_volume;
            }
        }
    }

    if (new_vol != pObject->last_special_volume && sv != NULL && sv->driver == eDriver_local_human) {
        if (pObject->last_special_volume != NULL && pObject->last_special_volume->exit_noise >= 0
                && (new_vol == NULL || new_vol->exit_noise != pObject->last_special_volume->exit_noise)) {
            DRS3StartSound(gXXX_outlet, pObject->last_special_volume->exit_noise);
        }
        if (new_vol != NULL && new_vol->entry_noise >= 0
                && (pObject->last_special_volume == NULL || pObject->last_special_volume->entry_noise != new_vol->entry_noise)) {
            DRS3StartSound(gXXX_outlet, new_vol->entry_noise);
        }
    }

    pObject->last_special_volume = new_vol;

    if (sv != NULL && new_vol != NULL) {
        br_scalar gravity_multiplier = new_vol->gravity_multiplier;
        if (gravity_multiplier < 1.0f) {
            if (IsCarSmoking(sv) != 0) {
                StopCarSmoking(sv);
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004b6ec0
void C2_HOOK_FASTCALL ProcessGravity(tPHIL_queued_header* pObject_info, tPhysics_object* pObject, float pGravity) {
    br_vector3 gravity;
    tSpecial_volume* pSpecial_volume;

    pSpecial_volume = NULL;
    if (pObject_info->field_0x4 & 0x8) {
        pSpecial_volume = pObject->last_special_volume;
    }
    if (pSpecial_volume != NULL) {
        if (pObject_info->field_0x14 == 0.0f || pSpecial_volume->viscosity_multiplier <= 2.0) {
            gravity.v[0] = 0.0f;
            gravity.v[2] = 0.0f;
            gravity.v[1] = pSpecial_volume->gravity_multiplier * pObject->water_depth_factor * pGravity * gGravity_multiplier * -0.05681159420289855;
        } else {
            gravity.v[0] = 0.0f;
            gravity.v[1] = ((1.0f - pSpecial_volume->gravity_multiplier) * pObject->water_depth_factor * pObject_info->field_0x14 - gGravity_multiplier * 1.4202898550724639 * pGravity) * 0.04;
            gravity.v[2] = 0.0f;
        }
    } else {
        gravity.v[0] = 0.0f;
        gravity.v[2] = 0.0f;
        gravity.v[1] = pGravity * gGravity_multiplier * -0.05681159420289855;
    }
    PhysicsObjectSetImpulse(pObject, &gravity);
    PhysicsObjectMoveVelocity(pObject);
}

// C2PORT-NONMATCHING: best 25.00% | difftest PASS 2000 trials (spec difftest/specs/004c2c30_PhysicsObjectApplyTorque.json) | reason VC5 x87 evaluation order and spills differ from the original
// FUNCTION: CARMA2_HW 0x004c2c30
void C2_HOOK_FASTCALL PhysicsObjectApplyTorque(tPhysics_object* pObject, const br_vector3* pTorque) {
    pObject->omega.v[0] += (pTorque->v[0] * 0.0210039914f) / pObject->I.v[0];
    pObject->omega.v[1] += (pTorque->v[1] * 0.0210039914f) / pObject->I.v[1];
    pObject->omega.v[2] += (pTorque->v[2] * 0.0210039914f) / pObject->I.v[2];
}

// FUNCTION: CARMA2_HW 0x004c2c90
void C2_HOOK_FASTCALL ApplyWaterOmegaBrake(tPhysics_object* pObject, const br_vector3* pV) {
    br_scalar b0;
    br_scalar b1;
    br_scalar b2;
    br_vector3 om = pObject->omega;

    b0 = (pV->v[0] * 0.0210039914f) / pObject->I.v[0];
    b1 = (pV->v[1] * 0.0210039914f) / pObject->I.v[1];
    b2 = (pV->v[2] * 0.0210039914f) / pObject->I.v[2];

    pObject->omega.v[0] -= b0;
    pObject->omega.v[1] -= b1;
    pObject->omega.v[2] -= b2;

    if (om.v[0] * pObject->omega.v[0] <= 0.0f) {
        pObject->omega.v[0] = 0.0f;
    }
    if (om.v[1] * pObject->omega.v[1] <= 0.0f) {
        pObject->omega.v[1] = 0.0f;
    }
    if (om.v[2] * pObject->omega.v[2] <= 0.0f) {
        pObject->omega.v[2] = 0.0f;
    }
}

// FUNCTION: CARMA2_HW 0x004c2d60
int C2_HOOK_FASTCALL FUN_004c2d60(tPhysics_object* pObject, tWorld_callbacks* pCar_callbacks) {
    br_vector3 normal;
    tPhysics_object** next_link;
    int next;
    int count;

    count = 0;
    next_link = &pObject->next;
    next = (int)*next_link;
    while (next != 0) {
        next = TestForObjectInSensiblePlace(pObject, *next_link, &normal, pCar_callbacks);
        if (next == 0) {
            count += 1;
        }
        pObject = *next_link;
        next_link = &pObject->next;
        next = (int)*next_link;
    }
    return count;
}

// FUNCTION: CARMA2_HW 0x004b6ce0
void C2_HOOK_FASTCALL ProcessDrag2(tPHIL_queued_header* pObject_info, tPhysics_object* pObject, float pDrag, int pObject_info_flags, tSpecial_volume* pSpecial_volume) {
    br_scalar drag;
    br_scalar s;
    tSpecial_volume* sp;
    br_vector3 v;

    drag = pDrag * 2e-05;
    sp = NULL;
    if (pObject_info_flags & 8) {
        sp = pSpecial_volume;
    }
    if (sp != NULL) {
        if (pObject_info->field_0x14 == 0.0f || sp->viscosity_multiplier <= 2.0) {
            drag = sp->viscosity_multiplier * pObject->water_depth_factor * drag;
        } else {
            drag = pObject->water_depth_factor * sp->viscosity_multiplier * drag * 0.5;
        }
    }

    s = BrVector3Length(&pObject->v);
    v.v[0] = pObject->v.v[0] * (s * drag * 6.9 / pObject->M);
    v.v[1] = pObject->v.v[1] * (s * drag * 6.9 / pObject->M);
    v.v[2] = pObject->v.v[2] * (s * drag * 6.9 / pObject->M);
    DRVector3Diminish(&pObject->v, &v);

    s = BrVector3Length(&pObject->omega);
    v.v[0] = pObject->omega.v[0] * (s * drag);
    v.v[1] = pObject->omega.v[1] * (s * drag);
    v.v[2] = pObject->omega.v[2] * (s * drag);
    ApplyWaterOmegaBrake(pObject, &v);

    for (pObject = pObject->child; pObject != NULL; pObject = pObject->next) {
        ProcessDrag2(pObject_info, pObject, pDrag, pObject_info_flags, pSpecial_volume);
    }
}

void C2_HOOK_FASTCALL ProcessDrag(tPHIL_queued_header* pObject_info, tPhysics_object* pObject, float pDrag) {

    ProcessDrag2(pObject_info, pObject, pDrag, pObject_info->field_0x4, pObject->last_special_volume);
}

// C2PORT-NONMATCHING: best 40.8% | difftest PASS 4000 trials (specs 004b6fb0_LevelOutOnSurface.json, 004b6fb0_LevelOutOnSurface_PARALLEL.json) | reason x87 math scheduling
// FUNCTION: CARMA2_HW 0x004b6fb0
void C2_HOOK_FASTCALL LevelOutOnSurface(tPhysics_object *pObject) {
    br_scalar v0;
    br_scalar v1;
    br_scalar v2;
    br_scalar dot;
    br_scalar p1;
    br_scalar tv;
    br_scalar px;
    br_scalar py;
    br_scalar pz;
    br_vector3 n;
    br_vector3 u;

    if (pObject->I.v[0] > pObject->I.v[1] && pObject->I.v[0] > pObject->I.v[2]) {
        v0 = pObject->actor->t.t.mat.m[0][0];
        v1 = pObject->actor->t.t.mat.m[0][1];
        v2 = pObject->actor->t.t.mat.m[0][2];
    } else if (pObject->I.v[1] > pObject->I.v[2] && pObject->I.v[1] > pObject->I.v[0]) {
        v0 = pObject->actor->t.t.mat.m[1][0];
        v1 = pObject->actor->t.t.mat.m[1][1];
        v2 = pObject->actor->t.t.mat.m[1][2];
    } else {
        v0 = pObject->actor->t.t.mat.m[2][0];
        v1 = pObject->actor->t.t.mat.m[2][1];
        v2 = pObject->actor->t.t.mat.m[2][2];
    }

    n.v[0] = -1.0f * v0;
    n.v[1] = -1.0f * v1;
    n.v[2] = -1.0f * v2;

    dot = n.v[0] * pObject->water_normal.v[0] + n.v[1] * pObject->water_normal.v[1] + n.v[2] * pObject->water_normal.v[2];

    if (dot > -0.99) {
        px = pObject->water_normal.v[1] * n.v[2] - pObject->water_normal.v[2] * n.v[1];
        py = pObject->water_normal.v[2] * n.v[0] - pObject->water_normal.v[0] * n.v[2];
        pz = pObject->water_normal.v[0] * n.v[1] - pObject->water_normal.v[1] * n.v[0];
        p1 = dot + 1.0f;
        tv = sqrtf(px * px + py * py + pz * pz);
        u.v[0] = px * (p1 / tv);
        tv = sqrtf(pz * pz + py * py + u.v[0] * u.v[0]);
        u.v[1] = py * (p1 / tv);
        tv = sqrtf(u.v[1] * u.v[1] + pz * pz + u.v[0] * u.v[0]);
        u.v[2] = pz * (p1 / tv);
        BrMatrix34TApplyV(&n, &u, &pObject->actor->t.t.mat);
        pObject->omega.v[0] += n.v[0];
        pObject->omega.v[1] += n.v[1];
        pObject->omega.v[2] += n.v[2];
    }
}

// FUNCTION: CARMA2_HW 0x004b6e90
void C2_HOOK_FASTCALL MarkObjectAndChildrenAsPassive(tPhysics_object* pObject) {
    tPhysics_object *child;

    pObject->field_0xf0 = 2;
    for (child = pObject->child; child != NULL; child = child->next) {
        MarkObjectAndChildrenAsPassive(child);
    }
}

// GLOBAL: CARMA2_HW 0x0058a8b8
const br_scalar gFLOAT_0058a8b8 = 0.0f;

// GLOBAL: CARMA2_HW 0x0058a8c0
const double gDOUBLE_0058a8c0 = 0.0;

// GLOBAL: CARMA2_HW 0x0058a8e0
const double gDOUBLE_0058a8e0 = 0.0001;
// GLOBAL: CARMA2_HW 0x0058a8e8
const float gFLOAT_0058a8e8 = 100000.0f;
// GLOBAL: CARMA2_HW 0x0058a8ec
const float gFLOAT_0058a8ec = -0.0001f;
// GLOBAL: CARMA2_HW 0x0058a8f0
const double gDOUBLE_0058a8f0 = -0.001;
// GLOBAL: CARMA2_HW 0x0058a8f8
const double gDOUBLE_0058a8f8 = 0.01;
// GLOBAL: CARMA2_HW 0x0058a900
const double gDOUBLE_0058a900 = -0.01;
// GLOBAL: CARMA2_HW 0x006940b8
int gDAT_006940b8;

// C2PORT-NONMATCHING: best 15.34% | difftest PASS 2000 trials (spec difftest/specs/004c3b60_FUN_004c3b60.json) | reason VC5 stack layout and register allocation
// FUNCTION: CARMA2_HW 0x004c3b60
undefined4 C2_HOOK_FASTCALL FUN_004c3b60(float* param_1, float* param_2, float* param_3, int param_4, undefined4 param_5, int param_6, int* param_7, int param_8, undefined4 param_9, undefined4 param_10, undefined4 param_11) {
  tU8 *pbVar1;
  tU32 *puVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  tU8 bVar7;
  tU8 uVar8;
  int bVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  float *pfVar13;
  float *pfVar14;
  float *pfVar15;
  float *pfVar16;
  int iVar17;
  tU32 uVar18;
  tU32 *puVar19;
  tU32 *puVar20;
  int iVar21;
  int iVar22;
  float *pfVar23;
  tU32 *puVar24;
  tU8 *puVar25;
  int local_8e4;
  int local_8e0;
  int local_8d8;
  int local_8d4;
  int local_8c8;
  float local_8c4;
  int local_8c0;
  tU32 local_8b8 [2];
  float local_8b0;
  int local_8ac;
  float local_8a8;
  tU32 local_8a4 [8];
  tU8 local_884 [100];
  float local_820 [99];
  float afStack_694 [101];
  float local_500 [99];
  float afStack_374 [101];
  tU32 local_1e0 [115];

  local_8c0 = -1;
  *param_7 = 0;
  if (param_4 == 1) {
    if (gDOUBLE_0058a8e0 < *param_1) {
      *param_3 = *param_2 / *param_1;
      return 0;
    }
    *param_3 = 0.0;
    return 0;
  }
  iVar10 = 0;
  if (0 < param_4) {
    do {
      local_884[iVar10] = (tU8)iVar10;
      iVar10 = iVar10 + 1;
    } while (iVar10 < param_4);
  }
  iVar10 = 0;
  iVar17 = (param_4 - param_8) + -1;
  local_8e4 = param_8 + 1;
  *param_7 = iVar17;
  if (0 < iVar17) {
    do {
      *(char *)(iVar10 + param_6) = (char)iVar10 + (char)param_8 + '\x01';
      iVar10 = iVar10 + 1;
    } while (iVar10 < *param_7);
  }
  FUN_004c5220((int)gPhysics_buffer1_ptr, (int)gPhysics_buffer2_ptr, (undefined4)(tU32)param_1, (undefined4*)(afStack_694 + 1), (int*)param_2, local_884, local_8e4, (int**)param_10, (int)param_11);
  local_8b8[1] = 0;
  local_8b8[0] = 1;
  local_8d8 = 0;
  local_8d4 = local_8e4;
  do {
    fVar6 = gFLOAT_0058a8ec;
    if ((0x31 < local_8d8) && (local_8c8 == -1)) {
      PhysicsNoOp();
      return 0xffffffff;
    }
    puVar2 = local_1e0 + local_8d8 * 2;
    iVar10 = 1;
    *puVar2 = local_8b8[0];
    local_1e0[local_8d8 * 2 + 1] = local_8b8[1];
    local_820[0] = afStack_694[1] / *((float*)gPhysics_buffer1_ptr);
    if (1 < local_8e4) {
      iVar17 = 0;
      pfVar16 = ((float*)gPhysics_buffer1_ptr);
      pfVar13 = ((float*)gPhysics_buffer1_ptr);
      do {
        pfVar13 = pfVar13 + 100;
        pfVar16 = pfVar16 + 0x65;
        fVar5 = gFLOAT_0058a8b8;
        if (-4 < iVar17) {
          pfVar14 = local_820;
          pfVar15 = pfVar13;
          iVar22 = iVar10;
          do {
            fVar3 = *pfVar14;
            fVar4 = *pfVar15;
            pfVar15 = pfVar15 + 1;
            pfVar14 = pfVar14 + 1;
            iVar22 = iVar22 + -1;
            fVar5 = fVar3 * fVar4 + fVar5;
          } while (iVar22 != 0);
        }
        iVar10 = iVar10 + 1;
        *(float *)((int)local_820 + iVar17 + 4) =
             (*(float *)((int)afStack_694 + iVar17 + 8U) - fVar5) / *pfVar16;
        iVar17 = iVar17 + 4;
      } while (iVar10 < local_8e4);
    }
    iVar10 = local_8e4 + -1;
    iVar17 = 0;
    if (0 < iVar10) {
      pfVar16 = ((float*)gPhysics_buffer1_ptr) + iVar10 * 100;
      do {
        if (fabsf(((float*)gPhysics_buffer1_ptr)[iVar10 * 0x65] * gFLOAT_0058a8e8) < fabsf(*pfVar16)) break;
        iVar17 = iVar17 + 1;
        pfVar16 = pfVar16 + 1;
      } while (iVar17 < iVar10);
    }
    if (iVar17 == iVar10) {
      if (0 < local_8e4) {
        pfVar16 = afStack_374;
        iVar17 = local_8e4;
        do {
          pfVar16 = (float *)((int)pfVar16 + 4);
          *pfVar16 = 0.0;
          pfVar14 = local_820;
          fVar5 = *pfVar16;
          pfVar13 = (float *)(((int)gPhysics_buffer2_ptr - (int)(afStack_374 + 1)) + (int)pfVar16);
          iVar22 = local_8e4;
          do {
            fVar3 = *pfVar13;
            fVar4 = *pfVar14;
            pfVar13 = pfVar13 + 100;
            pfVar14 = pfVar14 + 1;
            iVar22 = iVar22 + -1;
            fVar5 = fVar3 * fVar4 + fVar5;
          } while (iVar22 != 0);
          *pfVar16 = fVar5;
          iVar17 = iVar17 + -1;
        } while (iVar17 != 0);
      }
      local_8c8 = -1;
      if (-1 < local_8c0) {
        local_8c0 = -1;
        bVar7 = *(tU8 *)(*param_7 + -1 + param_6);
        if (param_2[bVar7] == gFLOAT_0058a8b8) {
          param_2[bVar7] = -10000.0;
        }
      }
      if (param_8 < local_8e4) {
        pfVar16 = afStack_374 + param_8 + 1;
        iVar17 = param_8;
        do {
          if (*pfVar16 < fVar6) {
            fVar6 = *pfVar16;
            local_8c8 = iVar17;
          }
          iVar17 = iVar17 + 1;
          pfVar16 = pfVar16 + 1;
        } while (iVar17 < local_8e4);
      }
      if (-1 < local_8c8) {
        if (gFLOAT_0058a8b8 <= afStack_374[local_8e4]) {
          iVar17 = iVar10;
          if (local_8d4 <= iVar10) {
            pfVar16 = afStack_374 + local_8e4;
            do {
              if (gFLOAT_0058a8b8 < *pfVar16) {
                iVar17 = iVar17 + -1;
                break;
              }
              iVar17 = iVar17 + -1;
              pfVar16 = pfVar16 + -1;
            } while (local_8d4 <= iVar17);
          }
          iVar17 = iVar17 + 1;
        }
        else {
          iVar17 = 0;
        }
        if (local_8c8 < iVar17) {
          *(tU8 *)(*param_7 + param_6) = local_884[local_8c8];
          iVar17 = (tU32)local_884[local_8c8] - param_8;
          local_884[local_8c8] = 0xff;
          iVar10 = iVar17 >> 5;
          local_8b8[iVar10] = local_8b8[iVar10] & ~(1 << ((tU8)iVar17 & 0x1f));
          *param_7 = *param_7 + 1;
        }
        else {
          local_8b0 = 0.0;
          for (iVar17 = param_8; iVar22 = param_8, local_8c8 = param_8, iVar17 < local_8e4;
              iVar17 = iVar17 + 1) {
            if (iVar17 <= iVar10) {
              pfVar16 = ((float*)gPhysics_buffer2_ptr) + iVar17 + iVar10 * 100;
              iVar22 = iVar10;
              pfVar13 = local_500 + iVar10;
              pfVar14 = ((float*)gPhysics_buffer1_ptr) + iVar10 * 0x65;
              do {
                fVar6 = gFLOAT_0058a8b8;
                if (iVar22 < iVar10) {
                  pfVar23 = local_500 + iVar10;
                  pfVar15 = ((float*)gPhysics_buffer1_ptr) + iVar22 + iVar10 * 100;
                  iVar11 = iVar10 - iVar22;
                  do {
                    fVar5 = *pfVar23;
                    fVar3 = *pfVar15;
                    pfVar15 = pfVar15 + -100;
                    pfVar23 = pfVar23 + -1;
                    iVar11 = iVar11 + -1;
                    fVar6 = fVar5 * fVar3 + fVar6;
                  } while (iVar11 != 0);
                }
                fVar5 = *pfVar16;
                iVar22 = iVar22 + -1;
                pfVar16 = pfVar16 + -100;
                *pfVar13 = (fVar5 - fVar6) / *pfVar14;
                pfVar13 = pfVar13 + -1;
                pfVar14 = pfVar14 + -0x65;
              } while (iVar17 <= iVar22);
            }
            local_820[iVar17] = local_500[iVar17];
          }
          for (; iVar22 < local_8e4; iVar22 = iVar22 + 1) {
            if ((afStack_374[iVar22 + 1] / local_820[iVar22] < gFLOAT_0058a8b8) &&
               (local_8b0 < fabsf(local_820[iVar22]))) {
              local_8b0 = fabsf(local_820[iVar22]);
              local_8c8 = iVar22;
            }
          }
          *(tU8 *)(*param_7 + param_6) = local_884[local_8c8];
          iVar17 = (tU32)local_884[local_8c8] - param_8;
          local_884[local_8c8] = 0xff;
          iVar10 = iVar17 >> 5;
          local_8b8[iVar10] = local_8b8[iVar10] & ~(1 << ((tU8)iVar17 & 0x1f));
          *param_7 = *param_7 + 1;
        }
        iVar10 = 0;
        local_8d4 = 0;
        if (0 < local_8e4) {
          pfVar16 = afStack_694 + 1;
          pfVar13 = afStack_694;
          do {
            pfVar13 = (float *)((int)pfVar13 + 4);
            if (local_884[iVar10] != 0xff) {
              local_884[local_8d4] = local_884[iVar10];
              *pfVar16 = *pfVar13;
              local_8d4 = local_8d4 + 1;
              pfVar16 = pfVar16 + 1;
            }
            iVar10 = iVar10 + 1;
          } while (iVar10 < local_8e4);
        }
        goto LAB_004c4320;
      }
      iVar10 = param_4;
      pfVar16 = param_3;
      if (0 < param_4) {
        for (; iVar10 != 0; iVar10 = iVar10 + -1) {
          *pfVar16 = 0.0;
          pfVar16 = pfVar16 + 1;
        }
      }
      iVar10 = 0;
      if (0 < local_8e4) {
        pfVar16 = afStack_374;
        do {
          pfVar16 = (float *)((int)pfVar16 + 4);
          pbVar1 = local_884 + iVar10;
          iVar10 = iVar10 + 1;
          param_3[*pbVar1] = *pfVar16;
        } while (iVar10 < local_8e4);
      }
      iVar10 = 0;
      local_8a4[6] = *puVar2;
      local_8a4[7] = local_1e0[local_8d8 * 2 + 1];
      if (0 < local_8d8) {
        puVar24 = local_1e0;
        do {
          iVar17 = 0;
          puVar20 = local_8a4 + 6;
          puVar19 = puVar24;
          do {
            if (*puVar20 != *puVar19) break;
            iVar17 = iVar17 + 1;
            puVar19 = puVar19 + 1;
            puVar20 = puVar20 + 1;
          } while (iVar17 < 2);
          if (1 < iVar17) {
            local_8ac = 1;
            goto LAB_004c43d6;
          }
          iVar10 = iVar10 + 1;
          puVar24 = puVar24 + 2;
        } while (iVar10 < local_8d8);
      }
      local_8ac = 0;
LAB_004c43d6:
      iVar10 = -1;
      iVar22 = 0;
      local_8c4 = 0.0;
      iVar17 = *param_7;
      local_8e0 = -1;
      if (0 < iVar17) {
        do {
          fVar6 = gFLOAT_0058a8b8;
          if (0 < param_4) {
            pfVar13 = param_1 + (tU32)*(tU8 *)(iVar22 + param_6) * 100;
            pfVar16 = param_3;
            iVar17 = param_4;
            do {
              fVar5 = *pfVar13;
              fVar3 = *pfVar16;
              pfVar13 = pfVar13 + 1;
              pfVar16 = pfVar16 + 1;
              iVar17 = iVar17 + -1;
              fVar6 = fVar5 * fVar3 + fVar6;
            } while (iVar17 != 0);
          }
          uVar18 = (tU32)*(tU8 *)(iVar22 + param_6);
          fVar5 = (fVar6 - param_2[uVar18]) / param_1[uVar18 * 0x65];
          iVar17 = local_8e0;
          fVar3 = local_8c4;
          if (((fVar6 - param_2[uVar18] < gDOUBLE_0058a8f0) && (fVar5 < local_8c4)) &&
             (iVar10 = iVar22, iVar17 = iVar22, fVar3 = fVar5, local_8ac != 0)) {
            iVar11 = uVar18 - param_8;
            local_8a4[4] = *puVar2;
            local_8a4[5] = local_1e0[local_8d8 * 2 + 1];
            if (-1 < iVar11) {
              local_8a4[(iVar11 >> 5) + 4] =
                   local_8a4[(iVar11 >> 5) + 4] ^ 1 << ((tU8)iVar11 & 0x1f);
            }
            iVar11 = 0;
            if (0 < local_8d8) {
              puVar24 = local_1e0;
              do {
                iVar21 = 0;
                puVar20 = local_8a4 + 4;
                puVar19 = puVar24;
                do {
                  if (*puVar20 != *puVar19) break;
                  iVar21 = iVar21 + 1;
                  puVar19 = puVar19 + 1;
                  puVar20 = puVar20 + 1;
                } while (iVar21 < 2);
                if (1 < iVar21) {
                  iVar10 = local_8e0;
                  iVar17 = local_8e0;
                  fVar3 = local_8c4;
                  if (local_8e0 == -1) {
                    iVar10 = iVar22;
                    iVar17 = iVar22;
                  }
                  break;
                }
                iVar11 = iVar11 + 1;
                puVar24 = puVar24 + 2;
              } while (iVar11 < local_8d8);
            }
          }
          local_8c4 = fVar3;
          local_8e0 = iVar17;
          iVar22 = iVar22 + 1;
          iVar17 = *param_7;
        } while (iVar22 < iVar17);
      }
      if (iVar10 < 0) {
        bVar9 = 0;
        if (0 < param_4) {
          pfVar16 = local_500;
          iVar10 = param_4;
          pfVar13 = param_1;
          do {
            *pfVar16 = 0.0;
            fVar6 = *pfVar16;
            pfVar14 = pfVar13;
            pfVar15 = param_3;
            iVar17 = param_4;
            do {
              fVar5 = *pfVar14;
              fVar3 = *pfVar15;
              pfVar14 = pfVar14 + 1;
              pfVar15 = pfVar15 + 1;
              iVar17 = iVar17 + -1;
              fVar6 = fVar5 * fVar3 + fVar6;
            } while (iVar17 != 0);
            *pfVar16 = fVar6;
            if (((*(float *)(((int)param_3 - (int)local_500) + (int)pfVar16) != gFLOAT_0058a8b8) &&
                (gDOUBLE_0058a8f8 <
                 fabsf(*pfVar16 - *(float *)(((int)param_2 - (int)local_500) + (int)pfVar16)))) ||
               (*pfVar16 - *(float *)(((int)param_2 - (int)local_500) + (int)pfVar16) <
                gDOUBLE_0058a900)) {
              bVar9 = 1;
            }
            pfVar13 = pfVar13 + 100;
            pfVar16 = pfVar16 + 1;
            iVar10 = iVar10 + -1;
          } while (iVar10 != 0);
        }
        if (bVar9) {
          iVar10 = 0;
          if (0 < local_8e4) {
            do {
              iVar17 = 0;
              pfVar16 = afStack_374;
              fVar6 = -afStack_694[iVar10 + 1];
              do {
                pfVar16 = (float *)((int)pfVar16 + 4);
                pbVar1 = local_884 + iVar17;
                iVar17 = iVar17 + 1;
                fVar6 = param_1[(tU32)*pbVar1 + (tU32)local_884[iVar10] * 100] * *pfVar16 + fVar6;
              } while (iVar17 < local_8e4);
              local_500[iVar10] = fVar6;
              iVar10 = iVar10 + 1;
            } while (iVar10 < local_8e4);
          }
          iVar10 = 1;
          local_820[0] = local_500[0] / *((float*)gPhysics_buffer1_ptr);
          if (1 < local_8e4) {
            iVar17 = 0;
            pfVar16 = ((float*)gPhysics_buffer1_ptr);
            pfVar13 = ((float*)gPhysics_buffer1_ptr);
            do {
              pfVar13 = pfVar13 + 0x65;
              pfVar16 = pfVar16 + 100;
              fVar6 = gFLOAT_0058a8b8;
              if (-4 < iVar17) {
                pfVar14 = local_820;
                pfVar15 = pfVar16;
                iVar22 = iVar10;
                do {
                  fVar5 = *pfVar14;
                  fVar3 = *pfVar15;
                  pfVar15 = pfVar15 + 1;
                  pfVar14 = pfVar14 + 1;
                  iVar22 = iVar22 + -1;
                  fVar6 = fVar5 * fVar3 + fVar6;
                } while (iVar22 != 0);
              }
              iVar10 = iVar10 + 1;
              *(float *)((int)local_820 + iVar17 + 4) =
                   (*(float *)((int)local_500 + iVar17 + 4) - fVar6) / *pfVar13;
              iVar17 = iVar17 + 4;
            } while (iVar10 < local_8e4);
          }
          if (0 < local_8e4) {
            iVar17 = (int)afStack_374 + (4U - (int)gPhysics_buffer2_ptr);
            pfVar16 = ((float*)gPhysics_buffer2_ptr);
            iVar10 = local_8e4;
            do {
              pfVar13 = local_820;
              pfVar14 = pfVar16;
              iVar22 = local_8e4;
              do {
                fVar6 = *pfVar14;
                fVar5 = *pfVar13;
                pfVar14 = pfVar14 + 100;
                pfVar13 = pfVar13 + 1;
                iVar22 = iVar22 + -1;
                *(float *)(iVar17 + (int)pfVar16) =
                     fVar6 * fVar5 + *(float *)(iVar17 + (int)pfVar16);
              } while (iVar22 != 0);
              pfVar16 = pfVar16 + 1;
              iVar10 = iVar10 + -1;
            } while (iVar10 != 0);
          }
        }
        return 0;
      }
      uVar8 = *(tU8 *)(iVar10 + param_6);
      iVar10 = iVar10 + 1;
      if (iVar10 < iVar17) {
        puVar25 = (tU8 *)(iVar10 + param_6);
        do {
          iVar10 = iVar10 + 1;
          puVar25[-1] = *puVar25;
          puVar25 = puVar25 + 1;
        } while (iVar10 < *param_7);
      }
      iVar10 = *param_7;
      *param_7 = iVar10 + -1;
      *(tU8 *)(param_6 + iVar10 + -1) = uVar8;
      bVar7 = *(tU8 *)(*param_7 + param_6);
      local_884[local_8e4] = bVar7;
      iVar10 = (tU32)bVar7 - param_8;
      iVar17 = iVar10 >> 5;
      iVar22 = local_8e4 + 1;
      local_8b8[iVar17] = local_8b8[iVar17] | 1 << ((tU8)iVar10 & 0x1f);
      if (gDAT_006940b8 == 0) {
        FUN_004c4f40(((float*)gPhysics_buffer1_ptr), (undefined4)(tU32)param_1, local_8e4, ((float*)gPhysics_buffer2_ptr), local_884);
      }
      else {
            FUN_004c4850(((float*)gPhysics_buffer1_ptr), (int)(tU32)param_1, local_8e4, iVar22, ((float*)gPhysics_buffer2_ptr), (int)(uintptr_t)local_884);
      }
      afStack_694[iVar22] = param_2[local_884[local_8e4]];
      local_8e4 = iVar22;
    }
    else {
      local_8a8 = 0.0;
      iVar17 = iVar10;
      if (param_8 < iVar10) {
        pfVar16 = ((float*)gPhysics_buffer2_ptr) + param_8 + iVar10 * 100;
        iVar22 = param_8;
        do {
          if (local_8a8 < fabsf(*pfVar16)) {
            local_8a8 = fabsf(*pfVar16);
            iVar17 = iVar22;
          }
          iVar22 = iVar22 + 1;
          pfVar16 = pfVar16 + 1;
        } while (iVar22 < iVar10);
      }
      local_8c0 = param_8;
      if (iVar17 != iVar10) {
        iVar22 = (tU32)local_884[iVar17] - param_8;
        local_8a4[0] = *puVar2;
        local_8a4[1] = local_1e0[local_8d8 * 2 + 1];
        if (-1 < iVar22) {
          local_8a4[iVar22 >> 5] = local_8a4[iVar22 >> 5] ^ 1 << ((tU8)iVar22 & 0x1f);
        }
        iVar22 = 0;
        if (0 < local_8d8) {
          puVar24 = local_1e0;
LAB_004c3e12:
          iVar11 = 0;
          puVar20 = local_8a4;
          puVar19 = puVar24;
          do {
            if (*puVar20 != *puVar19) break;
            iVar11 = iVar11 + 1;
            puVar19 = puVar19 + 1;
            puVar20 = puVar20 + 1;
          } while (iVar11 < 2);
          if (iVar11 < 2) break;
          if (param_8 < iVar10) {
            pfVar16 = ((float*)gPhysics_buffer2_ptr) + param_8 + iVar10 * 100;
            iVar22 = param_8;
            local_8e4 = iVar17;
            fVar6 = gFLOAT_0058a8b8;
            do {
              fVar5 = *pfVar16;
              iVar11 = local_8e4;
              fVar3 = fVar6;
              if (fVar6 < fabsf(fVar5)) {
                iVar17 = (tU32)local_884[iVar22] - param_8;
                local_8a4[2] = *puVar2;
                local_8a4[3] = local_1e0[local_8d8 * 2 + 1];
                if (-1 < iVar17) {
                  local_8a4[(iVar17 >> 5) + 2] =
                       local_8a4[(iVar17 >> 5) + 2] ^ 1 << ((tU8)iVar17 & 0x1f);
                }
                iVar21 = 0;
                puVar24 = local_1e0;
                do {
                  iVar12 = 0;
                  puVar20 = local_8a4 + 2;
                  puVar19 = puVar24;
                  do {
                    if (*puVar20 != *puVar19) break;
                    iVar12 = iVar12 + 1;
                    puVar19 = puVar19 + 1;
                    puVar20 = puVar20 + 1;
                  } while (iVar12 < 2);
                  iVar17 = local_8e4;
                  iVar11 = local_8e4;
                  fVar3 = fVar6;
                  if (1 < iVar12) break;
                  iVar21 = iVar21 + 1;
                  puVar24 = puVar24 + 2;
                  iVar17 = iVar22;
                  iVar11 = iVar22;
                  fVar3 = fabsf(fVar5);
                } while (iVar21 < local_8d8);
              }
              fVar6 = fVar3;
              local_8e4 = iVar11;
              iVar22 = iVar22 + 1;
              pfVar16 = pfVar16 + 1;
            } while (iVar22 < iVar10);
          }
          if (iVar17 != iVar10) goto LAB_004c3f3b;
          goto LAB_004c3f48;
        }
LAB_004c3f3b:
        local_8c0 = iVar17;
      }
LAB_004c3f48:
      *(tU8 *)(*param_7 + param_6) = local_884[local_8c0];
      iVar22 = (int)((tU32)local_884[local_8c0] - param_8) >> 5;
      iVar17 = *param_7;
      local_8b8[iVar22] =
           local_8b8[iVar22] & ~(1 << ((tU8)((tU32)local_884[local_8c0] - param_8) & 0x1f));
      *param_7 = iVar17 + 1;
      for (iVar17 = local_8c0; local_8d4 = iVar10, iVar17 < iVar10; iVar17 = iVar17 + 1) {
        local_884[iVar17] = local_884[iVar17 + 1];
      }
LAB_004c4320:
      FUN_004c5220((int)gPhysics_buffer1_ptr, (int)gPhysics_buffer2_ptr, (undefined4)(tU32)param_1, (undefined4*)(afStack_694 + 1), (int*)param_2, local_884, local_8d4, (int**)param_10, (int)param_11);
      local_8e4 = local_8d4;
    }
    local_8d8 = local_8d8 + 1;
  } while (1);
  iVar22 = iVar22 + 1;
  puVar24 = puVar24 + 2;
  if (local_8d8 <= iVar22) goto LAB_004c3f3b;
  goto LAB_004c3e12;
}


#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 25.30% | difftest PASS 2000 trials (spec difftest/specs/004c5830_FUN_004c5830.json) | reason vc5-register-spills
// FUNCTION: CARMA2_HW 0x004c5830
undefined4 C2_HOOK_FASTCALL FUN_004c5830(int param_1, int param_2, undefined4* param_3) {
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float *pfVar7;
  int iVar8;
  register float *pfVar9;
  float *pfVar10;
  undefined4 *puVar11;
  register int iVar12;
  register int iVar13;
  register float *pfVar14;
  undefined4 *puVar15;
  float *pfVar16;
  undefined4 *puVar17;
  float local_38;
  int local_34;
  float *local_30;
  int local_1c;
  int local_18;
  int local_10;
  undefined4 local_c;

  local_c = 0;
  iVar12 = param_2;
  puVar17 = param_3;
  puVar15 = param_3;
  iVar13 = param_2;
  puVar11 = param_3;
  if (0 < param_2) {
    do {
      for (; iVar12 != 0; iVar12 = iVar12 - 1) {
        *puVar17 = 0;
        puVar17 = puVar17 + 1;
      }
      *puVar15 = 0x3f800000;
      iVar13 = iVar13 - 1;
      iVar12 = param_2;
      puVar17 = puVar11 + param_2;
      puVar15 = puVar15 + param_2 + 1;
      puVar11 = puVar11 + param_2;
    } while (iVar13 != 0);
  }
  iVar12 = 0;
  if (0 < param_2) {
    local_34 = 0;
    local_1c = 0;
    iVar13 = param_1;
    do {
      local_38 = 0.0f;
      if (iVar12 < param_2) {
        pfVar14 = (float*)(iVar13 + local_34);
        iVar8 = param_2 - iVar12;
        do {
          fVar5 = (float)fabs(*pfVar14);
          if ((float)fabs(*pfVar14) < local_38) {
            fVar5 = local_38;
          }
          pfVar14 = pfVar14 + 1;
          iVar8 = iVar8 - 1;
          local_38 = fVar5;
        } while (iVar8 != 0);
      }
      if (local_38 == (float)gDOUBLE_0058a8c0) {
        local_c = 1;
      }
      else {
        if (iVar12 < param_2) {
          iVar8 = param_2 - iVar12;
          pfVar14 = (float*)(iVar13 + local_34);
          do {
            iVar8 = iVar8 - 1;
            *pfVar14 = *pfVar14 / local_38;
            pfVar14 = pfVar14 + 1;
          } while (iVar8 != 0);
        }
        fVar5 = gFLOAT_0058a8b8;
        if (iVar12 < param_2) {
          pfVar14 = (float*)(iVar13 + local_34);
          iVar8 = param_2 - iVar12;
          do {
            fVar1 = *pfVar14;
            pfVar14 = pfVar14 + 1;
            iVar8 = iVar8 - 1;
            fVar5 = fVar1 * fVar1 + fVar5;
          } while (iVar8 != 0);
        }
        pfVar14 = (float*)(local_34 + iVar13);
        fVar5 = (float)fabs(sqrt(fVar5));
        if (*pfVar14 <= (float)gDOUBLE_0058a8c0) {
          fVar5 = -fVar5;
        }
        fVar1 = fVar5 * (fVar5 + *pfVar14);
        *pfVar14 = fVar5 + *pfVar14;
        if (-1 < local_34) {
          pfVar9 = (float*)((int)param_3 + (iVar13 - param_1));
          local_18 = iVar12 + 1;
          do {
            fVar6 = gFLOAT_0058a8b8;
            if (iVar12 < param_2) {
              iVar8 = param_2 - iVar12;
              pfVar7 = pfVar9;
              pfVar10 = pfVar14;
              do {
                fVar2 = *pfVar10;
                fVar3 = *pfVar7;
                pfVar10 = pfVar10 + 1;
                pfVar7 = pfVar7 + param_2;
                iVar8 = iVar8 - 1;
                fVar6 = fVar2 * fVar3 + fVar6;
              } while (iVar8 != 0);
            }
            if (iVar12 < param_2) {
              iVar8 = param_2 - iVar12;
              pfVar7 = pfVar9;
              pfVar10 = pfVar14;
              do {
                fVar2 = *pfVar10;
                pfVar10 = pfVar10 + 1;
                *pfVar7 = *pfVar7 - (fVar6 / fVar1) * fVar2;
                pfVar7 = pfVar7 + param_2;
                iVar8 = iVar8 - 1;
              } while (iVar8 != 0);
            }
            pfVar9 = pfVar9 + 1;
            local_18 = local_18 - 1;
          } while (local_18 != 0);
        }
        local_10 = iVar12 + 1;
        if (local_10 < param_2) {
          pfVar9 = (float*)(param_3 + local_1c + local_10);
          local_30 = (float*)(param_1 + (iVar12 + param_2 + local_1c) * 4);
          local_10 = param_2 - local_10;
          do {
            fVar6 = gFLOAT_0058a8b8;
            if (iVar12 < param_2) {
              iVar8 = param_2 - iVar12;
              pfVar7 = local_30;
              pfVar10 = pfVar14;
              do {
                fVar2 = *pfVar10;
                fVar3 = *pfVar7;
                pfVar7 = pfVar7 + 1;
                pfVar10 = pfVar10 + 1;
                iVar8 = iVar8 - 1;
                fVar6 = fVar2 * fVar3 + fVar6;
              } while (iVar8 != 0);
            }
            fVar2 = gFLOAT_0058a8b8;
            if (iVar12 < param_2) {
              pfVar7 = (float*)(param_1 + (param_2 + 1) * iVar12 * 4);
              iVar8 = param_2 - iVar12;
              pfVar10 = pfVar9;
              do {
                fVar3 = *pfVar10;
                fVar4 = *pfVar7;
                pfVar7 = pfVar7 + 1;
                pfVar10 = pfVar10 + param_2;
                iVar8 = iVar8 - 1;
                fVar2 = fVar3 * fVar4 + fVar2;
              } while (iVar8 != 0);
            }
            if (iVar12 < param_2) {
              iVar8 = param_2 - iVar12;
              pfVar7 = local_30;
              pfVar10 = pfVar14;
              pfVar16 = pfVar9;
              do {
                *pfVar7 = *pfVar7 - (fVar6 / fVar1) * *pfVar10;
                *pfVar16 = *pfVar16 - (fVar2 / fVar1) * *pfVar10;
                pfVar16 = pfVar16 + param_2;
                iVar8 = iVar8 - 1;
                pfVar7 = pfVar7 + 1;
                pfVar10 = pfVar10 + 1;
              } while (iVar8 != 0);
            }
            local_30 = local_30 + param_2;
            pfVar9 = pfVar9 + 1;
            local_10 = local_10 - 1;
          } while (local_10 != 0);
        }
        *pfVar14 = -(fVar5 * local_38);
        if (iVar12 + 1 < param_2) {
          puVar17 = (undefined4*)(iVar13 + 4 + local_34);
          for (iVar8 = param_2 - (iVar12 + 1); iVar8 != 0; iVar8 = iVar8 - 1) {
            *puVar17 = 0;
            puVar17 = puVar17 + 1;
          }
        }
      }
      iVar12 = iVar12 + 1;
      local_1c = local_1c + param_2;
      local_34 = local_34 + 4;
      iVar13 = iVar13 + param_2 * 4;
    } while (iVar12 < param_2);
  }
  return local_c;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c5b50
void C2_HOOK_FASTCALL FUN_004c5b50(tU32 pFirst, tU32 pSecond, tU32 pThird) {
    FUN_004c5830(pFirst, pThird, pSecond);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c5b60
void C2_HOOK_FASTCALL RqBackSub(const br_scalar* pR, const br_scalar* pQ, const br_scalar* pRhs, br_scalar* pX, int pDim) {
    br_scalar intermediate[100];
    br_scalar sum;
    int i;
    int j;

    /* Solve Ax = b, with A represented by its RQ decomposition, where:
     * - R is a lower-diagonal matrix
     * - Q is an orthonormal matrix
     * with:
     * - all matrices (A, Q, R) are of size N x N (with N = pDim)
     * - vectors x, t are of size N x 1
     * A, Q, R matrices are of size [pDim x pDim]
     */

    C2_HOOK_ASSERT(pDim > 0 && pDim <= (int)CARPOCALYPSE2_ASIZE(intermediate));

    /* Use back substitution to solve Ry = b, with R a lower-diagonal matrix */
    intermediate[0] = pRhs[0] / pR[0 * pDim + 0];             /*  T[0] = B[0] / R[0, 0] */
    for (i = 1; i < pDim; i++) {
        sum = 0.0f;
        for (j = 0; j < i; j++) {
            sum += intermediate[j] * pR[i * pDim + j];        /*  sum += T[j] * R[i, j] */
        }
        intermediate[i] = (pRhs[i] - sum) / pR[i * pDim + i]; /*  T[i] = (B[i] - sum) / R[i, i] */
    }
    /* Solve Qx = T, by multiplying both sides with Q transposed */
    for (i = 0; i < pDim; i++) {
        pX[i] = pQ[0 * pDim + i] * intermediate[0];           /*  X[i]  = Q[0, i] * T[0] */
        for (j = 1; j < pDim; j++) {
            pX[i] += intermediate[j] * pQ[j * pDim + i];      /*  X[i] += Q[j, i] * T[j] */
        }
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c5c70
void* C2_HOOK_FASTCALL PhysicsScratchSpaceCheck(int pSize) {
    tU32 limit;

    limit = (tU32)gPhysics_other_buffer + gPhysics_other_buffer_capacity;
    if ((tU32)(pSize + (tU8*)gPhysics_other_buffer_cursor) > limit) {
        if (gPhysics_error_cb != NULL) {
            gPhysics_error_cb(ePhysicsError_OutOfMemory, NULL);
        }
        exit(ePhysicsError_OutOfMemory);
    }
    return gPhysics_other_buffer_cursor;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c5cb0
void C2_HOOK_FASTCALL IncreasePhysicsScratchPointer(int pSize) {
    if ((tU32)(pSize + (tU32)gPhysics_other_buffer_cursor)
            > (tU32)((tU32)gPhysics_other_buffer + gPhysics_other_buffer_capacity)) {
        if (gPhysics_error_cb != NULL) {
            gPhysics_error_cb(ePhysicsError_OutOfMemory, NULL);
        }
        exit(6);
    }
    gPhysics_other_buffer_cursor = (tU8*)gPhysics_other_buffer_cursor + pSize;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 14.29% | difftest PASS 2000 trials (spec difftest/specs/004c53b0_FUN_004c53b0.json) | reason original inlines the QR and solve loops
// FUNCTION: CARMA2_HW 0x004c53b0
void C2_HOOK_FASTCALL FUN_004c53b0(float* pMatrix, float* pData, float* pJoint_data, float* pJoint_dt, int pCount) {
    FUN_004c5830((int)pMatrix, pCount, (undefined4*)pData);
    if (pJoint_data == NULL || pJoint_dt == NULL) {
        return;
    }
    RqBackSub(pMatrix, pData, pJoint_data, pJoint_dt, pCount);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c5d00
void C2_HOOK_FASTCALL ClearPhysicsScratchSpace(void) {

    gPhysics_other_buffer_cursor = gPhysics_other_buffer;
}

void C2_HOOK_FASTCALL PhysicsWarning(const char* pMessage) {
    (void)pMessage;
}

// GLOBAL: CARMA2_HW 0x0058a7a0
const double gDOUBLE_0058a7a0 = 0.001;

// GLOBAL: CARMA2_HW 0x0058a808
const double gDOUBLE_0058a808 = -0.001;

// GLOBAL: CARMA2_HW 0x0058a810
const double gDOUBLE_0058a810 = 0.000001;

// GLOBAL: CARMA2_HW 0x0058a818
const double gDOUBLE_0058a818 = 0.002;

// GLOBAL: CARMA2_HW 0x0058a7a8
const double gDOUBLE_0058a7a8 = 0.00009;

// GLOBAL: CARMA2_HW 0x0058a7b0
const br_scalar gFLOAT_0058a7b0 = 0.012f;

// GLOBAL: CARMA2_HW 0x0058a7b4
const br_scalar gFLOAT_0058a7b4 = 0.9f;

// GLOBAL: CARMA2_HW 0x0058a7b8
const br_scalar gFLOAT_0058a7b8 = 0.0004f;

// GLOBAL: CARMA2_HW 0x0058a7c0
const double gDOUBLE_0058a7c0 = 0.999;

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 44.78% | difftest PASS 2000 trials (spec difftest/specs/004baa00_AddDoubleTorqueToMatrix.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x004baa00
int C2_HOOK_FASTCALL AddDoubleTorqueToMatrix(tPhysics_object* pObject) {
    tPhysics_object* obj;
    int all_stable;
    int stable;

    all_stable = 1;
    if (pObject == NULL) {
        return all_stable;
    }
    obj = pObject;
    do {
        if (obj->field_0x21c != 1.0f) {
            obj->I.v[0] /= obj->field_0x21c;
            obj->I.v[1] /= obj->field_0x21c;
            obj->I.v[2] /= obj->field_0x21c;
        }
        if (obj->field_0xf0 != 0) {
            obj->field_0xf0 = 0;
            obj->disable_move_rotate = (tU8)((obj->flags >> 5) & 1);
        }
        obj->flags = (obj->flags & ~0x100) | ((obj->flags >> 1) & 0x100);
        stable = 0;

        if (!obj->disable_move_rotate) {
            br_scalar dx;
            br_scalar dy;
            br_scalar dz;
            br_scalar velocity_squared;
            br_scalar energy;
            br_scalar axis_length;
            br_scalar axis_x;
            br_scalar axis_y;
            br_scalar axis_z;
            br_scalar projected_velocity;

            if (obj->field_0x1df != 0 && obj->field183_0x1d8 == obj) {
                obj->field_0x1df = 0;
            }

            dx = obj->field_0x1cc.v[0] + obj->omega.v[0];
            dy = obj->field_0x1cc.v[1] + obj->omega.v[1];
            dz = obj->field_0x1cc.v[2] + obj->omega.v[2];

            if (obj->collision_flag != 0 || obj->field_0x1df == 0) {
                velocity_squared = obj->v.v[2] * obj->v.v[2]
                    + obj->v.v[1] * obj->v.v[1]
                    + obj->v.v[0] * obj->v.v[0];
            } else {
                velocity_squared = gFLOAT_0058a768;
            }
            energy = (obj->I.v[2] * dz * dz
                + dx * dx * obj->I.v[0]
                + obj->I.v[1] * dy * dy) / obj->M;

            if (velocity_squared < gDOUBLE_0058a7a0 && energy < gDOUBLE_0058a7a8) {
                axis_length = (br_scalar)sqrt(
                    obj->field_0x54.v[2] * obj->field_0x54.v[2]
                    + obj->field_0x54.v[1] * obj->field_0x54.v[1]
                    + obj->field_0x54.v[0] * obj->field_0x54.v[0]);
                axis_x = obj->field_0x54.v[0] / axis_length;
                axis_y = obj->field_0x54.v[1] / axis_length;
                axis_z = obj->field_0x54.v[2] / axis_length;
                projected_velocity = -(obj->v.v[2] * axis_z
                    + obj->v.v[1] * axis_y
                    + obj->v.v[0] * axis_x);

                if (obj->field_0x1df == 0) {
                    if (projected_velocity < gFLOAT_0058a7b0
                            && projected_velocity
                                - -(obj->field_0x1b4.v[2] * axis_z
                                    + obj->field_0x1b4.v[1] * axis_y
                                    + obj->field_0x1b4.v[0] * axis_x)
                                > axis_length * gFLOAT_0058a7b4) {
                        int corner;

                        for (corner = 0; corner < 8; corner++) {
                            br_scalar corner_x;
                            br_scalar corner_y;
                            br_scalar corner_z;
                            br_scalar motion_x;
                            br_scalar motion_y;
                            br_scalar motion_z;

                            if ((corner & 2) == 0) {
                                corner_x = obj->bb1.max.v[0];
                            } else {
                                corner_x = obj->bb1.min.v[0];
                            }
                            if ((corner & 1) == 0) {
                                corner_y = obj->bb1.max.v[1];
                            } else {
                                corner_y = obj->bb1.min.v[1];
                            }
                            if ((corner & 4) == 0) {
                                corner_z = obj->bb1.max.v[2];
                            } else {
                                corner_z = obj->bb1.min.v[2];
                            }

                            motion_x = (obj->omega.v[1] * corner_z
                                - obj->omega.v[2] * corner_y)
                                + (obj->omega.v[1] * obj->cmpos.v[2]
                                    - obj->omega.v[2] * obj->cmpos.v[1])
                                + obj->velocity_car_space.v[0];
                            motion_y = (obj->omega.v[2] * corner_x
                                - obj->omega.v[0] * corner_z)
                                + (obj->omega.v[2] * obj->cmpos.v[0]
                                    - obj->cmpos.v[2] * obj->omega.v[0])
                                + obj->velocity_car_space.v[1];
                            motion_z = (obj->omega.v[0] * corner_y
                                - obj->omega.v[1] * corner_x)
                                + (obj->cmpos.v[1] * obj->omega.v[0]
                                    - obj->omega.v[1] * obj->cmpos.v[0])
                                + obj->velocity_car_space.v[2];
                            if (gFLOAT_0058a7b8
                                    < motion_x * motion_x
                                        + motion_y * motion_y
                                        + motion_z * motion_z) {
                                break;
                            }
                        }
                        if (corner < 8) {
                            goto finish_stability_check;
                        }
                    } else {
                        goto finish_stability_check;
                    }
                }

                if (*(tU32*)((tU8*)obj + 0x18c) == 0
                        || obj->collision_flag != 0
                        || gDOUBLE_0058a7c0 < obj->transform_matrix.m[1][1]) {
                    stable = 1;
                    BrVector3SetFloat(&obj->v, 0.0f, 0.0f, 0.0f);
                    BrVector3SetFloat(&obj->omega, 0.0f, 0.0f, 0.0f);
                    BrVector3SetFloat(&obj->rotate_omega, 0.0f, 0.0f, 0.0f);
                }
            }
        }

finish_stability_check:
        obj->field_0x1df = (tU8)stable;
        BrMatrix34Copy(&obj->transform_matrix, &obj->actor->t.t.mat);
        BrMatrix34ApplyP(&obj->pos, &obj->cmpos, &obj->transform_matrix);
        if (obj->child == NULL) {
            obj->field_0x1df = (tU8)(obj->field_0x1df * 3);
        } else {
            int child_stable;

            child_stable = AddDoubleTorqueToMatrix(obj->child);
            if (obj->field_0x1df != 0) {
                obj->field_0x1df = (tU8)(child_stable * 2 + obj->field_0x1df);
            }
        }
        all_stable &= (obj->field_0x1df >> 1) & 1;
        obj = obj->next;
    } while (obj != NULL);
    return all_stable;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004bae80
void C2_HOOK_FASTCALL DRMatrix33Inverse(tPhysics_object* pObject, int pFlag) {
    tPhysics_object* sibling;

    pObject->field_0x1dc = (tU8)pFlag;
    for (sibling = pObject->child; sibling != NULL; sibling = sibling->next) {
        if (sibling->physics_joint1 != NULL && sibling->physics_joint1->type != 0) {
            DRMatrix33Inverse(sibling, pFlag);
        }
    }
}

typedef struct tPhysics_group_data {
    int count;
    tCollision_contact* data;
    undefined4 field_0x8;
    undefined4 field_0xc;
    undefined4 field_0x10;
    float* field_0x14;
    float* field_0x18;
} tPhysics_group_data;

static void ResetObjectAndChildren(tPhysics_object* pObject);
int C2_HOOK_FASTCALL FUN_004bd7f0(tPhysics_object* pObject);

// C2PORT-NONMATCHING: best 36.35% | difftest PASS 4000 trials (spec difftest/specs/004baec0_SetUpQuickHingeData.json, _legacy/_torque) | reason x87-order-and-stack-layout (legacy B-vector path unobservable under stubs)
// FUNCTION: CARMA2_HW 0x004baec0
int C2_HOOK_FASTCALL SetUpQuickHingeData(tPhysics_object** pObject_list, tPhysics_object* pObject, void* pBuffer1, void* pBuffer2, int pArg, tWorld_callbacks* pWorld_callbacks) {
    tCollision_contact* contacts;
    tPhysics_object* overlap_head;
    tPhysics_object* list_head;
    tPhysics_object* obj;
    tPhysics_object* next_obj;
    tPhysics_object* child;
    tPhysics_object* top;
    tPhysics_object* orig;
    tCollision_contact* rows[100];
    tCollision_contact** rows_c;
    tPhysics_group_data* groups[100];
    float solve_a[100];
    float solve_b[100];
    float* a_c;
    float* b_c;
    tU8 solve_flags[100];
    tU8 solve_other[4];
    tPhysics_group_data* group;
    tCollision_contact* group_rows[40];
    float group_b[100];
    float group_a[100];
    tPhysics_joint* joint;
    tCollision_contact* rec;
    tCollision_contact* p;
    tPhysics_object* o1;
    tPhysics_object* o2;
    br_vector3 tmp;
    br_vector3 tmp2;
    br_vector3 torque;
    br_vector3 cross;
    float thr;
    float lam;
    float value;
    float max_value;
    float step;
    int count;
    int old_count;
    int ring_length;
    int total;
    int contact_count;
    int joint_rows;
    int group_count;
    int nrows;
    int i;
    int k;
    int idx;
    int n;
    int any;
    int solve_result;
    int changed;
    int sum_children;
    int sum_all;
    int flags_acc;
    float dx;
    float dy;
    float dz;

    contacts = (tCollision_contact*)pBuffer1;
    count = 0;
    obj = pObject;
    do {
        next_obj = obj->field183_0x1d8;
        old_count = count;
        list_head = *pObject_list;
        AppendOverlappingObjects(obj, list_head, &overlap_head);
        count = FUN_004bd9e0(obj, &overlap_head, pBuffer1, count, pWorld_callbacks, list_head);
        if (gNet_mode == 2 && obj->field_0xf0 == 1 && count > old_count) {
            dx = obj->transform_matrix.m[3][0] - ((float*)&obj->field_0xbc[0x24])[0];
            dy = obj->transform_matrix.m[3][1] - ((float*)&obj->field_0xbc[0x24])[1];
            dz = obj->transform_matrix.m[3][2] - ((float*)&obj->field_0xbc[0x24])[2];
            if (sqrtf(obj->v.v[2] * obj->v.v[2] + obj->v.v[1] * obj->v.v[1] + obj->v.v[0] * obj->v.v[0]) - gDOUBLE_0058a7d0
                    < sqrtf(dz * dz + dy * dy + dx * dx) * gFLOAT_0058a7c8) {
                obj->flags |= 0x1000;
                obj->field_0xf0 = 0;
                obj = pObject;
                do {
                    if (obj->disable_move_rotate == 0) {
                        if (obj->field_0xf0 == 1) {
                            BrMatrix34Copy(&obj->actor->t.t.mat, (const br_matrix34*)&obj->field_0xbc);
                        } else {
                            BrMatrix34LPNormalise(&obj->actor->t.t.mat, &obj->transform_matrix);
                        }
                        for (child = obj->child; child != NULL; child = child->next) {
                            ResetObjectAndChildren(child);
                        }
                        FUN_004b8ee0(obj CARPOCALYPSE2_THISCALL_EDX, 0.04f);
                        MergeBoundsIntoParent(obj, NULL);
                        if (obj->field_0x194 != 0) {
                            ((int (C2_HOOK_FASTCALL *)(tPhysics_object*))(uintptr_t)obj->field_0x194)(obj);
                        }
                    }
                    obj = obj->field183_0x1d8;
                } while (obj != pObject);
                return -1;
            }
        }
        obj = next_obj;
    } while (next_obj != pObject);

    gINT_00692dd8 = 0;
    ring_length = 0;
    obj = pObject;
    do {
        ring_length++;
        if (ring_length > 100) {
            PhysicsError((tPhysicsError)10, "Corrupt group");
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);

    obj = pObject;
    do {
        if ((obj->flags & 0x10) == 0) {
            old_count = count;
            count = FUN_004bca10(obj, (undefined4*)pBuffer1, count, 0x28);
            child = obj->child;
            if (child != NULL) {
                for (; child != NULL; child = child->next) {
                    count = FUN_004bca10(child, (undefined4*)pBuffer1, count, 0x28);
                    if (child->child != NULL) {
                        count = FUN_004bd6d0(child->child, (undefined4*)pBuffer1, count, 0x28);
                    }
                }
            }
            if (old_count != count && obj->field_0xf0 == 2) {
                top = obj;
                while (top->parent != NULL && top->physics_joint1 != NULL && top->physics_joint1->type != 0) {
                    top = top->parent;
                }
                top->field_0xf0 = 0;
                for (child = top->child; child != NULL; child = child->next) {
                    SetFieldF0Tree(child, 0);
                }
                if (pWorld_callbacks != NULL && pWorld_callbacks->activate_passive != NULL) {
                    pWorld_callbacks->activate_passive(top, NULL);
                }
                if ((top->child != NULL || top->physics_joint2 != NULL) && top->field_0x218 == 0) {
                    MoveJointedObject(top CARPOCALYPSE2_THISCALL_EDX, 0.0f);
                }
            }
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);

    if (count == 0) {
        return 0;
    }
    if (count > 0) {
        for (i = 0; i < count; i++) {
            rec = &contacts[i];
            orig = rec->object1;
            while ((rec->object1->flags & 0x400) != 0) {
                rec->object1 = rec->object1->parent;
            }
            if (orig != rec->object1) {
                rec->r1.v[0] = orig->cmpos.v[0] + rec->r1.v[0];
                rec->r1.v[1] = orig->cmpos.v[1] + rec->r1.v[1];
                rec->r1.v[2] = orig->cmpos.v[2] + rec->r1.v[2];
                BrMatrix34ApplyP(&tmp, &rec->r1, &orig->transform_matrix);
                DrMatrix34ApplyLPInverse(&rec->r1, &tmp, &rec->object1->transform_matrix);
                rec->r1.v[0] = rec->r1.v[0] - rec->object1->cmpos.v[0];
                rec->r1.v[1] = rec->r1.v[1] - rec->object1->cmpos.v[1];
                rec->r1.v[2] = rec->r1.v[2] - rec->object1->cmpos.v[2];
                BrMatrix34ApplyV(&tmp, &rec->normal, &orig->transform_matrix);
                BrMatrix34TApplyV(&rec->normal, &tmp, &rec->object1->transform_matrix);
            }
            orig = rec->object2;
            if (orig != NULL) {
                while ((rec->object2->flags & 0x400) != 0) {
                    rec->object2 = rec->object2->parent;
                }
                if (orig != rec->object1) {
                    rec->r2.v[0] = orig->cmpos.v[0] + rec->r2.v[0];
                    rec->r2.v[1] = orig->cmpos.v[1] + rec->r2.v[1];
                    rec->r2.v[2] = orig->cmpos.v[2] + rec->r2.v[2];
                    BrMatrix34ApplyP(&tmp, &rec->r2, &orig->transform_matrix);
                    DrMatrix34ApplyLPInverse(&rec->r2, &tmp, &rec->object2->transform_matrix);
                    rec->r2.v[0] = rec->r2.v[0] - rec->object2->cmpos.v[0];
                    rec->r2.v[1] = rec->r2.v[1] - rec->object2->cmpos.v[1];
                    rec->r2.v[2] = rec->r2.v[2] - rec->object2->cmpos.v[2];
                    BrMatrix34ApplyV(&tmp, &rec->field_0x38, &orig->transform_matrix);
                    BrMatrix34TApplyV(&rec->field_0x38, &tmp, &rec->object2->transform_matrix);
                }
            }
        }
    }

    contact_count = count;
    total = CopyPhysicsJointData((tPhysics_joint_data*)pBuffer1, count, (const tPhysics_joint_data*)pBuffer2, pArg);
    joint_rows = 0;
    group_count = 0;
    obj = pObject;
    do {
        group = (tPhysics_group_data*)(uintptr_t)obj->field_0x218;
        if (group != NULL) {
            groups[group_count] = group;
            group_count++;
            p = group->data;
            for (k = 0; k < group->count; k++) {
                rows[joint_rows] = p;
                joint_rows++;
                p++;
            }
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);
    if (total > 0) {
        for (i = 0; i < total; i++) {
            rows[joint_rows + i] = &contacts[i];
        }
    }
    if (joint_rows + total > 100) {
        total = 100 - joint_rows;
    }
    if (contact_count > total) {
        contact_count = total;
    }
    nrows = joint_rows + total;
    a_c = solve_a + joint_rows;
    b_c = solve_b + joint_rows;
    rows_c = rows + joint_rows;
    FUN_004bf490((int*)gPhysics_buffer3_ptr, 100, (int*)rows, nrows);

    obj = pObject;
    do {
        obj->omega.v[0] = obj->rotate_omega.v[0];
        obj->omega.v[1] = obj->rotate_omega.v[1];
        obj->omega.v[2] = obj->rotate_omega.v[2];
        for (child = obj->child; child != NULL; child = child->next) {
            child->omega.v[0] = child->rotate_omega.v[0];
            child->omega.v[1] = child->rotate_omega.v[1];
            child->omega.v[2] = child->rotate_omega.v[2];
            if (child->child != NULL) {
                ResetOmegaTree(child->child);
            }
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);

    any = 0;
    solve_result = 0;
    if (gINT_0065d014 != 0) {
        solve_result = FUN_004c0430(b_c, rows_c, total, contact_count);
    } else {
        if (contact_count < total) {
            for (i = contact_count; i < total; i++) {
                b_c[i] = 0.0f;
            }
        }
        if (contact_count > 0) {
            for (i = 0; i < contact_count; i++) {
                rec = rows_c[i];
                o1 = rec->object1;
                if (rec->type == 0xb) {
                    value = -(o1->omega.v[2] * rec->normal.v[2] + rec->normal.v[1] * o1->omega.v[1] + rec->normal.v[0] * o1->omega.v[0]);
                    b_c[i] = value;
                    o2 = rec->object2;
                    if (o2 != NULL) {
                        b_c[i] = o2->omega.v[0] * rec->field_0x38.v[0] + rec->field_0x38.v[1] * o2->omega.v[1] + rec->field_0x38.v[2] * o2->omega.v[2] + b_c[i];
                    }
                    if (b_c[i] < gDOUBLE_0058a7a0) {
                        b_c[i] = 0.001f;
                    } else {
                        any = 1;
                    }
                } else {
                    cross.v[0] = o1->omega.v[1] * rec->r1.v[2] - rec->r1.v[1] * o1->omega.v[2];
                    cross.v[1] = o1->omega.v[2] * rec->r1.v[0] - rec->r1.v[2] * o1->omega.v[0];
                    cross.v[2] = o1->omega.v[0] * rec->r1.v[1] - rec->r1.v[0] * o1->omega.v[1];
                    cross.v[0] = o1->velocity_car_space.v[0] + cross.v[0];
                    cross.v[1] = o1->velocity_car_space.v[1] + cross.v[1];
                    cross.v[2] = o1->velocity_car_space.v[2] + cross.v[2];
                    b_c[i] = -(rec->normal.v[0] * cross.v[0] + rec->normal.v[2] * cross.v[2] + rec->normal.v[1] * cross.v[1]);
                    o2 = rec->object2;
                    if (o2 != NULL) {
                        cross.v[0] = o2->omega.v[1] * rec->r2.v[2] - rec->r2.v[1] * o2->omega.v[2];
                        cross.v[1] = o2->omega.v[2] * rec->r2.v[0] - rec->r2.v[2] * o2->omega.v[0];
                        cross.v[2] = o2->omega.v[0] * rec->r2.v[1] - rec->r2.v[0] * o2->omega.v[1];
                        cross.v[0] = o2->velocity_car_space.v[0] + cross.v[0];
                        cross.v[1] = o2->velocity_car_space.v[1] + cross.v[1];
                        cross.v[2] = o2->velocity_car_space.v[2] + cross.v[2];
                        b_c[i] = rec->field_0x38.v[2] * cross.v[2] + rec->field_0x38.v[1] * cross.v[1] + rec->field_0x38.v[0] * cross.v[0] + b_c[i];
                    }
                    if (b_c[i] < gFLOAT_0058a768) {
                        b_c[i] = 0.0f;
                    } else if (b_c[i] > gDOUBLE_0058a798) {
                        any = 1;
                    }
                    b_c[i] = gDOUBLE_0058a798 - b_c[i] * gDOUBLE_0058a7d8;
                }
            }
        }
        if (any) {
            solve_result = 0;
        } else {
            if (contact_count > 0) {
                for (i = 0; i < contact_count; i++) {
                    rec = rows_c[i];
                    if (rec->type == 0xb) {
                        b_c[i] = (gDOUBLE_0058a7a0 - rec->impulse) * gDOUBLE_0058a7e0;
                    } else {
                        o1 = rec->object1;
                        o2 = rec->object2;
                        if (o2 != NULL) {
                            tmp.v[0] = o1->cmpos.v[0] + rec->r1.v[0];
                            tmp.v[1] = o1->cmpos.v[1] + rec->r1.v[1];
                            tmp.v[2] = o1->cmpos.v[2] + rec->r1.v[2];
                            BrMatrix34ApplyP(&cross, &tmp, &o1->transform_matrix);
                            DrMatrix34ApplyLPInverse(&tmp2, &cross, &o2->transform_matrix);
                            BrMatrix34ApplyP(&cross, &tmp, &o1->actor->t.t.mat);
                            DrMatrix34ApplyLPInverse(&torque, &cross, &o2->actor->t.t.mat);
                            tmp2.v[0] = torque.v[0] - tmp2.v[0];
                            tmp2.v[1] = torque.v[1] - tmp2.v[1];
                            tmp2.v[2] = torque.v[2] - tmp2.v[2];
                            if (rec->type == 5) {
                                BrMatrix34ApplyV(&cross, &rec->field_0x38, &o2->transform_matrix);
                                BrMatrix34TApplyV(&torque, &cross, &o2->actor->t.t.mat);
                                b_c[i] = -((torque.v[0] * tmp2.v[0] + torque.v[1] * tmp2.v[1] + torque.v[2] * tmp2.v[2]) * gFLOAT_0058a7c8);
                            } else {
                                b_c[i] = -((rec->field_0x38.v[0] * tmp2.v[0] + rec->field_0x38.v[1] * tmp2.v[1] + rec->field_0x38.v[2] * tmp2.v[2]) * gFLOAT_0058a7c8);
                            }
                        } else {
                            tmp.v[0] = o1->cmpos.v[0] + rec->r1.v[0];
                            tmp.v[1] = o1->cmpos.v[1] + rec->r1.v[1];
                            tmp.v[2] = o1->cmpos.v[2] + rec->r1.v[2];
                            BrMatrix34ApplyP(&cross, &tmp, &o1->actor->t.t.mat);
                            DrMatrix34ApplyLPInverse(&tmp2, &cross, &o1->transform_matrix);
                            tmp2.v[0] = tmp2.v[0] - tmp.v[0];
                            tmp2.v[1] = tmp2.v[1] - tmp.v[1];
                            tmp2.v[2] = tmp2.v[2] - tmp.v[2];
                            b_c[i] = gDOUBLE_0058a798 - (rec->normal.v[0] * tmp2.v[0] + rec->normal.v[2] * tmp2.v[2] + rec->normal.v[1] * tmp2.v[1]) * gFLOAT_0058a7c8;
                        }
                    }
                    if (b_c[i] < gDOUBLE_0058a798) {
                        b_c[i] = 0.01f;
                    } else {
                        any = 1;
                    }
                }
            }
            if (!any && contact_count > 0) {
                for (i = 0; i < contact_count; i++) {
                    if (rows_c[i]->type != 0xb) {
                        b_c[i] = 0.05f;
                    }
                }
            }
            solve_result = 1;
        }
    }

    FUN_004b77b0((int*)solve_b, (int*)rows, joint_rows);
    if (FUN_004c3b60((float*)gPhysics_buffer3_ptr, solve_b, solve_a, nrows, 100, (int)(uintptr_t)solve_flags, (int*)solve_other, joint_rows, contact_count, (undefined4)(uintptr_t)groups, group_count) != 0) {
        gINT_006940a4++;
    }

    if (gINT_00692dd8 != 0) {
        changed = 0;
        obj = pObject;
        do {
            joint = obj->physics_joint2;
            if (joint != NULL && obj->field_0x1e0[0] != 0) {
                n = joint->count_limits;
                idx = 0;
                if (n > 0) {
                    do {
                        if (joint->limits[idx].type == 10) {
                            break;
                        }
                        idx++;
                    } while (idx < n);
                }
                if (idx != n) {
                    thr = joint->limits[idx].value;
                    BrVector3SetFloat(&torque, 0.0f, 0.0f, 0.0f);
                    if (total > 0) {
                        for (i = 0; i < total; i++) {
                            rec = &contacts[i];
                            lam = a_c[i];
                            if (rec->object1 == obj) {
                                dx = rec->r1.v[0] - joint->field_0x08.v[0];
                                dy = rec->r1.v[1] - joint->field_0x08.v[1];
                                dz = rec->r1.v[2] - joint->field_0x08.v[2];
                                cross.v[0] = lam * (rec->normal.v[2] * dy - rec->normal.v[1] * dz);
                                cross.v[1] = lam * (rec->normal.v[0] * dz - rec->normal.v[2] * dx);
                                cross.v[2] = lam * (rec->normal.v[1] * dx - rec->normal.v[0] * dy);
                                torque.v[1] = cross.v[1] + torque.v[1];
                                torque.v[0] = cross.v[0] + torque.v[0];
                                torque.v[2] = cross.v[2] + torque.v[2];
                            } else if (rec->object2 == obj) {
                                dx = rec->r2.v[0] - joint->field_0x08.v[0];
                                dy = rec->r2.v[1] - joint->field_0x08.v[1];
                                dz = rec->r2.v[2] - joint->field_0x08.v[2];
                                step = -lam;
                                cross.v[0] = (rec->field_0x38.v[2] * dy - rec->field_0x38.v[1] * dz) * step;
                                cross.v[1] = (rec->field_0x38.v[0] * dz - rec->field_0x38.v[2] * dx) * step;
                                cross.v[2] = (rec->field_0x38.v[1] * dx - rec->field_0x38.v[0] * dy) * step;
                                torque.v[1] = cross.v[1] + torque.v[1];
                                torque.v[0] = cross.v[0] + torque.v[0];
                                torque.v[2] = cross.v[2] + torque.v[2];
                            }
                        }
                    }
                    if (torque.v[2] * torque.v[2] + torque.v[1] * torque.v[1] + torque.v[0] * torque.v[0] <= thr) {
                        obj->disable_move_rotate = 1;
                    } else {
                        changed = 1;
                        obj->field_0x1e0[0] = 0;
                        if (total > 0) {
                            for (i = 0; i < total; i++) {
                                if (contacts[i].object1 == obj || contacts[i].object2 == obj) {
                                    contacts[i].type = 5;
                                }
                            }
                        }
                    }
                }
            }
            obj = obj->field183_0x1d8;
        } while (obj != pObject);
        if (changed) {
            FUN_004bf490((int*)gPhysics_buffer3_ptr, 100, (int*)rows, nrows);
            FUN_004c3b60((float*)gPhysics_buffer3_ptr, solve_b, solve_a, nrows, 100, (int)(uintptr_t)solve_flags, (int*)solve_other, joint_rows, contact_count, (undefined4)(uintptr_t)groups, group_count);
        }
    }

    max_value = gFLOAT_0058a768;
    for (i = 0; i < contact_count; i++) {
        if (max_value < a_c[i]) {
            max_value = a_c[i];
        }
    }
    for (i = 0; i < joint_rows; i++) {
        if (gFLOAT_0058a7e8 < solve_a[i]) {
            i++;
        }
    }
    for (i = 0; i < total; i++) {
        if (a_c[i] == gFLOAT_0058a768) {
            contacts[i].type = 9;
        }
    }
    gINT_00692dc8 = 0;
    if (pWorld_callbacks->process_forces != NULL) {
        pWorld_callbacks->process_forces(pBuffer1, a_c, total);
    }
    if (gINT_00692dc8 == 0) {
        if (pWorld_callbacks->process_joint_forces != NULL) {
            pWorld_callbacks->process_joint_forces((undefined4)(uintptr_t)rows, (undefined4)(uintptr_t)solve_a, joint_rows);
        }
        if (gINT_00692dc8 == 0) {
            for (i = 0; i < joint_rows; i++) {
                FUN_004b7960((tPhysics_joint_data*)rows[i], solve_a[i]);
            }
        }
    }
    for (i = 0; i < total; i++) {
        rec = &contacts[i];
        value = a_c[i];
        if (value != gFLOAT_0058a768 && rec->type != 9) {
            o2 = rec->object2;
            o1 = rec->object1;
            FUN_004b7960((tPhysics_joint_data*)rec, value);
            if (rec->type != 0xb) {
                if (o2 != NULL) {
                    if ((o1->collision_flag & 2) != 0 && (o2->collision_flag & 2) != 0) {
                        continue;
                    }
                } else if ((o1->collision_flag & 1) != 0) {
                    continue;
                }
                if (gINT_00692ddc < 0x32) {
                    gCollision_contacts[gINT_00692ddc] = *rec;
                    gCollision_contacts[gINT_00692ddc].impulse = a_c[i];
                    gINT_00692ddc++;
                }
            }
        }
    }
    if (solve_result != 0 && gINT_00692dc8 == 0) {
        FUN_004bc190((int*)rows, solve_a, joint_rows, (int)(uintptr_t)pWorld_callbacks);
    }

    ring_length = 0;
    obj = pObject;
    do {
        ring_length++;
        if (ring_length > 100) {
            PhysicsError((tPhysicsError)10, "Corrupt group");
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);

    sum_all = 0;
    obj = pObject;
    do {
        sum_all += FUN_004bd720(obj);
        child = obj->child;
        if (child != NULL) {
            sum_children = 0;
            for (; child != NULL; child = child->next) {
                sum_children += FUN_004bd720(child);
                if (child->child != NULL) {
                    sum_children += FUN_004bd7f0(child->child);
                }
            }
            sum_all += sum_children;
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);

    if (sum_all != 0) {
        obj = pObject;
        do {
            group = (tPhysics_group_data*)(uintptr_t)obj->field_0x218;
            if (group != NULL) {
                p = group->data;
                for (k = 0; k < group->count; k++) {
                    group_rows[k] = p;
                    p++;
                }
                FUN_004b77b0((int*)group_b, (int*)group_rows, group->count);
                RqBackSub(group->field_0x18, group->field_0x14, group_b, group_a, group->count);
                for (k = 0; k < group->count; k++) {
                    FUN_004b7960((tPhysics_joint_data*)group_rows[k], group_a[k]);
                }
            }
            obj = obj->field183_0x1d8;
        } while (obj != pObject);
    }

    flags_acc = 0;
    obj = pObject;
    do {
        if (obj->disable_move_rotate == 0) {
            if (obj->field_0xf0 == 1) {
                BrMatrix34Copy(&obj->actor->t.t.mat, (const br_matrix34*)&obj->field_0xbc);
            } else {
                BrMatrix34LPNormalise(&obj->actor->t.t.mat, &obj->transform_matrix);
            }
            for (child = obj->child; child != NULL; child = child->next) {
                ResetObjectAndChildren(child);
            }
            FUN_004b8ee0(obj CARPOCALYPSE2_THISCALL_EDX, 0.04f);
            MergeBoundsIntoParent(obj, NULL);
            if (obj->field_0x194 != 0) {
                flags_acc |= ((int (C2_HOOK_FASTCALL *)(tPhysics_object*))(uintptr_t)obj->field_0x194)(obj);
            }
        }
        obj = obj->field183_0x1d8;
    } while (obj != pObject);
    if ((flags_acc | sum_all) != 0) {
        return -total;
    }
    return total;
}

// FUNCTION: CARMA2_HW 0x00428cf0
int C2_HOOK_FASTCALL CopyPhysicsJointData(tPhysics_joint_data* pDestination, int pDestination_index, const tPhysics_joint_data* pSource, int pCount) {
    int i;

    for (i = 0; i < pCount && pDestination_index < 40; i++) {
        if (pSource[i].field_0x5c != 9) {
            memcpy(&pDestination[pDestination_index], &pSource[i], sizeof(*pSource));
            pDestination_index++;
        }
    }
    return pDestination_index;
}

// C2PORT-NONMATCHING: best 28.52% | difftest PASS 2000 trials (spec difftest/specs/004bc190_FUN_004bc190.json) | reason x87 scheduling, register allocation, and stack layout
// FUNCTION: CARMA2_HW 0x004bc190
void C2_HOOK_FASTCALL FUN_004bc190(int* pObject_list, float* pVectors, int pCount, int pUnused) {
  int *piVar1;
  float *pfVar2;
  float *pfVar3;
  double fVar4;
  float fVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 *puVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  int iVar14;
  float *pfVar15;
  float *pfVar16;
  int *piVar17;
  float *pfVar18;
  int *piVar19;
  int *local_a0;
  float *local_9c;
  int local_98;
  int local_90;
  br_vector3 local_60;
  float local_48;
  br_vector3 local_3c;
  br_vector3 local_24;
  float local_20;
  br_matrix34 local_matrix;

  (void)pUnused;
  local_98 = 0;
  local_90 = 0;
  if (0 < pCount) {
    do {
      piVar1 = pObject_list + local_98;
      iVar6 = *(int *)(pObject_list[local_98] + 4);
      iVar14 = local_98;
      if ((*(int *)(iVar6 + 0x224) != 0) && (local_98 < pCount)) {
        pfVar2 = pVectors + local_98;
        pfVar3 = pVectors + 2 + local_98;
        piVar17 = pObject_list + 2 + local_98;
        iVar14 = local_98 * 4 + 4;
        pfVar15 = pVectors + local_98 + 1;
        piVar19 = pObject_list + local_98 + 1;
        pfVar16 = pfVar15;
        pfVar18 = pfVar2;
        local_a0 = piVar1;
        local_9c = pfVar3;
        do {
          iVar14 = *local_a0;
          if ((*(int *)(iVar14 + 8) == iVar6) &&
             (*(float *)(iVar6 + 0x1c8) * *(float *)(iVar6 + 0x1c8) +
              *(float *)(iVar6 + 0x1c4) * *(float *)(iVar6 + 0x1c4) +
              *(float *)(iVar6 + 0x1c0) * *(float *)(iVar6 + 0x1c0) <
              *(float *)(iVar6 + 0x7c) * *(float *)(iVar6 + 0x7c) +
              *(float *)(iVar6 + 0x78) * *(float *)(iVar6 + 0x78) +
              *(float *)(iVar6 + 0x74) * *(float *)(iVar6 + 0x74))) {
            iVar7 = *(int *)(iVar14 + 4);
            fVar10 = *(float *)(*(int *)(iVar6 + 0x188) + 8) -
                     *(float *)(*(int *)(iVar7 + 0x188) + 0x14);
            fVar11 = *(float *)(*(int *)(iVar6 + 0x188) + 0xc) -
                     *(float *)(*(int *)(iVar7 + 0x188) + 0x18);
            fVar12 = *(float *)(*(int *)(iVar6 + 0x188) + 0x10) -
                     *(float *)(*(int *)(iVar7 + 0x188) + 0x1c);
            fVar4 = sqrt(fVar12 * fVar12 + fVar11 * fVar11 + fVar10 * fVar10);
            fVar10 = fVar10 / fVar4;
            fVar11 = fVar11 / fVar4;
            fVar12 = fVar12 / fVar4;
            fVar4 = *(float *)(iVar6 + 4) * gDOUBLE_0058a7f0;
            if (fVar4 <= *pfVar3 * fVar12 + *pfVar2 * fVar10 + *pfVar15 * fVar11) {
              iVar7 = *piVar19;
              iVar8 = *piVar17;
              if (fVar4 <= (*(float *)(iVar8 + 0x40) * fVar12 +
                           *(float *)(iVar8 + 0x38) * fVar10 + *(float *)(iVar8 + 0x3c) * fVar11) *
                           *local_9c +
                           *pfVar16 *
                           (*(float *)(iVar7 + 0x40) * fVar12 +
                           *(float *)(iVar7 + 0x38) * fVar10 + *(float *)(iVar7 + 0x3c) * fVar11) +
                           *pfVar18 *
                           (*(float *)(iVar14 + 0x3c) * fVar11 +
                           *(float *)(iVar14 + 0x38) * fVar10 + *(float *)(iVar14 + 0x40) * fVar12))
              {
                local_20 = sqrt(*(float *)(iVar6 + 0x7c) * *(float *)(iVar6 + 0x7c) +
                                 *(float *)(iVar6 + 0x78) * *(float *)(iVar6 + 0x78) +
                                 *(float *)(iVar6 + 0x74) * *(float *)(iVar6 + 0x74));
                local_3c.v[0] = *(float *)(iVar6 + 0x74) / local_20;
                local_3c.v[1] = *(float *)(iVar6 + 0x78) / local_20;
                local_3c.v[2] = *(float *)(iVar6 + 0x7c) / local_20;
                BrMatrix34Rotate(&local_matrix, (br_angle)(local_20 * gFLOAT_0058a7f8 * gDOUBLE_0058a770), &local_3c);
                local_24.v[0] = fVar10;
                local_24.v[1] = fVar11;
                local_24.v[2] = fVar12;
                BrMatrix34ApplyV(&local_60, &local_24, &local_matrix);
                fVar4 = *pfVar2;
                fVar5 = *pfVar15;
                local_48 = *pfVar3;
                fVar13 = sqrt(local_48 * local_48 + fVar5 * fVar5 + fVar4 * fVar4);
                if (fVar13 <= gFLOAT_0058a7fc) {
                  local_48 = 0.0;
                  fVar4 = gFLOAT_0058a800;
                  fVar5 = gFLOAT_0058a768;
                }
                else {
                  fVar13 = (float)gDOUBLE_0058a780 / fVar13;
                  local_48 = fVar13 * local_48;
                  fVar4 = fVar13 * fVar4;
                  fVar5 = fVar13 * fVar5;
                }
                if ((fVar10 * fVar5 - fVar11 * fVar4) * (fVar5 * local_60.v[0] - fVar4 * local_60.v[1]) +
                    (fVar12 * fVar4 - fVar10 * local_48) * (fVar4 * local_60.v[2] - local_48 * local_60.v[0])
                    + (fVar11 * local_48 - fVar12 * fVar5) *
                      (local_48 * local_60.v[1] - fVar5 * local_60.v[2]) < gFLOAT_0058a768) {
                  fVar10 = *(float *)(iVar6 + 0x78) * gFLOAT_0058a804;
                  fVar4 = *(float *)(iVar6 + 0x7c) * gFLOAT_0058a804;
                  *(float *)(iVar6 + 0x74) = *(float *)(iVar6 + 0x74) * gFLOAT_0058a804;
                  *(float *)(iVar6 + 0x78) = fVar10;
                  *(float *)(iVar6 + 0x7c) = fVar4;
                  FUN_004bc8e0((tPhysics_object*)iVar6, 25.0f);
                }
              }
            }
          }
          iVar14 = *local_a0;
          puVar9 = *(undefined4 **)(*(int *)(iVar14 + 4) + 0x188);
          if (puVar9 != (undefined4 *)0x0) {
            switch(*puVar9) {
            case 1:
              pfVar18 = pfVar18 + 5;
              local_98 = local_98 + 5;
              piVar19 = piVar19 + 5;
              local_9c = local_9c + 5;
              pfVar16 = pfVar16 + 5;
              piVar17 = piVar17 + 5;
              local_a0 = local_a0 + 5;
              break;
            case 2:
              pfVar18 = pfVar18 + 4;
              local_98 = local_98 + 4;
              piVar19 = piVar19 + 4;
              local_9c = local_9c + 4;
              pfVar16 = pfVar16 + 4;
              piVar17 = piVar17 + 4;
              local_a0 = local_a0 + 4;
              break;
            case 3:
              pfVar18 = pfVar18 + 3;
              local_98 = local_98 + 3;
              piVar19 = piVar19 + 3;
              local_9c = local_9c + 3;
              pfVar16 = pfVar16 + 3;
              piVar17 = piVar17 + 3;
              local_a0 = local_a0 + 3;
              break;
            case 4:
              pfVar18 = pfVar18 + 2;
              local_98 = local_98 + 2;
              piVar19 = piVar19 + 2;
              local_9c = local_9c + 2;
              pfVar16 = pfVar16 + 2;
              piVar17 = piVar17 + 2;
              local_a0 = local_a0 + 2;
              break;
            default:
              ((void (C2_HOOK_FASTCALL *)(tPhysicsError, const char*))PhysicsError)(ePhysicsError_UnknownHingeType, gEmpty_string_00676920);
            }
          }
          puVar9 = *(undefined4 **)(*(int *)(iVar14 + 4) + 0x18c);
          if (puVar9 != (undefined4 *)0x0) {
            switch(*puVar9) {
            case 1:
              local_98 = local_98 + 5;
              iVar14 = 0x14;
              break;
            case 2:
              local_98 = local_98 + 4;
              iVar14 = 0x10;
              break;
            case 3:
              local_98 = local_98 + 3;
              iVar14 = 0xc;
              break;
            default:
              ((void (C2_HOOK_FASTCALL *)(tPhysicsError, const char*))PhysicsError)(ePhysicsError_UnknownHingeType, gEmpty_string_00676920);
              goto LAB_004bc7a6;
            case 5:
              local_98 = local_98 + 2;
              iVar14 = 8;
            }
            pfVar18 = (float *)((int)pfVar18 + iVar14);
            local_9c = (float *)((int)local_9c + iVar14);
            piVar19 = (int *)((int)piVar19 + iVar14);
            pfVar16 = (float *)((int)pfVar16 + iVar14);
            piVar17 = (int *)((int)piVar17 + iVar14);
            local_a0 = (int *)((int)local_a0 + iVar14);
          }
LAB_004bc7a6:
          iVar14 = local_90;
        } while (local_98 < pCount);
      }
      iVar6 = *piVar1;
      puVar9 = *(undefined4 **)(*(int *)(iVar6 + 4) + 0x188);
      if (puVar9 != (undefined4 *)0x0) {
        switch(*puVar9) {
        case 1:
          iVar14 = iVar14 + 5;
          local_90 = iVar14;
          break;
        case 2:
          iVar14 = iVar14 + 4;
          local_90 = iVar14;
          break;
        case 3:
          iVar14 = iVar14 + 3;
          local_90 = iVar14;
          break;
        case 4:
          iVar14 = iVar14 + 2;
          local_90 = iVar14;
          break;
        default:
          ((void (C2_HOOK_FASTCALL *)(tPhysicsError, const char*))PhysicsError)(ePhysicsError_UnknownHingeType, gEmpty_string_00676920);
          iVar14 = local_90;
        }
      }
      puVar9 = *(undefined4 **)(*(int *)(iVar6 + 4) + 0x18c);
      local_98 = iVar14;
      if (puVar9 != (undefined4 *)0x0) {
        switch(*puVar9) {
        case 1:
          local_98 = iVar14 + 5;
          local_90 = local_98;
          break;
        case 2:
          local_98 = iVar14 + 4;
          local_90 = local_98;
          break;
        case 3:
          local_98 = iVar14 + 3;
          local_90 = local_98;
          break;
        default:
          ((void (C2_HOOK_FASTCALL *)(tPhysicsError, const char*))PhysicsError)(ePhysicsError_UnknownHingeType, gEmpty_string_00676920);
          local_98 = local_90;
          break;
        case 5:
          local_98 = iVar14 + 2;
          local_90 = local_98;
        }
      }
    } while (local_98 < pCount);
  }
  return;
}



#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 12.31% | difftest PASS 2000 trials (spec difftest/specs/004bca10_FUN_004bca10.json) | reason VC5 register allocation and stack layout
// FUNCTION: CARMA2_HW 0x004bca10
int C2_HOOK_FASTCALL FUN_004bca10(tPhysics_object* pObject, undefined4* pCollision_data, int pCollision_count, int pCollision_capacity) {
    tCollision_contact* contacts;

    contacts = (tCollision_contact*)pCollision_data;
    {
        tPhysics_object* other;
        tPhysics_joint* joint;
        tPhysics_joint_limit* limit;
        tPhysics_joint_limit* next_limit;
        tCollision_contact* contact;
        br_vector3 point_world;
        br_vector3 point_local;
        br_vector3 other_point;
        br_vector3 transformed;
        br_vector3 direction;
        br_vector3 cross;
        float value1;
        float value2;
        float value3;
        float length;
        float scale;
        int i;

        joint = pObject->physics_joint1;
        if (joint != NULL) {
            other = pObject->field_0x230;
        if (other == NULL) {
            PhysicsError(11, gEmpty_string_00676920);
        }
        if (joint->count_limits > 0) {
            for (i = 0; i < joint->count_limits; i++) {
                if (pCollision_capacity <= pCollision_count) {
                    break;
                }
                limit = &joint->limits[i];
                contact = &contacts[pCollision_count];
                if (limit->type == eJoint_limit_plane) {
                    BrMatrix34ApplyV(&point_world, &limit->child, &pObject->actor->t.t.mat);
                    BrMatrix34TApplyV(&point_local, &point_world, &other->actor->t.t.mat);
                    value1 = limit->parent.v[0] * point_local.v[0]
                           + limit->parent.v[1] * point_local.v[1]
                           + limit->parent.v[2] * point_local.v[2];
                    if (value1 < gDOUBLE_0058a7a0) {
                        BrMatrix34ApplyV(&point_world, &limit->child, &pObject->transform_matrix);
                        BrMatrix34TApplyV(&other_point, &point_world, &other->transform_matrix);
                        value2 = limit->parent.v[0] * other_point.v[0]
                               + limit->parent.v[1] * other_point.v[1]
                               + limit->parent.v[2] * other_point.v[2];
                        if (gFLOAT_0058a768 <= value2
                                && (gDOUBLE_0058a7a0 <= value2 || pObject->field_0xf0 != 0)) {
                            contact->impulse = value1 - value2;
                            contact->type = 0xb;
                            contact->object1 = pObject;
                            contact->object2 = other;
                            cross.v[0] = limit->parent.v[2] * other_point.v[1] - limit->parent.v[1] * other_point.v[2];
                            cross.v[1] = limit->parent.v[0] * other_point.v[2] - limit->parent.v[2] * other_point.v[0];
                            cross.v[2] = limit->parent.v[1] * other_point.v[0] - limit->parent.v[0] * other_point.v[1];
                            length = sqrtf(cross.v[0] * cross.v[0] + cross.v[1] * cross.v[1] + cross.v[2] * cross.v[2]);
                            if (length <= gDOUBLE_0058a810) {
                                BrMatrix34ApplyV(&transformed, &pObject->v, &pObject->transform_matrix);
                                BrMatrix34TApplyV(&contact->field_0x38, &transformed, &other->transform_matrix);
                                contact->field_0x38.v[0] -= other->v.v[0];
                                contact->field_0x38.v[1] -= other->v.v[1];
                                contact->field_0x38.v[2] -= other->v.v[2];
                                length = sqrtf(contact->field_0x38.v[0] * contact->field_0x38.v[0]
                                        + contact->field_0x38.v[1] * contact->field_0x38.v[1]
                                        + contact->field_0x38.v[2] * contact->field_0x38.v[2]);
                                if (length <= gFLOAT_0058a7fc) {
                                    contact->field_0x38.v[0] = 1.0f;
                                    contact->field_0x38.v[1] = 0.0f;
                                    contact->field_0x38.v[2] = 0.0f;
                                } else {
                                    scale = (float)(gDOUBLE_0058a780 / length);
                                    contact->field_0x38.v[0] *= scale;
                                    contact->field_0x38.v[1] *= scale;
                                    contact->field_0x38.v[2] *= scale;
                                }
                            } else {
                                contact->field_0x38.v[0] = cross.v[0] / length;
                                contact->field_0x38.v[1] = cross.v[1] / length;
                                contact->field_0x38.v[2] = cross.v[2] / length;
                            }
                            BrMatrix34ApplyV(&transformed, &contact->field_0x38, &other->transform_matrix);
                            BrMatrix34TApplyV(&contact->normal, &transformed, &pObject->transform_matrix);
                            contact->r1 = joint->field_0x08;
                            contact->r2 = joint->field_0x14;
                            pCollision_count++;
                        }
                    }
                } else if (limit->type == eJoint_limit_universal) {
                    next_limit = &joint->limits[i + 1];
                    FUN_004bd4f0(&pObject->actor->t.t.mat,
                            &other->actor->t.t.mat,
                            &limit->child,
                            &limit->parent,
                            &next_limit->child,
                            &next_limit->parent,
                            &point_world,
                            &direction);
                    value1 = next_limit->parent.v[0] * point_world.v[0]
                           + next_limit->parent.v[1] * point_world.v[1]
                           + next_limit->parent.v[2] * point_world.v[2];
                    if (value1 < limit->value - gDOUBLE_0058a808) {
                        FUN_004bd4f0(&pObject->transform_matrix,
                                &other->transform_matrix,
                                &limit->child,
                                &limit->parent,
                                &next_limit->child,
                                &next_limit->parent,
                                &other_point,
                                &contact->field_0x38);
                        value2 = next_limit->parent.v[0] * other_point.v[0]
                               + next_limit->parent.v[1] * other_point.v[1]
                               + next_limit->parent.v[2] * other_point.v[2];
                        if (limit->value < value2) {
                            cross.v[0] = point_world.v[1] * other_point.v[2] - point_world.v[2] * other_point.v[1];
                            cross.v[1] = point_world.v[2] * other_point.v[0] - point_world.v[0] * other_point.v[2];
                            cross.v[2] = point_world.v[0] * other_point.v[1] - point_world.v[1] * other_point.v[0];
                            value3 = contact->field_0x38.v[0] * cross.v[0]
                                   + contact->field_0x38.v[1] * cross.v[1]
                                   + contact->field_0x38.v[2] * cross.v[2];
                            if (value3 < gFLOAT_0058a768) {
                                contact->field_0x38.v[0] = -contact->field_0x38.v[0];
                                contact->field_0x38.v[1] = -contact->field_0x38.v[1];
                                contact->field_0x38.v[2] = -contact->field_0x38.v[2];
                            }
                            BrMatrix34ApplyV(&transformed, &contact->field_0x38, &other->transform_matrix);
                            BrMatrix34TApplyV(&contact->normal, &transformed, &pObject->transform_matrix);
                            contact->impulse = value1 - value2;
                            contact->type = 0xb;
                            contact->object1 = pObject;
                            contact->object2 = other;
                            pCollision_count++;
                        }
                    }
                    i++;
                } else {
                    PhysicsError(8, gEmpty_string_00676920);
                }
            }
        }
        }
    }

    {
    tPhysics_joint* joint;
    tPhysics_joint_limit* limit;
    tCollision_contact* contact;
    br_vector3 axis_world;
    br_vector3 direction;
    br_vector3 cross;
    br_vector3 transformed;
    float value1;
    float value2;
    float value3;
    float value4;
    float length;
    float scale;
    int i;
    int j;

    joint = pObject->physics_joint2;
    if (joint == NULL || joint->count_limits < 1) {
        return pCollision_count;
    }
    for (i = 0; i < joint->count_limits; i++) {
        limit = &joint->limits[i];
        if (limit->type == eJoint_limit_plane) {
            BrMatrix34ApplyV(&axis_world, &limit->child, &pObject->actor->t.t.mat);
            value1 = limit->parent.v[0] * axis_world.v[0]
                   + limit->parent.v[1] * axis_world.v[1]
                   + limit->parent.v[2] * axis_world.v[2];
            if (value1 < gDOUBLE_0058a7a0) {
                if (pCollision_count == pCollision_capacity) {
                    return pCollision_count;
                }
                contact = &contacts[pCollision_count];
                contact->impulse = value1;
                contact->type = 0xb;
                contact->object1 = pObject;
                contact->object2 = NULL;
                cross.v[0] = limit->parent.v[2] * axis_world.v[1] - limit->parent.v[1] * axis_world.v[2];
                cross.v[1] = limit->parent.v[0] * axis_world.v[2] - limit->parent.v[2] * axis_world.v[0];
                cross.v[2] = limit->parent.v[1] * axis_world.v[0] - limit->parent.v[0] * axis_world.v[1];
                length = sqrtf(cross.v[0] * cross.v[0] + cross.v[1] * cross.v[1] + cross.v[2] * cross.v[2]);
                if (gDOUBLE_0058a810 < length) {
                    contact->field_0x38.v[0] = cross.v[0] / length;
                    contact->field_0x38.v[1] = cross.v[1] / length;
                    contact->field_0x38.v[2] = cross.v[2] / length;
                }
            }
        } else if (limit->type == eJoint_limit_10) {
            if (pObject->field_0x1e0[0] != 0) {
                gINT_00692dd8 = 1;
                j = 0;
                while (j < pCollision_count) {
                    contact = &contacts[j];
                    if (contact->object1 == pObject) {
                        if (contact->object2 == NULL) {
                            if (j < pCollision_count) {
                                memmove(contact, contact + 1, (pCollision_count - j) * sizeof(*contact));
                            }
                            pCollision_count--;
                            j--;
                        } else {
                            direction.v[0] = contact->normal.v[0];
                            direction.v[1] = contact->normal.v[1];
                            direction.v[2] = contact->normal.v[2];
                            contact->normal.v[0] = -contact->field_0x38.v[0];
                            contact->normal.v[1] = -contact->field_0x38.v[1];
                            contact->normal.v[2] = -contact->field_0x38.v[2];
                            contact->field_0x38.v[0] = -direction.v[0];
                            contact->field_0x38.v[1] = -direction.v[1];
                            contact->field_0x38.v[2] = -direction.v[2];
                            transformed = contact->r1;
                            contact->r1 = contact->r2;
                            contact->r2 = transformed;
                            contact->object1 = contact->object2;
                            contact->object2 = pObject;
                            contact->type = eJoint_limit_10 + 4;
                        }
                    } else if (contact->object2 == pObject) {
                        contact->type = eJoint_limit_10 + 4;
                    }
                    j++;
                }
            }
        } else if (limit->type == eJoint_limit_11) {
            BrMatrix34ApplyV(&axis_world, &joint->hinge_axis, &pObject->transform_matrix);
            value3 = pObject->field_0x17c.v[0] * axis_world.v[0]
                   + pObject->field_0x17c.v[1] * axis_world.v[1]
                   + pObject->field_0x17c.v[2] * axis_world.v[2];
            value2 = limit->value + value3;
            value1 = pObject->actor->t.t.mat.m[3][0] * axis_world.v[0]
                   + pObject->actor->t.t.mat.m[3][1] * axis_world.v[1]
                   + pObject->actor->t.t.mat.m[3][2] * axis_world.v[2];
            value4 = pObject->transform_matrix.m[3][0] * axis_world.v[0]
                   + pObject->transform_matrix.m[3][1] * axis_world.v[1]
                   + pObject->transform_matrix.m[3][2] * axis_world.v[2];
            if ((value1 <= value2 || value2 - gDOUBLE_0058a808 <= value4)
                    && value1 < value3 - gDOUBLE_0058a7a0
                    && value3 - gDOUBLE_0058a818 < value4) {
                if (pCollision_count == pCollision_capacity) {
                    return pCollision_count;
                }
                contact = &contacts[pCollision_count++];
                contact->impulse = (value3 - value4) / (value1 - value4);
                contact->field_0xc = NULL;
                contact->type = 2;
                contact->object1 = pObject;
                contact->object2 = NULL;
                contact->normal.v[0] = joint->hinge_axis.v[0];
                contact->normal.v[1] = joint->hinge_axis.v[1];
                contact->normal.v[2] = joint->hinge_axis.v[2];
                BrVector3SetFloat(&contact->r1, 0.0f, 0.0f, 0.0f);
            } else if (value1 > value2 && value2 - gDOUBLE_0058a808 > value4) {
                if (pCollision_count == pCollision_capacity) {
                    return pCollision_count;
                }
                contact = &contacts[pCollision_count++];
                contact->impulse = (value2 - value4) / (value1 - value4);
                contact->field_0xc = NULL;
                contact->type = 2;
                contact->object1 = pObject;
                contact->object2 = NULL;
                contact->normal.v[0] = -joint->hinge_axis.v[0];
                contact->normal.v[1] = -joint->hinge_axis.v[1];
                contact->normal.v[2] = -joint->hinge_axis.v[2];
                BrVector3SetFloat(&contact->r1, 0.0f, 0.0f, 0.0f);
            }
        } else {
            PhysicsError(8, gEmpty_string_00676920);
        }
    }
    return pCollision_count;
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 72.22% | difftest PASS 2000 trials (spec difftest/specs/004bc8e0_FUN_004bc8e0.json) | reason VC5 x87 scheduling and stack layout
// FUNCTION: CARMA2_HW 0x004bc8e0
void C2_HOOK_FAKE_THISCALL FUN_004bc8e0(tPhysics_object* pObject, br_scalar pScale) {
    tPhysics_object* pCurrent;
    int* piVar1;
    int local_a0[40];
    int iVar2;
    int iVar4;
    int* piVar3;

    pObject->I.v[0] = pScale * pObject->I.v[0];
    pObject->I.v[1] = pScale * pObject->I.v[1];
    pObject->I.v[2] = pScale * pObject->I.v[2];
    pObject->field_0x21c = pScale * pObject->field_0x21c;
    if (pObject->field_0x1e4 != 0) {
        iVar4 = *(int*)(pObject->field_0x1e4 + 4);
        *(float*)(iVar4 + 8) = pScale * *(float*)(iVar4 + 8);
        *(float*)(iVar4 + 0xc) = pScale * *(float*)(iVar4 + 0xc);
        *(float*)(iVar4 + 0x10) = pScale * *(float*)(iVar4 + 0x10);
        *(float*)(iVar4 + 0x21c) = pScale * *(float*)(iVar4 + 0x21c);
        FUN_004b8cf0(pObject);
    }
    pCurrent = pObject;
    while (pCurrent->field_0x218 == 0 && pCurrent->parent != NULL) {
        pCurrent = pCurrent->parent;
    }
    piVar1 = (int*)(uintptr_t)pCurrent->field_0x218;
    if (piVar1 != NULL) {
        iVar4 = 0;
        if (0 < *piVar1) {
            iVar2 = 0;
            piVar3 = local_a0;
            do {
                iVar4++;
                *piVar3 = iVar2 + piVar1[1];
                iVar2 += 0x60;
                piVar3++;
            } while (iVar4 < *piVar1);
        }
        FUN_004bf490((int*)(uintptr_t)piVar1[6], *piVar1, local_a0, *piVar1);
        FUN_004c5b50((tU32)(uintptr_t)piVar1[6], (tU32)piVar1[5], (tU32)*piVar1);
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 52.01% | difftest PASS 2000 trials (spec difftest/specs/004bd4f0_FUN_004bd4f0.json) | reason VC5 x87 scheduling/register allocation
// FUNCTION: CARMA2_HW 0x004bd4f0
void C2_HOOK_FASTCALL FUN_004bd4f0(const br_matrix34* pTransform, const br_matrix34* pInverse_transform, const br_vector3* pVector1, const br_vector3* pVector2, const br_vector3* pVector3, const br_vector3* pVector4, br_vector3* pVector5, br_vector3* pVector6) {
    br_vector3 transformed[2];
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;

    BrMatrix34ApplyV(&transformed[0], pVector1, pTransform);
    BrMatrix34TApplyV(&transformed[1], &transformed[0], pInverse_transform);
    pVector6->v[0] = transformed[1].v[0] + pVector2->v[0];
    pVector6->v[1] = transformed[1].v[1] + pVector2->v[1];
    pVector6->v[2] = transformed[1].v[2] + pVector2->v[2];
    fVar1 = sqrtf(pVector6->v[0] * pVector6->v[0] + pVector6->v[2] * pVector6->v[2] + pVector6->v[1] * pVector6->v[1]);
    if (fVar1 > gFLOAT_0058a7fc) {
        fVar1 = (float)(gDOUBLE_0058a780 / fVar1);
        pVector6->v[0] = fVar1 * pVector6->v[0];
        pVector6->v[1] = pVector6->v[1] * fVar1;
        pVector6->v[2] = pVector6->v[2] * fVar1;
    } else {
        pVector6->v[0] = 1.0f;
        pVector6->v[1] = 0.0f;
        pVector6->v[2] = 0.0f;
    }
    BrMatrix34ApplyV(&transformed[0], pVector3, pTransform);
    BrMatrix34TApplyV(pVector5, &transformed[0], pInverse_transform);
    fVar1 = pVector6->v[2] * pVector5->v[2] + pVector6->v[0] * pVector5->v[0] + pVector6->v[1] * pVector5->v[1];
    fVar1 = fVar1 + fVar1;
    fVar3 = pVector5->v[1] - pVector6->v[1] * fVar1;
    fVar4 = pVector5->v[2] - pVector6->v[2] * fVar1;
    pVector5->v[0] = pVector5->v[0] - fVar1 * pVector6->v[0];
    pVector5->v[1] = fVar3;
    pVector5->v[2] = fVar4;
    fVar1 = pVector2->v[1];
    fVar2 = pVector2->v[2];
    fVar3 = pVector2->v[2] * fVar4 + pVector2->v[1] * fVar3 + pVector5->v[0] * pVector2->v[0];
    fVar3 = fVar3 + fVar3;
    pVector5->v[0] = pVector5->v[0] - fVar3 * pVector2->v[0];
    pVector5->v[1] = pVector5->v[1] - fVar1 * fVar3;
    pVector5->v[2] = pVector5->v[2] - fVar2 * fVar3;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004bd6d0
int C2_HOOK_FASTCALL FUN_004bd6d0(tPhysics_object* pObject, undefined4* pCollision_data, int pCollision_count, int pCollision_capacity) {
    if (pObject != NULL) {
        do {
            pCollision_count = FUN_004bca10(pObject, pCollision_data, pCollision_count, pCollision_capacity);
            if (pObject->child != NULL) {
                pCollision_count = FUN_004bd6d0(pObject->child, pCollision_data, pCollision_count, pCollision_capacity);
            }
            pObject = pObject->next;
        } while (pObject != NULL);
    }
    return pCollision_count;
}

// C2PORT-NONMATCHING: best 70.89% | difftest PASS 2000 trials (spec difftest/specs/004bd720_FUN_004bd720.json) | reason VC5 x87 scheduling/register allocation
// FUNCTION: CARMA2_HW 0x004bd720
int C2_HOOK_FASTCALL FUN_004bd720(tPhysics_object* pObject) {
    br_scalar value;
    int joint_data;

    value = (pObject->omega.v[0] * pObject->omega.v[0]
        + pObject->omega.v[1] * pObject->omega.v[1]
        + pObject->omega.v[2] * pObject->omega.v[2]) * gFLOAT_0058a824;
    if (value > gFLOAT_0058a828) {
        value = sqrtf(value);
        joint_data = pObject->field_0x1e4;
        value = value * gFLOAT_0058a82c;
        pObject->omega.v[0] = pObject->omega.v[0] / (value + value);
        pObject->omega.v[1] = pObject->omega.v[1] / (value + value);
        pObject->omega.v[2] = pObject->omega.v[2] / (value + value);
        if (joint_data != 0) {
            tPhysics_object* pOther;

            pOther = *(tPhysics_object**)(uintptr_t)(joint_data + 4);
            pOther->omega.v[0] = pOther->omega.v[0] / (value + value);
            pOther->omega.v[1] = pOther->omega.v[1] / (value + value);
            pOther->omega.v[2] = pOther->omega.v[2] / (value + value);
        }
        FUN_004bc8e0((tPhysics_object*)pObject, value * value);
        {
            int callback;

            callback = pObject->field_0x198;
            if (callback != 0) {
                ((void (C2_HOOK_FASTCALL *)(tPhysics_object*))(uintptr_t)callback)((tPhysics_object*)pObject);
            }
        }
        return 1;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004bd7f0
int C2_HOOK_FASTCALL FUN_004bd7f0(tPhysics_object* pObject) {
    int count;

    count = 0;
    while (pObject != NULL) {
        count += FUN_004bd720(pObject);
        if (pObject->child != NULL) {
            count += FUN_004bd7f0(pObject->child);
        }
        pObject = pObject->next;
    }
    return count;
}

// FUNCTION: CARMA2_HW 0x004bd830
tPhysics_object** C2_HOOK_FASTCALL AppendOverlappingObjects(tPhysics_object* pObject, tPhysics_object* pList, tPhysics_object** pObjects) {
    tPhysics_object** pLink;
    tPhysics_object** pChild_link;
    tPhysics_object* pCurrent;
    tPhysics_object* child_head;
    br_scalar x;

    pLink = pObjects;
    if (pList->parent != NULL) {
        for (pCurrent = pList; pCurrent != NULL; pCurrent = pCurrent->next) {
            if ((pCurrent == pObject && (pObject->flags & 8) == 0)
                    || BoundsOverlapTest__finteray((br_bounds*)&pObject->field_0x10c, (br_bounds*)&pCurrent->field_0x10c)) {
                *pLink = pCurrent;
                pLink = (tPhysics_object**)&pCurrent->field_0x234;
            }
        }
    } else {
        x = pObject->field_0x10c.min.v[0] - gFLOAT_00692dd0;
        for (pCurrent = pObject->prev; pCurrent != NULL && pCurrent->transform_matrix.m[3][0] >= x; pCurrent = pCurrent->prev) {
            if (BoundsOverlapTest__finteray((br_bounds*)&pObject->field_0x10c, (br_bounds*)&pCurrent->field_0x10c)) {
                *pLink = pCurrent;
                pLink = (tPhysics_object**)&pCurrent->field_0x234;
            }
        }
        x = pObject->field_0x10c.max.v[0] - gFLOAT_00692dd4;
        for (pCurrent = pObject->next; pCurrent != NULL && pCurrent->transform_matrix.m[3][0] <= x; pCurrent = pCurrent->next) {
            if (BoundsOverlapTest__finteray((br_bounds*)&pObject->field_0x10c, (br_bounds*)&pCurrent->field_0x10c)) {
                *pLink = pCurrent;
                pLink = (tPhysics_object**)&pCurrent->field_0x234;
            }
        }
        if ((pObject->flags & 8) == 0) {
            *pLink = pObject;
            pLink = (tPhysics_object**)&pObject->field_0x234;
        }
    }
    *pLink = NULL;

    child_head = NULL;
    pCurrent = *pObjects;
    pChild_link = &child_head;
    while (pCurrent != NULL) {
        if (pCurrent->child != NULL) {
            pChild_link = AppendOverlappingObjects(pObject, pCurrent->child, pChild_link);
        }
        pCurrent = (tPhysics_object*)pCurrent->field_0x234;
    }
    if (child_head != NULL) {
        *pChild_link = NULL;
        *pLink = child_head;
        return pChild_link;
    }
    return pLink;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004bd9b0
void C2_HOOK_FASTCALL SetFieldF0Tree(tPhysics_object* pObject, int pValue) {
    tPhysics_object* child;

    pObject->field_0xf0 = pValue;
    for (child = pObject->child; child != NULL; child = child->next) {
        SetFieldF0Tree(child, pValue);
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 42.69% | difftest PASS 2000 trials (spec difftest/specs/004bd9e0_FUN_004bd9e0.json) | reason VC5 register allocation and stack layout
// FUNCTION: CARMA2_HW 0x004bd9e0
int C2_HOOK_FASTCALL FUN_004bd9e0(tPhysics_object* pObject, tPhysics_object** pReduced_objects, void* pCollision_contacts, int pCollision_count, tWorld_callbacks* pWorld_callbacks, tPhysics_object* pContext) {
    register tPhysics_object* pObject_register = pObject;
    register tPhysics_object** pReduced_register = pReduced_objects;
    int iVar1;
    tPhysics_object* fVar2;
    tPhysics_object* fVar3;
    char bVar4;
    tPhysics_object** pfVar5;
    int iVar6;
    tPhysics_object** pfVar7;
    tPhysics_object** pfVar8;
    float local_4;

    local_4 = pObject_register->field_0x1b4.v[2];
    gINT_006940ac = local_4 * local_4
        + pObject_register->field_0x1b4.v[1] * pObject_register->field_0x1b4.v[1]
        + pObject_register->field_0x1b4.v[0] * pObject_register->field_0x1b4.v[0] == gFLOAT_0058a768;
    gReduced_object_list = pReduced_register;
    GetFacesInBox(pObject_register, pWorld_callbacks);
    gReduced_object_list = NULL;
    gINT_006940ac = 0;

    iVar6 = FUN_004228b0(pObject_register, pCollision_contacts, pCollision_count);
    if (iVar6 != 0) {
        if (pObject_register->field_0xf0 == 2) {
            iVar1 = (int)pObject_register->parent;
            fVar3 = pObject_register;
            while (iVar1 != 0) {
                if (fVar3->physics_joint1 == NULL || fVar3->physics_joint1->type == 0) {
                    break;
                }
                fVar3 = fVar3->parent;
                iVar1 = (int)fVar3->parent;
            }
            iVar1 = (int)fVar3->child;
            fVar3->field_0xf0 = 0;
            for (; iVar1 != 0; iVar1 = (int)((tPhysics_object*)iVar1)->next) {
                SetFieldF0Tree((tPhysics_object*)iVar1, 0);
            }
            if (pWorld_callbacks != NULL && pWorld_callbacks->activate_passive != NULL) {
                pWorld_callbacks->activate_passive(fVar3, NULL);
            }
            if (((fVar3->child != NULL || fVar3->physics_joint2 != NULL)) && fVar3->field_0x218 == 0) {
                MoveJointedObject(fVar3 CARPOCALYPSE2_THISCALL_EDX, 0.0f);
            }
        }
        pCollision_count = pCollision_count + iVar6;
    }

    iVar6 = pCollision_count;
    for (fVar2 = *pReduced_register; fVar2 != NULL; fVar2 = (tPhysics_object*)fVar2->field_0x234) {
        if (fVar2 != pObject_register
                && ((pObject_register->field_0x1dc == 0 || fVar2->field_0x1dc != pObject_register->field_0x1dc || (uintptr_t)pObject_register < (uintptr_t)fVar2)
                    && pObject_register->parent != fVar2 && fVar2->parent != pObject_register)) {
            if (((pObject_register->field_0x1a0 & fVar2->field_0x1a4) == 0
                    && (pObject_register->field_0x1a4 & fVar2->field_0x1a0) == 0
                    && (pObject_register->flags & 4) == 0
                    && (fVar2->flags & 4) == 0)) {
                bVar4 = 0;
            } else {
                bVar4 = 1;
            }
            if (!bVar4 && (*(int*)&local_4 = FUN_00425ca0(pObject_register, fVar2, pCollision_contacts, iVar6), *(int*)&local_4 != 0)) {
                if (pObject_register->field_0xf0 == 2) {
                    iVar1 = (int)pObject_register->parent;
                    fVar3 = pObject_register;
                    while (iVar1 != 0 && fVar3->physics_joint1 != NULL && fVar3->physics_joint1->type != 0) {
                        fVar3 = fVar3->parent;
                        iVar1 = (int)fVar3->parent;
                    }
                    SetFieldF0Tree(fVar3, 0);
                    if (pWorld_callbacks != NULL && pWorld_callbacks->activate_passive != NULL) {
                        pWorld_callbacks->activate_passive(fVar3, fVar2);
                    }
                    if (((fVar3->child != NULL || fVar3->physics_joint2 != NULL)) && fVar3->field_0x218 == 0) {
                        MoveJointedObject(fVar3 CARPOCALYPSE2_THISCALL_EDX, 0.0f);
                    }
                }
                if (fVar2->field_0xf0 == 2) {
                    iVar1 = (int)fVar2->parent;
                    fVar3 = fVar2;
                    while (iVar1 != 0 && fVar3->physics_joint1 != NULL && fVar3->physics_joint1->type != 0) {
                        fVar3 = fVar3->parent;
                        iVar1 = (int)fVar3->parent;
                    }
                    SetFieldF0Tree(fVar3, 0);
                    if (pWorld_callbacks != NULL && pWorld_callbacks->activate_passive != NULL) {
                        pWorld_callbacks->activate_passive(fVar3, pObject_register);
                    }
                    if (((fVar3->child != NULL || fVar3->physics_joint2 != NULL)) && fVar3->field_0x218 == 0) {
                        MoveJointedObject(fVar3 CARPOCALYPSE2_THISCALL_EDX, 0.0f);
                    }
                }
                if (fVar2->disable_move_rotate) {
                    FUN_004be0e0(fVar2, pContext);
                }
                FUN_004bddd0(pObject_register, fVar2);
                iVar6 = pCollision_count + *(int*)&local_4;
                pCollision_count = iVar6;
            }
        }
    }

    pCollision_count = iVar6;
    for (fVar2 = pObject_register->child; fVar2 != NULL; fVar2 = fVar2->next) {
        if (fVar2->physics_joint1 != NULL && fVar2->physics_joint1->type != 0) {
            fVar3 = *pReduced_register;
            pfVar5 = (tPhysics_object**)&local_4;
            pfVar8 = pReduced_register;
            while (fVar3 != NULL) {
                if (fVar3 == fVar2 || BoundsOverlapTest__finteray((br_bounds*)((tU8*)fVar2 + 0x10c), (br_bounds*)((tU8*)fVar3 + 0x10c)) != 0) {
                    pfVar7 = (tPhysics_object**)&fVar3->field_0x234;
                    *pfVar8 = (tPhysics_object*)fVar3->field_0x234;
                    *pfVar5 = fVar3;
                } else {
                    pfVar8 = (tPhysics_object**)&fVar3->field_0x234;
                    pfVar7 = pfVar5;
                }
                pfVar5 = pfVar7;
                fVar3 = *pfVar8;
            }
            *pfVar5 = NULL;
            *pfVar8 = *(tPhysics_object**)&local_4;
            pCollision_count = FUN_004bd9e0(fVar2, pReduced_register, pCollision_contacts, pCollision_count, pWorld_callbacks, pContext);
        }
    }
    return pCollision_count;
}

// C2PORT-NONMATCHING: best 31.02% | difftest PASS 2000 trials (spec difftest/specs/004bddd0_FUN_004bddd0.json) | reason branch-register-layout
#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004bddd0
void C2_HOOK_FASTCALL FUN_004bddd0(tPhysics_object* pArg1, tPhysics_object* pArg2) {
    tPhysics_object* volatile pObject1 = pArg1;
    tPhysics_object* volatile pObject2 = pArg2;
    tPhysics_object* parent;
    register tPhysics_object* child;
    register tPhysics_object* group;
    char object1_mark;
    char mark;
    register int next_mark;

    parent = pObject1->parent;
    while (parent != NULL && pObject1->physics_joint1 != NULL && pObject1->physics_joint1->type != 0) {
        pObject1 = parent;
        parent = parent->parent;
    }
    parent = pObject2->parent;
    while (parent != NULL && pObject2->physics_joint1 != NULL && pObject2->physics_joint1->type != 0) {
        pObject2 = parent;
        parent = parent->parent;
    }

    if (pObject1 == pObject2) {
        return;
    }

    object1_mark = (char)pObject1->field_0x1dc;
    if (pObject2 == NULL) {
        if (object1_mark == 0) {
            next_mark = gINT_006940a0;
            pObject1->field_0x1dc = (tU8)next_mark;
            for (child = pObject1->child; child != NULL; child = child->next) {
                if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                    DRMatrix33Inverse(child, next_mark);
                }
            }
            pObject1->field183_0x1d8 = pObject1;
            gINT_006940a0++;
        }
        return;
    }

    if (object1_mark == 0) {
        if (pObject2->field_0x1dc == 0) {
            next_mark = gINT_006940a0;
            pObject1->field_0x1dc = (tU8)next_mark;
            for (child = pObject1->child; child != NULL; child = child->next) {
                if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                    DRMatrix33Inverse(child, next_mark);
                }
            }
            pObject2->field_0x1dc = (tU8)gINT_006940a0;
            for (child = pObject2->child; child != NULL; child = child->next) {
                if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                    DRMatrix33Inverse(child, gINT_006940a0);
                }
            }
            pObject1->field183_0x1d8 = pObject2;
            pObject2->field183_0x1d8 = pObject1;
            gINT_006940a0++;
            return;
        }

        mark = (char)pObject2->field_0x1dc;
        pObject1->field_0x1dc = (tU8)mark;
        for (child = pObject1->child; child != NULL; child = child->next) {
            if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                DRMatrix33Inverse(child, (int)mark);
            }
        }
        pObject1->field183_0x1d8 = pObject2->field183_0x1d8;
        pObject2->field183_0x1d8 = pObject1;
        return;
    }

    if (pObject2->field_0x1dc == 0) {
        pObject2->field_0x1dc = (tU8)object1_mark;
        for (child = pObject2->child; child != NULL; child = child->next) {
            if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                DRMatrix33Inverse(child, (int)object1_mark);
            }
        }
        pObject2->field183_0x1d8 = pObject1->field183_0x1d8;
        pObject1->field183_0x1d8 = pObject2;
        return;
    }

    if (object1_mark != (char)pObject2->field_0x1dc) {
        group = pObject2;
        while (group->field183_0x1d8 != pObject2) {
            group->field_0x1dc = (tU8)object1_mark;
            for (child = group->child; child != NULL; child = child->next) {
                if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                    DRMatrix33Inverse(child, (int)object1_mark);
                }
            }
            group = group->field183_0x1d8;
        }
        group->field_0x1dc = (tU8)object1_mark;
        for (child = group->child; child != NULL; child = child->next) {
            if (child->physics_joint1 != NULL && child->physics_joint1->type != 0) {
                DRMatrix33Inverse(child, (int)object1_mark);
            }
        }
        group->field183_0x1d8 = pObject1->field183_0x1d8;
        pObject1->field183_0x1d8 = pObject2;
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 76.19% | difftest PASS 2000 trials (spec difftest/specs/004be4b0_FUN_004be4b0.json) | reason x87-scheduling
// FUNCTION: CARMA2_HW 0x004be4b0
void C2_HOOK_FASTCALL FUN_004be4b0(float* pNormal, float* pScaledVector, const float* pVector1, const float* pVector2, tPhysics_object* pObject) {
    float fVar1;
    int iVar2;

    if ((pObject->flags & 0x800) == 0) {
        pNormal[0] = pVector1[1] * pVector2[2] - pVector1[2] * pVector2[1];
        pNormal[1] = pVector1[2] * pVector2[0] - pVector2[2] * pVector1[0];
        pNormal[2] = pVector2[1] * pVector1[0] - pVector1[1] * pVector2[0];
        for (iVar2 = 0; iVar2 < 3; iVar2++) {
            pNormal[iVar2] = pNormal[iVar2] / pObject->I.v[iVar2];
        }
    } else {
        pNormal[0] = 0.f;
        pNormal[1] = 0.f;
        pNormal[2] = 0.f;
    }
    fVar1 = (float)(1.0 / pObject->M);
    pScaledVector[0] = fVar1 * pVector2[0];
    pScaledVector[1] = fVar1 * pVector2[1];
    pScaledVector[2] = fVar1 * pVector2[2];
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 29.39% | difftest PASS 2000 trials (spec difftest/specs/004be550_FUN_004be550.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x004be550
tPhysics_object* C2_HOOK_FASTCALL FUN_004be550(br_vector3* pAngular_delta, br_vector3* pLinear_delta, br_vector3* pAngular_delta_other, br_vector3* pLinear_delta_other, const br_vector3* pImpulse, tPhysics_object* pObject) {
  float fVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float *pfVar7;
  int iVar8;
  int iVar9;
  float *pfVar10;
  struct {
    float local_48[4];
    float local_38;
    float local_34;
    float local_30;
    float local_2c;
    float local_28;
    float local_24[4];
    float local_14;
    float local_10;
    float local_4;
    float stack_padding[3];
  } local_storage;
#define local_48 local_storage.local_48
#define local_38 local_storage.local_38
#define local_34 local_storage.local_34
#define local_30 local_storage.local_30
#define local_2c local_storage.local_2c
#define local_28 local_storage.local_28
#define local_24 local_storage.local_24
#define local_14 local_storage.local_14
#define local_10 local_storage.local_10
#define local_4 local_storage.local_4

    register float* param_2 = (float*)pLinear_delta;
    register float* param_1 = (float*)pAngular_delta;
    register float* param_3 = (float*)pAngular_delta_other;
    float* param_4 = (float*)pLinear_delta_other;
    float* param_5 = (float*)pImpulse;
    int param_6 = (int)(uintptr_t)pObject;
  if ((*(unsigned int *)(param_6 + 0x19c) & 0x800) == 0) {
    *param_1 = *param_5 / *(float *)(param_6 + 8);
    param_1[1] = param_5[1] / *(float *)(param_6 + 0xc);
    param_1[2] = param_5[2] / *(float *)(param_6 + 0x10);
  }
  else {
    *param_1 = 0.0;
    param_1[1] = 0.0;
    param_1[2] = 0.0;
  }
  if (*(int *)(param_6 + 0x1e4) == 0) {
    BrVector3SetFloat((br_vector3*)param_2, 0.f, 0.f, 0.f);
    return 0;
  }
  iVar2 = *(int *)(*(int *)(param_6 + 0x1e4) + 4);
  if (iVar2 == *(int *)(param_6 + 0x230)) {
    iVar9 = *(int *)(param_6 + 0x188);
    local_48[3] = *(float *)(iVar9 + 8) - *(float *)(param_6 + 0x14);
    local_38 = *(float *)(iVar9 + 0xc) - *(float *)(param_6 + 0x18);
    local_34 = *(float *)(iVar9 + 0x10) - *(float *)(param_6 + 0x1c);
    local_24[3] = -((param_1[1] * local_34 - param_1[2] * local_38) * *(float *)(iVar9 + 0x38) +
                   *(float *)(iVar9 + 0x3c) * (param_1[2] * local_48[3] - *param_1 * local_34) +
                   *(float *)(iVar9 + 0x40) * (*param_1 * local_38 - param_1[1] * local_48[3]));
    local_14 = -(*param_1 * *(float *)(iVar9 + 0x20) +
                param_1[1] * *(float *)(iVar9 + 0x24) + param_1[2] * *(float *)(iVar9 + 0x28));
    local_10 = -(*param_1 * *(float *)(iVar9 + 0x2c) +
                param_1[1] * *(float *)(iVar9 + 0x30) + param_1[2] * *(float *)(iVar9 + 0x34));
    BrMatrix34ApplyV((br_vector3*)&local_30, (br_vector3*)&local_24[3], (br_matrix34*)(*(int *)(param_6 + 0x1e4) + 8));
    fVar1 = local_30 / *(float *)(param_6 + 4);
    *param_2 = *(float *)(*(int *)(param_6 + 0x188) + 0x38) * fVar1;
    param_2[1] = *(float *)(*(int *)(param_6 + 0x188) + 0x3c) * fVar1;
    param_2[2] = *(float *)(*(int *)(param_6 + 0x188) + 0x40) * fVar1;
    fVar1 = -(local_30 / *(float *)(iVar2 + 4));
    *param_4 = *(float *)(*(int *)(param_6 + 0x188) + 0x44) * fVar1;
    param_4[1] = *(float *)(*(int *)(param_6 + 0x188) + 0x48) * fVar1;
    iVar8 = 3;
    param_4[2] = *(float *)(*(int *)(param_6 + 0x188) + 0x4c) * fVar1;
    iVar9 = *(int *)(param_6 + 0x188);
    local_24[0] = *(float *)(iVar9 + 0x20) * local_2c + *(float *)(iVar9 + 0x2c) * local_28;
    local_24[1] = *(float *)(iVar9 + 0x24) * local_2c + *(float *)(iVar9 + 0x30) * local_28;
    local_24[2] = *(float *)(iVar9 + 0x28) * local_2c + *(float *)(iVar9 + 0x34) * local_28;
    fVar1 = *(float *)(iVar9 + 0x38) * local_30;
    fVar3 = *(float *)(iVar9 + 0x3c) * local_30;
    fVar4 = *(float *)(iVar9 + 0x40) * local_30;
    pfVar7 = (float *)(param_6 + 8);
    local_4 = local_48[3] * fVar3 - local_38 * fVar1;
    local_48[1] = (local_34 * fVar1 - local_48[3] * fVar4) + local_24[1];
    local_48[2] = local_4 + local_24[2];
    local_48[0] = (local_38 * fVar4 - local_34 * fVar3) + local_24[0];
    pfVar10 = param_1;
    do {
      fVar1 = *pfVar7;
      pfVar7 = pfVar7 + 1;
      iVar8 = iVar8 + -1;
      *pfVar10 = *(float *)(((int)local_48 - (int)param_1) + (int)pfVar10) / fVar1 + *pfVar10;
      pfVar10 = pfVar10 + 1;
    } while (iVar8 != 0);
    BrMatrix34ApplyV((br_vector3*)local_48, (br_vector3*)local_24, (br_matrix34*)(param_6 + 0x8c));
    BrMatrix34TApplyV((br_vector3*)param_3, (br_vector3*)local_48, (br_matrix34*)(iVar2 + 0x8c));
    iVar9 = *(int *)(param_6 + 0x188);
    pfVar7 = (float *)(iVar2 + 8);
    fVar1 = *(float *)(iVar9 + 0x44) * local_30;
    fVar3 = *(float *)(iVar9 + 0x14) - *(float *)(iVar2 + 0x14);
    fVar4 = *(float *)(iVar9 + 0x18) - *(float *)(iVar2 + 0x18);
    fVar5 = *(float *)(iVar9 + 0x1c) - *(float *)(iVar2 + 0x1c);
    fVar6 = *(float *)(iVar9 + 0x48) * local_30;
    local_30 = *(float *)(iVar9 + 0x4c) * local_30;
    *param_3 = (fVar4 * local_30 - fVar5 * fVar6) + *param_3;
    iVar9 = 3;
    param_3[1] = (fVar5 * fVar1 - fVar3 * local_30) + param_3[1];
    param_3[2] = (fVar3 * fVar6 - fVar4 * fVar1) + param_3[2];
    do {
      fVar1 = *pfVar7;
      pfVar7 = pfVar7 + 1;
      iVar9 = iVar9 + -1;
      *param_3 = -(*param_3 / fVar1);
      param_3 = param_3 + 1;
    } while (iVar9 != 0);
    return (tPhysics_object*)(uintptr_t)iVar2;
  }
  iVar9 = *(int *)(iVar2 + 0x188);
  local_48[3] = *(float *)(iVar9 + 0x14) - *(float *)(param_6 + 0x14);
  local_38 = *(float *)(iVar9 + 0x18) - *(float *)(param_6 + 0x18);
  local_34 = *(float *)(iVar9 + 0x1c) - *(float *)(param_6 + 0x1c);
  local_24[3] = (param_1[1] * local_34 - param_1[2] * local_38) * *(float *)(iVar9 + 0x44) +
                *(float *)(iVar9 + 0x48) * (param_1[2] * local_48[3] - *param_1 * local_34) +
                *(float *)(iVar9 + 0x4c) * (*param_1 * local_38 - param_1[1] * local_48[3]);
  BrMatrix34ApplyV((br_vector3*)local_24, (br_vector3*)param_1, (br_matrix34*)(param_6 + 0x8c));
  BrMatrix34TApplyV((br_vector3*)local_48, (br_vector3*)local_24, (br_matrix34*)(iVar2 + 0x8c));
  iVar9 = *(int *)(iVar2 + 0x188);
  local_14 = local_48[0] * *(float *)(iVar9 + 0x20) +
             local_48[1] * *(float *)(iVar9 + 0x24) + local_48[2] * *(float *)(iVar9 + 0x28);
  iVar9 = *(int *)(iVar2 + 0x188);
  local_10 = local_48[0] * *(float *)(iVar9 + 0x2c) +
             local_48[1] * *(float *)(iVar9 + 0x30) + local_48[2] * *(float *)(iVar9 + 0x34);
  BrMatrix34ApplyV((br_vector3*)&local_30, (br_vector3*)&local_24[3], (br_matrix34*)(*(int *)(iVar2 + 0x1e4) + 8));
  fVar1 = local_30 / *(float *)(iVar2 + 4);
  *param_4 = *(float *)(*(int *)(iVar2 + 0x188) + 0x38) * fVar1;
  param_4[1] = *(float *)(*(int *)(iVar2 + 0x188) + 0x3c) * fVar1;
  param_4[2] = *(float *)(*(int *)(iVar2 + 0x188) + 0x40) * fVar1;
  fVar1 = -(local_30 / *(float *)(param_6 + 4));
  *param_2 = *(float *)(*(int *)(iVar2 + 0x188) + 0x44) * fVar1;
  param_2[1] = *(float *)(*(int *)(iVar2 + 0x188) + 0x48) * fVar1;
  param_2[2] = *(float *)(*(int *)(iVar2 + 0x188) + 0x4c) * fVar1;
  *param_3 = *(float *)(*(int *)(iVar2 + 0x188) + 0x20) * local_2c;
  param_3[1] = *(float *)(*(int *)(iVar2 + 0x188) + 0x24) * local_2c;
  param_3[2] = *(float *)(*(int *)(iVar2 + 0x188) + 0x28) * local_2c;
  *param_3 = *(float *)(*(int *)(iVar2 + 0x188) + 0x2c) * local_28 + *param_3;
  param_3[1] = *(float *)(*(int *)(iVar2 + 0x188) + 0x30) * local_28 + param_3[1];
  param_3[2] = *(float *)(*(int *)(iVar2 + 0x188) + 0x34) * local_28 + param_3[2];
  BrMatrix34ApplyV((br_vector3*)local_48, (br_vector3*)param_3, (br_matrix34*)(iVar2 + 0x8c));
  BrMatrix34TApplyV((br_vector3*)local_24, (br_vector3*)local_48, (br_matrix34*)(param_6 + 0x8c));
  fVar1 = *(float *)(*(int *)(iVar2 + 0x188) + 0x44) * local_30;
  pfVar10 = (float *)(param_6 + 8);
  fVar3 = *(float *)(*(int *)(iVar2 + 0x188) + 0x48) * local_30;
  fVar4 = *(float *)(*(int *)(iVar2 + 0x188) + 0x4c) * local_30;
  local_24[0] = (local_38 * fVar4 - local_34 * fVar3) + local_24[0];
  local_24[1] = (local_34 * fVar1 - local_48[3] * fVar4) + local_24[1];
  local_24[2] = (local_48[3] * fVar3 - local_38 * fVar1) + local_24[2];
  iVar9 = *(int *)(iVar2 + 0x188);
  local_48[0] = *(float *)(iVar9 + 0x38) * local_30;
  fVar1 = *(float *)(iVar9 + 8) - *(float *)(iVar2 + 0x14);
  local_38 = *(float *)(iVar9 + 0xc) - *(float *)(iVar2 + 0x18);
  local_34 = *(float *)(iVar9 + 0x10) - *(float *)(iVar2 + 0x1c);
  local_48[1] = *(float *)(*(int *)(iVar2 + 0x188) + 0x3c) * local_30;
  local_48[2] = *(float *)(*(int *)(iVar2 + 0x188) + 0x40) * local_30;
  iVar9 = 3;
  *param_3 = (local_38 * local_48[2] - local_34 * local_48[1]) + *param_3;
  param_3[1] = (local_34 * local_48[0] - fVar1 * local_48[2]) + param_3[1];
  param_3[2] = (fVar1 * local_48[1] - local_38 * local_48[0]) + param_3[2];
  pfVar7 = param_1;
  do {
    fVar1 = *pfVar10;
    pfVar10 = pfVar10 + 1;
    iVar9 = iVar9 + -1;
    *pfVar7 = *pfVar7 - *(float *)(((int)local_24 - (int)param_1) + (int)pfVar7) / fVar1;
    pfVar7 = pfVar7 + 1;
  } while (iVar9 != 0);
  pfVar7 = (float *)(iVar2 + 8);
  iVar9 = 3;
  do {
    fVar1 = *pfVar7;
    pfVar7 = pfVar7 + 1;
    iVar9 = iVar9 + -1;
    *param_3 = *param_3 / fVar1;
    param_3 = param_3 + 1;
  } while (iVar9 != 0);
  return (tPhysics_object*)(uintptr_t)iVar2;
}
#undef local_48
#undef local_38
#undef local_34
#undef local_30
#undef local_2c
#undef local_28
#undef local_24
#undef local_14
#undef local_10
#undef local_4

// C2PORT-NONMATCHING: best 18.54% | difftest PASS 2000 trials (spec difftest/specs/004becf0_FUN_004becf0.json) | reason x87-order, register-allocation, stack-layout
// FUNCTION: CARMA2_HW 0x004becf0
tPhysics_object* C2_HOOK_FASTCALL FUN_004becf0(br_vector3* pAngular_delta, br_vector3* pLinear_delta, br_vector3* pAngular_delta_other, br_vector3* pLinear_delta_other, tPhysics_object* pObject) {
    tPhysics_object* pPartner;
    tPhysics_joint* joint;
    tPhysics_joint* partner_joint;
    int i;
    float fVar1;
    float fVar3;
    float fVar4;
    float fVar5;
    float fVar6;
    float local_48[4];
    float local_38;
    float local_34;
    float local_30[4];
    float local_20;
    float local_1c;
    float local_18;
    float local_14;
    float local_10;
    float local_4;
    br_vector3 matrix_input;
    br_vector3 matrix_output;
    pPartner = *(tPhysics_object**)(uintptr_t)(pObject->field_0x1e4 + 4);
    joint = pObject->physics_joint1;
    partner_joint = pPartner->physics_joint1;
    if (pPartner == pObject->field_0x230) {
        local_38 = joint->field_0x08.v[1] - pObject->cmpos.v[1];
        local_34 = joint->field_0x08.v[2] - pObject->cmpos.v[2];
        local_48[3] = joint->field_0x08.v[0] - pObject->cmpos.v[0];
        local_30[3] = (pAngular_delta->v[1] * local_34 - pAngular_delta->v[2] * local_38) + pLinear_delta->v[0];
        local_20 = (pAngular_delta->v[2] * local_48[3] - local_34 * pAngular_delta->v[0]) + pLinear_delta->v[1];
        local_1c = (local_38 * pAngular_delta->v[0] - pAngular_delta->v[1] * local_48[3]) + pLinear_delta->v[2];
        local_18 = -(joint->hinge_axis.v[0] * local_30[3]
            + joint->hinge_axis.v[1] * local_20
            + joint->hinge_axis.v[2] * local_1c);
        local_14 = -(joint->hinge_axis2.v[0] * pAngular_delta->v[0]
            + joint->hinge_axis2.v[1] * pAngular_delta->v[1]
            + joint->hinge_axis2.v[2] * pAngular_delta->v[2]);
        local_10 = -(joint->hinge_axis3.v[0] * pAngular_delta->v[0]
            + joint->hinge_axis3.v[1] * pAngular_delta->v[1]
            + joint->hinge_axis3.v[2] * pAngular_delta->v[2]);
        matrix_input.v[0] = local_18;
        matrix_input.v[1] = local_14;
        matrix_input.v[2] = local_10;
        BrMatrix34ApplyV(&matrix_output, &matrix_input,
            (br_matrix34*)(uintptr_t)(pObject->field_0x1e4 + 8));
        local_30[3] = matrix_output.v[0];
        local_20 = matrix_output.v[1];
        local_1c = matrix_output.v[2];
        fVar1 = local_30[3] / pObject->M;
        pLinear_delta->v[0] = joint->hinge_axis.v[0] * fVar1 + pLinear_delta->v[0];
        pLinear_delta->v[1] = joint->hinge_axis.v[1] * fVar1 + pLinear_delta->v[1];
        pLinear_delta->v[2] = joint->hinge_axis.v[2] * fVar1 + pLinear_delta->v[2];
        fVar1 = -(local_30[3] / pPartner->M);
        pLinear_delta_other->v[0] = joint->parent_bone_axis.v[0] * fVar1;
        pLinear_delta_other->v[1] = joint->parent_bone_axis.v[1] * fVar1;
        pLinear_delta_other->v[2] = joint->parent_bone_axis.v[2] * fVar1;

        local_30[0] = joint->hinge_axis2.v[0] * local_20 + joint->hinge_axis3.v[0] * local_1c;
        local_30[1] = joint->hinge_axis2.v[1] * local_20 + joint->hinge_axis3.v[1] * local_1c;
        local_30[2] = joint->hinge_axis2.v[2] * local_20 + joint->hinge_axis3.v[2] * local_1c;
        fVar1 = joint->hinge_axis.v[0] * local_30[3];
        fVar3 = joint->hinge_axis.v[1] * local_30[3];
        fVar4 = joint->hinge_axis.v[2] * local_30[3];
        local_4 = local_48[3] * fVar3 - local_38 * fVar1;
        local_48[1] = (local_34 * fVar1 - local_48[3] * fVar4) + local_30[1];
        local_48[2] = local_4 + local_30[2];
        local_48[0] = (local_38 * fVar4 - local_34 * fVar3) + local_30[0];
        for (i = 0; i < 3; i++) {
            pAngular_delta->v[i] = local_48[i] / pObject->I.v[i] + pAngular_delta->v[i];
        }
        BrMatrix34ApplyV((br_vector3*)&local_48[0], (br_vector3*)&local_30[0], &pObject->transform_matrix);
        BrMatrix34TApplyV(pAngular_delta_other, (br_vector3*)&local_48[0], &pPartner->transform_matrix);

        fVar1 = joint->parent_bone_axis.v[0] * local_30[3];
        fVar3 = joint->field_0x14.v[0] - pPartner->cmpos.v[0];
        fVar4 = joint->field_0x14.v[1] - pPartner->cmpos.v[1];
        fVar5 = joint->field_0x14.v[2] - pPartner->cmpos.v[2];
        fVar6 = joint->parent_bone_axis.v[1] * local_30[3];
        local_30[3] = joint->parent_bone_axis.v[2] * local_30[3];
        pAngular_delta_other->v[0] = (fVar4 * local_30[3] - fVar5 * fVar6) + pAngular_delta_other->v[0];
        pAngular_delta_other->v[1] = (fVar5 * fVar1 - fVar3 * local_30[3]) + pAngular_delta_other->v[1];
        pAngular_delta_other->v[2] = (fVar3 * fVar6 - fVar4 * fVar1) + pAngular_delta_other->v[2];
        for (i = 0; i < 3; i++) {
            pAngular_delta_other->v[i] = -pAngular_delta_other->v[i] / pPartner->I.v[i];
        }
        return pPartner;
    }

    local_38 = partner_joint->field_0x14.v[1] - pObject->cmpos.v[1];
    local_48[3] = partner_joint->field_0x14.v[0] - pObject->cmpos.v[0];
    local_34 = partner_joint->field_0x14.v[2] - pObject->cmpos.v[2];
    local_20 = (pAngular_delta->v[2] * local_48[3] - local_34 * pAngular_delta->v[0]) + pLinear_delta->v[1];
    local_1c = (local_38 * pAngular_delta->v[0] - pAngular_delta->v[1] * local_48[3]) + pLinear_delta->v[2];
    local_18 = partner_joint->parent_bone_axis.v[1] * local_20
        + partner_joint->parent_bone_axis.v[2] * local_1c
        + partner_joint->parent_bone_axis.v[0]
        * ((pAngular_delta->v[1] * local_34 - pAngular_delta->v[2] * local_38) + pLinear_delta->v[0]);
    BrMatrix34ApplyV((br_vector3*)&local_30[0], pAngular_delta, &pObject->transform_matrix);
    BrMatrix34TApplyV((br_vector3*)&local_48[0], (br_vector3*)&local_30[0], &pPartner->transform_matrix);
    local_14 = partner_joint->hinge_axis2.v[0] * local_48[0]
        + partner_joint->hinge_axis2.v[1] * local_48[1]
        + partner_joint->hinge_axis2.v[2] * local_48[2];
    local_10 = partner_joint->hinge_axis3.v[0] * local_48[0]
        + partner_joint->hinge_axis3.v[1] * local_48[1]
        + partner_joint->hinge_axis3.v[2] * local_48[2];
    matrix_input.v[0] = local_18;
    matrix_input.v[1] = local_14;
    matrix_input.v[2] = local_10;
    BrMatrix34ApplyV(&matrix_output, &matrix_input,
        (br_matrix34*)(uintptr_t)(pPartner->field_0x1e4 + 8));
    local_30[3] = matrix_output.v[0];
    local_20 = matrix_output.v[1];
    local_1c = matrix_output.v[2];
    fVar1 = local_30[3] / pPartner->M;
    pLinear_delta_other->v[0] = partner_joint->hinge_axis.v[0] * fVar1;
    pLinear_delta_other->v[1] = partner_joint->hinge_axis.v[1] * fVar1;
    pLinear_delta_other->v[2] = partner_joint->hinge_axis.v[2] * fVar1;
    fVar1 = -(local_30[3] / pObject->M);
    pLinear_delta->v[0] = partner_joint->parent_bone_axis.v[0] * fVar1 + pLinear_delta->v[0];
    pLinear_delta->v[1] = partner_joint->parent_bone_axis.v[1] * fVar1 + pLinear_delta->v[1];
    pLinear_delta->v[2] = partner_joint->parent_bone_axis.v[2] * fVar1 + pLinear_delta->v[2];

    pAngular_delta_other->v[0] = partner_joint->hinge_axis2.v[0] * local_20;
    pAngular_delta_other->v[1] = partner_joint->hinge_axis2.v[1] * local_20;
    pAngular_delta_other->v[2] = partner_joint->hinge_axis2.v[2] * local_20;
    pAngular_delta_other->v[0] = partner_joint->hinge_axis3.v[0] * local_1c + pAngular_delta_other->v[0];
    pAngular_delta_other->v[1] = partner_joint->hinge_axis3.v[1] * local_1c + pAngular_delta_other->v[1];
    pAngular_delta_other->v[2] = partner_joint->hinge_axis3.v[2] * local_1c + pAngular_delta_other->v[2];
    BrMatrix34ApplyV((br_vector3*)&local_48[0], pAngular_delta_other, &pPartner->transform_matrix);
    BrMatrix34TApplyV((br_vector3*)&local_30[0], (br_vector3*)&local_48[0], &pObject->transform_matrix);

    local_48[0] = partner_joint->parent_bone_axis.v[0] * local_30[3];
    local_48[1] = partner_joint->parent_bone_axis.v[1] * local_30[3];
    local_48[2] = partner_joint->parent_bone_axis.v[2] * local_30[3];
    local_30[0] = (local_38 * local_48[2] - local_34 * local_48[1]) + local_30[0];
    local_30[1] = (local_34 * local_48[0] - local_48[3] * local_48[2]) + local_30[1];
    local_30[2] = (local_48[3] * local_48[1] - local_38 * local_48[0]) + local_30[2];
    for (i = 0; i < 3; i++) {
        pAngular_delta->v[i] = pAngular_delta->v[i] - local_30[i] / pObject->I.v[i];
    }
    fVar1 = pPartner->physics_joint1->hinge_axis.v[0] * local_30[3];
    fVar3 = pPartner->physics_joint1->field_0x08.v[0] - pPartner->cmpos.v[0];
    fVar4 = pPartner->physics_joint1->field_0x08.v[1] - pPartner->cmpos.v[1];
    fVar5 = pPartner->physics_joint1->field_0x08.v[2] - pPartner->cmpos.v[2];
    fVar6 = pPartner->physics_joint1->hinge_axis.v[1] * local_30[3];
    local_30[3] = pPartner->physics_joint1->hinge_axis.v[2] * local_30[3];
    pAngular_delta_other->v[0] = (fVar4 * local_30[3] - fVar5 * fVar6) + pAngular_delta_other->v[0];
    pAngular_delta_other->v[1] = (fVar5 * fVar1 - fVar3 * local_30[3]) + pAngular_delta_other->v[1];
    pAngular_delta_other->v[2] = (fVar3 * fVar6 - fVar4 * fVar1) + pAngular_delta_other->v[2];
    for (i = 0; i < 3; i++) {
        pAngular_delta_other->v[i] = pAngular_delta_other->v[i] / pPartner->I.v[i];
    }
    return pPartner;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 48.59% | difftest PASS 2000 trials (spec difftest/specs/004b7960_FUN_004b7960.json) | reason VC5 register allocation and x87 scheduling; simple impulse paths also PASS (spec 004b7960_FUN_004b7960_SIMPLE.json)
// FUNCTION: CARMA2_HW 0x004b7960
void C2_HOOK_FAKE_THISCALL FUN_004b7960(tPhysics_joint_data* pJoint_data, float pDt) {
    tPhysics_object* pObject1;
    tPhysics_object* pObject2;
    tPhysics_object* pPartner;
    br_vector3 angular_delta;
    br_vector3 linear_delta;
    br_vector3 angular_delta_other;
    br_vector3 linear_delta_other;
    br_vector3 world_delta;
    br_vector3 impulse;
    br_vector3 linear_accel;
    float inverse_dt;

    if (pJoint_data->field_0x5c == 9 || pDt == gFLOAT_0058a768) {
        return;
    }

    pObject1 = pJoint_data->object1;
    pObject2 = pJoint_data->object2;
    if (pJoint_data->field_0x5c == 0xb) {
        pPartner = FUN_004be550(&angular_delta, &linear_delta, &angular_delta_other, &linear_delta_other, &pJoint_data->axis1, pObject1);
        pObject1->omega.v[0] += angular_delta.v[0] * pDt;
        pObject1->omega.v[1] += angular_delta.v[1] * pDt;
        pObject1->omega.v[2] += angular_delta.v[2] * pDt;
        if (pPartner != NULL) {
            linear_delta.v[0] *= pDt;
            linear_delta.v[1] *= pDt;
            linear_delta.v[2] *= pDt;
            pObject1->velocity_car_space.v[0] += linear_delta.v[0];
            pObject1->velocity_car_space.v[1] += linear_delta.v[1];
            pObject1->velocity_car_space.v[2] += linear_delta.v[2];
            BrMatrix34ApplyV(&pObject1->v, &pObject1->velocity_car_space, &pObject1->transform_matrix);
            linear_delta_other.v[0] *= pDt;
            linear_delta_other.v[1] *= pDt;
            linear_delta_other.v[2] *= pDt;
            pPartner->velocity_car_space.v[0] += linear_delta_other.v[0];
            pPartner->velocity_car_space.v[1] += linear_delta_other.v[1];
            pPartner->velocity_car_space.v[2] += linear_delta_other.v[2];
            BrMatrix34ApplyV(&pPartner->v, &pPartner->velocity_car_space, &pPartner->transform_matrix);
            pPartner->omega.v[0] += angular_delta_other.v[0] * pDt;
            pPartner->omega.v[1] += angular_delta_other.v[1] * pDt;
            pPartner->omega.v[2] += angular_delta_other.v[2] * pDt;
        }
        if (pObject2 != NULL) {
            pPartner = FUN_004be550(&angular_delta, &linear_delta, &angular_delta_other, &linear_delta_other, &pJoint_data->axis2, pObject2);
            inverse_dt = -pDt;
            pObject2->omega.v[0] += angular_delta.v[0] * inverse_dt;
            pObject2->omega.v[1] += angular_delta.v[1] * inverse_dt;
            pObject2->omega.v[2] += angular_delta.v[2] * inverse_dt;
            if (pPartner != NULL) {
                linear_delta.v[0] *= inverse_dt;
                linear_delta.v[1] *= inverse_dt;
                linear_delta.v[2] *= inverse_dt;
                pObject2->velocity_car_space.v[0] += linear_delta.v[0];
                pObject2->velocity_car_space.v[1] += linear_delta.v[1];
                pObject2->velocity_car_space.v[2] += linear_delta.v[2];
                BrMatrix34ApplyV(&pObject2->v, &pObject2->velocity_car_space, &pObject2->transform_matrix);
                linear_delta_other.v[0] *= inverse_dt;
                linear_delta_other.v[1] *= inverse_dt;
                linear_delta_other.v[2] *= inverse_dt;
                pPartner->velocity_car_space.v[0] += linear_delta_other.v[0];
                pPartner->velocity_car_space.v[1] += linear_delta_other.v[1];
                pPartner->velocity_car_space.v[2] += linear_delta_other.v[2];
                BrMatrix34ApplyV(&pPartner->v, &pPartner->velocity_car_space, &pPartner->transform_matrix);
                pPartner->omega.v[0] += angular_delta_other.v[0] * inverse_dt;
                pPartner->omega.v[1] += angular_delta_other.v[1] * inverse_dt;
                pPartner->omega.v[2] += angular_delta_other.v[2] * inverse_dt;
            }
        }
        return;
    }

    if (pObject1->field_0x1e4 == 0) {
        impulse.v[0] = pJoint_data->axis1.v[0] * pDt;
        linear_accel.v[0] = impulse.v[0] / pObject1->M;
        impulse.v[1] = pJoint_data->axis1.v[1] * pDt;
        linear_accel.v[1] = impulse.v[1] / pObject1->M;
        impulse.v[2] = pJoint_data->axis1.v[2] * pDt;
        linear_accel.v[2] = impulse.v[2] / pObject1->M;
        pObject1->velocity_car_space.v[0] += linear_accel.v[0];
        pObject1->velocity_car_space.v[1] += linear_accel.v[1];
        pObject1->velocity_car_space.v[2] += linear_accel.v[2];
        BrMatrix34ApplyV(&world_delta, &linear_accel, &pObject1->transform_matrix);
        pObject1->v.v[0] += world_delta.v[0];
        pObject1->v.v[1] += world_delta.v[1];
        pObject1->v.v[2] += world_delta.v[2];
        if ((pObject1->flags & 0x800) == 0) {
            angular_delta.v[0] = pJoint_data->anchor1.v[1] * impulse.v[2] - pJoint_data->anchor1.v[2] * impulse.v[1];
            angular_delta.v[1] = pJoint_data->anchor1.v[2] * impulse.v[0] - pJoint_data->anchor1.v[0] * impulse.v[2];
            angular_delta.v[2] = pJoint_data->anchor1.v[0] * impulse.v[1] - pJoint_data->anchor1.v[1] * impulse.v[0];
            pObject1->omega.v[0] += angular_delta.v[0] / pObject1->I.v[0];
            pObject1->omega.v[1] += angular_delta.v[1] / pObject1->I.v[1];
            pObject1->omega.v[2] += angular_delta.v[2] / pObject1->I.v[2];
        }
    } else {
        FUN_004be4b0(angular_delta.v, linear_delta.v, pJoint_data->anchor1.v, pJoint_data->axis1.v, pObject1);
        pPartner = FUN_004becf0(&angular_delta, &linear_delta, &angular_delta_other, &linear_delta_other, pObject1);
        angular_delta.v[0] *= pDt;
        angular_delta.v[1] *= pDt;
        angular_delta.v[2] *= pDt;
        pObject1->omega.v[0] += angular_delta.v[0];
        pObject1->omega.v[1] += angular_delta.v[1];
        pObject1->omega.v[2] += angular_delta.v[2];
        angular_delta_other.v[0] *= pDt;
        angular_delta_other.v[1] *= pDt;
        angular_delta_other.v[2] *= pDt;
        pPartner->omega.v[0] += angular_delta_other.v[0];
        pPartner->omega.v[1] += angular_delta_other.v[1];
        pPartner->omega.v[2] += angular_delta_other.v[2];
        linear_delta.v[0] *= pDt;
        linear_delta.v[1] *= pDt;
        linear_delta.v[2] *= pDt;
        pObject1->velocity_car_space.v[0] += linear_delta.v[0];
        pObject1->velocity_car_space.v[1] += linear_delta.v[1];
        pObject1->velocity_car_space.v[2] += linear_delta.v[2];
        BrMatrix34ApplyV(&world_delta, &linear_delta, &pObject1->transform_matrix);
        pObject1->v.v[0] += world_delta.v[0];
        pObject1->v.v[1] += world_delta.v[1];
        pObject1->v.v[2] += world_delta.v[2];
        linear_delta_other.v[0] *= pDt;
        linear_delta_other.v[1] *= pDt;
        linear_delta_other.v[2] *= pDt;
        pPartner->velocity_car_space.v[0] += linear_delta_other.v[0];
        pPartner->velocity_car_space.v[1] += linear_delta_other.v[1];
        pPartner->velocity_car_space.v[2] += linear_delta_other.v[2];
        BrMatrix34ApplyV(&world_delta, &linear_delta_other, &pPartner->transform_matrix);
        pPartner->v.v[0] += world_delta.v[0];
        pPartner->v.v[1] += world_delta.v[1];
        pPartner->v.v[2] += world_delta.v[2];
    }

    if (pObject2 != NULL && pJoint_data->field_0x5c != 0xe) {
        if (pObject2->field_0x1e4 != 0) {
            FUN_004be4b0(angular_delta.v, linear_delta.v, pJoint_data->anchor2.v, pJoint_data->axis2.v, pObject2);
            pPartner = FUN_004becf0(&angular_delta, &linear_delta, &angular_delta_other, &linear_delta_other, pObject2);
            pDt = -pDt;
            angular_delta.v[0] *= pDt;
            angular_delta.v[1] *= pDt;
            angular_delta.v[2] *= pDt;
            pObject2->omega.v[0] += angular_delta.v[0];
            pObject2->omega.v[1] += angular_delta.v[1];
            pObject2->omega.v[2] += angular_delta.v[2];
            angular_delta_other.v[0] *= pDt;
            angular_delta_other.v[1] *= pDt;
            angular_delta_other.v[2] *= pDt;
            pPartner->omega.v[0] += angular_delta_other.v[0];
            pPartner->omega.v[1] += angular_delta_other.v[1];
            pPartner->omega.v[2] += angular_delta_other.v[2];
            linear_delta.v[0] *= pDt;
            linear_delta.v[1] *= pDt;
            linear_delta.v[2] *= pDt;
            pObject2->velocity_car_space.v[0] += linear_delta.v[0];
            pObject2->velocity_car_space.v[1] += linear_delta.v[1];
            pObject2->velocity_car_space.v[2] += linear_delta.v[2];
            BrMatrix34ApplyV(&world_delta, &linear_delta, &pObject2->transform_matrix);
            pObject2->v.v[0] += world_delta.v[0];
            pObject2->v.v[1] += world_delta.v[1];
            pObject2->v.v[2] += world_delta.v[2];
            linear_delta_other.v[0] *= pDt;
            linear_delta_other.v[1] *= pDt;
            linear_delta_other.v[2] *= pDt;
            pPartner->velocity_car_space.v[0] += linear_delta_other.v[0];
            pPartner->velocity_car_space.v[1] += linear_delta_other.v[1];
            pPartner->velocity_car_space.v[2] += linear_delta_other.v[2];
            BrMatrix34ApplyV(&world_delta, &linear_delta_other, &pPartner->transform_matrix);
            pPartner->v.v[0] += world_delta.v[0];
            pPartner->v.v[1] += world_delta.v[1];
            pPartner->v.v[2] += world_delta.v[2];
        } else {
            inverse_dt = -pDt;
            impulse.v[0] = pJoint_data->axis2.v[0] * inverse_dt;
            linear_accel.v[0] = impulse.v[0] / pObject2->M;
            impulse.v[1] = pJoint_data->axis2.v[1] * inverse_dt;
            linear_accel.v[1] = impulse.v[1] / pObject2->M;
            impulse.v[2] = pJoint_data->axis2.v[2] * inverse_dt;
            linear_accel.v[2] = impulse.v[2] / pObject2->M;
            pObject2->velocity_car_space.v[0] += linear_accel.v[0];
            pObject2->velocity_car_space.v[1] += linear_accel.v[1];
            pObject2->velocity_car_space.v[2] += linear_accel.v[2];
            BrMatrix34ApplyV(&world_delta, &linear_accel, &pObject2->transform_matrix);
            pObject2->v.v[0] += world_delta.v[0];
            pObject2->v.v[1] += world_delta.v[1];
            pObject2->v.v[2] += world_delta.v[2];
            if ((pObject2->flags & 0x800) == 0) {
                angular_delta.v[0] = pJoint_data->anchor2.v[1] * impulse.v[2] - pJoint_data->anchor2.v[2] * impulse.v[1];
                angular_delta.v[1] = pJoint_data->anchor2.v[2] * impulse.v[0] - pJoint_data->anchor2.v[0] * impulse.v[2];
                angular_delta.v[2] = pJoint_data->anchor2.v[0] * impulse.v[1] - pJoint_data->anchor2.v[1] * impulse.v[0];
                pObject2->omega.v[0] += angular_delta.v[0] / pObject2->I.v[0];
                pObject2->omega.v[1] += angular_delta.v[1] / pObject2->I.v[1];
                pObject2->omega.v[2] += angular_delta.v[2] / pObject2->I.v[2];
            }
        }
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 26.21% | difftest PASS 2000 trials (spec difftest/specs/004b77b0_FUN_004b77b0.json) | reason x87-order and register allocation
// FUNCTION: CARMA2_HW 0x004b77b0
void C2_HOOK_FASTCALL FUN_004b77b0(int* pDestination, int* pJoint_data, int pCount) {
    int destination_offset;
    tPhysics_joint_data* joint_data;
    tPhysics_object* object1;
    tPhysics_object* object2;
    br_scalar* destination;
    volatile float angular_delta[3];

    if (pCount <= 0) {
        return;
    }
    destination_offset = (int)pDestination - (int)pJoint_data;
    do {
        joint_data = (tPhysics_joint_data*)(uintptr_t)*pJoint_data;
        object1 = joint_data->object1;
        destination = (br_scalar*)((char*)pJoint_data + destination_offset);
        if (joint_data->field_0x5c == eJoint_limit_11) {
            *destination = -(object1->omega.v[0] * joint_data->axis1.v[0]
                + joint_data->axis1.v[2] * object1->omega.v[2]
                + joint_data->axis1.v[1] * object1->omega.v[1]);
            object2 = joint_data->object2;
            if (object2 != NULL) {
                *destination = joint_data->axis2.v[2] * object2->omega.v[2]
                    + joint_data->axis2.v[0] * object2->omega.v[0]
                    + joint_data->axis2.v[1] * object2->omega.v[1]
                    + *destination;
            }
        } else {
            angular_delta[0] = (joint_data->anchor1.v[2] * object1->omega.v[1]
                    - joint_data->anchor1.v[1] * object1->omega.v[2])
                + object1->velocity_car_space.v[0];
            angular_delta[1] = (object1->omega.v[2] * joint_data->anchor1.v[0]
                    - object1->omega.v[0] * joint_data->anchor1.v[2])
                + object1->velocity_car_space.v[1];
            angular_delta[2] = (object1->omega.v[0] * joint_data->anchor1.v[1]
                    - object1->omega.v[1] * joint_data->anchor1.v[0])
                + object1->velocity_car_space.v[2];
            *destination = -(joint_data->axis1.v[0] * angular_delta[0]
                + joint_data->axis1.v[2] * angular_delta[2]
                + joint_data->axis1.v[1] * angular_delta[1]);
            object2 = joint_data->object2;
            if (object2 != NULL) {
                angular_delta[0] = (object2->omega.v[0] * joint_data->anchor2.v[1]
                        - joint_data->anchor2.v[0] * object2->omega.v[1])
                    + object2->velocity_car_space.v[2];
                angular_delta[1] = (object2->omega.v[2] * joint_data->anchor2.v[0]
                        - object2->omega.v[0] * joint_data->anchor2.v[2])
                    + object2->velocity_car_space.v[1];
                angular_delta[2] = (object2->omega.v[1] * joint_data->anchor2.v[2]
                        - joint_data->anchor2.v[1] * object2->omega.v[2])
                    + object2->velocity_car_space.v[0];
                *destination = joint_data->axis2.v[2] * angular_delta[0]
                    + joint_data->axis2.v[1] * angular_delta[1]
                    + joint_data->axis2.v[0] * angular_delta[2]
                    + *destination;
            }
        }
        pJoint_data++;
        pCount--;
    } while (pCount != 0);
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 35.02% | difftest PASS 2000 trials (spec difftest/specs/004be350_FUN_004be350.json) | reason vc5-x87-scheduling
// FUNCTION: CARMA2_HW 0x004be350
void C2_HOOK_FASTCALL FUN_004be350(float* pDestination, const float* pSource) {
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;

    pDestination[0] = -pSource[5] * pSource[7] + pSource[8] * pSource[4];
    pDestination[1] = -(-pSource[2] * pSource[7] + pSource[1] * pSource[8]);
    fVar1 = -pSource[2] * pSource[4] + pSource[1] * pSource[5];
    pDestination[2] = fVar1;
    fVar5 = (float)gDOUBLE_0058a780 /
        (pSource[1] * pDestination[1] + pSource[2] * fVar1 + pSource[0] * pDestination[0]);
    pDestination[0] = pDestination[0] * fVar5;
    pDestination[3] = -((-pSource[5] * pSource[6] + pSource[8] * pSource[3]) * fVar5);
    fVar1 = pSource[4];
    fVar2 = pSource[3];
    fVar3 = pSource[7];
    fVar4 = pSource[6];
    pDestination[1] = pDestination[1] * fVar5;
    pDestination[6] = (-fVar1 * fVar4 + fVar3 * fVar2) * fVar5;
    pDestination[4] = (-pSource[2] * pSource[6] + pSource[8] * pSource[0]) * fVar5;
    fVar1 = pSource[1];
    fVar2 = pSource[7];
    fVar3 = pSource[6];
    fVar4 = pSource[0];
    pDestination[2] = pDestination[2] * fVar5;
    pDestination[7] = -((-fVar1 * fVar3 + fVar2 * fVar4) * fVar5);
    pDestination[5] = -((-pSource[2] * pSource[3] + pSource[5] * pSource[0]) * fVar5);
    pDestination[8] = (-pSource[1] * pSource[3] + pSource[4] * pSource[0]) * fVar5;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 21.42% | difftest PASS 2000 trials (spec difftest/specs/004bf490_FUN_004bf490.json) | reason original stack layout and x87 operation ordering differ
// FUNCTION: CARMA2_HW 0x004bf490
void C2_HOOK_FASTCALL FUN_004bf490(int* param_1, int param_2, int* param_3, int param_4) {
  /* Keep each four-byte stack slot addressable for contiguous vector outputs. */
  union { int i[62]; float f[62]; } local_storage;
#define local_e0 local_storage.i[6]
#define local_dc local_storage.f[7]
#define local_d8 local_storage.f[8]
#define local_d4 local_storage.f[9]
#define local_d0 local_storage.f[10]
#define local_cc local_storage.f[11]
#define local_c8 local_storage.f[12]
#define local_c4 local_storage.f[13]
#define local_c0 local_storage.f[14]
#define local_bc local_storage.f[15]
#define local_b8 local_storage.f[16]
#define local_b4 local_storage.f[17]
#define local_b0 local_storage.f[18]
#define local_ac local_storage.f[19]
#define local_a8 local_storage.f[20]
#define local_a4 local_storage.f[21]
#define local_a0 local_storage.f[22]
#define local_9c local_storage.f[23]
#define local_98 local_storage.f[24]
#define local_94 local_storage.f[25]
#define local_90 local_storage.f[26]
#define local_8c local_storage.f[27]
#define local_88 local_storage.f[28]
#define local_84 local_storage.f[29]
#define local_80 local_storage.f[30]
#define local_7c local_storage.i[31]
#define local_78 local_storage.i[32]
#define local_74 local_storage.i[33]
#define local_70 local_storage.i[34]
#define local_6c local_storage.i[35]
#define local_68 local_storage.i[36]
#define local_64 local_storage.i[37]
#define local_60 local_storage.f[38]
#define local_5c local_storage.f[39]
#define local_58 local_storage.f[40]
#define local_54 local_storage.f[41]
#define local_50 local_storage.f[42]
#define local_4c local_storage.f[43]
#define local_48 local_storage.f[44]
#define local_44 local_storage.f[45]
#define local_40 local_storage.f[46]
#define local_3c local_storage.f[47]
#define local_38 local_storage.f[48]
#define local_34 local_storage.f[49]
#define local_30 local_storage.f[50]
#define local_2c local_storage.f[51]
#define local_28 local_storage.f[52]
#define local_24 local_storage.f[53]
#define local_20 local_storage.f[54]
#define local_1c local_storage.f[55]
#define local_18 local_storage.f[56]
#define local_14 local_storage.f[57]
#define local_10 local_storage.f[58]
#define local_c local_storage.f[59]
#define local_8 local_storage.f[60]
#define local_4 local_storage.f[61]
  int iVar1;
  float fVar2;
  int iVar3;
  float *pfVar4;
  int *piVar5;
  float *pfVar6;
  int iVar7;
  int iVar8;
  int iVar9;

  local_74 = 0;
  local_78 = param_2;
  local_64 = (int)(uintptr_t)param_1;
  if (0 < param_4) {
    do {
      iVar7 = local_74;
      iVar1 = param_3[local_74];
      iVar9 = *(int *)(iVar1 + 8);
      iVar8 = *(int *)(iVar1 + 4);
      if ((iVar9 != 0) && (*(int *)(iVar1 + 0x5c) == 0xe)) {
        iVar9 = 0;
      }
      local_7c = iVar8;
      if (*(int *)(iVar1 + 0x5c) == 0xb) {
        iVar9 = *(int *)(iVar1 + 4);
        iVar8 = *(int *)(iVar1 + 8);
        iVar3 = (int)(uintptr_t)FUN_004be550(&local_c4, &local_48, &local_94, &local_30, (const br_vector3*)(iVar1 + 0x2c), (tPhysics_object*)iVar9);
        if (iVar8 != 0) {
          local_6c = (int)(uintptr_t)FUN_004be550(&local_a0, &local_18, &local_b8, &local_3c, (const br_vector3*)(iVar1 + 0x38), (tPhysics_object*)iVar8);
        }
        if (-1 < iVar7) {
          pfVar6 = (float *)(local_64 + iVar7 * local_78 * 4);
          pfVar4 = (float *)(local_64 + iVar7 * 4);
          local_7c = iVar7 + 1;
          piVar5 = param_3;
          do {
            iVar1 = *piVar5;
            if (*(int *)(iVar1 + 0x5c) == 0xb) {
              if (*(int *)(iVar1 + 4) == iVar9) {
                *pfVar4 = *(float *)(iVar1 + 0x2c) * local_c4 +
                          *(float *)(iVar1 + 0x30) * local_c0 + *(float *)(iVar1 + 0x34) * local_bc;
              }
              else if (*(int *)(iVar1 + 8) == iVar9) {
                *pfVar4 = -(*(float *)(iVar1 + 0x40) * local_bc +
                           *(float *)(iVar1 + 0x38) * local_c4 + *(float *)(iVar1 + 0x3c) * local_c0
                           );
              }
              else {
                *pfVar4 = 0.0;
              }
              if (iVar3 != 0) {
                iVar1 = *piVar5;
                if (*(int *)(iVar1 + 4) == iVar3) {
                  fVar2 = *(float *)(iVar1 + 0x2c) * local_94 +
                          *(float *)(iVar1 + 0x30) * local_90 + *(float *)(iVar1 + 0x34) * local_8c
                          + *pfVar4;
                }
                else {
                  if (*(int *)(iVar1 + 8) != iVar3) goto LAB_004bf646;
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x40) * local_8c +
                                    *(float *)(iVar1 + 0x38) * local_94 +
                                    *(float *)(iVar1 + 0x3c) * local_90);
                }
                *pfVar4 = fVar2;
              }
LAB_004bf646:
              if (iVar8 != 0) {
                iVar1 = *piVar5;
                if (*(int *)(iVar1 + 4) == iVar8) {
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x2c) * local_a0 +
                                    *(float *)(iVar1 + 0x30) * local_9c +
                                    *(float *)(iVar1 + 0x34) * local_98);
LAB_004bf6a6:
                  *pfVar4 = fVar2;
                }
                else if (*(int *)(iVar1 + 8) == iVar8) {
                  fVar2 = *(float *)(iVar1 + 0x40) * local_98 +
                          *(float *)(iVar1 + 0x38) * local_a0 + *(float *)(iVar1 + 0x3c) * local_9c
                          + *pfVar4;
                  goto LAB_004bf6a6;
                }
                if (local_6c != 0) {
                  iVar1 = *piVar5;
                  if (*(int *)(iVar1 + 4) == local_6c) {
                    *pfVar4 = *pfVar4 - (*(float *)(iVar1 + 0x2c) * local_b8 +
                                        *(float *)(iVar1 + 0x30) * local_b4 +
                                        *(float *)(iVar1 + 0x34) * local_b0);
                  }
                  else if (*(int *)(iVar1 + 8) == local_6c) {
                    *pfVar4 = *(float *)(iVar1 + 0x40) * local_b0 +
                              *(float *)(iVar1 + 0x38) * local_b8 +
                              *(float *)(iVar1 + 0x3c) * local_b4 + *pfVar4;
                  }
                }
              }
            }
            else {
              if (*(int *)(iVar1 + 4) == iVar9) {
                *pfVar4 = *(float *)(iVar1 + 0x2c) *
                          ((*(float *)(iVar1 + 0x1c) * local_c0 -
                           *(float *)(iVar1 + 0x18) * local_bc) + local_48) +
                          *(float *)(iVar1 + 0x34) *
                          ((*(float *)(iVar1 + 0x18) * local_c4 -
                           *(float *)(iVar1 + 0x14) * local_c0) + local_40) +
                          *(float *)(iVar1 + 0x30) *
                          ((*(float *)(iVar1 + 0x14) * local_bc -
                           *(float *)(iVar1 + 0x1c) * local_c4) + local_44);
              }
              else if (*(int *)(iVar1 + 8) == iVar9) {
                *pfVar4 = -(*(float *)(iVar1 + 0x40) *
                            ((*(float *)(iVar1 + 0x24) * local_c4 -
                             *(float *)(iVar1 + 0x20) * local_c0) + local_40) +
                           *(float *)(iVar1 + 0x3c) *
                           ((*(float *)(iVar1 + 0x20) * local_bc -
                            *(float *)(iVar1 + 0x28) * local_c4) + local_44) +
                           *(float *)(iVar1 + 0x38) *
                           ((*(float *)(iVar1 + 0x28) * local_c0 -
                            *(float *)(iVar1 + 0x24) * local_bc) + local_48));
              }
              else {
                *pfVar4 = 0.0;
              }
              if (iVar3 != 0) {
                iVar1 = *piVar5;
                if (*(int *)(iVar1 + 4) == iVar3) {
                  fVar2 = *(float *)(iVar1 + 0x2c) *
                          ((*(float *)(iVar1 + 0x1c) * local_90 -
                           *(float *)(iVar1 + 0x18) * local_8c) + local_30) +
                          *(float *)(iVar1 + 0x34) *
                          ((*(float *)(iVar1 + 0x18) * local_94 -
                           *(float *)(iVar1 + 0x14) * local_90) + local_28) +
                          *(float *)(iVar1 + 0x30) *
                          ((*(float *)(iVar1 + 0x14) * local_8c -
                           *(float *)(iVar1 + 0x1c) * local_94) + local_2c) + *pfVar4;
                }
                else {
                  if (*(int *)(iVar1 + 8) != iVar3) goto LAB_004bf981;
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x40) *
                                     ((*(float *)(iVar1 + 0x24) * local_94 -
                                      *(float *)(iVar1 + 0x20) * local_90) + local_28) +
                                    *(float *)(iVar1 + 0x3c) *
                                    ((*(float *)(iVar1 + 0x20) * local_8c -
                                     *(float *)(iVar1 + 0x28) * local_94) + local_2c) +
                                    *(float *)(iVar1 + 0x38) *
                                    ((*(float *)(iVar1 + 0x28) * local_90 -
                                     *(float *)(iVar1 + 0x24) * local_8c) + local_30));
                }
                *pfVar4 = fVar2;
              }
LAB_004bf981:
              if (iVar8 == 0) goto LAB_004bfbea;
              iVar1 = *piVar5;
              if (*(int *)(iVar1 + 4) == iVar8) {
                fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x2c) *
                                   ((*(float *)(iVar1 + 0x1c) * local_9c -
                                    *(float *)(iVar1 + 0x18) * local_98) + local_18) +
                                  *(float *)(iVar1 + 0x34) *
                                  ((*(float *)(iVar1 + 0x18) * local_a0 -
                                   *(float *)(iVar1 + 0x14) * local_9c) + local_10) +
                                  *(float *)(iVar1 + 0x30) *
                                  ((*(float *)(iVar1 + 0x14) * local_98 -
                                   *(float *)(iVar1 + 0x1c) * local_a0) + local_14));
LAB_004bfaae:
                *pfVar4 = fVar2;
              }
              else if (*(int *)(iVar1 + 8) == iVar8) {
                fVar2 = *(float *)(iVar1 + 0x40) *
                        ((*(float *)(iVar1 + 0x24) * local_a0 - *(float *)(iVar1 + 0x20) * local_9c)
                        + local_10) +
                        *(float *)(iVar1 + 0x3c) *
                        ((*(float *)(iVar1 + 0x20) * local_98 - *(float *)(iVar1 + 0x28) * local_a0)
                        + local_14) +
                        *(float *)(iVar1 + 0x38) *
                        ((*(float *)(iVar1 + 0x28) * local_9c - *(float *)(iVar1 + 0x24) * local_98)
                        + local_18) + *pfVar4;
                goto LAB_004bfaae;
              }
              if (local_6c != 0) {
                iVar1 = *piVar5;
                if (*(int *)(iVar1 + 4) == local_6c) {
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x2c) *
                                     ((*(float *)(iVar1 + 0x1c) * local_b4 -
                                      *(float *)(iVar1 + 0x18) * local_b0) + local_3c) +
                                    *(float *)(iVar1 + 0x34) *
                                    ((*(float *)(iVar1 + 0x18) * local_b8 -
                                     *(float *)(iVar1 + 0x14) * local_b4) + local_34) +
                                    *(float *)(iVar1 + 0x30) *
                                    ((*(float *)(iVar1 + 0x14) * local_b0 -
                                     *(float *)(iVar1 + 0x1c) * local_b8) + local_38));
                }
                else {
                  if (*(int *)(iVar1 + 8) != local_6c) goto LAB_004bfbea;
                  fVar2 = *(float *)(iVar1 + 0x40) *
                          ((*(float *)(iVar1 + 0x24) * local_b8 -
                           *(float *)(iVar1 + 0x20) * local_b4) + local_34) +
                          *(float *)(iVar1 + 0x3c) *
                          ((*(float *)(iVar1 + 0x20) * local_b0 -
                           *(float *)(iVar1 + 0x28) * local_b8) + local_38) +
                          *(float *)(iVar1 + 0x38) *
                          ((*(float *)(iVar1 + 0x28) * local_b4 -
                           *(float *)(iVar1 + 0x24) * local_b0) + local_3c) + *pfVar4;
                }
                *pfVar4 = fVar2;
              }
            }
LAB_004bfbea:
            piVar5 = piVar5 + 1;
            *pfVar6 = *pfVar4;
            pfVar4 = pfVar4 + local_78;
            pfVar6 = pfVar6 + 1;
            local_7c = local_7c + -1;
            iVar7 = local_74;
          } while (local_7c != 0);
        }
      }
      else {
        FUN_004be4b0(&local_d0, &local_60, (const float*)(iVar1 + 0x14), (const float*)(iVar1 + 0x2c), (tPhysics_object*)iVar8);
        if (*(int *)(iVar8 + 0x1e4) == 0) {
          local_e0 = 0;
        }
        else {
          local_e0 = (int)(uintptr_t)FUN_004becf0(&local_d0, &local_60, &local_ac, &local_24, (tPhysics_object*)iVar8);
        }
        iVar8 = 0;
        local_68 = 0;
        if ((iVar9 != 0) &&
           (FUN_004be4b0(&local_dc, &local_54, (const float*)(iVar1 + 0x20), (const float*)(iVar1 + 0x38), (tPhysics_object*)iVar9), *(int *)(iVar9 + 0x1e4) != 0)) {
          iVar8 = (int)(uintptr_t)FUN_004becf0(&local_dc, &local_54, &local_88, &local_c, (tPhysics_object*)iVar9);
          local_68 = iVar8;
        }
        if (-1 < iVar7) {
          pfVar6 = (float *)(local_64 + local_74 * local_78 * 4);
          pfVar4 = (float *)(local_64 + local_74 * 4);
          local_70 = local_74 + 1;
          piVar5 = param_3;
          do {
            *pfVar4 = 0.0;
            iVar1 = *piVar5;
            if (*(int *)(iVar1 + 0x5c) == 0xb) {
              if (*(int *)(iVar1 + 4) == local_7c) {
                fVar2 = *(float *)(iVar1 + 0x34) * local_c8 +
                        *(float *)(iVar1 + 0x30) * local_cc + *(float *)(iVar1 + 0x2c) * local_d0;
LAB_004bfd67:
                *pfVar4 = fVar2;
              }
              else if (*(int *)(iVar1 + 8) == local_7c) {
                fVar2 = -(*(float *)(iVar1 + 0x3c) * local_cc +
                         *(float *)(iVar1 + 0x38) * local_d0 + *(float *)(iVar1 + 0x40) * local_c8);
                goto LAB_004bfd67;
              }
              if (iVar9 != 0) {
                if (*(int *)(iVar1 + 4) == iVar9) {
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x34) * local_d4 +
                                    *(float *)(iVar1 + 0x30) * local_d8 +
                                    *(float *)(iVar1 + 0x2c) * local_dc);
                }
                else {
                  if (*(int *)(iVar1 + 8) != iVar9) goto LAB_004bfdc5;
                  fVar2 = *(float *)(iVar1 + 0x3c) * local_d8 +
                          *(float *)(iVar1 + 0x38) * local_dc + *(float *)(iVar1 + 0x40) * local_d4
                          + *pfVar4;
                }
                *pfVar4 = fVar2;
              }
LAB_004bfdc5:
              if (local_e0 != 0) {
                if (*(int *)(iVar1 + 4) == local_e0) {
                  fVar2 = *(float *)(iVar1 + 0x34) * local_a4 +
                          *(float *)(iVar1 + 0x30) * local_a8 + *(float *)(iVar1 + 0x2c) * local_ac
                          + *pfVar4;
                }
                else {
                  if (*(int *)(iVar1 + 8) != local_e0) goto LAB_004bfe25;
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x3c) * local_a8 +
                                    *(float *)(iVar1 + 0x38) * local_ac +
                                    *(float *)(iVar1 + 0x40) * local_a4);
                }
                *pfVar4 = fVar2;
              }
LAB_004bfe25:
              if (iVar8 != 0) {
                if (*(int *)(iVar1 + 4) == iVar8) {
                  *pfVar4 = *pfVar4 - (*(float *)(iVar1 + 0x34) * local_80 +
                                      *(float *)(iVar1 + 0x30) * local_84 +
                                      *(float *)(iVar1 + 0x2c) * local_88);
                }
                else if (*(int *)(iVar1 + 8) == iVar8) {
                  *pfVar4 = *(float *)(iVar1 + 0x3c) * local_84 +
                            *(float *)(iVar1 + 0x38) * local_88 +
                            *(float *)(iVar1 + 0x40) * local_80 + *pfVar4;
                }
              }
            }
            else {
              if (*(int *)(iVar1 + 4) == local_7c) {
                fVar2 = *(float *)(iVar1 + 0x30) *
                        ((*(float *)(iVar1 + 0x14) * local_c8 - *(float *)(iVar1 + 0x1c) * local_d0)
                        + local_5c) +
                        *(float *)(iVar1 + 0x34) *
                        ((*(float *)(iVar1 + 0x18) * local_d0 - *(float *)(iVar1 + 0x14) * local_cc)
                        + local_58) +
                        *(float *)(iVar1 + 0x2c) *
                        ((*(float *)(iVar1 + 0x1c) * local_cc - *(float *)(iVar1 + 0x18) * local_c8)
                        + local_60);
LAB_004bffd0:
                *pfVar4 = fVar2;
              }
              else if ((*(int *)(iVar1 + 8) == local_7c) && (*(int *)(iVar1 + 0x5c) != 0xe)) {
                fVar2 = -(*(float *)(iVar1 + 0x38) *
                          ((*(float *)(iVar1 + 0x28) * local_cc -
                           *(float *)(iVar1 + 0x24) * local_c8) + local_60) +
                         *(float *)(iVar1 + 0x3c) *
                         ((*(float *)(iVar1 + 0x20) * local_c8 - *(float *)(iVar1 + 0x28) * local_d0
                          ) + local_5c) +
                         *(float *)(iVar1 + 0x40) *
                         ((*(float *)(iVar1 + 0x24) * local_d0 - *(float *)(iVar1 + 0x20) * local_cc
                          ) + local_58));
                goto LAB_004bffd0;
              }
              if (iVar9 != 0) {
                if (*(int *)(iVar1 + 4) == iVar9) {
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x30) *
                                     ((*(float *)(iVar1 + 0x14) * local_d4 -
                                      *(float *)(iVar1 + 0x1c) * local_dc) + local_50) +
                                    *(float *)(iVar1 + 0x34) *
                                    ((*(float *)(iVar1 + 0x18) * local_dc -
                                     *(float *)(iVar1 + 0x14) * local_d8) + local_4c) +
                                    *(float *)(iVar1 + 0x2c) *
                                    ((*(float *)(iVar1 + 0x1c) * local_d8 -
                                     *(float *)(iVar1 + 0x18) * local_d4) + local_54));
                }
                else {
                  if ((*(int *)(iVar1 + 8) != iVar9) || (*(int *)(iVar1 + 0x5c) == 0xe))
                  goto LAB_004c0107;
                  fVar2 = *(float *)(iVar1 + 0x38) *
                          ((*(float *)(iVar1 + 0x28) * local_d8 -
                           *(float *)(iVar1 + 0x24) * local_d4) + local_54) +
                          *(float *)(iVar1 + 0x3c) *
                          ((*(float *)(iVar1 + 0x20) * local_d4 -
                           *(float *)(iVar1 + 0x28) * local_dc) + local_50) +
                          *(float *)(iVar1 + 0x40) *
                          ((*(float *)(iVar1 + 0x24) * local_dc -
                           *(float *)(iVar1 + 0x20) * local_d8) + local_4c) + *pfVar4;
                }
                *pfVar4 = fVar2;
              }
LAB_004c0107:
              if (local_e0 != 0) {
                if (*(int *)(iVar1 + 4) == local_e0) {
                  fVar2 = *(float *)(iVar1 + 0x30) *
                          ((*(float *)(iVar1 + 0x14) * local_a4 -
                           *(float *)(iVar1 + 0x1c) * local_ac) + local_20) +
                          *(float *)(iVar1 + 0x34) *
                          ((*(float *)(iVar1 + 0x18) * local_ac -
                           *(float *)(iVar1 + 0x14) * local_a8) + local_1c) +
                          *(float *)(iVar1 + 0x2c) *
                          ((*(float *)(iVar1 + 0x1c) * local_a8 -
                           *(float *)(iVar1 + 0x18) * local_a4) + local_24) + *pfVar4;
                }
                else {
                  if (*(int *)(iVar1 + 8) != local_e0) goto LAB_004c0234;
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x38) *
                                     ((*(float *)(iVar1 + 0x28) * local_a8 -
                                      *(float *)(iVar1 + 0x24) * local_a4) + local_24) +
                                    *(float *)(iVar1 + 0x3c) *
                                    ((*(float *)(iVar1 + 0x20) * local_a4 -
                                     *(float *)(iVar1 + 0x28) * local_ac) + local_20) +
                                    *(float *)(iVar1 + 0x40) *
                                    ((*(float *)(iVar1 + 0x24) * local_ac -
                                     *(float *)(iVar1 + 0x20) * local_a8) + local_1c));
                }
                *pfVar4 = fVar2;
              }
LAB_004c0234:
              iVar8 = local_68;
              if (local_68 != 0) {
                if (*(int *)(iVar1 + 4) == local_68) {
                  fVar2 = *pfVar4 - (*(float *)(iVar1 + 0x30) *
                                     ((*(float *)(iVar1 + 0x14) * local_80 -
                                      *(float *)(iVar1 + 0x1c) * local_88) + local_8) +
                                    *(float *)(iVar1 + 0x34) *
                                    ((*(float *)(iVar1 + 0x18) * local_88 -
                                     *(float *)(iVar1 + 0x14) * local_84) + local_4) +
                                    *(float *)(iVar1 + 0x2c) *
                                    ((*(float *)(iVar1 + 0x1c) * local_84 -
                                     *(float *)(iVar1 + 0x18) * local_80) + local_c));
                }
                else {
                  if (*(int *)(iVar1 + 8) != local_68) goto LAB_004c0388;
                  fVar2 = *(float *)(iVar1 + 0x38) *
                          ((*(float *)(iVar1 + 0x28) * local_84 -
                           *(float *)(iVar1 + 0x24) * local_80) + local_c) +
                          *(float *)(iVar1 + 0x3c) *
                          ((*(float *)(iVar1 + 0x20) * local_80 -
                           *(float *)(iVar1 + 0x28) * local_88) + local_8) +
                          *(float *)(iVar1 + 0x40) *
                          ((*(float *)(iVar1 + 0x24) * local_88 -
                           *(float *)(iVar1 + 0x20) * local_84) + local_4) + *pfVar4;
                }
                *pfVar4 = fVar2;
              }
            }
LAB_004c0388:
            *pfVar6 = *pfVar4;
            piVar5 = piVar5 + 1;
            pfVar6 = pfVar6 + 1;
            pfVar4 = pfVar4 + local_78;
            local_70 = local_70 + -1;
            iVar7 = local_74;
          } while (local_70 != 0);
        }
      }
      local_74 = iVar7 + 1;
    } while (local_74 < param_4);
  }
  return;
}
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
#undef local_b8
#undef local_b4
#undef local_b0
#undef local_ac
#undef local_a8
#undef local_a4
#undef local_a0
#undef local_9c
#undef local_98
#undef local_94
#undef local_90
#undef local_8c
#undef local_88
#undef local_84
#undef local_80
#undef local_7c
#undef local_78
#undef local_74
#undef local_70
#undef local_6c
#undef local_68
#undef local_64
#undef local_60
#undef local_5c
#undef local_58
#undef local_54
#undef local_50
#undef local_4c
#undef local_48
#undef local_44
#undef local_40
#undef local_3c
#undef local_38
#undef local_34
#undef local_30
#undef local_2c
#undef local_28
#undef local_24
#undef local_20
#undef local_1c
#undef local_18
#undef local_14
#undef local_10
#undef local_c
#undef local_8
#undef local_4
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004b8cf0
void C2_HOOK_FASTCALL FUN_004b8cf0(tPhysics_object* pObject) {
    int uVar1;
    int uVar2;
    int i;
    int* piVar4;
    char* puVar3;
    int local_174[6];
    float local_15c[15];
    /* retail esp+0x120/0xc0/0x60: three consecutive 0x60-byte solver rows, walked below by
     * stepping 0x60 bytes from the first. Separate locals only worked where the compiler happened
     * to lay them out contiguously (MSVC x86); GCC/ARM did not and the solver read garbage objects
     * (Vita 00.23 crash in FUN_004be4b0 via FUN_004bf490). */
    union {
        tCollision_contact contact;
        tPhysics_joint_data joint;
    } rows[3];
    tPhysics_object* pObject2;
    tPhysics_object* pOther;

    pObject2 = pObject->field_0x230;
    pOther = (tPhysics_object*)*(int*)(pObject->field_0x1e4 + 4);
    if (pOther != pObject2) {
        pObject2 = pObject;
        pObject = pOther;
    }
    InitializeCollisionContact(pObject, pObject2, &rows[0].contact, (br_vector3*)((char*)pObject->physics_joint1 + 0x38));
    FUN_004b8e10(pObject, pObject2, &rows[1].joint, (br_vector3*)((char*)pObject->physics_joint1 + 0x20));
    FUN_004b8e10(pObject, pObject2, &rows[2].joint, (br_vector3*)((char*)pObject->physics_joint1 + 0x2c));
    piVar4 = local_174;
    puVar3 = (char*)rows;
    i = 3;
    do {
        *piVar4 = (int)puVar3;
        puVar3 += 0x60;
        piVar4++;
        i--;
    } while (i != 0);
    uVar1 = pObject2->field_0x1e4;
    uVar2 = pObject->field_0x1e4;
    pObject->field_0x1e4 = 0;
    pObject2->field_0x1e4 = 0;
    FUN_004bf490((int*)&local_15c[3], 3, local_174, 3);
    FUN_004be350((float*)(uintptr_t)(uVar2 + 8), &local_15c[3]);
    FUN_004b77b0(&local_174[3], local_174, 3);
    BrMatrix34ApplyV((br_vector3*)&local_15c[0], (br_vector3*)&local_174[3], (br_matrix34*)(uintptr_t)(uVar2 + 8));
    i = 0;
    do {
        FUN_004b7960((tPhysics_joint_data*)*(int*)((char*)local_174 + i), *(float*)((char*)local_15c + i));
        i += 4;
    } while (i < 0xc);
    pObject->field_0x1e4 = uVar2;
    pObject2->field_0x1e4 = uVar1;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c03f0
void C2_HOOK_FASTCALL ResetOmegaTree(tPhysics_object* pObject) {
    tPhysics_object* obj;

    obj = pObject;
    while (pObject != NULL) {
        pObject->omega.v[0] = pObject->rotate_omega.v[0];
        pObject->omega.v[1] = pObject->rotate_omega.v[1];
        pObject->omega.v[2] = pObject->rotate_omega.v[2];
        if (obj->child != NULL) {
            ResetOmegaTree(obj->child);
        }
        pObject = pObject->next;
        obj = pObject;
    }
}

// C2PORT-NONMATCHING: best 23.40% | difftest PASS 2000 trials (spec difftest/specs/004c0430_FUN_004c0430.json) | reason vc5-register-allocation-and-stack-layout
// FUNCTION: CARMA2_HW 0x004c0430
int C2_HOOK_FASTCALL FUN_004c0430(float* pFactors, void* pContact_list, int pCapacity, int pCount) {
    tCollision_contact** contacts;
    tCollision_contact* contact;
    tPhysics_object* object1;
    tPhysics_object* object2;
    br_vector3 velocity;
    br_vector3 point;
    br_vector3 world_point;
    br_vector3 local_point1;
    br_vector3 local_point2;
    br_vector3 separation;
    br_vector3 transformed_normal;
    br_vector3 normal_world;
    float value;
    float scale;
    float alternate_value;
    int has_bad_contact;
    int has_other_contact;
    int i;

    contacts = (tCollision_contact**)pContact_list;
    has_bad_contact = 0;
    if (pCount < pCapacity) {
        for (i = pCount; i < pCapacity; i++) {
            pFactors[i] = 0.0f;
        }
    }
    for (i = 0; i < pCount; i++) {
        contact = contacts[i];
        object1 = contact->object1;
        object2 = contact->object2;
        if (contact->type == 0xb) {
            value = -(object1->omega.v[2] * contact->normal.v[2]
                + contact->normal.v[1] * object1->omega.v[1]
                + contact->normal.v[0] * object1->omega.v[0]);
            if (object2 != NULL) {
                value += object2->omega.v[1] * contact->field_0x38.v[1]
                    + contact->field_0x38.v[0] * object2->omega.v[0]
                    + contact->field_0x38.v[2] * object2->omega.v[2];
            }
            pFactors[i] = value;
            if (gDOUBLE_0058a7a0 <= value) {
                has_bad_contact = 1;
            } else {
                pFactors[i] = 0.001f;
            }
        } else {
            velocity.v[2] = object1->velocity_car_space.v[2]
                + object1->omega.v[0] * contact->r1.v[1] - contact->r1.v[0] * object1->omega.v[1];
            velocity.v[1] = object1->velocity_car_space.v[1]
                + object1->omega.v[2] * contact->r1.v[0] - contact->r1.v[2] * object1->omega.v[0];
            velocity.v[0] = object1->velocity_car_space.v[0]
                + object1->omega.v[1] * contact->r1.v[2] - contact->r1.v[1] * object1->omega.v[2];
            value = -(contact->normal.v[2] * velocity.v[2] + contact->normal.v[1] * velocity.v[1]
                + contact->normal.v[0] * velocity.v[0]);
            if (object2 != NULL) {
                velocity.v[1] = object2->velocity_car_space.v[1]
                    + object2->omega.v[2] * contact->r2.v[0] - contact->r2.v[2] * object2->omega.v[0];
                velocity.v[0] = object2->velocity_car_space.v[0]
                    + object2->omega.v[1] * contact->r2.v[2] - contact->r2.v[1] * object2->omega.v[2];
                velocity.v[2] = object2->velocity_car_space.v[2]
                    + object2->omega.v[0] * contact->r2.v[1] - contact->r2.v[0] * object2->omega.v[1];
                value += contact->field_0x38.v[1] * velocity.v[1]
                    + contact->field_0x38.v[0] * velocity.v[0]
                    + contact->field_0x38.v[2] * velocity.v[2];
            }
            if (gDOUBLE_0058a798 < value) {
                has_bad_contact = 1;
            }
            if (gDOUBLE_0058a830 < value) {
                scale = (float)gDOUBLE_0058a830 - (contact->impulse - gFLOAT_0058a800);
                if (gFLOAT_0058a800 < scale) {
                    scale = gFLOAT_0058a800;
                }
                if (scale < (float)gDOUBLE_0058a830) {
                    scale = gFLOAT_0058a804;
                }
                value = scale * (value - (float)gDOUBLE_0058a830) - (float)gDOUBLE_0058a838;
            }
            if (value < gFLOAT_0058a768) {
                value = gFLOAT_0058a768;
            }
            value = (float)gDOUBLE_0058a798 - value * (float)gDOUBLE_0058a7d8;
            if ((object2 == NULL || object2->field_0x1df != 0) && object1->field_0x1df != 0) {
                value -= (float)gDOUBLE_0058a840;
            }
            pFactors[i] = value;
        }
    }
    if (!has_bad_contact) {
        has_other_contact = 0;
        for (i = 0; i < pCount; i++) {
            contact = contacts[i];
            object1 = contact->object1;
            object2 = contact->object2;
            if (contact->type == 0xb) {
                value = ((float)gDOUBLE_0058a7a0 - contact->impulse) * (float)gDOUBLE_0058a7e0;
                pFactors[i] = value;
                if ((float)gDOUBLE_0058a7a0 <= value) {
                    has_bad_contact = 1;
                } else {
                    pFactors[i] = 0.001f;
                }
            } else {
                if (object2 == NULL) {
                    point.v[0] = contact->r1.v[0] + object1->cmpos.v[0];
                    point.v[1] = contact->r1.v[1] + object1->cmpos.v[1];
                    point.v[2] = contact->r1.v[2] + object1->cmpos.v[2];
                    BrMatrix34ApplyP(&world_point, &point, &object1->actor->t.t.mat);
                    DrMatrix34ApplyLPInverse(&local_point1, &world_point, &object1->transform_matrix);
                    separation.v[0] = local_point1.v[0] - point.v[0];
                    separation.v[1] = local_point1.v[1] - point.v[1];
                    separation.v[2] = local_point1.v[2] - point.v[2];
                    pFactors[i] = (float)gDOUBLE_0058a798
                        - (contact->normal.v[0] * separation.v[0] + contact->normal.v[2] * separation.v[2]
                            + contact->normal.v[1] * separation.v[1]) * gFLOAT_0058a7c8;
                } else {
                    point.v[0] = contact->r1.v[0] + object1->cmpos.v[0];
                    point.v[1] = contact->r1.v[1] + object1->cmpos.v[1];
                    point.v[2] = contact->r1.v[2] + object1->cmpos.v[2];
                    BrMatrix34ApplyP(&world_point, &point, &object1->transform_matrix);
                    DrMatrix34ApplyLPInverse(&local_point1, &world_point, &object2->transform_matrix);
                    BrMatrix34ApplyP(&world_point, &point, &object1->actor->t.t.mat);
                    DrMatrix34ApplyLPInverse(&local_point2, &world_point, &object2->actor->t.t.mat);
                    separation.v[0] = local_point2.v[0] - local_point1.v[0];
                    separation.v[1] = local_point2.v[1] - local_point1.v[1];
                    separation.v[2] = local_point2.v[2] - local_point1.v[2];
                    pFactors[i] = -(contact->field_0x38.v[2] * separation.v[2]
                        + contact->field_0x38.v[1] * separation.v[1]
                        + contact->field_0x38.v[0] * separation.v[0]) * gFLOAT_0058a7c8;
                    if (contact->type == 5) {
                        BrMatrix34ApplyV(&transformed_normal, &contact->field_0x38, &object2->transform_matrix);
                        BrMatrix34TApplyV(&normal_world, &transformed_normal, &object2->actor->t.t.mat);
                        alternate_value = -(normal_world.v[0] * separation.v[0]
                            + normal_world.v[1] * separation.v[1] + normal_world.v[2] * separation.v[2]) * gFLOAT_0058a7c8;
                        if (pFactors[i] < alternate_value) {
                            pFactors[i] = alternate_value;
                        }
                    }
                    if (contact->field_0x10 > gFLOAT_0058a768) {
                        alternate_value = contact->field_0x10 * gFLOAT_0058a7c8;
                        if (pFactors[i] < alternate_value) {
                            pFactors[i] = alternate_value;
                        }
                    }
                }
                if (pFactors[i] <= (float)gDOUBLE_0058a830) {
                    if (pFactors[i] < gFLOAT_0058a768) {
                        pFactors[i] = gFLOAT_0058a768;
                    }
                } else {
                    scale = (float)gDOUBLE_0058a830 - (contact->impulse - gFLOAT_0058a800);
                    if (gFLOAT_0058a800 < scale) {
                        scale = gFLOAT_0058a800;
                    }
                    if (scale < (float)gDOUBLE_0058a830) {
                        scale = gFLOAT_0058a804;
                    }
                    pFactors[i] = scale * (pFactors[i] - (float)gDOUBLE_0058a830)
                        - (float)gDOUBLE_0058a838;
                }
                if ((float)gDOUBLE_0058a7a0 < pFactors[i]) {
                    has_other_contact = 1;
                }
                pFactors[i] = (float)gDOUBLE_0058a798 - pFactors[i] * (float)gDOUBLE_0058a7d8;
            }
        }
        if (!has_other_contact) {
            for (i = 0; i < pCount; i++) {
                if (contacts[i]->type != 0xb) {
                    pFactors[i] = 0.05f;
                }
            }
        }
    }
    return !has_bad_contact;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c0ac0
void C2_HOOK_FASTCALL RotateObjectFirstOrder(void* pContact, tWorld_callbacks* pWorld_callbacks) {
    tCollision_contact* contact;
    tPhysics_object* obj1;
    tPhysics_object* obj2;
    br_vector3 v1;
    br_vector3 v2;
    br_vector3 rel_vel;
    br_vector3 normal_world;
    br_vector3 tangent;
    br_vector3 impulse;
    br_scalar relative_speed;
    br_scalar denominator;
    br_scalar max_impulse;
    br_scalar mass1_inv;
    br_scalar mass2_inv;
    int i;

    contact = (tCollision_contact*)pContact;
    obj1 = contact->object1;
    obj2 = contact->object2;

    /* Two-object contact: retail 0x4c0ae6..0x4c11e8. Contact velocities and
     * friction directions are evaluated in each actor's local frame. */
    if (obj2 != NULL && contact->type != 0xe) {
        br_vector3 n;
        br_vector3 dir2;
        br_vector3 c1;
        br_vector3 c2;
        br_vector3 e1;
        br_vector3 e2;
        br_vector3 world;
        br_scalar projection;
        br_scalar scale;
        br_scalar j;
        br_scalar j_m1;
        br_scalar j_m2;

        obj1->collision_flag |= 2;
        obj2->collision_flag |= 2;
        v1.v[0] = obj1->omega.v[1] * contact->r1.v[2] - obj1->omega.v[2] * contact->r1.v[1] + obj1->velocity_car_space.v[0];
        v1.v[1] = obj1->omega.v[2] * contact->r1.v[0] - obj1->omega.v[0] * contact->r1.v[2] + obj1->velocity_car_space.v[1];
        v1.v[2] = obj1->omega.v[0] * contact->r1.v[1] - obj1->omega.v[1] * contact->r1.v[0] + obj1->velocity_car_space.v[2];
        v2.v[0] = obj2->omega.v[1] * contact->r2.v[2] - obj2->omega.v[2] * contact->r2.v[1] + obj2->velocity_car_space.v[0];
        v2.v[1] = obj2->omega.v[2] * contact->r2.v[0] - obj2->omega.v[0] * contact->r2.v[2] + obj2->velocity_car_space.v[1];
        v2.v[2] = obj2->omega.v[0] * contact->r2.v[1] - obj2->omega.v[1] * contact->r2.v[0] + obj2->velocity_car_space.v[2];
        BrMatrix34ApplyV(&world, &v2, &obj2->actor->t.t.mat);
        BrMatrix34TApplyV(&v2, &world, &obj1->actor->t.t.mat);
        rel_vel.v[0] = v1.v[0] - v2.v[0];
        rel_vel.v[1] = v1.v[1] - v2.v[1];
        rel_vel.v[2] = v1.v[2] - v2.v[2];
        BrMatrix34ApplyV(&normal_world, &contact->normal, &obj1->transform_matrix);
        BrMatrix34TApplyV(&n, &normal_world, &obj1->actor->t.t.mat);
        projection = n.v[0] * rel_vel.v[0] + n.v[1] * rel_vel.v[1] + n.v[2] * rel_vel.v[2];
        tangent.v[0] = rel_vel.v[0] - projection * n.v[0];
        tangent.v[1] = rel_vel.v[1] - projection * n.v[1];
        tangent.v[2] = rel_vel.v[2] - projection * n.v[2];
        relative_speed = sqrtf(tangent.v[0] * tangent.v[0] + tangent.v[1] * tangent.v[1] + tangent.v[2] * tangent.v[2]);
        max_impulse = contact->impulse * 0.35f;
        max_impulse = obj1->object_friction * max_impulse + max_impulse;
        max_impulse = obj2->object_friction * max_impulse + max_impulse;
        if (relative_speed >= 0.001f) {
            scale = 1.0f / -relative_speed;
            impulse.v[0] = scale * tangent.v[0];
            impulse.v[1] = scale * tangent.v[1];
            impulse.v[2] = scale * tangent.v[2];
            BrMatrix34ApplyV(&world, &impulse, &obj1->actor->t.t.mat);
            BrMatrix34TApplyV(&dir2, &world, &obj2->actor->t.t.mat);
            if (!(obj1->flags & 0x800)) {
                c1.v[0] = (contact->r1.v[1] * impulse.v[2] - contact->r1.v[2] * impulse.v[1]) / obj1->I.v[0];
                c1.v[1] = (contact->r1.v[2] * impulse.v[0] - contact->r1.v[0] * impulse.v[2]) / obj1->I.v[1];
                c1.v[2] = (contact->r1.v[0] * impulse.v[1] - contact->r1.v[1] * impulse.v[0]) / obj1->I.v[2];
            } else {
                BrVector3SetFloat(&c1, 0.f, 0.f, 0.f);
            }
            if (!(obj2->flags & 0x800)) {
                c2.v[0] = (contact->r2.v[1] * dir2.v[2] - contact->r2.v[2] * dir2.v[1]) / obj2->I.v[0];
                c2.v[1] = (contact->r2.v[2] * dir2.v[0] - contact->r2.v[0] * dir2.v[2]) / obj2->I.v[1];
                c2.v[2] = (contact->r2.v[0] * dir2.v[1] - contact->r2.v[1] * dir2.v[0]) / obj2->I.v[2];
            } else {
                BrVector3SetFloat(&c2, 0.f, 0.f, 0.f);
            }
            e1.v[0] = c1.v[1] * contact->r1.v[2] - c1.v[2] * contact->r1.v[1];
            e1.v[1] = c1.v[2] * contact->r1.v[0] - c1.v[0] * contact->r1.v[2];
            e1.v[2] = c1.v[0] * contact->r1.v[1] - c1.v[1] * contact->r1.v[0];
            e2.v[0] = c2.v[1] * contact->r2.v[2] - c2.v[2] * contact->r2.v[1];
            e2.v[1] = c2.v[2] * contact->r2.v[0] - c2.v[0] * contact->r2.v[2];
            e2.v[2] = c2.v[0] * contact->r2.v[1] - c2.v[1] * contact->r2.v[0];
            mass1_inv = 1.0f / obj1->M;
            mass2_inv = 1.0f / obj2->M;
            denominator = mass1_inv + mass2_inv
                + impulse.v[0] * e1.v[0] + impulse.v[1] * e1.v[1] + impulse.v[2] * e1.v[2]
                + dir2.v[0] * e2.v[0] + dir2.v[1] * e2.v[1] + dir2.v[2] * e2.v[2];
            if (fabsf(denominator) > 0.0001f) {
                j = relative_speed / denominator;
                if (!(j <= max_impulse)) {
                    j = max_impulse;
                }
                j_m1 = j / obj1->M;
                j_m2 = -(j / obj2->M);
                for (i = 0; i < 3; i++) {
                    impulse.v[i] *= j_m1;
                    dir2.v[i] *= j_m2;
                    contact->impulse1.v[i] = obj1->M * impulse.v[i];
                    contact->impulse2.v[i] = obj2->M * dir2.v[i];
                    obj1->omega.v[i] += j * c1.v[i];
                    obj2->omega.v[i] += -j * c2.v[i];
                }
                contact->type |= 0x100;
                BrMatrix34ApplyV(&world, &impulse, &obj1->actor->t.t.mat);
                for (i = 0; i < 3; i++) {
                    obj1->v.v[i] += world.v[i];
                    obj1->velocity_car_space.v[i] += impulse.v[i];
                }
                BrMatrix34ApplyV(&world, &dir2, &obj2->actor->t.t.mat);
                for (i = 0; i < 3; i++) {
                    obj2->v.v[i] += world.v[i];
                    obj2->velocity_car_space.v[i] += dir2.v[i];
                }
            }
        }
        FUN_004c1840(contact->object1);
        FUN_004c1840(contact->object2);
    } else {
        /* World contact (no second object, or type 0xe): retail 0x4c11e9..0x4c1837. Friction against the
         * world: the impulse is limited by the face friction and the object's world_friction. The port had
         * dropped this branch, so pedestrian bodies and loose parts slid on the ground forever. */
        br_vector3 r;
        br_vector3 v;
        br_vector3 n_world;
        br_vector3 n;
        br_vector3 dir;
        br_vector3 c;
        br_vector3 e;
        br_vector3 dv;
        br_scalar contact_impulse;
        br_scalar friction;
        br_scalar speed;
        br_scalar w_n;
        br_scalar j;
        br_scalar j_m;
        br_scalar d;
        float (C2_HOOK_FASTCALL * get_friction)(void*);
        void* face;

        contact_impulse = contact->impulse;
        obj1->collision_flag |= 1;
        if (contact->type == 1) {
            r.v[0] = contact->r1.v[0] * -0.005f + contact->r1.v[0];
            r.v[1] = contact->r1.v[1] * -0.005f + contact->r1.v[1];
            r.v[2] = contact->r1.v[2] * -0.005f + contact->r1.v[2];
        } else {
            r = contact->r1;
        }
        /* velocity of the contact point: omega x r + velocity_car_space */
        v.v[0] = obj1->omega.v[1] * r.v[2] - obj1->omega.v[2] * r.v[1] + obj1->velocity_car_space.v[0];
        v.v[1] = obj1->omega.v[2] * r.v[0] - obj1->omega.v[0] * r.v[2] + obj1->velocity_car_space.v[1];
        v.v[2] = obj1->omega.v[0] * r.v[1] - obj1->omega.v[1] * r.v[0] + obj1->velocity_car_space.v[2];
        BrMatrix34ApplyV(&n_world, &contact->normal, &obj1->transform_matrix);
        BrMatrix34TApplyV(&n, &n_world, &obj1->actor->t.t.mat);
        /* tangential part */
        d = n.v[0] * v.v[0] + n.v[1] * v.v[1] + n.v[2] * v.v[2];
        v.v[0] -= d * n.v[0];
        v.v[1] -= d * n.v[1];
        v.v[2] -= d * n.v[2];

        face = contact->field_0xc;
        get_friction = pWorld_callbacks != NULL ? pWorld_callbacks->get_friction_from_face : NULL;
        if (get_friction != NULL && face != NULL) {
            friction = get_friction(face) * contact_impulse;
        } else {
            friction = contact_impulse;
        }
        max_impulse = friction * 0.35f;
        max_impulse = obj1->world_friction * max_impulse + max_impulse;
        speed = sqrtf(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
        if (speed < 0.001f) {
            return;
        }
        d = 1.0f / -speed;
        dir.v[0] = d * v.v[0];
        dir.v[1] = d * v.v[1];
        dir.v[2] = d * v.v[2];
        if (!(obj1->flags & 0x800)) {
            c.v[0] = (contact->r1.v[1] * dir.v[2] - contact->r1.v[2] * dir.v[1]) / obj1->I.v[0];
            c.v[1] = (contact->r1.v[2] * dir.v[0] - contact->r1.v[0] * dir.v[2]) / obj1->I.v[1];
            c.v[2] = (contact->r1.v[0] * dir.v[1] - contact->r1.v[1] * dir.v[0]) / obj1->I.v[2];
        } else {
            c.v[0] = 0.f;
            c.v[1] = 0.f;
            c.v[2] = 0.f;
        }
        w_n = obj1->omega.v[1] * contact->normal.v[1] + obj1->omega.v[2] * contact->normal.v[2] + obj1->omega.v[0] * contact->normal.v[0];
        if (fabsf(w_n) < 0.1f
                && obj1->omega.v[1] * c.v[1] + obj1->omega.v[2] * c.v[2] + obj1->omega.v[0] * c.v[0] < 0.f
                && obj1->v.v[0] * obj1->v.v[0] + obj1->v.v[1] * obj1->v.v[1] + obj1->v.v[2] * obj1->v.v[2] < 0.001) {
            /* nearly at rest: drop the spin about the normal and brake the remaining sliding velocity */
            obj1->omega.v[0] -= contact->normal.v[0] * w_n;
            obj1->omega.v[1] -= contact->normal.v[1] * w_n;
            obj1->omega.v[2] -= contact->normal.v[2] * w_n;
            BrVector3SetFloat(&c, 0.f, 0.f, 0.f);
            speed = sqrtf(obj1->velocity_car_space.v[0] * obj1->velocity_car_space.v[0]
                + obj1->velocity_car_space.v[1] * obj1->velocity_car_space.v[1]
                + obj1->velocity_car_space.v[2] * obj1->velocity_car_space.v[2]);
            if (speed < 0.001f) {
                return;
            }
            dir.v[0] = obj1->velocity_car_space.v[0] / -speed;
            dir.v[1] = obj1->velocity_car_space.v[1] / -speed;
            dir.v[2] = obj1->velocity_car_space.v[2] / -speed;
        }
        mass1_inv = 1.0f / obj1->M;
        e.v[0] = c.v[1] * contact->r1.v[2] - c.v[2] * contact->r1.v[1];
        e.v[1] = c.v[2] * contact->r1.v[0] - c.v[0] * contact->r1.v[2];
        e.v[2] = c.v[0] * contact->r1.v[1] - c.v[1] * contact->r1.v[0];
        denominator = dir.v[0] * e.v[0] + dir.v[1] * e.v[1] + dir.v[2] * e.v[2] + mass1_inv;
        if (fabsf(denominator) <= 0.0001f) {
            return;
        }
        j = speed / denominator;
        if (!(j <= max_impulse)) {
            j = max_impulse;
        }
        c.v[0] *= j;
        c.v[1] *= j;
        c.v[2] *= j;
        j_m = j / obj1->M;
        contact->type |= 0x100;
        dir.v[0] *= j_m;
        dir.v[1] *= j_m;
        dir.v[2] *= j_m;
        contact->impulse1.v[0] = obj1->M * dir.v[0];
        contact->impulse1.v[1] = obj1->M * dir.v[1];
        contact->impulse1.v[2] = obj1->M * dir.v[2];
        obj1->omega.v[0] += c.v[0];
        obj1->omega.v[1] += c.v[1];
        obj1->omega.v[2] += c.v[2];
        BrMatrix34ApplyV(&dv, &dir, &obj1->actor->t.t.mat);
        obj1->v.v[0] += dv.v[0];
        obj1->v.v[1] += dv.v[1];
        obj1->v.v[2] += dv.v[2];
        obj1->velocity_car_space.v[0] += dir.v[0];
        obj1->velocity_car_space.v[1] += dir.v[1];
        obj1->velocity_car_space.v[2] += dir.v[2];
        FUN_004c1840(contact->object1);
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 48.00% | difftest PASS 2000 trials (spec difftest/specs/004c1840_FUN_004c1840.json) | reason x87-scheduling
// FUNCTION: CARMA2_HW 0x004c1840
void C2_HOOK_FASTCALL FUN_004c1840(volatile tPhysics_object* pObject) {
    br_scalar value;

    value = (pObject->omega.v[0] * pObject->omega.v[0]
           + pObject->omega.v[1] * pObject->omega.v[1]
           + pObject->omega.v[2] * pObject->omega.v[2]) * gFLOAT_0058a824;
    if (value > gFLOAT_0058a828) {
        value = sqrtf(value) * gFLOAT_0058a85c;
        value = value + value;
        pObject->omega.v[0] /= value;
        pObject->omega.v[1] /= value;
        pObject->omega.v[2] /= value;
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 42.62% | difftest PASS 5000 trials (spec difftest/specs/004c18d0_TranslateObject.json + _joint2) | reason x87 scheduling
// FUNCTION: CARMA2_HW 0x004c18d0
void C2_HOOK_FASTCALL TranslateObject(tPhysics_object* pObject) {
    tPhysics_joint* joint;
    tPhysics_object* parent;
    tPhysics_object* child;
    br_vector3 joint_axis_world;
    br_vector3 omega_parent;
    br_vector3 parent_share[3];
    br_scalar relative;
    int j;
    br_vector3 impulse;
    br_scalar friction;
    br_scalar friction_sq;
    br_scalar sum_sq;
    br_scalar scale;
    br_scalar denominator;
    int i;

    /* Joint 1 (connection to the parent object): per object axis k, take row k of our matrix in the parent's frame,
     * compute the impulse that removes the relative spin about it, limit the total by the joint friction and apply it
     * to both objects (derived from the original asm at 0x4c18d0). */
    joint = pObject->physics_joint1;
    if (joint != NULL && joint->friction != 0.0f && pObject->field_0x230 != NULL) {
        friction = joint->friction * 0.04f;
        friction_sq = friction * friction;
        sum_sq = 0.0f;
        for (i = 0; i < 3; i++) {
            BrMatrix34TApplyV(&joint_axis_world, (br_vector3*)pObject->transform_matrix.m[i], &pObject->field_0x230->transform_matrix);
            parent = pObject->field_0x230;
            relative = parent->omega.v[0] * joint_axis_world.v[0]
                + parent->omega.v[1] * joint_axis_world.v[1]
                + parent->omega.v[2] * joint_axis_world.v[2]
                - pObject->omega.v[i];
            for (j = 0; j < 3; j++) {
                parent_share[i].v[j] = joint_axis_world.v[j] / parent->I.v[j];
            }
            denominator = 1.0 / pObject->I.v[i]
                + parent_share[i].v[0] * joint_axis_world.v[0]
                + parent_share[i].v[1] * joint_axis_world.v[1]
                + parent_share[i].v[2] * joint_axis_world.v[2];
            impulse.v[i] = relative / denominator;
            sum_sq += impulse.v[i] * impulse.v[i];
        }
        if (sum_sq > friction_sq) {
            scale = sqrtf(friction_sq / sum_sq);
            impulse.v[0] *= scale;
            impulse.v[1] *= scale;
            impulse.v[2] *= scale;
        }
        for (i = 0; i < 3; i++) {
            pObject->omega.v[i] += impulse.v[i] / pObject->I.v[i];
            parent_share[i].v[0] *= -impulse.v[i];
            parent_share[i].v[1] *= -impulse.v[i];
            parent_share[i].v[2] *= -impulse.v[i];
            pObject->field_0x230->omega.v[0] += parent_share[i].v[0];
            pObject->field_0x230->omega.v[1] += parent_share[i].v[1];
            pObject->field_0x230->omega.v[2] += parent_share[i].v[2];
        }
    }

    /* Process joint2 constraints */
    joint = pObject->physics_joint2;
    if (joint != NULL && joint->friction != 0.0f) {
        friction = joint->friction * 0.04f;
        friction_sq = friction * friction;
        sum_sq = 0.0f;

        /* Compute omega × I */
        for (i = 0; i < 3; i++) {
            impulse.v[i] = pObject->omega.v[i] * pObject->I.v[i];
            sum_sq += impulse.v[i] * impulse.v[i];
        }

        /* Scale if needed */
        if (friction_sq < sum_sq) {
            scale = sqrtf(friction_sq / sum_sq);
            impulse.v[0] *= scale;
            impulse.v[1] *= scale;
            impulse.v[2] *= scale;
        }

        /* Apply impulse */
        for (i = 0; i < 3; i++) {
            pObject->omega.v[i] -= impulse.v[i] / pObject->I.v[i];
        }
    }

    /* Recursively process children */
    for (child = pObject->child; child != NULL; child = child->next) {
        if (child->disable_move_rotate == 0) {
            TranslateObject(child);
        }
    }
}
#pragma auto_inline(on)
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c1be0
void C2_HOOK_FAKE_THISCALL RotateObject(tPhysics_object* pObject, undefined4 pArg2, float pDelta_time) {
    union {
        br_vector3 axis;
        br_vector3 omega;
    } axis_omega;
    br_vector3 angular_momentum;
    br_vector3 rotated_angular_momentum;
    br_matrix34 rotation;
    br_vector3 translation;
    tU16 packed_rotation[4];
    br_scalar rad_rate;
    br_scalar rad;
    br_scalar rad_squared;
    int steps;
    int i;

    pObject->rotate_omega.v[0] = pObject->omega.v[0];
    rad_squared = pDelta_time * pDelta_time * BrVector3LengthSquared(&pObject->omega);
    pObject->rotate_omega.v[1] = pObject->omega.v[1];
    pObject->rotate_omega.v[2] = pObject->omega.v[2];
    if (rad_squared >= gDOUBLE_0058a860) {
        if (rad_squared > gFLOAT_0058a868) {
            steps = (int)sqrtf(rad_squared * gFLOAT_0058a86c) + 1;
            pDelta_time /= steps;
            for (i = 0; i < steps && i < 400; i++) {
                rad_rate = BrVector3Length(&pObject->omega);
                axis_omega.axis.v[0] = pObject->omega.v[0] / rad_rate;
                axis_omega.axis.v[1] = pObject->omega.v[1] / rad_rate;
                axis_omega.axis.v[2] = pObject->omega.v[2] / rad_rate;
                angular_momentum.v[0] = pObject->I.v[0] * pObject->omega.v[0];
                angular_momentum.v[1] = pObject->I.v[1] * pObject->omega.v[1];
                angular_momentum.v[2] = pObject->I.v[2] * pObject->omega.v[2];
                rad = rad_rate * pDelta_time;
                BrMatrix34Rotate(&rotation, (br_angle)(((br_angle)(rad * gDOUBLE_0058a770)) / 2), &axis_omega.axis);
                BrMatrix34TApplyV(&rotated_angular_momentum, &angular_momentum, &rotation);
                axis_omega.omega.v[0] = rotated_angular_momentum.v[0] / pObject->I.v[0];
                axis_omega.omega.v[1] = rotated_angular_momentum.v[1] / pObject->I.v[1];
                axis_omega.omega.v[2] = rotated_angular_momentum.v[2] / pObject->I.v[2];

                rad_rate = BrVector3Length(&axis_omega.omega);
                axis_omega.axis.v[0] = axis_omega.omega.v[0] / rad_rate;
                axis_omega.axis.v[1] = axis_omega.omega.v[1] / rad_rate;
                axis_omega.axis.v[2] = axis_omega.omega.v[2] / rad_rate;
                rad = rad_rate * pDelta_time;
                BrMatrix34Rotate(&rotation, (br_angle)(rad * gDOUBLE_0058a770), &axis_omega.axis);
                BrMatrix34PreTranslate(&rotation, -pObject->cmpos.v[0], -pObject->cmpos.v[1], -pObject->cmpos.v[2]);
                BrMatrix34PostTranslate(&rotation, pObject->cmpos.v[0], pObject->cmpos.v[1], pObject->cmpos.v[2]);
                BrMatrix34Pre(&pObject->actor->t.t.mat, &rotation);
                BrMatrix34TApplyV(&rotated_angular_momentum, &angular_momentum, &rotation);
                pObject->omega.v[0] = rotated_angular_momentum.v[0] / pObject->I.v[0];
                pObject->omega.v[1] = rotated_angular_momentum.v[1] / pObject->I.v[1];
                pObject->omega.v[2] = rotated_angular_momentum.v[2] / pObject->I.v[2];
            }
        } else {
            RotatePhysicsObject(pObject, *(undefined4*)&pDelta_time, pDelta_time);
        }
        if (gNet_mode != 0) {
            PackMatrixQuaternion(packed_rotation, &pObject->actor->t.t.mat);
            translation.v[0] = pObject->actor->t.t.mat.m[3][0];
            translation.v[1] = pObject->actor->t.t.mat.m[3][1];
            translation.v[2] = pObject->actor->t.t.mat.m[3][2];
            UnpackMatrixQuaternion(&pObject->actor->t.t.mat, packed_rotation);
            pObject->actor->t.t.mat.m[3][0] = translation.v[0];
            pObject->actor->t.t.mat.m[3][1] = translation.v[1];
            pObject->actor->t.t.mat.m[3][2] = translation.v[2];
        }
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c1f10
void C2_HOOK_FAKE_THISCALL RotatePhysicsObject(tPhysics_object* pObject, undefined4 pArg2, float pDelta_time) {
    br_vector3 axis;
    br_vector3 angular_momentum;
    br_vector3 rotated_angular_momentum;
    br_matrix34 rotation;
    br_scalar omega_length;

    (void)pArg2;

    omega_length = BrVector3Length(&pObject->omega);
    pDelta_time *= omega_length;
    if (pDelta_time < 0.0001) {
        return;
    }

    axis.v[0] = pObject->omega.v[0] / omega_length;
    axis.v[1] = pObject->omega.v[1] / omega_length;
    axis.v[2] = pObject->omega.v[2] / omega_length;
    angular_momentum.v[0] = pObject->I.v[0] * pObject->omega.v[0];
    angular_momentum.v[1] = pObject->omega.v[1] * pObject->I.v[1];
    angular_momentum.v[2] = pObject->I.v[2] * pObject->omega.v[2];

    BrMatrix34Rotate(&rotation, BrRadianToAngle(pDelta_time), &axis);
    BrMatrix34TApplyV(&rotated_angular_momentum, &angular_momentum, &rotation);
    BrMatrix34PreTranslate(&rotation, -pObject->cmpos.v[0], -pObject->cmpos.v[1], -pObject->cmpos.v[2]);
    BrMatrix34PostTranslate(&rotation, pObject->cmpos.v[0], pObject->cmpos.v[1], pObject->cmpos.v[2]);
    BrMatrix34Pre(&pObject->actor->t.t.mat, &rotation);
    pObject->omega.v[0] = rotated_angular_momentum.v[0] / pObject->I.v[0];
    pObject->omega.v[1] = rotated_angular_momentum.v[1] / pObject->I.v[1];
    pObject->omega.v[2] = rotated_angular_momentum.v[2] / pObject->I.v[2];
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004ba4f0
int C2_HOOK_FASTCALL CanPhysicsObjectsCollide(volatile tPhysics_object* pObject1, volatile tPhysics_object* pObject2) {
    int rejected;
    int mask;

    mask = pObject2->field_0x1a4;
    if ((mask & pObject1->field_0x1a0) != 0) {
        rejected = 1;
    } else {
        mask = pObject1->field_0x1a4;
        if ((pObject2->field_0x1a0 & mask) != 0) {
            rejected = 1;
        } else if (((*(tU8*)((tU8*)pObject1 + 0x19c) & 4) != 0)
                || ((*(tU8*)((tU8*)pObject2 + 0x19c) & 4) != 0)) {
            rejected = 1;
        } else {
            rejected = 0;
        }
    }
    return !rejected;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 58.70% | difftest PASS 2000 trials (spec difftest/specs/004ba6b0_DoCollisions.json) | reason scratch-frame and list-merge codegen
// FUNCTION: CARMA2_HW 0x004ba6b0
void C2_HOOK_FASTCALL DoCollisions(tPhysics_object** pObject_list, tWorld_callbacks* pWorld_callbacks) {
    char* pBuf1;
    char* pBuf2;
    char* tmp;
    tPhysics_object* obj;
    tPhysics_object* head;
    tPhysics_object* tail;
    tPhysics_object* cur;
    tPhysics_object* next;
    tPhysics_object* pos;
    tPhysics_object* sibling;
    tPhysics_object* node;
    tCollision_contact* contact;
    tU32 marker;
    int mask;
    int loops;
    int i;
    int r;
    char bufB[0xF00];
    char bufA[0xF00];

    pBuf1 = bufA;
    pBuf2 = bufB;

    ClearPhysicsScratchSpace();

    gINT_006940a0 = 1;
    gINT_00692ddc = 0;
    gFLOAT_00692dd0 = 0.f;
    gFLOAT_00692dd4 = 0.f;

    for (obj = *pObject_list; obj != NULL; obj = obj->next) {
        InternalPrepareObject(obj);
    }

    head = *pObject_list;
    head->prev = NULL;
    tail = head;
    cur = head->next;
    while (cur != NULL) {
        pos = tail;
        while (pos != NULL && cur->transform_matrix.m[3][0] < pos->transform_matrix.m[3][0]) {
            pos = pos->prev;
        }
        if (pos == tail) {
            tail->next = cur;
            cur->prev = tail;
            tail = cur;
            cur = cur->next;
        } else if (pos != NULL) {
            next = cur->next;
            cur->next = pos->next;
            pos->next->prev = cur;
            pos->next = cur;
            cur->prev = pos;
            cur = next;
        } else {
            head->prev = cur;
            next = cur->next;
            cur->next = head;
            head = cur;
            cur->prev = NULL;
            cur = next;
        }
    }
    tail->next = NULL;
    *pObject_list = head;

    do {
        gPrepared_objects = 0;
        for (obj = *pObject_list; obj != NULL; obj = obj->next) {
            if (obj->field_0x1dc != 0) {
                continue;
            }
            if (obj->disable_move_rotate != 0) {
                continue;
            }
            if (obj->child != NULL && obj->child->physics_joint1 != NULL && obj->child->physics_joint1->type != 0) {
                marker = gINT_006940a0;
                obj->field_0x1dc = (tU8)marker;
                for (sibling = obj->child; sibling != NULL; sibling = sibling->next) {
                    if (sibling->physics_joint1 != NULL && sibling->physics_joint1->type != 0) {
                        DRMatrix33Inverse(sibling, marker);
                    }
                }
                gINT_006940a0++;
            }
            loops = 0;
            gINT_006940a4 = 0;
            r = SetUpQuickHingeData(pObject_list, obj, pBuf1, pBuf2, 0, pWorld_callbacks);
            while (r != 0) {
                loops++;
                tmp = pBuf1;
                pBuf1 = pBuf2;
                pBuf2 = tmp;
                if (loops >= 0x1e) {
                    break;
                }
                if (gINT_006940a4 >= 3) {
                    break;
                }
                r = SetUpQuickHingeData(pObject_list, obj, pBuf1, pBuf2, r, pWorld_callbacks);
            }
            if (loops >= 0x1e) {
                PhysicsWarning("Too many loops in DoCollisions");
            }
        }
    } while (gPrepared_objects != 0);

    for (i = 0, contact = gCollision_contacts; i < gINT_00692ddc; i++, contact++) {
        RotateObjectFirstOrder(contact, pWorld_callbacks);
    }

    AddDoubleTorqueToMatrix(*pObject_list);

    for (obj = *pObject_list; obj != NULL; obj = obj->next) {
        if (obj->disable_move_rotate != 0) {
            continue;
        }
        if (!(obj->field_0x1df & 0x2)) {
            continue;
        }
        mask = 0x2;
        for (node = obj->field183_0x1d8; node != obj; node = node->field183_0x1d8) {
            mask &= node->field_0x1df;
        }
        if (!(mask & 0x2)) {
            continue;
        }
        obj->disable_move_rotate = 1;
        for (cur = obj;; cur = cur->field183_0x1d8) {
            sibling = cur->child;
            cur->disable_move_rotate = 1;
            cur->field_0x1df = 0;
            if (sibling != NULL) {
                for (sibling = cur->child; sibling != NULL; sibling = sibling->next) {
                    sibling->disable_move_rotate = 1;
                    sibling->field_0x1df = 0;
                    if (sibling->child != NULL) {
                        SetCollisionInfoDoNothing(sibling->child, 1);
                    }
                }
            }
            if (cur->field183_0x1d8 == NULL || cur->field183_0x1d8 == obj) {
                break;
            }
        }
    }

    for (obj = *pObject_list; obj != NULL; obj = obj->next) {
        if (obj->disable_move_rotate == 0) {
            TranslateObject(obj);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004b61a0
int C2_HOOK_FASTCALL PHILSetPassiveObjectsMatrix(tPhysics_object* pObject, br_matrix34* pMatrix) {
    tCollision_info_owner* owner;

    if (gPHIL_enabled) {
        return 0;
    }

    owner = pObject->field_0x240;
    if (pObject->field_0x239 == 2) {
        memcpy((char*)owner + 0x30, pMatrix, sizeof(br_matrix34));
        ((char*)owner)[0x60] |= 0x4;
        return 0;
    }

    if (owner == NULL) {
        return 3;
    }
    if (owner->field_0x08 != 1) {
        return 4;
    }

    memcpy((char*)(*(void**)&owner->field_0x00) + 0x2c, pMatrix, sizeof(br_matrix34));
    return 0;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c2830
void C2_HOOK_FAKE_THISCALL RepositionObjectGroup(tPhysics_object* pObject, float pDt) {
    tPhysics_object* pChild;
    float t0;
    float t1;
    float t2;

    t0 = pDt * pObject->v.v[0];
    t1 = pDt * pObject->v.v[1];
    t2 = pDt * pObject->v.v[2];
    pObject->actor->t.t.mat.m[3][0] += t0;
    pObject->actor->t.t.mat.m[3][1] += t1;
    pObject->actor->t.t.mat.m[3][2] += t2;
    InterpolateSingleObject(pObject, pDt);
    for (pChild = pObject->child; pChild != NULL; pChild = pChild->next) {
        RepositionObjectGroup(pChild, pDt);
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c2670
void C2_HOOK_FAKE_THISCALL InterpolateSingleObject(tPhysics_object* pObject, float pDt) {
    br_vector3 temp;
    br_vector3 axis;
    float len;
    float angle;

    if (pObject->disable_move_rotate) {
        return;
    }

    len = BrVector3Length(&pObject->rotate_omega);
    axis.v[0] = pObject->rotate_omega.v[0] / len;
    axis.v[1] = pObject->rotate_omega.v[1] / len;
    axis.v[2] = pObject->rotate_omega.v[2] / len;
    angle = len * pDt;
    if (fabs(angle) < 0.0001) {
        return;
    }

    BrMatrix34ApplyV(&temp, &pObject->cmpos, &pObject->actor->t.t.mat);
    pObject->actor->t.t.mat.m[3][0] += temp.v[0];
    pObject->actor->t.t.mat.m[3][1] += temp.v[1];
    pObject->actor->t.t.mat.m[3][2] += temp.v[2];
    BrMatrix34PreRotate(&pObject->actor->t.t.mat, BrRadianToAngle(angle), &axis);
    BrMatrix34ApplyV(&temp, &pObject->cmpos, &pObject->actor->t.t.mat);
    pObject->actor->t.t.mat.m[3][0] -= temp.v[0];
    pObject->actor->t.t.mat.m[3][1] -= temp.v[1];
    pObject->actor->t.t.mat.m[3][2] -= temp.v[2];
}

// C2PORT-NONMATCHING: best 80.46% | difftest PASS 2000 trials (spec difftest/specs/004c28a0_FUN_004c28a0.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x004c28a0
void C2_HOOK_FAKE_THISCALL FUN_004c28a0(tPhysics_object* pObject, float pDt) {
    float t0;
    float t1;
    float t2;

    for (; pObject != NULL; pObject = pObject->next) {
        t0 = pDt * pObject->v.v[0];
        t1 = pDt * pObject->v.v[1];
        t2 = pDt * pObject->v.v[2];
        pObject->actor->t.t.mat.m[3][0] += t0;
        pObject->actor->t.t.mat.m[3][1] += t1;
        pObject->actor->t.t.mat.m[3][2] += t2;
        InterpolateSingleObject(pObject, pDt);
        if (pObject->child != NULL) {
            FUN_004c28a0(pObject->child, pDt);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004c27c0
void C2_HOOK_FAKE_THISCALL InterpolateObjects(tPhysics_object* pObject, float pDt) {
    br_vector3 omega_world;

    pObject->rotate_omega.v[0] = pObject->omega.v[0];
    pObject->rotate_omega.v[1] = pObject->omega.v[1];
    pObject->rotate_omega.v[2] = pObject->omega.v[2];
    BrMatrix34ApplyV(&omega_world, &pObject->omega, &pObject->actor->t.t.mat);
    InterpolateSingleObject(pObject, pDt);
    BrMatrix34TApplyV(&pObject->omega, &omega_world, &pObject->actor->t.t.mat);
}

// FUNCTION: CARMA2_HW 0x004b6be0
void C2_HOOK_FASTCALL ChangedObjectsCallbacks(tPhysics_object* pObjects, tPhysics_callbacks* pCallbacks, tU32 pPeriod) {
    tPhysics_object* pObject;

    if (pObjects == NULL) {
        return;
    }

    for (pObject = pObjects; pObject != NULL; pObject = pObject->next) {
        tU32 val;
        tCollision_info_owner* owner;
        tPhysics_object* child;

        if (gPHIL_enabled) {
            val = 0;
        } else if (pObject->field_0x239 == 2) {
            val = ((tCollision_info_owner*)pObject->field_0x240)->field_0x08;
        } else {
            owner = pObject->field_0x240;
            val = owner != NULL ? owner->field_0x08 : 0;
        }

        if (val != 2) {
            continue;
        }

        if (pObject->disable_move_rotate) {
            owner = pObject->field_0x240;
            if (owner == NULL) {
                continue;
            }
            if (!(owner->field_0x04 & 0x2)) {
                continue;
            }
            if (pCallbacks->active_halted != NULL && !pCallbacks->active_halted(pObject)) {
                continue;
            }
            if (gPHIL_enabled) {
                continue;
            }
            if (pObject->field_0x239 == 2) {
                ((tCollision_info_owner*)pObject->field_0x240)->field_0x08 = 1;
            } else {
                if (owner == NULL) {
                    continue;
                }
                owner->field_0x08 = 1;
                pObject->flags |= 0x20;
            }
        } else {
            if (pCallbacks->changed_objects != NULL) {
                pCallbacks->changed_objects(pObject, pPeriod);
            }
            owner = pObject->field_0x240;
            if (owner == NULL) {
                continue;
            }
            if (!(owner->field_0x04 & 0x4)) {
                continue;
            }
            for (child = pObject->child; child != NULL; child = child->next) {
                PipeSinglePHILObject(child);
            }
        }
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c25a0
static void ResetObjectAndChildren(tPhysics_object* pObject) {
    tPhysics_object* child;

    if (pObject->field_0xf0 == 1) {
        BrMatrix34Copy(&pObject->actor->t.t.mat, (const br_matrix34*)&pObject->field_0xbc);
    } else {
        BrMatrix34LPNormalise(&pObject->actor->t.t.mat, &pObject->transform_matrix);
    }
    for (child = pObject->child; child != NULL; child = child->next) {
        ResetObjectAndChildren(child);
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004c2600
void C2_HOOK_FASTCALL ResetObjectList(tPhysics_object* pObjects) {
    tPhysics_object* pObject;

    pObject = pObjects;
    while (pObject != NULL) {
        tPhysics_object* child;

#ifndef CARPOCALYPSE2_MATCHING
        /* Passive pedestrians submit an animation trial pose every render frame.
         * Keep it between physics steps, as InternalPrepareObject does on a full
         * step, instead of rewinding only the root to the previous physics pose. */
        if (!PedestrianOwnsPassiveRoot(pObject)) {
#endif
        if (pObject->field_0xf0 == 1) {
            BrMatrix34Copy(&pObject->actor->t.t.mat, (const br_matrix34*)&pObject->field_0xbc);
        } else {
            BrMatrix34LPNormalise(&pObject->actor->t.t.mat, &pObject->transform_matrix);
        }
#ifndef CARPOCALYPSE2_MATCHING
        }
#endif
        for (child = pObject->child; child != NULL; child = child->next) {
            ResetObjectAndChildren(child);
        }
        pObject = pObject->next;
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004c64b0
int C2_HOOK_FASTCALL PhysicsObjectRecurseChildren(tPhysics_object* pObject, tEnumCollision_cbfn* pCallback, void* pContext) {
    tPhysics_object* child;

    for (child = pObject->child; child != NULL; child = child->next) {
        int r;

        r = PhysicsObjectRecurse(child, pCallback, pContext);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0049c5e0
int C2_HOOK_STDCALL QuantizeFloatToInt(float pVal, float pMin, float pMax) {

    if (pVal > pMax) {
        pVal = pMax;
    }
    if (pVal < pMin) {
        pVal = pMin;
    }
    return (int)((pVal - pMin) / (pMax - pMin) * 65534.f - (-0.5f));
}

// FUNCTION: CARMA2_HW 0x0049c0d0
int C2_HOOK_FASTCALL GetObjectNetworkStuff(tPhysics_object* pObject, tU8* pBuffer, int pSize) {
    tPhysics_joint* joint;
    br_quat quat;

    joint = pObject->physics_joint1;
    if (joint == NULL) {
        goto check_big;
    }
    switch (joint->type) {
    case 1:
    case 4:
        if ((tU32)pSize < 0xc) {
            return -1;
        }
        pBuffer[0] = (tU8)joint->type;
        pBuffer[1] = pObject->uid;
        *(float*)&pSize = pObject->omega.v[0] * joint->hinge_axis.v[0]
            + pObject->omega.v[1] * joint->hinge_axis.v[1]
            + pObject->omega.v[2] * joint->hinge_axis.v[2];
        if (*(float*)&pSize > 39.2699f) {
            *(float*)&pSize = 39.2699f;
        }
        if (*(float*)&pSize < -39.2699f) {
            *(float*)&pSize = -39.2699f;
        }
        ((tU16*)pBuffer)[1] = (tU16)((*(float*)&pSize + 39.2699f) * 0.0127324f * 65534.f - 0.5f);
        pObject = (tPhysics_object*)((tU8*)pObject + 0x8c);
        BrMatrix34ToQuat(&quat, (br_matrix34*)pObject);
        {
            float q = quat.z;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[2] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        {
            float q = quat.y;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[3] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        ((tU16*)pBuffer)[4] = QuantizeFloatToInt(quat.z, -1.0f, 1.0f);
        ((tU16*)pBuffer)[5] = QuantizeFloatToInt(quat.w, -1.0f, 1.0f);
        return 0xc;
    case 2:
    case 3:
        if ((tU32)pSize < 0x10) {
            return -1;
        }
        pBuffer[0] = (tU8)joint->type;
        pBuffer[1] = pObject->uid;
        {
            int k;
            br_vector3* v = (br_vector3*)((tU8*)pObject + 0x74);
            tU16* out = (tU16*)&pBuffer[2];
            for (k = 0; k < 3; k++) {
                float c = v->v[0];
                if (c > 39.2699f) {
                    c = 39.2699f;
                }
                if (c < -39.2699f) {
                    c = -39.2699f;
                }
                *out++ = (tU16)((c + 39.2699f) * 0.0127324f * 65534.f - 0.5f);
                v = (br_vector3*)((tU8*)v + 4);
            }
        }
        pObject = (tPhysics_object*)((tU8*)pObject + 0x8c);
        BrMatrix34ToQuat(&quat, (br_matrix34*)pObject);
        {
            float q = quat.z;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[4] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        {
            float q = quat.y;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[5] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        ((tU16*)pBuffer)[6] = QuantizeFloatToInt(quat.z, -1.0f, 1.0f);
        ((tU16*)pBuffer)[7] = QuantizeFloatToInt(quat.w, -1.0f, 1.0f);
        return 0x10;
    default:
        PhysicsError((tPhysicsError)9, "netphys got messed up");
        /* fall through */
    case 0:
check_big:
        if ((tU32)pSize < 0x28) {
            return -1;
        }
        pBuffer[0] = 0;
        pBuffer[1] = pObject->uid;
        {
            int k;
            br_vector3* v = (br_vector3*)((tU8*)pObject + 0x74);
            tU16* out = (tU16*)&pBuffer[2];
            for (k = 0; k < 3; k++) {
                float c = v->v[0];
                if (c > 39.2699f) {
                    c = 39.2699f;
                }
                if (c < -39.2699f) {
                    c = -39.2699f;
                }
                *out++ = (tU16)((c + 39.2699f) * 0.0127324f * 65534.f - 0.5f);
                v = (br_vector3*)((tU8*)v + 4);
            }
        }
        {
            tU32 vx = *(tU32*)((tU8*)pObject + 0x68);
            pObject = (tPhysics_object*)((tU8*)pObject + 0x8c);
            ((tU32*)pBuffer)[2] = vx;
            ((tU32*)pBuffer)[3] = *(tU32*)((tU8*)pObject - 0x20);
            ((tU32*)pBuffer)[4] = *(tU32*)((tU8*)pObject - 0x1c);
        }
        BrMatrix34ToQuat(&quat, (br_matrix34*)pObject);
        {
            float q = quat.z;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[10] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        {
            float q = quat.y;
            if (q > 1.0f) {
                q = 1.0f;
            }
            if (q < -1.0f) {
                q = -1.0f;
            }
            ((tU16*)pBuffer)[11] = (tU16)((q + 1.0f) * 0.5f * 65534.f - 0.5f);
        }
        ((tU16*)pBuffer)[12] = QuantizeFloatToInt(quat.z, -1.0f, 1.0f);
        ((tU16*)pBuffer)[13] = QuantizeFloatToInt(quat.w, -1.0f, 1.0f);
        ((tU32*)pBuffer)[7] = *(tU32*)((tU8*)pObject + 0x24);
        ((tU32*)pBuffer)[8] = *(tU32*)((tU8*)pObject + 0x28);
        ((tU32*)pBuffer)[9] = *(tU32*)((tU8*)pObject + 0x2c);
        return 0x28;
    }
}

// C2PORT-NONMATCHING: best 31.19% | difftest PASS 2000 trials (spec difftest/specs/0049c630_SetObjectNetworkStuff.json) | reason VC5 x87 scheduling and register allocation
// FUNCTION: CARMA2_HW 0x0049c630
int C2_HOOK_FASTCALL SetObjectNetworkStuff(const tU8* pBuffer, tPhysics_object* pObject, int pMode) {
    br_scalar component;
    br_scalar relative_rate;
    br_scalar projection;
    br_quat quat;
    br_vector3 tv;
    register tPhysics_joint* joint;
    register tPhysics_object* parent;
    register br_matrix34* parent_matrix;
    register br_matrix34* matrix;
    int result;

    C2_HOOK_BUG_ON(offsetof(tPhysics_object, v) != 0x68);
    C2_HOOK_BUG_ON(offsetof(tPhysics_object, omega) != 0x74);
    C2_HOOK_BUG_ON(offsetof(tPhysics_object, transform_matrix) != 0x8C);
    C2_HOOK_BUG_ON(offsetof(tPhysics_object, physics_joint1) != 0x188);
    C2_HOOK_BUG_ON(offsetof(tPhysics_object, parent) != 0x228);
    C2_HOOK_BUG_ON(offsetof(tPhysics_joint, hinge_axis) != 0x38);

    if (pObject == NULL) {
        switch (pBuffer[0] & 0xF) {
        case 0:
            return 0x28;
        case 1:
        case 4:
            return 0xC;
        case 2:
        case 3:
            return 0x10;
        default:
            PhysicsError(9, "netphys got messed up");
            return 0;
        }
    }

    parent = pObject->parent;
    if (pMode == 1) {
        matrix = &pObject->actor->t.t.mat;
        parent_matrix = parent != NULL ? &parent->actor->t.t.mat : NULL;
    } else {
        matrix = &pObject->transform_matrix;
        parent_matrix = parent != NULL ? &parent->transform_matrix : NULL;
    }

    switch (pBuffer[0] & 0xF) {
    case 0:
        pObject->omega.v[0] = (br_scalar)((const tU16*)pBuffer)[1] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        pObject->omega.v[1] = (br_scalar)((const tU16*)pBuffer)[2] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        pObject->omega.v[2] = (br_scalar)((const tU16*)pBuffer)[3] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        memcpy(&pObject->v, pBuffer + 8, sizeof(pObject->v));
        quat.x = (br_scalar)((const tU16*)pBuffer)[10] * gFLOAT_0058a40c;
        quat.x = quat.x + quat.x - gFLOAT_0058a3f8;
        quat.y = (br_scalar)((const tU16*)pBuffer)[11] * gFLOAT_0058a40c;
        quat.y = quat.y + quat.y - gFLOAT_0058a3f8;
        quat.z = (br_scalar)((const tU16*)pBuffer)[12] * gFLOAT_0058a40c;
        quat.z = quat.z + quat.z - gFLOAT_0058a3f8;
        quat.w = (br_scalar)((const tU16*)pBuffer)[13] * gFLOAT_0058a40c;
        quat.w = quat.w + quat.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quat, &quat);
        BrQuatToMatrix34(matrix, &quat);
        memcpy(matrix->m[3], pBuffer + 0x1C, sizeof(matrix->m[3]));
        result = 0x28;
        break;
    case 1:
    case 4:
        quat.x = (br_scalar)((const tU16*)pBuffer)[2] * gFLOAT_0058a40c;
        quat.x = quat.x + quat.x - gFLOAT_0058a3f8;
        quat.y = (br_scalar)((const tU16*)pBuffer)[3] * gFLOAT_0058a40c;
        quat.y = quat.y + quat.y - gFLOAT_0058a3f8;
        quat.z = (br_scalar)((const tU16*)pBuffer)[4] * gFLOAT_0058a40c;
        quat.z = quat.z + quat.z - gFLOAT_0058a3f8;
        quat.w = (br_scalar)((const tU16*)pBuffer)[5] * gFLOAT_0058a40c;
        quat.w = quat.w + quat.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quat, &quat);
        BrQuatToMatrix34(matrix, &quat);
        BrMatrix34ApplyV(&tv, &parent->omega, parent_matrix);
        BrMatrix34TApplyV(&pObject->omega, &tv, matrix);
        joint = pObject->physics_joint1;
        component = (br_scalar)((const tU16*)pBuffer)[1] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        projection = pObject->omega.v[0] * joint->hinge_axis.v[0]
            + pObject->omega.v[1] * joint->hinge_axis.v[1]
            + pObject->omega.v[2] * joint->hinge_axis.v[2];
        relative_rate = component - projection;
        pObject->omega.v[0] += joint->hinge_axis.v[0] * relative_rate;
        pObject->omega.v[1] += joint->hinge_axis.v[1] * relative_rate;
        pObject->omega.v[2] += joint->hinge_axis.v[2] * relative_rate;
        FUN_0049cb00(pObject, matrix, (int)parent_matrix);
        result = 0xC;
        break;
    case 2:
    case 3:
        pObject->omega.v[0] = (br_scalar)((const tU16*)pBuffer)[1] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        pObject->omega.v[1] = (br_scalar)((const tU16*)pBuffer)[2] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        pObject->omega.v[2] = (br_scalar)((const tU16*)pBuffer)[3] * gFLOAT_0058a40c * gFLOAT_0058a420 - gFLOAT_0058a414;
        quat.x = (br_scalar)((const tU16*)pBuffer)[4] * gFLOAT_0058a40c;
        quat.x = quat.x + quat.x - gFLOAT_0058a3f8;
        quat.y = (br_scalar)((const tU16*)pBuffer)[5] * gFLOAT_0058a40c;
        quat.y = quat.y + quat.y - gFLOAT_0058a3f8;
        quat.z = (br_scalar)((const tU16*)pBuffer)[6] * gFLOAT_0058a40c;
        quat.z = quat.z + quat.z - gFLOAT_0058a3f8;
        quat.w = (br_scalar)((const tU16*)pBuffer)[7] * gFLOAT_0058a40c;
        quat.w = quat.w + quat.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quat, &quat);
        BrQuatToMatrix34(matrix, &quat);
        FUN_0049cb00(pObject, matrix, (int)parent_matrix);
        result = 0x10;
        break;
    default:
        PhysicsError(9, "netphys got messed up");
        result = pMode;
        break;
    }

    if (pMode == 2) {
        BrMatrix34Copy(&pObject->actor->t.t.mat, matrix);
    }
    return result;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 21.88% | difftest PASS 2000 trials (spec difftest/specs/0049cb00_FUN_0049cb00.json) | reason VC5 x87 scheduling and register allocation
// FUNCTION: CARMA2_HW 0x0049cb00
void C2_HOOK_FASTCALL FUN_0049cb00(tPhysics_object* pObject, br_matrix34* pMatrix, int pParent_matrix) {
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    float fVar6;
    float fVar9;
    float fVar10;
    float fVar11;
    float fVar12;
    float fVar13;
    float fVar14;
    register tPhysics_joint* joint;
    register tPhysics_object* parent;
    register br_matrix34* matrix;
    register br_matrix34* parent_matrix;
    br_vector3 temp1;
    br_vector3 temp2;

    matrix = pMatrix;
    joint = pObject->physics_joint1;
    fVar9 = joint->field_0x08.v[0] - pObject->cmpos.v[0];
    fVar10 = joint->field_0x08.v[1] - pObject->cmpos.v[1];
    fVar11 = joint->field_0x08.v[2] - pObject->cmpos.v[2];
    fVar1 = pObject->omega.v[2];
    fVar2 = pObject->omega.v[1];
    fVar3 = pObject->omega.v[2];
    fVar4 = pObject->omega.v[1];
    fVar5 = pObject->omega.v[0];
    fVar6 = pObject->omega.v[0];
    temp1.v[0] = fVar1 * fVar10 - fVar2 * fVar11;
    temp1.v[1] = fVar5 * fVar11 - fVar3 * fVar9;
    temp1.v[2] = fVar4 * fVar9 - fVar6 * fVar10;
    BrMatrix34ApplyV(&pObject->v, &temp1, matrix);

    if (pParent_matrix != 0) {
        parent = pObject->parent;
        parent_matrix = (br_matrix34*)pParent_matrix;

        fVar10 = joint->field_0x14.v[1] - parent->cmpos.v[1];
        fVar9 = joint->field_0x14.v[2] - parent->cmpos.v[2];
        fVar1 = parent->omega.v[2];
        fVar2 = parent->omega.v[1];
        fVar11 = joint->field_0x14.v[0] - parent->cmpos.v[0];
        fVar3 = parent->omega.v[0];
        fVar4 = parent->omega.v[2];
        fVar5 = parent->omega.v[1];
        fVar6 = parent->omega.v[0];
        temp1.v[0] = fVar2 * fVar9 - fVar1 * fVar10;
        temp1.v[1] = fVar4 * fVar11 - fVar3 * fVar9;
        temp1.v[2] = fVar6 * fVar10 - fVar5 * fVar11;
        BrMatrix34ApplyV(&temp2, &temp1, matrix);

        fVar12 = parent->v.v[0] + temp2.v[0];
        fVar13 = parent->v.v[1] + temp2.v[1];
        fVar14 = parent->v.v[2] + temp2.v[2];
        pObject->v.v[0] = fVar12 + pObject->v.v[0];
        pObject->v.v[1] = fVar13 + pObject->v.v[1];
        pObject->v.v[2] = fVar14 + pObject->v.v[2];

        BrMatrix34ApplyV(&temp1, &joint->field_0x08, matrix);
        BrMatrix34ApplyP(&temp2, &joint->field_0x14, parent_matrix);
        matrix->m[3][0] = temp2.v[0] - temp1.v[0];
        matrix->m[3][1] = temp2.v[1] - temp1.v[1];
        matrix->m[3][2] = temp2.v[2] - temp1.v[2];
    } else {
        BrMatrix34ApplyV(&temp1, &joint->field_0x08, matrix);
        matrix->m[3][0] = pObject->field_0x17c.v[0] - temp1.v[0];
        matrix->m[3][1] = pObject->field_0x17c.v[1] - temp1.v[1];
        matrix->m[3][2] = pObject->field_0x17c.v[2] - temp1.v[2];
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x0049cd00
int C2_HOOK_FASTCALL GetHierarchyNetworkStuff(tPhysics_object* pObject, tU8* pNet_data, int pRemaining) {
    int size;
    int total_size;
    tPhysics_object* child;
    tU8* child_data;

    total_size = GetObjectNetworkStuff(pObject, pNet_data, pRemaining);
    if (total_size < 0) {
        return -1;
    }
    child = pObject->child;
    pRemaining -= total_size;
    if (child != NULL) {
        *pNet_data |= 0x80; // Mark parent node
        for (; child != NULL; child = child->next) {
            child_data = &pNet_data[total_size];
            size = GetHierarchyNetworkStuff(child, child_data, pRemaining);
            if (size < 0) {
                return -1;
            }
            *child_data |= 0x40; // Mark child node
            total_size += size;
            pRemaining -= size;
        }
        *child_data &= ~0x40; //
    }
    return total_size;
}

int C2_HOOK_FASTCALL SizeOfObjectNetworkStuff(tPhysics_joint_type pType) {
    switch (pType & 0xf) {
    case eJoint_none:
        return 0x28; // FIXME!
    case eJoint_hinge:
    case eJoint_quick_hinge:
        return 0xc; // FIXME!
    case eJoint_universal:
    case eJoint_ball_n_socket:
        return 0x10; // FIXME!
    default:
        PhysicsError(9, "netphys got messed up");
        return 0;
    }
}

// FUNCTION: CARMA2_HW 0x0049ce60
int C2_HOOK_FASTCALL GetHierarchyNetworkSize(tPhysics_object* pObject) {
    int size;
    tPhysics_object* child;

    size = SizeOfObjectNetworkStuff(pObject->physics_joint1 != NULL ? pObject->physics_joint1->type : eJoint_none);
    for (child = pObject->child; child != NULL; child = child->next) {
        size += GetHierarchyNetworkSize(child);
    }
    return size;
}
// FUNCTION: CARMA2_HW 0x004c5e80
void C2_HOOK_FASTCALL FreeShapeList(tPhysics_shape *pShape) {

    if (pShape->common.next != NULL) {
        FreeShapeList(pShape->common.next);
    }
    BrMemFree(pShape);
}

// FUNCTION: CARMA2_HW 0x004c5ea0
void C2_HOOK_FASTCALL FreePhysicsJoint(tPhysics_joint* pJoint) {

    BrMemFree(pJoint);
}

// GetBoundsFromPointList

// FillInShape

// UpdateCollisionObject

// PhysicsError

// PhysicsWarning

// PhysicsSetErrorHandler

// RepositionAndOrientateChildren

// PhysicsAddObject

// PhysicsAddObjectList

// PhysicsRemoveObject

// PhysicsObjectRecurse

// PhysicsObjectRecurseChildren

// InitPhysicsWorkspace

// FindFacesInBox

// GetFrictionFromFace

// DoPhysicsError

// ResetMan2

// SetUpTestObjects

// ResetObjectList

// ResetMan

// FUNCTION: CARMA2_HW 0x004b5c00
void C2_HOOK_FASTCALL StopObjectList(tPhysics_object* pObjects) {
    tPhysics_object* pObject;

    for (pObject = pObjects; pObject != NULL; pObject = pObject->next) {
        BrMatrix34Translate(&pObject->transform_matrix, 0.0f, 0.0f, 0.0f);
        BrVector3SetFloat(&pObject->v, 0.0f, 0.0f, 0.0f);
        BrVector3SetFloat(&pObject->omega, 0.0f, 0.0f, 0.0f);
        BrVector3SetFloat(&pObject->rotate_omega, 0.0f, 0.0f, 0.0f);
        BrVector3SetFloat(&pObject->velocity_car_space, 0.0f, 0.0f, 0.0f);
        pObject->disable_move_rotate = 1;
        if (pObject->child != NULL) {
            StopObjectList(pObject->child);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004b5ca0
void C2_HOOK_FASTCALL InitPhysics(void) {
    PhysicsSetErrorHandler(DoPhysicsError);
    InitPhysicsWorkspace(gTemporary_physics_render_buffer, (int)sizeof(gTemporary_physics_render_buffer));
}

// PointOutOfSightNotAR

// CalcCollObjIDFromCarSpec

// CalcCollObjID

// GetCarSpecFromCollObjID

// GetCollObjFromID
