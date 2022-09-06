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

#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const int32_t SPRINTF_0100_RESULT = 4;
const int32_t SPRINTF_0200_RESULT = 5;
const int32_t SPRINTF_0300_RESULT = 19;
const int32_t SECOND_PARAM = 2;
const int32_t THIRD_PARAM = 12345;

/**
 * @tc.name      : sprintf_0100
 * @tc.desc      : Verify sprintf process success with two params
 * @tc.level     : Level 0
 */
void sprintf_0100(void)
{
    char str[1024];
    int ret = sprintf(str, "test");
    EXPECT_EQ("sprintf_0100", ret, SPRINTF_0100_RESULT);
    EXPECT_STREQ("sprintf_0100", str, "test");
}

/**
 * @tc.name      : sprintf_0200
 * @tc.desc      : Verify sprintf process success with three params(the thrid praram type is int_32)
 * @tc.level     : Level 0
 */
void sprintf_0200(void)
{
    char str[1024];
    int ret = sprintf(str, "test%d", SECOND_PARAM);
    EXPECT_EQ("sprintf_0200", ret, SPRINTF_0200_RESULT);
    EXPECT_STREQ("sprintf_0200", str, "test2");
}

/**
 * @tc.name      : sprintf_0300
 * @tc.desc      : Verify sprintf process success with three params(the thrid praram type is string)
 * @tc.level     : Level 0
 */
void sprintf_0300(void)
{
    char str[1024];
    int ret = sprintf(str, "testname:%s%d", "test3", THIRD_PARAM);
    EXPECT_EQ("sprintf_0300", ret, SPRINTF_0300_RESULT);
    EXPECT_STREQ("sprintf_0300", str, "testname:test312345");
}

int main(void)
{
    sprintf_0100();
    sprintf_0200();
    sprintf_0300();
    return t_status;
}
