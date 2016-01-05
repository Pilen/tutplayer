#ifndef TUT_INIT_H
#define TUT_INIT_H

#include "config.h"
#include "state.h"

PlayerState *init(const Config *config);
PlayerState *init_default(void);

#endif
