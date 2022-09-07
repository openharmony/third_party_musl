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
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "fortify_test.h"
#include "test.h"

#define PPOLL_TIMESPEC_NSEC (100)

/**
 * @tc.name     : poll_0010
 * @tc.desc     : test poll normal condition
 * @tc.level    : Level 0
 */
static void poll_0010(void)
{
    errno = 0;
    TEST(poll(NULL, 0, 1) == 0);
    TEST(errno == 0);
    return;
}

/**
 * @tc.name     : poll_0020
 * @tc.desc     : test poll suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void poll_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    nfds_t fd_count = atoi("2");
    struct pollfd buf[1] = {{0, POLLIN, 0}};

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            // Set timeout to 0 to prevent waiting for polling if hardening tests fail.
            poll(buf, fd_count, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }

    return;
}

#ifdef _GNU_SOURCE
/**
 * @tc.name     : ppoll_0010
 * @tc.desc     : test ppoll normal condition
 * @tc.level    : Level 0
 */
static void ppoll_0010(void)
{
    errno = 0;
    struct timespec ts = { .tv_nsec = PPOLL_TIMESPEC_NSEC };
    TEST(ppoll(NULL, 0, &ts, NULL) == 0);
    TEST(errno == 0);
    return;
}

/**
 * @tc.name     : ppoll_0020
 * @tc.desc     : test ppoll suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void ppoll_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    nfds_t fd_count = atoi("2");
    struct pollfd buf[1] = {{0, POLLIN, 0}};
    // Set timeout to zero to prevent waiting in ppoll when fortify test fails.
    struct timespec timeout;
    timeout.tv_sec = timeout.tv_nsec = 0;

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            ppoll(buf, fd_count, &timeout, NULL);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }

    return;
}
#endif

int main(int argc, char *argv[]) {
    poll_0010();
    poll_0020();
#ifdef _GNU_SOURCE
    ppoll_0010();
    ppoll_0020();
#endif

    return t_status;
}