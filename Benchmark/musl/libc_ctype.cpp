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
#include "ctype.h"
#include "util.h"

using namespace std;

#define TOTAL_COMMON_CHARACTERS 127

static void Bm_function_Tolower_a(benchmark::State &state)
{
    int c = 97;
    for (auto _ : state) {
        benchmark::DoNotOptimize(tolower(c));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Tolower_A(benchmark::State &state)
{
    int c = 65;
    for (auto _ : state) {
        benchmark::DoNotOptimize(tolower(c));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Tolower_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for (int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(tolower(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isspace_space(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isspace(' '));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isspace_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for(int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(isspace(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_a(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('a'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_A(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('A'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_0(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('0'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for(int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(isalnum(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Tolower_a);
MUSL_BENCHMARK(Bm_function_Tolower_A);
MUSL_BENCHMARK(Bm_function_Tolower_all_ascii);
MUSL_BENCHMARK(Bm_function_Isspace_space);
MUSL_BENCHMARK(Bm_function_Isspace_all_ascii);
MUSL_BENCHMARK(Bm_function_Isalnum_a);
MUSL_BENCHMARK(Bm_function_Isalnum_A);
MUSL_BENCHMARK(Bm_function_Isalnum_0);
MUSL_BENCHMARK(Bm_function_Isalnum_all_ascii);