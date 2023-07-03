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

#define PI 3.14159265
#define FLT_MIN 1.175494351e-38F

static const double DOUBLE_VALUES[] = { 0.1, 10.0, -100.0, 0.0001, 5.14e11, -0.0001, 10000000.0, -100000000.0 };
static const double COSSIN_VALUES[] = { -334.143, -2.0, -1.0, 0.0, 0.5, 1.0, 243.01, 3.14 };
static const long double DIVIDEND_VALUES[] = { 10.5L, -10.5L, 2.71L, -2.71L, 3.14159265358979323846L,
                                               -3.14159265358979323846L, 1.0 / 3.0L, -1.0 / 3.0L };
static const long double DIVISOR_VALUES[] = { 3.0L, -3.0L, 1.414L, -1.414L, 0.5L, -0.5L,
                                              2.7182818284590452354L, -2.7182818284590452354L };
// The function generates a value that has the size of the parameter x and the symbol of the parameter y
static void Bm_function_Copysignl_Allpositive(benchmark::State &state)
{
    long double x = 2417851639229258349412352.000000;
    long double y = 6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Allnegative(benchmark::State &state)
{
    long double x = -2417851639229258349412352.000000;
    long double y = -6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Np(benchmark::State &state)
{
    long double x = -2417851639229258349412352.000000;
    long double y = 6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Pn(benchmark::State &state)
{
    long double x = 2417851639229258349412352.000000;
    long double y = -6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

// remainder(long doubel)
static void Bm_function_Fmodl(benchmark::State &state)
{
    long double x = DIVIDEND_VALUES[state.range(0)];
    long double y = DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmodl(x, y));
    }
}

// The 3.14th power of e
static void Bm_function_Exp(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(exp(x));
    }
}

// The logarithm of base e and x
static void Bm_function_Log(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(log(x));
    }
}

// Returns the cosine of the radian angle x
static void Bm_function_Cos(benchmark::State &state)
{
    double x = COSSIN_VALUES[state.range(0)];
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(cos(x * val));
    }
}

// Break floating point number into mantissa and exponent
static void Bm_function_Frexpl(benchmark::State &state)
{
    long double x = DIVIDEND_VALUES[state.range(0)];
    int e;
    for (auto _ : state) {
        benchmark::DoNotOptimize(frexpl(x, &e));
    }
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

static void Bm_function_Scalbnf1(benchmark::State &state)
{
    float x = 3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf2(benchmark::State &state)
{
    float x = 10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf3(benchmark::State &state)
{
    float x = -3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf4(benchmark::State &state)
{
    float x = -10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// remainder
static void Bm_function_Fmod(benchmark::State &state)
{
    double x = (double)DIVIDEND_VALUES[state.range(0)];
    double y = (double)DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmod(x, y));
    }
}

// Returns the sine of the radian angle x
static void Bm_function_Sin(benchmark::State &state)
{
    double x = COSSIN_VALUES[state.range(0)];
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sin(x*val));
    }
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

static void Bm_function_Expm1(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(expm1(x));
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Copysignl_Allpositive);
MUSL_BENCHMARK(Bm_function_Copysignl_Allnegative);
MUSL_BENCHMARK(Bm_function_Copysignl_Np);
MUSL_BENCHMARK(Bm_function_Copysignl_Pn);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmodl, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Log, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Cos, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Frexpl, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Scalbn1);
MUSL_BENCHMARK(Bm_function_Scalbn2);
MUSL_BENCHMARK(Bm_function_Scalbn3);
MUSL_BENCHMARK(Bm_function_Scalbn4);
MUSL_BENCHMARK(Bm_function_Scalbnl1);
MUSL_BENCHMARK(Bm_function_Scalbnl2);
MUSL_BENCHMARK(Bm_function_Scalbnl3);
MUSL_BENCHMARK(Bm_function_Scalbnl4);
MUSL_BENCHMARK(Bm_function_Scalbnf1);
MUSL_BENCHMARK(Bm_function_Scalbnf2);
MUSL_BENCHMARK(Bm_function_Scalbnf3);
MUSL_BENCHMARK(Bm_function_Scalbnf4);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmod, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Sin, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_fpclassifyl);
MUSL_BENCHMARK(Bm_function_fpclassifyl1);
MUSL_BENCHMARK(Bm_function_fpclassifyl2);
MUSL_BENCHMARK(Bm_function_fpclassifyl3);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Expm1, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Exp, "BENCHMARK_8");