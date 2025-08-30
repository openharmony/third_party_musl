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
#ifndef _ADLT_TEST_TLS_H_
#define _ADLT_TEST_TLS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef char c_data_t;
typedef int cpp_data_t;

extern bool waitFlag;

#define C_TLS_DATA_INIT		0x71
#define CPP_TLS_DATA_INIT	0x7E7E7E7E
#define C_ARRAY_LEN			(16*17)

typedef struct {
	int id;
	int data;
	void *dest;
} threadparam_t;

typedef void *(callback_t)(void *);

typedef c_data_t c_array_t[C_ARRAY_LEN];

#define C_I		C_TLS_DATA_INIT
// 'c' init data define (C_ARRAY_LEN units)
#define C_INIT \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, \
C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I, C_I+1

#ifdef __cplusplus
}
#endif

#endif