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
*@tc.name :memcmp_0100
*@tc.dest :Compare two identical memory blocks
*@tc.level:level 0
*/
void memcmp_0100() {
    char str1[] = "abcdef";
    char str2[] = "abcdef";
    size_t len = sizeof(str1);

    int result = memcmp(str1, str2, len);
    assert(result == 0 && "memcmp_0100: expected equal memory blocks");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0200
*@tc.dest :Compare two different memory blocks (str1 < str2)
*@tc.level:level 0
*/
void memcmp_0200() {
    char str1[] = "abcde";
    char str2[] = "abdce";
    size_t len = sizeof(str1);

    int result = memcmp(str1, str2, len);
    assert(result < 0 && "memcmp_0200: expected 'abcde' < 'abdce'");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0300
*@tc.dest :Compare two empty memory blocks
*@tc.level:level 0
*/
void memcmp_0300() {
    char str1[] = "";
    char str2[] = "";
    size_t len = strlen(str1) + 1;

    int result = memcmp(str1, str2, len);
    assert(result == 0 && "memcmp_0300: expected equal empty memory blocks");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0400
*@tc.dest :Compare binary memory blocks
*@tc.level:level 0
*/
void memcmp_0400() {
    unsigned char data1[] = {0x01, 0x02, 0x03, 0x04};
    unsigned char data2[] = {0x01, 0x02, 0x03, 0x04};
    size_t len = sizeof(data1);

    int result = memcmp(data1, data2, len);
    assert(result == 0 && "memcmp_0400: expected equal binary memory blocks");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0500
*@tc.dest :Compare memory blocks containing null characters
*@tc.level:level 0
*/
void memcmp_0500() {
    char data1[] = {'a', '\0', 'b'};
    char data2[] = {'a', '\0', 'b'};
    size_t len = sizeof(data1);

    int result = memcmp(data1, data2, len);
    assert(result == 0 && "memcmp_0500: expected equal with null chars");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0600
*@tc.dest :Compare memory blocks beyond string length
*@tc.level:level 0
*/
void memcmp_0600() {
    char data1[10] = "abc";
    char data2[10] = "abc";
    size_t len = 10;

    int result = memcmp(data1, data2, len);
    assert(result == 0 && "memcmp_0600: expected equal even when comparing beyond string length");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memcmp_0700
*@tc.dest :Compare completely different memory blocks
*@tc.level:level 0
*/
void memcmp_0700() {
    char data1[] = "abcdefgh";
    char data2[] = "xyz12345";
    size_t len = strlen(data1) + 1;

    int result = memcmp(data1, data2, len);
    assert(result != 0 && "memcmp_0700: expected difference between blocks");
    printf("[%s][Passed]\n", __func__);
}

int main() {
    printf("===== Starting memcmp Tests =====\n");

    memcmp_0100();
    memcmp_0200();
    memcmp_0300();
    memcmp_0400();
    memcmp_0500();
    memcmp_0600();
    memcmp_0700();

    printf("===== All memcmp Tests Completed =====\n");

    return 0;
}

