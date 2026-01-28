/*
 * Copyright (c) 2023 - 2026 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "functionalext.h"

const int STACK_SIZE = 1024 * 1024 *4;

/**
 * @tc.name      : getprocpid_0100
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0100(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1 > 1, true);
    pid_t pid2 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1, pid2);
    EXPECT_EQ("getprocpid_0100", pid1, getpid());
}

/**
 * @tc.name      : getprocpid_0200
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0200(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0200", pid1, getpid());

    pid_t child = fork();
    if (child == 0) {
        pid_t pid2 = getprocpid();
        if (pid2 != getpid()) {
            exit(1);
        }
        exit(0);
    }
    EXPECT_EQ("getprocpid_0200", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getprocpid_0200", ret, child);
    EXPECT_EQ("getprocpid_0200", WIFEXITED(status), true);
}

static int child_func(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    if (getpid() != 1) {
        return ESRCH;
    }

    pid_t pid = getprocpid();
    if (pid == parent) {
        return ENOSYS;
    }
    return 0;
}

/**
 * @tc.name      : getprocpid_0300
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0300(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0300", pid1, getpid());

    void* stack = malloc(STACK_SIZE);
    if (!stack) {
        t_error("malloc failed %s \n", strerror(errno));
    }

    pid_t child = clone(child_func, (char *)stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, &pid1);
    EXPECT_EQ("getprocpid_0300", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getprocpid_0300", ret, child);
    EXPECT_EQ("getprocpid_0300", WIFEXITED(status), true);

    free(stack);
}

static void *pthread_func(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    pid_t pid1 = getproctid();
    if (parent == pid1) {
        return (void *)ESRCH;
    }
    pid_t pid2 = gettid();
    if (pid1 != pid2) {
        return (void *)ENOSYS;
    }
    return NULL;
}

/**
 * @tc.name      : getproctid_0100
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0100(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0100", pid1 > 1, true);
    EXPECT_EQ("getproctid_0100", pid1, gettid());

    void *retVal = NULL;
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, pthread_func, &pid1);
    EXPECT_EQ("getproctid_0100", ret, 0);

    ret = pthread_join(thread, &retVal);
    EXPECT_EQ("getproctid_0100", ret, 0);
    EXPECT_EQ("getproctid_0100", retVal, NULL);
}

/**
 * @tc.name      : getproctid_0200
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0200(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0200", pid1, gettid());

    pid_t child = fork();
    if (child == 0) {
        pid_t pid2 = getproctid();
        if (pid2 != gettid()) {
            exit(1);
        }
        exit(0);
    }

    EXPECT_EQ("getproctid_0200", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getproctid_0200", ret, child);
    EXPECT_EQ("getproctid_0200", WIFEXITED(status), true);
}

static int child_func_tid(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    if (getpid() != 1) {
        return ESRCH;
    }

    pid_t pid = getproctid();
    if (pid == parent) {
        return ENOSYS;
    }
    return 0;
}

/**
 * @tc.name      : getproctid_0300
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0300(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0300", pid1, gettid());

    void* stack = malloc(STACK_SIZE);
    if (!stack) {
        t_error("malloc failed %s \n", strerror(errno));
    }

    pid_t child = clone(child_func_tid, (char *)stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, &pid1);
    EXPECT_EQ("getproctid_0300", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getproctid_0300", ret, child);
    EXPECT_EQ("getproctid_0300", WIFEXITED(status), true);

    free(stack);
}

int main(int argc, char *argv[])
{
    getprocpid_0100();
    getprocpid_0200();
    getprocpid_0300();
    getproctid_0100();
    getproctid_0200();
    getproctid_0300();
    return t_status;
}
