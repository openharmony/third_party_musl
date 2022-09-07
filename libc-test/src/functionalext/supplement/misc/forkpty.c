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
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : forkpty_0100
 * @tc.desc      : Each parameter is valid and can open a pair of pseudo-terminals for a new
 *                 session and create a processing process.
 * @tc.level     : Level 0
 */
void forkpty_0100(void)
{
    int master;
    pid_t pid = forkpty(&master, NULL, NULL, NULL);
    char *sign_r = "1";
    char *sign_f = "0";
    char list1[2];
    char list2[2];
    FILE *fp;
    if (pid < 0) {
        printf("error in fork!");
    } else if (pid == 0) {
        fp = fopen("test1.txt", "w+");
        fwrite(sign_r, sizeof(char), strlen(sign_r), fp);
        fclose(fp);
    } else {
        fp = fopen("test2.txt", "w+");
        fwrite(sign_r, sizeof(char), strlen(sign_r), fp);
        fclose(fp);
    }
    sleep(1);
    FILE *fp1 = fopen("test1.txt", "r");
    FILE *fp2 = fopen("test2.txt", "r");
    fread(list1, sizeof(list1), 1, fp1);
    fread(list2, sizeof(list2), 1, fp2);
    EXPECT_EQ("forkpty_0100", list1[0], '1');
    EXPECT_EQ("forkpty_0100", list2[0], '1');
    fclose(fp1);
    fclose(fp2);
    remove("test1.txt");
    remove("test2.txt");
}

TEST_FUN G_Fun_Array[] = {
    forkpty_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}