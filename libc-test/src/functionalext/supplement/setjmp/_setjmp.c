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

bool FLAG = false;
static jmp_buf envbuf;
void second()
{
    FLAG = true;
    longjmp(envbuf, 5);
}
void first()
{
    FLAG = false;
    second();
    FLAG = false;
}

/**
 * @tc.name      : _setjmp_0100
 * @tc.desc      : Verify that jumps between functions can be achieved
 * @tc.level     : Level 0
 */
void _setjmp_0100(void)
{
    if (setjmp(envbuf) == 0) {
        first();
    } else {
        EXPECT_TRUE("_setjmp_0100", FLAG);
    }
}

int main()
{
    _setjmp_0100();
    return t_status;
}