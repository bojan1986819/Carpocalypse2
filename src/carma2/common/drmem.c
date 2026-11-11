#include "drmem.h"

#include "errors.h"
#include "loading.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"

#ifdef __vita__
#include "sdl3/vita_controls.h"
#include "sdl3/vita_pack.h"
#endif

#include "brender/brender.h"

#include <stdio.h>
#include <stdlib.h>
#include "c2_stdlib.h"
#include "drmem.h"

#include "brender/brender.h"
#include "errors.h"

#include "carpocalypse2_macros.h"
#include "carpocalypse2_types.h"

#ifdef CARPOCALYPSE2_MATCHING
#include <windows.h>
#endif
// GLOBAL: CARMA2_HW 0x005933e8
extern const char* gMem_names[256];

// GLOBAL: CARMA2_HW 0x006815c8
br_resource_class gStainless_classes[126];

// GLOBAL: CARMA2_HW 0x005937e8
br_allocator gAllocator = {
    "Death Race",
    DRStdlibAllocate,
    DRStdlibFree,
    DRStdlibInquire,
    Claim4ByteAlignment
};

// GLOBAL: CARMA2_HW 0x00681fa4
int gNon_fatal_allocation_errors;

// GLOBAL: CARMA2_HW 0x005933e8
const char* gMem_names[256] = {
    "",
    "BR_MEMORY_SCRATCH",
    "BR_MEMORY_PIXELMAP",
    "BR_MEMORY_PIXELS",
    "BR_MEMORY_VERTICES",
    "BR_MEMORY_FACES",
    "BR_MEMORY_GROUPS",
    "BR_MEMORY_MODEL",
    "BR_MEMORY_MATERIAL",
    "BR_MEMORY_MATERIAL_INDEX",
    "BR_MEMORY_ACTOR",
    "BR_MEMORY_PREPARED_VERTICES",
    "BR_MEMORY_PREPARED_FACES",
    "BR_MEMORY_LIGHT",
    "BR_MEMORY_CAMERA",
    "BR_MEMORY_BOUNDS",
    "BR_MEMORY_CLIP_PLANE",
    "BR_MEMORY_STRING",
    "BR_MEMORY_REGISTRY",
    "BR_MEMORY_TRANSFORM",
    "BR_MEMORY_RESOURCE_CLASS",
    "BR_MEMORY_FILE",
    "BR_MEMORY_ANCHOR",
    "BR_MEMORY_POOL",
    "BR_MEMORY_RENDER_MATERIAL",
    "BR_MEMORY_DATAFILE",
    "BR_MEMORY_IMAGE",
    "BR_MEMORY_IMAGE_ARENA",
    "BR_MEMORY_IMAGE_SECTIONS",
    "BR_MEMORY_IMAGE_NAMES",
    "BR_MEMORY_EXCEPTION_HANDLER",
    "BR_MEMORY_RENDER_DATA",
    "BR_MEMORY_TOKEN",
    "BR_MEMORY_TOKEN_MAP",
    "BR_MEMORY_OBJECT",
    "BR_MEMORY_OBJECT_DATA",
    "BR_MEMORY_DRIVER",
    "BR_MEMORY_LEXER",
    "BR_MEMORY_OBJECT_LIST",
    "BR_MEMORY_OBJECT_LIST_ENTRY",
    "BR_MEMORY_ENABLED_ACTORS",
    "BR_MEMORY_FMT_RESULTS",
    "BR_MEMORY_PREPARED_MODEL",
    "BR_MEMORY_ORDER_TABLE",
    "BR_MEMORY_TOKEN_VALUE",
    "BR_MEMORY_TOKEN_TEMPLATE",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "kBoner_mem_type_remap",
    "kBoner_mem_type_sequence",
    "kMem_nodes_array",
    "kMem_sections_array",
    "kBoner_mem_type_frame_2",
    "kMem_columns_z",
    "kMem_columns_x",
    "kMem_non_car_list",
    "kMem_simp_level",
    "kMem_crush_data",
    "kMem_drone_collision_stuff",
    "kBoner_mem_type_form",
    "kBoner_mem_type_actor_sets",
    "kBoner_mem_type_simple_phys",
    "kBoner_mem_type_boned_phys",
    "kBoner_mem_type_bone_info",
    "kBoner_mem_type_boned_coll",
    "kBoner_mem_type_boned_coll_obj",
    "kBoner_mem_type_simple_coll_obj",
    "kBoner_mem_type_actor_ptrs",
    "kBoner_mem_type_moves",
    "kBoner_mem_type_stored_ptrs",
    "kBoner_mem_type_stored",
    "kBoner_mem_type_person",
    "kBoner_mem_type_person_parts",
    "kBoner_mem_type_person_moves",
    "kBoner_mem_type_char_instance",
    "kMem_boner_type_frames",
    "kMem_smash_levels",
    "kMem_smash_side_effects",
    "kMem_damage_clauses",
    "kMem_undamaged_vertices",
    "kMem_race_text_chunk",
    "kMem_race_text_str",
    "kMem_oppo_array",
    "kMem_oppo_text_chunk",
    "kMem_oppo_text_str",
    "kMem_br_font",
    "kMem_br_font_wid",
    "kMem_br_font_enc",
    "kMem_br_font_glyphs",
    "kMem_oppo_car_spec",
    "kMem_misc_string",
    "kMem_mac_host_buffer_1",
    "kMem_mac_host_buffer_2",
    "kMem_mac_net_details",
    "kMem_back_pixels",
    "kMem_quit_vfy_pixels",
    "kMem_quit_vfy_pal",
    "kMem_net_min_messages",
    "kMem_net_mid_messages",
    "kMem_net_max_messages",
    "kMem_net_pid_details",
    "kMem_net_car_spec",
    "kMem_dynamic_message",
    "kMem_player_list_join",
    "kMem_player_list_leave",
    "kMem_oppo_new_nodes",
    "kMem_oppo_new_sections",
    "kMem_drone_model_dup",
    "kMem_oppo_bit_per_node",
    "kMem_smash_poly_array",
    "kMem_smash_vertex_array",
    "kMem_ped_array_stain",
    "kMem_ped_array",
    "kMem_ped_instructions",
    "kMem_ped_new_instruc",
    "kMem_pipe_model_geometry",
    "kMem_powerup_array",
    "kMem_powerup_parms",
    "kMem_noisey_parts",
    "kMem_sound_generator",
    "kMem_pratcam_flic_array",
    "kMem_pratcam_flic_data",
    "kMem_pratcam_sequence_array",
    "kMem_pratcam_pixelmap",
    "kMem_video_pixels",
    "kMem_funk_prox_array",
    "kMem_new_mat_id",
    "kMem_packed_file",
    "kMem_stored_smash",
    "kMem_special_volume",
    "kMem_special_screen",
    "kMem_new_special_vol",
    "kMem_saved_game",
    "kMem_new_save_game",
    "kMem_stor_space_table",
    "kMem_boner",
    "kMem_ped_face_cache",
    "kMem_misc_poly_ped",
    "kMem_physics_joint",
    "kMem_funk_spec",
    "kMem_groove_spec",
    "kMem_non_car_spec",
    "kMem_S3_scan_name",
    "kMem_S3_sound_header",
    "kMem_S3_sample",
    "kMem_S3_mac_channel",
    "kMem_S3_mac_path",
    "kMem_S3_sentinel",
    "kMem_S3_outlet",
    "kMem_S3_channel",
    "kMem_S3_descriptor",
    "kMem_S3_reverse_buffer",
    "kMem_S3_source",
    "kMem_drone_specs",
    "kMem_drone_paths",
    "kMem_S3_DOS_SOS_patch",
    "kMem_S3_DOS_SOS_song_structure",
    "kMem_S3_DOS_SOS_song_data",
    "kMem_S3_Windows_95_load_WAV_file",
    "kMem_S3_Windows_95_create_temp_buffer_space_to_reverse_sample",
    "kMem_S3_Windows_95_path",
    "kMem_drone_funk_groove",
    "kMem_abuse_text",
    "kMem_action_replay_buffer",
    "kMem_master_crush_data",
    "kMem_collision_object",
    "kMem_collision_shape",
    "kMem_smashable_info",
    "kMem_smashable_env_info",
    "kMem_detached_bit_non_car_spec",
    "kMem_cloned_block",
    "kMem_explosion_data",
    "kMem_misc",
    "kMem_exception",
    NULL,
};

#ifdef CARPOCALYPSE2_MATCHING
// GLOBAL: CARMA2_HW 0x00681fa8
br_size_t gPage_size;

// GLOBAL: CARMA2_HW 0x0079ea80
SYSTEM_INFO gSystem_info;
#endif
// FUNCTION: CARMA2_HW 0x0044c810
int C2_HOOK_FASTCALL AllocationErrorsAreFatal(void) {

    return !gNon_fatal_allocation_errors;
}

#ifdef CARPOCALYPSE2_MATCHING
#endif

// FUNCTION: CARMA2_HW 0x0044c7f0
void C2_HOOK_FASTCALL SetNonFatalAllocationErrors(void) {

    gNon_fatal_allocation_errors = 1;
}

// FUNCTION: CARMA2_HW 0x0044c800
void C2_HOOK_FASTCALL ResetNonFatalAllocationErrors(void) {

    gNon_fatal_allocation_errors = 0;
}

// FUNCTION: CARMA2_HW 0x0044c820
void C2_HOOK_FASTCALL MAMSInitMem(void) {

}

#ifndef CARPOCALYPSE2_MATCHING
/* port: heap-corruption tripwire. Every registered material/model/map/table must still be a live
 * BRender resource; a NULL or freed item means something wrote over the heap (e.g. a registry entry
 * overwritten with 16-bit pixels). Logs the first bad item. */
extern br_uint_32 C2_HOOK_CDECL BrModelEnum(const char* pattern, br_model_enum_cbfn* callback, void* arg);
extern br_uint_32 C2_HOOK_CDECL BrMapEnum(const char* pattern, br_map_enum_cbfn* callback, void* arg);
static const char* gRegistry_check_title;
static int gRegistry_check_reported;

static br_uint_32 C2_HOOK_CDECL C2CheckRegisteredItem(void* pItem, void* pName) {
    const br_uint_8* p = pItem;
    int skip = 0;

    if (p != NULL) {
        while (skip < 8 && p[-1] == 0) {
            p--;
            skip++;
        }
        if (*(const br_uint_32*)(p - 4) == 0xdeadbeef) {
            return 0;
        }
    }
    if (gRegistry_check_reported < 8) {
        gRegistry_check_reported++;
        dr_dprintf("REGISTRY CORRUPT %s item=%p at '%s'", (const char*)pName, pItem, gRegistry_check_title);
#ifdef __vita__
        {
            extern void C2VitaLog(const char* pFormat, ...);
            C2VitaLog("REGISTRY CORRUPT %s item=%p at '%s'\n", (const char*)pName, pItem, gRegistry_check_title);
        }
#endif
    }
    return 1;
}

static void C2CheckRegistries(const char* pTitle) {
    /* test harness only (C2_TEST_REGISTRY): it walks every registry at each load scope and once a
     * second in a race; the heap corruption it was added for (minimap blips) is fixed */
    static int enabled = -1;

    if (enabled < 0) {
        enabled = getenv("C2_TEST_REGISTRY") != NULL;
    }
    if (!enabled) {
        return;
    }
    gRegistry_check_title = pTitle != NULL ? pTitle : "";
    BrMaterialEnum(NULL, (br_material_enum_cbfn*)C2CheckRegisteredItem, "material");
    BrModelEnum(NULL, (br_model_enum_cbfn*)C2CheckRegisteredItem, "model");
    BrMapEnum(NULL, (br_map_enum_cbfn*)C2CheckRegisteredItem, "map");
    BrTableEnum(NULL, (br_table_enum_cbfn*)C2CheckRegisteredItem, "table");
}

void C2_HOOK_FASTCALL C2CheckRegistriesAt(const char* pTitle) {
    C2CheckRegistries(pTitle);
}
#endif

// FUNCTION: CARMA2_HW 0x0044c850
void C2_HOOK_FASTCALL PrintMemoryDump(int pFlags, char* pTitle) {
#ifdef __vita__
    /* These existing startup checkpoints are otherwise empty in the port. */
    C2VitaCheckpoint(pTitle);
    {
        extern void C2VitaMemTally(const char* pWhy);
        C2VitaMemTally(pTitle);
    }
#endif
#ifndef CARPOCALYPSE2_MATCHING
    C2CheckRegistries(pTitle);
#endif
}

// FUNCTION: CARMA2_HW 0x0044c860
void C2_HOOK_FASTCALL TouchMemoryPages(void* pMemory, br_size_t pSize) {
#ifdef CARPOCALYPSE2_MATCHING
    br_size_t offset;

    if (gPage_size == 0) {
        GetSystemInfo(&gSystem_info);
        gPage_size = gSystem_info.dwOemId;
    }
    for (offset = gPage_size * 16; offset < pSize; offset += gPage_size) {
        *((volatile br_uint_8*)pMemory + (offset - 16 * gPage_size)) = *((volatile br_uint_8*)pMemory + (offset - 16 * gPage_size));
        *((volatile br_uint_8*)pMemory + offset) = *((volatile br_uint_8*)pMemory + offset);
    }
#else
    (void)pMemory;
    (void)pSize;
#endif
}

#ifdef __vita__
#include <malloc.h>
/* port diagnostics (playtest 00.70: out of memory after about 20 minutes of play): bytes in use per
 * memory type, kept in a 16-byte header in front of every block; C2VitaMemTally logs them. */
#define C2_MEM_HEADER 16
#define C2_MEM_MAGIC 0xc2a110c8u
static br_size_t gC2_mem_bytes[256];
static unsigned gC2_mem_blocks[256];
/* live bytes per (allocating caller, type) for a few types, set by BrResAllocate */
void* gC2_mem_caller;
#define C2_MEM_SITES 1024
static struct { void* caller; unsigned type, blocks; br_size_t bytes; } gC2_mem_sites[C2_MEM_SITES];

static int C2MemSite(void* pCaller, unsigned pType) {
    unsigned h = (((unsigned)(uintptr_t)pCaller >> 1) * 2654435761u + pType) % C2_MEM_SITES;
    unsigned n;

    for (n = 0; n < C2_MEM_SITES; n++, h = (h + 1) % C2_MEM_SITES) {
        if (gC2_mem_sites[h].caller == pCaller && gC2_mem_sites[h].type == pType) {
            return (int)h;
        }
        if (gC2_mem_sites[h].caller == NULL) {
            gC2_mem_sites[h].caller = pCaller;
            gC2_mem_sites[h].type = pType;
            return (int)h;
        }
    }
    return -1;
}

void C2VitaMemTally(const char* pWhy) {
    static br_size_t last_bytes[256];
    struct mallinfo memory = mallinfo();
    br_size_t total = 0;
    int i;
    int order[256];
    int n = 0;

    for (i = 0; i < 256; i++) {
        total += gC2_mem_bytes[i];
        if (gC2_mem_bytes[i] != 0 || last_bytes[i] != 0) {
            order[n++] = i;
        }
    }
    C2VitaLog("MEM TALLY %s: heap_used=%u game=%u other=%u\n", pWhy, (unsigned)memory.uordblks, (unsigned)total,
        (unsigned)(memory.uordblks - total));
    if (strstr(pWhy, "DISPOSING TRACK") != NULL) {
        for (i = 0; i < C2_MEM_SITES; i++) {
            if (gC2_mem_sites[i].caller != NULL && gC2_mem_sites[i].bytes >= 16384) {
                C2VitaLog("MEM SITE caller=%p type=%u bytes=%u blocks=%u\n", gC2_mem_sites[i].caller,
                    gC2_mem_sites[i].type, (unsigned)gC2_mem_sites[i].bytes, gC2_mem_sites[i].blocks);
            }
        }
    }
    for (i = 1; i < n; i++) {
        int t = order[i], j = i;
        while (j > 0 && gC2_mem_bytes[order[j - 1]] < gC2_mem_bytes[t]) {
            order[j] = order[j - 1];
            j--;
        }
        order[j] = t;
    }
    for (i = 0; i < n; i++) {
        int t = order[i];
        long delta = (long)gC2_mem_bytes[t] - (long)last_bytes[t];
        if (i < 12 || delta > 32768 || delta < -32768) {
            C2VitaLog("MEM TYPE %3d %-28s bytes=%u blocks=%u delta=%ld\n", t, gMem_names[t] ? gMem_names[t] : "?",
                (unsigned)gC2_mem_bytes[t], gC2_mem_blocks[t], delta);
        }
        last_bytes[t] = gC2_mem_bytes[t];
    }
}
#endif

// FUNCTION: CARMA2_HW 0x0044c8c0
void* C2_HOOK_CDECL DRStdlibAllocate(br_size_t size, br_uint_8 type) {
    void* p;
    char s[256];

#ifdef CARPOCALYPSE2_MATCHING
    if (size != 0) {
        p = HeapAlloc(GetProcessHeap(), 0, size);
        if (p != NULL) {
            br_size_t o;
            if (gPage_size == 0) {
                GetSystemInfo(&gSystem_info);
                gPage_size = gSystem_info.dwPageSize;
            }
            for (o = gPage_size * 16; o < size; o += gPage_size) {
                volatile br_uint_8 *ptr1 = (br_uint_8*)p + (o - 16 * gPage_size);
                volatile br_uint_8 *ptr2 = (br_uint_8*)p + o;
                *ptr1 = *ptr1;
                *ptr2 = *ptr2;
            }
            return p;
        }
        if (!gNon_fatal_allocation_errors) {
            sprintf(s, "%s/%d", gMem_names[type], size);
            FatalError(kFatalError_OOM_S, s);
        }
    }
    return NULL;
#else
    if (size != 0) {
#ifdef __vita__
        p = malloc(size + C2_MEM_HEADER);
        if (p == NULL) {
            C2VitaPackCacheClear();
            p = malloc(size + C2_MEM_HEADER);
        }
        if (p != NULL) {
            br_uint_32* header = p;
            header[0] = C2_MEM_MAGIC;
            header[1] = (br_uint_32)size;
            header[2] = type;
            header[3] = (br_uint_32)C2MemSite(gC2_mem_caller != NULL ? gC2_mem_caller : __builtin_return_address(0), type);
            gC2_mem_caller = NULL;
            if ((int)header[3] >= 0) {
                gC2_mem_sites[header[3]].bytes += size;
                gC2_mem_sites[header[3]].blocks++;
            }
            gC2_mem_bytes[type] += size;
            gC2_mem_blocks[type]++;
            p = (char*)p + C2_MEM_HEADER;
        }
#else
        p = malloc(size);
#endif
        if (p == NULL && !gNon_fatal_allocation_errors) {
#ifdef __vita__
            C2VitaLog("ALLOCATION FAILED type=%u bytes=%u\n", (unsigned)type, (unsigned)size);
            C2VitaCheckpoint("allocation failure");
            C2VitaMemTally("allocation failure");
#endif
            sprintf(s, "%s/%d", gMem_names[type], size);
            FatalError(kFatalError_OOM_S, s);
        }
        return p;
    }
    return NULL;
#endif
}

// FUNCTION: CARMA2_HW 0x0044c990
void C2_HOOK_CDECL DRStdlibFree(void* mem) {

    #ifdef CARPOCALYPSE2_MATCHING
    if (mem == NULL) {
        FatalError(kFatalError_OOM_S, "NULL POINTER BEING FREED");
    }
    HeapFree(GetProcessHeap(), 0, mem);
#elif defined(__vita__)
    {
        br_uint_32* header = (br_uint_32*)((char*)mem - C2_MEM_HEADER);
        if (header[0] == C2_MEM_MAGIC) {
            header[0] = 0;
            gC2_mem_bytes[header[2] & 0xff] -= header[1];
            gC2_mem_blocks[header[2] & 0xff]--;
            if (header[3] < C2_MEM_SITES) {
                gC2_mem_sites[header[3]].bytes -= header[1];
                gC2_mem_sites[header[3]].blocks--;
            }
            free(header);
        } else {
            free(mem);
        }
    }
#else
    free(mem);
#endif
}

// FUNCTION: CARMA2_HW 0x0044c9c0
br_size_t C2_HOOK_CDECL DRStdlibInquire(br_uint_8 type) {
    return 0;
}

// FUNCTION: CARMA2_HW 0x0044c9d0
br_uint_32 C2_HOOK_CDECL Claim4ByteAlignment(br_uint_8 type) {
    return 4;
}

// FUNCTION: CARMA2_HW 0x0044c9e0
void C2_HOOK_FASTCALL InstallDRMemCalls(void) {

    BrAllocatorSet(&gAllocator);
}

// MAMSUnlock (see plaform)

// MAMSLock (see plaform)

// FUNCTION: CARMA2_HW 0x0044ca40
void C2_HOOK_FASTCALL CreateStainlessClasses(void) {
    int i;

    for (i = 129; i < 129 + (int)CARPOCALYPSE2_ASIZE(gStainless_classes); i++) {
        gStainless_classes[i - 129].res_class = i;
        if (!BrResClassAdd(&gStainless_classes[i - 129])) {
            FatalError(kFatalError_OOM_S);
        }
    }
}

// FUNCTION: CARMA2_HW 0x0044ca80
void C2_HOOK_FASTCALL CheckMemory(void) {

}

// FUNCTION: CARMA2_HW 0x0051b9d0
void C2_HOOK_FASTCALL FUN_DisposeReplayBuffer(void* pBuffer) {

}
