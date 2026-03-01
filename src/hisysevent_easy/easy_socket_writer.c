/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "easy_socket_writer.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "easy_def.h"
#include "easy_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MUSL_TEMP_FAILURE_RETRY
#define MUSL_TEMP_FAILURE_RETRY(exp)           \
    ({                                         \
        long int _rc;                          \
        do {                                   \
            _rc = (long int)(exp);             \
        } while ((_rc == -1) && (errno == EINTR)); \
        _rc;                                   \
    })
#endif

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(exp) MUSL_TEMP_FAILURE_RETRY(exp)
#endif

static const char SOCKET_PATH[] = "/dev/unix/socket/hisysevent";

static int InitSendBuffer(int socketId)
{
    int sendBuffSize = 384 * 1024; // max buffer of socket is 384K
    if (setsockopt(socketId, SOL_SOCKET, SO_SNDBUF, (void*)(&sendBuffSize), sizeof(int)) < 0) {
        return ERR_SET_SOCKET_OPT_FAILED;
    }
    return SUCCESS;
}

static int InitSocket(struct sockaddr_un* socketAddr)
{
    if (socketAddr == NULL) {
        return ERR_SOCKET_ADDR_INVALID;
    }
    int ret = MemoryInit((uint8_t*)(socketAddr), sizeof(struct sockaddr_un));
    if (ret != SUCCESS) {
        return ret;
    }
    ret = MemoryCopy((uint8_t*)(socketAddr->sun_path), sizeof(socketAddr->sun_path), (uint8_t*)SOCKET_PATH,
        strlen(SOCKET_PATH));
    if (ret != SUCCESS) {
        return ret;
    }
    socketAddr->sun_family = AF_UNIX;
    return SUCCESS;
}

int Write(const uint8_t* data, const size_t dataLen)
{
    if (data == NULL) {
        return ERR_EVENT_BUF_INVALID;
    }
    int socketId = TEMP_FAILURE_RETRY(socket(AF_UNIX, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
    if (socketId < 0) {
        close(socketId);
        return ERR_INIT_SOCKET_FAILED;
    }
    int ret = InitSendBuffer(socketId);
    if (ret != SUCCESS) {
        close(socketId);
        return ret;
    }
    struct sockaddr_un socketAddr;
    ret = InitSocket(&socketAddr);
    if (ret != SUCCESS) {
        close(socketId);
        return ret;
    }

    int sendRet = 0;
    int retryTimes = 3; // retry 3 times to write socket
    do {
        sendRet = sendto(socketId, data, dataLen, 0, (struct sockaddr*)(&socketAddr),
            sizeof(socketAddr));
        retryTimes--;
    } while (sendRet < 0 && retryTimes > 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR));
    if (sendRet < 0) {
        close(socketId);
        return ERR_SOCKET_SEND_ERROR_BASE + errno;
    }
    close(socketId);
    return SUCCESS;
}

#ifdef __cplusplus
}
#endif