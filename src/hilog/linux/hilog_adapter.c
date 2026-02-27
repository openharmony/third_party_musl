/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#define _GNU_SOURCE

#include <hilog_adapter.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <atomic.h>

#include "hilog_common.h"
#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif
#include "vsnprintf_s_p.h"

#define LOG_LEN 3
#define ERROR_FD 2
#ifdef OHOS_ENABLE_PARAMETER
#define SYSPARAM_LENGTH 32
#endif

#define INVALID_SOCKET (-1)
#define INVALID_RESULT (-1)
#define CAS_FAIL (-1)

const int SOCKET_TYPE = SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC;
const struct sockaddr_un SOCKET_ADDR = {AF_UNIX, SOCKET_FILE_DIR INPUT_SOCKET_NAME};

static bool musl_log_enable = false;

#ifdef OHOS_ENABLE_PARAMETER
static const char *param_name = "musl.log.enable";
static const char *g_logLevelParam = "musl.log.level";
#endif
static int g_logLevel = LOG_WARN;
static volatile int g_socketFd = INVALID_SOCKET;

extern int __close(int fd);

// only generate a new socketFd
static int GenerateHilogSocketFd()
{
    int socketFd = TEMP_FAILURE_RETRY(socket(AF_UNIX, SOCKET_TYPE, 0));
    if (socketFd == INVALID_SOCKET) {
        dprintf(ERROR_FD, "HiLogAdapter_init: Can't create socket! Errno: %d\n", errno);
        return INVALID_SOCKET;
    }
    long int result =
        TEMP_FAILURE_RETRY(connect(socketFd, (const struct sockaddr *)(&SOCKET_ADDR), sizeof(SOCKET_ADDR)));
    if (result == INVALID_RESULT) {
        dprintf(ERROR_FD, "HiLogAdapter_init: Can't connect to server. Errno: %d\n", errno);
        __close(socketFd);
        return INVALID_SOCKET;
    }
    return socketFd;
}

HILOG_LOCAL_API
int CASHilogGlobalSocketFd(int socketFd)
{
    if (socketFd == INVALID_SOCKET) {
        return INVALID_RESULT;
    }
    // we should use CAS to avoid multi-thread problem
    if (a_cas(&g_socketFd, INVALID_SOCKET, socketFd) != INVALID_SOCKET) {
        // failure CAS: other threads execute to this branch to close extra fd
        return CAS_FAIL;
    }
    // success CAS: only one thread can execute to this branch
    return socketFd;
}

HILOG_LOCAL_API
bool CheckHilogValid()
{
    int socketFd = INVALID_SOCKET;
    // read fd by using atomic operation
#ifdef a_ll
    socketFd = a_ll(&g_socketFd);
#else
    socketFd = g_socketFd;
#endif
    return socketFd != INVALID_SOCKET;
}

/**
* This interface only for static-link style testcase, this symbol should not be exposed in dynamic libraries
* Dangerous operation, please do not use in normal business
*/
HILOG_LOCAL_API
void RefreshHiLogSocketFd()
{
    int socketFd = INVALID_SOCKET;
    // read fd by using atomic operation
#ifdef a_ll
    socketFd = a_ll(&g_socketFd);
#else
    socketFd = g_socketFd;
#endif
    if (socketFd == INVALID_SOCKET) {
        return;
    }
    a_store(&g_socketFd, INVALID_SOCKET);
    __close(socketFd);
}

void InitHilogSocketFd()
{
    int socketFd = GenerateHilogSocketFd();
    if (socketFd == INVALID_SOCKET) {
        return;
    }
    int result = CASHilogGlobalSocketFd(socketFd);
    if (result == CAS_FAIL) {
        __close(socketFd);
    }
}

static int SendMessage(HilogMsg *header, const char *tag, uint16_t tagLen, const char *fmt, uint16_t fmtLen)
{
    bool releaseSocket = false;
    int socketFd = INVALID_SOCKET;
    // read fd by using atomic operation
#ifdef a_ll
    socketFd = a_ll(&g_socketFd);
#else
    socketFd = g_socketFd;
#endif
    if (socketFd == INVALID_SOCKET) {
        socketFd = GenerateHilogSocketFd();
        if (socketFd == INVALID_SOCKET) {
            return INVALID_RESULT;
        }
        int result = CASHilogGlobalSocketFd(socketFd);
        if (result == CAS_FAIL) {
            releaseSocket = true;
        }
    }

    struct timespec ts = {0};
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    struct timespec ts_mono = {0};
    (void)clock_gettime(CLOCK_MONOTONIC, &ts_mono);
    header->tv_sec = (uint32_t)(ts.tv_sec);
    header->tv_nsec = (uint32_t)(ts.tv_nsec);
    header->mono_sec = (uint32_t)(ts_mono.tv_sec);
    header->len = sizeof(HilogMsg) + tagLen + fmtLen;
    header->tag_len = tagLen;

    struct iovec vec[LOG_LEN] = {0};
    vec[0].iov_base = header;                   // 0 : index of hos log header
    vec[0].iov_len = sizeof(HilogMsg);          // 0 : index of hos log header
    vec[1].iov_base = (void *)((char *)(tag));  // 1 : index of log tag
    vec[1].iov_len = tagLen;                    // 1 : index of log tag
    vec[2].iov_base = (void *)((char *)(fmt));  // 2 : index of log content
    vec[2].iov_len = fmtLen;                    // 2 : index of log content
    int ret = TEMP_FAILURE_RETRY(writev(socketFd, vec, LOG_LEN));
    if (releaseSocket) {
        __close(socketFd);
    }
    return ret;
}

HILOG_LOCAL_API
int HiLogAdapterPrintArgs(
    const LogType type, const LogLevel level, const unsigned int domain, const char *tag, const char *fmt, va_list ap)
{
    char buf[MAX_LOG_LEN] = {0};

    vsnprintfp_s(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt, ap);

    size_t tagLen = strnlen(tag, MAX_TAG_LEN - 1);
    size_t logLen = strnlen(buf, MAX_LOG_LEN - 1);
    HilogMsg header = {0};
    header.type = type;
    header.level = level;
#ifndef __RECV_MSG_WITH_UCRED_
    header.pid = getpid();
#endif
    header.tid = (uint32_t)(gettid());
    header.domain = domain;

    return SendMessage(&header, tag, tagLen + 1, buf, logLen + 1);
}

HILOG_LOCAL_API
int HiLogAdapterPrint(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...)
{
    if (!HiLogAdapterIsLoggable(domain, tag, level)) {
        return -1;
    }

    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = HiLogAdapterPrintArgs(type, level, domain, tag, fmt, ap);
    va_end(ap);
    return ret;
}

int HiLogBasePrint(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...)
{
    if (!HiLogBaseIsLoggable(domain, tag, level)) {
        return -1;
    }

    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = HiLogAdapterPrintArgs(type, level, domain, tag, fmt, ap);
    va_end(ap);
    return ret;
}

HILOG_LOCAL_API
int HiLogAdapterVaList(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, va_list ap)
{
    if (!HiLogAdapterIsLoggable(domain, tag, level)) {
        return -1;
    }
    return HiLogAdapterPrintArgs(type, level, domain, tag, fmt, ap);
}

bool is_musl_log_enable()
{
    if (getpid() == 1) {
        return false;
    }
    return musl_log_enable;
}

bool HiLogAdapterIsLoggable(unsigned int domain, const char *tag, LogLevel level)
{
    if (tag == NULL || level < g_logLevel || level <= LOG_LEVEL_MIN || level >= LOG_LEVEL_MAX) {
        return false;
    }

    if (!is_musl_log_enable()) {
        return false;
    }

    return true;
}

bool HiLogBaseIsLoggable(unsigned int domain, const char *tag, LogLevel level)
{
    if (level <= LOG_LEVEL_MIN || level >= LOG_LEVEL_MAX || tag == NULL) {
        return false;
    }
    return true;
}

#ifdef OHOS_ENABLE_PARAMETER
bool get_bool_sysparam(CachedHandle cachedhandle)
{
    const char *param_value = CachedParameterGet(cachedhandle);
    if (param_value != NULL) {
        if (strcmp(param_value, "true") == 0) {
            return true;
        }
    }
    return false;
}

void resetLogLevel()
{
    static CachedHandle muslLogLevelHandle = NULL;
    if (muslLogLevelHandle == NULL) {
        muslLogLevelHandle = CachedParameterCreate(g_logLevelParam, "WARN");
    }
    const char *value = CachedParameterGet(muslLogLevelHandle);
    if (value != NULL) {
        if (!strcmp(value, "DEBUG")) {
            g_logLevel = LOG_DEBUG;
        } else if (!strcmp(value, "INFO")) {
            g_logLevel = LOG_INFO;
        } else if (!strcmp(value, "WARN")) {
            g_logLevel = LOG_WARN;
        } else if (!strcmp(value, "ERROR")) {
            g_logLevel = LOG_ERROR;
        } else if (!strcmp(value, "FATAL")) {
            g_logLevel = LOG_FATAL;
        } else {
            g_logLevel = LOG_WARN;
        }
    } else {
        g_logLevel = LOG_WARN;
    }
}

#endif

void musl_log_reset()
{
#if (defined(OHOS_ENABLE_PARAMETER))
    static CachedHandle musl_log_Handle = NULL;
    if (musl_log_Handle == NULL) {
        musl_log_Handle = CachedParameterCreate(param_name, "false");
    }
    musl_log_enable = get_bool_sysparam(musl_log_Handle);
    resetLogLevel();
#elif (defined(ENABLE_MUSL_LOG))
    musl_log_enable = true;
#endif
}

HILOG_LOCAL_API
void __hilog_atfork(int who)
{
    if (who != 1) {
        return;
    }

    int socketFd = INVALID_SOCKET;
#ifdef a_ll
    socketFd = a_ll(&g_socketFd);
#else
    socketFd = g_socketFd;
#endif

    a_store(&g_socketFd, INVALID_SOCKET);
    if (socketFd != INVALID_SOCKET) {
        __close(socketFd);
    }
}
