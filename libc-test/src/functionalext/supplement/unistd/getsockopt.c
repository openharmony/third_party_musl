/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include "functionalext.h"

#define IPPROTO_IP 0
#define IP_HDRINCL 3
#define IP_TTL 2
#define TCP_MAXSEG 2
#define TCP_NODELAY 1
#define IP_TOS 1
#define IP_OPTIONS 4


/*
 * @tc.name      : getsockopt_0100
 * @tc.desc      : Verify that the parameters are SO_DEBUG, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0100(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_DEBUG,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0100",content,0);
}

/*
 * @tc.name      : getsockopt_0200
 * @tc.desc      : Verify that the parameters are SO_REUSEADDR, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0200(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0200",content,0);

}

/*
 * @tc.name      : getsockopt_0300
 * @tc.desc      : Verify that the parameters are SO_TYPE, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0300(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_TYPE,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0300",content,0);

}

/*
 * @tc.name      : getsockopt_0400
 * @tc.desc      : Verify that the parameters are SO_ERROR, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0400(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0400",content,0);

}

/*
 * @tc.name      : getsockopt_0500
 * @tc.desc      : Verify that the parameters are SO_DONTROUTE, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0500(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_DONTROUTE,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0500",content,0);

}

/*
 * @tc.name      : getsockopt_0600
 * @tc.desc      : Verify that the parameters are SO_BROADCAST, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0600(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0600",content,0);
}

/*
 * @tc.name      : getsockopt_0700
 * @tc.desc      : Verify that the parameters are SO_SNDBUF, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0700(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0700",content,0);

}

/*
 * @tc.name      : getsockopt_0800
 * @tc.desc      : Verify that the parameters are SO_RCVBUF, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0800(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0800",content,0);
}

/*
 * @tc.name      : getsockopt_0900
 * @tc.desc      : Verify that the parameters are SO_KEEPALIVE, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_0900(void)
{
     int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_0900",content,0);
}

/*
 * @tc.name      : getsockopt_1000
 * @tc.desc      : Verify that the parameters are SO_OOBINLINE, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1000(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_OOBINLINE,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1000",content,0);

}

/*
 * @tc.name      : getsockopt_1100
 * @tc.desc      : Verify that the parameters are SO_LINGER, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1100(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_LINGER,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1100",content,0);

}

/*
 * @tc.name      : getsockopt_1200
 * @tc.desc      : Verify that the parameters are SO_RCVLOWAT, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1200(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_RCVLOWAT,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1200",content,0);

}

/*
 * @tc.name      : getsockopt_1300
 * @tc.desc      : Verify that the parameters are SO_SNDLOWAT, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1300(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_SNDLOWAT,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1300",content,0);

}

/*
 * @tc.name      : getsockopt_1400
 * @tc.desc      : Verify that the parameters are SO_RCVTIMEO, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1400(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1400",content,0);

}

/*
 * @tc.name      : getsockopt_1500
 * @tc.desc      : Verify that the parameters are SO_SNDTIMEO, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1500(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1500",content,0);

}

/*
 * @tc.name      : getsockopt_1600
 * @tc.desc      : Verify that the parameters are SO_TIMESTAMP, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1600(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_TIMESTAMP,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1600",content,0);

}

/*
 * @tc.name      : getsockopt_1700
 * @tc.desc      : Verify that the parameters are SO_TIMESTAMPNS, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1700(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_TIMESTAMPNS,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1700",content,0);

}

/*
 * @tc.name      : getsockopt_1800
 * @tc.desc      : Verify that the parameters are IP_HDRINCL, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1800(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1800",content,0);
}

/*
 * @tc.name      : getsockopt_1900
 * @tc.desc      : Verify that the parameters are IP_OPTIONS, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_1900(void)
{
   int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,IP_OPTIONS,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_1900",content,0);

}

/*
 * @tc.name      : getsockopt_2000
 * @tc.desc      : Verify that the parameters are IP_TOS, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2000(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,IP_TOS,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_2000",content,0);

}

/*
 * @tc.name      : getsockopt_2100
 * @tc.desc      : Verify that the parameters are IP_TTL, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2100(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,IP_TTL,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_2100",content,0);


}

/*
 * @tc.name      : getsockopt_2200
 * @tc.desc      : Verify that the parameters are TCP_MAXSEG, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2200(void)
{
   int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,TCP_MAXSEG,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_2200",content,0);

}

/*
 * @tc.name      : getsockopt_2300
 * @tc.desc      : Verify that the parameters are TCP_NODELAY, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2300(void)
{
    int optval;
    int optlen = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,IPPROTO_IP,TCP_NODELAY,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_2300",content,0);

}

/*
 * @tc.name      : getsockopt_2400
 * @tc.desc      : Verify that the parameters are SO_RCVBUF, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2400(void)
{
    int optval;
    int optlen = 0;
    int content = getsockopt(-1,SOL_SOCKET,SO_RCVBUF,&optval,(socklen_t *)(&optlen));
    EXPECT_EQ("getsockopt_2400",content,-1);

}

/*
 * @tc.name      : getsockopt_2500
 * @tc.desc      : Verify that the parameters are SO_DEBUG, get the file status successfully
 * @tc.level     : Level 0
 */
void getsockopt_2500(void)
{
     int optval;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int content = getsockopt(sockfd,SOL_SOCKET,SO_DEBUG,&optval,NULL);
    EXPECT_EQ("getsockopt_2500",content,-1);

}


int main()
{
    getsockopt_0100();
    getsockopt_0200();
    getsockopt_0300();
    getsockopt_0400();
    getsockopt_0500();
    getsockopt_0600();
    getsockopt_0700();
    getsockopt_0800();
    getsockopt_0900();
    getsockopt_1000();
    getsockopt_1100();
    getsockopt_1200();
    getsockopt_1300();
    getsockopt_1400();
    getsockopt_1500();
    getsockopt_1600();
    getsockopt_1700();
    getsockopt_1800();
    getsockopt_1900();
    getsockopt_2000();
    getsockopt_2100();
    getsockopt_2200();
    getsockopt_2300();
    getsockopt_2400();
    getsockopt_2500();

    return t_status;
}