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

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : getnetbyname_0100
 * @tc.desc      : Verify the valid domain name to obtain network information successfully
 * @tc.level     : Level 0
 */
void getnetbyname_0100(void)
{
    struct netent *Socket = NULL;
    Socket = getnetbyname("127.0.0.1");
    if (!Socket) {
        t_error("%s getnetbyname failed\n", __func__);
    }
}

/**
 * @tc.name      : getnetbyname_0200
 * @tc.desc      : Failed to verify invalid domain name to obtain network information.
 * @tc.level     : Level 2
 */
void getnetbyname_0200(void)
{
    struct netent *Socket = NULL;
    Socket = getnetbyname("1ww.baidu.com");
    bool flag = false;
    if (Socket == NULL) {
        flag = true;
    }
    EXPECT_TRUE("getnetbyname_0200", flag);
}

TEST_FUN G_Fun_Array[] = {
    getnetbyname_0100,
    getnetbyname_0200,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}