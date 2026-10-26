#include "funks.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"
#include "c2_string.h"
#include "flicplay.h"
#include "utility.h"
#include "world.h"
// GLOBAL: CARMA2_HW 0x0068b840
br_scalar gSight_distance_squared;

// CalculateTextureBitsValue

// MapSawToTriangle
// FUNCTION: CARMA2_HW 0x00474880

void C2_HOOK_STDCALL SetSightDistance(br_scalar pYon) {

    gSight_distance_squared = (br_scalar)CARPOCALYPSE2_SQR(pYon * 1.02);
}

// AddFunkGrooveBinding

// ControlBoundFunkGroove

// ControlBoundFunkGroovePlus

// ShiftBoundGrooveFunks

// AddNewFunkotronic

// C2PORT-NONMATCHING: best 55.61% | difftest PASS 2000 trials (spec difftest/specs/00474950_DisposeFunkotronics.json) | reason regalloc-loop
// FUNCTION: CARMA2_HW 0x00474950
void C2_HOOK_FASTCALL DisposeFunkotronics(int pOwner) {
    int i;
    int j;
    tFunk_proximity** proximity_array;
    tFunkotronic_spec* the_funk;

    if (gFunkotronics_array == NULL || gFunkotronics_array_size <= 0) {
        return;
    }
    i = 0;
    proximity_array = &gFunkotronics_array[0].proximity_array;
    do {
        PossibleService();
        the_funk = (tFunkotronic_spec*)((char*)proximity_array - offsetof(tFunkotronic_spec, proximity_array));
        if (the_funk->owner == pOwner) {
            the_funk->owner = -999;
            if (*proximity_array != NULL) {
                BrMemFree(*proximity_array);
            }
            if (the_funk->texture_animation_type == eTexture_animation_frames
                    && the_funk->texture_animation_data.frames_info.mode == eMove_texturebits) {
                BrMemFree(the_funk->texture_animation_data.frames_info.texture_info.data);
            }
            if (the_funk->texture_animation_type == eTexture_animation_flic) {
                BrMemFree(the_funk->texture_animation_data.flic_info.flic_data);
                EndFlic(&the_funk->texture_animation_data.flic_info.flic_descriptor);
                BrMemFree(the_funk->material->colour_map->pixels);
                the_funk->material->colour_map->pixels = NULL;
                BrPixelmapFree(the_funk->material->colour_map);
                the_funk->material->colour_map = NULL;
            } else if (the_funk->texture_animation_type == eTexture_animation_camera) {
                br_actor** actor;

                j = 0;
                actor = &the_funk->texture_animation_data.camera_info.actors[0];
                while (j < the_funk->texture_animation_data.camera_info.count) {
                    BrActorRemove(*actor);
                    BrActorFree(*actor);
                    j++;
                    actor++;
                }
            } else if (the_funk->texture_animation_type == eTexture_animation_mirror
                    && the_funk->texture_animation_data.mirror_info.actor != NULL) {
                BrActorRemove(the_funk->texture_animation_data.mirror_info.actor);
                BrActorFree(the_funk->texture_animation_data.mirror_info.actor);
            }
        }
        i++;
        proximity_array += sizeof(tFunkotronic_spec) / sizeof(*proximity_array);
    } while (i < gFunkotronics_array_size);
}

// CalcProximities

// AddProximities

// TryThisFunkLink

// FindSmashableMaterial


// FUNCTION: CARMA2_HW 0x00476430
void C2_HOOK_FASTCALL DisposeGroovidelics(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;

    if (gGroovidelics_array == NULL || gGroovidelics_array_size <= 0) {
        return;
    }
    the_groove = gGroovidelics_array;
    for (i = 0; i < gGroovidelics_array_size; i++, the_groove++) {
        PossibleService();
        if (the_groove->owner == pOwner) {
            the_groove->owner = -999;
        }
    }
}

// AddNewGroovidelic

// AddGroovidelics

// C2PORT-NONMATCHING: best 47.27% | difftest PASS 2000 trials (spec difftest/specs/00477160_KillGroovadelic.json) | reason VC5 loop pointer setup and branch layout
// FUNCTION: CARMA2_HW 0x00477160
void C2_HOOK_FASTCALL KillGroovadelic(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;

    if (gGroovidelics_array == NULL) {
        return;
    }
    the_groove = gGroovidelics_array;
    for (i = 0; i < gGroovidelics_array_size; i++, the_groove++) {
        if (the_groove->owner != pOwner) {
            continue;
        }
        if (the_groove->path_mode == eMove_controlled || the_groove->path_mode == eMove_absolute) {
            continue;
        }
        if (the_groove->object_mode == eMove_controlled || the_groove->object_mode == eMove_absolute) {
            continue;
        }
        the_groove->owner = -999;
    }
}

// FUNCTION: CARMA2_HW 0x004771c0
void C2_HOOK_FASTCALL KillFunkotronic(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;

    if (gFunkotronics_array == NULL) {
        return;
    }
    the_funk = gFunkotronics_array;
    for (i = 0; i < gFunkotronics_array_size; i++, the_funk++) {
        if (the_funk->owner == pOwner
            && the_funk->matrix_mode != eMove_controlled
            && the_funk->matrix_mode != eMove_absolute
            && the_funk->lighting_animation_type != eMove_controlled
            && the_funk->lighting_animation_type != eMove_absolute
            && (the_funk->texture_animation_data.frames_info.mode != eMove_controlled
                && the_funk->texture_animation_data.frames_info.mode != eMove_texturebits
                || the_funk->texture_animation_type != 0)) {
            the_funk->owner = -999;
        }
    }
}

// NormaliseDegreeAngle

// FunkThoseTronics

// PathGrooveBastard

// ObjectGrooveBastard

// GrooveThisDelic

// GrooveThoseDelics

// StopGroovidelic

// DisableGroovidelic

// EnableGroovidelic

// SetGrooveInterrupt

// ResetGrooveFlags

// DisableFunkotronic

// EnableFunkotronic

// MasterDisableFunkotronic

// MasterEnableFunkotronic

// FUNCTION: CARMA2_HW 0x0047b310
void C2_HOOK_FASTCALL AdjustFunkEnable(int pIndex, tS16 pEnable) {
    tU32* flags;

    if (pEnable != 0) {
        flags = &gFunkotronics_array[pIndex].flags;
        *flags &= ~0x1;
        PipeSingleFunkEnable(pIndex, 1);
        return;
    }
    flags = &gFunkotronics_array[pIndex].flags;
    *flags |= 0x1;
    PipeSingleFunkEnable(pIndex, 0);
}

// AdjustFunkEnable

// ActorsGroove