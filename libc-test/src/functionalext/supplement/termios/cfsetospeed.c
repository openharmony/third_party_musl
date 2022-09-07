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
#include <stdint.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : cfsetospeed_0100
 * @tc.desc      : Verify information that can set output baud rate (parameter valid).
 * @tc.level     : Level 0
 */
void cfsetospeed_0100(void)
{
    int result;
    struct termios buff;
    tcgetattr(STDIN_FILENO, &buff);
    result = cfsetospeed(&buff, B0);
    EXPECT_EQ("cfsetospeed_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : cfsetospeed_0200
 * @tc.desc      : Verify that the output baud rate cannot be set (the speed parameter is invalid).
 * @tc.level     : Level 2
 */
void cfsetospeed_0200(void)
{
    int result;
    struct termios buff;
    tcgetattr(STDIN_FILENO, &buff);
    result = cfsetospeed(&buff, -10);
    EXPECT_EQ("cfsetospeed_0200", result, COUNT_NEGATIVE);
}

int main()
{
    cfsetospeed_0100();
    cfsetospeed_0200();
    return t_status;
}