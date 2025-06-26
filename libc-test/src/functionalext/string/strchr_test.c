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

void test_strchr_basic() {
    const char *str = "hello world";
    char *result = strchr(str, 'o');
    if (result == NULL || *result != 'o') {
        t_error("Basic strchr failed: expected 'o', got %s\n", result ? "NULL" : result);
    }
}

void test_strchr_first_char() {
    const char *str = "apple";
    char *result = strchr(str, 'a');
    if (result != str) {
        t_error("First char failed: expected 'a', got %s\n", result ? result : "NULL");
    }
}


void test_strchr_not_found() {
    const char *str = "abc";
    char *result = strchr(str, 'x');
    if (result != NULL) {
        t_error("Not found failed: expected NULL, got '%s'\n", result);
    }
}

void test_strchr_null_term() {
    const char *str = "hello";
    char *result = strchr(str, '\0');
    if (result != str + 5) {
        t_error("Null term failed: expected end pointer, got %p\n", result);
    }
}

void test_strchr_empty_str() {
    const char *str = "";
    char *result = strchr(str, 'a');
    if (result != NULL) {
        t_error("Empty string failed: expected NULL, got %p\n", result);
    }
}

void test_strchr_repeated() {
    const char *str = "aabbb";
    char *result = strchr(str, 'b');
    if (result != str + 2) {
        t_error("Repeated char failed: expected 'bbb', got '%s'\n",
               result ? result : "NULL");
    }
}

void test_strchr_case_sensitive() {
    const char *str = "Hello";
    char *result = strchr(str, 'h');
    if (result != NULL) {
        t_error("Case sensitive failed: expected NULL, got '%s'\n", result);
    }
}

void test_strchr_digits() {
    const char *str = "ID: 123";
    char *result = strchr(str, '2');
    if (result == NULL || *result != '2') {
        t_error("Digit failed: expected '2', got %s\n", result ? result : "NULL");
    }
}

void test_strchr_special_char() {
    const char *str = "a b#c";
    char *result = strchr(str, '#');
    if (result == NULL || *result != '#') {
        t_error("Special char failed: expected '#', got %s\n", result ? result : "NULL");
    }
}

void test_strchr_chained() {
    const char *str = "a,b,c";
    char *first = strchr(str, ',');
    char *second = first ? strchr(first + 1, ',') : NULL;

    if (!first || !second || *first != ',' || *second != ',') {
        t_error("Chained calls failed: first=%p, second=%p\n", first, second);
    }
}

int main() {
    test_strchr_basic();
    test_strchr_first_char();
    test_strchr_not_found();
    test_strchr_null_term();
    test_strchr_empty_str();
    test_strchr_repeated();
    test_strchr_case_sensitive();
    test_strchr_digits();
    test_strchr_special_char();
    test_strchr_chained();
    return 0;
}