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
#ifndef _ADLT_TEST_DUPLICATES_H_
#define _ADLT_TEST_DUPLICATES_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int val;
extern const int valA;
extern const int valB;
extern const int valC;

int get_val(void);

int get_val_from_A(void);
int get_val_from_B(void);
int get_val_from_C(void);

int get_valA_from_A(void);
int get_valB_from_A(void);
int get_valC_from_A(void);
int get_valA_from_B(void);
int get_valB_from_B(void);
int get_valC_from_B(void);
int get_valA_from_C(void);
int get_valB_from_C(void);
int get_valC_from_C(void);

#ifdef __cplusplus
}
#endif

#endif