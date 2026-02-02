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

static time_t gTime = 1659177614;

/**
 * @tc.name      : asctime_0100
 * @tc.desc      : according to different time zones, take an argument representing broken-down time,
 * which is a representation separated into year, month, day, and so on
 * @tc.level     : Level 0
 */
void asctime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("asctime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("asctime_0100", timeptr, NULL);
            return;
        }
        char *returnStr = asctime(timeptr);
        if (returnStr == NULL) {
            EXPECT_FALSE("asctime_0100", returnStr == NULL);
            return;
        }
        returnStr[strlen(returnStr) - 1] = 0x00;
        EXPECT_STREQ("asctime_0100", test_asctime_data[i].result, returnStr);
    }
}

/**
 * @tc.name      : asctime_0200
 * @tc.desc      : Test asctime with boundary time values (minimum/maximum valid tm struct fields)
 * @tc.level     : Level 1
 */
void asctime_0200(void)
{
    // Test minimum valid time values (1900-01-01 00:00:00)
    struct tm min_tm = {
        .tm_sec = 0,    // Minimum seconds (0-59)
        .tm_min = 0,    // Minimum minutes (0-59)
        .tm_hour = 0,   // Minimum hours (0-23)
        .tm_mday = 1,   // Minimum day of month (1-31)
        .tm_mon = 0,    // Minimum month (0=January, 11=December)
        .tm_year = 0,   // Year 1900 (tm_year = year - 1900)
        .tm_wday = 0,   // Sunday (0-6)
        .tm_yday = 0,   // First day of year (0-365)
        .tm_isdst = 0   // No daylight saving time
    };

    char *result = asctime(&min_tm);
    if (result == NULL) {
        EXPECT_FALSE("asctime_0200(min)", result == NULL);
        return;
    }
    // Expected format: "Day Mon DD HH:MM:SS YYYY\n" (trim the newline for comparison)
    result[strlen(result) - 1] = 0x00;
    EXPECT_STREQ("asctime_0200(min)", "Sun Jan  1 00:00:00 1900", result);

    // Test maximum valid time values (2024-12-31 23:59:59)
    struct tm max_tm = {
        .tm_sec = 59,    // Maximum seconds
        .tm_min = 59,    // Maximum minutes
        .tm_hour = 23,   // Maximum hours
        .tm_mday = 31,   // Maximum day of month
        .tm_mon = 11,    // December
        .tm_year = 124,  // Year 2024
        .tm_wday = 2,    // Tuesday
        .tm_yday = 365,  // Last day of leap year 2024
        .tm_isdst = 1    // Daylight saving time enabled
    };

    result = asctime(&max_tm);
    if (result == NULL) {
        EXPECT_FALSE("asctime_0200(max)", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00;
    EXPECT_STREQ("asctime_0200(max)", "Tue Dec 31 23:59:59 2024", result);
}

/**
 * @tc.name      : asctime_0300
 * @tc.desc      : Test asctime with invalid tm struct inputs
 * @tc.level     : Level 2
 */
void asctime_0300(void)
{
    // tm struct with out-of-range values (invalid seconds)
    struct tm invalid_sec_tm = {
        .tm_sec = 60,   // Invalid (seconds range: 0-59)
        .tm_min = 30,
        .tm_hour = 12,
        .tm_mday = 15,
        .tm_mon = 6,
        .tm_year = 120
    };
    char *result = asctime(&invalid_sec_tm);
    // Note: Behavior for invalid values is implementation-defined, check for non-crash first
    if (result != NULL) {
        EXPECT_PTRNE("asctime_0300(invalid_sec): returned non-NULL for invalid seconds", result, NULL);
    } else {
        EXPECT_FALSE("asctime_0100", result == NULL);
        return;
    }

    struct tm invalid_mon_tm = {
        .tm_sec = 30,
        .tm_min = 30,
        .tm_hour = 12,
        .tm_mday = 15,
        .tm_mon = 12,   // Invalid month
        .tm_year = 120
    };
    result = asctime(&invalid_mon_tm);
    if (result != NULL) {
        EXPECT_PTRNE("asctime_0300(invalid_mon): returned non-NULL for invalid month:", result, NULL);
    } else {
        EXPECT_FALSE("asctime_0100", result == NULL);
        return;
    }
}

/**
 * @tc.name      : asctime_0400
 * @tc.desc      : Test asctime with leap year dates (February 29) and special calendar dates
 * @tc.level     : Level 1
 */
void asctime_0400(void)
{
    // Leap year 2024 - February 29
    struct tm leap_day_tm = {
        .tm_sec = 15,
        .tm_min = 45,
        .tm_hour = 10,
        .tm_mday = 29,   // February 29 (valid for leap year 2024)
        .tm_mon = 1,     // February (0-based)
        .tm_year = 124,  // 2024 (1900 + 124)
        .tm_wday = 4,    // Thursday
        .tm_yday = 60,   // 60th day of leap year
        .tm_isdst = 0
    };

    // Non-leap year 2023 - February 28 (invalid to use 29)
    struct tm non_leap_day_tm = {
        .tm_sec = 20,
        .tm_min = 10,
        .tm_hour = 18,
        .tm_mday = 28,   // February 28 (2023 is not leap year)
        .tm_mon = 1,
        .tm_year = 123,  // 2023
        .tm_wday = 2,
        .tm_yday = 58,
        .tm_isdst = 0
    };

    // Test leap day
    char *result = asctime(&leap_day_tm);
    if (result == NULL) {
        EXPECT_FALSE("asctime_0400(leap_day)", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00;
    EXPECT_STREQ("asctime_0400(leap_day)", "Thu Feb 29 10:45:15 2024", result);

    // Test non-leap day
    result = asctime(&non_leap_day_tm);
    if (result == NULL) {
        EXPECT_FALSE("asctime_0400(non_leap_day)", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00;
    EXPECT_STREQ("asctime_0400(non_leap_day)", "Tue Feb 28 18:10:20 2023", result);
}

int main(void)
{
    asctime_0100();
    asctime_0200();
    asctime_0300();
    asctime_0400();
    return t_status;
}