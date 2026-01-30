/*
 * Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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
#include <stdio.h>
#include <string.h>

#include "functionalext.h"

/**
 * @tc.name      : strtok_r_0100
 * @tc.desc      : Test strtok_r function to intercept string according to input characters
 * @tc.level     : Level 0
 */
void strtok_r_0100(void)
{
    char *compare[3] = {"ABC", "DEF", "GHI"};
    char buf[] = "ABC,DEF,GHI";
    char *result[3];
    char *p = buf;
    char *outer_ptr = NULL;
    for (int i = 0; i < 3; i++) {
        result[i] = strtok_r(outer_ptr, ",", &p);
    }
    for (int a = 0; a < 3; a++) {
        if (strcmp(result[a], compare[a]) != 0) {
            t_error("%s strtok_r in %d get value is %s are not %s\n", __func__, a, result[a], compare[a]);
        }
    }
}

/**
 * @tc.name      : strtok_r_0200
 * @tc.desc      : Test the return value of the strtok_r function when the intercepted string is an empty string
 * @tc.level     : Level 1
 */
void strtok_r_0200(void)
{
    char *p;
    char *outer_ptr = NULL;
    char *result = strtok_r(outer_ptr, ",", &p);
    if (result) {
        t_error("%s strtok_r error get result is %s not null\n", __func__, result);
    }
}

#ifdef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : __strtok_r_0100
 * @tc.desc      : Test strtok_r function to intercept string according to input characters
 * @tc.level     : Level 1
 */

void __strtok_r_0100(void)
{
    char str1[] = "apple,banana,orange,grape";
    char *token;
    char *saveptr;
    int count = 0;
    const char *expected[] = {"apple", "banana", "orange", "grape"};
    token = __strtok_r(str1, ",", &saveptr);
    while (token != NULL) {
        if (strcmp(token, expected[count]) != 0) {
            t_error("%s __strtok_r error in %d get value is %s are not %s\n", __func__, count, token, expected[count]);
        }
        count++;
        token = __strtok_r(NULL, ",", &saveptr);
    }
    if (count != 4) {
        t_error("%s __strtok_r error total token count is %d not 4\n", __func__, count);
    }
}

/**
 * @tc.name      : strtok_r_0200
 * @tc.desc      : Test the return value of the __strtok_r function when the intercepted string is an empty string
 * @tc.level     : Level 1
 */
void __strtok_r_0200(void)
{
    char str[] = "";
    char *token;
    char *saveptr;
    token = __strtok_r(str, ",", &saveptr);
    if (token) {
        t_error("%s __strtok_r error get result is %s not null\n", __func__, token);
    }
}

/**
 * @tc.name      : strtok_r_0300
 * @tc.desc      : Test the return value of the __strtok_r function without a delimiter
 * @tc.level     : Level 1
 */
void __strtok_r_0300(void)
{
    char str[] = "singletoken";
    char *token;
    char *saveptr;
    token = __strtok_r(str, ",", &saveptr);
    if (token == NULL) {
        t_error("%s __strtok_r error get result is null\n", __func__);
        return;
    }
    if (strcmp(token, "singletoken") != 0) {
        t_error("%s __strtok_r error get value should be is \"singletoken\"\n", __func__);
        return;
    }
    token = __strtok_r(NULL, ",", &saveptr);
    if (token != NULL) {
        t_error("%s __strtok_r error get result should be null\n", __func__);
    }
}

/**
 * @tc.name      : strtok_r_0400
 * @tc.desc      : Test the return value of the __strtok_r function with Continuous delimiters
 * @tc.level     : Level 1
 */
void __strtok_r_0400(void)
{
    char str[] = "apple,,banana,,,orange";
    char *token;
    char *saveptr;
    const char *expected[] = {"apple", "banana", "orange"};
    int count = 0;

    token = __strtok_r(str, ",", &saveptr);
    while (token != NULL) {
        if (strcmp(token, expected[count]) != 0) {
            t_error("%s __strtok_r error: The %d token is not match. current is %s, expected is %s \n",
                    __func__, count, token, expected[count]);
            return;
        }
        count++;
        token = __strtok_r(NULL, ",", &saveptr);
    }

    if (count != 3) {
        t_error("%s __strtok_r error: only find %d tokens, expected 3\n", __func__, count);
        return;
    }
}

/**
 * @tc.name      : strtok_r_0500
 * @tc.desc      : Test the return value of the __strtok_r function when the string starts and ends with delimiters
 * @tc.level     : Level 1
 */
void __strtok_r_0500(void)
{
    char str[] = ",apple,banana,orange,";
    char *token;
    char *saveptr;
    const char *expected[] = {"apple", "banana", "orange"};
    int count = 0;

    token = __strtok_r(str, ",", &saveptr);
    while (token != NULL) {

        if (strcmp(token, expected[count]) != 0) {
            t_error("%s __strtok_r error: The %d token is not match. current is %s, expected is %s \n",
                    __func__, count + 1, token, expected[count + 1]);
            return;
        }
        count++;
        token = __strtok_r(NULL, ",", &saveptr);
    }

    if (count != 3) {
        t_error("%s __strtok_r error: only find %d tokens, expected 3\n", __func__, count);
        return;
    }
}

/**
 * @tc.name      : strtok_r_0600
 * @tc.desc      : Test the return value of the __strtok_r function when the string with only separators
 * @tc.level     : Level 1
 */
void __strtok_r_0600(void)
{
    char str[] = ",,,";
    char *token;
    char *saveptr;

    token = __strtok_r(str, ",", &saveptr);
    if (token != NULL) {
        t_error("%s __strtok_r error: Only the delimiter should return NULL\n", __func__);
        return;
    }
}

/**
 * @tc.name      : strtok_r_0700
 * @tc.desc      : Test the return value of the __strtok_r function with multiple delimiters
 * @tc.level     : Level 1
 */
void __strtok_r_0700(void)
{
    char str[] = "apple;banana,orange:grape|melon";
    char *token;
    char *saveptr;
    const char *expected[] = {"apple", "banana", "orange", "grape", "melon"};
    int count = 0;

    token = __strtok_r(str, ";,:|\n", &saveptr);
    while (token != NULL) {
        EXPECT_EQ("__strtok_r_0700", strcmp(token, expected[count]), 0);
        count++;
        token = __strtok_r(NULL, ";,:|\n", &saveptr);
    }

    EXPECT_EQ("__strtok_r_0700", count, 5);
}

/**
 * @tc.name      : strtok_r_0800
 * @tc.desc      : Test the return value of the __strtok_r function with delimiters containing spaces
 * @tc.level     : Level 1
 */
void __strtok_r_0800(void)
{
    char str[] = "apple\tbanana\norange grape";
    char *token;
    char *saveptr;
    const char *expected[] = {"apple", "banana", "orange", "grape"};
    int count = 0;

    token = __strtok_r(str, " \t\n\r", &saveptr);
    while (token != NULL) {
        EXPECT_EQ("__strtok_r_0800", strcmp(token, expected[count]), 0);
        count++;
        token = __strtok_r(NULL, " \t\n\r", &saveptr);
    }
    EXPECT_EQ("__strtok_r_0800", count, 4);
}

/**
 * @tc.name      : strtok_r_0900
 * @tc.desc      : Concurrent split test: Simultaneously split two independent strings to verify the ability of saveptr
 *                 to isolate the state
 * @tc.level     : Level 1
 */
void __strtok_r_0900(void)
{
    char str1_buf[] = "100;200;300;400;500";
    const char *expected1[] = {"100", "200", "300", "400", "500"};
    char *saveptr1 = NULL;
    const char *delim1 = ";";

    char str2_buf[] = "a-b-c-d-e-f-g";
    const char *expected2[] = {"a", "b", "c", "d", "e", "f", "g"};
    char *saveptr2 = NULL;
    const char *delim2 = "-";

    char *token1 = __strtok_r(str1_buf, delim1, &saveptr1);
    char *token2 = __strtok_r(str2_buf, delim2, &saveptr2);
    EXPECT_PTRNE("__strtok_r_0900", token1, NULL);
    EXPECT_PTRNE("__strtok_r_0900", token2, NULL);
    EXPECT_TRUE("__strtok_r_0900", strcmp(token1, expected1[0]) == 0);
    EXPECT_TRUE("__strtok_r_0900", strcmp(token2, expected2[0]) == 0);

    int round = 1;
    while (token1 != NULL || token2 != NULL) {
        if (token1 != NULL) {
            token1 = __strtok_r(NULL, delim1, &saveptr1);
            if (token1 != NULL) {
                EXPECT_TRUE("__strtok_r_0900", round < 5);
                EXPECT_STREQ("__strtok_r_0900", token1, expected1[round]);
            } else {
                EXPECT_EQ("__strtok_r_0900", round, 5);
            }
        }
        if (token2 != NULL) {
            token2 = __strtok_r(NULL, delim2, &saveptr2);
            if (token2 != NULL) {
                EXPECT_TRUE("__strtok_r_0900", round < 7);
                EXPECT_STREQ("__strtok_r_0900", token2, expected2[round]);
            } else {
                EXPECT_EQ("__strtok_r_0900", round, 7);
            }
        }
        round++;
    }
}

#define NUM_THREADS 4
#define ITERATIONS_PER_THREAD 100

struct thread_data {
    int thread_id;
    int success_count;
};

static void *thread_func(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        // Each thread uses independent strings and saveptr to avoid race conditions
        char str_buf[64];
        char *saveptr = NULL;

        // Create different test data for each thread
        (void)snprintf(str_buf, sizeof(str_buf), "thread%d,token1,token2,token3,token4", data->thread_id);

        const char *expected[] = {
            "thread%d", "token1", "token2", "token3", "token4"};
        char expected_buf[5][16];

        // Prepare expected values
        for (int j = 0; j < 5; j++) {
            if (j == 0) {
                (void)snprintf(expected_buf[j], sizeof(expected_buf[j]), "thread%d", data->thread_id);
            } else {
                (void)snprintf(expected_buf[j], sizeof(expected_buf[j]), "token%d", j);
            }
        }

        char *token = __strtok_r(str_buf, ",", &saveptr);
        int count = 0;
        int success = 1;

        while (token != NULL) {
            if (count < 5) {
                EXPECT_STREQ("__strtok_r_1000", token, expected_buf[count]);
            }
            count++;
            token = __strtok_r(NULL, ",", &saveptr);
        }

        if (count == 5) {
            data->success_count++;
        }
    }

    return NULL;
}

/**
 * @tc.name      : __strtok_r_1000
 * @tc.desc      : Multi-thread concurrent call stability test for __strtok_r function
 * @tc.level     : Level 1
 */
void __strtok_r_1000(void)
{
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data[NUM_THREADS];
    int ret;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].success_count = 0;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        ret = pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
        EXPECT_EQ("__strtok_r_1000", ret, 0);
    }


    for (int i = 0; i < NUM_THREADS; i++) {
        EXPECT_EQ("__strtok_r_1000", pthread_join(threads[i], NULL), 0);
    }

    int total_success = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_success += thread_data[i].success_count;
    }

    int expected_success = NUM_THREADS * ITERATIONS_PER_THREAD;
    EXPECT_EQ("__strtok_r_1000", total_success, expected_success);
}
#endif

int main(int argc, char *argv[])
{
    strtok_r_0100();
    strtok_r_0200();
#ifdef MUSL_EXTERNAL_FUNCTION
    __strtok_r_0100();
    __strtok_r_0200();
    __strtok_r_0300();
    __strtok_r_0400();
    __strtok_r_0500();
    __strtok_r_0600();
    __strtok_r_0700();
    __strtok_r_0800();
    __strtok_r_0900();
    __strtok_r_1000();
#endif
    return t_status;
}