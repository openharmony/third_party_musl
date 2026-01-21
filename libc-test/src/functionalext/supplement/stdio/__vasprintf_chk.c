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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

const int ERROR_RET = -1;

static int test_vasprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = __vasprintf_chk(strp, 0, fmt, ap);
    va_end(ap);
    return ret;
}

int __vasprintf_chk_wrapper(char **strp, int flag, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = __vasprintf_chk(strp, flag, format, ap);
    va_end(ap);
    return ret;
}

/**
 * @tc.name      : vasprintf_0100
 * @tc.desc      : Verify __vasprintf_chk with NULL format string
 * @tc.level     : Level 0
 */
void vasprintf_0100(void)
{
    char *buf = NULL;
    int ret = test_vasprintf(&buf, NULL, 0);
    EXPECT_EQ("vasprintf_0100", ret, ERROR_RET);
    EXPECT_EQ("vasprintf_0100", (void*)buf, (void*)NULL);
}

/**
 * @tc.name      : vasprintf_0200
 * @tc.desc      : Verify __vasprintf_chk with octal format (o)
 * @tc.level     : Level 0
 */
void vasprintf_0200(void)
{
    char *buf = NULL;
    int num = 64;
    int ret = test_vasprintf(&buf, "Oct: %o", num);
    EXPECT_TRUE("vasprintf_0200", ret > 0);
    EXPECT_STREQ("vasprintf_0200", buf, "Oct: 100");
    free(buf);
}

/**
 * @tc.name      : vasprintf_0300
 * @tc.desc      : Verify __vasprintf_chk with unsigned integer (u)
 * @tc.level     : Level 0
 */
void vasprintf_0300(void)
{
    char *buf = NULL;
    unsigned int num = 4294967295U;
    int ret = test_vasprintf(&buf, "Unsigned: %u", num);
    EXPECT_TRUE("vasprintf_0300", ret > 0);
    EXPECT_STREQ("vasprintf_0300", buf, "Unsigned: 4294967295");
    free(buf);
}

/**
 * @tc.name      : vasprintf_0400
 * @tc.desc      : Verify __vasprintf_chk with exponential format (e/E)
 * @tc.level     : Level 0
 */
void vasprintf_0400(void)
{
    char *buf = NULL;
    double testFloat = 12345.678;
    int ret = test_vasprintf(&buf, "Exp: %e, %E", testFloat, testFloat);
    EXPECT_TRUE("vasprintf_0400", ret > 0);
    EXPECT_STREQ("vasprintf_0400", buf, "Exp: 1.234568e+04, 1.234568E+04");
    free(buf);
}

/**
 * @tc.name      : vasprintf_0500
 * @tc.desc      : Verify __vasprintf_chk with general format (g/G)
 * @tc.level     : Level 0
 */
void vasprintf_0500(void)
{
    char *buf = NULL;
    double testFloat = 0.00001234;
    int ret = test_vasprintf(&buf, "Gen: %g, %G", testFloat, testFloat);
    EXPECT_TRUE("vasprintf_0500", ret > 0);
    EXPECT_STREQ("vasprintf_0500", buf, "Gen: 1.234e-05, 1.234E-05");
    free(buf);
}

/**
 * @tc.name      : vasprintf_0600
 * @tc.desc      : Verify __vasprintf_chk with pointer format (p)
 * @tc.level     : Level 0
 */
void vasprintf_0600(void)
{
    char *buf = NULL;
    int var = 0;
    void *ptr = &var;
    int ret = test_vasprintf(&buf, "Ptr: %p", ptr);
    EXPECT_TRUE("vasprintf_0600", ret > 0);
    free(buf);
}

/**
 * @tc.name      : vasprintf_0700
 * @tc.desc      : Verify __vasprintf_chk with empty format string
 * @tc.level     : Level 0
 */
void vasprintf_0700(void)
{
    char *buf = NULL;
    int ret = test_vasprintf(&buf, "", 123);
    EXPECT_EQ("vasprintf_0700", ret, 0);
    EXPECT_STREQ("vasprintf_0700", buf, "");
    free(buf);
}

/**
 * @tc.name      : vasprintf_0800
 * @tc.desc      : Test core functions
 * @tc.level     : Level 0
 */
void vasprintf_0800(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "%d, 0x%X, %o, %s", 12345, 0xABCDEF, 0755, "__vasprintf_chk_basic");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "12345, 0xABCDEF, 755, __vasprintf_chk_basic", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 1, "%d, 0x%X, %o, %s", 12345, 0xABCDEF, 0755, "__vasprintf_chk_basic");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "12345, 0xABCDEF, 755, __vasprintf_chk_basic", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "%.6f, %.2f, %c, %d", 3.1415926, 3.1415926, 'H', 1);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "3.141593, 3.14, H, 1", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 1, "%.6f, %.2f, %c, %d", 3.1415926, 3.1415926, 'H', 1);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "3.141593, 3.14, H, 1", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "");
    EXPECT_TRUE(__FUNCTION__, ret_val == 0);

    ret_val = __vasprintf_chk_wrapper(&buf, 1, "");
    EXPECT_TRUE(__FUNCTION__, ret_val == 0);
}

/**
 * @tc.name      : vasprintf_0900
 * @tc.desc      : Safety Feature Testing
 * @tc.level     : Level 0
 */
void vasprintf_0900(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "%d, %s, %o", 67890, "vasprintf_compatible", 0644);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "67890, vasprintf_compatible, 644", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 1, "%.3f, %x, %s", 9.876, 0x123456, "security_check_enable");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "9.876, 123456, security_check_enable", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 2, "%d, %.2f, %c", 11111, 66.88, 'S');
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "11111, 66.88, S", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;
}

/**
 * @tc.name      : vasprintf_1000
 * @tc.desc      : Boundary and Compatibility Testing
 * @tc.level     : Level 0
 */
void vasprintf_1000(void)
{
    char *buf = NULL;
    int ret_val = 0;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "\\n\\t\\r\\\"\\'\\\\\\a");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "\\n\\t\\r\\\"\\'\\\\\\a", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;
    ret_val = __vasprintf_chk_wrapper(&buf, 1, "\\n\\t\\r\\\"\\'\\\\\\a");
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, "\\n\\t\\r\\\"\\'\\\\\\a", buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, "%z %q %k", 123, 456);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    ret_val = __vasprintf_chk_wrapper(&buf, 1, "%z %q %k", 123, 456);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);

    char long_src[2048] = {0};
    for (int i = 0; i < 2047; i++) {
        long_src[i] = 'a' + (i % 26);
    }
    long_src[2047] = '\0';
    ret_val = __vasprintf_chk_wrapper(&buf, 0, "%s", long_src);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, long_src, buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 1, "%s", long_src);
    EXPECT_TRUE(__FUNCTION__, ret_val > 0);
    EXPECT_STREQ(__FUNCTION__, long_src, buf);
    EXPECT_EQ(__FUNCTION__, ret_val, strlen(buf));
    free(buf);
    buf = NULL;

    ret_val = __vasprintf_chk_wrapper(&buf, 0, NULL, 123);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    EXPECT_TRUE(__FUNCTION__, buf == NULL);
    ret_val = __vasprintf_chk_wrapper(&buf, 1, NULL, 123);
    EXPECT_TRUE(__FUNCTION__, ret_val < 0);
    EXPECT_TRUE(__FUNCTION__, buf == NULL);
}

/**
 * @tc.name      : vasprintf_1100
 * @tc.desc      : Memory Management Test
 * @tc.level     : Level 0
 */
void vasprintf_1100(void)
{
    char *buf1 = NULL, *buf2 = NULL, *buf3 = NULL;
    int ret1 = 0;
    int ret2 = 0;
    int ret3 = 0;

    ret1 = __vasprintf_chk_wrapper(&buf1, 0, "%d, %s", 11111, "mem_alloc_1");
    EXPECT_TRUE(__FUNCTION__, ret1 > 0);
    EXPECT_TRUE(__FUNCTION__, buf1 != NULL);

    ret2 = __vasprintf_chk_wrapper(&buf2, 1, "%.2f, %x", 99.99, 0xABCDEF);
    EXPECT_TRUE(__FUNCTION__, ret2 > 0);
    EXPECT_TRUE(__FUNCTION__, buf2 != NULL);

    ret3 = __vasprintf_chk_wrapper(&buf3, 2, "%o, %s", 0777, "mem_alloc_3");
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
    vasprintf_0200();
    return NULL;
}

/**
 * @tc.name      : vasprintf_1200
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void vasprintf_1200(void)
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
    vasprintf_0100();
    vasprintf_0200();
    vasprintf_0300();
    vasprintf_0400();
    vasprintf_0500();
    vasprintf_0600();
    vasprintf_0700();
    vasprintf_0800();
    vasprintf_0900();
    vasprintf_1000();
    vasprintf_1100();
    vasprintf_1200();

    return t_status;
}