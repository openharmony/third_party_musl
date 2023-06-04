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
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "util.h"

using namespace std;
#define EVENTSIZE 10
struct epoll_event events[EVENTSIZE], event;

// Used to create an epoll object to record events for files to be listened to
static void Bm_function_Epoll_createl(benchmark::State &state)
{
    for (auto _ : state) {
        int epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            perror("epoll_createl");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(epoll_fd);
        close(epoll_fd);
    }

    state.SetBytesProcessed(state.iterations());
}

// Used to add, modify, or delete events to an epoll object
static void Bm_function_Epoll_ctl(benchmark::State &state)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_createl");
        exit(EXIT_FAILURE);
    }
    int fd = open("/dev/zero", O_RDONLY);
    if (fd == -1) {
        perror("open epoll_ctl");
        exit(EXIT_FAILURE);
    }
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    for (auto _ : state) {
        benchmark::DoNotOptimize(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event));
    }
    close(epoll_fd);
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

// Used to wait for the file descriptor in the epoll object to appear for the event
static void Bm_function_Epoll_wait(benchmark::State &state)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_createl");
        exit(EXIT_FAILURE);
    }

    int fd = open("/dev/zero", O_RDONLY);
    if (fd == -1) {
        perror("open epoll_wait");
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    for (auto _ : state) {
        epoll_wait(epoll_fd, events, EVENTSIZE, 0);
    }
    close(epoll_fd);
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Epoll_createl);
MUSL_BENCHMARK(Bm_function_Epoll_ctl);
MUSL_BENCHMARK(Bm_function_Epoll_wait);
#endif