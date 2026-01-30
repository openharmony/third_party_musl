/*
 * Copyright (c) 2025-2026 Huawei Device Co., Ltd.
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
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <functionalext.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 4
#define ITERATIONS_PER_THREAD 100

/**
 * @tc.name      : __rawmemchr_0100
 * @tc.desc      : Search for a character in the specified memory block
 * @tc.level     : Level 0
 */
void __rawmemchr_0100(void)
{
    const char *src = "abcd";
    void *result;

    // Basic test.
    char *ret = __rawmemchr(src, 'c');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + 2);
    // Beginning.
    ret = __rawmemchr(src, 'a');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src);

    // End of string.
    ret = __rawmemchr(src, 'd');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + 3);
    // Finding NUL.
    ret = __rawmemchr(src, '\0');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + strlen(src));
    const char *src1 = "bili";
    ret = __rawmemchr(src1, 'i');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src1 + 1);
    // NUL in empty string.
    const char *src2 = "";
    ret = __rawmemchr(src2, '\0');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src2);
    // Search for bytes in any binary memory (non string, no '\0' termination).
    const char src3[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    ret = __rawmemchr(src3, 0x04);
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_EQ("__rawmemchr_0100", ret - src3, 3);
    // Search for 0x00 byte in binary memory (existing in the middle of the memory block).
    uint8_t bin_data[] = {0xFF, 0x00, 0xFF, 0x00, 0xFF};
    result = __rawmemchr(bin_data, 0x00);
    EXPECT_PTRNE("__rawmemchr_0100", result, NULL);
    EXPECT_EQ("__rawmemchr_0100", (char *)result - (char *)bin_data, 1);
    // Convert negative numbers to unsigned char (passing in int negative numbers, actually parsed as unsigned char).
    int negative_char = -1; // Corresponding to unsigned char 0xFF
    result = __rawmemchr(bin_data, negative_char);
    EXPECT_PTRNE("__rawmemchr_0100", result, NULL);
    EXPECT_EQ("__rawmemchr_0100", (char *)result - (char *)bin_data, 0);
}

/**
 * @tc.name      : __rawmemchr_0200
 * @tc.desc      : Provide an exception parameter to search for characters in the specified memory block
 * @tc.level     : Level 0
 */
void __rawmemchr_0200(void)
{
    const char *src = "this is __rawmemchr_0200 test.";
    char *ret = __rawmemchr(src, 'x');
    // Test the undefined behavior of __rawmemchr when searching for non-existent character
    // Since __rawmemchr uses memchr(s, c, (size_t)-1), the behavior is undefined
    if (ret != NULL) {
        // Character found outside the original string boundaries
        // Verify that the pointer is beyond the valid string range
        EXPECT_LT("__rawmemchr_0200", src + strlen(src) + 1, ret);
        // Additional verification: ensure the found character is actually 'x'
        EXPECT_EQ("__rawmemchr_0200", *ret, 'x');
    } else {
        // Character not found within the search range (unlikely but possible)
        // This can happen if no 'x' is found in the (size_t)-1 byte range
        EXPECT_PTREQ("__rawmemchr_0200", ret, NULL);
    }
}

/**
 * @tc.name      : __rawmemchr_0300
 * @tc.desc      : Test __rawmemchr memory alignment
 * @tc.level     : Level 0
 */
void __rawmemchr_0300(void)
{
    const int CONSTANT_FF = 0xFF;
    char buf[BUFFER_SIZE];
    int i;
    char *p;
    for (i = 0; i < CONSTANT_FF; i++) {
        p = (char *)((uintptr_t)(buf + 0xff) & ~0xff) + i;
        strcpy(p, "ok");
        strcpy(p + 3, "bad/wrong");
        char *ret = __rawmemchr(p, 'r');
        EXPECT_PTRNE("__rawmemchr_0300", ret, NULL);
        EXPECT_STREQ("__rawmemchr_0300", ret, p + 8);
    }
}

typedef struct {
    char test_string[128];
    char search_char;
    int thread_id;
} thread_args_t;

void *thread_func(void *arg)
{
    thread_args_t *args = (thread_args_t *)arg;
    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        if (strchr(args->test_string, args->search_char) != NULL) {
            char *result = __rawmemchr(args->test_string, args->search_char);
            EXPECT_PTRNE("__rawmemchr_0400", result, NULL);
            if (result != NULL) {
                EXPECT_EQ("__rawmemchr_0400", *result, args->search_char);
                EXPECT_MT("__rawmemchr_0400", result, args->test_string - 1);
                EXPECT_LT("__rawmemchr_0400", result, args->test_string + strlen(args->test_string) + 64);
            }
        }
    }

    return NULL;
}

/**
 * @tc.name      : __rawmemchr_0400
 * @tc.desc      : Test __rawmemchr thread safety with concurrent calls
 * @tc.level     : Level 0
 */
void __rawmemchr_0400(void)
{
    pthread_t threads[NUM_THREADS];
    thread_args_t args[NUM_THREADS];

    const char *test_strings[] = {
        "test_string_1_with_m_char_here_m_and_more_text_abcdefghijklmnopqrstuvwxyz",
        "TEST_STRING_2_WITH_M_CHAR_HERE_M_AND_MORE_TEXT_ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "test_0123456789_with_5_char_here_5_and_more_numbers_0123456789",
        "test_!@#$%^&*()_with_&_char_here_&_and_more_symbols_!@#$%^&*()_+-=[]{};':\",./<>?"};

    char search_chars[] = {'m', 'M', '5', '&'};

    for (int i = 0; i < NUM_THREADS; i++) {
        strncpy(args[i].test_string, test_strings[i], sizeof(args[i].test_string) - 1);
        args[i].test_string[sizeof(args[i].test_string) - 1] = '\0';

        size_t len = strlen(args[i].test_string);
        if (len < sizeof(args[i].test_string) - 10) {
            memset(args[i].test_string + len, 'X', 10);
            args[i].test_string[len + 10] = '\0';
        }

        args[i].search_char = search_chars[i];
        args[i].thread_id = i;

        int ret = pthread_create(&threads[i], NULL, thread_func, &args[i]);
        EXPECT_EQ("__rawmemchr_0400", ret, 0);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(void)
{
    __rawmemchr_0100();
    __rawmemchr_0200();
    __rawmemchr_0300();
    __rawmemchr_0400();
    return t_status;
}
