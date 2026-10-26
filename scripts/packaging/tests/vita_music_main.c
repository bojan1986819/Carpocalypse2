/* Host fixture: real SDL streams/threads and real OGG data, no game/device.
 * Redirect device open/resume to an unbound real SDL stream; drain it manually
 * to compare every PCM byte without waiting a whole song. */
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static int fail_open, fail_resume;
static SDL_AudioStream* TestOpen(SDL_AudioDeviceID device, const SDL_AudioSpec* spec,
        SDL_AudioStreamCallback callback, void* userdata) {
    assert(device == SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK && callback == NULL && userdata == NULL);
    return fail_open ? NULL : SDL_CreateAudioStream(spec, spec);
}
static bool TestResume(SDL_AudioStream* stream) {
    (void)stream;
    return fail_resume ? SDL_SetError("injected resume failure") : true;
}
#define SDL_ResumeAudioStreamDevice TestResume
#define SDL_OpenAudioDeviceStream TestOpen
#include "../../../src/carma2/platform/sdl3/vita_music.c"
#undef SDL_ResumeAudioStreamDevice
#undef SDL_OpenAudioDeviceStream

static Uint64 HashPCM(Uint64 hash, const void* bytes, size_t size) {
    const unsigned char* p = bytes;
    size_t i;
    for (i = 0; i < size; ++i) hash = (hash ^ p[i]) * 1099511628211ULL;
    return hash;
}

int main(int argc, char** argv) {
    SDL_AudioStream* stream;
    SDL_AudioSpec input;
    short* reference;
    short pcm[8192];
    int frames, channels, rate, i, queued, max_queued = 0;
    size_t reference_bytes, read_bytes = 0;
    Uint64 expected_hash, actual_hash = 14695981039346656037ULL;
    Uint64 begin, deadline;

    assert(argc == 2);
    setvbuf(stdout, NULL, _IONBF, 0);
    puts("Initializing SDL dummy audio");
    SDL_SetMainReady();
    assert(SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy"));
    assert(SDL_Init(SDL_INIT_AUDIO));
    assert(!C2VitaMusicStart("missing-c2-test-track.ogg"));
    assert(!C2VitaMusicStart(argv[0]));
    assert(!C2VitaMusicPlaying() && !music.stream && !music.decoder && !music.worker);

    puts("Decoding full reference PCM");
    frames = stb_vorbis_decode_filename(argv[1], &channels, &rate, &reference);
    assert(frames > 0 && reference != NULL);
    reference_bytes = (size_t)frames * channels * sizeof(short);
    expected_hash = HashPCM(actual_hash, reference, reference_bytes);
    free(reference);

    fail_open = 1;
    assert(!C2VitaMusicStart(argv[1]));
    assert(!music.stream && !music.decoder && !music.worker);
    fail_open = 0;
    puts("Checking resume failure cleanup");
    fail_resume = 1;
    assert(!C2VitaMusicStart(argv[1]));
    assert(!music.stream && !music.decoder && !music.worker);
    fail_resume = 0;

    puts("Starting bounded producer");
    begin = SDL_GetTicks();
    stream = C2VitaMusicStart(argv[1]);
    assert(stream != NULL && C2VitaMusicPlaying());
    printf("Startup %llu ms; reference PCM %zu bytes\n", (unsigned long long)(SDL_GetTicks() - begin), reference_bytes);
    deadline = SDL_GetTicks() + 10000;
    do {
        queued = SDL_GetAudioStreamQueued(stream);
        assert(queued >= 0 && queued <= MUSIC_QUEUE_LIMIT + (int)sizeof(music.pcm));
        SDL_Delay(1);
        assert(SDL_GetTicks() < deadline);
    } while (queued < MUSIC_QUEUE_LIMIT);
    SDL_Delay(25);
    assert(SDL_GetAudioStreamQueued(stream) <= MUSIC_QUEUE_LIMIT + (int)sizeof(music.pcm));
    assert(!SDL_GetAtomicInt(&music.eof) && C2VitaMusicPlaying());
    puts("Producer is bounded while paused; preparing manual drain");

    /* Keep output identical to the file format for the byte comparison. */
    assert(SDL_GetAudioStreamFormat(stream, &input, NULL));
    assert(SDL_SetAudioStreamFormat(stream, NULL, &input));
    puts("Draining stream");
    deadline = SDL_GetTicks() + 30000;
    while (C2VitaMusicPlaying()) {
        int bytes;
        queued = SDL_GetAudioStreamQueued(stream);
        if (queued > max_queued) max_queued = queued;
        assert(queued <= MUSIC_QUEUE_LIMIT + (int)sizeof(music.pcm));
        bytes = SDL_GetAudioStreamData(stream, pcm, sizeof(pcm));
        assert(bytes >= 0);
        if (bytes) {
            actual_hash = HashPCM(actual_hash, pcm, bytes);
            read_bytes += bytes;
        } else {
            SDL_Delay(1);
        }
        assert(SDL_GetTicks() < deadline);
    }
    assert(SDL_GetAtomicInt(&music.eof) && !SDL_GetAtomicInt(&music.failed));
    assert(read_bytes == reference_bytes && actual_hash == expected_hash);
    C2VitaMusicStop();
    C2VitaMusicStop();
    assert(!C2VitaMusicPlaying());

    for (i = 0; i < 20; ++i) {
        assert(C2VitaMusicStart(argv[1]));
        if (i % 2) SDL_Delay(10);
        C2VitaMusicStop();
        assert(!music.worker && !music.decoder && !music.stream);
    }
    SDL_Quit();
    printf("PASS real SDL streams/threads, byte-identical complete PCM (%zu bytes), queue peak %d <= %d, EOF/drain, bounded paused producer, missing/invalid track and open/resume failure cleanup, 20 stop/restarts; device open/resume mocked; no game/Vita execution\n",
        read_bytes, max_queued, MUSIC_QUEUE_LIMIT + (int)sizeof(music.pcm));
    return 0;
}
