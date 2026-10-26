/* The damage HUD is repainted before every draw, often with identical bytes.
 * Compare the full small image so blinking/damage changes still reach the GPU.
 * A bounded snapshot avoids allocation failures and hash collisions. */
#include "vita_hud.h"
#include <string.h>
static unsigned char previous[64 * 1024];
static size_t previous_size;
static int valid;
void C2VitaHUDReset(void) {valid = 0;}
int C2VitaDamageHUDChanged(const void* pixels, size_t bytes) {
    if (!pixels || !bytes || bytes > sizeof(previous)) {
        valid = 0;
        return 1;
    }
    if (valid && bytes == previous_size && !memcmp(previous, pixels, bytes)) return 0;
    memcpy(previous, pixels, bytes);
    previous_size = bytes;
    valid = 1;
    return 1;
}
