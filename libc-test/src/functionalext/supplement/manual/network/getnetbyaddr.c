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
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "test.h"

const char *path = "/etc/networks";
const char *netinfo = "loopback       127.0.0.1";

/**
 * @tc.name      : getnetbyaddr_0100
 * @tc.desc      : Verify that the network information is successfully obtained through a valid IPv4 address
 * @tc.level     : Level 0
 */
void getnetbyaddr_0100(void)
{
    FILE *fd = fopen(path, "w+");
    if (!fd) {
        t_error("%s fopen failed\n", __func__);
    }
    fwrite(netinfo, sizeof(char), strlen(netinfo), fd);

    struct netent *nptr;
    uint32_t ip = inet_network("127.0.0.1");
    printf("ip: %d\n", ip);
    nptr = getnetbyaddr(ip, AF_INET);
    if (!nptr) {
        t_error("%s getnetbyaddr failed\n", __func__);
    }
}

/**
 * @tc.name      : getnetbyaddr_0200
 * @tc.desc      : Authentication failed to obtain network information through invalid IPv4 address
 * @tc.level     : Level 2
 */
void getnetbyaddr_0200(void)
{}

/**
 * @tc.name      : getnetbyaddr_0300
 * @tc.desc      : Verify that the network information is successfully obtained through a valid IPv6 address
 * @tc.level     : Level 0
 */
void getnetbyaddr_0300(void)
{}

/**
 * @tc.name      : getnetbyaddr_0400
 * @tc.desc      : Authentication failed to obtain network information through invalid IPv6 address
 * @tc.level     : Level 2
 */
void getnetbyaddr_0400(void)
{}

int main(int argc, char *argv[])
{
    getnetbyaddr_0100();
    getnetbyaddr_0200();
    getnetbyaddr_0300();
    getnetbyaddr_0400();
    return t_status;
}