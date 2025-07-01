/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "test.h"

void test_strcmp_basic() {
    const char *s1 = "abc";
    const char *s2 = "abc";
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp(\"%s\", \"%s\") should return 0\n", s1, s2);
    }

    const char *s3 = "efg";
    const char *s4 = "efg";
    if (strcmp(s3, s4) != 0) {
        t_error("strcmp(\"%s\", \"%s\") should return 0\n", s3, s4);
    }
}

void test_strcmp_empty() {
    const char *s1 = "";
    const char *s2 = "";
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp empty strings failed (expected 0)\n");
    }
}

void test_strcmp_less() {
    const char *s1 = "apple";
    const char *s2 = "banana";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp(\"%s\", \"%s\") should return negative\n", s1, s2);
    }

    const char *s3 = "1234";
    const char *s4 = "23456";
    if (strcmp(s3, s4) >= 0) {
        t_error("strcmp(\"%s\", \"%s\") should return negative\n", s3, s4);
    }
}

void test_strcmp_greater() {
    const char *s1 = "zebra";
    const char *s2 = "apple";
    if (strcmp(s1, s2) <= 0) {
        t_error("strcmp(\"%s\", \"%s\") should return positive\n", s1, s2);
    }
}

void test_strcmp_case_sensitive() {
    const char *s1 = "Hello";
    const char *s2 = "hello";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp case-sensitive check failed (expected negative)\n");
    }

    const char *s3 = "World";
    const char *s4 = "world";
    if (strcmp(s3, s4) >= 0) {
        t_error("strcmp case-sensitive check failed (expected negative)\n");
    }
}

void test_strcmp_partial_match() {
    const char *s1 = "abcde";
    const char *s2 = "abcxyz";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp partial match failed (expected negative)\n");
    }

    const char *s3 = "12345";
    const char *s4 = "1234567890";
    if (strcmp(s3, s4) >= 0) {
        t_error("strcmp partial match failed (expected negative)\n");
    }
}

void test_strcmp_special_chars() {
    const char *s1 = "a$b*c";
    const char *s2 = "a$b*c";
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp special characters failed (expected 0)\n");
    }

    const char *s3 = "1&2^3";
    const char *s4 = "1&2^3";
    if (strcmp(s3, s4) != 0) {
        t_error("strcmp special characters failed (expected 0)\n");
    }
}

void test_strcmp_numeric() {
    const char *s1 = "123";
    const char *s2 = "456";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp numeric strings failed (expected negative)\n");
    }

    const char *s3 = "123456";
    const char *s4 = "456789";
    if (strcmp(s3, s4) >= 0) {
        t_error("strcmp numeric strings failed (expected negative)\n");
    }
}

void test_strcmp_long_strings() {
    const char *s1 = "This is a very long string with spaces and symbols @#!";
    const char *s2 = "This is a very long string with spaces and symbols @#!";
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp long strings failed (expected 0)\n");
    }
}

void test_strcmp_diff_length() {
    const char *s1 = "short";
    const char *s2 = "longer_string";
    if (strcmp(s1, s2) <= 0) {
        t_error("strcmp shorter vs longer failed (expected positive1)\n");
    }
}

void test_strcmp_leading_space() {
    const char *s1 = "  apple";
    const char *s2 = "apple";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp leading spaces failed (expected negative)\n");
    }
}

void test_strcmp_trailing_space() {
    const char *s1 = "apple  ";
    const char *s2 = "apple";
    if (strcmp(s1, s2) <= 0) {
        t_error("strcmp trailing spaces failed (expected positive)\n");
    }
}

void test_strcmp_utf8() {
    const char *s1 = "café";
    const char *s2 = "cafe";
    if (strcmp(s1, s2) <= 0) {
        t_error("strcmp UTF-8 chars failed (expected positive)\n");
    }
}

void test_strcmp_alphanumeric() {
    const char *s1 = "abc123";
    const char *s2 = "abc456";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp alphanumeric failed (expected negative)\n");
    }
}

void test_strcmp_completely_diff() {
    const char *s1 = "hello";
    const char *s2 = "world";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp completely different strings failed (expected negative)\n");
    }
}

void test_strcmp_same_pointer() {
    const char *s1 = "unique_string";
    const char *s2 = s1;
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp same pointer failed (expected 0)\n");
    }
}

void test_strcmp_single_char() {
    const char *s1 = "a";
    const char *s2 = "b";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp single char failed (expected negative)\n");
    }
}

void test_strcmp_huge_string() {
    char s1[100000] = {0};
    char s2[100000] = {0};
    memset(s1, 'a', sizeof(s1) - 1);
    memset(s2, 'a', sizeof(s2) - 1);
    s1[sizeof(s1)-1] = '\0';
    s2[sizeof(s2)-1] = '\0';
    if (strcmp(s1, s2) != 0) {
        t_error("strcmp huge strings failed (expected 0)\n");
    }
}

void test_strcmp_numeric_prefix() {
    const char *s1 = "123apple";
    const char *s2 = "456apple";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp numeric prefix failed (expected negative)\n");
    }
}

void test_strcmp_zero_vs_nonzero() {
    const char *s1 = "";
    const char *s2 = "a";
    if (strcmp(s1, s2) >= 0) {
        t_error("strcmp empty vs non-empty failed (expected negative)\n");
    }
}

int main() {
    test_strcmp_basic();
    test_strcmp_empty();
    test_strcmp_less();
    test_strcmp_greater();
    test_strcmp_case_sensitive();
    test_strcmp_partial_match();
    test_strcmp_special_chars();
    test_strcmp_numeric();
    test_strcmp_long_strings();
    test_strcmp_diff_length();
    test_strcmp_leading_space();
    test_strcmp_trailing_space();
    test_strcmp_utf8();
    test_strcmp_alphanumeric();
    test_strcmp_completely_diff();
    test_strcmp_same_pointer();
    test_strcmp_single_char();
    test_strcmp_huge_string();
    test_strcmp_numeric_prefix();
    test_strcmp_zero_vs_nonzero();

    return 0;
}