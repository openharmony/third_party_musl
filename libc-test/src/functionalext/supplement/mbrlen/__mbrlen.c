/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/mman.h>
#include <stdio.h>
#include <wchar.h>
#include <langinfo.h>
#include <locale.h>
#include "functionalext.h"

/**
 * @tc.name      : __mbrlen_0100
 * @tc.desc      : Verify that the obtained character length is 1 byte
 * @tc.level     : Level 0
 */
void __mbrlen_0100(void)
{
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    char test[] = {'m', 'u', 's', 'l'};
    const int num = sizeof(test) / sizeof(test[0]);
    size_t limitsize = sizeof(test[0]);
    for (unsigned int i = 0; i < num; i++) {
        EXPECT_EQ("__mbrlen_0100", __mbrlen(test + i, limitsize, NULL), 1);
    }
}

/**
 * @tc.name      : __mbrlen_0200
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void __mbrlen_0200(void)
{
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    char test[] = {'m', 'u', 's', 'l', '\0'};
    size_t index = 1;
    size_t ret = 0;
    const int num = sizeof(test) / sizeof(test[0]) - 1;
    for (unsigned int i = 0; i < num; i++) {
        ret = __mbrlen(&test[i], 0, NULL);
        EXPECT_NE("__mbrlen_0100", ret, index);
    }
    if (test[num] == '\0') {
        ret = __mbrlen(&test[num], 1, NULL);
        EXPECT_EQ("__mbrlen_0100", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : __mbrlen_0300
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void __mbrlen_0300(void)
{
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    unsigned char utf8_3byte[] = {0xE4, 0xB8, 0xAD};
    size_t ret = __mbrlen((char*)utf8_3byte, 3, NULL);
    EXPECT_EQ("test_mbrlen_multibyte(3-byte)", ret, 3);
}

/**
 * @tc.name      : __mbrlen_0400
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void __mbrlen_0400(void)
{
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    unsigned char utf8_4byte[] = {0xF0, 0x90, 0x8D, 0x88};
    size_t ret = __mbrlen((char*)utf8_4byte, 4, NULL);
    EXPECT_EQ("test_mbrlen_multibyte(4-byte)", ret, 4);
}

/**
 * @tc.name      : __mbrlen_0500
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void __mbrlen_0500(void)
{
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    unsigned char out_of_range[] = {0xF8};
    size_t ret = __mbrlen((char*)out_of_range, 1, NULL);
    EXPECT_EQ("test_mbrlen_multibyte(out_of_range)", ret, (size_t)-1);
}

/**
 * @tc.name      : __mbrlen_0600
 * @tc.desc      : UTF-8 single byte feeding test
 * @tc.level     : Level 2
 */
void __mbrlen_0600(void)
{
    mbstate_t s;
    char *loc = setlocale(LC_ALL, "en_US.UTF-8");
    if (loc == NULL) {
        return;
    }
    EXPECT_EQ("__mbrlen_0600", __mbrlen("\xE2", 1, &s), (size_t)-2);	/* 1st byte processed */
    EXPECT_EQ("__mbrlen_0600", __mbrlen("\x89", 1, &s), (size_t)-2);	/* 2nd byte processed */
    EXPECT_EQ("__mbrlen_0600", __mbrlen("\xA0", 1, &s), (size_t)1);	/* 3rd byte processed */
    EXPECT_EQ("__mbrlen_0600", __mbrlen("", 1, &s), 0);	/* test final byte processing */

    const char str[] = "\xe0\xa0\x80";
    EXPECT_EQ("__mbrlen_0600", __mbrlen(str, 1, &s), (size_t)-2);
    EXPECT_EQ("__mbrlen_0600", __mbrlen(str + 1, 2, &s), 2);
    EXPECT_EQ("__mbrlen_0600", __mbrlen(str, 3, &s), 3);
}

void __mbrlen_test(void)
{
    const char *cs;
	int i;
	mbstate_t st;
    if (strcmp(nl_langinfo(CODESET), "UTF-8")) {
        printf("cannot set UTF-8 locale for test (codeset=%s)\n", nl_langinfo(CODESET));
        return;
    }
	EXPECT_EQ("failed to catch error", __mbrlen("\x80", 1, &st), (size_t)-1);
	EXPECT_EQ("failed to catch illegal initial", __mbrlen("\xc0", 1, &st), (size_t)-1);
	EXPECT_EQ("aliasing nul", __mbrlen("\xc0\x80", 2, &st), (size_t)-1);
	EXPECT_EQ("aliasing slash", __mbrlen("\xc0\xaf", 2, &st), (size_t)-1);
	EXPECT_EQ("aliasing slash", __mbrlen("\xe0\x80\xaf", 3, &st), (size_t)-1);
	EXPECT_EQ("aliasing slash", __mbrlen("\xf0\x80\x80\xaf", 4, &st), (size_t)-1);
	EXPECT_EQ("aliasing slash", __mbrlen("\xf8\x80\x80\x80\xaf", 5, &st), (size_t)-1);
	EXPECT_EQ("aliasing slash", __mbrlen("\xfc\x80\x80\x80\x80\xaf", 6, &st), (size_t)-1);
	EXPECT_EQ("aliasing U+0080", __mbrlen("\xe0\x82\x80", 3, &st), (size_t)-1);
	EXPECT_EQ("aliasing U+07FF", __mbrlen("\xe0\x9f\xbf", 3, &st), (size_t)-1);
	EXPECT_EQ("aliasing U+0800", __mbrlen("\xf0\x80\xa0\x80", 4, &st), (size_t)-1);
	EXPECT_EQ("aliasing U+FFFD", __mbrlen("\xf0\x8f\xbf\xbd", 4, &st), (size_t)-1);
	EXPECT_EQ("failed to catch surrogate", __mbrlen("\xed\xa0\x80", 3, &st), (size_t)-1);
	EXPECT_EQ("failed to accept U+FFFE", __mbrlen("\xef\xbf\xbe", 3, &st), 3);
	EXPECT_EQ("failed to accept U+FFFF", __mbrlen("\xef\xbf\xbf", 3, &st), 3);
	EXPECT_EQ("failed to accept U+10FFFE", __mbrlen("\xf4\x8f\xbf\xbe", 4, &st), 4);
	EXPECT_EQ("failed to accept U+10FFFF", __mbrlen("\xf4\x8f\xbf\xbf", 4, &st), 4);
	EXPECT_EQ("wrong length", __mbrlen("\xc2\x80", 2, &st), 2);
	EXPECT_EQ("wrong length",__mbrlen("\xe0\xa0\x80", 3, &st), 3);
	EXPECT_EQ("wrong length",__mbrlen("\xf0\x90\x80\x80", 4, &st), 4);
	EXPECT_EQ("failed to accept initial byte", __mbrlen("\xc2", 1, &st), (size_t)-2);
	EXPECT_EQ("failed to resume", __mbrlen("\x80", 1, &st), 1);
	EXPECT_EQ( "failed to accept initial byte", __mbrlen("\xc2", 1, &st), (size_t)-2);
}

static char *loc_t[] = {
    "en_US.UTF-8",
    "en_GB.UTF-8",
    "en.UTF-8",
    "POSIX.UTF-8",
    "C.UTF-8",
    "UTF-8",
    "",
};

#define LENGTH(x) (sizeof(x) / sizeof *(x))

/**
 * @tc.name      : __mbrlen_0700
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void __mbrlen_0700(void)
{  
    for (unsigned int i = 0; i < LENGTH(loc_t); i++) {
        char *loc = setlocale(LC_CTYPE, loc_t[i]);
        if (loc != NULL) {
            __mbrlen_test();
        }
    }
}

int main(void)
{
    __mbrlen_0100();
    __mbrlen_0200();
    __mbrlen_0300();
    __mbrlen_0400();
    __mbrlen_0500();
    __mbrlen_0600();
    __mbrlen_0700();
    return t_status;
}