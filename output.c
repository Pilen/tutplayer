#include <math.h>
#include <pthread.h>

#include "assert.h"
#include "init.h"
#include "log.h"
#include "output.h"
#include "state.h"


void output_terminate_portaudio(void);

void output_init(PlayerState *state, const Config *config) {
    log_info("Portaudio version: %s", Pa_GetVersionText());
    int error = Pa_Initialize();
    if (error != paNoError) {
        log_fatal("Could not initialize PortAudio\n%s", Pa_GetErrorText(error));
    } else {
        log_info("PortAudio started succesfully");
    }
    atexit(output_terminate_portaudio);

    int num_audio_devices = Pa_GetDeviceCount();
    if (num_audio_devices <= 0) {
        log_fatal("No available audio devices\n%s", Pa_GetErrorText(num_audio_devices));
    }

    /* List audio devices */
    log_info("Number of available audio devices: %d", num_audio_devices);
    int default_audio_device = Pa_GetDefaultOutputDevice();
    if (default_audio_device <= 0) {
        log_error("No default audio device");
    }
    const PaDeviceInfo *device_info;
    int desired = -1;
    for (int i = 0; i < num_audio_devices; i++) {
        device_info = Pa_GetDeviceInfo(i);
        if (config -> output_device && strcmp(config -> output_device, device_info -> name) == 0) {
            desired = i;
        }
        log_info("Audio device:\n%s%s%s\nmax channels: %d\ndefault sample rate: %f\n",
                 device_info -> name,
                 (i == default_audio_device ? " [default]" : ""),
                 (desired == i ? " [desired]" : ""),
                 device_info -> maxOutputChannels,
                 device_info -> defaultSampleRate);
    }

    if (config -> output_device && desired == -1) {
        log_error("Desired audio device not found, using default");
        device_info = Pa_GetDeviceInfo(default_audio_device);
    } else if (desired >= 0) {
        log_info("Desired audio device used");
        device_info = Pa_GetDeviceInfo(desired);
    } else {
        log_info("Default audio device used");
        device_info = Pa_GetDeviceInfo(default_audio_device);
    }

    double output_sample_rate;
    if (config -> sample_rate > 0) {
        output_sample_rate = config -> sample_rate;
    } else {
        output_sample_rate = device_info -> defaultSampleRate;
        if (output_sample_rate <= 0) {
            log_fatal("No default output sample rate");
        }
    }

    PaStream *stream;
    error = Pa_OpenDefaultStream(&stream,
                                 0,
                                 2,
                                 paFloat32,
                                 output_sample_rate,
                                 paFramesPerBufferUnspecified,
                                 output_callback,
                                 state);
    if (error != paNoError) {
        log_fatal("Could not open PortAudio output stream\n%s", Pa_GetErrorText(error));
    }
    const PaStreamInfo *stream_info;
    stream_info = Pa_GetStreamInfo(stream);
    if (!stream_info) {
        log_fatal("Could not retrieve stream info");
    }
    if (config -> sample_rate > 0 && stream_info -> sampleRate != config -> sample_rate) {
        log_error("Output sample rate is %f, desired %f", stream_info -> sampleRate, config -> sample_rate);
    }
    if (stream_info -> sampleRate != output_sample_rate) {
        if (config -> sample_rate) {
            log_error("Output sample rate is %f, desired %f", stream_info -> sampleRate, output_sample_rate);
        } else {
            log_info("Output sample rate is %f, but tried to get %f", stream_info -> sampleRate, output_sample_rate);
        }
        output_sample_rate = stream_info -> sampleRate;
    } else {
        log_info("Output sample rate: %f", output_sample_rate);
    }

    error = Pa_StartStream(stream);
    if (error != paNoError) {
        log_fatal("Could not start PortAudio output stream\n%s", Pa_GetErrorText(error));
    }

    state -> output_stream = stream;
    state -> output_sample_rate = output_sample_rate;
    state -> output_status = 0;
}

void output_stop(PlayerState *state) {
    Pa_Sleep(1000*1000);

    t_assert(state -> output_stream);
    int error = Pa_StopStream(state -> output_stream);
    if (error != paNoError) {
        log_fatal("Could not stop PortAudio output stream\n%s", Pa_GetErrorText(error));
    }
}

void output_terminate_portaudio(void) {
    /* This automatically closes any PortAudio streams that are still open. */
    int error = Pa_Terminate();
    if (error != paNoError) {
        log_error("PortAudio could not terminate correctly\n%s", Pa_GetErrorText(error));
    }
}

int output_callback(const void *input,
                    void *output,
                    unsigned long frame_count,
                    const PaStreamCallbackTimeInfo* time_info,
                    PaStreamCallbackFlags status_flags,
                    void *user_data) {
    PlayerState *state = (PlayerState *) user_data;
    float *buffer = (float *) output;
    /* (void) input; */

    double volume = 0.3;

    state -> output_status |= status_flags;
    t_assert(pthread_setname_np(pthread_self(), "callback") == 0);
    for (unsigned long i = 0; i < frame_count; i++) {
        *buffer++ = state -> left_phase * volume;
        *buffer++ = state -> right_phase * volume;

        state -> left_phase += 0.1f;
        if (state -> left_phase >= 1.0f) {
            state -> left_phase = -1.0f;
        }
        state -> right_phase += 0.3f;
        if (state -> right_phase >= 1.0f) {
            state -> right_phase = -1.0f;
        }
    }
    return paContinue;
}
