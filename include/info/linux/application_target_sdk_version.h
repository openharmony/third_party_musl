/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

#ifndef _INFO_APPLICATION_TARGET_SDK_VERSION_H
#define _INFO_APPLICATION_TARGET_SDK_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#define __DISTRIBUTEOS_VERSION(maj, min, patch) maj##min##patch // min必须是两位数字dd(如00,01), patch也必须是两位（如00,01）
#define __OH_VERSION(maj,min) maj##.##min
#define __INNER_CONCAT(a,b) a##.##b
#define __INNER_DEPRECATED(platform, version) __attribute__((__availability__(platform, deprecated=version)))
#define __INNER_OBSOLETED(platform, version) __attribute__((__availability__(platform, obsoleted=version)))
#define __INNER_AVAILABILITY(platform, version) __attribute__((__availability__(platform, introduced=version)))

#define __OH_AVAILABILITY(version) __INNER_AVAILABILITY(ohos, version)
#define __DISTRIBUTEOS_AVAILABILITY(dos_ver, oh_ver) __INNER_AVAILABILITY(ohos, __INNER_CONCAT(oh_ver,dos_ver))
#define __OH_DEPRECATED(version) __INNER_DEPRECATED(ohos, version)
#define __DISTRIBUTEOS_DEPRECATED(dos_ver, oh_ver) __INNER_DEPRECATED(ohos, __INNER_CONCAT(oh_ver,dos_ver))
#define __OH_OBSOLETED(version) __INNER_OBSOLETED(ohos, version)
#define __DISTRIBUTEOS_OBSOLETED(dos_ver, oh_ver) __INNER_OBSOLETED(ohos, __INNER_CONCAT(oh_ver,dos_ver))
#define SINCE(api) __builtin_available(ohos api, *)
#define __INNER_SINCE(ver) __builtin_available(ohos ver, *)
#define DISTRIBUTEOS_SINCE(dos_ver, oh_ver) __INNER_SINCE(__INNER_CONCAT(oh_ver, dos_ver))

#define SDK_VERSION_FUTURE 9999
#define SDK_VERSION_7 7
#define SDK_VERSION_8 8
#define SDK_VERSION_9 9

/**
  * @brief Get the target sdk version number of the application.
  * @return The target sdk version number.
  */
int get_application_target_sdk_version(void);

/**
  * @brief Set the target sdk version number of the application.
  * @param target The target sdk version number.
  */
void set_application_target_sdk_version(int target);

#ifdef __cplusplus
}
#endif

#endif // _INFO_APPLICATION_TARGET_SDK_VERSION_H
