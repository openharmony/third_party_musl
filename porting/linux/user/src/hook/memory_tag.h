#ifndef _MEMORY_TAG_H
#define _MEMORY_TAG_H

#include <unistd.h>

typedef int (*mtypeset)(const void* addr, size_t addrlen, const char* tag, size_t tagLen);
extern mtypeset __mem_typeset;
#define MEM_TYPESET(addr, addrlen, tag, tagLen)        \
    do {                                               \
        if (__mem_typeset != NULL) {                   \
            __mem_typeset(addr, addrlen, tag, tagLen); \
        }                                              \
    } while (0)
#endif