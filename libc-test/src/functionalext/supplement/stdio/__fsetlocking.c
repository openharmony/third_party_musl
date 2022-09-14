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
#include <stdint.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : __fsetlocking_0100
 * @tc.desc      : Function is not implemented, always return 0
 * @tc.level     : Level 0
 */
void __fsetlocking_0100(void)
{
    FILE *fptr = fopen("test.txt", "w+");
    int result;
    result = __fsetlocking(fptr, FSETLOCKING_INTERNAL);
    EXPECT_EQ("__fsetlocking_0100", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

int main(int argc, char *argv[])
{
    __fsetlocking_0100();
    return t_status;
}