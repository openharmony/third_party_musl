/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "pthread_impl.h"

#ifdef MUSL_EXTERNAL_FUNCTION
int pthread_attr_getaffinity_np(pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset)
{
#ifndef __LITEOS_A__
    if (!get_pthread_extended_function_policy()) {
        return EINVAL;
    }
    if (!attr || !cpuset || cpusetsize == 0) {
        return EINVAL;
    }

    struct pthread_attr_ext *target_ext = (struct pthread_attr_ext *)attr->_a_extension;
    if (!target_ext || !target_ext->cpuset || target_ext->cpusetsize == 0) {
        memset(cpuset, -1, cpusetsize);
        return 0;
    }

    if (cpusetsize < target_ext->cpusetsize) {
        const unsigned char *src_bytes = (const unsigned char *)target_ext->cpuset;
        for (size_t i = cpusetsize; i < target_ext->cpusetsize; i++) {
            if (src_bytes[i] != 0) {
                return EINVAL;
            }
        }
    }

    size_t copy_size = target_ext->cpusetsize < cpusetsize ? target_ext->cpusetsize : cpusetsize;
    memcpy(cpuset, target_ext->cpuset, copy_size);

    if (cpusetsize > copy_size) {
        unsigned char *dst_bytes = (unsigned char *)cpuset;
        size_t pad_size = cpusetsize - copy_size;
        memset(dst_bytes + copy_size, 0, pad_size);
    }

    return 0;
#else
    return EINVAL;
#endif
}
#endif
