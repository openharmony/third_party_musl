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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

const int ERROR_RET = -1;

static int test_vasprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = __vasprintf_chk(strp, 0, fmt, ap);
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

int main(int argc, char *argv[])
{
    vasprintf_0100();
    vasprintf_0200();
    vasprintf_0300();
    vasprintf_0400();
    vasprintf_0500();
    vasprintf_0600();
    vasprintf_0700();
    return t_status;
}