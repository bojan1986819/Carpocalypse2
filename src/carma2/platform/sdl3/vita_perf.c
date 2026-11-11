/* Inclusive main-thread timings, with a bounded diagnostic monitor thread. */
#include "vita_perf.h"
#include <SDL3/SDL.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <psp2/kernel/threadmgr.h>
static Uint64 starts[C2VP_COUNT], elapsed[C2VP_COUNT], frame_start;
static unsigned depths[C2VP_COUNT], frame;
int C2VitaPerfDetail;
static unsigned counters[C2VPC_COUNT];
void C2VitaPerfCount(unsigned counter) { if (counter < C2VPC_COUNT) counters[counter]++; }

int C2VitaTestFrozen(unsigned pRace_ms) {
    static int freeze_ms = -1;
    if (freeze_ms < 0) freeze_ms = getenv("C2_TEST_FREEZE") != NULL ? atoi(getenv("C2_TEST_FREEZE")) * 1000 : 0;
    return freeze_ms > 0 && pRace_ms > (unsigned)freeze_ms && pRace_ms < 0x80000000u;
}

/* In-run A/B (C2_TEST_AB=<feature>): the named feature is switched on and off every 120-frame
 * FPS window (logged as ab=0/1), so both variants see the same frozen scene. */
int C2VitaAB;
const char* C2VitaABName(void) {
    static int read;
    static const char* name;
    if (!read) {
        read = 1;
        name = getenv("C2_TEST_AB");
    }
    return name;
}
int C2VitaABFeature(const char* pFeature, int pDefault) {
    const char* name = C2VitaABName();
    if (name != NULL && strcmp(name, "all61") == 0 && (strcmp(pFeature, "uboskip") == 0 || strcmp(pFeature, "cleanflush") == 0 || strcmp(pFeature, "flagskip") == 0)) {
        return C2VitaAB; /* the 00.61 overlay/uniform savings together */
    }
    return name != NULL && strcmp(name, pFeature) == 0 ? C2VitaAB : pDefault;
}

volatile unsigned char C2VitaPhase = 255;
unsigned char C2VitaPhaseStack[32];
int C2VitaPhaseDepth;
static SDL_Thread* sampler_thread;
static SDL_AtomicInt sampler_stop;
static unsigned sample_hist[256], sample_total;
static int SDLCALL sampler_main(void* unused) {
    (void)unused;
    sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), 0x40000); /* user core 2 */
    while (!SDL_GetAtomicInt(&sampler_stop)) {
        sceKernelDelayThread(100);
        sample_hist[C2VitaPhase]++;
        sample_total++;
    }
    return 0;
}
unsigned C2VitaActorCounts[4], C2VitaNonTrackActors;
static void sampler_log(void) {
    static const char* const names[] = { "input", "ai", "ped", "physics", "render", "sky", "shadows", "world", "hud", "present",
        "tint", "damage", "apo", "hud_scene", "hud_geometry", "hud_redraw", "rays", "g_lookup", "g_render", "g_updatemodel", "g_statecopy",
        "g_props", "g_ubo", "g_draw", "g_actors", "g_model", "g_overlay", "g_transp",
        "p_fill", "p_cull", "p_tex", "p_texparam", "p_blend", "p_depth", "a_push", "a_onscreen", "a_pop", "w_begin", "w_track", "w_nontrack", "w_limbs", "w_effects", "w_end", "h_dim", "h_headups", "h_instruments", "h_powerups", "h_map", "h_pipe", "w_batchcheck", "w_batchadd", "m_copy", "m_overlays", "m_rotate", "m_markers", "m_status", "h_test", "h_stoppre", "h_stoppost", "t_map", "t_mat", "o_pre", "o_cargfx", "o_net", "o_camera", "o_carmunge", "o_headups", "o_effects", "o_skids", "o_glass", "o_lod" };
    char line[900];
    int at = 0;
    unsigned i, total = sample_total;
    if (sampler_thread == NULL || total == 0) return;
    for (i = 0; i < 256; i++) {
        if (sample_hist[i] * 200u >= total && at < (int)sizeof(line) - 40) { /* phases with >= 0.5% */
            at += snprintf(line + at, sizeof(line) - at, " %s=%u.%u%%", i < sizeof(names) / sizeof(names[0]) ? names[i] : (i == 255 ? "other" : "?"),
                sample_hist[i] * 100u / total, sample_hist[i] * 1000u / total % 10);
        }
    }
    { extern void C2VitaLog(const char* format, ...); C2VitaLog("VITA SAMPLE n=%u%s\n", total, line);
      C2VitaLog("VITA ACTORS per frame: walked=%u models=%u identity=%u batch_skipped=%u non_track=%u\n", C2VitaActorCounts[0] / 600, C2VitaActorCounts[1] / 600, C2VitaActorCounts[2] / 600, C2VitaActorCounts[3] / 600, C2VitaNonTrackActors / 600); C2VitaNonTrackActors = 0;
      memset(C2VitaActorCounts, 0, sizeof(C2VitaActorCounts)); }
    memset(sample_hist, 0, sizeof(sample_hist));
    sample_total = 0;
}
/* The monitor never calls GL or inspects a heap that may itself be stuck.
 * Atomics publish only the current main-thread frame, scope mask and heartbeat. */
static SDL_Thread* watchdog_thread;
static SDL_AtomicInt watchdog_stop, watchdog_armed, watchdog_tick, watchdog_frame, watchdog_scopes;
static unsigned watchdog_mask;
static int watchdog_attempted;
static void watchdog_progress(Uint64 now) {
    SDL_SetAtomicInt(&watchdog_tick, (int)(Uint32)(now/1000000));
    SDL_SetAtomicInt(&watchdog_scopes, (int)watchdog_mask);
}
static void watchdog_poll(void) {
    static Uint32 reported_tick;
    static int reported_frame;
    Uint32 tick = (Uint32)SDL_GetAtomicInt(&watchdog_tick);
    Uint32 age = (Uint32)SDL_GetTicks() - tick;
    int current = SDL_GetAtomicInt(&watchdog_frame);
    extern void C2VitaLog(const char* format, ...);
    if (SDL_GetAtomicInt(&watchdog_armed) && age >= 5000
            && tick == (Uint32)SDL_GetAtomicInt(&watchdog_tick)
            && current == SDL_GetAtomicInt(&watchdog_frame)
            && (reported_frame != current || reported_tick != tick)) {
        reported_frame = current; reported_tick = tick;
        {
            extern void C2VitaGLStallInfo(unsigned*, unsigned*, unsigned*, unsigned*, unsigned*);
            unsigned draws, count, mode, program, texture;
            C2VitaGLStallInfo(&draws, &count, &mode, &program, &texture);
            C2VitaLog("VITA STALL last draw #%u this frame: mode=%u indices=%u program=%u texture=%u\n",
                draws, count, mode, program, texture);
        }
        C2VitaLog("VITA STALL frame=%u no_progress_ms=%u active_scopes=0x%04x (input/ai/ped/physics/render/sky/shadows/world/hud/present/tint/damage/apo/hud_scene/geometry/redraw/rays bits0..16)\n",
            (unsigned)current, age, (unsigned)SDL_GetAtomicInt(&watchdog_scopes));
    }
    /* A race frame that has not finished for 20 s will not recover (input is dead and the game would
     * have to be closed). Crash on purpose so the system writes
     * a core dump with every thread's PC and stack, which shows where the main thread is waiting. */
    if (SDL_GetAtomicInt(&watchdog_armed) && age >= 20000
            && tick == (Uint32)SDL_GetAtomicInt(&watchdog_tick)
            && current == SDL_GetAtomicInt(&watchdog_frame)) {
        extern void C2VitaLogFlush(void);
        C2VitaLog("VITA STALL frame=%u no_progress_ms=%u active_scopes=0x%04x: crashing on purpose for a core dump\n",
            (unsigned)current, age, (unsigned)SDL_GetAtomicInt(&watchdog_scopes));
        C2VitaLogFlush();
        *(volatile int*)0 = 0x57A11;
    }
}
static int SDLCALL watchdog_main(void* unused) {
    (void)unused;
    SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (!SDL_GetAtomicInt(&watchdog_stop)) {
        SDL_Delay(1000);
        if (!SDL_GetAtomicInt(&watchdog_stop)) watchdog_poll();
    }
    return 0;
}
void C2VitaPerfShutdown(void) {
    extern void C2VitaLogFlush(void);
    C2VitaLogFlush();
    SDL_SetAtomicInt(&watchdog_armed, 0);
    SDL_SetAtomicInt(&watchdog_stop, 1);
    if (watchdog_thread) SDL_WaitThread(watchdog_thread, NULL);
    watchdog_thread = NULL;
    watchdog_attempted = 0;
}
void C2VitaPerfReset(void) {
    memset(elapsed, 0, sizeof(elapsed));
    memset(depths, 0, sizeof(depths));
    memset(counters, 0, sizeof(counters));
    C2VitaPerfDetail = frame + 1 <= 30 || (frame + 1) % 600 == 0; /* timed detail frames are slower: keep them rare */
    frame_start = SDL_GetTicksNS();
    watchdog_mask = 0;
    watchdog_progress(frame_start);
    SDL_SetAtomicInt(&watchdog_frame, (int)(frame+1));
    SDL_SetAtomicInt(&watchdog_armed, 1);
    if (!watchdog_attempted) {
        watchdog_attempted = 1;
        if (getenv("C2_TEST_SAMPLER") != NULL && sampler_thread == NULL) {
            SDL_SetAtomicInt(&sampler_stop, 0);
            sampler_thread = SDL_CreateThread(sampler_main, "C2 sampler", NULL);
        }
        SDL_SetAtomicInt(&watchdog_stop, 0);
        watchdog_thread = SDL_CreateThread(watchdog_main, "C2 stall monitor", NULL);
        if (!watchdog_thread) {
            extern void C2VitaLog(const char* format, ...);
            C2VitaLog("VITA stall monitor unavailable: %s\n", SDL_GetError());
        }
    }
}
void C2VitaPerfBegin(unsigned scope) {
    C2VitaPhasePush(scope);
    if (scope < C2VP_COUNT && depths[scope]++ == 0) {
        starts[scope] = SDL_GetTicksNS();
        watchdog_mask |= 1u << scope;
        watchdog_progress(starts[scope]);
    }
}
void C2VitaPerfEnd(unsigned scope) {
    C2VitaPhasePop();
    if (scope < C2VP_COUNT && depths[scope] && --depths[scope] == 0) {
        Uint64 now = SDL_GetTicksNS();
        elapsed[scope] += now - starts[scope];
        watchdog_mask &= ~(1u << scope);
        watchdog_progress(now);
    }
}
void C2VitaPerfFinish(void) {
    Uint64 total = SDL_GetTicksNS() - frame_start;
    SDL_SetAtomicInt(&watchdog_armed, 0);
    extern void C2VitaLog(const char* format, ...);
    if ((frame + 1) % 600 == 0) sampler_log();
    if (++frame <= 30 || frame % 600 == 0)
        C2VitaLog("VITA CPU frame=%u total_us=%llu input_us=%llu ai_us=%llu ped_us=%llu physics_us=%llu render_us=%llu sky_us=%llu shadows_us=%llu world_us=%llu hud_us=%llu present_us=%llu tint_us=%llu damage_us=%llu apo_us=%llu hud_scene_us=%llu hud_geometry_us=%llu hud_redraw_us=%llu\n",
            frame, (unsigned long long)(total/1000), (unsigned long long)(elapsed[C2VP_INPUT]/1000),
            (unsigned long long)(elapsed[C2VP_AI]/1000), (unsigned long long)(elapsed[C2VP_PED]/1000),
            (unsigned long long)(elapsed[C2VP_PHYSICS]/1000), (unsigned long long)(elapsed[C2VP_RENDER]/1000),
            (unsigned long long)(elapsed[C2VP_SKY]/1000), (unsigned long long)(elapsed[C2VP_SHADOWS]/1000),
            (unsigned long long)(elapsed[C2VP_WORLD]/1000), (unsigned long long)(elapsed[C2VP_HUD]/1000),
            (unsigned long long)(elapsed[C2VP_PRESENT]/1000), (unsigned long long)(elapsed[C2VP_TINT]/1000),
            (unsigned long long)(elapsed[C2VP_DAMAGE]/1000), (unsigned long long)(elapsed[C2VP_APO]/1000),
            (unsigned long long)(elapsed[C2VP_HUD_SCENE]/1000),
            (unsigned long long)(elapsed[C2VP_HUD_GEOMETRY]/1000), (unsigned long long)(elapsed[C2VP_HUD_REDRAW]/1000));
    extern unsigned long long C2VitaGeometryBytesCompared;
    extern unsigned long long C2VitaGeometryStaticTrusted, C2VitaGeometryStampMisses;
    static unsigned long long trusted_last;
    unsigned long long trusted = C2VitaGeometryStaticTrusted - trusted_last;
    static unsigned long long compared_last;
    unsigned long long compared = C2VitaGeometryBytesCompared - compared_last;
    compared_last = C2VitaGeometryBytesCompared;
    trusted_last = C2VitaGeometryStaticTrusted;
    if (C2VitaPerfDetail)
        C2VitaLog("VITA DRAW frame=%u compared_kb=%llu trusted=%llu stamp_misses_total=%llu models=%u groups=%u cache_misses=%u lookup_us=%llu stored_render_us=%llu updatemodel_us=%llu statecopy_us=%llu props_us=%llu ubo_us=%llu draw_us=%llu actors_us=%llu dbmodel_us=%llu overlay_us=%llu\n",
            frame, compared / 1024, trusted, C2VitaGeometryStampMisses, counters[C2VPC_MODELS], counters[C2VPC_GROUPS], counters[C2VPC_CACHE_MISSES],
            (unsigned long long)(elapsed[C2VP_G_LOOKUP]/1000), (unsigned long long)(elapsed[C2VP_G_RENDER]/1000),
            (unsigned long long)(elapsed[C2VP_G_UPDATEMODEL]/1000), (unsigned long long)(elapsed[C2VP_G_STATECOPY]/1000),
            (unsigned long long)(elapsed[C2VP_G_PROPS]/1000), (unsigned long long)(elapsed[C2VP_G_UBO]/1000),
            (unsigned long long)(elapsed[C2VP_G_DRAW]/1000), (unsigned long long)(elapsed[C2VP_G_ACTORS]/1000),
            (unsigned long long)(elapsed[C2VP_G_MODEL]/1000), (unsigned long long)(elapsed[C2VP_G_OVERLAY]/1000));
}
unsigned C2VitaPerfActiveMask(void) { return watchdog_mask; }
