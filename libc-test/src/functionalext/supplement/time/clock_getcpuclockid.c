/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/timex.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : clock_getcpuclockid_0100
 * @tc.desc      : Verify that the clock ID can be obtained (all parameters are valid)
 * @tc.level     : Level 0
 */
void clock_getcpuclockid_0100(void)
{
    bool flag = false;
    clockid_t clk = 0;
    int result = -1;
    pid_t pid;
    pid = getppid();
    result = clock_getcpuclockid(pid, &clk);
    if (clk != 0) {
        flag = true;
    }
    EXPECT_TRUE("clock_getcpuclockid_0100", flag);
    EXPECT_EQ("clock_getcpuclockid_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : clock_getcpuclockid_0200
 * @tc.desc      : Verify could not get clock ID (pid parameter invalid)
 * @tc.level     : Level 2
 */
void clock_getcpuclockid_0200(void)
{
    bool flag = false;
    clockid_t clk = 0;
    int result = 0;
    result = clock_getcpuclockid(45000, &clk);
    if (result != 0) {
        flag = true;
    }
    EXPECT_TRUE("clock_getcpuclockid_0200", flag);
}

int main()
{
    clock_getcpuclockid_0100();
    clock_getcpuclockid_0200();
    return t_status;
}