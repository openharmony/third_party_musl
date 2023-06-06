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

#include <benchmark/benchmark.h>
#include "stdlib.h"
#include "stdio.h"
#include "util.h"

using namespace std;
#define BUFFER 100

#ifdef ONO_CURRENT_INTERFACE
int compare(const void *a, const void *b)
{
    int num1 = *(int *)a;
    int num2 = *(int *)b;
    if (num1 < num2) {
        return -1;
    } else if (num1 > num2) {
        return 1;
    } else {
        return 0;
    }
}

// Convert the string pointed to by str to floating point
static void Bm_function_Strtod(benchmark::State &state)
{
    const char *var[] = {"+2.86500000e+01", "3.1415", "29",
                         "-123.456", "1.23e5", "0x1.2p3",
                         "-inf", "123foo"};
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtod(str, &ptr));
    }
}

// Used to sort elements in an array
static void Bm_function_Qsort(benchmark::State &state)
{
    int arr[] = {5, 3, 7, 1, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    for (auto _ : state) {
        qsort(arr, n, sizeof(int), compare);
    }
}
#endif

static void Bm_function_Getenv_TZ(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("TZ"));
    }
}

static void Bm_function_Getenv_LD_LIBRARY_PATH(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LD_LIBRARY_PATH"));
    }
}

static void Bm_function_Getenv_LD_PRELOAD(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LD_PRELOAD"));
    }
}

static void Bm_function_Getenv_LC_ALL(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LC_ALL"));
    }
}

static void Bm_function_Getenv_LOGNAME(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LOGNAME"));
    }
}

#ifdef ONO_CURRENT_INTERFACE
// Converts any relative path to an absolute path
static void Bm_function_Realpath(benchmark::State &state)
{
    const char *realpathvariable[] = {"./log", "../dev", "log/hilog", "../dev/zero", "/dev"};
    const char *path = realpathvariable[state.range(0)];
    char resolved_path[BUFFER];
    for (auto _ : state) {
        benchmark::DoNotOptimize(realpath(path, resolved_path));
    }
}

MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtod, "BENCHMARK_VARIABLE");
MUSL_BENCHMARK(Bm_function_Qsort);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Realpath, "REALPATH_VARIABLE");
#endif

MUSL_BENCHMARK(Bm_function_Getenv_TZ);
MUSL_BENCHMARK(Bm_function_Getenv_LD_LIBRARY_PATH);
MUSL_BENCHMARK(Bm_function_Getenv_LD_PRELOAD);
MUSL_BENCHMARK(Bm_function_Getenv_LC_ALL);
MUSL_BENCHMARK(Bm_function_Getenv_LOGNAME);