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
#include <stdarg.h>
#include <stdlib.h>
#include <err.h>
#include "test.h"

/*
 * @tc.name      : veerx_0100
 * @tc.desc      : The test verrx method prints the error message
 * @tc.level     : Level 0
 */
void veerx_0100(int status, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrx(status, fmt, ap);
    va_end(ap);
}

int main(int argc, char *argv[])
{
    veerx_0100(t_status, "veerx_0100 printf info: %s", "this is msg");
    return t_status;
}