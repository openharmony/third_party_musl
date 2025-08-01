/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include "hook.h"


using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {

const int COUNT = 62;
const int SIZE = 128;
const int BPMF_COUNT = 43;
const int RET_TRUE = 1;
const int RET_FALSE = 0;

class IsalnumlTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name: isalnum_l_test_001
 * @tc.desc: Verify isalnum_l process success when using the en_US.UTF-8 character set.
 *           The parameter c is an English letter,
 *           and it is judged that the input character is a letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_001, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_002
 * @tc.desc: Verify isalnum_l process success when using the en_US.UTF-8 character set.
 *           The parameter c is an English number,
 *           and it is judged that the input character is a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_002, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "0123456789";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_003
 * @tc.desc: Verify isalnum_l process fail when using the en_US.UTF-8 character set.
 *           The parameter c is an special character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_003, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "@#$%^&";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_004
 * @tc.desc: Verify isalnum_l process fail when using the en_US.UTF-8 character set.
 *           Determine the number of letters and numbers in the ascii code table.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_004, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_005
 * @tc.desc: Verify isalnum_l process success when using the en_US.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_005, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_006
 * @tc.desc: Verify isalnum_l process success when using the en_US.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_006, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"0123456789";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_007
 * @tc.desc: Verify isalnum_l process fail when using the en_US.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_007, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"@#$%^&";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_008
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           The parameter c is an English letter,
 *           and it is judged that the input character is a letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_008, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_009
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           The parameter c is an English number,
 *           and it is judged that the input character is a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_009, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const char *str = "0123456789";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_010
 * @tc.desc: Verify isalnum_l process fail when using the zh_CNcharacter set.
 *           The parameter c is an special character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_010, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const char *str = "@#$%^&";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_011
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN character set.
 *           Determine the number of letters and numbers in the ascii code table.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_011, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    int total = 0;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_012
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_012, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE( ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_013
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_013, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const wchar_t *str = L"0123456789";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_014
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_014, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const wchar_t *str = L"@#$%^&";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_015
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           The characters are not in the en_US.UTF-8 and zh_CN character sets,
 *           and it is judged that the input character is a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_015, TestSize.Level0)
{
    int ret = RET_FALSE;
    int i = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //Arabic characters(U+0660 - U+0669)
    for (i = 0x0660; i <= 0x0669; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    //GREEK characters(U+03AC - U+03CE)
    //GREEK SMALL LETTER
    for (i = 0x03AC; i <= 0x03CE; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    //ARMENIAN characters(U+0531 - U+0556)
    //ARMENIAN CAPITAL LETTER
    for (i = 0x0531; i <= 0x0556; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_016
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN character set.
 *           The characters are not in the en_US.UTF-8 and zh_CN character sets,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_016, TestSize.Level0)
{
    int ret = RET_FALSE;
    int i = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //GREEK TONOS
    //GREEK DIALYTIKA TONOS
    for (i = 0x0384; i <= 0x0385; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
    }

    //ARMENIAN APOSTROPHE
    //ARMENIAN EMPHASIS MARK
    //ARMENIAN EXCLAMATION MARK
    //ARMENIAN COMMA
    //ARMENIAN QUESTION MARK
    //ARMENIAN ABBREVIATION MARK
    for (i = 0x055A; i <= 0x055F; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_017
 * @tc.desc: Verify isalnum_l process success when using the zh_CN character set.
 *           BOPOMOFO LETTER in the unicode is letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_017, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    int total = 0;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //0x3105(BOPOMOFO LETTER B) ~ 0x312f(BOPOMOFO LETTER NN)
    for (int i = 0x3105; i <= 0x312f; i++) {
        ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(total, BPMF_COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_018
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           The parameter c is an English letter,
 *           and it is judged that the input character is a letter.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_018, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_019
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           The parameter c is an English number,
 *           and it is judged that the input character is a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_019, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "0123456789";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_020
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *           The parameter c is an special character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.type: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_020, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "@#$%^&";
    const char *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != '\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_test_021
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *           Determine the number of letters and numbers in the ascii code table.
 * @tc.level: FUNC
 */
HWTEST_F(IsalnumlTest, isalnum_l_test_021, TestSize.Level0)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2200
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a letter.
 * @tc.level: FUNC
 */
void isalnum_l_2200(void)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2300
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input wide character is a number.
 * @tc.level: FUNC
 */
void isalnum_l_2300(void)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"0123456789";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2400
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *           The parameter c is an wide English character,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.level: FUNC
 */
void isalnum_l_2400(void)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"@#$%^&";
    const wchar_t *p = str;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    while (p && *p != L'\0') {
        ret = isalnum_l(*p, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2500
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           The characters are not in the en_US.UTF-8 and zh_CN.UTF-8 character sets,
 *           and it is judged that the input character is a letter or a number.
 * @tc.level: FUNC
 */
void isalnum_l_2500(void)
{
    int ret = RET_FALSE;
    int i = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //Arabic characters(U+0660 - U+0669)
    for (i = 0x0660; i <= 0x0669; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    //GREEK characters(U+03AC - U+03CE)
    //GREEK SMALL LETTER
    for (i = 0x03AC; i <= 0x03CE; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    //ARMENIAN characters(U+0531 - U+0556)
    //ARMENIAN CAPITAL LETTER
    for (i = 0x0531; i <= 0x0556; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_TRUE);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2600
 * @tc.desc: Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *           The characters are not in the en_US.UTF-8 and zh_CN.UTF-8 character sets,
 *           and it is judged that the input character is not a letter or a number.
 * @tc.level: FUNC
 */
void isalnum_l_2600(void)
{
    int ret = RET_FALSE;
    int i = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //GREEK TONOS
    //GREEK DIALYTIKA TONOS
    for (i = 0x0384; i <= 0x0385; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
    }

    //ARMENIAN APOSTROPHE
    //ARMENIAN EMPHASIS MARK
    //ARMENIAN EXCLAMATION MARK
    //ARMENIAN COMMA
    //ARMENIAN QUESTION MARK
    //ARMENIAN ABBREVIATION MARK
    for (i = 0x055A; i <= 0x055F; i++) {
        ret = isalnum_l(i, m_locale);
        EXPECT_EQ(ret, RET_FALSE);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name: isalnum_l_2700
 * @tc.desc: Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *           BOPOMOFO LETTER in the unicode is letter.
 * @tc.level: FUNC
 */
void isalnum_l_2700(void)
{
    int ret = RET_FALSE;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        EXPECT_NE(ret, RET_FALSE);
        return ;
    }

    //0x3105(BOPOMOFO LETTER B) ~ 0x312f(BOPOMOFO LETTER NN)
    for (int i = 0x3105; i <= 0x312f; i++) {
        ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(total, BPMF_COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

}
}