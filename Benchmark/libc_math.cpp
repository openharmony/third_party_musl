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
#include "math.h"
#include "util.h"

using namespace std;

#ifdef ONO_CURRENT_INTERFACE
#define PI 3.14159265
#define FLT_MIN 1.175494351e-38F

// The function generates a value that has the size of the parameter x and the symbol of the parameter y
static void Bm_function_Copysignl(benchmark::State &state)
{
    long double x = 2417851639229258349412352.000000;
    long double y = 6051711999279104.000000;
    for (auto _ : state) {
        // x is the result, y is the symbol
        benchmark::DoNotOptimize(copysignl(x, y));
    }
    state.SetItemsProcessed(state.iterations());
}

// remainder
static void Bm_function_Fmodl(benchmark::State &state)
{
    long double x = 6051711999279104.000000;
    long double y = 536870912.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmodl(x, y));
    }
    state.SetItemsProcessed(state.iterations());
}

#endif

// The 3.14th power of e
static void Bm_function_Exp(benchmark::State &state)
{
    double x = 3.14;
    for (auto _ : state) {
        benchmark::DoNotOptimize(exp(x));
    }
    state.SetItemsProcessed(state.iterations());
}

#ifdef ONO_CURRENT_INTERFACE
// The logarithm of base e and x
static void Bm_function_Log(benchmark::State &state)
{
    double x = 3.14;
    for (auto _ : state) {
        benchmark::DoNotOptimize(log(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// Returns the cosine of the radian angle x
static void Bm_function_Cos(benchmark::State &state)
{
    double x = 6.302892;
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(cos(x * val));
    }
    state.SetItemsProcessed(state.iterations());
}

// Break floating point number into mantissa and exponent
static void Bm_function_Frexpl(benchmark::State &state)
{
    long double x = 1024.0020;
    int e;
    for (auto _ : state) {
        benchmark::DoNotOptimize(frexpl(x, &e));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn1(benchmark::State &state)
{
    double x = 3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn2(benchmark::State &state)
{
    double x = 10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn3(benchmark::State &state)
{
    double x = -100.9;
    int n = -100;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn4(benchmark::State &state)
{
    double x = -100.9;
    int n = 100;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl1(benchmark::State &state)
{
    long double x = (long double)2e-10;
    int n = -16384;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl2(benchmark::State &state)
{
    long double x = (long double)2e-10;
    int n = 16384;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl3(benchmark::State &state)
{
    long double x = -PI;
    int n = 1000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl4(benchmark::State &state)
{
    long double x = -PI;
    int n = -1000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fmod(benchmark::State &state)
{
    double x = 10.5;
    double y = 3.2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmod(x, y));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_sin(benchmark::State &state)
{
    double x = 45.0;
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sin(x*val));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_INFINITE  The specified value is positive or negative infinity
static void Bm_function_fpclassifyl(benchmark::State &state)
{
    long double x = log(0.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_SUBNORMAL  The specified value is a positive or negative normalization value
static void Bm_function_fpclassifyl1(benchmark::State &state)
{
    long double x = (FLT_MIN / 2.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_NAN  The specified value is not a number
static void Bm_function_fpclassifyl2(benchmark::State &state)
{
    long double x = sqrt(-1.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_ZERO  Specify a value of zero
static void Bm_function_fpclassifyl3(benchmark::State &state)
{
    long double x = -0.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Copysignl);
MUSL_BENCHMARK(Bm_function_Fmodl);
MUSL_BENCHMARK(Bm_function_Log);
MUSL_BENCHMARK(Bm_function_Cos);
MUSL_BENCHMARK(Bm_function_Frexpl);
MUSL_BENCHMARK(Bm_function_Scalbn1);
MUSL_BENCHMARK(Bm_function_Scalbn2);
MUSL_BENCHMARK(Bm_function_Scalbn3);
MUSL_BENCHMARK(Bm_function_Scalbn4);
MUSL_BENCHMARK(Bm_function_Scalbnl1);
MUSL_BENCHMARK(Bm_function_Scalbnl2);
MUSL_BENCHMARK(Bm_function_Scalbnl3);
MUSL_BENCHMARK(Bm_function_Scalbnl4);
MUSL_BENCHMARK(Bm_function_Fmod);
MUSL_BENCHMARK(Bm_function_sin);
MUSL_BENCHMARK(Bm_function_fpclassifyl);
MUSL_BENCHMARK(Bm_function_fpclassifyl1);
MUSL_BENCHMARK(Bm_function_fpclassifyl2);
MUSL_BENCHMARK(Bm_function_fpclassifyl3);
#endif

MUSL_BENCHMARK(Bm_function_Exp);