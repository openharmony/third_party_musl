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
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include "pthread_impl.h"

#ifdef MUSL_EXTERNAL_FUNCTION
int pthread_attr_extension_init(pthread_attr_t *attr)
{
#ifndef __LITEOS_A__
    if (!get_pthread_extended_function_policy()) {
        return EINVAL;
    }
    pthread_attr_ext *ext = NULL;

    if (attr == NULL) {
        return EINVAL;
    }

    if (attr->_a_extension) {
        return 0;
    }

    ext = (pthread_attr_ext *)malloc(sizeof(pthread_attr_ext));
    if (!ext) {
        return ENOMEM;
    }

    ext->cpuset = NULL;
    ext->cpusetsize = 0;

    attr->_a_extension = (unsigned long)ext;
    return 0;
#else
    return EINVAL;
#endif
}

int pthread_attr_extension_destroy(pthread_attr_t *attr)
{
#ifndef __LITEOS_A__
    if (!get_pthread_extended_function_policy()) {
        return EINVAL;
    }
    if (attr == NULL) {
        return EINVAL;
    }

    if (attr->_a_extension) {
        struct pthread_attr_ext *ext = (struct pthread_attr_ext *)attr->_a_extension;
        if (ext->cpuset != NULL) {
            free(ext->cpuset);
            ext->cpuset = NULL;
            ext->cpusetsize = 0;
        }
        free((void *)attr->_a_extension);
        attr->_a_extension = 0;
    }
    return 0;
#else
    return EINVAL;
#endif
}
#endif
