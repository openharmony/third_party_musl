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
#include <stdbool.h>
#include "functionalext.h"

/**
 * @tc.name      : getloadavg_0100
 * @tc.desc      : Verify the average number of processes in the system running queue in different time periods
 * @tc.level     : Level 0
 */
void getloadavg_0100(void)
{
    bool flag = false;
    double buf[1024];
    int result = -10;
    result = getloadavg(buf, 1024);
    if (result > 0)
    {
        flag = true;
    }
    EXPECT_TRUE("getloadavg_0100", flag);
}

int main()
{
    getloadavg_0100();
    return t_status;
}