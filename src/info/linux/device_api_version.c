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

#include <stdlib.h>
#include <stdio.h>
#include <info/device_api_version.h>

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

#define API_VERSION_DEFAULT 0
#define API_VERSION_NAME_LEN 256

static int device_api_level = API_VERSION_DEFAULT;

void InitDeviceApiVersion(void)
{
#ifdef OHOS_ENABLE_PARAMETER
    const char para_name[API_VERSION_NAME_LEN] = "const.ohos.apiversion";
    CachedHandle para_handler = CachedParameterCreate(para_name, "0");
    if (para_handler == NULL) {
        device_api_level = API_VERSION_DEFAULT;
        return;
    }
    const char *para_value = CachedParameterGet(para_handler);
    if (para_value == NULL) {
        device_api_level = API_VERSION_DEFAULT;
        return;
    }
    int api_level = atoi(para_value);
    CachedParameterDestroy(para_handler);
    device_api_level = (api_level > 0 ) ? api_level : API_VERSION_DEFAULT;
#endif // OHOS_ENABLE_PARAMETER
}

int get_device_api_version(void)
{
    // depend subsystem of syspara support the interface of get system property
    return API_VERSION_DEFAULT;
}

int get_device_api_version_inner(void)
{
#ifdef OHOS_ENABLE_PARAMETER
    if (device_api_level == API_VERSION_DEFAULT) {
        // Api version may not init by InitDeviceApiVersion in dls3, try again.
        InitDeviceApiVersion();
    }
#endif // OHOS_ENABLE_PARAMETER
    return device_api_level;
}
