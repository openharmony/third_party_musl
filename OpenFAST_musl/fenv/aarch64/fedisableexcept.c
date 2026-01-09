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

int fedisableexcept(int excepts)
{
    uint64_t old_fpcr, new_fpcr;
    excepts &= FE_ALL_EXCEPT;
    __asm__ __volatile__("mrs %x0, fpcr" : "=r" (old_fpcr));
    new_fpcr = old_fpcr & (~(excepts << FPCR_EXCEPTION_TRAP_SHIFT));
    __asm__ __volatile__("msr fpcr, %x0" : : "r" (new_fpcr));
    return FPCR_GET_EXCEPT_MASK(old_fpcr);
}

#endif // __aarch64__
#endif
#ifdef __cplusplus
}
#endif