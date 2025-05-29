/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
*@tc.name :memrchr_0100
*@tc.dest :Find a character in the middle of the memory block
*@tc.level:level 0
*/
void memrchr_0100() {
    const char *str = "abcdefg";
    size_t len = strlen(str);

    void *result = memrchr(str, 'd', len);
    assert(result == str + 3 && "memrchr_0100: 'd' not found at expected position");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0200
*@tc.dest :Try to find a non-existent character
*@tc.level:level 0
*/
void memrchr_0200() {
    const char *str = "abcdefg";
    size_t len = strlen(str);

    void *result = memrchr(str, 'z', len);
    assert(result == NULL && "memrchr_0200: 'z' should not be found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0300
*@tc.dest :Find the last character in the string
*@tc.level:level 0
*/
void memrchr_0300() {
    const char *str = "abcdefg";
    size_t len = strlen(str);

    void *result = memrchr(str, 'g', len);
    assert(result == str + len - 1 && "memrchr_0300: 'g' not found at end");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0400
*@tc.dest :Find the first character in the string
*@tc.level:level 0
*/
void memrchr_0400() {
    const char *str = "abcdefg";
    size_t len = strlen(str);

    void *result = memrchr(str, 'a', len);
    assert(result == str && "memrchr_0400: 'a' not found at start");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0500
*@tc.dest :Search length is zero
*@tc.level:level 0
*/
void memrchr_0500() {
    const char *str = "abcdefg";

    void *result = memrchr(str, 'a', 0);
    assert(result == NULL && "memrchr_0500: unexpected result when length=0");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0600
*@tc.dest :Search in empty string
*@tc.level:level 0
*/
void memrchr_0600() {
    const char *str = "";
    
    void *result = memrchr(str, 'a', 0);
    assert(result == NULL && "memrchr_0600: non-NULL result on empty input");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memrchr_0700
*@tc.dest :Find a non-printable character
*@tc.level:level 0
*/
void memrchr_0700() {
    char data[] = {0x01, 0x02, 0x07, 0x03};
    size_t len = sizeof(data);

    void *result = memrchr(data, 0x07, len);
    assert(result == &data[2] && "memrchr_0700: non-printable 0x07 not found");
    printf("[%s][Passed]\n", __func__);
}

int main() {
    printf("===== Starting memrchr Tests =====\n");

    memrchr_0100();
    memrchr_0200();
    memrchr_0300();
    memrchr_0400();
    memrchr_0500();
    memrchr_0600();
    memrchr_0700();

    printf("===== All memrchr Tests Completed =====\n");

    return 0;
}


