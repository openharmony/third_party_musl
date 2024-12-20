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

#include <termios.h>
#include "test.h"

/**
 * @tc.name      : cfgetispeed_0100
 * @tc.desc      : Verify that information about the input baud rate is obtained.
 * @tc.level     : Level 0
 */
void cfgetispeed_0100(void)
{
    struct termios t = {};
    int result = cfsetispeed(&t, B1200);
    if (result != 0) {
        t_error("%s cfsetispeed failed\n", __func__);
    }

    speed_t ret = cfgetispeed(&t);
    if (ret != (speed_t)(B1200)) {
        t_error("%s cfgetispeed failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    cfgetispeed_0100();
    return t_status;
}