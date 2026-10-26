#include "racemem.h"

#include "carpocalypse2_macros.h"

#include "c2_string.h"
// GLOBAL: CARMA2_HW 0x006ba640
tU8 gRace_memory[1000];

// FUNCTION: CARMA2_HW 0x004e74b0
void C2_HOOK_FASTCALL ClearAllRuntimeVariables(void) {
    C2_HOOK_BUG_ON(sizeof(gRace_memory) != 1000);
    memset(gRace_memory, 0, sizeof(gRace_memory));
}

// C2PORT-NONMATCHING: best 100% | difftest PASS 2000 trials (spec difftest/specs/004e7440_GetRuntimeVariable.json) | reason signed byte result and invalid-path EAX preservation
// FUNCTION: CARMA2_HW 0x004e7440
int C2_HOOK_FASTCALL GetRuntimeVariable(int pAddress) {

    if (pAddress >= 0 && pAddress < (int)CARPOCALYPSE2_ASIZE(gRace_memory)) {
        return (tS8)gRace_memory[pAddress];
    }
}

// FUNCTION: CARMA2_HW 0x004e7480
int C2_HOOK_FASTCALL ToggleRuntimeVariable(int pAddress) {

    if (pAddress >= 0 && pAddress < (int)CARPOCALYPSE2_ASIZE(gRace_memory)) {
        if (gRace_memory[pAddress]) {
            gRace_memory[pAddress] = 0;
            return 0;
        }
        gRace_memory[pAddress] = 1;
        return 1;
    }
}

// C2PORT-NONMATCHING: best 26.67% | difftest PASS 2000 trials (spec difftest/specs/004e7460_SetRuntimeVariable.json) | reason VC5 uses DL as the byte accumulator and folds the upper bound into an unsigned branch
// FUNCTION: CARMA2_HW 0x004e7460
int C2_HOOK_FASTCALL SetRuntimeVariable(int pAddress, int pValue) {

    if (pAddress >= 0 && pAddress < (int)CARPOCALYPSE2_ASIZE(gRace_memory)) {
        return (tS8)(gRace_memory[pAddress] += (tU8)pValue);
    }
    /* the original leaves EAX unchanged here; both callers ignore the result */
}
