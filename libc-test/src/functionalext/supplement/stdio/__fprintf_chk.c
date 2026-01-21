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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

const size_t ERROR_LEN = -1;
const int STR_LEN = 1024;
const int TEST_NUM = 123;
const double TEST_DOUBLE = 45.67;
const int FORTITY_LEVEL = 2;

/**
 * @tc.name:      __fprintf_chk_0100
 * @tc.desc:      Verify __fprintf_chk and fp is NULL will return -1.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0100(void)
{
    const char* data = "Hello, this is a test.\n";
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)__fprintf_chk(NULL, FORTITY_LEVEL, "%s", data);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__fprintf_chk_0100", WIFSIGNALED(status));
        EXPECT_EQ("__fprintf_chk_0100", WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name:      __fprintf_chk_0200
 * @tc.desc:      Verify __fprintf_chk and format is NULL will return -1.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0200(void)
{
    const char* data = "Hello, this is a test.\n";
    ssize_t result = __fprintf_chk(stdout, FORTITY_LEVEL, NULL, data);
    EXPECT_EQ("__fprintf_chk_0200", result, ERROR_LEN);
    EXPECT_EQ("__fprintf_chk_0200", errno, EINVAL);
}

/**
 * @tc.name:      __fprintf_chk_0300
 * @tc.desc:      Verify __fprintf_chk and fopen("w") process success.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0300(void)
{
    char str[] = "这是一个测试.";
    int len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("__fprintf_chk_0300", len, strlen(str));
}

/**
 * @tc.name:      __fprintf_chk_0400
 * @tc.desc:      Verify __fprintf_chk and fopen("r") and return -1.
 * @tc.level:     level 2.
 */
void __fprintf_chk_0400(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "r");
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("__fprintf_chk_0400", len, ERROR_LEN);
    EXPECT_EQ("__fprintf_chk_0400", errno, EBADF);
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __fprintf_chk_0500
 * @tc.desc:      Verify __fprintf_chk and stdout process success.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0500(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = __fprintf_chk(stdout, FORTITY_LEVEL, "%s", str);
    EXPECT_EQ("__fprintf_chk_0500", len, strlen(str));
}

/**
 * @tc.name:      __fprintf_chk_0600
 * @tc.desc:      Verify __fprintf_chk and stderr process success.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0600(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = __fprintf_chk(stderr, FORTITY_LEVEL, "%s", str);
    EXPECT_EQ("__fprintf_chk_0600", len, strlen(str));
}

/**
 * @tc.name:      __fprintf_chk_0700
 * @tc.desc:      Verify __fprintf_chk with multiple format specifiers.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0700(void)
{
    char str[] = "Test";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "String: %s, Int: %d, Double: %.2f", str, TEST_NUM, TEST_DOUBLE);
        fclose(fptr);
    }
    int expected_len = strlen("String: Test, Int: 123, Double: 45.67");
    EXPECT_EQ("__fprintf_chk_0700", len, expected_len);
    (void)remove("tempory_testfprintfchk.txt");
}


/**
 * @tc.name:      __fprintf_chk_0800
 * @tc.desc:      Verify __fprintf_chk with append mode.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0800(void)
{
    char str1[] = "First line\n";
    char str2[] = "Second line\n";

    // Write first line
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        __fprintf_chk(fptr, FORTITY_LEVEL, "%s", str1);
        fclose(fptr);
    }

    // Append second line
    fptr = fopen("tempory_testfprintfchk.txt", "a");
    size_t len = 0;
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "%s", str2);
        fclose(fptr);
    }

    EXPECT_EQ("__fprintf_chk_0800", len, strlen(str2));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __fprintf_chk_0900
 * @tc.desc:      Verify __fprintf_chk with large string.
 * @tc.level:     level 0.
 */
void __fprintf_chk_0900(void)
{
    char large_str[STR_LEN];
    memset(large_str, 'A', sizeof(large_str) - 1);
    large_str[sizeof(large_str) - 1] = '\0';

    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "%s", large_str);
        fclose(fptr);
    }

    EXPECT_EQ("__fprintf_chk_0900", len, strlen(large_str));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __fprintf_chk_1000
 * @tc.desc:      Verify __fprintf_chk with special characters.
 * @tc.level:     level 0.
 */
void __fprintf_chk_1000(void)
{
    char str[] = "Special chars: \t\n\r\\\"\'";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = __fprintf_chk(fptr, FORTITY_LEVEL, "%s", str);
        fclose(fptr);
    }

    EXPECT_EQ("__fprintf_chk_1000", len, strlen(str));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __fprintf_chk_1100
 * @tc.desc:      Basic functionality test.
 * @tc.level:     level 0.
 */
void __fprintf_chk_1100(void)
{
    int ret = 0;

    ret = __fprintf_chk(stdout, 0, "%d, %.2f, %s\n", 123, 3.1415, "Hello __fprintf_chk");
    EXPECT_TRUE(__FUNCTION__, ret > 0);
    ret = __fprintf_chk(stdout, 1, "%d, %.2f, %s\n", 123, 3.1415, "Hello __fprintf_chk");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __fprintf_chk(stderr, 0, "%d\n", 999);
    EXPECT_TRUE(__FUNCTION__, ret > 0);
    ret = __fprintf_chk(stderr, 1, "%d\n", 999);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __fprintf_chk(stdout, 0, "\\n \\t \"%s\"\n", "");
    EXPECT_TRUE(__FUNCTION__, ret > 0);
    ret = __fprintf_chk(stdout, 1, "\\n \\t \"%s\"\n", "");
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

/**
 * @tc.name:      __fprintf_chk_1200
 * @tc.desc:      File stream output.
 * @tc.level:     level 0.
 */
void __fprintf_chk_1200(void)
{
    const char *filename = "tempory_testfprintfchk.txt";
    FILE *fp = NULL;
    int ret = 0;

    fp = fopen(filename, "w");
    EXPECT_TRUE(__FUNCTION__, fp != NULL);

    ret = __fprintf_chk(fp, 1, "__fprintf_chk test\n");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __fprintf_chk(fp, 1, "%d, %ld, %s\n", 1, 123456789L, "true");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    (void)fclose(fp);
    fp = NULL;

    fp = fopen(filename, "r");
    EXPECT_TRUE(__FUNCTION__, fp != NULL);

    char buf[256] = {0};
    char expected_content[256] = "__fprintf_chk test\n1, 123456789, true\n";
    (void)fread(buf, 1, sizeof(buf) - 1, fp);
    EXPECT_STREQ(__FUNCTION__, expected_content, buf);

    (void)fclose(fp);
    fp = NULL;
    (void)remove(filename);
}

/**
 * @tc.name:      __fprintf_chk_1300
 * @tc.desc:      Boundary scenario testing.
 * @tc.level:     level 0.
 */
void __fprintf_chk_1300(void)
{
    int ret = 0;

    ret = __fprintf_chk(stdout, 0, "%d\n", 123);
    EXPECT_TRUE(__FUNCTION__, ret > 0);
    ret = __fprintf_chk(stdout, 1, "%d\n", 123);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __fprintf_chk(stdout, 0, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);
    ret = __fprintf_chk(stdout, 1, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);
}

void* mt_worker(void* arg)
{
    __fprintf_chk_0500();
    return NULL;
}

/**
 * @tc.name      : __fprintf_chk_1400
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __fprintf_chk_1400(void)
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

int main(void)
{
    __fprintf_chk_0100();
    __fprintf_chk_0200();
    __fprintf_chk_0300();
    __fprintf_chk_0400();
    __fprintf_chk_0500();
    __fprintf_chk_0600();
    __fprintf_chk_0700();
    __fprintf_chk_0800();
    __fprintf_chk_0900();
    __fprintf_chk_1000();
    __fprintf_chk_1100();
    __fprintf_chk_1200();
    __fprintf_chk_1300();
    __fprintf_chk_1400();

    return t_status;
}
