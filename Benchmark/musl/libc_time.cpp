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
#include "time.h"
#include "util.h"

using namespace std;

// Used to put the current thread to sleep for the specified time
static void Bm_function_Nanosleep_0ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

static void Bm_function_Nanosleep_10ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

static void Bm_function_Nanosleep_100ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 100;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

// Used to set information about the time zone
static void Bm_function_Tzset(benchmark::State &state)
{
    while (state.KeepRunning()) {
        tzset();
    }
}
static void Bm_function_Clock_nanosleep_realtime(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_realtime_raw(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME_COARSE, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_realtime_coarse(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME_COARSE, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_monotonic(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_monotonic_coarse(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC_COARSE, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_monotonic_raw(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC_RAW, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_boottime(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 10;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_BOOTTIME, 0, &req, &rem));
    }
}

#define BUFFER_SIZE 32

static void Bm_function_Strftime(benchmark::State &state)
{
    time_t rawTime = time(nullptr);
    struct tm *localTime = localtime(&rawTime);
    char buf[BUFFER_SIZE];
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(strftime(buf, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localTime));
    }
}

MUSL_BENCHMARK(Bm_function_Nanosleep_0ns);
MUSL_BENCHMARK(Bm_function_Nanosleep_10ns);
MUSL_BENCHMARK(Bm_function_Nanosleep_100ns);
MUSL_BENCHMARK(Bm_function_Tzset);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime_raw);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime_coarse);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic_raw);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic_coarse);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_boottime);
MUSL_BENCHMARK(Bm_function_Strftime);
