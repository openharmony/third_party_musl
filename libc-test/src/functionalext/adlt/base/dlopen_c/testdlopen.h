/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
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
#ifndef _ADLT_TEST_DLOPEN_C_H_
#define _ADLT_TEST_DLOPEN_C_H_

namespace testdlopen {
int testVer(const char *filename);
int testPrintf(const char *filename, const char *text);
int testMax(const char *filename, int a, int b);
} // namespace testdlopen

#endif