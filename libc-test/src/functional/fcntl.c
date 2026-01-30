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
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functionalext.h"

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))
#define TESTE(c) (errno=0, TEST(c, "errno = %s\n", strerror(errno)))
#ifdef MUSL_EXTERNAL_FUNCTION
#define TEST_FILE "test_fcntl64.tmp"
#define LARGE_OFFSET_FILE "large_file.tmp"
#define LARGE_FILE_SIZE (2LL * 1024 * 1024 * 1024)
#endif

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

#ifdef MUSL_EXTERNAL_FUNCTION
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
    EXPECT_EQ("fcntl64_0100", written, (ssize_t)strlen(test_data));
    // Use F_SETFL to set non blocking mode (ONONBLOCK)
    int new_flags = new_fd | O_NONBLOCK;
    int ret = fcntl64(new_fd, F_SETFL, new_flags);
    EXPECT_EQ("fcntl64_0100", ret, 0);
    // Verify the setting results
    int verify_flags = fcntl64(new_fd, F_GETFL);
    EXPECT_GTE("fcntl64_0100", verify_flags, 0);

    TEST(verify_flags & O_NONBLOCK);

    new_flags = verify_flags & ~O_NONBLOCK;
    ret = fcntl64(new_fd, F_SETFL, new_flags);
    EXPECT_EQ("fcntl64_0100", ret, 0);
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

/**
 * @tc.name      : fcntl64_0400
 * @tc.desc      : File descriptor copy test: F_DUPFD/F_DUPFD_COOEXEC
 * @tc.level     : Level 0
 */
void fcntl64_0400(void)
{
	int fd1 = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GTE("fcntl64_0400", fd1, 0);

    int fd2 = fcntl64(fd1, F_DUPFD, 10);
    EXPECT_GTE("fcntl64_0400", fd2, 0);

    const char *write_data = "fcntl64 dup test data";
    int ret = write(fd1, write_data, strlen(write_data));
    EXPECT_EQ("fcntl64_0400", ret, (ssize_t)strlen(write_data));

    lseek64(fd2, 0, SEEK_SET);
    char read_buf[128] = {0};
    ret = read(fd2, read_buf, sizeof(read_buf) - 1);
    EXPECT_EQ("fcntl64_0400", ret, (ssize_t)strlen(write_data));
    EXPECT_STREQ("fcntl64_0400", read_buf, write_data);

    int fd3 = fcntl64(fd1, F_DUPFD_CLOEXEC, 20);
    EXPECT_GTE("fcntl64_0400", fd3, 20);

    close(fd1);
    close(fd2);
    close(fd3);
    unlink(TEST_FILE);
}

/**
 * @tc.name      : fcntl64_0500
 * @tc.desc      : File lock test: F_SETLK/F_GETLK (suggestive lock)
 * @tc.level     : Level 0
 */
void fcntl64_0500(void)
{
	int fd = -1;
    struct flock64 lock = {0};
    // open test file
    fd = open64(TEST_FILE, O_RDWR | O_CREAT, 0644);
    EXPECT_GTE("fcntl64_0500", fd, 0);

    // Set exclusive lock (F_WRLCK): Lock the first 100 bytes of the file
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 100;
    lock.l_pid = getpid();
    int ret = fcntl64(fd, F_SETLK, &lock);
    EXPECT_EQ("fcntl64_0500", ret, 0);
    // Get lock status using F_GETLK
    struct flock64 get_lock = {0};
    get_lock.l_type = F_WRLCK;
    get_lock.l_whence = SEEK_SET;
    get_lock.l_start = 0;
    get_lock.l_len = 100;
    ret = fcntl64(fd, F_GETLK, &get_lock);
    EXPECT_EQ("fcntl64_0500", ret, 0);
    EXPECT_EQ("fcntl64_0500", get_lock.l_type, F_UNLCK);
    // Release lock (set F_UNLCK)
    lock.l_type = F_UNLCK;
    ret = fcntl64(fd, F_SETLK, &lock);
    EXPECT_EQ("fcntl64_0500", ret, 0);
    // Verify that the lock has been released
    ret = fcntl64(fd, F_GETLK, &get_lock);
    bool is_unlocked = (ret >= 0 && get_lock.l_type == F_UNLCK);
    EXPECT_GTE("fcntl64_0500", is_unlocked, 0);

    close(fd);
    unlink(TEST_FILE);
}

/**
 * @tc.name      : fcntl64_0600
 * @tc.desc      : Boundary scenario testing: invalid file descriptors, illegal commands, large offset locks
 * @tc.level     : Level 0
 */
void fcntl64_0600(void)
{
    // Invalid file descriptor (fd=-1) calls fcntl64
	struct flock64 lock = {0};
    int ret = fcntl64(-1, F_GETFL);
    EXPECT_EQ("fcntl64_0600", ret, -1);
    // Illegal command (custom invalid cmd=999)
    int valid_fd = open64(TEST_FILE, O_RDWR | O_CREAT, 0644);
    if (valid_fd >= 0) {
        ret = fcntl64(valid_fd, 999, NULL);
        EXPECT_EQ("fcntl64_0600", ret, -1);
        close(valid_fd);
        unlink(TEST_FILE);
    }

    // Large offset file lock (64 bit large offset, verify 64 bit support of fcntl64)
    valid_fd = open64(LARGE_OFFSET_FILE, O_RDWR | O_CREAT, 0644);
    if (valid_fd >= 0) {
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0x7FFFFFFFFFFFFFFFLL;
        lock.l_len = 1024;
        lock.l_pid = getpid();

        ret = fcntl64(valid_fd, F_SETLK, &lock);
        EXPECT_EQ("fcntl64_0600", ret, -1);
        EXPECT_EQ("fcntl64_0600", errno, EOVERFLOW);

        close(valid_fd);
        unlink(LARGE_OFFSET_FILE);
    }
}

#define NUM_THREADS 4
#define ITERATIONS_PER_THREAD 100

struct thread_arg {
    int fd;
    int thread_id;
    int *result;
};

static void *thread_func(void *arg) {
    struct thread_arg *targ = (struct thread_arg *)arg;
    int fd = targ->fd;
    int success_count = 0;

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        int flags = fcntl64(fd, F_GETFL);
        if (flags >= 0) {
            success_count++;
        }

        int new_flags = flags | O_NONBLOCK;
        if (fcntl64(fd, F_SETFL, new_flags) == 0) {
            success_count++;
        }

        new_flags = flags & ~O_NONBLOCK;
        if (fcntl64(fd, F_SETFL, new_flags) == 0) {
            success_count++;
        }

        usleep(10);
    }

    *(targ->result) = success_count;
    return NULL;
}

/**
 * @tc.name      : fcntl64_0700
 * @tc.desc      : Multi-thread concurrent fcntl64 operations test (thread safety)
 * @tc.level     : Level 1
 */
void fcntl64_0700(void)
{
    int fd = open64(TEST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GTE("fcntl64_0700", fd, 0);

    pthread_t threads[NUM_THREADS];
    int thread_results[NUM_THREADS] = {0};

    struct thread_arg args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].fd = fd;
        args[i].thread_id = i;
        args[i].result = &thread_results[i];

        if (pthread_create(&threads[i], NULL, thread_func, &args[i]) != 0) {
            t_error("fcntl64_0700: Failed to create thread %d\n", i);
            thread_results[i] = -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_results[i] != -1) {
            pthread_join(threads[i], NULL);
        }
    }

    int total_success = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_results[i] > 0) {
            total_success += thread_results[i];
        }
    }

    int expected_min_success = NUM_THREADS * ITERATIONS_PER_THREAD * 2;
    EXPECT_GTE("fcntl64_0700", total_success, expected_min_success);

    close(fd);
    unlink(TEST_FILE);
}
#endif

int main(void)
{
	fcntl_0100();
#ifdef MUSL_EXTERNAL_FUNCTION
	fcntl64_0100();
	fcntl64_0200();
	fcntl64_0300();
	fcntl64_0400();
    fcntl64_0500();
    fcntl64_0600();
    fcntl64_0700();
#endif
	return t_status;
}
