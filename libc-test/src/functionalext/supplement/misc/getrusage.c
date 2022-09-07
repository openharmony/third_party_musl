/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <sys/resource.h>
#include <string.h>
#include <errno.h>
#include "functionalext.h"

int success = 0;
int fail = -1;

/**
 * @tc.name      : getrusage_0100
 * @tc.desc      : Verify the program running time (parameter is RUSAGE_SELF)
 * @tc.level     : Level 0
 */
void getrusage_0100(void)
{
    struct rusage usage;
    int result = getrusage(RUSAGE_SELF, &usage);
    EXPECT_EQ("getrusage_0100", result, success);
}

/**
 * @tc.name      : getrusage_0200
 * @tc.desc      : Verify the program running time (parameter is RUSAGE_CHILDREN)
 * @tc.level     : Level 1
 */
void getrusage_0200(void)
{
    struct rusage usage;
    int result = getrusage(RUSAGE_CHILDREN, &usage);
    EXPECT_EQ("getrusage_0200", result, success);
}

/**
 * @tc.name      : getrusage_0300
 * @tc.desc      : Verify the program running time (parameter is RUSAGE_THREAD)
 * @tc.level     : Level 1
 */
void getrusage_0300(void)
{
    struct rusage usage;
    int result = getrusage(RUSAGE_THREAD, &usage);
    EXPECT_EQ("getrusage_0300", result, success);
}

/**
 * @tc.name      : getrusage_0400
 * @tc.desc      : Verify that the program running time cannot be obtained (parameter is 100)
 * @tc.level     : Level 2
 */
void getrusage_0400(void)
{
    struct rusage usage;
    int result = getrusage(100, &usage);
    EXPECT_EQ("getrusage_0400", result, fail);
}

/**
 * @tc.name      : getrusage_0500
 * @tc.desc      : Verify that the program running time cannot be obtained (parameter is RUSAGE_SELF)
 * @tc.level     : Level 2
 */
void getrusage_0500(void)
{
    struct rusage usage;
    int result = getrusage(RUSAGE_SELF, NULL);
    EXPECT_EQ("getrusage_0500", result, fail);
}

int main(void)
{
    getrusage_0100();
    getrusage_0200();
    getrusage_0300();
    getrusage_0400();
    getrusage_0500();
    return t_status;
}