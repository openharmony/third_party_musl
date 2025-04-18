#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define ONES ((size_t)-1 / 255)
#define HIGHS (ONES * (255 / 2 + 1))
#define HASZERO(x) (((x) - ONES) & ~(x) & HIGHS)

void *rawmemchr(const void *s, int c)
{
    const unsigned char *ptr = s;
    unsigned char value = (unsigned char)c;

    /* Process bytes until aligned to sizeof(size_t) */
    while ((uintptr_t)ptr & (sizeof(size_t) - 1)) {
        if (*ptr == value)
            return (void *)ptr;
        ptr++;
    }

    /* Prepare word-sized mask */
    size_t mask = value * ONES;
    const size_t *wptr = (const size_t *)ptr;

    /* Scan word by word */
    while (1) {
        size_t word = *wptr;
        size_t xor = word ^ mask;
        if (HASZERO(xor))
            break;
        wptr++;
    }

    /* Find the exact byte within the word */
    ptr = (const unsigned char *)wptr;
    for (size_t i = 0; i < sizeof(size_t); i++) {
        if (ptr[i] == value)
            return (void *)(ptr + i);
    }

    /* Unreachable as per function contract */
    __builtin_unreachable();
}