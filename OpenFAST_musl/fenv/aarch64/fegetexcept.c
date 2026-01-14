/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2026. All rights reserved.
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

#include <fenv.h>
#include <features.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef MUSL_EXTERNAL_FUNCTION
#if defined(__aarch64__)

int fegetexcept(void)
{
    uint64_t fpcr;
    __asm__ __volatile__("mrs %x0, fpcr" : "=r" (fpcr));
    return FPCR_GET_EXCEPT_MASK(fpcr);
}

#endif // __aarch64__
#endif
#ifdef __cplusplus
}
#endif