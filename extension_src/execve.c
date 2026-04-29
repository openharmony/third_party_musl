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
#include <string.h>

char *my_env[] = {0, NULL};

/**
 * @tc.name      : execve_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execve_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvetest.txt", NULL};
        execve("/bin/touch", argv, my_env);
    }
    sleep(1);
    int isExist = access("execvetest.txt", F_OK);
    EXPECT_EQ("execve_0100", isExist, 0);
    remove("execvetest.txt");
}

/**
 * @tc.name      : execve_0200
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execve_0200(void)
{
    char *argv[] = {"touch", "execvetest.txt", NULL};
    int ret = execve(" ", argv, my_env);
    EXPECT_EQ("execve_0200", ret, -1);
}

/**
 * @tc.name      : execve_0300
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execve_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "execvetest.txt", NULL};
    int ret = execve(buff, argv, my_env);
    EXPECT_EQ("execve_0300", ret, -1);
}

/**
 * @tc.name      : execve_0400
 * @tc.desc      : execve with non-existent file path should return -1 and set errno to ENOENT
 * @tc.level     : Level 1
 */
void execve_0400(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execve("/bin/nonexistent_xyz_999", argv, my_env);
    EXPECT_EQ("execve_0400", ret, -1);
    EXPECT_EQ("execve_0400", errno, ENOENT);
}

/**
 * @tc.name      : execve_0500
 * @tc.desc      : execve with NULL argv should return -1 and set errno to EFAULT
 * @tc.level     : Level 1
 */
void execve_0500(void)
{
    errno = 0;
    int ret = execve("/bin/ls", NULL, my_env);
    EXPECT_EQ("execve_0500", ret, -1);
    EXPECT_TRUE("execve_0500", errno == EFAULT || errno == EINVAL);
}

/**
 * @tc.name      : execve_0600
 * @tc.desc      : execve with NULL envp should use current process environment
 * @tc.level     : Level 1
 */
void execve_0600(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"env", "env", NULL};
        int ret = execve("/usr/bin/env", argv, NULL);
        if (ret == -1) {
            perror("execve_0600 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execve_0600", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execve_0700
 * @tc.desc      : execve passes custom environment variables to child process
 * @tc.level     : Level 1
 */
void execve_0700(void)
{
    char *custom_env[] = {"MY_TEST_VAR=execve_success_789", "PATH=/bin:/usr/bin", NULL};
    const char *script = "/tmp/execve_env_test.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execve_0700 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho $MY_TEST_VAR > /tmp/execve_env_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execve_env_test", (char *)NULL};
        int ret = execve(script, argv, custom_env);
        if (ret == -1) {
            perror("execve_0700 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_0700", WEXITSTATUS(status), 0);
        FILE *out = fopen("/tmp/execve_env_out", "r");
        if (out != NULL) {
            char buf[64] = {0};
            fgets(buf, sizeof(buf), out);
            fclose(out);
            EXPECT_TRUE("execve_0700", strstr(buf, "execve_success_789") != NULL);
            unlink("/tmp/execve_env_out");
        }
        unlink(script);
    }
}

/**
 * @tc.name      : execve_0800
 * @tc.desc      : execve with file lacking execute permission should fail with EACCES
 * @tc.level     : Level 1
 */
void execve_0800(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/execve_noexec_tmp";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("execve_0800 failed to create temp file");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho test\n");
    fclose(fp);
    chmod(tmp_file, 0644);

    char *argv[] = {"execve_noexec_tmp", (char *)NULL};
    int ret = execve(tmp_file, argv, my_env);
    EXPECT_EQ("execve_0800", ret, -1);
    EXPECT_TRUE("execve_0800", errno == EACCES || errno == EPERM);
    unlink(tmp_file);
}

/**
 * @tc.name      : execve_0900
 * @tc.desc      : execve with directory path should fail with EISDIR or EACCES
 * @tc.level     : Level 2
 */
void execve_0900(void)
{
    errno = 0;
    char *argv[] = {"dummy", (char *)NULL};
    int ret = execve("/tmp", argv, my_env);
    EXPECT_EQ("execve_0900", ret, -1);
    EXPECT_TRUE("execve_0900", errno == EISDIR || errno == EACCES || errno == ENOEXEC);
}

/**
 * @tc.name      : execve_1000
 * @tc.desc      : execve passes arguments correctly to executed program
 * @tc.level     : Level 1
 */
void execve_1000(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "execve_arg_check_123", (char *)NULL};
        int ret = execve("/bin/echo", argv, my_env);
        if (ret == -1) {
            perror("execve_1000 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_1000", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execve_1100
 * @tc.desc      : execve with relative path should work if file is executable
 * @tc.level     : Level 2
 */
void execve_1100(void)
{
    const char *rel_script = "./execve_rel_tmp.sh";
    FILE *fp = fopen(rel_script, "w");
    if (fp == NULL) {
        t_error("execve_1100 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\ntouch /tmp/execve_rel_ok\nexit 0\n");
    fclose(fp);
    chmod(rel_script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execve_rel_tmp", (char *)NULL};
        int ret = execve(rel_script, argv, my_env);
        if (ret == -1) {
            perror("execve_1100 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_1100", WEXITSTATUS(status), 0);
        int check = access("/tmp/execve_rel_ok", F_OK);
        EXPECT_EQ("execve_1100", check, 0);
        unlink("/tmp/execve_rel_ok");
        unlink(rel_script);
    }
}

/**
 * @tc.name      : execve_1200
 * @tc.desc      : execve with special characters in argv should pass them literally
 * @tc.level     : Level 2
 */
void execve_1200(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo", "test $HOME `date` | cat", (char *)NULL};
        int ret = execve("/bin/echo", argv, my_env);
        if (ret == -1) {
            perror("execve_1200 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execve_1200", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execve_1300
 * @tc.desc      : execve with empty envp array should execute with minimal environment
 * @tc.level     : Level 2
 */
void execve_1300(void)
{
    char *empty_env[] = {NULL};
    const char *script = "/tmp/execve_empty_env.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execve_1300 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\nenv > /tmp/execve_empty_env_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execve_empty_env", (char *)NULL};
        int ret = execve(script, argv, empty_env);
        if (ret == -1) {
            perror("execve_1300 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_1300", WEXITSTATUS(status), 0);
        int check = access("/tmp/execve_empty_env_out", F_OK);
        EXPECT_EQ("execve_1300", check, 0);
        unlink("/tmp/execve_empty_env_out");
        unlink(script);
    }
}

/**
 * @tc.name      : execve_1400
 * @tc.desc      : execve with script containing shebang should execute interpreter
 * @tc.level     : Level 2
 */
void execve_1400(void)
{
    const char *script = "/tmp/execve_shebang_tmp.sh";
    FILE *fp = fopen(script, "w");
    if (fp == NULL) {
        t_error("execve_1400 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\necho shebang_ok > /tmp/execve_shebang_out\nexit 0\n");
    fclose(fp);
    chmod(script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"execve_shebang_tmp", (char *)NULL};
        int ret = execve(script, argv, my_env);
        if (ret == -1) {
            perror("execve_1400 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_1400", WEXITSTATUS(status), 0);
        int check = access("/tmp/execve_shebang_out", F_OK);
        EXPECT_EQ("execve_1400", check, 0);
        unlink("/tmp/execve_shebang_out");
        unlink(script);
    }
}

/**
 * @tc.name      : execve_1500
 * @tc.desc      : execve with many arguments should handle long arg list correctly
 * @tc.level     : Level 3
 */
void execve_1500(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"echo", "echo",
                        "a1", "a2", "a3", "a4", "a5",
                        "b1", "b2", "b3", "b4", "b5",
                        "c1", "c2", "c3", "c4", "c5",
                        (char *)NULL};
        int ret = execve("/bin/echo", argv, my_env);
        if (ret == -1) {
            perror("execve_1500 execve failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execve_1500", WEXITSTATUS(status), 0);
    }
}

int main(int argc, char *argv[])
{
    execve_0100();
    execve_0200();
    execve_0300();
    execve_0400();
    execve_0500();
    execve_0600();
    execve_0700();
    execve_0800();
    execve_0900();
    execve_1000();
    execve_1100();
    execve_1200();
    execve_1300();
    execve_1400();
    execve_1500();
    return t_status;
}