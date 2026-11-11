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
#include <psp2/kernel/processmgr.h>
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

/* Every sceIoWrite is a synchronous memory-card write (~8 ms on a slow card: the ~660 lines of a
 * boot and a race load cost 5.5 s). Lines go to a ring that a writer thread empties in order, so
 * no thread waits for the card; C2VitaLogFlush (quit, the stall monitor's deliberate crash) still
 * writes everything out before returning. Before the writer starts, lines are written directly. */
#define LOG_RING_BYTES (256 * 1024)
static char log_ring[LOG_RING_BYTES];
static unsigned log_head, log_tail; /* bytes ever queued / ever taken; the ring holds head - tail */
static SceKernelLwMutexWork log_ring_mutex, log_io_mutex;
static SceUID log_sema = -1;
static int log_async;
static unsigned long long log_write_ns, log_writes;
static void log_write_now(const char* text, int length) {
    int offset = 0;
    unsigned long long start = sceKernelGetProcessTimeWide();
    log_writes++;
    while (offset < length) {
        int written = sceIoWrite(boot_log_fd, text + offset, length - offset);
        if (written <= 0) return;
        offset += written;
    }
    log_write_ns += (sceKernelGetProcessTimeWide() - start) * 1000ull;
}
/* Writes out what the ring holds; the caller holds log_io_mutex, so chunks stay in order. */
static void log_drain(void) {
    static char chunk[16384];
    for (;;) {
        unsigned count, at, first;
        sceKernelLockLwMutex(&log_ring_mutex, 1, NULL);
        count = log_head - log_tail;
        if (count > sizeof(chunk)) count = sizeof(chunk);
        at = log_tail % LOG_RING_BYTES;
        first = count < LOG_RING_BYTES - at ? count : LOG_RING_BYTES - at;
        memcpy(chunk, log_ring + at, first);
        memcpy(chunk + first, log_ring, count - first);
        log_tail += count;
        sceKernelUnlockLwMutex(&log_ring_mutex, 1);
        if (count == 0) return;
        if (boot_log_fd >= 0) log_write_now(chunk, (int)count);
    }
}
static int log_writer_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;
    for (;;) {
        sceKernelWaitSema(log_sema, 1, NULL);
        /* a card write costs ~7 ms whatever its size and holds up the game's reads: collect half a
         * second of lines into one write */
        sceKernelDelayThread(500000);
        sceKernelLockLwMutex(&log_io_mutex, 1, NULL);
        log_drain();
        sceKernelUnlockLwMutex(&log_io_mutex, 1);
    }
    return 0;
}
static void log_start_writer(void) {
    SceUID thread;
    if (sceKernelCreateLwMutex(&log_ring_mutex, "C2 log ring", 0, 0, NULL) < 0
        || sceKernelCreateLwMutex(&log_io_mutex, "C2 log io", 0, 0, NULL) < 0) return;
    log_sema = sceKernelCreateSema("C2 log", 0, 0, 0x7fffffff, NULL);
    if (log_sema < 0) return;
    thread = sceKernelCreateThread("C2 log writer", log_writer_thread, 0x10000100, 0x4000, 0, 0, NULL);
    if (thread < 0 || sceKernelStartThread(thread, 0, NULL) < 0) return;
    log_async = 1;
}
static void log_write(const char* text, int length) {
    unsigned at, first;
    if (!log_async) {
        log_write_now(text, length);
        return;
    }
    sceKernelLockLwMutex(&log_ring_mutex, 1, NULL);
    if (log_head - log_tail + (unsigned)length > LOG_RING_BYTES) {
        /* the card is far behind: write it out here, in order */
        sceKernelUnlockLwMutex(&log_ring_mutex, 1);
        sceKernelLockLwMutex(&log_io_mutex, 1, NULL);
        log_drain();
        log_write_now(text, length);
        sceKernelUnlockLwMutex(&log_io_mutex, 1);
        return;
    }
    at = log_head % LOG_RING_BYTES;
    first = (unsigned)length < LOG_RING_BYTES - at ? (unsigned)length : LOG_RING_BYTES - at;
    memcpy(log_ring + at, text, first);
    memcpy(log_ring, text + first, length - first);
    log_head += length;
    sceKernelUnlockLwMutex(&log_ring_mutex, 1);
    sceKernelSignalSema(log_sema, 1);
}
void C2VitaLogFlush(void) {
    if (!log_async) return;
    sceKernelLockLwMutex(&log_io_mutex, 1, NULL);
    log_drain();
    sceKernelUnlockLwMutex(&log_io_mutex, 1);
}
void C2VitaLog(const char* format, ...) {
    char text[1024];
    va_list args;
    int length;
    if (boot_log_fd < 0) return;
    va_start(args, format);
    length = vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    if (length < 0) return;
    if (length >= (int)sizeof(text)) length = sizeof(text) - 1;
    log_write(text, length);
}

const char* C2VitaBootError(void) {
    return boot_error;
}

/* boot stages before the game's own checkpoints, in process time like the load phases */
void C2VitaBootMark(const char* pStage) {
    C2VitaLog("BOOT %llu ms: %s\n", sceKernelGetProcessTimeWide() / 1000ull, pStage);
}

void C2VitaCheckpoint(const char* title) {
    static unsigned int checkpoint_count;
    static Uint64 mallinfo_ns, progress_ns;
    Uint64 t0 = SDL_GetTicksNS(), t1;
    if (checkpoint_count++ < 256) {
        struct mallinfo memory = mallinfo();
        t1 = SDL_GetTicksNS();
        mallinfo_ns += t1 - t0;
        C2VitaLog("INIT %llu ms: %s; heap arena=%u used=%u free_in_arena=%u (mallinfo %llu ms, progress %llu ms so far)\n",
            (unsigned long long)SDL_GetTicks(), title, (unsigned)memory.arena,
            (unsigned)memory.uordblks, (unsigned)memory.fordblks, (unsigned long long)(mallinfo_ns / 1000000),
            (unsigned long long)(progress_ns / 1000000));
        C2VitaLog("INIT log writes=%llu %llu ms so far\n", log_writes, log_write_ns / 1000000);
    }
    t1 = SDL_GetTicksNS();
    C2VitaVideoLoadingProgress();
    progress_ns += SDL_GetTicksNS() - t1;
}

static void SDLCALL vita_sdl_log(void* context, int category, SDL_LogPriority priority, const char* message) {
    (void)context;
    C2VitaLog("SDL category=%d priority=%d: %s\n", category, (int)priority, message);
}

/* vita-elf-create appends ~5 KB of SCE data after the code segment, and the linker starts the data
 * segment at the next 64 KB boundary. When the code ends within those few KB of a boundary the
 * VELF cannot be built ("segment 1 overlaps"). This padding in the code segment moves the data
 * segment one boundary further; if the error comes back after code growth, change its size. */
const unsigned char gC2_vita_segment_pad[16384] = { 1 };

/* Remote test runs (vitacompanion): TESTENV.TXT holds NAME=VALUE lines that become the C2_TEST_*
 * environment of the desktop harness. It is renamed to TESTENV.USED on the way in, so a test
 * setup applies to exactly one launch and never to the next normal play session. */
static unsigned long long test_quit_ms;
/* C2_TEST_DUMPAT=<seconds since start>: crash on purpose then, so the system writes a core dump of a
 * hang outside race frames (the stall monitor only watches those). */
static unsigned test_dump_ms;
static int test_dump_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;
    sceKernelDelayThread(test_dump_ms * 1000u);
    /* no logging: a hang may hold the log's lock */
    *(volatile int*)0 = 0xD0D0;
    return 0;
}
/* C2_TEST_CTXPROBE: can a thread read the main thread's registers (for a PC sampling profiler)? */
extern int sceKernelGetThreadContextForVM(SceUID thid, void* cpu, void* vfp);
static SceUID test_main_thread;
static int test_ctx_probe_thread(SceSize args, void* argp) {
    static unsigned cpu[64], vfp[160];
    int i, k, result;
    (void)args;
    (void)argp;
    for (k = 0; k < 3; k++) {
        sceKernelDelayThread(3000000);
        memset(cpu, 0, sizeof(cpu));
        cpu[0] = sizeof(cpu);
        vfp[0] = sizeof(vfp);
        result = sceKernelGetThreadContextForVM(test_main_thread, cpu, vfp);
        C2VitaLog("TEST CTXPROBE result=0x%08x main=0x%08x probe_fn=%p\n", (unsigned)result, (unsigned)test_main_thread,
            (void*)test_ctx_probe_thread);
        for (i = 0; i < 24; i += 6) {
            C2VitaLog("TEST CTXPROBE w%02d: %08x %08x %08x %08x %08x %08x\n", i, cpu[i], cpu[i + 1], cpu[i + 2], cpu[i + 3],
                cpu[i + 4], cpu[i + 5]);
        }
    }
    return 0;
}
static int test_awake_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;
    for (;;) {
        sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DEFAULT);
        sceKernelDelayThread(1000000);
    }
    return 0;
}
static void load_test_env(void) {
    char line[512];
    FILE* f = fopen("ux0:/data/c2port/TESTENV.TXT", "rb");
    if (f == NULL) return;
    while (fgets(line, sizeof(line), f) != NULL) {
        char* value = strchr(line, '=');
        char* end;
        if (line[0] == '#' || value == NULL) continue;
        *value++ = '\0';
        end = value + strlen(value);
        while (end > value && (end[-1] == '\n' || end[-1] == '\r')) *--end = '\0';
        setenv(line, value, 1);
        C2VitaLog("TESTENV %s=%s\n", line, value);
    }
    fclose(f);
    sceIoRemove("ux0:/data/c2port/TESTENV.USED");
    sceIoRename("ux0:/data/c2port/TESTENV.TXT", "ux0:/data/c2port/TESTENV.USED");
    if (getenv("C2_TEST_QUIT") != NULL) test_quit_ms = (unsigned long long)(atof(getenv("C2_TEST_QUIT")) * 1000.0);
    if (getenv("C2_TEST_DUMPAT") != NULL) {
        SceUID thread;
        test_dump_ms = (unsigned)(atof(getenv("C2_TEST_DUMPAT")) * 1000.0);
        thread = sceKernelCreateThread("C2 test dump", test_dump_thread, 0x10000100, 0x4000, 0, 0, NULL);
        if (thread >= 0) sceKernelStartThread(thread, 0, NULL);
    }
    if (getenv("C2_TEST_QUIT") != NULL) {
        /* unattended runs: keep the display on, or the lock screen freezes the game (~30 s in) */
        SceUID thread = sceKernelCreateThread("C2 test awake", test_awake_thread, 0x10000100, 0x1000, 0, 0, NULL);
        if (thread >= 0) sceKernelStartThread(thread, 0, NULL);
    }
    if (getenv("C2_TEST_IOBENCH") != NULL) {
        /* test: raw card read speed of one file by chunk size (C2_TEST_IOBENCH=<path>) */
        static const int chunks[] = { 4096, 65536, 1048576 };
        static char buffer[1048576];
        int c;
        for (c = 0; c < 3; c++) {
            SceUID fd = sceIoOpen(getenv("C2_TEST_IOBENCH"), SCE_O_RDONLY, 0);
            unsigned long long start = sceKernelGetProcessTimeWide(), total = 0;
            int got;
            if (fd < 0) break;
            while (total < 8u * 1048576u && (got = sceIoRead(fd, buffer, chunks[c])) > 0) total += (unsigned)got;
            sceIoClose(fd);
            C2VitaLog("TEST IOBENCH chunk=%d bytes=%llu us=%llu\n", chunks[c], total, sceKernelGetProcessTimeWide() - start);
        }
    }
    if (getenv("C2_TEST_CTXPROBE") != NULL) {
        SceUID thread;
        test_main_thread = sceKernelGetThreadId();
        thread = sceKernelCreateThread("C2 ctx probe", test_ctx_probe_thread, 0x10000100, 0x4000, 0, 0, NULL);
        if (thread >= 0) sceKernelStartThread(thread, 0, NULL);
    }
}

/* C2_TEST_QUIT=<seconds since start>: write the logs and leave, so a remote run ends by itself. */
void C2VitaTestQuitCheck(void) {
    if (test_quit_ms != 0 && SDL_GetTicks() >= test_quit_ms) {
        C2VitaLog("TEST QUIT after %llu ms\n", (unsigned long long)SDL_GetTicks());
        C2VitaLogFlush();
        sceIoClose(boot_log_fd);
        boot_log_fd = -1;
        sceKernelExitProcess(0);
    }
}

int C2VitaOpenBootLog(void) {
    volatile unsigned char keep_pad = gC2_vita_segment_pad[0]; /* referenced, so --gc-sections keeps it */
    int mkdir_data, mkdir_game, stat_result, directory_result, directory_error;
    (void)keep_pad;
    SceIoStat info;
    FILE* probe;
    void* allocation;
    int written;
#ifdef C2_VITA_UNCAPPED
    static const char heading[] = "C2 Vita startup diagnostic v1.1 UNCAPPED; native boot log\n";
#else
    static const char heading[] = "C2 Vita startup diagnostic v1.1; native boot log\n";
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
    load_test_env();
    log_start_writer();
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
