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

#define __INNER_CONCAT(a,b) a##.##b
#define __API_VERSION(maj, min, patch) __INNER_CONCAT(__INNER_CONCAT(maj, min), patch)

#define __INNER_AVAILABILITY(platform, version) __attribute__((__availability__(platform, introduced=version)))
#define __INNER_DEPRECATED(platform, version) __attribute__((__availability__(platform, deprecated=version)))
#define __INNER_OBSOLETED(platform, version) __attribute__((__availability__(platform, obsoleted=version)))

// version: ohos的版本号,23或者是26.0.0等，最多支持三位小数
#define __API_AVAILABILITY(version) __INNER_AVAILABILITY(ohos, version)
// 指定废弃的ohos版本号
#define __API_DEPRECATED(version) __INNER_DEPRECATED(ohos, version)
// 指定废弃的ohos版本号
#define __API_OBSOLETED(version) __INNER_OBSOLETED(ohos, version)

// 编码时候用来隔离接口代码的宏
#define APIAVAILABLE(...)  _APIAVAILABLE(__VA_ARGS__, 3, 2, 1, 0)
#define _APIAVAILABLE(_1, _2, _3, N, ...)  __APIAVAILABLE_##N(_1, _2, _3)
#define __APIAVAILABLE_1(maj, ...)  __builtin_available(ohos maj, *)
#define __APIAVAILABLE_3(maj, min, patch)  __builtin_available(ohos __API_VERSION(maj, min, patch), *)

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
