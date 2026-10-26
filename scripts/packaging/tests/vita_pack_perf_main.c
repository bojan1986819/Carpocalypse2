#include "vita_pack.h"
#include "vita_perf.h"
#include <SDL3/SDL.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned disk_reads,fail_allocation;
static Uint64 now;
static char logged[2048];
size_t pack_test_read(void* p,size_t size,size_t count,FILE* f) {
    ++disk_reads;return fread(p,size,count,f);
}
void* pack_test_malloc(size_t size) {return fail_allocation?NULL:malloc(size);}
Uint64 SDL_GetTicksNS(void) {return now;}
static unsigned messages;
void C2VitaLog(const char* format,...) {
    ++messages;
    va_list a;va_start(a,format);vsnprintf(logged,sizeof(logged),format,a);va_end(a);
}
/* Mock thread lifecycle; poll the production monitor deterministically. */
static unsigned threads_created, threads_joined;
static int fail_thread;
SDL_Thread* SDL_CreateThread(int (*fn)(void*),const char* name,void* arg) {
    assert(fn && name && !arg);++threads_created;return fail_thread?NULL:(SDL_Thread*)1;
}
void SDL_WaitThread(SDL_Thread* thread,int* status) {assert(thread==(SDL_Thread*)1 && !status);++threads_joined;}
void SDL_Delay(Uint32 ms) {(void)ms;}
bool SDL_SetCurrentThreadPriority(int priority) {assert(priority==SDL_THREAD_PRIORITY_LOW);return true;}
const char* SDL_GetError(void) {return "mock thread failure";}
Uint64 SDL_GetTicks(void) {return now/1000000;}
#include "vita_perf.c"
static FILE* source(size_t size,int byte) {
    FILE* f=tmpfile();assert(f);
    unsigned char block[4096];memset(block,byte,sizeof(block));
    while(size) {size_t n=size<sizeof(block)?size:sizeof(block);assert(fwrite(block,n,1,f)==1);size-=n;}
    rewind(f);return f;
}
static void read_pack(FILE* f,void* out,size_t size,const char* path) {
    rewind(f);assert(C2VitaReadPack(out,size,f,path)==1);assert(ftell(f)==(long)size);
}
int main(void) {
    enum {MB=1024*1024};
    unsigned char* out=malloc(5*MB);assert(out);
    FILE* a=source(MB,'a'),*b=source(2*MB,'b'),*c=source(3*MB,'c');
    read_pack(a,out,MB,"ux0:/data/A.TWT");assert(disk_reads==1 && out[3]=='a');
    memset(out,'x',MB); /* Mounted header fixups/ungetc must not corrupt cache. */
    for(unsigned i=0;i<100;++i) {read_pack(a,out,MB,"ux0:/DATA/a.twt");assert(out[3]=='a');}
    assert(disk_reads==1);
    read_pack(b,out,2*MB,"B.TWT");assert(disk_reads==2);
    read_pack(c,out,3*MB,"C.TWT");assert(disk_reads==3);
    C2VitaPackCacheLog();assert(strstr(logged,"resident=3145728 budget=4194304"));
    read_pack(a,out,MB,"ux0:/data/A.TWT");assert(disk_reads==4); /* evicted */
    /* Size change must re-read, not hit a stale entry. */
    read_pack(b,out,2*MB,"ux0:/data/A.TWT");assert(disk_reads==5 && out[0]=='b');
    C2VitaPackCacheClear();
    for(unsigned i=0;i<10;++i) {char path[24];snprintf(path,sizeof(path),"small%u.TWT",i);read_pack(a,out,32,path);}
    unsigned before=disk_reads;
    read_pack(a,out,32,"small0.TWT");assert(disk_reads==before+1); /* slot bound */
    fail_allocation=1;
    read_pack(a,out,32,"optional.TWT");before=disk_reads;
    read_pack(a,out,32,"optional.TWT");assert(disk_reads==before+1); /* fallback */
    fail_allocation=0;
    FILE* large=source(5*MB,'z');
    read_pack(large,out,5*MB,"large.TWT");before=disk_reads;
    read_pack(large,out,5*MB,"large.TWT");assert(disk_reads==before+1); /* over budget */
    rewind(a);before=disk_reads;
    assert(!C2VitaReadPack(out,2*MB,a,"short.TWT"));
    rewind(a);assert(!C2VitaReadPack(out,2*MB,a,"short.TWT") && disk_reads==before+2);
    C2VitaPackCacheClear();fclose(a);fclose(b);fclose(c);fclose(large);free(out);

    now=100000;C2VitaPerfReset();C2VitaPerfBegin(C2VP_RENDER);
    now+=10000;C2VitaPerfBegin(C2VP_WORLD);now+=10000;C2VitaPerfBegin(C2VP_WORLD);
    now+=10000;C2VitaPerfEnd(C2VP_WORLD);now+=10000;C2VitaPerfEnd(C2VP_WORLD);
    now+=10000;C2VitaPerfEnd(C2VP_RENDER);C2VitaPerfEnd(C2VP_RENDER);
    now+=10000;C2VitaPerfFinish();
    assert(strstr(logged,"total_us=60 ") && strstr(logged,"render_us=50 ") && strstr(logged,"world_us=30 "));
    C2VitaPerfReset();now+=1000;C2VitaPerfFinish();assert(strstr(logged,"render_us=0 "));
    assert(threads_created==1);C2VitaPerfReset();C2VitaPerfBegin(C2VP_WORLD);
    unsigned before_logs=messages;
    now+=4999000000ULL;watchdog_poll();assert(messages==before_logs);
    now+=1000000ULL;watchdog_poll();assert(messages==before_logs+1 && strstr(logged,"VITA STALL") && strstr(logged,"active_scopes=0x0080"));
    watchdog_poll();assert(messages==before_logs+1); /* no repeated spam */
    C2VitaPerfEnd(C2VP_WORLD);C2VitaPerfFinish();before_logs=messages;
    now+=10000000000ULL;watchdog_poll();assert(messages==before_logs); /* idle/menu */
    now=((Uint64)UINT32_MAX-2000)*1000000ULL;C2VitaPerfReset();C2VitaPerfBegin(C2VP_PHYSICS);
    now+=6000000000ULL;watchdog_poll();assert(strstr(logged,"no_progress_ms=6000") && strstr(logged,"active_scopes=0x0008"));
    C2VitaPerfFinish();C2VitaPerfShutdown();assert(threads_joined==1);
    fail_thread=1;C2VitaPerfReset();assert(strstr(logged,"monitor unavailable: mock thread failure"));
    C2VitaPerfFinish();C2VitaPerfShutdown();assert(threads_created==2 && threads_joined==1);
    puts("PASS production stall monitor:5000ms threshold, scope mask, one report per stalled heartbeat, idle exclusion, 32bit tick wrap, start/stop/failure lifecycle; mocked thread/clock, no GL or heap access");
    puts("PASS production Vita raw-pack cache:100hits no disk reads, mutable-copy isolation, file position,4MiB/8slot eviction, changed size, optional-allocation failure, oversized and short-read fallback; CPU nested scope timing/reset; real host streams; ASan/UBSan");
}
