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
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include "stdio.h"
#include "util.h"

using namespace std;

static void Bm_function_Mmap_1(benchmark::State &state)
{
    int fd = open("/data/data/my_mmap1.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open mmap1");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    for (auto _ : state) {
        char* mem = (char *)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    remove("/data/data/my_mmap1.txt");
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mmap_2(benchmark::State &state)
{
    int fd = open("/data/data/my_mmap2.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open mmap2");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    for (auto _ : state) {
        char* mem = (char *)mmap(NULL, length, PROT_READ | PROT_EXEC, MAP_FIXED | MAP_PRIVATE, fd, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    remove("/data/data/my_mmap2.txt");
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mmap_3(benchmark::State &state)
{
    int fd = open("/data/data/my_mmap3.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open mmap3");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    for (auto _ : state) {
        char* mem = (char *)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE, fd, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    remove("/data/data/my_mmap3.txt");
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Munmap(benchmark::State &state)
{
    int fd = open("/data/data/my_munmap.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open munmap");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char*)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mem != MAP_FAILED) {
            state.ResumeTiming();
            benchmark::DoNotOptimize(munmap(mem, length));
        }
    }
    close(fd);
    remove("/data/data/my_munmap.txt");
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mmap_anonymous(benchmark::State &state)
{
    int fd = open("/data/data/my_mmap_anonymous.txt", O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open anonymous mmap");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    for (auto _ : state) {
        int *addr = (int*)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (addr != MAP_FAILED) {
            benchmark::DoNotOptimize(addr);
            state.PauseTiming();
            munmap(addr, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    remove("/data/data/my_mmap_anonymous.txt");
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK_WITH_ARG(Bm_function_Mmap_1, "MMAP_SIZE");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Mmap_2, "MMAP_SIZE");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Mmap_3, "MMAP_SIZE");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Munmap, "MMAP_SIZE");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Mmap_anonymous, "MMAP_SIZE");
#endif