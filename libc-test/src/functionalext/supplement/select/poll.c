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

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_FAILED = -1;

/*
 * @tc.name      : poll_0100
 * @tc.desc      : Verify the attribute changes of the monitored file descriptor (timeout parameter is greater than 0)
 * @tc.level     : Level 0
 */
void poll_0100(void)
{
    int fd, ret;
    unsigned char key_val;
    struct pollfd fds[1];
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd < 0) {
        fputs("open r is failed\n", stdout);
    }
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    ret = poll(fds, 1, 2000);
    bool flag = false;
    if (ret >= 0) {
        flag = true;
    }
    EXPECT_TRUE("poll_0100", flag);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : poll_0200
 * @tc.desc      : Verify the attribute changes of the monitored file descriptor (timeout parameter is equal to 0)
 * @tc.level     : Level 1
 */
void poll_0200(void)
{
    int fd, ret;
    unsigned char key_val;
    struct pollfd fds[1];
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd < 0) {
        fputs("open r is failed\n", stdout);
    }
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    ret = poll(fds, 1, 0);
    bool flag = false;
    if (ret >= 0) {
        flag = true;
    }
    EXPECT_TRUE("poll_0200", flag);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : poll_0300
 * @tc.desc      : Verify the attribute changes of the monitored file descriptor (timeout parameter is less than 0)
 * @tc.level     : Level 1
 */
void poll_0300(void)
{
    int fd, ret;
    unsigned char key_val;
    struct pollfd fds[1];
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd < 0) {
        fputs("open r is failed\n", stdout);
    }
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    ret = poll(fds, 1, -1);
    bool flag = false;
    if (ret > 0) {
        flag = true;
    }
    EXPECT_TRUE("poll_0300", flag);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : poll_0400
 * @tc.desc      : Verify that the attribute changes of the monitored file descriptor (fds parameter NULL)
 * @tc.level     : Level 2
 */
void poll_0400(void)
{
    int fd, ret;
    unsigned char key_val;
    struct pollfd fds[1];
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd < 0) {
        fputs("open r is failed\n", stdout);
    }
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    ret = poll(NULL, 1, 0);
    EXPECT_EQ("poll_0400", ret, COUNT_FAILED);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : poll_0500
 * @tc.desc      : Validate the attribute changes of the monitored file descriptor (n parameter -1)
 * @tc.level     : Level 2
 */
void poll_0500(void)
{
    int fd, ret;
    unsigned char key_val;
    struct pollfd fds[1];
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd < 0) {
        fputs("open r is failed\n", stdout);
    }
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    ret = poll(fds, -1, 2000);
    EXPECT_EQ("poll_0500", ret, COUNT_FAILED);
    close(fd);
    remove("/data/test.txt");
}

TEST_FUN G_Fun_Array[] = {
    poll_0100,
    poll_0200,
    poll_0300,
    poll_0400,
    poll_0500,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}