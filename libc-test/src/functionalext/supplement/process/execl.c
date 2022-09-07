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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_NEFATIVE = -1;
int result;

/**
 * @tc.name      : execl_0100
 * @tc.desc      : The file in the specified directory can be executed
 * @tc.level     : Level 0
 */
void execl_0100()
{
    mkdir("/data/tests/libc-test/src/functionalext/supplement/process/test", 0777);
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        result = execl("/bin/ls", "ls", "/data/tests/libc-test/src/functionalext/supplement/process/test/", (char *)0);
    }
    sleep(1);
    EXPECT_NE("execl_0100", result, COUNT_NEFATIVE);
}

TEST_FUN G_Fun_Array[] = {
    execl_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}