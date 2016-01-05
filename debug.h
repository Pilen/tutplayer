#ifndef TUT_DEBUG_H
#define TUT_DEBUG_H

#ifdef DEBUG
#include <stdio.h>
#include "log.h"

#define debugr(M) do{fprintf(log_output, "DEBUG: %s:%d: %s: " #M "\n", __FILE__, __LINE__, __func__);} while (0);
#define debugi(v) do{fprintf(log_output, "DEBUG: %s:%d: %s: "#v" = %d\n", __FILE__, __LINE__, __func__, v);} while (0);
#define debug(...) do{fprintf(log_output, "DEBUG: %s:%d: %s: ", __FILE__, __LINE__, __func__); fprintf(log_output, __VA_ARGS__); fprintf(log_output, "\n");} while (0);
#define debug_type(expression) do{fprintf(log_output, "DEBUG: %d is an int", (expression));} while (0);
#else
#define debugr(M) do{} while (0);
#define debugi(M) do{} while (0);
#define debug(M) do{} while (0);
#define debug_type(expression) do{} while (0);

#endif
#endif
