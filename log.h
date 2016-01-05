#ifndef TUT_LOG_H
#define TUT_LOG_H

#include <stdlib.h>
#include <stdio.h>

#define LOG_LEVEL_MAX 127
int log_level;
FILE *log_output;

#define LOG_FROM(STATUS, level, file, line, func, ...)                  \
    do {                                                                \
        if (level <= log_level) {                                       \
            fprintf(log_output, "%s %s:%d: %s: \t", #STATUS, file, line, func); \
            fprintf(log_output, __VA_ARGS__);                           \
            fprintf(log_output, "\n");                                  \
            fflush(log_output);                                         \
        }                                                               \
    } while (0);

#define LOG(STATUS, level, ...) LOG_FROM(STATUS, level, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define log_info(...) LOG(INFO, 6, __VA_ARGS__)
#define log_warning(...) LOG(WARNING, 5, __VA_ARGS__)
#define log_error(...) LOG(ERROR, 4, __VA_ARGS__)
#define log_memory 3
#define log_assert(v) LOG(ASSERTION-FAILURE, 2, "%s is false", #v)
#define log_fatal(...)                              \
    do {                                            \
        LOG(FATAL-ERROR, 1, __VA_ARGS__);           \
        if (1 <= log_level) {                       \
            fprintf(log_output, "Exiting...\n");    \
        }                                           \
        exit(EXIT_FAILURE);                         \
    } while (0);

#endif
