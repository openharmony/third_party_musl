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
#include "unistd.h"
#include "stddef.h"
#include "fenv.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/uio.h"
#include "sys/utsname.h"
#include "fcntl.h"
#include "stdio.h"
#include "sys/time.h"
#include "time.h"
#include "err.h"
#include "util.h"

using namespace std;

#define BUFFER 1024

void ReadWriteTest(benchmark::State& state, bool isRead)
{
    size_t chunkSize = state.range(0);
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open ReadWriteTest");
        exit(EXIT_FAILURE);
    }
    char* bufRead = new char[chunkSize];
    const char *bufWrite = "hello world!";
    if (isRead) {
        memset(bufRead, 0, chunkSize * sizeof(char));
    }

    while (state.KeepRunning()) {
        if (isRead) {
            if (read(fd, bufRead, chunkSize) == -1) {
                perror("Read proc");
                exit(EXIT_FAILURE);
            }
        } else {
            if (write(fd, bufWrite, chunkSize) == -1) {
                perror("write proc");
                exit(EXIT_FAILURE);
            }
        }
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunkSize));
    delete[] bufRead;
    close(fd);
}

template <typename Fn>
void PreadWriteTest(benchmark::State &state, Fn f, bool buffered)
{
    size_t chunkSize = state.range(0);
    int fp = open("/dev/zero", O_RDWR, OPEN_MODE);
    char *buf = new char[chunkSize];
    off64_t offset = 0;
    while (state.KeepRunning()) {
        if (f(fp, buf, strlen(buf), offset) == -1) {
            errx(1, "ERROR: op of %zu bytes failed.", chunkSize);
        }
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunkSize));
    delete[] buf;
    close(fp);
}

// Obtain the identification code of the current process
static void Bm_function_Getpid(benchmark::State &state)
{
    pid_t pid;
    for (auto _ : state) {
        pid = getpid();
        benchmark::DoNotOptimize(pid);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Geteuid(benchmark::State &state)
{
    uid_t uid;
    for (auto _ : state) {
        uid = geteuid();
        benchmark::DoNotOptimize(uid);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Close(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
        if (fd == -1) {
            perror("open Close");
            exit(EXIT_FAILURE);
        }
        state.ResumeTiming();
        benchmark::DoNotOptimize(close(fd));
    }
    state.SetBytesProcessed(state.iterations());
}

#define SECOND  50

static void Bm_function_Usleep(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(usleep(SECOND));
    }
    state.SetItemsProcessed(state.iterations());
}

void Bm_function_Pwrite64(benchmark::State &state)
{
    PreadWriteTest(state, pwrite, true);
}

void Bm_function_Pread64(benchmark::State &state)
{
    PreadWriteTest(state, pread, true);
}

// Stores the destination path of the symbolic link file into a buffer and returns the length of the destination path
static void Bm_function_Readlink(benchmark::State &state)
{
    char path[BUFFER] = {0};
    ssize_t len;
    int i = symlink("/dev/zero", "/data/local/tmp/tmplink");
    if (i == -1) {
        perror("symlink");
        exit(-1);
    }
    for (auto _ : state) {
        len = readlink("/data/local/tmp/tmplink", path, sizeof(path));
        benchmark::DoNotOptimize(len);
    }
    remove("/data/local/tmp/tmplink");
}

static void Bm_function_Getuid(benchmark::State &state)
{
    uid_t uid;
    for (auto _ : state) {
        uid = getuid();
        benchmark::DoNotOptimize(uid);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Getegid(benchmark::State &state)
{
    gid_t gid;
    for (auto _ : state) {
        gid = getegid();
        benchmark::DoNotOptimize(gid);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Read(benchmark::State &state)
{
    ReadWriteTest(state, true);
}

static void Bm_function_Write(benchmark::State &state)
{
    ReadWriteTest(state, false);
}

static void Bm_function_Access_exist(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, F_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_read(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, R_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_write(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, W_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_execute(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, X_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static const char *g_writevvariable1[] = {"Pretend inferiority and", "hello,",
                                          "non sa module libtoken_sync_manager_service.z.so",
                                          "The variable device_company was",
                                          "but never appeared in a",
                                          "The build continued as if",
                                          "product_name=rk3568", "build/toolchain/ohos:"};
static const char *g_writevvariable2[] = {"encourage others arrogance!", "world!", ":token_sync_manager_service",
                                          "set as a build argument", "declare_args() block in any buildfile",
                                          "that argument was unspecified", "ohos_build_type=", "ohos_clang_arm64"};
// Write the contents of multiple buffers to the file descriptor at once
static void Bm_function_Writev(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    const char *str1 = g_writevvariable1[state.range(0)];
    const char *str2 = g_writevvariable2[state.range(0)];
    struct iovec iov[2];
    ssize_t nwritten;
    iov[0].iov_base = (void *)str1;
    iov[0].iov_len = strlen(str1);
    iov[1].iov_base = (void *)str2;
    iov[1].iov_len = strlen(str2);

    for (auto _ : state) {
        nwritten = writev(fd, iov, 2);
        benchmark::DoNotOptimize(nwritten);
    }
    close(fd);
}

static void Bm_function_Uname(benchmark::State &state)
{
    for (auto _ : state) {
        struct utsname buffer;
        benchmark::DoNotOptimize(uname(&buffer));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Lseek(benchmark::State &state)
{
    int fd = open("/etc/passwd", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open lseek");
        exit(EXIT_FAILURE);
    }

    for (auto _ : state) {
        lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Dup(benchmark::State &state)
{
    int fd = -1;
    for (auto _ : state) {
        fd = dup(STDOUT_FILENO);
        if (fd == -1) {
            perror("dup");
            exit(EXIT_FAILURE);
        }

        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
}

MUSL_BENCHMARK(Bm_function_Getpid);
MUSL_BENCHMARK(Bm_function_Geteuid);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Pwrite64, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Readlink);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Pread64, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Close);
MUSL_BENCHMARK(Bm_function_Usleep);
MUSL_BENCHMARK(Bm_function_Getuid);
MUSL_BENCHMARK(Bm_function_Getegid);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Read, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Access_exist);
MUSL_BENCHMARK(Bm_function_Access_read);
MUSL_BENCHMARK(Bm_function_Access_write);
MUSL_BENCHMARK(Bm_function_Access_execute);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Writev, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Uname);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Write, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Lseek);
MUSL_BENCHMARK(Bm_function_Dup);
