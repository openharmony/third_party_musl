/**
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
#include <stdint.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
int __cxa_atexit(void (*func)(void *), void *arg, void *dso);
int __aeabi_atexit(void *obj, void (*func)(void *), void *d)
{
    return __cxa_atexit(func, obj, d);
}
const int32_t COUNT_ZERO = 0;

/*
 * @tc.name      : aeabi_atexit_0100
 * @tc.desc      : Verify that __aeabi_atexit is called correctly (all arguments are valid)
 * @tc.level     : Level 0
 */
void aeabi_atexit_0100()
{
    int result;
    result = __aeabi_atexit(NULL, NULL, NULL);
    EXPECT_EQ("aeabi_atexit_0100", result, COUNT_ZERO);
}

TEST_FUN G_Fun_Array[] = {
    aeabi_atexit_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}