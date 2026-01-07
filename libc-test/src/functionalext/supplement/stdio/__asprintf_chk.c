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

#include "functionalext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

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
    
    return t_status;
}