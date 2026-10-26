#define _GNU_SOURCE
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
static pthread_key_t key;
static pthread_once_t key_once=PTHREAD_ONCE_INIT;
static void create_key(void) {assert(!pthread_key_create(&key,free));}
void* SDL_GetTLS(SDL_TLSID* id) {(void)id;pthread_once(&key_once,create_key);return pthread_getspecific(key);}
bool SDL_SetTLS(SDL_TLSID* id,const void* value,void (*destructor)(void*)) {
    (void)id;assert(destructor==free);pthread_once(&key_once,create_key);return !pthread_setspecific(key,value);
}
static atomic_uint scans;
DIR* case_opendir(const char* path) {atomic_fetch_add(&scans,1);return opendir(path);}
FILE* __real_fopen(const char* path,const char* mode) {return fopen(path,mode);}
int __real_open(const char* path,int flags,...) {
    mode_t mode=0;va_list a;
    if(flags&O_CREAT) {va_start(a,flags);mode=va_arg(a,int);va_end(a);}
    return open(path,flags,mode);
}
int __real_open64(const char* path,int flags,...) {
    mode_t mode=0;va_list a;
    if(flags&O_CREAT) {va_start(a,flags);mode=va_arg(a,int);va_end(a);}
    return open(path,flags,mode);
}
extern const char* carpocalypse2_fix_path_case(const char*);
extern FILE* __wrap_fopen(const char*,const char*);
extern int __wrap_open(const char*,int,...);
static void* thread_probe(void* arg) {
    char path[256];snprintf(path,sizeof(path),"ux0:/DATA/ASSETS/missing%ld.pix",(long)arg);
    char expected[256];snprintf(expected,sizeof(expected),"ux0:/data/assets/missing%ld.pix",(long)arg);
    for(unsigned i=0;i<200;++i) assert(!strcmp(carpocalypse2_fix_path_case(path),expected));
    return NULL;
}
int main(void) {
    FILE* f; unsigned before;
    const char* p=carpocalypse2_fix_path_case("ux0:/DATA/ASSETS/MISSING.PIX");
    assert(!strcmp(p,"ux0:/data/assets/MISSING.PIX"));
    before=atomic_load(&scans);
    for(unsigned i=0;i<1000;++i) assert(!__wrap_fopen("ux0:/DATA/ASSETS/MISSING.PIX","rb"));
    assert(atomic_load(&scans)==before);
    for (unsigned i=0;i<100;++i) {
        char missing[256];snprintf(missing,sizeof(missing),"ux0:/DATA/ASSETS/new_missing%u.pix",i);
        assert(!__wrap_fopen(missing,"rb"));
    }
    assert(atomic_load(&scans)==before); /* distinct misses use directory index */

    p=carpocalypse2_fix_path_case("ux0:/DATA/ASSETS/OTHER.PIX");
    assert(!strcmp(p,"ux0:/data/assets/OTHER.PIX"));
    assert(atomic_load(&scans)<=before+1); /* cached parent prefixes */
    f=__wrap_fopen("ux0:/data/assets/missing.pix","wb");assert(f);fputs("hello",f);fclose(f);
    f=__wrap_fopen("ux0:/DATA/ASSETS/MISSING.PIX","rb");assert(f && fgetc(f)=='h');fclose(f);
    before=atomic_load(&scans);
    for(unsigned i=0;i<1000;++i) {f=__wrap_fopen("ux0:/DATA/ASSETS/MISSING.PIX","rb");assert(f);fclose(f);}
    assert(atomic_load(&scans)==before);
    assert(!__wrap_fopen("ux0:/DATA/ASSETS/NEW.PIX","rb"));
    int fd=__wrap_open("ux0:/data/assets/new.pix",O_WRONLY|O_CREAT|O_TRUNC,0666);assert(fd>=0);close(fd);
    f=__wrap_fopen("ux0:/DATA/ASSETS/NEW.PIX","rb");assert(f);fclose(f);
    /* Directory snapshots must fall back for entries beyond their bounds. */
    for (unsigned i=0;i<1100;++i) {
        char path[256];snprintf(path,sizeof(path),"ux0:/DATA/HUGE/filler%u.PIX",i);
        f=__wrap_fopen(path,"rb");assert(f);fclose(f);
    }
    char longpath[256]="ux0:/DATA/LONG/";
    for (unsigned i=0;i<12;++i) strcat(longpath,"LONGNAME");
    strcat(longpath,".PIX");f=__wrap_fopen(longpath,"rb");assert(f);fclose(f);
    /* Relative names must follow cwd changes, not a cached absolute result. */
    assert(!chdir("one"));assert(!strcmp(carpocalypse2_fix_path_case("FILE.PIX"),"file.pix"));
    assert(!chdir("../two"));assert(!strcmp(carpocalypse2_fix_path_case("FILE.PIX"),"File.Pix"));
    assert(!chdir(".."));
    pthread_t t[2];
    assert(!pthread_create(&t[0],NULL,thread_probe,(void*)1L));
    assert(!pthread_create(&t[1],NULL,thread_probe,(void*)2L));
    pthread_join(t[0],NULL);pthread_join(t[1],NULL);
    char huge[2048];memset(huge,'a',sizeof(huge)-1);huge[sizeof(huge)-1]=0;
    assert(carpocalypse2_fix_path_case(huge)==huge);
    puts("PASS production Vita case resolver:1000repeat missing probes+1000wrong-case opens require zero repeated directory scans; 100distinct misses indexed,1100-entry/long-name fallback, prefix sharing, fopen/open creation invalidation, relative cwd, concurrent threads and bounds; real host files;ASan/UBSan");
}
