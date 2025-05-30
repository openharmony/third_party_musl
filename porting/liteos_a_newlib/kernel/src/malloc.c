/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "securec.h"
#include "los_config.h"
#include "los_memory.h"
#include <malloc.h>

void __wrap__free_r(struct _reent *reent, void *aptr)
{
    if (aptr == NULL) {
        return;
    }

    LOS_MemFree(OS_SYS_MEM_ADDR, aptr);
}

size_t __wrap__malloc_usable_size_r(struct _reent *reent, void *aptr)
{
    return 0;
}

void *__wrap__malloc_r(struct _reent *reent, size_t nbytes)
{
    if (nbytes == 0) {
        return NULL;
    }

    return LOS_MemAlloc(OS_SYS_MEM_ADDR, nbytes);
}

void *__wrap__memalign_r(struct _reent *reent, size_t align, size_t nbytes)
{
    if (nbytes == 0) {
        return NULL;
    }

    return LOS_MemAllocAlign(OS_SYS_MEM_ADDR, nbytes, align);
}

void *__wrap__realloc_r(struct _reent *reent, void *aptr, size_t nbytes)
{
    if (aptr == NULL) {
        return malloc(nbytes);
    }

    if (nbytes == 0) {
        free(aptr);
        return NULL;
    }

    return LOS_MemRealloc(OS_SYS_MEM_ADDR, aptr, nbytes);
}

void *__wrap__calloc_r(struct _reent *reent, size_t nitems, size_t size)
{
    size_t real_size;
    void *ptr = NULL;

    if (nitems == 0 || size == 0) {
        return NULL;
    }

    real_size = (size_t)(nitems * size);
    ptr = LOS_MemAlloc(OS_SYS_MEM_ADDR, real_size);
    if (ptr != NULL) {
        (void)memset_s(ptr, real_size, 0, real_size);
    }
    return ptr;
}

void *_sbrk(intptr_t __incr)
{
    return NULL;
}

