#if defined(__vita__) || defined(C2_GLYPH_BATCH)
#include "vita_glyph.h"
#include "vita_text.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Slots grow only to the scene's high-water mark. Neither the slot nor its
 * prepared model moves: the Vita geometry cache keys on that model pointer.
 * Keep detached slots held until sceneEnd, including deferred renderer work. */
/* The geometry cache compares a slot's whole block every frame: keep it to
 * 64 glyphs (longer strings split into further runs). */
#define GLYPH_VERTICES 256
#define GLYPH_FACES 128
typedef struct glyph_slot {
    struct glyph_slot* next;
    br_actor* actor;
    br_material* material;
    br_material key;
    br_model model;
    v11model prepared;
    v11group group;
    fmt_vertex vertices[GLYPH_VERTICES];
    br_colour colours[GLYPH_VERTICES];
    v11face faces[GLYPH_FACES];
    br_colour face_colours[GLYPH_FACES];
    br_token_value *extra_surf, *extra_prim;
    size_t surf_capacity, prim_capacity;
    int held;
    int material_valid; /* slot->material holds key + tokens, already updated */
    /* hash of everything the run's geometry is built from (glyph models, UV transforms,
     * positions); the prepared block is re-stamped only when it changes, so the geometry
     * cache can trust an unchanged string instead of comparing ~18 KB every frame */
    unsigned long long input_hash, built_hash;
    unsigned changes; /* test log: content changes since the last log */
    float first_x, first_y; /* test log: position of the run's first glyph */
} glyph_slot;
#ifdef __vita__
extern void C2VitaStampPrepared(v11model* m);
#else
#define C2VitaStampPrepared(m) ((void)(m))
#endif
static unsigned long long glyph_hash(unsigned long long h, const void* data, size_t n) {
    const unsigned char* p = (const unsigned char*)data;
    while (n--) { h ^= *p++; h *= 1099511628211ull; }
    return h;
}

static glyph_slot *slots, *run;
static unsigned submitted, draws;

static br_material MaterialKey(const br_material* source) {
    br_material key = *source;
    key._reserved = 0;
    key.identifier = NULL;
    key.user = NULL;
    key.stored = NULL;
    key.extra_surf = NULL;
    key.extra_prim = NULL;
    BrMatrix23Identity(&key.map_transform);
    return key;
}

static int TokensEqual(const br_token_value* a, const br_token_value* b) {
    if (!a || !b) return a == b;
    for (; a->t == b->t; ++a, ++b) {
        if (a->t == BR_NULL_TOKEN) return 1;
        if (memcmp(&a->v, &b->v, sizeof(a->v)) != 0) return 0;
    }
    return 0;
}

static br_token_value* CopyTokens(const br_token_value* src,
        br_token_value** dst, size_t* capacity) {
    size_t n = 0;
    if (!src) return NULL;
    do { ++n; } while (src[n - 1].t != BR_NULL_TOKEN);
    if (n > *capacity) {
        br_token_value* grown = realloc(*dst, n * sizeof(**dst));
        if (!grown) BrFailure("Glyph token pool exhausted");
        *dst = grown;
        *capacity = n;
    }
    memcpy(*dst, src, n * sizeof(**dst));
    return *dst;
}

static glyph_slot* NewRun(br_actor* root, const br_material* material) {
    glyph_slot* slot;
    br_renderer_state_stored* stored;
    char* identifier;
    for (slot = slots; slot && slot->held; slot = slot->next) {}
    if (!slot) {
        slot = calloc(1, sizeof(*slot));
        if (!slot) BrFailure("Glyph model pool exhausted");
        slot->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        slot->material = BrMaterialAllocate("Glyph page run");
        if (!slot->actor || !slot->material) BrFailure("Glyph actor pool exhausted");
        slot->actor->t.type = BR_TRANSFORM_IDENTITY;
        slot->actor->model = &slot->model;
        slot->actor->material = slot->material;
        slot->model.identifier = "Glyph page run";
        slot->model.flags = BR_MODF_PREPREPARED | BR_MODF_CUSTOM_BOUNDS;
        slot->model.prepared = &slot->prepared;
        /* The exact-byte geometry cache requires a contiguous prepared block. */
        slot->prepared.size = offsetof(glyph_slot, extra_surf)
            - offsetof(glyph_slot, prepared);
        slot->prepared.ngroups = 1;
        slot->prepared.groups = &slot->group;
        slot->next = slots;
        slots = slot;
    }
    slot->held = 1;
    slot->group.nvertices = slot->group.nfaces = 0;
    slot->input_hash = 14695981039346656037ull;
    {
        /* Slots are reused in the same order every frame, so a slot usually gets
         * the same page material again: skip the driver state rebuild then. */
        br_material key = MaterialKey(material);
        if (slot->material_valid && memcmp(&slot->key, &key, sizeof(key)) == 0
                && TokensEqual(slot->material->extra_surf, material->extra_surf)
                && TokensEqual(slot->material->extra_prim, material->extra_prim)) {
            BrActorAdd(root, slot->actor);
            ++draws;
            return slot;
        }
        slot->key = key;
    }
    stored = slot->material->stored;
    identifier = slot->material->identifier;
    *slot->material = slot->key;
    slot->material->stored = stored;
    slot->material->identifier = identifier;
    slot->material->extra_surf = CopyTokens(material->extra_surf,
        &slot->extra_surf, &slot->surf_capacity);
    slot->material->extra_prim = CopyTokens(material->extra_prim,
        &slot->extra_prim, &slot->prim_capacity);
    BrMaterialUpdate(slot->material, BR_MATU_ALL);
    slot->material_valid = 1;
    /* BrActorAdd inserts at the head, as it does for the original glyphs. */
    BrActorAdd(root, slot->actor);
    ++draws;
    return slot;
}

void C2VitaGlyphStringBegin(void) { run = NULL; }

void C2VitaGlyphStringEnd(void) {
    unsigned f, v, axis, base;
    if (!run) return;
    base = GLYPH_VERTICES - run->group.nvertices;
    run->group.vertices = run->vertices + base;
    run->group.vertex_colours = run->colours + base;
    run->group.faces = run->faces + GLYPH_FACES - run->group.nfaces;
    run->group.face_colours.colours = run->face_colours + GLYPH_FACES - run->group.nfaces;
    for (f = 0; f < run->group.nfaces; ++f)
        for (v = 0; v < 3; ++v)
            run->group.faces[f].vertices[v] -= base;
    if (run->input_hash != run->built_hash || run->built_hash == 0) {
        run->changes++;
        run->built_hash = run->input_hash;
        C2VitaStampPrepared(&run->prepared);
    }
    run->model.bounds.min = run->model.bounds.max = run->group.vertices[0].p;
    for (v = 1; v < run->group.nvertices; ++v)
        for (axis = 0; axis < 3; ++axis) {
            float p = run->group.vertices[v].p.v[axis];
            if (p < run->model.bounds.min.v[axis]) run->model.bounds.min.v[axis] = p;
            if (p > run->model.bounds.max.v[axis]) run->model.bounds.max.v[axis] = p;
        }
    run = NULL;
}

void C2VitaGlyphSubmit(br_actor* root, const br_model* model,
        const br_material* material, float x, float y) {
    const v11group* source = &model->prepared->groups[0];
    br_material key = MaterialKey(material);
    unsigned vb, fb, i, j;
    if (model->prepared->ngroups != 1 || source->nvertices > GLYPH_VERTICES
            || source->nfaces > GLYPH_FACES)
        BrFailure("Unexpected polyfont glyph geometry");
    if (run && (memcmp(&run->key, &key, sizeof(key)) != 0
            || !TokensEqual(run->material->extra_surf, material->extra_surf)
            || !TokensEqual(run->material->extra_prim, material->extra_prim)
            || run->group.nvertices + source->nvertices > GLYPH_VERTICES
            || run->group.nfaces + source->nfaces > GLYPH_FACES))
        C2VitaGlyphStringEnd();
    if (!run) run = NewRun(root, material);
    /* Fill backwards to preserve the legacy reverse actor traversal WITHIN
     * each page run too. Overlapping glyphs must retain their painter order. */
    if (run->group.nvertices == 0) {
        run->first_x = x;
        run->first_y = y;
    }
    vb = GLYPH_VERTICES - run->group.nvertices - source->nvertices;
    fb = GLYPH_FACES - run->group.nfaces - source->nfaces;
    for (i = 0; i < source->nvertices; ++i) {
        fmt_vertex* dest = &run->vertices[vb + i];
        float u = source->vertices[i].map.v[0];
        float v = source->vertices[i].map.v[1];
        *dest = source->vertices[i];
        dest->p.v[0] += x;
        dest->p.v[1] -= y;
        dest->p.v[2] -= 1.1f;
        dest->map.v[0] = u * material->map_transform.m[0][0]
            + v * material->map_transform.m[1][0] + material->map_transform.m[2][0];
        dest->map.v[1] = u * material->map_transform.m[0][1]
            + v * material->map_transform.m[1][1] + material->map_transform.m[2][1];
        run->colours[vb + i] = source->vertex_colours[i];
    }
    for (i = 0; i < source->nfaces; ++i) {
        run->faces[fb + i] = source->faces[i];
        for (j = 0; j < 3; ++j) {
            run->faces[fb + i].vertices[j] += vb;
            run->faces[fb + i].edges[j] = 0;
        }
        run->faces[fb + i].eqn.v[3] += source->faces[i].eqn.v[0] * x
            - source->faces[i].eqn.v[1] * y - source->faces[i].eqn.v[2] * 1.1f;
        run->face_colours[fb + i] = source->face_colours.colours[i];
    }
    run->group.nvertices += source->nvertices;
    run->group.nfaces += source->nfaces;
    {
        const void* model_id = model;
        run->input_hash = glyph_hash(run->input_hash, &model_id, sizeof(model_id));
        run->input_hash = glyph_hash(run->input_hash, &material->map_transform, sizeof(material->map_transform));
        run->input_hash = glyph_hash(run->input_hash, &x, sizeof(x));
        run->input_hash = glyph_hash(run->input_hash, &y, sizeof(y));
        run->input_hash = glyph_hash(run->input_hash, &source->nvertices, sizeof(source->nvertices));
    }
    ++submitted;
}

int C2VitaGlyphHasActors(void) {
    glyph_slot* slot;
    for (slot = slots; slot; slot = slot->next)
        if (slot->actor->parent) return 1;
    return 0;
}

/* The HUD draws its text in two batches per frame. Releasing the slots after the first batch let the
 * second one reuse them, so six slots alternated between two strings and were rebuilt and re-uploaded
 * twice a frame (~170 KB). Held until the frame swap, every batch keeps its own slots and their
 * geometry stays unchanged from frame to frame. */
static int HoldUntilFrameEnd(void) {
#ifdef __vita__
    extern int C2VitaABFeature(const char* pFeature, int pDefault);
    return C2VitaABFeature("glyphhold", 1);
#else
    return 0;
#endif
}

void C2VitaGlyphFrameEnd(void) {
    glyph_slot* slot;
    for (slot = slots; slot; slot = slot->next)
        if (!slot->actor->parent) slot->held = 0;
}

void C2VitaGlyphClean(void) {
    glyph_slot* slot;
    C2VitaGlyphStringEnd();
    for (slot = slots; slot; slot = slot->next) {
        if (slot->actor->parent) BrActorRemove(slot->actor);
        if (!C2VitaTextSceneIsOpen() && !HoldUntilFrameEnd()) slot->held = 0;
    }
}

void C2VitaGlyphSceneEnded(void) {
    glyph_slot* slot;
    for (slot = slots; slot; slot = slot->next)
        if (!slot->actor->parent) slot->held = 0;
}

/* test log (C2_TEST_GLYPHLOG): per slot, content changes over the last 600 frames */
void C2VitaGlyphLog(unsigned pFrames) {
    static int on = -1;
    glyph_slot* slot;
    int i = 0;
    extern void C2VitaLog(const char* format, ...);
    if (on < 0) on = getenv("C2_TEST_GLYPHLOG") != NULL;
    if (!on) return;
    for (slot = slots; slot; slot = slot->next, i++) {
        C2VitaLog("VITA GLYPH slot=%d changes=%u/%u vertices=%u first=%.1f,%.1f\n", i, slot->changes, pFrames,
            (unsigned)slot->group.nvertices, (double)slot->first_x, (double)slot->first_y);
        slot->changes = 0;
    }
}

void C2VitaGlyphStats(unsigned* glyphs, unsigned* batches) {
    *glyphs = submitted;
    *batches = draws;
    submitted = draws = 0;
}
#endif
