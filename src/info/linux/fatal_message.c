/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <info/fatal_message.h>

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <errno.h>
#include "musl_log.h"

static pthread_mutex_t fatal_msg_lock = PTHREAD_MUTEX_INITIALIZER;
static fatal_msg_t *fatal_message = NULL;

extern uintptr_t DFX_SetCrashObj(uint8_t type, uintptr_t addr);

void set_fatal_message(const char *msg)
{
    if (pthread_mutex_trylock(&fatal_msg_lock) != 0) {
        return;
    }

    if (msg == NULL) {
        MUSL_LOGI("message null");
        pthread_mutex_unlock(&fatal_msg_lock);
        return;
    }

    if (fatal_message != NULL) {
        pthread_mutex_unlock(&fatal_msg_lock);
        return;
    }

    size_t size = sizeof(fatal_msg_t) + strlen(msg) + 1;
    void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (map == MAP_FAILED) {
        MUSL_LOGE("%{public}s:mmap failed,err:%{public}d:%{public}s", __FUNCTION__, errno, strerror(errno));
        fatal_message = NULL;
        pthread_mutex_unlock(&fatal_msg_lock);
        DFX_SetCrashObj(0, (uintptr_t)msg);
        return;
    }

    int ret = prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, map, size, "fatal message");
    if (ret < 0) {
        MUSL_LOGI("prctl set vma failed");
        munmap(map, size);
        fatal_message = NULL;
        pthread_mutex_unlock(&fatal_msg_lock);
        return;
    }

    fatal_message = (fatal_msg_t *)(map);
    fatal_message->size = size;
    strcpy(fatal_message->msg, msg);
    pthread_mutex_unlock(&fatal_msg_lock);
    return;
}

fatal_msg_t *get_fatal_message(void)
{
    return fatal_message;
}
