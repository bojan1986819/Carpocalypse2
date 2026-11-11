#include "frontend_networksummary.h"

#include "font.h"
#include "frontend.h"
#include "frontend_main.h"
#include "frontend_quit.h"
#include "globvars.h"
#include "globvrpb.h"
#include "globvrbm.h"
#include "graphics.h"
#include "input.h"
#include "intrface.h"
#include "loading1.h"
#include "network.h"
#include "polyfont.h"
#include "utility.h"

#include "c2_stdlib.h"
#include "c2_string.h"
// GLOBAL: CARMA2_HW 0x00686ec0
int gPlayer_lookup_netsummary[14];

// GLOBAL: CARMA2_HW 0x006864fc
int gFrontend_netsummary_first_iteration;


// FUNCTION: CARMA2_HW 0x004743c0
static int C2_HOOK_CDECL SortScores(const void* pArg1, const void* pArg2) {

    return gNet_players[*(int*)pArg1].score - gNet_players[*(int*)pArg2].score;
}

// FUNCTION: CARMA2_HW 0x00473f40
int C2_HOOK_FASTCALL NetSummary_Infunc(tFrontend_spec* pFrontend) {
    int i;

    if (!pFrontend->loaded) {
        LoadMenuSettings(pFrontend);
        FuckWithWidths(pFrontend);
        if (pFrontend->previous != NULL) {
            pFrontend->previous->isPreviousSomeOtherMenu = 1;
        }
    }
    gFrontend_scrollbars_updated = 0;
    EdgeTriggerModeOff();
    WaitForNoKeys();
    EdgeTriggerModeOn();
    gCurrent_frontend_scrollbars = NULL;
    gConnected_items = NULL;
    gPTR_00686508 = NULL;
    strcpy(pFrontend->items[60].text, GetMiscString(eMiscString_player));
    strcpy(pFrontend->items[61].text, GetMiscString(eMiscString_played));
    strcpy(pFrontend->items[62].text, GetMiscString(eMiscString_won));
    strcpy(pFrontend->items[63].text, GetMiscString(eMiscString_score));
    strcpy(pFrontend->items[64].text, IString_Get(182));

    for (i = 0; i < gNumber_of_net_players; i++) {
        gPlayer_lookup_netsummary[i] = i;
    }
    qsort(gPlayer_lookup_netsummary, gNumber_of_net_players, sizeof(int), SortScores);
    for (i = 0; i < gNumber_of_net_players; i++) {
        tNet_game_player_info* player = &gNet_players[gPlayer_lookup_netsummary[i]];

        pFrontend->items[ 0 + i].visible = 1;
        pFrontend->items[12 + i].visible = 1;
        pFrontend->items[24 + i].visible = 1;
        pFrontend->items[36 + i].visible = 1;
        pFrontend->items[48 + i].visible = 1;
        pFrontend->items[65 + i].visible = 1;
        strcpy(pFrontend->items[0 + i].text, player->player_name);
        if (player->host) {
            strcat(pFrontend->items[0 + i].text, " (");
            strcat(pFrontend->items[0 + i].text, GetMiscString(eMiscString_host));
            strcat(pFrontend->items[0 + i].text, ")");
        }
        sprintf(pFrontend->items[12 + i].text, gSkip_lines_format, player->field_0x7c);
        sprintf(pFrontend->items[24 + i].text, gSkip_lines_format, player->field_0x88);
        sprintf(pFrontend->items[36 + i].text, gSkip_lines_format, player->score);
        sprintf(pFrontend->items[48 + i].text, "%d%%",
                   (int)((double)player->score / (double)player->field_0x7c / gScore_winner[10] * 100.0));
    }
    for (i = gNumber_of_net_players; i < kMax_netplayers; i++) {
        pFrontend->items[ 0 + i].text[0] = '\0';
        pFrontend->items[12 + i].text[0] = '\0';
        pFrontend->items[24 + i].text[0] = '\0';
        pFrontend->items[36 + i].text[0] = '\0';
        pFrontend->items[48 + i].text[0] = '\0';
        pFrontend->items[ 0 + i].visible = 0;
        pFrontend->items[12 + i].visible = 0;
        pFrontend->items[24 + i].visible = 0;
        pFrontend->items[36 + i].visible = 0;
        pFrontend->items[48 + i].visible = 0;
        pFrontend->items[65 + i].visible = 0;
    }

    if (gCurrent_net_game->options.open_game) {
        sprintf(pFrontend->items[78].text, "%s: %s (%s)",
            GetMiscString(eMiscString_game_type),
            GetMiscString(eMiscString_network_type_start + gCurrent_net_game->type),
            GetMiscString(eMiscString_open));
    } else {
        sprintf(pFrontend->items[78].text, "%s: %s (%s)",
            GetMiscString(eMiscString_game_type),
            GetMiscString(eMiscString_network_type_start + gCurrent_net_game->type),
            GetMiscString(eMiscString_closed));
    }
    for (i = 0; i < pFrontend->count_items; i++) {
        pFrontend->items[i].visible = 1;
    }
    FuckWithWidths(pFrontend);
    gMouse_in_use = 0;
    gFrontend_netsummary_first_iteration = 1;
    return 1;
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x0044bac0
void C2_HOOK_FASTCALL DrawThisCarIconNow(int pCar_index, int pX, int pY) {

    gCar_icons_model_actor->material->colour_map = gTexture_maps[gCar_icons[pCar_index].index];
    BrMaterialUpdate(gCar_icons_model_actor->material, BR_MATU_COLOURMAP);
    gCar_icons_model_actor->model = gCar_icons[pCar_index].model;
    BrMatrix34Translate(&gCar_icons_model_actor->t.t.mat, (float)pX, (float)-pY, 0.f);
    BrActorAdd(g2d_camera, gCar_icons_model_actor);
    BrZbsSceneRender(g2d_camera, g2d_camera, gBack_screen, gDepth_buffer);
    BrActorRemove(gCar_icons_model_actor);
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL NetSummary_Draw(tFrontend_spec* pFrontend) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        int player = gPlayer_lookup_netsummary[i];

        pFrontend->items[0 + player].visible = 1;
        pFrontend->items[12 + player].visible = 1;
        pFrontend->items[24 + player].visible = 1;
        pFrontend->items[36 + player].visible = 1;
        pFrontend->items[48 + player].visible = 1;
        pFrontend->items[65 + player].visible = 1;
        DrawThisCarIconNow(gNet_players[player].car_index, 88, 112 + 24 * i);
    }

    for (i = gNumber_of_net_players; i < kMax_netplayers; i++) {

        pFrontend->items[0 + i].visible = 0;
        pFrontend->items[12 + i].visible = 0;
        pFrontend->items[24 + i].visible = 0;
        pFrontend->items[36 + i].visible = 0;
        pFrontend->items[48 + i].visible = 0;
        pFrontend->items[65 + i].visible = 0;
    }
}

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x00474400
int C2_HOOK_FASTCALL NetSummary_MenuHandler(tFrontend_spec* pFrontend) {
#ifdef CARPOCALYPSE2_MATCHING
    int result;
    int i;
    int player_index;

    if (gFrontend_netsummary_first_iteration) {

        gFrontend_selected_item_index = 79;
        gFrontend_netsummary_first_iteration = 0;
    }
    if (gProgram_state.prog_status == eProg_idling) {
        return 1;
    }
    result = Generic_MenuHandler(pFrontend);
    for (i = 0; i < gNumber_of_net_players; i++) {

        player_index = gPlayer_lookup_netsummary[i];
        pFrontend->items[player_index].visible = 1;
        pFrontend->items[12 + player_index].visible = 1;
        pFrontend->items[24 + player_index].visible = 1;
        pFrontend->items[36 + player_index].visible = 1;
        pFrontend->items[48 + player_index].visible = 1;
        pFrontend->items[65 + player_index].visible = 1;
        DrawThisCarIconNow(gNet_players[player_index].car_index, 88, 112 + 24 * i);
    }
    for (i = gNumber_of_net_players; i < kMax_netplayers; i++) {

        pFrontend->items[0 + i].visible = 0;
        pFrontend->items[12 + i].visible = 0;
        pFrontend->items[24 + i].visible = 0;
        pFrontend->items[36 + i].visible = 0;
        pFrontend->items[48 + i].visible = 0;
        pFrontend->items[65 + i].visible = 0;
    }
    return result;
#else
    int result;

    if (gFrontend_netsummary_first_iteration) {

        gFrontend_selected_item_index = 79;
        gFrontend_netsummary_first_iteration = 0;
    }
    if (gProgram_state.prog_status == eProg_idling) {
        return 1;
    }
    result = Generic_MenuHandler(pFrontend);
    NetSummary_Draw(pFrontend);
    return result;
#endif
}
#pragma inline_depth()
