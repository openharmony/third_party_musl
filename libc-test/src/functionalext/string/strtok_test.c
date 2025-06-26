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

void test_strtok_basic() {
    char str[] = "apple,banana,cherry";
    const char *delim = ",";
    char *token = strtok(str, delim);
    if (strcmp(token, "apple") != 0) {
        t_error("First token should be 'apple', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "banana") != 0) {
        t_error("Second token should be 'banana', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "cherry") != 0) {
        t_error("Third token should be 'cherry', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL after last token, got '%s'\n", token);
    }
}

void test_strtok_consecutive_delims() {
    char str[] = "apple,,banana,,,cherry";
    const char *delim = ",";
    char *token = strtok(str, delim);
    if (strcmp(token, "apple") != 0) {
        t_error("First token should be 'apple', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "banana") != 0) {
        t_error("Second token should be 'banana', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "cherry") != 0) {
        t_error("Third token should be 'cherry', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL after last token, got '%s'\n", token);
    }
}

void test_strtok_empty_input() {
    char str[] = "";
    const char *delim = ",";
    char *token = strtok(str, delim);
    if (token != NULL) {
        t_error("Expected NULL for empty input, got '%s'\n", token);
    }
}

void test_strtok_no_delims() {
    char str[] = "applebananacherry";
    const char *delim = ",";
    char *token = strtok(str, delim);
    if (strcmp(token, "applebananacherry") != 0) {
        t_error("Full string should be returned as single token, got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL after single token, got '%s'\n", token);
    }
}

void test_strtok_delim_start_end() {
    char str[] = ",apple,banana,cherry,";
    const char *delim = ",";
    char *token = strtok(str, delim);
    if (strcmp(token, "apple") != 0) {
        t_error("First token should skip leading delim, got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "banana") != 0) {
        t_error("Second token should be 'banana', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "cherry") != 0) {
        t_error("Third token should be 'cherry', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL after last token, got '%s'\n", token);
    }
}

void test_strtok_multi_char_delim() {
    char str[] = "apple##banana###cherry";
    const char *delim = "#";
    char *token = strtok(str, delim);
    if (strcmp(token, "apple") != 0) {
        t_error("First token should be 'apple', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "banana") != 0) {
        t_error("Second token should be 'banana', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (strcmp(token, "cherry") != 0) {
        t_error("Third token should be 'cherry', got '%s'\n", token);
    }
    token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL after last token, got '%s'\n", token);
    }
}

void test_strtok_r_thread_safe() {
    char str[] = "apple,banana,cherry";
    const char *delim = ",";
    char *saveptr;
    char *token = strtok_r(str, delim, &saveptr);
    if (strcmp(token, "apple") != 0) {
        t_error("First token should be 'apple', got '%s'\n", token);
    }
    token = strtok_r(NULL, delim, &saveptr);
    if (strcmp(token, "banana") != 0) {
        t_error("Second token should be 'banana', got '%s'\n", token);
    }
    token = strtok_r(NULL, delim, &saveptr);
    if (strcmp(token, "cherry") != 0) {
        t_error("Third token should be 'cherry', got '%s'\n", token);
    }
    token = strtok_r(NULL, delim, &saveptr);
    if (token != NULL) {
        t_error("Expected NULL after last token, got '%s'\n", token);
    }
}

void test_strtok_and_strtok_r_mixed() {
    char str1[] = "apple,banana";
    char str2[] = "cherry,durian";
    const char *delim = ",";

    // 使用 strtok 分割第一个字符串
    char *token1 = strtok(str1, delim);
    if (strcmp(token1, "apple") != 0) {
        t_error("strtok token1 should be 'apple', got '%s'\n", token1);
    }

    // 使用 strtok_r 分割第二个字符串（不干扰 strtok 的状态）
    char *saveptr;
    char *token2 = strtok_r(str2, delim, &saveptr);
    if (strcmp(token2, "cherry") != 0) {
        t_error("strtok_r token2 should be 'cherry', got '%s'\n", token2);
    }

    // 继续使用 strtok 分割第一个字符串
    token1 = strtok(NULL, delim);
    if (strcmp(token1, "banana") != 0) {
        t_error("strtok token1 should be 'banana', got '%s'\n", token1);
    }

    // 继续使用 strtok_r 分割第二个字符串
    token2 = strtok_r(NULL, delim, &saveptr);
    if (strcmp(token2, "durian") != 0) {
        t_error("strtok_r token2 should be 'durian', got '%s'\n", token2);
    }
}

void test_strtok_null_input() {
    const char *delim = ",";
    char *token = strtok(NULL, delim);
    if (token != NULL) {
        t_error("Expected NULL for NULL input, got '%s'\n", token);
    }
}

int main() {
    test_strtok_basic();
    test_strtok_consecutive_delims();
    test_strtok_empty_input();
    test_strtok_no_delims();
    test_strtok_delim_start_end();
    test_strtok_multi_char_delim();
    test_strtok_r_thread_safe();
    test_strtok_and_strtok_r_mixed();
    test_strtok_null_input();

    return 0;
}