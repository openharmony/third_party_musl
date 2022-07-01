/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <trace/trace_marker.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "musl_log.h"

#ifndef MUSL_TEMP_FAILURE_RETRY
#define MUSL_TEMP_FAILURE_RETRY(exp)            \
    ({                                     \
    long int _rc;                          \
    do {                                   \
        _rc = (long int)(exp);             \
    } while ((_rc == -1) && (errno == EINTR)); \
    _rc;                                   \
    })
#endif

// Check whether the user space trace function is enabled
static inline bool is_enable_trace(void)
{
    return true;
}

// Get the fd of trace_marker
static inline int get_trace_marker_fd(void)
{
    int trace_marker_fd = MUSL_TEMP_FAILURE_RETRY(open("/sys/kernel/tracing/trace_marker", O_CLOEXEC | O_WRONLY | O_APPEND));
    if (trace_marker_fd == -1) {
        trace_marker_fd = MUSL_TEMP_FAILURE_RETRY(open("/sys/kernel/debug/tracing/trace_marker", O_CLOEXEC | O_WRONLY | O_APPEND));
    }
    return trace_marker_fd;
}

/* Write the function call information to the trace_marker node in kernel space,
used on the same thread as trace_marker_end(),with the symbol "B". */
void trace_marker_begin(const char *message, const char *value)
{
    if (!is_enable_trace() || message == NULL) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "B|%d %s", getpid(), message);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "B|%d|%s %s", getpid(), message, value);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write B. %d", ret);
        }
    } else {
        MUSL_LOGE("B length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the terminator to the trace_marker node of the kernel space,
used on the same thread as trace_marker_begin(),with the symbol "E". */
void trace_marker_end(void)
{
    if (!is_enable_trace()) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "E|%d", getpid());
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write E. %d", ret);
        }
    } else {
        MUSL_LOGE("E length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the function call information to the trace_marker node in kernel space,
used in a different thread than trace_marker_async_end(),with the symbol "S". */
void trace_marker_async_begin(const char *message, const char *value, int taskId)
{
    if (!is_enable_trace() || message == NULL) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "S|%d|%s %d", getpid(), message, taskId);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "S|%d|%s|%s %d", getpid(), message, value, taskId);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write S. %d", ret);
        }
    } else {
        MUSL_LOGE("S length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the terminator to the trace_marker node in kernel space,
used in a different thread than trace_marker_async_begin(),with the symbol "F". */
void trace_marker_async_end(const char *message, const char *value, int taskId)
{
    if (!is_enable_trace() || message == NULL) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "F|%d|%s %d", getpid(), message, taskId);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "F|%d|%s|%s %d", getpid(), message, value, taskId);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write F. %d", ret);
        }
    } else {
        MUSL_LOGE("F length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

// A numeric variable used to mark a pre trace, with the symbol "C".
void trace_marker_count(const char *message, int value)
{
    if (!is_enable_trace() || message == NULL) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "C|%d|%s %d", getpid(), message, value);
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write C. %d", ret);
        }
    } else {
        MUSL_LOGE("C length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}
