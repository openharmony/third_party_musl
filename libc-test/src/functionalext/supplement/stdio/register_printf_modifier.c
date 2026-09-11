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

/**
 * @tc.name      : register_printf_modifier_0100
 * @tc.desc      : Normal registration returns bitmask value >= 1
 * @tc.level     : Level 0
 */
void register_printf_modifier_0100(void)
{
	int ret = register_printf_modifier(L"I");
	EXPECT_MT("register_printf_modifier_0100", ret, 0);
}

/**
 * @tc.name      : register_printf_modifier_0200
 * @tc.desc      : Multi-character modifier registration
 * @tc.level     : Level 0
 */
void register_printf_modifier_0200(void)
{
	int ret = register_printf_modifier(L"I32");
	EXPECT_MT("register_printf_modifier_0200", ret, 0);
}

/**
 * @tc.name      : register_printf_modifier_0300
 * @tc.desc      : Empty string returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_modifier_0300(void)
{
	errno = 0;
	int ret = register_printf_modifier(L"");
	EXPECT_EQ("register_printf_modifier_0300", ret, -1);
	EXPECT_EQ("register_printf_modifier_0300", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_modifier_0400
 * @tc.desc      : Character exceeding UCHAR_MAX returns -1 with EINVAL
 * @tc.level     : Level 1
 */
void register_printf_modifier_0400(void)
{
	errno = 0;
	wchar_t ws[2] = { 0x100, 0 };
	int ret = register_printf_modifier(ws);
	EXPECT_EQ("register_printf_modifier_0400", ret, -1);
	EXPECT_EQ("register_printf_modifier_0400", errno, EINVAL);
}

/**
 * @tc.name      : register_printf_modifier_0500
 * @tc.desc      : Modifier bitmask values are powers of 2
 * @tc.level     : Level 1
 */
void register_printf_modifier_0500(void)
{
	int b1 = register_printf_modifier(L"X1");
	int b2 = register_printf_modifier(L"X2");
	EXPECT_MT("register_printf_modifier_0500", b1, 0);
	EXPECT_MT("register_printf_modifier_0500", b2, 0);
	EXPECT_TRUE("register_printf_modifier_0500", (b1 & b2) == 0);
}

/**
 * @tc.name      : register_printf_modifier_0600
 * @tc.desc      : 16 modifiers max, then ENOSPC
 * @tc.level     : Level 2
 */
void register_printf_modifier_0600(void)
{
	int i;
	int last = 0;
	for (i = 0; i < 16; i++) {
		wchar_t ws[3] = { L'Z', L'0' + (wchar_t)i, 0 };
		int ret = register_printf_modifier(ws);
		if (ret < 0) {
			break;
		}
		last = ret;
	}
	errno = 0;
	wchar_t ws[3] = { L'Z', L'9', 0 };
	int ret = register_printf_modifier(ws);
	EXPECT_EQ("register_printf_modifier_0600", ret, -1);
	EXPECT_EQ("register_printf_modifier_0600", errno, ENOSPC);
}

static int mod_seen_user;

static int mod_printf_func(FILE *stream, const struct printf_info *info, const void *const *args)
{
	mod_seen_user = info->user;
	const int *ip = *(const int **)args[0];
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

static int mod_arginfo_func(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	if (n > 0) {
		argtypes[0] = PA_POINTER;
		size[0] = sizeof(int *);
	}
	return 1;
}

/**
 * @tc.name      : register_printf_modifier_0700
 * @tc.desc      : Single-char modifier reaches handler via info->user
 * @tc.level     : Level 1
 */
void register_printf_modifier_0700(void)
{
	int bit = register_printf_modifier(L"Q");
	EXPECT_TRUE("register_printf_modifier_0700", bit > 0);
	int ret = register_printf_specifier('R', mod_printf_func, mod_arginfo_func);
	EXPECT_EQ("register_printf_modifier_0700", ret, 0);

	int val = 7;
	char buf[64];
	mod_seen_user = 0;
	int nret = snprintf(buf, sizeof(buf), "%QR", &val);
	EXPECT_MT("register_printf_modifier_0700", nret, 0);
	EXPECT_STREQ("register_printf_modifier_0700", buf, "7");
	EXPECT_EQ("register_printf_modifier_0700", mod_seen_user, bit);
}

/* Note: 0600 exhausts the modifier bit space and must run last. */
int main(int argc, char *argv[])
{
	register_printf_modifier_0100();
	register_printf_modifier_0200();
	register_printf_modifier_0300();
	register_printf_modifier_0400();
	register_printf_modifier_0500();
	register_printf_modifier_0700();
	register_printf_modifier_0600();
	return t_status;
}
