/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

const size_t ERROR_LEN = -1;
const int TEST_NUM = 6;
const int FORTITY_LEVEL = 2;

/**
 * @tc.name      : __printf_chk_0100
 * @tc.desc      : Verify __printf_chk and format is NULL
 * @tc.level     : Level 0
 */
void __printf_chk_0100(void)
{
    int result = __printf_chk(FORTITY_LEVEL, NULL, TEST_NUM);
    EXPECT_EQ("__printf_chk_0100", result, ERROR_LEN);
}

/**
 * @tc.name      : __printf_chk_0200
 * @tc.desc      : Verify the output data in the specified format (the format character is o)
 * @tc.level     : Level 0
 */
void __printf_chk_0200(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%o\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0200", result > 0);
}

/**
 * @tc.name      : __printf_chk_0300
 * @tc.desc      : Verify the output data in the specified format (the format character is x,X)
 * @tc.level     : Level 0
 */
void __printf_chk_0300(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%x\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0300", result > 0);

    result = __printf_chk(FORTITY_LEVEL, "%X\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0300", result > 0);
}

/**
 * @tc.name      : __printf_chk_0400
 * @tc.desc      : Verify the output data in the specified format (the format character is u)
 * @tc.level     : Level 0
 */
void __printf_chk_0400(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%u\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0400", result > 0);
}

/**
 * @tc.name      : __printf_chk_0500
 * @tc.desc      : Verify the output data in the specified format (the format character is f)
 * @tc.level     : Level 0
 */
void __printf_chk_0500(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%f\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0500", result > 0);
}

/**
 * @tc.name      : __printf_chk_0600
 * @tc.desc      : Verify the output data in the specified format (the format character is e,E)
 * @tc.level     : Level 0
 */
void __printf_chk_0600(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%e\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0600", result > 0);

    result = __printf_chk(FORTITY_LEVEL, "%E\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0600", result > 0);
}

/**
 * @tc.name      : __printf_chk_0700
 * @tc.desc      : Verify the output data in the specified format (the format character is g,G)
 * @tc.level     : Level 0
 */
void __printf_chk_0700(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%g\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0700", result > 0);

    result = __printf_chk(FORTITY_LEVEL, "%G\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_0700", result > 0);
}

/**
 * @tc.name      : __printf_chk_0800
 * @tc.desc      : Verify the output data in the specified format (the format character is c)
 * @tc.level     : Level 0
 */
void __printf_chk_0800(void)
{
    char ch = 'a';
    int result = __printf_chk(FORTITY_LEVEL, "%c\n", ch);
    EXPECT_TRUE("__printf_chk_0800", result > 0);
}

/**
 * @tc.name      : __printf_chk_0900
 * @tc.desc      : Verify the output data in the specified format (the format character is s)
 * @tc.level     : Level 0
 */
void __printf_chk_0900(void)
{
    char str[] = "test";
    int result = __printf_chk(FORTITY_LEVEL, "%s\n", str);
    EXPECT_TRUE("__printf_chk_0900", result > 0);
}

/**
 * @tc.name      : __printf_chk_1000
 * @tc.desc      : Verify the output data in the specified format (the format character is d)
 * @tc.level     : Level 0
 */
void __printf_chk_1000(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%d\n", TEST_NUM);
    EXPECT_TRUE("__printf_chk_1000", result > 0);
}

/**
 * @tc.name      : __printf_chk_1100
 * @tc.desc      : Verify invalid %N$
 * @tc.level     : Level 0
 */
void __printf_chk_1100(void)
{
    const char* data = "Hello, this is a test.\n";
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)__printf_chk(FORTITY_LEVEL, "%3$d\n", 0);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__printf_chk_1100", WIFSIGNALED(status));
        EXPECT_EQ("__printf_chk_1100", WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __printf_chk_1200
 * @tc.desc      : Verify specified parameters
 * @tc.level     : Level 0
 */
void __printf_chk_1200(void)
{
    int result = __printf_chk(FORTITY_LEVEL, "%2$s, %3$s, %1$s\n", "test", "this", "is");
    EXPECT_TRUE("__printf_chk_1200", result > 0);
    int expected_len = strlen("this, is, test\n");
    EXPECT_EQ("__printf_chk_1200", result, expected_len);
}

/**
 * @tc.name      : __printf_chk_1300
 * @tc.desc      : Test core functions
 * @tc.level     : Level 0
 */
void __printf_chk_1300(void)
{
    int ret = 0;

    ret = __printf_chk(1, "%d, %x, %ld, %u\n", 123, 0xFF, LONG_MAX, UINT_MAX);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "%d, %x, %ld, %u\n", 123, 0xFF, LONG_MAX, UINT_MAX);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(1, "%.f, %.2f, %e\n", 3.14159, 3.14159, 123.456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "%.f, %.2f, %e\n", 3.14159, 3.14159, 123.456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(1, "\"%s\", \"%s\", (\\n\\t)\n", "Hello __printf_chk", "");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "\"%s\", \"%s\", (\\n\\t)\n", "Hello __printf_chk", "");
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

/**
 * @tc.name      : __printf_chk_1400
 * @tc.desc      : Boundary and Compatibility Testing
 * @tc.level     : Level 0
 */
void __printf_chk_1400(void)
{
    int ret = 0;

    ret = __printf_chk(1, "%d, %d\n", INT_MAX, INT_MIN);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "%d, %d\n", INT_MAX, INT_MIN);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(1, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);

    ret = __printf_chk(0, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);

    ret = __printf_chk(1, "Hello World\\n\\t\n");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "Hello World\\n\\t\n");
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

/**
 * @tc.name      : __printf_chk_1500
 * @tc.desc      : Test error scenario
 * @tc.level     : Level 0
 */
void __printf_chk_1500(void)
{
    int ret = 0;

    ret = __printf_chk(1, "%d\n", 456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "%d\n", 456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    char dangerous_format[32] = "%d";
    ret = __printf_chk(1, dangerous_format, 789);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, dangerous_format, 789);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(1, "%d\n", 1000);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __printf_chk(0, "%d\n", 1000);
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

void* mt_worker(void* arg)
{
    __printf_chk_0200();
    return NULL;
}

/**
 * @tc.name      : __printf_chk_1600
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __printf_chk_1600(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(__FUNCTION__, pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    __printf_chk_0100();
    __printf_chk_0200();
    __printf_chk_0300();
    __printf_chk_0400();
    __printf_chk_0500();
    __printf_chk_0600();
    __printf_chk_0700();
    __printf_chk_0800();
    __printf_chk_0900();
    __printf_chk_1000();
    __printf_chk_1100();
    __printf_chk_1200();
    __printf_chk_1300();
    __printf_chk_1400();
    __printf_chk_1500();
    __printf_chk_1600();

    return t_status;
}
