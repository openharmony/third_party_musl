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
static int16_t gBufferSize = 256;

extern char *__ctime64_r (const time_t *, char *);

/**
 * @tc.name      : ctime_r_0100
 * @tc.desc      : according to different time zones, covert date and time to string
 * @tc.level     : Level 0
 */
void ctime_r_0100(void)
{
    for (int32_t i = 0;i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("ctime_r_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char s[gBufferSize];
        char *returnStr = ctime_r(&gTime, s);
        if (returnStr == NULL) {
            EXPECT_FALSE("ctime_r_0100", returnStr == NULL);
            return;
        }
        returnStr[strlen(returnStr) - 1] = 0x00;
        EXPECT_STREQ("ctime_r_0100", returnStr, test_asctime_data[i].result);
        EXPECT_STREQ("ctime_r_0100", returnStr, s);
    }
}

/**
 * @tc.name      : ctime_r_0200
 * @tc.desc      : Test ctime_r with boundary time values (minimum/maximum valid time_t values)
 * @tc.level     : Level 1
 */
void ctime_r_0200(void)
{
    // Core fix: Force switch to UTC timezone to eliminate all timezone offsets
    const char *original_tz = getenv("TZ"); // Preserve the original timezone setting
    setenv("TZ", "UTC", 1);                // Force set timezone to UTC (no offset)
    tzset();                               // Apply timezone configuration immediately

    // Test minimum valid time_t value (epoch start: Jan 1 1970 00:00:00 UTC)
    time_t min_time = 0;
    char buffer[gBufferSize];

    char *result = ctime_r(&min_time, buffer);
    if (result == NULL) {
        EXPECT_FALSE("ctime_r_0200", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00; // Remove the trailing newline character
    EXPECT_TRUE("ctime_r_0200(min)",
                strcmp(result, "Thu Jan  1 00:00:00 1970") == 0 ||
                strcmp(result, "Thu Jan 1 00:00:00 1970") == 0);
    EXPECT_STREQ("ctime_r_0200(min_buffer)", result, buffer);

    // Test future time value (near 32-bit time_t limit: 2038-01-19 03:14:07 UTC)
    time_t future_time = 2147483647;
    result = ctime_r(&future_time, buffer);
    if (result == NULL) {
        EXPECT_FALSE("ctime_r_0200", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00; // Remove the trailing newline character
    EXPECT_TRUE("ctime_r_0200(future)",
                strcmp(result, "Tue Jan  19 03:14:07 2038") == 0 ||
                strcmp(result, "Tue Jan 19 03:14:07 2038") == 0 );
    EXPECT_STREQ("ctime_r_0200(future_buffer)", result, buffer);

    // Critical: Restore original timezone to avoid impacting other test cases
    if (original_tz != NULL) {
        setenv("TZ", original_tz, 1);
    } else {
        unsetenv("TZ"); // Clear TZ variable if no original timezone existed
    }
    tzset(); // Re-apply original timezone configuration
}

/**
 * @tc.name      : ctime64_r_0100
 * @tc.desc      : according to different time zones, covert date and time to string
 * @tc.level     : Level 0
 */
void ctime64_r_0100(void)
{
    for (int32_t i = 0;i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("ctime64_r_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char s[gBufferSize];
        char *returnStr = __ctime64_r(&gTime, s);
        if (returnStr == NULL) {
            EXPECT_FALSE("ctime64_r_0100", returnStr == NULL);
            return;
        }
        returnStr[strlen(returnStr) - 1] = 0x00;
        EXPECT_STREQ("ctime64_r_0100", returnStr, test_asctime_data[i].result);
        EXPECT_STREQ("ctime64_r_0100", returnStr, s);
    }
}

/**
 * @tc.name      : ctime64_r_0200
 * @tc.desc      : Test __ctime64_r with 64-bit boundary time values (beyond 2038 limit)
 * @tc.level     : Level 1
 */
void ctime64_r_0200(void)
{
    const char *original_tz = getenv("TZ"); // Preserve the original timezone setting
    setenv("TZ", "UTC", 1);                // Force set timezone to UTC (no offset)
    tzset();                               // Apply timezone configuration immediately

    // Test 64-bit time value (Jan 1 2100 00:00:00 UTC - beyond 32-bit time_t limit)
    int64_t future_64_time_val = 4102444800LL; // 2100-01-01 00:00:00 UTC
    time_t future_64_time = (time_t)future_64_time_val;
    char buffer[gBufferSize];
    memset(buffer, 0, gBufferSize);

    char *result = __ctime64_r(&future_64_time, buffer);
    if (result == NULL) {
        EXPECT_FALSE("ctime64_r_0100", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00;
    EXPECT_TRUE("ctime64_r_0200(64bit_future)",
                strcmp(result, "Fri Jan  1 00:00:00 2100") == 0 ||
                strcmp(result, "Fri Jan 1 00:00:00 2100") == 0);
    EXPECT_STREQ("ctime64_r_0200(64bit_buffer)", result, buffer);

    // Test negative 64-bit time value (Dec 31 1969 23:59:59 UTC - before epoch)
    time_t negative_64_time = -1;
    memset(buffer, 0, gBufferSize);
    result = __ctime64_r(&negative_64_time, buffer);
    if (result == NULL) {
        EXPECT_FALSE("ctime64_r_0200", result == NULL);
        return;
    }
    result[strlen(result) - 1] = 0x00;
    EXPECT_TRUE("ctime64_r_0200(negative)",
                strcmp(result, "Wed Dec 31 23:59:59 1969") == 0 ||
                strcmp(result, "Wed Dec  31 23:59:59 1969") == 0);

    // Critical: Restore original timezone to avoid impacting other test cases
    if (original_tz != NULL) {
        setenv("TZ", original_tz, 1);
    } else {
        unsetenv("TZ"); // Clear TZ variable if no original timezone existed
    }
    tzset(); // Re-apply original timezone configuration
}

int main(void)
{
    ctime_r_0100();
    ctime_r_0200();
    ctime64_r_0100();
    ctime64_r_0200();
    return t_status;
}