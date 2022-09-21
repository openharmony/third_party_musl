/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <time.h>
#include "asctime_data.h"
#include "functionalext.h"
#include "strftime_data.h"

static time_t gTime = 1659177614;
static int16_t gBufferSize = 256;

/**
 * @tc.name      : strftime_0100
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("strftime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strftime_0100", timeptr, NULL);
            return;
        }
        size_t count = strftime(buffer, sizeof(buffer) - 1, "%c", timeptr);
        EXPECT_TRUE("strftime_0100", count > 0);
        EXPECT_STREQ("strftime_0100", buffer, test_asctime_data[i].result);
    }
}

/**
 * @tc.name      : strftime_0200
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0200(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_strftime_data) / sizeof(test_strftime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_strftime_data[i].tz);
        if (!handlerChar) {
            t_error("strftime_0200 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strftime_0200", timeptr, NULL);
            return;
        }
        char buffer[gBufferSize];
        size_t count = strftime(buffer, sizeof(buffer) - 1, "%c %Z%z", timeptr);
        EXPECT_TRUE("strftime_0200", count > 0);
        EXPECT_STREQ("strftime_0200", buffer, test_strftime_data[i].result);
    }
}

/**
 * @tc.name      : strftime_0300
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0300(void)
{
    const char *handlerChar = test_handle_path("Pacific/Pitcairn");
    if (!handlerChar) {
        t_error("strftime_0300 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0300", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%k", timeptr);
    EXPECT_TRUE("strftime_0300", count > 0);
    EXPECT_STREQ("strftime_0300", buffer, " 2");
}

/**
 * @tc.name      : strftime_0400
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0400(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0400 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0400", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%k", timeptr);
    EXPECT_TRUE("strftime_0400", count > 0);
    EXPECT_STREQ("strftime_0400", buffer, "18");
}

/**
 * @tc.name      : strftime_0500
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0500(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0500 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0500", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%I", timeptr);
    EXPECT_TRUE("strftime_0500", count > 0);
    EXPECT_STREQ("strftime_0500", buffer, "06");
}

/**
 * @tc.name      : strftime_0600
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0600(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0600 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0600", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%P", timeptr);
    EXPECT_TRUE("strftime_0600", count > 0);
    EXPECT_STREQ("strftime_0600", buffer, "pm");
}

/**
 * @tc.name      : strftime_0700
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0700(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0700 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0700", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%v", timeptr);
    EXPECT_TRUE("strftime_0700", count > 0);
    EXPECT_STREQ("strftime_0700", buffer, "30-Jul-2022");
}

int main(void)
{
    strftime_0100();
    strftime_0200();
    strftime_0300();
    strftime_0400();
    strftime_0500();
    strftime_0600();
    strftime_0700();
    return t_status;
}