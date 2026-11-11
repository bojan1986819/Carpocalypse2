#include "opponent.h"

#include "car.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "drone.h"
#include "errors.h"
#include "finteray.h"
#include "graphics.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrme.h"
#include "globvrpb.h"
#include "loading.h"
#include "mainloop.h"
#include "oppoproc.h"
#include "physics.h"
#include "piping.h"
#include "platform.h"
#include "skidmark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"

#include <brender/brender.h>

#include "carpocalypse2_macros.h"

#include "c2_string.h"

#include <stdarg.h>
#include "c2_math.h"

#define CAR_SPEC_IS_ROZZER(CAR_SPEC) (VEHICLE_TYPE_FROM_ID((CAR_SPEC)->car_ID) == eVehicle_rozzer)
#define CAR_SPEC_IS_OPPONENT(CAR_SPEC) (VEHICLE_TYPE_FROM_ID((CAR_SPEC)->car_ID) == eVehicle_opponent)

#define GET_CAR_SPEED_FACTOR(CAR) (CAR_SPEC_IS_ROZZER(CAR) ? gCop_speed_factor : gOpponent_speed_factor)

extern const char gEmpty_string_00676920[];

extern const br_scalar gFLOAT_0058a61c;

// GLOBAL: CARMA2_HW 0x0058a620
const br_scalar gFLOAT_0058a620 = 6.9f;

// GLOBAL: CARMA2_HW 0x0058a69c
const br_scalar gFLOAT_0058a69c = 0.0625f;

// GLOBAL: CARMA2_HW 0x0058a6a0
const br_scalar gFLOAT_0058a6a0 = -0.001f;

// GLOBAL: CARMA2_HW 0x0058a6a4
const br_scalar gFLOAT_0058a6a4 = 0.14492753148078918f;

// GLOBAL: CARMA2_HW 0x0065a750
const char gFormat_opponent_headup_0065a750[] = "%s: obj: %d nsect: %d sect: %d(%d%s)%s%s";

// GLOBAL: CARMA2_HW 0x0065a77c
const char gSection_text_0065a77c[] = "T";

// GLOBAL: CARMA2_HW 0x0065a780
const char gSection_text_0065a780[] = "1";

// GLOBAL: CARMA2_HW 0x0065a784
const char gSection_text_0065a784[] = "0";

// GLOBAL: CARMA2_HW 0x0065a788
const char gSection_text_0065a788[] = "*";
#include "opponent.h"
#include "globvars.h"

#include <c2_hooks.h>
// GLOBAL: CARMA2_HW 0x0069173c
int gActive_car_list_rebuild_required;

// GLOBAL: CARMA2_HW 0x0065a3c4
int gBIG_APC_index = -1;

// GLOBAL: CARMA2_HW 0x00691744
int gNumber_of_cops_before_faffage;

// GLOBAL: CARMA2_HW 0x00691754
tU32 gNext_grudge_reduction;

// GLOBAL: CARMA2_HW 0x00691738
int gFirst_frame;

// GLOBAL: CARMA2_HW 0x00761a40
#define gOppo_path_nodes (gProgram_state.AI_vehicles.path_nodes)

// GLOBAL: CARMA2_HW 0x00761a44
#define gOppo_junction_table ((tU32)(uintptr_t)gProgram_state.AI_vehicles.path_sections)

// GLOBAL: CARMA2_HW 0x00691724
int gGrudge_reduction_per_period;

// GLOBAL: CARMA2_HW 0x00691750
int gAcknowledged_start;

// GLOBAL: CARMA2_HW 0x00691720
int gStart_jumped;

// GLOBAL: CARMA2_HW 0x00691760
br_scalar gMinimum_yness_before_knackerisation;

// GLOBAL: CARMA2_HW 0x00691728
tU32 gAcme_frame_count;

// GLOBAL: CARMA2_HW 0x00691768
br_scalar gHead_on_cos_value;

// GLOBAL: CARMA2_HW 0x00691714
int gBig_bang;

// GLOBAL: CARMA2_HW 0x00691764
br_scalar gIn_view_distance;

// GLOBAL: CARMA2_HW 0x0065a3c8
int gChallenger_index__opponent = -1;

// GLOBAL: CARMA2_HW 0x0069175c
tU8* gBit_per_node;

// GLOBAL: CARMA2_HW 0x0069171c
br_scalar gSFS_shortest_dist;

// GLOBAL: CARMA2_HW 0x0069172c
int gSFS_routes_found;

// GLOBAL: CARMA2_HW 0x0069174c
int gSFS_routes_found_copy;

// GLOBAL: CARMA2_HW 0x0069176c
int gSFS_cycles_this_time;

// GLOBAL: CARMA2_HW 0x00691770
int gSFS_search_count;

// GLOBAL: CARMA2_HW 0x00691774
int gSFS_total_cycles;

// GLOBAL: CARMA2_HW 0x00691778
int gSFS_max_cycles;

// GLOBAL: CARMA2_HW 0x0058a47c
float DAT_0058a47c = 100.0f;

// GLOBAL: CARMA2_HW 0x0058a55c
float DAT_0058a55c = -0.0625f;

// GLOBAL: CARMA2_HW 0x0058a4e0
double DAT_0058a4e0 = 0.14492753623188406;

// GLOBAL: CARMA2_HW 0x0058a4a0
const double DAT_0058a4a0 = 1.0;

// GLOBAL: CARMA2_HW 0x0058a4a8
const double DAT_0058a4a8 = 10430.378350470453;

// GLOBAL: CARMA2_HW 0x0058a4b0
const double DAT_0058a4b0 = 6.9;

// GLOBAL: CARMA2_HW 0x0058a4b8
const float DAT_0058a4b8 = -1.0f;

// GLOBAL: CARMA2_HW 0x0058a4bc
const float DAT_0058a4bc = -100.0f;

// GLOBAL: CARMA2_HW 0x0058a4c0
const float DAT_0058a4c0 = -5.0f;

// GLOBAL: CARMA2_HW 0x0058a4c4
const float DAT_0058a4c4 = -0.05f;

// GLOBAL: CARMA2_HW 0x0058a4c8
const float DAT_0058a4c8 = -0.05f;

// GLOBAL: CARMA2_HW 0x0058a4d0
const double DAT_0058a4d0 = -10430.378350470453;

// GLOBAL: CARMA2_HW 0x0058a4d8
const float DAT_0058a4d8 = 0.14492753148078918f;

// GLOBAL: CARMA2_HW 0x0058a4e8
const float DAT_0058a4e8 = 0.0000002384185791015625f;

// GLOBAL: CARMA2_HW 0x0058a4ec
const float DAT_0058a4ec = 10.0f;

// GLOBAL: CARMA2_HW 0x0058a4f0
const float DAT_0058a4f0 = 0.1f;

// GLOBAL: CARMA2_HW 0x0069177c
int gRematerialisation_count;

// GLOBAL: CARMA2_HW 0x00691780
int gRematerialisation_extra_attempts;

// GLOBAL: CARMA2_HW 0x00691784
int gRematerialisation_max_extra_attempts;

// GLOBAL: CARMA2_HW 0x00676858
br_material* gMat_dk_yel = NULL;

// GLOBAL: CARMA2_HW 0x00676850
int gPalette_materials_initialized;

// GLOBAL: CARMA2_HW 0x0067685c
br_material* gMat_md_yel = NULL;

// GLOBAL: CARMA2_HW 0x00676860
br_material* gMat_lt_yel = NULL;

// GLOBAL: CARMA2_HW 0x00676864
br_material* gMat_dk_red = NULL;

// GLOBAL: CARMA2_HW 0x00676868
br_material* gMat_md_red = NULL;

// GLOBAL: CARMA2_HW 0x0067686c
br_material* gMat_lt_red = NULL;

// GLOBAL: CARMA2_HW 0x00676870
br_material* gMat_dk_grn = NULL;

// GLOBAL: CARMA2_HW 0x00676874
br_material* gMat_md_grn = NULL;

// GLOBAL: CARMA2_HW 0x00676878
br_material* gMat_lt_grn = NULL;

// GLOBAL: CARMA2_HW 0x0067687c
br_material* gMat_dk_blu = NULL;

// GLOBAL: CARMA2_HW 0x00676880
br_material* gMat_lt_blu = NULL;

// GLOBAL: CARMA2_HW 0x00676884
br_material* gMat_dk_turq = NULL;

// GLOBAL: CARMA2_HW 0x00676888
br_material* gMat_lt_turq = NULL;

// GLOBAL: CARMA2_HW 0x0067688c
br_material* gMat_dk_gry = NULL;

// GLOBAL: CARMA2_HW 0x00676890
br_material* gMat_md_gry = NULL;

// GLOBAL: CARMA2_HW 0x00676894
br_material* gMat_lt_gry = NULL;

// GLOBAL: CARMA2_HW 0x00691718
int gNum_of_opponents_pursuing;

// GLOBAL: CARMA2_HW 0x00691740
int gNum_of_opponents_getting_near;

// GLOBAL: CARMA2_HW 0x00691758
int gNum_of_opponents_completing_race;

// GLOBAL: CARMA2_HW 0x00691734
float gDefinite_no_cop_pursuit_speed;

// GLOBAL: CARMA2_HW 0x00691730
float gCop_pursuit_speed_value_00691730;

// GLOBAL: CARMA2_HW 0x00691748
float gCop_pursuit_speed_percentage_multiplier;

// GLOBAL: CARMA2_HW 0x0058a478
float gOppo_render_zero = 0.0f;

// GLOBAL: CARMA2_HW 0x0058a480
float gOppo_pursuit_one = 1.0f;

// GLOBAL: CARMA2_HW 0x0058a488
float gOppo_pursuit_reverse_brake = 15.0f;

// GLOBAL: CARMA2_HW 0x0058a48c
float gOppo_pursuit_velocity_scale = 0.5f;

// GLOBAL: CARMA2_HW 0x0058a494
float gOppo_pursuit_minimum_velocity_ratio = 2.0f;

// GLOBAL: CARMA2_HW 0x0058a504
float gOppo_pursuit_height_offset = -0.15f;

// GLOBAL: CARMA2_HW 0x0058a50c
float gOppo_pursuit_precinct_distance = 75.0f;

// GLOBAL: CARMA2_HW 0x0058a510
float gOppo_pursuit_near_distance = 3.0f;

// GLOBAL: CARMA2_HW 0x0058a514
float gOppo_pursuit_stationary_speed = 0.043478261679410934f;

// GLOBAL: CARMA2_HW 0x0058a518
float gOppo_pursuit_cheat_distance = 30.0f;

// GLOBAL: CARMA2_HW 0x0058a51c
float gOppo_pursuit_sight_distance = 35.0f;

// GLOBAL: CARMA2_HW 0x0058a520
float gOppo_pursuit_trail_distance = 20.0f;

// GLOBAL: CARMA2_HW 0x0058a524
float gOppo_pursuit_reverse_acceleration = -8.0f;

// GLOBAL: CARMA2_HW 0x0065a3cc
float gOpponent_nastyness_frigger = 1.0f;

// GLOBAL: CARMA2_HW 0x0058a5a8
const double gCop_nastiness_skill_adjustment = -0.5;

// GLOBAL: CARMA2_HW 0x0058a5d8
const double gCop_nastiness_bias = -2.25;

// GLOBAL: CARMA2_HW 0x0058a5b0
const double gCop_nastiness_divisor = 0.33333333333333331;

// GLOBAL: CARMA2_HW 0x0074a684
int gMinTimeOpponentRepair;

// GLOBAL: CARMA2_HW 0x0074a688
int gMaxTimeOpponentRepair;

// FUNCTION: CARMA2_HW 0x004a7a20
void C2_HOOK_FASTCALL SetOppoRender(tOpponent_spec* pOpponent_spec, int pRender) {
    if (gAction_replay_mode && ARGetReplayRate() < gOppo_render_zero) {
        pRender = (pRender == 0);
    }
    pOpponent_spec->car_spec->car_master_actor->render_style = (pRender == 0) ? BR_RSTYLE_NONE : BR_RSTYLE_DEFAULT;
}

// FUNCTION: CARMA2_HW 0x004013d0
#pragma auto_inline(off)
void C2_HOOK_FASTCALL PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector) {
    br_transform trans;

    trans.type = BR_TRANSFORM_LOOK_UP;
    BrVector3Copy(&trans.t.look_up.look, pThe_vector);
    BrVector3Set(&trans.t.look_up.up, 0.f, 1.f, 0.f);
    BrVector3Copy(&trans.t.look_up.t, &pThe_actor->t.t.translate.t);
    BrTransformToTransform(&pThe_actor->t, &trans);
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x00401430
void C2_HOOK_FASTCALL PointActorAlongVectorWithUp(br_actor* pThe_actor, br_vector3* pLook, br_vector3* pUp) {
    br_transform trans;

    trans.type = BR_TRANSFORM_LOOK_UP;
    BrVector3Copy(&trans.t.look_up.look, pLook);
    BrVector3Copy(&trans.t.look_up.up, pUp);
    BrVector3Copy(&trans.t.look_up.t, &pThe_actor->t.t.translate.t);
    BrTransformToTransform(&pThe_actor->t, &trans);
}

// FUNCTION: CARMA2_HW 0x004014a0
void C2_HOOK_FASTCALL InitialiseOpponentPaletteMaterials(void) {
    br_material* material;
    br_colour* palette_colours;
    br_colour palette_colour;

    if (!gPalette_materials_initialized) {
        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x32];
        material->index_base = 0x32;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_dk_yel = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x33];
        material->index_base = 0x33;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_md_yel = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x34];
        material->index_base = 0x34;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_lt_yel = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[2];
        material->index_base = 2;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_dk_red = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[3];
        material->index_base = 3;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_md_red = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[4];
        material->index_base = 4;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_lt_red = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x42];
        material->index_base = 0x42;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_dk_grn = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x43];
        material->index_base = 0x43;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_md_grn = material;

        material = BrMaterialAllocate(NULL);
        palette_colours = gRender_palette->pixels;
        palette_colour = palette_colours[0x44];
        material->index_base = 0x44;
        material->colour = palette_colour;
        material->index_range = 1;
        material->flags = 0x1000;
        material->index_shade = NULL;
        material->colour_map = NULL;
        material->identifier = NULL;
        BrMaterialAdd(material);
        gMat_lt_grn = material;

        gMat_dk_blu = CreatePaletteMaterial(0xa2);
        gMat_lt_blu = CreatePaletteMaterial(0xa4);
        gMat_dk_turq = CreatePaletteMaterial(0x82);
        gMat_lt_turq = CreatePaletteMaterial(0x84);
        gMat_dk_gry = CreatePaletteMaterial(0xfd);
        gMat_md_gry = CreatePaletteMaterial(0xfe);
        material = CreatePaletteMaterial(0xff);
        gPalette_materials_initialized = 1;
        gMat_lt_gry = material;
    }
    FUN_004af490();
    ShowDronePaths();
}

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00401760
br_material* C2_HOOK_FASTCALL CreatePaletteMaterial(int pPalette_index) {
    br_material* material;
    br_colour* palette_colours;
    br_colour palette_colour;

    material = BrMaterialAllocate(NULL);
    palette_colours = gRender_palette->pixels;
    palette_colour = palette_colours[pPalette_index];
    material->index_base = (tU8)pPalette_index;
    material->colour = palette_colour;
    material->index_range = 1;
    material->flags = 0x1000;
    material->index_shade = NULL;
    material->colour_map = NULL;
    material->identifier = NULL;
    BrMaterialAdd(material);
    return material;
}
#pragma auto_inline(on)

void C2_HOOK_FASTCALL ReallocExtraPathNodes(int pCount) {
    tPath_node* nodes;

    C2_HOOK_BUG_ON(sizeof(tPath_node) != 0x20);

    if (pCount != 0) {

        nodes = BrMemAllocate((gProgram_state.AI_vehicles.number_of_path_nodes + pCount) * sizeof(tPath_node), kMem_oppo_new_nodes);
        memcpy(nodes, gProgram_state.AI_vehicles.path_nodes, gProgram_state.AI_vehicles.number_of_path_nodes * sizeof(tPath_node));

        if (gProgram_state.AI_vehicles.path_nodes != NULL) {
            BrMemFree(gProgram_state.AI_vehicles.path_nodes);
        }
        gProgram_state.AI_vehicles.path_nodes = nodes;
        gProgram_state.AI_vehicles.number_of_path_nodes += pCount;
    }
    DoNotDprintf_opponent("ReallocExtraPathNodes(): Allocated %d bytes for %d path nodes", (pCount + gProgram_state.AI_vehicles.number_of_path_nodes) * sizeof(tPath_node), pCount);
}

void C2_HOOK_FASTCALL ReallocExtraPathSections(int pCount) {
    tPath_section* sections;

    C2_HOOK_BUG_ON(sizeof(tPath_section) != 0x14);

    if (pCount != 0) {

        sections = BrMemAllocate((gProgram_state.AI_vehicles.number_of_path_sections + pCount) * sizeof(tPath_section), kMem_oppo_new_sections);
        memcpy(sections, gProgram_state.AI_vehicles.path_sections, gProgram_state.AI_vehicles.number_of_path_sections * sizeof(tPath_section));

        if (gProgram_state.AI_vehicles.path_sections != NULL) {
            BrMemFree(gProgram_state.AI_vehicles.path_sections);
        }
        gProgram_state.AI_vehicles.path_sections = sections;
        gProgram_state.AI_vehicles.number_of_path_sections += pCount;
    }
    DoNotDprintf_opponent("ReallocExtraPathSections(): Allocated %d bytes for %d path sections", (pCount + gProgram_state.AI_vehicles.number_of_path_sections) * sizeof(tPath_section), pCount);
}

// FUNCTION: CARMA2_HW 0x004a9610
void C2_HOOK_FASTCALL LoadInOppoPaths(FILE* pF) {
    char s[256];
    int count_nodes;
    int count_sections;
    int i;
    char* line;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tProgram_state, current_car, 0xac);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tProgram_state, player_name, 0x1a10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tProgram_state, special_volumes, 0x1c00);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tProgram_state, AI_vehicles, 0x1c20);
    C2_HOOK_BUG_ON(sizeof(tFollow_path_data) != 0x58);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tFollow_path_data, struggle_time, 0x0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tFollow_path_data, last_struggle_section, 0x30);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tFollow_path_data, corner_width, 0x50);
    C2_HOOK_BUG_ON(sizeof(tOpponent_spec) != 0x1a4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, index, 0x0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, current_objective, 0x4);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, player_to_oppo_v, 0x40);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, nnext_sections, 0x7c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, complete_race_data, 0xb8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, follow_path_data, 0xbc);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, pursue_car_data, 0x114);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, number_of_path_nodes, 0x8);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, number_of_path_sections, 0xc);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, cop_start_points, 0x10);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, cop_start_vectors, 0x88);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, opponents, 0x100);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, cops, 0x3238);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, path_nodes, 0x42a0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tIntelligent_vehicles, path_sections, 0x42a4);

    DoNotDprintf_opponent("Start of LoadInOppoPaths()...");
    gProgram_state.AI_vehicles.number_of_path_nodes = 0;
    gProgram_state.AI_vehicles.number_of_path_sections = 0;
    gProgram_state.AI_vehicles.path_nodes = NULL;
    gProgram_state.AI_vehicles.path_sections = NULL;
    gBit_per_node = NULL;
    gBIG_APC_index = -1;

    do {
        line = GetALineAndDontArgue(pF, s);
        if (line == NULL) {
            return;
        }
    } while (strcmp("START OF OPPONENT PATHS", s) != 0);
    if (line == NULL) {
        return;
    }

    /* Number of path nodes */
    count_nodes = GetAnInt(pF);
    ReallocExtraPathNodes(count_nodes);

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_nodes; i++) {
        br_vector3 pos;

        /* Node #i */
        GetThreeFloats(pF, &pos.v[0], &pos.v[1], &pos.v[2]);
        BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[i].pos, &pos);
        gProgram_state.AI_vehicles.path_nodes[i].number_of_sections = 0;
    }

    /* Number of path sections */
    count_sections = GetAnInt(pF);
    ReallocExtraPathSections(count_sections);

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_sections; i++) {
        br_scalar scalars[8];
        volatile br_vector3 dp;
        int j;

        C2_HOOK_BUG_ON(BR_ASIZE(scalars) != 8);

        PossibleService();

        /* Section #i: node[0], node[1], min_speed[0], max_speed[0], min_speed[1], max_speed[1], width, type*/
        GetNScalars(pF, BR_ASIZE(scalars), scalars);
        gProgram_state.AI_vehicles.path_sections[i].node_indices[0] = (tU16)scalars[0];
        gProgram_state.AI_vehicles.path_sections[i].node_indices[1] = (tU16)scalars[1];
        gProgram_state.AI_vehicles.path_sections[i].min_speed[0] = (tU8)scalars[2];
        gProgram_state.AI_vehicles.path_sections[i].max_speed[0] = (tU8)scalars[3];
        gProgram_state.AI_vehicles.path_sections[i].min_speed[1] = (tU8)scalars[4];
        gProgram_state.AI_vehicles.path_sections[i].max_speed[1] = (tU8)scalars[5];
        gProgram_state.AI_vehicles.path_sections[i].width = scalars[6];

        BrVector3Sub(&dp,
            &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[i].node_indices[1]].pos,
            &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[i].node_indices[0]].pos);
        gProgram_state.AI_vehicles.path_sections[i].length = BrVector3Length(&dp);

        if (scalars[7] >= 1000.0f) {
            gProgram_state.AI_vehicles.path_sections[i].type = (tU8)(scalars[7] - 1000.0f);
            gProgram_state.AI_vehicles.path_sections[i].one_way = 1;
        } else {
            gProgram_state.AI_vehicles.path_sections[i].type = (tU8)scalars[7];
            gProgram_state.AI_vehicles.path_sections[i].one_way = 0;
        }
        for (j = 0; j < 2; j++) {
            tPath_node* node_ptr;

            node_ptr = &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[i].node_indices[j]];
            if (node_ptr->number_of_sections < 8) {
                node_ptr->sections[node_ptr->number_of_sections] = (tS16)i;
                node_ptr->number_of_sections += 1;
            } else {
                DoNotDprintf_opponent("ERROR: Too many sections (including section #%d) attached to node #%d",
                    i, (tS16)gProgram_state.AI_vehicles.path_sections[i].node_indices[j]);
            }
        }
    }

    if (gAusterity_mode || gNet_mode != eNet_mode_none) {

        gProgram_state.AI_vehicles.number_of_cops = GetAnInt(pF);
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            GetALineAndDontArgue(pF, s);
        }
        gProgram_state.AI_vehicles.number_of_cops = 0;
    } else {

        gProgram_state.AI_vehicles.number_of_cops = GetAnInt(pF);
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            br_scalar scalars[6];
            br_vector3 cop_to_section;
            br_vector3 intersect;
            br_vector3 section_v;
            br_scalar distance;

            C2_HOOK_BUG_ON(CARPOCALYPSE2_ASIZE(scalars) != 6);

            PossibleService();

            /* x, y, z, type[0], type[1], type[2] */
            GetNScalars(pF, CARPOCALYPSE2_ASIZE(scalars), scalars);
            BrVector3Set(&gProgram_state.AI_vehicles.cop_start_points[i],
                scalars[0], scalars[1], scalars[2]);

            if (scalars[3] == 9.0f && scalars[4] == 9.0f && scalars[5] == 9.0f) {
                gBIG_APC_index = i;
            }

            FindNearestPathSection(&gProgram_state.AI_vehicles.cop_start_points[i], &cop_to_section, &intersect, &distance);
            gProgram_state.AI_vehicles.cop_start_vectors[i].v[0] = cop_to_section.v[2] - gOppo_render_zero;
            gProgram_state.AI_vehicles.cop_start_vectors[i].v[1] = 0.0f;
            gProgram_state.AI_vehicles.cop_start_vectors[i].v[2] = gOppo_render_zero - cop_to_section.v[0];
            BrVector3Sub(&section_v, &intersect, &gProgram_state.AI_vehicles.cop_start_points[i]);
            if (BrVector3Dot(&gProgram_state.AI_vehicles.cop_start_vectors[i], &section_v) < gOppo_render_zero) {
                BrVector3Negate(&gProgram_state.AI_vehicles.cop_start_vectors[i], &gProgram_state.AI_vehicles.cop_start_vectors[i]);
            }
        }
    }

    do {
        GetALineAndDontArgue(pF, s);
    } while (strcmp("END OF OPPONENT PATHS", s) != 0);

    if (gProgram_state.AI_vehicles.number_of_path_sections != 0) {
        gBit_per_node = BrMemAllocate((gProgram_state.AI_vehicles.number_of_path_nodes + 7) / 8, kMem_oppo_bit_per_node);
    } else {
        gBit_per_node = NULL;
    }
    DoNotDprintf_opponent("End of LoadInOppoPaths(), totals:");
    DoNotDprintf_opponent("Nodes: %d", gProgram_state.AI_vehicles.number_of_path_nodes);
    DoNotDprintf_opponent("Sections: %d", gProgram_state.AI_vehicles.number_of_path_sections);
}

// FUNCTION: CARMA2_HW 0x004aa400
int C2_HOOK_FASTCALL PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo) {
    br_vector3 from;
    br_vector3 dir;
    br_vector3 norm;
    br_scalar t;
    br_material* material;

    BrVector3Sub(&dir, pTo, pFrom);
    dir.v[1] += 0.15f;
    BrVector3Copy(&from, pFrom);
    from.v[1] += 0.15f;
    FindFace(&from, &dir, &norm, &t, &material);
    return t > 1.0;
}

// C2PORT-NONMATCHING: best 87.96% | difftest PASS 2000 trials (spec difftest/specs/004ad910_PossiblyRepairOpponent.json) | reason VC5 early-return and branch layout
// FUNCTION: CARMA2_HW 0x004ad910
void C2_HOOK_FASTCALL PossiblyRepairOpponent(tOpponent_spec* pOpponent_spec) {
    char copied_message[256];
    char repairing_message[256];
    tU8* pCar_probe;
    int i;

    if ((((tU16)pOpponent_spec->car_spec->car_ID >> 8) != 3)
            && gTime_stamp_for_this_munging > pOpponent_spec->next_repair_check
            && (pOpponent_spec->cheating != 0)
            && pOpponent_spec->current_objective != eOOT_levitate
            && pOpponent_spec->current_objective != eOOT_knackered_and_freewheeling
            && pOpponent_spec->car_spec->knackered == 0) {
        pOpponent_spec->next_repair_check = IRandomBetween(0, 2000) + 3000 + gTime_stamp_for_this_munging;
        if (gTime_stamp_for_this_munging > pOpponent_spec->car_spec->field_0x1548 + pOpponent_spec->repair_interval) {
            i = 0;
            pCar_probe = (tU8*)pOpponent_spec->car_spec + 0x4d8;
            do {
                if (i != 2 && pCar_probe != NULL) {
                    goto repair;
                }
                i++;
                pCar_probe += 0x2c;
            } while (i < 0xc);
            return;
repair:
            DoNotDprintf_opponent("*** Opponent %d, REPAIRING, timestamp %d", pOpponent_spec->car_spec->driver_name, gTime_stamp_for_this_munging);
            TotallyRepairACar(pOpponent_spec->car_spec);
            sprintf(repairing_message, "%s %s", pOpponent_spec->car_spec->driver_name, GetMiscString(0x111));
            strcpy(copied_message, repairing_message);
            i = IRandomBetween(gMinTimeOpponentRepair, gMaxTimeOpponentRepair);
            pOpponent_spec->repair_interval = i * 60000;
            pOpponent_spec->last_repair_time = gTime_stamp_for_this_munging;
            DoNotDprintf_opponent("*** Opponent %s, repair_interval is %d", pOpponent_spec->car_spec->driver_name, pOpponent_spec->repair_interval);
        }
    }
}

// FUNCTION: CARMA2_HW 0x004ada90
void C2_HOOK_FASTCALL CalcOpponentDistanceFromHome(tOpponent_spec* pOpponent_spec) {
    br_vector3 delta;

    delta.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] - pOpponent_spec->start_pos.v[0];
    delta.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] - pOpponent_spec->start_pos.v[1];
    delta.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] - pOpponent_spec->start_pos.v[2];
    pOpponent_spec->distance_from_home = sqrtf(delta.v[0] * delta.v[0] + delta.v[1] * delta.v[1] + delta.v[2] * delta.v[2]);
}

// FUNCTION: CARMA2_HW 0x004adae0
void C2_HOOK_FASTCALL CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec)  {
    br_vector3 pos_in_cop_space;
    br_matrix34 inverse_transform;

    if (pOpponent_spec->next_player_visibility_check >= gTime_stamp_for_this_munging) {
        return;
    }
    pOpponent_spec->player_in_view_now = 0;
    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        pOpponent_spec->next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 100;
        if (pOpponent_spec->player_to_oppo_d < 20.f) {
            BrMatrix34LPInverse(&inverse_transform, &pOpponent_spec->car_spec->car_master_actor->t.t.mat);
            BrMatrix34ApplyP(&pos_in_cop_space, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &inverse_transform);
            if (pos_in_cop_space.v[2] < 0.f && PointVisibleFromHere(&gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
                pOpponent_spec->player_in_view_now = 1;
                pOpponent_spec->acknowledged_piv = 0;
            }
        }
    } else {
        pOpponent_spec->next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 4000;
        DoNotDprintf_opponent("%s: Time now: %9.2f; next vis check at %9.2f", pOpponent_spec->car_spec->driver_name, gTime_stamp_for_this_munging / 1000.f, pOpponent_spec->next_player_visibility_check / 1000.0f);
        if (pOpponent_spec->player_to_oppo_d < 30.f && PointVisibleFromHere(&gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
            pOpponent_spec->player_in_view_now = 1;
            pOpponent_spec->acknowledged_piv = 0;
        }
    }
}

// C2PORT-NONMATCHING: best 40.90% | difftest PASS 2000 trials (spec difftest/specs/004ab190_ProcessPursueAndTwat.json) | reason VC5 branch/register layout
// FUNCTION: CARMA2_HW 0x004ab190
void C2_HOOK_FASTCALL ProcessPursueAndTwat(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    tPursue_car_data* pursue_data;
    tCar_spec* pursuee;
    br_vector3 local_140;
    br_vector3 local_134;
    br_vector3 local_10c;
    br_vector3 local_124;
    br_vector3 normal;
    br_material* material;
    br_scalar local_144;
    br_scalar local_148;
    float velocity_scale;
    int result;
    tS16 section_no;
    char message[256];

    if (pCommand == ePOC_start) {
        DoNotDprintf_opponent("%s: ProcessPursueAndTwat() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->pursue_car_data.direct_line_nodes[0].number_of_sections = 1;
        pOpponent_spec->pursue_car_data.direct_line_nodes[0].sections[0] = 10000;
        pOpponent_spec->pursue_car_data.direct_line_nodes[1].number_of_sections = 1;
        pOpponent_spec->pursue_car_data.direct_line_nodes[1].sections[0] = 10000;
        pOpponent_spec->pursue_car_data.direct_line_section.node_indices[0] = 10000;
        pOpponent_spec->pursue_car_data.direct_line_section.node_indices[1] = 10001;
        pOpponent_spec->pursue_car_data.direct_line_section.min_speed[0] = 0;
        pOpponent_spec->pursue_car_data.direct_line_section.min_speed[1] = 0;
        pOpponent_spec->pursue_car_data.direct_line_section.max_speed[0] = 0xff;
        pOpponent_spec->pursue_car_data.direct_line_section.max_speed[1] = 0xff;
        pOpponent_spec->pursue_car_data.direct_line_section.width = 0.5f;
        pOpponent_spec->pursue_car_data.direct_line_section.type = 0;
        pOpponent_spec->pursue_car_data.time_of_next_visibility_check = 0;
        pOpponent_spec->pursue_car_data.start_backup_time = 0;
        pOpponent_spec->pursue_car_data.time_pursuee_last_visible = 0;
        pOpponent_spec->pursue_car_data.time_last_twatted_em = 0;
        pOpponent_spec->pursue_car_data.time_last_away_from_pursuee = gTime_stamp_for_this_munging;
        pOpponent_spec->pursue_car_data.state = ePCS_what_now;
        return;
    }
    if (pCommand != ePOC_run) {
        return;
    }
    pursue_data = &pOpponent_spec->pursue_car_data;
    pursuee = pursue_data->pursuee;

    if ((pursuee->car_ID & 0xff00) == 0x300 && pOpponent_spec->distance_from_home > gOppo_pursuit_precinct_distance) {
        DoNotDprintf_opponent("%s: Completing pursuit objective because I'm out of my precinct", pOpponent_spec->car_spec->driver_name);
        NewObjective(pOpponent_spec, eOOT_return_to_start);
        return;
    }
    velocity_scale = gOppo_pursuit_near_distance;
    if (gOppo_pursuit_near_distance <= pOpponent_spec->player_to_oppo_d) {
        velocity_scale = pOpponent_spec->player_to_oppo_d;
    }
    pursue_data->direct_line_section.length = velocity_scale;
    if (pOpponent_spec->player_to_oppo_d > gOppo_pursuit_near_distance) {
        pursue_data->time_last_away_from_pursuee = gTime_stamp_for_this_munging;
    }
    if (pOpponent_spec->field_0xb4 != 3 && gOpponents[pOpponent_spec->index].psyche.grudge_against_player < 15) {
        DoNotDprintf_opponent("%s: Completing pursuit objective because I'm happy now", pOpponent_spec->car_spec->driver_name);
        ObjectiveComplete(pOpponent_spec);
        return;
    }

    if (pursue_data->state != ePCS_backing_up) {
        if (gTime_stamp_for_this_munging > pursue_data->time_last_twatted_em + 1000U
                && gTime_stamp_for_this_munging < pursue_data->time_last_twatted_em + 3000U) {
            local_148 = sqrtf(pursuee->collision_info->v.v[0] * pursuee->collision_info->v.v[0]
                    + pursuee->collision_info->v.v[1] * pursuee->collision_info->v.v[1]
                    + pursuee->collision_info->v.v[2] * pursuee->collision_info->v.v[2]);
            if (local_148 < gOppo_pursuit_stationary_speed) {
                DoNotDprintf_opponent("%s: Backing up because we're 'stationary' after colliding with pursuee", pOpponent_spec->car_spec->driver_name);
                pursue_data->start_backup_time = gTime_stamp_for_this_munging;
                pursue_data->state = ePCS_backing_up;
                goto process_state;
            }
        }
        if (pursue_data->time_last_away_from_pursuee + 7000U < gTime_stamp_for_this_munging
                && pursue_data->time_last_twatted_em + 7000U < gTime_stamp_for_this_munging
                && pursue_data->start_backup_time + 10000U < gTime_stamp_for_this_munging) {
            DoNotDprintf_opponent("%s: Backing up because we're too close to pursuee without having twatted him", pOpponent_spec->car_spec->driver_name);
            pursue_data->start_backup_time = gTime_stamp_for_this_munging;
            pursue_data->state = ePCS_backing_up;
            goto process_state;
        } else if (!pOpponent_spec->cheating) {
            if (pursue_data->time_of_next_visibility_check < gTime_stamp_for_this_munging
                    && pOpponent_spec->player_to_oppo_d < gOppo_pursuit_sight_distance) {
                local_140.v[0] = pursuee->car_master_actor->t.t.translate.t.v[0];
                local_140.v[1] = pursuee->car_master_actor->t.t.translate.t.v[1] - gOppo_pursuit_height_offset;
                local_140.v[2] = pursuee->car_master_actor->t.t.translate.t.v[2];
                local_134.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] - local_140.v[0];
                local_134.v[1] = (pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1]
                        - pursuee->car_master_actor->t.t.translate.t.v[1]) - gOppo_pursuit_height_offset;
                local_134.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] - local_140.v[2];
                FindFace(&local_140, &local_134, &normal, &local_144, &material);
                if (gOppo_pursuit_one < local_144) {
                    pursue_data->time_pursuee_last_visible = gTime_stamp_for_this_munging;
                    pursue_data->time_of_next_visibility_check = gTime_stamp_for_this_munging + 900;
                }
            }
        } else if (pOpponent_spec->player_to_oppo_d < gOppo_pursuit_cheat_distance) {
            local_140.v[0] = pursuee->car_master_actor->t.t.translate.t.v[0];
            local_140.v[1] = pursuee->car_master_actor->t.t.translate.t.v[1] - gOppo_pursuit_height_offset;
            local_140.v[2] = pursuee->car_master_actor->t.t.translate.t.v[2];
            local_134.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] - local_140.v[0];
            local_134.v[1] = (pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1]
                    - pursuee->car_master_actor->t.t.translate.t.v[1]) - gOppo_pursuit_height_offset;
            local_134.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] - local_140.v[2];
            FindFace(&local_140, &local_134, &normal, &local_144, &material);
            if (gOppo_pursuit_one < local_144) {
                pursue_data->time_pursuee_last_visible = gTime_stamp_for_this_munging;
            } else {
                pursue_data->time_pursuee_last_visible = 0;
            }
        } else {
            pursue_data->time_pursuee_last_visible = 0;
        }

        if (gTime_stamp_for_this_munging < pursue_data->time_pursuee_last_visible + 3000U) {
            if (pursue_data->state != ePCS_following_line_of_sight) {
                DoNotDprintf_opponent("%s: Commencing ePCS_following_line_of_sight state", pOpponent_spec->car_spec->driver_name);
                pursue_data->state = ePCS_following_line_of_sight;
                sprintf(message, "%s: I've spotted you!", pOpponent_spec->car_spec->driver_name);
                ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 1, 0);
            }
        } else {
            if (pursuee->my_trail.number_of_nodes < 2) {
                DoNotDprintf_opponent("%s: Giving up pursuit - not visible & no trail yet", pOpponent_spec->car_spec->driver_name);
                NewObjective(pOpponent_spec, eOOT_get_near_player);
                return;
            }
            if (pursue_data->state != ePCS_following_trail) {
                section_no = FindNearestGeneralSection(pursuee,
                        &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t,
                        &local_10c, &local_124, &local_148);
                pursue_data->state = ePCS_following_trail;
                if (gOppo_pursuit_trail_distance < local_148 || section_no == -1) {
                    DoNotDprintf_opponent("%s: ...which unfortunately is too far away (%fBRU) or not visible - end of pursuit",
                            pOpponent_spec->car_spec->driver_name, local_148);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                    return;
                }
                local_140.v[0] = local_124.v[0];
                local_140.v[1] = local_124.v[1] - gOppo_pursuit_height_offset;
                local_140.v[2] = local_124.v[2];
                local_134.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] - local_124.v[0];
                local_134.v[1] = (pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] - local_124.v[1])
                        - gOppo_pursuit_height_offset;
                local_134.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] - local_124.v[2];
                FindFace(&local_140, &local_134, &normal, &local_144, &material);
                if (local_144 <= gOppo_pursuit_one) {
                    DoNotDprintf_opponent("%s: ...which unfortunately is too far away (%fBRU) or not visible - end of pursuit",
                            pOpponent_spec->car_spec->driver_name, local_148);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                    return;
                }
                pOpponent_spec->follow_path_data.section_no = section_no;
                DoNotDprintf_opponent("%s: Commencing ePCS_following_trail state", pOpponent_spec->car_spec->driver_name);
                ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 0, 0);
            }
        }
    }

process_state:
    switch (pursue_data->state) {
    case ePCS_backing_up:
        if (gTime_stamp_for_this_munging <= pursue_data->start_backup_time + 0x898U) {
            pOpponent_spec->car_spec->curvature = 0;
            pOpponent_spec->car_spec->brake_force = 0;
            pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->collision_info->M * gOppo_pursuit_reverse_acceleration;
            return;
        }
        pOpponent_spec->car_spec->acc_force = 0;
        pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->collision_info->M * gOppo_pursuit_reverse_brake;
        if (pursue_data->start_backup_time + 3000U < gTime_stamp_for_this_munging) {
            pOpponent_spec->car_spec->brake_force = 0;
            pursue_data->state = ePCS_what_now;
            DoNotDprintf_opponent("%s: Finished backing up.", pOpponent_spec->car_spec->driver_name);
            return;
        }
        break;
    case ePCS_following_line_of_sight:
        pursue_data->direct_line_nodes[0].pos.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0];
        pursue_data->direct_line_nodes[0].pos.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1];
        pursue_data->direct_line_nodes[0].pos.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2];
        local_140.v[0] = pursuee->car_master_actor->t.t.mat.m[3][0] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        local_140.v[1] = pursuee->car_master_actor->t.t.mat.m[3][1] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        local_140.v[2] = pursuee->car_master_actor->t.t.mat.m[3][2] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
        local_134.v[0] = pursuee->collision_info->v.v[0] - pOpponent_spec->car_spec->collision_info->v.v[0];
        local_134.v[1] = pursuee->collision_info->v.v[1] - pOpponent_spec->car_spec->collision_info->v.v[1];
        local_134.v[2] = pursuee->collision_info->v.v[2] - pOpponent_spec->car_spec->collision_info->v.v[2];
        local_144 = sqrtf(local_134.v[0] * local_134.v[0] + local_134.v[1] * local_134.v[1]
                + local_134.v[2] * local_134.v[2]);
        velocity_scale = gOppo_render_zero;
        if (gOppo_pursuit_one <= local_144) {
            velocity_scale = (sqrtf(local_140.v[0] * local_140.v[0] + local_140.v[1] * local_140.v[1]
                    + local_140.v[2] * local_140.v[2]) / local_144)
                    * gOppo_pursuit_velocity_scale;
        }
        pursue_data->direct_line_nodes[1].pos.v[0] = pursuee->collision_info->v.v[0] * velocity_scale;
        pursue_data->direct_line_nodes[1].pos.v[1] = pursuee->collision_info->v.v[1] * velocity_scale;
        pursue_data->direct_line_nodes[1].pos.v[2] = pursuee->collision_info->v.v[2] * velocity_scale;
        pursue_data->direct_line_nodes[1].pos.v[0] += pursuee->car_master_actor->t.t.mat.m[3][0];
        pursue_data->direct_line_nodes[1].pos.v[1] += pursuee->car_master_actor->t.t.mat.m[3][1];
        pursue_data->direct_line_nodes[1].pos.v[2] += pursuee->car_master_actor->t.t.mat.m[3][2];
        ProcessFollowPath(pOpponent_spec, ePOC_run, 1, 1, velocity_scale >= gOppo_pursuit_minimum_velocity_ratio);
        return;
    case ePCS_following_trail:
        if (pursuee->my_trail.nodes_shifted_this_frame != 0) {
            if (pOpponent_spec->follow_path_data.section_no < 15001) {
                pursue_data->state = ePCS_following_trail;
                section_no = FindNearestGeneralSection(pursuee,
                        &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t,
                        &local_10c, &local_124, &local_148);
                DoNotDprintf_opponent("%s: Trail got away; found new trail section %d",
                        pOpponent_spec->car_spec->driver_name, (int)section_no);
                if (section_no == -1 || gOppo_pursuit_trail_distance < local_148) {
                    DoNotDprintf_opponent("%s: ...which unfortunately is too far away (%fBRU) or not visible - end of pursuit",
                            pOpponent_spec->car_spec->driver_name, local_148);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                    return;
                }
                local_140.v[0] = local_124.v[0];
                local_140.v[1] = local_124.v[1] - gOppo_pursuit_height_offset;
                local_140.v[2] = local_124.v[2];
                local_134.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] - local_124.v[0];
                local_134.v[1] = (pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] - local_124.v[1])
                        - gOppo_pursuit_height_offset;
                local_134.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] - local_124.v[2];
                FindFace(&local_140, &local_134, &normal, &local_144, &material);
                if (local_144 <= gOppo_pursuit_one) {
                    DoNotDprintf_opponent("%s: ...which unfortunately is too far away (%fBRU) or not visible - end of pursuit",
                            pOpponent_spec->car_spec->driver_name, local_148);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                    return;
                }
                pOpponent_spec->follow_path_data.section_no = section_no;
                ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 0, 0);
            } else {
                pOpponent_spec->follow_path_data.section_no -= 1;
            }
            DoNotDprintf_opponent("%s: Following re-jobbied section %d/%d", pOpponent_spec->car_spec->driver_name,
                    (int)pOpponent_spec->follow_path_data.section_no, (int)pursuee->my_trail.number_of_nodes - 1);
        }
        sprintf(message, "%s: Trail section %d/%d", pOpponent_spec->car_spec->driver_name,
                (int)pOpponent_spec->follow_path_data.section_no, (int)pursuee->my_trail.number_of_nodes - 1);
        result = ProcessFollowPath(pOpponent_spec, ePOC_run, 1, 0, 0);
        if (result == eFPR_given_up || result == eFPR_end_of_path) {
            NewObjective(pOpponent_spec, eOOT_get_near_player);
            return;
        }
        break;
    case ePCS_what_now:
        PDEnterDebugger("ERROR: what_now state called in ProcessPursueAndTwat()");
        return;

    }
}

// FUNCTION: CARMA2_HW 0x004a9c50
void C2_HOOK_FASTCALL DisposeOpponentPaths(void) {

    if (gProgram_state.AI_vehicles.path_nodes != NULL) {
        BrMemFree(gProgram_state.AI_vehicles.path_nodes);
    }
    gProgram_state.AI_vehicles.number_of_path_nodes = 0;
    gProgram_state.AI_vehicles.path_nodes = NULL;

    if (gProgram_state.AI_vehicles.path_sections != NULL) {
        BrMemFree(gProgram_state.AI_vehicles.path_sections);
    }
    gProgram_state.AI_vehicles.number_of_path_sections = 0;
    gProgram_state.AI_vehicles.path_sections = NULL;

    if (gBit_per_node != NULL) {
        BrMemFree(gBit_per_node);
    }
    gBit_per_node = NULL;
}

// C2PORT-NONMATCHING: best 48.21% | difftest PASS 2000 trials (spec difftest/specs/004a9cb0_UpdateViewedOpponentHeadup.json) | reason vc5-regalloc-branch-layout
// FUNCTION: CARMA2_HW 0x004a9cb0
void C2_HOOK_FASTCALL UpdateViewedOpponentHeadup(void) {
    int i;
    tOpponent_spec* opponent;
    tCar_spec** car_specs;
    int* opponent_data;
    int displayed_section_no;
    const char* cheating_marker;
    int section_no;
    const char* direction;
    const char* extra_section;
    char message[256];
    tCar_spec* viewed_car;

    viewed_car = gCar_to_view;
    if (viewed_car == NULL || viewed_car->driver != eDriver_oppo) {
        message[0] = '\0';
    } else {
        if ((viewed_car->car_ID & 0xff00) == 0x0200) {
            for (i = 0, car_specs = &gProgram_state.AI_vehicles.opponents[0].car_spec;
                i < gProgram_state.AI_vehicles.number_of_opponents;
                i++, car_specs += sizeof(tOpponent_spec) / sizeof(*car_specs)) {
                if (*car_specs == viewed_car) {
                    opponent_data = (int*)&gProgram_state.AI_vehicles.opponents[0]
                        + i * sizeof(tOpponent_spec) / sizeof(int);
                    goto opponent_found;
                }
            }
        } else if ((viewed_car->car_ID & 0xff00) == 0x0300) {
            for (i = 0, car_specs = &gProgram_state.AI_vehicles.cops[0].car_spec;
                i < gNumber_of_cops_before_faffage;
                i++, car_specs += sizeof(tOpponent_spec) / sizeof(*car_specs)) {
                if (*car_specs == viewed_car) {
                    opponent_data = (int*)&gProgram_state.AI_vehicles.cops[0]
                        + i * sizeof(tOpponent_spec) / sizeof(int);
                    goto opponent_found;
                }
            }
        }
        opponent_data = NULL;

opponent_found:
        opponent = (tOpponent_spec*)opponent_data;
        section_no = opponent->follow_path_data.section_no;
        cheating_marker = gSection_text_0065a788;
        if (!opponent->cheating) {
            cheating_marker = gEmpty_string_00676920;
        }

        if (section_no < 20000 || opponent->next_sections[section_no - 20000].direction != 0) {
            direction = gSection_text_0065a780;
        } else {
            direction = gSection_text_0065a784;
        }
        if (section_no < 15000 || section_no >= 20000) {
            extra_section = gEmpty_string_00676920;
        } else {
            extra_section = gSection_text_0065a77c;
        }
        displayed_section_no = section_no;
        if (section_no >= 20000) {
            displayed_section_no = opponent->next_sections[section_no - 20000].section_no;
        }
        sprintf(message, gFormat_opponent_headup_0065a750, viewed_car->name,
            opponent->current_objective, opponent->nnext_sections, section_no, displayed_section_no,
            extra_section, direction, cheating_marker);
    }
    ChangeHeadupText(gProgram_state.frame_rate_headup, message);
}

// FUNCTION: CARMA2_HW 0x004a7ea0
void C2_HOOK_FASTCALL AddIfNotInList(tPhysics_object* pObject, tPhysics_object** pList, int pCount) {
    int i;

    for (i = 0; i < pCount; i++) {
        if (pList[i] == pObject) {
            pList[i] = NULL;
            return;
        }
    }
    PHILAddActiveObject(pObject, NULL, NULL, NULL);
}

// FUNCTION: CARMA2_HW 0x004a7ef0
void C2_HOOK_FASTCALL RemoveAnythingStillInList(tPhysics_object** pList, int pCount) {
    int i;

    for (i = 0; i < pCount; i++) {
        if (pList[i] != NULL) {
            PHILRemoveObject(pList[i]);
            pList[i]->field183_0x1d8 = 0;
        }
    }
}

void C2_HOOK_FASTCALL NoteCarsCurrentlyUsed(tPhysics_object** pCollision_infos, int* pCount) {
    tPhysics_object* info;
    tPhysics_object* next;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, flags_0x238, 0x238);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tPhysics_object, owner, 0x23c);

    for (info = PHILGetFirstObject(); info != NULL; info = next) {
        next = PHILGetNextObject(info);
        if (info != NULL && info->owner != NULL && info->flags_0x238 == 1 && ((tCar_spec*)info->owner)->driver > 5) {
            pCollision_infos[*pCount] = info;
            *pCount += 1;
        }
    }
}

int C2_HOOK_FASTCALL IsNetCarActive(const br_vector3* pPoint) {
    br_vector3 tv;

    BrVector3Sub(&tv, &gProgram_state.current_car.car_master_actor->t.t.translate.t, pPoint);
    if (BrVector3LengthSquared(&tv) < 100.f) {
        return 1;
    }
    if (gCar_to_view != &gProgram_state.current_car) {
        BrVector3Sub(&tv, &gCar_to_view->car_master_actor->t.t.translate.t, pPoint);
        return BrVector3LengthSquared(&tv) < 100.f;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004a7a80
void C2_HOOK_FASTCALL RebuildActiveCarList(void) {
    int new_count_active_cars = gNum_active_cars;

    if (gActive_car_list_rebuild_required) {
        int count_used_cars;
        tPhysics_object* used_cars[1000];
        int i;

        gActive_car_list_rebuild_required = 0;

        count_used_cars = 0;
        NoteCarsCurrentlyUsed(used_cars, &count_used_cars);

        new_count_active_cars = 0;
        for (i = 0; i < GetCarCount(eVehicle_self); i++) {
            tCar_spec* car;

            if (i > 0) {
                PDFatalError("mGet_car_count( eVehicle_self ) > 1 - I didn't know this could happen!");
            }
            car = GetCarSpec(eVehicle_self, i);
            if (!car->disabled || gAction_replay_mode) {
                car->active = 1;
                gActive_car_list[new_count_active_cars] = car;
                new_count_active_cars += 1;
                AddIfNotInList(car->collision_info, used_cars, count_used_cars);
            }
        }
        if (gNet_mode == eNet_mode_host) {
            for (i = 0; i < (gNet_mode == eNet_mode_none ? 0 : gNumber_of_net_players - 1); i++) {
                tCar_spec* car;

                car = GetCarSpec(eVehicle_net_player, i);
                if (!car->disabled) {
                    car->active = 1;
                    gActive_car_list[new_count_active_cars] = car;
                    new_count_active_cars += 1;

                    AddIfNotInList(car->collision_info, used_cars, count_used_cars);
                } else {
                    car->active = 0;
                }
            }
        } else if (gNet_mode == eNet_mode_client) {
            for (i = 0; i < (gNet_mode == eNet_mode_none ? 0 : gNumber_of_net_players - 1); i++) {
                tCar_spec* car;

                car = GetCarSpec(eVehicle_net_player, i);
                if (!car->disabled && IsNetCarActive(&car->car_master_actor->t.t.translate.t)) {
                    car->active = 1;
                    gActive_car_list[new_count_active_cars] = car;
                    new_count_active_cars += 1;
                    AddIfNotInList(car->collision_info, used_cars, count_used_cars);
                } else {
                    car->active = 0;
                }
            }
        }

        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            tCar_spec* car;

            car = GetCarSpec(eVehicle_opponent, i);
            if (gProgram_state.AI_vehicles.opponents[i].physics_me || gAction_replay_mode) {
                car->active = 1;
                gActive_car_list[new_count_active_cars] = car;
                new_count_active_cars += 1;
                AddIfNotInList(car->collision_info,used_cars, count_used_cars);
            } else {
                car->active = 0;
            }
        }

        for (i = 0; i < gNumber_of_cops_before_faffage; i++) {
            tCar_spec* car;

            car = GetCarSpec(eVehicle_rozzer, i);
            if (gProgram_state.AI_vehicles.cops[i].physics_me || gAction_replay_mode) {
                car->active = 1;
                gActive_car_list[new_count_active_cars] = car;
                new_count_active_cars += 1;
                AddIfNotInList(car->collision_info, used_cars, count_used_cars);
            }
        }
        RemoveAnythingStillInList(used_cars, count_used_cars);
    }
    if (gNum_active_cars != new_count_active_cars) {
        gNum_active_cars = new_count_active_cars;
    }
}

// C2PORT-NONMATCHING: best 57.78% | difftest PASS 2000 trials (spec difftest/specs/004ae380_SetInitialCopPositions.json) | reason VC5 pointer-base and loop register allocation
// FUNCTION: CARMA2_HW 0x004ae380
void C2_HOOK_FASTCALL SetInitialCopPositions(void) {
    int i;
    tU32 time_stamp;
    volatile tU32* pCop_fields;
    volatile tU32* pStart_words;

    i = 0;
    gNumber_of_cops_before_faffage = gProgram_state.AI_vehicles.number_of_cops;
    gDefinite_no_cop_pursuit_speed = 17.8788f;
    gCop_pursuit_speed_value_00691730 = 44.697f;
    gCop_pursuit_speed_percentage_multiplier = 3.7288110256195068359375f;

    /* pCop_fields starts at stun_time_ends; its relative words address the full tOpponent_spec. */
    if (gProgram_state.AI_vehicles.number_of_cops > 0) {
        pCop_fields = (volatile tU32*)&gProgram_state.AI_vehicles.cops[0].stun_time_ends;
        pStart_words = (tU32*)&gProgram_state.AI_vehicles.cop_start_points[0].v[1];
        do {
            PossibleService();
            ((tCar_spec*)pCop_fields[-0x15])->car_ID = i + 0x300;
            pCop_fields[-0x17] = 0x62;
            time_stamp = gTime_stamp_for_this_munging;
            pCop_fields[4] = time_stamp;
            pCop_fields[5] = time_stamp;
            pCop_fields[2] = time_stamp;
            pCop_fields[3] = 0;
            pCop_fields[0] = 0;
            pCop_fields[1] = IRandomBetween(0, 900) + 5000 + gTime_stamp_for_this_munging;
            pCop_fields[-4] = gTime_stamp_for_this_munging + 500;
            *(volatile tU32*)&pCop_fields[0x15] = (*(volatile tU32*)&pCop_fields[0x15] & 0xfffff7c9) | 8;
            *(tU16*)&pCop_fields[7] = 0xffff;
            pCop_fields[8] = 0;
            *(volatile tU32*)&pCop_fields[0x15] |= 1;
            pCop_fields[-0x16] = 0;
            *(volatile tU32*)&pCop_fields[0x15] &= 0xfffff8ff;
            /* The original pointer walk writes this value into opponents[i].nastiness. */
            *(volatile float*)&pCop_fields[-0xc62] = (((gCurrent_race.race_spec->opponent_nastiness_level - (float)gProgram_state.skill_level * gCop_nastiness_skill_adjustment) - gCop_nastiness_bias) * gCop_nastiness_divisor) * gOpponent_nastyness_frigger;
            pCop_fields[-0x10] = pStart_words[-1];
            pCop_fields[-0xf] = pStart_words[0];
            pCop_fields[-0xe] = pStart_words[1];
            pCop_fields[-0xd] = pStart_words[0x1d];
            pCop_fields[-0xc] = pStart_words[0x1e];
            pCop_fields[-0xb] = pStart_words[0x1f];
            pCop_fields[-10] = 0;
            pCop_fields[-9] = 0;
            pCop_fields[-8] = 0;
            gViewable_car_list[gNum_viewable_cars] = (tCar_spec*)pCop_fields[-0x15];
            gNum_viewable_cars++;
            ((tCar_spec*)pCop_fields[-0x15])->last_person_to_hit_us = NULL;
            ((tCar_spec*)pCop_fields[-0x15])->last_person_we_hit = NULL;
            ((tCar_spec*)pCop_fields[-0x15])->last_collision_time = 0;
            ((tCar_spec*)pCop_fields[-0x15])->field_0x1548 = 0;
            ((tCar_spec*)pCop_fields[-0x15])->no_of_processes_recording_my_trail = 0;
            ((tCar_spec*)pCop_fields[-0x15])->grudge_raised_recently = 1;
            gOpponents[pCop_fields[-0x17]].psyche.grudge_against_player = 10;
            ((tCar_spec*)pCop_fields[-0x15])->acc_force = 0.f;
            ((tCar_spec*)pCop_fields[-0x15])->brake_force = 0.f;
            ((tCar_spec*)pCop_fields[-0x15])->curvature = 0.f;
            time_stamp = gTime_stamp_for_this_munging + 0x0fffffff;
            if (pCop_fields[0] > time_stamp) {
                time_stamp = pCop_fields[0];
            }
            pCop_fields[0] = time_stamp;
            i++;
            pStart_words += 3;
            pCop_fields += 0x69;
        } while (i < gProgram_state.AI_vehicles.number_of_cops);
    }

    i = 0;
    pCop_fields = (volatile tU32*)&gProgram_state.AI_vehicles.cops[0].car_spec;
    pStart_words = (tU32*)&gProgram_state.AI_vehicles.cop_start_points[0].v[1];
second_cop_loop:
    if (i >= *(volatile int*)&gNumber_of_cops_before_faffage) {
        goto done;
    }
    PossibleService();
    *(tU32*)&((tCar_spec*)pCop_fields[0])->car_master_actor->t.t.translate.t.v[0] = pStart_words[-1];
    *(tU32*)&((tCar_spec*)pCop_fields[0])->car_master_actor->t.t.translate.t.v[1] = pStart_words[0];
    *(tU32*)&((tCar_spec*)pCop_fields[0])->car_master_actor->t.t.translate.t.v[2] = pStart_words[1];
    PointActorAlongThisBloodyVector(((tCar_spec*)pCop_fields[0])->car_master_actor, (br_vector3*)(pStart_words + 0x1d));
    pCop_fields[0x2a] &= 0xfffffff7;
    RematerialiseOpponent((tOpponent_spec*)&pCop_fields[-2], 0.f);
    InitCarSkidStuff((tCar_spec*)pCop_fields[0]);
    i++;
    pStart_words += 3;
    pCop_fields += 0x69;
    goto second_cop_loop;
done:
    return;
}

// FUNCTION: CARMA2_HW 0x004ae5d0
void C2_HOOK_FASTCALL DisposeOpponents(void) {
    int i;

    C2_HOOK_BUG_ON(offsetof(tIntelligent_vehicles, cops) != 12856);
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
        DisposeCar(gProgram_state.AI_vehicles.cops[i].car_spec, (i == gBIG_APC_index) ? 99 : 98);
        BrMemFree(gProgram_state.AI_vehicles.cops[i].car_spec);
    }
}

// FUNCTION: CARMA2_HW 0x004ae880
const char* C2_HOOK_FASTCALL GetDriverName(tVehicle_type pCategory, int pIndex) {
    tCar_spec* c;
    switch (pCategory) {
    case eVehicle_self:
        return gProgram_state.player_name;
    case eVehicle_opponent:
        c = gProgram_state.AI_vehicles.opponents[pIndex].car_spec;
        if (c != NULL) {
            return c->driver_name;
        }
        return NULL;
    case eVehicle_rozzer:
        return "Faceless Cop";
    case eVehicle_drone:
        return "Innocent Civilian";
    case eVehicle_not_really:
    default:
        return NULL;
    }
}

void C2_HOOK_FASTCALL StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds) {

    pOpponent_spec->car_spec->acc_force = 0.f;
    pOpponent_spec->car_spec->brake_force = 0.f;
    pOpponent_spec->car_spec->curvature = 0.f;
    pOpponent_spec->stun_time_ends = MAX(gTime_stamp_for_this_munging + pMilliseconds, pOpponent_spec->stun_time_ends);
}

// FUNCTION: CARMA2_HW 0x004ae970
tCar_spec* C2_HOOK_FASTCALL GetCarSpecFromGlobalOppoIndex(int pIndex) {
    int i;

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        if (gProgram_state.AI_vehicles.opponents[i].index == pIndex) {
            return gProgram_state.AI_vehicles.opponents[i].car_spec;
        }
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x004ae8e0
tOpponent_spec* C2_HOOK_FASTCALL GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec) {
    int i;

    if ((pCar_spec->car_ID & 0xff00) == 0x200) {
        for (i = 0; i < GetCarCount(eVehicle_opponent); i++) {
            if (gProgram_state.AI_vehicles.opponents[i].car_spec == pCar_spec) {
                return &gProgram_state.AI_vehicles.opponents[i];
            }
        }
    } else if ((pCar_spec->car_ID & 0xff00) == 0x300) {
        for (i = 0; i < GetCarCount(eVehicle_rozzer); i++) {
            if (gProgram_state.AI_vehicles.cops[i].car_spec == pCar_spec) {
                return &gProgram_state.AI_vehicles.cops[i];
            }
        }
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x004a8160
void C2_HOOK_CDECL DoNotDprintf_opponent(const char* pMessage, ...) {
}

// FUNCTION: CARMA2_HW 0x004aeeb0
void C2_HOOK_FASTCALL MarkPlayerInteraction(tCar_spec* pCar_a, tCar_spec* pCar_b) {

    if (pCar_a != NULL && pCar_a->driver == eDriver_local_human) {
        pCar_b->field_0x1548 = gTime_stamp_for_this_munging;
    }
    if (pCar_b != NULL && pCar_b->driver == eDriver_local_human) {
        pCar_a->field_0x1548 = gTime_stamp_for_this_munging;
    }
}

// Keep the original GetCarCount calls in the final cop lookup.
#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x004aeef0
void C2_HOOK_FASTCALL RecordOpponentTwattageOccurrence(tCar_spec* pTwatter, tCar_spec* pTwattee) {
    int bangness;
    int twatter_index;
    int twattee_index;
    int grudginess_caused_by_damage;
    int new_grudge_value;
    int stun_duration;
    float damage;
    tOpponent_spec* twattee_opponent_spec;
    tOpponent_spec* twatter_opponent_spec;
    tOpponent_spec* cop;

    if ((pTwatter != NULL && pTwatter->driver == eDriver_oppo) || (pTwattee != NULL && pTwattee->driver == eDriver_oppo)) {
        damage = pTwattee->damage_magnitude_accumulator;
        bangness = (int)((100.0 < sqrt(damage * 300000.0)) ? 100.0 : sqrt(damage * 300000.0));
        grudginess_caused_by_damage = bangness / 10;
        grudginess_caused_by_damage += 50 * ((pTwattee->car_ID & 0xff00) == 0x300);
        DoNotDprintf_opponent("Frame %0.6d: %s hit %s, damage %f, bangness %d, gBig_bang %d, grudginess %d",
            gAcme_frame_count,
            pTwatter->driver_name,
            pTwattee->driver_name,
            damage,
            bangness,
            gBig_bang,
            grudginess_caused_by_damage);
        if (bangness < 5) {
            return;
        }
        pTwatter->last_person_we_hit = pTwattee;
        pTwatter->last_collision_time = gTime_stamp_for_this_munging;
        pTwattee->last_collision_time = gTime_stamp_for_this_munging;
        pTwattee->last_person_to_hit_us = pTwatter;
        pTwattee->grudge_raised_recently = 1;
        if (bangness >= gBig_bang || (((pTwattee->car_ID & 0xff00) == 0x300) != 0)) {
            pTwattee->big_bang = 1;
        }
        if (bangness >= 80) {
            pTwattee->scary_bang = 1;
        }
        if (pTwatter != NULL && pTwatter->driver == eDriver_local_human) {
            if ((pTwattee->car_ID & 0xff00) == 0x200) {
                for (twattee_index = 0; twattee_index < gProgram_state.AI_vehicles.number_of_opponents; twattee_index++) {
                    if (gProgram_state.AI_vehicles.opponents[twattee_index].car_spec == pTwattee) {
                        twattee_opponent_spec = &gProgram_state.AI_vehicles.opponents[twattee_index];
                        goto found_player_victim;
                    }
                }
            } else if ((pTwattee->car_ID & 0xff00) == 0x300) {
                for (twattee_index = 0; twattee_index < gNumber_of_cops_before_faffage; twattee_index++) {
                    if (gProgram_state.AI_vehicles.cops[twattee_index].car_spec == pTwattee) {
                        twattee_opponent_spec = &gProgram_state.AI_vehicles.cops[twattee_index];
                        goto found_player_victim;
                    }
                }
            }
            twattee_opponent_spec = NULL;
        found_player_victim:
            if (pTwattee->scary_bang) {
                if (gOpponents[twattee_opponent_spec->index].strength_rating < 5) {
                    stun_duration = 30 * MIN(bangness, 100) + 1000;
                    twattee_opponent_spec->car_spec->acc_force = 0.f;
                    twattee_opponent_spec->car_spec->brake_force = 0.f;
                    twattee_opponent_spec->car_spec->curvature = 0.f;
                    twattee_opponent_spec->stun_time_ends = MAX(twattee_opponent_spec->stun_time_ends, gTime_stamp_for_this_munging + stun_duration);
                }
            }
            twattee_index = twattee_opponent_spec->index;
            new_grudge_value = grudginess_caused_by_damage + gOpponents[twattee_index].psyche.grudge_against_player;
            if (new_grudge_value > 100) {
                new_grudge_value = 100;
            }
            gOpponents[twattee_index].psyche.grudge_against_player = new_grudge_value;
        } else if (pTwattee != NULL && pTwattee->driver == eDriver_local_human) {
            if ((pTwatter->car_ID & 0xff00) == 0x200) {
                for (twatter_index = 0; twatter_index < gProgram_state.AI_vehicles.number_of_opponents; twatter_index++) {
                    if (gProgram_state.AI_vehicles.opponents[twatter_index].car_spec == pTwatter) {
                        twatter_opponent_spec = &gProgram_state.AI_vehicles.opponents[twatter_index];
                        goto found_player_attacker;
                    }
                }
            } else if ((pTwatter->car_ID & 0xff00) == 0x300) {
                for (twatter_index = 0; twatter_index < gNumber_of_cops_before_faffage; twatter_index++) {
                    if (gProgram_state.AI_vehicles.cops[twatter_index].car_spec == pTwatter) {
                        twatter_opponent_spec = &gProgram_state.AI_vehicles.cops[twatter_index];
                        goto found_player_attacker;
                    }
                }
            }
            twatter_opponent_spec = NULL;
        found_player_attacker:
            if (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat && twatter_opponent_spec->pursue_car_data.pursuee == pTwattee) {
                twatter_opponent_spec->pursue_car_data.time_last_twatted_em = gTime_stamp_for_this_munging;
            }
            twatter_index = twatter_opponent_spec->index;
            new_grudge_value = gOpponents[twatter_index].psyche.grudge_against_player - (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat ? 0 : 2 * grudginess_caused_by_damage);
            if (new_grudge_value < 0) {
                new_grudge_value = 0;
            }
            gOpponents[twatter_index].psyche.grudge_against_player = new_grudge_value;
        } else {
            if ((pTwatter->car_ID & 0xff00) == 0x200) {
                for (twatter_index = 0; twatter_index < gProgram_state.AI_vehicles.number_of_opponents; twatter_index++) {
                    if (gProgram_state.AI_vehicles.opponents[twatter_index].car_spec == pTwatter) {
                        twatter_opponent_spec = &gProgram_state.AI_vehicles.opponents[twatter_index];
                        goto found_ai_attacker;
                    }
                }
            } else if ((pTwatter->car_ID & 0xff00) == 0x300) {
                for (twatter_index = 0; twatter_index < gNumber_of_cops_before_faffage; twatter_index++) {
                    if (gProgram_state.AI_vehicles.cops[twatter_index].car_spec == pTwatter) {
                        twatter_opponent_spec = &gProgram_state.AI_vehicles.cops[twatter_index];
                        goto found_ai_attacker;
                    }
                }
            }
            twatter_opponent_spec = NULL;
        found_ai_attacker:
            if ((pTwattee->car_ID & 0xff00) == 0x200) {
                for (twattee_index = 0; twattee_index < gProgram_state.AI_vehicles.number_of_opponents; twattee_index++) {
                    if (gProgram_state.AI_vehicles.opponents[twattee_index].car_spec == pTwattee) {
                        twattee_opponent_spec = &gProgram_state.AI_vehicles.opponents[twattee_index];
                        goto found_ai_victim;
                    }
                }
            } else if ((pTwattee->car_ID & 0xff00) == 0x300) {
                twattee_index = 0;
                if (GetCarCount(eVehicle_rozzer) > 0) {
                    cop = gProgram_state.AI_vehicles.cops;
                    do {
                        if (cop->car_spec == pTwattee) {
                            twattee_opponent_spec = &gProgram_state.AI_vehicles.cops[twattee_index];
                            goto found_ai_victim;
                        }
                        twattee_index++;
                        cop++;
                    } while (twattee_index < GetCarCount(eVehicle_rozzer));
                }
            }
            twattee_opponent_spec = NULL;
        found_ai_victim:
            if (pTwattee->scary_bang) {
                if (gOpponents[twattee_opponent_spec->index].strength_rating < 5) {
                    stun_duration = 30 * bangness + 1000;
                    twattee_opponent_spec->car_spec->acc_force = 0.f;
                    twattee_opponent_spec->car_spec->brake_force = 0.f;
                    twattee_opponent_spec->car_spec->curvature = 0.f;
                    twattee_opponent_spec->stun_time_ends = MAX(twattee_opponent_spec->stun_time_ends, gTime_stamp_for_this_munging + stun_duration);
                }
            }
            twattee_index = twattee_opponent_spec->index;
            if (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat && twatter_opponent_spec->pursue_car_data.pursuee == pTwattee) {
                twatter_opponent_spec->pursue_car_data.time_last_twatted_em = gTime_stamp_for_this_munging;
            }
            if (CAR_SPEC_IS_OPPONENT(pTwatter) && (((pTwattee->car_ID & 0xff00) == 0x300) != 0)) {
                new_grudge_value = grudginess_caused_by_damage + gOpponents[twattee_index].psyche.grudge_against_player;
                if (new_grudge_value > 100) {
                    new_grudge_value = 100;
                }
                gOpponents[twattee_index].psyche.grudge_against_player = new_grudge_value;
            }
        }
    }
}

#pragma inline_depth()

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004af490
void C2_HOOK_FASTCALL FUN_004af490(void) {

    // empty
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004add50
void C2_HOOK_FASTCALL InitOpponents(tRace_info* pRace_info) {
    int i;
    int opponent_number;
    int rank_dependent_difficulty;
    br_bounds bounds;
    tCar_spec* car_spec;
    tOpponent_spec* opponent_spec;
    int opponents_left;

    gNext_grudge_reduction = gTime_stamp_for_this_munging + 10000;
    gFirst_frame = 1;
    gNum_viewable_cars = 1;
    gGrudge_reduction_per_period = 3 - gProgram_state.skill_level;
    gAcknowledged_start = 0;
    gStart_jumped = 0;
    gViewable_car_list[0] = &gProgram_state.current_car;
    BrActorToBounds(&bounds, gProgram_state.track_spec.the_actor);
    gMinimum_yness_before_knackerisation = bounds.min.v[1] - 2.f;
    gAcme_frame_count = 0;
    gProgram_state.current_car.no_of_processes_recording_my_trail = 0;
    gProgram_state.current_car.field_0x1540 = 0;
    gProgram_state.current_car.field_0x155c = -1;
    gHead_on_cos_value = cosf(.5235668420791626f); /* around 30 degrees */
    rank_dependent_difficulty = (int)((101.f - 99.f * (1.f - (float)gCurrent_race.index / (float)gNumber_of_races)) / -10.f);
    gBig_bang = (int)(70.f - (10 * gProgram_state.skill_level - 3 * rank_dependent_difficulty) * gOpponent_nastyness_frigger);
    gIn_view_distance = gCamera_yon + 10.f;
    if (gIn_view_distance < 45.f) {
        gIn_view_distance = 45.f;
    }
    if (gNet_mode == eNet_mode_none) {
        gProgram_state.AI_vehicles.number_of_opponents = pRace_info->number_of_racers - 1;
    } else {
        gProgram_state.AI_vehicles.number_of_opponents = 0;
    }
    for (i = 0, opponent_number = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++, opponent_number++) {
        PossibleService();
        if (pRace_info->opponent_list[opponent_number].index < 0) {
            opponent_number++;
        }
        gProgram_state.AI_vehicles.opponents[i].car_spec = pRace_info->opponent_list[opponent_number].car_spec;
        gProgram_state.AI_vehicles.opponents[i].car_spec->car_ID = i | 0x200;
        DoNotDprintf_opponent("Car '%s', car_ID %x",
            gProgram_state.AI_vehicles.opponents[i].car_spec->driver_name,
            gProgram_state.AI_vehicles.opponents[i].car_spec->car_ID);
        gProgram_state.AI_vehicles.opponents[i].index = pRace_info->opponent_list[opponent_number].index;
        gProgram_state.AI_vehicles.opponents[i].time_last_processed = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].next_repair_check = 0;
        gProgram_state.AI_vehicles.opponents[i].repair_interval = 60000 * IRandomBetween(gMinTimeOpponentRepair, gMaxTimeOpponentRepair);
        gProgram_state.AI_vehicles.opponents[i].last_repair_time = gTime_stamp_for_this_munging;
        DoNotDprintf_opponent("*** Opponent %s, repair_interval is %d",
            gProgram_state.AI_vehicles.opponents[i].car_spec->driver_name,
            gProgram_state.AI_vehicles.opponents[i].repair_interval);
        gProgram_state.AI_vehicles.opponents[i].time_this_objective_started = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].last_moved_ok = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].last_in_view = 0;
        gProgram_state.AI_vehicles.opponents[i].stun_time_ends = 0;
        gProgram_state.AI_vehicles.opponents[i].next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 2000;
        gProgram_state.AI_vehicles.opponents[i].next_out_of_world_check = gTime_stamp_for_this_munging + 500;

        /*
         * gProgram_state.AI_vehicles.opponents[i].flags &= ~0x36;
         * gProgram_state.AI_vehicles.opponents[i].flags |= 0x8;
         */
        gProgram_state.AI_vehicles.opponents[i].finished_for_this_race = 0;
        gProgram_state.AI_vehicles.opponents[i].physics_me = 1;
        gProgram_state.AI_vehicles.opponents[i].pursue_from_start = 0;
        gProgram_state.AI_vehicles.opponents[i].cheating = 0;
        gProgram_state.AI_vehicles.opponents[i].knackeredness_detected = 0;

        gProgram_state.AI_vehicles.opponents[i].players_section_when_last_calced_full_path = -1;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_person_to_hit_us = NULL;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_person_we_hit = NULL;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_collision_time = 0;
        gProgram_state.AI_vehicles.opponents[i].car_spec->field_0x1548 = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].car_spec->grudge_raised_recently = 1;
        gProgram_state.AI_vehicles.opponents[i].car_spec->no_of_processes_recording_my_trail = 0;
        gProgram_state.AI_vehicles.opponents[i].nnext_sections = 0;
        gProgram_state.AI_vehicles.opponents[i].new_objective_required = 1;
        gProgram_state.AI_vehicles.opponents[i].current_objective = eOOT_none;

        /* gProgram_state.AI_vehicles.opponents[i].flags &= ~0x700; */
        gProgram_state.AI_vehicles.opponents[i].has_moved_at_some_point = 0;
        gProgram_state.AI_vehicles.opponents[i].player_in_view_now = 0;
        gProgram_state.AI_vehicles.opponents[i].acknowledged_piv = 0;
        gProgram_state.AI_vehicles.opponents[i].field_0xb4 = 0;
        gProgram_state.AI_vehicles.opponents[i].nastiness = (
                  gCurrent_race.race_spec->opponent_nastiness_level
                + gProgram_state.skill_level / 2.f
                + (gOpponents[gProgram_state.AI_vehicles.opponents[i].index].strength_rating - 1) / 4.f
                ) / 3.f * gOpponent_nastyness_frigger;

        BrVector3Set(&gProgram_state.AI_vehicles.opponents[i].pos_last_frame, 0.f, 0.f, 0.f);
        gOpponents[gProgram_state.AI_vehicles.opponents[i].index].psyche.grudge_against_player = 10;
        gViewable_car_list[gNum_viewable_cars] = gProgram_state.AI_vehicles.opponents[i].car_spec;
        gNum_viewable_cars++;
        StunTheBugger(&gProgram_state.AI_vehicles.opponents[i], 0xfffffff);
    }
    if (gChallenger_index__opponent >= 0) {
        car_spec = GetCarSpecFromGlobalOppoIndex(gChallenger_index__opponent);
        opponent_spec = GetOpponentSpecFromCarSpec(car_spec);
        if (opponent_spec == NULL) {
            DoNotDprintf_opponent("ERROR - can't record dare - no opponent_spec for car_spec");
        } else {
            opponent_spec->pursue_from_start = 1;
        }
        gChallenger_index__opponent = -1;
    }
    /* cops are not initialized in carpocalypse2 */

    opponents_left = NumberOfOpponentsStillRunning();
    if (opponents_left != 0) {
        if (gCurrent_race.race_spec->opponent_nastiness_level < 10.f) {
            int count_type_2;
            int count_type_1;

            for (i = 0; i < GetCarCount(eVehicle_opponent); i++) {
                gProgram_state.AI_vehicles.opponents[i].field_0xb4 = 0;
            }

            count_type_2 = (int)(0.6f * opponents_left);
            for (i = 0; i < count_type_2; i++) {
                int rstart;
                int r;

                rstart = IRandomBetween(0, opponents_left - 1 - i);
                for (r = rstart;;) {
                    if (gProgram_state.AI_vehicles.opponents[r].field_0xb4 == 0) {
                        gProgram_state.AI_vehicles.opponents[r].field_0xb4 = 2;
                        break;
                    }
                    r++;
                    if (r >= opponents_left) {
                        r = 0;
                    }
                    if (r == rstart) {
                        break;
                    }
                }
            }
            count_type_1 = (int)(0.2f * opponents_left);
            for (i = 0; i < count_type_1; i++) {
                int rstart;
                int r;

                rstart = IRandomBetween(0, opponents_left - count_type_2 - 1 - i);
                for (r = rstart;;) {
                    if (gProgram_state.AI_vehicles.opponents[r].field_0xb4 == 0) {
                        gProgram_state.AI_vehicles.opponents[r].field_0xb4 = 1;
                        break;
                    }
                    r++;
                    if (r >= opponents_left) {
                        r = 0;
                    }
                    if (r == rstart) {
                        break;
                    }
                }

            }
        } else {
            for (i = 0; i < GetCarCount(eVehicle_opponent); i++) {
                gProgram_state.AI_vehicles.opponents[i].field_0xb4 = 3;
            }
        }
    }

    gActive_car_list_rebuild_required = 1;
    RebuildActiveCarList();
}

// FUNCTION: CARMA2_HW 0x00401fe0
void C2_HOOK_FASTCALL CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength) {
    br_vector3 positive_y_vector;
    br_vector3 path_vector;

    BrVector3Sub(&path_vector, pFinish, pStart);
    BrVector3Set(&positive_y_vector, 0.f, 1.f, 0.f);
    BrVector3Cross(pNegative_x_vector, &path_vector, &positive_y_vector);
    BrVector3Normalise(pNegative_x_vector, pNegative_x_vector);
    BrVector3Scale(pNegative_x_vector, pNegative_x_vector, pLength);
}

// GLOBAL: CARMA2_HW 0x00589318
const br_scalar gFLOAT_00589318 = -0.15f;
// GLOBAL: CARMA2_HW 0x0058931c
const br_scalar gFLOAT_0058931c = -0.1f;
// GLOBAL: CARMA2_HW 0x00589320
const br_scalar gFLOAT_00589320 = 0.1f;

// C2PORT-NONMATCHING: best 39.27% | difftest PASS 2000 trials (spec 00401aa0_FUN_00401aa0.json) | reason VC5 x87 scheduling and register allocation
// FUNCTION: CARMA2_HW 0x00401aa0
void C2_HOOK_FASTCALL BuildDronePathNodeMesh(br_model* pModel, int pVert_index, int pFace_index, const br_vector3* pPosition, tU32 pMaterial1, tU32 pMaterial2, tU32 pMaterial3) {
    tU32 uVar2;
    tU32 uVar3;
    float fVar5;
    int iVar1;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    int iVar10;
    int vertex1;
    int vertex2;
    int vertex3;
    int vertex4;
    int vertex5;
    int vertex6;
    int vertex7;
    tS16 sVar4;
    float* pfVar11;
    br_face* faces;

    uVar2 = *(tU32*)&pPosition->v[0];
    uVar3 = *(tU32*)&pPosition->v[2];
    fVar5 = pPosition->v[1] - gFLOAT_00589318;
    vertex1 = pVert_index + 1;
    vertex2 = pVert_index + 2;
    vertex3 = pVert_index + 3;
    vertex4 = pVert_index + 4;
    vertex5 = pVert_index + 5;
    vertex6 = pVert_index + 6;
    vertex7 = pVert_index + 7;
    iVar6 = pVert_index * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar6) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar6 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar6 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar6);
    *pfVar11 = *pfVar11 - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar6 + 4) = *(float*)((tU8*)pModel->vertices + iVar6 + 4) - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar6 + 8) = *(float*)((tU8*)pModel->vertices + iVar6 + 8) - gFLOAT_0058931c;
    iVar6 = pVert_index + 1;
    iVar7 = iVar6 * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar7) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar7 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar7 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar7);
    *pfVar11 = *pfVar11 - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar7 + 4) = *(float*)((tU8*)pModel->vertices + iVar7 + 4) - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar7 + 8) = *(float*)((tU8*)pModel->vertices + iVar7 + 8) - gFLOAT_0058931c;
    iVar7 = pVert_index + 2;
    iVar8 = iVar7 * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar8) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar8 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar8);
    *pfVar11 = *pfVar11 - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = *(float*)((tU8*)pModel->vertices + iVar8 + 4) - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar8 + 8) = *(float*)((tU8*)pModel->vertices + iVar8 + 8) - gFLOAT_0058931c;
    iVar8 = (pVert_index + 3) * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar8) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar8 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar8);
    *pfVar11 = *pfVar11 - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = *(float*)((tU8*)pModel->vertices + iVar8 + 4) - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar8 + 8) = *(float*)((tU8*)pModel->vertices + iVar8 + 8) - gFLOAT_0058931c;
    iVar8 = (pVert_index + 4) * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar8) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar8 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar8);
    *pfVar11 = *pfVar11 - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar8 + 4) = *(float*)((tU8*)pModel->vertices + iVar8 + 4) - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar8 + 8) = *(float*)((tU8*)pModel->vertices + iVar8 + 8) - gFLOAT_00589320;
    iVar9 = pVert_index + 5;
    iVar9 = iVar9 * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar9) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar9 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar9 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar9);
    *pfVar11 = *pfVar11 - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar9 + 4) = *(float*)((tU8*)pModel->vertices + iVar9 + 4) - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar9 + 8) = *(float*)((tU8*)pModel->vertices + iVar9 + 8) - gFLOAT_00589320;
    iVar1 = pVert_index + 6;
    iVar9 = iVar1 * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar9) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar9 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar9 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar9);
    *pfVar11 = *pfVar11 - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar9 + 4) = *(float*)((tU8*)pModel->vertices + iVar9 + 4) - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar9 + 8) = *(float*)((tU8*)pModel->vertices + iVar9 + 8) - gFLOAT_00589320;
    iVar9 = pVert_index + 7;
    iVar10 = iVar9 * 0x28;
    *(tU32*)((tU8*)pModel->vertices + iVar10) = uVar2;
    *(float*)((tU8*)pModel->vertices + iVar10 + 4) = fVar5;
    *(tU32*)((tU8*)pModel->vertices + iVar10 + 8) = uVar3;
    pfVar11 = (float*)((tU8*)pModel->vertices + iVar10);
    *pfVar11 = *pfVar11 - gFLOAT_00589320;
    *(float*)((tU8*)pModel->vertices + iVar10 + 4) = *(float*)((tU8*)pModel->vertices + iVar10 + 4) - gFLOAT_0058931c;
    *(float*)((tU8*)pModel->vertices + iVar10 + 8) = *(float*)((tU8*)pModel->vertices + iVar10 + 8) - gFLOAT_00589320;
    sVar4 = (tS16)pVert_index;
    iVar10 = pFace_index * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4 + 3;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 2;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4 + 1;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial1;
    iVar10 = (pFace_index + 1) * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 3;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4 + 1;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial1;
    iVar10 = (pFace_index + 2) * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4 + 1;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 5;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4 + 4;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial2;
    iVar10 = (pFace_index + 3) * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4 + 1;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 4;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial2;
    iVar10 = (pFace_index + 4) * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 4;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4 + 3;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial3;
    iVar10 = (pFace_index + 5) * 0x28;
    *(tS16*)((tU8*)pModel->faces + iVar10) = sVar4 + 3;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 2) = sVar4 + 4;
    *(tS16*)((tU8*)pModel->faces + iVar10 + 4) = sVar4 + 7;
    *(tU16*)((tU8*)pModel->faces + iVar10 + 6) = 0xffff;
    *(tU32*)((tU8*)pModel->faces + iVar10 + 8) = pMaterial3;    FUN_00401f90((int)pModel, pFace_index + 6, vertex4, vertex5, vertex7, pMaterial1);
    FUN_00401f90((int)pModel, pFace_index + 7, vertex5, vertex6, vertex7, pMaterial1);
    FUN_00401f90((int)pModel, pFace_index + 8, vertex2, vertex7, vertex6, pMaterial2);
    FUN_00401f90((int)pModel, pFace_index + 9, vertex2, vertex3, vertex7, pMaterial2);
    FUN_00401f90((int)pModel, pFace_index + 10, vertex1, vertex2, vertex6, pMaterial3);
    FUN_00401f90((int)pModel, pFace_index + 11, vertex1, vertex6, vertex5, pMaterial3);
}
#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x00401f90
void C2_HOOK_FASTCALL FUN_00401f90(int pBase, int pIndex, tU16 pValue0, tU16 pValue1, tU16 pValue2, tU32 pValue3) {
    pIndex = pIndex * 0x28;
    *(tU16*)(*(int*)(pBase + 0xc) + pIndex) = pValue0;
    *(tU16*)(*(int*)(pBase + 0xc) + 2 + pIndex) = pValue1;
    *(tU16*)(*(int*)(pBase + 0xc) + 4 + pIndex) = pValue2;
    *(tU16*)(*(int*)(pBase + 0xc) + 6 + pIndex) = 0xffff;
    *(tU32*)(*(int*)(pBase + 0xc) + 8 + pIndex) = pValue3;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004a9550
void C2_HOOK_FASTCALL CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec) {

    BrVector3Sub(&pOpponent_spec->player_to_oppo_v, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &gProgram_state.current_car.car_master_actor->t.t.translate.t);
    pOpponent_spec->player_to_oppo_d = BrVector3Length(&pOpponent_spec->player_to_oppo_v);
    if (pOpponent_spec->player_to_oppo_d < gIn_view_distance) {
        pOpponent_spec->last_in_view = gTime_stamp_for_this_munging;
    }
}

// FUNCTION: CARMA2_HW 0x004a95f0
void C2_HOOK_FASTCALL SetChallengerIndexIf(int pIndex, int pSet) {

    if (pSet) {
        gChallenger_index__opponent = pIndex;
    }
}

// FUNCTION: CARMA2_HW 0x004a7960
void C2_HOOK_FASTCALL TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec) {
    pOpponent_spec->car_spec->car_master_actor->render_style = BR_RSTYLE_DEFAULT;
    PipeSingleOppoRenderage(pOpponent_spec, 1);
    if (!pOpponent_spec->physics_me) {
        pOpponent_spec->physics_me = 1;
        gActive_car_list_rebuild_required = 1;
    }
}

// FUNCTION: CARMA2_HW 0x004a79a0
void C2_HOOK_FASTCALL TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec) {
    tCar_spec* car = pOpponent_spec->car_spec;

    car->acc_force = 0.f;
    car->brake_force = 0.f;
    car->curvature = 0.f;
    car->gear = 0;
    car->revs = 0.f;
    BrVector3Set(&car->collision_info->omega, 0.f, 0.f, 0.f);
    BrVector3Set(&car->collision_info->v, 0.f, 0.f, 0.f);
    if (pOpponent_spec->physics_me) {
        pOpponent_spec->physics_me = 0;
        pOpponent_spec->car_spec->car_master_actor->render_style = BR_RSTYLE_NONE;
        PipeSingleOppoRenderage(pOpponent_spec, 0);
        gActive_car_list_rebuild_required = 1;
    }
}

// FUNCTION: CARMA2_HW 0x004af8d0
int C2_HOOK_FASTCALL TimeToStopStruggling(tOpponent_spec* pOpponent_spec) {

    return gTime_stamp_for_this_munging >
        (float)(750 * (pOpponent_spec->follow_path_data.number_of_struggles - 1))
            + 30.f * pOpponent_spec->car_spec->collision_info->M
            + (float)(pOpponent_spec->follow_path_data.struggle_time + 2750);
}

void C2_HOOK_FASTCALL DisplayOpponentRecoveringHeadup(tOpponent_spec* pOpponent_spec) {

    // empty
}

void C2_HOOK_FASTCALL OiStopCheating(tOpponent_spec* pOpponent_spec) {

    DoNotDprintf_opponent("%s: OiStopCheating() - End of cheating sesh", pOpponent_spec->car_spec->driver_name);
    pOpponent_spec->cheating = 0;
    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        DoNotDprintf_opponent ("%s: OiStopCheating() - Turning physics ON", pOpponent_spec->car_spec->driver_name);
        TurnOpponentPhysicsOn(pOpponent_spec);
        RebuildActiveCarList();
    }
}

void C2_HOOK_FASTCALL StartToCheat(tOpponent_spec* pOpponent_spec) {

    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        DoNotDprintf_opponent("%s: StartToCheat() - Turning physics OFF", pOpponent_spec->car_spec->driver_name);
        TurnOpponentPhysicsOff(pOpponent_spec);
        RebuildActiveCarList();
    } else {
        DoNotDprintf_opponent("%s: StartToCheat() - Starting to cheat", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->cheating = 1;
        switch (pOpponent_spec->current_objective) {
        case eOOT_levitate:
        case eOOT_knackered_and_freewheeling:
        case eOOT_none:
            break;
        default:
            if (!pOpponent_spec->car_spec->knackered
                    && ((pOpponent_spec->car_spec->collision_info->last_special_volume != NULL && pOpponent_spec->car_spec->collision_info->last_special_volume->gravity_multiplier < 1.f)
                        || !pOpponent_spec->follow_path_data.has_moved_during_this_task
                        || (pOpponent_spec->follow_path_data.struggle_time != 0 && !TimeToStopStruggling(pOpponent_spec)))) {

                DisplayOpponentRecoveringHeadup(pOpponent_spec);
            }
            break;
        }

        switch (pOpponent_spec->current_objective) {
        case eOOT_complete_race:
        case eOOT_pursue_and_twat:
        case eOOT_run_away:
        case eOOT_get_near_player:
        case eOOT_return_to_start:
            if (pOpponent_spec->follow_path_data.section_no < 15000
                    || pOpponent_spec->follow_path_data.section_no >= 20000) {
                int section = pOpponent_spec->follow_path_data.section_no - 20000;
                if (section >= 0 && section < pOpponent_spec->nnext_sections - 1) {
                    br_vector3 direction_v;
                    br_vector3 intersect;
                    br_scalar distance;

                    tS16 nearest_section = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
                    tS16 real_section = GetOpponentsRealSection(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);

                    if (nearest_section != real_section) {
                        int i;

                        for (i = section; i < pOpponent_spec->nnext_sections; i++) {
                            if (pOpponent_spec->next_sections[i].section_no == nearest_section) {
                                break;
                            }
                        }
                        if (i < pOpponent_spec->nnext_sections) {
                            ShiftOpponentsProjectedRoute(pOpponent_spec, i);
                            pOpponent_spec->follow_path_data.section_no = 20000;
                        } else {
                            br_vector3 dir;

                            pOpponent_spec->follow_path_data.section_no = 20000;
                            pOpponent_spec->nnext_sections = 1;
                            pOpponent_spec->next_sections[0].section_no = nearest_section;
                            BrVector3Sub(&dir, &intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                            pOpponent_spec->next_sections[0].direction = BrVector3Dot(&dir, (br_vector3*)pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[2]) >= 0;
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x004aa570
void C2_HOOK_FASTCALL ProcessThisOpponent(tOpponent_spec* pOpponent_spec) {

    if (!((gMap_view == 2 && gShow_opponents) || gTime_stamp_for_this_munging <= pOpponent_spec->last_in_view + 3000)) {
        if (!pOpponent_spec->cheating) {
            if (((unsigned int)(tU16)pOpponent_spec->car_spec->car_ID & 0xff00) != 0x300) {
                DoNotDprintf_opponent("%s: StartToCheat() - Starting to cheat", pOpponent_spec->car_spec->driver_name);
                pOpponent_spec->cheating = 1;
                if (pOpponent_spec->current_objective != eOOT_levitate
                        && pOpponent_spec->current_objective != eOOT_knackered_and_freewheeling
                        && pOpponent_spec->current_objective != eOOT_none
                        && !pOpponent_spec->car_spec->knackered
                        && (pOpponent_spec->car_spec->collision_info->last_special_volume == NULL
                            || pOpponent_spec->car_spec->collision_info->last_special_volume->gravity_multiplier >= gOppo_pursuit_one)
                        && pOpponent_spec->follow_path_data.has_moved_during_this_task
                        && pOpponent_spec->follow_path_data.struggle_time != 0) {
                    TimeToStopStruggling(pOpponent_spec);
                }

                if (pOpponent_spec->current_objective == eOOT_complete_race
                        || pOpponent_spec->current_objective == eOOT_pursue_and_twat
                        || pOpponent_spec->current_objective == eOOT_run_away
                        || pOpponent_spec->current_objective == eOOT_get_near_player
                        || pOpponent_spec->current_objective == eOOT_return_to_start) {
                    if (pOpponent_spec->follow_path_data.section_no < 15000
                            || pOpponent_spec->follow_path_data.section_no >= 20000) {
                        int section = pOpponent_spec->follow_path_data.section_no - 20000;
                        if (section >= 0 && section < pOpponent_spec->nnext_sections - 1) {
                            br_vector3 direction_v;
                            br_vector3 intersect;
                            br_scalar distance;

                            tS16 nearest_section = FindNearestGeneralSection(NULL, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
                            int real_section = GetOpponentsRealSection(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);

                            if (nearest_section != real_section) {
                                for (; section < pOpponent_spec->nnext_sections; section++) {
                                    if (pOpponent_spec->next_sections[section].section_no == nearest_section) {
                                        break;
                                    }
                                }
                                if (section < pOpponent_spec->nnext_sections) {
                                    ShiftOpponentsProjectedRoute(pOpponent_spec, section);
                                    pOpponent_spec->follow_path_data.section_no = 20000;
                                } else {
                                    br_vector3 dir;

                                    pOpponent_spec->follow_path_data.section_no = 20000;
                                    pOpponent_spec->nnext_sections = 1;
                                    pOpponent_spec->next_sections[0].section_no = nearest_section;
                                    BrVector3Sub(&dir, &intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                                    pOpponent_spec->next_sections[0].direction = !(BrVector3Dot(&dir, (br_vector3*)pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[2]) < gOppo_render_zero);
                                }
                            }
                        }
                    }
                }
            } else {
                DoNotDprintf_opponent("%s: StartToCheat() - Turning physics OFF", pOpponent_spec->car_spec->driver_name);
                TurnOpponentPhysicsOff(pOpponent_spec);
                RebuildActiveCarList();
            }
        }
    } else {
        if (pOpponent_spec->cheating) {
            DoNotDprintf_opponent("%s: OiStopCheating() - End of cheating sesh", pOpponent_spec->car_spec->driver_name);
            pOpponent_spec->cheating = 0;
            if (((unsigned int)(tU16)pOpponent_spec->car_spec->car_ID & 0xff00) == 0x300) {
                DoNotDprintf_opponent ("%s: OiStopCheating() - Turning physics ON", pOpponent_spec->car_spec->driver_name);
                pOpponent_spec->car_spec->car_master_actor->render_style = BR_RSTYLE_DEFAULT;
                PipeSingleOppoRenderage(pOpponent_spec, 1);
                if (!pOpponent_spec->physics_me) {
                    pOpponent_spec->physics_me = 1;
                    gActive_car_list_rebuild_required = 1;
                }
                RebuildActiveCarList();
            }
        }
    }
    PossiblyRepairOpponent(pOpponent_spec);
    ChooseNewObjective(pOpponent_spec, pOpponent_spec->new_objective_required || gTime_stamp_for_this_munging > pOpponent_spec->time_for_this_objective_to_finish);
    pOpponent_spec->new_objective_required = 0;
    if (gCountdown != 0 || gRace_finished) {
        pOpponent_spec->car_spec->brake_force = DAT_0058a4ec * pOpponent_spec->car_spec->collision_info->M;
    }
    if (!pOpponent_spec->finished_for_this_race && !gStop_opponents_moving && !gRace_finished && gTime_stamp_for_this_munging > pOpponent_spec->stun_time_ends) {
        ProcessCurrentObjective(pOpponent_spec, ePOC_run);
    }
    if (pOpponent_spec->cheating) {
        BrVector3Copy(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
    }
}
#pragma inline_depth()

// FUNCTION: CARMA2_HW 0x004aa490
void C2_HOOK_FASTCALL StartRecordingTrail(tCar_spec* pPursuee) {

    if (pPursuee->no_of_processes_recording_my_trail == 0) {
        DoNotDprintf_opponent("StartRecordingTrail - starting from scratch");
        pPursuee->no_of_processes_recording_my_trail = 1;
        pPursuee->my_trail.nodes_shifted_this_frame = 0;
        pPursuee->my_trail.has_deviated_recently = 0;
        pPursuee->my_trail.number_of_nodes = 2;
        pPursuee->my_trail.time_of_next_recording = gTime_stamp_for_this_munging + 500;
        BrVector3Copy(&pPursuee->my_trail.base_heading, &pPursuee->direction);
        BrVector3Copy(&pPursuee->my_trail.trail_nodes[0], &pPursuee->car_master_actor->t.t.translate.t);
        BrVector3Copy(&pPursuee->my_trail.trail_nodes[1], &pPursuee->car_master_actor->t.t.translate.t);
        pPursuee->my_trail.trail_nodes[0].v[2] += 0.2f;
    } else {
        DoNotDprintf_opponent("StartRecordingTrail - another pursuer attaching");
        pPursuee->no_of_processes_recording_my_trail += 1;
    }
}

void C2_HOOK_FASTCALL RecordNextTrailNode(tCar_spec* pPursuee) {
    tPursuee_trail* trail;
    br_scalar length;
    br_vector3 car_to_last_point_v;
    br_vector3 offset_v;
    br_vector3 start1;
    br_vector3 finish1;
    br_vector3 start2;
    br_vector3 finish2;
    int visible;

    trail = &pPursuee->my_trail;
    trail->nodes_shifted_this_frame = 0;
    if (trail->time_of_next_recording >= gTime_stamp_for_this_munging) {
        return;
    }
    trail->time_of_next_recording = gTime_stamp_for_this_munging + 500;
    if (BrVector3Dot(&pPursuee->direction, &trail->base_heading) < FastScalarCos(30)) {
        trail->has_deviated_recently = 1;
    }
    BrVector3Sub(&car_to_last_point_v, &trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t);
    length = BrVector3Length(&car_to_last_point_v);
    if (length < .3f) {
        return;
    }
    CalcNegativeXVector(&offset_v, &trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t, .5f);

    BrVector3Add(&start1, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
    BrVector3Add(&finish1, &pPursuee->car_master_actor->t.t.translate.t, &offset_v);
    BrVector3Sub(&start2, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
    BrVector3Sub(&finish2, &pPursuee->car_master_actor->t.t.translate.t, &offset_v);
    visible = 1;
    if (!pPursuee->my_trail.has_deviated_recently) {
        visible = PointVisibleFromHere(&start1, &finish1);
        if (!visible) {
            visible = PointVisibleFromHere(&start2, &finish2);
        }
        if (!visible) {
            visible = PointVisibleFromHere(&trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t);
        }
    }

    C2_HOOK_BUG_ON(sizeof(pPursuee->my_trail.trail_nodes) != 0x12c);

    if (pPursuee->my_trail.has_deviated_recently || !visible) {
        if ((visible && length > 2.0f) || (!visible && length > 1.5f)) {
            if (pPursuee->my_trail.number_of_nodes < CARPOCALYPSE2_ASIZE(pPursuee->my_trail.trail_nodes)) {
                pPursuee->my_trail.number_of_nodes += 1;
            } else {
                memmove(&pPursuee->my_trail.trail_nodes[0], &pPursuee->my_trail.trail_nodes[1], sizeof(pPursuee->my_trail.trail_nodes[0]));
                pPursuee->my_trail.nodes_shifted_this_frame = 1;
            }
            pPursuee->my_trail.has_deviated_recently = 0;
            BrVector3Copy(&trail->base_heading, &pPursuee->direction);
        }
    }
    BrVector3Copy(&trail->trail_nodes[trail->number_of_nodes - 1], &pPursuee->car_master_actor->t.t.translate.t);
}

void C2_HOOK_FASTCALL ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->car_spec->big_bang = 0;
    pOpponent_spec->car_spec->scary_bang = 0;
    pOpponent_spec->car_spec->grudge_raised_recently = 0;
    pOpponent_spec->car_spec->last_person_to_hit_us = NULL;
    pOpponent_spec->car_spec->last_person_we_hit = NULL;
}

// GLOBAL: CARMA2_HW 0x0058a4fc
static const float gTrail_minimum_distance = .3f;

// GLOBAL: CARMA2_HW 0x0058a500
static const float gTrail_obstructed_distance = 1.5f;

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x004a9e10
void C2_HOOK_FASTCALL MungeOpponents(void) {
    int i;
    int un_stun_flag;

    un_stun_flag = 0;
    if (gProgram_state.AI_vehicles.number_of_opponents == 0 && gNumber_of_cops_before_faffage == 0) {
        return;
    }
    if (gProgram_state.AI_vehicles.number_of_path_nodes == 0) {
        return;
    }
    if (gProgram_state.AI_vehicles.number_of_path_sections == 0) {
        return;
    }
    gAcme_frame_count += 1;
    if (!gAcknowledged_start && gCountdown == 0) {
        gAcknowledged_start = 1;
        if (!gStart_jumped) {
            un_stun_flag = 1;
        }
    }
    if (gProgram_state.current_car.no_of_processes_recording_my_trail != 0) {
        tPursuee_trail* trail;
        br_scalar length;
        br_vector3 car_to_last_point_v;
        br_vector3 offset_v;
        br_vector3 start1;
        br_vector3 finish1;
        br_vector3 start2;
        br_vector3 finish2;
        int visible;

        visible = 1;
        trail = &gProgram_state.current_car.my_trail;
        trail->nodes_shifted_this_frame = 0;
        if (gTime_stamp_for_this_munging <= trail->time_of_next_recording) {
            goto trail_updated;
        }
        trail->time_of_next_recording = gTime_stamp_for_this_munging + 500;
        if (BR_MAC3(trail->base_heading.v[2], gProgram_state.current_car.direction.v[2], trail->base_heading.v[1], gProgram_state.current_car.direction.v[1], trail->base_heading.v[0], gProgram_state.current_car.direction.v[0]) < FastScalarCos(30)) {
            trail->has_deviated_recently = 1;
        }
        BrVector3Sub(&car_to_last_point_v, &trail->trail_nodes[trail->number_of_nodes - 2], &gProgram_state.current_car.car_master_actor->t.t.translate.t);
        length = BrVector3Length(&car_to_last_point_v);
        if (length < gTrail_minimum_distance) {
            goto trail_updated;
        }
        CalcNegativeXVector(&offset_v, &trail->trail_nodes[trail->number_of_nodes - 2], &gProgram_state.current_car.car_master_actor->t.t.translate.t, .5f);

        BrVector3Add(&start1, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
        BrVector3Add(&finish1, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &offset_v);
        BrVector3Sub(&start2, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
        BrVector3Sub(&finish2, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &offset_v);
        if (!gProgram_state.current_car.my_trail.has_deviated_recently) {
            visible = PointVisibleFromHere(&start1, &finish1);
            if (visible) {
                visible = PointVisibleFromHere(&start2, &finish2);
            }
            if (visible) {
                visible = PointVisibleFromHere(&trail->trail_nodes[trail->number_of_nodes - 2], &gProgram_state.current_car.car_master_actor->t.t.translate.t);
            }
            if (visible) {
                goto trail_endpoint;
            }
        }

        C2_HOOK_BUG_ON(sizeof(gProgram_state.current_car.my_trail.trail_nodes) != 0x12c);

        if ((visible && length > gOppo_pursuit_minimum_velocity_ratio) || (!visible && length > gTrail_obstructed_distance)) {
            if (gProgram_state.current_car.my_trail.number_of_nodes < CARPOCALYPSE2_ASIZE(gProgram_state.current_car.my_trail.trail_nodes)) {
                gProgram_state.current_car.my_trail.number_of_nodes += 1;
            } else {
                memmove(&gProgram_state.current_car.my_trail.trail_nodes[0], &gProgram_state.current_car.my_trail.trail_nodes[1], sizeof(gProgram_state.current_car.my_trail.trail_nodes) - sizeof(gProgram_state.current_car.my_trail.trail_nodes[0]));
                gProgram_state.current_car.my_trail.nodes_shifted_this_frame = 1;
            }
            gProgram_state.current_car.my_trail.has_deviated_recently = 0;
            BrVector3Copy(&trail->base_heading, &gProgram_state.current_car.direction);
        }
trail_endpoint:
        BrVector3Copy(&trail->trail_nodes[trail->number_of_nodes - 1], &gProgram_state.current_car.car_master_actor->t.t.translate.t);
    } else {
        StartRecordingTrail(&gProgram_state.current_car);
    }
trail_updated:
    gNum_of_opponents_pursuing = 0;
    gNum_of_opponents_getting_near = 0;
    gNum_of_opponents_completing_race = 0;
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        tOpponent_spec* oppo = &gProgram_state.AI_vehicles.opponents[i];

        if (!oppo->finished_for_this_race) {
            if (oppo->current_objective == eOOT_pursue_and_twat) {
                gNum_of_opponents_pursuing += 1;
            } else if (oppo->current_objective == eOOT_get_near_player) {
                gNum_of_opponents_getting_near += 1;
            } else if (oppo->current_objective == eOOT_complete_race) {
                gNum_of_opponents_completing_race += 1;
            }
        }
    }
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        tOpponent_spec* oppo = &gProgram_state.AI_vehicles.opponents[i];

        if (!oppo->finished_for_this_race) {
            if (un_stun_flag) {
                oppo->stun_time_ends = 0;
            }
            CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(oppo);
            CalcPlayerConspicuousness(oppo);
            ProcessThisOpponent(oppo);
            oppo->car_spec->big_bang = 0;
            oppo->car_spec->scary_bang = 0;
            oppo->car_spec->grudge_raised_recently = 0;
            oppo->car_spec->last_person_to_hit_us = NULL;
            oppo->car_spec->last_person_we_hit = NULL;
        }
    }
    for (i = 0; i < gNumber_of_cops_before_faffage; i++) {
        tOpponent_spec* oppo = &gProgram_state.AI_vehicles.cops[i];

        if (!oppo->finished_for_this_race) {
            if (un_stun_flag) {
                oppo->stun_time_ends = 0;
            }
            CalcOpponentDistanceFromHome(oppo);
            CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(oppo);
            CalcPlayerConspicuousness(oppo);
            ProcessThisOpponent(oppo);
            oppo->car_spec->big_bang = 0;
            oppo->car_spec->scary_bang = 0;
            oppo->car_spec->grudge_raised_recently = 0;
            oppo->car_spec->last_person_to_hit_us = NULL;
            oppo->car_spec->last_person_we_hit = NULL;
            oppo->murder_reported = 0;
        }
    }
    if (gTime_stamp_for_this_munging > gNext_grudge_reduction) {
        gNext_grudge_reduction = gTime_stamp_for_this_munging + 5000;

        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            tOpponent_spec* oppo = &gProgram_state.AI_vehicles.opponents[i];

            if (!oppo->finished_for_this_race) {
                tU8* grudge = &gOpponents[oppo->index].psyche.grudge_against_player;

                *grudge -= MIN(gGrudge_reduction_per_period, (int)*grudge);
            }
        }
    }
    RebuildActiveCarList();
    gFirst_frame = 0;
}

#pragma inline_depth()

void C2_HOOK_FASTCALL UnStunTheBugger(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->stun_time_ends = 0;
}

// FUNCTION: CARMA2_HW 0x004ae620
void C2_HOOK_FASTCALL WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was) {
    int i;

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        UnStunTheBugger(&gProgram_state.AI_vehicles.opponents[i]);
        if (IRandomBetween(1000, 2500) < 1000 * pWhat_the_countdown_was) {
            StunTheBugger(&gProgram_state.AI_vehicles.opponents[i], IRandomBetween(1000, 2500));
        } else {
            StunTheBugger(&gProgram_state.AI_vehicles.opponents[i], 1000 * pWhat_the_countdown_was);
        }
    }
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
        UnStunTheBugger(&gProgram_state.AI_vehicles.cops[i]);
        if (IRandomBetween(1000, 2500) < 1000 * pWhat_the_countdown_was) {
            StunTheBugger(&gProgram_state.AI_vehicles.cops[i], IRandomBetween(1000, 2500));
        } else {
            StunTheBugger(&gProgram_state.AI_vehicles.cops[i], 1000 * pWhat_the_countdown_was);
        }
    }
    gAcknowledged_start = 1;
    gStart_jumped = 1;
}

tS16 C2_HOOK_FASTCALL FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {

    return FindNearestGeneralSection(NULL, pActor_coords, pPath_direction, pIntersect, pDistance);
}

void C2_HOOK_FASTCALL WeightedFindNearestNodeAndSection(tCar_spec* pCar, br_vector3* pActor_coords, tS16* pNearest_node_section_no, tS16* pNearest_node, br_scalar* pT, float pWeight) {
    int section_no;
    int no_sections;
    tS16 nearest_section;
    br_scalar nearest_node_distance_squared;
    br_scalar closest_distance_squared;
    br_scalar the_distance_squared;
    br_scalar t;
    br_scalar length_squared_a;
    br_vector3 a;
    br_vector3 p;

#ifdef CARPOCALYPSE2_FIX_BUGS
    nearest_section = -1;
#endif

    *pNearest_node_section_no = -1;
    *pNearest_node = -1;
    nearest_node_distance_squared = BR_SCALAR_MAX;
    closest_distance_squared = BR_SCALAR_MAX;
    if (pCar != NULL) {
        no_sections = pCar->my_trail.number_of_nodes - 1;
    } else {
        no_sections = gProgram_state.AI_vehicles.number_of_path_sections;
    }
    for (section_no = 0; section_no < no_sections; section_no++) {
        br_vector3* start;
        br_vector3* finish;

        if (pCar != NULL) {
            start = &pCar->my_trail.trail_nodes[section_no + 0];
            finish = &pCar->my_trail.trail_nodes[section_no + 1];
        } else {
            start = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].pos;
            finish = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].pos;
        }
        BrVector3Sub(&a, finish, start);
        a.v[1] *= pWeight;
        BrVector3Sub(&p, pActor_coords, start);
        p.v[1] *= pWeight;
        the_distance_squared = Vector3DistanceSquared(&p, &a);
        if (the_distance_squared < closest_distance_squared) {
            if (pCar != NULL) {
                *pNearest_node = section_no;
            } else {
                *pNearest_node = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1];
            }
            nearest_section = section_no + 0;
            closest_distance_squared = the_distance_squared;
        }
        the_distance_squared = BrVector3LengthSquared(&p);
        if (the_distance_squared < closest_distance_squared) {
            if (pCar != NULL) {
                *pNearest_node = section_no + 1;
            } else {
                *pNearest_node = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0];
            }
            nearest_section = section_no;
            closest_distance_squared = the_distance_squared;
        }
        length_squared_a = BrVector3LengthSquared(&a);
        if (length_squared_a >= 0.0001f) {
            t = BrVector3Dot(&p, &a) / length_squared_a;
            if (t >= 0 && t <= 1.f) {
                *pNearest_node_section_no = section_no;
                *pT = t;
                nearest_node_distance_squared = the_distance_squared;
            }
        }
    }
    if (nearest_node_distance_squared <= closest_distance_squared) {
        *pNearest_node = -1;
    } else {
        *pNearest_node_section_no = nearest_section;
    }
}

// FUNCTION: CARMA2_HW 0x004a82d0
tS16 C2_HOOK_FASTCALL FindNearestGeneralSection(tCar_spec* pCar, br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {
    int section_no;
    int no_sections;
    int nearest_section;
    int nearest_node;
    int nearest_node_section;
    br_scalar nearest_node_distance_squared;
    br_scalar closest_distance_squared;
    br_scalar the_distance_squared;
    br_scalar t;
    br_scalar nearest_t;
    br_scalar scale;
    br_scalar length_squared_a;
    br_vector3 a;
    br_vector3 p;
    br_vector3 offset;
    br_vector3 intersect;
    br_vector3* start;
    br_vector3* finish;

    nearest_section = -1;
    nearest_node = -1;
    nearest_node_distance_squared = BR_SCALAR_MAX;
    closest_distance_squared = BR_SCALAR_MAX;
    if (pCar != NULL) {
        no_sections = pCar->my_trail.number_of_nodes - 1;
    } else {
        no_sections = gProgram_state.AI_vehicles.number_of_path_sections;
    }
    for (section_no = 0; section_no < no_sections; section_no++) {
        if (pCar != NULL) {
            start = &pCar->my_trail.trail_nodes[section_no];
            finish = &pCar->my_trail.trail_nodes[section_no + 1];
        } else {
            start = &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].pos;
            finish = &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].pos;
        }
        BrVector3Sub(&a, finish, start);
        a.v[1] *= 2.f;
        BrVector3Sub(&p, pActor_coords, start);
        p.v[1] *= 2.f;
        BrVector3Sub(&offset, &p, &a);
        the_distance_squared = BrVector3LengthSquared(&offset);
        if (the_distance_squared < nearest_node_distance_squared) {
            nearest_node_distance_squared = the_distance_squared;
            /* The original associates the finish distance with this trail index. */
            nearest_node = pCar != NULL ? section_no : (tS16)gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1];
            nearest_node_section = section_no;
        }
        the_distance_squared = BrVector3LengthSquared(&p);
        if (the_distance_squared < nearest_node_distance_squared) {
            nearest_node_distance_squared = the_distance_squared;
            nearest_node = pCar != NULL ? section_no + 1 : (tS16)gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0];
            nearest_node_section = section_no;
        }
        length_squared_a = BrVector3LengthSquared(&a);
        if (length_squared_a >= 0.0001f) {
            t = BrVector3Dot(&p, &a) / length_squared_a;
            if (t >= gOppo_render_zero && t <= gOppo_pursuit_one) {
                BrVector3Scale(&intersect, &a, t);
                BrVector3Sub(&offset, &p, &intersect);
                the_distance_squared = BrVector3LengthSquared(&offset);
                if (the_distance_squared < closest_distance_squared) {
                    nearest_section = section_no;
                    closest_distance_squared = the_distance_squared;
                    nearest_t = t;
                }
            }
        }
    }
    if (nearest_node_distance_squared < closest_distance_squared) {
        nearest_section = nearest_node_section;
    } else {
        nearest_node = -1;
    }
    if (pCar != NULL) {
        BrVector3Sub(&a,
            &pCar->my_trail.trail_nodes[nearest_section + 1],
            &pCar->my_trail.trail_nodes[nearest_section]);
    } else {
        BrVector3Sub(&a,
            &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[nearest_section].node_indices[1]].pos,
            &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[nearest_section].node_indices[0]].pos);
    }
    if (nearest_node >= 0) {
        BrVector3Copy(pIntersect, pCar != NULL ? &pCar->my_trail.trail_nodes[nearest_node] : &gProgram_state.AI_vehicles.path_nodes[nearest_node].pos);
    } else {
        BrVector3Scale(pIntersect, &a, nearest_t);
        /* The original uses the general path start here even for a car trail. */
        BrVector3Accumulate(pIntersect, &gProgram_state.AI_vehicles.path_nodes[(tS16)gProgram_state.AI_vehicles.path_sections[nearest_section].node_indices[0]].pos);
    }
    scale = (br_scalar)(DAT_0058a4a0 / BrVector3Length(&a));
    BrVector3Scale(pPath_direction, &a, scale);
    BrVector3Sub(&offset, pActor_coords, pIntersect);
    *pDistance = BrVector3Length(&offset);
    if (pCar != NULL) {
        nearest_section += 15000;
    }
    return nearest_section;
}

// FUNCTION: CARMA2_HW 0x004ae9e0
int C2_HOOK_FASTCALL GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no) {

    if (pSection_no >= 20000) {
        return pOpponent_spec->next_sections[pSection_no - 20000].section_no;
    } else if (pSection_no >= 15000) {
        return pSection_no - 15000;
    } else {
        return pSection_no;
    }
}

// FUNCTION: CARMA2_HW 0x004ab100
int C2_HOOK_FASTCALL ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces) {
    size_t i;

    if (pOpponent_spec->nnext_sections <= pPlaces) {
        return 0;
    }
    for (i = 0; i < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - pPlaces; i++) {
        pOpponent_spec->next_sections[i].section_no = pOpponent_spec->next_sections[pPlaces + i].section_no;
        pOpponent_spec->next_sections[i].direction = pOpponent_spec->next_sections[pPlaces + i].direction;
    }
    pOpponent_spec->nnext_sections -= pPlaces;
    return 1;
}

int C2_HOOK_FASTCALL AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee) {

    return pOpponent_spec->current_objective == eOOT_pursue_and_twat && pOpponent_spec->pursue_car_data.pursuee == pPursuee;
}

int C2_HOOK_FASTCALL LastTwatterAPlayer(tOpponent_spec* pOpponent_spec) {

    return pOpponent_spec->car_spec->last_person_to_hit_us != NULL && pOpponent_spec->car_spec->last_person_to_hit_us->driver == eDriver_local_human;
}

int C2_HOOK_FASTCALL LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec) {

    return pOpponent_spec->car_spec->last_person_we_hit != NULL && pOpponent_spec->car_spec->last_person_we_hit->driver == eDriver_local_human;
}

int C2_HOOK_FASTCALL HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec) {
    br_vector3 oppo_to_player_norm;

    BrVector3Sub(&oppo_to_player_norm,
        &gProgram_state.current_car.car_master_actor->t.t.translate.t,
        &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
    BrVector3Normalise(&oppo_to_player_norm, &oppo_to_player_norm);
    if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &oppo_to_player_norm) > gHead_on_cos_value
            && BrVector3Dot(&gProgram_state.current_car.direction, &oppo_to_player_norm) < -gHead_on_cos_value) {
        DoNotDprintf_opponent("HOORAY! Head-on imminent");
        return 1;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004ace70
void C2_HOOK_FASTCALL ChooseNewObjective(tOpponent_spec* pOpponent_spec, int pMust_choose_one) {
    char str[256];

    if (pOpponent_spec->current_objective == eOOT_knackered_and_freewheeling || pOpponent_spec->knackeredness_detected) {
        return;
    }
    if (gTime_stamp_for_this_munging > pOpponent_spec->next_out_of_world_check) {
        pOpponent_spec->next_out_of_world_check = gTime_stamp_for_this_munging + 500;
        if (HasCarFallenOffWorld(pOpponent_spec->car_spec)) {
            DisplayOpponentRecoveringHeadup(pOpponent_spec);
            TeleportOpponentToNearestSafeLocation(pOpponent_spec);
            NewObjective(pOpponent_spec, eOOT_get_near_player);
            return;
        }
    }
    if (pOpponent_spec->car_spec->knackered && !pOpponent_spec->knackeredness_detected) {
        pOpponent_spec->knackeredness_detected = 1;
        DoNotDprintf_opponent("%s: Knackered - dealing with appropriately", pOpponent_spec->car_spec->driver_name);
        if (pOpponent_spec->car_spec->has_been_stolen) {
            NewObjective(pOpponent_spec, eOOT_levitate);
        } else {
            NewObjective(pOpponent_spec, eOOT_knackered_and_freewheeling);
        }
        return;
    }

    if (pOpponent_spec->current_objective == eOOT_frozen) {
        if (GET_CAR_SPEED_FACTOR(pOpponent_spec->car_spec) == 0.f) {
            return;
        }
        DoNotDprintf_opponent("%s: Time to unfreeze", pOpponent_spec->car_spec->driver_name);
        if (pOpponent_spec->pursuing_player_before_freeze) {
            NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
        } else {
            NewObjective(pOpponent_spec, eOOT_get_near_player);
        }
        return;
    }
    if (GET_CAR_SPEED_FACTOR(pOpponent_spec->car_spec) == 0.f) {
        DoNotDprintf_opponent("%s: Decided to freeze", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->pursuing_player_before_freeze = pOpponent_spec->current_objective == eOOT_pursue_and_twat || pOpponent_spec->pursue_car_data.pursuee == &gProgram_state.current_car;
        NewObjective(pOpponent_spec, eOOT_frozen);
        return;
    }
    if (!gFirst_frame) {
        int general_grudge_increase;

        general_grudge_increase = (int)(pOpponent_spec->nastiness * 40.0f + 10.0f);
        if (pOpponent_spec->car_spec->scary_bang && pOpponent_spec->player_to_oppo_d < 10.f) {
            if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                if (PercentageChance(20)) {
                    DoNotDprintf_opponent("%s: Decided to run away", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_run_away);
                    return;
                }
            } else if (gOpponents[pOpponent_spec->index].strength_rating < 5) {
                if (PercentageChance((int)((pOpponent_spec->current_objective == eOOT_pursue_and_twat ? 100 : 60) - 50.f * pOpponent_spec->nastiness))) {
                    DoNotDprintf_opponent("%s: Decided to run away", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_run_away);
                    return;
                }
            }
        }

        if (pOpponent_spec->current_objective != eOOT_run_away || gTime_stamp_for_this_munging >= pOpponent_spec->time_this_objective_started + 20000) {
            if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) && pOpponent_spec->murder_reported && pOpponent_spec->player_to_oppo_d < 20.f && !AlreadyPursuingCar(pOpponent_spec, &gProgram_state.current_car)) {
                gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                sprintf(str, "%s: Furderous melon!", pOpponent_spec->car_spec->driver_name);
                DoNotDprintf_opponent("%s: Decided to pursue after MURDER", pOpponent_spec->car_spec->driver_name);
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                return;
            }
            if (pOpponent_spec->car_spec->big_bang
                    && LastTwatterAPlayer(pOpponent_spec)
                    && !AlreadyPursuingCar(pOpponent_spec, pOpponent_spec->car_spec->last_person_to_hit_us)) {

                gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                sprintf(str, "%s: Christ! What was that?", pOpponent_spec->car_spec->driver_name);
                DoNotDprintf_opponent("%s: Decided to pursue after big bang; last person to twat us was %s",
                    pOpponent_spec->car_spec->driver_name,
                    pOpponent_spec->car_spec->last_person_to_hit_us->driver_name);
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, pOpponent_spec->car_spec->last_person_to_hit_us);
                return;
            }
            if (LastTwatteeAPlayer(pOpponent_spec) && !AlreadyPursuingCar(pOpponent_spec, pOpponent_spec->car_spec->last_person_we_hit)) {

                gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                sprintf(str, "%s: Ha! Bet you weren't expecting that!", pOpponent_spec->car_spec->driver_name);
                DoNotDprintf_opponent("%s: Decided to pursue %s after accidentally hitting them",
                    pOpponent_spec->car_spec->driver_name,
                    pOpponent_spec->car_spec->last_person_we_hit->driver_name);
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                return;
            }
            if (!AlreadyPursuingCar(pOpponent_spec, &gProgram_state.current_car)) {
                if (pOpponent_spec->car_spec->grudge_raised_recently
                        && (!CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) || pOpponent_spec->player_to_oppo_d <= 20.f)
                        && LastTwatterAPlayer(pOpponent_spec)) {
                    int grudge = gOpponents[pOpponent_spec->index].psyche.grudge_against_player;
                    if (grudge > 20) {
                        gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(grudge + general_grudge_increase, 100);
                        sprintf(str, "%s: Right! That's enough, %s!", pOpponent_spec->car_spec->driver_name, gProgram_state.current_car.driver_name);
                        DoNotDprintf_opponent("%s: Decided to pursue after grudginess raised; last person to twat us was %s",
                            pOpponent_spec->car_spec->driver_name, pOpponent_spec->car_spec->last_person_to_hit_us->driver_name);
                        NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                        return;
                    }
                }
                if (pOpponent_spec->player_in_view_now && !pOpponent_spec->acknowledged_piv) {
                    int pursuit_percentage;
                    int do_it;

                    pOpponent_spec->acknowledged_piv = 1;
                    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                        pursuit_percentage = (int)((BrVector3Length(&gProgram_state.current_car.collision_info->v) * WORLD_SCALE - gDefinite_no_cop_pursuit_speed) * gCop_pursuit_speed_percentage_multiplier);
                    } else if (gProgram_state.skill_level + 3 > gNum_of_opponents_pursuing) {
                        pursuit_percentage = (int)((float)(gOpponents[pOpponent_spec->index].psyche.grudge_against_player - 20) + 30.f * pOpponent_spec->nastiness);
                    } else {
                        pursuit_percentage = 0;
                    }

                    pursuit_percentage += HeadOnWithPlayerPossible(pOpponent_spec) ? 50 : 0;
                    do_it = PercentageChance(pursuit_percentage);
                    DoNotDprintf_opponent("%s: Spotted player; chance of pursuing %d%%: %s", pOpponent_spec->car_spec->driver_name, pursuit_percentage, do_it ? "YES, Decided to pursue" : "NO, Decided NOT to pursue");
                    if (do_it) {
                        gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                        sprintf(str, "%s: I've decided to kill you for the fun of it", pOpponent_spec->car_spec->driver_name);
                        NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                        return;
                    }
                }
            }
        }
    }
    if (pMust_choose_one) {
        DoNotDprintf_opponent("%s: Choosing new objective because we have to...", pOpponent_spec->car_spec->driver_name);
        if (pOpponent_spec->has_moved_at_some_point) {
            int pursuit_percentage;

            if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                NewObjective(pOpponent_spec, eOOT_return_to_start);
                return;
            }
            switch (pOpponent_spec->field_0xb4) {
            case 1:
                NewObjective(pOpponent_spec, eOOT_complete_race);
                break;
            case 2:
                NewObjective(pOpponent_spec, eOOT_get_near_player);
                break;
            case 3:
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                break;
            default:
                pursuit_percentage = (int)pOpponent_spec->player_to_oppo_d - 15;
                if (PercentageChance(pursuit_percentage)) {
                    DoNotDprintf_opponent("%s: Choosing to get_near because chance dictated it (%d%%)", pOpponent_spec->car_spec->driver_name, pursuit_percentage);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                } else {
                    DoNotDprintf_opponent("%s: Choosing to complete_race because chance dictated it (%d%%)", pOpponent_spec->car_spec->driver_name, pursuit_percentage);
                    NewObjective(pOpponent_spec, eOOT_complete_race);
                }
                return;
            }
        } else if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
            NewObjective(pOpponent_spec, eOOT_wait_for_some_hapless_sod);
            return;
        } else if (pOpponent_spec->pursue_from_start) {

            gOpponents[pOpponent_spec->index].psyche.grudge_against_player = (int)MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + 20 + 40 * pOpponent_spec->nastiness);
            NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
            return;
        } else {
            NewObjective(pOpponent_spec, eOOT_complete_race);
        }
    }
}

// C2PORT-NONMATCHING: best 44.59% | difftest PASS 2000 trials (spec difftest/specs/004aae20_CalcRaceRoute.json) | reason VC5 register allocation and stack layout
// FUNCTION: CARMA2_HW 0x004aae20
void C2_HOOK_FASTCALL CalcRaceRoute(tOpponent_spec *pOpponent_spec) {
    tS16 section_no;
    tS16 node_no;
    tS16 race_section_count;
    tS16 normal_section_ok_direction_count;
    tS16 normal_section_wrong_direction_count;
    tS16 temp_section_array[8];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    int i;
    tRoute_section *current;

    if (pOpponent_spec->nnext_sections >= CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
        DoNotDprintf_opponent("%s: CalcRaceRoute() - Pissing off 'cos projected route full up", pOpponent_spec->car_spec->driver_name);
        return;
    }
    if (pOpponent_spec->nnext_sections == 0) {
        DoNotDprintf_opponent("%s: CalcRaceRoute() - Projected route empty; starting from nearest section", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->complete_race_data.finished_calcing_race_route = 0;
        pOpponent_spec->complete_race_data.found_race_section = 0;
        section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
        if (section_no < 0) {
            return;
        }
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
        if (gProgram_state.AI_vehicles.path_sections[section_no].type == ePST_race_path) {
            pOpponent_spec->complete_race_data.found_race_section = 1;
        }
    }
    while (pOpponent_spec->nnext_sections < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) && !pOpponent_spec->complete_race_data.finished_calcing_race_route) {
        current = &pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1];
        node_no = gProgram_state.AI_vehicles.path_sections[current->section_no].node_indices[current->direction];
        race_section_count = 0;
        normal_section_ok_direction_count = 0;
        normal_section_wrong_direction_count = 0;
        for (i = 0; i < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; i++) {
            section_no = gProgram_state.AI_vehicles.path_nodes[node_no].sections[i];
            if (current->section_no != section_no) {
                if (gProgram_state.AI_vehicles.path_sections[section_no].type == ePST_race_path && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] == node_no) {
                    pOpponent_spec->complete_race_data.found_race_section = 1;
                    temp_section_array[race_section_count] = section_no;
                    race_section_count += 1;
                } else if (race_section_count == 0 && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] == node_no) {
                    temp_section_array[normal_section_ok_direction_count] = section_no;
                    normal_section_ok_direction_count++;
                } else if (race_section_count == 0 && normal_section_ok_direction_count == 0 && (!gProgram_state.AI_vehicles.path_sections[section_no].one_way || gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no)) {
                    temp_section_array[normal_section_wrong_direction_count] = section_no;
                    normal_section_wrong_direction_count++;
                }
            }
        }
        if (race_section_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, race_section_count - 1)], 1);
        } else if (normal_section_ok_direction_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, normal_section_ok_direction_count - 1)], 1);
        } else if (normal_section_wrong_direction_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, normal_section_wrong_direction_count - 1)], 1);
        } else if (!pOpponent_spec->complete_race_data.found_race_section) {
            AddToOpponentsProjectedRoute(pOpponent_spec, current->section_no, current->direction == 0);
        } else {
            pOpponent_spec->complete_race_data.finished_calcing_race_route = 1;
        }
    }
}

// C2PORT-NONMATCHING: best 33.75% | difftest PASS 2000 trials (spec difftest/specs/004b18b0_SetMaxSpeedFromSOCs.json) | reason VC5 x87 scheduling and register allocation
// FUNCTION: CARMA2_HW 0x004b18b0
void C2_HOOK_FASTCALL SetMaxSpeedFromSOCs(tSOC* socs, int count_socs, float* pDesired_speed, float pSpeed, const br_vector2* pPos2d, tCorner* pCorner, tOpponent_spec* pOpponent_spec) {
    int i;
    int max_speed;
    int previous_was_straight;
    float distance;
    float braking_distance;
    float dx;
    float dy;
    float previous_x;
    float previous_y;
    float corner_distance;
    tSOC* current_soc;
    const br_vector3* point;

    previous_was_straight = 0;
    distance = gFLOAT_0058a61c;
    braking_distance = (pSpeed * pSpeed * gFLOAT_0058a69c
        - (float)gFrame_period_for_this_munging * pSpeed * gFLOAT_0058a6a0) * gFLOAT_0058a6a4;
    previous_x = pPos2d->v[0];
    previous_y = pPos2d->v[1];
    current_soc = socs;
    for (i = 0; i < count_socs; i++, current_soc++) {
        if (braking_distance <= distance) {
            return;
        }
        if (previous_was_straight) {
            distance -= corner_distance;
        }
        if (!current_soc->field_0x0) {
            previous_was_straight = 1;
            point = GetOpponentsSectionStartNodePoint(pOpponent_spec, (tS16)pCorner[current_soc->field_0x4].section);
            max_speed = pCorner[current_soc->field_0x4].field_0x18;
            corner_distance = pCorner[current_soc->field_0x4].field_0x0c;
            distance -= corner_distance;
        } else {
            previous_was_straight = 0;
            point = GetOpponentsSectionFinishNodePoint(pOpponent_spec, (tS16)current_soc->field_0x4);
            max_speed = GetOpponentsSectionMaxSpeed(pOpponent_spec, (tS16)current_soc->field_0x4, 1);
        }
        dx = previous_x - point->v[2];
        dy = previous_y - point->v[0];
        distance += sqrtf(dx * dx + dy * dy);
        if (distance < gFLOAT_0058a61c) {
            distance = gFLOAT_0058a61c;
        }
        if (distance * gFLOAT_0058a620 <
            (pSpeed * pSpeed - max_speed * max_speed) * gFLOAT_0058a69c
                - (float)gFrame_period_for_this_munging * pSpeed * gFLOAT_0058a6a0
            && max_speed < *pDesired_speed) {
            *pDesired_speed = (float)max_speed;
        }
        previous_x = point->v[2];
        previous_y = point->v[0];
    }
}

int C2_HOOK_FASTCALL TimeToBeBrakingWhenStruggling(tOpponent_spec* pOpponent_spec) {

    return gTime_stamp_for_this_munging >
        pOpponent_spec->follow_path_data.struggle_time + 2000
            + 15.f * pOpponent_spec->car_spec->collision_info->M
            + 750 * (pOpponent_spec->follow_path_data.number_of_struggles - 1);
}

void C2_HOOK_FASTCALL SetCurvature(tCar_spec* pCar_spec, float pCurvature) {

    float error = pCurvature - pCar_spec->curvature;
    float max_error = gFrame_period_for_this_munging * 5.f / 1000.f;
    if (error > max_error) {
        pCar_spec->curvature += max_error;
    } else if (error < -max_error) {
        pCar_spec->curvature -= max_error;
    } else {
        pCar_spec->curvature = pCurvature;
    }
}

br_scalar C2_HOOK_FASTCALL Distance2D(const br_vector3* pV1, const br_vector3* pV2) {
    br_vector2 v;

    BrVector2Set(&v, pV1->v[2] - pV2->v[2], pV1->v[0] - pV2->v[0]);
    return BrVector2Length(&v);
}

br_scalar C2_HOOK_FASTCALL SectionLength2D(const tOpponent_spec* pOpponent_spec, int pSection) {

    return Distance2D(
        GetOpponentsSectionStartNodePoint(pOpponent_spec, pSection),
        GetOpponentsSectionFinishNodePoint(pOpponent_spec, pSection));
}

int C2_HOOK_FASTCALL NearestSectionInStraight(const br_vector2* pPos2d, const br_vector2* pStart2d, const br_vector2* pFinish2d, tS16 pSection_no, tS16 pEnd_section, const tOpponent_spec* pOpponent_spec) {
    br_vector2 dir;
    br_vector2 rel_pos;
    br_scalar cumLength;
    br_scalar unknown;

    BrVector2Sub(&dir, pFinish2d, pStart2d);
    BrVector2Sub(&rel_pos, pPos2d, pStart2d);
    unknown = BrVector2Dot(&dir, &rel_pos) / BrVector2Dot(&dir, &dir);
    cumLength = 0.f;
    for (;;) {
        tS16 prev_section;
        int smaller;

        smaller = 0;
        prev_section = pSection_no;
        pSection_no = GetOpponentsNextSection(pOpponent_spec, prev_section);
#ifndef CARPOCALYPSE2_MATCHING
        if (pSection_no < 0) {
            return prev_section;
        }
#endif
        if (pSection_no != pEnd_section) {
            cumLength += SectionLength2D(pOpponent_spec, prev_section);
            if (cumLength < unknown) {
                smaller = 1;
            }
        }
        if (!smaller) {
            return prev_section;
        }
    }
}

void C2_HOOK_FASTCALL StuffDataFromCorner(tFollow_path_data* pFollow_path, tCorner *pCorner) {

    pFollow_path->left_not_right = pCorner->left_not_right;
    pFollow_path->cornering = 1;
    BrVector2Copy(&pFollow_path->field_0x40, &pCorner->field_0x00);
    pFollow_path->field_0x48 = pCorner->field_0x08;
    pFollow_path->field_0x4c = pCorner->field_0x0c;
    pFollow_path->corner_section = pCorner->section;
}

float C2_HOOK_FASTCALL MaxCurvatureForCarSpeed(const tCar_spec* pCar, float pSpeed) {
    float max_curve;

    max_curve = pCar->maxcurve;
    if (pSpeed >= 12.5) {
        max_curve = pCar->maxcurve * 12.5f / pSpeed;
    }
    return max_curve;
}

// FUNCTION: CARMA2_HW 0x004af960
tFollow_path_result C2_HOOK_FASTCALL ProcessFollowPath(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand, int pPursuit_mode, int pIgnore_end, int pNever_struggle) {
    tFollow_path_data* data;
    tCorner corners[4];
    tSOC socs[10];
    br_vector3 wank;
    br_vector3 section_dir;
    br_vector3 section_v;
    br_vector3 goal_dir;
    br_actor* car_master_actor;
    br_scalar stopped_speed;
    br_scalar t;
    br_scalar acc;
    br_scalar speed;
    br_scalar dist_along;
    br_scalar acc_factor;
    br_scalar max_acc;
    br_scalar error;
    tCar_spec* car_spec;
    int engine_damage;
    int trans_damage;
    br_vector2 oppo_pos2d;
    br_vector2 start2d;
    br_vector2 finish2d;
    br_vector2 oppo_pos_rel;
    br_vector2 oppo_pos_rel_next;
    br_vector2 v2d;
    br_scalar width;
    br_scalar effective_speed_factor;
    int count_corners;
    int count_socs;
    int straight_section_no;
    br_scalar unk1;
    br_scalar unk2;
    br_scalar unk3;
    br_scalar curv;

    car_spec = pOpponent_spec->car_spec;
    engine_damage = car_spec->damage_units[eDamage_engine].damage_level;
    trans_damage = car_spec->damage_units[eDamage_transmission].damage_level;
    data = &pOpponent_spec->follow_path_data;
    car_master_actor = car_spec->car_master_actor;

    switch (pCommand) {
    case ePOC_start:
        data->first_section_no = GetOpponentsFirstSection(pOpponent_spec);
        data->section_no = data->first_section_no;
        DoNotDprintf_oppoproc("%s: ProcessFollowPath() - new task started, first real section #%d", pOpponent_spec->car_spec->driver_name, GetOpponentsRealSection(pOpponent_spec, data->first_section_no));
        data->has_moved_during_this_task = 0;
        data->struggle_time = 0;
        data->last_finished_struggle_time = gTime_stamp_for_this_munging;
        data->prev_acc = 0.f;
        data->prev_acc_error = 0.f;
        data->borrowed_time_start = gTime_stamp_for_this_munging;
        data->last_struggle_section = -1;
        data->made_it = 1;
        data->cheating = 0;
        data->cornering = 0;
        if (!pOpponent_spec->cheating && !pOpponent_spec->physics_me) {
            DoNotDprintf_oppoproc("%s: Rematerialising from ePOC_start in ProcessFollowPath().+..", pOpponent_spec->car_spec->driver_name);
            /* speed is not initialized in exe */
            RematerialiseOpponentOnNearestSection(pOpponent_spec, BrVector3Length(&car_spec->collision_info->v));
        }
        return eFPR_OK;
    case ePOC_run:
        if (data->cheating || pOpponent_spec->cheating != data->cheating) {
            return FollowCheatyPath(pOpponent_spec);
        }
        if (!pIgnore_end && !data->made_it && gTime_stamp_for_this_munging > data->borrowed_time_start + 1000 && gTime_stamp_for_this_munging < data->borrowed_time_start + 10000) {
            BrVector3Sub(&section_dir, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            BrVector3Sub(&goal_dir, &car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            dist_along = BrVector3Dot(&section_dir, &goal_dir) / BrVector3LengthSquared(&section_dir);
            BrVector3Scale(&section_v, &section_dir, dist_along);
            BrVector3Sub(&wank, &goal_dir, &section_v);
            if (GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= BrVector3Length(&wank)) {
                data->made_it = 1;
            }
        }
        if (gTime_stamp_for_this_munging > data->borrowed_time_start + 10000 && !data->made_it) {
            DoNotDprintf_oppoproc("%s: ProcessFollowPath() giving up due to not making the corner", pOpponent_spec->car_spec->driver_name);
            return eFPR_given_up;
        }
        car_spec->keys.acc = 1;
        speed = WORLD_SCALE * BrVector3Length(&car_spec->collision_info->v);
        if (speed > 0.2f) {
            data->has_moved_during_this_task = 1;
            pOpponent_spec->has_moved_at_some_point = 1;
        }
        if (data->struggle_time != 0) {

            if (!TimeToStopStruggling(pOpponent_spec)) {
                if (TimeToBeBrakingWhenStruggling(pOpponent_spec)) {
                    car_spec->acc_force = 0.f;
                    car_spec->brake_force = 15.f * car_spec->collision_info->M;
                    SetCurvature(car_spec, 0.f);
                } else {
                    car_spec->brake_force = 0.f;
                    car_spec->acc_force = -6.f * car_spec->collision_info->M;
                    SetCurvature(car_spec, 0.f);
                }
                return eFPR_OK;
            }
            DoNotDprintf_oppoproc("%s: done struggling. speed = %.2f m/s", pOpponent_spec->car_spec->driver_name, speed);
            data->made_it = 0;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->struggle_time = 0;
            data->last_finished_struggle_time = gTime_stamp_for_this_munging;
            car_spec->brake_force = 0.0f;
            car_spec->acc_force = 0.0f;
        } else {
            stopped_speed = (pIgnore_end ? 2.f : 6.f) / 30.f;
            if (!pNever_struggle && stopped_speed >= speed && gTime_stamp_for_this_munging > data->last_finished_struggle_time + 2000 && (pPursuit_mode || data->has_moved_during_this_task)) {
                DoNotDprintf_oppoproc("%s: 'Stopped,' section #%d, speed = %.2f m/s, about to start a-strugglin'", pOpponent_spec->car_spec->driver_name, data->section_no, speed);
                data->struggle_time = gTime_stamp_for_this_munging;
                if (pIgnore_end || data->section_no != data->last_struggle_section) {
                    data->last_struggle_section = data->section_no;
                    data->number_of_struggles = 0;
                } else {
                    if (data->number_of_struggles >= 3) {
                        car_spec->acc_force = 0.0f;
                        car_spec->brake_force = 0.0f;
                        DoNotDprintf_oppoproc("%s: Giving up trying to follow path 'cos we've struggled too much", pOpponent_spec->car_spec->driver_name);
                        return eFPR_given_up;
                    }
                    data->number_of_struggles += 1;
                }
            }
        }

        straight_section_no = GetStraight(&start2d, &finish2d, &width, pOpponent_spec->follow_path_data.section_no, pOpponent_spec);
        BrVector2Set(&oppo_pos2d,
            car_spec->car_master_actor->t.t.translate.t.v[2],
            car_spec->car_master_actor->t.t.translate.t.v[0]);
        BrVector2Sub(&oppo_pos_rel_next, &oppo_pos2d, &finish2d);
        BrVector2Set(&v2d,
            car_spec->collision_info->v.v[2],
            car_spec->collision_info->v.v[0]);
        BrVector2Scale(&v2d, &v2d, WORLD_SCALE);
        speed = BrVector2Length(&v2d);
        if (!data->cornering) {
            tS16 nearest_straight_section;

            count_corners = CalcCorners(corners, straight_section_no, width, &start2d, pOpponent_spec);
            nearest_straight_section = NearestSectionInStraight(&oppo_pos2d, &start2d, &finish2d, pOpponent_spec->follow_path_data.section_no, straight_section_no, pOpponent_spec);
            count_socs = CalcSOCs(nearest_straight_section, count_corners, corners, pOpponent_spec, socs, pOpponent_spec->car_spec);
            if (count_corners != 0 && gFrame_period_for_this_munging * speed * .0005f > WORLD_SCALE * (BrVector2Length(&oppo_pos_rel_next) - corners[0].field_0x0c)) {

                StuffDataFromCorner(data, &corners[0]);
                pOpponent_spec->follow_path_data.corner_width = GetOpponentsSectionWidth(pOpponent_spec, corners[0].section);
            } else {
                data->desired_speed = 80.f;
                SetMaxSpeedFromSOCs(socs, count_socs, &data->desired_speed, speed, &oppo_pos2d, &corners[0], pOpponent_spec);
            }
        } else {
            if (BrVector2LengthSquared(&oppo_pos_rel_next) <= CARPOCALYPSE2_SQR(data->field_0x4c)) {

                count_corners = CalcCorners(corners, straight_section_no, width, &start2d, pOpponent_spec);
                count_socs = CalcSOCs(corners[0].section, count_corners, corners, pOpponent_spec, socs, pOpponent_spec->car_spec);
                if (corners[0].field_0x18 > 80.f) {
                    data->desired_speed = 80.f;
                } else {
                    data->desired_speed = (float)corners[0].field_0x18;
                }
                SetMaxSpeedFromSOCs(socs, count_socs, &data->desired_speed, speed, &oppo_pos2d, &corners[0], pOpponent_spec);
            } else {
                data->cornering = 0;
                data->section_no = data->corner_section;
                if (data->section_no >= 20000) {
                    GetStraight(&start2d, &finish2d, &width, data->section_no, pOpponent_spec);
                }
            }
        }
        if (data->cornering) {
            br_vector2 delta;

            BrVector2Sub(&delta, &oppo_pos2d, &data->field_0x40);
            unk1 = (BrVector2Length(&delta) - data->field_0x48) / data->corner_width;
            if (speed > BR_SCALAR_EPSILON) {
                unk2 = BrVector2Dot(&v2d, &delta) / data->field_0x48 * speed;
            } else {
                unk2 = 0.f;
            }
            unk3 = 1.f / (WORLD_SCALE * data->field_0x48);
            if (data->left_not_right) {
                unk2 = -unk2;
            } else {
                unk1 = -unk1;
                unk3 = -unk3;
            }
        } else {
            br_scalar l;
            br_vector2 delta;
            br_vector2 oppo_pos_rel;

            BrVector2Sub(&delta, &finish2d, &start2d);
            BrVector2Sub(&oppo_pos_rel, &oppo_pos2d, &start2d);
            l = BrVector2Length(&delta);
            if (l <= BR_SCALAR_EPSILON) {
                unk1 = 0.f;
            } else {
                unk1 = Vector2Cross(&oppo_pos_rel, &delta) / l;
                if (unk1 > 0.f) {
                    if (unk1 > l) {
                        unk1 -= l;
                    } else {
                        unk1 /= l;
                    }
                }
            }
            if (l > BR_SCALAR_EPSILON && speed > BR_SCALAR_EPSILON) {
                unk2 = Vector2Cross(&delta, &v2d) / (speed * l);
                unk3 = 0.f;
            } else {
                unk2 = 0.f;
                unk3 = 0.f;
            }
        }
        if (unk1 <= 1.f) {
            data->field_0x08 = 0;
            if (pOpponent_spec->follow_path_data.desired_speed > 6.f) {
                pOpponent_spec->follow_path_data.desired_speed += (6.f - pOpponent_spec->follow_path_data.desired_speed) * fabsf(unk2);
            }
            curv = unk3 + MaxCurvatureForCarSpeed(car_spec, speed) * (unk1 / 10.f - unk2);
            if (curv > car_spec->maxcurve) {
                curv = car_spec->maxcurve;
            } else if (curv < -car_spec->maxcurve) {
                curv = -car_spec->maxcurve;
            }
        } else {
            curv = 0.f;
            pOpponent_spec->follow_path_data.desired_speed = 6.f;
            if (speed < 12.0) {
                br_vector2 delta;
                br_scalar l;
                br_scalar crs;

                BrVector2Sub(&delta, &finish2d, &start2d);
                BrVector2Sub(&oppo_pos_rel, &oppo_pos2d, &start2d);
                t = BrVector2Dot(&delta, &oppo_pos_rel) / BrVector2Dot(&delta, &delta);
                if (t < 0.f) {
                    BrVector2Set(&delta, 0.f, 0.f);
                } else if (t <= 1.f) {
                    BrVector2Scale(&delta, &delta, t);
                }
                BrVector2Sub(&delta, &oppo_pos_rel, &delta);
                l = BrVector2Length(&delta);
                crs = Vector2Cross(&v2d, &delta);
                if (speed != 0.f && l != 0.f) {
                    crs /= speed * l;
                }
                curv = -crs * MaxCurvatureForCarSpeed(car_spec, speed);
                if (BrVector2Dot(&delta, &v2d) > 0.f) {
                    if (data->field_0x08 == 0) {
                        data->field_0x38 = 0;
                        data->field_0x08 = gTime_stamp_for_this_munging;
                    } else if (gTime_stamp_for_this_munging - data->field_0x08 > 5000) {
                        data->field_0x08 = gTime_stamp_for_this_munging;
                        data->field_0x38 ^= 0x1;
                    }
                    if (data->field_0x38) {
                        curv = -1.f * curv;
                    }
                }
            }
        }
        effective_speed_factor = GET_CAR_SPEED_FACTOR(car_spec);
        acc_factor = MAX(effective_speed_factor, 1.f);
        if (engine_damage > 50 && engine_damage < 98) {
            acc_factor -= (engine_damage - 50) / 80.f;
        } else if (engine_damage >= 98) {
            acc_factor -= .6f;
        }
        if (trans_damage > 50 && trans_damage < 98) {
            acc_factor -= (trans_damage - 50) / 160.f;
        } else if (trans_damage >= 98) {
            acc_factor -= .3f;
        }
        if (engine_damage >= 99 || trans_damage >= 99) {
            acc_factor = 0.f;
        }
        max_acc = acc_factor * 12.0f;
        error = data->desired_speed * effective_speed_factor - speed;
        acc = (error - data->prev_acc_error) * 1000.0f / gFrame_period_for_this_munging / 10.f + error + data->prev_acc;
        if (acc > max_acc) {
            acc = max_acc;
        }
        if (acc < -max_acc) {
            acc = -max_acc;
        }
        data->prev_acc = acc;
        data->prev_acc_error = error;
        acc *= car_spec->collision_info->M;
        if (acc > 0.0f) {
            car_spec->acc_force = acc;
            car_spec->brake_force = 0.0f;
        } else {
            car_spec->acc_force = 0.0f;
            car_spec->brake_force = -acc;
        }
        SetCurvature(car_spec, curv);
        return eFPR_OK;
    default:
        BrFatal("C:\\Carma2\\Source\\Common\\Oppoproc.c", 1266, "C:\\Carma2\\Source\\Common\\Oppoproc.c line %d", 1266);
        return eFPR_OK;
    }
}

// FUNCTION: CARMA2_HW 0x004ace20
void C2_HOOK_FASTCALL ObjectiveComplete(tOpponent_spec* pOpponent_spec) {

    DoNotDprintf_opponent("%s: Objective Completed", pOpponent_spec->car_spec->driver_name);
    pOpponent_spec->new_objective_required = 1;
    switch (pOpponent_spec->current_objective) {
    case eOOT_complete_race:
        gNum_of_opponents_completing_race -= 1;
        break;
    case eOOT_pursue_and_twat:
        gNum_of_opponents_pursuing -= 1;
        break;
    case eOOT_get_near_player:
        gNum_of_opponents_getting_near -= 1;
        break;
    default:
        break;
    }
}

// FUNCTION: CARMA2_HW 0x004ab150
int C2_HOOK_FASTCALL AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection) {

    if (pOpponent_spec->nnext_sections >= CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
        return 0;
    }
    pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].section_no = pSection_no;
    pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].direction = pDirection;
    pOpponent_spec->nnext_sections += 1;
    return 1;
}

// FUNCTION: CARMA2_HW 0x004a93c0
void C2_HOOK_FASTCALL TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add) {
    tS16 section_no;
    tS16 node_no;
    tS16 temp_section_array[8];
    int i;
    int target;
    int num_of_temp_sections;
    int direction;

    if (!pSections_to_add) {
        PDEnterDebugger("TopUpRandomRoute() called with no seed (woof, bark, etc.)");
    }
    if (pSections_to_add < 0) {
        target = CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections);
    } else {
        target = MIN(pSections_to_add + pOpponent_spec->nnext_sections, (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections));
    }
    while (pOpponent_spec->nnext_sections < target) {
        node_no = gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no].node_indices[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction];
        if (gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections > 1) {
            num_of_temp_sections = 0;
            for (i = 0; i < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; i++) {
                section_no = gProgram_state.AI_vehicles.path_nodes[node_no].sections[i];
                if (pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no != section_no
                    && (!gProgram_state.AI_vehicles.path_sections[section_no].one_way || gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no)
                    && (pOpponent_spec->cheating || gProgram_state.AI_vehicles.path_sections[section_no].type != ePST_cheat_only)) {
                    temp_section_array[num_of_temp_sections] = section_no;
                    num_of_temp_sections++;
                }
            }

            if (num_of_temp_sections > 1) {
                section_no = temp_section_array[IRandomBetween(0, num_of_temp_sections - 1)];
                direction = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no;
            } else if (num_of_temp_sections == 1) {
                section_no = temp_section_array[0];
                direction = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no;
            } else {
                section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
                direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction == 0;
            }
        } else {
            section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
            direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction == 0;
        }
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, direction);
    }
}

// C2PORT-NONMATCHING: best 84.51% | difftest PASS 2000 trials (spec difftest/specs/004ac890_ProcessGetNearPlayer.json) | reason VC5 branch layout and register allocation
// FUNCTION: CARMA2_HW 0x004ac890
void C2_HOOK_FASTCALL ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    int sections_to_skip;
    int i;
    tFollow_path_result result;

    switch (pCommand) {
    case ePOC_run:
        if (((tU16)pOpponent_spec->car_spec->car_ID & 0xff00) == 0x300
                && pOpponent_spec->distance_from_home > gOppo_pursuit_precinct_distance) {
            DoNotDprintf_opponent("%s: Completing get_near objective because I'm out of my precinct", pOpponent_spec->car_spec->driver_name);
            NewObjective(pOpponent_spec, eOOT_return_to_start);
            return;
        }
        if (pOpponent_spec->follow_path_data.section_no > 20000) {
            sections_to_skip = pOpponent_spec->follow_path_data.section_no - 20000;
            if (pOpponent_spec->nnext_sections > sections_to_skip) {
                if (CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - sections_to_skip > 0) {
                    for (i = 0; i < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - sections_to_skip; i++) {
                        pOpponent_spec->next_sections[i].section_no = pOpponent_spec->next_sections[i + sections_to_skip].section_no;
                        pOpponent_spec->next_sections[i].direction = pOpponent_spec->next_sections[i + sections_to_skip].direction;
                    }
                }
                pOpponent_spec->nnext_sections -= sections_to_skip;
            }
            pOpponent_spec->follow_path_data.section_no = 20000;
        }
        if (pOpponent_spec->nnext_sections <= 10) {
            NewCalcGetNearPlayerRoute(pOpponent_spec, &gProgram_state.current_car, ePOC_start);
        }
        result = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
        if (result == eFPR_given_up) {
            NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
            return;
        }
        if (result != eFPR_end_of_path) {
            return;
        }
        DoNotDprintf_opponent("%s: Restarting get_near_player route because ran out of path!", pOpponent_spec->car_spec->driver_name);
        break;
    case ePOC_start:
        DoNotDprintf_opponent("%s: ProcessGetNearPlayer() - new objective started", pOpponent_spec->car_spec->driver_name);
        break;
    default:
        return;
    }
    NewCalcGetNearPlayerRoute(pOpponent_spec, &gProgram_state.current_car, ePOC_run);
    ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
}

// FUNCTION: CARMA2_HW 0x004ac540
void C2_HOOK_FASTCALL ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    float t;
    float terminal_time;
    float y;

    switch (pCommand) {
    case ePOC_start:
        DoNotDprintf_opponent("%s: ProcessLevitate() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->levitate_data.waiting_to_levitate = 1;
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->levitate_data.time_started = gTime_stamp_for_this_munging;
        break;

    case ePOC_run:
        if (pOpponent_spec->levitate_data.waiting_to_levitate) {
            if ((BrVector3Length(&pOpponent_spec->car_spec->collision_info->v) < (float)(.01 / 6.9) && BrVector3Length(&pOpponent_spec->car_spec->collision_info->omega) < 1.f) || gTime_stamp_for_this_munging - pOpponent_spec->levitate_data.time_started > 4000) {
                pOpponent_spec->levitate_data.waiting_to_levitate = 0;
                pOpponent_spec->levitate_data.time_started = gTime_stamp_for_this_munging;
                pOpponent_spec->levitate_data.initial_y = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1];
                if (pOpponent_spec->car_spec->has_been_stolen) {
                    NewTextHeadupSlot(4, 250, 5000, -4, GetMiscString(170));
                }
            } else {
                pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
                pOpponent_spec->car_spec->acc_force = 0.f;
                BrVector3InvScale(&pOpponent_spec->car_spec->collision_info->omega, &pOpponent_spec->car_spec->collision_info->omega,
                    pow(2.f, gFrame_period_for_this_munging / 1000.0));
            }
        }
        if (!pOpponent_spec->levitate_data.waiting_to_levitate) {
            TurnOpponentPhysicsOff(pOpponent_spec);
            t = (gTime_stamp_for_this_munging - pOpponent_spec->levitate_data.time_started) / 1000.0;
            terminal_time = 20.f;
            if (t < terminal_time) {
                y = t * t * .5f / 2.f;
            } else {
                y = terminal_time * terminal_time * 0.5 / 2.0 + (t - terminal_time) * 10.0;
            }
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] = pOpponent_spec->levitate_data.initial_y + y;
            if (y > 200.f) {
                pOpponent_spec->finished_for_this_race = 1;
            }
        }
        break;
    case ePOC_die:
        return;
    default:
        break;
    }
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 40.23% | difftest PASS 2000 trials (spec difftest/specs/004a87a0_RematerialiseOpponent.json) + PASS 2000 trials (spec difftest/specs/004a87a0_RematerialiseOpponent_EXHAUSTED.json) | reason x87-scheduling/stack-layout
// FUNCTION: CARMA2_HW 0x004a87a0
int C2_HOOK_FASTCALL RematerialiseOpponent(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    int this_total;
    br_matrix34* mat;
    br_vector3 sensible_pos;
    br_matrix34 original_mat;
    br_vector3 a;
    br_vector3 b;
    br_vector3 norm;
    br_vector3 norm2;
    br_scalar dist;
    br_scalar dist2;
    br_scalar ts;
    br_angle phi;
    int i;
    int j;
    int massage_count;
    br_angle theta;
    int sensible_place;

    this_total = 0;
    massage_count = 0;
    mat = &pOpponent_spec->car_spec->car_master_actor->t.t.mat;
    theta = BR_ANGLE_DEG(90) - (br_angle)(long)(atan2f(mat->m[2][2], mat->m[2][0]) * DAT_0058a4a8);
    if (pOpponent_spec->physics_me) {
        DoNotDprintf_opponent("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }
    gRematerialisation_count++;
    BrMatrix34Copy(&original_mat, mat);
    pOpponent_spec->car_spec->collision_info->flags |= 0x100;
    pOpponent_spec->car_spec->car_master_actor->render_style = BR_RSTYLE_DEFAULT;
    PipeSingleOppoRenderage(pOpponent_spec, 1);
    if (!pOpponent_spec->physics_me) {
        pOpponent_spec->physics_me = 1;
    }
    gActive_car_list_rebuild_required = 1;
    RebuildActiveCarList();
    do {
        gRematerialisation_extra_attempts++;
        this_total++;
        BrVector3Scale((br_vector3*)mat->m[3], (br_vector3*)mat->m[3], DAT_0058a4b0);
        b = *(br_vector3*)mat->m[3];
        BrMatrix34RotateY(mat, theta);
        *(br_vector3*)mat->m[3] = b;
        BrVector3SetFloat(&b, 0.0f, -100.0f, 0.0f);
        a = *(br_vector3*)mat->m[3];
        a.v[1] -= DAT_0058a4b8;
        FUN_0041e360(&a, &b, &norm, &dist);
        a.v[1] -= DAT_0058a4bc;
        FUN_0041e360(&a, &b, &norm2, &dist2);
        if (dist2 <= gOppo_pursuit_one) {
            BrVector3SetFloat(&b, 0.0f, -5.01f, 0.0f);
            a.v[1] -= DAT_0058a47c;
            i = 0;
            do {
                a.v[1] -= DAT_0058a4c0;
                FUN_0041e360(&a, &b, &norm2, &dist2);
                if (dist2 <= gOppo_pursuit_one) {
                    break;
                }
                i++;
            } while (i < 20);
            dist2 = dist2 * DAT_0058a4c4 - (br_scalar)(i + 1) * DAT_0058a4c8;
        }
        if (dist2 < dist) {
            dist = -dist2;
            norm = norm2;
        }
        if (fabsf(dist) <= gOppo_pursuit_one) {
            mat->m[3][1] -= dist * DAT_0058a47c - gOppo_pursuit_minimum_velocity_ratio;
            ts = mat->m[2][0] * norm.v[0] + mat->m[2][1] * norm.v[1] + mat->m[2][2] * norm.v[2];
            phi = (br_angle)(long)(asin(ts) * DAT_0058a4a8);
            BrMatrix34PreRotateX(mat, phi);
            ts = mat->m[0][0] * norm.v[0] + mat->m[0][1] * norm.v[1] + mat->m[0][2] * norm.v[2];
            phi = (br_angle)(long)(asin(ts) * DAT_0058a4d0);
            BrMatrix34PreRotateZ(mat, phi);
        }
        pOpponent_spec->car_spec->direction.v[0] = -mat->m[2][0];
        pOpponent_spec->car_spec->direction.v[1] = -mat->m[2][1];
        pOpponent_spec->car_spec->direction.v[2] = -mat->m[2][2];
        BrMatrix34ApplyP(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->centre_of_mass_world_scale, mat);
        BrVector3Scale(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->pos, DAT_0058a4d8);
        BrVector3Scale((br_vector3*)mat->m[3], (br_vector3*)mat->m[3], DAT_0058a4e0);
        BrVector3Negate(&pOpponent_spec->car_spec->collision_info->v, (br_vector3*)mat->m[2]);
        ts = (br_scalar)sqrt(pOpponent_spec->car_spec->collision_info->v.v[0] * pOpponent_spec->car_spec->collision_info->v.v[0]
                + pOpponent_spec->car_spec->collision_info->v.v[1] * pOpponent_spec->car_spec->collision_info->v.v[1]
                + pOpponent_spec->car_spec->collision_info->v.v[2] * pOpponent_spec->car_spec->collision_info->v.v[2]);
        if (DAT_0058a4e8 < ts) {
            ts = (br_scalar)(DAT_0058a4a0 / ts);
            pOpponent_spec->car_spec->collision_info->v.v[0] *= ts;
            pOpponent_spec->car_spec->collision_info->v.v[1] *= ts;
            pOpponent_spec->car_spec->collision_info->v.v[2] *= ts;
        } else {
            pOpponent_spec->car_spec->collision_info->v.v[0] = gOppo_pursuit_one;
            pOpponent_spec->car_spec->collision_info->v.v[1] = gOppo_render_zero;
            pOpponent_spec->car_spec->collision_info->v.v[2] = gOppo_render_zero;
        }
        ts = pSpeed;
        if (DAT_0058a4ec < pSpeed) {
            ts = DAT_0058a4ec;
        }
        pOpponent_spec->car_spec->collision_info->v.v[0] *= ts;
        ts = pSpeed;
        if (DAT_0058a4ec < pSpeed) {
            ts = DAT_0058a4ec;
        }
        pOpponent_spec->car_spec->collision_info->v.v[1] *= ts;
        ts = pSpeed;
        if (DAT_0058a4ec < pSpeed) {
            ts = DAT_0058a4ec;
        }
        pOpponent_spec->car_spec->collision_info->v.v[2] *= ts;
        pOpponent_spec->car_spec->collision_info->omega.v[0] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->omega.v[1] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->omega.v[2] = gOppo_render_zero;
        pOpponent_spec->car_spec->curvature = gOppo_render_zero;
        BrMatrix34Copy(&pOpponent_spec->car_spec->collision_info->transform_matrix, mat);
        BrMatrix34Copy(&pOpponent_spec->car_spec->old_frame_mat, mat);
        for (j = 0; j < 4; j++) {
            pOpponent_spec->car_spec->oldd[j] = pOpponent_spec->car_spec->ride_height;
        }
        pOpponent_spec->car_spec->revs = gOppo_render_zero;
        pOpponent_spec->car_spec->gear = 0;
        pOpponent_spec->car_spec->traction_control = 1;
        BrMatrix34ApplyP(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->centre_of_mass_world_scale, mat);
        BrVector3Scale(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->pos, DAT_0058a4d8);
        pOpponent_spec->car_spec->direction.v[0] = -pOpponent_spec->car_spec->collision_info->transform_matrix.m[3][0];
        pOpponent_spec->car_spec->direction.v[1] = -pOpponent_spec->car_spec->collision_info->transform_matrix.m[3][1];
        pOpponent_spec->car_spec->direction.v[2] = -pOpponent_spec->car_spec->collision_info->transform_matrix.m[3][2];
        pOpponent_spec->car_spec->collision_info->box_face_ref = gFace_num__car - 2;
        pOpponent_spec->car_spec->collision_info->disable_move_rotate = 0;
        sensible_place = TestForCarInSensiblePlace(pOpponent_spec->car_spec, &sensible_pos);
        if (!sensible_place) {
            BrMatrix34Copy(mat, &original_mat);
            if (massage_count <= 22) {
                if (massage_count <= 20) {
                    if ((massage_count % 4) < 2) {
                        BrVector3Negate(&norm2, (br_vector3*)mat->m[2]);
                        ts = (br_scalar)sqrt(norm2.v[0] * norm2.v[0] + norm2.v[1] * norm2.v[1] + norm2.v[2] * norm2.v[2]);
                        if (DAT_0058a4e8 < ts) {
                            dist = (br_scalar)(DAT_0058a4a0 / ts);
                            BrVector3Scale(&norm2, &norm2, dist);
                        } else {
                            BrVector3SetFloat(&norm2, 1.0f, 0.0f, 0.0f);
                        }
                        dist2 = (br_scalar)(massage_count >> 2) * gOppo_pursuit_velocity_scale;
                    } else {
                        norm2.v[0] = -mat->m[2][2];
                        norm2.v[1] = gOppo_render_zero;
                        norm2.v[2] = mat->m[2][0];
                        ts = (br_scalar)sqrt(norm2.v[0] * norm2.v[0] + norm2.v[2] * norm2.v[2]);
                        if (DAT_0058a4e8 < ts) {
                            dist = (br_scalar)(DAT_0058a4a0 / ts);
                            norm2.v[0] *= dist;
                            norm2.v[2] *= dist;
                        } else {
                            norm2.v[0] = gOppo_pursuit_one;
                        }
                        dist2 = (br_scalar)(massage_count >> 2) * DAT_0058a4f0;
                    }
                    if ((massage_count & 1) != 0) {
                        BrVector3Negate(&norm2, &norm2);
                    }
                    BrVector3Scale(&norm2, &norm2, dist2);
                    BrVector3Accumulate((br_vector3*)mat->m[3], &norm2);
                } else {
                    mat->m[3][1] += (float)(massage_count - 20) + (float)(massage_count - 20);
                }
            }
            massage_count++;
        }
    } while (!sensible_place && massage_count <= 23);

    gRematerialisation_extra_attempts--;
    this_total--;
    if (sensible_place) {
        DoNotDprintf_opponent("%s: Rematerialised (took %d attempts, orig. pos. (%7.3f,%7.3f,%7.3f), actual pos. (%7.3f,%7.3f,%7.3f))",
                pOpponent_spec->car_spec->driver_name, this_total + 1,
                original_mat.m[3][0], original_mat.m[3][1], original_mat.m[3][2],
                mat->m[3][0], mat->m[3][1], mat->m[3][2]);
    }
    if (gRematerialisation_max_extra_attempts < this_total) {
        gRematerialisation_max_extra_attempts = this_total;
    }
    if (gRematerialisation_extra_attempts != 0) {
        DoNotDprintf_opponent("MassageOpponentPosition() called an avg of %.1f times (max %d) per ReMaterialisation",
                (double)gRematerialisation_extra_attempts / gRematerialisation_count,
                gRematerialisation_max_extra_attempts);
    }
    if (!sensible_place) {
        pOpponent_spec->car_spec->acc_force = gOppo_render_zero;
        pOpponent_spec->car_spec->brake_force = gOppo_render_zero;
        pOpponent_spec->car_spec->curvature = gOppo_render_zero;
        pOpponent_spec->car_spec->revs = gOppo_render_zero;
        pOpponent_spec->car_spec->gear = 0;
        pOpponent_spec->car_spec->collision_info->omega.v[0] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->omega.v[1] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->omega.v[2] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->v.v[0] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->v.v[1] = gOppo_render_zero;
        pOpponent_spec->car_spec->collision_info->v.v[2] = gOppo_render_zero;
        if (pOpponent_spec->physics_me) {
            pOpponent_spec->physics_me = 0;
            pOpponent_spec->car_spec->car_master_actor->render_style = BR_RSTYLE_NONE;
            PipeSingleOppoRenderage(pOpponent_spec, 0);
        }
        gActive_car_list_rebuild_required = 1;
        RebuildActiveCarList();
        TeleportOpponentToNearestSafeLocation(pOpponent_spec);
        return 1;
    }
    ResetCarSpecialVolume(pOpponent_spec->car_spec->collision_info);
    return 1;
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004a8170
int C2_HOOK_FASTCALL RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    br_vector3 intersect;
    br_vector3 direction_v;
    br_vector3 car_to_end;
    br_vector3* finish;
    br_scalar distance;
    br_scalar distance_to_end;
    tS16 section_no;

    if (pOpponent_spec->physics_me) {
        DoNotDprintf_opponent("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }
    section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
    finish = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].pos;
    BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &intersect);
    PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &direction_v);
    BrVector3Sub(&car_to_end, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
    if (RematerialiseOpponent(pOpponent_spec, pSpeed)) {
        pOpponent_spec->car_spec->brake_force = 0.0f;
        pOpponent_spec->car_spec->acc_force = 0.0f;

        distance_to_end = BrVector3Length(&car_to_end);
        if (distance_to_end < 5.0f) {
            pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->collision_info->M * 15.0f;
        } else {
            pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->collision_info->M / 2.0f;
        }
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004abc00
void C2_HOOK_FASTCALL ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pCommand) {
    case ePOC_start:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
        break;
    case ePOC_run:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
        break;
    default:
        break;
    }
}

// FUNCTION: CARMA2_HW 0x004ac180
int C2_HOOK_FASTCALL SearchForSection(tRoute_section* pTemp_store, tRoute_section* pPerm_store, int* pNum_of_perm_store_sections, tS16 pTarget_section, int pDepth, br_scalar pDistance_so_far, tOpponent_spec* pOpponent_spec) {
    int section_offset;
    char depth_indent[32];
    int direction;
    tPath_node* node_ptr;
    tS16 node_no;
    tS16 section_no;
    tS16 section_no_index;
    br_scalar distance_so_far;

    DoNotDprintf_opponent("%sSearchForSection() - START: depth %d", depth_indent, pDepth);
    gSFS_cycles_this_time++;
    if (pDepth == 1) {
        DoNotDprintf_opponent("%sSearchForSection() - First time called", depth_indent);
        memset(gBit_per_node, 0, (gProgram_state.AI_vehicles.number_of_path_nodes + 7) / 8);
        gSFS_shortest_dist = BR_SCALAR_MAX;
        gSFS_routes_found = 0;
        *pNum_of_perm_store_sections = 0;
    }
    if (pDepth >= 10) {
        DoNotDprintf_opponent("%sSearchForSection() - *** Reached max depth ***", depth_indent);
        return 0;
    }
    node_no = ((tS16*)gOppo_junction_table)[pTemp_store[pDepth - 1].section_no * 10 + pTemp_store[pDepth - 1].direction];
    node_ptr = &gOppo_path_nodes[node_no];
    gBit_per_node[node_no / 8] |= 1 << (node_no % 8);
    DoNotDprintf_opponent("%sSearchForSection() - node #%d has %d sections", depth_indent, node_no, node_ptr->number_of_sections);
    for (section_no_index = 0; section_no_index < node_ptr->number_of_sections; section_no_index++) {
        section_no = node_ptr->sections[section_no_index];
        section_offset = section_no * 0x14;
        direction = *(tS16*)((tU8*)gOppo_junction_table + section_offset + 2) != node_no;
        DoNotDprintf_opponent("%sSearchForSection() - node's section #%d is #%d", depth_indent, section_no_index, section_no);
        if ((gBit_per_node[(*(tS16*)((tU8*)gOppo_junction_table + section_offset + direction * 2)) / 8] & (1 << ((*(tS16*)((tU8*)gOppo_junction_table + section_offset + direction * 2)) % 8))) != 0) {
            DoNotDprintf_opponent("%sSearchForSection() - *** node #%d already visited ***", depth_indent, *(tS16*)((tU8*)gOppo_junction_table + section_offset + direction * 2));
            continue;
        }
        if (*(tU8*)((tU8*)gOppo_junction_table + section_offset + 0x11) && *(tS16*)((tU8*)gOppo_junction_table + section_offset + 2) == node_no) {
            DoNotDprintf_opponent("%sSearchForSection() - *** no entry ***", depth_indent);
            continue;
        }
        if (!pOpponent_spec->cheating && *(tU8*)((tU8*)gOppo_junction_table + section_offset + 0x10) == ePST_cheat_only) {
            continue;
        }
        DoNotDprintf_opponent("%sSearchForSection() - adding section #%d to list", depth_indent, section_no);
        pTemp_store[pDepth].section_no = section_no;
        pTemp_store[pDepth].direction = direction;
        distance_so_far = *(br_scalar*)((tU8*)gOppo_junction_table + section_offset + 0xc) + pDistance_so_far;
        if ((section_no == pTarget_section) && (distance_so_far < gSFS_shortest_dist)) {
            gSFS_shortest_dist = distance_so_far;
            *pNum_of_perm_store_sections = pDepth + 1;
            memcpy(pPerm_store, pTemp_store, sizeof(tRoute_section) * *pNum_of_perm_store_sections);
            gSFS_routes_found_copy = gSFS_routes_found + 1;
            gSFS_routes_found = gSFS_routes_found_copy;
            if (gSFS_routes_found < 2) {
                DoNotDprintf_opponent("%sSearchForSection() - Found a target", depth_indent);
                DoNotDprintf_opponent("%sSearchForSection() - Returning to depth %d", depth_indent, pDepth - 1);
                gBit_per_node[node_no / 8] &= ~(1 << (node_no % 8));
                return 0;
            }
            DoNotDprintf_opponent("%sSearchForSection() - Found final target", depth_indent);
            return 1;
        }
        if ((pDepth < 9) && SearchForSection(pTemp_store, pPerm_store, pNum_of_perm_store_sections, pTarget_section, pDepth + 1, distance_so_far, pOpponent_spec)) {
            return 1;
        }
    }
    DoNotDprintf_opponent("%sSearchForSection() - Returning to depth %d", depth_indent, pDepth - 1);
    gBit_per_node[node_no / 8] &= ~(1 << (node_no % 8));
    return 0;
}

// C2PORT-NONMATCHING: best 43.88% | difftest PASS 2000 trials (spec difftest/specs/004ac9e0_NewCalcGetNearPlayerRoute.json) | reason compiler codegen
// FUNCTION: CARMA2_HW 0x004ac9e0
void C2_HOOK_FASTCALL NewCalcGetNearPlayerRoute(tOpponent_spec* pOpponent_spec, tCar_spec* pPlayer, tProcess_objective_command pCommand) {
    br_vector3 player_path_direction;
    br_vector3 player_intersection;
    br_vector3 path_direction;
    br_vector3 intersection;
    br_scalar distance;
    tRoute_section temp_route[10];
    tRoute_section found_route[10];
    int found_route_count;
    br_scalar stopping_distance;
    br_scalar distance_to_node;
    tS16 opponent_section;
    tS16 node_no;
    int update_route;
    int stop_searching;
    int route_index;
    int i;
    int sections_to_copy;
    br_actor* actor;
    float projection;

    stop_searching = 0;
    if (pOpponent_spec->nnext_sections >= CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
        return;
    }
    if (pPlayer->field_0x155c < 0 || pPlayer->field_0x1540 < gTime_stamp_for_this_munging) {
        pPlayer->field_0x1540 = IRandomBetween(-100, 100) + 1000 + gTime_stamp_for_this_munging;
        pPlayer->field_0x155c = FindNearestGeneralSection(NULL,
            &pPlayer->car_master_actor->t.t.translate.t,
            &player_path_direction,
            &player_intersection,
            &distance);
    }
    if (pPlayer->field_0x155c < 0) {
        PDEnterDebugger("No path section near player. THIS CAN'T HAPPEN!");
        return;
    }
    update_route = pOpponent_spec->nnext_sections == 0 ? 1 : pCommand;
    if (update_route == 0 && pPlayer->field_0x155c == pOpponent_spec->players_section_when_last_calced_full_path) {
        goto search_route;
    }
    opponent_section = FindNearestGeneralSection(NULL,
        &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t,
        &path_direction,
        &intersection,
        &distance);
    if (opponent_section < 0) {
        PDEnterDebugger("No path section near us. THIS CAN'T HAPPEN!");
        return;
    }
    route_index = -1;
    i = 0;
    if (pOpponent_spec->nnext_sections > 0) {
        tRoute_section* route;

        route = pOpponent_spec->next_sections;
        do {
            route->section_no = opponent_section;
            route_index = i;
            if (opponent_section != 0) {
                break;
            }
            i++;
            route++;
            route_index = -1;
        } while (i < pOpponent_spec->nnext_sections);
    }
    if (route_index < 0) {
        actor = pOpponent_spec->car_spec->car_master_actor;
        projection = (intersection.v[1] - actor->t.t.translate.t.v[1]) * actor->t.t.mat.m[2][1]
            + actor->t.t.mat.m[2][0] * (intersection.v[0] - actor->t.t.translate.t.v[0])
            + (intersection.v[2] - actor->t.t.translate.t.v[2]) * actor->t.t.mat.m[2][2];
        pOpponent_spec->nnext_sections = 0;
        pOpponent_spec->next_sections[0].section_no = opponent_section;
        pOpponent_spec->next_sections[0].direction = projection >= gOppo_render_zero;
        pOpponent_spec->nnext_sections = 1;
    } else {
        stopping_distance = (DAT_0058a47c -
            (pOpponent_spec->car_spec->collision_info->v.v[2] * pOpponent_spec->car_spec->collision_info->v.v[2]
                + pOpponent_spec->car_spec->collision_info->v.v[1] * pOpponent_spec->car_spec->collision_info->v.v[1]
                + pOpponent_spec->car_spec->collision_info->v.v[0] * pOpponent_spec->car_spec->collision_info->v.v[0]))
            * DAT_0058a55c * (br_scalar)DAT_0058a4e0;
        if (stopping_distance > gOppo_render_zero) {
            node_no = ((tS16*)gOppo_junction_table)[pOpponent_spec->next_sections[route_index].section_no * 10
                + pOpponent_spec->next_sections[route_index].direction];
            path_direction.v[0] = gOppo_path_nodes[node_no].pos.v[0] - intersection.v[0];
            path_direction.v[1] = gOppo_path_nodes[node_no].pos.v[1] - intersection.v[1];
            path_direction.v[2] = gOppo_path_nodes[node_no].pos.v[2] - intersection.v[2];
            distance_to_node = sqrtf(path_direction.v[2] * path_direction.v[2]
                + path_direction.v[1] * path_direction.v[1]
                + path_direction.v[0] * path_direction.v[0]);
            if (route_index < pOpponent_spec->nnext_sections - 1) {
                for (i = route_index + 1; i < pOpponent_spec->nnext_sections - 1; i++) {
                    if (stopping_distance <= distance_to_node) {
                        break;
                    }
                    stopping_distance -= distance_to_node;
                    route_index++;
                    distance_to_node = *(br_scalar*)((tU8*)gOppo_junction_table
                        + 0xc + pOpponent_spec->next_sections[route_index].section_no * 0x14);
                }
            }
        }
        if (pOpponent_spec->nnext_sections > route_index) {
            tRoute_section* src;
            tRoute_section* dst;

            i = 12 - route_index;
            if (i > 0) {
                src = &pOpponent_spec->next_sections[route_index];
                dst = pOpponent_spec->next_sections;
                do {
                    dst->section_no = src->section_no;
                    dst->direction = src->direction;
                    dst++;
                    i--;
                    src++;
                } while (i != 0);
            }
            pOpponent_spec->nnext_sections -= route_index;
        }
    }

search_route:
    for (;;) {
        if (pOpponent_spec->nnext_sections >= CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) || stop_searching != 0) {
            pOpponent_spec->players_section_when_last_calced_full_path = pPlayer->field_0x155c;
            return;
        }
        temp_route[0].section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].section_no;
        temp_route[0].direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].direction;
        gSFS_search_count++;
        gSFS_cycles_this_time = 0;
        SearchForSection(temp_route, found_route, &found_route_count, pPlayer->field_0x155c, 1, 0, pOpponent_spec);
        gSFS_total_cycles += gSFS_cycles_this_time;
        if (gSFS_max_cycles < gSFS_cycles_this_time) {
            gSFS_max_cycles = gSFS_cycles_this_time;
        }
        if (found_route_count < 2) {
            stop_searching = 1;
            if (pOpponent_spec->nnext_sections < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
                TopUpRandomRoute(pOpponent_spec, gOpponents[pOpponent_spec->index].strength_rating < 5 ? 4 : 2);
            }
        } else {
            sections_to_copy = MIN(found_route_count - 1,
                (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - pOpponent_spec->nnext_sections);
            memcpy(&pOpponent_spec->next_sections[pOpponent_spec->nnext_sections], found_route,
                sections_to_copy * sizeof(tRoute_section));
            pOpponent_spec->nnext_sections += sections_to_copy;
            if (pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no == pPlayer->field_0x155c) {
                if (gOpponents[pOpponent_spec->index].strength_rating < 5) {
                    sections_to_copy = IRandomBetween(1, 2);
                } else {
                    sections_to_copy = 1;
                }
                TopUpRandomRoute(pOpponent_spec, sections_to_copy);
            }
        }
    }
}

// FUNCTION: CARMA2_HW 0x004abf60
void C2_HOOK_FASTCALL CalcReturnToStartPointRoute(tOpponent_spec* pOpponent_spec) {
    tRoute_section found_route[10];
    tRoute_section temp_route[10];
    int found_route_count;
    br_vector3 path_direction;
    br_vector3 intersection;
    br_scalar distance;
    br_scalar len;
    tS16 section_no;
    int sections_to_copy;

    pOpponent_spec->nnext_sections = 0;
    section_no = FindNearestGeneralSection(NULL,
        &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t,
        &path_direction,
        &intersection,
        &distance);
    len = sqrtf(path_direction.v[0] * path_direction.v[0]
        + path_direction.v[1] * path_direction.v[1]
        + path_direction.v[2] * path_direction.v[2]);
    if (len > DAT_0058a4e8) {
        len = (br_scalar)(DAT_0058a4a0 / len);
        path_direction.v[0] = len * path_direction.v[0];
        path_direction.v[1] = len * path_direction.v[1];
        path_direction.v[2] = len * path_direction.v[2];
    } else {
        path_direction.v[0] = 1.0f;
        path_direction.v[1] = 0.0f;
        path_direction.v[2] = 0.0f;
    }
    if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &path_direction) > gOppo_render_zero) {
        if (pOpponent_spec->nnext_sections < (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
            pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].section_no = section_no;
            pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].direction = 1;
            pOpponent_spec->nnext_sections++;
        }
    } else {
        if (pOpponent_spec->nnext_sections < (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
            pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].section_no = section_no;
            pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].direction = 0;
            pOpponent_spec->nnext_sections++;
        }
    }
    temp_route[0].section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
    temp_route[0].direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction;
    gSFS_cycles_this_time = 0;
    gSFS_search_count++;
    SearchForSection(temp_route, found_route, &found_route_count, pOpponent_spec->return_to_start_data.section_no, 1, 0, pOpponent_spec);
    gSFS_total_cycles += gSFS_cycles_this_time;
    if (gSFS_cycles_this_time > gSFS_max_cycles) {
        gSFS_max_cycles = gSFS_cycles_this_time;
    }
    DoNotDprintf_opponent(">>>SearchForSection() - max %d, avg %.1f", gSFS_max_cycles, (double)gSFS_total_cycles / gSFS_search_count);
    if (found_route_count > 1) {
        sections_to_copy = MIN(found_route_count - 1,
            (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - pOpponent_spec->nnext_sections);
        memcpy(&pOpponent_spec->next_sections[pOpponent_spec->nnext_sections], &found_route[1],
            sections_to_copy * sizeof(tRoute_section));
        pOpponent_spec->nnext_sections += sections_to_copy;
        TopUpRandomRoute(pOpponent_spec, 2);
        return;
    }
    DoNotDprintf_opponent("%s: CalcReturnToStartPointRoute() - SearchForSection() produced bugger all", pOpponent_spec->car_spec->driver_name);
    if (pOpponent_spec->nnext_sections <= (int)CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
        TopUpRandomRoute(pOpponent_spec, 14 - pOpponent_spec->nnext_sections);
    }
}

int C2_HOOK_FASTCALL TeleportCopToStart(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;

    if (!pOpponent_spec->cheating || !CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        return 0;
    }
    BrVector3Sub(&wank, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
    if (BrVector3Length(&wank) <= gIn_view_distance) {
        return 0;
    }
    BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
    PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &pOpponent_spec->start_direction);
    pOpponent_spec->physics_me = 0;
    RematerialiseOpponent(pOpponent_spec, 0.0);
    TurnOpponentPhysicsOff(pOpponent_spec);
    RebuildActiveCarList();
    NewObjective(pOpponent_spec, eOOT_wait_for_some_hapless_sod);
    return 1;
}

// FUNCTION: CARMA2_HW 0x004abc40
void C2_HOOK_FASTCALL ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3 section_v;
    br_vector3 our_pos_xz;
    br_vector3 cop_to_start;
    br_scalar distance;
    br_vector3 teleport_delta;
    tCar_spec* teleport_car;
    int res;

    switch (pCommand) {
    case ePOC_start:
        DoNotDprintf_opponent("%s: ProcessReturnToStart() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->return_to_start_data.waiting_near_start = 0;
        pOpponent_spec->return_to_start_data.section_no = FindNearestPathSection(&pOpponent_spec->start_pos, &section_v, &pOpponent_spec->return_to_start_data.nearest_path_point, &distance);
        pOpponent_spec->return_to_start_data.nearest_path_point.v[1] = 0.0;
        CalcReturnToStartPointRoute(pOpponent_spec);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        break;
    case ePOC_run:
        if (pOpponent_spec->cheating && CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
            BrVector3Sub(&teleport_delta, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
            if (BrVector3Length(&teleport_delta) > gIn_view_distance) {
                BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
                PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &pOpponent_spec->start_direction);
                pOpponent_spec->physics_me = 0;
                RematerialiseOpponent(pOpponent_spec, 0.0);
                teleport_car = pOpponent_spec->car_spec;
                teleport_car->acc_force = 0.f;
                teleport_car->brake_force = 0.f;
                teleport_car->curvature = 0.f;
                teleport_car->gear = 0;
                teleport_car->revs = 0.f;
                BrVector3Set(&teleport_car->collision_info->omega, 0.f, 0.f, 0.f);
                BrVector3Set(&teleport_car->collision_info->v, 0.f, 0.f, 0.f);
                if (pOpponent_spec->physics_me) {
                    pOpponent_spec->physics_me = 0;
                    teleport_car->car_master_actor->render_style = BR_RSTYLE_NONE;
                    PipeSingleOppoRenderage(pOpponent_spec, 0);
                    gActive_car_list_rebuild_required = 1;
                }
                RebuildActiveCarList();
                NewObjective(pOpponent_spec, eOOT_wait_for_some_hapless_sod);
                break;
            }
        }
        if (pOpponent_spec->return_to_start_data.waiting_near_start) {
            pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->collision_info->M * 15.0f;
        } else {
            BrVector3Copy(&our_pos_xz, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            our_pos_xz.v[1] = 0.0f;
            BrVector3Sub(&cop_to_start, &pOpponent_spec->start_pos, &our_pos_xz);
            if (BrVector3Length(&cop_to_start) >= 10.0f) {
                if (pOpponent_spec->follow_path_data.section_no > 20000) {
                    ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
                    pOpponent_spec->follow_path_data.section_no = 20000;
                }
                if (pOpponent_spec->nnext_sections < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - 1) {
                    CalcReturnToStartPointRoute(pOpponent_spec);
                }
                res = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
                if (res == eFPR_given_up || res == eFPR_end_of_path) {
                    if (res == eFPR_given_up) {
                        DoNotDprintf_opponent("%s: Restarting return_to_start route because ProcessFollowPath() gave up.", pOpponent_spec->car_spec->driver_name);
                    } else {
                        DoNotDprintf_opponent("%s: Restarting return_to_start route because ran out of path!", pOpponent_spec->car_spec->driver_name);
                    }
                    ClearOpponentsProjectedRoute(pOpponent_spec);
                    CalcReturnToStartPointRoute(pOpponent_spec);
                    ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
                }
            } else {
                pOpponent_spec->return_to_start_data.waiting_near_start = 1;
                pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->collision_info->M * 15.0f;
            }
        }
        break;
    default:
        break;
    }
}

void C2_HOOK_FASTCALL ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->nnext_sections = 0;
}

void C2_HOOK_FASTCALL ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    tFollow_path_result result;

    switch (pCommand) {
    case ePOC_run:
        if (pOpponent_spec->follow_path_data.section_no > 20000) {
            ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
            pOpponent_spec->follow_path_data.section_no = 20000;
        }
        if (pOpponent_spec->nnext_sections <= 10 && !pOpponent_spec->complete_race_data.finished_calcing_race_route) {
            CalcRaceRoute(pOpponent_spec);
        }
        // The original leaves result uninitialised when the route is already empty.
        if (pOpponent_spec->nnext_sections == 0 || (result = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0)) == eFPR_end_of_path) {
            DoNotDprintf_opponent("%s: Giving up following race path because ran out of race path", pOpponent_spec->car_spec->driver_name);
            NewObjective(pOpponent_spec, eOOT_get_near_player);
        }
        if (result == eFPR_OK) {
            break;
        }
        if (result != eFPR_given_up) {
            ObjectiveComplete(pOpponent_spec);
            break;
        }
        goto restart_race_path;
    case ePOC_start:
        DoNotDprintf_opponent("%s: ProcessCompleteRace() - new objective started", pOpponent_spec->car_spec->driver_name);
    restart_race_path:
        ClearOpponentsProjectedRoute(pOpponent_spec);
        CalcRaceRoute(pOpponent_spec);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        break;
    default:
        break;
    }
}

void C2_HOOK_FASTCALL ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    tS16 section_no;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    char str[256];

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, run_away_data, 0x190);

    switch (pCommand) {
    case ePOC_start:
        DoNotDprintf_opponent("%s: ProcessRunAway() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->run_away_data.time_to_stop = gTime_stamp_for_this_munging + 1000 * IRandomBetween(30, 90);
        ClearOpponentsProjectedRoute(pOpponent_spec);
        section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
        if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) < 0.0f) {
            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
        } else {
            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
        }
        TopUpRandomRoute(pOpponent_spec, -1);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        sprintf(str, "%s: Shit! I'm out of here...", pOpponent_spec->car_spec->driver_name);
        break;
    case ePOC_run:
        if (gTime_stamp_for_this_munging <= pOpponent_spec->run_away_data.time_to_stop) {
            if (pOpponent_spec->follow_path_data.section_no > 20000) {
                ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
                pOpponent_spec->follow_path_data.section_no = 20000;
            }
            if (pOpponent_spec->nnext_sections < CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections)) {
                TopUpRandomRoute(pOpponent_spec, CARPOCALYPSE2_ASIZE(pOpponent_spec->next_sections) - pOpponent_spec->nnext_sections);
            }
            if (ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0) == eFPR_given_up) {
                ClearOpponentsProjectedRoute(pOpponent_spec);
                section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
                if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) < 0.0f) {
                    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
                } else {
                    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
                }
                TopUpRandomRoute(pOpponent_spec, -1);
                ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
            }
        } else {
            ObjectiveComplete(pOpponent_spec);
        }
        break;
    default:
        break;
    }
}

void C2_HOOK_FASTCALL ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pCommand) {
    case ePOC_start:
        DoNotDprintf_opponent("%d ProcessFrozen() - new task started", pOpponent_spec->index);
        DoNotDprintf_opponent("%s: Rematerialising from ePOC_start in ProcessFrozen()...", pOpponent_spec->car_spec->driver_name);
        RematerialiseOpponentOnNearestSection(pOpponent_spec, 0.f);
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
        break;
    case ePOC_run:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->collision_info->M;
        break;
    case ePOC_die:
        pOpponent_spec->car_spec->brake_force = 0.f;
        break;
    }
}

#pragma inline_depth(0)
// FUNCTION: CARMA2_HW 0x004aa900
void C2_HOOK_FASTCALL ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pOpponent_spec->current_objective) {
    case eOOT_complete_race:
        {
            tFollow_path_result result;

            switch (pCommand) {
            case ePOC_run:
                if (pOpponent_spec->follow_path_data.section_no > 20000) {
                    ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
                    pOpponent_spec->follow_path_data.section_no = 20000;
                }
                if (pOpponent_spec->nnext_sections <= 10 && !pOpponent_spec->complete_race_data.finished_calcing_race_route) {
                    CalcRaceRoute(pOpponent_spec);
                }
                // The original leaves result uninitialised when the route is already empty.
                if (pOpponent_spec->nnext_sections == 0 || (result = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0)) == eFPR_end_of_path) {
                    DoNotDprintf_opponent("%s: Giving up following race path because ran out of race path", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_get_near_player);
                }
                if (result == eFPR_OK) {
                    break;
                }
                if (result != eFPR_given_up) {
                    DoNotDprintf_opponent("%s: Objective Completed", pOpponent_spec->car_spec->driver_name);
                    pOpponent_spec->new_objective_required = 1;
                    switch (pOpponent_spec->current_objective) {
                    case eOOT_complete_race:
                        gNum_of_opponents_completing_race -= 1;
                        break;
                    case eOOT_pursue_and_twat:
                        gNum_of_opponents_pursuing -= 1;
                        break;
                    case eOOT_get_near_player:
                        gNum_of_opponents_getting_near -= 1;
                        break;
                    default:
                        break;
                    }
                    break;
                }
                goto restart_race_path;
            case ePOC_start:
                DoNotDprintf_opponent("%s: ProcessCompleteRace() - new objective started", pOpponent_spec->car_spec->driver_name);
            restart_race_path:
                pOpponent_spec->nnext_sections = 0;
                CalcRaceRoute(pOpponent_spec);
                ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
                break;
            default:
                break;
            }
        }
        break;
    case eOOT_pursue_and_twat:
        ProcessPursueAndTwat(pOpponent_spec, pCommand);
        break;
    case eOOT_run_away:
        {
            tS16 section_no;
            br_scalar distance;
            br_vector3 intersect;
            br_vector3 direction_v;
            char str[256];

            C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tOpponent_spec, run_away_data, 0x190);

            switch (pCommand) {
            case ePOC_start:
                DoNotDprintf_opponent("%s: ProcessRunAway() - new objective started", pOpponent_spec->car_spec->driver_name);
                pOpponent_spec->run_away_data.time_to_stop = gTime_stamp_for_this_munging + 1000 * IRandomBetween(30, 90);
                pOpponent_spec->nnext_sections = 0;
                section_no = FindNearestGeneralSection(NULL, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
                if (BR_MAC3(pOpponent_spec->car_spec->direction.v[1], direction_v.v[1], pOpponent_spec->car_spec->direction.v[0], direction_v.v[0], pOpponent_spec->car_spec->direction.v[2], direction_v.v[2]) < gOppo_render_zero) {
                    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
                } else {
                    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
                }
                TopUpRandomRoute(pOpponent_spec, -1);
                ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
                sprintf(str, "%s: Shit! I'm out of here...", pOpponent_spec->car_spec->driver_name);
                break;
            case ePOC_run:
                if (gTime_stamp_for_this_munging > pOpponent_spec->run_away_data.time_to_stop) {
                    ObjectiveComplete(pOpponent_spec);
                    return;
                }
                {
                    if (pOpponent_spec->follow_path_data.section_no > 20000) {
                        ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
                        pOpponent_spec->follow_path_data.section_no = 20000;
                    }
                    if (pOpponent_spec->nnext_sections < 12) {
                        TopUpRandomRoute(pOpponent_spec, 12 - pOpponent_spec->nnext_sections);
                    }
                    if (ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0) == eFPR_given_up) {
                        pOpponent_spec->nnext_sections = 0;
                        section_no = FindNearestGeneralSection(NULL, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
                        if (BR_MAC3(pOpponent_spec->car_spec->direction.v[1], direction_v.v[1], pOpponent_spec->car_spec->direction.v[0], direction_v.v[0], pOpponent_spec->car_spec->direction.v[2], direction_v.v[2]) < gOppo_render_zero) {
                            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
                        } else {
                            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
                        }
                        TopUpRandomRoute(pOpponent_spec, -1);
                        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
                    }
                }
                break;
            default:
                break;
            }
        }
        break;
    case eOOT_get_near_player:
        ProcessGetNearPlayer(pOpponent_spec, pCommand);
        break;
    case eOOT_levitate:
        ProcessLevitate(pOpponent_spec, pCommand);
        break;
    case eOOT_knackered_and_freewheeling:
        memset(&pOpponent_spec->car_spec->keys, 0, sizeof(pOpponent_spec->car_spec->keys));
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->car_spec->brake_force = 0.f;
        pOpponent_spec->car_spec->curvature = 0.f;
        break;
    case eOOT_frozen:
        {
            switch (pCommand) {
            case ePOC_start:
                DoNotDprintf_opponent("%d ProcessFrozen() - new task started", pOpponent_spec->index);
                DoNotDprintf_opponent("%s: Rematerialising from ePOC_start in ProcessFrozen()...", pOpponent_spec->car_spec->driver_name);
                RematerialiseOpponentOnNearestSection(pOpponent_spec, 0.f);
                pOpponent_spec->car_spec->acc_force = 0.f;
                pOpponent_spec->car_spec->brake_force = gOppo_pursuit_reverse_brake * pOpponent_spec->car_spec->collision_info->M;
                break;
            case ePOC_run:
                pOpponent_spec->car_spec->brake_force = gOppo_pursuit_reverse_brake * pOpponent_spec->car_spec->collision_info->M;
                break;
            case ePOC_die:
                pOpponent_spec->car_spec->brake_force = 0.f;
                break;
            }
        }
        break;
    case eOOT_wait_for_some_hapless_sod:
        ProcessWaitForSomeHaplessSod(pOpponent_spec, pCommand);
        break;
    case eOOT_return_to_start:
        ProcessReturnToStart(pOpponent_spec, pCommand);
        break;
    default:
        break;
    }
}
#pragma inline_depth()

// C2PORT-NONMATCHING: best 56.52% | difftest PASS 2000 trials (spec difftest/specs/004a9180_TeleportOpponentToNearestSafeLocation.json) | reason VC5 x87, stack and route-loop layout
// FUNCTION: CARMA2_HW 0x004a9180
void C2_HOOK_FASTCALL TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tU8 section_direction;
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    tCar_spec* car;
    int section_counter;
    int found_safe_place;

    found_safe_place = 0;
    section_no = FindNearestGeneralSection(NULL, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
    if (section_no < 0) {
        return;
    }
    pOpponent_spec->last_in_view = 0;
    car = pOpponent_spec->car_spec;
    car->acc_force = 0.f;
    car->brake_force = 0.f;
    car->curvature = 0.f;
    car->gear = 0;
    car->revs = 0.f;
    BrVector3Set(&car->collision_info->omega, 0.f, 0.f, 0.f);
    BrVector3Set(&car->collision_info->v, 0.f, 0.f, 0.f);
    if (pOpponent_spec->physics_me) {
        pOpponent_spec->physics_me = 0;
        car->car_master_actor->render_style = BR_RSTYLE_NONE;
        PipeSingleOppoRenderage(pOpponent_spec, 0);
        gActive_car_list_rebuild_required = 1;
    }
    section_direction = BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) < 0.0f;
    ClearOpponentsProjectedRoute(pOpponent_spec);
    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, section_direction);
    TopUpRandomRoute(pOpponent_spec, -1);
    section_counter = 1;
#ifndef CARPOCALYPSE2_MATCHING
    {
    /* port: on a small track (or with a NaN player position) no route node may be out of view, and the original
     * loop then never ends (Vita 00.75 test: hang in the AI, watchdog core dump). After 2000 nodes the farthest
     * one seen is used. */
    int c2_tries = 0;
    br_scalar c2_best_d = -1.f;
    br_vector3 c2_best_pos;
    BrVector3Copy(&c2_best_pos, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
#endif
    while (!found_safe_place) {
        pOpponent_spec->car_spec->car_master_actor->t.t.translate.t = gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[section_counter].section_no].node_indices[pOpponent_spec->next_sections[section_counter].direction]].pos;
        CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(pOpponent_spec);
        if (pOpponent_spec->player_to_oppo_d > gIn_view_distance) {
            found_safe_place = 1;
        }
#ifndef CARPOCALYPSE2_MATCHING
        if (pOpponent_spec->player_to_oppo_d > c2_best_d) {
            c2_best_d = pOpponent_spec->player_to_oppo_d;
            BrVector3Copy(&c2_best_pos, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        }
        if (!found_safe_place && ++c2_tries >= 2000) {
            BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &c2_best_pos);
            CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(pOpponent_spec);
            dr_dprintf("TELEPORT '%s': no node out of view after %d tries, farthest %.2f used (view %.2f)",
                pOpponent_spec->car_spec->driver_name, c2_tries, c2_best_d, gIn_view_distance);
            break;
        }
#endif
        section_counter++;
        if (pOpponent_spec->nnext_sections <= section_counter) {
            ShiftOpponentsProjectedRoute(pOpponent_spec, section_counter - 1);
            section_counter = 0;
            TopUpRandomRoute(pOpponent_spec, -1);
        }
    }
#ifndef CARPOCALYPSE2_MATCHING
    }
#endif
}

// FUNCTION: CARMA2_HW 0x004aad70
void C2_HOOK_CDECL NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...) {
    va_list ap;

    if (pOpponent_spec->current_objective != eOOT_none) {
        ProcessCurrentObjective(pOpponent_spec, ePOC_die);
    }
    pOpponent_spec->current_objective = pObjective_type;
    pOpponent_spec->time_this_objective_started = gTime_stamp_for_this_munging;
    pOpponent_spec->time_for_this_objective_to_finish = gTime_stamp_for_this_munging + IRandomBetween(20, 60) * 1000;
    if (pObjective_type == eOOT_pursue_and_twat) {
        pOpponent_spec->time_for_this_objective_to_finish += 90000;
    }
    switch (pObjective_type) {
    case eOOT_complete_race:
        gNum_of_opponents_completing_race += 1;
        break;
    case eOOT_pursue_and_twat:
        va_start(ap, pObjective_type);
        pOpponent_spec->pursue_car_data.pursuee = va_arg(ap, tCar_spec*);
        va_end(ap);
        gNum_of_opponents_pursuing += 1;
        break;
    case eOOT_get_near_player:
        gNum_of_opponents_getting_near += 1;
        break;
    default:
        break;
    }
    DoNotDprintf_opponent("%s: NewObjective() - type %d", pOpponent_spec->car_spec->driver_name, pObjective_type);
    ProcessCurrentObjective(pOpponent_spec, ePOC_start);
}

tS16 C2_HOOK_FASTCALL CalcNextTrailSection(const tOpponent_spec* pOpponent_spec, int pSection) {
    int section_no;
    tPursuee_trail* trail;

    trail = &pOpponent_spec->pursue_car_data.pursuee->my_trail;
    section_no = pSection - 15000;

    if (trail->number_of_nodes - 2 > section_no) {
        return pSection + 1;
    }
    return -1;
}

// FUNCTION: CARMA2_HW 0x004aea30
int C2_HOOK_FASTCALL GetOpponentsNextSection(const tOpponent_spec* pOpponent_spec, tS16 pCurrent_section) {
#ifdef CARPOCALYPSE2_MATCHING
    if (pCurrent_section >= 20000) {
        if (pOpponent_spec->nnext_sections > pCurrent_section - 19999
                && (pOpponent_spec->cheating || gProgram_state.AI_vehicles.path_sections[pCurrent_section - 19999].type != ePST_cheat_only)) {
            return pCurrent_section + 1;
        }
        return -1;
    }
    if (pCurrent_section >= 15000) {
        int trail_section = pCurrent_section - 15000;

        if (trail_section >= pOpponent_spec->pursue_car_data.pursuee->my_trail.number_of_nodes - 2) {
            trail_section = -1;
        } else {
            trail_section += 15001;
        }
        return (tS16)trail_section;
    }
    return -1;
#else
    if (pCurrent_section < 20000) {
        if (pCurrent_section < 15000) {
            return -1;
        } else {
            return CalcNextTrailSection(pOpponent_spec, pCurrent_section);
        }
    } else if (pCurrent_section - 19999 >= pOpponent_spec->nnext_sections || (!pOpponent_spec->cheating && gProgram_state.AI_vehicles.path_sections[pCurrent_section - 19999].type == ePST_cheat_only)) {
        return -1;
    } else {
        return pCurrent_section + 1;
    }
#endif
}

// C2PORT-NONMATCHING: best 75.00% | difftest PASS 2000 trials (spec difftest/specs/004aeab0_GetOpponentsSectionStartNode.json) | reason vc5-register-allocation
// FUNCTION: CARMA2_HW 0x004aeab0
tU16 C2_HOOK_FASTCALL GetOpponentsSectionStartNode(const tOpponent_spec* pOpponent, tS16 pSection) {
    int section;
    const tU8* opponent_bytes = (const tU8*)pOpponent;

    if (pSection >= 20000) {
        section = (int)pSection;
        if (pOpponent->nnext_sections > section - 20000) {
            return *(tU16*)(gOppo_junction_table
                + ((unsigned)(*(const tU8*)(opponent_bytes + section * 4 - 0x137fe) == 0)
                    + *(const tS16*)(opponent_bytes + section * 4 - 0x13800) * 10) * 2);
        }
    }
    DoNotDprintf_opponent("BIG ERROR - GetOpponentsSectionStartNode() - section not found in next_section array for opponent %s", pOpponent->car_spec->driver_name);
    PDEnterDebugger("BIG ERROR - GetOpponentsSectionStartNode()");
    return 0xffff;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 49.02% | difftest PASS 2000 trials (spec difftest/specs/004aeb90_GetOpponentsSectionStartNodePoint.json) | reason VC5 route-table and pointer register scheduling
// FUNCTION: CARMA2_HW 0x004aeb90
const br_vector3* C2_HOOK_FASTCALL GetOpponentsSectionStartNodePoint(const tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 node_no;

    if (pSection >= 20000 && pOpponent_spec->nnext_sections > pSection - 20000) {
        tU8* route = (tU8*)pOpponent_spec + (tS32)pSection * 4 - 0x13800;
        tS16 junction = *(tS16*)route;
        tU8 direction = route[2];
        int junction_offset = junction * 10;
        node_no = ((tS16*)gOppo_junction_table)[junction_offset + (direction == 0)];
        return (const br_vector3*)((tU8*)gOppo_path_nodes + node_no * 0x20);
    }

    if (pSection >= 15000) {
        return &pOpponent_spec->pursue_car_data.pursuee->my_trail.trail_nodes[pSection - 15000];
    }
    if (pSection == 10000) {
        return &pOpponent_spec->pursue_car_data.direct_line_nodes[0].pos;
    }
    DoNotDprintf_opponent("BIG ERROR - GetOpponentsSectionStartNodePoint() - section not found in next_section array for opponent %s", pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("BIG ERROR - GetOpponentsSectionStartNodePoint()");
    return NULL;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: CARMA2_HW 0x004aec40
const br_vector3* C2_HOOK_FASTCALL GetOpponentsSectionFinishNodePoint(const tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;

    if (pSection >= 20000 && pOpponent_spec->nnext_sections > pSection - 20000) {
        section_no = pOpponent_spec->next_sections[pSection - 20000].section_no;
        node_index_index = pOpponent_spec->next_sections[pSection - 20000].direction;
        node_no = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[node_index_index];
        return &gProgram_state.AI_vehicles.path_nodes[node_no].pos;
    } else if (pSection >= 15000) {
        return &pOpponent_spec->pursue_car_data.pursuee->my_trail.trail_nodes[(pSection + 1) - 15000];
    } else if (pSection == 10000) {
        return &pOpponent_spec->pursue_car_data.direct_line_nodes[1].pos;
    } else {
        dr_dprintf("BIG ERROR - GetOpponentsSectionFinishNodePoint() - section not found in next_section array for opponent %s",
                   pOpponent_spec->car_spec->driver_name);
        PDEnterDebugger("BIG ERROR - GetOpponentsSectionFinishNodePoint()");
        return NULL;
    }
}
#pragma auto_inline(on)

// FUNCTION: CARMA2_HW 0x004aea00
int C2_HOOK_FASTCALL GetOpponentsFirstSection(const tOpponent_spec* pOpponent_spec) {

    if (pOpponent_spec->current_objective == eOOT_pursue_and_twat) {
        if (pOpponent_spec->pursue_car_data.state == ePCS_following_trail) {
            return pOpponent_spec->follow_path_data.section_no;
        }
        if (pOpponent_spec->pursue_car_data.state == ePCS_following_line_of_sight) {
            return 10000;
        }
    }
    return 20000;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 47.83% | difftest PASS 2000 trials (spec difftest/specs/004aedf0_GetOpponentsSectionMaxSpeed.json) | reason VC5 selects route speed and fallback paths with different code layout
// FUNCTION: CARMA2_HW 0x004aedf0
tS16 C2_HOOK_FASTCALL GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    if (pSection >= 20000) {
        int sections = pOpponent_spec->nnext_sections;
        int s = pSection - 20000;
        if (sections > s) {
            tS16 idx = *(tS16 *)((tU8 *)pOpponent_spec + (tS32)pSection * 4 - 0x13800);
            tU8 rt_dir = *(tU8 *)((tU8 *)pOpponent_spec + (tS32)pSection * 4 - 0x137fe);
            tU8* tab = (tU8 *)gOppo_junction_table + (idx * 5 * 4) + 6 + (rt_dir == pTowards_finish);
            return *tab;
        }
    }
    if (pSection >= 15000) {
        return 0xff;
    }
    if (pSection == 10000) {
        return *(tU8 *)((tU8 *)pOpponent_spec + 0x172 + pTowards_finish);
    }
    DoNotDprintf_opponent("WARNING - GetOpponentsSectionMaxSpeed() - section not found in next_section array for opponent %s", (tU8 *)pOpponent_spec->car_spec + 0xec);
    PDEnterDebugger("WARNING - GetOpponentsSectionMaxSpeed()");
    return 0xff;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 64.52% | difftest PASS 2000 trials (spec difftest/specs/004aed50_GetOpponentsSectionMinSpeed.json) | reason VC5 register allocation and byte-return codegen
// FUNCTION: CARMA2_HW 0x004aed50
tS16 C2_HOOK_FASTCALL GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    if (pSection >= 20000) {
        int section_index = pSection - 20000;
        if (pOpponent_spec->nnext_sections > section_index) {
            tRoute_section* section = &pOpponent_spec->next_sections[section_index];
            return *((tU8 *)gOppo_junction_table + section->section_no * 20 + 4 + (section->direction == pTowards_finish));
        }
    }
    if (pSection >= 15000) {
        return 0;
    }
    if (pSection == 10000) {
        return *(tU8 *)((tU8 *)pOpponent_spec + 0x170 + pTowards_finish);
    }
    DoNotDprintf_opponent("WARNING - GetOpponentsSectionMinSpeed() - section not found in next_section array for opponent %s", (tU8 *)pOpponent_spec->car_spec + 0xec);
    PDEnterDebugger("WARNING - GetOpponentsSectionMinSpeed()");
    return 0;
}

// FUNCTION: CARMA2_HW 0x004a7f20
int C2_HOOK_FASTCALL RematerialiseOpponentOnThisSection(tOpponent_spec* pOpponent_spec, tS16 pSection_no, float pSpeed) {
    const br_vector3* start;
    const br_vector3* finish;
    br_vector3 a;
    br_vector3 p;
    br_vector3 section_v;
    br_scalar t;
    br_scalar length;

    if (pOpponent_spec->physics_me) {
        DoNotDprintf_opponent("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }

    if (pSection_no >= 20000 && pOpponent_spec->nnext_sections > pSection_no - 20000) {
        tS16 section_no = pOpponent_spec->next_sections[pSection_no - 20000].section_no;
        tU8 direction = pOpponent_spec->next_sections[pSection_no - 20000].direction;
        tS16 node_no = ((tS16*)gOppo_junction_table)[section_no * 10 + (tS32)(direction == 0)];
        start = &gOppo_path_nodes[node_no].pos;
    } else if (pSection_no >= 15000) {
        start = (br_vector3*)((tU8*)pOpponent_spec->pursue_car_data.pursuee + (pSection_no - 0x38ee) * 12);
    } else if (pSection_no == 10000) {
        start = &pOpponent_spec->pursue_car_data.direct_line_nodes[0].pos;
    } else {
        DoNotDprintf_opponent("BIG ERROR - GetOpponentsSectionStartNodePoint() - section not found in next_section array for opponent %s", pOpponent_spec->car_spec->driver_name);
        PDEnterDebugger("BIG ERROR - GetOpponentsSectionStartNodePoint()");
        start = NULL;
    }

    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pSection_no);

    a.v[0] = finish->v[0] - start->v[0];
    a.v[1] = finish->v[1] - start->v[1];
    a.v[2] = finish->v[2] - start->v[2];
    length = sqrtf(a.v[0] * a.v[0] + a.v[1] * a.v[1] + a.v[2] * a.v[2]);
    if (length != gOppo_render_zero) {
        p.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] - start->v[0];
        p.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] - start->v[1];
        p.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] - start->v[2];
        t = (a.v[0] * p.v[0] + a.v[1] * p.v[1] + a.v[2] * p.v[2]) / (a.v[0] * a.v[0] + a.v[1] * a.v[1] + a.v[2] * a.v[2]);
        if (t < gOppo_render_zero) {
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] = start->v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] = start->v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] = start->v[2];
        } else if (t > 1.f) {
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] = finish->v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] = finish->v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] = finish->v[2];
        } else {
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] = start->v[0] + a.v[0] * t;
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] = start->v[1] + a.v[1] * t;
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] = start->v[2] + a.v[2] * t;
        }
section_v.v[0] = finish->v[0] - start->v[0];
            section_v.v[1] = finish->v[1] - start->v[1];
            section_v.v[2] = finish->v[2] - start->v[2];
            PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &section_v);
        }
        return RematerialiseOpponent(pOpponent_spec, pSpeed);
    }

// FUNCTION: CARMA2_HW 0x004aece0
float C2_HOOK_FASTCALL GetOpponentsSectionWidth(const tOpponent_spec* pOpponent_spec, tS16 pSection) {
    if (pSection >= 20000) {
        int sections = pOpponent_spec->nnext_sections;
        int s = pSection - 20000;
        if (sections > s) {
            tS16 idx = *(tS16 *)((tU8 *)pOpponent_spec + (tS32)pSection * 4 - 0x13800);
            return ((float *)gOppo_junction_table)[idx * 5 + 2];
        }
    }
    if (pSection >= 15000) {
        return 0.5f;
    }
    if (pSection == 10000) {
        return *(float *)((tU8 *)pOpponent_spec + 0x174);
    }
    return ((float *)gOppo_junction_table)[(tS32)pSection * 5 + 2];
}
// DoNotDprintf

// ProcessCurrentObjective

// ReallocExtraPathNodes

// ReallocExtraPathSections

// PointVisibleFromHere

// WeightedFindNearestNodeAndSection

// FindNearestGeneralSection

// FindNearestPathSection

// DeadStopCar

// TurnOpponentPhysicsOn

// TurnOpponentPhysicsOff

// ApplyOppoRenderage2

// NewObjective

// CalcRaceRoute

// TopUpRandomRoute

// SearchForSection

// UpdatePlayersSection

// AllowForDecel

// ShiftOpponentsProjectedRoute

// NewCalcGetNearPlayerRoute

// CalcReturnToStartPointRoute

// ClearOpponentsProjectedRoute

// AddToOpponentsProjectedRoute

// StunTheBugger

// UnStunTheBugger

// ProcessCompleteRace

// StartRecordingTrail

// RecordNextTrailNode

// FindNearestTrailSection

// CalcNextTrailSection

// ProcessPursueAndTwat

// ProcessRunAway

// ProcessWaitForSomeHaplessSod

// ProcessReturnToStart

// ProcessLevitate

// ProcessGetNearPlayer

// ProcessFrozen

// HeadOnWithPlayerPossible

// AlreadyPursuingCar

// LastTwatteeAPlayer

// LastTwatterAPlayer

// ObjectiveComplete

// TeleportOpponentToNearestSafeLocation

// ChooseNewObjective

// ProcessThisOpponent

// IsNetCarActive

// NoteCarsCurrentlyUsed

// AddIfNotInList

// RemoveAnythingStillInList

// RebuildActiveCarList

// FUNCTION: CARMA2_HW 0x004a7a60
void C2_HOOK_FASTCALL ForceRebuildActiveCarList(void) {
    gActive_car_list_rebuild_required = 1;
    if (gProgram_state.racing) {
        RebuildActiveCarList();
    }
}

// OpponentRepairNecessary

// DisplayOpponentRecoveringHeadup

// StartToCheat

// OiStopCheating

// TeleportCopToStart

// CalcDistanceFromHome

// MassageOpponentPosition

// RematerialiseOpponentOnThisSection

// RematerialiseOpponentOnNearestSection

// RematerialiseOpponent

// CalcPlayerConspicuousness

// CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck

// NumberOfOpponentsStillRunning

// ResetPredominantJobbies

// LoadCopCars

// LoadInOppoPaths

// DisposeOpponentPaths

// MungeOpponents

// InitOpponents

// DisposeOpponents

// WakeUpOpponentsToTheFactThatTheStartHasBeenJumped

// FUNCTION: CARMA2_HW 0x004ae750
void C2_HOOK_FASTCALL ReportMurderToAllCops(tCar_spec* pCar) {
    int i;

    if (pCar == &gProgram_state.current_car) {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            gProgram_state.AI_vehicles.cops[i].murder_reported = 1;
        }
    }
}

// FUNCTION: CARMA2_HW 0x004ae790
int C2_HOOK_FASTCALL GetCarCount(tVehicle_type pCategory) {
#ifdef CARPOCALYPSE2_MATCHING
    switch (pCategory) {
    case eVehicle_self:
        return 1;
    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.number_of_opponents;
    case eVehicle_net_player:
        if (gNet_mode != eNet_mode_none) {
            return gNumber_of_net_players - 1;
        } else {
            return 0;
        }
    case eVehicle_rozzer:
        return gNumber_of_cops_before_faffage;
    case eVehicle_drone:
        return 0;
    case eVehicle_not_really:
        return gNum_active_non_cars;
    }
    return 0;
#else
    switch (pCategory) {
    case eVehicle_self:
        return 1;
    case eVehicle_net_player:
        return gNumber_of_net_players;
    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.number_of_opponents;
    case eVehicle_rozzer:
        return gNumber_of_cops_before_faffage;
    case eVehicle_drone:
    case eVehicle_not_really:
    default:
        return 0;
    }
#endif
}

// FUNCTION: CARMA2_HW 0x004ae7e0
tCar_spec* C2_HOOK_FASTCALL GetCarSpec(tVehicle_type pCategory, int pIndex) {
#ifdef CARPOCALYPSE2_MATCHING
    switch (pCategory) {
    case eVehicle_self:
        return &gProgram_state.current_car;
    case eVehicle_net_player:
        if (pIndex < gThis_net_player_index) {
            return gNet_players[pIndex].car;
        } else {
            return (&gNet_players[1])[pIndex].car;
        }
    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.opponents[pIndex].car_spec;
    case eVehicle_rozzer:
        return gProgram_state.AI_vehicles.cops[pIndex].car_spec;
    case eVehicle_drone:
        PDEnterDebugger("OPPONENT.C: GetCarSpec() can't return drone car_specs");
        return NULL;
    case eVehicle_not_really:
        return (tCar_spec*)gActive_non_car_list[pIndex];
    default:
        return NULL;
    }
#else
    switch (pCategory) {
    case eVehicle_self:
        return &gProgram_state.current_car;
    case eVehicle_net_player:
        if (pIndex < gThis_net_player_index) {
            return gNet_players[pIndex].car;
        }
        return gNet_players[pIndex + 1].car;
    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.opponents[pIndex].car_spec;
    case eVehicle_rozzer:
        return gProgram_state.AI_vehicles.cops[pIndex].car_spec;
    case eVehicle_drone:
        PDEnterDebugger("OPPONENT.C: GetCarSpec() can't return drone car_specs");
        return NULL;
    case eVehicle_not_really:
        return (tCar_spec*)gActive_non_car_list[pIndex];
    default:
        return NULL;
    }
#endif
}

// GetDriverName

// GetOpponentSpecFromCarSpec

// GetCarSpecFromGlobalOppoIndex

// GetOpponentsRealSection

// GetOpponentsFirstSection

// GetOpponentsNextSection

// GetOpponentsSectionStartNodePoint

// GetOpponentsSectionFinishNodePoint

// GetOpponentsSectionWidth

// GetOpponentsSectionMinSpeed

// GetOpponentsSectionMaxSpeed

// FUNCTION: CARMA2_HW 0x004aee90
void C2_HOOK_FASTCALL InitOpponentPsyche(int pOpponent_index) {

    gOpponents[pOpponent_index].psyche.grudge_against_player = 0;
}

// ClearTwattageOccurrenceVariables

// TwoCarsHitEachOther

// RecordOpponentTwattageOccurrence

// GetOpponentMood

