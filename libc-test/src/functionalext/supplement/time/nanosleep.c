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
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int COUNT_ZERO = 0;

/*
 * @tc.name      : nanosleep_0100
 * @tc.desc      : Verify that the execution of the program can be paused
 * @tc.level     : Level 0
 */
void nanosleep_0100(void)
{
    struct timespec n_sleep;
    n_sleep.tv_sec = 0;
    n_sleep.tv_nsec = 5e8L;
    int data;
    data = nanosleep(&n_sleep, NULL);
    EXPECT_EQ("nanosleep_0100", data, COUNT_ZERO);
}

TEST_FUN G_Fun_Array[] = {
    nanosleep_0100,

};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}