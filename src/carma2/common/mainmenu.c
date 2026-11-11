#include "mainmenu.h"

#include "frontend.h"
#include "flicplay.h"
#include "globvars.h"
#include "graphics.h"
#include "globvrpb.h"
#include "network.h"
#include "sound.h"
#include "mainmenu.h"
#include "intrface.h"
#include "loading.h"
#include "structur.h"
#include "utility.h"

#include "interface.h"
#include "interface-main.h"
#include "globvars.h"
#include <string.h>
#include <stdlib.h>
#include "platform.h"

#ifdef CARPOCALYPSE2_MATCHING
#include "c2_hooks.h"
#endif

// QuitVerifyStart

// QuitVerifyDone

// FUNCTION: CARMA2_HW 0x00494570
void C2_HOOK_FASTCALL DoOptionsMenu(void) {
    tPlayer_status player_status;

#ifdef CARPOCALYPSE2_FIX_BUGS
    player_status = ePlayer_status_unknown;
#endif

    if (gNet_mode != eNet_mode_none) {
        player_status = gNet_players[gThis_net_player_index].player_status;
        NetPlayerStatusChanged(ePlayer_status_main_menu);
    }
    if (FrontEndProcessMenus(kFrontend_menu_options) == 0) {
        gProgram_state.prog_status = eProg_quit;
    }
    if (gNet_mode != eNet_mode_none && gNet_players[gThis_net_player_index].player_status == ePlayer_status_main_menu) {
        NetPlayerStatusChanged(player_status);
    }
}
// GLOBAL: CARMA2_HW 0x0068c850
int gMouse_was_started__mainmenu;

// GLOBAL: CARMA2_HW 0x0068c854
int gReplace_background;

// GLOBAL: CARMA2_HW 0x00659270
tInterface_spec gQuit_verify_interface;

// FUNCTION: CARMA2_HW 0x00494400
int C2_HOOK_FASTCALL QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    (void)pCurrent_mode;
    (void)pGo_ahead;
    (void)pEscaped;
    (void)pTimed_out;

    FadePaletteDown();
    TurnOnPanelFlics();
    if (gMouse_was_started__mainmenu != 0) {
        RemoveTransientBitmaps(1);
    }
    if (gReplace_background != 0) {
        if (pCurrent_choice == 0) {
            ClearEntireScreen();
        }
        PDScreenBufferSwap(0);
        FadePaletteUp();
    }
    return pCurrent_choice == 0;
}

// FUNCTION: CARMA2_HW 0x00494450
int C2_HOOK_FASTCALL DoVerifyQuit(int pReplace_background) {
    int was_racing;
    int random;
    int result;

    was_racing = 0;
    if (gAusterity_mode != 0) {
        return 1;
    }
    if (gProgram_state.racing != 0) {
        FadePaletteDown();
        ClearEntireScreen();
        GoingToInterfaceFromRace();
        was_racing = 1;
        gProgram_state.racing = 0;
    }
    random = IRandomBetween(1, 3);
    switch (random) {
    case 1:
        gQuit_verify_interface.first_opening_flic = 0x82;
        break;
    case 2:
        gQuit_verify_interface.first_opening_flic = 0x83;
        break;
    case 3:
        gQuit_verify_interface.first_opening_flic = 0x84;
        break;
    }
    gMouse_was_started__mainmenu = gMouse_started;
    if (gMouse_started != 0) {
        RemoveTransientBitmaps(1);
        EndMouseCursor();
    }
    TurnOffPanelFlics();
    gReplace_background = pReplace_background;
    DRS3StopOutletSound(gEffects_outlet);
    LoadInterfaceStuff(was_racing);
    result = DoInterfaceScreen(&gQuit_verify_interface, 0, 0);
    if (gMouse_was_started__mainmenu != 0) {
        LoadInterfaceStuff(was_racing);
        StartMouseCursor();
    }
    if (was_racing != 0) {
        GoingBackToRaceFromInterface();
        gProgram_state.racing = 1;
    }
    return result;
}

#ifdef CARPOCALYPSE2_MATCHING
// FUNCTION: CARMA2_HW 0x00494540
int C2_HOOK_FASTCALL DoMainScreen(void) {
    int result;

    StartMusicTrack(0x270e);

    result = FrontEndProcessMenus(kFrontend_menu_main);

    switch (result) {
    case 0:
        gProgram_state.prog_status = eProg_quit;
        break;
    case 1:
        gProgram_state.prog_status = eProg_game_starting;
        break;
    }
}
#else
int C2_HOOK_FASTCALL DoMainScreen(void) {
    int result;

    {
        /* port test harness: C2_TEST_RACE=1 skips the frontend once and starts the current race */
        static int test_race_done;
        if (test_race_done && getenv("C2_TEST_RACES") != NULL && test_race_done < atoi(getenv("C2_TEST_RACES"))) {
            /* C2_TEST_RACES=<n>,<s> (mainloop.c TestRaces): the next of <n> races */
            test_race_done++;
            dr_dprintf("TEST RACES race %d", test_race_done);
            gProgram_state.prog_status = eProg_game_starting;
            return 1;
        }
        if (!test_race_done && getenv("C2_TEST_RACE") != NULL) {
            const char* index = getenv("C2_TEST_RACE_INDEX");
            if (index != NULL) {
                char* end;
                long race = strtol(index, &end, 10);
                if (end != index && *end == '\0' && race >= 0 && race < gNumber_of_races) {
                    gProgram_state.current_race_index = (int)race;
                    gCurrent_race_group = gRace_list[race].group;
                    dr_dprintf("TEST RACE index=%ld", race);
                }
            }
            test_race_done = 1;
#ifdef __vita__
            if (getenv("C2_TEST_RACE_DELAY") != NULL) {
                /* time a player spends in the menus (background loading gets it) */
                extern int sceKernelDelayThread(unsigned int delay);
                sceKernelDelayThread((unsigned int)atoi(getenv("C2_TEST_RACE_DELAY")) * 1000u);
            }
#endif
            gProgram_state.prog_status = eProg_game_starting;
            return 1;
        }
    }
    StartMusicTrack(0x270e);

    result = FrontEndProcessMenus(kFrontend_menu_main);

    if (result < 0) {
        gProgram_state.prog_status = eProg_quit;
        return 0;
    }
    if (result > 0) {
        gProgram_state.prog_status = eProg_game_starting;
    }
    return 1;
}
#endif

// DoOptionsMenu
