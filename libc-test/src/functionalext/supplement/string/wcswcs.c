/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

#define N(s, sub)                                                                  \
    {                                                                              \
        wchar_t *p = s;                                                            \
        wchar_t *q = wcswcs(p, sub);                                               \
        if (q)                                                                     \
            t_error("wcswcs(%s,%s) returned str+%d, wanted 0\n", #s, #sub, q - p); \
    }

#define T(s, sub, n)                                                                       \
    {                                                                                      \
        wchar_t *p = s;                                                                    \
        wchar_t *q = wcswcs(p, sub);                                                       \
        if (q == 0)                                                                        \
            t_error("wcswcs(%s,%s) returned 0, wanted str+%d\n", #s, #sub, n);             \
        else if (q - p != n)                                                               \
            t_error("wcswcs(%s,%s) returned str+%d, wanted str+%d\n", #s, #sub, q - p, n); \
    }

/**
 * @tc.name      : wcswcs_0100
 * @tc.desc      : Test the wcswcs method to find the first occurrence of the second wide string from the first wide
 *                 string
 * @tc.level     : Level 0
 */
void wcswcs_0100(void)
{
    T(L"", L"", 0)
    T(L"abcd", L"", 0)
    T(L"abcd", L"a", 0)
    T(L"abcd", L"b", 1)
    T(L"abcd", L"c", 2)
    T(L"abcd", L"d", 3)
    T(L"abcd", L"ab", 0)
    T(L"abcd", L"bc", 1)
    T(L"abcd", L"cd", 2)
    T(L"ababa", L"baba", 1)
    T(L"ababab", L"babab", 1)
    T(L"abababa", L"bababa", 1)
    T(L"abababab", L"bababab", 1)
    T(L"ababababa", L"babababa", 1)
    T(L"abbababab", L"bababa", 2)
    T(L"abbababab", L"ababab", 3)
    T(L"abacabcabcab", L"abcabcab", 4)
    T(L"nanabanabanana", L"aba", 3)
    T(L"nanabanabanana", L"ban", 4)
    T(L"nanabanabanana", L"anab", 1)
    T(L"nanabanabanana", L"banana", 8)
    T(L"_ _\xff_ _", L"_\xff_", 2)
}

/**
 * @tc.name      : wcswcs_0200
 * @tc.desc      : Test the result of the wcswcs method when the second substring cannot be found from the first
 * substring
 * @tc.level     : Level 1
 */
void wcswcs_0200(void)
{
    N(L"", L"a")
    N(L"a", L"aa")
    N(L"a", L"b")
    N(L"aa", L"ab")
    N(L"aa", L"aaa")
    N(L"abba", L"aba")
    N(L"abc abc", L"abcd")
    N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-56-7-8-")
    N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-5+6-7-8-")
    N(L"_ _ _\xff_ _ _", L"_\x7f_")
    N(L"_ _ _\x7f_ _ _", L"_\xff_")
}

/**
 * @tc.name      : wcswcs_0300
 * @tc.desc      : Test wcswcs with overlapping patterns and edge cases
 * @tc.level     : Level 1
 */
void wcswcs_0300(void)
{
    // Overlapping pattern tests
    T(L"aaaabaaaa", L"aaa", 0)
    T(L"mississippi", L"issi", 1)
    T(L"abcabcabc", L"abcabc", 0)
    T(L"ababababab", L"ababab", 0)

    // Edge cases
    T(L"x", L"x", 0)
    T(L"xy", L"xy", 0)
    T(L"xyz", L"yz", 1)
    T(L"a\0b", L"a", 0)  // String containing null character
}

/**
 * @tc.name      : wcswcs_0400
 * @tc.desc      : Test wcswcs with very long strings and patterns
 * @tc.level     : Level 2
 */
void wcswcs_0400(void)
{
    wchar_t long_str[100];
    wchar_t pattern[20];

    // Generate long string "abcabc...abc"
    for (int i = 0; i < 33; i++) {
        wcscpy(long_str + i * 3, L"abc");
    }
    long_str[99] = L'\0';

    T(long_str, L"abc", 0)
    T(long_str, L"bca", 1)
    T(long_str, L"abcabc", 0)

    // Test long pattern
    wcscpy(pattern, L"abcabcabcabc");
    T(long_str, pattern, 0)
}

/**
 * @tc.name      : wcswcs_0500
 * @tc.desc      : Test wcswcs with whitespace characters and control characters
 * @tc.level     : Level 1
 */
void wcswcs_0500(void)
{
    // Whitespace character tests
    T(L"hello world", L" ", 5)
    T(L"tab\ttab", L"\t", 3)
    T(L"new\nline", L"\n", 3)
    T(L"carriage\rreturn", L"\r", 8)

    // Mixed whitespace characters
    T(L"a b c d", L" c ", 3)
    T(L"test\t\n\rstring", L"\t\n\r", 4)
}

/**
 * @tc.name      : wcswcs_0600
 * @tc.desc      : Test wcswcs with repeated characters and patterns
 * @tc.level     : Level 0
 */
void wcswcs_0600(void)
{
    // Repeated character tests
    T(L"aaaaaaaaaa", L"aaa", 0)
    T(L"aaaaaaaaaa", L"aaaaa", 0)
    T(L"ababababab", L"abab", 0)
    T(L"abcabcabc", L"bcab", 1)

    // Partial matches that ultimately fail
    N(L"abcdeabcdf", L"abcdg")  // Should not find "abcdg"
    N(L"xyzxyzx", L"xyzb")
}

/**
 * @tc.name      : wcswcs_0700
 * @tc.desc      : Test wcswcs with case sensitivity and character variations
 * @tc.level     : Level 1
 */
void wcswcs_0700(void)
{
    // Case sensitivity tests
    T(L"Hello World", L"World", 6)
    N(L"Hello WORLD", L"World")

    // Special character variants
    T(L"café au lait", L"é", 3)
    T(L"naïve approach", L"ï", 2)
    T(L"über", L"ü", 0)
}

/**
 * @tc.name      : wcswcs_0800
 * @tc.desc      : Test wcswcs performance with partial matches and near misses
 * @tc.level     : Level 2
 */
void wcswcs_0800(void)
{
    // Partial match tests
    T(L"abcdefghijklmnopqrstuvwxyz", L"xyz", 23)
    T(L"abcdeffghijkl", L"ffg", 5)
    T(L"abracadabra", L"dab", 6)

    // Close but not exact matches
    N(L"abcdefgh", L"abcdeg")
    N(L"hello world", L"worlds")
    N(L"programming", L"programmer")

    // Pattern longer than the string
    N(L"short", L"this pattern is much longer than the string")
}

int main(int argc, char *argv[])
{
    wcswcs_0100();
    wcswcs_0200();
    wcswcs_0300();
    wcswcs_0400();
    wcswcs_0500();
    wcswcs_0600();
    wcswcs_0700();
    wcswcs_0800();
    return t_status;
}