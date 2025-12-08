/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "test.h"
#include <functionalext.h>

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))
#define TESTE(c) (errno=0, TEST(c, "errno = %s\n", strerror(errno)))
#define TEST_FILE "test_fcntl64.tmp"
#define LARGE_FILE_SIZE (2LL * 1024 * 1024 * 1024) // 2GB

/**
 * @tc.name      : fcntl_0100
 * @tc.desc      : Test basic functionality of fcntl
 * @tc.level     : Level 0
 */
void fcntl_0100(void)
{
    struct flock fl = {0};
    FILE *f;
    int fd;
    pid_t pid;
    int status;

    if (!TESTE(f = fopen("tempory_testfcntl.txt", "w+"))) {
        return;
    }
    fd = fileno(f);

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    TESTE(fcntl(fd, F_SETLK, &fl) == 0);

    pid = fork();
    if (!pid) {
        fl.l_type = F_RDLCK;
        _exit(fcntl(fd, F_SETLK, &fl) == 0 ||
              (errno != EAGAIN && errno != EACCES));
    }
    while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
        ;
    TEST(status == 0, "lock failed to work\n");

    pid = fork();
    if (!pid) {
        fl.l_type = F_WRLCK;
        _exit(fcntl(fd, F_GETLK, &fl) || fl.l_pid != getppid());
    }
    while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
        ;
    TEST(status == 0, "child failed to detect lock held by parent\n");

    fclose(f);
}

/**
 * @tc.name      : fcntl64_0100
 * @tc.desc      : Test basic functionality  of fcntl64
 * @tc.level     : Level 0
 */
void fcntl64_0100(void)
{
	int fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GTE("fcntl64_0100", fd, 0);

	// minimum descriptor
    int new_fd = fcntl64(fd, F_DUPFD, 10);
    EXPECT_GTE("fcntl64_0100", 10, new_fd);
	//  Verify that the replicated descriptor can be used normally
    const char *test_data = "test data";
    ssize_t written = write(new_fd, test_data, strlen(test_data));
    EXPECT_EQ("fcntl64_0100", written, strlen(test_data));

	close(new_fd);
    close(fd);
    unlink(TEST_FILE);
}

/**
 * @tc.name      : fcntl64_0200
 * @tc.desc      : Get/Set File Status Flag (F_GETFL/F.SETFL)
 * @tc.level     : Level 0
 */
void fcntl64_0200(void)
{
    int fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GTE("fcntl64_0200", fd,0);

    int flags = fcntl64(fd, F_GETFL);
    EXPECT_GTE("fcntl64_0200", flags, 0);

    TEST(flags & O_RDWR);

    int ret = fcntl64(fd, F_SETFL, flags | O_NONBLOCK);
    EXPECT_EQ("fcntl64_0200", 0,ret);

    flags = fcntl64(fd, F_GETFL);
    TEST(flags & O_NONBLOCK);

    ret = fcntl64(fd, F_SETFL, flags & ~O_NONBLOCK);
    EXPECT_EQ("fcntl64_0200", 0,ret);

    close(fd);
    unlink(TEST_FILE);
}

/**
 * @tc.name      : fcntl64_0300
 * @tc.desc      : Retrieve/set file descriptor flags (F_GETFD/F.SETFD)
 * @tc.level     : Level 0
 */
void fcntl64_0300(void)
{
	int fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GTE("fcntl64_0300", fd, 0);

    int fd_flags = fcntl64(fd, F_GETFD);
    EXPECT_GTE("fcntl64_0300", fd_flags, 0);

    int ret = fcntl64(fd, F_SETFD, fd_flags | FD_CLOEXEC);
    EXPECT_EQ("fcntl64_0300", 0, ret);

    fd_flags = fcntl64(fd, F_GETFD);
    TEST(fd_flags & FD_CLOEXEC);

    close(fd);
    unlink(TEST_FILE);
}

int main(void)
{
	fcntl_0100();
	fcntl64_0100();
	fcntl64_0200();
	fcntl64_0300();
	return t_status;
}
