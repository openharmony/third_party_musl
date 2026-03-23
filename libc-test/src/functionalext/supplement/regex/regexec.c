/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <regex.h>
#include "functionalext.h"


#define TEST_BUFFER_SIZE 100

/**
 * @tc.name      : regexec_0100
 * @tc.desc      : Match string
 * @tc.level     : Level 0
 */
void regexec_0100(void)
{
    regex_t reg;
    const char *pattern = "musl";
    const char *str = "musl test";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0100", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0100", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0100", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0200
 * @tc.desc      : Alternation match
 * @tc.level     : Level 0
 */
void regexec_0200(void)
{
    regex_t reg;
    const char *pattern = "a|b";
    const char *str = "ade"; 
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0200", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0200", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0200", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0300
 * @tc.desc      : Repetition with zero occurrences
 * @tc.level     : Level 0
 */
void regexec_0300(void)
{
    regex_t reg;
    const char *pattern = "m*";
    const char *str = "abc";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0300", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0300", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0300", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0400
 * @tc.desc      : Nested repetition
 * @tc.level     : Level 0
 */
void regexec_0400(void)
{
    regex_t reg;
    const char *pattern = "(m+)+l";
    const char *str = "mmmmmmmml";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0400", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0400", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0400", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0500
 * @tc.desc      : long repetition
 * @tc.level     : Level 0
 */
void regexec_0500(void)
{
    regex_t reg;
    const char *pattern = "m{100}";
    char str[TEST_BUFFER_SIZE + 1] = {0};
    for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
        str[i] = 'm';
    }
    str[TEST_BUFFER_SIZE] = '\0';
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0500", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0500", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0500", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0600
 * @tc.desc      : Mismatched brace
 * @tc.level     : Level 0
 */
void regexec_0600(void)
{
    regex_t reg;
    const char *pattern = "m{5";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_NE("regexec_0600", ret, 0);
}

/**
 * @tc.name      : regexec_0700
 * @tc.desc      : Subexpression count exceeds implementation limit (test robustness)
 * @tc.level     : Level 0
 */
void regexec_0700(void)
{
    regex_t reg;
    const char *pattern = "^musl.*";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0700", ret, 0);
    const char *match_str = "musl test 123";
    ret = regexec(&reg, match_str, 0, NULL, 0);
    if (ret != 0) {
        regfree(&reg);
        EXPECT_EQ("regexec_0700", ret, 0);
        return;
    }
    EXPECT_EQ("regexec_0700", ret, 0);
    const char *non_match_str = "test musl 456";
    ret = regexec(&reg, non_match_str, 0, NULL, 0);
    if (ret == 0) {
        regfree(&reg);
        EXPECT_NE("regexec_0700", ret, 0);
        return;
    }
    EXPECT_NE("regexec_0700", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0800
 * @tc.desc      : Match string containing null byte
 * @tc.level     : Level 0
 */
void regexec_0800(void)
{
    regex_t reg;
    const char *pattern = "mu.*sl";
    char str[] = "mu\x00sl";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_0800", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == 0) {
        regfree(&reg);
        EXPECT_NE("regexec_0800", ret, 0);
        return;
    }
    EXPECT_NE("regexec_0800", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_0900
 * @tc.desc      : Use of bad interval expression
 * @tc.level     : Level 0
 */
void regexec_0900(void)
{
    regex_t reg;
    const char *pattern = "m{4,2}";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_NE("regexec_0900", ret, 0);
}

/**
 * @tc.name      : regexec_01000
 * @tc.desc      : Bad backreference
 * @tc.level     : Level 0
 */
void regexec_01000(void)
{
    regex_t reg;
    const char *pattern = "(m)\\1";
    int ret = regcomp(&reg, pattern, 0);
    EXPECT_NE("regexec_01000", ret, 0);
}

/**
 * @tc.name      : regexec_01100
 * @tc.desc      : Pattern with unescaped forward
 * @tc.level     : Level 0
 */
void regexec_01100(void)
{
    regex_t reg;
    const char *pattern = "[/]";
    const char *str = "test/dev/data";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01100", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01100", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01200
 * @tc.desc      : Empty character
 * @tc.level     : Level 0
 */
void regexec_01200(void)
{
    regex_t reg;
    const char *pattern = "[]";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_NE("regexec_01200", ret, 0);
}

/**
 * @tc.name      : regexec_01300
 * @tc.desc      : Character class with range reversal
 * @tc.level     : Level 0
 */
void regexec_01300(void)
{
    regex_t reg;
    const char *pattern = "[s-m]";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_NE("regexec_01300", ret, 0);
}

/**
 * @tc.name      : regexec_01400
 * @tc.desc      : Match number
 * @tc.level     : Level 0
 */
void regexec_01400(void)
{
    regex_t reg;
    const char *pattern = "[0-9]+";
    const char *match_str = "musl123test";
    const char *non_match_str = "musl";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01400", ret, 0);
    ret = regexec(&reg, match_str, 0, NULL, 0);
    EXPECT_EQ("regexec_01400", ret, 0);
    ret = regexec(&reg, non_match_str, 0, NULL, 0);
    EXPECT_NE("regexec_01400", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01500
 * @tc.desc      : Match at end of string with "$" 
 * @tc.level     : Level 0
 */
void regexec_01500(void)
{
    regex_t reg;
    const char *pattern = "test$";
    const char *str = "musltest";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01500", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01500", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01600
 * @tc.desc      : Match empty string
 * @tc.level     : Level 0
 */
void regexec_01600(void)
{
    regex_t reg;
    const char pattern[] = "^";
    const char str[] = "";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01600", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01600", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01700
 * @tc.desc      : Match empty string
 * @tc.level     : Level 0
 */
void regexec_01700(void)
{
    regex_t reg;
    const char *pattern = "m*";
    const char *str = "";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01700", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01700", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01800
 * @tc.desc      : Use of escaped metacharacter
 * @tc.level     : Level 0
 */
void regexec_01800(void)
{
    regex_t reg;
    const char *pattern = "[\\^]";
    const char *str = "musl ^ test";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01800", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01800", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_01900
 * @tc.desc      : Match empty string
 * @tc.level     : Level 0
 */
void regexec_01900(void)
{
    regex_t reg;
    const char *pattern = "^$";
    const char *str = "";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_01900", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_EQ("regexec_01900", ret, 0);
    regfree(&reg);
}

/**
 * @tc.name      : regexec_02000
 * @tc.desc      : Complex test
 * @tc.level     : Level 0
 */
void regexec_02000(void)
{
    regex_t reg;
    const char *pattern = "^((m|u)+s)*l$";
    const char *str = "mussssl";
    int ret = regcomp(&reg, pattern, REG_EXTENDED);
    EXPECT_EQ("regexec_02000", ret, 0);
    ret = regexec(&reg, str, 0, NULL, 0);
    EXPECT_NE("regexec_02000", ret, 0);

    const char *str2 = "muuussssl";
    ret = regexec(&reg, str2, 0, NULL, 0);
    EXPECT_NE("regexec_02000", ret, 0);
    const char *str3 = "musl";
    ret = regexec(&reg, str3, 0, NULL, 0);
    EXPECT_EQ("regexec_02000", ret, 0);
    regfree(&reg);
}


int main(void)
{
    regexec_0100();
    regexec_0200();
    regexec_0300();
    regexec_0400();
    regexec_0500();
    regexec_0600();
    regexec_0700();
    regexec_0800();
    regexec_0900();
    regexec_01000();
    regexec_01100();
    regexec_01200();
    regexec_01300();
    regexec_01400();
    regexec_01500();
    regexec_01600();
    regexec_01700();
    regexec_01800();
    regexec_01900();
    regexec_02000();
    return t_status;
}
