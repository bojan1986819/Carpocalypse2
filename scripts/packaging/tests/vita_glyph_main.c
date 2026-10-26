#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brender/brender.h"
#include "carpocalypse2_types.h"
#include "sdl3/vita_glyph.h"
#include "sdl3/vita_text.h"

#define CARPOCALYPSE2_ASIZE(a) (sizeof(a) / sizeof((a)[0]))
static tPolyFont gPoly_fonts[27];
static br_material* gPoly_font_materials[80];
static br_pixelmap* gTexture_maps[1024];
static tU16 polyFontMaterialCounter;
static int currentPolyFontMaterialIdx, gCount_polyfont_glyph_actors;
/* Expanded legacy storage supplies an oracle past the original 256 limit. */
static br_actor* gPolyfont_glyph_actors[8192];
static br_actor *gString_root_actor, *gHUD_root, *gHUD_camera;
static br_pixelmap screen, depth;
static br_pixelmap *gBack_screen = &screen, *gDepth_buffer = &depth;
static unsigned availability_checks, allocations, begins, ends;
static int legacy;

void BrFailure(const char* fmt, ...) { va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap); abort(); }
void BrMatrix23Identity(br_matrix23* m) { memset(m, 0, sizeof(*m)); m->m[0][0] = m->m[1][1] = 1; }
void BrMatrix23Scale(br_matrix23* m, float x, float y) { BrMatrix23Identity(m); m->m[0][0] = x; m->m[1][1] = y; }
br_actor* BrActorAllocate(br_uint_8 type, void* data) { br_actor* a = calloc(1, sizeof(*a)); assert(a); ++allocations; a->type = type; a->type_data = data; a->t.type = BR_TRANSFORM_IDENTITY; return a; }
br_material* BrMaterialAllocate(const char* id) { br_material* m = calloc(1, sizeof(*m)); assert(m); ++allocations; m->identifier = (char*)id; BrMatrix23Identity(&m->map_transform); return m; }
void BrMaterialUpdate(br_material* m, br_uint_16 flags) { (void)flags; m->stored = (void*)m; }
br_actor* BrActorAdd(br_actor* parent, br_actor* a) { assert(!a->parent); a->next = parent->children; parent->children = a; a->parent = parent; return a; }
br_actor* BrActorRemove(br_actor* a) { br_actor** p = &a->parent->children; while (*p != a) p = &(*p)->next; *p = a->next; a->parent = NULL; a->next = NULL; return a; }
static void CheckAvailabilityOfThisFont(int f) { ++availability_checks; gPoly_fonts[f].available = 1; }
static void CleanPolyFontDanglers(void) {
    for (int i = 0; i < gCount_polyfont_glyph_actors; ++i)
        if (gPolyfont_glyph_actors[i]->parent) BrActorRemove(gPolyfont_glyph_actors[i]);
    gCount_polyfont_glyph_actors = 0;
    C2VitaGlyphClean();
}

/* Flatten into triangles, independent of vertex indexing/model grouping. */
typedef struct sample { fmt_vertex v; br_colour colour; br_material material; } sample;
static sample actual[60000], expected[60000];
static unsigned count, expected_count, rendered_models;
typedef struct pending { const v11model* model; unsigned char* bytes; size_t size; const br_material* material; br_material copy; } pending;
static pending held[4096];
static unsigned held_count;

static br_material Normalized(const br_material* src) {
    br_material m = *src;
    m._reserved = 0; m.identifier = NULL; m.user = NULL; m.stored = NULL;
    BrMatrix23Identity(&m.map_transform);
    return m;
}
static void Capture(br_actor* a) {
    if (a->type == BR_ACTOR_MODEL) {
        v11model* m = a->model->prepared;
        ++rendered_models;
        if (!legacy) {
            /* Same contiguous-range contract as the Vita geometry cache. */
            const v11group* g = m->groups;
            assert((char*)g >= (char*)m && (char*)(g + 1) <= (char*)m + m->size);
            assert((char*)g->vertices >= (char*)m && (char*)(g->vertices + g->nvertices) <= (char*)m + m->size);
            assert((char*)g->faces >= (char*)m && (char*)(g->faces + g->nfaces) <= (char*)m + m->size);
            assert(held_count < CARPOCALYPSE2_ASIZE(held));
            pending* p = &held[held_count++];
            p->model = m; p->size = m->size; p->bytes = malloc(m->size);
            memcpy(p->bytes, m, m->size); p->material = a->material; p->copy = *a->material;
        }
        for (unsigned gi = 0; gi < m->ngroups; ++gi) {
            const v11group* g = &m->groups[gi];
            for (unsigned f = 0; f < g->nfaces; ++f)
                for (unsigned corner = 0; corner < 3; ++corner) {
                    unsigned v = g->faces[f].vertices[corner];
                    assert(v < g->nvertices && count < CARPOCALYPSE2_ASIZE(actual));
                    sample* s = &actual[count++];
                    s->v = g->vertices[v]; s->colour = g->vertex_colours[v]; s->material = Normalized(a->material);
                    if (a->t.type == BR_TRANSFORM_TRANSLATION)
                        for (unsigned axis = 0; axis < 3; ++axis) s->v.p.v[axis] += a->t.t.translate.t.v[axis];
                    float u = s->v.map.v[0], vv = s->v.map.v[1];
                    s->v.map.v[0] = u * a->material->map_transform.m[0][0] + vv * a->material->map_transform.m[1][0] + a->material->map_transform.m[2][0];
                    s->v.map.v[1] = u * a->material->map_transform.m[0][1] + vv * a->material->map_transform.m[1][1] + a->material->map_transform.m[2][1];
                }
        }
    }
    for (br_actor* c = a->children; c; c = c->next) Capture(c);
}
void BrZbSceneRenderBegin(br_actor* a, br_actor* b, br_pixelmap* c, br_pixelmap* d) { (void)a; (void)b; (void)c; (void)d; ++begins; }
void BrZbSceneRenderAdd(br_actor* a) { Capture(a); }
void BrZbSceneRenderEnd(void) {
    ++ends;
    for (unsigned i = 0; i < held_count; ++i) {
        pending* p = &held[i];
        assert(memcmp(p->bytes, p->model, p->size) == 0);
        assert(memcmp(&p->copy, p->material, sizeof(p->copy)) == 0);
        free(p->bytes);
    }
    held_count = 0;
}
void BrZbSceneRender(br_actor* a, br_actor* b, br_pixelmap* c, br_pixelmap* d) {
    BrZbSceneRenderBegin(a,b,c,d);
    for (br_actor* child = a->children; child; child = child->next) Capture(child);
    BrZbSceneRenderEnd();
}

#include "sdl3/vita_glyph.c"
#include "sdl3/vita_text.c"
#include "font_functions.h"

static void Init(void) {
    static br_pixelmap pages[8];
    static br_model models[3];
    static v11model prepared[3];
    static v11group groups[3];
    static fmt_vertex vertices[3][4];
    static v11face faces[3][2];
    static br_colour colours[3][4], face_colours[3][2];
    gString_root_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gHUD_root = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gHUD_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    for (unsigned i = 0; i < CARPOCALYPSE2_ASIZE(gPolyfont_glyph_actors); ++i) {
        gPolyfont_glyph_actors[i] = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gPolyfont_glyph_actors[i]->t.type = BR_TRANSFORM_TRANSLATION;
    }
    for (unsigned i = 0; i < 8; ++i) gTexture_maps[i] = &pages[i];
    for (unsigned i = 0; i < 80; ++i) {
        br_material* m = gPoly_font_materials[i] = BrMaterialAllocate("cached glyph");
        m->flags = BR_MATF_FORCE_FRONT | BR_MATF_ALWAYS_VISIBLE | BR_MATF_SMOOTH | BR_MATF_PRELIT;
        m->colour = 0xff639e12; m->opacity = 191; m->index_base = 7; m->index_range = 13;
        m->ka = .2f; m->kd = .7f; m->ks = .1f; m->power = 3; m->mode = 0x15;
    }
    for (unsigned f = 0; f < 3; ++f) {
        models[f].prepared = &prepared[f]; prepared[f].ngroups = 1; prepared[f].groups = &groups[f];
        groups[f].nvertices = 4; groups[f].nfaces = 2; groups[f].vertices = vertices[f];
        groups[f].faces = faces[f]; groups[f].vertex_colours = colours[f]; groups[f].face_colours.colours = face_colours[f];
        for (unsigned v = 0; v < 4; ++v) {
            BrVector3Set(&vertices[f][v].p, (v & 1) * 17.f, -(v >> 1) * 17.f, -1.2f);
            vertices[f][v].map.v[0] = v & 1; vertices[f][v].map.v[1] = v >> 1;
            vertices[f][v].n.v[2] = 1;
            colours[f][v] = 0xff010203u + 0x10317 * v + f * 0x321701;
        }
        faces[f][0].vertices[0] = 0; faces[f][0].vertices[1] = 1; faces[f][0].vertices[2] = 2;
        faces[f][1].vertices[0] = 1; faces[f][1].vertices[1] = 3; faces[f][1].vertices[2] = 2;
        for (unsigned i = 0; i < 2; ++i) { faces[f][i].eqn.v[2] = 1; faces[f][i].eqn.v[3] = -1.2f; }
        tPolyFont* font = &gPoly_fonts[f];
        font->fontSize = 17; font->interCharacterSpacing = -2; font->fontCharacterHeight = 19;
        font->widthOfBlank = 5; font->available = 1;
        for (unsigned c = 33; c < 256; ++c) {
            tPolyFontGlyph* glyph = &font->glyphs[c];
            glyph->used = 1; glyph->model = &models[f]; glyph->glyph_width = 7 + c % 8;
            glyph->index = (c / 8) % 8;
            glyph->texCoord.v[0] = (c % 4) * .25f; glyph->texCoord.v[1] = ((c / 4) % 4) * .25f;
        }
    }
    screen.origin_x = 320; screen.origin_y = 240; screen.width = 640; screen.height = 480;
}

static void Compare(void) {
    assert(count == expected_count);
    for (unsigned i = 0; i < count; ++i) {
        /* i386's x87 can retain an intermediate at extended precision while
         * the baked production buffer necessarily rounds to float. */
        for (unsigned a = 0; a < 3; ++a) {
            assert(fabsf(actual[i].v.p.v[a] - expected[i].v.p.v[a]) < 0.000001f);
            assert(actual[i].v.n.v[a] == expected[i].v.n.v[a]);
        }
        for (unsigned a = 0; a < 2; ++a)
            assert(fabsf(actual[i].v.map.v[a] - expected[i].v.map.v[a]) < 0.000001f);
        assert(actual[i].colour == expected[i].colour);
        assert(memcmp(&actual[i].material, &expected[i].material, sizeof(br_material)) == 0);
    }
}
static void Pass(int old, const char* text, int just, int immediate) {
    legacy = old; count = rendered_models = 0;
    void (*textfn)(const char*,int,int,int,tJustification,int) = old ? LegacyPolyFontText : PolyFontText;
    if (immediate && !old) C2VitaTextBatchBegin();
    textfn(text, 310, 24, 0, just, immediate);
    if (immediate) {
        textfn("0123 green\rHUD", 121, 100, 1, eJust_right, 1);
        textfn("abc DEF", 250, 160, 2, eJust_centre, 1);
        if (!old) C2VitaTextBatchEnd();
    } else {
        textfn("0123 green\rHUD", 121, 100, 1, eJust_right, 0);
        BrActorAdd(gHUD_root, gString_root_actor);
        BrZbSceneRender(gHUD_root, gHUD_camera, &screen, &depth);
        BrActorRemove(gString_root_actor); CleanPolyFontDanglers();
    }
    assert(!gString_root_actor->children && !gString_root_actor->parent);
    assert(!C2VitaTextSceneIsOpen());
    assert(screen.origin_x == 320 && screen.origin_y == 240 && screen.width == 640 && screen.height == 480);
}

int main(void) {
    Init();
    char long_text[701]; memset(long_text, 'A', 700); long_text[700] = 0;
    const char* texts[] = { "", "   ", "A", "ABCabc DEF\rGHI!", "ABABABAB", "AHIPAHIP", long_text };
    for (int pass = 0; pass < 2; ++pass)
        for (int just = 0; just < 3; ++just)
            for (unsigned t = 0; t < CARPOCALYPSE2_ASIZE(texts); ++t) {
                Pass(1, texts[t], just, pass);
                memcpy(expected, actual, count * sizeof(sample)); expected_count = count;
                unsigned old_draws = rendered_models;
                Pass(0, texts[t], just, pass); Compare();
                if (t == 6) assert(rendered_models < old_draws / 5);
            }
    /* Evict the real 80-entry material cache within one string. The oracle
     * renders each original glyph before its cache entry can be overwritten. */
    char eviction[201];
    int x = 20;
    legacy = 1; count = 0;
    for (unsigned i = 0; i < 200; ++i) {
        unsigned char c = 33 + i;
        eviction[i] = c;
        char single[2] = {(char)c, 0};
        LegacyPolyFontText(single, x, 30, 0, eJust_left, 1);
        if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
        x += gPoly_fonts[0].glyphs[c].glyph_width + gPoly_fonts[0].interCharacterSpacing;
    }
    eviction[200] = 0; expected_count = count;
    for (unsigned i = 0; i < 200; ++i)
        memcpy(expected + i * 6, actual + (199 - i) * 6, 6 * sizeof(sample));
    for (unsigned pass = 0; pass < 2; ++pass) {
        legacy = 0; count = 0;
        if (pass) C2VitaTextBatchBegin();
        PolyFontText(eviction, 20, 30, 0, eJust_left, pass);
        if (pass) C2VitaTextBatchEnd();
        else {
            BrActorAdd(gHUD_root, gString_root_actor);
            BrZbSceneRender(gHUD_root, gHUD_camera, &screen, &depth);
            BrActorRemove(gString_root_actor); CleanPolyFontDanglers();
        }
        Compare();
    }
    /* Unavailable font follows the production availability path. */
    gPoly_fonts[0].available = 0; unsigned checks = availability_checks;
    PolyFontText("ABC", 0, 0, 0, eJust_left, 1); assert(availability_checks == checks + 1);
    /* Slots, geometry, material state and copied token lists survive sceneEnd. */
    br_token_value tokens[] = {{.t=BRT_BLEND_B,.v.b=1},{.t=BR_NULL_TOKEN}};
    br_material material = *gPoly_font_materials[0]; material.extra_prim = tokens;
    C2VitaTextBatchBegin();
    C2VitaGlyphStringBegin();
    C2VitaGlyphSubmit(gString_root_actor, gPoly_fonts[0].glyphs['A'].model, &material, 20, 30);
    assert(gString_root_actor->children->material->extra_prim != tokens);
    tokens[0].v.b = 0; assert(gString_root_actor->children->material->extra_prim[0].v.b == 1);
    C2VitaGlyphSubmit(gString_root_actor, gPoly_fonts[0].glyphs['A'].model, &material, 40, 30);
    C2VitaGlyphStringEnd();
    /* The same source token pointer with changed contents must start a run. */
    assert(gString_root_actor->children->next != NULL);
    assert(gString_root_actor->children->material->extra_prim[0].v.b == 0);
    assert(gString_root_actor->children->next->material->extra_prim[0].v.b == 1);
    BrActorAdd(gHUD_root, gString_root_actor);
    C2VitaTextSceneRender(gHUD_root,gHUD_camera,&screen,&depth);
    BrActorRemove(gString_root_actor); CleanPolyFontDanglers();
    PolyFontText("OTHER PAGE", 3, 4, 0, eJust_left, 1);
    C2VitaTextBatchFlush(); C2VitaTextBatchEnd();
    /* Repeated identical workloads allocate no more pool actors/materials. */
    Pass(0, long_text, eJust_left, 1); unsigned warm = allocations;
    for (unsigned i = 0; i < 20; ++i) Pass(0, long_text, eJust_left, 1);
    assert(allocations == warm && begins == ends);
    puts("PASS production legacy/batch triangles: both passes, three colours/justifications, blanks, CR, page changes, 700 glyphs, 80-entry material eviction, availability, scene lifetime, token snapshots, stable pooled allocations (ASan/UBSan)");
}
