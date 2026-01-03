/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

int __isinff(float x)
{
    union {float f; uint32_t i;} u = {x};
    uint32_t sign = u.i >> 31;
    bool ret = (u.i & 0x7fffffff) == 0x7f800000;
    return (bool)sign ? -ret : ret;
}
