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
#include "string.h"
#include "util.h"

using namespace std;

typedef struct {
    char name[20];
    int age;
} Nier;

int CompareInt(const void *a, const void *b)
{
    int c = *(int *)a;
    int d = *(int *)b;
    if (c < d) {
        return -1;
    } else if (c > d) {
        return 1;
    } else {
        return 0;
    }
}

int CompareDouble(const void *a, const void *b)
{
    double c = *(double *)a;
    double d = *(double *)b;
    if (c == d) {
        return 0;
    } else if (c > d) {
        return 1;
    } else {
        return -1;
    }
}

int CompareString(const void *a, const void *b)
{
    const char* c = (char *)a;
    const char* d = (char *)b;
    return strcmp(c, d);
}

int CompareStruct(const void *a, const void *b)
{
    return strcmp(((Nier *)a)->name, ((Nier *)b)->name);
}

void InitRandomArray(int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        arr[i] = rand() % (2 * n);
    }
}

// Convert the string pointed to by str to floating point
static void Bm_function_Strtod(benchmark::State &state)
{
    const char *var[] = { "+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo" };
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtod(str, &ptr));
    }
}

static void Bm_function_Strtof(benchmark::State &state)
{
     const char *var[] = {"+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo"};
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtof(str, &ptr));
    }
}

static void Bm_function_Strtold(benchmark::State &state)
{
     const char *var[] = {"+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo"};
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtold(str, &ptr));
    }
}

// Used to sort elements in an array
// int type
static void Bm_function_Qsortint(benchmark::State &state)
{
    for (auto _ : state) {
        int arr[] = { 12, 89, 5, 3, 7, 1, 9, 2, 6 };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(int), CompareInt);
    }
}

// double type
static void Bm_function_Qsortdouble(benchmark::State &state)
{
    for (auto _ : state) {
        double arr[] = { 34.541, 5.32, 3.56, 7.897, 1.2324, 9.34543, 5.324, 98.543, 34.665 };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(double), CompareDouble);
    }
}

// string type
static void Bm_function_Qsortstring(benchmark::State &state)
{
    for (auto _ : state) {
        const char *arr[] = { "nihuangela", "xiaozhenhuniang", "our story",
                              "a language", "love", "qluhanagala",
                              "for elun", "sakuruwuma", "benchmark_musl" };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(char *), CompareString);
    }
}

// struct type
static void Bm_function_Qsortstruct(benchmark::State &state)
{
    const int len = 9;

    for (auto _ : state) {
        Nier nidate[len] = { {"Meihuagao", 23}, {"Sdifenzhou", 68}, {"Amusterlang", 99},
                             {"elun", 56}, {"yishinuala", 120}, {"huajiahaochi", 22},
                             {"lunala", 66}, {"cocolou", 77}, {"xinnoqikala", 55} };
        qsort(nidate, len, sizeof(Nier), CompareStruct);
    }
}

static void Bm_function_Qsort_random(benchmark::State &state)
{
    srand(1);
    int n = state.range(0);
    int *arr1 = new int[n];
    int *arr2 = new int[n];
    InitRandomArray(arr1, n);

    for (auto _ : state)
    {
        state.PauseTiming();
        memcpy(arr2, arr1, n);
        state.ResumeTiming();
        qsort(arr2, n, sizeof(int), CompareInt);
    }

    delete[] arr1;
    delete[] arr2;
}

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

// Converts any relative path to an absolute path
static void Bm_function_Realpath(benchmark::State &state)
{
    const int bufferLen = 4096;
    const char *realpathvariable[] = { "./log", "../dev", "log/hilog", "../dev/zero", "/dev" };
    const char *path = realpathvariable[state.range(0)];
    char resolvedPath[bufferLen];
    for (auto _ : state) {
        benchmark::DoNotOptimize(realpath(path, resolvedPath));
    }
}

MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtod, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtof, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtold, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Qsortint);
MUSL_BENCHMARK(Bm_function_Qsortdouble);
MUSL_BENCHMARK(Bm_function_Qsortstring);
MUSL_BENCHMARK(Bm_function_Qsortstruct);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Qsort_random, "COMMON_ARGS");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Realpath, "BENCHMARK_5");
MUSL_BENCHMARK(Bm_function_Getenv_TZ);
MUSL_BENCHMARK(Bm_function_Getenv_LD_LIBRARY_PATH);
MUSL_BENCHMARK(Bm_function_Getenv_LD_PRELOAD);
MUSL_BENCHMARK(Bm_function_Getenv_LC_ALL);
MUSL_BENCHMARK(Bm_function_Getenv_LOGNAME);