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

static int globalB = 0;
static pthread_mutex_t lock;

void *b1(void *p) {
	pthread_mutex_lock(&lock);
	globalB++;
	pthread_mutex_unlock(&lock);
	return nullptr;
}

void *b2(void *p) {
	pthread_mutex_lock(&lock);
	globalB--;
	pthread_mutex_unlock(&lock);
	return nullptr;
}

void testB() {
	pthread_t t[2];
	pthread_mutex_init(&lock, nullptr);
	pthread_create(&t[0], nullptr, b1, nullptr);
	pthread_create(&t[1], nullptr, b2, nullptr);
	pthread_join(t[0], nullptr);
	pthread_join(t[1], nullptr);
	pthread_mutex_destroy(&lock);
}
