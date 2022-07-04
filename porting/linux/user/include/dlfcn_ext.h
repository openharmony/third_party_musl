/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DLFCN_EXT_H
#define DLFCN_EXT_H

#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif

/* for dl_extinfo flag */
#define DL_EXT_WRITE_RELRO 0x1
#define DL_EXT_USE_RELRO 0x2
#define DL_EXT_RESERVED_ADDRESS_RECURSIVE 0x4

/** Mask of valid bits. */
#define DL_EXT_VALID_FLAG_BITS (DL_EXT_WRITE_RELRO | DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE)

typedef struct {
    int flag;
    int relro_fd;
} dl_extinfo;

/**
 * @brief Loads the dynamic shared object (shared library) file with the extended feature.
 *        If extinfo is NULL, it is equivalent to dlopen.
 *        If DL_EXT_WRITE_RELRO is set in extinfo, the GNU RELRO section will be written to relro_fd and allowed to
 *        reused by other process loading the same library at the same address.
 *        IF DL_EXT_USE_RELRO is set in extinfo, the GNU RELRO section written in relro_fd will be reused.
 * @param file Equivalent to the argument of dlopen.
 * @param mode Equivalent to the argument of dlopen.
 * @param extinfo Indicates the dl_extinfo struct.
 * @return Returns a non-NULL handle for the loaded object on success. On error returns NULL.
 */
void *dlopen_ext(const char *file, int mode, const dl_extinfo *extinfo);

#ifdef __cplusplus
}
#endif

#endif
