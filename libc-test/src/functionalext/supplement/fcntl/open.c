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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/*
 * @tc.name      : open_0100
 * @tc.desc      : Verify file open mode is O_RDONLY
 * @tc.level     : Level 0
 */
void open_0100(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_RDONLY + O_CREAT);
    if (fd == -1) {
        fputs("open r is failed\n", stdout);
    }
    int len = write(fd, s, sizeof(s));
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_0100", fd, 4);
    EXPECT_EQ("open_0100", len, -1);
    EXPECT_STREQ("open_0100", buffer, "");
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : open_0200
 * @tc.desc      : Verify file open mode is O_RDONLY (file does not exist)
 * @tc.level     : Level 2
 */
void open_0200(void)
{
    int fd;
    fd = open("/data/test.txt", O_RDONLY);
    EXPECT_EQ("open_0200", fd, -1);
    close(fd);
}

/*
 * @tc.name      : open_0300
 * @tc.desc      : VVerify file open mode is O_WRONLY
 * @tc.level     : Level 0
 */
void open_0300(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_WRONLY + O_CREAT);
    if (fd == -1) {
        fputs("open r is failed\n", stdout);
    }
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_0300", fd, 4);
    EXPECT_STREQ("open_0300", buffer, "");
    int len = write(fd, s, sizeof(s));
    EXPECT_EQ("open_0300", len, 5);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : open_0400
 * @tc.desc      : VVerify file open mode is O_WRONLY(file does not exist)
 * @tc.level     : Level 2
 */
void open_0400(void)
{
    int fd;
    fd = open("/data/test.txt", O_WRONLY);
    EXPECT_EQ("open_0400", fd, -1);
    close(fd);
}

/*
 * @tc.name      : open_0500
 * @tc.desc      : VVerify file open mode is O_RDWR | O_APPEND
 * @tc.level     : Level 0
 */
void open_0500(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_RDWR | O_APPEND + O_CREAT);
    if (fd == -1) {
        fputs("open r is failed\n", stdout);
    }
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_0500", fd, 4);
    EXPECT_STREQ("open_0500", buffer, "");
    int len = write(fd, s, sizeof(s));
    EXPECT_EQ("open_0500", len, 5);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : open_0600
 * @tc.desc      : VVerify file open mode is O_RDWR | O_APPEND(file does not exist)
 * @tc.level     : Level 1
 */
void open_0600(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_RDWR | O_APPEND);
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_0600", fd, -1);
    EXPECT_STREQ("open_0600", buffer, "");
    int len = write(fd, s, sizeof(s));
    EXPECT_EQ("open_0600", len, -1);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : open_0700
 * @tc.desc      : VVerify file open mode is O_RDWR | O_APPEND(file pathname is invalid)
 * @tc.level     : Level 2
 */
void open_0700(void)
{
    int fd;
    fd = open("/data/test.txt", O_RDWR | O_APPEND);
    EXPECT_EQ("open_0700", fd, -1);
    close(fd);
}

/*
 * @tc.name      : open_0800
 * @tc.desc      : VVerify file open mode is O_RDWR | O_CREAT
 * @tc.level     : Level 0
 */
void open_0800(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        fputs("open r is failed\n", stdout);
    }
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_0800", fd, 4);
    EXPECT_STREQ("open_0800", buffer, "");
    int len = write(fd, s, sizeof(s));
    EXPECT_EQ("open_0800", len, 5);
    close(fd);
    remove("/data/test.txt");
}

/*
 * @tc.name      : open_0900
 * @tc.desc      : VVerify file open mode is O_RDWR | O_CLOEXEC
 * @tc.level     : Level 0
 */
void open_0900(void)
{
    int fd, size;
    char s[] = "test";
    char buffer[80] = {0};
    fd = open("/data/test.txt", O_RDWR | O_CLOEXEC + O_CREAT);
    if (fd == -1) {
        fputs("open r is failed\n", stdout);
    }
    size = read(fd, buffer, sizeof(buffer));
    EXPECT_EQ("open_1000", fd, 4);
    EXPECT_STREQ("open_1000", buffer, "");
    int len = write(fd, s, sizeof(s));
    EXPECT_EQ("open_1000", len, 5);
    close(fd);
    remove("/data/test.txt");
}

TEST_FUN G_Fun_Array[] = {
    open_0100,
    open_0200,
    open_0300,
    open_0400,
    open_0500,
    open_0600,
    open_0700,
    open_0800,
    open_0900,

};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}