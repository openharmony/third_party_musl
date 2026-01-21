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

static int VfprintfHelper(FILE* fp, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __vfprintf_chk(fp, FORTITY_LEVEL, format, args);
    va_end(args);
    return result;
}

int __vfprintf_chk_wrapper(FILE *stream, int flag, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = __vfprintf_chk(stream, flag, format, ap);
    va_end(ap);
    return ret;
}

/**
 * @tc.name:      __vfprintf_chk_0100
 * @tc.desc:      Verify __vfprintf_chk and fp is NULL will return -1.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0100(void)
{
    const char* data = "Hello, this is a test.\n";
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)VfprintfHelper(NULL, "%s", data);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__vfprintf_chk_0100", WIFSIGNALED(status));
        EXPECT_EQ("__vfprintf_chk_0100", WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name:      __vfprintf_chk_0200
 * @tc.desc:      Verify __vfprintf_chk and format is NULL will return -1.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0200(void)
{
    const char* data = "Hello, this is a test.\n";
    ssize_t result = VfprintfHelper(stdout, NULL, data);
    EXPECT_EQ("__vfprintf_chk_0200", result, ERROR_LEN);
    EXPECT_EQ("__fprintf_chk_0400", errno, EINVAL);
}

/**
 * @tc.name:      __vfprintf_chk_0300
 * @tc.desc:      Verify __vfprintf_chk and fopen("w") process success.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0300(void)
{
    char str[] = "这是一个测试.";
    int len = 0;
    FILE *fptr = fopen("tempory_testvfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("__vfprintf_chk_0300", len, strlen(str));
}

/**
 * @tc.name:      __fprintf_chk_0400
 * @tc.desc:      Verify __vfprintf_chk and fopen("r") and return -1.
 * @tc.level:     level 2.
 */
void __vfprintf_chk_0400(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testvfprintfchk.txt", "r");
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("__vfprintf_chk_0400", len, ERROR_LEN);
    EXPECT_EQ("__vfprintf_chk_0400", errno, EBADF);
    (void)remove("tempory_testvfprintfchk.txt");
}

/**
 * @tc.name:      __vfprintf_chk_0500
 * @tc.desc:      Verify __vfprintf_chk and stdout process success.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0500(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = VfprintfHelper(stdout, "%s", str);
    EXPECT_EQ("__vfprintf_chk_0500", len, strlen(str));
}

/**
 * @tc.name:      __vfprintf_chk_0600
 * @tc.desc:      Verify __vfprintf_chk and stderr process success.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0600(void)
{
    char str[] = "Hello, this is a test.\n";
    size_t len = VfprintfHelper(stderr, "%s", str);
    EXPECT_EQ("__vfprintf_chk_0600", len, strlen(str));
}

/**
 * @tc.name:      __vfprintf_chk_0700
 * @tc.desc:      Verify __vfprintf_chk with multiple format specifiers.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0700(void)
{
    char str[] = "Test";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "String: %s, Int: %d, Double: %.2f", str, TEST_NUM, TEST_DOUBLE);
        fclose(fptr);
    }
    int expected_len = strlen("String: Test, Int: 123, Double: 45.67");
    EXPECT_EQ("__vfprintf_chk_0700", len, expected_len);
    (void)remove("tempory_testfprintfchk.txt");
}


/**
 * @tc.name:      __vfprintf_chk_0800
 * @tc.desc:      Verify __vfprintf_chk with append mode.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0800(void)
{
    char str1[] = "First line\n";
    char str2[] = "Second line\n";

    // Write first line
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        VfprintfHelper(fptr, "%s", str1);
        fclose(fptr);
    }

    // Append second line
    fptr = fopen("tempory_testfprintfchk.txt", "a");
    size_t len = 0;
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "%s", str2);
        fclose(fptr);
    }

    EXPECT_EQ("__vfprintf_chk_0800", len, strlen(str2));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __vfprintf_chk_0900
 * @tc.desc:      Verify __vfprintf_chk with large string.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_0900(void)
{
    char large_str[STR_LEN];
    memset(large_str, 'A', sizeof(large_str) - 1);
    large_str[sizeof(large_str) - 1] = '\0';

    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "%s", large_str);
        fclose(fptr);
    }

    EXPECT_EQ("__vfprintf_chk_0900", len, strlen(large_str));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __vfprintf_chk_1000
 * @tc.desc:      Verify __vfprintf_chk with special characters.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_1000(void)
{
    char str[] = "Special chars: \t\n\r\\\"\'";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintfchk.txt", "w");
    if (fptr != NULL) {
        len = VfprintfHelper(fptr, "%s", str);
        fclose(fptr);
    }

    EXPECT_EQ("__vfprintf_chk_1000", len, strlen(str));
    (void)remove("tempory_testfprintfchk.txt");
}

/**
 * @tc.name:      __vfprintf_chk_1100
 * @tc.desc:      Basic functionality test.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_1100(void)
{
    int ret = 0;
    const char *test_file = "tempory_testvfprintfchk.txt";
    FILE *fp = NULL;

    ret = __vfprintf_chk_wrapper(stdout, 0, "%s, %d, 0x%X, %o\n", "Clang", 12345, 0xABCDEF, 0755);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 2, "%s, %d, 0x%X, %o\n", "Clang", 12345, 0xABCDEF, 0755);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    fp = fopen(test_file, "w");
    EXPECT_TRUE(__FUNCTION__, fp != NULL);

    ret = __vfprintf_chk_wrapper(fp, 2, "%.6f, %.2f, %c", 3.1415926, 3.1415926, 'H');
    EXPECT_TRUE(__FUNCTION__, ret > 0);
    (void)fclose(fp);

    fp = fopen(test_file, "r");
    EXPECT_TRUE(__FUNCTION__, fp != NULL);

    char buf[256] = {0};
    (void)fread(buf, 1, sizeof(buf) - 1, fp);
    EXPECT_STREQ(__FUNCTION__, "3.141593, 3.14, H", buf);
    (void)fclose(fp);

    (void)remove(test_file);

    ret = __vfprintf_chk_wrapper(stdout, 0, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);

    ret = __vfprintf_chk_wrapper(stdout, 2, "");
    EXPECT_TRUE(__FUNCTION__, ret == 0);
}

/**
 * @tc.name:      __vfprintf_chk_1200
 * @tc.desc:      Safety Feature Testing.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_1200(void)
{
    int ret = 0;

    ret = __vfprintf_chk_wrapper(stdout, 0, "%d, %s\n", 67890, "vfprintf_compatible");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, "%d, %s\n", 67890, "vfprintf_compatible");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 0, "%.3f, %x\n", 9.876, 0x123456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, "%.3f, %x\n", 9.876, 0x123456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

/**
 * @tc.name:      __vfprintf_chk_1300
 * @tc.desc:      Boundary scenario testing.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_1300(void)
{
    int ret;
    FILE *invalid_fp = NULL;

    ret = __vfprintf_chk_wrapper(stdout, 0, "\\n\\t\\r\\\"\\'\\\\\n");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, "\\n\\t\\r\\\"\\'\\\\\n");
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 0, "%z %q\n", 123);
    EXPECT_TRUE(__FUNCTION__, ret < 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, "%z %q\n", 123);
    EXPECT_TRUE(__FUNCTION__, ret < 0);

    char long_format[1024] = {0};
    for (int i = 0; i < 1024 - 1; i++) {
        strncat(long_format, "a", 1);
    }

    ret = __vfprintf_chk_wrapper(stdout, 0, long_format, 9999);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, long_format, 9999);
    EXPECT_TRUE(__FUNCTION__, ret > 0);
}

/**
 * @tc.name:      __vfprintf_chk_1400
 * @tc.desc:      Error Recovery Test.
 * @tc.level:     level 0.
 */
void __vfprintf_chk_1400(void)
{
    int ret = 0;
    FILE *fp = NULL;
    const char *test_file = "tempory_testvfprintfchk.txt";

    ret = __vfprintf_chk_wrapper(stdout, 0, "%s, %d\n", "recovery_success", 123456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    ret = __vfprintf_chk_wrapper(stdout, 1, "%s, %d\n", "recovery_success", 123456);
    EXPECT_TRUE(__FUNCTION__, ret > 0);

    fp = fopen(test_file, "w");
    if (fp != NULL) {
        ret = __vfprintf_chk_wrapper(fp, 0, "%.2f\n", 66.88);
        EXPECT_TRUE(__FUNCTION__, ret > 0);
        ret = __vfprintf_chk_wrapper(fp, 1, "%.2f\n", 66.88);
        EXPECT_TRUE(__FUNCTION__, ret > 0);
        (void)fclose(fp);
        (void)remove(test_file);
    }
}

void* mt_worker(void* arg)
{
    __vfprintf_chk_0500();
    return NULL;
}

/**
 * @tc.name      : __vfprintf_chk_1500
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __vfprintf_chk_1500(void)
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
    __vfprintf_chk_0100();
    __vfprintf_chk_0200();
    __vfprintf_chk_0300();
    __vfprintf_chk_0400();
    __vfprintf_chk_0500();
    __vfprintf_chk_0600();
    __vfprintf_chk_0700();
    __vfprintf_chk_0800();
    __vfprintf_chk_0900();
    __vfprintf_chk_1000();
    __vfprintf_chk_1100();
    __vfprintf_chk_1200();
    __vfprintf_chk_1300();
    __vfprintf_chk_1400();
    __vfprintf_chk_1500();

    return t_status;
}
