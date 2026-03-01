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

#include "easy_event_builder.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "easy_def.h"
#include "easy_event_encoder.h"
#include "easy_util.h"

#ifdef __cplusplus
extern "C" {
#endif

static const uint8_t STR_PARAM_VALUE_TYPE = 12; // refer to enum ValueType: ValueType::STRING
static const int DATA_MAX_LEN = 1024;

int AppendHeader(uint8_t* eventBuffer, const size_t bufferLen, size_t* offset,
    const struct HiSysEventEasyHeader* header)
{
    if ((eventBuffer == NULL) || (offset == NULL) || (header == NULL)) {
        return ERR_EVENT_BUF_INVALID;
    }
    if ((bufferLen < *offset) || ((bufferLen - *offset) < sizeof(struct HiSysEventEasyHeader))) {
        return ERR_MEM_OPT_FAILED;
    }
    *((struct HiSysEventEasyHeader*)(eventBuffer + *offset)) = *header;
    *offset += sizeof(struct HiSysEventEasyHeader);
    return SUCCESS;
}

int AppendStringParam(uint8_t* eventBuffer, const size_t bufferLen, size_t* offset, const char* key, const char* val)
{
    if ((eventBuffer == NULL) || (offset == NULL) || (key == NULL)) {
        return ERR_EVENT_BUF_INVALID;
    }
    if ((val == NULL) || (strlen(val) > DATA_MAX_LEN)) {
        return ERR_PARAM_VALUE_INVALID;
    }
    // append key
    int ret = EncodeStringValue(eventBuffer, bufferLen, offset, key);
    if (ret != SUCCESS) {
        return ret;
    }
    // append value type
    struct HiSysEventEasyParamValueType paramValueType;
    paramValueType.isArray = 0;
    paramValueType.valueType = STR_PARAM_VALUE_TYPE;
    paramValueType.valueByteCnt = 0;
    ret = EncodeValueType(eventBuffer, bufferLen, offset, &paramValueType);
    if (ret != SUCCESS) {
        return ret;
    }
    // append value
    ret = EncodeStringValue(eventBuffer, bufferLen, offset, val);
    if (ret != SUCCESS) {
        return ret;
    }
    return SUCCESS;
}

#ifdef __cplusplus
}
#endif