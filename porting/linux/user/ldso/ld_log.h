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

#ifndef LD_LOG_H
#define LD_LOG_H

#include <musl_log.h>

#define LD_LOG_ERROR 1
#define LD_LOG_WARNING 2
#define LD_LOG_INFO 4
#define LD_LOG_DEBUG 8

#define LD_LOG_LEVEL (LD_LOG_ERROR | LD_LOG_WARNING)

#define LD_LOG_TAG "MUSL-LDSO"

#if (defined(ENABLE_MUSL_LOG) && (LD_LOG_LEVEL & LD_LOG_ERROR))
#define LD_LOGE(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, LD_LOG_TAG, __VA_ARGS__))
#else
#define LD_LOGE(...)
#endif

#if (defined(ENABLE_MUSL_LOG) && (LD_LOG_LEVEL & LD_LOG_WARNING))
#define LD_LOGW(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_WARN, MUSL_LOG_DOMAIN, LD_LOG_TAG, __VA_ARGS__))
#else
#define LD_LOGW(...)
#endif

#if (defined(ENABLE_MUSL_LOG) && (LD_LOG_LEVEL & LD_LOG_INFO))
#define LD_LOGI(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, LD_LOG_TAG, __VA_ARGS__))
#else
#define LD_LOGI(...)
#endif

#if (defined(ENABLE_MUSL_LOG) && (LD_LOG_LEVEL & LD_LOG_DEBUG))
#define LD_LOGD(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_DEBUG, MUSL_LOG_DOMAIN, LD_LOG_TAG, __VA_ARGS__))
#else
#define LD_LOGD(...)
#endif

#endif  // LD_LOG_H
