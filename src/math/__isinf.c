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

#ifdef MUSL_EXTERNAL_FUNCTION
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

int __isinf(double x)
{
    union {double f; uint64_t i;} u = {x};
    uint64_t sign = u.i >> 63;
    bool ret = (u.i & -1ULL>>1) == 0x7ffULL<<52;
    return (bool)sign ? -ret : ret;
}
#endif
