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
 * @tc.name      : capset_0100
 * @tc.desc      : Verify that process permissions can be set (parameter valid)
 * @tc.level     : Level 0
 */
void capset_0100(void)
{
    struct __user_cap_header_struct cap_header_data;
    cap_user_header_t cap_header = &cap_header_data;
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;
    int result;
    cap_header->pid = getpid();
    cap_header->version = _LINUX_CAPABILITY_VERSION_1;
    result = capset(cap_header, cap_data);
#ifdef __POSIX_SOURCE
    EXPECT_EQ("capset_0100", result, COUNT_ZERO);
#endif
}

/**
 * @tc.name      : capset_0200
 * @tc.desc      : Validation cannot set process permissions (a parameter is invalid)
 * @tc.level     : Level 2
 */
void capset_0200(void)
{
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;
    int result;
    result = capset(NULL, cap_data);
    EXPECT_EQ("capset_0200", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : capset_0300
 * @tc.desc      : Validation cannot set process permissions (b parameter invalid)
 * @tc.level     : Level 2
 */
void capset_0300(void)
{
    struct __user_cap_header_struct cap_header_data;
    cap_user_header_t cap_header = &cap_header_data;
    cap_header->pid = getpid();
    cap_header->version = _LINUX_CAPABILITY_VERSION_1;
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = NULL;
    int result;
    result = capset(cap_header, NULL);
    EXPECT_EQ("capset_0300", result, COUNT_NEGATIVE);
}

int main(int argc, char *argv[])
{
    capset_0100();
    capset_0200();
    capset_0300();
    return t_status;
}