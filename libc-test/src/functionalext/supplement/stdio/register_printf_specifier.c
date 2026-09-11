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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <printf.h>
#include "functionalext.h"

static int my_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	const long *lp = *(const long **)args[0];
	char buf[64];
	int len = snprintf(buf, sizeof(buf), "%ld", *lp);
	if (len > 0) {
		size_t wlen = fwrite(buf, 1, (size_t)len, stream);
		if (wlen != (size_t)len) {
			return -1;
		}
	}
	return len;
}

static int my_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	if (n > 0) {
		argtypes[0] = PA_POINTER;
		size[0] = sizeof(long *);
	}
	return 1;
}

/**
 * @tc.name      : register_printf_specifier_0100
 * @tc.desc      : Normal registration returns 0
 * @tc.level     : Level 0
 */
void register_printf_specifier_0100(void)
{
	int ret = register_printf_specifier('B', my_printf_func, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0100", ret, 0);
}

/**
 * @tc.name      : register_printf_specifier_0200
 * @tc.desc      : Duplicate registration replaces old handler
 * @tc.level     : Level 1
 */
void register_printf_specifier_0200(void)
{
	int ret1 = register_printf_specifier('C', my_printf_func, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0200", ret1, 0);
	int ret2 = register_printf_specifier('C', my_printf_func, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0200", ret2, 0);
}

/**
 * @tc.name      : register_printf_specifier_0300
 * @tc.desc      : spec out of range (-1) returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_specifier_0300(void)
{
	errno = 0;
	int ret = register_printf_specifier(-1, my_printf_func, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0300", ret, -1);
	EXPECT_EQ("register_printf_specifier_0300", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_specifier_0400
 * @tc.desc      : spec out of range (256) returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_specifier_0400(void)
{
	errno = 0;
	int ret = register_printf_specifier(256, my_printf_func, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0400", ret, -1);
	EXPECT_EQ("register_printf_specifier_0400", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_specifier_0500
 * @tc.desc      : NULL func returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_specifier_0500(void)
{
	errno = 0;
	int ret = register_printf_specifier('D', NULL, my_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0500", ret, -1);
	EXPECT_EQ("register_printf_specifier_0500", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_specifier_0600
 * @tc.desc      : NULL arginfo returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_specifier_0600(void)
{
	errno = 0;
	int ret = register_printf_specifier('E', my_printf_func, NULL);
	EXPECT_EQ("register_printf_specifier_0600", ret, -1);
	EXPECT_EQ("register_printf_specifier_0600", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_specifier_0700
 * @tc.desc      : Registered specifier produces correct output
 * @tc.level     : Level 0
 */
void register_printf_specifier_0700(void)
{
	register_printf_specifier('B', my_printf_func, my_arginfo_func);
	long val = 42;
	char buf[256];
	int ret = snprintf(buf, sizeof(buf), "%B", &val);
	EXPECT_MT("register_printf_specifier_0700", ret, 0);
	EXPECT_STREQ("register_printf_specifier_0700", buf, "42");
}

/**
 * @tc.name      : register_printf_specifier_0800
 * @tc.desc      : Registered non-builtin specifier with pointer argument
 * @tc.level     : Level 1
 */
static int ptr_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	if (n > 0) {
		argtypes[0] = PA_POINTER;
		size[0] = sizeof(long *);
	}
	return 1;
}

void register_printf_specifier_0800(void)
{
	int ret = register_printf_specifier('H', my_printf_func, ptr_arginfo_func);
	EXPECT_EQ("register_printf_specifier_0800", ret, 0);
	long val = 99;
	char buf[256];
	int nret = snprintf(buf, sizeof(buf), "%H", &val);
	EXPECT_MT("register_printf_specifier_0800", nret, 0);
	EXPECT_STREQ("register_printf_specifier_0800", buf, "99");
}

/**
 * @tc.name      : register_printf_specifier_0900
 * @tc.desc      : Built-in specifiers (%d) are not overridden
 * @tc.level     : Level 0
 */
void register_printf_specifier_0900(void)
{
	register_printf_specifier('d', my_printf_func, my_arginfo_func);
	char buf[256];
	int ret = snprintf(buf, sizeof(buf), "%d", 123);
	EXPECT_MT("register_printf_specifier_0900", ret, 0);
	EXPECT_STREQ("register_printf_specifier_0900", buf, "123");
}

/**
 * @tc.name      : register_printf_specifier_1000
 * @tc.desc      : arginfo returning PA_INT|PA_FLAG_PTR passes a pointer
 * @tc.level     : Level 1
 */
static int ptrwrite_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	int *ip = *(int **)args[0];
	*ip = *ip + 1;
	char buf[32];
	int len = snprintf(buf, sizeof(buf), "%d", *ip);
	if (len > 0) {
		size_t wlen = fwrite(buf, 1, (size_t)len, stream);
		if (wlen != (size_t)len) {
			return -1;
		}
	}
	return len;
}

static int ptrwrite_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	if (n > 0) {
		argtypes[0] = PA_INT | PA_FLAG_PTR;
		size[0] = sizeof(int *);
	}
	return 1;
}

void register_printf_specifier_1000(void)
{
	int ret = register_printf_specifier('J', ptrwrite_printf_func, ptrwrite_arginfo_func);
	EXPECT_EQ("register_printf_specifier_1000", ret, 0);
	int val = 41;
	char buf[256];
	int nret = snprintf(buf, sizeof(buf), "%J", &val);
	EXPECT_MT("register_printf_specifier_1000", nret, 0);
	EXPECT_STREQ("register_printf_specifier_1000", buf, "42");
	EXPECT_EQ("register_printf_specifier_1000", val, 42);
}

/**
 * @tc.name      : register_printf_specifier_1100
 * @tc.desc      : info.pad is cleared when '-' and '0' flags are combined (ABI contract)
 * @tc.level     : Level 1
 */
static char seen_pad = '?';
static int seen_left = -1;

static int padcheck_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	seen_pad = (char)info->pad;
	seen_left = (int)info->left;
	if (fputc('.', stream) == EOF) {
		return -1;
	}
	return 1;
}

static int padcheck_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	return 0;
}

void register_printf_specifier_1100(void)
{
	int ret = register_printf_specifier('K', padcheck_printf_func, padcheck_arginfo_func);
	EXPECT_EQ("register_printf_specifier_1100", ret, 0);
	char buf[256];
	int nret = snprintf(buf, sizeof(buf), "%0-K", 123);
	EXPECT_MT("register_printf_specifier_1100", nret, 0);
	EXPECT_EQ("register_printf_specifier_1100", seen_pad, ' ');
	EXPECT_EQ("register_printf_specifier_1100", seen_left, 1);
	/* zero-pad alone still reaches the handler. */
	nret = snprintf(buf, sizeof(buf), "%0K", 123);
	EXPECT_MT("register_printf_specifier_1100", nret, 0);
	EXPECT_EQ("register_printf_specifier_1100", seen_pad, '0');
	EXPECT_EQ("register_printf_specifier_1100", seen_left, 0);
}

/**
 * @tc.name      : register_printf_specifier_1200
 * @tc.desc      : negative '.*' precision is normalized to -1 (ABI contract)
 * @tc.level     : Level 1
 */
static int seen_prec = -99;

static int preccheck_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	seen_prec = info->prec;
	if (fputc('.', stream) == EOF) {
		return -1;
	}
	return 1;
}

static int preccheck_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	return 0;
}

void register_printf_specifier_1200(void)
{
	int ret = register_printf_specifier('M', preccheck_printf_func, preccheck_arginfo_func);
	EXPECT_EQ("register_printf_specifier_1200", ret, 0);
	char buf[256];
	int nret = snprintf(buf, sizeof(buf), "%.*M", -5, 123);
	EXPECT_MT("register_printf_specifier_1200", nret, 0);
	EXPECT_EQ("register_printf_specifier_1200", seen_prec, -1);
	nret = snprintf(buf, sizeof(buf), "%.3M", 123);
	EXPECT_MT("register_printf_specifier_1200", nret, 0);
	EXPECT_EQ("register_printf_specifier_1200", seen_prec, 3);
	nret = snprintf(buf, sizeof(buf), "%M", 123);
	EXPECT_MT("register_printf_specifier_1200", nret, 0);
	EXPECT_EQ("register_printf_specifier_1200", seen_prec, -1);
}

int main(int argc, char *argv[])
{
	register_printf_specifier_0100();
	register_printf_specifier_0200();
	register_printf_specifier_0300();
	register_printf_specifier_0400();
	register_printf_specifier_0500();
	register_printf_specifier_0600();
	register_printf_specifier_0700();
	register_printf_specifier_0800();
	register_printf_specifier_0900();
	register_printf_specifier_1000();
	register_printf_specifier_1100();
	register_printf_specifier_1200();
	return t_status;
}
