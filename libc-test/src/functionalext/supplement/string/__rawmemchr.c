/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <functionalext.h>

#define BUFFER_SIZE 4096

/**
 * @tc.name      : __rawmemchr_0100
 * @tc.desc      : Search for a character in the specified memory block
 * @tc.level     : Level 0
 */
void __rawmemchr_0100(void)
{
    const char *src = "abcd";

    // Basic test.
    char *ret = __rawmemchr(src, 'c');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + 2);
    // Beginning.
    ret = __rawmemchr(src, 'a');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src);

    // End of string.
    ret = __rawmemchr(src, 'd');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + 3);
    // Finding NUL. 
    ret = __rawmemchr(src, '\0');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src + strlen(src));
    const char *src1 = "bili";
    ret = __rawmemchr(src1, 'i');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src1 + 1);
    // NUL in empty string.
    const char *src2 = "";
    ret = __rawmemchr(src2, '\0');
    EXPECT_PTRNE("__rawmemchr_0100", ret, NULL);
    EXPECT_STREQ("__rawmemchr_0100", ret, src2);
}

/**
 * @tc.name      : __rawmemchr_0200
 * @tc.desc      : Provide an exception parameter to search for characters in the specified memory block
 * @tc.level     : Level 0
 */
void __rawmemchr_0200(void)
{
    const char *src = "this is __rawmemchr_0200 test.";
    char *ret = __rawmemchr(src, 'x');
    EXPECT_LT("__rawmemchr_0200", src + strlen(src) + 1, ret);
}

/**
 * @tc.name      : __rawmemchr_0300
 * @tc.desc      : Test __rawmemchr memory alignment
 * @tc.level     : Level 0
 */
void __rawmemchr_0300(void)
{
    const int CONSTANT_FF = 0xFF;
    char buf[BUFFER_SIZE];
    int i;
    char *p;
    for (i = 0; i < CONSTANT_FF; i++) {
        p = (char *)((uintptr_t)(buf + 0xff) & ~0xff) + i;
        strcpy(p, "ok");
        strcpy(p + 3, "bad/wrong");
        char *ret = __rawmemchr(p, 'r');
        EXPECT_PTRNE("__rawmemchr_0300", ret, NULL);
        EXPECT_STREQ("__rawmemchr_0300", ret, p + 8);
    }
}

int main(void)
{
    __rawmemchr_0100();
    __rawmemchr_0200();
    __rawmemchr_0300();
    return t_status;
}
