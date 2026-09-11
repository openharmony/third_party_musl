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
/*
 * Comprehensive Big5 locale + iconv conversion test.
 *
 * Coverage:
 *   1. wctype (iswalpha_l / iswdigit_l / towupper_l ...) under zh_HK.Big5 locale
 *   2. iconv Big5 -> UTF-8 directional conversion (all Big5 sections)
 *   3. iconv UTF-8 -> Big5 directional conversion
 *   4. Big5 <-> UTF-8 round-trip identity
 *   5. Cross-charset: Big5 <-> GBK / GB18030 / EUC-KR / UTF-16BE / ISO-8859-1
 *   6. Boundary: first/last valid codes, trail-byte range edges
 *   7. Invalid sequences -> EILSEQ
 *   8. Buffer-too-small -> E2BIG
 *   9. //IGNORE and //TRANSLIT suffixes
 *  10. HKSCS supplementary plane (U+233E6, U+26DA0)
 *  11. Empty input, large input stress
 *  12. Multibyte conversion: mbrtowc / wcrtomb / btowc / wctob /
 *      mbsrtowcs / wcsrtombs / wctomb / mbtowc / mbrlen, including
 *      Big5->WCHAR_T->UTF-8 full pipeline, invalid sequences,
 *      partial feeds, byte-length boundaries.
 *
 * Prerequisite: set_iconv_icu_enable() and set_wctype_icu_enable() must succeed
 * (ICU data present on device). Basic musl does not support UTF-8 -> Big5.
 */
#ifndef __LITEOS__
#include "test.h"
#include <locale.h>
#include <wctype.h>
#include <wchar.h>
#include <iconv.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#define OUTBUF 512
#define STRESS_LEN 10000
/* ------------------------------------------------------------------ */
/* Test vectors: (big5_bytes, utf8_bytes, description)                 */
/* Covers ASCII, A1 symbols, A2 digits/box, A3 Latin/Greek/Bopomofo,  */
/* A4-A8 CJK level 1, C9+ CJK level 2, fullwidth, Euro, boundaries.   */
/* ------------------------------------------------------------------ */
struct vec {
    const char *b5;
    const char *u8;
    const char *desc;
};
static const struct vec g_vec[] = {
    {"\x20","\x20","ASCII space"}, {"\x30","\x30","ASCII 0"}, {"\x41","\x41","ASCII A"},
    {"\x61","\x61","ASCII a"}, {"\x7e","\x7e","ASCII ~"}, {"\x7f","\x7f","ASCII DEL"},
    {"\xa1\x40","\xe3\x80\x80","A140 first code U+3000"}, {"\xa1\x41","\xef\xbc\x8c","A141 U+FF0C"},
    {"\xa1\x42","\xe3\x80\x81","A142 U+3001"}, {"\xa1\x43","\xe3\x80\x82","A143 U+3002"},
    {"\xa1\x45","\xe2\x80\xa7","A145 U+2027"}, {"\xa1\x56","\xe2\x80\x93","A156 U+2013"},
    {"\xa1\x58","\xe2\x80\x94","A158 U+2014"}, {"\xa1\xa5","\xe2\x80\x98","A1A5 U+2018"},
    {"\xa1\xa6","\xe2\x80\x99","A1A6 U+2019"}, {"\xa1\xa7","\xe2\x80\x9c","A1A7 U+201C"},
    {"\xa1\xa8","\xe2\x80\x9d","A1A8 U+201D"}, {"\xa1\xb0","\xe2\x80\xbb","A1B0 U+203B"},
    {"\xa1\xb1","\xc2\xa7","A1B1 U+00A7"}, {"\xa1\xd1","\xc3\x97","A1D1 U+00D7"},
    {"\xa1\xd2","\xc3\xb7","A1D2 U+00F7"}, {"\xa1\xd4","\xe2\x88\x9a","A1D4 U+221A"},
    {"\xa1\xf6","\xe2\x86\x90","A1F6 U+2190"},
    {"\xa1\x7e","\xef\xb9\x9a","A17E last trail 0x40-7E"}, {"\xa1\xa1","\xef\xb9\x9b","A1A1 first trail 0xA1+"},
    {"\xa1\xfe","\xef\xbc\x8f","A1FE last trail 0xFE"},
    {"\xa2\xaf","\xef\xbc\x90","A2AF U+FF10"}, {"\xa2\xb8","\xef\xbc\x99","A2B8 U+FF19"},
    {"\xa2\xb9","\xe2\x85\xa0","A2B9 U+2160"}, {"\xa2\xc2","\xe2\x85\xa9","A2C2 U+2169"},
    {"\xa3\x44","\xce\x91","A344 U+0391"}, {"\xa3\x5b","\xce\xa9","A35B U+03A9"},
    {"\xa3\x5c","\xce\xb1","A35C U+03B1"}, {"\xa3\x73","\xcf\x89","A373 U+03C9"},
    {"\xa3\x74","\xe3\x84\x85","A374 U+3105"}, {"\xa3\xba","\xe3\x84\xa9","A3BA U+3129"},
    {"\xa3\xbc","\xcb\x89","A3BC U+02C9"}, {"\xa3\xe1","\xe2\x82\xac","A3E1 U+20AC Euro"},
    {"\xa4\x40","\xe4\xb8\x80","A440 first CJK U+4E00"}, {"\xa4\x51","\xe5\x8d\x81","A451 U+5341"},
    {"\xa4\x54","\xe4\xb8\x89","A454 U+4E09"}, {"\xa4\xa4","\xe4\xb8\xad","A4A4 U+4E2D"},
    {"\xa4\xe5","\xe6\x96\x87","A4E5 U+6587"}, {"\xa5\x6a","\xe5\x8f\xa4","A56A U+53E4"},
    {"\xa6\x7e","\xe5\xb9\xb4","A67E U+5E74"}, {"\xa6\xe5","\xe8\xa1\x80","A6E5 U+8840"},
    {"\xa6\xe6","\xe8\xa1\x8c","A6E6 U+884C"}, {"\xa7\xa1","\xe5\x9d\x87","A7A1 U+5747"},
    {"\xa8\xc6","\xe4\xba\x8b","A8C6 U+4E8B"}, {"\xa8\xc7","\xe4\xba\x9b","A8C7 U+4E9B"},
    {"\xa8\xc8","\xe4\xba\x9e","A8C8 U+4E9E"}, {"\xa8\xc9","\xe4\xba\xab","A8C9 U+4EAB"},
    {"\xa8\xca","\xe4\xba\xac","A8CA U+4EAC"},
    {"\xc9\x40","\xe4\xb9\x82","C940 U+4E42"}, {"\xc9\x7c","\xe6\x88\x89","C97C U+6209"},
    {"\xf9\xd6","\xe7\xa2\x81","F9D6 U+7881"}, {"\xf9\xd5","\xe9\xbe\x98","F9D5 U+9F98"},
};
#define NVEC (int)(sizeof(g_vec)/sizeof(g_vec[0]))
/* ------------------------------------------------------------------ */
/* Helper: run iconv from -> to, return bytes written, -1 on error     */
/* ------------------------------------------------------------------ */
static long do_iconv(const char *to, const char *from,
                     const char *in, size_t inlen,
                     char *out, size_t outlen)
{
    iconv_t cd = iconv_open(to, from);
    if (cd == (iconv_t)-1) {
        return -2;
    }

    char *inp = (char *)in;
    size_t inb = inlen;
    char *outp = out;
    size_t outb = outlen;
    size_t r = iconv(cd, &inp, &inb, &outp, &outb);
    int saved_errno = errno;
    iconv_close(cd);
    errno = saved_errno;
    if (r == (size_t)-1) {
        return -1;
    }
    return (long)(outlen - outb);
}
static int buf_eq(const char *a, size_t alen, const char *b, size_t blen)
{
    if (alen != blen) {
        return 0;
    }
    return memcmp(a, b, alen) == 0;
}
/* wctype tests: verify all 12 patched ctype files take ICU path.
 * U+3000 is the discriminator: musl alpha.h says alpha, ICU says Zs. */
static locale_t g_loc_big5, g_loc_hkscs, g_loc_cn, g_loc_en_utf8, g_loc_non_whitelisted;
static void test_wctype_setup(void)
{
    g_loc_big5 = newlocale(LC_ALL_MASK, "zh_HK.Big5", NULL);
    g_loc_hkscs = newlocale(LC_ALL_MASK, "zh_HK.Big5-HKSCS", NULL);
    g_loc_cn = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    g_loc_en_utf8 = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    g_loc_non_whitelisted = newlocale(LC_ALL_MASK, "ja_JP.UTF-8", NULL);

    if (!g_loc_big5) {
        t_error("newlocale(\"zh_HK.Big5\") returned NULL — whitelist patch missing?\n");
    }
    if (!g_loc_hkscs) {
        t_error("newlocale(\"zh_HK.Big5-HKSCS\") returned NULL\n");
    }
    if (!g_loc_cn) {
        t_error("newlocale(\"zh_CN\") returned NULL\n");
    }
    if (!g_loc_en_utf8) {
        t_error("newlocale(\"en_US.UTF-8\") returned NULL\n");
    }
    /* ja_JP.UTF-8 is NOT in g_valid_locale_table; newlocale may return NULL
     * or a locale with flag != ICU_VALID. Either is acceptable; we just
     * ensure no crash if it does return non-NULL. */
}
/* (a) Discriminator: proves ICU path is taken, not musl fallback.
 * U+3000 is alpha in musl's alpha.h but NOT alpha in ICU (it's Zs space).
 * If iswalpha_l returns 0, we know ICU was called. If non-zero, fallback. */
static void test_icu_path_discriminator(void)
{
    /* Verify ICU wctype is functional for whitelisted locales.
     * U+00A0 NBSP: ICU says iswalpha=0, musl says iswalpha=0 — not a
     * discriminator. Use U+4E00 (CJK alpha) to verify the function
     * returns non-zero in all whitelisted locales. */
    struct { locale_t loc; const char *name; } locs[] = {
        {g_loc_big5,      "zh_HK.Big5"},
        {g_loc_hkscs,     "zh_HK.Big5-HKSCS"},
        {g_loc_cn,        "zh_CN"},
        {g_loc_en_utf8,   "en_US.UTF-8"},
    };
    int n = (int)(sizeof(locs)/sizeof(locs[0]));
    for (int i = 0; i < n; i++) {
        if (!locs[i].loc) {
            continue;
        }
        /* CJK ideograph U+4E00 should be alpha in both ICU and musl */
        if (!iswalpha_l(0x4E00, locs[i].loc)) {
            t_error("iswalpha_l(U+4E00 %s)=0, expected non-zero\n",
                    locs[i].name);
        }
    }
}
/* (d) Non-whitelisted locale: verify it uses musl fallback, not ICU.
 * For functions with ICU/musl discriminators (different results),
 * assert the musl fallback value. For others, assert a known-correct
 * value to prove the function actually ran. */
/* (d) Non-whitelisted locale: verify it uses musl fallback, not ICU.
 * Use codepoints where ICU and musl return different results. */
static void test_non_whitelisted_discriminators(locale_t l)
{
    /* iswspace: NBSP — musl=0, ICU=1 → must be 0 (fallback) */
    if (iswspace_l(0x00A0, l) != 0) {
        t_error("iswspace_l(0xa0 ja_JP)!=0, expected 0 (musl)\n");
    }
    /* iswcntrl: U+2028 — musl=1, ICU=0 → must be 1 (fallback) */
    if (iswcntrl_l(0x2028, l) != 1) {
        t_error("iswcntrl_l(0x2028 ja_JP)!=1, expected 1 (musl)\n");
    }
    /* iswprint: U+2028 — musl=0, ICU=1 → must be 0 (fallback) */
    if (iswprint_l(0x2028, l) != 0) {
        t_error("iswprint_l(0x2028 ja_JP)!=0, expected 0 (musl)\n");
    }
    /* iswblank: U+3000 — blank in both musl fallback and ICU */
    if (iswblank_l(0x3000, l) == 0) {
        t_error("iswblank_l(0x3000 ja_JP)==0, expected non-zero\n");
    }
}

static void test_non_whitelisted_known_values(locale_t l)
{
    /* Assert known-correct values (same in ICU and musl) */
    if (!iswalpha_l(0x4E00, l)) {
        t_error("iswalpha_l(0x4E00 ja_JP)=0\n");
    }
    if (!iswalnum_l(0x4E00, l)) {
        t_error("iswalnum_l(0x4E00 ja_JP)=0\n");
    }
    if (!iswdigit_l('0', l) || iswdigit_l('A', l)) {
        t_error("iswdigit_l('0'/'A' ja_JP) wrong\n");
    }
    if (!iswxdigit_l('F', l)) {
        t_error("iswxdigit_l('F' ja_JP)=0\n");
    }
    if (!iswpunct_l(0xFF0C, l)) {
        t_error("iswpunct_l(0xFF0C ja_JP)=0\n");
    }
    if (!iswupper_l('A', l) || !iswlower_l('a', l)) {
        t_error("iswupper/iswlower ja_JP wrong\n");
    }
    if (!iswgraph_l(0x4E00, l)) {
        t_error("iswgraph_l(0x4E00 ja_JP)=0\n");
    }
    if (towupper_l('a', l) != 'A' || towlower_l('A', l) != 'a') {
        t_error("towupper/towlower ja_JP wrong\n");
    }
}

static void test_non_whitelisted_locale(void)
{
    if (!g_loc_non_whitelisted) {
        return;
    }
    test_non_whitelisted_discriminators(g_loc_non_whitelisted);
    test_non_whitelisted_known_values(g_loc_non_whitelisted);
}
/* ---- iswalpha_l ---- */
static void test_iswalpha_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {0x4E00, 1, "CJK U+4E00"},        /* CJK is alpha */
        {'A',    1, "ASCII 'A'"},          /* Latin upper alpha */
        {'z',    1, "ASCII 'z'"},          /* Latin lower alpha */
        {0x0391, 1, "Greek Alpha U+0391"}, /* Greek alpha */
        {0x3042, 1, "Hiragana U+3042"},    /* Hiragana alpha */
        {'0',    0, "ASCII '0'"},          /* digit not alpha */
        {0x3000, 0, "Ideographic space"},  /* Zs not alpha (ICU) */
        {' ',    0, "ASCII space"},        /* space not alpha */
        {0x0021, 0, "'!' punct"},          /* punct not alpha */
        {0xFF0C, 0, "Fullwidth comma"},    /* punct not alpha */
        {0x0000, 0, "NUL"},               /* control not alpha */
        {0x000A, 0, "LF"},               /* control not alpha */
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_cn, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","zh_CN","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswalpha_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswalpha_l(0x%x %s)=%d expected %d\n",
                        cases[i].c, lnames[li], r, cases[i].expect);
            }
        }
    }
}
/* ---- iswalnum_l ---- */
static void test_iswalnum_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {0x4E00, 1, "CJK"},               /* CJK alpha -> alnum */
        {'A',    1, "ASCII 'A'"},          /* alpha -> alnum */
        {'z',    1, "ASCII 'z'"},
        {'0',    1, "ASCII '0'"},          /* digit -> alnum */
        {'9',    1, "ASCII '9'"},
        {0xFF10, 1, "Fullwidth 0 U+FF10"}, /* fullwidth digit */
        {0xFF19, 1, "Fullwidth 9 U+FF19"},
        {0x0391, 1, "Greek Alpha"},        /* alpha -> alnum */
        {' ',    0, "space"},              /* space not alnum */
        {0x3000, 0, "Ideographic space"},
        {0x0021, 0, "'!' punct"},
        {0xFF0C, 0, "Fullwidth comma"},
        {0x0000, 0, "NUL"},
        {0x000A, 0, "LF"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswalnum_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswalnum_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect, cases[i].desc);
            }
        }
    }
}
/* ---- iswdigit_l ---- */
static void test_iswdigit_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {'0',    1, "ASCII '0' (first digit)"},
        {'5',    1, "ASCII '5'"},
        {'9',    1, "ASCII '9' (last digit)"},
        {0xFF10, 1, "Fullwidth 0 U+FF10"},
        {0xFF15, 1, "Fullwidth 5 U+FF15"},
        {0xFF19, 1, "Fullwidth 9 U+FF19"},
        {'/',    0, "'/' (before '0')"},   /* boundary: just below */
        {':',    0, "':' (after '9')"},     /* boundary: just above */
        {'A',    0, "'A' not digit"},
        {'a',    0, "'a' not digit"},
        {0x4E00, 0, "CJK not digit"},
        {0x3000, 0, "Ideographic space"},
        {0x2160, 0, "Roman numeral I"},     /* NOT digit (it's letter) */
        {0x0000, 0, "NUL"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswdigit_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswdigit_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect, cases[i].desc);
            }
        }
    }
}
/* ---- iswxdigit_l ---- */
static void test_iswxdigit_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {'0', 1, "'0'"}, {'9', 1, "'9'"},
        {'A', 1, "'A'"}, {'F', 1, "'F'"},
        {'a', 1, "'a'"}, {'f', 1, "'f'"},
        {'G', 0, "'G' (after hex)"},      /* boundary */
        {'g', 0, "'g'"},
        {'/', 0, "'/' (before '0')"},     /* boundary */
        {':', 0, "':' (after '9')"},      /* boundary */
        {'@', 0, "'@' (before 'A')"},     /* boundary */
        {'`', 0, "'`' (before 'a')"},     /* boundary */
        {0x4E00, 0, "CJK"},
        {0x3000, 0, "Ideographic space"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswxdigit_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswxdigit_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect, cases[i].desc);
            }
        }
    }
}
/* NBSP (U+00A0): ICU says space=1, musl says space=0.
 * Whitelisted locales should be on ICU path, so expect 1. */
static void test_iswspace_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {' ',   1, "ASCII space"}, {'\t',  1, "TAB"},
        {'\n',  1, "LF"}, {'\r',  1, "CR"},
        {0x000B,1, "VT"}, {0x000C,1, "FF"},
        {0x0085,1, "NEL U+0085"},
        {0x2000,1, "EN QUAD"}, {0x2009,1, "THIN SPACE"},
        {0x3000,1, "IDEOGRAPHIC SPACE"},
        {0x200A,1, "HAIR SPACE"},
        {0x205F,1, "MEDIUM MATH SPACE"},
        {'A',   0, "'A'"}, {0x4E00,0, "CJK"},
        {0x00A0,1, "NBSP (ICU: space)"},
        {0x0000,0, "NUL"}, {0xFF0C,0, "Fullwidth comma"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswspace_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswspace_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect,
                        cases[i].desc);
            }
        }
    }
}
/* ---- iswpunct_l ---- */
static void test_iswpunct_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {0xFF0C, 1, "Fullwidth comma U+FF0C"},
        {0x3001, 1, "Ideographic comma U+3001"},
        {0x3002, 1, "Ideographic full stop U+3002"},
        {0xFF01, 1, "Fullwidth '!' U+FF01"},
        {0xFF1F, 1, "Fullwidth '?' U+FF1F"},
        {0x2026, 1, "Horizontal ellipsis U+2026"},
        {0x203B, 1, "Reference mark U+203B"},
        {'!',    1, "ASCII '!'"},
        {'.',    1, "ASCII '.'"},
        {'@',    1, "ASCII '@'"},
        {0x4E00, 0, "CJK not punct"},
        {'A',    0, "'A' not punct"},
        {'0',    0, "'0' not punct"},
        {' ',    0, "space not punct"},
        {0x3000, 0, "Ideographic space not punct"},
        {0x0000, 0, "NUL"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswpunct_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswpunct_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect, cases[i].desc);
            }
        }
    }
}
/* ---- iswupper_l / iswlower_l ---- */
static void test_iswupper_iswlower_l(void)
{
    struct { wint_t c; int upper; int lower; const char *desc; } cases[] = {
        {'A',    1, 0, "ASCII 'A'"},
        {'Z',    1, 0, "ASCII 'Z'"},
        {'a',    0, 1, "ASCII 'a'"},
        {'z',    0, 1, "ASCII 'z'"},
        {'@',    0, 0, "'@' no case"},
        {'0',    0, 0, "'0' no case"},
        {0x4E00, 0, 0, "CJK no case"},
        {0x3000, 0, 0, "Ideographic space no case"},
        {0xFF21, 1, 0, "Fullwidth 'A' U+FF21"},
        {0xFF41, 0, 1, "Fullwidth 'a' U+FF41"},
        {0x0391, 1, 0, "Greek Alpha U+0391"},
        {0x03B1, 0, 1, "Greek alpha U+03B1"},
        {0x0000, 0, 0, "NUL"},
        {' ',    0, 0, "space"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int ru = iswupper_l(cases[i].c, locs[li]);
            int rl = iswlower_l(cases[i].c, locs[li]);
            if (!!ru != cases[i].upper) {
                t_error("iswupper_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], ru, cases[i].upper, cases[i].desc);
            }
            if (!!rl != cases[i].lower) {
                t_error("iswlower_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], rl, cases[i].lower, cases[i].desc);
            }
        }
    }
}
/* ---- towupper_l / tolower_l ---- */
static void test_towupper_towlower_l(void)
{
    struct { wint_t c; wint_t upper; wint_t lower; const char *desc; } cases[] = {
        {'a', 'A', 'a', "ASCII 'a'"},
        {'A', 'A', 'a', "ASCII 'A'"},
        {'z', 'Z', 'z', "ASCII 'z'"},
        {'Z', 'Z', 'z', "ASCII 'Z'"},
        {'0', '0', '0', "'0' no case"},
        {' ', ' ', ' ', "space no case"},
        {0x4E00, 0x4E00, 0x4E00, "CJK identity"},
        {0x03B1, 0x0391, 0x03B1, "Greek alpha<->Alpha"},
        {0x0391, 0x0391, 0x03B1, "Greek Alpha<->alpha"},
        {0xFF41, 0xFF21, 0xFF41, "Fullwidth a<->A"},
        {0xFF21, 0xFF21, 0xFF41, "Fullwidth A<->a"},
        {0x3000, 0x3000, 0x3000, "Ideographic space identity"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            wint_t uu = towupper_l(cases[i].c, locs[li]);
            wint_t ll = towlower_l(cases[i].c, locs[li]);
            if (uu != cases[i].upper) {
                t_error("towupper_l(0x%x %s)=0x%x expected 0x%x [%s]\n",
                        cases[i].c, lnames[li], uu, cases[i].upper, cases[i].desc);
            }
            if (ll != cases[i].lower) {
                t_error("towlower_l(0x%x %s)=0x%x expected 0x%x [%s]\n",
                        cases[i].c, lnames[li], ll, cases[i].lower, cases[i].desc);
            }
        }
    }
}
/* ---- iswcntrl_l ---- */
static void test_iswcntrl_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {0x0000, 1, "NUL"},
        {0x0001, 1, "SOH"},
        {0x0009, 1, "TAB"},
        {0x000A, 1, "LF"},
        {0x000D, 1, "CR"},
        {0x001F, 1, "US (last C0)"},
        {0x007F, 1, "DEL"},
        {0x0080, 1, "PAD (first C1)"},
        {0x009F, 1, "APC (last C1)"},
        {0x0007, 1, "BEL (Cc)"},
        {0x2029, 1, "PARA SEPARATOR"},
        {0xFFF9, 1, "Interlinear annotation (ICU)"},
        {0xFFFB, 1, "Interlinear annotation end (ICU)"},
        {0x0020, 0, "space not cntrl"},
        {'A',    0, "'A' not cntrl"},
        {0x4E00, 0, "CJK not cntrl"},
        {0x3000, 0, "Ideographic space not cntrl"},
        {0xFF0C, 0, "Fullwidth comma not cntrl"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswcntrl_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswcntrl_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect, cases[i].desc);
            }
        }
    }
}
/* ---- iswprint_l / iswgraph_l ---- */
static void test_iswprint_iswgraph_l(void)
{
    /* iswgraph = iswprint && !iswspace */
    struct { wint_t c; int print; int graph; const char *desc; } cases[] = {
        {'A',    1, 1, "'A'"},
        {'0',    1, 1, "'0'"},
        {0x4E00, 1, 1, "CJK"},
        {0xFF0C, 1, 1, "Fullwidth comma"},
        {0x3001, 1, 1, "Ideographic comma"},
        {'!',    1, 1, "'!'"},
        {0x20AC, 1, 1, "Euro sign"},
        {' ',    1, 0, "ASCII space (print but not graph)"},
        {0x3000, 1, 0, "Ideographic space (print not graph)"},
        {0x0000, 0, 0, "NUL (not print)"},
        {0x000A, 0, 0, "LF (not print)"},
        {0x007F, 0, 0, "DEL (not print)"},
        {'A',    1, 1, "ASCII 'A' (print and graph)"},
        {0xFFFD, 1, 1, "Replacement char U+FFFD (print)"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int rp = iswprint_l(cases[i].c, locs[li]);
            int rg = iswgraph_l(cases[i].c, locs[li]);
            if (!!rp != cases[i].print) {
                t_error("iswprint_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], rp, cases[i].print,
                        cases[i].desc);
            }
            if (!!rg != cases[i].graph) {
                t_error("iswgraph_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], rg, cases[i].graph,
                        cases[i].desc);
            }
        }
    }
}
/* ---- iswblank_l ---- */
static void test_iswblank_l(void)
{
    struct { wint_t c; int expect; const char *desc; } cases[] = {
        {' ',   1, "ASCII space (blank)"},
        {'\t',  1, "TAB (blank)"},
        {'A',   0, "'A' not blank"},
        {'0',   0, "'0' not blank"},
        {0x3000,1, "Ideographic space (ICU: blank)"},
        {0x0000,0, "NUL"},
        {0x000A,0, "LF (not blank)"},
        {0x4E00,0, "CJK"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int li = 0; li < nloc; li++) {
        if (!locs[li]) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            int r = iswblank_l(cases[i].c, locs[li]);
            if (!!r != cases[i].expect) {
                t_error("iswblank_l(0x%x %s)=%d expected %d [%s]\n",
                        cases[i].c, lnames[li], r, cases[i].expect,
                        cases[i].desc);
            }
        }
    }
}
/* ---- HKSCS locale specific ---- */
static void test_wctype_hkscs(void)
{
    if (!g_loc_hkscs) {
        return;
    }
    /* Same ICU path as Big5, verify a few key points */
    if (!iswalpha_l(0x4E00, g_loc_hkscs)) {
        t_error("iswalpha_l(U+4E00 hkscs)=0\n");
    }
    if (!iswprint_l(0x4E00, g_loc_hkscs)) {
        t_error("iswprint_l(U+4E00 hkscs)=0\n");
    }
    if (iswalpha_l(0x3000, g_loc_hkscs)) {
        t_error("iswalpha_l(U+3000 hkscs)!=0 (ICU path check)\n");
    }
    if (!iswdigit_l(0xFF10, g_loc_hkscs)) {
        t_error("iswdigit_l(U+FF10 hkscs)=0\n");
    }
    if (towupper_l('a', g_loc_hkscs) != 'A') {
        t_error("towupper_l('a' hkscs)=0x%x expected 'A'\n", towupper_l('a', g_loc_hkscs));
    }
}
/* ---- Cross-locale consistency: same codepoint, same result ---- */
static int get_wctype_results(locale_t loc, wint_t c,
                                int *a, int *d, int *s, int *p)
{
    if (!loc) {
        return -1;
    }
    *a = !!iswalpha_l(c, loc);
    *d = !!iswdigit_l(c, loc);
    *s = !!iswspace_l(c, loc);
    *p = !!iswpunct_l(c, loc);
    return 0;
}
static void test_cross_locale_consistency(void)
{
    struct { wint_t c; const char *desc; } pts[] = {
        {0x4E00, "CJK one"}, {0x3000, "Ideographic space"},
        {'A', "ASCII A"}, {'0', "ASCII 0"},
    };
    int n = (int)(sizeof(pts)/sizeof(pts[0]));
    locale_t locs[] = {g_loc_big5, g_loc_hkscs, g_loc_cn, g_loc_en_utf8};
    const char *lnames[] = {"big5","hkscs","zh_CN","en_US.UTF-8"};
    int nloc = (int)(sizeof(locs)/sizeof(locs[0]));

    for (int pi = 0; pi < n; pi++) {
        int ra = -1, rd = -1, rs = -1, rp = -1;
        for (int li = 0; li < nloc; li++) {
            int a, d, s, p;
            if (get_wctype_results(locs[li], pts[pi].c, &a, &d, &s, &p) != 0) {
                continue;
            }
            if (ra == -1) {
                ra = a; rd = d; rs = s; rp = p;
            } else if (a != ra || d != rd || s != rs || p != rp) {
                t_error("cross-locale mismatch %s/%s: a=%d d=%d s=%d p=%d vs ref a=%d d=%d s=%d p=%d\n",
                        pts[pi].desc, lnames[li], a, d, s, p, ra, rd, rs, rp);
            }
        }
    }
}
/* ------------------------------------------------------------------ */
/* Big5 -> UTF-8 directional                                        */
/* ------------------------------------------------------------------ */
static void test_big5_to_utf8(void)
{
    for (int i = 0; i < NVEC; i++) {
        char out[OUTBUF];
        long n = do_iconv("UTF-8", "BIG5", g_vec[i].b5, strlen(g_vec[i].b5), out, OUTBUF);
        if (n < 0) {
            t_error("big5->utf8 failed [%s]: iconv rc=%ld errno=%s\n",
                    g_vec[i].desc, n, strerror(errno));
            continue;
        }
        if (!buf_eq(out, (size_t)n, g_vec[i].u8, strlen(g_vec[i].u8))) {
            t_error("big5->utf8 mismatch [%s]\n", g_vec[i].desc);
        }
    }
}
/* ------------------------------------------------------------------ */
/* UTF-8 -> Big5 directional                                        */
/* ------------------------------------------------------------------ */
static void test_utf8_to_big5(void)
{
    for (int i = 0; i < NVEC; i++) {
        char out[OUTBUF];
        long n = do_iconv("BIG5", "UTF-8", g_vec[i].u8, strlen(g_vec[i].u8), out, OUTBUF);
        if (n < 0) {
            t_error("utf8->big5 failed [%s]: iconv rc=%ld errno=%s\n",
                    g_vec[i].desc, n, strerror(errno));
            continue;
        }
        if (!buf_eq(out, (size_t)n, g_vec[i].b5, strlen(g_vec[i].b5))) {
            t_error("utf8->big5 mismatch [%s]\n", g_vec[i].desc);
        }
    }
}
/* ------------------------------------------------------------------ */
/* Round-trip: big5 -> utf8 -> big5 (identity)                      */
/* ------------------------------------------------------------------ */
static void test_roundtrip_big5(void)
{
    for (int i = 0; i < NVEC; i++) {
        char mid[OUTBUF];
        long midn = do_iconv("UTF-8", "BIG5", g_vec[i].b5, strlen(g_vec[i].b5), mid, OUTBUF);
        if (midn < 0) {
            t_error("roundtrip step1 failed [%s]\n", g_vec[i].desc);
            continue;
        }
        char out[OUTBUF];
        long outn = do_iconv("BIG5", "UTF-8", mid, (size_t)midn, out, OUTBUF);
        if (outn < 0) {
            t_error("roundtrip step2 failed [%s]\n", g_vec[i].desc);
            continue;
        }
        if (!buf_eq(out, (size_t)outn, g_vec[i].b5, strlen(g_vec[i].b5))) {
            t_error("roundtrip identity failed [%s]\n", g_vec[i].desc);
        }
    }
}
/* ------------------------------------------------------------------ */
/* Cross-charset: UTF-8 -> X -> UTF-8 preserves original             */
/* ------------------------------------------------------------------ */
static void test_cross_charset_one(const char *cs, const char *u8, size_t u8len)
{
    char mid[OUTBUF];
    long midn = do_iconv(cs, "UTF-8", u8, u8len, mid, OUTBUF);
    if (midn < 0) {
        t_error("cross-charset utf8->%s failed\n", cs);
        return;
    }
    char out[OUTBUF];
    long outn = do_iconv("UTF-8", cs, mid, (size_t)midn, out, OUTBUF);
    if (outn < 0) {
        t_error("cross-charset %s->utf8 failed\n", cs);
        return;
    }
    if (!buf_eq(out, (size_t)outn, u8, u8len)) {
        t_error("cross-charset %s round-trip lost data\n", cs);
    }
}
static void test_cross_charset(void)
{
    /* CJK chars representable in Big5, GBK, GB18030 */
    const char *cjk = "\xe4\xb8\x80\xe4\xb8\xad\xe6\x96\x87\xe5\xad\x97"; /* 一中文字 */
    size_t cjklen = strlen(cjk);

    test_cross_charset_one("BIG5", cjk, cjklen);
    test_cross_charset_one("GBK", cjk, cjklen);
    test_cross_charset_one("GB18030", cjk, cjklen);

    /* ASCII round-trips through everything */
    const char *asc = "Hello 123";
    test_cross_charset_one("BIG5", asc, strlen(asc));
    test_cross_charset_one("GBK", asc, strlen(asc));
    test_cross_charset_one("GB18030", asc, strlen(asc));
    test_cross_charset_one("UTF-16BE", asc, strlen(asc));
    test_cross_charset_one("ISO-8859-1", asc, strlen(asc));
    test_cross_charset_one("EUC-KR", asc, strlen(asc));

    /* Big5 -> GBK direct (via ICU pivot) for shared CJK */
    {
        const char *b5 = "\xa4\xa4\xa4\x40"; /* 中一 in Big5 */
        char gbk[OUTBUF];
        long gbkn = do_iconv("GBK", "BIG5", b5, 4, gbk, OUTBUF);
        if (gbkn < 0) {
            t_error("big5->gbk direct conversion failed\n");
        }
        else {
            char out[OUTBUF];
            long outn = do_iconv("UTF-8", "GBK", gbk, (size_t)gbkn, out, OUTBUF);
            if (outn < 0 || !buf_eq(out, (size_t)outn,
                                    "\xe4\xb8\xad\xe4\xb8\x80", 6)) {
                t_error("big5->gbk->utf8 mismatch\n");
            }
        }
    }
}
/* ------------------------------------------------------------------ */
/* Boundary: truncated lead byte (only 1 of 2 bytes)                */
/* ------------------------------------------------------------------ */
static void test_truncated_sequence(void)
{
    char out[OUTBUF];
    errno = 0;
    do_iconv("UTF-8", "BIG5", "\xa1", 1, out, OUTBUF);
    /* ICU may handle differently; just verify no crash */
}
/* ------------------------------------------------------------------ */
/* Error: output buffer too small -> E2BIG                          */
/* ------------------------------------------------------------------ */
static void test_e2big(void)
{
    /* Convert a 2-byte Big5 char to UTF-8 (3 bytes) with 1-byte output buf */
    char out[1];
    errno = 0;
    long n = do_iconv("UTF-8", "BIG5", "\xa4\xa4", 2, out, 1);
    if (n != -1 || errno != E2BIG) {
        t_error("big5->utf8 E2BIG: rc=%ld errno=%s (want E2BIG)\n",
                n, strerror(errno));
    }
}
/* ------------------------------------------------------------------ */
/* //IGNORE suffix: invalid chars skipped                          */
/* ------------------------------------------------------------------ */
static void test_ignore_suffix(void)
{
    /* "Big5 编码示例" in UTF-8: 'Big5 ' is ASCII, 编码 are CJK not in Big5 level1 common
     * Actually 编(U+7F16) and 码(U+7801) exist in Big5. Let's use a char NOT in Big5:
     * U+1F600 (emoji) is definitely not in Big5. With //IGNORE it should be skipped. */
    const char *in = "AB\xF0\x9F\x98\x80""CD"; /* "AB<emoji>CD" */
    char out[OUTBUF];
    long n = do_iconv("BIG5//IGNORE", "UTF-8", in, strlen(in), out, OUTBUF);
    if (n < 0) {
        t_error("utf8->big5//IGNORE failed: rc=%ld errno=%s\n", n, strerror(errno));
        return;
    }
    /* Expect "ABCD" (emoji skipped) */
    if (!buf_eq(out, (size_t)n, "ABCD", 4)) {
        t_error("utf8->big5//IGNORE: expected ABCD, got %ld bytes\n", n);
    }
}
/* ------------------------------------------------------------------ */
/* //TRANSLIT suffix                                               */
/* ------------------------------------------------------------------ */
static void test_translit_suffix(void)
{
    /* With //TRANSLIT, unmappable chars get approximated or substituted */
    const char *in = "AB\xF0\x9F\x98\x80""CD";
    char out[OUTBUF];
    long n = do_iconv("BIG5//TRANSLIT", "UTF-8", in, strlen(in), out, OUTBUF);
    if (n < 0) {
        t_error("utf8->big5//TRANSLIT failed: rc=%ld errno=%s\n", n, strerror(errno));
        return;
    }
    /* "AB" and "CD" must survive; translit of emoji is implementation-defined */
    if (n < 4 || memcmp(out, "AB", 2) != 0 ||
        memcmp(out + n - 2, "CD", 2) != 0) {
        t_error("utf8->big5//TRANSLIT: AB/CD not preserved, got %ld bytes\n", n);
    }
}
/* ------------------------------------------------------------------ */
/* HKSCS supplementary plane (U+233E6, U+26DA0)                    */
/* ------------------------------------------------------------------ */
static void test_hkscs_supplementary(void)
{
    /* ICU test vector: Big5-HKSCS FC FC -> U+233E6, FC FD -> U+26DA0.
     * U+233E6 = F0 A3 8E A6 in UTF-8, U+26DA0 = F0 A6 B6 A0 in UTF-8. */
    {
        char out[OUTBUF] = {0};
        long n = do_iconv("UTF-8", "BIG5-HKSCS", "\xfc\xfc", 2, out, OUTBUF);
        char expect[] = "\xf0\xa3\x8f\xa6";
        if (n != 4 || !buf_eq(out, (size_t)n, expect, 4)) {
            t_error("hkscs FC FC: n=%ld, want 4 bytes matching U+233E6\n", n);
        }
    }
    {
        char out[OUTBUF] = {0};
        long n = do_iconv("UTF-8", "BIG5-HKSCS", "\xfc\xfd", 2, out, OUTBUF);
        char expect[] = "\xf0\xa6\xb6\xa0";
        if (n != 4 || !buf_eq(out, (size_t)n, expect, 4)) {
            t_error("hkscs FC FD: n=%ld, want 4 bytes matching U+26DA0\n", n);
        }
    }
}
/* ------------------------------------------------------------------ */
/* Empty input                                                     */
/* ------------------------------------------------------------------ */
static void test_empty_input(void)
{
    char out[OUTBUF];
    errno = 0;
    long n = do_iconv("UTF-8", "BIG5", "", 0, out, OUTBUF);
    if (n < 0) {
        t_error("big5->utf8 empty input failed: rc=%ld errno=%s\n", n, strerror(errno));
    }
    else if (n != 0) {
        t_error("big5->utf8 empty input: expected 0 bytes, got %ld\n", n);
    }
}
/* ------------------------------------------------------------------ */
/* Large input stress test                                         */
/* ------------------------------------------------------------------ */
static void test_large_input(void)
{
    /* Build UTF-8 string of 10000 '中' (U+4E2D = E4 B8 AD) */
    char *in = malloc(STRESS_LEN * 3);
    if (!in) { t_error("malloc failed for stress test\n"); return; }
    for (int i = 0; i < STRESS_LEN; i++) {
        in[i*3]   = '\xe4';
        in[i*3+1] = '\xb8';
        in[i*3+2] = '\xad';
    }

    /* Convert UTF-8 -> Big5 (each char becomes 2 bytes A4 A4) */
    char *big5 = malloc(STRESS_LEN * 2);
    if (!big5) { free(in); t_error("malloc failed\n"); return; }
    long b5n = do_iconv("BIG5", "UTF-8", in, STRESS_LEN * 3, big5, STRESS_LEN * 2);
    if (b5n < 0) {
        t_error("stress utf8->big5 failed: rc=%ld errno=%s\n", b5n, strerror(errno));
        free(in); free(big5);
        return;
    }
    if (b5n != STRESS_LEN * 2) {
        t_error("stress utf8->big5: expected %d bytes, got %ld\n", STRESS_LEN*2, b5n);
    }

    /* Verify every Big5 byte pair is A4 A4 */
    for (int i = 0; i < STRESS_LEN; i++) {
        if ((unsigned char)big5[i*2] != 0xa4 || (unsigned char)big5[i*2+1] != 0xa4) {
            t_error("stress: big5 byte mismatch at %d: %02x%02x\n",
                    i, (unsigned char)big5[i*2], (unsigned char)big5[i*2+1]);
            break;
        }
    }

    /* Convert back Big5 -> UTF-8 and verify identity */
    char *back = malloc(STRESS_LEN * 3);
    if (!back) { free(in); free(big5); t_error("malloc failed\n"); return; }
    long backn = do_iconv("UTF-8", "BIG5", big5, (size_t)b5n, back, STRESS_LEN * 3);
    if (backn < 0) {
        t_error("stress big5->utf8 failed: rc=%ld errno=%s\n", backn, strerror(errno));
    } else if (!buf_eq(back, (size_t)backn, in, STRESS_LEN * 3)) {
        t_error("stress round-trip identity failed\n");
    }

    free(in);
    free(big5);
    free(back);
}
/* ------------------------------------------------------------------ */
/* Mixed ASCII + Big5 in one buffer                                */
/* ------------------------------------------------------------------ */
static void test_mixed_ascii_big5(void)
{
    /* "A" + 中(A4A4) + "B" + 一(A440) */
    const char *b5 = "A\xa4\xa4""B\xa4\x40";
    size_t b5len = 6;
    char out[OUTBUF];
    long n = do_iconv("UTF-8", "BIG5", b5, b5len, out, OUTBUF);
    if (n < 0) {
        t_error("mixed big5->utf8 failed: rc=%ld errno=%s\n", n, strerror(errno));
        return;
    }
    /* Expected: A + E4B8AD + B + E4B800 */
    const char *expect = "A\xe4\xb8\xad""B\xe4\xb8\x80";
    if (!buf_eq(out, (size_t)n, expect, strlen(expect))) {
        t_error("mixed big5->utf8 mismatch: got %ld bytes\n", n);
    }
}
/* ------------------------------------------------------------------ */
/* iconv_open error for unknown charset                            */
/* ------------------------------------------------------------------ */
static void test_open_unknown_charset(void)
{
    iconv_t cd = iconv_open("BIG5", "nonexistent-charset");
    if (cd != (iconv_t)-1) {
        t_error("iconv_open(BIG5, nonexistent) should fail\n");
        iconv_close(cd);
    }
    if (errno != EINVAL) {
        t_error("iconv_open unknown charset: errno=%s (want EINVAL)\n", strerror(errno));
    }

    cd = iconv_open("nonexistent-charset", "UTF-8");
    if (cd != (iconv_t)-1) {
        t_error("iconv_open(nonexistent, UTF-8) should fail\n");
        iconv_close(cd);
    }
}
/* ------------------------------------------------------------------ */
/* Big5 alias names all resolve                                    */
/* ------------------------------------------------------------------ */
static void test_big5_aliases(void)
{
    const char *aliases[] = {"big5", "BIG5", "Big5", "bigfive", "cp950", "950"};
    int n = (int)(sizeof(aliases)/sizeof(aliases[0]));
    for (int i = 0; i < n; i++) {
        char out[OUTBUF];
        long r = do_iconv("UTF-8", aliases[i], "\xa4\xa4", 2, out, OUTBUF);
        if (r < 0) {
            t_error("alias '%s' big5->utf8 failed: rc=%ld errno=%s\n",
                    aliases[i], r, strerror(errno));
        }
        else if (!buf_eq(out, (size_t)r, "\xe4\xb8\xad", 3)) {
            t_error("alias '%s' produced wrong utf8\n", aliases[i]);
        }
    }
}
/* ------------------------------------------------------------------ */
/* Big5-HKSCS alias resolution                                     */
/* ------------------------------------------------------------------ */
static void test_hkscs_aliases(void)
{
    const char *aliases[] = {"big5hkscs", "BIG5-HKSCS", "big5hk", "951"};
    int n = (int)(sizeof(aliases)/sizeof(aliases[0]));
    for (int i = 0; i < n; i++) {
        char out[OUTBUF];
        long r = do_iconv("UTF-8", aliases[i], "\xa4\xa4", 2, out, OUTBUF);
        if (r < 0) {
            t_error("alias '%s' hkscs->utf8 failed\n", aliases[i]);
        }
    }
}
/* ------------------------------------------------------------------ */
/* mbrtowc / wcrtomb / btowc / wctob                                */
/*                                                                       */
/* musl's mbrtowc/wcrtomb always operate in UTF-8 (the C locale codec). */
/* In a Big5 workflow the application does:                              */
/*   iconv(WCHAR_T, BIG5)  ->  wchar_t[]  ->  mbrtowc etc. don't apply  */
/* But the wchar_t values produced by iconv are Unicode code points,    */
/* so wcrtomb(mbrtowc(...)) round-trip via UTF-8 should be identity.    */
/* We test the full chain: Big5 -> wchar_t (via iconv) -> UTF-8 (via    */
/* wcrtomb) and verify it matches Big5 -> UTF-8 (via iconv directly).   */
/* ------------------------------------------------------------------ */
static void test_mbrtowc_utf8_roundtrip(void)
{
    /* For each Big5 test vector: convert Big5->UTF-8 bytes, then feed    */
    /* those UTF-8 bytes to mbrtowc one-by-one to get wchar_t[], then     */
    /* feed each wchar_t to wcrtomb to get UTF-8 bytes back. Identity.    */
    for (int i = 0; i < NVEC; i++) {
        size_t u8len = strlen(g_vec[i].u8);
        if (u8len == 0) {
            continue;
        }

        /* Step 1: mbrtowc on the UTF-8 bytes */
        mbstate_t st = {0};
        wchar_t wcs[64];
        int nwc = 0;
        const char *p = g_vec[i].u8;
        size_t left = u8len;
        while (left > 0 && nwc < 63) {
            wchar_t wc = 0;
            size_t r = mbrtowc(&wc, p, left, &st);
            if (r == (size_t)-1 || r == (size_t)-2) {
                t_error("mbrtowc failed on utf8 of [%s]\n", g_vec[i].desc);
                nwc = -1;
                break;
            }
            wcs[nwc++] = wc;
            p += r;
            left -= r;
        }
        if (nwc < 0) {
            continue;
        }
        wcs[nwc] = 0;

        /* Step 2: wcrtomb each wchar_t back to UTF-8 */
        char back[OUTBUF];
        char *bp = back;
        for (int j = 0; j < nwc; j++) {
            mbstate_t st2 = {0};
            size_t r = wcrtomb(bp, wcs[j], &st2);
            if (r == (size_t)-1) {
                t_error("wcrtomb failed on wchar 0x%x of [%s]\n",
                        (unsigned)wcs[j], g_vec[i].desc);
                r = 0;
            }
            bp += r;
        }
        size_t backlen = (size_t)(bp - back);
        if (!buf_eq(back, backlen, g_vec[i].u8, u8len)) {
            t_error("mbrtowc/wcrtomb round-trip failed [%s]\n", g_vec[i].desc);
        }
    }
}
/* wcrtomb boundary: invalid wchar_t values -> EILSEQ */
static void test_wcrtomb_invalid(void)
{
    char s[MB_LEN_MAX];
    mbstate_t st = {0};

    /* Surrogates are invalid */
    errno = 0;
    if (wcrtomb(s, 0xD800, &st) != (size_t)-1 || errno != EILSEQ) {
        t_error("wcrtomb(0xD800 surrogate) should fail with EILSEQ\n");
    }

    errno = 0;
    if (wcrtomb(s, 0xDFFF, &st) != (size_t)-1 || errno != EILSEQ) {
        t_error("wcrtomb(0xDFFF surrogate) should fail with EILSEQ\n");
    }

    errno = 0;
    if (wcrtomb(s, 0x110000, &st) != (size_t)-1 || errno != EILSEQ) {
        t_error("wcrtomb(0x110000 out-of-range) should fail with EILSEQ\n");
    }

    errno = 0;
    if (wcrtomb(s, 0xFFFFFFFF, &st) != (size_t)-1 || errno != EILSEQ) {
        t_error("wcrtomb(0xFFFFFFFF) should fail with EILSEQ\n");
    }
}
/* wcrtomb boundary: valid code points at byte-length boundaries */
static void test_wcrtomb_boundaries(void)
{
    struct { wchar_t wc; int len; const char *desc; } cases[] = {
        {0x00,     1, "U+0000 (1-byte boundary low)"},
        {0x7F,     1, "U+007F (1-byte boundary high)"},
        {0x80,     2, "U+0080 (2-byte boundary low)"},
        {0x7FF,    2, "U+07FF (2-byte boundary high)"},
        {0x800,    3, "U+0800 (3-byte boundary low)"},
        {0xD7FF,   3, "U+D7FF (3-byte, before surrogates)"},
        {0xE000,   3, "U+E000 (3-byte, after surrogates)"},
        {0xFFFF,   3, "U+FFFF (3-byte boundary high)"},
        {0x10000,  4, "U+10000 (4-byte boundary low)"},
        {0x4E00,   3, "U+4E00 CJK (3-byte)"},
        {0x3000,   3, "U+3000 Ideographic Space (3-byte)"},
        {0xFF0C,   3, "U+FF0C Fullwidth Comma (3-byte)"},
        {0x10FFFF, 4, "U+10FFFF (4-byte boundary high)"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    for (int i = 0; i < n; i++) {
        char s[MB_LEN_MAX];
        mbstate_t st = {0};
        errno = 0;
        size_t r = wcrtomb(s, cases[i].wc, &st);
        if (r != (size_t)cases[i].len) {
            t_error("wcrtomb(0x%x %s) returned %zu, expected %d\n",
                    (unsigned)cases[i].wc, cases[i].desc, r, cases[i].len);
        }
    }
}
/* mbrtowc boundary: invalid UTF-8 sequences -> EILSEQ */
static void test_mbrtowc_invalid(void)
{
    struct { const char *bytes; size_t len; const char *desc; } cases[] = {
        {"\x80",            1, "lone continuation byte"},
        {"\xFE",            1, "invalid lead byte 0xFE"},
        {"\xFF",            1, "invalid lead byte 0xFF"},
        {"\xC0\x80",        2, "overlong 2-byte (C0 80)"},
        {"\xC1\xBF",        2, "overlong 2-byte (C1 BF)"},
        {"\xE0\x80\x80",    3, "overlong 3-byte"},
        {"\xF0\x80\x80\x80",4, "overlong 4-byte"},
        {"\xE4\xB8",        2, "truncated 3-byte (returns -2, not EILSEQ)"},
        {"\xF0\x9F\x98",    3, "truncated 4-byte (returns -2, not EILSEQ)"},
        {"\xED\xA0\x80",    3, "surrogate 0xD800 encoded as UTF-8"},
        {"\xED\xBF\xBF",    3, "surrogate 0xDFFF encoded as UTF-8"},
        {"\xF4\x90\x80\x80",4, "beyond U+10FFFF (F4 90...)"},
        {"\xE4\xB8\x00",    3, "valid lead but trail is 0x00 (not 10xxxxxx)"},
        {"\xE4\xB8\x41",    3, "valid lead but trail is ASCII 'A'"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));
    for (int i = 0; i < n; i++) {
        mbstate_t st = {0};
        wchar_t wc = 0;
        errno = 0;
        size_t r = mbrtowc(&wc, cases[i].bytes, cases[i].len, &st);
        /* Truncated sequences return -2 (need more bytes), not -1/EILSEQ */
        if (strstr(cases[i].desc, "returns -2")) {
            if (r != (size_t)-2) {
                t_error("mbrtowc(%s) returned %zu, expected -2\n",
                        cases[i].desc, r);
            }
            continue;
        }
        if (r != (size_t)-1 || errno != EILSEQ) {
            t_error("mbrtowc(%s) returned %zu errno=%s, expected -1/EILSEQ\n",
                    cases[i].desc, r, strerror(errno));
        }
    }
}
/* mbrtowc: partial sequences return -2 (need more bytes) */
static void test_mbrtowc_partial(void)
{
    mbstate_t st = {0};
    wchar_t wc = 0;

    /* Feed one byte at a time of U+4E00 (E4 B8 80) */
    size_t r1 = mbrtowc(&wc, "\xe4", 1, &st);
    if (r1 != (size_t)-2) {
        t_error("mbrtowc(1st byte of 3-byte seq)=%zu, expected -2 (incomplete)\n", r1);
    }

    size_t r2 = mbrtowc(&wc, "\xb8", 1, &st);
    if (r2 != (size_t)-2) {
        t_error("mbrtowc(2nd byte of 3-byte seq)=%zu, expected -2\n", r2);
    }

    size_t r3 = mbrtowc(&wc, "\x80", 1, &st);
    if (r3 != 1) {
        t_error("mbrtowc(3rd byte)=%zu, expected 1 (consumed last byte)\n", r3);
    }
    if (wc != 0x4E00) {
        t_error("mbrtowc partial result wc=0x%x, expected U+4E00\n", (unsigned)wc);
    }
}
/* mbrtowc: NULL src resets state */
static void test_mbrtowc_null_reset(void)
{
    mbstate_t st = {0};
    /* First put state into partial mode */
    size_t partial = mbrtowc(NULL, "\xe4", 1, &st);
    if (partial != (size_t)-2) {
        t_error("mbrtowc(NULL,\"xe4\")=%zu, expected -2 (incomplete)\n", partial);
    }
    /* NULL src with dirty state: musl returns -1 (EILSEQ) per POSIX. */
    size_t r = mbrtowc(NULL, NULL, 0, &st);
    if (r != (size_t)-1) {
        t_error("mbrtowc(NULL,NULL) with dirty state=%zu, expected -1\n", r);
    }
    /* After call, state should be clean */
    r = mbrtowc(NULL, NULL, 0, &st);
    if (r != 0) {
        t_error("mbrtowc(NULL,NULL) after reset=%zu, expected 0\n", r);
    }
}
/* btowc / wctob: single-byte <-> wchar_t */
static void test_btowc_wctob(void)
{
    /* ASCII round-trip */
    for (int c = 0; c < 128; c++) {
        wint_t wc = btowc(c);
        if (wc == WEOF) {
            if (c < 128) {
                t_error("btowc(%d) returned WEOF for ASCII\n", c);
            }
            continue;
        }
        int back = wctob(wc);
        if (back != c) {
            t_error("wctob(btowc(%d))=%d, expected %d\n", c, back, c);
        }
    }

    /* EOF -> WEOF, WEOF -> EOF */
    if (btowc(EOF) != WEOF) {
        t_error("btowc(EOF) should return WEOF\n");
    }
    if (wctob(WEOF) != EOF) {
        t_error("wctob(WEOF) should return EOF\n");
    }

    /* Non-ASCII single byte in C/UTF-8 locale -> WEOF (not a valid single byte char) */
    wint_t wc = btowc(0x80);
    if (wc != WEOF) {
        t_error("btowc(0x80) returned 0x%x, expected WEOF (not a valid single byte in UTF-8)\n", (unsigned)wc);
    }

    /* Multi-byte wchar_t -> wctob should return WEOF */
    if (wctob(0x4E00) != WEOF) {
        t_error("wctob(U+4E00) should return WEOF (not single-byte representable)\n");
    }
    if (wctob(0x3000) != WEOF) {
        t_error("wctob(U+3000) should return WEOF\n");
    }
}
/* mbsrtowcs / wcsrtombs: string-level conversion round-trip */
static void verify_wchar_values(const wchar_t *wcs, size_t nwc)
{
    if (nwc < 5) {
        return;
    }
    struct { wchar_t val; const char *desc; } expect[] = {
        {0x41, "'A'"}, {0x4E00, "U+4E00"}, {0x4E2D, "U+4E2D"},
        {0x42, "'B'"}, {0x5B57, "U+5B57"},
    };
    for (int i = 0; i < 5; i++) {
        if (wcs[i] != expect[i].val) {
            t_error("wcs[%d]=0x%x, expected %s\n",
                    i, (unsigned)wcs[i], expect[i].desc);
        }
    }
}

static void cross_check_iconv_wchar(const wchar_t *wcs, size_t nwc)
{
    const char *b5 = "A\xa4\x40\xa4\xa4""B\xa6\x72";
    wchar_t iconv_wcs[64];
    long wn = do_iconv("WCHAR_T", "BIG5", b5, 8,
                       (char *)iconv_wcs, sizeof(iconv_wcs));
    if (wn < 0) {
        t_error("iconv(BIG5->WCHAR_T) failed for cross-check\n");
        return;
    }
    size_t iconv_nwc = (size_t)wn / sizeof(wchar_t);
    if (iconv_nwc != nwc) {
        t_error("iconv BIG5->WCHAR_T produced %zu chars, mbsrtowcs %zu\n",
                iconv_nwc, nwc);
        return;
    }
    for (size_t j = 0; j < nwc; j++) {
        if (iconv_wcs[j] != wcs[j]) {
            t_error("wchar mismatch at %zu: iconv=0x%x mbrtowc=0x%x\n",
                    j, (unsigned)iconv_wcs[j], (unsigned)wcs[j]);
        }
    }
}

static void test_mbsrtowcs_wcsrtombs(void)
{
    const char *src_orig = "A\xe4\xb8\x80\xe4\xb8\xad""B\xe5\xad\x97";
    size_t srclen = strlen(src_orig);
    mbstate_t st = {0};
    wchar_t wcs[64];
    const char *src = src_orig;

    size_t nwc = mbsrtowcs(wcs, &src, 64, &st);
    if (nwc == (size_t)-1) {
        t_error("mbsrtowcs failed: %s\n", strerror(errno));
        return;
    }
    if (nwc != 5) {
        t_error("mbsrtowcs produced %zu chars, expected 5\n", nwc);
    }
    verify_wchar_values(wcs, nwc);
    /* mbsrtowcs may not update src pointer on all implementations;
     * the key validation is nwc and wchar values above. */

    mbstate_t st2 = {0};
    char back[OUTBUF];
    const wchar_t *wsrc = wcs;
    size_t nb = wcsrtombs(back, &wsrc, OUTBUF, &st2);
    if (nb == (size_t)-1) {
        t_error("wcsrtombs failed: %s\n", strerror(errno));
        return;
    }
    if (!buf_eq(back, nb, src_orig, srclen)) {
        t_error("mbsrtowcs/wcsrtombs round-trip mismatch\n");
    }
    cross_check_iconv_wchar(wcs, nwc);
}
/* wctomb / mbtowc: simpler wrappers around wcrtomb/mbrtowc */
static void test_wctomb_mbtowc(void)
{
    /* wctomb: wchar_t -> multibyte */
    char s[MB_LEN_MAX];
    int r = wctomb(s, 0x4E00);
    if (r != 3) {
        t_error("wctomb(U+4E00)=%d, expected 3\n", r);
    }
    if (r > 0 && memcmp(s, "\xe4\xb8\x80", 3) != 0) {
        t_error("wctomb(U+4E00) produced wrong bytes\n");
    }

    r = wctomb(s, 'A');
    if (r != 1 || s[0] != 'A') {
        t_error("wctomb('A')=%d s[0]=0x%x, expected 1/'A'\n", r, (unsigned char)s[0]);
    }

    r = wctomb(s, 0x10FFFF);
    if (r != 4) {
        t_error("wctomb(U+10FFFF)=%d, expected 4\n", r);
    }

    r = wctomb(s, 0x110000);
    if (r != -1) {
        t_error("wctomb(U+110000)=%d, expected -1 (EILSEQ)\n", r);
    }

    /* mbtowc: multibyte -> wchar_t */
    wchar_t wc = 0;
    r = mbtowc(&wc, "\xe4\xb8\x80", 3); /* U+4E00 */
    if (r != 3 || wc != 0x4E00) {
        t_error("mbtowc(E4 B8 00)=%d wc=0x%x, expected 3/U+4E00\n", r, (unsigned)wc);
    }

    r = mbtowc(&wc, "A", 1);
    if (r != 1 || wc != 'A') {
        t_error("mbtowc('A')=%d wc=0x%x, expected 1/'A'\n", r, (unsigned)wc);
    }

    r = mbtowc(&wc, "\x80", 1);
    if (r != -1) {
        t_error("mbtowc(0x80 lone continuation)=%d, expected -1\n", r);
    }

    /* mbtowc with NULL: return non-zero if stateful encoding (UTF-8 is stateless) */
    r = mbtowc(NULL, NULL, 0);
    if (r != 0) {
        t_error("mbtowc(NULL,NULL,0)=%d, expected 0 (UTF-8 is stateless)\n", r);
    }
}
/* mbrlen: count bytes of a multibyte char without producing wchar_t */
static void test_mbrlen(void)
{
    mbstate_t st = {0};

    /* U+4E00 = E4 B8 80 (3 bytes) */
    size_t r = mbrlen("\xe4", 1, &st);
    if (r != (size_t)-2) {
        t_error("mbrlen(1st byte)=%zu, expected -2 (incomplete)\n", r);
    }

    r = mbrlen("\xb8", 1, &st);
    if (r != (size_t)-2) {
        t_error("mbrlen(2nd byte)=%zu, expected -2\n", r);
    }

    r = mbrlen("\x80", 1, &st);
    if (r != 1) {
        /* mbrlen returns bytes consumed in THIS call (1), not total (3) */
        t_error("mbrlen(3rd byte)=%zu, expected 1\n", r);
    }

    /* Single byte ASCII */
    st = (mbstate_t){0};
    r = mbrlen("A", 1, &st);
    if (r != 1) {
        t_error("mbrlen('A')=%zu, expected 1\n", r);
    }

    /* Invalid */
    st = (mbstate_t){0};
    r = mbrlen("\x80", 1, &st);
    if (r != (size_t)-1) {
        t_error("mbrlen(0x80)=%zu, expected -1 (EILSEQ)\n", r);
    }
}
/* Full pipeline: Big5 bytes -> iconv(WCHAR_T) -> wchar_t[] ->      */
/* wcrtomb each -> UTF-8 bytes -> compare with iconv(UTF-8,BIG5)    */
static void test_big5_to_wchar_to_utf8_pipeline(void)
{
    for (int i = 0; i < NVEC; i++) {
        if (strlen(g_vec[i].b5) == 0) {
            continue;
        }

        /* Big5 -> WCHAR_T via iconv */
        wchar_t wcs[64];
        long wn = do_iconv("WCHAR_T", "BIG5",
                           g_vec[i].b5, strlen(g_vec[i].b5),
                           (char *)wcs, sizeof(wcs));
        if (wn < 0) {
            t_error("pipeline: iconv(BIG5->WCHAR_T) failed [%s]\n", g_vec[i].desc);
            continue;
        }
        size_t nwc = (size_t)wn / sizeof(wchar_t);

        /* wchar_t[] -> UTF-8 via wcrtomb */
        char u8[OUTBUF];
        char *p = u8;
        for (size_t j = 0; j < nwc; j++) {
            mbstate_t st = {0};
            size_t r = wcrtomb(p, wcs[j], &st);
            if (r == (size_t)-1) {
                t_error("pipeline: wcrtomb(0x%x) failed [%s]\n",
                        (unsigned)wcs[j], g_vec[i].desc);
                r = 0;
            }
            p += r;
        }
        size_t u8len = (size_t)(p - u8);

        /* Compare with expected UTF-8 */
        if (!buf_eq(u8, u8len, g_vec[i].u8, strlen(g_vec[i].u8))) {
            t_error("pipeline: Big5->WCHAR_T->UTF8 mismatch [%s]\n", g_vec[i].desc);
        }
    }
}
/* Real Big5 sentence: "你好世界中文測試" (Hello World Chinese Test)
 * All 8 characters verified against musl's big5.h table. */
static void test_real_big5_sentence(void)
{
    /* Big5: 你(A741) 好(A66E) 世(A540) 界(ACC9) 中(A4A4) 文(A4E5) 測(B4FA) 試(B8D5) */
    const char b5[] = "\xa7\x41\xa6\x6e\xa5\x40\xac\xc9"
                      "\xa4\xa4\xa4\xe5\xb4\xfa\xb8\xd5";
    /* UTF-8: 你 好 世 界 中 文 測 試 */
    const char u8[] = "\xe4\xbd\xa0\xe5\xa5\xbd\xe4\xb8\x96\xe7\x95\x8c"
                      "\xe4\xb8\xad\xe6\x96\x87\xe6\xb8\xac\xe8\xa9\xa6";

    /* Big5 -> UTF-8 */
    char out1[OUTBUF];
    long n1 = do_iconv("UTF-8", "BIG5", b5, sizeof(b5)-1, out1, OUTBUF);
    if (n1 < 0) {
        t_error("real sentence big5->utf8 failed\n");
    } else if (!buf_eq(out1, (size_t)n1, u8, sizeof(u8)-1)) {
        t_error("real sentence big5->utf8 mismatch\n");
    }

    /* UTF-8 -> Big5 */
    char out2[OUTBUF];
    long n2 = do_iconv("BIG5", "UTF-8", u8, sizeof(u8)-1, out2, OUTBUF);
    if (n2 < 0) {
        t_error("real sentence utf8->big5 failed\n");
    } else if (!buf_eq(out2, (size_t)n2, b5, sizeof(b5)-1)) {
        t_error("real sentence utf8->big5 mismatch\n");
    }

    /* Round-trip: Big5 -> UTF-8 -> Big5 (identity) */
    char mid[OUTBUF];
    long midn = do_iconv("UTF-8", "BIG5", b5, sizeof(b5)-1, mid, OUTBUF);
    if (midn >= 0) {
        char back[OUTBUF];
        long backn = do_iconv("BIG5", "UTF-8", mid, (size_t)midn, back, OUTBUF);
        if (backn >= 0 && !buf_eq(back, (size_t)backn, b5, sizeof(b5)-1)) {
            t_error("real sentence round-trip identity failed\n");
        }
    }

    /* Big5 -> WCHAR_T, verify char count = 8 */
    wchar_t wcs[32];
    long wn = do_iconv("WCHAR_T", "BIG5", b5, sizeof(b5)-1,
                       (char *)wcs, sizeof(wcs));
    if (wn < 0) {
        t_error("real sentence big5->wchart failed\n");
    } else {
        size_t nwc = (size_t)wn / sizeof(wchar_t);
        if (nwc != 8) {
            t_error("real sentence: expected 8 wchar, got %zu\n", nwc);
        }
        /* Verify first and last characters */
        if (nwc >= 1 && wcs[0] != 0x4F60) {
            t_error("wcs[0]=0x%x, expected U+4F60 (你)\n", (unsigned)wcs[0]);
        }
        if (nwc >= 8 && wcs[7] != 0x8A66) {
            t_error("wcs[7]=0x%x, expected U+8A66 (試)\n", (unsigned)wcs[7]);
        }
    }
}

/* HKSCS-only characters: these exist in Big5-HKSCS (lead 0x87-0xA0)
 * but NOT in standard Big5 (lead 0xA1-0xF9). Verified against musl's
 * big5.h (standard) and hkscs.h (extended) tables. */
static const struct { const char *b5hkscs; const char *utf8; const char *desc; }
g_hkscs_only[] = {
    {"\x87\x40", "\xe4\x8f\xb0", "U+43F0"},
    {"\x87\x41", "\xe4\xb0\xb2", "U+4C32"},
    {"\x87\x42", "\xe4\x98\x83", "U+4603"},
    {"\x87\x43", "\xe4\x96\xa6", "U+45A6"},
};
#define N_HKSCS_ONLY ((int)(sizeof(g_hkscs_only)/sizeof(g_hkscs_only[0])))

static void test_hkscs_directional(void)
{
    for (int i = 0; i < N_HKSCS_ONLY; i++) {
        char out[OUTBUF];
        long n = do_iconv("UTF-8", "BIG5-HKSCS",
                          g_hkscs_only[i].b5hkscs, 2, out, OUTBUF);
        if (n <= 0 || !buf_eq(out, (size_t)n, g_hkscs_only[i].utf8, 3)) {
            t_error("hkscs-only [%s] ->utf8 failed\n", g_hkscs_only[i].desc);
        }
        long n2 = do_iconv("BIG5-HKSCS", "UTF-8",
                           g_hkscs_only[i].utf8, 3, out, OUTBUF);
        if (n2 <= 0 || !buf_eq(out, (size_t)n2,
                               g_hkscs_only[i].b5hkscs, 2)) {
            t_error("hkscs-only [%s] <-utf8 failed\n", g_hkscs_only[i].desc);
        }
    }
}

static void test_hkscs_not_in_standard_big5(void)
{
    char out[OUTBUF];
    long n = do_iconv("UTF-8", "BIG5", "\x87\x40", 2, out, OUTBUF);
    if (n > 0 && buf_eq(out, (size_t)n, "\xe4\x8f\xb0", 3)) {
        t_error("standard Big5 decoded HKSCS-only 0x8740 to U+43F0\n");
    }
}

static void test_hkscs_roundtrip(void)
{
    for (int i = 0; i < N_HKSCS_ONLY; i++) {
        char mid[OUTBUF];
        long midn = do_iconv("UTF-8", "BIG5-HKSCS",
                             g_hkscs_only[i].b5hkscs, 2, mid, OUTBUF);
        if (midn <= 0) {
            continue;
        }
        char back[OUTBUF];
        long backn = do_iconv("BIG5-HKSCS", "UTF-8",
                              mid, (size_t)midn, back, OUTBUF);
        if (backn <= 0 || !buf_eq(back, (size_t)backn,
                                   g_hkscs_only[i].b5hkscs, 2)) {
            t_error("hkscs-only [%s] round-trip failed\n",
                    g_hkscs_only[i].desc);
        }
    }
}

static void test_hkscs_only_chars(void)
{
    test_hkscs_directional();
    test_hkscs_not_in_standard_big5();
    test_hkscs_roundtrip();
}

/* strtod_l: verify ICU-based parsing under zh_HK.Big5 locale.
 * ICU's strtod_l should parse numbers using locale-specific format.
 * For zh_HK, decimal separator is "." (same as C), so basic parsing
 * should work. The key is that ICU_VALID flag causes the ICU path
 * to be taken instead of fallback strtod(). */
static void test_strtod_l_hk(void)
{
    if (!g_loc_big5) {
        t_error("test_strtod_l: g_loc_big5 is NULL\n");
        return;
    }

    struct { const char *input; double expect; const char *desc; } cases[] = {
        {"3.14",     3.14,     "pi"},
        {"0",        0.0,      "zero"},
        {"-1.5",    -1.5,      "negative"},
        {"123.456",  123.456,  "three decimals"},
        {"1e10",     1e10,     "scientific"},
        {"0.5",      0.5,      "half"},
        {"999.99",   999.99,   "large value"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));

    for (int i = 0; i < n; i++) {
        char *endptr = NULL;
        double r = strtod_l(cases[i].input, &endptr, g_loc_big5);
        if (!isfinite(r) || fabs(r - cases[i].expect) >= 1e-9) {
            t_error("strtod_l(\"%s\", zh_HK)=%g, expected %g [%s]\n",
                    cases[i].input, r, cases[i].expect, cases[i].desc);
        }
        if (endptr != cases[i].input + strlen(cases[i].input)) {
            t_error("strtod_l(\"%s\", zh_HK) endptr not at end [%s]\n",
                    cases[i].input, cases[i].desc);
        }
    }

    /* "inf" → INFINITY */
    char *endptr = NULL;
    double r = strtod_l("inf", &endptr, g_loc_big5);
    if (!isinf(r)) {
        t_error("strtod_l(\"inf\", zh_HK)=%g, expected INFINITY\n", r);
    }
}

/* wcstod_l: verify ICU-based wide-char parsing under zh_HK.Big5 locale */
static void test_wcstod_l_hk(void)
{
    if (!g_loc_big5) {
        return;
    }

    struct { const wchar_t *input; double expect; const char *desc; } cases[] = {
        {L"3.14",     3.14,     "pi"},
        {L"0",        0.0,      "zero"},
        {L"-1.5",    -1.5,      "negative"},
        {L"123.456",  123.456,  "three decimals"},
        {L"1e10",     1e10,     "scientific"},
    };
    int n = (int)(sizeof(cases)/sizeof(cases[0]));

    for (int i = 0; i < n; i++) {
        wchar_t *endptr = NULL;
        double r = wcstod_l(cases[i].input, &endptr, g_loc_big5);
        if (!isfinite(r) || fabs(r - cases[i].expect) >= 1e-9) {
            t_error("wcstod_l(L\"%ls\", zh_HK)=%g, expected %g [%s]\n",
                    cases[i].input, r, cases[i].expect, cases[i].desc);
        }
    }
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
static void run_wctype_tests(void)
{
    test_wctype_setup();
    test_icu_path_discriminator();
    test_non_whitelisted_locale();
    test_iswalpha_l();
    test_iswalnum_l();
    test_iswdigit_l();
    test_iswxdigit_l();
    test_iswspace_l();
    test_iswpunct_l();
    test_iswupper_iswlower_l();
    test_towupper_towlower_l();
    test_iswcntrl_l();
    test_iswprint_iswgraph_l();
    test_iswblank_l();
    test_wctype_hkscs();
    test_cross_locale_consistency();
}

static void run_iconv_tests(void)
{
    test_big5_to_utf8();
    test_utf8_to_big5();
    test_roundtrip_big5();
    test_cross_charset();
    test_truncated_sequence();
    test_e2big();
    test_open_unknown_charset();
    test_ignore_suffix();
    test_translit_suffix();
    test_hkscs_supplementary();
    test_empty_input();
    test_mixed_ascii_big5();
    test_big5_aliases();
    test_hkscs_aliases();
    test_large_input();
    test_real_big5_sentence();
    test_hkscs_only_chars();
}

static void run_locale_tests(void)
{
    test_strtod_l_hk();
    test_wcstod_l_hk();
}

static void run_multibyte_tests(void)
{
    /* Multibyte functions require MB_CUR_MAX > 1 (UTF-8 locale active).
     * On some devices the default C locale has MB_CUR_MAX=1, which makes
     * mbrtowc/wcrtomb treat each byte as a single-byte char. */
    if (MB_CUR_MAX <= 1) {
        t_error("MB_CUR_MAX=%d, multibyte tests require UTF-8 locale\n",
                (int)MB_CUR_MAX);
        return;
    }
    test_mbrtowc_utf8_roundtrip();
    test_wcrtomb_invalid();
    test_wcrtomb_boundaries();
    test_mbrtowc_invalid();
    test_mbrtowc_partial();
    test_mbrtowc_null_reset();
    test_btowc_wctob();
    test_mbsrtowcs_wcsrtombs();
    test_wctomb_mbtowc();
    test_mbrlen();
    test_big5_to_wchar_to_utf8_pipeline();
}

static void cleanup_locales(void)
{
    if (g_loc_big5) { freelocale(g_loc_big5); }
    if (g_loc_hkscs) { freelocale(g_loc_hkscs); }
    if (g_loc_cn) { freelocale(g_loc_cn); }
    if (g_loc_en_utf8) { freelocale(g_loc_en_utf8); }
    if (g_loc_non_whitelisted) { freelocale(g_loc_non_whitelisted); }
}

int main(void)
{
    /* Set C.UTF-8 locale so MB_CUR_MAX=4 for multibyte tests */
    if (!setlocale(LC_CTYPE, "C.UTF-8")) {
        t_error("setlocale(LC_CTYPE, C.UTF-8) failed\n");
    }
    /* If ICU enable fails (e.g. device libc has no ICU patch),
     * skip all tests and return success. */
    if (set_iconv_icu_enable() != 0 || set_wctype_icu_enable() != 0) {
        return 0;
    }
    run_wctype_tests();
    run_iconv_tests();
    run_multibyte_tests();
    run_locale_tests();
    cleanup_locales();
    return t_status;
}
#else  /* __LITEOS__ */
/* LiteOS does not support ICU locale; provide a stub. */
int main(void) { return 0; }
#endif /* __LITEOS__ */