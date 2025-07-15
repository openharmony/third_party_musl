/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include "stdio.h"
#include <pthread.h>
#include <limits.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <dirent.h>
#include <utime.h>
#include "functionalext.h"

/**
 * @tc.name      : ftell_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void ftell_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("ftell_0100: Error forking process");
    } else if (pid == 0) {
        ftell(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("ftell_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fwrite_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fwrite_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fwrite_0100: Error forking process");
    } else if (pid == 0) {
        char buf[] = "this is test";
        fwrite(buf, sizeof(char), strlen(buf), NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fwrite_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : getc_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void getc_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("getc_0100: Error forking process");
    } else if (pid == 0) {
        getc(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("getc_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pclose_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pclose_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pclose_0100: Error forking process");
    } else if (pid == 0) {
        pclose(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pclose_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : rewind_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void rewind_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("rewind_0100: Error forking process");
    } else if (pid == 0) {
        rewind(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("rewind_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : setvbuf_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void setvbuf_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("setvbuf_0100: Error forking process");
    } else if (pid == 0) {
        char buff[1024] = {0};
        setvbuf(NULL, buff, -1, 1024);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("setvbuf_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : wcsnrtombs_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void wcsnrtombs_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("wcsnrtombs_0100: Error forking process");
    } else if (pid == 0) {
        char buffer[32];
        mbstate_t mbs;
        size_t wn = 4;
        size_t n = 4;
        mbrlen(NULL, 0, &mbs);
        memset(buffer, 0, sizeof(buffer));
        wcsnrtombs(buffer, NULL, wn, n, &mbs);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("wcsnrtombs_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : unsetenv_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void unsetenv_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("unsetenv_0100: Error forking process");
    } else if (pid == 0) {
        unsetenv(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("unsetenv_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : readdir_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void readdir_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("readdir_0100: Error forking process");
    } else if (pid == 0) {
        readdir(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("readdir_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_join_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_join_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_join_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        void *res;
        pthread_join(invalid_thread, &res);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_join_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_kill_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_kill_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_kill_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        int valid_sig = SIGTERM;
        pthread_kill(invalid_thread, valid_sig);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_kill_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_setschedparam_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_setschedparam_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_setschedparam_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        struct sched_param param;
        param.sched_priority = 1;
        pthread_setschedparam(invalid_thread, SCHED_FIFO, &param);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_setschedparam_0100", SIGABRT, sig);
        }
    }
}

int main(int argc, char *argv[])
{
    ftell_0100();
    fwrite_0100();
    getc_0100();
    pclose_0100();
    rewind_0100();
    setvbuf_0100();
    wcsnrtombs_0100();
    unsetenv_0100();
    readdir_0100();
    pthread_join_0100();
    pthread_kill_0100();
    pthread_setschedparam_0100();

    return t_status;
}
