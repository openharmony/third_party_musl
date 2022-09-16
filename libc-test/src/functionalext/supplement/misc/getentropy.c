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

#include <unistd.h>
#include <sys/random.h>
#include <pthread.h>
#include <errno.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;

/*
 * @tc.name      : getentropy_0100
 * @tc.desc      : Verify that secure random numbers can be generated
 * @tc.level     : Level 0
 */
void getentropy_0100(void)
{
    void *buffer[256];
    int result = getentropy(buffer, 256);
    EXPECT_EQ("getentrcopy_0100", result, SUCCESS);
}

/*
 * @tc.name      : getentropy_0200
 * @tc.desc      : Validation cannot generate secure random numbers
 * @tc.level     : Level 2
 */
void getentropy_0200(void)
{
    void *buffer[256];
    int result = getentropy(buffer, 257);
    EXPECT_EQ("getentrcopy_0200", result, FAILED);
}

int main(int argc, char *argv[])
{
    getentropy_0100();
    getentropy_0200();
    return t_status;
}