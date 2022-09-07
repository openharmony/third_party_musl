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
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : cfmakeraw_0100
 * @tc.desc      : Verify that the terminal is set to raw mode.
 * @tc.level     : Level 0
 */
void cfmakeraw_0100(void)
{
    static struct termios tin;
    static struct termios tlocal;
    memcpy(&tlocal, &tin, sizeof(tin));
    cfmakeraw(&tlocal);
    tcsetattr(STDIN_FILENO, TCSANOW, &tlocal);
    tcgetattr(STDIN_FILENO, &tin);
    EXPECT_TRUE("cfmakeraw_0100", tin.c_cflag >= 0);
    EXPECT_EQ("cfmakeraw_0100", tin.c_iflag, 0);
    EXPECT_EQ("cfmakeraw_0100", tin.c_oflag, 0);
    EXPECT_EQ("cfmakeraw_0100", tin.c_cc[VTIME], 0);
    EXPECT_EQ("cfmakeraw_0100", tin.c_iflag, 0);
}

int main()
{
    cfmakeraw_0100();
    return t_status;
}