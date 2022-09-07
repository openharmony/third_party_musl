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
#include <stdio_ext.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "functionalext.h"

const size_t COUNT_ZERO = 0;

/**
 * @tc.name      : __fbufsize_0100
 * @tc.desc      : Verify that you can get the size of the buffer used by the file
                  (In bytes. Valid, file size greater than 0)
 * @tc.level     : Level 0
 */
void __fbufsize_0100(void)
{
    bool flag = false;
    FILE *fptr = fopen("test.txt", "w+");
    fwrite("1", 1, 1, fptr);
    size_t result = 0;
    result = __fbufsize(fptr);
    if (result > 0)
    {
        flag = true;
    }
    EXPECT_TRUE("__fbufsize_0200", flag);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : __fbufsize_0200
 * @tc.desc      : Verify that you can get the size of the buffer used by the file
                  (In bytes. File size = 0)
 * @tc.level     : Level 0
 */
void __fbufsize_0200(void)
{
    FILE *fptr = fopen("test.txt", "w+");
    setbuf(fptr, NULL);
    size_t result = 0;
    result = __fbufsize(fptr);
    EXPECT_EQ("__fbufsize_0200", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

int main()
{
    __fbufsize_0100();
    __fbufsize_0200();
    return t_status;
}