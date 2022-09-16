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

#include <ifaddrs.h>
#include <stdbool.h>
#include "functionalext.h"

/**
 * @tc.name      : freeifaddrs_0100
 * @tc.desc      : Verifies that the specified memory can be freed (parameter valid)
 * @tc.level     : Level 0
 */
void freeifaddrs_0100(void)
{
    struct ifaddrs *ifaddr;
    int ret = getifaddrs(&ifaddr);
    ifaddr->ifa_next = NULL;
    freeifaddrs(ifaddr);
    ifaddr = NULL;
    bool flag = false;
    if (ifaddr == NULL) {
        flag = true;
    }
    EXPECT_TRUE("freeifaddrs_0100", flag);
}

int main(int argc, char *argv[])
{
    freeifaddrs_0100();
    return t_status;
}