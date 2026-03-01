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

#include "easy_util.h"
#include "easy_def.h"

#ifdef __cplusplus
extern "C" {
#endif

int MemoryInit(uint8_t* data, const size_t dataLen)
{
    if (data == NULL) {
        return ERR_MEM_OPT_FAILED;
    }
    uint8_t* tmpData = data;
    for (size_t index = 0; index < dataLen; ++index) {
        *tmpData = 0;
        ++tmpData;
    }
    return SUCCESS;
}

int MemoryCopy(uint8_t* dest, size_t destLen, uint8_t* src, const size_t srcLen)
{
    if ((dest == NULL) || (src == NULL) || (destLen < srcLen)) {
        return ERR_MEM_OPT_FAILED;
    }
    uint8_t* destTmpData = dest;
    uint8_t* srcTmpData = src;
    for (size_t index = 0; index < srcLen; ++index) {
        *destTmpData = *srcTmpData;
        ++destTmpData;
        ++srcTmpData;
    }
    return SUCCESS;
}

#ifdef __cplusplus
}
#endif