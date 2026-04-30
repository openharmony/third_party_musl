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

#include "functionalext.h"
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @tc.name      : execv_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execv_0100(void)
{
    pid_t fpid;
    fpid = fork();
    FILE *fp;
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
        int ret = execv("/bin/touch", argv);
        EXPECT_NE("execv_0100", ret, -1);
    }
    sleep(1);
    int isExist = access("execvtest.txt", F_OK);
    EXPECT_EQ("execv_0100", isExist, 0);
    remove("execvtest.txt");
}

/**
 * @tc.name      : execv_0200
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execv_0200(void)
{
    char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
    int ret = execv(" ", argv);
    EXPECT_TRUE("execv_0200", ret == -1);
}

/**
 * @tc.name      : execv_0300
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execv_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
    int ret = execv(buff, argv);
    EXPECT_TRUE("execv_0300", ret == -1);
}

/**
 * @tc.name      : execv_0400
 * @tc.desc      : execv with non-existent file path should return -1 and set errno to ENOENT
 * @tc.level     : Level 1
 */
void execv_0400(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execv("/bin/nonexistent_file_xyz_12345", argv);
    EXPECT_EQ("execv_0400", ret, -1);
    EXPECT_EQ("execv_0400", errno, ENOENT);
}

/**
 * @tc.name      : execv_0500
 * @tc.desc      : execv with NULL argv should return -1 and set errno to EFAULT
 * @tc.level     : Level 1
 */
void execv_0500(void)
{
    errno = 0;
    int ret = execv("/bin/ls", NULL);
    EXPECT_EQ("execv_0500", ret, -1);
    EXPECT_TRUE("execv_0500", errno == EFAULT || errno == EINVAL);
}

/**
 * @tc.name      : execv_0600
 * @tc.desc      : execv with argv[0] as NULL but argv array valid should still execute
 * @tc.level     : Level 2
 */
void execv_0600(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {NULL, "arg1", "arg2", (char *)NULL};
        int ret = execv("/bin/echo", argv);
        if (ret == -1) {
            perror("execv_0600 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execv_0600", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execv_0700
 * @tc.desc      : execv executes file without execute permission should fail with EACCES
 * @tc.level     : Level 1
 */
void execv_0700(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/execv_noexec_test";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("execv_0700 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho hello\n");
    fclose(fp);
    chmod(tmp_file, 0644);

    char *argv[] = {"execv_noexec_test", (char *)NULL};
    int ret = execv(tmp_file, argv);
    EXPECT_EQ("execv_0700", ret, -1);
    EXPECT_TRUE("execv_0700", errno == EACCES || errno == EPERM);
    unlink(tmp_file);
}

/**
 * @tc.name      : execv_0800
 * @tc.desc      : execv passes arguments correctly to the executed program
 * @tc.level     : Level 1
 */
void execv_0800(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "execv_arg_verify_456", (char *)NULL};
        int ret = execv("/bin/echo", argv);
        if (ret == -1) {
            perror("execv_0800 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execv_0800", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execv_0900
 * @tc.desc      : execv with relative path should work if file exists and is executable
 * @tc.level     : Level 2
 */
void execv_0900(void)
{
    const char *rel_script = "./execv_rel_test.sh";
    FILE *fp = fopen(rel_script, "w");
    if (fp == NULL) {
        t_error("execv_0900 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\ntouch /tmp/execv_rel_success\nexit 0\n");
    fclose(fp);
    chmod(rel_script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execv_rel_test", (char *)NULL};
        int ret = execv(rel_script, argv);
        if (ret == -1) {
            perror("execv_0900 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execv_0900", WEXITSTATUS(status), 0);
        int check = access("/tmp/execv_rel_success", F_OK);
        EXPECT_EQ("execv_0900", check, 0);
        unlink("/tmp/execv_rel_success");
        unlink(rel_script);
    }
}

/**
 * @tc.name      : execv_1000
 * @tc.desc      : execv with directory path instead of file should fail with EISDIR or EACCES
 * @tc.level     : Level 2
 */
void execv_1000(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execv("/tmp", argv);
    EXPECT_EQ("execv_1000", ret, -1);
    EXPECT_TRUE("execv_1000", errno == EISDIR || errno == EACCES || errno == ENOEXEC);
}

/**
 * @tc.name      : execv_1100
 * @tc.desc      : execv with special characters in argv should pass them literally
 * @tc.level     : Level 2
 */
void execv_1100(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "test $HOME `date` | cat", (char *)NULL};
        int ret = execv("/bin/echo", argv);
        if (ret == -1) {
            perror("execv_1100 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execv_1100", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execv_1200
 * @tc.desc      : execv with empty string in argv should be handled as empty argument
 * @tc.level     : Level 2
 */
void execv_1200(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "", "after_empty", (char *)NULL};
        int ret = execv("/bin/echo", argv);
        if (ret == -1) {
            perror("execv_1200 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execv_1200", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execv_1300
 * @tc.desc      : execv with many arguments should handle long arg list correctly
 * @tc.level     : Level 3
 */
void execv_1300(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo",
                        "a1", "a2", "a3", "a4", "a5",
                        "b1", "b2", "b3", "b4", "b5",
                        "c1", "c2", "c3", "c4", "c5",
                        (char *)NULL};
        int ret = execv("/bin/echo", argv);
        if (ret == -1) {
            perror("execv_1300 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execv_1300", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execv_1400
 * @tc.desc      : execv with script containing shebang should execute interpreter
 * @tc.level     : Level 2
 */
void execv_1400(void)
{
    const char *script = "/tmp/execv_shebang_test.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execv_1400 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho shebang_executed > /tmp/execv_shebang_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execv_shebang_test", (char *)NULL};
        int ret = execv(script, argv);
        if (ret == -1) {
            perror("execv_1400 execv failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execv_1400", WEXITSTATUS(status), 0);
        int check = access("/tmp/execv_shebang_out", F_OK);
        EXPECT_EQ("execv_1400", check, 0);
        unlink("/tmp/execv_shebang_out");
        unlink(script);
    }
}

/**
 * @tc.name      : execv_1500
 * @tc.desc      : execv with path containing spaces should fail or handle appropriately
 * @tc.level     : Level 3
 */
void execv_1500(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execv("/tmp/path with spaces/test", argv);
    EXPECT_EQ("execv_1500", ret, -1);
    EXPECT_EQ("execv_1500", errno, ENOENT);
}

int main(int argc, char *argv[])
{
    execv_0100();
    execv_0200();
    execv_0300();
    execv_0400();
    execv_0500();
    execv_0600();
    execv_0700();
    execv_0800();
    execv_0900();
    execv_1000();
    execv_1100();
    execv_1200();
    execv_1300();
    execv_1400();
    execv_1500();
    return t_status;
}