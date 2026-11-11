/* Port: static track batching.
 *
 * The track is split into columns of small models, and BRender draws every model as one draw per
 * material, so a race frame issues several hundred tiny draws (most under 10 faces) whose CPU cost
 * is almost all per-draw overhead. After the track has loaded, the opaque faces of the fixed track
 * models of each block of columns (by default one block: the whole track) are copied, in world space, into merged models,
 * which BRender prepares as one draw per material. RenderTrack draws the blocks overlapping the
 * visible columns. For a merged model BrDbModelRender draws a substitute holding only its
 * remaining faces (those the renderer draws blended, which must stay in its sorted transparent
 * pass), or nothing. The original models stay in the actor tree and keep being used for collision,
 * picking and everything else; only what is drawn changes.
 *
 * Only models that never change are merged: not non-car or smashable objects ('&' actors), not
 * animated (groovidelic) actors, not custom or hidden models. If a merged model is updated or its
 * actor changes model or render style (or a smashable object moves) during the race, its faces are
 * taken out of its block, which is prepared again, and its original is drawn again. TRACKBATCH.OFF (Vita) or C2_TEST_NOBATCH (desktop) disables it. */
#include "trackbatch.h"
#include "loadprof.h"

#ifndef CARPOCALYPSE2_MATCHING
#include "brender/brender.h"
#include "carpocalypse2_types.h"
#include "globvars.h"
#include "world.h"
#include "errors.h"
#ifdef __vita__
#include "sdl3/vita_perf.h"
#endif

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BATCH_VERTICES 60000
#define MAX_BATCH_FACES 60000

typedef struct {
    br_actor* actor;
    br_model* model;
    br_model* substitute; /* the faces still drawn per model (blended ones), or NULL */
    br_uint_8 style;
    int block;
    int opaque_faces;
    br_actor* parent;   /* smashables: knocked off or removed when this changes */
    br_transform t;     /* smashables: moved when this changes */
    int face_first;     /* where its faces start in its block's model (as built) */
    int model_changed;  /* its model was updated (BrModelUpdate -> C2TrackBatchModelChanged) */
    br_matrix34 world;  /* its model-to-world matrix when merged (test check) */
} tBatch_source;

typedef struct {
    br_actor* actor;
    br_model* model;
    int first_source;
    int count_sources;
    int x0, z0, x1, z1; /* columns covered, inclusive */
    int enabled;
    int smash; /* a block of smashable objects (C2_TEST_AB=smashbatch toggles these) */
    int nfaces; /* faces of its model as built */
} tBatch_block;

static int gSmash_on = 1;


static int BlockActive(const tBatch_block* pBlock) {
    return pBlock->enabled && (!pBlock->smash || gSmash_on);
}

static tBatch_block* gBlocks;
static int gCount_blocks;
static tBatch_source* gSources;
static int gCount_sources;
static int gCapacity_sources;

/* pointer -> int, open addressing (actors -> source index, materials -> transparency) */
typedef struct {
    const void** keys;
    int* values;
    unsigned int size;
} tPointer_map;

static tPointer_map gActor_map;
static tPointer_map gMaterial_map;

static unsigned int HashPointer(const void* p) {
    uintptr_t v = (uintptr_t)p;
    v ^= v >> 16;
    v *= 0x45d9f3bu;
    v ^= v >> 13;
    return (unsigned int)v;
}

static int MapInit(tPointer_map* pMap, unsigned int pEntries) {
    for (pMap->size = 64; pMap->size < pEntries * 2; pMap->size *= 2) {
    }
    pMap->keys = calloc(pMap->size, sizeof(void*));
    pMap->values = calloc(pMap->size, sizeof(int));
    return pMap->keys != NULL && pMap->values != NULL;
}

static void MapFree(tPointer_map* pMap) {
    free((void*)pMap->keys);
    free(pMap->values);
    pMap->keys = NULL;
    pMap->values = NULL;
    pMap->size = 0;
}

static void MapPut(tPointer_map* pMap, const void* pKey, int pValue) {
    unsigned int i = HashPointer(pKey) & (pMap->size - 1);
    while (pMap->keys[i] != NULL && pMap->keys[i] != pKey) {
        i = (i + 1) & (pMap->size - 1);
    }
    pMap->keys[i] = pKey;
    pMap->values[i] = pValue;
}

static int MapGet(const tPointer_map* pMap, const void* pKey) {
    unsigned int i;
    if (pMap->size == 0) {
        return -1;
    }
    i = HashPointer(pKey) & (pMap->size - 1);
    while (pMap->keys[i] != NULL) {
        if (pMap->keys[i] == pKey) {
            return pMap->values[i];
        }
        i = (i + 1) & (pMap->size - 1);
    }
    return -1;
}

/* SMASHBATCH.OFF (Vita) / C2_TEST_NOSMASHBATCH: smashable objects are not batched */
static int SmashDisabled(void) {
#ifdef __vita__
    FILE* f = fopen("ux0:/data/c2port/SMASHBATCH.OFF", "rb");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
#else
    return getenv("C2_TEST_NOSMASHBATCH") != NULL;
#endif
}

static int Disabled(void) {
#ifdef __vita__
    FILE* f = fopen("ux0:/data/c2port/TRACKBATCH.OFF", "rb");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
#else
    return getenv("C2_TEST_NOBATCH") != NULL;
#endif
}

/* Whether the GL renderer draws this material in its sorted transparent pass (or out of depth
 * order): as glrend decides it, RGBA 4444 textures are blended when any pixel is not opaque and
 * 32-bit RGBA ones always. Such faces are not merged. */
static int ComputeTransparent(const br_material* pMaterial) {
    const br_pixelmap* map;
    int x, y;

    if (pMaterial->opacity != 255 || pMaterial->index_blend != NULL
            || (pMaterial->flags & (BR_MATF_FORCE_FRONT | BR_MATF_FORCE_BACK))) {
        return 1;
    }
    map = pMaterial->colour_map;
    if (map == NULL) {
        return 0;
    }
    if (map->type == BR_PMT_RGBA_8888 || map->type == BR_PMT_ARGB_8888) {
        return 1;
    }
    if (map->type != BR_PMT_RGBA_4444) {
        return 0;
    }
    if (map->pixels == NULL) {
        return 1;
    }
    /* alpha only 0 or 15 is a cut-out, drawn unblended (glrend sbuffer.c): opaque here too */
    for (y = 0; y < map->height; y++) {
        const br_uint_16* row = (const br_uint_16*)((const br_uint_8*)map->pixels + (size_t)(map->base_y + y) * map->row_bytes) + map->base_x;
        for (x = 0; x < map->width; x++) {
            if ((row[x] >> 12) != 0xf && ((row[x] >> 12) != 0 || getenv("C2_TEST_NOCUTOUT") != NULL)) {
                return 1;
            }
        }
    }
    return 0;
}

static int IsTransparent(const br_material* pMaterial) {
    int known;

    if (pMaterial == NULL) {
        return 0;
    }
    known = MapGet(&gMaterial_map, pMaterial);
    if (known < 0) {
        known = ComputeTransparent(pMaterial);
        MapPut(&gMaterial_map, pMaterial, known);
    }
    return known;
}

static br_material* InheritedMaterial(br_actor* pActor) {
    br_actor* a;
    for (a = pActor; a != NULL; a = a->parent) {
        if (a->material != NULL) {
            return a->material;
        }
    }
    return NULL;
}

static int IsGrooved(const br_actor* pActor) {
    int i;
    for (i = 0; i < gGroovidelics_array_size; i++) {
        if (gGroovidelics_array[i].actor == pActor) {
            return 1;
        }
    }
    return 0;
}

static int Eligible(br_actor* pActor) {
    br_model* model = pActor->model;

    if (pActor->type != BR_ACTOR_MODEL || model == NULL) {
        return 0;
    }
    if (pActor->render_style != BR_RSTYLE_DEFAULT && pActor->render_style != BR_RSTYLE_FACES) {
        return 0;
    }
    if (model->flags & (BR_MODF_CUSTOM | BR_MODF_PREPREPARED)) {
        return 0;
    }
    if (model->nfaces == 0 || model->nvertices == 0 || model->faces == NULL || model->vertices == NULL || model->prepared == NULL) {
        return 0;
    }
    return model->nvertices <= MAX_BATCH_VERTICES && model->nfaces <= MAX_BATCH_FACES;
}

static int CountOpaque(br_actor* pActor, br_model* pModel) {
    br_material* inherited = InheritedMaterial(pActor);
    int f, opaque = 0;

    for (f = 0; f < pModel->nfaces; f++) {
        opaque += !IsTransparent(pModel->faces[f].material != NULL ? pModel->faces[f].material : inherited);
    }
    return opaque;
}

static void AddSource(br_actor* pActor) {
    br_model* model = pActor->model;
    int opaque = CountOpaque(pActor, model);

    if (opaque == 0) {
        return; /* nothing to merge */
    }
    if (gCount_sources == gCapacity_sources) {
        int capacity = gCapacity_sources ? gCapacity_sources * 2 : 256;
        tBatch_source* grown = realloc(gSources, capacity * sizeof(tBatch_source));
        if (grown == NULL) {
            return;
        }
        gSources = grown;
        gCapacity_sources = capacity;
    }
    memset(&gSources[gCount_sources], 0, sizeof(tBatch_source));
    gSources[gCount_sources].actor = pActor;
    gSources[gCount_sources].model = model;
    gSources[gCount_sources].style = pActor->render_style;
    gSources[gCount_sources].block = -1;
    gSources[gCount_sources].opaque_faces = opaque;
    gSources[gCount_sources].parent = pActor->parent;
    gSources[gCount_sources].t = pActor->t;
    gCount_sources++;
}

/* Collects the eligible model actors of a column subtree. */
static void Collect(br_actor* pActor) {
    br_actor* child;

    if (pActor->identifier != NULL && pActor->identifier[0] == '&') {
        return; /* non-car / smashable object and everything attached to it */
    }
    if (pActor->render_style == BR_RSTYLE_NONE || IsGrooved(pActor)) {
        return;
    }
    if (Eligible(pActor)) {
        AddSource(pActor);
    }
    for (child = pActor->children; child != NULL; child = child->next) {
        Collect(child);
    }
}

/* Collects the smashable objects ('&' actors) of a column subtree that can be merged: childless,
 * not animated (groovidelic), drawn normally. They are batched in small blocks of their own; any
 * change to one (smashed, swapped model, knocked off, moved) switches its block off. */
static void CollectSmash(br_actor* pActor) {
    br_actor* child;

    if (pActor->render_style == BR_RSTYLE_NONE || IsGrooved(pActor)) {
        return;
    }
    if (pActor->identifier != NULL && pActor->identifier[0] == '&') {
        if (pActor->children == NULL && Eligible(pActor)) {
            AddSource(pActor);
        }
        return;
    }
    for (child = pActor->children; child != NULL; child = child->next) {
        CollectSmash(child);
    }
}

/* Copies the faces of pSource whose transparency equals pTransparent into pDest (vertices are
 * copied whole, positions and normals transformed by pMatrix when given). */
static void CopyFaces(br_model* pDest, int* pV_at, int* pF_at, const tBatch_source* pSource, const br_matrix34* pMatrix, int pTransparent) {
    br_model* src = pSource->model;
    br_material* inherited = InheritedMaterial(pSource->actor);
    int v, f;

    for (v = 0; v < src->nvertices; v++) {
        br_vertex* dst = &pDest->vertices[*pV_at + v];
        *dst = src->vertices[v];
        if (pMatrix != NULL) {
            BrMatrix34ApplyP(&dst->p, &src->vertices[v].p, pMatrix);
            BrMatrix34ApplyV((br_vector3*)&dst->n, (const br_vector3*)&src->vertices[v].n, pMatrix);
        }
    }
    for (f = 0; f < src->nfaces; f++) {
        br_material* m = src->faces[f].material != NULL ? src->faces[f].material : inherited;
        br_face* dst;
        if (IsTransparent(m) != pTransparent) {
            continue;
        }
        dst = &pDest->faces[(*pF_at)++];
        *dst = src->faces[f];
        dst->vertices[0] = (br_uint_16)(src->faces[f].vertices[0] + *pV_at);
        dst->vertices[1] = (br_uint_16)(src->faces[f].vertices[1] + *pV_at);
        dst->vertices[2] = (br_uint_16)(src->faces[f].vertices[2] + *pV_at);
        if (pMatrix != NULL && dst->material == NULL) {
            dst->material = inherited;
        }
    }
    *pV_at += src->nvertices;
}

/* The merged model of gSources[first .. first + count) (those of block pOnly_block, if not -1):
 * their opaque faces in world space. */
static int gBuilt_faces; /* faces of the last BuildBlockModel (prepared models keep none) */

static br_model* BuildBlockModel(int pFirst, int pCount, int pOnly_block) {
    int nvertices = 0, nfaces = 0, i, v_at = 0, f_at = 0;
    br_model* model;

    for (i = pFirst; i < pFirst + pCount; i++) {
        if (pOnly_block >= 0 && gSources[i].block != pOnly_block) {
            continue;
        }
        /* counted again: CopyFaces below copies exactly these */
        gSources[i].opaque_faces = CountOpaque(gSources[i].actor, gSources[i].model);
        nvertices += gSources[i].model->nvertices;
        nfaces += gSources[i].opaque_faces;
    }
    gBuilt_faces = nfaces;
    unsigned long long note_t = C2LoadDetail() ? C2LoadMicros() : 0;
    model = BrModelAllocate("C2 track batch", nvertices, nfaces);
    if (model == NULL) {
        return NULL;
    }
    for (i = pFirst; i < pFirst + pCount; i++) {
        br_matrix34 m;
        if (pOnly_block >= 0 && gSources[i].block != pOnly_block) {
            continue;
        }
        BrActorToActorMatrix34(&m, gSources[i].actor, gUniverse_actor);
        gSources[i].face_first = f_at;
        gSources[i].world = m;
        CopyFaces(model, &v_at, &f_at, &gSources[i], &m, 0);
    }
    /* keep every source's own vertices: welding coincident vertices across sources would change
     * their shared normals */
    model->flags = BR_MODF_DONT_WELD;
    if (getenv("C2_TEST_BLOCKIMMEDIATE") == NULL) {
        /* drawn from GPU buffers built once (the renderer's stored geometry), not from the prepared
         * data each frame; the prepared faces stay, so HideFaces can edit them */
        model->flags |= BR_MODF_FACES_ONLY;
    }
    if (C2LoadDetail()) { unsigned long long now_ = C2LoadMicros(); C2LoadNote("batch-copy", now_ - note_t); note_t = now_; }
    BrModelAdd(model);
    if (C2LoadDetail()) C2LoadNote("batch-prepare", C2LoadMicros() - note_t);
    return model;
}

/* The faces a merged source still draws itself (blended ones), in its own space. */
static br_model* BuildSubstitute(const tBatch_source* pSource) {
    int nfaces = pSource->model->nfaces - pSource->opaque_faces;
    int v_at = 0, f_at = 0;
    br_model* model;

    if (nfaces == 0) {
        return NULL;
    }
    model = BrModelAllocate("C2 track batch rest", pSource->model->nvertices, nfaces);
    if (model == NULL) {
        return NULL;
    }
    CopyFaces(model, &v_at, &f_at, pSource, NULL, 1);
    model->flags = pSource->model->flags & ~(C2_MODF_BATCHED | BR_MODF_UPDATEABLE);
    BrModelAdd(model);
    return model;
}

static void LogBatch(const char* pFormat, int pA, int pB, unsigned int pC, int pD) {
    dr_dprintf(pFormat, pA, pB, pC, pD);
#ifdef __vita__
    {
        extern void C2VitaLog(const char* format, ...);
        char line[256];
        snprintf(line, sizeof(line), pFormat, pA, pB, pC, pD);
        C2VitaLog("%s\n", line);
    }
#endif
}

/* Splits gSources[pFirst ..) into merged block models under the 16-bit vertex/face limits. */
static void MakeBlocks(int pFirst, int pCapacity, int pX0, int pZ0, int pX1, int pZ1, int pSmash, unsigned int* pFaces, int* pSubstitutes) {
    int at = pFirst, i;

    while (at < gCount_sources && gCount_blocks < pCapacity) {
        int end = at, nv = 0, nf = 0;
        tBatch_block* block = &gBlocks[gCount_blocks];

        while (end < gCount_sources && nv + gSources[end].model->nvertices <= MAX_BATCH_VERTICES
                && nf + gSources[end].opaque_faces <= MAX_BATCH_FACES) {
            nv += gSources[end].model->nvertices;
            nf += gSources[end].opaque_faces;
            end++;
        }
        block->model = BuildBlockModel(at, end - at, -1);
        block->nfaces = gBuilt_faces;
        if (block->model == NULL) {
            gCount_sources = at; /* drop these sources: their originals keep drawing */
            break;
        }
        {
            unsigned long long note_t = C2LoadDetail() ? C2LoadMicros() : 0;
            for (i = at; i < end; i++) {
                gSources[i].substitute = BuildSubstitute(&gSources[i]);
                *pSubstitutes += gSources[i].substitute != NULL;
            }
            if (C2LoadDetail()) C2LoadNote("batch-substitutes", C2LoadMicros() - note_t);
        }
        block->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        block->actor->model = block->model;
        block->actor->render_style = BR_RSTYLE_DEFAULT;
        block->first_source = at;
        block->count_sources = end - at;
        block->x0 = pX0;
        block->z0 = pZ0;
        block->x1 = pX1;
        block->z1 = pZ1;
        block->enabled = 1;
        block->smash = pSmash;
        for (i = at; i < end; i++) {
            gSources[i].block = gCount_blocks;
            gSources[i].model->flags |= C2_MODF_BATCHED;
        }
        *pFaces += nf;
        gCount_blocks++;
        at = end;
    }
    /* sources past a failed block were dropped above; any left over (no block capacity) too */
    if (at < gCount_sources) {
        gCount_sources = at;
    }
}

/* Columns per side of a smashable block. Each block costs a draw per material, every source of a
 * drawn block is checked each frame, and a changed source is hidden in place (HideFaces); test:
 * C2_TEST_SMASHCELL=<n>. */
#define SMASH_CELL_DEFAULT 4

void C2TrackBatchBuild(tTrack_spec* pTrack_spec) {
    int bx, bz, i;
    int blocks_x, blocks_z;
    int capacity_blocks;
    unsigned int faces = 0;
    int substitutes = 0;

    int block_size;
    int track_sources, track_blocks;
    int smash_cell = SMASH_CELL_DEFAULT;

    C2TrackBatchDispose();
    if (pTrack_spec->columns == NULL || Disabled()) {
        return;
    }
    /* Blocks cover the whole track by default: its opaque geometry is only some 15-20k faces, a
     * trivial load for the GPU, and one block means about one draw per track material. Geometry
     * outside the view is clipped (beyond the draw distance by the far plane, as before). */
    block_size = pTrack_spec->ncolumns_x > pTrack_spec->ncolumns_z ? pTrack_spec->ncolumns_x : pTrack_spec->ncolumns_z;
#ifndef __vita__
    if (getenv("C2_TEST_BATCHBLOCK") != NULL && atoi(getenv("C2_TEST_BATCHBLOCK")) > 0) {
        block_size = atoi(getenv("C2_TEST_BATCHBLOCK"));
    }
#endif
    if (!MapInit(&gMaterial_map, 4096)) {
        return;
    }
    if (getenv("C2_TEST_SMASHCELL") != NULL && atoi(getenv("C2_TEST_SMASHCELL")) > 0) {
        smash_cell = atoi(getenv("C2_TEST_SMASHCELL"));
    }
    blocks_x = (pTrack_spec->ncolumns_x + block_size - 1) / block_size;
    blocks_z = (pTrack_spec->ncolumns_z + block_size - 1) / block_size;
    capacity_blocks = blocks_x * blocks_z * 4
        + ((pTrack_spec->ncolumns_x + smash_cell - 1) / smash_cell) * ((pTrack_spec->ncolumns_z + smash_cell - 1) / smash_cell) * 2;
    gBlocks = calloc(capacity_blocks, sizeof(tBatch_block));
    if (gBlocks == NULL) {
        MapFree(&gMaterial_map);
        return;
    }
    for (bz = 0; bz < blocks_z; bz++) {
        for (bx = 0; bx < blocks_x; bx++) {
            int first = gCount_sources;
            int x, z;

            for (z = bz * block_size; z < (bz + 1) * block_size && z < pTrack_spec->ncolumns_z; z++) {
                for (x = bx * block_size; x < (bx + 1) * block_size && x < pTrack_spec->ncolumns_x; x++) {
                    br_actor* column = pTrack_spec->columns[z][x].actor_0x0;
                    if (column != NULL) {
                        Collect(column);
                    }
                }
            }
            MakeBlocks(first, capacity_blocks, bx * block_size, bz * block_size, bx * block_size + block_size - 1,
                bz * block_size + block_size - 1, 0, &faces, &substitutes);
        }
    }
    track_sources = gCount_sources;
    track_blocks = gCount_blocks;
    if (!SmashDisabled()) {
        for (bz = 0; bz < pTrack_spec->ncolumns_z; bz += smash_cell) {
            for (bx = 0; bx < pTrack_spec->ncolumns_x; bx += smash_cell) {
                int first = gCount_sources;
                int x, z;

                for (z = bz; z < bz + smash_cell && z < pTrack_spec->ncolumns_z; z++) {
                    for (x = bx; x < bx + smash_cell && x < pTrack_spec->ncolumns_x; x++) {
                        br_actor* column = pTrack_spec->columns[z][x].actor_0x0;
                        if (column != NULL) {
                            CollectSmash(column);
                        }
                    }
                }
                MakeBlocks(first, capacity_blocks, bx, bz, bx + smash_cell - 1, bz + smash_cell - 1, 1, &faces, &substitutes);
            }
        }
    }
    if (!MapInit(&gActor_map, gCount_sources)) {
        C2TrackBatchDispose();
        return;
    }
    for (i = 0; i < gCount_sources; i++) {
        MapPut(&gActor_map, gSources[i].actor, i);
    }
    LogBatch("TRACK BATCH: %d blocks from %d track models (%u merged faces, %d keep their blended faces)",
        gCount_blocks, gCount_sources, faces, substitutes);
    LogBatch("TRACK BATCH: of these %d smashable blocks from %d objects", gCount_blocks - track_blocks,
        gCount_sources - track_sources, 0u, 0);
    if (gCount_blocks > 0) {
        LogBatch("TRACK BATCH: block 0 has stored geometry %d, prepared %d bytes, %u faces", gBlocks[0].model->stored != NULL,
            gBlocks[0].model->prepared != NULL ? (int)((v11model*)gBlocks[0].model->prepared)->size : -1, (unsigned)gBlocks[0].nfaces, 0);
    }
}

static void DisableBlock(int pBlock, const char* pReason) {
    if (gBlocks[pBlock].enabled) {
        gBlocks[pBlock].enabled = 0;
        dr_dprintf("TRACK BATCH: block %d off (%s)", pBlock, pReason);
    }
}

void C2TrackBatchDispose(void) {
    int i;

    for (i = 0; i < gCount_sources; i++) {
        gSources[i].model->flags &= ~C2_MODF_BATCHED;
        if (gSources[i].substitute != NULL) {
            BrModelRemove(gSources[i].substitute);
            BrModelFree(gSources[i].substitute);
        }
    }
    for (i = 0; i < gCount_blocks; i++) {
        if (gBlocks[i].actor != NULL) {
            gBlocks[i].actor->model = NULL;
            BrActorFree(gBlocks[i].actor);
        }
        if (gBlocks[i].model != NULL) {
            BrModelRemove(gBlocks[i].model);
            BrModelFree(gBlocks[i].model);
        }
    }
    free(gBlocks);
    free(gSources);
    MapFree(&gActor_map);
    MapFree(&gMaterial_map);
    gBlocks = NULL;
    gSources = NULL;
    gCount_blocks = gCount_sources = gCapacity_sources = 0;
}

static int SourceStillValid(const tBatch_source* pSource, int pSmash);

/* The source of pActor drawn with pModel, if a block that is on draws it (-1 if not). Blocks are
 * checked only when drawn (C2TrackBatchRender), so a source of an unseen block is checked here, as
 * its actor is walked (e.g. a smashable knocked into view): a changed one draws itself, and its
 * block takes it out when next drawn. */
static int ClaimedSource(const br_actor* pActor, const br_model* pModel) {
    int s = MapGet(&gActor_map, pActor);
    if (s < 0 || gSources[s].model != pModel || gSources[s].block < 0 || !BlockActive(&gBlocks[gSources[s].block])
            || !SourceStillValid(&gSources[s], gBlocks[gSources[s].block].smash)) {
        return -1;
    }
    return s;
}

br_model* C2TrackBatchRenderModel(const br_actor* pActor, br_model* pModel) {
    int s;

    if (gCount_blocks == 0 || pActor == NULL) {
        return pModel;
    }
    s = ClaimedSource(pActor, pModel);
    return s < 0 ? pModel : gSources[s].substitute; /* NULL: everything of it is in the block */
}

/* Whether pActor draws nothing itself: its model is merged into an enabled block whole. */
int C2TrackBatchSkipsActor(const br_actor* pActor, const br_model* pModel) {
    int s;
    if (gCount_blocks == 0) {
        return 0;
    }
    s = ClaimedSource(pActor, pModel);
    return s >= 0 && gSources[s].substitute == NULL;
}

void C2TrackBatchModelChanged(br_model* pModel) {
    int i;

    pModel->flags &= ~C2_MODF_BATCHED;
    for (i = 0; i < gCount_sources; i++) {
        if (gSources[i].model == pModel) {
            if (gSources[i].block >= 0 && !gSources[i].model_changed) {
                dr_dprintf("TRACK BATCH: model %s updated", pModel->identifier != NULL ? pModel->identifier : "?");
            }
            gSources[i].model_changed = 1;
        }
    }
}

static int gTest_hide_half; /* C2_TEST_HIDEHALF: every other source counts as changed */

/* Called for every merged source every frame, so it reads as little as it can: the model's own
 * changes are flagged by C2TrackBatchModelChanged; the transform is compared as words (newlib's
 * memcmp goes byte by byte). */
static int SourceStillValid(const tBatch_source* pSource, int pSmash) {
    const br_actor* actor = pSource->actor;

    if (gTest_hide_half && ((pSource - gSources) & 1)) {
        return 0;
    }
    if (pSource->model_changed || actor->model != pSource->model || actor->render_style != pSource->style) {
        return 0;
    }
    if (pSmash) {
        const uint32_t* a = (const uint32_t*)&actor->t;
        const uint32_t* b = (const uint32_t*)&pSource->t;
        unsigned i;
        C2_HOOK_BUG_ON(sizeof(br_transform) % 4 != 0);
        if (actor->parent != pSource->parent) {
            return 0;
        }
        for (i = 0; i < sizeof(br_transform) / 4; i++) {
            if (a[i] != b[i]) {
                return 0;
            }
        }
    }
    return 1;
}

#ifdef __vita__
extern void StoredGLMarkIndexRefresh(void* pStored);
extern unsigned long long C2VitaGLTicksNS(void);

/* Hides the faces of the sources marked -2 in the prepared block model, in place: they become
 * degenerate (the GPU skips them) and only the index buffer is uploaded again. Building the merged
 * track again instead takes the Vita some 400 ms. 0: not possible here. */
static int HideFaces(tBatch_block* pBlock) {
    v11model* prepared = (v11model*)pBlock->model->prepared;
    unsigned char* hide;
    int i, g, f;

    if (prepared == NULL || pBlock->model->stored == NULL || pBlock->nfaces <= 0) {
        return 0;
    }
    hide = calloc(pBlock->nfaces, 1);
    if (hide == NULL) {
        return 0;
    }
    for (i = pBlock->first_source; i < pBlock->first_source + pBlock->count_sources; i++) {
        if (gSources[i].block == -2 && gSources[i].face_first + gSources[i].opaque_faces <= pBlock->nfaces) {
            memset(hide + gSources[i].face_first, 1, gSources[i].opaque_faces);
        }
    }
    for (g = 0; g < prepared->ngroups; g++) {
        v11group* group = &prepared->groups[g];
        for (f = 0; f < group->nfaces; f++) {
            if (group->face_user[f] < pBlock->nfaces && hide[group->face_user[f]]) {
                group->faces[f].vertices[1] = group->faces[f].vertices[0];
                group->faces[f].vertices[2] = group->faces[f].vertices[0];
            }
        }
    }
    free(hide);
    StoredGLMarkIndexRefresh(pBlock->model->stored);
    return 1;
}
#endif

/* Takes the changed sources out of block pBlock: they draw themselves again, and the block stops
 * drawing their faces. */
static void CheckBlock(int pBlock) {
    tBatch_block* block = &gBlocks[pBlock];
    br_model* model;
    int i, ejected = 0, left = 0;

    for (i = block->first_source; i < block->first_source + block->count_sources; i++) {
        if (gSources[i].block == pBlock) {
            if (!SourceStillValid(&gSources[i], block->smash)) {
                gSources[i].block = -2;
                ejected++;
            } else {
                left++;
            }
        }
    }
    if (ejected == 0) {
        return;
    }
    {
#ifdef __vita__
        unsigned long long t0 = C2VitaGLTicksNS();
        int hidden = left > 0 && HideFaces(block);
#else
        int hidden = 0;
#endif
        for (i = block->first_source; i < block->first_source + block->count_sources; i++) {
            if (gSources[i].block == -2) {
                gSources[i].block = -1;
            }
        }
        if (left == 0) {
            DisableBlock(pBlock, "all its objects changed");
            return;
        }
        if (!hidden) {
            /* merged models keep no faces once prepared: build it again from the others */
            model = BuildBlockModel(block->first_source, block->count_sources, pBlock);
            if (model == NULL) {
                for (i = block->first_source; i < block->first_source + block->count_sources; i++) {
                    if (gSources[i].block == pBlock) {
                        gSources[i].block = -1;
                    }
                }
                DisableBlock(pBlock, "no memory to build it again");
                return;
            }
            block->actor->model = model;
            BrModelRemove(block->model);
            BrModelFree(block->model);
            block->model = model;
            block->nfaces = gBuilt_faces;
        }
#ifdef __vita__
        LogBatch(hidden ? "TRACK BATCH: block %d hides %d objects (%d left, %d us)" : "TRACK BATCH: block %d rebuilt without %d objects (%d left, %d us)",
            pBlock, ejected, (unsigned)left, (int)((C2VitaGLTicksNS() - t0) / 1000));
#else
        LogBatch("TRACK BATCH: block %d rebuilt without %d objects (%d left)", pBlock, ejected, (unsigned)left, 0);
#endif
    }
}

/* Test check (C2_TEST_BATCHCHECK, once, ten seconds after the first batched frame): logs the merged
 * objects whose model-to-world matrix has changed since they were merged without their own
 * transform or parent changing (an ancestor moved: the block draws them in the wrong place). */
static void CheckWorldMatrices(void) {
    static int frames;
    int i, moved = 0;
    if (getenv("C2_TEST_BATCHCHECK") == NULL || ++frames != 300) {
        return;
    }
    for (i = 0; i < gCount_sources; i++) {
        br_matrix34 m;
        float diff = 0.f;
        int r, c;
        if (gSources[i].block < 0) {
            continue;
        }
        BrActorToActorMatrix34(&m, gSources[i].actor, gUniverse_actor);
        for (r = 0; r < 4; r++) {
            for (c = 0; c < 3; c++) {
                diff += fabsf(m.m[r][c] - gSources[i].world.m[r][c]);
            }
        }
        if (diff > 1e-3f) {
            moved++;
            dr_dprintf("TRACK BATCH CHECK: %s (block %d) moved by %.3f: y %.2f -> %.2f", gSources[i].actor->identifier != NULL ? gSources[i].actor->identifier : "?",
                gSources[i].block, diff, gSources[i].world.m[3][1], m.m[3][1]);
        }
    }
    LogBatch("TRACK BATCH CHECK: %d of %d merged objects moved since merging", moved, gCount_sources, 0u, 0);
}

void C2TrackBatchRender(int pMin_x, int pMax_x, int pMin_z, int pMax_z) {
    int i;

    CheckWorldMatrices();
    if (getenv("C2_TEST_HIDEHALF") != NULL && !gTest_hide_half) {
        /* test: C2_TEST_HIDEHALF=<frames> takes every other source out of its block after that many
         * batched frames; a frozen scene must look the same before and after */
        static int frames;
        if (++frames == atoi(getenv("C2_TEST_HIDEHALF"))) {
            gTest_hide_half = 1;
            LogBatch("TRACK BATCH: test: every other source taken out", 0, 0, 0u, 0);
        }
    }

#ifdef __vita__
    {
        extern int C2VitaABFeature(const char* pFeature, int pDefault);
        gSmash_on = C2VitaABFeature("smashbatch", 1);
    }
#endif
    /* Only the blocks drawn now are checked (just before drawing): reading every merged actor each
     * frame cost the Vita 1 ms. An unseen block's changed sources are found as their actors are
     * walked (ClaimedSource). */
    for (i = 0; i < gCount_blocks; i++) {
        tBatch_block* block = &gBlocks[i];
        if (!BlockActive(block) || block->x1 < pMin_x || block->x0 > pMax_x || block->z1 < pMin_z || block->z0 > pMax_z) {
            continue;
        }
#ifdef __vita__
        C2VitaPhasePush(C2VP_W_BATCHCHECK);
#endif
        CheckBlock(i);
#ifdef __vita__
        C2VitaPhasePop();
        C2VitaPhasePush(C2VP_W_BATCHADD);
#endif
        if (block->enabled) {
            BrZbsSceneRenderAdd(block->actor);
        }
#ifdef __vita__
        C2VitaPhasePop();
#endif
    }
}
#endif
