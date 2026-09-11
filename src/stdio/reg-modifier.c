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
#include <wchar.h>
#include <errno.h>
#include <pthread.h>

struct printf_modifier_record **__printf_modifier_table;

static unsigned int next_bit = 1;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int __handle_registered_modifier_mb(const char **s, unsigned int *user)
{
	if (!__printf_modifier_table) {
		return 1;
	}

	unsigned char ch = (unsigned char)**s;
	struct printf_modifier_record *rec = __printf_modifier_table[ch];
	if (!rec) {
		return 1;
	}

	struct printf_modifier_record *best = 0;
	int best_len = -1;

	for (; rec; rec = rec->next) {
		const char *p = *s + 1;
		const char *q = rec->str;
		while (*q && *p == *q) {
			p++;
			q++;
		}
		if (*q == '\0') {
			int len = (int)(p - *s - 1);
			if (len > best_len) {
				best_len = len;
				best = rec;
			}
		}
	}

	if (!best) {
		return 1;
	}

	*user |= best->bit;
	*s += best_len + 1;
	return 0;
}

int __handle_registered_modifier_wc(const wchar_t **s, unsigned int *user)
{
	if (!__printf_modifier_table) {
		return 1;
	}

	/* Registered modifiers only contain narrow characters, so a wide
	 * character outside unsigned char range can never start a match. */
	if (**s > UCHAR_MAX) {
		return 1;
	}

	unsigned char ch = (unsigned char)**s;
	struct printf_modifier_record *rec = __printf_modifier_table[ch];
	if (!rec) {
		return 1;
	}

	struct printf_modifier_record *best = 0;
	int best_len = -1;

	for (; rec; rec = rec->next) {
		const wchar_t *p = *s + 1;
		const char *q = rec->str;
		while (*q && *p <= UCHAR_MAX && *p == (unsigned char)*q) {
			p++;
			q++;
		}
		if (*q == '\0') {
			int len = (int)(p - *s - 1);
			if (len > best_len) {
				best_len = len;
				best = rec;
			}
		}
	}

	if (!best) {
		return 1;
	}

	*user |= best->bit;
	*s += best_len + 1;
	return 0;
}

int register_printf_modifier(const wchar_t *str)
{
	size_t len;

	if (!str || str[0] == L'\0') {
		errno = EINVAL;
		return -1;
	}

	len = wcslen(str);
	if (len > UCHAR_MAX) {
		errno = EINVAL;
		return -1;
	}

	for (size_t k = 0; k < len; k++) {
		if (str[k] > UCHAR_MAX) {
			errno = EINVAL;
			return -1;
		}
	}

	pthread_mutex_lock(&lock);

	if (next_bit == 0 || next_bit > (1u << (sizeof(((struct printf_info *)0)->user) * 8 - 1))) {
		pthread_mutex_unlock(&lock);
		errno = ENOSPC;
		return -1;
	}

	if (!__printf_modifier_table) {
		__printf_modifier_table = calloc(UCHAR_MAX + 1, sizeof(void *));
		if (!__printf_modifier_table) {
			pthread_mutex_unlock(&lock);
			return -1;
		}
	}

	unsigned char ch = (unsigned char)str[0];

	struct printf_modifier_record *rec = malloc(sizeof(*rec));
	if (!rec) {
		pthread_mutex_unlock(&lock);
		return -1;
	}

	rec->bit = next_bit;
	rec->next = 0;

	for (size_t k = 1; k < len; k++) {
		rec->str[k - 1] = (char)str[k];
	}
	rec->str[len - 1] = '\0';

	rec->next = __printf_modifier_table[ch];
	__printf_modifier_table[ch] = rec;

	unsigned int bit = next_bit;
	next_bit <<= 1;

	pthread_mutex_unlock(&lock);
	return (int)bit;
}
