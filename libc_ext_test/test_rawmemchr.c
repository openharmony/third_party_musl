#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare the rawmemchr function to avoid linking issues
void *rawmemchr(const void *s, int c);

// Test basic functionality
static void test_basic(void) {
    char str[] = "test_string";
    char *p = rawmemchr(str, '_');
    assert(p == str + 4);
}

// Test finding the last byte in a buffer
static void test_end_of_buffer(void) {
    char buf[16];
    memset(buf, 0xAA, sizeof(buf));
    buf[sizeof(buf) - 1] = 0xBB;
    char *p = rawmemchr(buf, 0xBB);
    assert(p == buf + sizeof(buf) - 1);
}

// Test alignment handling (unaligned start)
static void test_unaligned(void) {
    size_t buf[4];
    memset(buf, 0xCC, sizeof(buf));
    char *ptr = (char *)buf + 3; // Force unaligned address
    *ptr = 0xDD;
    char *p = rawmemchr(ptr, 0xDD);
    assert(p == ptr);
}

// Test finding a zero byte (non-string usage)
static void test_zero_byte(void) {
    char buf[16];
    memset(buf, 0xFF, sizeof(buf));
    buf[7] = 0x00;
    char *p = rawmemchr(buf, 0x00);
    assert(p == buf + 7);
}

// Test all possible byte values (0-255), ensuring `c` appears only at position 1
static void test_all_bytes(void) {
    unsigned char c;
    for (c = 0; ; c++) {
        // Use filler = (c + 1) to avoid collisions with `c`
        unsigned char filler = (c + 1) % 256;
        unsigned char buf[4] = {filler, c, filler, filler};
        void *p = rawmemchr(buf, c);
        assert(p == buf + 1);
        if (c == 255) break;
    }
}

// Test word-aligned search
static void test_aligned_word(void) {
    size_t buf[2];
    memset(buf, 0xEE, sizeof(buf));
    char *ptr = (char *)buf;
    ptr[sizeof(size_t)] = 0x55;
    char *p = rawmemchr(ptr, 0x55);
    assert(p == ptr + sizeof(size_t));
}

int main(void) {
    test_basic();
    test_end_of_buffer();
    test_unaligned();
    test_zero_byte();
    test_all_bytes();
    test_aligned_word();
    printf("All tests passed!\n");
    return 0;
}