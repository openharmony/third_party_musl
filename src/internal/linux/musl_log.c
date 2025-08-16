/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>

#include "musl_log.h"

#define DEFAULT_STRING_SIZE 409600
#define DFX_LOG_LIB "libasan_logger.z.so"
#define DFX_LOG_INTERFACE "WriteSanitizerLog"
static void (*g_dfxLogPtr)(char*, size_t, char*);
static void* g_dfxLibHandler = NULL;
static pthread_mutex_t g_muslLogMutex = PTHREAD_MUTEX_INITIALIZER;
extern bool g_dl_inited; // flag indicates musl ldso initialization completed
extern bool g_global_destroyed; // flag indicates global variables have been destroyed
char *g_buffer = NULL; // an area of memory used to cache each line of sanitizer logs
size_t g_offset = 0; // the size of logs that have been cached to g_buffer
size_t g_end = DEFAULT_STRING_SIZE; // the size of g_buffer

static void buffer_clean_up(const char *str)
{
    if (g_buffer != NULL) {
        int unmapRes = munmap(g_buffer, g_end);
        if (unmapRes == -1) {
            MUSL_LOGW("[ohos_dfx_log] munmap g_buffer failed: %{public}d %{public}s:\n %{public}s\n",
                errno, strerror(errno), str);
        }
    }
    g_buffer = NULL;
    g_offset = 0;
    g_end = DEFAULT_STRING_SIZE;
}

bool load_asan_logger()
{
    if (g_dfxLibHandler == NULL) {
        g_dfxLibHandler = dlopen(DFX_LOG_LIB, RTLD_LAZY);
        if (g_dfxLibHandler == NULL) {
            MUSL_LOGW("[ohos_dfx_log] dlopen %{public}s failed!\n", DFX_LOG_LIB);
            return false;
        }
    }
    if (g_dfxLogPtr == NULL) {
        *(void **)(&g_dfxLogPtr) = dlsym(g_dfxLibHandler, DFX_LOG_INTERFACE);
        if (g_dfxLogPtr == NULL) {
            MUSL_LOGW("[ohos_dfx_log] dlsym %{public}s, failed!\n", DFX_LOG_INTERFACE);
            return false;
        }
    }
    return true;
}

/*
 * This function is not async-signal-safe
 * Don't use it in signal handler or in child process that is forked from any other process
 * Don't use it during initialization
 * Don't use it before execve
*/
static void write_to_dfx(const char *str, const char *path)
{
    if (g_dfxLogPtr != NULL) {
        g_dfxLogPtr(g_buffer, g_offset, (char *)path);
        return;
    }

    if (!g_dl_inited) {
        return;
    }

    if (!load_asan_logger()) {
        return;
    }
    g_dfxLogPtr(g_buffer, g_offset, (char *)path);
}

/*
 * This function is exclusively for LLVM Sanitizers to flush logs to disk
 * Don't use it for other purposes
 * This function is not async-signal-safe
 * Don't use it in signal handler or in child process that is forked from any other process
 * Don't use it during initialization
 * Don't use it before execve
*/
int ohos_dfx_log(const char *str, const char *path)
{
    if (g_global_destroyed || str == NULL) {
        return 0;
    }

    pthread_mutex_lock(&g_muslLogMutex);
    if (g_buffer == NULL) {
        g_buffer = mmap(NULL, DEFAULT_STRING_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (g_buffer == MAP_FAILED) {
            MUSL_LOGW("[ohos_dfx_log] mmap g_buffer failed: %{public}d %{public}s:\n %{public}s\n",
                errno, strerror(errno), str);
            pthread_mutex_unlock(&g_muslLogMutex);
            return 0;
        }
    }

    size_t str_len = strlen(str);
    size_t new_end = g_offset + str_len + 1;
    if (new_end > g_end) {
        char *new_buffer = mremap(g_buffer, g_end, g_end * 2, MREMAP_MAYMOVE);
        if (new_buffer == MAP_FAILED) {
            MUSL_LOGW("[ohos_dfx_log] mremap new_buffer failed: %{public}d %{public}s:\n %{public}s\n",
                errno, strerror(errno), str);
            pthread_mutex_unlock(&g_muslLogMutex);
            return 0;
        }
        g_end = g_end * 2;
        g_buffer = new_buffer;
        MUSL_LOGW("[ohos_dfx_log] g_end expand to %{public}lu\n", g_end);
    }

    strcpy(g_buffer + g_offset, str);
    g_offset += str_len;

    if (!(strstr(str, "End Hwasan report") || strstr(str, "End Asan report") ||
    strstr(str, "End Tsan report") || strstr(str, "End Ubsan report") ||
    strstr(str, "End CFI report"))) {
        pthread_mutex_unlock(&g_muslLogMutex);
        return 0;
    }

    write_to_dfx(str, path);
    buffer_clean_up(str);
    pthread_mutex_unlock(&g_muslLogMutex);

    return 0;
}

int musl_log(const char *fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = HiLogAdapterPrintArgs(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, fmt, ap);
    va_end(ap);
    return ret;
}