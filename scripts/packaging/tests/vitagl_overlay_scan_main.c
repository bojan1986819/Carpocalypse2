#include "overlay_scan.h"
#include <assert.h>
#include <stdio.h>
static uint16_t pixels[640*480+32];
static int scalar(const uint16_t* p,size_t n,uint16_t key) {
    for(size_t i=0;i<n;++i) if(p[i]!=key) return 0;
    return 1;
}
int main(void) {
    const uint16_t keys[]={0,0xffff,0xf81f,0x8001};
    assert(GLOverlayAllKey(NULL,0,123));GLOverlayFillKey(NULL,0,123);
    for(unsigned k=0;k<4;++k) for(size_t offset=1;offset<=8;++offset) {
        uint16_t* p=pixels+offset;
        const size_t boundaries[]={255,256,257,511,512,513,767,768,769,1023,1024,1025};
        for(size_t sample=0;sample<130+sizeof(boundaries)/sizeof(boundaries[0]);++sample) {
            size_t n=sample<130?sample:boundaries[sample-130];
            pixels[offset-1]=0xdead;p[n]=0xbeef;
            GLOverlayFillKey(p,n,keys[k]);
            assert(pixels[offset-1]==0xdead && p[n]==0xbeef);
            assert(GLOverlayAllKey(p,n,keys[k]));
            for(size_t i=0;i<n;++i) {
                p[i]^=1;assert(!GLOverlayAllKey(p,n,keys[k]));p[i]^=1;
            }
        }
        size_t n=640*480;
        GLOverlayFillKey(p,n,keys[k]);assert(GLOverlayAllKey(p,n,keys[k]));
        for(unsigned trial=0;trial<40;++trial) {
            size_t pos=trial<8?trial:trial<16?n-1-(trial-8):(trial*2654435761u)%n;
            p[pos]^=(uint16_t)(1u<<(trial%16));
            assert(GLOverlayAllKey(p,n,keys[k])==scalar(p,n,keys[k]));
            p[pos]=keys[k];
        }
    }
    puts("PASS production overlay scan/fill: scalar and NEON-branch exact-key decisions, every small-buffer pixel/tail/alignment, full640x480 scattered single-bit changes and guard preservation; ASan/UBSan; host vector intrinsics emulate NEON semantics, no hardware timing claim");
}
