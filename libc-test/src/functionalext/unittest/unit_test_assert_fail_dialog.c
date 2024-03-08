/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <assert.h>
#include <info/fatal_message.h>

Assert_Status callbackFunction(AssertFailureInfo assert_fail) {
    printf("Fatal message: %s\n", assert_fail.expression);
    printf("Fatal message: %s\n", assert_fail.file);    
    printf("Fatal message: %s\n", assert_fail.function);
    printf("Fatal message: %d\n", assert_fail.line);
    Assert_Status res = ASSERT_ABORT;
    return res;
}

int main(void)
{
    printf("This is a log message before assert.\n");
    set_assert_callback(callbackFunction);
    assert(0);
    return 0;
}
