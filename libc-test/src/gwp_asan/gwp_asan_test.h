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
#include <stdio.h>
#include <stdlib.h>
#include "gwp_asan.h"

void config_gwp_asan_environment(bool logPath)
{
    system("param set gwp_asan.sample.all true");

    // If gwp_asan is inited by __dls3,
    // it just samples some allocations randomly,
    // the test may not be able to trigger the error,
    // so just skip the test.
    if (!may_init_gwp_asan(true)) {
        system("param set gwp_asan.sample.all false");
        printf("Skip gwp_asan test because it may be inited by __dls3 randomly, just rerun the test.\n");
        exit(0);
    }

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