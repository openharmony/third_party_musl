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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

printf_function **__printf_function_table;
printf_arginfo_size_function **__printf_arginfo_table;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static int ensure_tables(void)
{
	if (__printf_arginfo_table) {
		return 0;
	}

	void *mem = calloc(2 * (UCHAR_MAX + 1), sizeof(void *));
	if (!mem) {
		return -1;
	}
	__printf_arginfo_table = mem;
	__printf_function_table = (printf_function **)((char *)mem + (UCHAR_MAX + 1) * sizeof(void *));
	return 0;
}

int register_printf_specifier(int spec, printf_function *func, printf_arginfo_size_function *arginfo)
{
	if (spec < 0 || spec > UCHAR_MAX || !func || !arginfo) {
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock(&lock);

	if (ensure_tables() != 0) {
		pthread_mutex_unlock(&lock);
		return -1;
	}

	__printf_arginfo_table[spec] = arginfo;
	__printf_function_table[spec] = func;

	pthread_mutex_unlock(&lock);
	return 0;
}
