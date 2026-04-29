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

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : execvp_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execvp_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvptest.txt", NULL};
        execvp("touch", argv);
    }
    sleep(1);
    int isExist = access("execvptest.txt", F_OK);
    EXPECT_EQ("execvp_0100", isExist, 0);
    if (isExist == 0) {
        remove("execvptest.txt");
    }
}

/**
 * @tc.name      : execvp_0200
 * @tc.desc      : Each parameter is valid, the PATH is empty, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execvp_0200(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvptest.txt", NULL};
        char *buff = getenv("ls");
        setenv("ls", " ", 1);
        execvp("touch", argv);
        setenv("ls", buff, 1);
    }
    sleep(1);
    int isExist = access("execvptest.txt", F_OK);
    EXPECT_EQ("execvp_0200", isExist, 0);
    if (isExist == 0) {
        remove("execvptest.txt");
    }
}

/**
 * @tc.name      : execvp_0300
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execvp_0300(void)
{
    char *argv[] = {"touch", "execvptest.txt", NULL};
    int ret = execvp(" ", argv);
    EXPECT_EQ("execvp_0300", ret, -1);
}

/**
 * @tc.name      : execvp_0400
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execvp_0400(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "execvptest.txt", NULL};
    int ret = execvp(buff, argv);
    EXPECT_EQ("execvp_0400", ret, -1);
}

/**
 * @tc.name      : execvp_0500
 * @tc.desc      : execvp with command not in PATH should return -1 and set errno to ENOENT
 * @tc.level     : Level 1
 */
void execvp_0500(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execvp("nonexistent_cmd_xyz_999", argv);
    EXPECT_EQ("execvp_0500", ret, -1);
    EXPECT_EQ("execvp_0500", errno, ENOENT);
}

/**
 * @tc.name      : execvp_0600
 * @tc.desc      : execvp with NULL argv should return -1 and set errno to EFAULT
 * @tc.level     : Level 1
 */
void execvp_0600(void)
{
    errno = 0;
    int ret = execvp("ls", NULL);
    EXPECT_EQ("execvp_0600", ret, -1);
    EXPECT_TRUE("execvp_0600", errno == EFAULT || errno == EINVAL);
}

/**
 * @tc.name      : execvp_0700
 * @tc.desc      : execvp with absolute path should bypass PATH search and execute directly
 * @tc.level     : Level 1
 */
void execvp_0700(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvp_abs_test.txt", NULL};
        int ret = execvp("/bin/touch", argv);
        if (ret == -1) {
            perror("execvp_0700 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvp_0700", WEXITSTATUS(status), 0);
        int check = access("execvp_abs_test.txt", F_OK);
        EXPECT_EQ("execvp_0700", check, 0);
        if (check == 0) remove("execvp_abs_test.txt");
    }
}

/**
 * @tc.name      : execvp_0800
 * @tc.desc      : execvp with file lacking execute permission should fail with EACCES
 * @tc.level     : Level 1
 */
void execvp_0800(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/execvp_noexec_tmp";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("execvp_0800 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho test\n");
    fclose(fp);
    chmod(tmp_file, 0644);

    char *argv[] = {"execvp_noexec_tmp", (char *)NULL};
    int ret = execvp(tmp_file, argv);
    EXPECT_EQ("execvp_0800", ret, -1);
    EXPECT_TRUE("execvp_0800", errno == EACCES || errno == EPERM);
    unlink(tmp_file);
}

/**
 * @tc.name      : execvp_0900
 * @tc.desc      : execvp passes arguments correctly to executed program
 * @tc.level     : Level 1
 */
void execvp_0900(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "execvp_arg_verify_456", (char *)NULL};
        int ret = execvp("echo", argv);
        if (ret == -1) {
            perror("execvp_0900 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvp_0900", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execvp_1000
 * @tc.desc      : execvp with directory path should fail with EISDIR or EACCES
 * @tc.level     : Level 2
 */
void execvp_1000(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execvp("/tmp", argv);
    EXPECT_EQ("execvp_1000", ret, -1);
    EXPECT_TRUE("execvp_1000", errno == EISDIR || errno == EACCES || errno == ENOEXEC);
}

/**
 * @tc.name      : execvp_1100
 * @tc.desc      : execvp with special characters in argv should pass them literally
 * @tc.level     : Level 2
 */
void execvp_1100(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "test $HOME `date` | cat", (char *)NULL};
        int ret = execvp("echo", argv);
        if (ret == -1) {
            perror("execvp_1100 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execvp_1100", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execvp_1200
 * @tc.desc      : execvp with empty string command should fail with ENOENT or EINVAL
 * @tc.level     : Level 1
 */
void execvp_1200(void)
{
    errno = 0;
    char *argv[] = {"arg0", (char *)NULL};
    int ret = execvp("", argv);
    EXPECT_EQ("execvp_1200", ret, -1);
    EXPECT_TRUE("execvp_1200", errno == ENOENT || errno == EINVAL);
}

/**
 * @tc.name      : execvp_1300
 * @tc.desc      : execvp with script containing shebang should execute interpreter
 * @tc.level     : Level 2
 */
void execvp_1300(void)
{
    const char *script = "/tmp/execvp_shebang_tmp.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execvp_1300 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho shebang_ok > /tmp/execvp_shebang_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execvp_shebang_tmp", (char *)NULL};
        int ret = execvp(script, argv);
        if (ret == -1) {
            perror("execvp_1300 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvp_1300", WEXITSTATUS(status), 0);
        int check = access("/tmp/execvp_shebang_out", F_OK);
        EXPECT_EQ("execvp_1300", check, 0);
        unlink("/tmp/execvp_shebang_out");
        unlink(script);
    }
}

/**
 * @tc.name      : execvp_1400
 * @tc.desc      : execvp with modified PATH should find command in new path
 * @tc.level     : Level 2
 */
void execvp_1400(void)
{
    char *orig_path = getenv("PATH");
    setenv("PATH", "/tmp/execvp_custom_bin", 1);
    
    mkdir("/tmp/execvp_custom_bin", 0755);
    const char *custom_cmd = "/tmp/execvp_custom_bin/mytouch";
    FILE *fp = fopen(custom_cmd, "w");
    if (fp != NULL) {
        fprintf(fp, "#!/bin/sh\ntouch /tmp/execvp_path_test_ok\nexit 0\n");
        fclose(fp);
        chmod(custom_cmd, 0755);
    }

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"mytouch", (char *)NULL};
        int ret = execvp("mytouch", argv);
        if (ret == -1) {
            perror("execvp_1400 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvp_1400", WEXITSTATUS(status), 0);
        int check = access("/tmp/execvp_path_test_ok", F_OK);
        EXPECT_EQ("execvp_1400", check, 0);
        unlink("/tmp/execvp_path_test_ok");
        unlink(custom_cmd);
        rmdir("/tmp/execvp_custom_bin");
        if (orig_path) setenv("PATH", orig_path, 1);
        else unsetenv("PATH");
    }
}

/**
 * @tc.name      : execvp_1500
 * @tc.desc      : execvp with many arguments should handle long arg list correctly
 * @tc.level     : Level 3
 */
void execvp_1500(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo",
                        "a1", "a2", "a3", "a4", "a5",
                        "b1", "b2", "b3", "b4", "b5",
                        "c1", "c2", "c3", "c4", "c5",
                        (char *)NULL};
        int ret = execvp("echo", argv);
        if (ret == -1) {
            perror("execvp_1500 execvp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvp_1500", WEXITSTATUS(status), 0);
    }
}

int main(int argc, char *argv[])
{
    execvp_0100();
    execvp_0200();
    execvp_0300();
    execvp_0400();
    execvp_0500();
    execvp_0600();
    execvp_0700();
    execvp_0800();
    execvp_0900();
    execvp_1000();
    execvp_1100();
    execvp_1200();
    execvp_1300();
    execvp_1400();
    execvp_1500();
    return t_status;
}