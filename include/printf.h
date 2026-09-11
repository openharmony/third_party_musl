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

#ifndef _PRINTF_H
#define _PRINTF_H

#include <features.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Note on thread-safety: the register_printf_* functions synchronize
 * with each other, but the printf family reads the registered tables
 * without locking. Registrations should therefore be completed during
 * single-threaded initialization, before any printf-family calls that
 * use the registered conversions. */

struct printf_info {
	int prec;
	int width;
	wchar_t spec;
	unsigned int is_long_double:1;
	unsigned int is_short:1;
	unsigned int is_long:1;
	unsigned int alt:1;
	unsigned int space:1;
	unsigned int left:1;
	unsigned int showsign:1;
	unsigned int group:1;
	unsigned int extra:1;
	unsigned int is_char:1;
	unsigned int wide:1;
	unsigned int i18n:1;
	unsigned int is_binary128:1;
	unsigned int __pad:3;
	unsigned short user;
	wchar_t pad;
};

enum {
	PA_INT,
	PA_CHAR,
	PA_WCHAR,
	PA_STRING,
	PA_WSTRING,
	PA_POINTER,
	PA_FLOAT,
	PA_DOUBLE,
	PA_LAST
};

#define PA_FLAG_MASK       0xff00
#define PA_FLAG_LONG_LONG  (1 << 8)
#define PA_FLAG_LONG_DOUBLE PA_FLAG_LONG_LONG
#define PA_FLAG_LONG       (1 << 9)
#define PA_FLAG_SHORT      (1 << 10)
#define PA_FLAG_PTR        (1 << 11)

typedef int printf_function(FILE *, const struct printf_info *, const void *const *);
typedef int printf_arginfo_size_function(const struct printf_info *, size_t, int *, int *);
typedef void printf_va_arg_function(void *, va_list *);

int register_printf_specifier(int, printf_function *, printf_arginfo_size_function *);
int register_printf_type(printf_va_arg_function *);
int register_printf_modifier(const wchar_t *);

#ifdef __cplusplus
}
#endif

#endif
