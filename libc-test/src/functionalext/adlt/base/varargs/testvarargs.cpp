/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <cstdarg>
#include <cstdio>

#include "testvarargs.h"

namespace testvarargs {
int simpleSumFunction(int a, int b) {
    printf("Entering simple SUM function\n");
    return a + b;
}

// count is the number of following arguments
// if you want to sum 5 numbers then count should be 5, followed by 5 arguments
int varargsSumFunction(int count, ...) {
    printf("Entering variadic arguments SUM function\n");
    int result = 0;

    std::va_list list;
    va_start(list, count);

    for (int arg = 0; arg < count; ++arg) {
        result += va_arg(list, int);
    }

    va_end(list);

    return result;
}

} // namespace testvarargs