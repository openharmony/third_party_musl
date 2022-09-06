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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include "functionalext.h"

/**
 * @tc.name      : cfgetispeed_0100
 * @tc.desc      : Verify that information about the output baud rate is obtained.
 * @tc.level     : Level 0
 */
void cfgetispeed_0100(void)
{
    struct termios termios_p;
    tcgetattr(STDIN_FILENO, &termios_p);
    speed_t result = cfgetospeed(&termios_p);
    EXPECT_TRUE("cfgetispeed_0100", result >= 0);
}

int main()
{
    cfgetispeed_0100();
    return t_status;
}