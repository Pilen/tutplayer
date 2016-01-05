
#include "memory.h"
#include "log.h"

void *tut_mallocz_actual(size_t size, const char *file, int line, const char *function) {
    void *allocated = calloc(1, size);
    if (!allocated) {
        LOG_FROM(MEMORY, log_memory, file, line, function, "Out of memory when allocating %zd bytes", size);
        exit(EXIT_FAILURE);
        log_fatal("Out of memory");
    }
    return allocated;
}
