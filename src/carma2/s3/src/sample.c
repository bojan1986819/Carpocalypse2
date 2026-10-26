#include "../include/s3/internal/sample.h"

#include "../include/s3/internal/audio.h"
#include "resource.h"

#include "platform.h"

#include <brender/brender.h>
#include "carpocalypse2_types.h"

#include "c2_string.h"

#ifdef CARPOCALYPSE2_PLATFORM_WIN32
#include "../../platform/win32/win32_sound.h"
#endif


// GLOBAL: CARMA2_HW 0x006b2c88
int gS3_low_memory_mode;

// GLOBAL: CARMA2_HW 0x007a05a0
tS3_error_codes gS3_last_error;

// GLOBAL: CARMA2_HW 0x007a05c0
char gS3_sound_dirname[256];

// FUNCTION: CARMA2_HW 0x005687ff
tS3_buffer_desc* C2_HOOK_FASTCALL S3GetBufferDescription(int pSample_id) {
    tS3_descriptor* descriptor;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tS3_descriptor, buffer_description, 0x40);

    if (!gS3_enabled) {
        return NULL;
    }
    descriptor = S3GetDescriptorByID(pSample_id);
    if (descriptor == NULL) {
        return NULL;
    }
    return descriptor->buffer_description;
}

// FUNCTION: CARMA2_HW 0x00568830
tS3_error_codes C2_HOOK_FASTCALL S3LoadSample(int pSample_id) {
    tS3_descriptor* descriptor;
    char path[512];
    tS3_buffer_desc* buffer_description;

    if (gS3_enabled) {
        descriptor = S3GetDescriptorByID(pSample_id);
        if (descriptor == NULL) {
            return eS3_error_bad_id;
        }
        if (descriptor->type == 0) {
            if (descriptor->buffer_description != NULL) {
                return eS3_error_none;
            }
            path[0] = '\0';
            strcpy(path, descriptor->path);

            C2_HOOK_BUG_ON(sizeof(tS3_buffer_desc) != 0x18);

            buffer_description = S3MemAllocate(sizeof(tS3_buffer_desc), kMem_S3_sound_header);
            if (buffer_description == NULL) {
                return eS3_error_memory;
            }
            memset(buffer_description, 0, sizeof(*buffer_description));
#ifdef __vita__
            {
                extern void C2VitaLog(const char* format, ...);
                extern unsigned long long C2VitaGLTicksNS(void);
                unsigned long long c2_t0 = C2VitaGLTicksNS();
                descriptor->pd_handle = S3BufferWav(path, buffer_description);
                C2VitaLog("VITA SOUND lazy load id=%d %s took %llu ms\n", pSample_id, path,
                    (C2VitaGLTicksNS() - c2_t0) / 1000000ull);
            }
#else
            descriptor->pd_handle = S3BufferWav(path, buffer_description);
#endif
            if (descriptor->pd_handle == NULL) {
                BrFailure("Cound not load sample:%s", path);
                S3MemFree(buffer_description);
                return gS3_last_error;
            }
            descriptor->effects_enabled = 0;
            descriptor->buffer_description = buffer_description;
        }
    }
    return eS3_error_none;
}

#ifdef __vita__
/* Vita: load every sample of the soundbank up front. Lazy first-use loading reads the WAV from
 * the memory card in the middle of physics/collision (crash, scrape, ped sounds) and stalls the
 * race for tens of ms each time. Missing files are skipped (the lazy path would only fail if the
 * sound is ever played). ~6 MB for the whole bank. */
void C2VitaPreloadSamples(void) {
    extern void C2VitaLog(const char* format, ...);
    extern unsigned long long C2VitaGLTicksNS(void);
    tS3_descriptor* desc;
    unsigned loaded = 0, failed = 0, already = 0;
    unsigned long long start = C2VitaGLTicksNS();
    C2VitaLog("VITA SOUND preload start enabled=%d descriptors=%p\n", gS3_enabled, (void*)gS3_state.descriptors);
    if (!gS3_enabled) return;
    for (desc = gS3_state.descriptors; desc != NULL; desc = desc->next) {
        tS3_buffer_desc* buffer_description;
        if (desc->type != 0) continue;
        if (desc->buffer_description != NULL) { ++already; continue; }
        buffer_description = S3MemAllocate(sizeof(tS3_buffer_desc), kMem_S3_sound_header);
        if (buffer_description == NULL) { ++failed; break; }
        memset(buffer_description, 0, sizeof(*buffer_description));
        desc->pd_handle = S3BufferWav(desc->path, buffer_description);
        if (desc->pd_handle == NULL) {
            S3MemFree(buffer_description);
            ++failed;
            continue;
        }
        desc->effects_enabled = 0;
        desc->buffer_description = buffer_description;
        ++loaded;
    }
    C2VitaLog("VITA SOUND preloaded=%u already=%u skipped=%u in %llu ms\n",
        loaded, already, failed, (C2VitaGLTicksNS() - start) / 1000000ull);
}
#endif

// FUNCTION: CARMA2_HW 0x00565a3d
tS3_descriptor* C2_HOOK_FASTCALL S3GetDescriptorByID(int pSample_id) {
    tS3_descriptor *desc;

    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tS3_descriptor, sample_id, 0x0);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tS3_descriptor, next, 0x2c);
    C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tS3_descriptor, low_memory_alternative, 0x3c);

    for (desc = gS3_state.descriptors; desc != NULL; desc = desc->next) {
        if (desc->sample_id == pSample_id) {
            if (desc->low_memory_alternative >= 0) {
                return S3GetDescriptorByID(desc->low_memory_alternative);
            }
            return desc;
        }
    }
    return NULL;
}

// FUNCTION: CARMA2_HW 0x00564e5e
int C2_HOOK_FASTCALL FUN_00564e5e(int pSample_id) {
    struct {
        tS3_descriptor* descriptor;
        tS3_buffer_desc* buffer_description;
    } volatile locals;

    locals.descriptor = S3GetDescriptorByID(pSample_id);
    if (locals.descriptor == NULL) {
        return 0;
    }
    locals.buffer_description = locals.descriptor->buffer_description;
    return locals.buffer_description->sample_size * 1000 / locals.buffer_description->sample_rate;
}

// FUNCTION: CARMA2_HW 0x00565b34
void C2_HOOK_CDECL s3_debug_disable_sound(void) {

}

// FUNCTION: CARMA2_HW 0x00565b39
void C2_HOOK_CDECL s3_debug_init(void) {
    tPath_name path;
    int result;

    (void) path;
    (void) result;

}

// FUNCTION: CARMA2_HW 0x00565b46
void C2_HOOK_CDECL s3_dprintf(const char* pFormat, ...) {

}

// C2PORT-NONMATCHING: best 35.42% | difftest PASS 2000 trials (spec difftest/specs/00568ee0_S3CheckWavHeader.json) | reason compiler lays out RIFF chunk traversal differently
// FUNCTION: CARMA2_HW 0x00568ee0
int C2_HOOK_FASTCALL S3CheckWavHeader(tS3_wav_file* pWav_buffer, tS3_wav_chunk_info_header** pWav_format_info_header, void** pSamples, int* pSample_size) {
    void* chunk_start;
    void* file_end;

    if (pWav_format_info_header != NULL) {
        *pWav_format_info_header = NULL;
    }
    if (pSamples != NULL) {
        *pSamples = NULL;
    }
    if (pSample_size != NULL) {
        *pSample_size = 0;
    }
    if (strncmp(pWav_buffer->riff_header.magic, "RIFF", 4) != 0 || strncmp(pWav_buffer->wave_magic, "WAVE", 4) != 0) {
        return 0;
    }
    file_end = (br_uint_8*)pWav_buffer->wave_magic + pWav_buffer->riff_header.chunk_size;
    chunk_start = &pWav_buffer->fmt_chunk;
    while (chunk_start < file_end) {
        tS3_riff_chunk_header* chunk_header = chunk_start;
        if (strncmp(chunk_header->magic, "fmt ", 4) == 0) {
            tS3_riff_fmt_chunk* fmt_chunk = chunk_start;
            if (pWav_format_info_header != NULL && *pWav_format_info_header == NULL) {
                C2_HOOK_STATIC_ASSERT_STRUCT_OFFSET(tS3_wav_chunk_info_header, bits_per_sample, 0xe);
                if (chunk_header->chunk_size < offsetof(tS3_wav_chunk_info_header, bits_per_sample)) {
                    return 0;
                }
                *pWav_format_info_header = &fmt_chunk->wav_info_header;
                if ((pSamples == NULL || *pSamples != NULL) && (pSample_size == NULL || *pSample_size != 0)) {
                    return 1;
                }
            }
        } else if (strncmp(chunk_header->magic, "data", 4) == 0) {
            tS3_riff_data_chunk* data_chunk = chunk_start;
            if ((pSamples != NULL && *pSamples == NULL) || (pSample_size != NULL && *pSample_size == 0)) {
                if (pSamples != NULL) {
                    *pSamples = data_chunk->data;
                }
                if (pSample_size != NULL) {
                    *pSample_size = data_chunk->header.chunk_size;
                }
                if (pWav_format_info_header == NULL || *pWav_format_info_header != NULL) {
                    return 1;
                }
            }
        }
        chunk_start = (br_uint_8*)&chunk_header[1] + ((chunk_header->chunk_size + 1) & ~1);
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x0056907c
void* C2_HOOK_FASTCALL S3BufferWav(const char* pPath, tS3_buffer_desc* pBuffer_desc) {
#ifdef CARPOCALYPSE2_PLATFORM_WIN32
    struct {
        FILE* f;
        int unused;
        int file_size;
        char filename[512];
        DWORD buffer_data_size;
        tS3_wav_file* wav_buffer;
        LPDIRECTSOUNDBUFFER pd_handle;
        DSBUFFERDESC buffer_desc;
        tS3_wav_info wav_info;
        void* buffer_data;
        char wav_path[512];
    } locals;
#else
    struct {
        FILE* f;
        int file_size;
        char filename[512];
        tS3_wav_file* wav_buffer;
        void* pd_handle;
        tS3_wav_info wav_info;
        char wav_path[512];
    } locals;
#endif

#ifdef CARPOCALYPSE2_PLATFORM_WIN32
    locals.buffer_data = NULL;
    locals.unused = 0;
    locals.buffer_data_size = 0;
#endif

    if (gS3_low_memory_mode) {
        sprintf(locals.wav_path, "DATA%sSOUNDS%s%s", gS3_path_separator, gS3_path_separator, pPath);
        locals.f = S3_low_memory_fopen(locals.wav_path, "rb");
    } else {
        if (gS3_sound_dirname[0] != '\0') {
            PDExtractFilename(locals.filename, pPath);
            sprintf(locals.wav_path, "%s\\%s", gS3_sound_dirname, locals.filename);
            locals.f = fopen(locals.wav_path, "rb");
        } else {
            locals.f = fopen(pPath, "rb");
        }
    }
    if (locals.f == NULL) {
        gS3_last_error = eS3_error_readfile;
        return NULL;
    }
    locals.file_size = S3GetFileSize(locals.f);
    if (locals.file_size == 0) {
        fclose(locals.f);
        gS3_last_error = eS3_error_readfile;
        return NULL;
    }
    locals.wav_buffer = S3MemAllocate(locals.file_size, kMem_S3_Windows_95_load_WAV_file);
    if (locals.wav_buffer == NULL) {
        fclose(locals.f);
        gS3_last_error = eS3_error_memory;
        return NULL;
    }
    fread(locals.wav_buffer, 1, locals.file_size, locals.f);
    fclose(locals.f);

    C2_HOOK_BUG_ON(sizeof(tS3_wav_info) != 0xc);

    memset(&locals.wav_info, 0, sizeof(tS3_wav_info));
    if (!S3CheckWavHeader(locals.wav_buffer, &locals.wav_info.wav_info_header, &locals.wav_info.samples, &locals.wav_info.sample_size)) {
        gS3_last_error = eS3_error_readfile;
        s3_dprintf("ERROR: .WAV file '%s' is crap", locals.wav_path);
        return NULL;
    }
    pBuffer_desc->field_0x14 = 0;
    pBuffer_desc->field_0x10 = 0;
    pBuffer_desc->sample_size = locals.wav_info.sample_size;
    pBuffer_desc->sample_rate = locals.wav_info.wav_info_header->sample_rate;
    pBuffer_desc->bytes_per_second = locals.wav_info.wav_info_header->avg_bytes_per_second;
    pBuffer_desc->count_channels = locals.wav_info.wav_info_header->number_of_channels;
#ifdef CARPOCALYPSE2_PLATFORM_WIN32
    (void)locals.unused;
    C2_HOOK_BUG_ON(sizeof(DSBUFFERDESC) != 0x14);
    memset(&locals.buffer_desc, 0, sizeof(locals.buffer_desc));
    locals.buffer_desc.dwSize = sizeof(locals.buffer_desc);
    locals.buffer_desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC;
    locals.buffer_desc.dwBufferBytes = locals.wav_info.sample_size;
    locals.buffer_desc.lpwfxFormat = (LPWAVEFORMATEX)locals.wav_info.wav_info_header;
    if (FAILED(IDirectSound_CreateSoundBuffer(gPD_S3_direct_sound, &locals.buffer_desc, &locals.pd_handle, NULL))) {
        return NULL;
    }
    if (FAILED(IDirectSoundBuffer_Lock(locals.pd_handle, 0, locals.wav_info.sample_size, &locals.buffer_data, &locals.buffer_data_size, NULL, NULL, 0))) {
        return NULL;
    }
    memmove(locals.buffer_data, locals.wav_info.samples, locals.buffer_data_size);
    S3MemFree(locals.wav_buffer);
    IDirectSoundBuffer_Unlock(locals.pd_handle, locals.buffer_data, locals.buffer_data_size, NULL, 0);
#else
    locals.pd_handle = PDS3CreateSoundBuffer(&locals.wav_info, locals.wav_buffer);
    if (locals.pd_handle != NULL) {
        S3MemFree(locals.wav_buffer);
    }
#endif
    return locals.pd_handle;
}

// C2PORT-NONMATCHING: best 80.39% | difftest PASS 2000 trials (spec difftest/specs/00569865_S3PlaySample.json) | reason stack-layout
// FUNCTION: CARMA2_HW 0x00569865
int C2_HOOK_FASTCALL S3PlaySample(tS3_channel* pChannel, tS3_descriptor* pDescriptor) {
    int unused;
    void* sound_buffer;
    int looping;

    (void)pDescriptor;
    unused = 0;
    looping = 0;
#if defined(CARPOCALYPSE2_PLATFORM_WIN32)
    if (pChannel->descriptor != NULL && pChannel->descriptor->type == pChannel->type) {
        sound_buffer = pChannel->descriptor->pd_handle;
        if (sound_buffer != NULL) {
            if (pChannel->repetitions == 0) {
                looping = 1;
            }
            ((int (C2_HOOK_STDCALL *)(void*, unsigned int, int, unsigned int))(*(void***)sound_buffer)[12])(sound_buffer, 0, 0, looping);
            if (looping == 0) {
                ((int (C2_HOOK_STDCALL *)(void*, unsigned int))(*(void***)sound_buffer)[13])(sound_buffer, 0);
            }
        }
    }
#else
    PDS3PlaySample(pChannel);
#endif
    PDS3UpdateChannelVolume(pChannel);
    PDS3UpdateChannelFrequency(pChannel);
    (void)unused;
    return 1;
}
