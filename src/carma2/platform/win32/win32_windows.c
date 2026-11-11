#include "win32_windows.h"

#include "win32.h"

#include "main.h"
#include "errors.h"
#include "packfile.h"
#include "platform.h"

#include <windows.h>

// C2PORT-NONMATCHING: best 45.03% | difftest PASS 2000 trials (spec difftest/specs/0051cad0_SDL3ServiceMessages.json) | reason vc5-frame-register-allocation
// FUNCTION: CARMA2_HW 0x0051cad0
void C2_HOOK_CDECL SDL3ServiceMessages(void) {
    MSG msg;

    dr_dprintf("Win32ServiceMessages() - START");
    for (;;) {
        if (gWindowActiveState == 1) SetForegroundWindow(gHWnd);
        if (gWindowActiveState != 0) {
            if (PeekMessageA(&msg, NULL, 0, 0, 1) == 0) goto peek_message_empty;
            if (gWindowActiveState != 0) goto dispatch;
        }
        if (GetMessageA(&msg, NULL, 0, 0) == -1) goto get_message_error;
dispatch:
        if (msg.message == WM_QUIT) {
            dr_dprintf("WM_QUIT received.");
            if (gWindowActiveState == 2) {
                dr_dprintf("Active, so lock the surface");
                dr_dprintf("QuitGame being called...");
                QuitGame();
            }
            PDShutdownSystem();
        }
        TranslateMessage(&msg);
        dr_dprintf("Win32ServiceMessages() - dispatching message...");
        DispatchMessageA(&msg);
    }
peek_message_empty:
    dr_dprintf("Win32ServiceMessages() - breaking cos PeekMessage() returned 0");
    goto finish;
get_message_error:
    dr_dprintf("Win32ServiceMessages() - breaking cos GetMessage() returned -1");
finish:
    dr_dprintf("Win32ServiceMessages() - END");
}


// FUNCTION: CARMA2_HW 0x0044c830
void C2_HOOK_FASTCALL PDDisposeActionReplayBuffer(void) {

    if (gActionReplayBufferFile != NULL) {
        PFfclose(gActionReplayBufferFile);
        gActionReplayBufferFile = NULL;
    }
}
