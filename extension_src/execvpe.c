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

#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "functionalext.h"

char *my_env[] = {"THIS=environment will be", "PASSED=to new process by", "the EXEC=functions", NULL};

/**
 * @tc.name      : execvpe_0100
 * @tc.desc      : Verify that the specified file can be executed
 *                 (all parameters are valid, PATH is not empty by default)
 * @tc.level     : Level 0
 */
void execvpe_0100(void)
{
    char *path = getenv("touch");
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "/data/test.txt", 0};
        int result = execvpe("touch", argv, &path);
        EXPECT_NE("execvpe_0100", result, -1);
    }

    sleep(1);
    FILE *fptr = fopen("/data/test.txt", "r");
    EXPECT_PTRNE("execvpe_0100", fptr, NULL);
    fclose(fptr);
    remove("/data/test.txt");
}

/**
 * @tc.name      : execvpe_0200
 * @tc.desc      : Verify that the specified file can be executed (all parameters are valid, PATH is null)
 * @tc.level     : Level 0
 */
void execvpe_0200(void)
{
    char *path = getenv("touch");
    setenv("touch", "\0", 1);
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "/data/test.txt", 0};
        int result = execvpe("touch", argv, &path);
        EXPECT_NE("execvpe_0100", result, -1);
    }

    sleep(1);
    FILE *fptr = fopen("/data/test.txt", "r");
    EXPECT_PTRNE("execvpe_0100", fptr, NULL);

    fclose(fptr);
    remove("/data/test.txt");
    unsetenv("touch");
}

/**
 * @tc.name      : execvpe_0300
 * @tc.desc      : Verify that the specified file cannot be executed (file argument is invalid, file
 *                 argument points to NULL)
 * @tc.level     : Level 2
 */
void execvpe_0300(void)
{
    char buff[] = "\0";
    char *argv[] = {"ls", "-al", "/etc/passwd", 0};
    int result = execvpe(buff, argv, my_env);
    EXPECT_EQ("execvpe_0300", result, -1);
}

/**
 * @tc.name      : execvpe_0400
 * @tc.desc      : Verify that the specified file cannot be executed (file argument is invalid, file
 *                 length exceeds NAME_MAX)
 * @tc.level     : Level 2
 */
void execvpe_0400(void)
{
    char *argv[] = {"ls", "-al", "/etc/passwd", 0};
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    int result = execvpe(buff, argv, my_env);
    EXPECT_EQ("execvpe_0400", result, -1);
}

/**
 * @tc.name      : execvpe_0500
 * @tc.desc      : execvpe with command not in PATH should return -1 and set errno to ENOENT
 * @tc.level     : Level 1
 */
void execvpe_0500(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execvpe("nonexistent_cmd_xyz_999", argv, my_env);
    EXPECT_EQ("execvpe_0500", ret, -1);
    EXPECT_EQ("execvpe_0500", errno, ENOENT);
}

/**
 * @tc.name      : execvpe_0600
 * @tc.desc      : execvpe with NULL argv should return -1 and set errno to EFAULT
 * @tc.level     : Level 1
 */
void execvpe_0600(void)
{
    errno = 0;
    int ret = execvpe("ls", NULL, my_env);
    EXPECT_EQ("execvpe_0600", ret, -1);
    EXPECT_TRUE("execvpe_0600", errno == EFAULT || errno == EINVAL);
}

/**
 * @tc.name      : execvpe_0700
 * @tc.desc      : execvpe passes custom environment variables to child process correctly
 * @tc.level     : Level 1
 */
void execvpe_0700(void)
{
    char *custom_env[] = {"MY_EXECVPE_VAR=test_value_789", "PATH=/bin:/usr/bin", NULL};
    const char *script = "/tmp/execvpe_env_test.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execvpe_0700 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho $MY_EXECVPE_VAR > /tmp/execvpe_env_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execvpe_env_test", (char *)NULL};
        int ret = execvpe(script, argv, custom_env);
        if (ret == -1) {
            perror("execvpe_0700 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_0700", WEXITSTATUS(status), 0);
        FILE *out = fopen("/tmp/execvpe_env_out", "r");
        if (out != NULL) {
            char buf[64] = {0};
            fgets(buf, sizeof(buf), out);
            fclose(out);
            EXPECT_TRUE("execvpe_0700", strstr(buf, "test_value_789") != NULL);
            unlink("/tmp/execvpe_env_out");
        }
        unlink(script);
    }
}

/**
 * @tc.name      : execvpe_0800
 * @tc.desc      : execvpe with NULL envp should use current process environment
 * @tc.level     : Level 1
 */
void execvpe_0800(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"env", "env", NULL};
        int ret = execvpe("/usr/bin/env", argv, NULL);
        if (ret == -1) {
            perror("execvpe_0800 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execvpe_0800", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execvpe_0900
 * @tc.desc      : execvpe with file lacking execute permission should fail with EACCES
 * @tc.level     : Level 1
 */
void execvpe_0900(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/execvpe_noexec_tmp";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("execvpe_0900 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho test\n");
    fclose(fp);
    chmod(tmp_file, 0644);

    char *argv[] = {"execvpe_noexec_tmp", (char *)NULL};
    int ret = execvpe(tmp_file, argv, my_env);
    EXPECT_EQ("execvpe_0900", ret, -1);
    EXPECT_TRUE("execvpe_0900", errno == EACCES || errno == EPERM);
    unlink(tmp_file);
}

/**
 * @tc.name      : execvpe_1000
 * @tc.desc      : execvpe with absolute path should bypass PATH search and execute directly
 * @tc.level     : Level 1
 */
void execvpe_1000(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvpe_abs_test.txt", NULL};
        int ret = execvpe("/bin/touch", argv, my_env);
        if (ret == -1) {
            perror("execvpe_1000 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_1000", WEXITSTATUS(status), 0);
        int check = access("execvpe_abs_test.txt", F_OK);
        EXPECT_EQ("execvpe_1000", check, 0);
        if (check == 0) remove("execvpe_abs_test.txt");
    }
}

/**
 * @tc.name      : execvpe_1100
 * @tc.desc      : execvpe passes arguments correctly to executed program
 * @tc.level     : Level 1
 */
void execvpe_1100(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "execvpe_arg_check_123", (char *)NULL};
        int ret = execvpe("echo", argv, my_env);
        if (ret == -1) {
            perror("execvpe_1100 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_1100", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execvpe_1200
 * @tc.desc      : execvpe with directory path should fail with EISDIR or EACCES
 * @tc.level     : Level 2
 */
void execvpe_1200(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execvpe("/tmp", argv, my_env);
    EXPECT_EQ("execvpe_1200", ret, -1);
    EXPECT_TRUE("execvpe_1200", errno == EISDIR || errno == EACCES || errno == ENOEXEC);
}

/**
 * @tc.name      : execvpe_1300
 * @tc.desc      : execvpe with script containing shebang should execute interpreter
 * @tc.level     : Level 2
 */
void execvpe_1300(void)
{
    const char *script = "/tmp/execvpe_shebang_tmp.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execvpe_1300 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho shebang_ok > /tmp/execvpe_shebang_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execvpe_shebang_tmp", (char *)NULL};
        int ret = execvpe(script, argv, my_env);
        if (ret == -1) {
            perror("execvpe_1300 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_1300", WEXITSTATUS(status), 0);
        int check = access("/tmp/execvpe_shebang_out", F_OK);
        EXPECT_EQ("execvpe_1300", check, 0);
        unlink("/tmp/execvpe_shebang_out");
        unlink(script);
    }
}

/**
 * @tc.name      : execvpe_1400
 * @tc.desc      : execvpe with modified PATH env var should find command in new path
 * @tc.level     : Level 2
 */
void execvpe_1400(void)
{
    char *custom_env[] = {"PATH=/tmp/execvpe_custom_bin", NULL};
    mkdir("/tmp/execvpe_custom_bin", 0755);
    const char *custom_cmd = "/tmp/execvpe_custom_bin/mytouch";
    FILE *fp = fopen(custom_cmd, "w");
    if (fp != NULL) {
        fprintf(fp, "#!/bin/sh\ntouch /tmp/execvpe_path_ok\nexit 0\n");
        fclose(fp);
        chmod(custom_cmd, 0755);
    }

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"mytouch", (char *)NULL};
        int ret = execvpe("mytouch", argv, custom_env);
        if (ret == -1) {
            perror("execvpe_1400 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_1400", WEXITSTATUS(status), 0);
        int check = access("/tmp/execvpe_path_ok", F_OK);
        EXPECT_EQ("execvpe_1400", check, 0);
        unlink("/tmp/execvpe_path_ok");
        unlink(custom_cmd);
        rmdir("/tmp/execvpe_custom_bin");
    }
}

/**
 * @tc.name      : execvpe_1500
 * @tc.desc      : execvpe with many arguments should handle long arg list correctly
 * @tc.level     : Level 3
 */
void execvpe_1500(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo",
                        "a1", "a2", "a3", "a4", "a5",
                        "b1", "b2", "b3", "b4", "b5",
                        "c1", "c2", "c3", "c4", "c5",
                        (char *)NULL};
        int ret = execvpe("echo", argv, my_env);
        if (ret == -1) {
            perror("execvpe_1500 execvpe failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execvpe_1500", WEXITSTATUS(status), 0);
    }
}

int main(int argc, char *argv[])
{
    execvpe_0100();
    execvpe_0200();
    execvpe_0300();
    execvpe_0400();
    execvpe_0500();
    execvpe_0600();
    execvpe_0700();
    execvpe_0800();
    execvpe_0900();
    execvpe_1000();
    execvpe_1100();
    execvpe_1200();
    execvpe_1300();
    execvpe_1400();
    execvpe_1500();
    return t_status;
}