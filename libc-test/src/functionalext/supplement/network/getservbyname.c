/**
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

#include <netdb.h>
#include "functionalext.h"
#define FTP_PORT 21
#define HTTP_PORT 80

/**
 * @tc.name      : getservbyname_0100
 * @tc.desc      : Get service entry
 * @tc.level     : Level 0
 */
void getservbyname_0100(void)
{
    struct servent *se = getservbyname("smtp", "tcp");
    if (se) {
        EXPECT_STREQ("getservbyname_0100", se->s_proto, "tcp");
    }

    se = getservbyname("echo", "udp");
    if (se) {
        EXPECT_STREQ("getservbyname_0100", se->s_proto, "udp");
    }
}

/**
 * @tc.name      : getservbyname_0200
 * @tc.desc      : Get the service information of the specified port
 * @tc.level     : Level 0
 */
void getservbyname_0200(void)
{
    struct servent *se = getservbyname("name", "proto");
    EXPECT_PTREQ("getservbyname_0200", se, NULL);
}

/**
 * @tc.name      : getservbyname_0300
 * @tc.desc      : Get the service information of the specified port
 * @tc.level     : Level 0
*/
void getservbyname_0300(void)
{
    struct servent *se = getservbyname("ftp", "tcp");
    EXPECT_NE("getservbyname_0300", se, NULL);
    EXPECT_EQ("getservbyname_0300", ntohs(se->s_port), FTP_PORT);
}

/**
 * @tc.name      : getservbyname_0400
 * @tc.desc      : Get the service information of the specified port
 * @tc.level     : Level 0
*/
void getservbyname_0400(void)
{
    struct servent *se = getservbyname("http", "tcp");
    EXPECT_NE("getservbyname_0400", se, NULL);
    EXPECT_EQ("getservbyname_0400", ntohs(se->s_port), HTTP_PORT);
}

int main(void)
{
    getservbyname_0100();
    getservbyname_0200();
    getservbyname_0300();
    getservbyname_0400();
    return t_status;
}