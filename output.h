#ifndef TUT_OUTPUT_H
#define TUT_OUTPUT_H

#include <portaudio.h>

#include "state.h"

void output_init(PlayerState *state, const Config *config);
void output_stop(PlayerState *state);

int output_callback(const void *input,
                    void *output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData);

#endif
