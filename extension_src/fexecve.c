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

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : fexecve_0100
 * @tc.desc      : Each parameter is valid, the fexecve function can execute the specified file.
 * @tc.level     : Level 0
 */
void fexecve_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"./fexecverely", NULL};
        char *environ[] = {0, NULL};
        int fd = open("fexecverely", O_RDONLY);
        int ret = fexecve(fd, argv, environ);
        EXPECT_NE("fexecve_0100", ret, -1);
    }
}

/**
 * @tc.name      : fexecve_0200
 * @tc.desc      : The fd parameter is invalid (NULL), the fexecve function cannot execute the specified file.
 * @tc.level     : Level 2
 */
void fexecve_0200(void)
{
    char *my_env[] = {NULL};
    char *argv[] = {"touch", "fexecvetest.txt", NULL};
    int ret = fexecve(-1, argv, my_env);
    EXPECT_EQ("fexecve_0200", ret, -1);
}

/**
 * @tc.name      : fexecve_0300
 * @tc.desc      : fexecve with closed fd should return -1 and set errno to EBADF
 * @tc.level     : Level 1
 */
void fexecve_0300(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    char *envp[] = {NULL};
    int fd = open("/bin/ls", O_RDONLY);
    close(fd);  // Close before calling fexecve
    int ret = fexecve(fd, argv, envp);
    EXPECT_EQ("fexecve_0300", ret, -1);
    EXPECT_EQ("fexecve_0300", errno, EBADF);
}

/**
 * @tc.name      : fexecve_0400
 * @tc.desc      : fexecve with fd pointing to non-executable file should fail with EACCES or ENOEXEC
 * @tc.level     : Level 1
 */
void fexecve_0400(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/fexecve_noexec_tmp";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("fexecve_0400 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho test\n");
    fclose(fp);
    chmod(tmp_file, 0644);  // Remove execute permission

    char *argv[] = {"fexecve_noexec_tmp", (char *)NULL};
    char *envp[] = {NULL};
    int fd = open(tmp_file, O_RDONLY);
    EXPECT_TRUE("fexecve_0400", fd >= 0);
    int ret = fexecve(fd, argv, envp);
    close(fd);
    EXPECT_EQ("fexecve_0400", ret, -1);
    EXPECT_TRUE("fexecve_0400", errno == EACCES || errno == ENOEXEC || errno == EPERM);
    unlink(tmp_file);
}

/**
 * @tc.name      : fexecve_0500
 * @tc.desc      : fexecve with fd pointing to directory should fail with EACCES or EISDIR
 * @tc.level     : Level 2
 */
void fexecve_0500(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    char *envp[] = {NULL};
    int fd = open("/tmp", O_RDONLY);
    EXPECT_TRUE("fexecve_0500", fd >= 0);
    int ret = fexecve(fd, argv, envp);
    close(fd);
    EXPECT_EQ("fexecve_0500", ret, -1);
    EXPECT_TRUE("fexecve_0500", errno == EACCES || errno == EISDIR || errno == ENOEXEC);
}

/**
 * @tc.name      : fexecve_0600
 * @tc.desc      : fexecve with NULL argv should return -1 and set errno to EFAULT
 * @tc.level     : Level 1
 */
void fexecve_0600(void)
{
    errno = 0;
    char *envp[] = {NULL};
    int fd = open("/bin/ls", O_RDONLY);
    EXPECT_TRUE("fexecve_0600", fd >= 0);
    int ret = fexecve(fd, NULL, envp);
    close(fd);
    EXPECT_EQ("fexecve_0600", ret, -1);
    EXPECT_TRUE("fexecve_0600", errno == EFAULT || errno == EINVAL);
}

/**
 * @tc.name      : fexecve_0700
 * @tc.desc      : fexecve passes custom environment variables to child process correctly
 * @tc.level     : Level 1
 */
void fexecve_0700(void)
{
    char *custom_env[] = {"MY_FEXECVE_VAR=fd_env_test_456", NULL};
    const char *script = "/tmp/fexecve_env_test.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("fexecve_0700 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho $MY_FEXECVE_VAR > /tmp/fexecve_env_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"fexecve_env_test", (char *)NULL};
        int fd = open(script, O_RDONLY);
        if (fd < 0) {
            perror("fexecve_0700 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, custom_env);
        if (ret == -1) {
            perror("fexecve_0700 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("fexecve_0700", WEXITSTATUS(status), 0);
        FILE *out = fopen("/tmp/fexecve_env_out", "r");
        if (out != NULL) {
            char buf[64] = {0};
            fgets(buf, sizeof(buf), out);
            fclose(out);
            EXPECT_TRUE("fexecve_0700", strstr(buf, "fd_env_test_456") != NULL);
            unlink("/tmp/fexecve_env_out");
        }
        unlink(script);
    }
}

/**
 * @tc.name      : fexecve_0800
 * @tc.desc      : fexecve with NULL envp should use current process environment
 * @tc.level     : Level 1
 */
void fexecve_0800(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"env", "env", NULL};
        int fd = open("/usr/bin/env", O_RDONLY);
        if (fd < 0) {
            perror("fexecve_0800 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, NULL);
        if (ret == -1) {
            perror("fexecve_0800 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("fexecve_0800", WIFEXITED(status));
    }
}

/**
 * @tc.name      : fexecve_0900
 * @tc.desc      : fexecve passes arguments correctly to executed program
 * @tc.level     : Level 1
 */
void fexecve_0900(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "fexecve_arg_check_789", (char *)NULL};
        int fd = open("/bin/echo", O_RDONLY);
        if (fd < 0) {
            perror("fexecve_0900 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, NULL);
        if (ret == -1) {
            perror("fexecve_0900 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("fexecve_0900", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : fexecve_1000
 * @tc.desc      : fexecve with script containing shebang should execute interpreter
 * @tc.level     : Level 2
 */
void fexecve_1000(void)
{
    const char *script = "/tmp/fexecve_shebang_tmp.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("fexecve_1000 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho shebang_fd_ok > /tmp/fexecve_shebang_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"fexecve_shebang_tmp", (char *)NULL};
        int fd = open(script, O_RDONLY);
        if (fd < 0) {
            perror("fexecve_1000 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, NULL);
        if (ret == -1) {
            perror("fexecve_1000 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("fexecve_1000", WEXITSTATUS(status), 0);
        int check = access("/tmp/fexecve_shebang_out", F_OK);
        EXPECT_EQ("fexecve_1000", check, 0);
        unlink("/tmp/fexecve_shebang_out");
        unlink(script);
    }
}

/**
 * @tc.name      : fexecve_1100
 * @tc.desc      : fexecve with fd opened with O_WRONLY should fail with EACCES
 * @tc.level     : Level 2
 */
void fexecve_1100(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/fexecve_wronly_tmp";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("fexecve_1100 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho test\n");
    fclose(fp);
    chmod(tmp_file, 0755);

    char *argv[] = {"fexecve_wronly_tmp", (char *)NULL};
    char *envp[] = {NULL};
    int fd = open(tmp_file, O_WRONLY);  // Write-only, not readable
    EXPECT_TRUE("fexecve_1100", fd >= 0);
    int ret = fexecve(fd, argv, envp);
    close(fd);
    EXPECT_EQ("fexecve_1100", ret, -1);
    EXPECT_EQ("fexecve_1100", errno, EACCES);
    unlink(tmp_file);
}

/**
 * @tc.name      : fexecve_1200
 * @tc.desc      : fexecve with special characters in argv should pass them literally
 * @tc.level     : Level 2
 */
void fexecve_1200(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "test $HOME `date` | cat", (char *)NULL};
        int fd = open("/bin/echo", O_RDONLY);
        if (fd < 0) {
            perror("fexecve_1200 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, NULL);
        if (ret == -1) {
            perror("fexecve_1200 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("fexecve_1200", WIFEXITED(status));
    }
}

/**
 * @tc.name      : fexecve_1300
 * @tc.desc      : fexecve with many arguments should handle long arg list correctly
 * @tc.level     : Level 3
 */
void fexecve_1300(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo",
                        "a1", "a2", "a3", "a4", "a5",
                        "b1", "b2", "b3", "b4", "b5",
                        "c1", "c2", "c3", "c4", "c5",
                        (char *)NULL};
        int fd = open("/bin/echo", O_RDONLY);
        if (fd < 0) {
            perror("fexecve_1300 open failed");
            _exit(1);
        }
        int ret = fexecve(fd, argv, NULL);
        if (ret == -1) {
            perror("fexecve_1300 fexecve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("fexecve_1300", WEXITSTATUS(status), 0);
    }
}

int main(int argc, char *argv[])
{
    fexecve_0100();
    fexecve_0200();
    fexecve_0300();
    fexecve_0400();
    fexecve_0500();
    fexecve_0600();
    fexecve_0700();
    fexecve_0800();
    fexecve_0900();
    fexecve_1000();
    fexecve_1100();
    fexecve_1200();
    fexecve_1300();
    return t_status;
}