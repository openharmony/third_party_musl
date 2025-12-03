/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#ifndef LIBC_TEST_FUNCTIONALEXT_LDSO_DLOPEN_CONFIG_ABS_PATH_LDSO_DLOPEN_CONFIG_ABS_PATH_H
#define LIBC_TEST_FUNCTIONALEXT_LDSO_DLOPEN_CONFIG_ABS_PATH_LDSO_DLOPEN_CONFIG_ABS_PATH_H

extern bool InitDlopenConfigAbsPathPolicy(void);
extern bool CheckPrerequisites(void);
extern bool OpenSo(bool, bool);
typedef void(FuncType)(bool flag);
extern FuncType *dlopen_config_abs_path_policy_func;
#endif