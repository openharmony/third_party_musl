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
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "functionalext.h"

const int32_t COUNT_NEGATIVE = -1;
char *my_env[] = {"THIS=environment will be", "PASSED=to new process by", "the EXEC=functions", NULL};
int result_1;
int result_2;

/**
 * @tc.name      : execvpe_0100
 * @tc.desc      : Verify that the specified file can be executed
 *                 (all parameters are valid, PATH is not empty by default)
 * @tc.level     : Level 0
 */
void execvpe_0100(void)
{
    bool flag = false;
    char *path;
    path = getenv("touch");
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "/data/test.txt", 0};
        result_1 = execvpe("touch", argv, &path);
    }
    sleep(1);
    FILE *fptr = fopen("/data/test.txt", "r");
    if (fptr != NULL) {
        flag = true;
    }
    EXPECT_NE("execvpe_0100", result_1, COUNT_NEGATIVE);
    EXPECT_TRUE("execvpe_0100", flag);
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
    bool flag = false;
    char *path = getenv("touch");
    setenv("touch", "\0", 1);
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "/data/test.txt", 0};
        result_2 = execvpe("touch", argv, &path);
    }
    sleep(1);
    FILE *fptr = fopen("/data/test.txt", "r");
    if (fptr != NULL) {
        flag = true;
    }
    fclose(fptr);
    remove("/data/test.txt");
    EXPECT_NE("execvpe_0200", result_2, COUNT_NEGATIVE);
    EXPECT_TRUE("execvpe_0200", flag);
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
    int result;
    char buff[] = "\0";
    char *argv[] = {"ls", "-al", "/etc/passwd", 0};
    result = execvpe(buff, argv, my_env);
    EXPECT_EQ("execvpe_0300", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : execvpe_0400
 * @tc.desc      : Verify that the specified file cannot be executed (file argument is invalid, file
 *                 length exceeds NAME_MAX)
 * @tc.level     : Level 2
 */
void execvpe_0400(void)
{
    int result;
    char *argv[] = {"ls", "-al", "/etc/passwd", 0};
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    result = execvpe(buff, argv, my_env);
    EXPECT_EQ("execvpe_0400", result, COUNT_NEGATIVE);
}

int main(int argc, char *argv[])
{
    execvpe_0100();
    execvpe_0200();
    execvpe_0300();
    execvpe_0400();
    return t_status;
}