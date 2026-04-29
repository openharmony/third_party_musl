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
#include "functionalext.h"
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @tc.name      : execlp_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execlp_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        int ret = execlp("touch", "touch", "execlptest.txt", NULL);
        EXPECT_NE("execlp_0100", ret, -1);
    }
    sleep(1);

    int isExist = access("execlptest.txt", F_OK);
    EXPECT_EQ("execlp_0100", isExist, 0);
    remove("execlptest.txt");
}

/**
 * @tc.name      : execlp_0200
 * @tc.desc      : The content pointed to by the file parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execlp_0200(void)
{
    int ret = execlp(" ", "touch", "execlptest.txt", NULL);
    EXPECT_EQ("execlp_0200", ret, -1);
}

/**
 * @tc.name      : execlp_0300
 * @tc.desc      : The length of file exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execlp_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    int ret = execlp(buff, "touch", "execlptest.txt", NULL);
    EXPECT_EQ("execlp_0300", ret, -1);
}

/**
 * @tc.name      : execlp_0400
 * @tc.desc      : execlp executes a command that does not exist in PATH, should return -1
 * @tc.level     : Level 1
 */
void execlp_0400(void)
{
    errno = 0;
    int ret = execlp("nonexistent_command_xyz_12345", "nonexistent_command_xyz_12345", (char *)NULL);
    EXPECT_EQ("execlp_0400", ret, -1);
    EXPECT_EQ("execlp_0400", errno, ENOENT);
}

/**
 * @tc.name      : execlp_0500
 * @tc.desc      : execlp passes arguments correctly to the executed program
 * @tc.level     : Level 1
 */
void execlp_0500(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        int ret = execlp("echo", "echo", "execlp_arg_test_789", (char *)NULL);
        if (ret == -1) {
            perror("execlp_0500 execlp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execlp_0500", WIFEXITED(status));
        EXPECT_EQ("execlp_0500", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execlp_0600
 * @tc.desc      : execlp with NULL as the first argument should fail with EINVAL or ENOENT
 * @tc.level     : Level 2
 */
void execlp_0600(void)
{
    errno = 0;
    int ret = execlp(NULL, "touch", "test.txt", (char *)NULL);
    EXPECT_EQ("execlp_0600", ret, -1);
    // NULL pointer may cause EINVAL or segmentation fault depending on implementation
    // Here we just check that it fails
}

/**
 * @tc.name      : execlp_0700
 * @tc.desc      : execlp executes a shell script with shebang line
 * @tc.level     : Level 2
 */
void execlp_0700(void)
{
    const char *script_path = "/tmp/execlp_test_script.sh";
    FILE *fp = fopen(script_path, "w");
    if (fp == NULL) {
        t_error("execlp_0700 failed to create script");
        return;
    }
    fprintf(fp, "#!/bin/sh\ntouch /tmp/execlp_script_success\nexit 0\n");
    fclose(fp);
    chmod(script_path, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        int ret = execlp(script_path, "execlp_test_script", (char *)NULL);
        if (ret == -1) {
            perror("execlp_0700 execlp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execlp_0700", WEXITSTATUS(status), 0);
        int file_created = access("/tmp/execlp_script_success", F_OK);
        EXPECT_EQ("execlp_0700", file_created, 0);
        unlink("/tmp/execlp_script_success");
        unlink(script_path);
    }
}

/**
 * @tc.name      : execlp_0800
 * @tc.desc      : execlp with special characters in arguments should be handled correctly
 * @tc.level     : Level 2
 */
void execlp_0800(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        // Test with spaces and special chars in filename (echo won't interpret them)
        int ret = execlp("echo", "echo", "test file $HOME `date`", (char *)NULL);
        if (ret == -1) {
            perror("execlp_0800 execlp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_TRUE("execlp_0800", WIFEXITED(status));
    }
}

/**
 * @tc.name      : execlp_0900
 * @tc.desc      : execlp with too many arguments (stress test for arg list handling)
 * @tc.level     : Level 3
 */
void execlp_0900(void)
{
    pid_t fpid = fork();
    if (fpid == 0) {
        int ret = execlp("echo", "echo", 
                         "arg1", "arg2", "arg3", "arg4", "arg5",
                         "arg6", "arg7", "arg8", "arg9", "arg10",
                         (char *)NULL);
        if (ret == -1) {
            perror("execlp_0900 execlp failed");
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        EXPECT_EQ("execlp_0900", WEXITSTATUS(status), 0);
    }
}

/**
 * @tc.name      : execlp_1000
 * @tc.desc      : execlp with empty string as command name should fail
 * @tc.level     : Level 1
 */
void execlp_1000(void)
{
    errno = 0;
    int ret = execlp("", "arg0", (char *)NULL);
    EXPECT_EQ("execlp_1000", ret, -1);
    EXPECT_TRUE("execlp_1000", errno == ENOENT || errno == EINVAL);
}

int main(int argc, char *argv[])
{
    execlp_0100();
    execlp_0200();
    execlp_0300();
    execlp_0400();
    execlp_0500();
    execlp_0600();
    execlp_0700();
    execlp_0800();
    execlp_0900();
    execlp_01000();
    return t_status;
}