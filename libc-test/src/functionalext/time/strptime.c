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
#include "strptime_data.h"

static int16_t gBufferSize = 256;
static time_t gTime = 1659177614;
static int16_t gYearBase = 1900;

/**
 * @tc.name      : strptime_0100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0100(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("strptime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strptime_0100", timeptr, NULL);
            return;
        }
        size_t cnt = strftime(buffer, sizeof(buffer) - 1, "%c", timeptr);
        EXPECT_TRUE("strptime_0100", cnt > 0);

        struct tm tmTime = {0};
        strptime(buffer, "%c", &tmTime);
        char *result = asctime(&tmTime);
        if (result == NULL) {
            EXPECT_FALSE("strptime_0100", result == NULL);
            return;
        }
        result[strlen(result) - 1] = 0x00;
        EXPECT_STREQ("strptime_0100", test_asctime_data[i].result, result);
    }
}

/**
 * @tc.name      : strptime_0200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0200(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(test_strptime_data)/sizeof(test_strptime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_strptime_data[i].tz);
        if (!handlerChar) {
            t_error("strptime_0200 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_TRUE("strptime_0200", timeptr == NULL);
            return;
        }
        size_t len = strftime(buffer, sizeof(buffer) - 1, "%c %Z%z", timeptr);
        EXPECT_TRUE("strptime_0200", len > 0);
        struct tm tmTime = {0};
        strptime(buffer, "%c %Z%z", &tmTime);
        char buffResult[gBufferSize];

        int cnt = sprintf(buffResult, "%d-%d-%d %d:%d:%d wday=%d,yday=%d,isdst=%d,gmtoff=%ld,zone=%s",
            (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday, tmTime.tm_hour,
            tmTime.tm_min, tmTime.tm_sec, tmTime.tm_wday, tmTime.tm_yday, tmTime.tm_isdst,
            tmTime.tm_gmtoff, tmTime.tm_zone);
        EXPECT_TRUE("strptime_0200", cnt > 0);
        EXPECT_STREQ("strptime_0200", test_strptime_data[i].result, buffResult);
    }
}

/**
 * @tc.name      : strptime_0300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0300(void)
{
    char *buffer = "2022-04-10";
    struct tm tmTime = {0};
    strptime(buffer, "%F", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%04d-%02d-%02d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon + 1, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0300", cnt > 0);
    EXPECT_STREQ("strptime_0300", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0400(void)
{
    char *buffer = "23";
    struct tm tmTime = {0};
    strptime(buffer, "%g", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_year);
    EXPECT_TRUE("strptime_0400", cnt > 0);
    EXPECT_STREQ("strptime_0400", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0500(void)
{
    const char *buffer = "16";
    struct tm tmTime = {0};
    strptime(buffer, " %k", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0500", cnt > 0);
    EXPECT_STREQ("strptime_0500", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0600(void)
{
    const char *buffer = " 4";
    struct tm tmTime = {0};
    strptime(buffer, " %l", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0600", cnt > 0);
    EXPECT_STREQ("strptime_0600", "4", buffResult);
}

/**
 * @tc.name      : strptime_0700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0700(void)
{
    const char *buffer = "1659177614";
    const char *handlerChar = test_handle_path(test_asctime_data[0].tz);
    if (!handlerChar) {
        t_error("strptime_0700 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    time_t second = 0;
    struct tm tmTime = {0};
    strptime(buffer, "%s", &tmTime);
    second = mktime(&tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%ld", second);
    EXPECT_TRUE("strptime_0700", cnt > 0);
    EXPECT_STREQ("strptime_0700", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0800(void)
{
    const char *buffer = "1";
    struct tm tmTime = {0};
    strptime(buffer, "%u", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_wday);
    EXPECT_TRUE("strptime_0800", cnt > 0);
    EXPECT_STREQ("strptime_0800", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0900(void)
{
    const char *buffer = "30-Oct-2021";
    struct tm tmTime = {0};
    strptime(buffer, "%v", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d-%d-%d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0900", cnt > 0);
    EXPECT_STREQ("strptime_0900", "2021-9-30", buffResult);
}

int main(void)
{
    strptime_0100();
    strptime_0200();
    strptime_0300();
    strptime_0400();
    strptime_0500();
    strptime_0600();
    strptime_0700();
    strptime_0800();
    strptime_0900();
    return t_status;
}