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

static void Bm_function_Getaddrinfo_intranet2(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_ALL;  // Find IPv4 and IPV6
    hint.ai_family = AF_UNSPEC; // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet2");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet3(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_V4MAPPED; // If IPV6 is not found, return IPv4 mapped in ipv6 format
    hint.ai_family = AF_INET6;   // IPv6 Internet Domain
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet3");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet4(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_V4MAPPED;  // If IPV6 is not found, return IPv4 mapped in ipv6 format
    hint.ai_family = AF_UNSPEC;   // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet4");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet5(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_ADDRCONFIG; // Query the configured address type IPV4 or IPV6
    hint.ai_family = AF_UNSPEC;    // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet5");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet6(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_NUMERICSERV;
    hint.ai_family = AF_UNSPEC;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet6");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(n);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_intranet7(benchmark::State &state)
{
    struct addrinfo *res;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, nullptr, &res);
        if (n != 0) {
            perror("getaddrinfo intranet7");
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
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet2);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet3);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet4);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet5);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet6);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_intranet7);
MUSL_BENCHMARK(Bm_function_Network_ntohs);