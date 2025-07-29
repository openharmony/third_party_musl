/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <stddef.h>
#include <stdint.h>

#ifndef weak_alias
#define weak_alias(old, new) \
        extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
#endif // !weak_alias

void __SetAsyncStackCallbackFunc(void* func) {}
weak_alias(__SetAsyncStackCallbackFunc, SetAsyncStackCallbackFunc);

int __DFX_SetAppRunningUniqueId(const char* appRunningId, size_t len) { return 0; }
weak_alias(__DFX_SetAppRunningUniqueId, DFX_SetAppRunningUniqueId);

/**
 * @brief set crash object which is measurement information of crash
 *
 * @param type  type of object, using enum CrashObjType
 * @param addr  addr of object
 * @return return crash Object which set up last time
*/
uintptr_t __DFX_SetCrashObj(uint8_t type, uintptr_t addr) { return 0; }
weak_alias(__DFX_SetCrashObj, DFX_SetCrashObj);

/**
 * @brief reset crash object
 *
 * @param crashObj return of DFX_SetCrashObj
*/
void __DFX_ResetCrashObj(uintptr_t crashObj) {}
weak_alias(__DFX_ResetCrashObj, DFX_ResetCrashObj);

/**
 * @brief set crash log config through HiAppEvent
 *
 * @param type  type of config attribute, using enum CrashLogConfigType
 * @param value value of config attribute
 * @return if succeed return 0, otherwise return -1. The reason for the failure can be found in 'errno'
 * @warning this interface is non-thread safety and signal safety
*/
int __DFX_SetCrashLogConfig(uint8_t type, uint32_t value) { return 0; }
weak_alias(__DFX_SetCrashLogConfig, DFX_SetCrashLogConfig);
