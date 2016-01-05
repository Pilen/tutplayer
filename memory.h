#ifndef TUT_MEMORY_H
#define TUT_MEMORY_H

#include <stdlib.h>

#define TUT_MALLOCZ(type) tut_mallocz_actual(sizeof(type), __FILE__, __LINE__, __func__)
void *tut_mallocz_actual(size_t size, const char *file, int line, const char *function);
#define tut_free(ptr) free(ptr)
#endif
