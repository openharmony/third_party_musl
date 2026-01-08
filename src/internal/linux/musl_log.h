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

#ifndef _MUSL_LOG_H
#define _MUSL_LOG_H

#include <hilog_adapter.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MUSL_LOG_TYPE LOG_CORE
#define MUSL_LOG_DOMAIN 0xD003F07
#define MUSL_LOG_TAG "MUSL"

#if (defined(OHOS_ENABLE_PARAMETER) || defined(ENABLE_MUSL_LOG))
#define MUSL_LOGE(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGW(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_WARN, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGI(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGD(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_DEBUG, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))

#else
#define MUSL_LOGE(...)
#define MUSL_LOGW(...)
#define MUSL_LOGI(...)
#define MUSL_LOGD(...)
#endif

#ifdef __cplusplus
}
#endif
#endif
