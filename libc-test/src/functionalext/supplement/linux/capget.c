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
#include <stdio.h>
#include <netinet/in.h>
#include <sys/capability.h>
#include <sys/socket.h>
#include "functionalext.h"

#undef _POSIX_SOURCE
const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : capget_0100
 * @tc.desc      : Verify that you can obtain process permissions (parameter valid)
 * @tc.level     : Level 0
 */
void capget_0100(void)
{
    struct __user_cap_header_struct cap_header_data;
    cap_user_header_t cap_header = &cap_header_data;
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;
    int result;
    cap_header->pid = getpid();
    cap_header->version = _LINUX_CAPABILITY_VERSION_1;
    result = capget(cap_header, cap_data);
    EXPECT_EQ("capget_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : capget_0200
 * @tc.desc      : Validation could not obtain process permissions (parameter a is invalid)
 * @tc.level     : Level 2
 */
void capget_0200(void)
{
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;
    int result;
    result = capget(NULL, cap_data);
    EXPECT_EQ("capget_0200", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : capget_0300
 * @tc.desc      : Verify that the process permission can be obtained
 * @tc.level     : Level 1
 */
void capget_0300(void)
{
    struct __user_cap_header_struct cap_header_data;
    cap_user_header_t cap_header = &cap_header_data;
    cap_header->pid = getpid();
    cap_header->version = _LINUX_CAPABILITY_VERSION_1;
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = NULL;
    int result;
    result = capget(cap_header, NULL);
    EXPECT_EQ("capget_0300", result, COUNT_ZERO);
}

int main()
{
    capget_0100();
    capget_0200();
    capget_0300();
    return t_status;
}