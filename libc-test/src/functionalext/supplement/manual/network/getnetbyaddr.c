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
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : getnetbyaddr_0100
 * @tc.desc      : Verify that the network information is successfully obtained through a valid IPv4 address
 * @tc.level     : Level 0
 */
void getnetbyaddr_0100(void)
{
    bool flag = false;
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    struct in_addr *addr;
    struct sockaddr_in saddr;
    inet_aton("127.0.0.1", &saddr.sin_addr);
    hptr = gethostbyaddr((void *)&saddr.sin_addr, 4, AF_INET);
    if (hptr != NULL) {
        flag = true;
    }
    EXPECT_STREQ("getnetbyaddr_0100", hptr->h_name, "127.0.0.1");
    EXPECT_TRUE("getnetbyaddr_0100", flag);
}

/**
 * @tc.name      : getnetbyaddr_0200
 * @tc.desc      : Authentication failed to obtain network information through invalid IPv4 address
 * @tc.level     : Level 2
 */
void getnetbyaddr_0200(void)
{
    bool flag = false;
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    struct in_addr *addr;
    struct sockaddr_in saddr;
    inet_aton("-128.256.w.1", &saddr.sin_addr);
    hptr = gethostbyaddr((void *)&saddr.sin_addr, 0, AF_INET);
    if (hptr == NULL) {
        flag = true;
    }
    EXPECT_TRUE("getnetbyaddr_0200", flag);
}

/**
 * @tc.name      : getnetbyaddr_0300
 * @tc.desc      : Verify that the network information is successfully obtained through a valid IPv6 address
 * @tc.level     : Level 0
 */
void getnetbyaddr_0300(void)
{
    bool flag = false;
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    struct in_addr *addr;
    struct sockaddr_in saddr;
    inet_aton("::1", &saddr.sin_addr);
    hptr = gethostbyaddr((void *)&saddr.sin_addr, 16, AF_INET6);
    if (hptr != NULL) {
        flag = true;
    }
    EXPECT_TRUE("getnetbyaddr_0300", flag);
}

/**
 * @tc.name      : getnetbyaddr_0400
 * @tc.desc      : Authentication failed to obtain network information through invalid IPv6 address
 * @tc.level     : Level 2
 */
void getnetbyaddr_0400(void)
{
    bool flag = false;
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    struct in_addr *addr;
    struct sockaddr_in saddr;
    inet_aton("0:-127.w.-256.1", &saddr.sin_addr);
    hptr = gethostbyaddr((void *)&saddr.sin_addr, 4, AF_INET6);
    if (hptr == NULL) {
        flag = true;
    }
    EXPECT_TRUE("getnetbyaddr_0400", flag);
}

TEST_FUN G_Fun_Array[] = {
    getnetbyaddr_0100,
    getnetbyaddr_0200,
    getnetbyaddr_0300,
    getnetbyaddr_0400,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}