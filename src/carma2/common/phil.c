#include "phil.h"

#include "carpocalypse2_macros.h"
#include "physics.h"

extern const br_scalar gFLOAT_0058a3f8;
extern const br_scalar gFLOAT_0058a40c;
extern br_quat* C2_HOOK_CDECL BrQuatNormalise(br_quat* q, const br_quat* qq);
extern br_matrix34* C2_HOOK_CDECL BrQuatToMatrix34(br_matrix34* mat, const br_quat* q);
// GLOBAL: CARMA2_HW 0x0065d004
int gPHIL_enabled = 1;

// ScalarToFraction

// FractionToScalar

// GetOrientationFromMatrix

// GetMatrixFromOrientation

// OmegaCompScalarTo16

// OmegaComp16ToScalar

// GetOmega16FromOmega

// GetOmegaFromOmega16

// GetObjectNetworkStuff

// GetVelocityAndPosition

// SizeOfObjectNetworkStuff

// WriteObjectNetworkStuff

// GetHierarchyNetworkStuff

// FindObject

// WriteHierarchyNetworkStuff

// GetHierarchyNetworkSize

// FUNCTION: CARMA2_HW 0x0049cee0
int C2_HOOK_FASTCALL GetNetworkDataSize(tU8* pNet_data) {
    int size;
    tU8* next;
    tU8 more;

    switch (*pNet_data & 0xf) {
    case 0:
        size = 0x28;
        break;
    case 1:
    case 4:
        size = 0xc;
        break;
    case 2:
    case 3:
        size = 0x10;
        break;
    default:
        PhysicsError(9, "netphys got messed up");
        size = 0;
    }
    more = *pNet_data & 0x80;
    while (more != 0) {
        next = pNet_data + size;
        size += GetNetworkDataSize(next);
        more = *next & 0x40;
    }
    return size;
}

// GetSingleMatrixFromNetworkData

// C2PORT-NONMATCHING: best 49.42% | difftest PASS 2000 trials (spec difftest/specs/0049cf50_GetSingleMatrixFromNetworkData.json) | reason x87-order
// FUNCTION: CARMA2_HW 0x0049cf50
int C2_HOOK_FASTCALL GetSingleMatrixFromNetworkData(tU8* pNet_data, br_matrix34* pMatrix) {
    br_quat quaternion;

    switch (pNet_data[0] & 0xF) {
    case 0:
        quaternion.x = (br_scalar)((const tU16*)(pNet_data + 0x14))[0] * gFLOAT_0058a40c;
        quaternion.x = quaternion.x + quaternion.x - gFLOAT_0058a3f8;
        quaternion.y = (br_scalar)((const tU16*)(pNet_data + 0x14))[1] * gFLOAT_0058a40c;
        quaternion.y = quaternion.y + quaternion.y - gFLOAT_0058a3f8;
        quaternion.z = (br_scalar)((const tU16*)(pNet_data + 0x14))[2] * gFLOAT_0058a40c;
        quaternion.z = quaternion.z + quaternion.z - gFLOAT_0058a3f8;
        quaternion.w = (br_scalar)((const tU16*)(pNet_data + 0x14))[3] * gFLOAT_0058a40c;
        quaternion.w = quaternion.w + quaternion.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quaternion, &quaternion);
        BrQuatToMatrix34(pMatrix, &quaternion);
        memcpy(pMatrix->m[3], pNet_data + 0x1C, sizeof(pMatrix->m[3]));
        return 0x28;
    case 1:
    case 4:
        quaternion.x = (br_scalar)((const tU16*)(pNet_data + 4))[0] * gFLOAT_0058a40c;
        quaternion.x = quaternion.x + quaternion.x - gFLOAT_0058a3f8;
        quaternion.y = (br_scalar)((const tU16*)(pNet_data + 4))[1] * gFLOAT_0058a40c;
        quaternion.y = quaternion.y + quaternion.y - gFLOAT_0058a3f8;
        quaternion.z = (br_scalar)((const tU16*)(pNet_data + 4))[2] * gFLOAT_0058a40c;
        quaternion.z = quaternion.z + quaternion.z - gFLOAT_0058a3f8;
        quaternion.w = (br_scalar)((const tU16*)(pNet_data + 4))[3] * gFLOAT_0058a40c;
        quaternion.w = quaternion.w + quaternion.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quaternion, &quaternion);
        BrQuatToMatrix34(pMatrix, &quaternion);
        return 0xC;
    case 2:
    case 3:
        quaternion.x = (br_scalar)((const tU16*)(pNet_data + 8))[0] * gFLOAT_0058a40c;
        quaternion.x = quaternion.x + quaternion.x - gFLOAT_0058a3f8;
        quaternion.y = (br_scalar)((const tU16*)(pNet_data + 8))[1] * gFLOAT_0058a40c;
        quaternion.y = quaternion.y + quaternion.y - gFLOAT_0058a3f8;
        quaternion.z = (br_scalar)((const tU16*)(pNet_data + 8))[2] * gFLOAT_0058a40c;
        quaternion.z = quaternion.z + quaternion.z - gFLOAT_0058a3f8;
        quaternion.w = (br_scalar)((const tU16*)(pNet_data + 8))[3] * gFLOAT_0058a40c;
        quaternion.w = quaternion.w + quaternion.w - gFLOAT_0058a3f8;
        BrQuatNormalise(&quaternion, &quaternion);
        BrQuatToMatrix34(pMatrix, &quaternion);
        return 0x10;
    default:
        PhysicsError(9, "netphys got messed up");
        return -1;
    }
}

// GetNetworkDataSize

// PHILInit

// FUNCTION: CARMA2_HW 0x004b5d20
void C2_HOOK_FASTCALL PHILDisable(void) {
    gPHIL_enabled = 1;
}

// FUNCTION: CARMA2_HW 0x004b5d30
int C2_HOOK_FASTCALL FUN_004b5d30(void) {

    return 100 - gPHIL_count_list_collision_infos;
}

// PHILAddObject

// FUNCTION: CARMA2_HW 0x004b5ea0
int C2_HOOK_FASTCALL PHILRemoveObject(tPhysics_object* pObject) {
    tCollision_info_owner* owner;
    int i;

    if (gPHIL_enabled) {
        return 0;
    }

    if (pObject->field_0x239 == 2) {
        ((tQueued_object_info*)pObject->field_0x240)->object = NULL;
        pObject->field_0x240 = NULL;
        pObject->field_0x239 = 0;
        return 0;
    }

    if (gPHIL_munging_objects && !gPHIL_object_added) {
        gPHIL_queued_objects_for_removal[gPHIL_count_queued_objects_for_removal] = pObject;
        gPHIL_count_queued_objects_for_removal += 1;
        return 0;
    }

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers); i++) {
        if (gPhil_queued_objects.headers[i].collision_info == pObject) {
            break;
        }
    }
    if (i == (int)CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers)
        || (owner = pObject->field_0x240) == NULL) {
        return 3;
    }
    owner->field_0x00 = NULL;
    pObject->field_0x240 = NULL;
    gPHIL_count_list_collision_infos -= 1;
    if (pObject == gList_collision_infos) {
        gList_collision_infos = pObject->next;
        if (pObject->next != NULL) {
            pObject->next->prev = NULL;
        }
    }

    for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers); i++) {
        if (gPhil_queued_objects.headers[i].collision_info != NULL
            && gPhil_queued_objects.headers[i].collision_info->next == pObject) {
            break;
        }
    }
    if (i < (int)CARPOCALYPSE2_ASIZE(gPhil_queued_objects.headers)) {
        gPhil_queued_objects.headers[i].collision_info->next = pObject->next;
        if (pObject->next != NULL) {
            pObject->next->prev = gPhil_queued_objects.headers[i].collision_info;
        }
    }
    return 0;
}

// PHILGetFirstObject

// PHILGetNextObject

// FUNCTION: CARMA2_HW 0x004b6010
tU32 C2_HOOK_FASTCALL PHILReturnObjectStatus(tPhysics_object* pObject) {
    tCollision_info_owner* owner;

    if (!gPHIL_enabled) {
        if (pObject->field_0x239 == 2) {
            return ((tCollision_info_owner*)pObject->field_0x240)->field_0x08;
        }
        owner = pObject->field_0x240;
        if (owner != NULL) {
            return owner->field_0x08;
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004b6040
int C2_HOOK_FASTCALL PHILMakeObjectPassive(tPhysics_object* pObject) {
    tQueued_object_info* object_info;

    if (gPHIL_enabled) {
        return 0;
    }
    if (pObject->field_0x239 == 2) {
        object_info = pObject->field_0x240;
        object_info->field_0x8 = 1;
        return 0;
    }
    object_info = pObject->field_0x240;
    if (object_info != NULL) {
        object_info->field_0x8 = 1;
        pObject->flags |= 0x20;
        return 0;
    }
    return 3;
}


// PHILMakeObjectActive

// PHILSetPassiveObjectsMatrix

// PHILAddActiveObject

// PHILAddObjectImmediately

// PHILAddActiveObjectImmediately

// PHILSetObjectProperty

// PHILGetObjectProperty

// FlushQueuedAddsAndRemoves

// ChangedObjectsCallbacks

// ProcessDrag2

// ProcessDrag

// MarkObjectAndChildrenAsPassive

// SetStandardGravity

// ProcessGravity

// LevelOutOnSurface

// PHILMungeObjects

// PHILActivatePassive

// PHILInterpolateObjects

// PHILDoPhysics

// PHILApplyPHILObject

// PHILGetPHILObjectState

// PhysicsObjectSetImpulse

// PhysicsObjectMoveVelocity

// FUNCTION: CARMA2_HW 0x004c29d0
void C2_HOOK_FASTCALL PhysicsObjectSetImpulse(tPhysics_object* pObject, br_vector3* pImpulse) {
    tPhysics_object* pChild;

    pObject->field_0x54 = *pImpulse;
    for (pChild = pObject->child; pChild != NULL; pChild = pChild->next) {
        PhysicsObjectSetImpulse(pChild, pImpulse);
    }
}

// FUNCTION: CARMA2_HW 0x004c2910
void C2_HOOK_FASTCALL PhysicsObjectMoveVelocity(tPhysics_object* pObject) {
    tPhysics_object* pChild;

    if (!pObject->disable_move_rotate) {
        pObject->v.v[0] += pObject->field_0x54.v[0];
        pObject->v.v[1] += pObject->field_0x54.v[1];
        pObject->v.v[2] += pObject->field_0x54.v[2];
    }
    if (pObject->child != NULL) {
        for (pChild = pObject->child; pChild != NULL; pChild = pChild->next) {
            PhysicsObjectMoveVelocity(pChild);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004c2970
void C2_HOOK_FASTCALL PhysicsObjectMoveVelocityList(tPhysics_object* pObject) {

    while (pObject != NULL) {
        if (!pObject->disable_move_rotate) {
            pObject->v.v[0] += pObject->field_0x54.v[0];
            pObject->v.v[1] += pObject->field_0x54.v[1];
            pObject->v.v[2] += pObject->field_0x54.v[2];
        }
        if (pObject->child != NULL) {
            PhysicsObjectMoveVelocityList(pObject->child);
        }
        pObject = pObject->next;
    }
}
