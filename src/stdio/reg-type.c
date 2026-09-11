/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "printf_ext.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

printf_va_arg_function **__printf_va_arg_table;

static int pa_next_type = PA_LAST;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int register_printf_type(printf_va_arg_function *fct)
{
	if (!fct) {
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock(&lock);

	if (pa_next_type >= PA_TYPE_MAX) {
		pthread_mutex_unlock(&lock);
		errno = ENOSPC;
		return -1;
	}

	if (!__printf_va_arg_table) {
		__printf_va_arg_table = calloc(PA_TYPE_MAX - PA_LAST, sizeof(void *));
		if (!__printf_va_arg_table) {
			pthread_mutex_unlock(&lock);
			return -1;
		}
	}

	int type_id = pa_next_type++;
	__printf_va_arg_table[type_id - PA_LAST] = fct;

	pthread_mutex_unlock(&lock);
	return type_id;
}
