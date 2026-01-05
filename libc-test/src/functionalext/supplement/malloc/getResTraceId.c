/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <stdbool.h>
#include "functionalext.h"
#include "memory_trace.h"

/**
 * @tc.name      : getResTraceId_0100
 * @tc.desc      : Verify that getResTraceId is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void getResTraceId_0100(void)
{
    uint32_t oldTraceType = 0;
    uint64_t oldTraceId = 0;
    uint32_t expectedTraceType = 1;
    uint64_t expectedTraceId = 100;

    setResTraceId(expectedTraceType, expectedTraceId, &oldTraceType, &oldTraceId);

    uint32_t traceType = 0;
    uint64_t traceId = 0;
    bool ret = getResTraceId(&traceType, &traceId);
}

int main(void)
{
    getResTraceId_0100();
    return t_status;
}