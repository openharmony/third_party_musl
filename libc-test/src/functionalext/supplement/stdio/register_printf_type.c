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

struct two_arg {
	long i;
	double d;
};

static void my_va_arg_func(void *mem, va_list *ap)
{
	struct two_arg *p = (struct two_arg *)mem;
	p->i = va_arg(*ap, long);
	p->d = va_arg(*ap, double);
}

static int type_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	const struct two_arg *p = *(const struct two_arg **)args[0];
	char buf[128];
	int len = snprintf(buf, sizeof(buf), "(%ld, %.1f)", p->i, p->d);
	if (len > 0) {
		size_t wlen = fwrite(buf, 1, (size_t)len, stream);
		if (wlen != (size_t)len) {
			return -1;
		}
	}
	return len;
}

static int type_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	if (n > 0) {
		argtypes[0] = PA_POINTER;
		size[0] = sizeof(struct two_arg *);
	}
	return 1;
}

static int registered_type = -1;

/**
 * @tc.name      : register_printf_type_0100
 * @tc.desc      : Normal registration returns type ID >= PA_LAST
 * @tc.level     : Level 0
 */
void register_printf_type_0100(void)
{
	registered_type = register_printf_type(my_va_arg_func);
	EXPECT_MT("register_printf_type_0100", registered_type, PA_LAST - 1);
}

/**
 * @tc.name      : register_printf_type_0200
 * @tc.desc      : Second registration returns next type ID
 * @tc.level     : Level 1
 */
void register_printf_type_0200(void)
{
	int type2 = register_printf_type(my_va_arg_func);
	EXPECT_MT("register_printf_type_0200", type2, registered_type);
}

/**
 * @tc.name      : register_printf_type_0300
 * @tc.desc      : NULL function returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_type_0300(void)
{
	errno = 0;
	int ret = register_printf_type(NULL);
	EXPECT_EQ("register_printf_type_0300", ret, -1);
	EXPECT_EQ("register_printf_type_0300", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_type_0400
 * @tc.desc      : Custom type used with register_printf_specifier
 * @tc.level     : Level 0
 */
void register_printf_type_0400(void)
{
	register_printf_specifier('P', type_printf_func, type_arginfo_func);
	struct two_arg val = { 42, 3.14 };
	char buf[256];
	int ret = snprintf(buf, sizeof(buf), "%P", &val);
	EXPECT_MT("register_printf_type_0400", ret, 0);
	EXPECT_STREQ("register_printf_type_0400", buf, "(42, 3.1)");
}

int main(int argc, char *argv[])
{
	register_printf_type_0100();
	register_printf_type_0200();
	register_printf_type_0300();
	register_printf_type_0400();
	return t_status;
}
