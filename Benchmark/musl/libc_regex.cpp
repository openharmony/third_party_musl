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

#include <regex.h>
#include <vector>
#include "util.h"

static const std::vector<int> regCompFlags {
    0,
    REG_EXTENDED,
    REG_ICASE,
    REG_NEWLINE,
    REG_NOSUB
};

static void PrepareRegcompArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : regCompFlags) {
        b->Args({l});
    }
}

static void Bm_function_Regcomp(benchmark::State &state)
{
    const char* pattern = "hello.*world";
    int flag = state.range(0);
    for (auto _state: state) {
        regex_t reg;
        benchmark::DoNotOptimize(regcomp(&reg, pattern, flag));
	regfree(&reg);
    }
}

MUSL_BENCHMARK_WITH_APPLY(Bm_function_Regcomp, PrepareRegcompArgs);

