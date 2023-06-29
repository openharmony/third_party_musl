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
#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "util.h"

using namespace std;

static void Bm_function_Fstatat_relativepath(benchmark::State &state)
{
    struct stat st;
    int fd = -1;
    for (auto _ : state) {
        fd = fstatat(AT_FDCWD, "/dev/zero", &st, 0);
        if (fd == -1) {
            perror("fstatat relativepath");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstatat_symbollink(benchmark::State &state)
{
    symlink("/etc/passwd", "/data/local/tmp/passwd_link");
    struct stat st;
    int fd = -1;
    for (auto _ : state) {
        fd = fstatat(AT_FDCWD, "/data/local/tmp/passwd_link", &st, AT_SYMLINK_NOFOLLOW);
        if (fd == -1) {
            perror("fstatat symbollink");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fd);
    }
    remove("/data/local/tmp/passwd_link");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstatat_absolutepath(benchmark::State &state)
{
    struct stat st;
    int ret = -1;
    for (auto _ : state) {
        ret = fstatat(0, "/dev/zero", &st, 0);
        if (ret == -1) {
            perror("fstatat absolutepath");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(ret);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstat64(benchmark::State &state)
{
    struct stat buf;
    int fd = open("/etc/passwd", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open fstat64");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(fstat(fd, &buf));
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mkdir(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(mkdir("/data/data/test_mkdir", S_IRWXU | S_IRWXG | S_IXGRP | S_IROTH | S_IXOTH));
        state.PauseTiming();
        rmdir("/data/data/test_mkdir");
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Fstatat_relativepath);
MUSL_BENCHMARK(Bm_function_Fstatat_symbollink);
MUSL_BENCHMARK(Bm_function_Fstatat_absolutepath);
MUSL_BENCHMARK(Bm_function_Fstat64);
MUSL_BENCHMARK(Bm_function_Mkdir);