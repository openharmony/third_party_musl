/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 *
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

#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
*@tc.name :memccpy_0100
*@tc.dest :Find the specified character 'c' within n bytes
*@tc.level:level 0
*/
void memccpy_0100() {
    const char *src = "hello world";
    char dest[20] = {0};

    void *result = memccpy(dest, src, ' ', 12);
    assert(result != NULL && ((char *)result)[-1] == ' ' && "memccpy_0100: space not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0200
*@tc.dest :The specified character 'c' is not found within n bytes
*@tc.level:level 0
*/
void memccpy_0200() {
    const char *src = "abcdefg";
    char dest[10] = {0};

    void *result = memccpy(dest, src, 'g', 5);
    assert(result == NULL && memcmp(dest, "abcde", 5) == 0 && "memccpy_0200: 'g' found unexpectedly");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0300
*@tc.dest :The first character matches
*@tc.level:level 0
*/
void memccpy_0300() {
    const char *src = "abcde";
    char dest[10] = {0};
    size_t len = strlen(src);

    void *result = memccpy(dest, src, 'a', len);
    assert(result != NULL && ((char *)result)[-1] == 'a' && "memccpy_0300: first char 'a' not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0400
*@tc.dest :Character found at the last byte
*@tc.level:level 0
*/
void memccpy_0400() {
    const char *src = "hello world!";
    char dest[20] = {0};
    size_t len = strlen(src);

    void *result = memccpy(dest, src, '!', len);
    assert(result != NULL && ((char *)result)[-1] == '!' && "memccpy_0400: '!' not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0500
*@tc.dest :Search length is zero
*@tc.level:level 0
*/
void memccpy_0500() {
    const char *src = "abcdefg";
    char dest[20] = {0};
    char expected[20] = {0};

    void *result = memccpy(dest, src, 'c', 0);
    assert(result == NULL && memcmp(dest, expected, sizeof(dest)) == 0 && "memccpy_0500: unexpected copy when n=0");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0600
*@tc.dest :Target character is '\0'
*@tc.level:level 0
*/
void memccpy_0600() {
    const char src[] = "hello\0world";
    char dest[20] = {0};

    void *result = memccpy(dest, src, '\0', 12);
    assert(result != NULL && ((char *)result)[-1] == '\0' && "memccpy_0600: null byte not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memccpy_0700
*@tc.dest :Character not in range
*@tc.level:level 0
*/
void memccpy_0700() {
    const char *src = "abcdefgh";
    char dest[20] = {0};

    void *result = memccpy(dest, src, 'x', 5);
    assert(result == NULL && memcmp(dest, "abcde", 5) == 0 && "memccpy_0700: unexpected match for 'x'");
    printf("[%s][Passed]\n", __func__);
}

int main() {
    printf("===== Starting memccpy Tests =====\n");

    memccpy_0100();
    memccpy_0200();
    memccpy_0300();
    memccpy_0400();
    memccpy_0500();
    memccpy_0600();
    memccpy_0700();

    printf("===== All memccpy Tests Completed =====\n");

    return 0;
}


