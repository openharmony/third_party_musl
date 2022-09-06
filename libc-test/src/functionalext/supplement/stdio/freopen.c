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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdbool.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : freopen_0100
 * @tc.desc      : Each parameter is valid, f is stdin, which can realize input redirection.
 * @tc.level     : Level 0
 */
void freopen_0100(void)
{
    int flag = false;
    FILE *fp;
    FILE *fd;
    const char *ptr = "/data/tests/libc-test/src/functionalext/supplement/stdio/freopen.txt";
    fd = freopen(ptr, "w+", stdin);
    if (fd != 0) {
        flag = true;
    }
    EXPECT_TRUE("freopen_0100", flag);
    fclose(stdin);
    fclose(fd);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    freopen_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}