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

void test_strcat_basic() {
    char dest[20] = "hello";
    const char *src = " world";
    strcat(dest, src);
    if (strcmp(dest, "hello world") != 0) {
        t_error("strcat basic failed: expected 'hello world', got '%s'\n", dest);
    }
}

void test_strcat_empty_dest() {
    char dest[20] = "";
    const char *src = "apple";
    strcat(dest, src);
    if (strcmp(dest, "apple") != 0) {
        t_error("strcat empty dest failed: expected 'apple', got '%s'\n", dest);
    }
}

void test_strcat_empty_src() {
    char dest[20] = "hello";
    const char *src = "";
    strcat(dest, src);
    if (strcmp(dest, "hello") != 0) {
        t_error("strcat empty src failed: expected 'hello', got '%s'\n", dest);
    }
}

void test_strcat_exact_fit() {
    char dest[12] = "hello";
    const char *src = " world";
    strcat(dest, src);
    if (strcmp(dest, "hello world") != 0) {
        t_error("strcat exact fit failed: buffer size 12, got '%s'\n", dest);
    }
}

void test_strcat_multiple() {
    char dest[30] = "start";
    strcat(dest, " middle");
    strcat(dest, " end");
    if (strcmp(dest, "start middle end") != 0) {
        t_error("strcat multiple failed: expected 'start middle end', got '%s'\n", dest);
    }
}

void test_strcat_special_chars() {
    char dest[20] = "abc";
    const char *src = "@#$%";
    strcat(dest, src);
    if (strcmp(dest, "abc@#$%") != 0) {
        t_error("strcat special chars failed: expected 'abc@#$%', got '%s'\n", dest);
    }
}

void test_strcat_numbers() {
    char dest[20] = "num: ";
    const char *src = "123";
    strcat(dest, src);
    if (strcmp(dest, "num: 123") != 0) {
        t_error("strcat numbers failed: expected 'num: 123', got '%s'\n", dest);
    }
}

void test_strcat_null_termination() {
    char dest[10] = "a";
    strcat(dest, "b");
    strcat(dest, "c");
    if (dest[0] != 'a' || dest[1] != 'b' || dest[2] != 'c' || dest[3] != '\0') {
        t_error("strcat null-termination failed");
    }
}

void test_strcat_utf8() {
    char dest[20] = "中文";
    const char *src = "→日本語";
    strcat(dest, src);
    if (strcmp(dest, "中文→日本語") != 0) {
        t_error("strcat UTF-8 failed: expected '中文→日本語', got '%s'\n", dest);
    }
}

void test_strcat_src_contains_null() {
    char dest[20] = "start";
    const char src[] = {'a', '\0', 'b'};
    strcat(dest, src);
    if (strcmp(dest, "starta") != 0) {
        t_error("strcat src contains null failed: expected 'starta', got '%s'\n", dest);
    }
}

typedef struct {
    char name[32];
} User;

void test_strcat_struct_field() {
    User user;
    strcpy(user.name, "Alice");
    strcat(user.name, " Smith");
    if (strcmp(user.name, "Alice Smith") != 0) {
        t_error("Struct field failed. Got: '%s'\n", user.name);
    }
}

int main() {
    test_strcat_basic();
    test_strcat_empty_dest();
    test_strcat_empty_src();
    test_strcat_exact_fit();
    test_strcat_multiple();
    test_strcat_special_chars();
    test_strcat_numbers();
    test_strcat_null_termination();
    test_strcat_utf8();
    test_strcat_src_contains_null();
    test_strcat_struct_field();

    return 0;
}