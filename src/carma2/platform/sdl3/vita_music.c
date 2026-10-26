#include "vita_music.h"
#include "stb/stb_vorbis.c"

/* Keep PCM storage independent of song duration. SDL copies each submitted
 * chunk, so retaining a full decoded song would use roughly twice its size. */
enum { MUSIC_SHORTS = 8192, MUSIC_QUEUE_LIMIT = 128 * 1024 };
static struct {
    stb_vorbis* decoder;
    SDL_AudioStream* stream;
    SDL_Thread* worker;
    SDL_AtomicInt stop;
    SDL_AtomicInt eof;
    SDL_AtomicInt failed;
    int channels;
    short pcm[MUSIC_SHORTS];
} music;

/* Called by the main thread before starting the worker, then by that worker
 * alone. A decoder is never used concurrently or closed before the join. */
static int MusicReadChunk(void) {
    int frames = stb_vorbis_get_samples_short_interleaved(music.decoder,
        music.channels, music.pcm, MUSIC_SHORTS);
    if (frames == 0) {
        if (stb_vorbis_get_error(music.decoder) != VORBIS__no_error
                || !SDL_FlushAudioStream(music.stream)) {
            SDL_SetAtomicInt(&music.failed, 1);
        }
        SDL_SetAtomicInt(&music.eof, 1);
        return 0;
    }
    if (!SDL_PutAudioStreamData(music.stream, music.pcm,
            frames * music.channels * (int)sizeof(short))) {
        SDL_SetAtomicInt(&music.failed, 1);
        return 0;
    }
    return 1;
}

static int SDLCALL MusicWorker(void* unused) {
    (void)unused;
    while (!SDL_GetAtomicInt(&music.stop)) {
        int queued = SDL_GetAudioStreamQueued(music.stream);
        if (queued < 0) {
            SDL_SetAtomicInt(&music.failed, 1);
            break;
        }
        if (queued >= MUSIC_QUEUE_LIMIT) {
            SDL_Delay(5);
        } else if (!MusicReadChunk()) {
            break;
        }
    }
    return 0;
}

void C2VitaMusicStop(void) {
    SDL_SetAtomicInt(&music.stop, 1);
    if (music.worker != NULL) {
        SDL_WaitThread(music.worker, NULL);
        music.worker = NULL;
    }
    if (music.stream != NULL) {
        SDL_DestroyAudioStream(music.stream);
        music.stream = NULL;
    }
    if (music.decoder != NULL) {
        stb_vorbis_close(music.decoder);
        music.decoder = NULL;
    }
}

SDL_AudioStream* C2VitaMusicStart(const char* path) {
    SDL_AudioSpec spec;
    SDL_PropertiesID properties;
    stb_vorbis_info info;
    int error;

    C2VitaMusicStop();
    SDL_SetAtomicInt(&music.stop, 0);
    SDL_SetAtomicInt(&music.eof, 0);
    SDL_SetAtomicInt(&music.failed, 0);
    music.decoder = stb_vorbis_open_filename(path, &error, NULL);
    if (music.decoder == NULL) {
        SDL_SetError("Music decoder open failed (%d): %s", error, path);
        return NULL;
    }
    info = stb_vorbis_get_info(music.decoder);
    if (info.channels < 1 || info.channels > 2 || info.sample_rate == 0) {
        SDL_SetError("Unsupported music format: %s", path);
        C2VitaMusicStop();
        return NULL;
    }
    music.channels = info.channels;
    SDL_zero(spec);
    spec.freq = info.sample_rate;
    spec.channels = music.channels;
    spec.format = SDL_AUDIO_S16;
    music.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (music.stream == NULL || !MusicReadChunk()) {
        C2VitaMusicStop();
        return NULL;
    }

    properties = SDL_CreateProperties();
    if (properties == 0) {
        C2VitaMusicStop();
        return NULL;
    }
    if (!SDL_SetPointerProperty(properties, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)MusicWorker)
            || !SDL_SetStringProperty(properties, SDL_PROP_THREAD_CREATE_NAME_STRING, "C2 music")
            || !SDL_SetNumberProperty(properties, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 512 * 1024)) {
        SDL_DestroyProperties(properties);
        C2VitaMusicStop();
        return NULL;
    }
    music.worker = SDL_CreateThreadWithProperties(properties);
    SDL_DestroyProperties(properties);
    if (music.worker == NULL || !SDL_ResumeAudioStreamDevice(music.stream)) {
        C2VitaMusicStop();
        return NULL;
    }
    return music.stream;
}

int C2VitaMusicPlaying(void) {
    if (music.stream == NULL || SDL_GetAtomicInt(&music.failed)) return 0;
    /* An empty queue while the worker is decoding is an underrun, not EOF. */
    if (!SDL_GetAtomicInt(&music.eof)) return 1;
    return SDL_GetAudioStreamQueued(music.stream) > 0
        || SDL_GetAudioStreamAvailable(music.stream) > 0;
}
