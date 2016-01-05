#ifndef TUT_ASSERT_H
#define TUT_ASSERT_H

#include <stdlib.h>
#include "log.h"

#ifdef DEBUG
#define t_assert(v)                             \
    do {                                        \
        if (!!!(v)) {                           \
            log_assert(#v);                     \
            exit(EXIT_FAILURE);                 \
        }                                       \
    } while (0);

#define assert_build(v)                         \
    do {                                        \
        (void) sizeof(char[1 - 2*!!!(v)]);      \
    } while (0);

#else

#define t_assert(v)                             \
    do {                                        \
        (void) sizeof((v));                     \
    } while (0);

#define assert_build(v) t_assert(v)
#endif
#endif
