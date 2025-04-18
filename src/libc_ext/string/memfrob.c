#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define ONES ((size_t)-1 / 255)
#define MASK (0x2a * ONES)

void *memfrob(void *s, size_t n) {
    unsigned char *p = s;
    size_t i = 0;

    // Process unaligned leading bytes
    while ((uintptr_t)(p + i) % sizeof(size_t) != 0 && i < n) {
        p[i] ^= 0x2a;
        i++;
    }

    // Process word-sized chunks
    size_t *word_ptr = (size_t *)(p + i);
    size_t word_count = (n - i) / sizeof(size_t);
    for (size_t j = 0; j < word_count; j++) {
        word_ptr[j] ^= MASK;
    }

    // Process remaining bytes
    i += word_count * sizeof(size_t);
    for (; i < n; i++) {
        p[i] ^= 0x2a;
    }

    return s;
}