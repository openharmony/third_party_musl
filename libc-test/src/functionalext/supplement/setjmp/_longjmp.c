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
#include <stdbool.h>
#include <setjmp.h>
#include "functionalext.h"

static jmp_buf buf;
bool FLAG = false;
void second()
{
    longjmp(buf, 1);
}
void first()
{
    FLAG = true;
    second();
    FLAG = false;
}

/**
 * @tc.name      : _longjmp_0100
 * @tc.desc      : Verify that jumps between functions can be achieved
 * @tc.level     : Level 0
 */
void _longjmp_0100(void)
{
    if (!setjmp(buf)) {
        first();
    } else {
        EXPECT_TRUE("_longjmp_0100", FLAG);
    }
}

int main()
{
    _longjmp_0100();
    return t_status;
}