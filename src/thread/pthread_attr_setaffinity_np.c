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
#include <string.h>
#include "pthread_impl.h"

#ifdef MUSL_EXTERNAL_FUNCTION

int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset)
{
#ifndef __LITEOS_A__
    if (!get_pthread_extended_function_policy()) {
        return EINVAL;
    }
    struct pthread_attr_ext *target_ext;
    if (!attr || (cpusetsize > 0 && !cpuset)) {
        return EINVAL;
    }

    if (!cpuset || cpusetsize == 0) {
        target_ext = (struct pthread_attr_ext *)attr->_a_extension;
        if (target_ext && target_ext->cpuset) {
            pthread_attr_extension_destroy(target_ext);
        }
        return 0;
    }
    target_ext = (struct pthread_attr_ext *)attr->_a_extension;
    if (!target_ext) {
        int init_ret = pthread_attr_extension_init(attr);
        if (init_ret != 0) {
            return init_ret;
        }

        target_ext = (struct pthread_attr_ext *)attr->_a_extension;
    }

    if (!target_ext->cpuset || target_ext->cpusetsize != cpusetsize) {
        void *new_buffer = malloc(cpusetsize);
        if (!new_buffer)
            return ENOMEM;

        if (target_ext->cpuset) {
            free(target_ext->cpuset);
        }

        target_ext->cpuset = new_buffer;
        target_ext->cpusetsize = cpusetsize;
    }

    memcpy(target_ext->cpuset, cpuset, cpusetsize);
    return 0;
#else
    return EINVAL;
#endif
}
#endif