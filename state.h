#ifndef TUT_STATE_H
#define TUT_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mem.h>
#include <portaudio.h>

typedef struct PlayerState_s {
    AVFormatContext *format_ctx;
    char            *filename;
    float           left_phase;
    float           right_phase;

    /* Decide unit */
    double          volume;
    double          max_volume;

    PaStream        *output_stream;
    double          output_sample_rate; /* Dont modify */
    int             output_status;
    bool            output_need_data;
    double          track_position;
} PlayerState;

PlayerState *global_player_state;

#endif
