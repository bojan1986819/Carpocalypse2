#include "powerup.h"

#include "displays.h"
#include "globvars.h"
#include "graphics1.h"
#include "pedestrn.h"
#include "loading.h"
#include "network.h"
#include "piping.h"
#include "pratcam.h"
#include "spark.h"
#include "globvrpb.h"
#include "powerups.h"
#include "utility.h"
#include "carpocalypse2_macros.h"

#include "c2_string.h"
// GLOBAL: CARMA2_HW 0x006a0ad4
const char* gPowerup_txt_path;

// GLOBAL: CARMA2_HW 0x007059e0
tHeadup_icon gPickedup_powerups[20];

// GLOBAL: CARMA2_HW 0x00763480
tSlot_info gCost_APO;

// GLOBAL: CARMA2_HW 0x0075b900
tSlot_info gTrade_in_value_APO;

// GLOBAL: CARMA2_HW 0x0074d380
tSlot_info gSubstitution_value_APO;

// GLOBAL: CARMA2_HW 0x00761c60
tSlot_info gPotential_substitution_value_APO;

// GLOBAL: CARMA2_HW 0x0074d1a4
int gNet_powerup_time_replacement;

// GLOBAL: CARMA2_HW 0x007447d8
int gPickup_respawn_min_time_ms;

// GLOBAL: CARMA2_HW 0x007447e8
int gPickup_respawn_max_extra_time_ms;

// GLOBAL: CARMA2_HW 0x007059c0
int gCount_mutant_tail_parts;

// GLOBAL: CARMA2_HW 0x00705b70
tU32 gINT_00705b70;

// GLOBAL: CARMA2_HW 0x00705b78
float gMass_mutant_tail_link;

// GLOBAL: CARMA2_HW 0x00705b74
float gMass_mutant_tail_ball;

// GLOBAL: CARMA2_HW 0x0065ebbc
float gMass_mine = 1.0f;

// GLOBAL: CARMA2_HW 0x006a0454
int gCount_cloaked_cars;

// GLOBAL: CARMA2_HW 0x006a0910
tCar_spec* gCloaked_cars[12];

// GLOBAL: CARMA2_HW 0x006a0a54
tPowerup* gPowerup_array;

// GetNextGoodyTime

// DoPowerupTextHeadup

// FUNCTION: CARMA2_HW 0x004d9670
void C2_HOOK_FASTCALL QueueIconGoing(tPowerup* pPowerup, tHeadup_icon* pHeadup_icons, int* pCount_icons) {
    int i;

    for (i = 0; i < *pCount_icons; i++) {
        if (pHeadup_icons[i].powerup == pPowerup) {
            pHeadup_icons[i].fizzle_stage = 3;
            pHeadup_icons[i].fizzle_direction = -1;
            pHeadup_icons[i].fizzle_start = GetTotalTime();
            break;
        }
    }
}

// QueueIconComing

// LosePowerupX

// LosePowerup

// LoseKeyboardPowerupN

// KeyboardPowerupFinished

// LoseAllSimilarPowerups
extern int gNumber_of_powerups;

void C2_HOOK_FASTCALL LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;

    for (i = 0, the_powerup = gPowerup_array; i < gNumber_of_powerups; i++, the_powerup++) {
        if (the_powerup != pThe_powerup && the_powerup->got_time != 0
            && (pThe_powerup->group_inclusion & the_powerup->group_inclusion) != 0) {
            LosePowerup(the_powerup);
        }
    }
}

// ExcludedPowerup

// GLOBAL: CARMA2_HW 0x0065e8f8
br_token_value gPowerup_icon_material_prims[3] = {
    { BRT_BLEND_B, { 1 } },
    { BRT_OPACITY_X, { 6553600 } },
    { BR_NULL_TOKEN, { 0 } },
};

// FUNCTION: CARMA2_HW 0x004d9150
int C2_HOOK_FASTCALL GotPowerupX(tCar_spec* pCar, int pIndex, int pArg3, int pMessage, tU32 pTime) {
    tPowerup* the_powerup;
    tPowerup* old_powerup;
    tRace_list_spec* race;
    int i;
    int ps_power;
    int original_index;
    tNet_message* msg;
    br_actor* actor;
    tHeadup_icon* icon;
    char s[256];
    char* s2;

    if (pIndex >= gNumber_of_powerups || pIndex < 0) {
        return -1;
    }
    the_powerup = &gPowerup_array[pIndex];
    if (the_powerup->type == ePowerup_dummy) {
        return -1;
    }
    race = gCurrent_race.race_spec;
    if (GetPedCount() < 1 && (the_powerup->net_type & 0x40)) {
        goto replacement;
    }
    if ((gNet_mode != eNet_mode_none && (the_powerup->net_type & 2))
        || (gNet_mode == eNet_mode_none && pIndex == 0x61)) {
        goto replacement;
    }
    if (!IsItOkayToFireHorribleBallsOfNastyNapalmDeathAtPerfectlyInnocentPassersByAndByInnocentIDoMeanInTheBiblicalSense() && pIndex == 0x5b) {
        goto replacement;
    }
    if (!IsItReallyOKThatWeDontMakeAnyEffortToProtectAnySadFuckersOutThereThatDontWishToSeeInnocentPeopleBlownToBitsByHighExplosiveMinesAndShells()
        && (pIndex == 0x34 || pIndex == 0x60)) {
        goto replacement;
    }
    if (GetHowMuchBloodAndSnotToSmearAbout() > 0 && (pIndex == 4 || pIndex == 0x39)) {
        goto replacement;
    }
    for (i = 0; i < race->count_powerup_exclusions; i++) {
        if (pIndex == race->powerup_exclusions[i]) {
            goto replacement;
        }
    }
    goto selected;
replacement:
    pIndex = 0;
    the_powerup = gPowerup_array;
selected:
    the_powerup->current_value = -1;
    for (i = 0, old_powerup = gPowerup_array; i < gNumber_of_powerups; i++, old_powerup++) {
        if (old_powerup != the_powerup && old_powerup->got_time != 0
            && ((tU16)old_powerup->group_inclusion & (tU16)the_powerup->group_inclusion)) {
            if (old_powerup->lose_proc != NULL) {
                old_powerup->lose_proc(old_powerup, old_powerup->car);
            }
            if (old_powerup->net_type & 0x80) {
                ARPowerupLose(old_powerup, old_powerup->car);
            }
            if (gNet_mode != eNet_mode_none) {
                msg = NetBuildGuaranteedMessage(0x16, 0);
                msg->contents.powerup.field_0x10 = 2;
                msg->contents.powerup.player_id = gLocal_net_ID;
                msg->contents.powerup.powerup_index = old_powerup - gPowerup_array;
                NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
            }
            gProgram_state.current_car.powerups[old_powerup - gPowerup_array] = 0;
            if (old_powerup->initial_value == 0) {
                QueueIconGoing(old_powerup, gPickedup_powerups, &gNumber_of_icons);
            }
            old_powerup->got_time = 0;
        }
    }
    ps_power = gNet_mode != eNet_mode_none && the_powerup->got_proc == GotTimeOrPower;
    original_index = pIndex;
    the_powerup->car = pCar;
    if (the_powerup->got_proc != NULL) {
        pIndex = the_powerup->got_proc(the_powerup, pCar);
    }
    if (pIndex < 0) {
        return -1;
    }
    the_powerup = &gPowerup_array[pIndex];
    if (the_powerup->message[0] != '\0' && pMessage && the_powerup->initial_value == 0 && !ps_power) {
        s2 = s;
        if (pMessage == 2) {
            sprintf(s, "%s %s", GetMiscString(190), the_powerup->message);
        } else {
            strcpy(s, the_powerup->message);
        }
        if (the_powerup->got_proc == FreezeTimer) {
            s2 = strtok(s, "/");
            if (gFreeze_timer) {
                s2 = strtok(NULL, "/");
            }
        }
        NewTextHeadupSlot(4, 0, 3000, -4, s2);
    }
    if ((the_powerup->net_type & 0x80) && gProgram_state.current_car.powerups[pIndex] == 0) {
        ARPowerupGot(the_powerup, pCar);
    }
    switch (the_powerup->type) {
    case ePowerup_instantaneous:
        break;
    case ePowerup_timed:
        the_powerup->got_time = GetTotalTime();
        if (!pArg3 && gNet_mode != eNet_mode_none) {
            the_powerup->lose_time = the_powerup->got_time + pTime;
        } else {
            the_powerup->lose_time = the_powerup->got_time + the_powerup->duration;
        }
        gProgram_state.current_car.powerups[pIndex] = the_powerup->lose_time;
        break;
    case ePowerup_whole_race:
        the_powerup->got_time = GetTotalTime();
        gProgram_state.current_car.powerups[pIndex] = -1;
        break;
    }
    if (the_powerup->prat_cam_event >= 0) {
        PratcamEvent(the_powerup->prat_cam_event);
    }
    if (gNet_mode != eNet_mode_none && pArg3 && original_index == pIndex
        && !ps_power && the_powerup->got_proc != RepulseOpponents) {
        msg = NetBuildGuaranteedMessage(0x16, 0);
        msg->contents.powerup.field_0x10 = 0;
        msg->contents.powerup.player_id = gLocal_net_ID;
        msg->contents.powerup.powerup_index = pIndex;
        msg->contents.powerup.time_remaining = the_powerup->type == ePowerup_timed ? the_powerup->duration : 0;
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
    }
    if (the_powerup->type != ePowerup_instantaneous && the_powerup->icon != NULL && the_powerup->initial_value == 0) {
        for (i = 0; i < gNumber_of_icons; i++) {
            if (gPickedup_powerups[i].powerup == the_powerup) {
                gPickedup_powerups[i].fizzle_stage = 4;
                return pIndex;
            }
        }
        if (gNumber_of_icons != CARPOCALYPSE2_ASIZE(gPickedup_powerups)) {
            icon = &gPickedup_powerups[gNumber_of_icons];
            icon->powerup = the_powerup;
            icon->fizzle_stage = 0;
            icon->fizzle_direction = 1;
            icon->fizzle_start = GetTotalTime();
            actor = the_powerup->icon_actor;
            if (actor != NULL) {
                icon->icon_actor = actor;
                memcpy(the_powerup->material_prims, gPowerup_icon_material_prims, sizeof(the_powerup->material_prims));
                actor->material->extra_prim[1].v.x = 0xff0000;
                BrMaterialUpdate(actor->material, BR_MATU_EXTRA_PRIM);
                actor->render_style = BR_RSTYLE_NONE;
                RenderThisHeadup(actor);
            }
            gNumber_of_icons++;
        }
    }
    return pIndex;
}

// GotPowerupXX

// GotPowerupEarwig

// FUNCTION: CARMA2_HW 0x004d8d40
void C2_HOOK_FASTCALL GotPowerupEarwig(tCar_spec* pCar, int pIndex, int pMessage, int pSelect_inventory) {
    tPowerup* the_powerup;
    tPowerup* old_powerup;
    tRace_list_spec* race;
    int inventory_index;
    int i;
    tNet_message* msg;
    char s[256];
    char* text;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPowerup, value, 0x30);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPowerup, initial_value, 0x34);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tRace_list_spec, count_powerup_exclusions, 0x1d4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_powerup, player_id, 0x4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_powerup, powerup_index, 0x8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_powerup, field_0x10, 0x10);

    inventory_index = -1;
    if (pIndex >= gNumber_of_powerups) {
        return;
    }
    the_powerup = &gPowerup_array[pIndex];
    if (the_powerup->initial_value != 0) {
        race = gCurrent_race.race_spec;
        if (GetPedCount() < 1 && (the_powerup->net_type & 0x40)) {
            goto replacement;
        }
        if ((gNet_mode != eNet_mode_none && (the_powerup->net_type & 2))
            || (gNet_mode == eNet_mode_none && pIndex == 0x61)) {
            goto replacement;
        }
        if (!IsItOkayToFireHorribleBallsOfNastyNapalmDeathAtPerfectlyInnocentPassersByAndByInnocentIDoMeanInTheBiblicalSense() && pIndex == 0x5b) {
            goto replacement;
        }
        if (!IsItReallyOKThatWeDontMakeAnyEffortToProtectAnySadFuckersOutThereThatDontWishToSeeInnocentPeopleBlownToBitsByHighExplosiveMinesAndShells()
            && (pIndex == 0x34 || pIndex == 0x60)) {
            goto replacement;
        }
        if (GetHowMuchBloodAndSnotToSmearAbout() > 0 && (pIndex == 4 || pIndex == 0x39)) {
            goto replacement;
        }
        for (i = 0; i < race->count_powerup_exclusions; i++) {
            if (pIndex == race->powerup_exclusions[i]) {
                goto replacement;
            }
        }
        if (pMessage) {
            text = s;
            if (pMessage == 2) {
                sprintf(s, "%s %s", GetMiscString(190), the_powerup->message);
            } else {
                strcpy(s, the_powerup->message);
            }
            if (the_powerup->got_proc == FreezeTimer) {
                text = strtok(s, "/");
                if (gFreeze_timer) {
                    text = strtok(NULL, "/");
                }
            }
            NewTextHeadupSlot(4, 0, 3000, -4, text);
        }
        for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gInventory); i++) {
            if (gInventory[i] == pIndex) {
                inventory_index = i;
                break;
            }
        }
        if (inventory_index < 0) {
            for (i = 0; i < (int)CARPOCALYPSE2_ASIZE(gInventory); i++) {
                if (gInventory[i] < 0) {
                    inventory_index = i;
                    the_powerup->value = 0;
                    break;
                }
            }
            if (inventory_index < 0) {
                return;
            }
        }
        if (inventory_index != gInventory_selected && gInventory_selected >= 0) {
            old_powerup = &gPowerup_array[gInventory[gInventory_selected]];
            if (old_powerup->initial_value < 0 || gINT_006a0a6c != 0) {
                if (old_powerup->got_time != 0 && old_powerup->lose_proc != NULL) {
                    old_powerup->lose_proc(old_powerup, old_powerup->car);
                }
                if (old_powerup->net_type & 0x80) {
                    ARPowerupLose(old_powerup, old_powerup->car);
                }
                if (gNet_mode != eNet_mode_none) {
                    msg = NetBuildGuaranteedMessage(0x16, 0);
                    msg->contents.powerup.field_0x10 = 2;
                    msg->contents.powerup.player_id = gLocal_net_ID;
                    msg->contents.powerup.powerup_index = old_powerup - gPowerup_array;
                    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, msg, NULL);
                }
                gPowerup_net_state[old_powerup - gPowerup_array] = 0;
                if (old_powerup->initial_value == 0) {
                    for (i = 0; i < gNumber_of_icons; i++) {
                        if (old_powerup == gPickedup_powerups[i].powerup) {
                            gPickedup_powerups[i].fizzle_stage = 3;
                            gPickedup_powerups[i].fizzle_direction = -1;
                            gPickedup_powerups[i].fizzle_start = GetTotalTime();
                            break;
                        }
                    }
                }
                old_powerup->got_time = 0;
            }
            gINT_006a0a5c = 1;
        }
        if (pSelect_inventory || gInventory_selected < 0) {
            gInventory_selected = inventory_index;
        }
        the_powerup->value += the_powerup->initial_value;
        gInventory[inventory_index] = pIndex;
        return;
    }
    GotPowerupX(pCar, pIndex, gNet_mode != eNet_mode_none && pCar != NULL && pCar->driver == eDriver_local_human, pMessage, 0);
    return;

replacement:
    GotPowerupX(pCar, 0, gNet_mode != eNet_mode_none && pCar != NULL && pCar->driver == eDriver_local_human, pMessage, 0);
}

// AutoGoody

// GotPowerup

// DRMapFindMeAPixie


// LoadPowerups (implemented in powerups.c)

// InitRepulseEffects

// MungeBoxObject

// MungeShereObject

// InitTail

// ThisObjectHitAMineSoSpamItUpUsingThisPowerupsSettings

// InitMineShit

// InitPowerups (implemented in powerups.c)

// RemoveRepulseEffect

// ResetRepulseEffects

// EmptyInventory

// ResetPowerups

// CloseDownPowerUps (implemented in powerups.c)

// NewRepulseRay

// RemoveRepulsiveReferences

// MungeRepulseRays

// AdjustRepulseRay

// ResetRepulseRay

// DrawSinglePowerupIcon

// DrawPowerupIcons2

// DrawKeyPowerups

// ToggleInventory

// RightInventory

// LeftInventory

// DrawPowerups

// MungeKeyboardPowerup

// DoPowerupPeriodics

// CheckRespawnQueue

// MungePowerupStuff

// GotPowerupN

// GotPowerup0

// GotPowerup1

// GotPowerup2

// GotPowerup3

// GotPowerup4

// GotPowerup5

// GotPowerup6

// GotPowerup7

// GotPowerup8

// GotPowerup9

// GotCredits

// GainAPO2

// GainAPOPot2

// GainAPO

// GotTimeOrPower

// GainAPOPotential

// SetPedSpeed

// SetPedSize

// SetPedHeadSize

// SetPedExplode

// SetPedBrittle

// SetInvulnerability

// ResetInvulnerability

// SetFreeRepairs

// ResetFreeRepairs

// SetStupidPedestrians

// ResetStupidPedestrians

// FreezeTimer

// UnfreezeTimer

// DoInstantRepair

// ResetPedSpeed

// ResetPedSize

// ResetPedHeadSize

// ResetPedExplode

// ResetPedBrittle

// SetEngineFactor

// SetUnderwater

// TrashBodywork

// TakeDrugs

// PaletteFuckedUpByDrugs

// TheEffectsOfDrugs

// SetOpponentsSpeed

// SetCopsSpeed

// SetGravity

// SetPinball

// SetWallclimb

// SetBouncey

// SetSuspension

// SetTyreGrip

// SetImmortalPeds

// ResetImmortalPeds

// SetSuicidalPeds

// ResetSuicidalPeds

// SetDamageMultiplier

// FillInMine

// SendMines

// MinePreCollisionStuff
extern tCar_spec* gActive_car_list[139];
extern int gNum_active_cars;

/* port: an invented MinePreCollisionStuff lived here; the real one is retail 0x4dd1b0 in powerups.c */

// ActivateMine

// PositionMine

// MineSound

// ReceivedMine

// ShitMine

// MineExplode

// TiggerMineToExplode

// HitAShatOutMine

// ClearMineReferencesToCar

// MungeShatMines

// ARDoShitMine

// ARUndoShitMine

// PissOutOil

// KangerooJump

// AnnihilatePeds

// NapalmPeds

// RepulseOpponents2

// RepulseOpponents

// RepulsePedestrians

// ReceivedRepulse

// ResetEngineFactor

// ResetUnderwater

// PukeDrugsBackUp

// ResetOpponentsSpeed

// ResetCopsSpeed

// ResetGravity

// ResetPinball

// ResetWallclimb

// ResetBouncey

// ResetSuspension

// ResetDamageMultiplier

// ResetTyreGrip

// PickAtRandom

// GotVouchers

// MungeVouchers

// SetInstantHandbrake

// ResetInstantHandbrake

// SetDancingPeds

// SetPanickingPeds

// SetLowGravityPeds

// SetPissedPeds

// SetGhostPeds

// ResetDancingPeds

// ResetPanickingPeds

// ResetLowGravityPeds

// ResetPissedPeds

// ResetGhostPeds

// DoBouncey

// HitMine

// SetMassMultiplier

// ResetMassMultiplier

// ShowPedestrians

// HidePedestrians

// SetElectroBastard

// ResetElectroBastard

// SetMutantCorpses

// ResetMutantCorpses

// ReceivedPowerup

// SendCurrentPowerups

// LoseAllLocalPowerups

// LoseDodgyPowerups

// GetPowerupCount

// GetPowerupPosition

// ARPowerupGot

// ARPowerupLose

// SetPissed

// ResetPissed

// PissedEffect

// RenderSpinningPowerup

// RenderChangingPowerup

// SetSpinningPowerup

// DuplicateModel

// SetChangingPowerup

// PedValium

// PowerupCancel

// GrowATail

// CutOffTail

// RemoveTail (implemented in powerups.c)

// DoInitialTail

// UndoInitialTail

// MayQueuePowerupRespawn

// ARDoPowerupRespawn

// ARUndoPowerupRespawn

// C2PORT-NONMATCHING: best 54.5% | difftest PASS 2000 trials (spec 004e07f0_PrintPowerupIconIn3D.json) | reason original animated render logic is behavior-tested; remaining diff is compiler codegen
// FUNCTION: CARMA2_HW 0x004e07f0
void C2_HOOK_FASTCALL PrintPowerupIconIn3D(int pX, int pY, tHeadup_icon* pIcon, tPowerup* pPowerup, int pScale, tU32 pTime) {
    float f;
    float scale;
    br_fixed_ls prev_opacity;

    if (pIcon == NULL) {
#ifdef CARPOCALYPSE2_FIX_BUGS
        /* a power-up whose icon pixelmap was not found has no billboard (inventory crash) */
        if (pPowerup->icon_actor == NULL) {
            return;
        }
#endif
        pPowerup->icon_actor->render_style = BR_RSTYLE_FACES;
        BrVector3Set(&pPowerup->icon_actor->t.t.translate.t, (float)pX + 16.0f, (float)-(pY + 16), 0.0f);
        RenderThisHeadup(pPowerup->icon_actor);
        return;
    }
    if (pIcon->icon_actor != NULL) {
        pIcon->icon_actor->render_style = BR_RSTYLE_FACES;
        if (pScale != 0) {
            f = (float)(pTime - pIcon->fizzle_start) / 600.0f;
            if (pIcon->fizzle_direction > 0) {
                f = 1.0f - f;
                scale = 1.0f + 5.0f * f;
                BrMatrix34Scale(&pIcon->icon_actor->t.t.mat, scale, scale, 1.0f);
            } else {
                BrMatrix34Scale(&pIcon->icon_actor->t.t.mat, 1.01f - f, 1.01f - f, 1.0f);
            }
            BrMatrix34PostRotateZ(&pIcon->icon_actor->t.t.mat, (br_angle)((1.0f - f / 2.0f) * 65536.0f));
            pIcon->icon_actor->material->extra_prim[1].v.x = BR_FIXED_INT((1.0f - f) * 255.0f);
            BrMaterialUpdate(pIcon->icon_actor->material, BR_MATU_EXTRA_PRIM);
        } else {
            prev_opacity = pIcon->icon_actor->material->extra_prim[1].v.x;
            pIcon->icon_actor->material->extra_prim[1].v.x = BR_FIXED_INT(255);
            if (prev_opacity != pIcon->icon_actor->material->extra_prim[1].v.x) {
                BrMaterialUpdate(pIcon->icon_actor->material, BR_MATU_EXTRA_PRIM);
            }
            BrMatrix34Identity(&pIcon->icon_actor->t.t.mat);
        }
        BrVector3Set(&pIcon->icon_actor->t.t.translate.t, (float)pX + 16.0f, (float)-(pY + 16), 0.0f);
        RenderThisHeadup(pIcon->icon_actor);
    }
}

// FUNCTION: CARMA2_HW 0x004e09d0
br_actor* C2_HOOK_FASTCALL CreateBillBoard(br_pixelmap* pTexture) {
    br_actor* actor;
    br_material* material;
    br_model* model;
    int width;
    int height;
    br_scalar left;
    br_scalar right;
    br_scalar top;
    br_scalar bottom;

    width = pTexture->width;
    height = pTexture->height;
    model = BrModelAllocate("Billboard Model", 4, 2);
    material = BrMaterialAllocate("Billboard Material");
    actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    actor->identifier = "Billboard Actor";
    actor->model = model;
    actor->material = material;
    actor->render_style = BR_RSTYLE_FACES;
    model->faces[0].vertices[0] = 0;
    model->faces[0].vertices[1] = 1;
    model->faces[0].vertices[2] = 2;
    model->faces[1].vertices[0] = 2;
    model->faces[1].vertices[1] = 3;
    model->faces[1].vertices[2] = 0;
    model->faces[0].material = NULL;
    model->faces[1].material = NULL;
    left = (float)-(width / 2);
    top = (float)+(height / 2);
    BrVector3Set(&model->vertices[0].p, left,  top, -2.0f);
    bottom = (float)-(height / 2);
    BrVector3Set(&model->vertices[1].p, left, bottom, -2.0f);
    right = (float)+(width / 2);
    BrVector3Set(&model->vertices[2].p, right, bottom, -2.0f);
    BrVector3Set(&model->vertices[3].p, right,  top, -2.0f);
    BrVector2Set(&model->vertices[0].map, 0.0f, 0.0f);
    BrVector2Set(&model->vertices[1].map, 0.0f, 1.0f);
    BrVector2Set(&model->vertices[2].map, 1.0f, 1.0f);
    BrVector2Set(&model->vertices[3].map, 1.0f, 0.0f);
    material->colour = 0;
    material->colour_map = pTexture;
    material->flags = BR_MATF_ALWAYS_VISIBLE | BR_MATF_FORCE_FRONT;
    model->flags |= BR_MODF_KEEP_ORIGINAL;
    BrMapAdd(pTexture);
    BrMaterialAdd(material);
    BrModelAdd(model);
    return actor;
}

// FUNCTION: CARMA2_HW 0x004e0ba0
void C2_HOOK_FASTCALL DisposeBillBoard(br_actor* pActor) {

    if (pActor != NULL) {
        if (pActor->material != NULL) {
            BrMaterialRemove(pActor->material);
            BrMaterialFree(pActor->material);
        }
        if (pActor->model != NULL) {
            BrModelRemove(pActor->model);
            BrModelFree(pActor->model);
        }
        if (pActor->parent != NULL) {
            BrActorRemove(pActor);
        }
        BrActorFree(pActor);
    }
}

// FUNCTION: CARMA2_HW 0x004e0c00
void C2_HOOK_FASTCALL SetDefaultPowerupFilename(void) {
    gPowerup_txt_path = "POWERUP.TXT";
}

// FUNCTION: CARMA2_HW 0x004e0c40
int C2_HOOK_FASTCALL TurnOnCloaking(tPowerup* pPowerup, tCar_spec* pCar) {

    if (gNet_mode != eNet_mode_none && !IsCarCloaked(pCar)) {
        BlendifyCar(pCar);
        MasterDisableCarFunks(pCar);
        gCloaked_cars[gCount_cloaked_cars++] = pCar;
    }
    return pPowerup - gPowerup_array;
}

// FUNCTION: CARMA2_HW 0x004e0cb0
void C2_HOOK_FASTCALL RemoveFromCloakingList(tCar_spec* pCar) {
    int i;

    if (gNet_mode != eNet_mode_none) {
        for (i = 0; i < gCount_cloaked_cars; i++) {

            if (gCloaked_cars[i] == pCar) {
#ifdef CARPOCALYPSE2_FIX_BUGS
                memmove(&gCloaked_cars[i], &gCloaked_cars[i + 1], (gCount_cloaked_cars - i - 1) * sizeof(tCar_spec*));
#else
                memcpy(&gCloaked_cars[i], &gCloaked_cars[i + 1], (CARPOCALYPSE2_ASIZE(gCloaked_cars) - i - 1) * sizeof(tCar_spec*));
#endif
                gCount_cloaked_cars -= 1;
                break;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004e0d10
int C2_HOOK_FASTCALL IsCarCloaked(tCar_spec* pCar) {
    int i;

    if (gNet_mode != eNet_mode_none) {
        for (i = 0; i < gCount_cloaked_cars; i++) {
            if (gCloaked_cars[i] == pCar) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004e0d50
void C2_HOOK_FASTCALL TurnOffCloaking(tPowerup* pPowerup, tCar_spec* pCar) {

    if (gNet_mode != eNet_mode_none) {
        UnBlendifyCar(pCar);
        RemoveFromCloakingList(pCar);
        MasterEnableCarFunks(pCar);
        RestoreCarPixelmaps(pCar);
    }
}

// FUNCTION: CARMA2_HW 0x004e0de0
void C2_HOOK_FASTCALL PeriodicCloaking(void) {
    int i;

    if (gNet_mode != eNet_mode_none) {

        for (i = 0; i < gCount_cloaked_cars; i++) {
            DoCamouflageThing(gCloaked_cars[i]);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004e0e20
void C2_HOOK_FASTCALL FUN_004e0e20(void) {

    // empty
}