/*
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/timex.h>
#include <stdbool.h>
#include <time.h>
#include "functionalext.h"

const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : clock_adjtime_0100
 * @tc.desc      : Verify that the kernel time can be adjusted (all parameters are valid, clock_id is CLOCK_REALTIME)
 * @tc.level     : Level 0
 */
void clock_adjtime_0100(void)
{
    bool flag = false;
    struct timex tx = {0};
    int result = -1;
    result = clock_adjtime(CLOCK_REALTIME, &tx);
    if (result != -1)
    {
        flag = true;
    }
    EXPECT_TRUE("clock_adjtime_0100", flag);
}

/**
 * @tc.name      : clock_adjtime_0200
 * @tc.desc      : Verify that the kernel time can not be adjusted (each parameter is valid, clock_id is
 *                 CLOCK_MONOTONIC)
 * @tc.level     : Level 2
 */
void clock_adjtime_0200(void)
{
    struct timex tx = {ADJ_OFFSET_SS_READ};
    int result = -1;
    result = clock_adjtime(CLOCK_MONOTONIC, &tx);
    EXPECT_EQ("clock_adjtime_0200", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : clock_adjtime_0300
 * @tc.desc      : Verify that the kernel time can not be adjusted (all parameters are valid, clock_id is
 *                 CLOCK_PROCESS_CPUTIME_ID)
 * @tc.level     : Level 2
 */
void clock_adjtime_0300(void)
{
    struct timex tx = {0};
    int result = -1;
    result = clock_adjtime(CLOCK_PROCESS_CPUTIME_ID, &tx);
    EXPECT_EQ("clock_adjtime_0300", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : clock_adjtime_0400
 * @tc.desc      : Verify that the kernel time can not be adjusted (all parameters are valid, clock_id is
 *                 CLOCK_THREAD_CPUTIME_ID)
 * @tc.level     : Level 2
 */
void clock_adjtime_0400(void)
{
    struct timex tx = {0};
    int result = -1;
    result = clock_adjtime(CLOCK_THREAD_CPUTIME_ID, &tx);
    EXPECT_EQ("clock_adjtime_0400", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : clock_adjtime_0500
 * @tc.desc      : Verify that kernel time cannot be adjusted (tx parameter invalid)
 * @tc.level     : Level 2
 */
void clock_adjtime_0500(void)
{
    struct timex tx = {100000};
    int result = 0;
    result = clock_adjtime(CLOCK_REALTIME, &tx);
    EXPECT_EQ("clock_adjtime_0500", result, COUNT_NEGATIVE);
}

int main()
{
    clock_adjtime_0100();
    clock_adjtime_0200();
    clock_adjtime_0300();
    clock_adjtime_0400();
    clock_adjtime_0500();
    return t_status;
}