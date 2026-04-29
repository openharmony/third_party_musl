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

#include <errno.h>
#include <unistd.h>
#include "test.h"
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

/**
 * @tc.name      : execl_0100
 * @tc.desc      : The file in the specified directory can be executed
 * @tc.level     : Level 0
 */
void execl_0100(void)
{
    errno = 0;
    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);
        if (result == -1) {
            t_error("%s execl failed\n", __func__);
        }
        if (errno != 0) {
            t_error("%s failed, errno is %d\n", __func__, errno);
        }
    }
}

/**
 * @tc.name      : execl_0200
 * @tc.desc      : execl executes a non-existent file, should return -1 and set errno to ENOENT
 * @tc.level     : Level 1
 */
void execl_0200(void)
{
    errno = 0;
    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl("/bin/nonexistent_file_xyz", "nonexistent_file_xyz", (char *)0);
        if (result != -1) {
            t_error("%s execl should fail for non-existent file\n", __func__);
            _exit(1);
        }
        if (errno != ENOENT) {
            t_error("%s failed, errno expected ENOENT(%d), but got %d\n", 
                    __func__, ENOENT, errno);
            _exit(1);
        }
        _exit(0); 
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("%s child process exited with error\n", __func__);
        }
    } else {
        t_error("%s fork failed\n", __func__);
    }
}

/**
 * @tc.name      : execl_0300
 * @tc.desc      : execl executes a file without execute permission, should fail with EACCES
 * @tc.level     : Level 1
 */
void execl_0300(void)
{
    errno = 0;
    const char *tmp_file = "/tmp/musl_test_noexec";
    FILE *fp = fopen(tmp_file, "w");
    if (fp == NULL) {
        t_error("%s failed to create temp file\n", __func__);
        return;
    }
    fprintf(fp, "#!/bin/sh\necho hello\n");
    fclose(fp);
    chmod(tmp_file, 0644);

    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl(tmp_file, "musl_test_noexec", (char *)0);
        if (result != -1) {
            t_error("%s execl should fail for no-exec permission file\n", __func__);
            _exit(1);
        }
        if (errno != EACCES && errno != EPERM) {
            t_error("%s failed, errno expected EACCES/EPERM, but got %d\n", __func__, errno);
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("%s child process exited with error\n", __func__);
        }
        unlink(tmp_file);
    } else {
        t_error("%s fork failed\n", __func__);
        unlink(tmp_file);
    }
}

/**
 * @tc.name      : execl_0400
 * @tc.desc      : execl passes arguments correctly to the executed program
 * @tc.level     : Level 1
 */
void execl_0400(void)
{
    errno = 0;
    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl("/bin/echo", "echo", "musl_execl_test_success", (char *)0);
        if (result == -1) {
            t_error("%s execl failed, errno is %d\n", __func__, errno);
            _exit(1);
        }
        _exit(0);
    }

}

/**
 * @tc.name      : execl_0500
 * @tc.desc      : execl with relative path should work if file exists in current directory
 * @tc.level     : Level 2
 */
void execl_0500(void)
{
    errno = 0;
    const char *rel_script = "./musl_rel_test.sh";
    FILE *fp = fopen(rel_script, "w");
    if (fp == NULL) {
        t_error("%s failed to create temp script\n", __func__);
        return;
    }
    fprintf(fp, "#!/bin/sh\nexit 0\n");
    fclose(fp);
    chmod(rel_script, 0755);

    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl(rel_script, "musl_rel_test", (char *)0);
        if (result == -1) {
            t_error("%s execl with relative path failed, errno is %d\n", __func__, errno);
            _exit(1);
        }
        _exit(0);
    } else if (fpid > 0) {
        int status = 0;
        waitpid(fpid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("%s relative path execl child exited with error\n", __func__);
        }
        unlink(rel_script);
    } else {
        t_error("%s fork failed\n", __func__);
        unlink(rel_script);
    }
}

int main(int argc, char *argv[])
{
    execl_0100();
    execl_0200();
    execl_0300();
    execl_0400();
    execl_0500();
    return t_status;
}