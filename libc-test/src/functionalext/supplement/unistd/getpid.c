/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getpid_0100
 * @tc.desc      : Verify to get the current process ID (shell command and call getpid function).
 * @tc.level     : Level 0
 */
void getpid_0100(void)
{
    system("ps -eo command,pid | grep -E \"PID|getpid\" > "
           "/data/tests/libc-test/src/functionalext/supplement/unistd/ps.txt");
    char abc[256] = {0};
    bool successflag = false;
    const char *ptr = "/data/tests/libc-test/src/functionalext/supplement/unistd/ps.txt";
    FILE *fptr = fopen(ptr, "r");
    if (fptr) {
        while (!feof(fptr)) {
            fread(abc, sizeof(abc), 1, fptr);
            char num[8] = {0};
            int index = 0;
            for (int i = 0; i < (int)strlen(abc); i++) {
                if (abc[i] >= '0' && abc[i] <= '9') {
                    num[index++] += abc[i];
                }
            }
            int32_t intuid = atoi(num);
            pid_t uid = getpid();
            if (intuid == uid) {
                successflag = true;
            }
        }
    }
    EXPECT_TRUE("getpid_0100", successflag);
    fclose(fptr);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    getpid_0100();
    return t_status;
}