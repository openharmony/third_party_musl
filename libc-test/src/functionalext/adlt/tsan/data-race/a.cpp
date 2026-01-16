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
#include <pthread.h>
#include "common.h"

static constexpr int iter_check_num = 10000; 

void *testThread(void *x) {
	for (int i = 0; i < iter_check_num; ++i) {
		global = 42;
	}
	return x;
}

int testDataRace() {
	pthread_t t;
	pthread_create(&t, nullptr, testThread, nullptr);

	for (int i = 0; i < iter_check_num; ++i) {
		global = 43;
	}

	pthread_join(t, nullptr);
	return global;
}
