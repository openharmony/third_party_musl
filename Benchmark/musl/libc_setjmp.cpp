/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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

#include "csetjmp"
#include "util.h"

static jmp_buf g_testJmpBuf;

static void Bm_function_Setjmp(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setjmp(g_testJmpBuf));
    }
}

MUSL_BENCHMARK(Bm_function_Setjmp);