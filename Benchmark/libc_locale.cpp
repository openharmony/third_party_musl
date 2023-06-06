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

#ifdef ONO_CURRENT_INTERFACE
#include <benchmark/benchmark.h>
#include <locale.h>
#include "sys/types.h"
#include "sys/epoll.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdarg.h"
#include "wchar.h"
#include "time.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "util.h"
#include "sys/syscall.h"

using namespace std;

static void Bm_function_Setlocale_All(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, NULL));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_All1(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, "C"));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_All2(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, ""));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Collate(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_COLLATE, NULL));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Ctype(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_CTYPE, NULL));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Time(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_TIME, NULL));
    }
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Setlocale_All);
MUSL_BENCHMARK(Bm_function_Setlocale_All1);
MUSL_BENCHMARK(Bm_function_Setlocale_All2);
MUSL_BENCHMARK(Bm_function_Setlocale_Collate);
MUSL_BENCHMARK(Bm_function_Setlocale_Ctype);
MUSL_BENCHMARK(Bm_function_Setlocale_Time);
#endif
