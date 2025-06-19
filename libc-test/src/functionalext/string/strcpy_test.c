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

void test_strcpy_basic() {
    char dest[10];
    strcpy(dest, "abc");
    if (strcmp(dest, "abc")) {
        t_error("Expected 'abc', got '%s'\n", dest);
    }
}

void test_strcpy_empty() {
    char dest[10] = "xxxxxx";
    strcpy(dest, "");
    if (strcmp(dest, "")) {
        t_error("Expected empty string, got '%s'\n", dest);
    }
}

void test_strcpy_longer_string() {
    char dest[100];
    strcpy(dest, "This is a longer string with multiple words.");
    if (strcmp(dest, "This is a longer string with multiple words.")) {
        t_error("Long string copy failed\n");
    }
}

void test_strcpy_special_characters() {
    char dest[20];
    strcpy(dest, "@#$%^&*()");
    if (strcmp(dest, "@#$%^&*()")) {
        t_error("Special characters copy failed\n");
    }
}

void test_strcpy_single_char() {
    char dest[10];
    strcpy(dest, "Z");
    if (strcmp(dest, "Z")) {
        t_error("Single char copy failed\n");
    }
}

void test_strcpy_case_mixed() {
    char dest[20];
    strcpy(dest, "AbCdEfGh");
    if (strcmp(dest, "AbCdEfGh")) {
        t_error("Mixed case copy failed\n");
    }
}

void test_strcpy_with_spaces() {
    char dest[20];
    strcpy(dest, "   ");
    if (strcmp(dest, "   ")) {
        t_error("Spaces copy failed\n");
    }
}

void test_strcpy_with_escapes() {
    char dest[20];
    strcpy(dest, "Line1\nLine2");
    if (strcmp(dest, "Line1\nLine2")) {
        t_error("Escapes copy failed\n");
    }
}

void test_strcpy_overwrite_old_content() {
    char dest[20] = "OldContentHere";
    strcpy(dest, "New");
    if (strcmp(dest, "New")) {
        t_error("Overwrite failed\n");
    }
}

void test_strcpy_with_embedded_null() {
    char src[10] = {'a', 'b', '\0', 'c'};
    char dest[10];
    strcpy(dest, src);
    if (dest[0] != 'a' || dest[1] != 'b' || dest[2] != '\0') {
        t_error("Embedded null copy failed\n");
    }
}

void test_strcpy_modify_src_after_copy() {
    char src[] = "copyme";
    char dest[20];
    strcpy(dest, src);
    src[0] = 'X';
    if (strcmp(dest, "copyme")) {
        t_error("Source modification affected destination\n");
    }
}

void test_strcpy_utf8() {
    char dest[20];
    strcpy(dest, "café");
    if (strcmp(dest, "café")) {
        t_error("UTF-8 copy failed\n");
    }
}

void test_strcpy_multiple_assignments() {
    char dest[20];
    strcpy(dest, "first");
    if (strcmp(dest, "first")) {
        t_error("First assignment failed\n");
    }
    strcpy(dest, "second");
    if (strcmp(dest, "second")) {
        t_error("Second assignment failed\n");
    }
}

void test_strcpy_to_offset() {
    char buffer[20] = "1234567890";
    strcpy(buffer + 5, "abc");
    if (strcmp(buffer + 5, "abc")) {
        t_error("Offset copy failed\n");
    }
}

void test_strcpy_array_vs_pointer() {
    char a[10];
    char *b = a;
    strcpy(b, "test");
    if (strcmp(a, "test")) {
        t_error("Array vs pointer copy failed\n");
    }
}

void test_strcpy_all_caps() {
    char dest[30];
    strcpy(dest, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (strcmp(dest, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
        t_error("All caps copy failed\n");
    }
}

void test_strcpy_sentence() {
    char dest[100];
    strcpy(dest, "The quick brown fox jumps over the lazy dog.");
    if (strcmp(dest, "The quick brown fox jumps over the lazy dog.")) {
        t_error("Sentence copy failed\n");
    }
}

void test_strcpy_series() {
    char dest[50];
    strcpy(dest, "one");
    if (strcmp(dest, "one")) {
        t_error("First in series failed\n");
    }
    strcpy(dest, "two");
    if (strcmp(dest, "two")) {
        t_error("Second in series failed\n");
    }
    strcpy(dest, "three");
    if (strcmp(dest, "three")) {
        t_error("Third in series failed\n");
    }
}

void test_strcpy_manual_src_array() {
    char src[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    char dest[10];
    strcpy(dest, src);
    if (strcmp(dest, "hello")) {
        t_error("Manual array copy failed\n");
    }
}

void test_strcpy_vs_memcpy() {
    char dest1[10];
    char dest2[10];
    char *src = "abc";
    strcpy(dest1, src);
    memcpy(dest2, src, strlen(src) + 1);
    if (strcmp(dest1, dest2)) {
        t_error("strcpy vs memcpy comparison failed\n");
    }
}

void test_strcpy_batch_group_1() {
    char dest[100];
    strcpy(dest, "batch1");
    if (strcmp(dest, "batch1")) {
        t_error("Batch1 first copy failed\n");
    }
    strcpy(dest, "batch2");
    if (strcmp(dest, "batch2")) {
        t_error("Batch1 second copy failed\n");
    }
    strcpy(dest, "batch3");
    if (strcmp(dest, "batch3")) {
        t_error("Batch1 third copy failed\n");
    }
}

void test_strcpy_batch_group_2() {
    char dest[100];
    strcpy(dest, "hello world");
    if (strcmp(dest, "hello world")) {
        t_error("Batch2 first copy failed\n");
    }
    strcpy(dest, "C programming");
    if (strcmp(dest, "C programming")) {
        t_error("Batch2 second copy failed\n");
    }
    strcpy(dest, "libc test");
    if (strcmp(dest, "libc test")) {
        t_error("Batch2 third copy failed\n");
    }
}

void test_strcpy_control_chars() {
    char dest[20];
    strcpy(dest, "\x01\x02\x03\x04");
    if (memcmp(dest, "\x01\x02\x03\x04", 4)) {
        t_error("Control chars copy failed\n");
    }
}

void test_strcpy_long_strings() {
    char long_str[256];
    memset(long_str, 'a', 255);
    long_str[255] = '\0';
    char dest[256];
    strcpy(dest, long_str);
    if (strcmp(dest, long_str)) {
        t_error("Long string copy failed\n");
    }
}

void test_strcpy_non_ascii() {
    char dest[20];
    strcpy(dest, "éñüß");
    if (strcmp(dest, "éñüß")) {
        t_error("Non-ASCII copy failed\n");
    }
}

void test_strcpy_hex_escape() {
    char dest[20];
    strcpy(dest, "\x41\x42\x43");
    if (strcmp(dest, "ABC")) {
        t_error("Hex escape copy failed\n");
    }
}

void test_strcpy_octal_escape() {
    char dest[20];
    strcpy(dest, "\101\102\103");
    if (strcmp(dest, "ABC")) {
        t_error("Octal escape copy failed\n");
    }
}

void test_strcpy_partial_overwrite() {
    char dest[20] = "abcdefghij";
    strcpy(dest + 3, "123");
    if (!strcmp(dest, "abc123ghij")) {
        t_error("Partial overwrite check failed\n");
    }
}

void test_strcpy_pointer_arithmetic() {
    char src[] = "abcdef";
    char dest[10];
    strcpy(dest, src + 2);
    if (strcmp(dest, "cdef")) {
        t_error("Pointer arithmetic copy failed\n");
    }
}

void test_strcpy_large_buffer() {
    char dest[1024];
    strcpy(dest, "This is a large buffer test");
    if (strcmp(dest, "This is a large buffer test")) {
        t_error("Large buffer copy failed\n");
    }
}

void test_strcpy_null_termination() {
    char dest[10];
    strcpy(dest, "12345");
    if (dest[5] != '\0') {
        t_error("Null termination failed\n");
    }
}

void test_strcpy_special_whitespace() {
    char dest[20];
    strcpy(dest, "\t\n\v\f\r");
    if (strcmp(dest, "\t\n\v\f\r")) {
        t_error("Special whitespace copy failed\n");
    }
}

void test_strcpy_alternating_chars() {
    char dest[20];
    strcpy(dest, "a\0b\0c");
    if (strcmp(dest, "a")) {
        t_error("Alternating chars copy failed\n");
    }
}

void test_strcpy_all_null() {
    char dest[10];
    strcpy(dest, "\0\0\0");
    if (strcmp(dest, "")) {
        t_error("All null copy failed\n");
    }
}

void test_strcpy_very_long() {
    char very_long[2048];
    memset(very_long, 'x', 2047);
    very_long[2047] = '\0';
    char dest[2048];
    strcpy(dest, very_long);
    if (strcmp(dest, very_long)) {
        t_error("Very long copy failed\n");
    }
}

void test_strcpy_non_terminated_src() {
    char src[5] = {'a', 'b', 'c', 'd', 'e'};
    char dest[10];
    strcpy(dest, src);
    if (strncmp(dest, "abcde", 5)) {
        t_error("Non-terminated source copy failed\n");
    }
}

void test_strcpy_different_sizes() {
    char dest1[5], dest2[10], dest3[20];
    strcpy(dest1, "test");
    if (strcmp(dest1, "test")) {
        t_error("Small size copy failed\n");
    }
    strcpy(dest2, "longer test");
    if (strcmp(dest2, "longer test")) {
        t_error("Medium size copy failed\n");
    }
    strcpy(dest3, "the longest test here");
    if (strcmp(dest3, "the longest test here")) {
        t_error("Large size copy failed\n");
    }
}

int main() {
    test_strcpy_basic();
    test_strcpy_empty();
    test_strcpy_longer_string();
    test_strcpy_special_characters();
    test_strcpy_single_char();
    test_strcpy_case_mixed();
    test_strcpy_with_spaces();
    test_strcpy_with_escapes();
    test_strcpy_overwrite_old_content();
    test_strcpy_with_embedded_null();
    test_strcpy_modify_src_after_copy();
    test_strcpy_utf8();
    test_strcpy_multiple_assignments();
    test_strcpy_to_offset();
    test_strcpy_array_vs_pointer();
    test_strcpy_all_caps();
    test_strcpy_sentence();
    test_strcpy_series();
    test_strcpy_manual_src_array();
    test_strcpy_vs_memcpy();
    test_strcpy_batch_group_1();
    test_strcpy_batch_group_2();
    test_strcpy_control_chars();
    test_strcpy_long_strings();
    test_strcpy_non_ascii();
    test_strcpy_hex_escape();
    test_strcpy_octal_escape();
    test_strcpy_partial_overwrite();
    test_strcpy_pointer_arithmetic();
    test_strcpy_large_buffer();
    test_strcpy_null_termination();
    test_strcpy_special_whitespace();
    test_strcpy_alternating_chars();
    test_strcpy_all_null();
    test_strcpy_very_long();
    test_strcpy_non_terminated_src();
    test_strcpy_different_sizes();

    return 0;
}