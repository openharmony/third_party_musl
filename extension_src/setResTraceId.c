/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdint.h>
#include "functionalext.h"
#include "memory_trace.h"

/**
 * @tc.name      : setResTraceId_0100
 * @tc.desc      : Verify that setResTraceId is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void setResTraceId_0100(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = 1;
    uint64_t tmpTraceId = 100;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0200
 * @tc.desc      : Verify that setResTraceId is successful with large traceId
 * @tc.level     : Level 0
 */
void setResTraceId_0200(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = 1;
    uint64_t tmpTraceId = UINT64_MAX;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0300
 * @tc.desc      : Verify that setResTraceId is successful with large traceType
 * @tc.level     : Level 0
 */
void setResTraceId_0300(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = UINT32_MAX;
    uint64_t tmpTraceId = 100;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0400
 * @tc.desc      : Verify that setResTraceId is successful with zero values
 * @tc.level     : Level 0
 */
void setResTraceId_0400(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = 0;
    uint64_t tmpTraceId = 0;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0500
 * @tc.desc      : Verify that setResTraceId is successful with pOldTraceType is NULL
 * @tc.level     : Level 2
 */
void setResTraceId_0500(void)
{
    uint32_t oldTraceType = 1;
    uint64_t oldTraceId = 100;
    uint64_t tmpTraceId = 200;
    setResTraceId(oldTraceType, oldTraceId, NULL, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0600
 * @tc.desc      : Verify that setResTraceId is successful with pOldTraceID is NULL
 * @tc.level     : Level 2
 */
void setResTraceId_0600(void)
{
    uint32_t oldTraceType = 1;
    uint64_t oldTraceId = 100;
    uint32_t tmpTraceType = 2;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, NULL);
}

/**
 * @tc.name      : setResTraceId_0700
 * @tc.desc      : Verify that setResTraceId is successful with both pOldTraceType and pOldTraceID are NULL
 * @tc.level     : Level 2
 */
void setResTraceId_0700(void)
{
    uint32_t oldTraceType = 1;
    uint64_t oldTraceId = 100;
    setResTraceId(oldTraceType, oldTraceId, NULL, NULL);
}

/**
 * @tc.name      : setResTraceId_0800
 * @tc.desc      : Verify that setResTraceId is successful with multiple calls
 * @tc.level     : Level 1
 */
void setResTraceId_0800(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = 1;
    uint64_t tmpTraceId = 100;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
    
    oldTraceType = tmpTraceType;
    oldTraceId = tmpTraceId;
    tmpTraceType = 2;
    tmpTraceId = 200;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_0900
 * @tc.desc      : Verify that setResTraceId is successful with typical traceType values
 * @tc.level     : Level 0
 */
void setResTraceId_0900(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = 256;
    uint64_t tmpTraceId = 123456789;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

/**
 * @tc.name      : setResTraceId_1000
 * @tc.desc      : Verify that setResTraceId is successful with max values for both parameters
 * @tc.level     : Level 1
 */
void setResTraceId_1000(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t tmpTraceType = UINT32_MAX;
    uint64_t tmpTraceId = UINT64_MAX;
    setResTraceId(oldTraceType, oldTraceId, &tmpTraceType, &tmpTraceId);
}

int main(void)
{
    setResTraceId_0100();
    setResTraceId_0200();
    setResTraceId_0300();
    setResTraceId_0400();
    setResTraceId_0500();
    setResTraceId_0600();
    setResTraceId_0700();
    setResTraceId_0800();
    setResTraceId_0900();
    setResTraceId_1000();
    return t_status;
}