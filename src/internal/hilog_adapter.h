/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef MUSL_HILOG_ADAPTER_H
#define MUSL_HILOG_ADAPTER_H

#include <features.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef hidden
#define hidden __attribute__((visibility("hidden")))
#endif

// Log type
typedef enum {
    LOG_TYPE_MIN = 0,
    LOG_APP = 0,
    // Log to kmsg, only used by init phase.
    LOG_INIT = 1,
    // Used by core service, framework.
    LOG_CORE = 3,
    LOG_KMSG = 4,
    LOG_TYPE_MAX
} LogType;

// Log level
typedef enum {
    LOG_LEVEL_MIN = 0,
    LOG_DEBUG = 3,
    LOG_INFO = 4,
    LOG_WARN = 5,
    LOG_ERROR = 6,
    LOG_FATAL = 7,
    LOG_LEVEL_MAX = 8,
} LogLevel;

hidden bool CheckHilogValid();
// Dangerous operation, please do not use in normal business
hidden void RefreshHiLogSocketFd();

hidden void InitHilogSocketFd();

hidden int HiLogAdapterPrint(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...)
    __attribute__((__format__(os_log, 5, 6)));

hidden int HiLogAdapterPrintArgs(const LogType type, const LogLevel level, const unsigned int domain, const char *tag,
                                 const char *fmt, va_list ap);
hidden int HiLogAdapterVaList(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt,
                              va_list ap);

hidden bool HiLogAdapterIsLoggable(unsigned int domain, const char *tag, LogLevel level);

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
hidden bool get_bool_sysparam(CachedHandle cachedhandle);
#endif

hidden bool is_musl_log_enable();
hidden void musl_log_reset();

#endif  // MUSL_HILOG_ADAPTER_H
