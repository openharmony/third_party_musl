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
#include <wchar.h>
#include <unistd.h>
#include <printf.h>
#include "functionalext.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"

static int user_type;

struct two_argument {
	long i;
	double d;
};

static void my_va_arg_function(void *mem, va_list *ap)
{
	struct two_argument *pair = mem;
	pair->i = va_arg(*ap, long);
	pair->d = va_arg(*ap, double);
}

static int my_printf_function(FILE *fp, const struct printf_info *info,
                              const void *const *args)
{
	size_t nargs;
	int printed;

	if (info->prec >= 0) {
		if (info->wide) {
			if (fputwc(L'{', fp) < 0) {
				return -1;
			}
		} else {
			if (fputc('{', fp) < 0) {
				return -1;
			}
		}
		nargs = info->prec;
		printed = 1;
	} else {
		nargs = 1;
		printed = 0;
	}

	for (size_t i = 0; i < nargs; ++i) {
		if (i != 0) {
			if (info->wide) {
				if (fputwc(L',', fp) < 0) {
					return -1;
				}
			} else {
				if (fputc(',', fp) < 0) {
					return -1;
				}
			}
			++printed;
		}
		struct two_argument *pair = *(void **)args[i];
		int ret;
		if (info->wide) {
			ret = fwprintf(fp, L"(%ld, %f)", pair->i, pair->d);
		} else {
			ret = fprintf(fp, "(%ld, %f)", pair->i, pair->d);
		}
		if (ret < 0) {
			return -1;
		}
		printed += ret;
	}

	if (info->prec >= 0) {
		if (info->wide) {
			if (fputwc(L'}', fp) < 0) {
				return -1;
			}
		} else {
			if (fputc('}', fp) < 0) {
				return -1;
			}
		}
		++printed;
	}
	return printed;
}

static int my_arginfo_function(const struct printf_info *info,
                               size_t n, int *argtypes, int *size)
{
	if (info->spec != 'P') {
		return -1;
	}

	size_t nargs;
	if (info->prec >= 0) {
		nargs = info->prec;
	} else {
		nargs = 1;
	}

	size_t to_fill = nargs;
	if (to_fill > n) {
		to_fill = n;
	}
	for (size_t i = 0; i < to_fill; ++i) {
		argtypes[i] = user_type;
		size[i] = sizeof(struct two_argument);
	}
	return nargs;
}

/**
 * @tc.name      : tst_vfprintf_user_type_0100
 * @tc.desc      : register_printf_type returns ID >= PA_LAST
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0100(void)
{
	user_type = register_printf_type(my_va_arg_function);
	EXPECT_MT("tst_vfprintf_user_type_0100", user_type, PA_LAST - 1);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0200
 * @tc.desc      : register_printf_specifier succeeds
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0200(void)
{
	int ret = register_printf_specifier('P', my_printf_function, my_arginfo_function);
	EXPECT_TRUE("tst_vfprintf_user_type_0200", ret >= 0);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0300
 * @tc.desc      : Single %P with asprintf ([[%P]] with 123L, 456.0)
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0300(void)
{
	char *str = NULL;
	int ret = asprintf(&str, "[[%P]]", 123L, 456.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0300", ret >= 0);
	EXPECT_STREQ("tst_vfprintf_user_type_0300", str, "[[(123, 456.000000)]]");
	free(str);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0400
 * @tc.desc      : Precision %.1P wraps output in braces
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0400(void)
{
	char *str = NULL;
	int ret = asprintf(&str, "[[%.1P]]", 1L, 2.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0400", ret >= 0);
	EXPECT_STREQ("tst_vfprintf_user_type_0400", str, "[[{(1, 2.000000)}]]");
	free(str);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0500
 * @tc.desc      : Precision %.2P with two pairs
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0500(void)
{
	char *str = NULL;
	int ret = asprintf(&str, "[[%.2P]]", 1L, 2.0, 3L, 4.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0500", ret >= 0);
	EXPECT_STREQ("tst_vfprintf_user_type_0500", str, "[[{(1, 2.000000),(3, 4.000000)}]]");
	free(str);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0600
 * @tc.desc      : Two %.2P and %.3P in one format
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0600(void)
{
	char *str = NULL;
	int ret = asprintf(&str, "[[%.2P | %.3P]]",
	                   1L, 2.0, 3L, 4.0,
	                   5L, 6.0, 7L, 8.0, 9L, 10.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0600", ret >= 0);
	EXPECT_STREQ("tst_vfprintf_user_type_0600", str,
	             "[[{(1, 2.000000),(3, 4.000000)} | {(5, 6.000000),(7, 8.000000),(9, 10.000000)}]]");
	free(str);
}

/**
 * @tc.name      : tst_vfprintf_user_type_0700
 * @tc.desc      : %P with snprintf output correct
 * @tc.level     : Level 0
 */
void tst_vfprintf_user_type_0700(void)
{
	char buf[256];
	int ret = snprintf(buf, sizeof(buf), "[[%P]]", 123L, 456.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0700", ret >= 0);
	EXPECT_STREQ("tst_vfprintf_user_type_0700", buf, "[[(123, 456.000000)]]");
}

/**
 * @tc.name      : tst_vfprintf_user_type_0800
 * @tc.desc      : %P with fprintf output correct
 * @tc.level     : Level 1
 */
void tst_vfprintf_user_type_0800(void)
{
	char tmpfile[256];
	char buf[256] = {0};
	int nret = snprintf(tmpfile, sizeof(tmpfile), "/data/local/tmp/tst_vfprintf_%d", getpid());
	EXPECT_TRUE("tst_vfprintf_user_type_0800", nret > 0 && nret < (int)sizeof(tmpfile));
	if (nret <= 0 || nret >= (int)sizeof(tmpfile)) {
		return;
	}
	FILE *fp = fopen(tmpfile, "w");
	EXPECT_TRUE("tst_vfprintf_user_type_0800", fp != NULL);
	if (!fp) {
		return;
	}
	int ret = fprintf(fp, "[[%P]]", 123L, 456.0);
	int cres = fclose(fp);
	EXPECT_TRUE("tst_vfprintf_user_type_0800", ret >= 0);
	EXPECT_EQ("tst_vfprintf_user_type_0800", cres, 0);
	if (ret < 0) {
		remove(tmpfile);
		return;
	}

	fp = fopen(tmpfile, "r");
	EXPECT_TRUE("tst_vfprintf_user_type_0800", fp != NULL);
	if (!fp) {
		remove(tmpfile);
		return;
	}
	size_t rlen = fread(buf, 1, sizeof(buf) - 1, fp);
	cres = fclose(fp);
	int dres = remove(tmpfile);
	EXPECT_TRUE("tst_vfprintf_user_type_0800", rlen > 0);
	EXPECT_EQ("tst_vfprintf_user_type_0800", cres, 0);
	EXPECT_EQ("tst_vfprintf_user_type_0800", dres, 0);

	EXPECT_STREQ("tst_vfprintf_user_type_0800", buf, "[[(123, 456.000000)]]");
}

/**
 * @tc.name      : tst_vfprintf_user_type_0900
 * @tc.desc      : Wide character swprintf with %P
 * @tc.level     : Level 1
 */
void tst_vfprintf_user_type_0900(void)
{
	wchar_t buf[200];
	int ret = swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"[[%P]]", 123L, 456.0);
	EXPECT_TRUE("tst_vfprintf_user_type_0900", ret >= 0);
	EXPECT_TRUE("tst_vfprintf_user_type_0900", wcscmp(buf, L"[[(123, 456.000000)]]") == 0);
}

/**
 * @tc.name      : tst_vfprintf_user_type_1000
 * @tc.desc      : Wide swprintf with precision %.1P
 * @tc.level     : Level 1
 */
void tst_vfprintf_user_type_1000(void)
{
	wchar_t buf[200];
	int ret = swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"[[%.1P]]", 1L, 2.0);
	EXPECT_TRUE("tst_vfprintf_user_type_1000", ret >= 0);
	EXPECT_TRUE("tst_vfprintf_user_type_1000", wcscmp(buf, L"[[{(1, 2.000000)}]]") == 0);
}

/**
 * @tc.name      : tst_vfprintf_user_type_1100
 * @tc.desc      : Wide swprintf with precision %.2P
 * @tc.level     : Level 1
 */
void tst_vfprintf_user_type_1100(void)
{
	wchar_t buf[200];
	int ret = swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"[[%.2P]]", 1L, 2.0, 3L, 4.0);
	EXPECT_TRUE("tst_vfprintf_user_type_1100", ret >= 0);
	EXPECT_TRUE("tst_vfprintf_user_type_1100", wcscmp(buf, L"[[{(1, 2.000000),(3, 4.000000)}]]") == 0);
}

/**
 * @tc.name      : tst_vfprintf_user_type_1200
 * @tc.desc      : Wide swprintf with two conversions
 * @tc.level     : Level 1
 */
void tst_vfprintf_user_type_1200(void)
{
	wchar_t buf[200];
	int ret = swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"[[%.2P | %.3P]]",
	                   1L, 2.0, 3L, 4.0,
	                   5L, 6.0, 7L, 8.0, 9L, 10.0);
	EXPECT_TRUE("tst_vfprintf_user_type_1200", ret >= 0);
	EXPECT_TRUE("tst_vfprintf_user_type_1200",
	            wcscmp(buf, L"[[{(1, 2.000000),(3, 4.000000)} | {(5, 6.000000),(7, 8.000000),(9, 10.000000)}]]") == 0);
}

#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
	tst_vfprintf_user_type_0100();
	tst_vfprintf_user_type_0200();
	tst_vfprintf_user_type_0300();
	tst_vfprintf_user_type_0400();
	tst_vfprintf_user_type_0500();
	tst_vfprintf_user_type_0600();
	tst_vfprintf_user_type_0700();
	tst_vfprintf_user_type_0800();
	tst_vfprintf_user_type_0900();
	tst_vfprintf_user_type_1000();
	tst_vfprintf_user_type_1100();
	tst_vfprintf_user_type_1200();
	return t_status;
}
