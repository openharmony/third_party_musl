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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

#define PORT 1500 // port
#define BACKLOG 5

static void Bm_function_socket_server(benchmark::State &state)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any IP address of this host
    serverAddr.sin_port = htons(PORT);
    bzero(&(serverAddr.sin_zero), 8); // Set other attributes to 0
    for (auto _ : state) {
        int serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd == -1) {
            printf("socket failed:%d", errno);
            break;
        }
        close(serverFd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_socketpair_sendmsg_recvmsg(benchmark::State &state)
{
    int ret;
    int socks[2];
    struct msghdr msg;
    struct iovec iov[1];
    char sendBuf[100] = "it is a test";
    struct msghdr msgr;
    struct iovec iovr[1];
    char recvBuf[100];

    for (auto _ : state) {
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
        if (ret == -1) {
            printf("socketpair err\n");
            break;
        }

        bzero(&msg, sizeof(msg));
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        iov[0].iov_base = sendBuf;
        iov[0].iov_len = sizeof(sendBuf);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;

        ret = sendmsg(socks[1], &msg, 0);
        if (ret == -1) {
            printf("sendmsg err\n");
            close(socks[0]);
            close(socks[1]);
            break;
        }

        bzero(&msg, sizeof(msg));
        msgr.msg_name = NULL;
        msgr.msg_namelen = 0;
        iovr[0].iov_base = &recvBuf;
        iovr[0].iov_len = sizeof(recvBuf);
        msgr.msg_iov = iovr;
        msgr.msg_iovlen = 1;
        ret = recvmsg(socks[0], &msgr, 0);
        if (ret == -1) {
            printf("recvmsg err\n");
        }
        close(socks[0]);
        close(socks[1]);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_getsockopt(benchmark::State &state)
{
    int optVal;
    socklen_t optLen = sizeof(optVal);

    for (auto _ : state) {
        int sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (getsockopt(sockFd, SOL_SOCKET, SO_SNDBUF, &optVal, &optLen) < 0) {
            printf("fail to getsockopt");
        }
        close(sockFd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_setsockopt(benchmark::State &state)
{
    for (auto _ : state) {
        int nRecvBuf = 32*1024; // Set to 32K
        int sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (setsockopt(sockFd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) < 0) {
            printf("fail to setsockopt");
        }
        close(sockFd);
    }
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_socket_server);
MUSL_BENCHMARK(Bm_function_socketpair_sendmsg_recvmsg);
MUSL_BENCHMARK(Bm_function_getsockopt);
MUSL_BENCHMARK(Bm_function_setsockopt);