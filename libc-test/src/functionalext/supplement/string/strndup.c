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
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functionalext.h"

/**
 * @tc.name      : strndup_0100
 * @tc.desc      : Test the strndup function to copy the string according to the input parameter
 * @tc.level     : Level 0
 */
void strndup_0100(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 2);
    if (strcmp(result, "he")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
    free(result);
}

/**
 * @tc.name      : strndup_0200
 * @tc.desc      : Test the return value of strndup when the number of copies of the input exceeds the passed string
 * @tc.level     : Level 1
 */
void strndup_0200(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 12);
    if (strcmp(result, "helloworld")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
    free(result);
}

/**
 * @tc.name      : strndup_0300
 * @tc.desc      : The return value of strndup when the number of test copies is 0
 * @tc.level     : Level 1
 */
void strndup_0300(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 0);
    if (strcmp(result, "") != 0) {
        t_error("%s strndup get result is '%s' are not ''\n", __func__, result);
    }
    free(result);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : __strndup_0100
 * @tc.desc      : The return value of __strndup when the number of test copies is different with the size of string
 * @tc.level     : Level 1
 */
void __strndup_0100(void)
{
    const char *src = "Hello, World!";
    // n is larger than the length of src
    char *result = __strndup(src, 20);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "Hello, World!");
    EXPECT_LT("__strndup_0100", strlen(result), 20);
    free(result);
    // n is less than the length of src
    result = __strndup(src, 5);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "Hello");
    EXPECT_LT("__strndup_0100", strlen(result), strlen(src));
    free(result);
    // n is equal to the length of src
    result = __strndup(src, strlen(src));
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, src);
    EXPECT_EQ("__strndup_0100", strlen(result), strlen(src));
    free(result);
    // n is 0
    const char *src1 = "Hello";
    result = __strndup(src, 0);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "");
    EXPECT_EQ("__strndup_0100", strlen(result), 0);
    free(result);
}

/**
 * @tc.name      : __strndup_0200
 * @tc.desc      : The return value of __strndup in different scenario as the following:
 *                 1. large n value (boundary test)
 *                 2. string containing special characters
 *                 3. empty string
 *                 4. Ultra long source string
 * @tc.level     : Level 1
 */
void __strndup_0200(void)
{
    // large n value (boundary test)
    const char *str = "Short";
    char *result = __strndup(str, SIZE_MAX);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, "Short");
    EXPECT_LT("__strndup_0200", strlen(result), SIZE_MAX);
    free(result);
    // string containing special characters
    const char *src = "Line1\nLine2\tTab";
    result = __strndup(src, 12);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, "Line1\nLine2\t");
    EXPECT_EQ("__strndup_0200", result[12], '\0');
    free(result);
    // empty string
    const char *src1 = "";
    result = __strndup(src1, 5);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, "");
    EXPECT_EQ("__strndup_0200", strlen(result), 0);
    free(result);
    // Ultra long source string
    size_t ultra_len = 10000;
    char *ultra_str = (char *)malloc(ultra_len + 1);
    EXPECT_PTRNE("__strndup_0200", ultra_str, NULL);
    for (size_t i = 0; i < ultra_len; i++) {
        ultra_str[i] = 'A';
    }
    ultra_str[ultra_len] = '\0';
    result = __strndup(ultra_str, ultra_len);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, ultra_str);
    free(ultra_str);
    free(result);
}

/**
 * @tc.name      : __strndup_0300
 * @tc.desc      : Test __strndup when the source pointer is NULL
 * @tc.level     : Level 1
 */
void __strndup_0300(void)
{
    const char *data  = NULL;
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        char *result = __strndup(data, 10);
        if (result != NULL) {
            t_error("__strndup with NULL pointer should return NULL or cause SIGSEGV\n");
            free(result);
        }
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__strndup_0300", WIFSIGNALED(status));
        EXPECT_EQ("__strndup_0300", WTERMSIG(status), SIGSEGV);
    }
}

/**
 * @tc.name      : __strndup_0400
 * @tc.desc      : Buffer overflow security test
 * @tc.level     : Level 1
 */
void __strndup_0400(void)
{
    char *dst = NULL;
    char unsafe_src[10] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
    size_t copy_len = 8;
    dst = __strndup((const char *)unsafe_src, copy_len);
    EXPECT_PTRNE("__strndup_0400", dst, NULL);
    EXPECT_EQ("__strndup_0400", strncmp(dst, unsafe_src, copy_len), 0);
    EXPECT_EQ("__strndup_0400", dst[copy_len], '\0');
    free(dst);
}

/**
 * @tc.name      : __strndup_0500
 * @tc.desc      : Memory management test: Multiple allocations and releases to verify no memory leaks
 * @tc.level     : Level 1
 */
void __strndup_0500(void)
{
    char *dst1 = NULL;
    char *dst2 = NULL;
    char *dst3 = NULL;
    const char *src1 = "Memory management test 1";
    const char *src2 = "Memory management test 2";
    const char *src3 = "Memory management test 3";

    dst1 = __strndup(src1, strlen(src1));
    EXPECT_PTRNE("__strndup_0500", dst1, NULL);
    dst2 = __strndup(src2, strlen(src2) + 50); // n larger than source string length
    EXPECT_PTRNE("__strndup_0500", dst2, NULL);
    dst3 = __strndup(src3, 0); // n is 0
    EXPECT_PTRNE("__strndup_0500", dst3, NULL);
    free(dst1);
    free(dst2);
    free(dst3);
}

#define NUM_THREADS 5
#define ITERATIONS_PER_THREAD 1000

struct thread_data {
    int thread_id;
    int success_count;
};

void *thread_func(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        char *test_str = "HelloWorld";
        size_t n = (i % 5) + 1; // Vary n between 1-5

        char *result = strndup(test_str, n);
        if (result == NULL) {
            continue;
        }

        // Verify the result
        if (strncmp(result, test_str, n) == 0 && result[n] == '\0') {
            data->success_count++;
        }

        free(result);
    }
    return NULL;
}

/**
 * @tc.name      : __strndup_0600
 * @tc.desc      : Multi-thread concurrent call stability test for __strndup function
 * @tc.level     : Level 1
 */
void __strndup_0600(void)
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
        EXPECT_EQ("__strndup_0600", ret, 0);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        EXPECT_EQ("__strndup_0600", pthread_join(threads[i], NULL), 0);
    }

    // Verify all threads completed successfully
    int total_success = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_success += thread_data[i].success_count;
    }

    int expected_success = NUM_THREADS * ITERATIONS_PER_THREAD;
    EXPECT_EQ("__strndup_0600", total_success, expected_success);
}
#endif

int main(int argc, char *argv[])
{
    strndup_0100();
    strndup_0200();
    strndup_0300();
#ifdef MUSL_EXTERNAL_FUNCTION
    __strndup_0100();
    __strndup_0200();
    __strndup_0300();
    __strndup_0400();
    __strndup_0500();
    __strndup_0600();
#endif
    return t_status;
}