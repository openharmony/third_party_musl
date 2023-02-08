#ifndef _MEMORY_TAG_H
#define _MEMORY_TAG_H

#include <ctype.h>
#include <unistd.h>
#include "common_def.h"
#include "musl_preinit_common.h"

typedef int (*mmemtrace)(void* addr, size_t size, const char* tag, bool is_using);
extern volatile atomic_llong __mem_trace;
#define MEM_TRACE(addr, size, tag, is_using)                                                                    \
    do {                                                                                                        \
        volatile mmemtrace func = (volatile mmemtrace)atomic_load_explicit(&__mem_trace, memory_order_acquire); \
        if (__predict_false(!__get_global_hook_flag())) {                                                       \
            if (func != NULL) {                                                                                 \
                func(addr, size, tag, is_using);                                                                \
            }                                                                                                   \
        }                                                                                                       \
    } while (0)
#endif
