
#include <stdio.h>

#include <libavutil/mem.h>
#include "assert.h"
#include "config.h"
#include "init.h"
#include "log.h"
#include "output.h"
#include "state.h"

PlayerState *init(const Config *config) {
    log_level = LOG_LEVEL_MAX;
    log_output = stderr;

    PlayerState *state = av_mallocz(sizeof(PlayerState));
    if (!state) {
        log_fatal("Could not create state");
    }

    output_init(state, config);

    state -> volume = config -> volume;
    state -> max_volume = config -> max_volume;

    global_player_state = state;
    return state;
}

PlayerState *init_default(void) {
    Config config = {0.3, -1, -1, NULL};
    return init(&config);
}
