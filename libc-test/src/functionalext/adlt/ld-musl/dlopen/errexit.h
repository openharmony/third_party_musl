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
#ifndef _ADLT_TEST_LD_MUSL_DLOPEN_ERREXIT_H_
#define _ADLT_TEST_LD_MUSL_DLOPEN_ERREXIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define expect(cond) errexit_if_false(__FILE__, __LINE__, #cond, cond)
void errexit_if_false(const char *file, int lineno, const char *scond, int cond);
#define SET_VERBOSE_EXPECT() 	setenv("VERBOSE_EXPECT", "1", 1)
#define VERBOSE_EXPECT_REQUESTED() NULL != getenv("VERBOSE_EXPECT")

#ifdef __cplusplus
}
#endif

#endif
