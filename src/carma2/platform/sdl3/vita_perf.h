#ifndef C2_VITA_PERF_H
#define C2_VITA_PERF_H
enum {
    C2VP_INPUT, C2VP_AI, C2VP_PED, C2VP_PHYSICS, C2VP_RENDER,
    C2VP_SKY, C2VP_SHADOWS, C2VP_WORLD, C2VP_HUD, C2VP_PRESENT, C2VP_TINT, C2VP_DAMAGE, C2VP_APO, C2VP_HUD_SCENE, C2VP_HUD_GEOMETRY, C2VP_HUD_REDRAW, C2VP_RAYS,
    /* detail scopes: only timed on logged frames (C2VitaPerfDetail) */
    C2VP_G_LOOKUP, C2VP_G_RENDER, C2VP_G_UPDATEMODEL, C2VP_G_STATECOPY, C2VP_G_PROPS, C2VP_G_UBO, C2VP_G_DRAW, C2VP_G_ACTORS, C2VP_G_MODEL, C2VP_G_OVERLAY,
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
#endif
