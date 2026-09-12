/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#ifndef _SYS_GROUP_IPC_H
#define _SYS_GROUP_IPC_H

#include <semaphore.h>
#include <sys/mman.h>

#define __NEED_gid_t
#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

sem_t *group_sem_open(const char *, int, gid_t, ...);
int group_sem_unlink(const char *, gid_t);
int group_shm_open(const char *, int, mode_t, gid_t);
int group_shm_unlink(const char *, gid_t);

#ifdef __cplusplus
}
#endif
#endif
