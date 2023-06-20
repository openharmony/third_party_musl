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
#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "util.h"

using namespace std;

static void Bm_function_Getaddrinfo_external_network(benchmark::State &state)
{
    struct addrinfo *res;
    for (auto _ : state) {
        int n = getaddrinfo("www.baidu.com", nullptr, nullptr, &res);
        if (n != 0) {
            perror("getaddrinfo external_network");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet1(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_PASSIVE;
    hint.ai_family = AF_UNSPEC;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet1");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Network_ntohs(benchmark::State &state)
{
    uint16_t srcPort = 5060;
    for (auto _ : state) {
        benchmark::DoNotOptimize(ntohs(srcPort));
    }
}
MUSL_BENCHMARK(Bm_function_Getaddrinfo_external_network);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet1);
MUSL_BENCHMARK(Bm_function_Network_ntohs);
#endif