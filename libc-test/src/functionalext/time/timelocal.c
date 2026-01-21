/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#include "localtime_data.h"
#include "mktime_data.h"
#include "functionalext.h"

static time_t gTime[] = {
    1659117614,
    1759127615,
    1859137616,
    1959147617,
    1059157618,
    1159167619,
    1259177610,
    1359187611,
    1459197612,
    1559107613,
    1659177615,
};

/**
 * @tc.name      : timelocal_0100
 * @tc.desc      : according to different time zones, convert time to seconds in duration since 1970-1-1
 * @tc.level     : Level 0
 */
void timelocal_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_mktime_data) / sizeof(test_mktime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_mktime_data[i].tz);
        if (!handlerChar) {
            t_error("timelocal_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *localtm = localtime(&(test_mktime_data[i].result));
        if (!localtm) {
            EXPECT_PTRNE("timelocal_0100", localtm, NULL);
            return;
        }
        time_t mk = timelocal(localtm);
        EXPECT_EQ("timelocal_0100", mk, test_mktime_data[i].result);
    }
}

/**
 * @tc.name      : timelocal_0200
 * @tc.desc      : according to different time zones
 * @tc.level     : Level 0
 */
void timelocal_0200(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_localtime_data) / sizeof(test_localtime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_localtime_data[i].tz);
        if (!handlerChar) {
            printf("newlocale failed");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        for (int32_t j = 0; j < (int32_t)(sizeof(gTime) / sizeof(gTime[0])); j++) {
            struct tm *localtm = localtime(&gTime[j]);
            if (!localtm) {
                EXPECT_PTRNE("timelocal_0200", localtm, NULL);
                return;
            }
            time_t timel = timelocal(localtm);
            EXPECT_TRUE("timelocal_0200", timel == gTime[j]);
        }
    }
}

/**
 * @tc.name      : timelocal_0300
 * @tc.desc      : basic case
 * @tc.level     : Level 0
 */
void timelocal_0300(void)
{
    struct tm local_tm;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();

    local_tm.tm_year = 2025 - 1900;
    local_tm.tm_mon = 12 - 1;
    local_tm.tm_mday = 31;
    local_tm.tm_hour = 23;
    local_tm.tm_min = 59;
    local_tm.tm_sec = 50;
    local_tm.tm_isdst = -1;

    time_t unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
        return;
    }
    EXPECT_LONGLONGEQ("timelocal_0300 unix_ts", unix_ts, 1767196790LL);

    struct tm *verify_tm = localtime(&unix_ts);
    if (verify_tm == NULL) {
        t_error("localtime fail\n");
    } else {
        EXPECT_EQ("timelocal_0300 year", verify_tm->tm_year, local_tm.tm_year);
        EXPECT_EQ("timelocal_0300 mon", verify_tm->tm_mon, local_tm.tm_mon);
        EXPECT_EQ("timelocal_0300 day", verify_tm->tm_mday, local_tm.tm_mday);
        EXPECT_EQ("timelocal_0300 hour", verify_tm->tm_hour, local_tm.tm_hour);
        EXPECT_EQ("timelocal_0300 min", verify_tm->tm_min, local_tm.tm_min);
        EXPECT_EQ("timelocal_0300 sec", verify_tm->tm_sec, local_tm.tm_sec);
    }
}

/**
 * @tc.name      : timelocal_0400
 * @tc.desc      : edge case
 * @tc.level     : Level 0
 */
void timelocal_0400(void)
{
    struct tm local_tm;
    time_t unix_ts;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();

    local_tm.tm_year = 2024 - 1900;
    local_tm.tm_mon = 2 - 1;
    local_tm.tm_mday = 29;
    local_tm.tm_hour = 12;
    local_tm.tm_min = 0;
    local_tm.tm_sec = 0;
    local_tm.tm_isdst = -1;
    unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0400 unix_ts", unix_ts, 1709179200LL);

    local_tm.tm_year = 2026 - 1900;
    local_tm.tm_mon = 1 - 1;
    local_tm.tm_mday = 1;
    local_tm.tm_hour = 0;
    local_tm.tm_min = 0;
    local_tm.tm_sec = 0;
    local_tm.tm_isdst = -1;
    unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0400 unix_ts", unix_ts, 1767196800LL);
}

/**
 * @tc.name      : timelocal_0500
 * @tc.desc      : dst case
 * @tc.level     : Level 0
 */
void timelocal_0500(void)
{
    struct tm local_tm;

    local_tm.tm_year = 2025 - 1900;
    local_tm.tm_mon = 6 - 1;
    local_tm.tm_mday = 10;
    local_tm.tm_hour = 15;
    local_tm.tm_min = 30;
    local_tm.tm_sec = 0;

    local_tm.tm_isdst = -1;
    time_t ts1 = timelocal(&local_tm);
    if (ts1 == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0500 tm_isdst=-1", ts1, 1749540600LL);

    local_tm.tm_isdst = 0;
    time_t ts2 = timelocal(&local_tm);
    if (ts2 == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0500 tm_isdst=0", ts2, 1749540600LL);

    local_tm.tm_isdst = 1;
    time_t ts3 = timelocal(&local_tm);
    if (ts3 == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0500 tm_isdst=1", ts2, 1749540600LL);
}

/**
 * @tc.name      : timelocal_0600
 * @tc.desc      : auto correct case
 * @tc.level     : Level 0
 */
void timelocal_0600(void)
{
    struct tm local_tm;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();

    local_tm.tm_year = 2025 - 1900;
    local_tm.tm_mon = 12 - 1;
    local_tm.tm_mday = 35;
    local_tm.tm_hour = 25;
    local_tm.tm_min = 65;
    local_tm.tm_sec = 70;
    local_tm.tm_isdst = -1;

    time_t unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
        return;
    }
    EXPECT_LONGLONGEQ("timelocal_0600", unix_ts, 1767549970LL);

    struct tm *corrected_tm = localtime(&unix_ts);
    if (corrected_tm == NULL) {
        t_error("err: localtime fail!\n");
        return;
    }
    EXPECT_EQ("timelocal_0600 year", corrected_tm->tm_year, 2026 - 1900);
    EXPECT_EQ("timelocal_0600 mon", corrected_tm->tm_mon, 0);
    EXPECT_EQ("timelocal_0600 day", corrected_tm->tm_mday, 5);
    EXPECT_EQ("timelocal_0600 hour", corrected_tm->tm_hour, 2);
    EXPECT_EQ("timelocal_0600 min", corrected_tm->tm_min, 6);
    EXPECT_EQ("timelocal_0600 sec", corrected_tm->tm_sec, 10);
}

/**
 * @tc.name      : timelocal_0700
 * @tc.desc      : edge case
 * @tc.level     : Level 0
 */
void timelocal_0700(void)
{
    struct tm local_tm;
    time_t unix_ts;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();

    local_tm.tm_year = 2025 - 1900;
    local_tm.tm_mon = 12 - 1;
    local_tm.tm_mday = 31;
    local_tm.tm_hour = 23;
    local_tm.tm_min = 59;
    local_tm.tm_sec = 60;
    local_tm.tm_isdst = -1;
    unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0700 unix_ts", unix_ts, 1767196800LL);
}

/**
 * @tc.name      : timelocal_0800
 * @tc.desc      : edge case
 * @tc.level     : Level 0
 */
void timelocal_0800(void)
{
    struct tm local_tm;
    time_t unix_ts;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();

    local_tm.tm_year = 2025 - 1900;
    local_tm.tm_mon = 2 - 1;
    local_tm.tm_mday = 30;
    local_tm.tm_hour = 10;
    local_tm.tm_min = 0;
    local_tm.tm_sec = 0;
    local_tm.tm_isdst = -1;
    unix_ts = timelocal(&local_tm);
    if (unix_ts == (time_t)-1) {
        t_error("err: timelocal fail!\n");
    }
    EXPECT_LONGLONGEQ("timelocal_0800 unix_ts", unix_ts, 1740880800LL);
}

int main(void)
{
    timelocal_0100();
    timelocal_0200();
    timelocal_0300();
    timelocal_0400();
    timelocal_0500();
    timelocal_0600();
    timelocal_0700();
    timelocal_0800();
    return t_status;
}