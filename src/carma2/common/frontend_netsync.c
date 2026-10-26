#include "frontend_netsync.h"

#include "frontend.h"
#include "frontend_main.h"
#include "interface.h"
#include "frontend_network.h"
#include "frontend_networksummary.h"
#include "frontend_quit.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "input.h"
#include "main.h"
#include "netgame.h"
#include "network.h"
#include "platform.h"
#include "racestrt.h"
#include "utility.h"

#include <stdio.h>
#include "c2_string.h"
// GLOBAL: CARMA2_HW 0x006864d0
tU32 gNet_synch_start;


// FUNCTION: CARMA2_HW 0x00473570
int C2_HOOK_FASTCALL NetSynchRaceStart2(tNet_synch_mode pMode) {

    if (pMode != eNet_synch_client) {
        if (gCurrent_net_game->status.stage == 0) {
            gCurrent_net_game->status.stage = 1;
        }
        SetUpNetCarPositions();
        gNet_synch_start = PDGetTotalTime();
    }
    return 3;
}

// FUNCTION: CARMA2_HW 0x00473610
int C2_HOOK_FASTCALL NetSync_Infunc(tFrontend_spec* pFrontend) {
    int i;
    int zero = 0;
    int one = 1;
    int now;
    int* player_status;
    tNet_message* message;

    int first = *(int*)((char*)pFrontend + 0xad1c);
    if (first == zero) {
        LoadMenuSettings(pFrontend);
        FuckWithWidths(pFrontend);
        first = *(int*)((char*)pFrontend + 0x114);
        if (first != zero) {
            *(int*)(first + 0xb8c0) = one;
        }
    }
    gFrontend_scrollbars_updated = zero;
    EdgeTriggerModeOff();
    WaitForNoKeys();
    EdgeTriggerModeOn();
    gCurrent_frontend_scrollbars = NULL;
    gConnected_items = NULL;
    gPTR_00686508 = NULL;
    now = PDGetTotalTime();
    i = zero;
    if (gNumber_of_net_players > zero) {
        player_status = (int*)((char*)gNet_players + 0x48);
        do {
            if (i != gThis_net_player_index && (tU32)(now - player_status[-13]) > 60000) {
                *player_status = 5;
            }
            one = 1;
            i++;
            player_status += 0x35;
        } while (i < gNumber_of_net_players);
    }
    if (gNet_mode == 3 && gLast_host_query == zero) {
        message = NetBuildGuaranteedMessage(0xe, zero);
        NetGuaranteedSendMessageToHost(gCurrent_net_game, message, (void*)zero);
        gLast_host_query = now;
    }
    if (gFrontend_net_mode == 2) {
        if (gNo_races_yet != zero) {
            if (gCurrent_net_game->status.stage == zero) {
                gCurrent_net_game->status.stage = one;
            }
        } else if (gCurrent_net_game->status.stage == zero) {
            gCurrent_net_game->status.stage = one;
        }
        SetUpNetCarPositions();
        gNet_synch_start = PDGetTotalTime();
    }
    player_status = (int*)((char*)pFrontend + 0x3f8);
    i = 0xc;
    do {
        *player_status = 0xa0;
        player_status += 0x56;
        i--;
    } while (i != zero);
    if (gCurrent_net_game->options.open_game == zero) {
        sprintf(pFrontend->items[1].text, "%s: %s (%s)",
            GetMiscString(0x47),
            GetMiscString(gCurrent_net_game->type + 0x3b),
            GetMiscString(0xcf));
    } else {
        sprintf(pFrontend->items[1].text, "%s: %s (%s)",
            GetMiscString(0x48),
            GetMiscString(gCurrent_net_game->type + 0x3b),
            GetMiscString(0xcf));
    }
    if (gNet_mode == 2) {
        if (gNo_races_yet != zero) {
            *(int*)((char*)pFrontend + 0x2448) = one;
            *(int*)((char*)pFrontend + 0x244c) = one;
            *(int*)((char*)pFrontend + 0x25a0) = one;
            *(int*)((char*)pFrontend + 0x25a4) = one;
            *(int*)((char*)pFrontend + 0x2420) = 0xe;
            *(int*)((char*)pFrontend + 0x2578) = 0x15;
            FuckWithWidths(pFrontend);
            return zero;
        }
        *(int*)((char*)pFrontend + 0x2448) = -1;
        *(int*)((char*)pFrontend + 0x244c) = zero;
        *(int*)((char*)pFrontend + 0x25a0) = one;
        *(int*)((char*)pFrontend + 0x25a4) = one;
        *(int*)((char*)pFrontend + 0x2578) = 0x15;
        FuckWithWidths(pFrontend);
        return zero;
    }
    *(int*)((char*)pFrontend + 0x2448) = -1;
    *(int*)((char*)pFrontend + 0x244c) = zero;
    *(int*)((char*)pFrontend + 0x25a0) = one;
    *(int*)((char*)pFrontend + 0x25a4) = one;
    *(int*)((char*)pFrontend + 0x2578) = 0xb3;
    FuckWithWidths(pFrontend);
    return zero;
}

void C2_HOOK_FASTCALL MungePlayers(tFrontend_spec* pFrontend) {
    int count_ready = 0;
    int i;

    CheckPlayersAreResponding();
    NetPlayerStatusChanged(ePlayer_status_ready);
    for (i = 0; i < gNumber_of_net_players; i++) {

        pFrontend->items[ 2 + i].visible = 1;
        pFrontend->items[ 2 + i].enabled = kFrontendItemEnabled_default;
        pFrontend->items[14 + i].visible = 1;
        pFrontend->items[14 + i].enabled = kFrontendItemEnabled_default;
        if (gNet_players[i].host) {
            sprintf(pFrontend->items[ 2 + i].text, "%s (%s)",
                gNet_players[i].player_name, IString_Get(154));
        } else {
            strcpy(pFrontend->items[ 2 + i].text, gNet_players[i].player_name);
        }
        if (gNet_players[i].player_status == ePlayer_status_ready) {
            count_ready += 1;
        }
    }
    for (i = gNumber_of_net_players; i < kMax_netplayers; i++) {

        pFrontend->items[ 2 + i].visible = 0;
        pFrontend->items[ 2 + i].enabled = kFrontendItemEnabled_default;
        pFrontend->items[14 + i].visible = 0;
        pFrontend->items[14 + i].enabled = kFrontendItemEnabled_default;
    }
    if (gNet_mode == eNet_mode_host
            && count_ready == gNumber_of_net_players
            && gNumber_of_net_players > 1
            && (!gNo_races_yet || gNumber_of_net_players == kMax_netplayers)) {

        SignalToStartRace();
        gStart_race_sent = 1;
    }
}

void C2_HOOK_FASTCALL NetSync_Draw(tFrontend_spec* pFrontend) {
    int i;
    char s[128];

    for (i = 0; i < gNumber_of_net_players; i++) {

        DrawThisCarIconNow(gNet_players[i].car_index, 115, 99 + 26 * i);
        SolidPolyFontText(GetMiscString(eMiscString_player_status_start + gNet_players[i].player_status),
            520, 96 + 26 * i, pFrontend->items[2 + i].unlitFont, eJust_right, 1);
    }
    if (gNet_mode != eNet_mode_host) {
        sprintf(s, "...%s...", IString_Get(250));
        SolidPolyFontText(s, 320, 96 + 26 * gNumber_of_net_players,
            pFrontend->items[2].unlitFont, eJust_centre, 1);
    }
}

// FUNCTION: CARMA2_HW 0x00473870
int C2_HOOK_FASTCALL NetSync_MenuHandler(tFrontend_spec* pFrontend) {
    int key;
    int mouse_down;
    int mouse_item;
    int original;

    if (gProgram_state.prog_status == eProg_idling) {
        return 1;
    }
    original = gFrontend_selected_item_index;
    gTyping_slot = -1;
    EdgeTriggerModeOn();
    KillSplashScreen();
    PollKeys();
    EdgeTriggerModeOff();
    key = PDAnyKeyDown();
    if (key != -1 && key != 4) {
        gMouse_in_use = 0;
        gFrontend_time_last_input = PDGetTotalTime();
    }
    EdgeTriggerModeOn();
    mouse_down = 0;
    if (gMouse_in_use) {
        int mouse_x;
        int mouse_y;

        gFrontend_time_last_input = PDGetTotalTime();
        GetMousePosition(&mouse_x, &mouse_y);
        mouse_down = EitherMouseButtonDown();
        mouse_item = GetItemAtMousePos(gCurrent_frontend_spec, mouse_x, mouse_y);
        gFrontend_selected_item_index = mouse_item;
        if (mouse_item == -1) {
            gFrontend_selected_item_index = 99;
        }
    }
    if (PDKeyDown(72) || PDKeyDown(89))  {
        int i;

        for (i = gFrontend_selected_item_index - 1; i >= 0; i--) {
            if (gCurrent_frontend_spec->items[i].enabled > 0 && gCurrent_frontend_spec->items[i].visible) {
                break;
            }
        }
        if (i < 0) {
            i = FindPrevActiveItem(gCurrent_frontend_spec, gCurrent_frontend_spec->count_items);
        }
        gFrontend_selected_item_index = i;
        if (!gMouse_in_use
                && gFrontend_selected_item_index >= gCurrent_frontend_spec->scrollers[0].indexFirstScrollableItem
                && gFrontend_selected_item_index <= gCurrent_frontend_spec->scrollers[0].indexLastScrollableItem) {
            ToggleSelection(pFrontend);
            if (pFrontend->items[gFrontend_selected_item_index].action != NULL) {
                pFrontend->items[gFrontend_selected_item_index].action(pFrontend);
            }
        }
    }
    if (PDKeyDown(73) || PDKeyDown(83)) {
        int i;

        if (gFrontend_selected_item_index < pFrontend->count_items - 1) {
            i = gFrontend_selected_item_index + 1;
        } else {
            gFrontend_selected_item_index = -1;
            i = 0;
        }
        for (; i < gCurrent_frontend_spec->count_items; i++) {
            if (gCurrent_frontend_spec->items[i].enabled > 0 && gCurrent_frontend_spec->items[i].visible) {
                break;
            }
        }
        if (i >= gCurrent_frontend_spec->count_items) {
            i = 0;
        }
        gFrontend_selected_item_index = i;
        if (!gMouse_in_use
                && gFrontend_selected_item_index >= gCurrent_frontend_spec->scrollers[0].indexFirstScrollableItem
                && gFrontend_selected_item_index <= gCurrent_frontend_spec->scrollers[0].indexLastScrollableItem) {
            ToggleSelection(pFrontend);
            if (pFrontend->items[gFrontend_selected_item_index].action != NULL) {
                pFrontend->items[gFrontend_selected_item_index].action(pFrontend);
            }
        }
    }
    if (PDKeyDown(51) || PDKeyDown(52) || (mouse_down == 1 && mouse_item != -1)) {
        int result;

        result = pFrontend->items[original].field_0xc;
        ToggleSelection(pFrontend);
        if (pFrontend->items[gFrontend_selected_item_index].field_0xc == 2) {
            return 2;
        }
        if (pFrontend->items[gFrontend_selected_item_index].action != NULL) {
            result = pFrontend->items[gFrontend_selected_item_index].action(pFrontend);
        }
        if (pFrontend->items[gFrontend_selected_item_index].menuInfo != NULL) {
            int i;

            gCurrent_frontend_spec->default_item = gFrontend_selected_item_index;
            for (i = 0; i < gCurrent_frontend_spec->count_scrollers; i++) {
                gCurrent_frontend_spec->scrollers[i].indexOfItemAtTop = gCurrent_frontend_spec->scrollers[i].indexTopItem;
            }
            FRONTEND_DestroyMenu(pFrontend);
            gCurrent_frontend_spec = pFrontend->items[gFrontend_selected_item_index].menuInfo;
            FRONTEND_CreateMenu(gCurrent_frontend_spec);
            if (gCurrent_frontend_spec != pFrontend->previous) {
                gCurrent_frontend_spec->previous = pFrontend;
            }
            Morph_Initialise(pFrontend, gCurrent_frontend_spec);
            if (gCurrent_frontend_spec == &gFrontend_QUIT) {
                gFrontend_selected_item_index = 0;
            } else {
                gFrontend_selected_item_index = gCurrent_frontend_spec->default_item;
            }
            for (i = 0; i < gCurrent_frontend_spec->count_scrollers; i++) {
                gCurrent_frontend_spec->scrollers[i].indexTopItem = gCurrent_frontend_spec->scrollers[i].indexOfItemAtTop;
            }
            UpdateScrollPositions(gCurrent_frontend_spec);
        }
        return result;
    }

    ServiceGame();
    {
        int count_ready = 0;
        int i;

        tU32 now;
        tNet_message* message;

        now = PDGetTotalTime();
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (i != gThis_net_player_index && now - gNet_players[i].this_players_time_stamp > 60000) {
                gNet_players[i].player_status = ePlayer_status_not_responding;
            }
        }
        if (gNet_mode == eNet_mode_client && gLast_host_query == 0) {
            message = NetBuildGuaranteedMessage(eNet_message_chunk_type_netsynch, 0);
            NetGuaranteedSendMessageToHost(gCurrent_net_game, message, NULL);
            gLast_host_query = now;
        }
        NetPlayerStatusChanged(ePlayer_status_ready);
        for (i = 0; i < gNumber_of_net_players; i++) {

            pFrontend->items[ 2 + i].visible = 1;
            pFrontend->items[14 + i].visible = 1;
            pFrontend->items[ 2 + i].enabled = kFrontendItemEnabled_default;
            pFrontend->items[14 + i].enabled = kFrontendItemEnabled_default;
            if (!gNet_players[i].host) {
                strcpy(pFrontend->items[ 2 + i].text, gNet_players[i].player_name);
            } else {
                sprintf(pFrontend->items[ 2 + i].text, "%s (%s)",
                    gNet_players[i].player_name, IString_Get(154));
            }
            if (gNet_players[i].player_status == ePlayer_status_ready) {
                count_ready += 1;
            }
        }
        for (i = gNumber_of_net_players; i < kMax_netplayers; i++) {

            pFrontend->items[ 2 + i].visible = 0;
            pFrontend->items[14 + i].visible = 0;
            pFrontend->items[ 2 + i].enabled = kFrontendItemEnabled_default;
            pFrontend->items[14 + i].enabled = kFrontendItemEnabled_default;
        }
        if (gNet_mode == eNet_mode_host
                && count_ready == gNumber_of_net_players
                && gNumber_of_net_players > 1
                && (!gNo_races_yet || gNumber_of_net_players == kMax_netplayers)) {

            SignalToStartRace();
            gStart_race_sent = 1;
        }
    }
    {
        int i;
        char s[128];

        for (i = 0; i < gNumber_of_net_players; i++) {

            DrawThisCarIconNow(gNet_players[i].car_index, 115, 99 + 26 * i);
            SolidPolyFontText(GetMiscString(eMiscString_player_status_start + gNet_players[i].player_status),
                520, 96 + 26 * i, pFrontend->items[2 + i].unlitFont, eJust_right, 1);
        }
        if (gNet_mode != eNet_mode_host) {
            sprintf(s, "...%s...", IString_Get(250));
            SolidPolyFontText(s, 320, 96 + 26 * i,
                pFrontend->items[2].unlitFont, eJust_centre, 1);
        }
    }
    FuckWithWidths(pFrontend);
    if (gFrontend_leave_current_menu || gStart_race_sent || gProgram_state.prog_status == eProg_game_starting) {
        return 1;
    } else {
        return 0;
    }
}

// FUNCTION: CARMA2_HW 0x004735a0
int C2_HOOK_FASTCALL NetSync_Start(tFrontend_spec* pFrontend) {

    if (gNumber_of_net_players <= 1) {
        return 0;
    }
    SignalToStartRace();
    gStart_race_sent = 1;
    gNo_races_yet = 0;
    return 1;
}

// FUNCTION: CARMA2_HW 0x004735d0
int C2_HOOK_FASTCALL NetSync_Abort(tFrontend_spec* pFrontend) {

    if (gProgram_state.racing || gNet_mode != eNet_mode_none) {

        if (gProgram_state.racing) {
            gAbandon_game = 1;
        }
        gProgram_state.prog_status = eProg_idling;
        gNo_current_game = 1;
    }
    return 1;
}