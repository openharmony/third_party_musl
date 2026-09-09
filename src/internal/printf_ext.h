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

#ifndef _PRINTF_EXT_H
#define _PRINTF_EXT_H

#include <printf.h>
#include <limits.h>

#define PA_TYPE_MAX 0x100

struct printf_modifier_record {
	unsigned int bit;
	char str[UCHAR_MAX + 1];
	struct printf_modifier_record *next;
};

extern hidden printf_function **__printf_function_table;
extern hidden printf_arginfo_size_function **__printf_arginfo_table;
extern hidden struct printf_modifier_record **__printf_modifier_table;
extern hidden printf_va_arg_function **__printf_va_arg_table;

int __handle_registered_modifier_mb(const char **s, unsigned int *user);
int __handle_registered_modifier_wc(const wchar_t **s, unsigned int *user);

#endif
