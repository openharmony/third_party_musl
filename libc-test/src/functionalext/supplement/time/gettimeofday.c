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

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

const int SUCCESS = 0;

/**
 * @tc.name      : gettimeofday_0100
 * @tc.desc      : Each parameter is valid, and the current system time can be obtained.
 * @tc.level     : Level 0
 */
void gettimeofday_0100(void)
{
    struct timeval tv;
    char str[10];
    system("date +%s > ./time.txt");
    FILE *fptr = fopen("time.txt", "r");
    fflush(fptr);
    fread(str, sizeof(str), 1, fptr);
    int sec = atoi(str);
    int returnflag = gettimeofday(&tv, NULL);
    EXPECT_EQ("gettimeofday_0100", returnflag, SUCCESS);
    EXPECT_EQ("gettimeofday_0100", (long)sec, (long)tv.tv_sec);
    remove("time.txt");
}

/**
 * @tc.name      : gettimeofday_0200
 * @tc.desc      : Each parameter is valid, and the current system time can be obtained.
 * @tc.level     : Level 2
 */
void gettimeofday_0200(void)
{
    struct timeval tv;
    int returnflag = gettimeofday(&tv, NULL);
    EXPECT_EQ("gettimeofday_0200", returnflag, SUCCESS);
}

int main()
{
    gettimeofday_0100();
    gettimeofday_0200();

    return t_status;
}