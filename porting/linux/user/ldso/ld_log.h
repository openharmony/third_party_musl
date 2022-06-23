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

#include <musl_log.h>

#ifndef LD_LOG_H
#define LD_LOG_H

#define LD_LOG_ERROR 1
#define LD_LOG_WARNING 2
#define LD_LOG_INFO 4
#define LD_LOG_DEBUG 8

#define LD_LOG_LEVEL (LD_LOG_ERROR | LD_LOG_WARNING)

#if (LD_LOG_LEVEL & LD_LOG_ERROR)
#define LD_LOGE(...) MUSL_LOGE(...)
#else
#define LD_LOGE(...)
#endif

#if (LD_LOG_LEVEL & LD_LOG_WARNING)
#define LD_LOGW(...) MUSL_LOGW(...)
#else
#define LD_LOGW(...)
#endif

#if (LD_LOG_LEVEL & LD_LOG_INFO)
#define LD_LOGI(...) MUSL_LOGI(...)
#else
#define LD_LOGI(...)
#endif

#if (LD_LOG_LEVEL & LD_LOG_DEBUG)
#define LD_LOGD(...) MUSL_LOGD(...)
#else
#define LD_LOGD(...)
#endif

#endif  // LD_LOG_H
