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
#include "signal.h"
#include "util.h"

static void SignalHandler(int i) {}

static void Bm_function_Sigaction(benchmark::State &state)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = SignalHandler;

    for (auto _ : state) {
        benchmark::DoNotOptimize(sigaction(SIGUSR1, &sa, nullptr));
    }
}

MUSL_BENCHMARK(Bm_function_Sigaction);