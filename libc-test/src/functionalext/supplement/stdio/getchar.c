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
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getchar_0100
 * @tc.desc      : Verify that the characters in the file can be get
 * @tc.level     : Level 0
 */
void getchar_0100(void)
{
    bool flag = false;
    char str[] = "r\n";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT);
    write(fd, str, sizeof(str));
    FILE *fp = freopen("/data/readtest.txt", "r", stdin);
    char reschar;
    reschar = getchar();
    if (reschar == 'r')
    {
        flag = true;
    }
    EXPECT_TRUE("getchar_0100", flag);
    fclose(stdin);
    close(fd);
    remove("/data/readtest.txt");
}

int main()
{
    getchar_0100();
    return t_status;
}