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
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t SUCCESS = 0;
const int32_t FAILED = -1;

/**
 * @tc.name      : getdomainname_0100
 * @tc.desc      : Verify domain name (valid)
 * @tc.level     : Level 0
 */
void getdomainname_0100(void)
{
    int result;
    char name[1024] = "\0";
    result = getdomainname(name, 1024);
    EXPECT_EQ("getdomainname_0100", result, SUCCESS);
    EXPECT_TRUE("getdomainname_0100", strcmp(name, "\0") != 0);
}

/**
 * @tc.name      : getdomainname_0200
 * @tc.desc      : Verify domain name cannot be obtained (len parameter invalid, 0)
 * @tc.level     : Level 2
 */
void getdomainname_0200(void)
{
    int result;
    char name[1024] = "\0";
    result = getdomainname(name, 0);
    EXPECT_EQ("getdomainname_0200", result, FAILED);
}

/**
 * @tc.name      : getdomainname_0300
 * @tc.desc      : Failed to obtain domain name (len parameter invalid, less than domain name length)
 * @tc.level     : Level 2
 */
void getdomainname_0300(void)
{
    int result;
    char name[1024] = "\0";
    result = getdomainname(name, 1);
    EXPECT_EQ("getdomainname_0300", result, FAILED);
}

int main()
{
    getdomainname_0100();
    getdomainname_0200();
    getdomainname_0300();
    return t_status;
}