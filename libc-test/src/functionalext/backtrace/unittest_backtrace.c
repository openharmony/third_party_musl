/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <execinfo.h>
#include <stdlib.h>
#include "functionalext.h"

#define ZERO_LENGTH 0
#define INVALID_LENGTH (-1)
#define VALID_LENGTH 1
#define ZERO_RESULT 0
#define BT_BUF_SIZE 100

void backtrace_1001()
{
    void *frames[10];
    EXPECT_EQ("backtrace_1001", ZERO_RESULT, backtrace(frames, ZERO_LENGTH));
}

int backtrace_1002_supplement()
{
    void *frames[10];
    return backtrace(frames, VALID_LENGTH);
}

void backtrace_1002()
{
    EXPECT_LT("backtrace_1002", 0, backtrace_1002_supplement());
}

void backtrace_1003()
{
    void *bt_buffer[BT_BUF_SIZE];
    int bt_size = backtrace(bt_buffer, BT_BUF_SIZE);
    char **bt_strings = backtrace_symbols(bt_buffer, bt_size);
    EXPECT_PTRNE("backtrace_1003", bt_strings, NULL);
    free(bt_strings);
}

int main()
{
    backtrace_1001();
    backtrace_1002();
    backtrace_1003();
    return t_status;
}