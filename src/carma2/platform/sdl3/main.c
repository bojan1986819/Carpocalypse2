#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <signal.h>
#include <stdio.h>
#ifdef __vita__
#include "vita_controls.h"
#include "vita_video.h"
#endif
#if defined(SDL_PLATFORM_ANDROID)
#include <unistd.h>
#endif

#include "sdl3.h"
#include "sdl3_display.h"
#include "sdl3_platform.h"

#include "errors.h"
#include "globvars.h"
#include "loading.h"
#include "main.h"
#include "pedestrn.h"

extern int carpocalypse2_MenuQuitRequested(void);
extern void carpocalypse2_RequestQuit(void);

#if defined(_WIN32)
#include <windows.h>
#include <stdio.h>

/* Port-only crash report: CRASH.TXT (faulting address relative to the exe and likely return addresses
 * on the stack, resolve with the .map) and CRASH.DMP (minidump) in the working directory. Lets crashes
 * that do not reproduce under a debugger be diagnosed. */
typedef BOOL(WINAPI* tMiniDumpWriteDump)(HANDLE, DWORD, HANDLE, int, void*, void*, void*);

static LONG WINAPI carpocalypse2_CrashFilter(EXCEPTION_POINTERS* pInfo) {
    FILE* f;
    HMODULE exe = GetModuleHandleA(NULL);
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((char*)exe + ((IMAGE_DOS_HEADER*)exe)->e_lfanew);
    size_t base = (size_t)exe, end = base + nt->OptionalHeader.SizeOfImage;
    size_t eip = (size_t)pInfo->ExceptionRecord->ExceptionAddress;
    HMODULE dbghelp;

    f = fopen("CRASH.TXT", "w");
    if (f != NULL) {
        size_t* sp;
        int i, n = 0;
#if defined(_M_IX86)
        sp = (size_t*)pInfo->ContextRecord->Esp;
#else
        sp = (size_t*)pInfo->ContextRecord->Rsp;
#endif
        fprintf(f, "exception 0x%08lx at %p (exe base %p, rva+0x400000 = 0x%08zx)\n",
            pInfo->ExceptionRecord->ExceptionCode, (void*)eip, (void*)base, eip - base + 0x400000);
        if (pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
            fprintf(f, "access type %zu address %p\n", (size_t)pInfo->ExceptionRecord->ExceptionInformation[0],
                (void*)pInfo->ExceptionRecord->ExceptionInformation[1]);
        }
        fprintf(f, "stack return address candidates (rva+0x400000):\n");
        for (i = 0; i < 4096 && n < 48; i++) {
            MEMORY_BASIC_INFORMATION mbi;
            if (VirtualQuery(sp + i, &mbi, sizeof(mbi)) == 0 || mbi.State != MEM_COMMIT) {
                break;
            }
            if (sp[i] > base + 0x1000 && sp[i] < end) {
                fprintf(f, "  [sp+0x%03x] 0x%08zx\n", i * (int)sizeof(size_t), sp[i] - base + 0x400000);
                n++;
            }
        }
        fclose(f);
    }
    dbghelp = LoadLibraryA("dbghelp.dll");
    if (dbghelp != NULL) {
        tMiniDumpWriteDump write_dump = (tMiniDumpWriteDump)(void*)GetProcAddress(dbghelp, "MiniDumpWriteDump");
        HANDLE h = CreateFileA("CRASH.DMP", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (write_dump != NULL && h != INVALID_HANDLE_VALUE) {
            struct { DWORD tid; EXCEPTION_POINTERS* info; BOOL client; } ex = { GetCurrentThreadId(), pInfo, FALSE };
            write_dump(GetCurrentProcess(), GetCurrentProcessId(), h, 0 /* MiniDumpNormal */, &ex, NULL, NULL);
        }
        if (h != INVALID_HANDLE_VALUE) {
            CloseHandle(h);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

/* Faults inside window/SDL callbacks can bypass the unhandled filter, so also report first-chance
 * access violations (once; the report is only written, the exception continues normally). */
static LONG WINAPI carpocalypse2_CrashVectored(EXCEPTION_POINTERS* pInfo) {
    static LONG reported;
    DWORD code = pInfo->ExceptionRecord->ExceptionCode;

    if ((code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_STACK_OVERFLOW || code == EXCEPTION_ILLEGAL_INSTRUCTION
            || code == EXCEPTION_INT_DIVIDE_BY_ZERO) && InterlockedExchange(&reported, 1) == 0) {
        carpocalypse2_CrashFilter(pInfo);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

static void carpocalypse2_SignalHandler(int sig) {
    (void)sig;
    carpocalypse2_RequestQuit();
}

#if defined(SDL_PLATFORM_ANDROID)
// Android starts an app with "/" as the working directory, which is
// read-only, and the engine resolves everything against the working
// directory: PDBuildAppPath() below is SDL_GetCurrentDirectory() + "DATA",
// MUSIC/TrackNN.ogg is opened relative to it, and DATA/OPTIONS.TXT,
// DATA/SAVEDGAMES.ARS and DIAGNOST.TXT are written back into the same tree.
// So the process has to chdir into storage it owns before GameMain runs.
//
// Prefer the app's own external files directory
// (/storage/emulated/0/Android/data/com.carpocalypse2.game/files): it needs
// no storage permission and it is where the APK's install notes tell the
// player to push DATA/ and MUSIC/ with adb. Fall back to the internal files
// directory if the external one cannot be entered (unmounted card, locked
// profile).
//
// SDL_CreateDirectory() makes exactly one level and reports an
// already-present directory as a failure, so walk the path creating each
// level and ignore those returns: chdir() is what decides.
static int carpocalypse2_TryDataDir(const char *root) {
    char buf[1024];
    char *p;
    size_t len;

    len = SDL_strlen(root);
    if (len == 0 || len >= sizeof(buf)) {
        return 0;
    }
    SDL_memcpy(buf, root, len + 1);
    for (p = buf + 1; *p != '\0'; p++) {
        if (*p == '/') {
            *p = '\0';
            SDL_CreateDirectory(buf);
            *p = '/';
        }
    }
    SDL_CreateDirectory(buf);
    return chdir(buf) == 0;
}

static int carpocalypse2_EnterAndroidDataDir(void) {
    const char *root;

    // Called after SDL_Init: both accessors go through SDL's JNI binding.
    root = SDL_GetAndroidExternalStoragePath();
    if (root != NULL && carpocalypse2_TryDataDir(root)) {
        return 1;
    }
    root = SDL_GetAndroidInternalStoragePath();
    if (root != NULL && carpocalypse2_TryDataDir(root)) {
        return 1;
    }
    return 0;
}
#endif

#if defined(_M_IX86_FP) && _M_IX86_FP >= 2
#include <xmmintrin.h>
#include <pmmintrin.h>
#endif
int main(int argc, char *argv[]) {
#if defined(_M_IX86_FP) && _M_IX86_FP >= 2
    /* test harness (SSE2 builds): C2_TEST_FTZ=1 flushes denormals like the Vita (FPSCR FZ) */
    if (SDL_getenv("C2_TEST_FTZ") != NULL) {
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    }
#endif
    int i;
    char *path;

#ifdef __vita__
    if (!C2VitaOpenBootLog()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, C2VitaBootError(),
            "Vita startup failed. Native error codes identify the failed operation.", NULL);
        return 1;
    }
#endif

#if defined(_WIN32)
    SetUnhandledExceptionFilter(carpocalypse2_CrashFilter);
    AddVectoredExceptionHandler(1, carpocalypse2_CrashVectored);
#endif
    signal(SIGINT, carpocalypse2_SignalHandler);
    signal(SIGTERM, carpocalypse2_SignalHandler);

    gAFE = 0;
    SetDefaultPedFolderNames();
    for (i = 1; i < argc; i++) {
        if (SDL_strcmp(argv[i], "-NOCUTSCENE") == 0 || SDL_strcmp(argv[i], "-NOCUTSCENES") == 0) {
            gNoCutscenes = 1;
        } else if (SDL_strcmp(argv[i], "-ZOMBIE") == 0) {
            ConfigurePedZombiePaths();
        } else if (SDL_strcmp(argv[i], "-BLOOD") == 0) {
            ConfigurePedBloodPaths();
        } else if (SDL_strcmp(argv[i], "-ALIEN") == 0) {
            ConfigurePedAlienPaths();
        } else if (SDL_strcmp(argv[i], "-AFE") == 0) {
            gAFE = 1;
//        } else if (SDL_strcmp(argv[i], "-SCALEMOUSE") == 0) {
//            gScaleMouse = 1;
        } else {
            dr_dprintf("Unknown argument: \"%s\"", argv[i]);
            return 1;
        }
    }

    // Before the first path lookup: on Android this chdirs into the data
    // directory, and SDL_GetCurrentDirectory() below must report where the
    // game actually runs from afterwards.
#ifdef __vita__
    if (!C2VitaInitSDL()) {
#else
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
#endif
#ifdef __vita__
        C2VitaLog("Startup stopped: %s\n", SDL_GetError());
#endif
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
        return 1;
    }
#ifdef __vita__
    {
        extern void C2VitaSoundPrefetchStart(void);
        extern void C2VitaBootMark(const char* pStage);
        C2VitaBootMark("SDL initialised");
        C2VitaSoundPrefetchStart(); /* read the sound bank on a second core while the boot continues */
        C2VitaBootMark("sound prefetch started");
    }
#endif

#if defined(SDL_PLATFORM_ANDROID)
    if (!carpocalypse2_EnterAndroidDataDir()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No writable data directory (%s)", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "carpocalypse2",
            "No writable data directory on this device.", NULL);
        SDL_Quit();
        return 1;
    }
#endif

#ifdef __vita__
    {
        extern int C2VitaPrepareData(void);
        extern const char* C2VitaDataHint(void);
        extern void C2VitaBootMark(const char* pStage);
        int prepared = C2VitaPrepareData();
        C2VitaBootMark("game data checked");
        if (!prepared) {
            SDL_Window* window=SDL_CreateWindow("Carmageddon 2",960,544,0);
            SDL_Renderer* renderer=window ? SDL_CreateRenderer(window,NULL) : NULL;
            if (renderer) {
                SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderDebugText(renderer,40,200,"Carmageddon 2 - game data not ready");
                SDL_RenderDebugText(renderer,40,224,C2VitaDataHint());
                SDL_RenderDebugText(renderer,40,248,"Close this app, then launch again after resolving the error.");
                SDL_RenderPresent(renderer);SDL_Delay(15000);
            }
            SDL_Quit();return 1;
        }
    }
#endif

    path = SDL_GetCurrentDirectory();
    if (path == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetCurrentDirectory() failed (%s)", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_GetCurrentDirectory failed", SDL_GetError(), NULL);
        SDL_Quit();
        return 1;
    }
    SDL_strlcpy(gPathNetworkIni, path, SDL_arraysize(gPathNetworkIni));
    SDL_free(path);

    g_PerformanceFrequency = SDL_GetPerformanceFrequency();

    {
        extern SDL_WindowFlags carpocalypse2_GLWindowFlags(void);
        #ifdef __vita__
        g_SDL_Window = SDL_CreateWindow("Carmageddon 2",960,544,SDL_WINDOW_FULLSCREEN);
#else
        g_SDL_Window = SDL_CreateWindow("carpocalypse2", 640, 480, SDL_WINDOW_RESIZABLE | carpocalypse2_GLWindowFlags());
#endif
    }
    if (g_SDL_Window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed (%s)", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow failed", SDL_GetError(), NULL);
        SDL_Quit();
        return 1;
    }
#ifdef __vita__
    C2VitaLog("Game window created: 960x544\n");
    {
        extern void C2VitaBootMark(const char* pStage);
        C2VitaBootMark("window created");
    }
    if (!C2VitaVideoInit(g_SDL_Window)) {
        C2VitaLog("VIDEO renderer startup failed: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Vita video startup failed", SDL_GetError(), g_SDL_Window);
        SDL_Quit();
        return 1;
    }
#endif
    gHWnd = g_SDL_Window;
    carpocalypse2_DisplayInit();

    const char* args[] = { "carpocalypse2", NULL };

#ifdef __vita__
    C2VitaLog("Entering GameMain\n");
    {
        extern void C2VitaBootMark(const char* pStage);
        C2VitaBootMark("video ready, entering GameMain");
    }
#endif
    GameMain(1, args);

    SDL_Quit();
    return 0;
}
