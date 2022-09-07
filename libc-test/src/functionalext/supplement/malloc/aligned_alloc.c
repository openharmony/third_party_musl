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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "functionalext.h"

/**
 * @tc.name      : aligned_alloc_0100
 * @tc.desc      : Verify that memory allocation is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void aligned_alloc_0100(void)
{
    bool flag = false;
    int pagesize = getpagesize();
    int *p1 = (int *)aligned_alloc(pagesize, 1024 * sizeof(int));
    if (p1 != 0) {
        flag = true;
    }
    EXPECT_TRUE("aligned_alloc_0100", flag);
}

int main(void)
{
    aligned_alloc_0100();
    return t_status;
}