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
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : getchar_unlocked_0100
 * @tc.desc      : Verify that a character can be read from standard input.
 * @tc.level     : Level 0
 */
void getchar_unlocked_0100(void)
{
    bool flag = false;
    char str[] = "r\n";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT);
    write(fd, str, sizeof(str));
    FILE *fp = freopen("/data/readtest.txt", "r", stdin);
    char result = getchar_unlocked();
    if (result == 'r') {
        flag = true;
    }
    EXPECT_TRUE("getchar_unlocked_0100", flag);
    fclose(fp);
    close(fd);
    fclose(stdin);
    remove("/data/readtest.txt");
}

TEST_FUN G_Fun_Array[] = {
    getchar_unlocked_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}