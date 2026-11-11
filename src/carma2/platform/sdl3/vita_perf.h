#ifndef C2_VITA_PERF_H
#define C2_VITA_PERF_H
enum {
    C2VP_INPUT, C2VP_AI, C2VP_PED, C2VP_PHYSICS, C2VP_RENDER,
    C2VP_SKY, C2VP_SHADOWS, C2VP_WORLD, C2VP_HUD, C2VP_PRESENT, C2VP_TINT, C2VP_DAMAGE, C2VP_APO, C2VP_HUD_SCENE, C2VP_HUD_GEOMETRY, C2VP_HUD_REDRAW, C2VP_RAYS,
    /* detail scopes: only timed on logged frames (C2VitaPerfDetail) */
    C2VP_G_LOOKUP, C2VP_G_RENDER, C2VP_G_UPDATEMODEL, C2VP_G_STATECOPY, C2VP_G_PROPS, C2VP_G_UBO, C2VP_G_DRAW, C2VP_G_ACTORS, C2VP_G_MODEL, C2VP_G_OVERLAY, C2VP_G_TRANSP,
    /* sampler-only phases inside the state setup (StoredGLApplyProperties) */
    C2VP_P_FILL, C2VP_P_CULL, C2VP_P_TEX, C2VP_P_TEXPARAM, C2VP_P_BLEND, C2VP_P_DEPTH,
    /* sampler-only phases inside actorRender */
    C2VP_A_PUSH, C2VP_A_ONSCREEN, C2VP_A_POP,
    /* sampler-only phases inside the world pass (graphics.c) */
    C2VP_W_BEGIN, C2VP_W_TRACK, C2VP_W_NONTRACK, C2VP_W_LIMBS, C2VP_W_EFFECTS, C2VP_W_END,
    C2VP_H_DIM, C2VP_H_HEADUPS, C2VP_H_INSTRUMENTS, C2VP_H_POWERUPS, C2VP_H_MAP, C2VP_H_PIPE, C2VP_W_BATCHCHECK, C2VP_W_BATCHADD,
    C2VP_M_COPY, C2VP_M_OVERLAYS, C2VP_M_ROTATE, C2VP_M_MARKERS, C2VP_M_STATUS,
    C2VP_H_TEST, C2VP_H_STOPPRE, C2VP_H_STOPPOST, C2VP_T_MAP, C2VP_T_MAT,
    /* main loop regions outside the timed scopes (C2VitaPhaseMark) */
    C2VP_O_PRE, C2VP_O_CARGFX, C2VP_O_NET, C2VP_O_CAMERA, C2VP_O_CARMUNGE, C2VP_O_HEADUPS, C2VP_O_EFFECTS, C2VP_O_SKIDS, C2VP_O_GLASS, C2VP_O_LOD,
    C2VP_COUNT
};
void C2VitaPerfReset(void);
void C2VitaPerfShutdown(void);
void C2VitaPerfBegin(unsigned scope);
void C2VitaPerfEnd(unsigned scope);
void C2VitaPerfFinish(void);
extern int C2VitaPerfDetail; /* nonzero on frames whose timings are logged */
void C2VitaPerfCount(unsigned counter);
enum { C2VPC_MODELS, C2VPC_GROUPS, C2VPC_CACHE_MISSES, C2VPC_COUNT };
/* Sampling profiler (C2_TEST_SAMPLER): the innermost scope the main thread is in, read every
 * 100 us by a thread on core 2. 255 = outside every scope. */
int C2VitaTestFrozen(unsigned pRace_ms);
extern int C2VitaAB;
const char* C2VitaABName(void);
int C2VitaABFeature(const char* pFeature, int pDefault);
extern volatile unsigned char C2VitaPhase;
extern unsigned char C2VitaPhaseStack[32];
extern int C2VitaPhaseDepth;
static inline void C2VitaPhasePush(unsigned pPhase) {
    if (C2VitaPhaseDepth < 32) C2VitaPhaseStack[C2VitaPhaseDepth] = C2VitaPhase;
    C2VitaPhaseDepth++;
    C2VitaPhase = (unsigned char)pPhase;
}
/* Labels the main loop's top level (not inside any scope) for the sampler. */
static inline void C2VitaPhaseMark(unsigned pPhase) {
    if (C2VitaPhaseDepth == 0) C2VitaPhase = (unsigned char)pPhase;
}
static inline void C2VitaPhasePop(void) {
    if (C2VitaPhaseDepth > 0 && --C2VitaPhaseDepth < 32) C2VitaPhase = C2VitaPhaseStack[C2VitaPhaseDepth];
}
#endif
