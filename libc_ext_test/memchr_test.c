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
*@tc.name :memchr_0100
*@tc.dest :Find an existing character in the string
*@tc.level:level 0
*/
void memchr_0100() {
    const char *str = "Hello world!";
    char ch = 'o';
    size_t len = strlen(str);

    void *result = memchr(str, ch, len);
    assert(result != NULL && "memchr_0100: 'o' not found");
    printf("[%s][Passed]\n", __func__);
    }
}

/**
*@tc.name :memchr_0200
*@tc.dest :Try to find a character that does not exist
*@tc.level:level 0
*/
void memchr_0200() {
    const char *str = "Hello world!";
    char ch = 'z';
    size_t len = strlen(str);

    void *result = memchr(str, ch, len);
    assert(result == NULL && "memchr_0200: 'z' not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0300
*@tc.dest :Find the null character
*@tc.level:level 0
*/
void memchr_0300() {
    const char data[] = "abc\0def";
    size_t len = sizeof(data);

    void *result = memchr(data, '\0', len);
    assert(result !=NULL && *(char *)result == '\0' && "memchr_0300: '\\0' not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0400
*@tc.dest :Find an empty string
*@tc.level:level 0
*/
void memchr_0400() {
    const char *str = "";
    char c = 'a';
    size_t len = 0;

    void *result = memchr(str, c, len);
    assert(result == NULL && "memchr_0400: non-NULL on empty input");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0500
*@tc.dest :Find the first character of a string
*@tc.level:level 0
*/
void memchr_0500() {
    const char *str = "abcdefg";
    char c = 'a';
    size_t len = strlen(str);

    void *result = memchr(str, c, len);
    assert(result == str && "memchr_0500: 'a' not found at start");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0600
*@tc.dest :Find the last character of a string
*@tc.level:level 0
*/
void memchr_0600() {
    const char *str = "abcdefg";
    char c = 'g';
    size_t len = strlen(str);

    void *result = memchr(str, c, len);
    assert(result == str + len - 1 && "memchr_0600: 'g' not found at end");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0700
*@tc.dest :search length exceeds actual string length
*@tc.level:level 0
*/
void memchr_0700() {
    const char *str = "Hello world!";
    char ch = '!';
    size_t actual_len = strlen(str);
    size_t search_len = actual_len + 10;

    void *result = memchr(str, ch, search_len);
    assert(result != NULL && *(char *)result == ch && "memchr_0700: '!' not found with extended length");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0800
*@tc.dest :Find non-printable characters
*@tc.level:level 0
*/
void memchr_0800() {
    char data[] = {0x01, 0x02, 0x07, 0x03};
    size_t len = sizeof(data);

    void *result = memchr(data, 0x07, len);
    assert(result == &data[2] && "memchr_0800: non-printable 0x07 not found");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_0900
*@tc.dest :Matching character in the middle, but the search length is insufficient
*@tc.level:level 0
*/
void memchr_0900() {
    const char *str = "abcdefgh";
    char c = 'd';
    size_t len = 3;

    void *result = memchr(str, c, len);
    assert(result == NULL && "memchr_0900: 'd' found beyond search length");
    printf("[%s][Passed]\n", __func__);
}

/**
*@tc.name :memchr_1000
*@tc.dest :Search length is zero
*@tc.level:level 0
*/
void memchr_1000() {
    const char *str = "abcdefg";
    char c = 'd';
    size_t len = 0;

    void *result = memchr(str, c, len);
    assert(result == NULL && "memchr_1000: unexpected result when len=0");
    printf("[%s][Passed]\n", __func__);
}

int main(){
    printf("===== Starting memchr Tests =====\n");

    memchr_0100();
    memchr_0200();
    memchr_0300();
    memchr_0400();
    memchr_0500();
    memchr_0600();
    memchr_0700();
    memchr_0800();
    memchr_0900();
    memchr_1000();
    
    printf("===== All memchr Tests Completed =====\n");

    return 0;
}

