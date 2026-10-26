#include "network.h"

#include "controls.h"
#include "car.h"
#include "crush.h"
#include "crush2.h"
#include "pratcam.h"
#include "structur.h"
#include "smashing.h"
#include "smash.h"
#include "piping.h"
#include "displays.h"
#include "errors.h"
#include "explosions.h"
#include "font.h"
#include "globvrme.h"
#include "globvars.h"
#include "globvrpb.h"
#include "loading.h"
#include "netgame.h"
#include "newgame.h"
#include "oil.h"
#include "opponent.h"
#include "pedestrn.h"
#include "platform.h"
#include "physics.h"
#include "powerups.h"
#include "replay.h"
#include "spark.h"
#include "timers.h"
#include "utility.h"
#include "world.h"
#include "win95net.h"
#include "polyfont.h"

#include "c2_string.h"
#include "c2_stdio.h"

#include "brender/brender.h"
#include "network.h"

#include "netgame.h"
#include "spark.h"
#include "controls.h"
#include "platform.h"
#include "globvars.h"
#include "globvrpb.h"

#include "carpocalypse2_macros.h"

#include <stdlib.h>
#include <limits.h>

extern int gUNK_0075bc24;
extern void C2_HOOK_FASTCALL FUN_0049e100(int pPlayer_index);
extern char* gCurrent_palette_pixels;
extern void C2_HOOK_FASTCALL FadePaletteDown(void);
extern void C2_HOOK_FASTCALL ClearEntireScreen(void);
extern void C2_HOOK_FASTCALL EnsureRenderPalette(void);
extern void C2_HOOK_FASTCALL Darken(void);
extern void C2_HOOK_FASTCALL FadePaletteUp(void);
extern void C2_HOOK_FASTCALL FUN_004a48d0(void* pPayload, void* pMessage);
extern void C2_HOOK_FASTCALL FUN_0049bad0(void* pPayload, void* pMessage, int pReceive_time);
extern void C2_HOOK_FASTCALL ReceivedRecover(tNet_message_chunk_recover* pContents);
extern void C2_HOOK_FASTCALL ReceivedCrush(tU8* pContents);
extern void C2_HOOK_FASTCALL FUN_00498bb0(void);
extern void C2_HOOK_FASTCALL FUN_0043c940(void* pPayload);
extern void C2_HOOK_FASTCALL FUN_0043cd60(void* pPayload);
extern void C2_HOOK_FASTCALL ReceiveMineUpdate(tNet_message_chunk_mine_explode* pChunk);

#define MIN_MESSAGES_CAPACITY 200
#define MID_MESSAGES_CAPACITY 200
#define MAX_MESSAGES_CAPACITY 200

#define MESSAGE_MAGIC_NUMBER 0x763a5059

#define MAX_GUARANTEED_MESSAGES 200
#define NET_MSG_STATUS 0x05
#define NET_MSG_LEAVE 0x0b

// GLOBAL: CARMA2_HW 0x0058fd6c
const char gSTRING_0058fd6c[] = "%s %d";

// GLOBAL: CARMA2_HW 0x0065a2e0
const char gSTRING_0065a2e0[] = "PLAYER";

// GLOBAL: CARMA2_HW 0x0065a2d4
const char gSTRING_0065a2d4[] = "%s %s %s %s";

// GLOBAL: CARMA2_HW 0x0065a2c8
const char gSTRING_0065a2c8[] = "%s %s %s";

// GLOBAL: CARMA2_HW 0x0065a148
const char gSTRING_0065a148[] = "Trying to send network message to self";

// GLOBAL: CARMA2_HW 0x0068d96c
extern int gNet_initialised;
// GLOBAL: CARMA2_HW 0x00690c48
extern tNet_message_memory* gDynamic_messages;
// GLOBAL: CARMA2_HW 0x00688b1c
int gNet_storage_space_initialized;

// GLOBAL: CARMA2_HW 0x00659c04
int gScore_winner[11] = { 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 20 };

// GLOBAL: CARMA2_HW 0x00659d34
int gINT_00659d34 = -1;

// GLOBAL: CARMA2_HW 0x00659d38
int gRace_only_flags[52] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 1, 1, 1,
};

// GLOBAL: CARMA2_HW 0x0074a694
int gReceived_game_scores;

// GLOBAL: CARMA2_HW 0x0074a698
tReceived_message_cache_entry gReceived_message_cache[16];

// GLOBAL: CARMA2_HW 0x0068d968
int gINT_0068d968;

// GLOBAL: CARMA2_HW 0x0068d964
int gINT_0068d964;

// GLOBAL: CARMA2_HW 0x00690c40
tU32 gUINT_00690c40;

// GLOBAL: CARMA2_HW 0x00690c50
int gINT_00690c50;

// GLOBAL: CARMA2_HW 0x00690c6c
tU32 gUINT_00690c6c;

// GLOBAL: CARMA2_HW 0x00690c70
tU32 gUINT_00690c70;

// GLOBAL: CARMA2_HW 0x00690c74
tU32 gUINT_00690c74;

// GLOBAL: CARMA2_HW 0x00690c78
undefined4* gPTR_00690c78;

// GLOBAL: CARMA2_HW 0x00690c7c
undefined4* gPTR_00690c7c;

// GLOBAL: CARMA2_HW 0x0068d990
tGuaranteed_message gGuarantee_list[200];

// GLOBAL: CARMA2_HW 0x0068d95c
int gJoin_poll_index;

// GLOBAL: CARMA2_HW 0x0068d978
int gBastard_has_answered;

// GLOBAL: CARMA2_HW 0x0068d984
tU32 gAsk_time;

// GLOBAL: CARMA2_HW 0x0068d98c
int gMessage_header_size;

// GLOBAL: CARMA2_HW 0x00690c80
tU32 gLast_status_broadcast;

// GLOBAL: CARMA2_HW 0x00690230
tAddToJoinListProc* gAdd_proc;

// GLOBAL: CARMA2_HW 0x00690238
int gTime_for_next_one;

// GLOBAL: CARMA2_HW 0x00690c34
tNet_game_details* gCurrent_join_poll_game;

// GLOBAL: CARMA2_HW 0x00690c38
int gNet_service_disable;

// GLOBAL: CARMA2_HW 0x00690c3c
int gIn_net_service;

// GLOBAL: CARMA2_HW 0x00690c48
tNet_message_memory* gDynamic_messages;

// GLOBAL: CARMA2_HW 0x00690c4c
tU32 gLast_flush_message;

// GLOBAL: CARMA2_HW 0x00690c54
int gJoin_list_mode;

// GLOBAL: CARMA2_HW 0x00690c30
int gINT_00690c30;

// GLOBAL: CARMA2_HW 0x0068d974
int gINT_0068d974;

// GLOBAL: CARMA2_HW 0x0068d988
int gINT_0068d988;

// GLOBAL: CARMA2_HW 0x00690234
int gINT_00690234;

// GLOBAL: CARMA2_HW 0x00690240
tNet_game_player_info gReceived_net_player_list[12];

// GLOBAL: CARMA2_HW 0x00762430
int gINT_00762430;

// GLOBAL: CARMA2_HW 0x00763088
int gINT_00763088[48];

// GLOBAL: CARMA2_HW 0x0068d968
int gINT_0068d968;

// GLOBAL: CARMA2_HW 0x0074b7dc
tPlayer_ID gLocal_net_ID;

// GLOBAL: CARMA2_HW 0x00690c44
int gOnly_receive_guarantee_replies;

// GLOBAL: CARMA2_HW 0x0068d97c
tMin_message* gMin_messages;

// GLOBAL: CARMA2_HW 0x0068d970
tMid_message* gMid_messages;

// GLOBAL: CARMA2_HW 0x0068d980
tMax_message* gMax_messages;

// GLOBAL: CARMA2_HW 0x0068d96c
int gNet_initialised;

// GLOBAL: CARMA2_HW 0x0068d960
int gDont_allow_joiners;

// GLOBAL: CARMA2_HW 0x0074a690
tU32 gUINT_0074a690;

// GLOBAL: CARMA2_HW 0x0074a718
tU32 gUINT_0074a718;

// GLOBAL: CARMA2_HW 0x00659d30
tU16 gGuarantee_number = 1;

// GLOBAL: CARMA2_HW 0x0068d958
int gNext_guarantee;

tNet_message_chunk* C2_HOOK_FASTCALL NetStartBroadcastContents(tNet_message_type pType, tS32 pSize_decider);
void C2_HOOK_FASTCALL NetBroadcastContents(tNet_message_chunk* pMessage);

// FUNCTION: CARMA2_HW 0x0049d420
void C2_HOOK_FASTCALL FUN_0049d420(void) {

    gIn_net_service = 1;
}

// C2PORT-NONMATCHING: best 7.14% | difftest PASS 2000 trials (spec difftest/specs/0049d430_NetSendHeadupToAllPlayers.json) | reason inline-network-buffer-codegen
// FUNCTION: CARMA2_HW 0x0049d430
void C2_HOOK_FASTCALL NetSendHeadupToAllPlayers(char* pMessage) {
    tNet_message_chunk* chunk;

    if (gNet_mode == eNet_mode_none) {
        return;
    }
    chunk = NetStartBroadcastContents(0xd, 0);
    if (chunk == NULL) {
        return;
    }
    strcpy((char*)chunk + sizeof(tNet_message_chunk_header), pMessage);
    NetBroadcastContents(chunk);
}

// FUNCTION: CARMA2_HW 0x0049daf0
void C2_HOOK_FASTCALL NetSendHeadupToPlayer(const char* pMessage, tPlayer_ID pPlayer_id) {
    if (gNet_mode != eNet_mode_none) {
        if (pPlayer_id == gLocal_net_ID) {
            if (gProgram_state.racing) {
                NewTextHeadupSlot(4, 0, 3000, -4, pMessage);
                return;
            }
        } else {
            int size_headup;
            int size_guaranteed;
            int size_headup_again;
            int full_size;
            tNet_message* message;

            size_headup = NetGetContentsSize(0xd, 0);
            size_guaranteed = NetGetContentsSize(5, 0);
            size_headup_again = NetGetContentsSize(0xd, 0);
            full_size = size_headup_again + 0x18 + size_guaranteed;
            message = NetAllocateMessage(full_size);
            if (message != NULL) {
                message->contents.raw.header.size = (tU8)size_headup;
                message->header.field_0x14 = 0xff;
                message->header.field_0x16 = (tU16)full_size;
                message->contents.raw.header.type = 0xd;
                ((tU8*)message)[0x19 + (tU8)size_headup] = 5;
                ((tU8*)message)[0x18 + message->contents.raw.header.size] = (tU8)size_guaranteed;
            }
            strcpy((char*)message + 0x1a, pMessage);
            SendGuaranteedMessageToPlayer(gCurrent_net_game, message, pPlayer_id, 0);
        }
    }
}

void C2_HOOK_FASTCALL GetCheckSum(tNet_message* pMessage) {

}

// FUNCTION: CARMA2_HW 0x0049d3e0
void C2_HOOK_FASTCALL DisableNetService(void) {

    gNet_service_disable = 1;
}

// FUNCTION: CARMA2_HW 0x0049d3f0
void C2_HOOK_FASTCALL ReenableNetService(void) {

    gNet_service_disable = 0;
}

// FUNCTION: CARMA2_HW 0x0049d400
int C2_HOOK_FASTCALL FUN_0049d400(void) {

    if (gIn_net_service != 0) {
        gIn_net_service = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0049f860
tNet_message_chunk* C2_HOOK_FASTCALL NetAllocateMessageChunk(tU8 pType, int pOption) {
    int i;
    int chunk_size;
    int offset;
    tNet_message* message;
    tNet_message_chunk* chunk;
    tNet_game_player_info* player;
    tPlayer_ID key;

    key = gNet_players[0].ID;
    chunk_size = NetGetContentsSize(pType, pOption);
    i = 0;
    if (gNumber_of_net_players <= 0) {
        return NULL;
    }
    for (; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == key) {
            goto found_player;
        }
    }
    return NULL;

found_player:
    player = &gNet_players[i];
    if (player == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    message = player->field_0xcc;
    if (message != NULL && message->header.field_0x16 + chunk_size > 0x200) {
        tPlayer_ID player_id = player->ID;
        tNet_game_details* game = gCurrent_net_game;

        if (gNet_mode != eNet_mode_none) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == player_id) {
                    goto send_message;
                }
            }
            goto clear_message;

send_message:
            PDNetSendMessageToAddress(game, message, &gNet_players[i]);
        }
clear_message:
        player->field_0xcc = message = NULL;
    }
    if (message == NULL) {
        message = NetAllocateMessage(0x200);
        player->field_0xcc = message;
        message->header.field_0x16 = (tU16)sizeof(tNet_message_header);
        message->header.field_0x14 = 0;
    }
    offset = message->header.field_0x16;
    chunk = (tNet_message_chunk*)((tU8*)message + offset);
    message->header.field_0x16 += (tU16)chunk_size;
    message->header.field_0x14 += 1;
    chunk->raw.header.type = pType;
    chunk->raw.header.size = (tU8)chunk_size;
    return chunk;
}

// FUNCTION: CARMA2_HW 0x004a64b0
int C2_HOOK_FASTCALL FUN_004a64b0(unsigned int pArg) {
    return pArg >= 10000;
}

// FUNCTION: CARMA2_HW 0x004a6960
tCar_spec* C2_HOOK_FASTCALL NetCarFromPlayerID(tPlayer_ID pPlayer_id) {
    int i;
    tNet_game_player_info* player;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pPlayer_id) {
            player = &gNet_players[i];
            goto found;
        }
    }
    player = NULL;
found:
    if (player != NULL) {
        return player->car;
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x004a69b0
tNet_game_player_info* C2_HOOK_FASTCALL NetPlayerFromCar(tCar_spec *pCar) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].car == pCar) {
            return &gNet_players[i];
        }
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x004a5710
void C2_HOOK_FASTCALL NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason) {
    int i;
    int size_a;
    int size_b;
    int size_c;
    int total_size;
    int fill;
    int result;
    tNet_message* message;
    tNet_game_details* game;

    gINT_0074c6d4 = 0;
    size_a = NetGetContentsSize(0xa, 0);
    size_b = NetGetContentsSize(5, 0);
    size_c = NetGetContentsSize(0xa, 0);
    total_size = size_c + size_b + sizeof(tNet_message_header);
    message = NetAllocateMessage(total_size);
    fill = 0xff;
    if (message != NULL) {
        size_a &= fill;
        message->header.field_0x14 = fill;
        message->header.field_0x16 = (tU16)total_size;
        message->contents.raw.header.size = (tU8)size_a;
        message->contents.raw.header.type = 0xa;
        ((tU8*)message)[0x19 + size_a] = 5;
        ((tU8*)message)[0x18 + message->contents.raw.header.size] = (tU8)size_b;
    }
    *(tU32*)&message->contents.raw.data[2] = pReason;
    game = gCurrent_net_game;
    result = 0;
    if (gNumber_of_net_players == 1) {
        ((tU8*)message)[0x19] = (tU8)fill;
        return;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index) {
            result |= SendGuaranteedMessageToPlayer(game, message, gNet_players[i].ID, 0);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004a0080
void C2_HOOK_FASTCALL NetFullScreenMessage(int pStr_index, int pLeave_it_up_there) {
    tU32 start_time;
    const char* s;
    int size;
    int restore_screen;
    char* pixels_copy;
    char* palette_copy;

    if (!(pLeave_it_up_there || (gProgram_state.racing && !gInterface_within_race_mode))) {
        size = gBack_screen->height * gBack_screen->row_bytes;
        pixels_copy = BrMemAllocate(size, 0xb0);
        palette_copy = BrMemAllocate(0x400, 0xb1);
        memcpy(pixels_copy, gBack_screen->pixels, size);
        memcpy(palette_copy, gCurrent_palette_pixels, 0x400);
        restore_screen = 1;
    } else {
        restore_screen = 0;
    }
    FadePaletteDown();
    LoadFont(4);
    ClearEntireScreen();
    s = GetMiscString(pStr_index);
    OoerrIveGotTextInMeBoxMissus(4, s, gBack_screen,
        gGraf_specs[gGraf_spec_index].total_width / 10,
        gGraf_specs[gGraf_spec_index].total_height / 2 - gFonts[4].height,
        gGraf_specs[gGraf_spec_index].total_width * 9 / 10,
        gGraf_specs[gGraf_spec_index].total_height - 64, 1);
    PDScreenBufferSwap(0);
    EnsureRenderPalette();
    Darken();
    if (pLeave_it_up_there) {
    } else {
        start_time = PDGetTotalTime();
        while (PDGetTotalTime() - start_time < 3000) {
        }
        FadePaletteDown();
        if (restore_screen) {
            memcpy(gBack_screen->pixels, pixels_copy, size);
            memcpy(gCurrent_palette_pixels, palette_copy, 0x400);
            if (pixels_copy != NULL) {
                BrMemFree(pixels_copy);
            }
            if (palette_copy != NULL) {
                BrMemFree(palette_copy);
            }
            PDScreenBufferSwap(0);
            FadePaletteUp();
        } else {
            ClearEntireScreen();
        }
    }
}

// C2PORT-NONMATCHING: best 45.45% | difftest PASS 2000 trials (spec 0049dc00_NetEndJoinList.json) | reason VC5 folds helper sequence and removes unreachable net-mode branch
// FUNCTION: CARMA2_HW 0x0049dc00
void C2_HOOK_FASTCALL NetEndJoinList(void) {

    if (gJoin_list_mode) {
        gJoin_list_mode = 0;
        DisposeCurrentJoinPollGame();
        LeaveTempGame();
        PDNetEndJoinList();
    }
}

// FUNCTION: CARMA2_HW 0x0049dc70
void C2_HOOK_FASTCALL FUN_0049dc70(void* pPointer) {

    if (pPointer != NULL) {
        BrMemFree(pPointer);
    }
}

// FUNCTION: CARMA2_HW 0x0049dc80
void C2_HOOK_FASTCALL NetDisposeGameDetails(tNet_game_details* pDetails) {

    if (pDetails != NULL) {
        BrMemFree(pDetails);
    }
}

// FUNCTION: CARMA2_HW 0x0049e210
tNet_game_details* C2_HOOK_FASTCALL NetHostGame(tNet_game_type pNet_type, tNet_game_options* pOptions, int pRace_index, char* pHost_name, int pCar_index) {
    tNet_game_details* game;
    tNet_game_player_info me;
    void* host_address;
    tU32* player_status;

    game = BrMemAllocate(sizeof(tNet_game_details), kMem_net_pid_details);
    if (pHost_name[0] == '\0') {
        sprintf(pHost_name, "%s", "HOST");
    }
    gNet_service_disable = 1;
    if (((int (C2_HOOK_FASTCALL *)(tNet_game_details*, const char*, void**))PDNetHostGame)(game, pHost_name, &host_address) != 0) {
        gCurrent_net_game = game;
        gINT_0074c6d4 = 0;
        strcpy((char*)game + 0x0e, pHost_name);
        game->host_ID = PDNetExtractPlayerID(game);
        game->num_players = 1;
        memcpy(&game->options, pOptions, sizeof(game->options));
        game->status.stage = 0;
        game->type = pNet_type;
        game->start_race = pRace_index;
        game->no_races_yet = 1;
        gINT_00690c30 = 0;
        gINT_0068d968 = 0;
        gNumber_of_net_players = 0;
        gThis_net_player_index = 0;
        gLocal_net_ID = game->host_ID;
        FillInThisPlayer(game, &me, pCar_index, 1);
        PDNetSetPlayerSystemInfo(&me.pd_net_info, host_address);
        NetPlayersChanged(1, &me);
        player_status = (tU32*)&gNet_players[0].player_status;
        do {
            player_status[-13] = PDGetTotalTime();
            *player_status = 2;
            player_status += 0x35;
        } while ((int)player_status < (int)((tU8*)gNet_players + sizeof(gNet_players) + offsetof(tNet_game_player_info, player_status)));
        gNet_mode = eNet_mode_host;
        InitNetworkShapesStuff(gNet_players[0].car->car_crush_spec);
        gDont_allow_joiners = 0;
        return game;
    }
    if (game != NULL) {
        BrMemFree(game);
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x0049e3a0
void C2_HOOK_FASTCALL FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost) {
    int car_choice;

    pPlayer->host = pHost;
    pPlayer->ID = ((tPlayer_ID (C2_HOOK_FASTCALL *)(void))PDNetExtractPlayerID)();
    strcpy(pPlayer->player_name, gProgram_state.player_name);
    pPlayer->this_players_time_stamp = PDGetTotalTime();
    pPlayer->car_index = pCar_index;
    pPlayer->reposition_time = 0;
    pPlayer->last_waste_message = 0;
    pPlayer->wasteage_attributed = 0;
    pPlayer->field_0x58 = 0;
    pPlayer->name_not_clipped = 1;
    pPlayer->field_0x7c = 0;
    pPlayer->field_0x88 = 0;
    pPlayer->score = 0;
    pPlayer->field_0x80 = 0;
    pPlayer->field_0x84 = 0;
    pPlayer->field_0xcc = 0;
    pPlayer->field_0xd0 = 0;
    car_choice = pGame->options.car_choice;
    if (car_choice == eNet_car_all || car_choice == eNet_car_novelty) {
        pPlayer->next_car_index = -1;
    }
    InitialisePlayerScore(pPlayer);
}

// FUNCTION: CARMA2_HW 0x0049e970
int C2_HOOK_FASTCALL NetJoinGame(tNet_game_details *pGame_details,char *pPlayer_name,int pCar_index) {
    int contents_size_2;
    int contents_size_5;
    int total_size;
    tU8 contents_size_5_byte;
    tNet_message* message;
    tNet_message_memory* next_message;
    tU32 start_time;
    tU32 time;
    int result;

    result = PDNetJoinGame(pGame_details, pPlayer_name);
    if (result == 0) {
        gNet_service_disable = 1;
        gINT_00690c30 = 0;
        gNet_mode = (tNet_mode)3;
        gCurrent_net_game = pGame_details;
        gLocal_net_ID = PDNetExtractPlayerID(pGame_details);
        gNumber_of_net_players = 0;
        gINT_0068d974 = 0;
        gINT_0068d988 = 0;
        gINT_00690234 = 0;
        gINT_0068d968 = 0;

        contents_size_2 = NetGetContentsSize(2, 0);
        contents_size_5 = NetGetContentsSize(5, 0);
        total_size = NetGetContentsSize(2, 0) + sizeof(tNet_message_header) + contents_size_5;
        message = NetAllocateMessage(total_size);
        if (message != NULL) {
            contents_size_5_byte = (tU8)contents_size_5;
            message->contents.raw.header.size = (tU8)contents_size_2;
            message->header.field_0x14 = 0xff;
            message->header.field_0x16 = (tU16)total_size;
            message->contents.raw.header.type = 2;
            ((tU8*)message)[message->contents.raw.header.size + 0x19] = 5;
            ((tU8*)message)[message->contents.raw.header.size + 0x18] = contents_size_5_byte;
        }
        FillInThisPlayer(pGame_details, (tNet_game_player_info*)((tU8*)message + 0x1c), pCar_index, 0);
        gNet_service_disable = 0;
        if (gNet_mode != 0 || gJoin_list_mode != 0) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            AddToGuaranteeList(message, pGame_details, 0, 0);
            PDNetSendMessageToAddress(pGame_details, message, pGame_details);
        }

        start_time = PDGetTotalTime();
        do {
            if (gIn_net_service == 0 && gNet_service_disable == 0) {
                time = PDGetTotalTime();
                gIn_net_service = 1;
                if (gJoin_list_mode != 0) {
                    while (gDynamic_messages != NULL
                            && *(tU8*)((tU8*)gDynamic_messages + gMessage_header_size + 0x1d) == 0xff) {
                        next_message = gDynamic_messages->next;
                        BrMemFree(gDynamic_messages);
                        gDynamic_messages = next_message;
                    }
                    DoNextJoinPoll();
                    NetReceiveAndProcessMessages();
                } else if (gNet_mode != 0) {
                    while (gDynamic_messages != NULL
                            && *(tU8*)((tU8*)gDynamic_messages + gMessage_header_size + 0x1d) == 0xff) {
                        next_message = gDynamic_messages->next;
                        BrMemFree(gDynamic_messages);
                        gDynamic_messages = next_message;
                    }
                    NetReceiveAndProcessMessages();
                    if (time - gLast_status_broadcast > 1000) {
                        gLast_status_broadcast = PDGetTotalTime();
                        BroadcastStatus();
                    }
                    CheckForDisappearees();
                    if (gNet_mode == 2) {
                        CheckForPendingStartRace();
                        CheckForNeedyEnvironmentRecipients();
                    }
                }
                if ((gJoin_list_mode != 0 || gNet_mode != 0)
                        && (ResendGuaranteedMessages(), time > gLast_flush_message + 200U || gProgram_state.racing == 0)) {
                    NetSendMessageStacks();
                }
                gIn_net_service = 0;
            }
        } while (gNumber_of_net_players == 0
                && (tU32)(PDGetTotalTime() - start_time) < 30000
                && gINT_0068d988 == 0
                && gINT_0068d968 == 0);

        gNet_service_disable = 1;
        {
            tU8* player_status;
            tU8* player_status_end;
            player_status = (tU8*)&gNet_players[0].player_status;
            player_status_end = (tU8*)&gNet_players[0].player_status + sizeof(gNet_players);
            do {
                *(tU32*)(player_status - 0x34) = PDGetTotalTime();
                *(int*)player_status = 2;
                player_status += sizeof(tNet_game_player_info);
            } while ((int)player_status < (int)player_status_end);
        }
        if (gNumber_of_net_players == 0) {
            gNet_service_disable = 0;
            if (gINT_0068d988 != 0 && gINT_00690234 != 0) {
                result = -4;
            } else {
                gNet_mode = eNet_mode_none;
                if (gCurrent_net_game != NULL) {
                    BrMemFree(gCurrent_net_game);
                }
                gCurrent_net_game = NULL;
                result = -1 - (gINT_0068d988 != 0);
            }
        }
    }
    return result;
}

// FUNCTION: CARMA2_HW 0x0049dbd0
void C2_HOOK_FASTCALL NetStartProducingJoinList(tAddToJoinListProc *pAdd_proc) {

    gJoin_list_mode = 1;
    gBastard_has_answered = 0;
    gTime_for_next_one = 1;
    gJoin_poll_index = 0;
    gCurrent_join_poll_game = NULL;
    gAdd_proc = pAdd_proc;
    PDNetStartProducingJoinList();
}

// FUNCTION: CARMA2_HW 0x00468750
void C2_HOOK_FASTCALL InitNetStorageSpace(void) {
    int i;

    for (i = 0; i < kMax_netplayers; i++) {
        gCurrent_race.opponent_list[i].car_spec = NULL;
    }
    gNet_storage_space_initialized = 1;
    InitialiseStorageSpace(0, &gNet_cars_storage_space, 2000, 50, 2000, 2000, 50);
}

void C2_HOOK_FASTCALL DisposeNetStorageSpace(void) {

    if (gNet_storage_space_initialized) {
        DisposeStorageSpace(&gNet_cars_storage_space);
    }
    gNet_storage_space_initialized = 0;
}

// FUNCTION: CARMA2_HW 0x0049f680
tNet_message* C2_HOOK_FASTCALL NetBuildGuaranteedMessage(tU8 pNet_message_type, int pOption) {
    int size_a;
    int size_b;
    int size_c;
    int total_size;
    tNet_message* message;

    size_a = NetGetContentsSize(pNet_message_type, pOption);
    size_b = NetGetContentsSize(5, 0);
    size_c = NetGetContentsSize(pNet_message_type, pOption);
    total_size = size_c + size_b + sizeof(tNet_message_header);
    message = NetAllocateMessage(total_size);
    if (message != NULL) {
        size_a &= 0xff;
        message->header.field_0x14 = 0xff;
        message->header.field_0x16 = (tU16)total_size;
        message->contents.raw.header.size = (tU8)size_a;
        message->contents.raw.header.type = pNet_message_type;
        ((tU8*)message)[0x19 + size_a] = 5;
        ((tU8*)message)[0x18 + message->contents.raw.header.size] = (tU8)size_b;
    }
    return message;
}

// FUNCTION: CARMA2_HW 0x004a5c80
void C2_HOOK_FASTCALL NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, void* pNotifyFail) {

    SendGuaranteedMessageToPlayer(pDetails, pMessage, gNet_players[0].ID, (undefined4)pNotifyFail);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049ff70
tNet_message* C2_HOOK_FASTCALL NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    tNet_message* message;

    Timers_Push(TIMER_LLR);
    message = PDNetGetNextMessage(pDetails, pSender_address);
    Timers_Pop(TIMER_LLR);
    return message;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 22.15% | difftest PASS 2000 trials (spec difftest/specs/004a0260_ReceivedMessage.json) | reason vc5-register-stack-layout
// FUNCTION: CARMA2_HW 0x004a0260
void C2_HOOK_FASTCALL ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time) {
  typedef unsigned char byte;
  typedef unsigned short ushort;
  typedef unsigned int uint;
  typedef int bool;
  typedef unsigned char undefined1;
#define true 1
#define false 0

  char cVar1;
  ushort uVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 *extraout_EAX;
  int iVar9;
  undefined1 *puVar10;
  byte *pbVar11;
  int iVar12;
  uint uVar13;
  undefined4 *puVar14;
  uint uVar15;
  byte *pbVar16;
  uint uVar17;
  byte *pbVar18;
  int iVar19;
  undefined4 *puVar20;
  byte *pbVar21;
  uint *puVar22;
  byte *pbVar23;
  bool bVar24;
  undefined1 local_114;
  undefined4 *local_110;
  int local_108;
  int local_104;
  char local_100 [256];

  pbVar18 = (byte *)((int)pMessage + 0x18);
  if (*(short *)((int)pMessage + 0x14) == 0xff) {
    *(undefined2 *)((int)pMessage + 0x14) = 2;
  }
  Timers_Push(TIMER_GUA);
  if ((1 < *(ushort *)((int)pMessage + 0x14)) &&
     (local_110 = (undefined4 *)0x1, iVar12 = gINT_00690c50, pbVar11 = pbVar18 + *pbVar18,
     pbVar16 = pbVar18, 1 < *(ushort *)((int)pMessage + 0x14))) {
    do {
      pbVar21 = pbVar11;
      if (pbVar21[1] == 5) {
        bVar24 = false;
        piVar6 = &gReceived_message_cache[1].sender_id;
        do {
          if (((*(short *)(pbVar21 + 2) == (short)piVar6[1]) && (*(int *)((int)pMessage + 8) == *piVar6))
             && ((ushort)pbVar16[1] == *(ushort *)((int)piVar6 + 6))) {
            bVar24 = true;
            break;
          }
          piVar6 = piVar6 + 2;
        } while ((int)piVar6 < (int)(gReceived_message_cache + CARPOCALYPSE2_ASIZE(gReceived_message_cache)));
        if (bVar24) {
          pbVar16[1] = 0xfe;
          iVar12 = gINT_00690c50;
        }
        else {
          gReceived_message_cache[iVar12 + 1].sequence = *(short *)(pbVar21 + 2);
          iVar12 = iVar12 + 1;
          *(undefined4 *)&gReceived_message_cache[iVar12].sender_id = *(undefined4 *)((int)pMessage + 8);
          *(ushort *)&gReceived_message_cache[iVar12].type = (ushort)pbVar16[1];
          gINT_00690c50 = iVar12;
          if (0xe < iVar12) {
            iVar12 = 0;
            gINT_00690c50 = 0;
          }
        }
      }
      local_110 = (undefined4 *)((int)local_110 + 1);
      pbVar11 = pbVar21 + *pbVar21;
      pbVar16 = pbVar21;
    } while ((int)local_110 < (int)(uint)*(ushort *)((int)pMessage + 0x14));
  }
  Timers_Pop(TIMER_GUA);
  iVar12 = gTimers_max_index;
  iVar7 = gTimers_stack_size;
  local_104 = 0;
  if (*(short *)((int)pMessage + 0x14) == 0) {
    return;
  }
LAB_004a0483:
  if (((gProgram_state.racing == 0) && (*(int *)&gRace_only_flags[pbVar18[1]] != 0)) ||
     ((gOnly_receive_guarantee_replies != 0 && (pbVar18[1] != 4)))) goto switchD_004a051d_caseD_1a;
  if (7 < pbVar18[1]) {
    iVar19 = 0;
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      do {
        if (*piVar6 == *(int *)((int)pMessage + 8)) {
          uVar8 = PDGetTotalTime();
          iVar12 = gTimers_max_index;
          (&gNet_players[0].this_players_time_stamp)[iVar19 * 0x35] = uVar8;
          iVar7 = gTimers_stack_size;
          goto LAB_004a050f;
        }
        iVar19 = iVar19 + 1;
        piVar6 = piVar6 + 0x35;
      } while (iVar19 < (int)gNumber_of_net_players);
    }
    goto switchD_004a051d_caseD_1a;
  }
LAB_004a050f:
  switch(pbVar18[1]) {
  case 0:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gDont_allow_joiners == 0) {
      iVar12 = NetGetContentsSize((((uint)gTimers_stack_size & 0xffffff00U) | 1U),0);
      iVar7 = NetAllocateMessage(iVar12 + 0x18);
      if (iVar7 != 0) {
        *(undefined2 *)(iVar7 + 0x14) = 1;
        *(short *)(iVar7 + 0x16) = (short)(iVar12 + 0x18);
        *(undefined1 *)(iVar7 + 0x19) = 1;
      }
      puVar14 = (undefined4 *)((char *)gCurrent_net_game + 0xe);
      puVar20 = (undefined4 *)(iVar7 + 0x2a);
      for (iVar12 = 0x1a; iVar12 != 0; iVar12 = iVar12 + -1) {
        *puVar20 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar20 = puVar20 + 1;
      }
      *(undefined2 *)puVar20 = *(undefined2 *)puVar14;
      iVar12 = gCurrent_net_game;
      if ((gNet_mode != 0) || (gJoin_list_mode != 0)) {
        *(int *)(iVar7 + 8) = gLocal_net_ID;
        uVar8 = PDGetTotalTime();
        *(undefined4 *)(iVar7 + 0x10) = uVar8;
        PDNetSendMessageToAddress(iVar12,iVar7,(undefined4)pSender_address);
      }
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (1 < iVar12) goto LAB_004a4474;
LAB_004a449c:
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_max_index * 0x1c) = uVar8;
    break;
  case 1:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gCurrent_join_poll_game != 0) {
      gBastard_has_answered = 1;
      pbVar11 = pbVar18 + 0x12;
      puVar14 = (undefined4 *)(gCurrent_join_poll_game + 0xe);
      for (iVar12 = 0x1a; iVar12 != 0; iVar12 = iVar12 + -1) {
        *puVar14 = *(undefined4 *)pbVar11;
        pbVar11 = pbVar11 + 4;
        puVar14 = puVar14 + 1;
      }
      *(undefined2 *)puVar14 = *(undefined2 *)pbVar11;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 2:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    uVar13 = gNumber_of_net_players;
    gTimers_stack_size = gTimers_stack_size + 1;
    if ((gNet_mode == 2) && (gCurrent_net_game != 0)) {
      iVar12 = 0;
      if (0 < (int)gNumber_of_net_players) {
        piVar6 = &gNet_players[0].ID;
        do {
          if (*piVar6 == *(int *)((int)pMessage + 8)) goto LAB_004a0b6c;
          iVar12 = iVar12 + 1;
          piVar6 = piVar6 + 0x35;
        } while (iVar12 < (int)gNumber_of_net_players);
      }
      extraout_EAX = (undefined4 *)BrMemAllocate((uVar13 + 1) * sizeof(tNet_game_player_info), kMem_player_list_join);
      puVar14 = &gNet_players;
      puVar20 = extraout_EAX;
      for (uVar15 = uVar13 * 0x35 & 0x3fffffff; uVar15 != 0; uVar15 = uVar15 - 1) {
        *puVar20 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar20 = puVar20 + 1;
      }
      for (iVar12 = 0; iVar12 != 0; iVar12 = iVar12 + -1) {
        *(undefined1 *)puVar20 = *(undefined1 *)puVar14;
        puVar14 = (undefined4 *)((int)puVar14 + 1);
        puVar20 = (undefined4 *)((int)puVar20 + 1);
      }
      if ((((gCurrent_net_game->options.open_game == 0) && (gProgram_state.racing != 0)) ||
          (0xb < gCurrent_net_game->num_players)) || (gDont_allow_joiners != 0)) {
        iVar12 = NetGetContentsSize(3,0);
        iVar7 = NetAllocateMessage(iVar12 + 0x18);
        if (iVar7 != 0) {
          *(undefined2 *)(iVar7 + 0x14) = 1;
          *(short *)(iVar7 + 0x16) = (short)(iVar12 + 0x18);
          *(undefined1 *)(iVar7 + 0x19) = 3;
        }
        *(undefined4 *)(iVar7 + 0x1c) = 0;
        iVar12 = gCurrent_net_game;
        if ((gNet_mode != 0) || (gJoin_list_mode != 0)) {
          *(int *)(iVar7 + 8) = gLocal_net_ID;
          uVar8 = PDGetTotalTime();
          *(undefined4 *)(iVar7 + 0x10) = uVar8;
LAB_004a0b63:
          PDNetSendMessageToAddress(iVar12,iVar7,pSender_address);
        }
      }
      else {
        iVar12 = 0;
        if (0 < (int)uVar13) {
          piVar6 = extraout_EAX + 9;
          do {
            if (*piVar6 == *(int *)(pbVar18 + 0x28)) goto LAB_004a0b6c;
            iVar12 = iVar12 + 1;
            piVar6 = piVar6 + 0x35;
          } while (iVar12 < (int)uVar13);
        }
        if (*(int *)(pbVar18 + 0x50) < 0) {
          uVar8 = PickARandomCar();
          *(undefined4 *)(pbVar18 + 0x50) = uVar8;
        }
        else {
          iVar12 = 0;
          if (0 < (int)gNumber_of_net_players) {
            piVar6 = &gNet_players[0].car_index;
            do {
              if (*piVar6 == *(int *)(pbVar18 + 0x50)) {
                iVar12 = NetGetContentsSize(3,0);
                iVar7 = NetAllocateMessage(iVar12 + 0x18);
                if (iVar7 != 0) {
                  *(undefined2 *)(iVar7 + 0x14) = 1;
                  *(short *)(iVar7 + 0x16) = (short)(iVar12 + 0x18);
                  *(undefined1 *)(iVar7 + 0x19) = 3;
                }
                *(undefined4 *)(iVar7 + 0x1c) = 0xffffffff;
                iVar12 = gCurrent_net_game;
                if ((gNet_mode == 0) && (gJoin_list_mode == 0)) goto LAB_004a0b6c;
                *(int *)(iVar7 + 8) = gLocal_net_ID;
                uVar8 = PDGetTotalTime();
                *(undefined4 *)(iVar7 + 0x10) = uVar8;
                goto LAB_004a0b63;
              }
              iVar12 = iVar12 + 1;
              piVar6 = piVar6 + 0x35;
            } while (iVar12 < (int)gNumber_of_net_players);
          }
        }
        if (-1 < *(int *)(pbVar18 + 0x50)) {
          gCar_details[*(int *)(pbVar18 + 0x50)].ownership = eCar_owner_someone;
        }
        puVar14 = extraout_EAX + uVar13 * 0x35;
        puVar20 = puVar14;
        pbVar11 = pbVar18;
        for (iVar12 = 0x35; pbVar11 = pbVar11 + 4, iVar12 != 0; iVar12 = iVar12 + -1) {
          *puVar20 = *(undefined4 *)pbVar11;
          puVar20 = puVar20 + 1;
        }
        puVar14[0x12] = 2;
        uVar8 = PDGetTotalTime();
        puVar14[5] = uVar8;
        puVar14[0x15] = 0;
        puVar14[0x16] = 0;
        if (*(char *)(puVar14 + 10) == '\0') {
          sprintf((char *)(puVar14 + 10),gSTRING_0058fd6c,gSTRING_0065a2e0,uVar13);
        }
        PDNetSetPlayerSystemInfo(&((tNet_game_player_info *)puVar14)->pd_net_info, pSender_address);
        NetPlayersChanged((int)uVar13 + 1, (tNet_game_player_info *)extraout_EAX);
        BrMemFree(extraout_EAX);
        local_108 = 0;
        gCurrent_net_game->num_players = gNumber_of_net_players;
        if (0 < (int)gNumber_of_net_players) {
          local_110 = &gNet_players;
          do {
            bVar4 = NetGetContentsSize(3,1);
            iVar7 = NetGetContentsSize(5,0);
            iVar12 = NetGetContentsSize(3,1);
            iVar12 = iVar12 + 0x18 + iVar7;
            iVar19 = NetAllocateMessage(iVar12);
            if (iVar19 != 0) {
              local_114 = (undefined1)iVar7;
              *(byte *)(iVar19 + 0x18) = bVar4;
              *(undefined2 *)(iVar19 + 0x14) = 0xff;
              *(short *)(iVar19 + 0x16) = (short)iVar12;
              *(undefined1 *)(iVar19 + 0x19) = 3;
              *(undefined1 *)(iVar19 + 0x19 + (uint)bVar4) = 5;
              *(undefined1 *)(iVar19 + 0x18 + (uint)*(byte *)(iVar19 + 0x18)) = local_114;
            }
            *(uint *)(iVar19 + 0x1c) = gNumber_of_net_players;
            *(int *)(iVar19 + 0x20) = local_108;
            *(int *)(iVar19 + 0x24) = gUINT_00690c40;
            puVar14 = local_110;
            puVar20 = (undefined4 *)(iVar19 + 0x28);
            for (iVar12 = 0x35; iVar12 != 0; iVar12 = iVar12 + -1) {
              *puVar20 = *puVar14;
              puVar14 = puVar14 + 1;
              puVar20 = puVar20 + 1;
            }
            uVar13 = 0;
            *(undefined4 *)(iVar19 + 0xf4) = 0;
            iVar12 = gCurrent_net_game;
            if (gNumber_of_net_players == 1) {
              *(undefined1 *)(iVar19 + 0x19) = 0xff;
              uVar15 = gNumber_of_net_players;
            }
            else {
              uVar15 = gNumber_of_net_players;
              if (0 < (int)gNumber_of_net_players) {
                puVar14 = &gNet_players[0].ID;
                do {
                  if (uVar13 != gThis_net_player_index) {
                    SendGuaranteedMessageToPlayer(iVar12,iVar19,*puVar14,0);
                    uVar15 = gNumber_of_net_players;
                  }
                  uVar13 = uVar13 + 1;
                  puVar14 = puVar14 + 0x35;
                } while ((int)uVar13 < (int)uVar15);
              }
            }
            local_108 = local_108 + 1;
            local_110 = local_110 + 0x35;
          } while (local_108 < (int)uVar15);
        }
        gUINT_00690c40 = gUINT_00690c40 + 1;
        if (((gInitialised_grid != 0) && (SetUpNetCarPositions(), gINT_0074b780 != 0)) &&
           (gINT_0074c6d4 = 1, 1 < (int)gNumber_of_net_players)) {
          puVar14 = &gNet_players[1].field_0x60;
          iVar12 = gNumber_of_net_players - 1;
          do {
            *puVar14 = 1;
            puVar14 = puVar14 + 0x35;
            iVar12 = iVar12 + -1;
          } while (iVar12 != 0);
        }
      }
    }
LAB_004a0b6c:
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (1 < iVar12) {
      uVar8 = PDGetMicroseconds();
      iVar12 = gTimers_max_index;
      iVar7 = gTimers_stack_size + -1;
      *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) =
           uVar8;
      gTimers_stack_size = iVar7;
      goto switchD_004a051d_caseD_1a;
    }
    goto LAB_004a2e18;
  case 3:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_004a48d0(pbVar18, pMessage);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 4:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xf;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[15].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (0 < gNext_guarantee) {
      puVar14 = &gGuarantee_list[0].recieved;
      iVar12 = gNext_guarantee;
      do {
        if (*(short *)(puVar14 + 7) == *(short *)(pbVar18 + 2)) {
          *puVar14 = 1;
        }
        puVar14 = puVar14 + 0xd;
        iVar12 = iVar12 + -1;
      } while (iVar12 != 0);
    }
    uVar13 = gTimers[15].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[15].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[15].durations[0] + uVar13 * 4) + (iVar7 - gTimers[15].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 5:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xf;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[15].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    iVar12 = *(int *)((int)pMessage + 8);
    iVar7 = NetGetContentsSize(4,0);
    iVar19 = 0;
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      do {
        if (*piVar6 == iVar12) {
          if (&gNet_players + iVar19 * 0x35 == &gNet_players + gThis_net_player_index * 0x35) {
            PDFatalError(gSTRING_0065a148);
          }
          iVar5 = gCurrent_net_game;
          iVar12 = (&gNet_players[0].field_0xcc)[iVar19 * 0x35];
          if (iVar12 == 0) goto LAB_004a1570;
          if ((int)((uint)*(ushort *)(iVar12 + 0x16) + iVar7) < 0x201) goto LAB_004a156c;
          iVar3 = (&gNet_players[0].ID)[iVar19 * 0x35];
          if (gNet_mode == 0) goto LAB_004a1560;
          *(int *)(iVar12 + 8) = gLocal_net_ID;
          uVar8 = PDGetTotalTime();
          *(undefined4 *)(iVar12 + 0x10) = uVar8;
          iVar9 = 0;
          if ((int)gNumber_of_net_players < 1) goto LAB_004a1560;
          piVar6 = &gNet_players[0].ID;
          goto LAB_004a1534;
        }
        iVar19 = iVar19 + 1;
        piVar6 = piVar6 + 0x35;
      } while (iVar19 < (int)gNumber_of_net_players);
    }
    puVar10 = (undefined1 *)0x0;
    goto LAB_004a15b2;
  case 6:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    bVar4 = NetGetContentsSize((((uint)gTimers_stack_size & 0xffffff00U) | 7U),0);
    iVar7 = NetGetContentsSize(5,0);
    iVar12 = NetGetContentsSize(7,0);
    iVar12 = iVar12 + 0x18 + iVar7;
    iVar19 = NetAllocateMessage(iVar12);
    if (iVar19 != 0) {
      local_114 = (undefined1)iVar7;
      *(byte *)(iVar19 + 0x18) = bVar4;
      *(undefined2 *)(iVar19 + 0x14) = 0xff;
      *(short *)(iVar19 + 0x16) = (short)iVar12;
      *(undefined1 *)(iVar19 + 0x19) = 7;
      *(undefined1 *)(bVar4 + 0x19 + iVar19) = 5;
      *(undefined1 *)(iVar19 + 0x18 + (uint)*(byte *)(iVar19 + 0x18)) = local_114;
    }
    iVar12 = 0;
    *(uint *)(iVar19 + 0x1c) = gNumber_of_net_players;
    if (0 < (int)gNumber_of_net_players) {
      puVar14 = (undefined4 *)(iVar19 + 0x24);
      puVar20 = (undefined4 *)&gNet_players[0].player_name;
      do {
        puVar14[-1] = puVar20[9];
        iVar12 = iVar12 + 1;
        *puVar14 = *puVar20;
        puVar14[1] = puVar20[1];
        puVar14[2] = puVar20[2];
        puVar14[3] = puVar20[3];
        *(undefined1 *)((int)puVar14 + 0xf) = 0;
        puVar14 = puVar14 + 5;
        puVar20 = puVar20 + 0x35;
      } while (iVar12 < (int)gNumber_of_net_players);
    }
    iVar12 = gCurrent_net_game;
    if ((gNet_mode != 0) || (gJoin_list_mode != 0)) {
      *(int *)(iVar19 + 8) = gLocal_net_ID;
      uVar8 = PDGetTotalTime();
      *(undefined4 *)(iVar19 + 0x10) = uVar8;
      AddToGuaranteeList((tNet_message*)iVar19, pSender_address, 0, 0);
      PDNetSendMessageToAddress(iVar12,iVar19,(undefined4)pSender_address);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    goto LAB_004a2de0;
  case 7:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    SetNetAvailability(gCurrent_net_game_options);
    local_110 = (undefined4 *)0x0;
    if (0 < gNumber_of_racers) {
      pbVar11 = (byte *)&gCar_details[0].name;
      do {
        iVar12 = 0;
        if (0 < *(int *)(pbVar18 + 4)) {
          pbVar16 = pbVar18 + 0xc;
          do {
            if ((undefined4 *)*(int *)(pbVar16 + -4) == local_110) {
              uVar13 = 0xffffffff;
              pbVar11[-0xffffffff00000004] = 1;
              pbVar11[-0xffffffff00000003] = 0;
              pbVar11[-0xffffffff00000002] = 0;
              pbVar11[-0xffffffff00000001] = 0;
              pbVar21 = pbVar16;
              do {
                pbVar23 = pbVar21;
                if (uVar13 == 0) break;
                uVar13 = uVar13 - 1;
                pbVar23 = pbVar21 + 1;
                bVar4 = *pbVar21;
                pbVar21 = pbVar23;
              } while (bVar4 != 0);
              uVar13 = ~uVar13;
              pbVar21 = pbVar23 + -uVar13;
              pbVar23 = pbVar11;
              for (uVar15 = uVar13 >> 2; uVar15 != 0; uVar15 = uVar15 - 1) {
                *(undefined4 *)pbVar23 = *(undefined4 *)pbVar21;
                pbVar21 = pbVar21 + 4;
                pbVar23 = pbVar23 + 4;
              }
              for (uVar13 = uVar13 & 3; uVar13 != 0; uVar13 = uVar13 - 1) {
                *pbVar23 = *pbVar21;
                pbVar21 = pbVar21 + 1;
                pbVar23 = pbVar23 + 1;
              }
            }
            iVar12 = iVar12 + 1;
            pbVar16 = pbVar16 + 0x14;
          } while (iVar12 < *(int *)(pbVar18 + 4));
        }
        local_110 = (undefined4 *)((int)local_110 + 1);
        pbVar11 = pbVar11 + 0x14;
      } while ((int)local_110 < gNumber_of_racers);
    }
    uVar13 = gTimers[14].index & 0xff;
    gReceived_car_details = 1;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
LAB_004a2de0:
    if (1 < iVar12) {
      uVar8 = PDGetMicroseconds();
      iVar12 = gTimers_max_index;
      iVar7 = gTimers_stack_size + -1;
      *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) =
           uVar8;
      gTimers_stack_size = iVar7;
      goto switchD_004a051d_caseD_1a;
    }
    goto LAB_004a2e18;
  case 8:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    RemoveNetPlayer(pMessage->header.field_0x8);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 9:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gINT_0068d968 == 0) {
      gINT_0068d968 = 1;
      NetFullScreenMessage(0x49, 0);
      gProgram_state.prog_status = 2;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 10:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    gRace_finished = 0;
    if ((gProgram_state.racing != 0) && ((gNet_mode == 3 || (7 < *(int *)(pbVar18 + 4))))) {
      RaceCompleted((tRace_over_reason)*(int*)(pbVar18 + 4));
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0xb:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    iVar12 = 0;
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      do {
        if (*piVar6 == *(int *)((int)pMessage + 8)) {
          (&gNet_players[0].player_status)[iVar12 * 0x35] = (uint)*(ushort *)(pbVar18 + 2);
          uVar8 = PDGetTotalTime();
          (&gNet_players[0].this_players_time_stamp)[iVar12 * 0x35] = uVar8;
          if ((int)(&gNet_players[0].player_status)[iVar12 * 0x35] < 6) {
            DisableCar(gNet_players[iVar12].car);
          }
          else if ((&gNet_players[0].player_status)[iVar12 * 0x35] != 8) {
            if (*(int *)((&gNet_players[0].car)[iVar12 * 0x35] + 4) != 0) {
              SendCurrentPowerups();
            }
            EnableCar(gNet_players[iVar12].car);
          }
          break;
        }
        iVar12 = iVar12 + 1;
        piVar6 = piVar6 + 0x35;
      } while (iVar12 < (int)gNumber_of_net_players);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0xc:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    uVar13 = gNumber_of_net_players;
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gStart_race_sent == 0) {
      iVar12 = 0;
      if ('\0' < (char)pbVar18[2]) {
        piVar6 = &gCurrent_race.opponent_list[0].car_spec;
        pbVar11 = pbVar18 + 6;
        do {
          if ((int)gNumber_of_net_players <= iVar12) break;
          iVar12 = iVar12 + 1;
          gCurrent_race.number_of_racers = iVar12;
          piVar6[-3] = -1;
          piVar6[-1] = -1;
          iVar7 = (&gNet_players[0].car)[(*pbVar11 & 0x7f) * 0x35];
          *piVar6 = iVar7;
          if (iVar7 == 0) {
            if (gINT_0068d968 == 0) {
              gINT_0068d968 = 1;
              NetFullScreenMessage(0x59, 0);
              gProgram_state.prog_status = 2;
            }
            goto LAB_004a138a;
          }
          uVar13 = *pbVar11 & 0x7f;
          piVar6[-2] = uVar13;
          (&gNet_players[0].last_waste_message)[uVar13 * 0x35] = 0;
          (&gNet_players[0].wasteage_attributed)[uVar13 * 0x35] = 0;
          (&gNet_players[0].field_0x80)[uVar13 * 0x35] = 0;
          if (((gProgram_state.racing == 0) || (*piVar6 == 0)) || (*(int *)(*piVar6 + 0xc) != 8)) {
            ExpandMatrix34(&gNet_players[uVar13].car->car_master_actor->t.t.mat, (const tCompressed_matrix3*)(pbVar11 + 2), (int)(*pbVar11 & 0x80));
            InitialiseCar(gNet_players[uVar13].car);
          }
          bVar4 = *pbVar11;
          piVar6 = piVar6 + 4;
          pbVar16 = pbVar11 + 1;
          pbVar11 = pbVar11 + 0x14;
          (&gNet_players[0].next_car_index)[(bVar4 & 0x7f) * 0x35] = (int)(char)*pbVar16;
        } while (iVar12 < (char)pbVar18[2]);
      }
      gPending_race = (int)(char)pbVar18[4];
      gStart_race_sent = 1;
      gCurrent_race.number_of_racers = iVar12;
      if ((pbVar18[3] == 0) || (gProgram_state.racing != 0)) {
        gWait_for_it = 0;
      }
      else {
        gWait_for_it = 1000;
      }
    }
    else {
      iVar12 = 0;
      if ('\0' < (char)pbVar18[2]) {
        pbVar11 = pbVar18 + 6;
        do {
          if ((int)uVar13 <= iVar12) break;
          bVar4 = *pbVar11;
          pbVar16 = pbVar11 + 1;
          pbVar11 = pbVar11 + 0x14;
          iVar12 = iVar12 + 1;
          (&gNet_players[0].next_car_index)[(bVar4 & 0x7f) * 0x35] = (int)(char)*pbVar16;
        } while (iVar12 < (char)pbVar18[2]);
      }
    }
LAB_004a138a:
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0xd:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gProgram_state.racing != 0) {
      NewTextHeadupSlot(4,0,3000,0xfffffffc,pbVar18 + 2);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0xe:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    bVar4 = NetGetContentsSize(0xf,0);
    iVar7 = NetGetContentsSize(5,0);
    iVar12 = NetGetContentsSize(0xf,0);
    iVar12 = iVar12 + 0x18 + iVar7;
    iVar19 = NetAllocateMessage(iVar12);
    if (iVar19 != 0) {
      local_114 = (undefined1)iVar7;
      *(byte *)(iVar19 + 0x18) = bVar4;
      *(undefined2 *)(iVar19 + 0x14) = 0xff;
      *(short *)(iVar19 + 0x16) = (short)iVar12;
      *(undefined1 *)(iVar19 + 0x19) = 0xf;
      *(undefined1 *)(bVar4 + 0x19 + iVar19) = 5;
      *(undefined1 *)(iVar19 + 0x18 + (uint)*(byte *)(iVar19 + 0x18)) = local_114;
    }
    *(int *)(iVar19 + 0x1c) = gProgram_state.racing;
    *(int *)(iVar19 + 0x20) = gProgram_state.current_race_index;
    *(int *)(iVar19 + 0x24) = gPending_race;
    SendGuaranteedMessageToPlayer(gCurrent_net_game,iVar19,*(undefined4 *)((int)pMessage + 8),0);
    if (gProgram_state.racing != 0) {
      SignalToStartRace();
      iVar12 = 0;
      if (0 < (int)gNumber_of_net_players) {
        piVar6 = &gNet_players[0].ID;
        do {
          if (*piVar6 == *(int *)((int)pMessage + 8)) {
            puVar14 = &gNet_players + iVar12 * 0x35;
            goto LAB_004a1a0e;
          }
          iVar12 = iVar12 + 1;
          piVar6 = piVar6 + 0x35;
        } while (iVar12 < (int)gNumber_of_net_players);
      }
      puVar14 = (undefined4 *)0x0;
LAB_004a1a0e:
      puVar14[0x16] = 1;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a2e18;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    iVar7 = gTimers_stack_size + -1;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    gTimers_stack_size = iVar7;
    goto switchD_004a051d_caseD_1a;
  case 0xf:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (*(int *)(pbVar18 + 8) == gProgram_state.current_race_index) {
      if (*(int *)(pbVar18 + 4) != 0) {
        if (gCurrent_net_game->options.open_game == 0) {
          NetFullScreenMessage(0x80, 0);
          gProgram_state.prog_status = 2;
        }
        else {
          gPending_race = *(int *)(pbVar18 + 0xc);
        }
      }
    }
    else {
      NetLeaveGame(gCurrent_net_game);
      NetFullScreenMessage(0x59, 0);
      gProgram_state.prog_status = 2;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x10:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedCarData((tNet_message_chunk_car_data*)pbVar18);
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x11:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedNonCar((tNet_message_chunk_non_car*)pbVar18);
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x12:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if ((int)pReceive_time + 10U < (uint)(*(int *)((int)pMessage + 0x10) - *(int *)(pbVar18 + 4))) {
      gRace_start = gRace_start +
                    (((int)pReceive_time + *(int *)(pbVar18 + 4)) - *(int *)((int)pMessage + 0x10));
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x13:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    iVar12 = 0;
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      do {
        if (*piVar6 == *(int *)(pbVar18 + 4)) {
          gNet_players[iVar12].field_0x60 = 0;
          break;
        }
        iVar12 = iVar12 + 1;
        piVar6 = piVar6 + 0x35;
      } while (iVar12 < (int)gNumber_of_net_players);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x14:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x15:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x16:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedPowerup((tNet_message_chunk_powerup*)pbVar18);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x17:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedRecover((tNet_message_chunk_recover*)pbVar18);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x18:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0049b7e0(*(tPlayer_ID*)(pbVar18 + 4));
    if (0 < (int)gNumber_of_net_players) {
      puVar14 = &gNet_players[0].score2;
      pbVar11 = pbVar18 + 8;
      uVar13 = gNumber_of_net_players;
      do {
        uVar8 = *(undefined4 *)pbVar11;
        pbVar11 = pbVar11 + 4;
        *puVar14 = uVar8;
        puVar14 = puVar14 + 0x35;
        uVar13 = uVar13 - 1;
      } while (uVar13 != 0);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x19:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    iVar12 = 0;
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      do {
        if (*piVar6 == *(int *)(pbVar18 + 4)) {
          puVar14 = &gNet_players + iVar12 * 0x35;
          goto LAB_004a256a;
        }
        iVar12 = iVar12 + 1;
        piVar6 = piVar6 + 0x35;
      } while (iVar12 < (int)gNumber_of_net_players);
    }
    puVar14 = (undefined4 *)0x0;
LAB_004a256a:
    if (puVar14 != (undefined4 *)0x0) {
      *(undefined4 *)(puVar14[0x32] + 0x1d4) = 1;
      if (gNet_mode != 2) {
        ExplodeMeCarUpMeOldMatey(((tNet_game_player_info*)puVar14)->car);
      }
      if (*(int *)(pbVar18 + 4) == gLocal_net_ID) {
        if (gCurrent_net_game->type == 0) {
          DoFancyHeadup(0x19);
          gRace_finished = 1;
        }
        else {
          gUINT_00690c6c = PDGetTotalTime();
          if (gUINT_00690c6c - gUINT_00690c70 < 0x3e9) {
            DoFancyHeadup(0x18);
          }
          else {
            DoFancyHeadup(0x1c);
          }
        }
      }
      iVar7 = gCurrent_net_game;
      iVar12 = *(int *)(pbVar18 + 8);
      if (iVar12 == -1) {
        if (puVar14[7] == 0) {
          uVar8 = GetTotalTime();
          puVar14[7] = uVar8;
        }
      }
      else if (puVar14[0x1c] == 0) {
        if ((iVar12 != -2) && (iVar19 = 0, 0 < (int)gNumber_of_net_players)) {
          piVar6 = &gNet_players[0].ID;
          do {
            if (*piVar6 == iVar12) {
              puVar20 = &gNet_players + iVar19 * 0x35;
              goto LAB_004a2652;
            }
            iVar19 = iVar19 + 1;
            piVar6 = piVar6 + 0x35;
          } while (iVar19 < (int)gNumber_of_net_players);
        }
        puVar20 = (undefined4 *)0x0;
LAB_004a2652:
        if ((((puVar20 != (undefined4 *)0x0) && (gNet_mode == 2)) &&
            (gCurrent_net_game->type == 1)) &&
           (iVar12 = puVar20[0x19], puVar20[0x19] = iVar12 + 1, *(int *)(iVar7 + 0x60) <= iVar12 + 1
           )) {
          DeclareWinner((tNet_game_player_info*)puVar20 - gNet_players);
        }
        puVar14[7] = GetTotalTime();
        bVar24 = puVar14 == gPTR_00690c78;
        puVar14[0x1c] = 1;
        if ((bVar24) && (puVar20 == gPTR_00690c7c)) {
          if (puVar20 == (undefined4 *)0x0) goto LAB_004a2705;
          if ((int)(PDGetTotalTime() - gUINT_00690c74) >= 1000) goto LAB_004a26fc;
          sprintf(local_100, gSTRING_0065a2d4, puVar14 + 10, GetMiscString(0x7e), puVar20 + 10, GetMiscString(0x7f));
        }
        else {
LAB_004a26fc:
          if (puVar20 == (undefined4 *)0x0) {
LAB_004a2705:
            sprintf(local_100, gSTRING_0065a2c8, puVar14 + 10, GetMiscString(0xab), GetMiscString(0xd8));
          }
          else {
            sprintf(local_100, gSTRING_0065a2c8, puVar14 + 10, GetMiscString(0xab), puVar20 + 10);
          }
        }
        NewTextHeadupSlot2(4,0,3000,0xfffffffc,local_100,0);
        gUINT_00690c74 = PDGetTotalTime();
        gPTR_00690c78 = puVar20;
        gPTR_00690c7c = puVar14;
        if (*(int *)(pbVar18 + 8) == gLocal_net_ID) {
          PratcamEvent(0x20);
          gUINT_00690c70 = PDGetTotalTime();
          if (gUINT_00690c70 - gUINT_00690c6c < 0x3e9) {
            DoFancyHeadup(0x19);
          }
          else {
            DoFancyHeadup(0x14);
          }
        }
      }
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  default:
    goto switchD_004a051d_caseD_1a;
  case 0x1b:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0049bad0(pbVar18, pMessage, pReceive_time);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x1c:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedNonCarPosition((tNet_message_chunk_non_car_position*)pbVar18);
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x1d:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_00498bb0();
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x1e:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    gReceived_game_scores = 1;
    if (0 < (int)gNumber_of_net_players) {
      puVar14 = &gNet_players[0].field_0x88;
      pbVar11 = pbVar18 + 8;
      uVar13 = gNumber_of_net_players;
      do {
        puVar14[-3] = *(undefined4 *)(pbVar11 + -4);
        *puVar14 = *(undefined4 *)pbVar11;
        puVar14[2] = *(undefined4 *)(pbVar11 + 4);
        puVar14 = puVar14 + 0x35;
        uVar13 = uVar13 - 1;
        pbVar11 = pbVar11 + 0xc;
      } while (uVar13 != 0);
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x1f:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_004a78e0((tNet_message_chunk*)pbVar18);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x20:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedCrush(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x21:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xb;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[11].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    QueueNetSmash((tQueued_net_smash*)pbVar18);
    uVar13 = gTimers[11].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[11].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[11].durations[0] + uVar13 * 4) + (iVar7 - gTimers[11].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x22:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xb;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[11].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_004f6320((tSmashable_noncar_action*)pbVar18);
    uVar13 = gTimers[11].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[11].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[11].durations[0] + uVar13 * 4) + (iVar7 - gTimers[11].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x23:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedRepulseOpponents((tNet_message_chunk_powerup*)pbVar18);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x24:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0043c940(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x25:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedFullDetachBit(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x26:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0043cd60(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x27:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedDetachedNonCarData(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x28:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0043d5f0(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x29:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_0043d810(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2a:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedCrushSync((tNet_message_chunk_car_crush_sync*)pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2b:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedCrushReattachBit((tNet_message_chunk_crush_reattach_bit*)pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2c:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ToggleFlapsFromNetwork((tNet_message_chunk*)pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2d:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_004984c0((tNet_message_chunk_car_reposition*)pbVar18);
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2e:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    iVar12 = 0;
    uVar13 = PDGetTotalTime();
    if (0 < (int)gNumber_of_net_players) {
      piVar6 = &gNet_players[0].ID;
      uVar15 = gNumber_of_net_players;
      do {
        iVar7 = *piVar6;
        piVar6 = piVar6 + 0x35;
        iVar12 = iVar12 + iVar7;
        uVar15 = uVar15 - 1;
      } while (uVar15 != 0);
    }
    if ((iVar12 == *(int *)(pbVar18 + 4)) && (gNumber_of_net_players == pbVar18[8])) {
      uVar15 = 0;
      if (0 < (int)gNumber_of_net_players) {
        puVar22 = &gNet_players[0].player_status;
        uVar17 = gNumber_of_net_players;
        do {
          if ((uVar15 != gThis_net_player_index) &&
             (bVar4 = pbVar18[uVar15 + 9], *puVar22 = (uint)bVar4, bVar4 != 5)) {
            puVar22[-0xd] = uVar13;
            if ((int)*puVar22 < 6) {
              DisableCar(gNet_players[iVar12].car);
              uVar17 = gNumber_of_net_players;
            }
            else if (*puVar22 != 8) {
              if (*(int *)(puVar22[0x20] + 4) != 0) {
                SendCurrentPowerups();
              }
              EnableCar(gNet_players[iVar12].car);
              uVar17 = gNumber_of_net_players;
            }
          }
          uVar15 = uVar15 + 1;
          puVar22 = puVar22 + 0x35;
        } while ((int)uVar15 < (int)uVar17);
      }
    }
    else {
      gNet_players[0].player_status = (uint)pbVar18[9];
      gNet_players[0].this_players_time_stamp = uVar13;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x2f:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    FUN_00497860((tNet_message*)pbVar18, pMessage);
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x30:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if ((*(uint *)(pbVar18 + 4) < gProgram_state.track_spec.count_non_cars) &&
       (iVar12 = gProgram_state.track_spec.non_car_list[*(ushort *)(pbVar18 + 2)], iVar12 != 0)) {
      iVar7 = *(int *)(iVar12 + 0x5c);
      if (iVar7 == 0) {
LAB_004a41c3:
        cVar1 = *(char *)(*(int *)(iVar12 + 0x14) + 1);
        if (('/' < cVar1) && (cVar1 < ':')) {
          DoPullActorFromWorld(iVar12);
          iVar7 = *(int *)(iVar12 + 0x5c);
        }
        if (iVar7 == 0) goto LAB_004a41f3;
      }
      else {
        if ((*(int *)(iVar7 + 0xc) != 4) ||
           ((uint)*(ushort *)(iVar7 + 0x80) != *(uint *)(pbVar18 + 4))) {
          iVar7 = 0;
        }
        if (iVar7 == 0) goto LAB_004a41c3;
      }
      *(uint *)(iVar7 + 0x100) = *(uint *)(iVar7 + 0x100) & 0xffffff01 | 1;
    }
LAB_004a41f3:
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x31:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xc;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[12].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceiveMineUpdate((tNet_message_chunk_mine_explode*)pbVar18);
    uVar13 = gTimers[12].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[12].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[12].durations[0] + uVar13 * 4) + (iVar7 - gTimers[12].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x32:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xe;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[14].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    if (gProgram_state.racing != 0) {
      uVar13 = (uint)pbVar18[0xc];
      ExpandMatrix34(&gNet_players[uVar13].car->car_master_actor->t.t.mat,
                     (const tCompressed_matrix3*)(pbVar18 + 0xe), *(int*)(pbVar18 + 8));
      ReinitialiseCar(gNet_players[uVar13].car);
      if (uVar13 == gThis_net_player_index) {
        if (gInterface_within_race_mode == 0) {
          FadePaletteDown();
        }
        CancelPendingCunningStunt();
        gProgram_state.credits = gCurrent_net_game->options.starting_credits;
        InitialiseExternalCamera();
      }
      (&gNet_players[0].last_waste_message)[uVar13 * 0x35] = 0;
      (&gNet_players[0].wasteage_attributed)[uVar13 * 0x35] = 0;
    }
    uVar13 = gTimers[14].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[14].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[14].durations[0] + uVar13 * 4) + (iVar7 - gTimers[14].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
LAB_004a4474:
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    break;
  case 0x33:
    *(undefined4 *)(&gTimers_stack + iVar7 * 4) = 0xa;
    if (iVar7 == 0) {
      iVar19 = iVar12 * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + iVar12 * 7;
    }
    else {
      iVar19 = gTimers_stack[iVar7 - 1] * 0x1c;
      iVar12 = (uint)(byte)(&gTimers[0].index)[iVar19] + gTimers_stack[iVar7 - 1] * 7;
    }
    iVar7 = PDGetMicroseconds();
    (&gTimers[0].durations[0])[iVar12] = (&gTimers[0].durations[0])[iVar12] + (iVar7 - *(int *)(&gTimers[0].start_time + iVar19));
    gTimers[10].start_time = PDGetMicroseconds();
    gTimers_stack_size = gTimers_stack_size + 1;
    ReceivedCrushDetach(pbVar18);
    uVar13 = gTimers[10].index & 0xff;
    iVar7 = PDGetMicroseconds();
    iVar12 = gTimers_stack_size;
    *(int *)(&gTimers[10].durations[0] + uVar13 * 4) =
         *(int *)(&gTimers[10].durations[0] + uVar13 * 4) + (iVar7 - gTimers[10].start_time);
    if (iVar12 < 2) goto LAB_004a449c;
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
  }
  iVar7 = gTimers_stack_size + -1;
  gTimers_stack_size = iVar7;
  goto switchD_004a051d_caseD_1a;
  while( true ) {
    iVar9 = iVar9 + 1;
    piVar6 = piVar6 + 0x35;
    if ((int)gNumber_of_net_players <= iVar9) break;
LAB_004a1534:
    if (*piVar6 == iVar3) {
      PDNetSendMessageToAddress(iVar5,iVar12,&gNet_players + iVar9 * 0x35);
      break;
    }
  }
LAB_004a1560:
  iVar12 = 0;
  (&gNet_players[0].field_0xcc)[iVar19 * 0x35] = 0;
LAB_004a156c:
  if (iVar12 == 0) {
LAB_004a1570:
    iVar12 = NetAllocateMessage(0x200);
    (&gNet_players[0].field_0xcc)[iVar19 * 0x35] = iVar12;
    *(undefined2 *)(iVar12 + 0x16) = 0x18;
    *(undefined2 *)(iVar12 + 0x14) = 0;
  }
  uVar2 = *(ushort *)(iVar12 + 0x16);
  *(ushort *)(iVar12 + 0x16) = uVar2 + (short)iVar7;
  puVar10 = (undefined1 *)(iVar12 + (uint)uVar2);
  *(short *)(iVar12 + 0x14) = *(short *)(iVar12 + 0x14) + 1;
  puVar10[1] = 4;
  *puVar10 = (char)iVar7;
LAB_004a15b2:
  if (puVar10 == (undefined1 *)0x0) {
    iVar12 = NetGetContentsSize(4,0);
    iVar7 = NetAllocateMessage(iVar12 + 0x18);
    if (iVar7 != 0) {
      *(undefined2 *)(iVar7 + 0x14) = 1;
      *(short *)(iVar7 + 0x16) = (short)(iVar12 + 0x18);
      *(undefined1 *)(iVar7 + 0x19) = 4;
    }
    *(undefined2 *)(iVar7 + 0x1a) = *(undefined2 *)(pbVar18 + 2);
    iVar12 = gCurrent_net_game;
    if ((gNet_mode != 0) || (gJoin_list_mode != 0)) {
      *(int *)(iVar7 + 8) = gLocal_net_ID;
      uVar8 = PDGetTotalTime();
      *(undefined4 *)(iVar7 + 0x10) = uVar8;
      PDNetSendMessageToAddress(iVar12,iVar7,(undefined4)pSender_address);
    }
  }
  else {
    *(undefined2 *)(puVar10 + 2) = *(undefined2 *)(pbVar18 + 2);
  }
  uVar13 = gTimers[15].index & 0xff;
  iVar7 = PDGetMicroseconds();
  iVar12 = gTimers_stack_size;
  *(int *)(&gTimers[15].durations[0] + uVar13 * 4) =
       *(int *)(&gTimers[15].durations[0] + uVar13 * 4) + (iVar7 - gTimers[15].start_time);
  if (iVar12 < 2) {
LAB_004a2e18:
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_max_index * 0x1c) = uVar8;
    iVar7 = gTimers_stack_size + -1;
    gTimers_stack_size = iVar7;
  }
  else {
    uVar8 = PDGetMicroseconds();
    iVar12 = gTimers_max_index;
    iVar7 = gTimers_stack_size + -1;
    *(undefined4 *)(&gTimers[0].start_time + gTimers_stack[gTimers_stack_size - 1] * 0x1c) = uVar8
    ;
    gTimers_stack_size = iVar7;
  }
switchD_004a051d_caseD_1a:
  pbVar18 = pbVar18 + *pbVar18;
  local_104 = local_104 + 1;
  if ((int)(uint)*(ushort *)((int)pMessage + 0x14) <= local_104) {
    return;
  }
  goto LAB_004a0483;

}

#undef true
#undef false

void C2_HOOK_FASTCALL CheckCheckSum(tNet_message* pMessage) {
}

void C2_HOOK_FASTCALL StatReceivePacket(void) {
}

void C2_HOOK_FASTCALL NetReallyDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    pMessage->contents.raw.header.type = eNetMsg_none;
}

// C2PORT-NONMATCHING: best 39.13% | difftest PASS 2000 trials (spec difftest/specs/004a5b50_NetGuaranteedSendMessageToAllPlayers.json) | reason register and branch codegen
// FUNCTION: CARMA2_HW 0x004a5b50
int C2_HOOK_FASTCALL NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, undefined4 *pArg3) {
    int i;
    int result;

    if (gNumber_of_net_players == 1) {
        pMessage->contents.raw.header.type = 0xff;
        return 0;
    }
    result = 0;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index) {
            result |= SendGuaranteedMessageToPlayer(pDetails, pMessage, gNet_players[i].ID, (undefined4)pArg3);
        }
    }
    return result;
}

// FUNCTION: CARMA2_HW 0x0049eea0
int C2_HOOK_FASTCALL NetReallySendMessageToPlayer(tNet_game_details* pNet_game, tNet_message* pMessage, tPlayer_ID pPlayer_id) {
    int i;

    if (gNet_mode == eNet_mode_none) {
        return -1;
    }

    pMessage->header.field_0x8 = gLocal_net_ID;
    pMessage->header.field_0x10 = PDGetTotalTime();

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pPlayer_id) {
            return PDNetSendMessageToAddress(pNet_game, pMessage, &gNet_players[i].pd_net_info);
        }
    }
    return -3;
}

// FUNCTION: CARMA2_HW 0x0049ef20
int C2_HOOK_FASTCALL NetSendMessageToPlayer(tNet_game_details* pNet_game, tNet_message* pMessage, tPlayer_ID pPlayer_id) {
    int i;
    int j;
    int offset;
    int contents_size;
    tPlayer_ID* player_id;
    tPlayer_ID* scan_player_id;
    tNet_message* message;
    tNet_game_details* game;
    tPlayer_ID target_id;
    tU8* destination;

    i = 0;
    player_id = &gNet_players[0].ID;
    while (i < gNumber_of_net_players) {
        if (*player_id == pPlayer_id) {
            contents_size = pMessage->header.field_0x16 - sizeof(tNet_message_header);
            pMessage->contents.raw.header.size = (tU8)contents_size;
            if ((tNet_game_player_info*)((tU8*)player_id - 0x24) == &gNet_players[gThis_net_player_index]) {
                PDFatalError("Trying to send network message to self");
            }

            message = ((tNet_game_player_info*)((tU8*)player_id - 0x24))->field_0xcc;
            if (message != NULL && message->header.field_0x16 + contents_size > 0x200) {
                target_id = *player_id;
                game = gCurrent_net_game;
                if (gNet_mode != eNet_mode_none) {
                    message->header.field_0x8 = gLocal_net_ID;
                    message->header.field_0x10 = PDGetTotalTime();
                    j = 0;
                    scan_player_id = &gNet_players[0].ID;
                    while (j < gNumber_of_net_players && *scan_player_id != target_id) {
                        j++;
                        scan_player_id += 0x35;
                    }
                    if (j < gNumber_of_net_players) {
                        PDNetSendMessageToAddress(game, message, &gNet_players[j].pd_net_info);
                    }
                }
                ((tNet_game_player_info*)((tU8*)player_id - 0x24))->field_0xcc = NULL;
                message = NULL;
            }
            if (message == NULL) {
                ((tNet_game_player_info*)((tU8*)player_id - 0x24))->field_0xcc = NetAllocateMessage(0x200);
                message = ((tNet_game_player_info*)((tU8*)player_id - 0x24))->field_0xcc;
                message->header.field_0x16 = sizeof(tNet_message_header);
                message->header.field_0x14 = 0;
            }
            offset = message->header.field_0x16;
            destination = (tU8*)message + offset;
            message->header.field_0x14++;
            message->header.field_0x16 += (tU16)contents_size;
            if (destination != NULL) {
                memcpy(destination, (tU8*)pMessage + sizeof(tNet_message_header), contents_size);
                if (pMessage->header.field_0x14 != 0xff) {
                    pMessage->contents.raw.header.type = 0xff;
                }
                return 0;
            }
        }
        i++;
        player_id += 0x35;
    }
    if (pMessage->header.field_0x14 != 0xff) {
        pMessage->contents.raw.header.type = 0xff;
    }
    return -3;
}

// FUNCTION: CARMA2_HW 0x0049f0d0
int C2_HOOK_FASTCALL FUN_0049f0d0(tNet_game_details* pNet_game, tNet_message* pMessage) {

    if (gNet_mode != eNet_mode_none) {
        pMessage->header.field_0x8 = gLocal_net_ID;
        pMessage->header.field_0x10 = PDGetTotalTime();
        return PDNetSendMessageToAddress(pNet_game, pMessage, pNet_game);
    }
    return -3;
}

// C2PORT-NONMATCHING: best 89.08% | difftest PASS 2000 trials (spec difftest/specs/0049f2c0_NetSendMessageToAllPlayers.json) | reason vc5-player-pointer-bias-regalloc
// FUNCTION: CARMA2_HW 0x0049f2c0
int C2_HOOK_FASTCALL NetSendMessageToAllPlayers(tNet_game_details* pNet_game, tNet_message* pMessage) {
    int i;
    int j;
    int contents_size;
    tPlayer_ID player_id;
    tNet_game_details* game;
    tNet_game_player_info* player;
    tNet_message* message;
    tU8* destination;

    contents_size = pMessage->header.field_0x16 - sizeof(tNet_message_header);
    pMessage->contents.raw.header.size = (tU8)contents_size;
    i = 0;
    player = gNet_players;
    if (0 < gNumber_of_net_players) {
        do {
            if (i != gThis_net_player_index) {
                if (&player->field_0xcc == &gNet_players[gThis_net_player_index].field_0xcc) {
                    PDFatalError("Trying to send network message to self");
                }
                message = player->field_0xcc;
                if (message != NULL && message->header.field_0x16 + contents_size > 0x200) {
                    player_id = player->ID;
                    game = gCurrent_net_game;
                    if (gNet_mode != eNet_mode_none) {
                        message->header.field_0x8 = gLocal_net_ID;
                        message->header.field_0x10 = PDGetTotalTime();
                        for (j = 0; j < gNumber_of_net_players; j++) {
                            if (gNet_players[j].ID == player_id) {
                                PDNetSendMessageToAddress(game, message, &gNet_players[j].pd_net_info);
                                break;
                            }
                        }
                    }
                    player->field_0xcc = NULL;
                    message = NULL;
                }
                if (message == NULL) {
                    message = NetAllocateMessage(0x200);
                    player->field_0xcc = message;
                    message->header.field_0x16 = sizeof(tNet_message_header);
                    message->header.field_0x14 = 0;
                }
                destination = (tU8*)message + message->header.field_0x16;
                message->header.field_0x14++;
                message->header.field_0x16 += contents_size;
                if (destination != NULL) {
                    memcpy(destination, &pMessage->contents, contents_size);
                }
            }
            i++;
            player++;
        } while (i < gNumber_of_net_players);
    }
    if (pMessage->header.field_0x14 != 0xff) {
        pMessage->contents.raw.header.type = 0xff;
    }
    return 0;
}
// FUNCTION: CARMA2_HW 0x004a5140
void* C2_HOOK_FASTCALL NetGetToPlayerContentsSize(tNet_game_player_info* pNet_player, int pSize) {
    tNet_message* message;
    int offset;

    C2_HOOK_BUG_ON(sizeof(tNet_message_header) != 0x18);

    if (pNet_player == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    message = pNet_player->field_0xcc;
    if (pNet_player->field_0xcc != NULL && pNet_player->field_0xcc->header.field_0x16 + pSize > 512) {
        NetReallySendMessageToPlayer(gCurrent_net_game, pNet_player->field_0xcc, pNet_player->ID);
        pNet_player->field_0xcc = message = NULL;
    }
    if (message == NULL) {
        pNet_player->field_0xcc = message = NetAllocateMessage(512);
        message->header.field_0x16 = (tU16)sizeof(tNet_message_header);
        message->header.field_0x14 = 0;
    }
    offset = message->header.field_0x16;
    message->header.field_0x16 += (tU16)pSize;
    message->header.field_0x14 += 1;
    return (tU8*)message + offset;
}

// FUNCTION: CARMA2_HW 0x004a5b30
tPlayer_status C2_HOOK_FASTCALL NetGetPlayerStatus(void) {

    return gNet_players[gThis_net_player_index].player_status;
}

// FUNCTION: CARMA2_HW 0x0049f9c0
tNet_message_chunk* C2_HOOK_FASTCALL NetStartBroadcastContents(tNet_message_type pType, tS32 pSize_decider) {
    int i;
    int size;
    int offset;
    tNet_message* message;
    tNet_message_chunk* chunk;
    tNet_game_player_info* player;
    tPlayer_ID key;

    if (gINT_00659d34 >= 0) {
        PDFatalError("Broadcast stacks messed up");
    }
    i = 0;
    if (gNumber_of_net_players <= 0) {
        return NULL;
    }
    for (; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index) {
            goto found_player;
        }
    }
    return NULL;

found_player:
    gINT_00659d34 = i;
    key = gNet_players[i].ID;
    size = NetGetContentsSize(pType, pSize_decider);
    if (gNumber_of_net_players <= 0) {
        return NULL;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == key) {
            goto found_target;
        }
    }
    return NULL;

found_target:
    player = &gNet_players[i];
    if (player == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    message = player->field_0xcc;
    if (message != NULL && size + message->header.field_0x16 > 0x200) {
        tPlayer_ID player_id = player->ID;
        tNet_game_details* game = gCurrent_net_game;

        if (gNet_mode != eNet_mode_none) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == player_id) {
                    goto send_message;
                }
            }
            goto clear_message;

send_message:
            PDNetSendMessageToAddress(game, message, &gNet_players[i]);
        }
clear_message:
        player->field_0xcc = message = NULL;
    }
    if (message == NULL) {
        player->field_0xcc = message = NetAllocateMessage(0x200);
        message->header.field_0x16 = (tU16)sizeof(tNet_message_header);
        message->header.field_0x14 = 0;
    }
    offset = message->header.field_0x16;
    chunk = (tNet_message_chunk*)((tU8*)message + offset);
    message->header.field_0x16 += (tU16)size;
    message->header.field_0x14 += 1;
    chunk->raw.header.type = pType;
    chunk->raw.header.size = (tU8)size;
    return chunk;
}

// FUNCTION: CARMA2_HW 0x0049fb70
void C2_HOOK_FASTCALL NetBroadcastContents(tNet_message_chunk* pMessage) {
    tNet_message* message;
    int size;
    tU8* dest;
    int i;
    int j;
    tPlayer_ID player_id;
    tNet_game_details* game;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index && i != gINT_00659d34) {
            size = pMessage->raw.header.size;
            if (&gNet_players[i] == &gNet_players[gThis_net_player_index]) {
                PDFatalError("Trying to send network message to self");
            }
            message = gNet_players[i].field_0xcc;
            if (message != NULL && message->header.field_0x16 + size > 0x200) {
                player_id = gNet_players[i].ID;
                game = gCurrent_net_game;
                if (gNet_mode != eNet_mode_none) {
                    message->header.field_0x8 = gLocal_net_ID;
                    message->header.field_0x10 = PDGetTotalTime();
                    for (j = 0; j < gNumber_of_net_players; j++) {
                        if (gNet_players[j].ID == player_id) {
                            goto send_message;
                        }
                    }
                    goto clear_message;

send_message:
                    PDNetSendMessageToAddress(game, message, &gNet_players[j]);
                }
clear_message:
                gNet_players[i].field_0xcc = message = NULL;
            }
            if (message == NULL) {
                message = NetAllocateMessage(0x200);
                gNet_players[i].field_0xcc = message;
                message->header.field_0x16 = (tU16)sizeof(tNet_message_header);
                message->header.field_0x14 = 0;
            }
            dest = (tU8*)message;
            dest += message->header.field_0x16;
            message->header.field_0x16 = size + message->header.field_0x16;
            message->header.field_0x14 += 1;
            memcpy(dest, pMessage, pMessage->raw.header.size);
        }
    }
    gINT_00659d34 = -1;
}

// FUNCTION: CARMA2_HW 0x0049d780
void C2_HOOK_FASTCALL NetSendHeadupToEverybody(const char* pMessage) {
    tNet_message_chunk* chunk;
    tNet_message* message;
    tNet_game_player_info* player;
    tPlayer_ID key;
    tPlayer_ID player_id;
    tNet_game_details* game;
    tU8* dest;
    int i;
    int j;
    int size;
    int offset;

    if (gNet_mode == eNet_mode_none) {
        return;
    }
    if (gProgram_state.racing) {
        NewTextHeadupSlot(4, 0, 3000, -4, pMessage);
    }
    if (gINT_00659d34 >= 0) {
        PDFatalError("Broadcast stacks messed up");
    }
    i = 0;
    if (gNumber_of_net_players <= 0) {
        chunk = NULL;
    } else {
        for (; i < gNumber_of_net_players; i++) {
            if (i != gThis_net_player_index) {
                goto found_player;
            }
        }
        chunk = NULL;
        goto finish_start;
    }
found_player:
    gINT_00659d34 = i;
    key = gNet_players[i].ID;
    size = NetGetContentsSize(0xd, 0);
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == key) {
            goto found_target;
        }
    }
    chunk = NULL;
    goto finish_start;
found_target:
    player = &gNet_players[i];
    if (player == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    message = player->field_0xcc;
    if (message != NULL && size + message->header.field_0x16 > 0x200) {
        player_id = player->ID;
        game = gCurrent_net_game;
        if (gNet_mode != eNet_mode_none) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == player_id) {
                    goto send_first_message;
                }
            }
            goto clear_first_message;
send_first_message:
            PDNetSendMessageToAddress(game, message, &gNet_players[i]);
        }
clear_first_message:
        player->field_0xcc = message = NULL;
    }
    if (message == NULL) {
        player->field_0xcc = message = NetAllocateMessage(0x200);
        message->header.field_0x16 = sizeof(tNet_message_header);
        message->header.field_0x14 = 0;
    }
    offset = message->header.field_0x16;
    chunk = (tNet_message_chunk*)((tU8*)message + offset);
    message->header.field_0x16 += (tU16)size;
    message->header.field_0x14 += 1;
    chunk->raw.header.type = 0xd;
    chunk->raw.header.size = (tU8)size;
finish_start:
    if (chunk == NULL) {
        return;
    }
    strcpy((char*)chunk + sizeof(tNet_message_chunk_header), pMessage);

    player = gNet_players;
    for (i = 0; i < gNumber_of_net_players; i++, player++) {
        if (i != gThis_net_player_index && i != gINT_00659d34) {
            size = chunk->raw.header.size;
            if (player == &gNet_players[gThis_net_player_index]) {
                PDFatalError("Trying to send network message to self");
            }
            message = player->field_0xcc;
            if (message != NULL && message->header.field_0x16 + size > 0x200) {
                player_id = player->ID;
                game = gCurrent_net_game;
                if (gNet_mode != eNet_mode_none) {
                    message->header.field_0x8 = gLocal_net_ID;
                    message->header.field_0x10 = PDGetTotalTime();
                    for (j = 0; j < gNumber_of_net_players; j++) {
                        if (gNet_players[j].ID == player_id) {
                            goto send_next_message;
                        }
                    }
                    goto clear_next_message;
send_next_message:
                    PDNetSendMessageToAddress(game, message, &gNet_players[j]);
                }
clear_next_message:
                player->field_0xcc = message = NULL;
            }
            if (message == NULL) {
                message = NetAllocateMessage(0x200);
                player->field_0xcc = message;
                message->header.field_0x16 = sizeof(tNet_message_header);
                message->header.field_0x14 = 0;
            }
            dest = (tU8*)message;
            dest += message->header.field_0x16;
            message->header.field_0x16 += (tU16)size;
            message->header.field_0x14 += 1;
            memcpy(dest, chunk, chunk->raw.header.size);
        }
    }
    gINT_00659d34 = -1;
}

// FUNCTION: CARMA2_HW 0x004a5bc0
int C2_HOOK_FASTCALL NetGuaranteedSendMessageToEverybody(tNet_game_details* pNet_game, tNet_message* pMessage, undefined4 pArg3) {
    int i;
    int result;

    pMessage->header.field_0x8 = gLocal_net_ID;
    pMessage->header.field_0x10 = PDGetTotalTime();
    if (pMessage->header.field_0x14 != 0xff) {
        PDFatalError("Guarantee message not built proper");
    }
    pMessage->header.field_0x14 = 1;
    ReceivedMessage(pMessage, &gNet_players[gThis_net_player_index], GetRaceTime());
    pMessage->header.field_0x14 = 0xff;
    if (gNumber_of_net_players == 1) {
        ((tU8*)pMessage)[0x19] = 0xff;
        return 0;
    }
    result = 0;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index) {
            result |= SendGuaranteedMessageToPlayer(pNet_game, pMessage, gNet_players[i].ID, pArg3);
        }
    }
    return result;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004a5e20
int C2_HOOK_FASTCALL SendGuaranteedMessageToPlayer(tNet_game_details* pNet_game, tNet_message* pMessage, undefined4 pPlayer, undefined4 pArg4) {
    int i;
    int idx;
    tNet_game_player_info* target;
    tNet_message* stack;
    tU8* src;
    tU8* dst;
    int total;
    int old_size;
    tU8* p;
    int k;

    if (pMessage->header.field_0x14 != 0xff) {
        PDFatalError("Guaranteed message not built properly");
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == (tPlayer_ID)pPlayer) {
            goto found;
        }
    }
    return -1;

found:
    idx = i;
    if ((tPlayer_ID)pPlayer == gLocal_net_ID) {
        pMessage->header.field_0x8 = gLocal_net_ID;
        pMessage->header.field_0x10 = PDGetTotalTime();
        pMessage->header.field_0x14 = 1;
        ReceivedMessage(pMessage, &gNet_players[idx], GetRaceTime());
        if (pMessage->header.field_0x14 != 0xff) {
            ((tU8*)pMessage)[0x19] = 0xff;
        }
        return 0;
    }
    AddToGuaranteeList(pMessage, NULL, (tPlayer_ID)pPlayer, (void*)pArg4);
    src = (tU8*)pMessage + sizeof(tNet_message_header);
    total = 0;
    p = src;
    for (k = 0; k < 2; k++) {
        total += *p;
        p += *p;
    }
    target = &gNet_players[idx];
    if (target == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    stack = target->field_0xcc;
    if (stack != NULL && stack->header.field_0x16 + total > 0x200) {
        tPlayer_ID tid = target->ID;
        tNet_game_details* game = gCurrent_net_game;

        if (gNet_mode != eNet_mode_none) {
            stack->header.field_0x8 = gLocal_net_ID;
            stack->header.field_0x10 = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == tid) {
                    goto send_stack;
                }
            }
            goto clear_stack;

send_stack:
            PDNetSendMessageToAddress(game, stack, &gNet_players[i]);
        }
clear_stack:
        target->field_0xcc = stack = NULL;
    }
    if (stack == NULL) {
        stack = NetAllocateMessage(0x200);
        target->field_0xcc = stack;
        stack->header.field_0x16 = sizeof(tNet_message_header);
        stack->header.field_0x14 = 0;
    }
    old_size = stack->header.field_0x16;
    dst = (tU8*)stack + old_size;
    stack->header.field_0x14++;
    stack->header.field_0x16 = (tU16)(old_size + total);
    target->field_0xcc->header.field_0x14++;
    memcpy(dst, src, total);
    return 0;
}
#pragma auto_inline(on)
// FUNCTION: CARMA2_HW 0x0049d210
int C2_HOOK_FASTCALL NetInitialise(void) {
    int i;

    InitAbuseomatic();
    gNet_service_disable = 0;
    gIn_net_service = 0;
    gMessage_header_size = PDNetGetHeaderSize();
    gOnly_receive_guarantee_replies = 0;
    gMin_messages = BrMemAllocate(MIN_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMin_message)), kMem_net_min_messages);
    gMid_messages = BrMemAllocate(MID_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMid_message)), kMem_net_mid_messages);
    gMax_messages = BrMemAllocate(MAX_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMax_message)), kMem_net_max_messages);

    for (i = 0; i < MIN_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)((br_uint_8*)&gMin_messages[i] + gMessage_header_size))->contents.raw.header.type = eNetMsg_none;
    }
    for (i = 0; i < MID_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)((br_uint_8*)&gMid_messages[i] + gMessage_header_size))->contents.raw.header.type = eNetMsg_none;
    }
    for (i = 0; i < MAX_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)((br_uint_8*)&gMax_messages[i] + gMessage_header_size))->contents.raw.header.type = eNetMsg_none;
    }
    gNet_initialised = PDNetInitialise() == 0;
    if (gNet_initialised) {
        InitNetHeadups();
    }
    GenerateItFoxShadeTable();
    gDont_allow_joiners = 0;
    gUINT_0074a690 = PDGetTotalTime();
    gUINT_0074a718 = PDGetTotalTime();
    gNext_guarantee = 0;
    gDynamic_messages = NULL;
    gGuarantee_number = ((tU16)PDGetTotalTime() & 0x3ff) + 1;
    return !gNet_initialised;
}

int C2_HOOK_FASTCALL NetShutdown(void) {
    int err;

    err = PDNetShutdown();
    DisposeAbuseomatic();
    BrMemFree(gMin_messages);
    BrMemFree(gMid_messages);
    BrMemFree(gMax_messages);
    DisposeExcessMemory();
    DisposeNetHeadups();
    return err;
}

// FUNCTION: CARMA2_HW 0x0049d370
void C2_HOOK_FASTCALL ShutdownNetIfRequired(void) {

    if (gNet_initialised) {
        NetShutdown();
        gNet_initialised = 0;
    }
}

// DisableNetService

// ReenableNetService

// PermitNetServiceReentrancy

// HaltNetServiceReentrancy

// NetSendHeadupToAllPlayers

// NetSendHeadupToEverybody

// NetSendHeadupToPlayer

// InitialisePlayerStati

void C2_HOOK_FASTCALL LeaveTempGame(void) {

    if (gCurrent_join_poll_game != NULL) {
        NetLeaveGameLowLevel();
    }
    gTime_for_next_one = 1;
    gCurrent_join_poll_game = NULL;
}

void C2_HOOK_FASTCALL DisposeCurrentJoinPollGame(void) {

    if (gCurrent_join_poll_game != NULL) {
        BrMemFree(gCurrent_join_poll_game);
        gCurrent_join_poll_game = NULL;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049ec70
void C2_HOOK_FASTCALL DoNextJoinPoll(void) {

    if (gTime_for_next_one) {
        gCurrent_join_poll_game = NetAllocatePIDGameDetails();
        if (gCurrent_join_poll_game == NULL) {
            return;
        }
        if (PDNetGetNextJoinGame(gCurrent_join_poll_game, gJoin_poll_index)) {
            if (!NetJoinGameLowLevel(gCurrent_join_poll_game, "!TEMP!")) {
                tNet_message* message;
                gTime_for_next_one = 0;
                message = NetBuildMessage(0, 0);
                NetSendMessageToAddress(gCurrent_join_poll_game, message, &gCurrent_join_poll_game->pd_net_info);
                gBastard_has_answered = 0;
                gAsk_time = PDGetTotalTime();
            } else {
                DisposeCurrentJoinPollGame();
            }
            gJoin_poll_index += 1;
            return;
        } else {
            gJoin_poll_index = 0;
            DisposeCurrentJoinPollGame();
            return;
        }
    } else {
        if (gBastard_has_answered) {
            gAdd_proc(gCurrent_join_poll_game);
            LeaveTempGame();
        } else {
            if (PDGetTotalTime() - gAsk_time > 10000) {
                LeaveTempGame();
                DisposeCurrentJoinPollGame();
            }
        }
    }
}
#pragma auto_inline(on)

// NetStartProducingJoinList

// NetEndJoinList

// NetDisposePIDGameInfo

// NetDisposeGameDetails

tNet_game_details* C2_HOOK_FASTCALL NetAllocatePIDGameDetails(void) {

    C2_HOOK_BUG_ON(sizeof(tNet_game_details) != 0x78);

    return BrMemAllocate(sizeof(tNet_game_details), kMem_net_pid_details);
}

void C2_HOOK_FASTCALL NetLeaveGameLowLevel(void) {

    if (gNet_mode == eNet_mode_host) {
        PDNetHostFinishGame();
    } else {
        PDNetLeaveGame();
    }
}

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x0049dc90
void C2_HOOK_FASTCALL NetLeaveGame(tNet_game_details* pNet_game) {
    tNet_message* message;
    tNet_message_chunk* chunk;
    int size_a;
    int size_b;
    int size_c;
    int total_size;
    const char* suffix;
    int i;
    int was_in_service;
    int result;
    tU32 start_time;
    char text[256];

    dr_dprintf("NetLeaveGame() START");
    if (gNet_mode != eNet_mode_none) {
        gOnly_receive_guarantee_replies = 1;
        if (gNet_mode == eNet_mode_host) {
            dr_dprintf("NetLeaveGame() Point 1");
            gDont_allow_joiners = 1;
            gLast_flush_message = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].field_0xcc != NULL) {
                    NetReallySendMessageToPlayer(gCurrent_net_game, gNet_players[i].field_0xcc, gNet_players[i].ID);
                    gNet_players[i].field_0xcc = NULL;
                }
            }
            dr_dprintf("NetLeaveGame() Point 2");
            size_a = NetGetContentsSize(9, 0);
            size_b = NetGetContentsSize(5, 0);
            size_c = NetGetContentsSize(9, 0);
            total_size = size_c + size_b + sizeof(tNet_message_header);
            message = NetAllocateMessage(total_size);
            if (message != NULL) {
                message->contents.raw.header.size = (tU8)size_a;
                size_a &= 0xff;
                message->header.field_0x14 = 0xff;
                message->header.field_0x16 = (tU16)total_size;
                message->contents.raw.header.type = 9;
                ((tU8*)message + size_a)[0x19] = 5;
                ((tU8*)message)[0x18 + message->contents.raw.header.size] = (tU8)size_b;
            }
            dr_dprintf("NetLeaveGame() Point 3");
            if (gIn_net_service != 0) {
                gIn_net_service = 0;
                was_in_service = 1;
            } else {
                was_in_service = 0;
            }
            dr_dprintf("NetLeaveGame() Point 4");
            result = 0;
            if (gNumber_of_net_players == 1) {
                message->contents.raw.header.type = 0xff;
            } else {
                for (i = 0; i < gNumber_of_net_players; i++) {
                    if (i != gThis_net_player_index) {
                        result |= SendGuaranteedMessageToPlayer(pNet_game, message, gNet_players[i].ID, 0);
                    }
                }
            }
            dr_dprintf("NetLeaveGame() Point 5");
            if (was_in_service) {
                gIn_net_service = 1;
            }
            dr_dprintf("NetLeaveGame() Point 6");
        } else if (gINT_0068d968 == 0) {
            size_a = NetGetContentsSize(8, 0);
            size_b = NetGetContentsSize(5, 0);
            size_c = NetGetContentsSize(8, 0);
            total_size = size_c + size_b + sizeof(tNet_message_header);
            message = NetAllocateMessage(total_size);
            if (message != NULL) {
                message->contents.raw.header.size = (tU8)size_a;
                size_a &= 0xff;
                message->header.field_0x14 = 0xff;
                message->header.field_0x16 = (tU16)total_size;
                message->contents.raw.header.type = 8;
                ((tU8*)message + size_a)[0x19] = 5;
                ((tU8*)message)[0x18 + message->contents.raw.header.size] = (tU8)size_b;
            }
            SendGuaranteedMessageToPlayer(pNet_game, message, gNet_players[0].ID, 0);
            strcpy(text, gProgram_state.player_name);
            suffix = GetMiscString(0x4a);
            strcat(text, " ");
            strcat(text, suffix);
            if (gNet_mode != eNet_mode_none) {
                chunk = NetStartBroadcastContents(0xd, 0);
                if (chunk != NULL) {
                    strcpy((char*)chunk + sizeof(tNet_message_chunk_header), text);
                    NetBroadcastContents(chunk);
                }
            }
        }
        dr_dprintf("NetLeaveGame() Point 7");
        for (i = 0; i < gNumber_of_net_players; i++) {
            FUN_0049e100(i);
        }
        dr_dprintf("NetLeaveGame() Point 8");
        ClearOutStorageSpace(&gOur_car_storage_space);
        dr_dprintf("NetLeaveGame() Point 9");
        ClearOutStorageSpace(&gNet_cars_storage_space);
        dr_dprintf("NetLeaveGame() Point 10");
        if (gIn_net_service != 0) {
            gIn_net_service = 0;
            was_in_service = 1;
        } else {
            was_in_service = 0;
        }
        gLast_flush_message = PDGetTotalTime();
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].field_0xcc != NULL) {
                NetReallySendMessageToPlayer(gCurrent_net_game, gNet_players[i].field_0xcc, gNet_players[i].ID);
                gNet_players[i].field_0xcc = NULL;
            }
        }
        start_time = PDGetTotalTime();
        while (gNext_guarantee != 0 && PDGetTotalTime() - start_time < 5000U) {
            NetService(0);
        }
        if (gNet_mode == eNet_mode_host) {
            ((void (C2_HOOK_FASTCALL *)(tNet_game_details*))PDNetHostFinishGame)(gCurrent_net_game);
        } else {
            ((void (C2_HOOK_FASTCALL *)(tNet_game_details*))PDNetLeaveGame)(gCurrent_net_game);
        }
        if (was_in_service) {
            gIn_net_service = 1;
        }
        gCurrent_net_game = NULL;
        gNet_mode = eNet_mode_none;
        gNumber_of_net_players = 0;
        gUNK_0075bc24 = 2;
        gOnly_receive_guarantee_replies = 0;
        dr_dprintf("NetLeaveGame() Point 11");
    }
}
#pragma inline_depth()

// NetSetPlayerSystemInfo

// FillInThisPlayer

// LoadCarN

// DisposeCarN

// PlayerHasLeft

// RemoveReferencesToCar

// NetPlayersChanged

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049e100
void C2_HOOK_FASTCALL FUN_0049e100(int pPlayer_index) {
    int i;
    int j;

    if (gNet_players[pPlayer_index].car != NULL) {
        for (i = 0; i < gCurrent_race.number_of_racers; i++) {
            if (gCurrent_race.opponent_list[i].car_spec == gNet_players[pPlayer_index].car) {
                gCurrent_race.number_of_racers--;
                for (j = i; j < gCurrent_race.number_of_racers; j++) {
                    gCurrent_race.opponent_list[j] = gCurrent_race.opponent_list[j + 1];
                }
            }
        }
        PHILRemoveObject(gNet_players[pPlayer_index].car->collision_info);
        if (gProgram_state.racing != 0) {
            RemoveCarFromEffects(gNet_players[pPlayer_index].car);
        }
        DisposeCar(gNet_players[pPlayer_index].car, gNet_players[pPlayer_index].car_index);
        if (pPlayer_index != gThis_net_player_index) {
            BrMemFree(gNet_players[pPlayer_index].car);
            gNet_players[pPlayer_index].car = NULL;
        }
        if (gAction_replay_mode != 0) {
            ToggleReplay(NULL, NULL);
        }
        ARResetPiping();
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 55.83% | difftest PASS 2000 trials (spec difftest/specs/0049e450_NetPlayersChanged.json) | reason vc5-register-allocation
// FUNCTION: CARMA2_HW 0x0049e450
void C2_HOOK_FASTCALL NetPlayersChanged(int pPlayer_count, tNet_game_player_info* pPlayers) {
    int i;
    int j;
    int k;
    int found;
    int opponent_index;
    tCar_spec* car;
    tCar_spec* other_car;
    tNet_game_player_info* player;
    tPlayer_ID it_player_id;

    gNet_service_disable = 1;
    if (gCurrent_net_game->type == 7) {
        it_player_id = gNet_players[gIt_or_fox].ID;
    }
    for (i = 0; i < pPlayer_count; i++) {
        player = &pPlayers[i];
        found = 0;
        for (j = 0; j < gNumber_of_net_players; j++) {
            if (player->ID == gNet_players[j].ID) {
                found = 1;
                player->last_waste_message = gNet_players[j].last_waste_message;
                player->player_status = gNet_players[j].player_status;
                player->car_index = gNet_players[j].car_index;
                player->score2 = gNet_players[j].score2;
                player->credits = gNet_players[j].credits;
                player->wasted = gNet_players[j].wasted;
                player->wasteage_attributed = gNet_players[j].wasteage_attributed;
                player->field_0x58 = gNet_players[j].field_0x58;
                player->next_car_index = gNet_players[j].next_car_index;
                player->car = gNet_players[j].car;
                break;
            }
        }
        if (!found) {
            if (player->ID == gLocal_net_ID) {
                car = &gProgram_state.current_car;
                LoadCar(gOpponents[player->car_index].car_file_name,
                    eDriver_local_human,
                    car,
                    player->car_index,
                    player->player_name,
                    &gNet_cars_storage_space);
                player->car = car;
                if (player->player_status < 6) {
                    car->disabled = 1;
                }
            } else {
                car = BrMemAllocate(0x1964, kMem_net_car_spec);
                player->car = car;
                LoadCar(gOpponents[player->car_index].car_file_name,
                    eDriver_net_human,
                    car,
                    player->car_index,
                    player->player_name,
                    &gNet_cars_storage_space);
                InitialiseCar(car);
                if (player->player_status < 6) {
                    car->disabled = 1;
                }
            }
            SetCarStorageTexturingLevel(&gNet_cars_storage_space, GetCarTexturingLevel(), eCTL_full);
            player->opponent_list_index = i;
            gCurrent_race.opponent_list[i].index = -1;
            gCurrent_race.opponent_list[i].net_player_index = i;
            gCurrent_race.opponent_list[i].ranking = IRandomBetween(0, 99);
            gCurrent_race.opponent_list[i].car_spec = player->car;
            gCurrent_race.number_of_racers = pPlayer_count;
            if (gNet_mode == 2) {
                AddPlayerToShapeStatusLists(player->ID, pPlayer_count, pPlayers);
            }
        }
    }

    for (i = 0; i < gNumber_of_net_players; i++) {
        found = 0;
        for (j = 0; j < pPlayer_count; j++) {
            if (gNet_players[i].ID == pPlayers[j].ID) {
                found = 1;
                break;
            }
        }
        if (!found) {
            opponent_index = gNumber_of_net_players;
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (gCurrent_race.opponent_list[j].net_player_index == i) {
                    opponent_index = j;
                    break;
                }
            }
            if (opponent_index < gNumber_of_net_players) {
                for (k = opponent_index; k < gNumber_of_net_players - 1; k++) {
                    gCurrent_race.opponent_list[k] = gCurrent_race.opponent_list[k + 1];
                }
            }
            for (j = 0; j < pPlayer_count; j++) {
                if (pPlayers[j].opponent_list_index > opponent_index) {
                    pPlayers[j].opponent_list_index--;
                }
            }

            car = gNet_players[i].car;
            if (car != NULL) {
                for (j = 0; j < gNumber_of_net_players; j++) {
                    other_car = gNet_players[j].car;
                    if (other_car != NULL) {
                        if (*(tCar_spec**)((tU8*)other_car + 0x1560) == car) {
                            *(tCar_spec**)((tU8*)other_car + 0x1560) = NULL;
                        }
                        if (*(tCar_spec**)((tU8*)other_car + 0x1564) == car) {
                            *(tCar_spec**)((tU8*)other_car + 0x1564) = NULL;
                        }
                        if (*(tCar_spec**)((tU8*)other_car + 0xc8) == car) {
                            *(tCar_spec**)((tU8*)other_car + 0xc8) = NULL;
                        }
                        if (*(tCar_spec**)((tU8*)other_car + 0x13ec) == car) {
                            *(tCar_spec**)((tU8*)other_car + 0x13ec) = NULL;
                        }
                        if (*(tCar_spec**)((tU8*)other_car + 0x13f0) == car) {
                            *(tCar_spec**)((tU8*)other_car + 0x13f0) = NULL;
                        }
                    }
                }
                RemoveFromCloakingList(car);
                PHILRemoveObject(car->collision_info);
                FUN_004a7940(car);
                RemoveExplosionsForCar(car);
                FUN_004ddde0(car);
                RemoveRepulseLinksForCar(car);
            }

            if (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_novelty) {
                if (gNet_players[i].car_index >= 0) {
                    gCar_details[gNet_players[i].car_index].ownership = eCar_owner_none;
                }
                if (gNet_players[i].next_car_index >= 0) {
                    gCar_details[gNet_players[i].next_car_index].ownership = eCar_owner_none;
                }
            }
            FUN_0049e100(i);
            if (gInitialised_grid != 0) {
                for (i = 0; i < pPlayer_count; i++) {
                    opponent_index = pPlayers[i].opponent_list_index;
                    gCurrent_race.opponent_list[opponent_index].net_player_index = i;
                }
            }
            if (gNet_mode == 2) {
                RemovePlayerFromShapeStatusLists(pPlayers[i].ID, pPlayer_count, pPlayers);
            }
        }
    }

    gNumber_of_net_players = pPlayer_count;
    memcpy(gNet_players, pPlayers, pPlayer_count * sizeof(tNet_game_player_info));
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[i].this_players_time_stamp = PDGetTotalTime();
    }
    ForceRebuildActiveCarList();
    if (gCurrent_net_game->type == 7) {
        gIt_or_fox = -1;
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (it_player_id == gNet_players[i].ID) {
                gIt_or_fox = i;
                gNet_service_disable = 0;
                return;
            }
        }
    } else if (gCurrent_net_game->type == 6) {
        FUN_0049a940();
    }
    gNet_service_disable = 0;
}
#pragma auto_inline(on)

// NetHostGame

int C2_HOOK_FASTCALL NetJoinGameLowLevel(tNet_game_details* pGame_details, const char* pName) {

    return PDNetJoinGame(pGame_details, pName);
}

// NetJoinGame

// FUNCTION: CARMA2_HW 0x0049ee20
void C2_HOOK_FASTCALL NetObtainSystemUserName(char* pName, int pMax_length) {

    PDNetObtainSystemUserName(pName, pMax_length);
    pName[9] = '\0';
}

// NetExtractGameID
tU32 C2_HOOK_FASTCALL NetExtractGameID(tNet_game_details* pDetails) {

    return PDNetExtractGameID(pDetails);
}

// NetExtractPlayerID
tPlayer_ID C2_HOOK_FASTCALL NetExtractPlayerID(tNet_game_details* pDetails) {

    return PDNetExtractPlayerID(pDetails);
}

// FUNCTION: CARMA2_HW 0x0049ee50
int C2_HOOK_FASTCALL NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {

    if (gNet_mode == eNet_mode_none && !gJoin_list_mode) {
        return -3;
    }
    pMessage->header.field_0x8 = gLocal_net_ID;
    pMessage->header.field_0x10 = PDGetTotalTime();
    return PDNetSendMessageToAddress(pDetails, pMessage, pAddress);
}

// NetReallySendMessageToPlayer

// NetSendMessageToPlayer

// NetSendMessageToHost

// NetSendMessageToAllPlayers

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 40.99% | difftest PASS 2000 trials (spec difftest/specs/0049f440_NetGetContentsSize.json) | reason jump-table-codegen-and-uninitialized-fallback
// FUNCTION: CARMA2_HW 0x0049f440
int C2_HOOK_FASTCALL NetGetContentsSize(tU8 pType, int pOption) {
    unsigned int size;
    char buf[256];

    switch (pType) {
    case 0x01:
        size = 0x7c;
        break;
    case 0x02:
        size = 0xd8;
        break;
    case 0x03:
        size = 0xe4;
        break;
    case 0x0c:
        size = 0xf6;
        break;
    case 0x05:
        size = pOption + 4;
        break;
    case 0x0d:
        size = 0x82;
        break;
    case 0x1a:
        switch (pOption) {
        case 0:
            size = 0x18;
            break;
        case 1:
            size = 0x24;
            break;
        case 2:
            size = 0x3c;
            break;
        default:
            size = *(int*)&pType;
            break;
        }
        break;
    case 0x1c:
        size = 0x40;
        break;
    case 0x1d:
        size = 0x68;
        break;
    case 0x00:
    case 0x06:
    case 0x08:
    case 0x09:
    case 0x0e:
        size = 2;
        break;
    case 0x07:
        size = 0xf8;
        break;
    case 0x1e:
        size = 0x94;
        break;
    case 0x18:
    case 0x21:
        size = 0x38;
        break;
    case 0x22:
        size = 0x64;
        break;
    case 0x33:
        size = 0x1c;
        break;
    case 0x20:
    case 0x24:
    case 0x25:
        size = 0x30;
        break;
    case 0x0f:
    case 0x26:
        size = 0x10;
        break;
    case 0x28:
        size = 0x3c;
        break;
    case 0x17:
    case 0x19:
    case 0x23:
    case 0x2b:
        size = 0xc;
        break;
    case 0x16:
    case 0x1f:
    case 0x2d:
        size = 0x14;
        break;
    case 0x2e:
        size = pOption + 0xc;
        break;
    case 0x0a:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x2c:
    case 0x2f:
        size = 8;
        break;
    case 0x10:
    case 0x11:
    case 0x27:
    case 0x29:
    case 0x2a:
    case 0x31:
        size = pOption;
        break;
    case 0x32:
        size = 0x20;
        break;
    default:
        size = 4;
        break;
    }
    size += size & 1;
    if (size > 0xff) {
        sprintf(buf, " type %d", pType);
        FatalError(0x72, buf);
    }
    return size;
}
#pragma auto_inline(on)

// NetGetMessageSize

// FUNCTION: CARMA2_HW 0x0049f650
tNet_message* C2_HOOK_FASTCALL NetBuildMessage(undefined pArg1, undefined4 pArg2) {
    int size;
    tNet_message* message;

    size = NetGetContentsSize(pArg1, pArg2) + sizeof(tNet_message_header);
    message = NetAllocateMessage(size);
    if (message != NULL) {
        message->header.field_0x14 = 1;
        message->header.field_0x16 = (tU16)size;
        message->contents.raw.header.type = pArg1;
    }
    return message;
}

// NetBuildGuaranteedMessage

// NetGetToPlayerContentsSize

// FUNCTION: CARMA2_HW 0x0049f6f0
tNet_message_chunk* C2_HOOK_FASTCALL NetGetToPlayerContents(tU8 pType, int pSize_decider, tPlayer_ID pPlayer_id) {
    int i;
    int chunk_size;
    int offset;
    tNet_message* message;
    tNet_message_chunk* chunk;
    tNet_game_player_info* player;

    chunk_size = NetGetContentsSize(pType, pSize_decider);
    i = 0;
    if (gNumber_of_net_players <= 0) {
        return NULL;
    }
    for (; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pPlayer_id) {
            goto found_player;
        }
    }
    return NULL;

found_player:
    player = &gNet_players[i];
    if (player == &gNet_players[gThis_net_player_index]) {
        PDFatalError("Trying to send network message to self");
    }
    message = player->field_0xcc;
    if (message != NULL && message->header.field_0x16 + chunk_size > 0x200) {
        tPlayer_ID player_id = player->ID;
        tNet_game_details* game = gCurrent_net_game;

        if (gNet_mode != eNet_mode_none) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == player_id) {
                    goto send_message;
                }
            }
            goto clear_message;

send_message:
            PDNetSendMessageToAddress(game, message, &gNet_players[i]);
        }
clear_message:
        player->field_0xcc = message = NULL;
    }
    if (message == NULL) {
        message = NetAllocateMessage(0x200);
        player->field_0xcc = message;
        message->header.field_0x16 = (tU16)sizeof(tNet_message_header);
        message->header.field_0x14 = 0;
    }
    offset = message->header.field_0x16;
    chunk = (tNet_message_chunk*)((tU8*)message + offset);
    message->header.field_0x16 += (tU16)chunk_size;
    message->header.field_0x14 += 1;
    chunk->raw.header.type = pType;
    chunk->raw.header.size = (tU8)chunk_size;
    return chunk;
}

// NetGetToHostContents

// NetStartBroadcastContents

// NetBroadcastContents

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049fcf0
void C2_HOOK_FASTCALL NetSendMessageStacks(void) {
#ifndef CARPOCALYPSE2_MATCHING
    int i;

    if (gNet_mode == eNet_mode_none) {
        return;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index && gNet_players[i].field_0xcc != NULL) {
            NetReallySendMessageToPlayer(gCurrent_net_game, gNet_players[i].field_0xcc, gNet_players[i].ID);
            gNet_players[i].field_0xcc = NULL;
        }
    }
#else
    int i;

    if (gNet_mode == eNet_mode_none) {
        return;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index && gNet_players[i].field_0xcc != NULL) {
            NetReallySendMessageToPlayer(gCurrent_net_game, gNet_players[i].field_0xcc, gNet_players[i].ID);
            gNet_players[i].field_0xcc = NULL;
        }
    }
#endif
}
#pragma auto_inline(on)

// GLOBAL: CARMA2_HW 0x00690c60
static int rr_min;

// GLOBAL: CARMA2_HW 0x00690c64
static int rr_mid;

// GLOBAL: CARMA2_HW 0x00690c68
static int rr_max;

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 25.91% | difftest PASS 2000 trials (spec difftest/specs/0049fdb0_NetAllocateMessage.json) | reason original and recomp use different loop/register layout
// FUNCTION: CARMA2_HW 0x0049fdb0
tNet_message* C2_HOOK_FASTCALL NetAllocateMessage(int pSize) {
    tU8* pool;
    tU8* msg;
    int i;
    tNet_message_memory* block;
    tNet_message_memory* tail;

    pool = NULL;
    if (pSize <= (int)sizeof(tMin_message) - 4) {
        for (i = 0; i < MIN_MESSAGES_CAPACITY; i++) {
            msg = (tU8*)gMin_messages + rr_min * sizeof(tMin_message) + gMessage_header_size;
            if (msg[0x19] == eNetMsg_none) {
                pool = (tU8*)gMin_messages + rr_min * sizeof(tMin_message);
                break;
            }
            rr_min++;
            if (rr_min >= MIN_MESSAGES_CAPACITY) {
                rr_min = 0;
            }
        }
    }
    if (pool == NULL && pSize <= (int)sizeof(tMid_message) - 4) {
        for (i = 0; i < MID_MESSAGES_CAPACITY; i++) {
            msg = (tU8*)gMid_messages + rr_mid * sizeof(tMid_message) + gMessage_header_size;
            if (msg[0x19] == eNetMsg_none) {
                pool = (tU8*)gMid_messages + rr_mid * sizeof(tMid_message);
                break;
            }
            rr_mid++;
            if (rr_mid >= MID_MESSAGES_CAPACITY) {
                rr_mid = 0;
            }
        }
    }
    if (pool == NULL && pSize <= (int)sizeof(tMax_message) - 4) {
        for (i = 0; i < MAX_MESSAGES_CAPACITY; i++) {
            msg = (tU8*)gMax_messages + rr_max * sizeof(tMax_message) + gMessage_header_size;
            if (msg[0x19] == eNetMsg_none) {
                pool = (tU8*)gMax_messages + rr_max * sizeof(tMax_message);
                break;
            }
            rr_max++;
            if (rr_max >= MAX_MESSAGES_CAPACITY) {
                rr_max = 0;
            }
        }
    }
    if (pool == NULL) {
        block = BrMemAllocate(gMessage_header_size + pSize + sizeof(void*), kMem_dynamic_message);
        if (block == NULL) {
            return NULL;
        }
        block->next = NULL;
        if (gDynamic_messages != NULL) {
            tail = gDynamic_messages;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = block;
        } else {
            gDynamic_messages = block;
        }
        pool = (tU8*)block + sizeof(void*);
    }
    msg = pool + gMessage_header_size;
    ((tNet_message*)msg)->header.field_0xc = 1;
    ((tNet_message*)msg)->header.field_0x4 = MESSAGE_MAGIC_NUMBER;
    return (tNet_message*)msg;
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL NetFreeExcessMemory(void) {

    while (gDynamic_messages != NULL && *(&gDynamic_messages->message.contents.raw.header.type + gMessage_header_size) == eNetMsg_none) {
        tNet_message_memory* next = gDynamic_messages->next;
        BrMemFree(gDynamic_messages);
        gDynamic_messages = next;
    }
}

void C2_HOOK_FASTCALL DisposeExcessMemory(void) {
    tNet_message_memory* next;

    while (gDynamic_messages != NULL) {
        next = gDynamic_messages->next;
        BrMemFree(gDynamic_messages);
        gDynamic_messages = next;
    }
}

// FUNCTION: CARMA2_HW 0x0049ff50
int C2_HOOK_FASTCALL NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    if (pMessage->header.field_0x14 == 0xff) {
        return -1;
    }
    pMessage->contents.raw.header.type = eNetMsg_none;
    return 0;
}

// NetReallyDisposeMessage

// NetGetNextMessage

// ReceivedSendMeDetails

// ReceivedDetails

// SendOutPlayerList

// ReceivedJoin

// KickPlayerOut

// ReceivedLeave

// NetFullScreenMessage

// HostHasBittenTheDust

// ReceivedHosticide

// ReceivedNewPlayerList

// ReceivedRaceOver

// ReceivedStatusReport

// ReceivedGroupStatus

// ReceivedRepositionCar

// ReceivedStartRace

// ReceivedGuaranteeReply

// ReceivedHeadup

// ReceivedHostQuery

// ReceivedHostReply

// ReceivedGuaranteedMessage

// PlayerIsInList

// ReceivedTimeSync

// ReceivedConfirm

// ReceivedDisableCar

// ReceivedEnableCar

// ReceivedActivateLift

// ReceivedScores

// ReceivedWasted

// ReceivedCarDetailsReq

// ReceivedCarDetails

// ReceivedGameScores

// ProcessGuaranteeStuff

// ReceivedMessage

// C2PORT-NONMATCHING: best 71.51% | difftest PASS 2000 trials (spec difftest/specs/004a4660_RemoveNetPlayer.json) | reason vc5-register-allocation
// FUNCTION: CARMA2_HW 0x004a4660
void C2_HOOK_FASTCALL RemoveNetPlayer(register tPlayer_ID pPlayer_id) {
    int player_count;
    int player_index;
    int i;
    int move_count;
    int message_size;
    int contents_size;
    int guarantee_size;
    int trailing_contents_size;
    tNet_game_player_info* volatile player_list;
    tNet_game_player_info* player;
    tNet_message* message;
    tNet_game_player_info* message_player;
    tNet_game_details* volatile current_game;
    tU8* message_bytes;
    tU32* shift_destination;

    player_count = gNumber_of_net_players;
    player_list = BrMemAllocate(player_count * sizeof(tNet_game_player_info), kMem_player_list_leave);
    memcpy(player_list, gNet_players, player_count * sizeof(tNet_game_player_info));
    player = player_list;
    player_index = 0;
    if (player_count > 0) {
        do {
            if (pPlayer_id == player->ID) {
                if (player->car_index >= 0) {
                    gCar_details[player->car_index].ownership = 0;
                }
                if (player->next_car_index >= 0
                        && (gCurrent_net_game->options.car_choice == eNet_car_all
                            || gCurrent_net_game->options.car_choice == eNet_car_novelty)) {
                    gCar_details[player->next_car_index].ownership = 0;
                }
                player_count--;
                if (player_index < player_count) {
                    move_count = player_count - player_index;
                    shift_destination = (tU32*)player;
                    do {
                        move_count--;
                        memcpy(shift_destination, shift_destination + 0x35, sizeof(tNet_game_player_info));
                        shift_destination += 0x35;
                    } while (move_count != 0);
                }
                break;
            }
            player++;
            player_index++;
        } while (player_index < player_count);
    }
    NetPlayersChanged(player_count, player_list);
    BrMemFree(player_list);
    i = 0;
    gCurrent_net_game->num_players = gNumber_of_net_players;
    if (gNumber_of_net_players > 0) {
        player = gNet_players;
        do {
            contents_size = NetGetContentsSize(3, 1);
            guarantee_size = NetGetContentsSize(5, 0);
            trailing_contents_size = NetGetContentsSize(3, 1);
            message_size = trailing_contents_size + 0x18 + guarantee_size;
            message = NetAllocateMessage(message_size);
            if (message != NULL) {
                message_bytes = (tU8*)message;
                message->contents.raw.header.size = (tU8)contents_size;
                message->header.field_0x14 = 0xff;
                message->header.field_0x16 = (tU16)message_size;
                message->contents.raw.header.type = 3;
                message_bytes[0x19 + (tU8)contents_size] = 5;
                message_bytes[0x18 + message->contents.raw.header.size] = (tU8)guarantee_size;
            }
            message_bytes = (tU8*)message;
            *(int*)(message_bytes + 0x1c) = gNumber_of_net_players;
            *(int*)(message_bytes + 0x20) = i;
            *(int*)(message_bytes + 0x24) = gUINT_00690c40;
            message_player = (tNet_game_player_info*)(message_bytes + 0x28);
            memcpy(message_player, player, sizeof(tNet_game_player_info));
            message_player->field_0xcc = NULL;
            current_game = gCurrent_net_game;
            if (gNumber_of_net_players == 1) {
                message->contents.raw.header.type = 0xff;
            } else {
                int recipient;

                for (recipient = 0; recipient < gNumber_of_net_players; recipient++) {
                    if (recipient != gThis_net_player_index) {
                        SendGuaranteedMessageToPlayer(current_game, message, gNet_players[recipient].ID, 0);
                    }
                }
            }
            i++;
            player++;
        } while (i < gNumber_of_net_players);
    }
    gUINT_00690c40++;
    if (gInitialised_grid != 0) {
        SetUpNetCarPositions();
        if (gINT_0074b780 != 0) {
            gINT_0074c6d4 = 1;
            player = &gNet_players[1];
            for (i = 1; i < gNumber_of_net_players; i++, player++) {
                player->field_0x60 = 1;
            }
        }
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 70.71% | difftest PASS 2000 trials (spec difftest/specs/004a4a40_NetReceiveAndProcessMessages.json) | reason vc5-register-scheduling
// FUNCTION: CARMA2_HW 0x004a4a40
void C2_HOOK_FASTCALL NetReceiveAndProcessMessages(void) {
    tNet_message* message;
    void* sender_address;

    if (gNet_mode != eNet_mode_none || gJoin_list_mode != 0) {
        int old_in_net_service = gIn_net_service;
        register int completion_byte = 0xff;
        register tU32 message_signature = 0x763a5059;

        gIn_net_service = 1;
        do {
            message = NetGetNextMessage(gCurrent_net_game, &sender_address);
            if (message == NULL) {
                gIn_net_service = old_in_net_service;
                return;
            }
            if (message->header.field_0x14 != 0) {
                tU32 receive_time = GetRaceTime();

                if (message->header.field_0x4 == message_signature) {
                    ReceivedMessage(message, sender_address, receive_time);
                }
                ((tU8*)message)[0x19] = (tU8)completion_byte;
            }
        } while (message != NULL);
        gIn_net_service = old_in_net_service;
    }
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 40.89% | difftest PASS 2000 trials (spec difftest/specs/004a4ac0_BroadcastStatus.json) | reason host-client packet batching with compiler register scheduling
// FUNCTION: CARMA2_HW 0x004a4ac0
void C2_HOOK_FASTCALL BroadcastStatus(void) {
    tNet_message* message;
    tNet_message* queued_message;
    tNet_game_player_info* player;
    tU8* source;
    tU8* destination;
    tU32 contents_size;
    int i;
    int j;

    if (gNet_mode == eNet_mode_client) {
        contents_size = NetGetContentsSize(0xb, 0);
        message = NetAllocateMessage(contents_size + sizeof(tNet_message_header));
        if (message != NULL) {
            message->header.field_0x14 = 1;
            message->header.field_0x16 = (tU16)(contents_size + sizeof(tNet_message_header));
            message->contents.raw.header.type = 0xb;
        }
        *(tU16*)((tU8*)message + 0x1a) = (tU16)gNet_players[gThis_net_player_index].player_status;
        if (gNet_mode != eNet_mode_none) {
            message->header.field_0x8 = gLocal_net_ID;
            message->header.field_0x10 = PDGetTotalTime();
            PDNetSendMessageToAddress(gCurrent_net_game, message, gCurrent_net_game);
        }
    } else if (gNet_mode == eNet_mode_host) {
        contents_size = NetGetContentsSize(0x2e, gNumber_of_net_players);
        message = NetAllocateMessage(contents_size + sizeof(tNet_message_header));
        if (message != NULL) {
            message->header.field_0x14 = 1;
            message->header.field_0x16 = (tU16)(contents_size + sizeof(tNet_message_header));
            message->contents.raw.header.type = 0x2e;
        }
        *(tU8*)((tU8*)message + 0x20) = (tU8)gNumber_of_net_players;
        *(tU32*)((tU8*)message + 0x1c) = 0;
        source = (tU8*)&gNet_players[0].player_status;
        destination = (tU8*)message + 0x21;
        i = 0;
        if (gNumber_of_net_players > 0) {
            do {
                *(tU32*)((tU8*)message + 0x1c) += *(tU32*)(source - 0x24);
                *destination = *source;
                source += sizeof(tNet_game_player_info);
                destination++;
                i++;
            } while (i < gNumber_of_net_players);
        }

        message->contents.raw.header.size = (tU8)(message->header.field_0x16 - sizeof(tNet_message_header));
        contents_size = message->header.field_0x16 - sizeof(tNet_message_header);
        i = 0;
        player = gNet_players;
        if (gNumber_of_net_players > 0) {
            do {
                if (i != gThis_net_player_index) {
                if (&player->field_0xcc == &gNet_players[gThis_net_player_index].field_0xcc) {
                    PDFatalError("Trying to send network message to self");
                }
                queued_message = player->field_0xcc;
                if (queued_message == NULL) {
                    queued_message = NetAllocateMessage(0x200);
                    player->field_0xcc = queued_message;
                    queued_message->header.field_0x16 = sizeof(tNet_message_header);
                    queued_message->header.field_0x14 = 0;
                } else if (0x200 < queued_message->header.field_0x16 + contents_size) {
                    NetReallySendMessageToPlayer(gCurrent_net_game, queued_message, player->ID);
                    player->field_0xcc = NULL;
                    queued_message = NetAllocateMessage(0x200);
                    player->field_0xcc = queued_message;
                    queued_message->header.field_0x16 = sizeof(tNet_message_header);
                    queued_message->header.field_0x14 = 0;
                }
                destination = (tU8*)queued_message + queued_message->header.field_0x16;
                queued_message->header.field_0x16 += (tU16)contents_size;
                queued_message->header.field_0x14++;
                source = (tU8*)message + sizeof(tNet_message_header);
                memcpy(destination, source, contents_size);
                }
                i++;
                player++;
            } while (i < gNumber_of_net_players);
        }
        if (message->header.field_0x14 != 0xff) {
            message->contents.raw.header.type = 0xff;
        }
    }

    gLast_flush_message = PDGetTotalTime();
    player = gNet_players;
    i = 0;
    if (gNumber_of_net_players > 0) {
        do {
        queued_message = player->field_0xcc;
        if (queued_message != NULL) {
            if (gNet_mode != eNet_mode_none) {
                queued_message->header.field_0x8 = gLocal_net_ID;
                queued_message->header.field_0x10 = PDGetTotalTime();
                for (j = 0; j < gNumber_of_net_players; j++) {
                    if (gNet_players[j].ID == player->ID) {
                        PDNetSendMessageToAddress(gCurrent_net_game, queued_message, &gNet_players[j]);
                        break;
                    }
                }
            }
            player->field_0xcc = NULL;
        }
            i++;
            player++;
        } while (i < gNumber_of_net_players);
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004a4da0
void C2_HOOK_FASTCALL CheckForDisappearees(void) {
#ifndef CARPOCALYPSE2_MATCHING
    tU32 now;
    int i;

    now = PDGetTotalTime();
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index && now - gNet_players[i].last_heard_from_him > 60000) {
            gNet_players[i].player_status = ePlayer_status_not_responding;
        }
    }
#else
    tU32 now;
    int i;

    now = PDGetTotalTime();
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (i != gThis_net_player_index && now - gNet_players[i].last_heard_from_him > 60000) {
            gNet_players[i].player_status = ePlayer_status_not_responding;
        }
    }
#endif
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 93.75% | difftest PASS 2000 trials (spec difftest/specs/004a5240_CheckForPendingStartRace.json) | reason overlapping global alias
// FUNCTION: CARMA2_HW 0x004a5240
void C2_HOOK_FASTCALL CheckForPendingStartRace(void) {
    int i;
    tNet_game_player_info* player;

    if (gINT_0074c6d4 == 0) {
        return;
    }
    player = &gNet_players[1];
    for (i = 1; i < gNumber_of_net_players; i++, player++) {
        if (player->field_0x60 != 0) {
            return;
        }
    }
    SignalToStartRace();
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 22.66% | difftest PASS 2000 trials (spec difftest/specs/004a5280_NetService_idle.json) | reason helper-inlining-codegen
// FUNCTION: CARMA2_HW 0x004a5280
void C2_HOOK_FASTCALL NetService(int pIn_race) {
    tU32 time;

    if (gIn_net_service || gNet_service_disable) {
        return;
    }
    time = PDGetTotalTime();
    gIn_net_service = 1;
    if (gJoin_list_mode) {
        NetFreeExcessMemory();
        DoNextJoinPoll();
        NetReceiveAndProcessMessages();
    } else {
        if (gNet_mode != eNet_mode_none) {
            NetFreeExcessMemory();
            if (!pIn_race) {
                NetReceiveAndProcessMessages();
            }
            if (time - gLast_status_broadcast > 1000) {
                gLast_status_broadcast = PDGetTotalTime();
                BroadcastStatus();
            }
            CheckForDisappearees();
            if (gNet_mode == eNet_mode_host) {
                CheckForPendingStartRace();
                CheckForNeedyEnvironmentRecipients();
            }
        }
    }
    if (gJoin_list_mode || gNet_mode != eNet_mode_none) {
        ResendGuaranteedMessages();
        if (time > gLast_flush_message + 200u || !gProgram_state.racing) {
            NetSendMessageStacks();
        }
    }
    gIn_net_service = 0;
}

// NetFinishRace

// C2PORT-NONMATCHING: best 33.93% | difftest PASS 2000 trials (spec difftest/specs/004a57e0_NetPlayerStatusChanged.json) | reason inline network batching and register scheduling
// FUNCTION: CARMA2_HW 0x004a57e0
void C2_HOOK_FASTCALL NetPlayerStatusChanged(tPlayer_status pNew_status) {
    tNet_message* message;
    tNet_message* queued_message;
    tNet_game_player_info* player;
    tU8* source;
    tU8* destination;
    tU32 contents_size;
    int i;
    int j;

    if (gNet_mode != eNet_mode_none) {
        player = &gNet_players[gThis_net_player_index];
        if (player->player_status != pNew_status) {
            player->player_status = pNew_status;
            if (gNet_mode == eNet_mode_client) {
                contents_size = NetGetContentsSize(0xb, 0);
                message = NetAllocateMessage(contents_size + sizeof(tNet_message_header));
                if (message != NULL) {
                    message->header.field_0x14 = 1;
                    message->header.field_0x16 = (tU16)(contents_size + sizeof(tNet_message_header));
                    message->contents.raw.header.type = 0xb;
                }
                *(tU16*)((tU8*)message + 0x1a) = (tU16)player->player_status;
                if (gNet_mode != eNet_mode_none) {
                    message->header.field_0x8 = gLocal_net_ID;
                    message->header.field_0x10 = PDGetTotalTime();
                    PDNetSendMessageToAddress(gCurrent_net_game, message, gCurrent_net_game);
                }
            } else if (gNet_mode == eNet_mode_host) {
                contents_size = NetGetContentsSize(0x2e, gNumber_of_net_players);
                message = NetAllocateMessage(contents_size + sizeof(tNet_message_header));
                if (message != NULL) {
                    message->header.field_0x14 = 1;
                    message->header.field_0x16 = (tU16)(contents_size + sizeof(tNet_message_header));
                    message->contents.raw.header.type = 0x2e;
                }
                *(tU8*)((tU8*)message + 0x20) = (tU8)gNumber_of_net_players;
                *(tU32*)((tU8*)message + 0x1c) = 0;
                source = (tU8*)&gNet_players[0].player_status;
                destination = (tU8*)message + 0x21;
                i = 0;
                if (gNumber_of_net_players > 0) {
                    do {
                        *(tU32*)((tU8*)message + 0x1c) += *(tU32*)(source - 0x24);
                        *destination = *source;
                        source += sizeof(tNet_game_player_info);
                        destination++;
                        i++;
                    } while (i < gNumber_of_net_players);
                }
                contents_size = message->header.field_0x16 - sizeof(tNet_message_header);
                message->contents.raw.header.size = (tU8)contents_size;
                i = 0;
                player = gNet_players;
                if (gNumber_of_net_players > 0) {
                    do {
                        if (i != gThis_net_player_index) {
                            if (&player->field_0xcc == &gNet_players[gThis_net_player_index].field_0xcc) {
                                PDFatalError("Trying to send network message to self");
                            }
                            queued_message = player->field_0xcc;
                            if (queued_message == NULL) {
                                queued_message = NetAllocateMessage(0x200);
                                player->field_0xcc = queued_message;
                                queued_message->header.field_0x16 = sizeof(tNet_message_header);
                                queued_message->header.field_0x14 = 0;
                            } else if (0x200 < queued_message->header.field_0x16 + contents_size) {
                                NetReallySendMessageToPlayer(gCurrent_net_game, queued_message, player->ID);
                                player->field_0xcc = NULL;
                                queued_message = NetAllocateMessage(0x200);
                                player->field_0xcc = queued_message;
                                queued_message->header.field_0x16 = sizeof(tNet_message_header);
                                queued_message->header.field_0x14 = 0;
                            }
                            destination = (tU8*)queued_message + queued_message->header.field_0x16;
                            queued_message->header.field_0x16 += (tU16)contents_size;
                            queued_message->header.field_0x14++;
                            source = (tU8*)message + sizeof(tNet_message_header);
                            memcpy(destination, source, contents_size);
                        }
                        i++;
                        player++;
                    } while (i < gNumber_of_net_players);
                }
                if (message->header.field_0x14 != 0xff) {
                    message->contents.raw.header.type = 0xff;
                }
            }

            gLast_flush_message = PDGetTotalTime();
            player = gNet_players;
            i = 0;
            if (gNumber_of_net_players > 0) {
                do {
                    queued_message = player->field_0xcc;
                    if (queued_message != NULL) {
                        if (gNet_mode != eNet_mode_none) {
                            queued_message->header.field_0x8 = gLocal_net_ID;
                            queued_message->header.field_0x10 = PDGetTotalTime();
                            for (j = 0; j < gNumber_of_net_players; j++) {
                                if (gNet_players[j].ID == player->ID) {
                                    PDNetSendMessageToAddress(gCurrent_net_game, queued_message, &gNet_players[j]);
                                    break;
                                }
                            }
                        }
                        player->field_0xcc = NULL;
                    }
                    i++;
                    player++;
                } while (i < gNumber_of_net_players);
            }

            if (gProgram_state.current_car.disabled != 0) {
                if (pNew_status > ePlayer_status_racing && pNew_status != ePlayer_status_recovering) {
                    EnableCar(&gProgram_state.current_car);
                    return;
                }
                if (gProgram_state.current_car.disabled != 0) {
                    return;
                }
            }
            if (pNew_status < ePlayer_status_racing) {
                DisableCar(&gProgram_state.current_car);
            }
        }
    }
}

// NetGetPlayerStatus

// NetGuaranteedSendMessageToAllPlayers

// NetGuaranteedSendMessageToEverybody

// NetGuaranteedSendMessageToHost

// AddToGuaranteeList
// C2PORT-NONMATCHING: best 45.92% | difftest PASS 2000 trials (spec 004a5ca0_AddToGuaranteeList.json) | reason equivalent queue bookkeeping with compiler loop and stack layout differences
// FUNCTION: CARMA2_HW 0x004a5ca0
void C2_HOOK_FASTCALL AddToGuaranteeList(tNet_message* pMessage, void* pAddress, tPlayer_ID pPlayer, void* pNotifyFail) {
    int i;
    int n;
    tU32 time;
    tU8 size;
    int idx;

    if (pMessage->header.field_0x14 != 0xff) {
        PDFatalError("Guaranteed message not built correctly");
    }
    if (gNext_guarantee >= 200) {
        for (i = 0; i < 10; i++) {
            gGuarantee_list[i].message->contents.raw.header.type = eNetMsg_none;
        }
        n = 10;
        if (gGuarantee_list[9].message == gGuarantee_list[10].message) {
            /* count consecutive duplicates starting at index 10 */
            {
                tU8* p = (tU8*)&gGuarantee_list[10];
                tU8* end = (tU8*)&gGuarantee_list[200];
                while (p < end) {
                    tNet_message* ca = *(tNet_message**)p;
                    tNet_message* cb = *(tNet_message**)(p + sizeof(tGuaranteed_message));
                    p += sizeof(tGuaranteed_message);
                    n++;
                    if (ca != cb) {
                        break;
                    }
                }
            }
        }
        memmove(&gGuarantee_list[0], &gGuarantee_list[n], (200 - n) * sizeof(tGuaranteed_message));
        gNext_guarantee -= n;
        if (gNext_guarantee >= 200) {
            return;
        }
    }
    size = pMessage->contents.raw.header.size;
    *(tU16*)((tU8*)pMessage + size + 0x1a) = gGuarantee_number;
    idx = gNext_guarantee;
    gGuarantee_list[idx].field_0x2c = gGuarantee_number;
    gGuarantee_number++;
    if (gGuarantee_number == 0) {
        gGuarantee_number = 1;
    }
    gGuarantee_list[idx].message = pMessage;
    time = PDGetTotalTime();
    gGuarantee_list[idx].send_time = time;
    gGuarantee_list[idx].next_resend_time = time + 100;
    gGuarantee_list[idx].resend_period = 100;
    if (pAddress == NULL) {
        gGuarantee_list[idx].field_0x30 = 0;
    } else {
        ((tU32*)gGuarantee_list[idx].pd_address)[0] = ((tU32*)pAddress)[0];
        ((tU32*)gGuarantee_list[idx].pd_address)[1] = ((tU32*)pAddress)[1];
        ((tU32*)gGuarantee_list[idx].pd_address)[2] = ((tU32*)pAddress)[2];
        *(tU16*)&gGuarantee_list[idx].pd_address[12] = *(tU16*)((tU8*)pAddress + 12);
        gGuarantee_list[idx].field_0x30 = 1;
    }
    gGuarantee_list[idx].local_id = pPlayer;
    gGuarantee_list[idx].NotifyFail = pNotifyFail;
    gGuarantee_list[idx].recieved = 0;
    gNext_guarantee = idx + 1;
}

// NetGuaranteedSendMessageToPlayer

// FUNCTION: CARMA2_HW 0x004a6020
int C2_HOOK_FASTCALL NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, void* pNotifyFail) {

    if (gNet_mode == eNet_mode_none && !gJoin_list_mode) {
        return -3;
    }
    pMessage->header.field_0x8 = gLocal_net_ID;
    pMessage->header.field_0x10 = PDGetTotalTime();
    AddToGuaranteeList(pMessage, pAddress, 0, pNotifyFail);
    return PDNetSendMessageToAddress(pDetails, pMessage, pAddress);
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004a6080
void C2_HOOK_FASTCALL ResendGuaranteedMessages(void) {
    int retained;
    int i;
    tU32 time;
    tGuaranteed_message* entry;
    int player_index;
    tNet_game_player_info* player;
    tU8* contents;
    int contents_size;
    int j;
    tNet_message* message;
    tPlayer_ID target_id;
    tNet_game_details* game;
    int offset;

    retained = 0;
    time = PDGetTotalTime();
    for (i = 0; i < gNext_guarantee; i++) {
        entry = &gGuarantee_list[retained];
        if (i != retained) {
            *entry = gGuarantee_list[i];
        }
        if (entry->recieved == 0) {
            if (entry->message->header.field_0x14 != 0xff) {
                PDFatalError("Dodgy message in guarantee list");
            }
            if (entry->NotifyFail != NULL) {
                entry->recieved |= entry->NotifyFail(time - entry->send_time, entry->message);
            } else if (time - entry->send_time > 10000U) {
                entry->recieved = 1;
            }
        }
        if (entry->recieved == 0) {
            if (time > entry->next_resend_time) {
                *(tU16*)((tU8*)entry->message + entry->message->contents.raw.header.size + 0x1a) = entry->field_0x2c;
                if (entry->field_0x30 == 1) {
                    PDNetSendMessageToAddress(gCurrent_net_game, entry->message, entry->pd_address);
                } else if (entry->field_0x30 == 0) {
                    player_index = 0;
                    player = gNet_players;
                    while (player_index < gNumber_of_net_players && player->ID != entry->local_id) {
                        player_index++;
                        player++;
                    }
                    if (player_index == gNumber_of_net_players) {
                        /* Keep the disconnected entry until the next cleanup pass. */
                        entry->recieved = 1;
                        retained++;
                        continue;
                    }
                    contents = (tU8*)entry->message + sizeof(tNet_message_header);
                    contents_size = 0;
                    for (j = 0; j < 2; j++) {
                        contents_size += *contents;
                        contents += *contents;
                    }
                    if (player == &gNet_players[gThis_net_player_index]) {
                        PDFatalError("Trying to send network message to self");
                    }
                    message = player->field_0xcc;
                    if (message != NULL && message->header.field_0x16 + contents_size > 0x200) {
                        target_id = player->ID;
                        game = gCurrent_net_game;
                        if (gNet_mode != eNet_mode_none) {
                            message->header.field_0x8 = gLocal_net_ID;
                            message->header.field_0x10 = PDGetTotalTime();
                            for (j = 0; j < gNumber_of_net_players; j++) {
                                if (gNet_players[j].ID == target_id) {
                                    PDNetSendMessageToAddress(game, message, &gNet_players[j].pd_net_info);
                                    break;
                                }
                            }
                        }
                        player->field_0xcc = NULL;
                        message = NULL;
                    }
                    if (message == NULL) {
                        message = NetAllocateMessage(0x200);
                        player->field_0xcc = message;
                        message->header.field_0x16 = sizeof(tNet_message_header);
                        message->header.field_0x14 = 0;
                    }
                    offset = message->header.field_0x16;
                    message->header.field_0x16 += contents_size;
                    message->header.field_0x14++;
                    /* Both increments are present in the original, even when these alias. */
                    player->field_0xcc->header.field_0x14++;
                    memcpy((tU8*)message + offset, (tU8*)entry->message + sizeof(tNet_message_header), contents_size);
                } else {
                    PDFatalError("oops - guaranteed messages");
                }
                entry->resend_period = (tU32)(entry->resend_period * 1.2);
                entry->next_resend_time += entry->resend_period;
            }
            retained++;
        } else if ((retained == 0 || entry->message != entry[-1].message)
            && (i + 1 >= gNext_guarantee || entry->message != gGuarantee_list[i + 1].message)) {
            entry->message->header.field_0x14 = 0;
            if (entry->message->header.field_0x14 != 0xff) {
                entry->message->contents.raw.header.type = 0xff;
            }
        }
    }
    gNext_guarantee = retained;
}
#pragma auto_inline(on)

// NetWaitForGuaranteeReplies

// NetPlayerFromID

// NetCarFromPlayerID

// NetPlayerFromCar

// GetCheckSum

// CheckCheckSum

// StatReceivePacket

// StatSendPacket
typedef struct tNet_score_entry {
    int player_index;
    int score;
    int out_of_game;
} tNet_score_entry;

// GLOBAL: CARMA2_HW 0x0068d93c
int gNet_score_scene_initialised;

// GLOBAL: CARMA2_HW 0x0068d94c
tU32 gNet_score_last_flash;

// GLOBAL: CARMA2_HW 0x0068d950
int gNet_score_flash_state;

// GLOBAL: CARMA2_HW 0x0074a71c
br_actor* gNet_score_root;

// GLOBAL: CARMA2_HW 0x0074a720
br_material* gNet_score_materials[4];

// GLOBAL: CARMA2_HW 0x0074a730
br_actor* gNet_score_camera;

// GLOBAL: CARMA2_HW 0x0074a73c
br_model* gNet_score_model;

// GLOBAL: CARMA2_HW 0x0074a740
br_actor* gNet_score_actors[12];

// FUNCTION: CARMA2_HW 0x0049a330
static int C2_HOOK_CDECL SortNetScoresAscending(const void* pFirst, const void* pSecond) {
    const tNet_score_entry* first;
    const tNet_score_entry* second;

    first = (const tNet_score_entry*)pFirst;
    second = (const tNet_score_entry*)pSecond;
    if (first->out_of_game != 0) {
        if (second->out_of_game != 0) {
            return first->out_of_game - second->out_of_game;
        }
        return INT_MAX;
    }
    if (second->out_of_game != 0) {
        return INT_MIN + 1;
    }
    if (first->score != second->score) {
        return first->score - second->score;
    }
    return gNet_players[first->player_index].last_score_index - gNet_players[second->player_index].last_score_index;
}

// FUNCTION: CARMA2_HW 0x0049a3a0
static int C2_HOOK_CDECL SortNetScoresDescending(const void* pFirst, const void* pSecond) {
    const tNet_score_entry* first;
    const tNet_score_entry* second;

    first = (const tNet_score_entry*)pFirst;
    second = (const tNet_score_entry*)pSecond;
    if (first->out_of_game != 0) {
        if (second->out_of_game != 0) {
            return first->out_of_game - second->out_of_game;
        }
        return INT_MAX;
    }
    if (second->out_of_game != 0) {
        return INT_MIN + 1;
    }
    if (first->score != second->score) {
        return second->score - first->score;
    }
    return gNet_players[first->player_index].last_score_index - gNet_players[second->player_index].last_score_index;
}

static void C2_HOOK_FASTCALL FUN_0049a410(void);

// C2PORT-NONMATCHING: best 53.26% | difftest PASS 2000 trials (spec difftest/specs/00499a00_DoNetScores2_sort_only.json) | reason stack-layout
// FUNCTION: CARMA2_HW 0x00499a00
void C2_HOOK_FASTCALL DoNetScores2(int pOnly_sort_scores) {
    tNet_game_player_info* player_ptr;
    tNet_score_entry* fill_entry;
    tNet_score_entry* entry_ptr;
    br_actor** actor_ptr;
    int ascending_order;
    int player_index;
    int i;
    int row_index;
    int own_rank;
    int score;
    int car_index;
    int score_y;
    char name_buffer[40];
    char score_buffer[256];
    tNet_score_entry entries[12];

    ascending_order = 0;
    if (gCurrent_net_game->type == 3 || gCurrent_net_game->type == 6) {
        ascending_order = 1;
    }
    Flash(200, &gNet_score_last_flash, &gNet_score_flash_state);

    player_ptr = gNet_players;
    fill_entry = entries;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (player_ptr->player_status >= 6) {
            fill_entry->player_index = i;
            fill_entry->score = player_ptr->score2;
            score = player_ptr->score2;
            if (abs(score) == 1000000 || (score < 0 && gCurrent_net_game->type != 1)) {
                fill_entry->out_of_game = player_ptr->last_score_index + 1;
            } else {
                fill_entry->out_of_game = 0;
            }
        } else {
            fill_entry->player_index = -1;
            fill_entry->score = ascending_order ? 1000001 : -1000001;
            fill_entry->out_of_game = 1000;
        }
        fill_entry++;
        player_ptr++;
    }
    for (i = gNumber_of_net_players; i < 12; i++) {
        fill_entry->player_index = -1;
        fill_entry->score = ascending_order ? 1000001 : -1000001;
        fill_entry->out_of_game = 1000;
        fill_entry++;
    }

    qsort(entries, 12, sizeof(tNet_score_entry), ascending_order ? SortNetScoresAscending : SortNetScoresDescending);
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (entries[i].player_index == gThis_net_player_index) {
            own_rank = i;
            break;
        }
    }

    entry_ptr = entries;
    actor_ptr = gNet_score_actors;
    row_index = 0;
    score_y = 239;
    for (; row_index < 12; row_index++) {
        player_index = entry_ptr->player_index;
        if (player_index >= 0) {
            gNet_players[player_index].last_score_index = row_index;
        }

        if (pOnly_sort_scores == 0 && player_index >= 0) {
            if (own_rank == row_index) {
                if (gNet_score_scene_initialised == 0) {
                    FUN_0049a410();
                }
                if (gNet_score_scene_initialised != 0) {
                    br_actor* actor;
                    actor = *actor_ptr;
                    actor->material = gNet_score_materials[1];
                    actor->model = gNet_score_model;
                    BrMatrix34Translate(&actor->t.t.mat, 220.0f, (float)score_y, 0.0f);
                    BrActorAdd(gNet_score_root, actor);
                    BrZbSceneRender(gNet_score_root, gNet_score_camera, gRender_screen, gDepth_buffer);
                    BrActorRemove(actor);
                }
            } else {
                if (gNet_score_scene_initialised == 0) {
                    int material_index;
                    gNet_score_root = BrActorAllocate(0, NULL);
                    for (material_index = 0; material_index < 12; material_index++) {
                        gNet_score_actors[material_index] = BrActorAllocate(1, NULL);
                        if (gNet_score_actors[material_index] == NULL) {
                            BrFailure("Out of memory for NetPlay");
                        }
                        gNet_score_actors[material_index]->render_style = 4;
                    }
                    gNet_score_model = BrModelAllocate("NetScore Model", 4, 2);
                    gNet_score_model->faces[0].vertices[0] = 0;
                    gNet_score_model->faces[0].vertices[1] = 1;
                    gNet_score_model->faces[0].vertices[2] = 2;
                    gNet_score_model->faces[1].vertices[0] = 1;
                    gNet_score_model->faces[1].vertices[1] = 3;
                    gNet_score_model->faces[1].vertices[2] = 2;
                    gNet_score_model->faces[0].material = NULL;
                    gNet_score_model->faces[1].material = NULL;
                    gNet_score_model->vertices[0].p.v[0] = 0.0f;
                    gNet_score_model->vertices[0].p.v[1] = 0.0f;
                    gNet_score_model->vertices[0].p.v[2] = -6.0f;
                    gNet_score_model->vertices[1].p.v[0] = 100.0f;
                    gNet_score_model->vertices[1].p.v[1] = 0.0f;
                    gNet_score_model->vertices[1].p.v[2] = -6.0f;
                    gNet_score_model->vertices[2].p.v[0] = 0.0f;
                    gNet_score_model->vertices[2].p.v[1] = -38.0f;
                    gNet_score_model->vertices[2].p.v[2] = -6.0f;
                    gNet_score_model->vertices[3].p.v[0] = 100.0f;
                    gNet_score_model->vertices[3].p.v[1] = -38.0f;
                    gNet_score_model->vertices[3].p.v[2] = -6.0f;
                    gNet_score_model->flags |= 2;
                    BrModelAdd(gNet_score_model);

                    for (material_index = 0; material_index < 4; material_index++) {
                        sprintf(name_buffer, "NETBOX%i.PIX", material_index + 1);
                        gNet_score_materials[material_index] = BrMaterialAllocate("NetScore Material");
                        gNet_score_materials[material_index]->colour_map = LoadPixelmap(name_buffer);
                        if (gNet_score_materials[material_index]->colour_map == NULL) {
                            BrFailure("Cannot find headup graphic %s", name_buffer);
                        }
                        BrMapAdd(gNet_score_materials[material_index]->colour_map);
                        gNet_score_materials[material_index]->colour = 0;
                        gNet_score_materials[material_index]->flags = 0x2800;
                        BrMaterialAdd(gNet_score_materials[material_index]);
                    }

                    gNet_score_camera = BrActorAllocate(3, NULL);
                    gNet_score_camera->identifier = "Net HUD Camera";
                    ((br_camera*)gNet_score_camera->type_data)->type = 0;
                    ((br_camera*)gNet_score_camera->type_data)->hither_z = 1.0f;
                    ((br_camera*)gNet_score_camera->type_data)->yon_z = 3.0f;
                    ((br_camera*)gNet_score_camera->type_data)->width = (float)gScreen->width;
                    ((br_camera*)gNet_score_camera->type_data)->height = (float)gScreen->height;
                    BrActorAdd(gNet_score_root, gNet_score_camera);
                    gNet_score_scene_initialised = 1;
                }

                {
                    br_actor* actor = *actor_ptr;
                    actor->material = gNet_score_materials[0];
                    actor->model = gNet_score_model;
                    BrMatrix34Translate(&actor->t.t.mat, 220.0f, (float)score_y, 0.0f);
                    BrActorAdd(gNet_score_root, actor);
                    BrZbSceneRender(gNet_score_root, gNet_score_camera, gRender_screen, gDepth_buffer);
                    BrActorRemove(actor);
                }
            }

            if (gNet_score_flash_state != 0 || gCurrent_net_game->type != 7 || player_index != gIt_or_fox) {
                if (gNet_players[player_index].name_not_clipped != 0) {
                    int width;
                    char* name = gNet_players[player_index].player_name;
                    width = DRTextWidth(&gFonts[23], name);
                    while (width > 77) {
                        name[strlen(name) - 1] = '\0';
                        width = DRTextWidth(&gFonts[23], name);
                    }
                    gNet_players[player_index].name_not_clipped = 0;
                }
                score = own_rank == row_index ? 19 : 18;
                RenderPolyTextLine(gNet_players[player_index].player_name, 547, 5, score, eJust_left, 0);
            }

            score = gNet_players[player_index].score2;
            if (abs(score) == 1000000) {
                if (gNet_score_flash_state != 0) {
                    strcpy(score_buffer, GetMiscString(0xad));
                } else {
                    score_buffer[0] = '\0';
                }
            } else {
                switch (gCurrent_net_game->type) {
                case 0:
                    sprintf(score_buffer, "%d%%", score);
                    break;
                case 1:
                case 2:
                    sprintf(score_buffer, "%d", score);
                    break;
                case 3:
                    sprintf(score_buffer, "%d left", score >> 16);
                    break;
                case 4:
                    if (score < 0) {
                        if (gNet_score_flash_state != 0) {
                            sprintf(score_buffer, "%s", GetMiscString(0x5d));
                        } else {
                            score_buffer[0] = '\0';
                        }
                    } else {
                        sprintf(score_buffer, "%s -%d-", GetMiscString(0x5c), score);
                    }
                    break;
                case 5:
                    sprintf(score_buffer, "%d-%d", score / gCurrent_race.check_point_count + 1,
                        score % gCurrent_race.check_point_count + 1);
                    break;
                case 6:
                    if (score == 0) {
                        sprintf(score_buffer, "%s", GetMiscString(0xc0));
                    } else if (gNet_score_flash_state != 0) {
                        sprintf(score_buffer, "%s", GetMiscString(0xc1));
                    } else {
                        score_buffer[0] = '\0';
                    }
                    break;
                case 7:
                    if (player_index == gIt_or_fox && gNet_score_flash_state == 0) {
                        score_buffer[0] = '\0';
                    } else {
                        TimerString(score, score_buffer, 0, 1, 0);
                    }
                    break;
                default:
                    score_buffer[0] = '\0';
                    break;
                }
            }

            name_buffer[0] = (char)('0' + row_index);
            name_buffer[1] = '\0';
            RenderPolyTextLine(name_buffer, 624, 4, 25, eJust_left, 0);
            car_index = gNet_players[player_index].car_index;
            BrMatrix34Translate(&gCar_icons_model_actor->t.t.mat, 1090.0f, (float)(score_y - 20), 0.0f);
            gCar_icons_model_actor->material->colour_map = gTexture_maps[gCar_icons[car_index].index];
            BrMaterialUpdate(gCar_icons_model_actor->material, 8);
            gCar_icons_model_actor->model = gCar_icons[car_index].model;
            BrActorAdd(gNet_score_camera, gCar_icons_model_actor);
            BrZbsSceneRender(gNet_score_camera, gNet_score_camera, gBack_screen, gDepth_buffer);
            BrActorRemove(gCar_icons_model_actor);
            score = own_rank == row_index ? 19 : 18;
            RenderPolyTextLine(score_buffer, 547, 19, score, eJust_left, 0);
            score_y -= 40;
        }
        entry_ptr++;
        actor_ptr++;
    }
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0049a410
static void C2_HOOK_FASTCALL FUN_0049a410(void) {
    br_actor** actor_ptr;
    br_camera* camera;
    br_material* material;
    br_pixelmap* pixelmap;
    int material_index;
    char name_buffer[32];

    gNet_score_root = BrActorAllocate(0, NULL);
    actor_ptr = gNet_score_actors;
    do {
        *actor_ptr = BrActorAllocate(1, NULL);
        if (*actor_ptr == NULL) {
            BrFailure("Out of memory for NetPlay");
        }
        (*actor_ptr)->render_style = 4;
        actor_ptr++;
    } while ((int)actor_ptr < (int)(gNet_score_actors + 12));

    gNet_score_model = BrModelAllocate("NetScore Model", 4, 2);
    gNet_score_model->faces[0].vertices[0] = 0;
    gNet_score_model->faces[0].vertices[1] = 1;
    gNet_score_model->faces[0].vertices[2] = 2;
    gNet_score_model->faces[1].vertices[0] = 1;
    gNet_score_model->faces[1].vertices[1] = 3;
    gNet_score_model->faces[1].vertices[2] = 2;
    gNet_score_model->faces[0].material = NULL;
    gNet_score_model->faces[1].material = NULL;
    gNet_score_model->vertices[0].p.v[0] = 0.0f;
    gNet_score_model->vertices[0].p.v[1] = 0.0f;
    gNet_score_model->vertices[0].p.v[2] = -2.0f;
    gNet_score_model->vertices[1].p.v[0] = 100.0f;
    gNet_score_model->vertices[1].p.v[1] = 0.0f;
    gNet_score_model->vertices[1].p.v[2] = -2.0f;
    gNet_score_model->vertices[2].p.v[0] = 0.0f;
    gNet_score_model->vertices[2].p.v[1] = -38.0f;
    gNet_score_model->vertices[2].p.v[2] = -2.0f;
    gNet_score_model->vertices[3].p.v[0] = 100.0f;
    gNet_score_model->vertices[3].p.v[1] = -38.0f;
    gNet_score_model->vertices[3].p.v[2] = -2.0f;
    gNet_score_model->vertices[0].map.v[0] = 0.0f;
    gNet_score_model->vertices[0].map.v[1] = 0.0f;
    gNet_score_model->vertices[1].map.v[0] = 0.78125f;
    gNet_score_model->vertices[1].map.v[1] = 0.0f;
    gNet_score_model->vertices[2].map.v[0] = 0.0f;
    gNet_score_model->vertices[2].map.v[1] = 0.296875f;
    gNet_score_model->vertices[3].map.v[0] = 0.78125f;
    gNet_score_model->vertices[3].map.v[1] = 0.296875f;
    gNet_score_model->flags |= 2;
    BrModelAdd(gNet_score_model);

    for (material_index = 0; material_index < 4; material_index++) {
        sprintf(name_buffer, "NETBOX%i.PIX", material_index + 1);
        pixelmap = LoadPixelmap(name_buffer);
        if (pixelmap == NULL) {
            BrFailure("Cannot find headup graphic %s", name_buffer);
        }
        BrMapAdd(pixelmap);
        material = BrMaterialAllocate("NetScore Material");
        material->colour = 0;
        material->colour_map = pixelmap;
        material->flags = 0x2800;
        BrMaterialAdd(material);
        gNet_score_materials[material_index] = material;
    }

    gNet_score_camera = BrActorAllocate(3, NULL);
    gNet_score_camera->identifier = "Net HUD Camera";
    camera = (br_camera*)gNet_score_camera->type_data;
    camera->type = 0;
    camera->hither_z = 1.0f;
    camera->yon_z = 3.0f;
    camera->width = (float)gScreen->width;
    camera->height = (float)gScreen->height;
    BrActorAdd(gNet_score_root, gNet_score_camera);
    gNet_score_scene_initialised = 1;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004a69f0
void C2_HOOK_FASTCALL NetworkNoOp(void) {
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 74.56% | difftest PASS 2000 trials (spec difftest/specs/004a48d0_FUN_004a48d0.json) | reason VC5 register allocation and global-table operand layout
// FUNCTION: CARMA2_HW 0x004a48d0
void C2_HOOK_FASTCALL FUN_004a48d0(void* pPayload, void* pMessage) {
    register int iVar1;
    register int iVar3;
    register int iVar4;
    register volatile int* piVar5;
    tNet_game_player_info* player;

    if (gCurrent_net_game != NULL) {
        iVar3 = gINT_0068d964;
        while (1) {
            if (*(int*)((tU8*)pPayload + 4) <= 0) {
                gINT_0068d988 = 1;
                if (*(int*)((tU8*)pPayload + 4) >= 0) {
                    return;
                }
                gINT_00690234 = 1;
                return;
            }
            if (iVar3 > *(int*)((tU8*)pPayload + 0xc)) {
                return;
            }
            if (gINT_00690c30 == 0) {
                gINT_0068d974 = *(int*)((tU8*)pMessage + 0x10);
                piVar5 = (int*)&gReceived_net_player_list[0].car_index;
                do {
                    *piVar5 = -1;
                    piVar5 += 0x35;
                } while ((int)piVar5 < (int)((tU8*)gReceived_net_player_list + 0xa3c));
                gINT_00690c30 = 1;
                iVar3 = *(int*)((tU8*)pPayload + 0xc);
                gINT_0068d964 = iVar3;
            }
            if (iVar3 >= *(int*)((tU8*)pPayload + 0xc)) {
                break;
            }
            gINT_00690c30 = 0;
        }
        memcpy((tU8*)gReceived_net_player_list + *(int*)((tU8*)pPayload + 8) * 0xd4,
            (tU8*)pPayload + 0x10, sizeof(tNet_game_player_info));
        iVar3 = 0;
        if (0 < *(int*)((tU8*)pPayload + 4)) {
            piVar5 = (int*)&gReceived_net_player_list[0].car_index;
            do {
                if (*piVar5 < 0) {
                    return;
                }
                iVar3++;
                piVar5 += 0x35;
            } while (iVar3 < *(int*)((tU8*)pPayload + 4));
        }
        gINT_00690c30 = 0;
        NetPlayersChanged(*(int*)((tU8*)pPayload + 4), gReceived_net_player_list);
        gThis_net_player_index = -1;
        iVar4 = 0;
        if (0 < gNumber_of_net_players) {
            player = gNet_players;
            do {
                if (player->ID == gLocal_net_ID) {
                    gThis_net_player_index = iVar4;
                    break;
                }
                iVar4++;
                player++;
            } while (iVar4 < gNumber_of_net_players);
        }
        if (gThis_net_player_index < 0) {
            FatalError(0x69);
        }
        gNet_players[0].this_players_time_stamp = PDGetTotalTime();
        iVar3 = gNumber_of_net_players;
        gINT_00762430 = gNumber_of_net_players;
        if (gStart_race_sent != 0 && (iVar4 = 0, 0 < gNumber_of_net_players)) {
            piVar5 = (int*)&gNet_players[0].opponent_list_index;
            do {
                iVar1 = *piVar5;
                piVar5 += 0x35;
                gINT_00763088[iVar1 * 4] = iVar4;
                iVar4++;
            } while (iVar4 < iVar3);
        }
    }
}
#pragma auto_inline(on)


/* Rebuild after the Timers_Pop stack index correction. */
