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
#include <string.h>
#include <time.h>
#include "functionalext.h"
#include "sys_param.h"

/**
 * @tc.name      : mktime_noenv_0100
 * @tc.desc      : test mktime_noenv roundtrip with localtime_noenv_r
 * @tc.level     : Level 0
 */
void mktime_noenv_0100(void)
{
    time_t t;
    for (t = 0; t < 1000; t++) {
        struct tm tm_buf;
        struct tm *tm = localtime_noenv_r(&t, &tm_buf);
        EXPECT_PTRNE("mktime_noenv_0100", tm, NULL);
        if (!tm) {
            continue;
        }
        time_t r = mktime_noenv(tm);
        EXPECT_EQ("mktime_noenv_0100", r, t);
    }
}

/**
 * @tc.name      : mktime_noenv_0200
 * @tc.desc      : test mktime_noenv DST spring forward gap resolution
 *                 set system timezone to EST5EDT, input 2026-03-08 02:00
 *                 which does not exist, expect normalization to 03:00 EDT
 * @tc.level     : Level 0
 */
void mktime_noenv_0200(void)
{
    char old_tz[256] = {0};
    CachedHandle handle = CachedParameterCreate("persist.time.timezone", "");
    const char *p = CachedParameterGet(handle);
    if (p) {
        strncpy(old_tz, p, sizeof(old_tz) - 1);
    }

    system("param set persist.time.timezone \"EST5EDT,M3.2.0/2,M11.1.0/2\"");
    tzset();

    struct tm tm = {
        .tm_sec = 0, .tm_min = 0, .tm_hour = 2,
        .tm_mday = 8, .tm_mon = 2, .tm_year = 126,
        .tm_wday = 0, .tm_yday = 0, .tm_isdst = -1
    };
    time_t t = mktime_noenv(&tm);
    EXPECT_EQ("mktime_noenv_0200", t, (time_t)1772953200);
    EXPECT_EQ("mktime_noenv_0200", tm.tm_hour, 3);
    EXPECT_EQ("mktime_noenv_0200", tm.tm_mday, 8);
    EXPECT_EQ("mktime_noenv_0200", tm.tm_mon, 2);
    EXPECT_EQ("mktime_noenv_0200", tm.tm_year, 126);
    EXPECT_EQ("mktime_noenv_0200", tm.tm_isdst, 1);

    if (old_tz[0]) {
        char cmd[512] = {0};
        snprintf(cmd, sizeof(cmd), "param set persist.time.timezone \"%s\"", old_tz);
        system(cmd);
    }
    tzset();
}

int main(void)
{
    mktime_noenv_0100();
    mktime_noenv_0200();
    return t_status;
}
