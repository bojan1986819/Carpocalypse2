#include "netgame.h"

#include "displays.h"
#include "brucetrk.h"
#include "flap.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "network.h"
#include "newgame.h"
#include "controls.h"
#include "structur.h"
#include "opponent.h"
#include "physics.h"
#include "phil.h"
#include "spark.h"
#include "graphics.h"
#include "platform.h"
#include "crush.h"
#include "crush2.h"
#include "powerups.h"
#include "utility.h"
#include "car.h"
#include "racestrt.h"
#include "world.h"
#include "errors.h"
#include "racemem.h"
#include "smashing.h"
#include "carpocalypse2_macros.h"
#include "c2_stdlib.h"

#include <brender/brender.h>
#include "netgame.h"

extern tU8 gNon_car_spec_indices[100];

extern br_quat* C2_HOOK_CDECL BrQuatNormalise(br_quat* q, const br_quat* qq);
extern br_matrix34* C2_HOOK_CDECL BrQuatToMatrix34(br_matrix34* mat, const br_quat* q);

void C2_HOOK_FASTCALL FUN_00498ab0(void* pArg1, tU8* pSrc);

#include "network.h"
#include "loading3.h"
#include "globvars.h"
// GLOBAL: CARMA2_HW 0x0074a738
int gInitialised_grid;

// GLOBAL: CARMA2_HW 0x0074a734
int gIt_or_fox;

// GLOBAL: CARMA2_HW 0x0068d924
tNet_game_player_info* gLast_lepper;

// GLOBAL: CARMA2_HW 0x0068d928
tU32 gLast_it_change;

// GLOBAL: CARMA2_HW 0x0068d934
int gNot_shown_race_type_headup;

// GLOBAL: CARMA2_HW 0x0068d938
int gWinner_declared;

// GLOBAL: CARMA2_HW 0x0068d92c
tU32 gTime_for_punishment;

// GLOBAL: CARMA2_HW 0x0068d920
int gINT_0068d920;

// FUNCTION: CARMA2_HW 0x00499220
void C2_HOOK_FASTCALL ReinitialiseCar(tCar_spec* pCar) {
    StopCarSmokingInstantly(pCar);
    CloseDownPowerUpsForCar(pCar);
    InitialiseCar(pCar);
    TotallyRepairACar(pCar);
    if (pCar != NULL && pCar->driver == eDriver_local_human) {
        gTime_for_punishment = PDGetTotalTime() + 2000;
        EmptyInventory();
    }
}

// GLOBAL: CARMA2_HW 0x0075b8f8
br_pixelmap* gDigits_pix;
// GLOBAL: CARMA2_HW 0x0058a3f8
const br_scalar gFLOAT_0058a3f8 = 1.0f;
// GLOBAL: CARMA2_HW 0x0058a3fc
const br_scalar gFLOAT_0058a3fc = -1.0f;
// GLOBAL: CARMA2_HW 0x0058a400
const br_scalar gFLOAT_0058a400 = 0.5f;
// GLOBAL: CARMA2_HW 0x0058a3d0
const br_scalar gFLOAT_0058a3d0 = 32767.0f;
// GLOBAL: CARMA2_HW 0x0058a3d4
const br_scalar gFLOAT_0058a3d4 = 255.0f;
// GLOBAL: CARMA2_HW 0x0058a3d8
const br_scalar gFLOAT_0058a3d8 = 40.0f;
// GLOBAL: CARMA2_HW 0x0058a3f0
const double gDOUBLE_0058a3f0 = 4.0;
// GLOBAL: CARMA2_HW 0x0058a3e8
const br_scalar gNon_car_pull_actor_distance_squared = 900.0f;
// GLOBAL: CARMA2_HW 0x0058a404
const br_scalar gFLOAT_0058a404 = 65534.0f;
// GLOBAL: CARMA2_HW 0x0058a408
const br_scalar gFLOAT_0058a408 = -0.5f;
// GLOBAL: CARMA2_HW 0x0058a40c
const br_scalar gFLOAT_0058a40c = 0.000015259254723787308f;
// FUNCTION: CARMA2_HW 0x00499260

void C2_HOOK_FASTCALL DisableCar(tCar_spec* pCar) {

    if (pCar->driver_name[0] != '\0') {
        if (!pCar->disabled) {
            pCar->disabled = 1;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] < 500.0f) {
            BrVector3Accumulate(&pCar->car_master_actor->t.t.translate.t, &gInitial_position);
            BrVector3Copy((br_vector3*)pCar->old_frame_mat.m[3], &pCar->car_master_actor->t.t.translate.t);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004992e0
void C2_HOOK_FASTCALL EnableCar(tCar_spec* pCar) {

    if (pCar->driver_name[0] != '\0') {
        if (pCar->disabled) {
            pCar->disabled = 0;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] > 500.0f) {
            BrVector3Sub(&pCar->car_master_actor->t.t.translate.t, &pCar->car_master_actor->t.t.translate.t, &gInitial_position);
            BrVector3Copy((br_vector3*)pCar->old_frame_mat.m[3], &pCar->car_master_actor->t.t.translate.t);
        }
    }
}

// C2PORT-NONMATCHING: best 100% | difftest PASS 2000 trials (spec 0049b690_InitialisePlayerScore.json) | reason removed spurious abort for out-of-range game types
// FUNCTION: CARMA2_HW 0x0049b690
void C2_HOOK_FASTCALL InitialisePlayerScore(tNet_game_player_info* pPlayer) {

    C2_HOOK_BUG_ON(sizeof(tNet_game_details) != 0x78);
    C2_HOOK_BUG_ON(sizeof(tNet_game_options) != 0x30);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_details, field_0x10, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_details, field_0x14, 0x14);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_details, start_race, 0x38);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_details, options, 0x44);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_details, type, 0x74);
    C2_HOOK_BUG_ON(sizeof(tNet_game_player_info) != 0xd4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_player_info, reposition_time, 0x18);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_player_info, score2, 0x64);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_player_info, credits, 0x68);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_game_player_info, wasted, 0x6c);

    PossibleService();
    switch (gCurrent_net_game->type) {
    case eNet_game_type_fight_to_death:
        pPlayer->score2 = 100;
        break;
    case eNet_game_type_1:
    case eNet_game_type_2:
    case eNet_game_type_4:
    case eNet_game_type_5:
    case eNet_game_type_6:
        pPlayer->score2 = 0;
        break;
    case eNet_game_type_checkpoint:
        pPlayer->score2 = 0xffff;
        break;
    case eNet_game_type_foxy:
        pPlayer->score2 = 0;
        break;
    default:
        break;
    }
    pPlayer->credits = gCurrent_net_game->options.starting_credits;
    pPlayer->wasted = 0;
    pPlayer->reposition_time = 0;
}

// C2PORT-NONMATCHING: best 64.08% | difftest PASS 2000 trials (spec difftest/specs/0049b720_InitPlayers.json) | reason VC5 loop induction and register allocation differ
// FUNCTION: CARMA2_HW 0x0049b720
void C2_HOOK_FASTCALL InitPlayers(void) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        InitialisePlayerScore(&gNet_players[i]);
    }
    if (gNet_mode == eNet_mode_host) {
        gLast_it_change = 0;
        gLast_lepper = NULL;
    }
    gTime_for_punishment = 0;
    gNot_shown_race_type_headup = 1;
    gIt_or_fox = -1;
    gWinner_declared = 0;
}

// FUNCTION: CARMA2_HW 0x0049b7e0
void C2_HOOK_FASTCALL FUN_0049b7e0(tU32 pPlayer_id) {
    int game_type;
    int i;
    int player_count;
    tNet_game_player_info* player;

    game_type = gCurrent_net_game->type;
    game_type -= 2;
    if (game_type != 0) {
        game_type -= 4;
        if (game_type != 0) {
            if (--game_type != 0) {
                return;
            }
            if (gNet_players[gIt_or_fox].ID != pPlayer_id) {
                i = 0;
                if (0 < gNumber_of_net_players) {
                    player = gNet_players;
                    do {
                        StopCarBeingIt(player->car);
                        i++;
                        player++;
                    } while (i < gNumber_of_net_players);
                }
                i = 0;
                if (0 < gNumber_of_net_players) {
                    player = gNet_players;
                    player_count = gNumber_of_net_players;
                    do {
                        if (player->ID == pPlayer_id) {
                            FUN_004fe360(player->car);
                            player_count = gNumber_of_net_players;
                            gIt_or_fox = i;
                        }
                        i++;
                        player++;
                    } while (i < player_count);
                }
            }
        } else {
            i = 0;
            if (0 < gNumber_of_net_players) {
                player = gNet_players;
                do {
                    if (player->field_0x80 == 0 && (pPlayer_id & (1u << i)) != 0) {
                        PlayerIsInfected(player);
                    }
                    i++;
                    player++;
                } while (i < gNumber_of_net_players);
            }
        }
        return;
    }
    gINT_0068d920 = pPlayer_id;
}

// FUNCTION: CARMA2_HW 0x0049bd00
void C2_HOOK_FASTCALL InitNetGameplayStuff(void) {

}

// FUNCTION: CARMA2_HW 0x0049bd20
void C2_HOOK_FASTCALL FUN_0049bd20(int* pDst, const int* pSrc) {

    pDst[0] = pSrc[0];
    pDst[1] = pSrc[1];
    pDst[2] = pSrc[2];
    pDst[3] = pSrc[3];
    pDst[4] = pSrc[4];
    pDst[5] = pSrc[5];
    pDst[6] = pSrc[9];
    pDst[7] = pSrc[10];
    pDst[8] = pSrc[11];
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049a760
void C2_HOOK_FASTCALL DeclareWinner(int pWinner_index) {
    int i;
    int j;
    int score_index;
    int score_key;
    int local_index;
    int* player_score;
    tNet_game_player_info* player;
    tNet_game_player_info* candidate;
    tNet_message* message;
    tU32* message_data;
    char winner_message[256];

    if (gWinner_declared != 0) {
        return;
    }

    gWinner_declared = 1;
    DoNetScores2(1);
    message = NetBuildGuaranteedMessage(10, 0);
    message_data = (tU32*)&message->contents.raw.data[2];
    message_data[0] = 8;
    SendGuaranteedMessageToPlayer(gCurrent_net_game, message, gNet_players[pWinner_index].ID, 0);
    gNet_players[pWinner_index].field_0x88++;
    if (gCurrent_net_game->type == eNet_game_type_4 || gCurrent_net_game->type == eNet_game_type_fight_to_death) {
        gNet_players[pWinner_index].score += gScore_winner[10];
    }

    sprintf(winner_message, "%s %s", gNet_players[pWinner_index].player_name, GetMiscString(0xbd));
    local_index = 0;
    for (; local_index < gNumber_of_net_players; local_index++) {
        i = local_index;
        player = &gNet_players[i];
        player_score = &player->score;
        if (gCurrent_net_game->type != eNet_game_type_4 && gCurrent_net_game->type != eNet_game_type_fight_to_death) {
            score_index = player_score[1];
            if (gNumber_of_net_players > 0) {
                for (j = 0; j < gNumber_of_net_players; j++) {
                    candidate = &gNet_players[j];
                    if (candidate->score2 == player->score2 && candidate->last_score_index < score_index) {
                        score_index = candidate->last_score_index;
                    }
                }
            }
            *player_score += gScore_winner[10 - score_index];
        }
        score_key = player->field_0x7c;
        player->field_0x7c = score_key + 1;
        if (local_index != pWinner_index) {
            message = NetBuildGuaranteedMessage(10, 0);
            message_data = (tU32*)&message->contents.raw.data[2];
            message_data[0] = 9;
            SendGuaranteedMessageToPlayer(gCurrent_net_game, message, player->ID, 0);
            NetSendHeadupToPlayer(winner_message, player->ID);
        }
    }

    gReceived_game_scores = 1;
    message = NetBuildGuaranteedMessage(0x1e, 0);
    message_data = (tU32*)&message->contents.raw.data[2];
    for (i = 0; i < gNumber_of_net_players; i++) {
        player = &gNet_players[i];
        message_data[0] = player->field_0x7c;
        message_data[1] = player->field_0x88;
        message_data[2] = player->score;
        message_data += 3;
    }
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, 0);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x0049a940
void C2_HOOK_FASTCALL FUN_0049a940(void) {
    int i;
    int winner_index;
    int players_not_finished;

    players_not_finished = 0;
    winner_index = 0;
    if (gProgram_state.racing != 0 && gNet_mode == eNet_mode_host) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].field_0x80 == 0) {
                players_not_finished++;
                winner_index = i;
            }
        }
        if (players_not_finished <= 1) {
            DeclareWinner(winner_index);
        }
    }
}

// GLOBAL: CARMA2_HW 0x0068d954
int gUNK_0068d954;

// GLOBAL: CARMA2_HW 0x0075bc24
int gUNK_0075bc24;

int C2_HOOK_FASTCALL FUN_0049d400(void);
void C2_HOOK_FASTCALL FUN_0049d420(void);
void C2_HOOK_FASTCALL FadePaletteDown(void);
void C2_HOOK_FASTCALL ClearEntireScreen(void);
void C2_HOOK_FASTCALL FadePaletteUp(void);
void C2_HOOK_FASTCALL KnackerThisCar(tCar_spec* pCar);
void C2_HOOK_FASTCALL ChangeAmbientPratcam(int pIndex);

// FUNCTION: CARMA2_HW 0x0049bad0
void C2_HOOK_FASTCALL FUN_0049bad0(void* pArg1, void* pArg2, int pArg3) {
    int iVar1;
    int iVar2;

    iVar1 = *(int*)((tU8*)pArg1 + 4);
    if (iVar1 == 4) {
        if (gUNK_0068d954 == 0) {
            gUNK_0068d954 = 1;
            NetFullScreenMessage(0xe4, 1);
            iVar1 = FUN_0049d400();
            do {
                NetService(0);
                iVar2 = CheckQuit();
                if (iVar2 != 0) {
                    NetFullScreenMessage(0xe4, 1);
                }
            } while (((gWaiting_for_unpause != 0) && (gUNK_0075bc24 != 2)) &&
                     ((gRace_finished == 0 || (gRace_over_reason != 4))));
            if (iVar1 != 0) {
                FUN_0049d420();
            }
            gWaiting_for_unpause = 1;
            FadePaletteDown();
            ClearEntireScreen();
            FadePaletteUp();
            gUNK_0068d954 = 0;
            return;
        }
    } else {
        if (iVar1 == 5) {
            gWaiting_for_unpause = 0;
            return;
        }
        if (iVar1 == 3) {
            KnackerThisCar(NetCarFromPlayerID(*(tPlayer_ID*)((tU8*)pArg2 + 8)));
            return;
        }
        if (*(int*)((tU8*)gCurrent_net_game + 0x74) >= 3 && *(int*)((tU8*)gCurrent_net_game + 0x74) <= 5) {
            switch (iVar1) {
            case 0:
                Checkpoint(*(int*)((tU8*)pArg1 + 8), 1);
                break;
            case 1:
                WrongCheckpoint(*(int*)((tU8*)pArg1 + 8));
                return;
            case 2:
                DoFancyHeadup(0x1a);
                ChangeAmbientPratcam(0x24);
                gRace_finished = 1;
                return;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x0049bc20
void C2_HOOK_FASTCALL SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* message = NetBuildGuaranteedMessage(0x1b, 0);

    *(int *)((tU8 *)message + 0x20) = pParam_1;
    *(int *)((tU8 *)message + 0x24) = pParam_2;
    *(int *)((tU8 *)message + 0x1c) = pMess;
    *(int *)((tU8 *)message + 0x28) = pParam_3;
    *(int *)((tU8 *)message + 0x2c) = pParam_4;
    SendGuaranteedMessageToPlayer(gCurrent_net_game, message, pPlayer, 0);
}

// FUNCTION: CARMA2_HW 0x0049bc70
void C2_HOOK_FASTCALL SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* message = NetBuildGuaranteedMessage(0x1b, 0);

    *(int *)((tU8 *)message + 0x24) = pParam_2;
    *(int *)((tU8 *)message + 0x1c) = pMess;
    *(int *)((tU8 *)message + 0x20) = pParam_1;
    *(int *)((tU8 *)message + 0x28) = pParam_3;
    *(int *)((tU8 *)message + 0x2c) = pParam_4;
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, 0);
}

// C2PORT-NONMATCHING: best 52.89% | difftest PASS 2000 trials (spec difftest/specs/0049abf0_CalcPlayerScores.json) | reason large state machine has different VC5 stack and register layout
// FUNCTION: CARMA2_HW 0x0049abf0
void C2_HOOK_FASTCALL CalcPlayerScores(void) {
    float dx;
    float dy;
    float dz;
    int i;
    int j;
    int score;
    int alive_count;
    int other_count;
    int other_player_index;
    int checkpoint_count;
    int now;
    int minimum_score;
    int bit_index;
    tNet_game_player_info* player;
    tNet_game_player_info* candidate;
    tNet_game_player_info* other_player;
    tCar_spec* car;
    tCar_spec* fox_car;
    tNet_message* message;
    char headup[272];

    now = (int)GetTotalTime();
    if (gCurrent_net_game->type == eNet_game_type_2) {
        int top_score;
        int second_score;

        top_score = 0;
        second_score = 0;
        for (i = 0; i < gNumber_of_net_players; i++) {
            score = gNet_players[i].score2;
            if (score > top_score) {
                second_score = top_score;
                top_score = score;
            } else if (score > second_score) {
                second_score = score;
            }
        }
        gINT_0068d920 = (top_score + second_score + gCount_smashable_race_targets - GetRuntimeVariable(1)) / 2 + 1;
        if (gCurrent_net_game->options.starting_target < gINT_0068d920) {
            gINT_0068d920 = gCurrent_net_game->options.starting_target;
        }
    } else if (gCurrent_net_game->type == eNet_game_type_foxy) {
        if (gIt_or_fox < 0) {
            gLast_lepper = NULL;
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].last_score_index == gNumber_of_net_players - 1) {
                    break;
                }
            }
            if (i == gNumber_of_net_players) {
                i = IRandomBetween(0, gNumber_of_net_players - 1);
                while (i == -1 && gNet_players[i].car->knackered == 0) {
                    i = IRandomBetween(0, gNumber_of_net_players - 1);
                }
            }
            FUN_0049a9e0(&gNet_players[i]);
        }
        if (gLast_lepper != NULL && gIt_or_fox >= 0) {
            car = gLast_lepper->car;
            fox_car = gNet_players[gIt_or_fox].car;
            dx = car->pos.v[0] - fox_car->pos.v[0];
            dy = car->pos.v[1] - fox_car->pos.v[1];
            dz = car->pos.v[2] - fox_car->pos.v[2];
            if (dx * dx + dy * dy + dz * dz >= gDOUBLE_0058a3f0) {
                gLast_lepper = NULL;
            }
        }
    }

    minimum_score = 9999;
    candidate = NULL;
    for (i = 0; i < gNumber_of_net_players; i++) {
        player = &gNet_players[i];
        car = player->car;
        if (player->reposition_time != 0
                && (int)now >= (int)player->reposition_time
                && (gRace_finished == 0 || gRace_over_reason == -1)) {
            player->wasted = 0;
            player->reposition_time = 0;
            if (gCurrent_net_game->type == eNet_game_type_2
                    || gCurrent_net_game->type == eNet_game_type_checkpoint
                    || gCurrent_net_game->type == eNet_game_type_4) {
                BrMatrix34Copy(&car->car_master_actor->t.t.mat, &player->initial_position);
            } else {
                SetInitialPosition(&gCurrent_race, player->opponent_list_index, player->grid_index);
            }
            StopCarSmokingInstantly(car);
            CloseDownPowerUpsForCar(car);
            InitialiseCar(car);
            TotallyRepairACar(car);
            if (car != NULL && car->driver == eDriver_local_human) {
                gTime_for_punishment = PDGetTotalTime() + 2000;
                EmptyInventory();
            }
            SetFlipUpCar(car);
            car->collision_info->field_0x49c = GetRaceTime() + 100;
            gINT_0074c6d4 = 0;
            gINT_0074b780 = 1;
            message = NetBuildGuaranteedMessage(0x32, 0);
            *(tU32*)((tU8*)message + 0x1c) = gProgram_state.racing;
            player->last_waste_message = 0;
            player->wasteage_attributed = 0;
            *((tU8*)message + 0x24) = (tU8)i;
            CompressMatrix34((tCompressed_matrix3*)((tU8*)message + 0x26),
                (int*)((tU8*)message + 0x20),
                &gCurrent_race.opponent_list[player->opponent_list_index].car_spec->car_master_actor->t.t.mat);
            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, 0);
            if (gProgram_state.racing != 0) {
                SendCurrentPowerups();
            }
            if (car != NULL && car->driver == eDriver_local_human) {
                CancelPendingCunningStunt();
                gProgram_state.credits = gCurrent_net_game->options.starting_credits;
            }
        }
        if (player->last_waste_message != 0 && player->wasteage_attributed == 0
                && (int)(now - player->last_waste_message) > 500) {
            sprintf(headup, "%s %s", player->player_name, GetMiscString(0xac));
            player->last_waste_message = 0;
            player->wasteage_attributed = 0;
            if (gCurrent_net_game->type == eNet_game_type_1) {
                player->score2--;
            }
            NetSendHeadupToEverybody(headup);
        }

        SetKnackeredFlag(car);
        if (car->knackered != 0 && player->wasted == 0) {
            player->wasted = 1;
            message = NetBuildGuaranteedMessage(0x19, 0);
            *(int*)((tU8*)message + 0x20) = -1;
            *(tPlayer_ID*)((tU8*)message + 0x1c) = player->ID;
            NetGuaranteedSendMessageToEverybody(gCurrent_net_game, message, 0);
            switch (gCurrent_net_game->type) {
            case eNet_game_type_fight_to_death:
                alive_count = 0;
                for (j = 0; j < gNumber_of_net_players; j++) {
                    if (gNet_players[j].wasted == 0) {
                        alive_count++;
                    }
                }
                player->score += gScore_winner[10 - alive_count];
                if (alive_count == 1) {
                    DeclareWinner(i);
                } else if (alive_count < 1) {
                    for (j = 0; j < gNumber_of_net_players; j++) {
                        gNet_players[j].field_0x7c++;
                        message = NetBuildGuaranteedMessage(10, 0);
                        *(int*)((tU8*)message + 0x1c) = 9;
                        SendGuaranteedMessageToPlayer(gCurrent_net_game, message, gNet_players[j].ID, 0);
                    }
                }
                break;
            case eNet_game_type_2:
                player->reposition_time = GetTotalTime() + 5000;
                player->score2 /= 2;
                break;
            case eNet_game_type_checkpoint:
                checkpoint_count = gCurrent_race.check_point_count;
                if ((player->score2 >> 16) != checkpoint_count) {
                    bit_index = IRandomBetween(0, checkpoint_count - 1);
                    while ((player->score2 & (1 << bit_index)) != 0) {
                        bit_index++;
                        if (bit_index >= checkpoint_count) {
                            bit_index = 0;
                        }
                    }
                    player->score2 |= 1 << bit_index;
                }
                /* fall through */
            case eNet_game_type_1:
            case eNet_game_type_4:
            case eNet_game_type_5:
                player->reposition_time = GetTotalTime() + 5000;
                break;
            case eNet_game_type_6:
                player->reposition_time = GetTotalTime() + 5000;
                if (player->field_0x80 == 0) {
                    PlayerIsInfected(player);
                }
                break;
            case eNet_game_type_foxy:
                player->reposition_time = GetTotalTime() + 5000;
                player->score2 /= 2;
                if (gNumber_of_net_players > 1 && i == gIt_or_fox) {
                    gLast_lepper = NULL;
                    for (j = 0; j < gNumber_of_net_players; j++) {
                        if (j != i && gNet_players[j].last_score_index == gNumber_of_net_players - 1) {
                            FUN_0049a9e0(&gNet_players[j]);
                            break;
                        }
                    }
                    if (j == gNumber_of_net_players) {
                        j = IRandomBetween(0, gNumber_of_net_players - 1);
                        while (j == i && gNet_players[j].car->knackered == 0) {
                            j = IRandomBetween(0, gNumber_of_net_players - 1);
                        }
                        FUN_0049a9e0(&gNet_players[j]);
                    }
                }
                break;
            }
        }

        switch (gCurrent_net_game->type) {
        case eNet_game_type_fight_to_death:
            if (car->knackered == 0) {
                player->score2 = GetOpponentDamageScore(car);
            } else {
                player->score2 = -1000000;
            }
            break;
        case eNet_game_type_2:
            if (player->score2 >= gINT_0068d920 && gRace_finished == 0) {
                DeclareWinner(i);
            }
            break;
        case eNet_game_type_checkpoint:
            score = player->score2 & 0xffff;
            player->score2 = score;
            checkpoint_count = gCurrent_race.check_point_count;
            alive_count = 0;
            bit_index = score;
            for (j = 0; j < checkpoint_count; j++) {
                if ((bit_index & 1) != 0) {
                    alive_count++;
                }
                bit_index >>= 1;
            }
            player->score2 = (alive_count << 16) | score;
            if (alive_count == 0 && gRace_finished == 0) {
                DeclareWinner(i);
            }
            break;
        case eNet_game_type_4:
            score = player->score2;
            if (score >= 0) {
                if (score < minimum_score) {
                    candidate = player;
                    minimum_score = score;
                } else if (score == minimum_score) {
                    candidate = NULL;
                }
            }
            break;
        case eNet_game_type_foxy:
            if (i == gIt_or_fox && gCountdown == 0 && gRace_finished == 0) {
                player->score2 += gFrame_period;
                if (player->score2 >= gCurrent_net_game->options.starting_target) {
                    DeclareWinner(i);
                }
            }
            break;
        }
    }

    if (gCurrent_net_game->type == eNet_game_type_4 && candidate != NULL && candidate->score2 >= 0) {
        other_count = 0;
        other_player_index = -1;
        for (i = 0; i < gNumber_of_net_players; i++) {
            other_player = &gNet_players[i];
            if (other_player->score2 >= 0 && other_player != candidate) {
                other_count++;
                if (other_player_index == -1) {
                    other_player_index = i;
                } else {
                    other_player_index = -2;
                }
            }
        }
        if (other_count != 0) {
            candidate->car->knackered = 1;
            candidate->wasted = 1;
            candidate->score2 = -1;
            candidate->score += gScore_winner[10 - other_count];
            if (other_player_index == -1) {
                for (i = 0; i < gNumber_of_net_players; i++) {
                    gNet_players[i].field_0x7c++;
                    message = NetBuildGuaranteedMessage(10, 0);
                    *(int*)((tU8*)message + 0x1c) = 9;
                    SendGuaranteedMessageToPlayer(gCurrent_net_game, message, gNet_players[i].ID, 0);
                }
                return;
            }
            if (other_player_index >= 0) {
                DeclareWinner(other_player_index);
                return;
            }
            message = NetBuildGuaranteedMessage(0x1b, 0);
            *(int*)((tU8*)message + 0x1c) = 2;
            *(int*)((tU8*)message + 0x20) = 0;
            *(int*)((tU8*)message + 0x24) = 0;
            *(int*)((tU8*)message + 0x28) = 0;
            *(int*)((tU8*)message + 0x2c) = 0;
            SendGuaranteedMessageToPlayer(gCurrent_net_game, message, candidate->ID, 0);
            sprintf(headup, "%s %s", candidate->player_name, GetMiscString(0xb8));
            NetSendHeadupToEverybody(headup);
        }
    }

}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049b540
void C2_HOOK_FASTCALL PlayerIsInfected(tNet_game_player_info* pPlayer) {
    int i;
    int offset;
    int infected_count;
    int winner_index;
    tPlayer_ID* player_id;
    int* player_infected;
    char message[256];

    if (pPlayer->field_0x80 == 0) {
        FUN_004fe360(pPlayer->car);
        *(int*)((tU8*)pPlayer->car + 0x1bc) = 1;
        *(int*)((tU8*)pPlayer->car + 0x1b4) = 1;
        *(int*)((tU8*)pPlayer->car + 0x1b8) = 1;
        pPlayer->field_0x80 = 1;
        offset = 0x18a8;
        do {
            offset += 4;
            *(tU32*)(offset - 4 + (tU8*)pPlayer->car) = *(tU32*)(offset + 8 + (tU8*)pPlayer->car);
        } while (offset < 0x18b4);
        if (gNet_mode == eNet_mode_host) {
            sprintf(message, "%s", GetMiscString(0xba));
            NetSendHeadupToPlayer(message, pPlayer->ID);
            i = 0;
            if (0 < gNumber_of_net_players) {
                player_id = &gNet_players[0].ID;
                do {
                    if ((tNet_game_player_info*)(player_id - 9) != pPlayer) {
                        sprintf(message, "%s %s", pPlayer->player_name, GetMiscString(0xb9));
                        NetSendHeadupToPlayer(message, *player_id);
                    }
                    i++;
                    player_id += 0x35;
                } while (i < gNumber_of_net_players);
            }
        }
        pPlayer->score2 = -1 - GetTotalTime();
        infected_count = 0;
        i = gProgram_state.racing;
        winner_index = 0;
        if (i != 0 && gNet_mode == eNet_mode_host) {
            i = 0;
            if (0 < gNumber_of_net_players) {
                player_infected = &gNet_players[0].field_0x80;
                do {
                    if (*player_infected == 0) {
                        infected_count++;
                        winner_index = i;
                    }
                    i++;
                    player_infected += 0x35;
                } while (i < gNumber_of_net_players);
            }
            if (infected_count <= 1) {
                DeclareWinner(winner_index);
            }
        }
    }
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL SendPlayerScores(void) {
    tNet_message_chunk* the_contents;
    int i;

    the_contents = NetStartBroadcastContents(24, 0);
    if (the_contents == NULL) {
        return;
    }
    switch (gCurrent_net_game->type) {
    case eNet_game_type_2:
        the_contents->scores.general_score = gINT_0068d920;
        break;
    case eNet_game_type_6:
        the_contents->scores.general_score = 0;
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].field_0x80) {
                the_contents->scores.general_score |= 1 << i;
            }
        }
        break;
    case eNet_game_type_foxy:
        the_contents->scores.general_score = gNet_players[gIt_or_fox].ID;
        break;
    default:
        break;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        the_contents->scores.scores[i] = gNet_players[i].score2;
    }
    NetBroadcastContents(the_contents);
}

// FUNCTION: CARMA2_HW 0x0049aaf0
void C2_HOOK_FASTCALL FUN_0049aaf0(void) {

}

// FUNCTION: CARMA2_HW 0x0049a990
void C2_HOOK_FASTCALL FUN_0049a990(tNet_game_player_info* pNet_player) {
    int elapsed;

    elapsed = PDGetTotalTime();
    if (500 < elapsed - (int)gLast_it_change && gLast_lepper != pNet_player) {
        gLast_it_change = PDGetTotalTime();
        if (gIt_or_fox >= 0) {
            gLast_lepper = &gNet_players[gIt_or_fox];
        }
        FUN_0049a9e0(pNet_player);
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049a9e0
void C2_HOOK_FASTCALL FUN_0049a9e0(tNet_game_player_info* pNet_player) {
    int player_index;
    int i;
    char message[256];

    player_index = pNet_player - gNet_players;
    if (gIt_or_fox != player_index) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            StopCarBeingIt(gNet_players[i].car);
        }
        if (gCurrent_net_game->type == 7) {
            pNet_player->car->power_up_levels[1] = 0;
        }
        FUN_004fe360(pNet_player->car);
        gIt_or_fox = player_index;
        sprintf(message, "%s", GetMiscString(0xbc));
        NetSendHeadupToPlayer(message, pNet_player->ID);
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (&gNet_players[i] != pNet_player) {
                sprintf(message, "%s %s", pNet_player->player_name, GetMiscString(0xbb));
                NetSendHeadupToPlayer(message, gNet_players[i].ID);
            }
        }
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 46.15% | difftest PASS 2000 trials (spec difftest/specs/0049ab00_DoNetGameManagement.json) | reason VC5 inlines score broadcast payload setup
// FUNCTION: CARMA2_HW 0x0049ab00
void C2_HOOK_FASTCALL DoNetGameManagement(void) {
    // GLOBAL: CARMA2_HW 0x00659c34
    static tU32 last_playerscores_sent = -1;

    if (gNet_mode == eNet_mode_host) {
        CalcPlayerScores();
        if (gPHIL_last_physics_tick <= last_playerscores_sent || gPHIL_last_physics_tick >= last_playerscores_sent + 80) {
            last_playerscores_sent = gPHIL_last_physics_tick;
            SendPlayerScores();
        }
    }
}

// FUNCTION: CARMA2_HW 0x00499000
void C2_HOOK_FASTCALL SetUpNetCarPositions(void) {
    int i;
    int j;
    int k;
    int grid_index;
    int racer_count;

    DisableNetService();
    if (!gInitialised_grid) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            gNet_players[i].grid_position_set = 0;
            gNet_players[i].last_waste_message = 0;
            gNet_players[i].wasteage_attributed = 0;
        }
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        gCurrent_race.opponent_list[i].index = -1;
        gCurrent_race.opponent_list[i].ranking = IRandomBetween(0, 99);
        gCurrent_race.opponent_list[i].car_spec = gNet_players[i].car;
        gCurrent_race.opponent_list[i].net_player_index = i;
        gNet_players[i].opponent_list_index = i;
    }
    racer_count = gNumber_of_net_players;
    if (!gInitialised_grid && gCurrent_net_game->options.grid_start) {
        qsort(gCurrent_race.opponent_list, gNumber_of_net_players, sizeof(tOpp_spec), SortGridFunction);
    }
    gCurrent_race.number_of_racers = 0;
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[gCurrent_race.opponent_list[i].net_player_index].opponent_list_index = i;
    }
    for (i = 0; i < racer_count; i++) {
        if ((gCurrent_race.opponent_list[i].car_spec != NULL
                && gCurrent_race.opponent_list[i].car_spec->driver == eDriver_oppo
                && !gInitialised_grid)
            || (gCurrent_race.opponent_list[i].car_spec != NULL
                && gCurrent_race.opponent_list[i].car_spec->driver >= eDriver_net_human
                && !gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_position_set)) {
            grid_index = -1;
            for (j = 0; j < (int)CARPOCALYPSE2_ASIZE(gNet_players) && grid_index < 0; j++) {
                grid_index = j;
                for (k = 0; k < racer_count; k++) {
                    if (k != i
                        && gNet_players[gCurrent_race.opponent_list[k].net_player_index].grid_position_set
                        && gNet_players[gCurrent_race.opponent_list[k].net_player_index].grid_index == j) {
                        grid_index = -1;
                        break;
                    }
                }
            }
            if (grid_index < 0) {
                FatalError(kFatalError_NetworkCodeSelfCheck);
            }
            SetInitialPosition(&gCurrent_race, i, grid_index);
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_index = grid_index;
            if (!gInitialised_grid) {
                gCurrent_race.number_of_racers = i + 1;
            } else {
                InitialiseCar2(gCurrent_race.opponent_list[i].car_spec, 0);
            }
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_position_set = 1;
        }
    }
    gCurrent_race.number_of_racers = racer_count;
    gInitialised_grid = 1;
    ReenableNetService();
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004985f0
void C2_HOOK_FASTCALL ReceivedCarData(tNet_message_chunk_car_data* pContents) {
    tCar_spec* car;
    int player_index;
    int i;
    int input_axis;
    int axis_value;
    tU32 repair_delta;
    float local_4;

    car = NULL;
    for (player_index = 0; player_index < gNumber_of_net_players; player_index++) {
        if (gNet_players[player_index].ID == pContents->player_id) {
            car = gNet_players[player_index].car;
            break;
        }
    }
    if (car == NULL) {
        return;
    }

    if (car->collision_info->message_time > pContents->next_frame_time) {
        return;
    }

    if (car->disabled != 0) {
        EnableCar(car);
        SelectCrushNetwork((undefined4*)pContents->object_hierarchy, car->collision_info, 2);
        car->collision_info->box_face_ref = gFace_num__car - 2;
        car->collision_info->message_time = pContents->next_frame_time;
        car->collision_info->field_0x261 = 0xff;
        for (i = 0; i < 4; i++) {
            car->collision_info->field_0x26c[i] = pContents->suspension_compression[i];
        }
        for (i = 0; i < 12; i++) {
            car->collision_info->field_0x280[i] = pContents->damage_levels[i];
        }
        DisableCar(car);
        return;
    }

    if (gNet_mode == eNet_mode_host) {
        if (player_index == gThis_net_player_index) {
            return;
        }
        if (pContents->hierarchy_flags >= (tU32)car->collision_info->field_0x49c) {
            FUN_00498ab0(car, (tU8*)pContents);
            if (pContents->crush_state != car->car_crush_spec->field_0x24) {
                MarkNetworkShapeStatusesDirty(car->car_crush_spec->network_stuff);
                if (car->car_crush_spec->network_stuff == NULL) {
                    car->car_crush_spec->field_0x26 = pContents->crush_state;
                } else {
                    for (i = 0; i < 12; i++) {
                        if (car->car_crush_spec->network_stuff->items[i].network_id == (int)pContents->player_id) {
                            car->car_crush_spec->network_stuff->items[i].field_0x4 = 0;
                            break;
                        }
                    }
                    if (i == 12) {
                        car->car_crush_spec->field_0x26 = pContents->crush_state;
                    }
                }
            }
        } else {
            if (pContents->hierarchy_flags == 0) {
                car->collision_info->field_0x274 = 0;
                car->collision_info->field_0x261 = 0;
            }
            if (pContents->crush_state == car->car_crush_spec->field_0x24) {
                if (car->car_crush_spec->network_stuff != NULL) {
                    for (i = 0; i < 12; i++) {
                        if (car->car_crush_spec->network_stuff->items[i].network_id == (int)pContents->player_id) {
                            car->car_crush_spec->network_stuff->items[i].field_0x4 = 0;
                            break;
                        }
                    }
                }
            }
        }
        car->power_up_levels[0] = pContents->power_up_levels[0];
        car->power_up_levels[1] = pContents->power_up_levels[1];
        car->power_up_levels[2] = pContents->power_up_levels[2];
        car->keys = *(tCar_controls*)&pContents->controls;
        axis_value = (int)(pContents->controls << 24);
        input_axis = -1;
        if (axis_value >= 0) {
            input_axis = axis_value >> 15;
        }
        car->joystick.acc = input_axis;
        axis_value = (int)((pContents->controls & 0xffffff00U) << 16);
        if (axis_value >= 0) {
            input_axis = axis_value >> 15;
        } else {
            input_axis = -1;
        }
        car->joystick.dec = input_axis;
        return;
    }

    if (gNet_mode != eNet_mode_client) {
        return;
    }
    if (gCar_to_view_id == gNet_players[player_index].ID) {
        ViewNetPlayerByID();
    }
    if (player_index == gThis_net_player_index) {
        car->car_crush_spec->field_0x26 = pContents->crush_state;
        if (pContents->hierarchy_flags <= (tU32)car->collision_info->field_0x49c) {
            return;
        }
        FUN_00498ab0(car, (tU8*)pContents);
        return;
    }

    FUN_00498ab0(car, (tU8*)pContents);
    car->power_up_levels[0] = pContents->power_up_levels[0];
    car->power_up_levels[1] = pContents->power_up_levels[1];
    car->power_up_levels[2] = pContents->power_up_levels[2];
    car->keys = *(tCar_controls*)&car->collision_info->field_0x26c[4];
    axis_value = (int)(*(tU32*)&car->collision_info->field_0x26c[4] << 24);
    input_axis = -1;
    if (axis_value >= 0) {
        input_axis = axis_value >> 15;
    }
    car->joystick.acc = input_axis;
    axis_value = (int)((*(tU32*)&car->collision_info->field_0x26c[4] & 0xffffff00U) << 16);
    if (axis_value >= 0) {
        input_axis = axis_value >> 15;
    } else {
        input_axis = -1;
    }
    car->joystick.dec = input_axis;
    if (car->active != 0) {
        return;
    }

    SelectCrushNetwork((undefined4*)pContents->object_hierarchy, car->collision_info, 2);
    car->pos.v[0] = car->collision_info->transform_matrix.m[3][0];
    car->pos.v[1] = car->collision_info->transform_matrix.m[3][1];
    car->pos.v[2] = car->collision_info->transform_matrix.m[3][2];
    car->collision_info->pos.v[0] = car->collision_info->transform_matrix.m[3][0];
    car->collision_info->pos.v[1] = car->collision_info->transform_matrix.m[3][1];
    car->collision_info->pos.v[2] = car->collision_info->transform_matrix.m[3][2];

    if (car->driver >= eDriver_net_human && car->collision_info->field_0x27c > car->repair_time) {
        repair_delta = car->collision_info->field_0x27c - car->repair_time;
        if (repair_delta >= 100000) {
            TotallyRepairACar(car);
            car->repair_time = car->collision_info->field_0x27c;
        } else {
            ComputeCarImpact(car, repair_delta, &local_4);
        }
        for (i = 0; i < 12; i++) {
            car->damage_units[i].damage_level = car->collision_info->field_0x280[i];
        }
        SetSmokeLastDamageLevel(car);
        StopCarSmoking(car);
    } else {
        for (i = 0; i < 12; i++) {
            car->damage_units[i].damage_level = car->collision_info->field_0x280[i];
        }
        SortOutSmoke(car);
    }

    car->collision_info->box_face_ref = gFace_num__car - 2;
    if ((car->collision_info->field_0x292 & 1) != 0
            && car->collision_info->field_0x290 != car->car_crush_spec->field_0x24) {
        if ((car->collision_info->field_0x292 & 2) != 0) {
            WeldCarByNetwork(car);
        } else if (car->car_crush_spec->field_0x144 != 0) {
            CrushCarByNetwork(car);
        }
        if ((car->collision_info->field_0x292 & 4) != 0) {
            WeldCarPartial(car);
        } else if (car->car_crush_spec->field_0x4b8 != 0) {
            WeldCar(car);
        }
        UpdateCrushVertices(car, (tU8*)&car->collision_info->field_0x2fc);
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00498ab0
void C2_HOOK_FASTCALL FUN_00498ab0(void* pArg1, tU8* pSrc) {
    tU8* data;
    tU8* base;
    int size;
    int i;

    memcpy(*(tU8**)((tU8*)pArg1 + 8) + 0x260, pSrc, 0x34);
    data = pSrc + 0x34;
    size = GetNetworkDataSize(data);
    memcpy(*(tU8**)((tU8*)pArg1 + 8) + 0x294, data, size);
    data += size;
    if ((*(tU8*)(*(tU8**)((tU8*)pArg1 + 8) + 0x292) & 8) != 0) {
        for (i = 0x48c; i < 0x49c; i += 4) {
            base = *(tU8**)((tU8*)pArg1 + 8);
            *(tU32*)(base + i) = *(tU32*)data;
            data += 4;
        }
    } else {
        for (i = 0x48c; i < 0x49c; i += 4) {
            base = *(tU8**)((tU8*)pArg1 + 8);
            *(tU32*)(base + i) = 0;
        }
    }
    if ((*(tU8*)(*(tU8**)((tU8*)pArg1 + 8) + 0x292) & 1) != 0) {
        size = GetCompressedCrushVertexDataSize(data);
        memcpy(*(tU8**)((tU8*)pArg1 + 8) + 0x2fc, data, size);
        data += size;
    }
    if ((*(tU8*)(*(tU8**)((tU8*)pArg1 + 8) + 0x292) & 2) != 0) {
        *(tU8*)(*(tU8**)((tU8*)pArg1 + 8) + 0x481) = *data;
        data += 1;
        *(tU16*)(*(tU8**)((tU8*)pArg1 + 8) + 0x482) = *(tU16*)data;
        data += 2;
    }
    base = *(tU8**)((tU8*)pArg1 + 8);
    if ((*(tU8*)(base + 0x292) & 4) != 0) {
        *(tU32*)(base + 0x484) = *(tU32*)data;
        *(tU32*)(base + 0x488) = *(tU32*)(data + 4);
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00498bb0
void C2_HOOK_FASTCALL FUN_00498bb0(void) {

}

// FUNCTION: CARMA2_HW 0x00498bc0
void C2_HOOK_FASTCALL ReceivedNonCarPosition(tNet_message_chunk_non_car_position* pContents) {
    br_actor* actor;

    actor = gProgram_state.track_spec.non_car_list[pContents->ID];
    if (actor != NULL && gNet_mode != eNet_mode_host) {
        BrMatrix34Copy(&actor->t.t.mat, &pContents->mat);
        if (pContents->flags != 0) {
            actor->identifier[3] = '!';
        }
    }
}

// FUNCTION: CARMA2_HW 0x00498c00
void C2_HOOK_FASTCALL ReceivedNonCar(tNet_message_chunk_non_car* pContents) {
    br_actor* actor;
    tNon_car_spec* non_car;
    tNon_car_spec* candidate_non_car;
    tTrack_square* column;
    int non_car_index;
    tS8 non_car_spec_index;
    tU8 column_x;
    tU8 column_z;
    float dx;
    float dy;
    float dz;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, non_car_id, 0x4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, player_id, 0x8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, flags, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, object_hierarchy, 0x12);

    if (pContents->non_car_id >= (tU32)gProgram_state.track_spec.count_non_cars) {
        return;
    }
    actor = gProgram_state.track_spec.non_car_list[pContents->non_car_id];
    if (actor == NULL) {
        return;
    }

    non_car = (tNon_car_spec*)actor->type_data;
    if (non_car != NULL && (non_car->driver != eDriver_4 || (tU16)non_car->car_ID != pContents->non_car_id)) {
        non_car = NULL;
    }
    if ((pContents->flags & 1) != 0) {
        actor->identifier[3] = '!';
    } else {
        actor->identifier[3] = 'x';
    }
    if (non_car == NULL) {
        if (actor->identifier[1] >= '0' && actor->identifier[1] <= '9') {
            candidate_non_car = NULL;
            non_car_index = actor->identifier[1] * 10 + actor->identifier[2] - 0x210;
            non_car_spec_index = (tS8)gNon_car_spec_indices[non_car_index];
            if (non_car_spec_index != 0) {
                candidate_non_car = &gProgram_state.non_cars[non_car_spec_index + 0x27];
            }
            dx = gProgram_state.current_car.car_master_actor->t.t.translate.t.v[0] - actor->t.t.translate.t.v[0];
            dy = gProgram_state.current_car.car_master_actor->t.t.translate.t.v[1] - actor->t.t.translate.t.v[1];
            dz = gProgram_state.current_car.car_master_actor->t.t.translate.t.v[2] - actor->t.t.translate.t.v[2];
            if ((candidate_non_car != NULL && (candidate_non_car->flags & 0x10000) != 0)
                    || dx * dx + dy * dy + dz * dz < gNon_car_pull_actor_distance_squared) {
                DoPullActorFromWorld(actor);
                non_car = (tNon_car_spec*)actor->type_data;
            }
        }
        if (non_car == NULL) {
            GetSingleMatrixFromNetworkData(pContents->object_hierarchy, &actor->t.t.mat);
            XZToColumnXZ(&column_x, &column_z,
                    actor->t.t.translate.t.v[0], actor->t.t.translate.t.v[2], &gProgram_state.track_spec);
            column = gProgram_state.track_spec.columns[column_z];
            if (column[column_x].actor_0x0 == actor->parent) {
                return;
            }
            if (column[column_x].actor_0x0 == NULL) {
                return;
            }
            BrActorRemove(actor);
            BrActorAdd(gProgram_state.track_spec.columns[column_z][column_x].actor_0x0, actor);
            return;
        }
    }

    if ((pContents->flags & 4) != 0) {
        if ((tU8)non_car->flags == 0) {
            non_car->flags |= 1;
        }
    } else {
        non_car->flags &= ~0xff;
    }
    if ((pContents->flags & 8) != 0) {
        if ((non_car->flags & 0xff00) == 0) {
            non_car->flags |= 0x100;
        }
    } else {
        non_car->flags &= ~0xff00;
    }

    if ((pContents->flags & 2) != 0) {
        SelectCrushNetwork((undefined4*)pContents->object_hierarchy, non_car->collision_info, 2);
        non_car->collision_info->disable_move_rotate = 1;
    } else {
        memcpy(&non_car->collision_info->field_0x294, pContents->object_hierarchy,
                GetNetworkDataSize(pContents->object_hierarchy));
        non_car->collision_info->message_time = pContents->player_id;
        non_car->collision_info->field_0x261 = 0x11;
        non_car->collision_info->disable_move_rotate = 0;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00497ef0
void C2_HOOK_FASTCALL FUN_00497ef0(tCar_crush_network_shapes* pShapes, int pNetwork_id) {
    int i;

    if (pShapes != NULL) {
        for (i = 0; i < 12; i++) {
            if (pShapes->items[i].network_id == pNetwork_id) {
                pShapes->items[i].field_0x4 = 0;
                return;
            }
        }
    }
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 62.44% | difftest PASS 2000 trials (spec difftest/specs/00498e70_SignalToStartRace.json) | reason netgame-loop register allocation and stack layout
// FUNCTION: CARMA2_HW 0x00498e70
void C2_HOOK_FASTCALL SignalToStartRace(void) {
    tNet_message* message;
    tCar_spec* car;
    tU8* dst;
    int i;
    int j;
    int inactive;
    int car_index;

    gCurrent_net_game->no_races_yet = 0;
    gINT_0074c6d4 = 0;
    gINT_0074b780 = 1;
    message = NetBuildGuaranteedMessage(0xc, 0);
    ((tU8*)message)[0x1b] = (tU8)gProgram_state.racing;
    ((tU8*)message)[0x1a] = (tU8)gCurrent_race.number_of_racers;
    dst = (tU8*)message + 0x1e;
    for (i = 0; i < gCurrent_race.number_of_racers; i++) {
        car = gCurrent_race.opponent_list[i].car_spec;
        CompressMatrix34((tCompressed_matrix3*)(dst + 2), &inactive, &car->car_master_actor->t.t.mat);
        for (j = 0; j < gNumber_of_net_players; j++) {
            if (gNet_players[j].car == car) {
                dst[0] = (tU8)j;
                if (inactive != 0) {
                    dst[0] |= 0x80;
                }
                break;
            }
        }
        if (gCurrent_net_game->options.car_choice == 2 || gCurrent_net_game->options.car_choice == 1) {
            car_index = gNet_players[dst[0] & 0x7f].next_car_index;
            if (car_index < 0) {
                car_index = PickARandomCar();
                gNet_players[dst[0] & 0x7f].next_car_index = car_index;
            }
            gCar_details[car_index].ownership = 1;
            dst[1] = (tU8)gNet_players[dst[0] & 0x7f].next_car_index;
        }
        dst += 0x14;
    }
    if (gPending_race < 0) {
        gPending_race = PickNetRace(gProgram_state.current_race_index, gCurrent_net_game->options.race_sequence_type);
    }
    ((tU8*)message)[0x1c] = (tU8)gPending_race;
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
    if (gProgram_state.racing != 0) {
        SendCurrentPowerups();
    }
}

// FUNCTION: CARMA2_HW 0x00499360
void C2_HOOK_FASTCALL DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    static tU32 last_flash;
    static int flash_state;

    if (gNot_shown_race_type_headup) {
        gNot_shown_race_type_headup = 0;
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(gCurrent_net_game->type + 59));
    }
    if (gTime_for_punishment != 0 && PDGetTotalTime() >= gTime_for_punishment) {
        gTime_for_punishment = 0;
        switch (gCurrent_net_game->type) {
        case 2:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(219));
            break;
        case 3:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(220));
            break;
        case 4:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(221));
            break;
        case 7:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(222));
            break;
        default:
            break;
        }
    }
    if (gNet_mode == eNet_mode_none
        || (float)gProgram_state.credits >= gRecovery_cost.initial_network[gCurrent_net_game->type]
        || Flash(200, &last_flash, &flash_state)) {
        sprintf(s, "\xf8%d\xfa %s", pCredits, GetMiscString(94));
        ChangeHeadupText(gNet_cash_headup, s);
    } else {
        ChangeHeadupText(gNet_cash_headup, "");
    }
    switch (gCurrent_net_game->type) {
    case 1:
        sprintf(s, "\xf8%d\xfa %s", gCurrent_net_game->options.starting_target, GetMiscString(241));
        break;
    case 2:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(180), gINT_0068d920);
        break;
    case 5:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(181), gCurrent_net_game->options.starting_target);
        break;
    case 7:
        TimerString(gCurrent_net_game->options.starting_target, s2, 1, 1, 0);
        sprintf(s, "%s \xf8%s\xfa", GetMiscString(182), s2);
        break;
    default:
        s[0] = '\0';
        break;
    }
    ChangeHeadupText(gNet_ped_headup, s);
    switch (gHeadup_detail_level) {
    case 0:
    case 3:
        MoveHeadupTo(gNet_cash_headup, 0x50, 0x19);
        MoveHeadupTo(gNet_ped_headup, 0x50, 0);
        break;
    case 1:
    case 4:
        MoveHeadupTo(gNet_cash_headup, 0x1e0, 0x19);
        MoveHeadupTo(gNet_ped_headup, 0x1e0, 0);
        break;
    case 2:
    case 5:
        MoveHeadupTo(gNet_cash_headup, 0x1ae, 0x3c);
        MoveHeadupTo(gNet_ped_headup, 0x1ae, 0x14);
        break;
    default:
        break;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00498370
int C2_HOOK_FASTCALL FUN_00498370(int* pTable, int pKey) {
    int i;
    int* entry;

    i = 0;
    entry = pTable;
    for (; i < 12; i++, entry += 2) {
        if (*entry == pKey) {
            return (char)pTable[i * 2 + 1];
        }
    }
    return 0;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00498460
void C2_HOOK_FASTCALL CompressCarDataChunk(tU8* pChunk, int pPlayer_index) {
    tCar_spec* car;
    br_actor* actor;

    car = gNet_players[pPlayer_index].car;
    CompressVector3((tCompressed_vector3*)(pChunk + 8), &car->car_master_actor->t.t.translate.t, -500.0f, 500.0f);
    actor = car->car_master_actor;
    *(tS16*)(pChunk + 2) = (tS16)BrRadianToAngle(atan2(actor->t.t.mat.m[2][0], actor->t.t.mat.m[2][2]));
    *(int*)(pChunk + 4) = gNet_players[pPlayer_index].ID;
    *(int*)(pChunk + 0x10) = *(int*)((tU8*)car + 0x18a4);
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 43.11% | difftest PASS 2000 trials (spec difftest/specs/00497900_SendCarData.json) | reason x87-order/regalloc
// FUNCTION: CARMA2_HW 0x00497900
void C2_HOOK_FASTCALL SendCarData(tU32 pNext_frame_time) {
    tNet_message_chunk* message;
    tNet_message_chunk_car_data* local_message;
    tNet_message_chunk_recover* recover_message;
    tNet_game_player_info* source_player;
    tNet_game_player_info* recipient;
    tCar_spec* car;
    tCar_spec* target_car;
    tCar_crush_spec* crush;
    tU8 local_packet[0x208];
    tU8* pData;
    tU8 flags;
    tU8 wheel_flags;
    int hierarchy_size;
    int base_message_size;
    int message_size;
    int crush_size;
    int i;
    int j;
    int k;
    int proxy_index;
    float dx;
    float dy;
    float dz;

    if (gNet_mode == eNet_mode_none) {
        return;
    }

    if (gNet_mode == eNet_mode_host) {
        message = NetStartBroadcastContents(0x12, 0);
        if (message != NULL) {
            *(tU32*)((tU8*)message + 4) = gRace_start;
            NetBroadcastContents(message);
        }
    } else {
        message = NetAllocateMessageChunk(0x12, 0);
        *(tU32*)((tU8*)message + 4) = gRace_start;
    }

    if (gNet_mode == eNet_mode_host) {
        source_player = &gNet_players[0];
        for (i = 0; i < gNumber_of_net_players; i++, source_player++) {
            car = source_player->car;
            if (car->disabled == 0) {
                wheel_flags = 0;
                hierarchy_size = GetHierarchyNetworkSize(car->collision_info);
                base_message_size = hierarchy_size + 0x34;
                if (car->damage_units[8].damage_level > 30
                        || car->damage_units[9].damage_level > 30
                        || car->damage_units[10].damage_level > 30
                        || car->damage_units[11].damage_level > 30) {
                    wheel_flags = 8;
                    base_message_size += 0x10;
                }

                local_message = (tNet_message_chunk_car_data*)local_packet;
                local_message->header.size = (tU8)base_message_size;
                local_message->header.type = eNet_message_chunk_type_car_data;
                local_message->player_id = source_player->ID;
                local_message->next_frame_time = pNext_frame_time;
                local_message->controls = *(tU32*)&car->keys;
                local_message->controls = (local_message->controls & 0xffffff00U) | ((car->joystick.acc >> 9) & 0xff);
                local_message->controls = (local_message->controls & 0xffff00ffU) | ((car->joystick.dec >> 1) & 0xff00);
                for (j = 0; j < 4; j++) {
                    local_message->suspension_compression[j] = (tU8)(car->oldd[j] / car->susp_height[j >> 1] * gFLOAT_0058a3d4);
                }
                local_message->hierarchy_flags = car->collision_info->field_0x49c;
                local_message->curvature = (tU16)(car->curvature / car->maxcurve * gFLOAT_0058a3d0);
                local_message->revs = (tU16)car->revs;
                local_message->repair_time = car->repair_time;
                for (j = 0; j < 12; j++) {
                    local_message->damage_levels[j] = (tU8)car->damage_units[j].damage_level;
                }
                for (j = 0; j < 3; j++) {
                    local_message->power_up_levels[j] = (tU8)car->power_up_levels[j];
                }
                crush = car->car_crush_spec;
                local_message->crush_state = crush->field_0x24;
                local_message->flags = wheel_flags;
                if (GetHierarchyNetworkStuff(car->collision_info, local_message->object_hierarchy, 0x68) != hierarchy_size) {
                    FatalError(0x72, "in car object data");
                }

                pData = local_message->object_hierarchy + hierarchy_size;
                if ((wheel_flags & 8) != 0) {
                    for (j = 0; j < 4; j++) {
                        *(float*)pData = car->wheel_dam_offset[j];
                        pData += sizeof(float);
                    }
                }

                for (j = 1; j < gNumber_of_net_players; j++) {
                    recipient = &gNet_players[j];
                    if (i != j || car->collision_info->field_0x49c != recipient->car->collision_info->field_0x274) {
                        proxy_index = j;
                        if (recipient->field_0xd0 != 0) {
                            for (k = 0; k < gNumber_of_net_players - 1; k++) {
                                if (gNet_players[k].ID == recipient->field_0xd0) {
                                    break;
                                }
                            }
                            proxy_index = k;
                        }

                        if (i != j) {
                            target_car = gNet_players[proxy_index].car;
                            dx = car->car_master_actor->t.t.translate.t.v[0] - target_car->car_master_actor->t.t.translate.t.v[0];
                            dy = car->car_master_actor->t.t.translate.t.v[1] - target_car->car_master_actor->t.t.translate.t.v[1];
                            dz = car->car_master_actor->t.t.translate.t.v[2] - target_car->car_master_actor->t.t.translate.t.v[2];
                            if (sqrt(dx * dx + dy * dy + dz * dz) >= gFLOAT_0058a3d8) {
                                message = NetGetToPlayerContents(0x2d, 0, recipient->ID);
                                CompressCarDataChunk((tU8*)message, i);
                                continue;
                            }
                        }

                        local_message->flags = wheel_flags;
                        message_size = base_message_size;
                        pData = local_message->object_hierarchy + hierarchy_size + ((wheel_flags & 8) != 0 ? 0x10 : 0);
                        crush = car->car_crush_spec;
                        if (crush->version_le_100 != 0) {
                            if (crush->field_0x28 != 0) {
                                crush->field_0x24 = (tU16)(crush->field_0x24 + ((crush->field_0x24 & 1) != 0) + 1);
                                MarkNetworkShapeStatusesDirty(crush->network_stuff);
                                FUN_00497ef0(crush->network_stuff, source_player->ID);
                                crush->field_0x28 = 0;
                            }
                            if (FUN_00498370((int*)crush->network_stuff, gNet_players[proxy_index].ID) != 0) {
                                crush_size = FUN_0043d900(car);
                                message_size += crush_size;
                                local_message->flags |= 1;
                                if (crush->field_0x144 != 0) {
                                    message_size += 3;
                                    local_message->flags |= 2;
                                } else {
                                    message_size++;
                                }
                                if (crush->field_0x4b8 != 0) {
                                    message_size += 8;
                                    local_message->flags |= 4;
                                }
                                CompressCrushVertices(pData, car);
                                pData += crush_size;
                                if ((local_message->flags & 2) != 0) {
                                    flags = 0;
                                    if (crush->field_0x174 == crush->bend_z_min) {
                                        flags |= 1;
                                    }
                                    if (crush->field_0x190 != 0) {
                                        flags |= 2;
                                        *(tU16*)(pData + 1) = crush->field_0x190;
                                    } else if (crush->field_0x192 != 0) {
                                        flags |= 4;
                                        *(tU16*)(pData + 1) = crush->field_0x192;
                                    } else {
                                        flags |= 8;
                                        *(tU16*)(pData + 1) = crush->field_0x194;
                                    }
                                    *pData = flags;
                                    pData += 3;
                                }
                                if ((local_message->flags & 4) != 0) {
                                    CompressVector4((tU16*)pData, &crush->field_0x548.v[0]);
                                }
                            }
                        }
                        local_message->header.size = (tU8)message_size;
                        message = NetGetToPlayerContents(eNet_message_chunk_type_car_data, message_size, recipient->ID);
                        memcpy(message, local_packet, message_size);
                    }
                }

                if (car->time_to_recover != 0 && car->time_to_recover - 500U < pNext_frame_time) {
                    recover_message = (tNet_message_chunk_recover*)NetStartBroadcastContents(eNet_message_chunk_type_car_recover, 0);
                    if (recover_message != NULL) {
                        recover_message->player = source_player->ID;
                        recover_message->time = car->time_to_recover;
                        NetBroadcastContents((tNet_message_chunk*)recover_message);
                    }
                }
            }
        }

        for (i = 0; i < gNum_active_non_cars; i++) {
            if ((gActive_non_car_list[i]->flags & 0x100000) != 0) {
                if (gActive_non_car_list[i]->driver == 2) {
                    SendDetachedNonCarData(gActive_non_car_list[i], pNext_frame_time, NULL);
                } else if (gActive_non_car_list[i]->driver == 3) {
                    SendSplitHalfNonCarData(gActive_non_car_list[i], pNext_frame_time, NULL);
                } else {
                    SendNonCar(gActive_non_car_list[i], pNext_frame_time);
                }
            }
        }
    } else if (gNet_mode == eNet_mode_client) {
        SendLocalCarData(pNext_frame_time);
    }
}

// FUNCTION: CARMA2_HW 0x00497f10
void C2_HOOK_FASTCALL SendNonCar(tNon_car_spec* pNon_car, tU32 pPlayer_id) {
    tNet_message_chunk* message;
    int hierarchy_size;
    int written_size;
    int raw_flags;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, non_car_id, 0x4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, player_id, 0x8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, flags, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tNet_message_chunk_non_car, object_hierarchy, 0x12);

    hierarchy_size = GetHierarchyNetworkSize(pNon_car->collision_info);
    message = NetStartBroadcastContents(eNet_message_chunk_type_non_car, hierarchy_size + 0x12);
    if (message != NULL) {
        message->non_car.non_car_id = (tU16)pNon_car->car_ID;
        message->non_car.player_id = pPlayer_id;
        raw_flags = pNon_car->flags;
        message->non_car.flags = (tU16)(((raw_flags & 0xff00) != 0) * 8
                | ((char)raw_flags != '\0') * 4
                | pNon_car->collision_info->disable_move_rotate * 2
                | (pNon_car->actor->identifier[3] == '!'));
        written_size = GetHierarchyNetworkStuff(pNon_car->collision_info, message->non_car.object_hierarchy, 0x48);
        if (written_size != hierarchy_size) {
            FatalError(kFatalError_NetContentsTooBig_S, "in non_car object data");
        }
        NetBroadcastContents(message);
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 69.62% | difftest PASS 2000 trials (spec difftest/specs/00497fc0_SendLocalCarData.json) | reason compiler codegen/register allocation and x87 scheduling
// FUNCTION: CARMA2_HW 0x00497fc0
void C2_HOOK_FASTCALL SendLocalCarData(tU32 pNext_frame_time) {
    tNet_message_chunk_car_data* message;
    tNet_message_chunk_recover* recover_message;
    tNet_message_chunk_car_crush_sync* crush_sync_message;
    tU8 flags;
    tU8 hierarchy_size;
    int message_size;
    int crush_size;
    int i;
    tU8* pData;

    if (gProgram_state.current_car.disabled != 0) {
        return;
    }

    flags = 0;
    hierarchy_size = (tU8)GetHierarchyNetworkSize(gProgram_state.current_car.collision_info);
    message_size = hierarchy_size + 0x34;
    if (gProgram_state.current_car.damage_units[8].damage_level > 0x1e
            || gProgram_state.current_car.damage_units[9].damage_level > 0x1e
            || gProgram_state.current_car.damage_units[10].damage_level > 0x1e
            || gProgram_state.current_car.damage_units[11].damage_level > 0x1e) {
        flags = 8;
        message_size += 0x10;
    }

    if (gProgram_state.current_car.car_crush_spec->version_le_100 != 0) {
        if (gProgram_state.current_car.car_crush_spec->field_0x28 != 0) {
            gProgram_state.current_car.car_crush_spec->field_0x24 = (tU16)(gProgram_state.current_car.car_crush_spec->field_0x24 + 2 - ((gProgram_state.current_car.car_crush_spec->field_0x24 & 1) != 0));
            gProgram_state.current_car.car_crush_spec->field_0x28 = 0;
        }
        if (gProgram_state.current_car.car_crush_spec->field_0x24 != gProgram_state.current_car.car_crush_spec->field_0x26) {
            crush_size = FUN_0043d900(&gProgram_state.current_car);
            message_size += crush_size;
            flags |= 1;
            if (gProgram_state.current_car.car_crush_spec->field_0x144 != 0) {
                message_size += 3;
                flags |= 2;
            }
            if (gProgram_state.current_car.car_crush_spec->field_0x4b8 != 0) {
                message_size += 8;
                flags |= 4;
            }
        }
    }

    message = (tNet_message_chunk_car_data*)NetAllocateMessageChunk(eNet_message_chunk_type_car_data, message_size);
    message->next_frame_time = pNext_frame_time;
    message->player_id = gNet_players[gThis_net_player_index].ID;
    message->curvature = (tU16)(gProgram_state.current_car.curvature / gProgram_state.current_car.maxcurve * gFLOAT_0058a3d0);
    message->controls = *(tU32*)&gProgram_state.current_car.keys;
    message->controls = (message->controls & 0xffffff00U) | ((gProgram_state.current_car.joystick.acc >> 9) & 0xff);
    message->controls = (message->controls & 0xffff00ffU) | ((gProgram_state.current_car.joystick.dec >> 1) & 0xff00);
    message->revs = (tU16)gProgram_state.current_car.revs;
    message->hierarchy_flags = gProgram_state.current_car.collision_info->field_0x49c;
    for (i = 0; i < 4; i++) {
        message->suspension_compression[i] = (tU8)(gProgram_state.current_car.oldd[i] / gProgram_state.current_car.susp_height[i >> 1] * gFLOAT_0058a3d4);
    }
    for (i = 0; i < 12; i++) {
        message->damage_levels[i] = (tU8)gProgram_state.current_car.damage_units[i].damage_level;
    }
    message->power_up_levels[0] = (tU8)gProgram_state.current_car.power_up_levels[0];
    message->power_up_levels[1] = (tU8)gProgram_state.current_car.power_up_levels[1];
    message->power_up_levels[2] = (tU8)gProgram_state.current_car.power_up_levels[2];
    message->repair_time = gProgram_state.current_car.repair_time;
    message->crush_state = gProgram_state.current_car.car_crush_spec->field_0x24;
    message->flags = flags;
    if (GetHierarchyNetworkStuff(gProgram_state.current_car.collision_info, message->object_hierarchy, 0x68) != hierarchy_size) {
        FatalError(0x72, "in car object data");
    }

    pData = message->object_hierarchy + hierarchy_size;
    if ((flags & 8) != 0) {
        for (i = 0; i < 4; i++) {
            *(float*)pData = gProgram_state.current_car.wheel_dam_offset[i];
            pData += sizeof(float);
        }
    }
    if ((flags & 1) != 0) {
        CompressCrushVertices(pData, &gProgram_state.current_car);
        pData += crush_size;
    }
    if ((flags & 2) != 0) {
        tU8 deformation_flags;
        tU16 deformation_value;

        deformation_flags = 0;
        if (gProgram_state.current_car.car_crush_spec->field_0x174 == gProgram_state.current_car.car_crush_spec->bend_z_min) {
            deformation_flags |= 1;
        }
        if (gProgram_state.current_car.car_crush_spec->field_0x190 != 0) {
            deformation_flags |= 2;
            deformation_value = gProgram_state.current_car.car_crush_spec->field_0x190;
        } else if (gProgram_state.current_car.car_crush_spec->field_0x192 != 0) {
            deformation_flags |= 4;
            deformation_value = gProgram_state.current_car.car_crush_spec->field_0x192;
        } else {
            deformation_flags |= 8;
            deformation_value = gProgram_state.current_car.car_crush_spec->field_0x194;
        }
        *pData++ = deformation_flags;
        *(tU16*)pData = deformation_value;
        pData += sizeof(tU16);
    }
    if ((flags & 4) != 0) {
        CompressVector4((tU16*)pData, &gProgram_state.current_car.car_crush_spec->field_0x548.v[0]);
    }

    if (gProgram_state.current_car.time_to_recover != 0 && gProgram_state.current_car.time_to_recover - 500U < pNext_frame_time) {
        recover_message = (tNet_message_chunk_recover*)NetAllocateMessageChunk(eNet_message_chunk_type_car_recover, 0);
        recover_message->player = gNet_players[gThis_net_player_index].ID;
        recover_message->time = gProgram_state.current_car.time_to_recover;
    }

    crush_sync_message = (tNet_message_chunk_car_crush_sync*)NetAllocateMessageChunk(
        eNet_message_chunk_type_car_crush_sync, gNumber_of_net_players * 8 + 4);
    crush_sync_message->player_id = gNet_players[gThis_net_player_index].ID;
    crush_sync_message->count = (tU8)(gNumber_of_net_players - 1);
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index) {
            crush_sync_message->players[i].player = gNet_players[i].ID;
            crush_sync_message->players[i].crush_state = gNet_players[i].car->car_crush_spec->field_0x24;
        }
    }
}
#pragma auto_inline(on)
// SendShapeNumbers

// ClearShapeStatusFlag

// SendNonCar

// HostFillInCarContents

// ClientSendCarData

// DistanceBetweenCars

// CarShapeNeedsSendingToPlayer

// AddShapeStuffToMechanicsMessage

// TimeToSendData

// SendCarData

// FUNCTION: CARMA2_HW 0x00498390
void C2_HOOK_FASTCALL ReceivedRecover(tNet_message_chunk_recover* pContents) {
    int i;

    if (gNet_players[gThis_net_player_index].ID != pContents->player) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].ID == pContents->player) {
                gNet_players[i].car->time_to_recover = pContents->time;
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004983f0
void C2_HOOK_FASTCALL ReceivedCrushSync(tNet_message_chunk_car_crush_sync* pContents) {
    int i;
    int j;
    int player_id;
    tCar_spec* car;
    tCar_crush_network_shapes* network_stuff;
    tU16* pCrush_state;
    int* network_id;

    i = 0;
    if (pContents->count > 0) {
        pCrush_state = &pContents->players[0].crush_state;
        do {
            car = NetCarFromPlayerID(*((tPlayer_ID*)pCrush_state - 1));
            if (car != NULL && *pCrush_state == car->car_crush_spec->field_0x24) {
                network_stuff = car->car_crush_spec->network_stuff;
                player_id = pContents->player_id;
                if (network_stuff != NULL) {
                    j = 0;
                    network_id = (int*)network_stuff;
                    while (j < 12) {
                        if (*network_id == player_id) {
                            goto clear_network_flag;
                        }
                        j++;
                        network_id += 2;
                    }
                    goto next_crush_state;
clear_network_flag:
                    network_stuff->items[j].field_0x4 = 0;
next_crush_state:
                    ;
                }
            }
            i++;
            pCrush_state += 4;
        } while (i < pContents->count);
    }
}

// C2PORT-NONMATCHING: best 82.02% | difftest PASS 2000 trials (spec difftest/specs/004984c0_FUN_004984c0.json) | reason VC5 register allocation/position copy scheduling
// FUNCTION: CARMA2_HW 0x004984c0
void C2_HOOK_FASTCALL FUN_004984c0(tNet_message_chunk_car_reposition* pContents) {
    int i;
    tCar_spec* car;
    br_matrix34* car_matrix;
    tU32 repair_delta;
    float deflection;

    car = NULL;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pContents->player_id) {
            car = gNet_players[i].car;
            break;
        }
    }
    if (car == NULL) {
        return;
    }
    car->active = 0;
    car_matrix = &car->car_master_actor->t.t.mat;
    BrMatrix34RotateY(car_matrix, pContents->angle);
    ExpandVector3((br_vector3*)&car_matrix->m[3][0], &pContents->position, -500.f, 500.f);
    BrMatrix34Copy(&car->collision_info->transform_matrix, car_matrix);
    car->pos.v[0] = car->collision_info->transform_matrix.m[3][0];
    car->pos.v[1] = car->collision_info->transform_matrix.m[3][1];
    car->pos.v[2] = car->collision_info->transform_matrix.m[3][2];
    car->collision_info->pos.v[0] = car->collision_info->transform_matrix.m[3][0];
    car->collision_info->pos.v[1] = car->collision_info->transform_matrix.m[3][1];
    car->collision_info->pos.v[2] = car->collision_info->transform_matrix.m[3][2];
    if (car->driver >= eDriver_net_human) {
        repair_delta = pContents->repair_time;
        if (repair_delta > car->repair_time) {
            repair_delta -= car->repair_time;
            if (repair_delta >= 100000) {
                TotallyRepairACar(car);
                car->repair_time = car->collision_info->field_0x27c;
                return;
            }
            ComputeCarImpact(car, (int)repair_delta, &deflection);
        }
    }
}

// ReceivedRecover

// CopyMechanics

// ReceivedShapeNumbers

// FillInSimpleCarPos

// ReceivedSimpleCarPos

// ReceivedMechanics

// ReceivedCopInfo

// SendAllNonCarPositions

// ReceivedNonCarPosition

// ReceivedNonCar

// SignalToStartRace

// SignalToRepositionCar

// SetUpNetCarPositions

// ReinitialiseCar

// RepositionPlayer

// DisableCar

// EnableCar

// DoNetworkHeadups

// SortNetHeadAscending

// SortNetHeadDescending

// ClipName

// CreateBoxes

// DrawScoreBoxes

// FUNCTION: CARMA2_HW 0x004999f0
void C2_HOOK_FASTCALL DoNetScores(void) {

    DoNetScores2(0);
}

// FUNCTION: CARMA2_HW 0x0049a710
void C2_HOOK_FASTCALL InitNetHeadups(void) {

    gDigits_pix = LoadPixelmap("HDIGITS.PIX");
    if (gDigits_pix != NULL) {
        BrMapAdd(gDigits_pix);
    }
}

// FUNCTION: CARMA2_HW 0x0049a730
void C2_HOOK_FASTCALL DisposeNetHeadups(void) {

    if (gDigits_pix != NULL) {
        BrMapRemove(gDigits_pix);
        BrPixelmapFree(gDigits_pix);
    }
}

// EverybodysLost

// DeclareWinner

// PlayerIsIt

// CheckForVampireWinner

// PlayerIsInfected

// FarEnoughAway

// CarInContactWithFox

// SelectRandomFox

// CalcPlayerScores

// SendPlayerScores

// DoNetGameManagement

// InitialisePlayerScore

// InitPlayers

// UseGeneralScore

// NetSendEnvironmentChanges

// FUNCTION: CARMA2_HW 0x0049b8d0
void C2_HOOK_FASTCALL NetSendEnvironmentChanges(tNet_game_player_info* pRecipient) {
    int i;
    char exclamation;
    br_actor** non_car_list;
    tNon_car_spec* non_car;
    tNet_message_chunk* message;

    exclamation = '!';
    non_car_list = gProgram_state.track_spec.non_car_list;
    for (i = 0; i < gProgram_state.track_spec.count_non_cars; i++, non_car_list++) {
        if ((*non_car_list)->identifier[3] != exclamation && (*non_car_list)->identifier[3] != '&') {
            continue;
        }
        non_car = (tNon_car_spec*)(*non_car_list)->type_data;
        if (non_car != NULL
                && non_car->driver != 0
                && (tU16)non_car->car_ID == i
                && (non_car->flags & 0x100000) != 0) {
            continue;
        }
        message = NetGetToPlayerContents(eNet_message_chunk_type_non_car_position, 0, pRecipient->ID);
        if (message != NULL) {
            BrMatrix34Copy(&message->non_car_position.mat, &(*non_car_list)->t.t.mat);
            message->non_car_position.ID = i;
            message->non_car_position.flags = (*non_car_list)->identifier[3] == exclamation;
        }
    }
    SendDetachedNonCarDataToPlayer(pRecipient);
    FUN_0043d7d0(pRecipient);
    FUN_0042da20((undefined4*)pRecipient);
    FUN_004f0f80(pRecipient);
    NetSendMessageStacks();
}

// FUNCTION: CARMA2_HW 0x0049b9a0
void C2_HOOK_FASTCALL CheckForNeedyEnvironmentRecipients(void) {
    int player_index;
    int* recipient_field;

    player_index = 1;
    if (player_index < gNumber_of_net_players) {
        recipient_field = &gNet_players[1].field_0x58;
        do {
            if (*recipient_field != 0 && recipient_field[-4] == 6) {
                tNet_game_player_info* recipient;
                register br_actor** non_car_list;
                int non_car_index;

                recipient = (tNet_game_player_info*)((char*)recipient_field - 0x58);
                non_car_list = gProgram_state.track_spec.non_car_list;
                non_car_index = 0;
                if (*(volatile int*)&gProgram_state.track_spec.count_non_cars > 0) {
                    do {
                        if ((*non_car_list)->identifier[3] == '!' || (*non_car_list)->identifier[3] == '&') {
                            tNon_car_spec* non_car = (tNon_car_spec*)(*non_car_list)->type_data;

                            if (non_car == NULL || non_car->driver == 0
                                    || (tU16)non_car->car_ID != non_car_index || (non_car->flags & 0x100000) == 0) {
                                tNet_message_chunk* message;

                                message = NetGetToPlayerContents(eNet_message_chunk_type_non_car_position, 0, recipient->ID);
                                if (message != NULL) {
                                    BrMatrix34Copy(&message->non_car_position.mat, &(*non_car_list)->t.t.mat);
                                    message->non_car_position.ID = non_car_index;
                                    message->non_car_position.flags = (*non_car_list)->identifier[3] == '!';
                                }
                            }
                        }
                        non_car_index++;
                        non_car_list++;
                    } while (non_car_index < *(volatile int*)&gProgram_state.track_spec.count_non_cars);
                }
                SendDetachedNonCarDataToPlayer(recipient);
                FUN_0043d7d0(recipient);
                FUN_0042da20((undefined4*)recipient);
                FUN_004f0f80(recipient);
                NetSendMessageStacks();
                *recipient_field = 0;
                NetSendMessageStacks();
            }
            player_index++;
            recipient_field = (int*)((char*)recipient_field + 0xd4);
        } while (player_index < gNumber_of_net_players);
    }
}

// ReceivedGameplay

// SendGameplay

// SendGameplayToAllPlayers

// SendGameplayToHost

// InitNetGameplayStuff

// FUNCTION: CARMA2_HW 0x0049bd10
void C2_HOOK_FASTCALL DefaultNetName(void) {

    NetObtainSystemUserName(gProgram_state.player_name, 32);
}

// FUNCTION: CARMA2_HW 0x0049bde0
void C2_HOOK_FASTCALL AddPlayerToShapeStatusLists(tPlayer_ID pPlayer_id, int pPlayer_count, tNet_game_player_info* pPlayers) {
    int i;

    for (i = 0; i < pPlayer_count; i++) {
        tCar_spec* car = pPlayers[i].car;

        if (car != NULL && car->car_crush_spec != NULL) {
            tCar_crush_network_shapes* network_shapes = car->car_crush_spec->network_stuff;
            int j;

            for (j = 0; j < 12; j++) {
                if (network_shapes->items[j].network_id == 0) {
                    network_shapes->items[j].network_id = pPlayer_id;
                    network_shapes->items[j].field_0x4 = 1;
                    break;
                }
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x0049be40
void C2_HOOK_FASTCALL RemovePlayerFromShapeStatusLists(tPlayer_ID pPlayer_id, int pPlayer_count, tNet_game_player_info* pPlayers) {
    int i;

    for (i = 0; i < pPlayer_count; i++) {
        tCar_spec* car = pPlayers[i].car;

        if (car != NULL && car->car_crush_spec != NULL) {
            tCar_crush_network_shapes* network_shapes = car->car_crush_spec->network_stuff;
            int j;

            for (j = 0; j < 12; j++) {
                if ((tU32)network_shapes->items[j].network_id == pPlayer_id) {
                    network_shapes->items[j].network_id = 0;
                    break;
                }
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x0049be90
void C2_HOOK_FASTCALL PackMatrixQuaternion(tU16* pOrientation, const br_matrix34* pMatrix) {
    br_quat quaternion;
    br_scalar value;

    BrMatrix34ToQuat(&quaternion, pMatrix);
    value = quaternion.x;
    if (value > gFLOAT_0058a3f8) {
        value = gFLOAT_0058a3f8;
    }
    if (value < gFLOAT_0058a3fc) {
        value = gFLOAT_0058a3fc;
    }
    value = (value - gFLOAT_0058a3fc) * gFLOAT_0058a400;
    value *= gFLOAT_0058a404;
    pOrientation[0] = (tU16)(value - gFLOAT_0058a408);
    value = quaternion.y;
    if (value > gFLOAT_0058a3f8) {
        value = gFLOAT_0058a3f8;
    }
    if (value < gFLOAT_0058a3fc) {
        value = gFLOAT_0058a3fc;
    }
    value = (value - gFLOAT_0058a3fc) * gFLOAT_0058a400;
    value *= gFLOAT_0058a404;
    pOrientation[1] = (tU16)(value - gFLOAT_0058a408);
    value = quaternion.z;
    if (value > gFLOAT_0058a3f8) {
        value = gFLOAT_0058a3f8;
    }
    if (value < gFLOAT_0058a3fc) {
        value = gFLOAT_0058a3fc;
    }
    value = (value - gFLOAT_0058a3fc) * gFLOAT_0058a400;
    value *= gFLOAT_0058a404;
    pOrientation[2] = (tU16)(value - gFLOAT_0058a408);
    value = quaternion.w;
    if (value > gFLOAT_0058a3f8) {
        value = gFLOAT_0058a3f8;
    }
    if (value < gFLOAT_0058a3fc) {
        value = gFLOAT_0058a3fc;
    }
    value = (value - gFLOAT_0058a3fc) * gFLOAT_0058a400;
    value *= gFLOAT_0058a404;
    pOrientation[3] = (tU16)(value - gFLOAT_0058a408);
}

// C2PORT-NONMATCHING: best 23.19% | difftest PASS 2000 trials (spec difftest/specs/0049c000_UnpackMatrixQuaternion.json) | reason x87-scheduling
// FUNCTION: CARMA2_HW 0x0049c000
void C2_HOOK_FASTCALL UnpackMatrixQuaternion(br_matrix34* pMatrix, const tU16* pPacked_quat) {
    volatile int packed[4];
    br_quat quaternion;

    packed[0] = pPacked_quat[0];
    packed[1] = pPacked_quat[1];
    packed[2] = pPacked_quat[2];
    packed[3] = pPacked_quat[3];
    quaternion.x = (float)packed[0] * gFLOAT_0058a40c + (float)packed[0] * gFLOAT_0058a40c - gFLOAT_0058a3f8;
    quaternion.y = (float)packed[1] * gFLOAT_0058a40c + (float)packed[1] * gFLOAT_0058a40c - gFLOAT_0058a3f8;
    quaternion.z = (float)packed[2] * gFLOAT_0058a40c + (float)packed[2] * gFLOAT_0058a40c - gFLOAT_0058a3f8;
    quaternion.w = (float)packed[3] * gFLOAT_0058a40c + (float)packed[3] * gFLOAT_0058a40c - gFLOAT_0058a3f8;
    BrQuatNormalise(&quaternion, &quaternion);
    BrQuatToMatrix34(pMatrix, &quaternion);
}
