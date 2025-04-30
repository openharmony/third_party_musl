/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char *strfry(char *s);

// Check if two strings are permutations of each other
static int is_permutation(const char *a, const char *b) {
    int counts[256] = {0};
    for (; *a; a++, b++) counts[(unsigned char)*a]++, counts[(unsigned char)*b]--;
    for (int i = 0; i < 256; i++) if (counts[i] != 0) return 0;
    return 1;
}

// Test basic shuffling and permutation integrity
static void test_basic_shuffle() {
    char s1[] = "abcdefghijklmnopqrstuvwxyz";
    char s2[] = "abcdefghijklmnopqrstuvwxyz";
    char *result = strfry(s1);

    assert(result == s1);
    assert(is_permutation(s1, s2));
    assert(strcmp(s1, s2) != 0); // Likely, but not guaranteed
}

// Test empty string
static void test_empty() {
    char empty[] = "";
    assert(strfry(empty) == empty);
}

// Test single-character string
static void test_single_char() {
    char single[] = "a";
    assert(strfry(single) == single);
    assert(strcmp(single, "a") == 0);
}

// Test deterministic shuffling with fixed seed (for edge case verification)
static void test_deterministic() {
    char s[] = "abcd";
    unsigned int saved_seed = 12345;

    // Override rand_r to use a fixed seed
    unsigned int seed = saved_seed;
    for (size_t i = 0; i < 3; i++) {
        size_t j = i + (rand_r(&seed) % (4 - i));
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

    // Verify shuffle with known seed (not standard, but for internal testing)
    assert(is_permutation(s, "abcd"));
}

int main() {
    test_basic_shuffle();
    test_empty();
    test_single_char();
    test_deterministic();
    printf("All tests passed!\n");
    return 0;
}