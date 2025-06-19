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

void test_strlen_empty_string() {
    if (strlen("") != 0) {
        t_error("Empty string length should be 0\n");
    }
}

void test_strlen_with_spaces() {
    if (strlen(" ") != 1) {
        t_error("Length of single space should be 1\n");
    }
    if (strlen("  ") != 2) {
        t_error("Length of double space should be 2\n");
    }
    if (strlen(" a ") != 3) {
        t_error("Length of ' a ' should be 3\n");
    }
    if (strlen(" a b ") != 5) {
        t_error("Length of ' a b ' should be 5\n");
    }
    if (strlen("    ") != 4) {
        t_error("Length of 4 spaces should be 4\n");
    }
}

void test_strlen_special_chars() {
    if (strlen("!@#$%^&*()") != 10) {
        t_error("Length of special chars string should be 10\n");
    }
    if (strlen("[]{}<>?|") != 8) {
        t_error("Length of bracket string should be 8\n");
    }
    if (strlen("`~\\\"'") != 5) {
        t_error("Length of escaped chars string should be 5\n");
    }
}

void test_strlen_escaped_chars() {
    if (strlen("a\nb") != 3) {
        t_error("Length of 'a\\nb' should be 3\n");
    }
    if (strlen("a\tb") != 3) {
        t_error("Length of 'a\\tb' should be 3\n");
    }
    if (strlen("\n\n\n") != 3) {
        t_error("Length of '\\n\\n\\n' should be 3\n");
    }
    if (strlen("line1\nline2") != 11) {
        t_error("Length of multiline string should be 11\n");
    }
    if (strlen("tab\tseparated") != 13) {
        t_error("Length of tab-separated string should be 13\n");
    }
}

void test_strlen_with_embedded_null() {
    const char str1[] = {'a', '\0', 'b', '\0'};
    if (strlen(str1) != 1) {
        t_error("Length of string with embedded null should be 1\n");
    }

    const char str2[] = {'h', 'e', '\0', 'l', 'l', 'o'};
    if (strlen(str2) != 2) {
        t_error("Length of string with early null should be 2\n");
    }

    const char str3[] = {'\0', 'x'};
    if (strlen(str3) != 0) {
        t_error("Length of null-first string should be 0\n");
    }
}

void test_strlen_long_string() {
    char str100[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0";
    if (strlen(str100) != 100) {
        t_error("Length of 100-char string should be 100\n");
    }
    char str255[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                   "aaaaa\0";
    if (strlen(str255) != 255) {
        t_error("Length of 255-char string should be 255 actually %d\n", strlen(str255));
    }
}

void test_strlen_char_arrays() {
    char s1[10] = "";
    if (strlen(s1) != 0) {
        t_error("Length of empty array should be 0\n");
    }

    char s2[10] = "hi";
    if (strlen(s2) != 2) {
        t_error("Length of 'hi' should be 2\n");
    }

    char s3[5] = {'x', 'y', 'z', 0, 'q'};
    if (strlen(s3) != 3) {
        t_error("Length of 'xyz' array should be 3\n");
    }

    char s4[] = {'h', 'e', 'l', 'l', 'o', 0};
    if (strlen(s4) != 5) {
        t_error("Length of 'hello' array should be 5\n");
    }
}

void test_strlen_boundary_manual() {
    char s1[] = "a";
    if (strlen(s1) != 1) {
        t_error("Length of 'a' should be 1\n");
    }

    char s2[] = "aa";
    if (strlen(s2) != 2) {
        t_error("Length of 'aa' should be 2\n");
    }

    char s3[] = "aaa";
    if (strlen(s3) != 3) {
        t_error("Length of 'aaa' should be 3\n");
    }

    char s4[] = "aaaa";
    if (strlen(s4) != 4) {
        t_error("Length of 'aaaa' should be 4\n");
    }

    char s5[] = "aaaaa";
    if (strlen(s5) != 5) {
        t_error("Length of 'aaaaa' should be 5\n");
    }

    char s6[] = "aaaaaa";
    if (strlen(s6) != 6) {
        t_error("Length of 'aaaaaa' should be 6\n");
    }

    char s7[] = "aaaaaaa";
    if (strlen(s7) != 7) {
        t_error("Length of 'aaaaaaa' should be 7\n");
    }

    char s8[] = "aaaaaaaa";
    if (strlen(s8) != 8) {
        t_error("Length of 'aaaaaaaa' should be 8\n");
    }

    char s9[] = "aaaaaaaaa";
    if (strlen(s9) != 9) {
        t_error("Length of 'aaaaaaaaa' should be 9\n");
    }

    char s10[] = "aaaaaaaaaa";
    if (strlen(s10) != 10) {
        t_error("Length of 'aaaaaaaaaa' should be 10\n");
    }
}

void test_strlen_utf8() {
    if (strlen("你好") != 6) {
        t_error("Length of Chinese '你好' should be 6\n");
    }
    if (strlen("世界") != 6) {
        t_error("Length of Chinese '世界' should be 6\n");
    }
    if (strlen("你好世界") != 12) {
        t_error("Length of Chinese '你好世界' should be 12\n");
    }
}

void test_strlen_misc_cases() {
    char s[] = "test\0hidden";
    if (strlen(s) != 4) {
        t_error("Length of 'test\\0hidden' should be 4\n");
    }

    const char *p = "pointer test";
    if (strlen(p) != 12) {
        t_error("Length of 'pointer test' should be 12\n");
    }

    char s2[] = {'t', 'e', 's', 't', 0};
    if (strlen(s2) != 4) {
        t_error("Length of 'test' array should be 4\n");
    }

    char s3[] = {0};
    if (strlen(s3) != 0) {
        t_error("Length of null array should be 0\n");
    }
}

void test_strlen_concatenated_literals() {
    if (strlen("hello" "world") != 10) {
        t_error("Length of concatenated 'helloworld' should be 10\n");
    }
    if (strlen("foo" "bar" "baz") != 9) {
        t_error("Length of concatenated 'foobarbaz' should be 9\n");
    }
    if (strlen("" "") != 0) {
        t_error("Length of concatenated empty strings should be 0\n");
    }
    if (strlen("abc" "") != 3) {
        t_error("Length of 'abc' concatenated with empty should be 3\n");
    }
    if (strlen("" "xyz") != 3) {
        t_error("Length of empty concatenated with 'xyz' should be 3\n");
    }
}

void test_strlen_repeated_chars() {
    char a[] = "aaaaaaaaaaaaaaaaaaaa";
    if (strlen(a) != 20) {
        t_error("Length of 20 'a's should be 20\n");
    }

    char b[] = "bbbbbbbbbbbbbbbbb";
    if (strlen(b) != 17) {
        t_error("Length of 17 'b's should be 17\n");
    }

    char c[] = "cccccccccccc";
    if (strlen(c) != 12) {
        t_error("Length of 12 'c's should be 12\n");
    }
}

void test_strlen_with_pointer_offsets() {
    char base[] = "abcdef";
    if (strlen(base + 1) != 5) {
        t_error("Length from offset 1 should be 5\n");
    }
    if (strlen(base + 2) != 4) {
        t_error("Length from offset 2 should be 4\n");
    }
    if (strlen(base + 5) != 1) {
        t_error("Length from offset 5 should be 1\n");
    }
    if (strlen(base + 6) != 0) {
        t_error("Length from offset 6 should be 0\n");
    }
}

void test_strlen_static_consts() {
    static const char *a = "const1";
    if (strlen(a) != 6) {
        t_error("Length of 'const1' should be 6\n");
    }

    static const char *b = "a slightly longer const string";
    if (strlen(b) != strlen("a slightly longer const string")) {
        t_error("Length of long const string mismatch\n");
    }

    static const char *c = "";
    if (strlen(c) != 0) {
        t_error("Length of empty const string should be 0\n");
    }
}

void test_strlen_stack_allocated() {
    char a[6] = "hello";
    if (strlen(a) != 5) {
        t_error("Length of stack 'hello' should be 5\n");
    }

    char b[] = {'w', 'o', 'r', 'l', 'd', '\0'};
    if (strlen(b) != 5) {
        t_error("Length of stack 'world' should be 5\n");
    }
}

void test_strlen_with_backslash_sequences() {
    if (strlen("a\\b\\c") != 5) {
        t_error("Length of 'a\\\\b\\\\c' should be 5\n");
    }
    if (strlen("line\\n") != 6) {
        t_error("Length of 'line\\\\n' should be 6\n");
    }
    if (strlen("quote\\\"") != 7) {
        t_error("Length of 'quote\\\\\"' should be 7\n");
    }
}

void test_strlen_inline_null_characters() {
    char a[] = {'A', 'B', 'C', '\0', 'D', 'E', '\0'};
    if (strlen(a) != 3) {
        t_error("Length of array with inline null should be 3\n");
    }

    char b[] = {'x', '\0', '\0', 'z'};
    if (strlen(b) != 1) {
        t_error("Length of array with early null should be 1\n");
    }

    char c[] = {'\0', 'a', 'b'};
    if (strlen(c) != 0) {
        t_error("Length of array with first null should be 0\n");
    }
}

void test_strlen_extended_ascii() {
    char a[] = {127, 0};
    if (strlen(a) != 1) {
        t_error("Length of extended ASCII 127 should be 1\n");
    }

    char b[] = {(char)200, (char)201, 0};
    if (strlen(b) != 2) {
        t_error("Length of extended ASCII 200,201 should be 2\n");
    }

    char c[] = {(char)255, 0};
    if (strlen(c) != 1) {
        t_error("Length of extended ASCII 255 should be 1\n");
    }
}

void test_strlen_null_at_end_of_buffer() {
    char a[100];
    memset(a, 'x', 99);
    a[99] = '\0';
    if (strlen(a) != 99) {
        t_error("Length of 99 'x's should be 99\n");
    }
}

void test_strlen_partial_init_array() {
    char s[10] = "abc";
    if (strlen(s) != 3) {
        t_error("Length of partially initialized 'abc' should be 3\n");
    }

    char t[20] = "abcdefgh";
    if (strlen(t) != 8) {
        t_error("Length of partially initialized 'abcdefgh' should be 8\n");
    }
}

void test_strlen_max_safe() {
    char longbuf[256];
    for (int i = 0; i < 255; ++i) {
        longbuf[i] = 'a';
    }
    longbuf[255] = '\0';
    if (strlen(longbuf) != 255) {
        t_error("Length of 255 'a's should be 255\n");
    }
}

void test_strlen_manual_truncate() {
    char s[] = "abcdefghij";
    s[5] = '\0';
    if (strlen(s) != 5) {
        t_error("Length after truncate at 5 should be 5\n");
    }

    s[1] = '\0';
    if (strlen(s) != 1) {
        t_error("Length after truncate at 1 should be 1\n");
    }
}

void test_strlen_weird_printables() {
    char s1[] = "abc\tdef";
    if (strlen(s1) != 7) {
        t_error("Length of 'abc\\tdef' should be 7\n");
    }

    char s2[] = "abc\vdef";
    if (strlen(s2) != 7) {
        t_error("Length of 'abc\\vdef' should be 7\n");
    }
}

void test_strlen_whitespace_mix() {
    if (strlen("    ") != 4) {
        t_error("Length of 4 spaces should be 4\n");
    }
    if (strlen(" \t \n ") != 5) {
        t_error("Length of mixed whitespace should be 5\n");
    }
    if (strlen("\t\t\t") != 3) {
        t_error("Length of 3 tabs should be 3\n");
    }
}

void test_strlen_reverse_embedded_null() {
    char s[] = {'z', 'y', 'x', '\0', 'w', 'v'};
    if (strlen(s) != 3) {
        t_error("Length with reverse embedded null should be 3\n");
    }
}

void test_strlen_manual_fill_buffer() {
    char s[6];
    s[0] = 'a';
    s[1] = 'b';
    s[2] = 'c';
    s[3] = 'd';
    s[4] = '\0';
    s[5] = 'x';
    if (strlen(s) != 4) {
        t_error("Length of manually filled buffer should be 4\n");
    }
}

void test_strlen_dynamic_init_short() {
    char s[4] = {0};
    s[0] = 'a';
    s[1] = 'b';
    s[2] = 'c';
    s[3] = '\0';
    if (strlen(s) != 3) {
        t_error("Length of dynamically initialized buffer should be 3\n");
    }
}

void test_strlen_unicode_mix() {
    if (strlen("a你b好c") != 9) {
        t_error("Length of mixed Unicode string should be 9\n");
    }
    if (strlen("αβγ") != 6) {
        t_error("Length of Greek letters should be 6\n");
    }
}

void test_strlen_very_long_string() {
    char very_long[1025];
    memset(very_long, 'x', 1024);
    very_long[1024] = '\0';
    if (strlen(very_long) != 1024) {
        t_error("Length of very long string should be 1024\n");
    }
}

void test_strlen_null_only() {
    char s[] = {'\0'};
    if (strlen(s) != 0) {
        t_error("Length of null-only string should be 0\n");
    }
}

void test_strlen_alternating_chars() {
    char s[] = "a\0b\0c\0d";
    if (strlen(s) != 1) {
        t_error("Length of alternating chars string should be 1\n");
    }
}

void test_strlen_all_control_chars() {
    char s[] = {1, 2, 3, 4, 5, 0};
    if (strlen(s) != 5) {
        t_error("Length of control chars string should be 5\n");
    }
}

void test_strlen_hex_chars() {
    char s[] = "\x41\x42\x43\x00\x44";
    if (strlen(s) != 3) {
        t_error("Length of hex string should be 3\n");
    }
}

void test_strlen_octal_chars() {
    char s[] = "\101\102\103\000\104";
    if (strlen(s) != 3) {
        t_error("Length of octal string should be 3\n");
    }
}

void test_strlen_one_past_null() {
    char s[] = "abc\0d";
    if (strlen(s+4) != 1) {
        t_error("Length past null should be 1\n");
    }
}

void test_strlen_empty_after_null() {
    char s[] = "\0abc";
    if (strlen(s+1) != 3) {
        t_error("Length after null should be 3\n");
    }
}

int main() {
    test_strlen_empty_string();
    test_strlen_with_spaces();
    test_strlen_special_chars();
    test_strlen_escaped_chars();
    test_strlen_with_embedded_null();
    test_strlen_long_string();
    test_strlen_char_arrays();
    test_strlen_boundary_manual();
    test_strlen_utf8();
    test_strlen_misc_cases();
    test_strlen_concatenated_literals();
    test_strlen_repeated_chars();
    test_strlen_with_pointer_offsets();
    test_strlen_static_consts();
    test_strlen_stack_allocated();
    test_strlen_with_backslash_sequences();
    test_strlen_inline_null_characters();
    test_strlen_extended_ascii();
    test_strlen_null_at_end_of_buffer();
    test_strlen_partial_init_array();
    test_strlen_max_safe();
    test_strlen_manual_truncate();
    test_strlen_weird_printables();
    test_strlen_whitespace_mix();
    test_strlen_reverse_embedded_null();
    test_strlen_manual_fill_buffer();
    test_strlen_dynamic_init_short();
    test_strlen_unicode_mix();
    test_strlen_very_long_string();
    test_strlen_null_only();
    test_strlen_alternating_chars();
    test_strlen_all_control_chars();
    test_strlen_hex_chars();
    test_strlen_octal_chars();
    test_strlen_one_past_null();
    test_strlen_empty_after_null();
    return 0;
}