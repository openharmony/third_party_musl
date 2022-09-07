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
#include <fcntl.h>
#include <unistd.h>
#include "functionalext.h"

#define MAX 10

/**
 * @tc.name      : brk_0100
 * @tc.desc      : Verify Change the location of program breakpoints
 * @tc.level     : Level 0
 */
void brk_0100(void)
{
    int *p, n = 0, max = MAX;
    FILE *fp;
    int result;
    p = (int *)malloc(MAX * sizeof(int));
    max++;
    result = brk(p);
#ifdef __BSD_SOURCE
    EXPECT_EQ("brk_0100", result, 0);
#endif
}

int main()
{
    brk_0100();
    return t_status;
}