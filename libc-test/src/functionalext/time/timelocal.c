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

int main(void)
{
    timelocal_0100();
    timelocal_0200();
    return t_status;
}