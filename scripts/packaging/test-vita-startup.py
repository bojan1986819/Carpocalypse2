#!/usr/bin/env python3
"""Host test of unchanged production early log and staged SDL init functions.
Vita storage and SDL calls are mocked; no game or hardware runs.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
sdk = Path(os.environ["VITASDK"])
source = (root / "src/carma2/platform/sdl3/vita_platform.c").read_text()
def function(name):
    m = re.search(r"^(?:static void SDLCALL |void |int )" + name + r"\([^\n]*\) \{", source, re.M)
    assert m, name
    end = m.end()
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[m.start():end]
prefix = r"""
#include <SDL3/SDL.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
static int boot_log_ready;
typedef int SceUID;
typedef struct {unsigned int st_mode;} SceIoStat;
static SceUID boot_log_fd=-1;
static char boot_error[256];
#define SCE_O_WRONLY O_WRONLY
#define SCE_O_CREAT O_CREAT
#define SCE_O_TRUNC O_TRUNC
unsigned int _newlib_heap_size_user=128*1024*1024;
unsigned int _sceLibcMainThreadStackSize=2*1024*1024;
static char storage[1024], saved_error[1024];
static int failure, empty_error, video_calls, audio_calls;
static SDL_InitFlags initialized;
static void data_path(char* p) {snprintf(p,2048,"%s/c2port",storage);}
int sceIoMkdir(const char* path,int mode) {
    char actual[2048];
    if (!strcmp(path,"ux0:/data")) return mkdir(storage,mode);
    assert(!strcmp(path,"ux0:/data/c2port"));data_path(actual);return mkdir(actual,mode);
}
static int test_chdir(const char* path) {
    char actual[2048];assert(!strcmp(path,"ux0:/data/c2port"));data_path(actual);return chdir(actual);
}
static int fail_open,fail_write,fail_chdir,fail_stdio;
int sceIoGetstat(const char* path,SceIoStat* info) {
    char actual[2048];struct stat st;data_path(actual);
    int rc=stat(actual,&st);if(rc==0)info->st_mode=st.st_mode;return rc;
}
int sceIoOpen(const char* path,int flags,int mode) {
    char actual[2048];assert(!strcmp(path,"ux0:/data/c2port/VITA_BOOT.LOG"));
    if(fail_open)return -13;
    snprintf(actual,sizeof(actual),"%s/c2port/VITA_BOOT.LOG",storage);return open(actual,flags,mode);
}
int sceIoWrite(int fd,const void* data,unsigned int size) {return fail_write ? -13 : write(fd,data,size);}
static FILE* test_fopen(const char* path,const char* mode) {
    char actual[2048];assert(!strcmp(path,"ux0:/data/c2port/VITA_BOOT.LOG"));
    if(fail_stdio){errno=EACCES;return NULL;}
    snprintf(actual,sizeof(actual),"%s/c2port/VITA_BOOT.LOG",storage);return fopen(actual,mode);
}
static int checked_chdir(const char* path) {if(fail_chdir){errno=EACCES;return -1;}return test_chdir(path);}
#define chdir checked_chdir
#define fopen test_fopen
#define freopen DO_NOT_REOPEN_STDERR
void SDL_SetLogOutputFunction(SDL_LogOutputFunction callback,void* context) {callback(context,0,SDL_LOG_PRIORITY_INFO,"mock SDL log callback");}
void SDL_SetLogPriorities(SDL_LogPriority p) {}
int SDL_GetVersion(void) {return SDL_VERSION;}
const char* SDL_GetRevision(void) {return "host fixture";}
int SDL_GetNumVideoDrivers(void) {return 1;}
const char* SDL_GetVideoDriver(int i) {return "fixture-video";}
int SDL_GetNumAudioDrivers(void) {return 1;}
const char* SDL_GetAudioDriver(int i) {return "fixture-audio";}
const char* SDL_GetCurrentVideoDriver(void) {return "fixture-video";}
const char* SDL_GetCurrentAudioDriver(void) {return "fixture-audio";}
bool SDL_Init(SDL_InitFlags flags) {
    assert(flags==SDL_INIT_VIDEO);video_calls++;
    if(failure==1) {strcpy(saved_error,empty_error ? "" : "video detail");return false;}
    initialized|=flags;return true;
}
bool SDL_InitSubSystem(SDL_InitFlags flags) {
    assert(flags==SDL_INIT_AUDIO);assert(initialized&SDL_INIT_VIDEO);audio_calls++;
    if(failure==2) {strcpy(saved_error,"audio detail");return false;}
    initialized|=flags;return true;
}
SDL_InitFlags SDL_WasInit(SDL_InitFlags flags) {return initialized;}
const char* SDL_GetError(void) {return saved_error;}
bool SDL_SetError(const char* fmt,...) {
    va_list ap;va_start(ap,fmt);vsnprintf(saved_error,sizeof(saved_error),fmt,ap);va_end(ap);return false;
}
"""
suffix = r"""
static void reset(int stage,int empty) {failure=stage;empty_error=empty;video_calls=audio_calls=0;initialized=0;saved_error[0]=0;}
int main(int argc,char** argv) {
    assert(argc==2);snprintf(storage,sizeof(storage),"%s",argv[1]);
    fail_open=1;assert(!C2VitaOpenBootLog());assert(strstr(boot_error,"Native log open"));fail_open=0;
    fail_write=1;assert(!C2VitaOpenBootLog());assert(strstr(boot_error,"Native log write"));close(boot_log_fd);fail_write=0;
    fail_chdir=1;assert(!C2VitaOpenBootLog());assert(strstr(boot_error,"chdir failed, errno=13"));close(boot_log_fd);fail_chdir=0;
    fail_stdio=1;assert(!C2VitaOpenBootLog());assert(strstr(boot_error,"fopen failed, errno=13"));close(boot_log_fd);fail_stdio=0;
    assert(C2VitaOpenBootLog());assert(video_calls==0 && audio_calls==0);
    C2VitaLog("EARLY LOG BEFORE SDL\n");assert(C2VitaOpenBootLog());
    reset(0,0);assert(C2VitaInitSDL());assert(video_calls==1 && audio_calls==1);
    assert(initialized==(SDL_INIT_VIDEO|SDL_INIT_AUDIO));
    reset(1,0);assert(!C2VitaInitSDL());assert(audio_calls==0);assert(strstr(saved_error,"VIDEO startup failed: video detail"));
    reset(1,1);assert(!C2VitaInitSDL());assert(strstr(saved_error,"no SDL error supplied"));
    reset(2,0);assert(!C2VitaInitSDL());assert(strstr(saved_error,"AUDIO startup failed: audio detail"));
    puts("PASS native logging without freopen; native open/write and libc chdir/fopen failure codes; early log precedes SDL and is preserved; staged video/audio success, failures/empty error, native pad avoids SDL gamepad init; mocked OS/SDL, no game/hardware");
    return 0;
}
"""
with tempfile.TemporaryDirectory(prefix="c2-vita-startup-") as tmp:
    tmp = Path(tmp)
    headers = tmp / "include"
    headers.mkdir()
    (headers / "SDL3").symlink_to(sdk / "arm-vita-eabi/include/SDL3", target_is_directory=True)
    fixture = tmp / "startup.c"
    fixture.write_text(prefix + function("C2VitaLog") + "\n" + function("vita_sdl_log") + "\n" + function("C2VitaOpenBootLog") + "\n" + function("C2VitaInitSDL") + suffix)
    subprocess.run(["gcc","-std=c11","-O2","-Wall","-Werror","-I"+str(headers),str(fixture),"-o",str(tmp / "startup")],check=True)
    subprocess.run([str(tmp / "startup"),str(tmp / "storage")],check=True)
    log = (tmp / "storage/c2port/VITA_BOOT.LOG").read_text()
    assert "EARLY LOG BEFORE SDL" in log and "mock SDL log callback" in log
    assert "stage VIDEO FAILED: video detail" in log and "<empty SDL error>" in log
    assert "stage AUDIO FAILED: audio detail" in log and "stage AUDIO OK" in log
