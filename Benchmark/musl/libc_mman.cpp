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
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include "stdio.h"
#include "malloc.h"
#include "util.h"

using namespace std;

static const vector<int> mmapFlags = {
    MAP_PRIVATE | MAP_ANONYMOUS,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_1GB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_1GB | MAP_POPULATE,
};

static const vector<int> mmapLength {
    8,
    16,
    64,
    1 * K,
    4 * K,
    64 * K,
    256 * K,
    1 * M,
    4 * M,
    64 * M,
    256 * M,
    1 * G,
};

static const vector<int> mdviseType {
    MADV_NORMAL,
    MADV_RANDOM,
    MADV_SEQUENTIAL,
    MADV_WILLNEED,
    MADV_DONTNEED,
    MADV_FREE,
    MADV_REMOVE,
    MADV_HUGEPAGE,
    MADV_SOFT_OFFLINE
};

static const vector<int> mprotectLength {
    1,
    2,
    4,
    32,
    1 * K,
    4 * K,
    32 * K,
    1 * M,
};

static void PrepareArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : mmapLength) {
        for (auto f : mmapFlags) {
            b->Args({l, f});
        }
    }
}

static void PrepareArgsInMdvise(benchmark::internal::Benchmark* b)
{
    for (auto l : mmapLength) {
        for (auto t : mdviseType) {
            b->Args({l, t});
        }
    }
}

static void Bm_function_Mmap_anonymous(benchmark::State &state)
{
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, -1, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Munmap_anonymous(benchmark::State &state)
{
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, -1, 0);
        state.ResumeTiming();
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            munmap(mem, length);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mmap_fd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open /dev/zero failed.");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, fd, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Munmap_fd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open /dev/zero failed.");
        exit(EXIT_FAILURE);
    }
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, fd, 0);
        state.ResumeTiming();
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            munmap(mem, length);
        }
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Madvise(benchmark::State &state)
{
    size_t length = state.range(0);
    int type = state.range(1);
    int *addr = (int*)mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(madvise(addr, length, type));
    }
    madvise(addr, length, MADV_NORMAL);
    munmap(addr, length);
}

static void Bm_function_mprotect(benchmark::State &state)
{
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    size_t size = pagesize * mprotectLength[state.range(0)];
    void *pages = memalign(pagesize, size);

    for (auto _ : state) {
        benchmark::DoNotOptimize(mprotect(pages, size, PROT_READ | PROT_WRITE));
    }
    state.SetItemsProcessed(state.iterations());

    free(pages);
}

BENCHMARK(Bm_function_Mmap_anonymous)->Apply(PrepareArgs);
BENCHMARK(Bm_function_Munmap_anonymous)->Apply(PrepareArgs);
BENCHMARK(Bm_function_Mmap_fd)->Apply(PrepareArgs);
BENCHMARK(Bm_function_Munmap_fd)->Apply(PrepareArgs);
BENCHMARK(Bm_function_Madvise)->Apply(PrepareArgsInMdvise);
MUSL_BENCHMARK_WITH_ARG(Bm_function_mprotect, "BENCHMARK_8");
