#include "vita_controls.h"
#include "vita_perf.h"
#include "vita_data.h"
#include "vita_video.h"
#include "globvars.h"
#include "input.h"
#include <SDL3/SDL.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
/* Start with VitaSDK's default reservation. A larger all-or-nothing block can
 * fail before main, leaving both libc file I/O and SDL without allocations. */
unsigned int _newlib_heap_size_user=128*1024*1024;
unsigned int _sceLibcMainThreadStackSize=2*1024*1024;
static int boot_log_ready;
static SceUID boot_log_fd = -1;
static char boot_error[256] = "Vita boot log is not ready";

/* Periodic performance lines ("VITA ..."/"VGL ...") from the main thread are collected and
 * written in one go: every sceIoWrite is a synchronous memory-card write, and ~8 of them every
 * 120 frames showed up as hitches. Any other line flushes the collection first and is written
 * immediately, so ordering and crash-time diagnostics are kept. */
static char log_pending[16384];
static int log_pending_length;
static SceUID log_main_thread = -1;
static void log_write(const char* text, int length) {
    int offset = 0;
    while (offset < length) {
        int written = sceIoWrite(boot_log_fd, text + offset, length - offset);
        if (written <= 0) return;
        offset += written;
    }
}
void C2VitaLogFlush(void) {
    if (boot_log_fd >= 0 && log_pending_length > 0) log_write(log_pending, log_pending_length);
    log_pending_length = 0;
}
void C2VitaLog(const char* format, ...) {
    char text[1024];
    va_list args;
    int length;
    int periodic;
    if (boot_log_fd < 0) return;
    va_start(args, format);
    length = vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    if (length < 0) return;
    if (length >= (int)sizeof(text)) length = sizeof(text) - 1;
    if (log_main_thread < 0) log_main_thread = sceKernelGetThreadId();
    if (sceKernelGetThreadId() != log_main_thread) {
        log_write(text, length); /* other threads (music) write through */
        return;
    }
    periodic = (strncmp(text, "VITA ", 5) == 0 || strncmp(text, "VGL ", 4) == 0)
        && strncmp(text, "VITA PHYS SPIKE", 15) != 0;
    if (!periodic) {
        C2VitaLogFlush();
        log_write(text, length);
        return;
    }
    if (log_pending_length + length > (int)sizeof(log_pending)) C2VitaLogFlush();
    memcpy(log_pending + log_pending_length, text, length);
    log_pending_length += length;
    if (log_pending_length >= 12288) C2VitaLogFlush();
}

const char* C2VitaBootError(void) {
    return boot_error;
}

void C2VitaCheckpoint(const char* title) {
    static unsigned int checkpoint_count;
    if (checkpoint_count++ < 256) {
        struct mallinfo memory = mallinfo();
        C2VitaLog("INIT %llu ms: %s; heap arena=%u used=%u free_in_arena=%u\n",
            (unsigned long long)SDL_GetTicks(), title, (unsigned)memory.arena,
            (unsigned)memory.uordblks, (unsigned)memory.fordblks);
    }
    C2VitaVideoLoadingProgress();
}

static void SDLCALL vita_sdl_log(void* context, int category, SDL_LogPriority priority, const char* message) {
    (void)context;
    C2VitaLog("SDL category=%d priority=%d: %s\n", category, (int)priority, message);
}

int C2VitaOpenBootLog(void) {
    int mkdir_data, mkdir_game, stat_result, directory_result, directory_error;
    SceIoStat info;
    FILE* probe;
    void* allocation;
    int written;
#ifdef C2_VITA_UNCAPPED
    static const char heading[] = "C2 Vita startup diagnostic v1.0 UNCAPPED; native boot log\n";
#else
    static const char heading[] = "C2 Vita startup diagnostic v1.0; native boot log\n";
#endif
    if (boot_log_ready) return 1;
    mkdir_data = sceIoMkdir("ux0:/data", 0777);
    mkdir_game = sceIoMkdir("ux0:/data/c2port", 0777);
    memset(&info, 0, sizeof(info));
    stat_result = sceIoGetstat("ux0:/data/c2port", &info);
    /* Keep the previous run's log: a quick relaunch must not erase a long test run. */
    sceIoRemove("ux0:/data/c2port/VITA_BOOT.PREV.LOG");
    sceIoRename("ux0:/data/c2port/VITA_BOOT.LOG", "ux0:/data/c2port/VITA_BOOT.PREV.LOG");
    boot_log_fd = sceIoOpen("ux0:/data/c2port/VITA_BOOT.LOG",
        SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0666);
    if (boot_log_fd < 0) {
        snprintf(boot_error, sizeof(boot_error), "Native log open: %08x; folder stat: %08x; mkdir: %08x/%08x",
            (unsigned)boot_log_fd, (unsigned)stat_result, (unsigned)mkdir_data, (unsigned)mkdir_game);
        return 0;
    }
    written = sceIoWrite(boot_log_fd, heading, sizeof(heading) - 1);
    if (written != (int)sizeof(heading) - 1) {
        snprintf(boot_error, sizeof(boot_error), "Native log write failed: %08x", (unsigned)written);
        return 0;
    }
    C2VitaLog("mkdir=%08x/%08x stat=%08x mode=%08x requested heap=%u stack=%u\n",
        (unsigned)mkdir_data, (unsigned)mkdir_game, (unsigned)stat_result,
        (unsigned)info.st_mode, _newlib_heap_size_user, _sceLibcMainThreadStackSize);
    /* The game is CPU bound (render preparation ~100 ms/frame); run at the maximum clocks homebrew
     * normally uses instead of the 333 MHz application default. Logged so hardware runs show it. */
    C2VitaLog("CLOCK before arm=%d bus=%d gpu=%d xbar=%d MHz\n", scePowerGetArmClockFrequency(),
        scePowerGetBusClockFrequency(), scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
    C2VitaLog("CLOCK after arm=%d bus=%d gpu=%d xbar=%d MHz\n", scePowerGetArmClockFrequency(),
        scePowerGetBusClockFrequency(), scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
    {
        /* Flush denormals to zero on this (game/physics) thread: decaying physics values can go
         * subnormal, which the VFP handles very slowly. ux0:/data/c2port/NOFTZ.SET keeps IEEE mode. */
        FILE* f = fopen("ux0:/data/c2port/NOFTZ.SET", "rb");
        unsigned fpscr;
        __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
        if (f == NULL) {
            fpscr |= 1u << 24; /* FZ */
            __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));
        } else {
            fclose(f);
        }
        __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
        C2VitaLog("VITA FPU fpscr=%08x flush-to-zero %s\n", fpscr, (fpscr >> 24) & 1 ? "on" : "off");
    }
    errno = 0;
    allocation = malloc(16);
    C2VitaLog("malloc(16)=%s errno=%d\n", allocation ? "OK" : "FAILED", errno);
    free(allocation);
    directory_result = chdir("ux0:/data/c2port");
    directory_error = directory_result == 0 ? 0 : errno;
    C2VitaLog("chdir result=%d errno=%d\n", directory_result, directory_error);
    if (directory_result != 0) {
        snprintf(boot_error, sizeof(boot_error), "C runtime chdir failed, errno=%d. Native log saved in c2port.", directory_error);
        return 0;
    }
    /* Do not reopen stderr: a normal Vita app may have no usable tty stream. */
    probe = fopen("ux0:/data/c2port/VITA_BOOT.LOG", "ab");
    if (!probe) {
        int error = errno;
        C2VitaLog("C runtime fopen FAILED errno=%d\n", error);
        snprintf(boot_error, sizeof(boot_error), "C runtime fopen failed, errno=%d. Native log saved in c2port.", error);
        return 0;
    }
    fclose(probe);
    C2VitaLog("Native file I/O and C runtime file I/O OK\n");
    boot_log_ready = 1;
    return 1;
}

int C2VitaInitSDL(void) {
    char error[512];
    int i;
    SDL_SetLogOutputFunction(vita_sdl_log, NULL);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);
    C2VitaLog("SDL headers=%d.%d.%d runtime=%d revision=%s\n",
        SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION, SDL_GetVersion(), SDL_GetRevision());
    for (i = 0; i < SDL_GetNumVideoDrivers(); i++) {
        C2VitaLog("Available video driver: %s\n", SDL_GetVideoDriver(i));
    }
    for (i = 0; i < SDL_GetNumAudioDrivers(); i++) {
        C2VitaLog("Available audio driver: %s\n", SDL_GetAudioDriver(i));
    }
    /* The Vita pad is polled through sceCtrl, independent of SDL gamepads. */
    C2VitaLog("SDL stage VIDEO starting\n");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        snprintf(error, sizeof(error), "%.*s", (int)sizeof(error) - 1, SDL_GetError());
        C2VitaLog("SDL stage VIDEO FAILED: %s (initialized=0x%x)\n",
            error[0] ? error : "<empty SDL error>", (unsigned)SDL_WasInit(0));
        SDL_SetError("VIDEO startup failed: %s. See VITA_BOOT.LOG", error[0] ? error : "no SDL error supplied");
        return 0;
    }
    C2VitaLog("SDL stage VIDEO OK driver=%s initialized=0x%x\n",
        SDL_GetCurrentVideoDriver(), (unsigned)SDL_WasInit(0));
    C2VitaLog("SDL stage AUDIO starting\n");
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        snprintf(error, sizeof(error), "%.*s", (int)sizeof(error) - 1, SDL_GetError());
        C2VitaLog("SDL stage AUDIO FAILED: %s (initialized=0x%x)\n",
            error[0] ? error : "<empty SDL error>", (unsigned)SDL_WasInit(0));
        SDL_SetError("AUDIO startup failed: %s. See VITA_BOOT.LOG", error[0] ? error : "no SDL error supplied");
        return 0;
    }
    C2VitaLog("SDL stage AUDIO OK driver=%s initialized=0x%x\n",
        SDL_GetCurrentAudioDriver(), (unsigned)SDL_WasInit(0));
    C2VitaLog("Native Vita controls active; SDL gamepad startup not required\n");
    return 1;
}

static C2VitaPad read_pad(void) {
    SceCtrlData raw;
    C2VitaPad pad={0};
    C2VitaPerfBegin(C2VP_INPUT);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
    if (sceCtrlPeekBufferPositive(0,&raw,1)<=0) {C2VitaPerfEnd(C2VP_INPUT);return pad;}
#define BUTTON(SCE,C2) if (raw.buttons & SCE) pad.buttons |= C2
    BUTTON(SCE_CTRL_UP,C2V_UP); BUTTON(SCE_CTRL_DOWN,C2V_DOWN);
    BUTTON(SCE_CTRL_LEFT,C2V_LEFT); BUTTON(SCE_CTRL_RIGHT,C2V_RIGHT);
    BUTTON(SCE_CTRL_CROSS,C2V_CROSS); BUTTON(SCE_CTRL_CIRCLE,C2V_CIRCLE);
    BUTTON(SCE_CTRL_SQUARE,C2V_SQUARE); BUTTON(SCE_CTRL_TRIANGLE,C2V_TRIANGLE);
    BUTTON(SCE_CTRL_LTRIGGER,C2V_L); BUTTON(SCE_CTRL_RTRIGGER,C2V_R);
    BUTTON(SCE_CTRL_START,C2V_START); BUTTON(SCE_CTRL_SELECT,C2V_SELECT);
#undef BUTTON
    pad.lx=(int)raw.lx-128;pad.ly=(int)raw.ly-128;
    pad.rx=(int)raw.rx-128;pad.ry=(int)raw.ry-128;
    C2VitaPerfEnd(C2VP_INPUT);
    return pad;
}
void C2VitaReadKeys(int* keys,int mark) {
    C2VitaPad pad=read_pad();
    /* In-race options still use frontend navigation, rather than driving. */
    extern int gInterface_within_race_mode;
    C2VitaMapKeys(&pad,gProgram_state.racing && !gInterface_within_race_mode,gKey_mapping,keys,mark);
    /* race over (bonus tally / wreck camera): Cross acts as Return, which MungeRaceFinished treats as skip */
    { extern tU32 gRace_finished; static int armed; /* needs a fresh press: Cross is also the handbrake */
      if (!gRace_finished || !gProgram_state.racing) armed=0;
      else if (!(pad.buttons&C2V_CROSS)) armed=1;
      else if (armed) keys[51]=mark; }
}
void C2VitaReadSteering(tJoystick* joystick) {
    C2VitaPad pad=read_pad();int steer=C2VitaSteering(pad.lx);
    joystick->left=steer<0 ? -steer : 0;
    joystick->right=steer>0 ? steer : 0;
}
static const char* data_hint = "Copy DATA and MUSIC into ux0:data/c2port";

const char* C2VitaDataHint(void) {
    return data_hint;
}

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_draw;
    unsigned long long copied;
} C2VitaInstallDisplay;

static int install_progress(void* context, const char* path, unsigned int bytes) {
    C2VitaInstallDisplay* display = context;
    SDL_Event event;
    Uint64 now = SDL_GetTicks();
    char text[96];
    size_t length = strlen(path);
    display->copied += bytes;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return 0;
    }
    if (read_pad().buttons & C2V_CIRCLE) return 0;
    if (display->renderer && (now - display->last_draw >= 200 || display->last_draw == 0)) {
        SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
        SDL_RenderClear(display->renderer);
        SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(display->renderer, 16, 64, "Preparing bundled game data");
        SDL_RenderDebugText(display->renderer, 16, 86, "First start only - please wait");
        SDL_RenderDebugText(display->renderer, 16, 112, length > 52 ? path + length - 52 : path);
        snprintf(text, sizeof(text), "Copied %llu MiB", display->copied / (1024 * 1024));
        SDL_RenderDebugText(display->renderer, 16, 138, text);
        SDL_RenderDebugText(display->renderer, 16, 166, "Circle cancels; launch again to resume.");
        SDL_RenderPresent(display->renderer);
        display->last_draw = now;
    }
    return 1;
}

static int read_bundle_id(const char* path, char* id) {
    FILE* file = fopen(path, "rb");
    size_t n;
    if (!file) return 0;
    n = fread(id, 1, 64, file);
    id[n] = 0;
    fclose(file);
    return n == 64;
}

static int prepare_bundled_data(void) {
    C2VitaInstallDisplay display = {0};
    char bundled_id[65], installed_id[65];
    FILE* file;
    int ok;

    file = fopen("app0:/bundled/DATA/DATA.TWT", "rb");
    if (!file) return 1; /* The small VPK still supports manually copied data. */
    fclose(file);
    data_hint = "Check VITA_BOOT.LOG; launch again to retry setup.";
    if (!read_bundle_id("app0:/bundled/bundle.id", bundled_id)) {
        C2VitaLog("Bundled data has no valid bundle.id\n");
        return 0;
    }
    if (read_bundle_id("C2BUNDLE.ID", installed_id) && !strcmp(bundled_id, installed_id)) {
        file = fopen("DATA/DATA.TWT", "rb");
        if (file) { fclose(file); return 1; }
    }
    C2VitaLog("Preparing bundled data %s; preserving existing files\n", bundled_id);
    display.window = SDL_CreateWindow("Carmageddon 2 setup", 960, 544, SDL_WINDOW_FULLSCREEN);
    display.renderer = display.window ? SDL_CreateRenderer(display.window, NULL) : NULL;
    if (display.renderer) SDL_SetRenderScale(display.renderer, 2, 2);
    ok = install_progress(&display, "Starting...", 0)
        && C2VitaCopyMissingTree("app0:/bundled/DATA", "DATA", install_progress, &display)
        && C2VitaCopyMissingTree("app0:/bundled/MUSIC", "MUSIC", install_progress, &display);
    if (display.renderer) SDL_DestroyRenderer(display.renderer);
    if (display.window) SDL_DestroyWindow(display.window);
    if (!ok) return 0;

    file = fopen("C2BUNDLE.ID.c2-part", "wb");
    if (!file) return 0;
    ok = fwrite(bundled_id, 1, 64, file) == 64;
    if (fclose(file) != 0) ok = 0;
    if (!ok) { remove("C2BUNDLE.ID.c2-part"); return 0; }
    remove("C2BUNDLE.ID");
    if (rename("C2BUNDLE.ID.c2-part", "C2BUNDLE.ID") != 0) return 0;
    C2VitaLog("Bundled data ready; copied %llu bytes\n", display.copied);
    return 1;
}

int C2VitaPrepareData(void) {
    FILE* f;
    if (!C2VitaOpenBootLog()) return 0;
    C2VitaLog("Preparing game data\n");
    if (!prepare_bundled_data()) return 0;
    f = fopen("DATA/DATA.TWT", "rb");
    if (f == NULL) {
        C2VitaLog("Missing DATA/DATA.TWT in ux0:data/c2port\n");
        return 0;
    }
    fclose(f);
    return 1;
}
