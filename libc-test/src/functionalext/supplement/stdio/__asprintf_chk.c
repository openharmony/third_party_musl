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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

const int ERROR_RET = -1;
const int UNUSED_PLACEHOLDER_PARAM = 123;

/**
 * @tc.name      : __asprintf_chk_0100
 * @tc.desc      : Verify __asprintf_chk with NULL format string
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0100(void)
{
    char *buf = NULL;
    int ret = __asprintf_chk(&buf, 0, NULL, UNUSED_PLACEHOLDER_PARAM);
    EXPECT_EQ("__asprintf_chk_0100", ret, ERROR_RET);
    EXPECT_EQ("__asprintf_chk_0100", (void*)buf, (void*)NULL);
}

/**
 * @tc.name      : __asprintf_chk_0200
 * @tc.desc      : Verify __asprintf_chk with integer format (d)
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0200(void)
{
    char *buf = NULL;
    int num = -456;
    int ret = __asprintf_chk(&buf, 0, "Num: %d", num);
    EXPECT_TRUE("__asprintf_chk_0200", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0200", buf, "Num: -456");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0300
 * @tc.desc      : Verify __asprintf_chk with string format (s)
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0300(void)
{
    char *buf = NULL;
    char *str = "hello";
    int ret = __asprintf_chk(&buf, 0, "Str: %s", str);
    EXPECT_TRUE("__asprintf_chk_0300", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0300", buf, "Str: hello");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0400
 * @tc.desc      : Verify __asprintf_chk with character format (c)
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0400(void)
{
    char *buf = NULL;
    char ch = 'Z';
    int ret = __asprintf_chk(&buf, 0, "Char: %c", ch);
    EXPECT_TRUE("__asprintf_chk_0400", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0400", buf, "Char: Z");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0500
 * @tc.desc      : Verify __asprintf_chk with hexadecimal format (x/X)
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0500(void)
{
    char *buf = NULL;
    int num = 255;
    int ret = __asprintf_chk(&buf, 0, "Hex: %x, %X", num, num);
    EXPECT_TRUE("__asprintf_chk_0500", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0500", buf, "Hex: ff, FF");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0600
 * @tc.desc      : Verify __asprintf_chk with floating-point format (f)
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0600(void)
{
    char *buf = NULL;
    double testFloat = 3.14159;
    int ret = __asprintf_chk(&buf, 0, "Float: %.2f", testFloat);
    EXPECT_TRUE("__asprintf_chk_0600", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0600", buf, "Float: 3.14");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0700
 * @tc.desc      : Verify __asprintf_chk with multiple format specifiers
 * @tc.level     : Level 0
 */
void  __asprintf_chk_0700(void)
{
    char *buf = NULL;
    int testInteger = 10;
    char *testString = "test";
    float testFloat = 2.5f;
    int ret = __asprintf_chk(&buf, 0, "Int: %d, Str: %s, Float: %f", testInteger, testString, testFloat);
    EXPECT_TRUE("__asprintf_chk_0700", ret > 0);
    EXPECT_STREQ("__asprintf_chk_0700", buf, "Int: 10, Str: test, Float: 2.500000");
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_0800
 * @tc.desc      : Verify invalid %N$
 * @tc.level     : Level 0
 */
void __asprintf_chk_0800(void)
{
    char *buf = NULL;
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)__asprintf_chk(&buf, 2, "%3$d\n", 0);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__asprintf_chk_0800", WIFSIGNALED(status));
        EXPECT_EQ("__asprintf_chk_0800", WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __asprintf_chk_0900
 * @tc.desc      : Verify specified parameters
 * @tc.level     : Level 0
 */
void __asprintf_chk_0900(void)
{
    char *buf = NULL;
    int result = __asprintf_chk(&buf, 2, "%2$s, %3$s, %1$s\n", "test", "this", "is");
    EXPECT_TRUE("__asprintf_chk_0900", result > 0);
    int expected_len = strlen("this, is, test\n");
    EXPECT_EQ("__asprintf_chk_0900", result, expected_len);
    free(buf);
}

/**
 * @tc.name      : __asprintf_chk_1000
 * @tc.desc      : Test core functions
 * @tc.level     : Level 0
 */
void __asprintf_chk_1000(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __asprintf_chk(&buf, 0, "%d, 0x%X, %o", 12345, 0xABCDEF, 0755);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "12345, 0xABCDEF, 755", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 1, "%d, 0x%X, %o", 12345, 0xABCDEF, 0755);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "12345, 0xABCDEF, 755", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 0, "%.6f, %.2f, %c", 3.1415926, 3.1415926, 'H');
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "3.141593, 3.14, H", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 1, "%.6f, %.2f, %c", 3.1415926, 3.1415926, 'H');
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "3.141593, 3.14, H", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 0, "");
    EXPECT_TRUE(__FUNCTION__, ret_val == 0);

    ret_val = __asprintf_chk(&buf, 1, "");
    EXPECT_TRUE(__FUNCTION__, ret_val == 0);
}

/**
 * @tc.name      : __asprintf_chk_1100
 * @tc.desc      : Safety Feature Testing
 * @tc.level     : Level 0
 */
void __asprintf_chk_1100(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __asprintf_chk(&buf, 0, "%d, %s", 67890, "vasprintf_compatible");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "67890, vasprintf_compatible", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 1, "%.3f, %x", 9.876, 0x123456);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "9.876, 123456", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 2, "%o, %s", 0777, "security_level_2");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "777, security_level_2", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;
}

/**
 * @tc.name      : __asprintf_chk_1200
 * @tc.desc      : Boundary and Compatibility Testing
 * @tc.level     : Level 0
 */
void __asprintf_chk_1200(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __asprintf_chk(&buf, 0, "\\n\\t\\r\\\"\\'\\\\\\a");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "\\n\\t\\r\\\"\\'\\\\\\a", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;
    ret_val = __asprintf_chk(&buf, 1, "\\n\\t\\r\\\"\\'\\\\\\a");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "\\n\\t\\r\\\"\\'\\\\\\a", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 0, "%z %q %k", 123, 456);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    ret_val = __asprintf_chk(&buf, 1, "%z %q %k", 123, 456);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);

    char long_src[2048] = {0};
    for (int i = 0; i < 2047; i++) {
        long_src[i] = 'a' + (i % 26);
    }
    long_src[2047] = '\0';
    ret_val = __asprintf_chk(&buf, 0, "%s", long_src);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, long_src, buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 1, "%s", long_src);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, long_src, buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __asprintf_chk(&buf, 0, NULL, 123);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    EXPECT_TRUE(__FUNCTION__, buf == NULL);
    ret_val = __asprintf_chk(&buf, 1, NULL, 123);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    EXPECT_TRUE(__FUNCTION__, buf == NULL);
}

/**
 * @tc.name      : __asprintf_chk_1300
 * @tc.desc      : Memory Management Test
 * @tc.level     : Level 0
 */
void __asprintf_chk_1300(void)
{
    char *buf1 = NULL, *buf2 = NULL, *buf3 = NULL;
    int ret1 = 0;
    int ret2 = 0;
    int ret3 = 0;

    ret1 = __asprintf_chk(&buf1, 0, "%d, %s", 11111, "mem_alloc_1");
    EXPECT_TRUE(__FUNCTION__, ret1 > 0);
    EXPECT_TRUE(__FUNCTION__, buf1 != NULL);

    ret2 = __asprintf_chk(&buf2, 1, "%.2f, %x", 99.99, 0xABCDEF);
    EXPECT_TRUE(__FUNCTION__, ret2 > 0);
    EXPECT_TRUE(__FUNCTION__, buf2 != NULL);

    ret3 = __asprintf_chk(&buf3, 2, "%o, %s", 0777, "mem_alloc_3");
    EXPECT_TRUE(__FUNCTION__, ret3 > 0);
    EXPECT_TRUE(__FUNCTION__, buf3 != NULL);

    if (buf1) {
        free(buf1);
        buf1 = NULL;
    }
    if (buf2) {
        free(buf2);
        buf2 = NULL;
    }
    if (buf3) {
        free(buf3);
        buf3 = NULL;
    }
}

void* mt_worker(void* arg)
{
    __asprintf_chk_0200();
    return NULL;
}

/**
 * @tc.name      : __asprintf_chk_1400
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __asprintf_chk_1400(void)
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
    __asprintf_chk_0100();
    __asprintf_chk_0200();
    __asprintf_chk_0300();
    __asprintf_chk_0400();
    __asprintf_chk_0500();
    __asprintf_chk_0600();
    __asprintf_chk_0700();
    __asprintf_chk_0800();
    __asprintf_chk_0900();
    __asprintf_chk_1000();
    __asprintf_chk_1100();
    __asprintf_chk_1200();
    __asprintf_chk_1300();
    __asprintf_chk_1400();

    return t_status;
}