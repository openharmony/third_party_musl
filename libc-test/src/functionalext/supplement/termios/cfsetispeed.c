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

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : cfgetispeed_0100
 * @tc.desc      : Verify information that can set the input baud rate (parameter valid).
 * @tc.level     : Level 0
 */
void cfgetispeed_0100(void)
{
    struct termios termios_p;
    tcgetattr(STDIN_FILENO, &termios_p);
    speed_t speed = B50;
    int result = cfsetispeed(&termios_p, speed);
    EXPECT_EQ("cfgetispeed_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : cfgetispeed_0200
 * @tc.desc      : Verify that the input baud rate cannot be set (the speed parameter is invalid).
 * @tc.level     : Level 2
 */
void cfgetispeed_0200(void)
{
    struct termios termios_p;
    tcgetattr(STDIN_FILENO, &termios_p);
    speed_t speed = -2;
    int result = cfsetispeed(&termios_p, speed);
    EXPECT_EQ("cfgetispeed_0200", result, COUNT_NEGATIVE);
}

int main()
{
    cfgetispeed_0100();
    cfgetispeed_0200();
    return t_status;
}