/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef GWP_ASAN_TEST_H
#define GWP_ASAN_TEST_H

#include <stdbool.h>
#include <stdlib.h>
#include "gwp_asan.h"

void config_gwp_asan_environment(bool logPath)
{
    system("param set gwp_asan.sample.all true");
    may_init_gwp_asan(true);
    if (logPath) {
        system("param set gwp_asan.log.path file");
    }
}

void cancel_gwp_asan_environment(bool logPath)
{
    if (logPath) {
        system("param set gwp_asan.log.path default");
    }
    system("param set gwp_asan.sample.all false");
}

#endif