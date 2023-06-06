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
#include "malloc.h"
#include "stdlib.h"
#include "util.h"

using namespace std;

#define MALLOC_SIZE 2

static void Bm_function_realloc_twice(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size * 2));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_realloc_half(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size / 2));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_realloc_equal(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_malloc_usable_size(benchmark::State &state)
{
    char *p =  (char *)malloc(MALLOC_SIZE);
    if (p == nullptr) {
        perror("malloc Je_malloc_usable_size");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(malloc_usable_size(p));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_realloc_twice);
MUSL_BENCHMARK(Bm_function_realloc_half);
MUSL_BENCHMARK(Bm_function_realloc_equal);
MUSL_BENCHMARK(Bm_function_malloc_usable_size);