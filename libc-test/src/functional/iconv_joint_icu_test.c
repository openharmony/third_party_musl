/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 *
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
#ifndef __LITEOS__
#include "test.h"
#include <errno.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define IGNORE_SIZE 9

typedef struct StatefulCombined {
    unsigned sign;
    const unsigned char* to;
    const unsigned char* from;
    iconv_t base_cd;
    unsigned state;
} StatefulCombined;

typedef struct NameMap {
    const char* source;
    const char* target;
} NameMap;


// Global Variables
static NameMap g_mappings[] = {
    {"utf8\0char\0\0", "UTF-8"},
    {"utf7\0\0", "UTF-7"},
    {"ucs2\0utf16\0ucs2be\0utf16be\0\0", "UTF-16BE"},
    {"ucs2le\0utf16le\0\0", "UTF-16LE"},
    {"ucs4\0utf32\0ucs4be\0utf32be\0\0", "UTF-32BE"},
    {"wchart\0ucs4le\0utf32le\0\0", "UTF-32LE"},
    {"ascii\0usascii\0""20127\0iso646\0iso646us\0\0", "US-ASCII"},
    {"eucjp\0eucjp2007\0\0", "euc-jp-2007"},
    {"shiftjis\0sjis\0cp932\0ibm943p15a2003\0\0", "ibm-943_P15A-2003"},
    {"gb18030\0\0", "gb18030"},
    {"gbk\0""54936\0windows9362000\0\0", "windows-936-2000"},
    {"gb2312\0""52936\0ibm1383p1101999\0\0", "ibm-1383_P110-1999"},
    {"big5\0""950\0bigfive\0cp950\0windows9502000\0\0", "windows-950-2000"},
    {"big5hk\0big5hkscs\0""951\0ibm1375p1002008\0\0", "ibm-1375_P100-2008"},
    {"euckr\0ibm970p110p1102006u2\0\0", "ibm-970_P110_P110-2006_U2"},
    {"ksc5601\0ksx1001\0cp949\0windows9492000\0\0", "windows-949-2000"},
    {"iso88591\0latin1\0\0", "ISO-8859-1"},
    {"iso88592\0ibm912p1001995\0\0", "ibm-912_P100-1995"},
    {"iso88593\0ibm913p1002000\0\0", "ibm-913_P100-2000"},
    {"iso88594\0ibm914p1001995\0\0", "ibm-914_P100-1995"},
    {"iso88595\0ibm915p1001995\0\0", "ibm-915_P100-1995"},
    {"iso88596\0ibm1089p1001995\0\0", "ibm-1089_P100-1995"},
    {"iso88597\0ibm9005x1102007\0\0", "ibm-9005_X110-2007"},
    {"iso88598\0ibm5012p1001999\0\0", "ibm-5012_P100-1999"},
    {"iso88599\0ibm920p1001995\0\0", "ibm-920_P100-1995"},
    {"iso885910\0iso8859101998\0\0", "iso-8859_10-1998"},
    {"iso885911\0iso8859112001\0\0", "iso-8859_11-2001"},
    {"tis620\0windows8742000\0\0", "windows-874-2000"},
    {"iso885913\0ibm921p1001995\0\0", "ibm-921_P100-1995"},
    {"iso885914\0iso8859141998\0\0", "iso-8859_14-1998"},
    {"iso885915\0latin9\0ibm923p1001998\0\0", "ibm-923_P100-1998"},
    {"cp1250\0windows1250\0ibm5346p1001998\0\0", "ibm-5346_P100-1998"},
    {"cp1251\0windows1251\0ibm5347p1001998\0\0", "ibm-5347_P100-1998"},
    {"cp1252\0windows1252\0ibm5348p1001997\0\0", "ibm-5348_P100-1997"},
    {"cp1253\0windows1253\0ibm5349p1001998\0\0", "ibm-5349_P100-1998"},
    {"cp1254\0windows1254\0ibm5350p1001998\0\0", "ibm-5350_P100-1998"},
    {"cp1255\0windows1255\0ibm9447p1002002\0\0", "ibm-9447_P100-2002"},
    {"cp1256\0windows1256\0ibm9448x1002005\0\0", "ibm-9448_X100-2005"},
    {"cp1257\0windows1257\0ibm9449p1002002\0\0", "ibm-9449_P100-2002"},
    {"cp1258\0windows1258\0ibm5354p1001998\0\0", "ibm-5354_P100-1998"},
    {"koi8r\0ibm878p1001996\0\0", "ibm-878_P100-1996"},
    {"koi8u\0ibm1168p1002002\0\0", "ibm-1168_P100-2002"},
    {"cp437\0ibm437p1001995\0\0", "ibm-437_P100-1995"},
    {"cp850\0ibm850p1001995\0\0", "ibm-850_P100-1995"},
    {"cp866\0ibm866p1001995\0\0", "ibm-866_P100-1995"},
    {"ibm1047\0cp1047\0ibm1047p1001995\0\0", "ibm-1047_P100-1995"},
};
char* g_source_str;
char* g_target_str;
char g_mids[BUFFER_SIZE];
char g_outs[BUFFER_SIZE];
int32_t g_target_num = sizeof(g_mappings) / sizeof(g_mappings[0]);
char g_ins[] = {
    0x00, 0x20, // ' ' (space)
    0x00, 0x21, // '!'
    0x00, 0x22, // '"'
    0x00, 0x23, // '#'
    0x00, 0x24, // '$'
    0x00, 0x25, // '%'
    0x00, 0x26, // '&'
    0x00, 0x27, // '''
    0x00, 0x28, // '('
    0x00, 0x29, // ')'
    0x00, 0x2A, // '*'
    0x00, 0x2B, // '+'
    0x00, 0x2C, // ','
    0x00, 0x2D, // '-'
    0x00, 0x2E, // '.'
    0x00, 0x2F, // '/'
    0x00, 0x30, // '0'
    0x00, 0x31, // '1'
    0x00, 0x32, // '2'
    0x00, 0x33, // '3'
    0x00, 0x34, // '4'
    0x00, 0x35, // '5'
    0x00, 0x36, // '6'
    0x00, 0x37, // '7'
    0x00, 0x38, // '8'
    0x00, 0x39, // '9'
    0x00, 0x3A, // ':'
    0x00, 0x3B, // ';'
    0x00, 0x3C, // '<'
    0x00, 0x3D, // '='
    0x00, 0x3E, // '>'
    0x00, 0x3F, // '?'
    0x00, 0x40, // '@'
    0x00, 0x41, // 'A'
    0x00, 0x42, // 'B'
    0x00, 0x43, // 'C'
    0x00, 0x44, // 'D'
    0x00, 0x45, // 'E'
    0x00, 0x46, // 'F'
    0x00, 0x47, // 'G'
    0x00, 0x48, // 'H'
    0x00, 0x49, // 'I'
    0x00, 0x4A, // 'J'
    0x00, 0x4B, // 'K'
    0x00, 0x4C, // 'L'
    0x00, 0x4D, // 'M'
    0x00, 0x4E, // 'N'
    0x00, 0x4F, // 'O'
    0x00, 0x50, // 'P'
    0x00, 0x51, // 'Q'
    0x00, 0x52, // 'R'
    0x00, 0x53, // 'S'
    0x00, 0x54, // 'T'
    0x00, 0x55, // 'U'
    0x00, 0x56, // 'V'
    0x00, 0x57, // 'W'
    0x00, 0x58, // 'X'
    0x00, 0x59, // 'Y'
    0x00, 0x5A, // 'Z'
    0x00, 0x5B, // '['
    0x00, 0x5C, // '\\'
    0x00, 0x5D, // ']'
    0x00, 0x5E, // '^'
    0x00, 0x5F, // '_'
    0x00, 0x60, // '`'
    0x00, 0x61, // 'a'
    0x00, 0x62, // 'b'
    0x00, 0x63, // 'c'
    0x00, 0x64, // 'd'
    0x00, 0x65, // 'e'
    0x00, 0x66, // 'f'
    0x00, 0x67, // 'g'
    0x00, 0x68, // 'h'
    0x00, 0x69, // 'i'
    0x00, 0x6A, // 'j'
    0x00, 0x6B, // 'k'
    0x00, 0x6C, // 'l'
    0x00, 0x6D, // 'm'
    0x00, 0x6E, // 'n'
    0x00, 0x6F, // 'o'
    0x00, 0x70, // 'p'
    0x00, 0x71, // 'q'
    0x00, 0x72, // 'r'
    0x00, 0x73, // 's'
    0x00, 0x74, // 't'
    0x00, 0x75, // 'u'
    0x00, 0x76, // 'v'
    0x00, 0x77, // 'w'
    0x00, 0x78, // 'x'
    0x00, 0x79, // 'y'
    0x00, 0x7A, // 'z'
    0x00, 0x7B, // '{'
    0x00, 0x7C, // '|'
    0x00, 0x7D, // '}'
    0x00, 0x7E  // '~'
};
size_t g_ins_len = sizeof(g_ins);
char* g_ins_zh = "Big5 编码示例";
size_t g_ins_zh_len;

void iconv_close_with_strerror(iconv_t cd)
{
    if (iconv_close(cd)) {
        t_error("closed iconv failed, error: %s \n", strerror(errno));
    }
}

void alias_test(void)
{
    char* s;
    StatefulCombined *scd = 0;
    for (s = g_source_str; *s;) {
        scd = (void *)iconv_open(s, s);
        if (scd == (iconv_t)-1) {
            t_error("iconv opened failed, from: %s, to: %s, error: %s \n", s, s, strerror(errno));
            return;
        }
        if (strcmp((void*)scd->to, g_target_str) != 0) {
            t_error("verify error: [input]%s, [actual_to]%s  [expect_to]%s\n", s, scd->to, g_target_str);
        }
        if (strcmp((void*)scd->from, g_target_str) != 0) {
            t_error("verify error: [input]%s, [actual_from]%s  [expect_from]%s\n", s, scd->from, g_target_str);
        }
        s += strlen(s) + 1;
        iconv_close_with_strerror(scd);
    }
}

size_t iconv_test(char* from, char* to,
    char** inptrptr, size_t* input_len_ptr, char** outptrptr, size_t* output_len_ptr)
{
    iconv_t cd = iconv_open((void*)to, (void*)from);
    if (cd == (iconv_t)-1) {
        t_error("iconv opened failed, from: %s, to: %s, error: %s \n", from, to, strerror(errno));
        return (size_t)-1;
    }

    size_t res = iconv(cd, inptrptr, input_len_ptr, outptrptr, output_len_ptr);
    if (res) {
        iconv_close_with_strerror(cd);
        return res;
    }

    iconv_close_with_strerror(cd);
    return res;
}

void iconv_exchange(char* from, char* to, char* ins, size_t ins_len)
{
    char* ins_ptr = ins;
    char* mids_ptr = g_mids;
    char* outs_ptr = g_outs;
    size_t ins_bytes = ins_len;
    size_t mids_bytes = BUFFER_SIZE;
    size_t outs_bytes = BUFFER_SIZE;

    iconv_test(from, to, &ins_ptr, &ins_bytes, &mids_ptr, &mids_bytes);

    ins_ptr = g_mids;
    ins_bytes = BUFFER_SIZE - mids_bytes;
    iconv_test(to, from, &ins_ptr, &ins_bytes, &outs_ptr, &outs_bytes);
}

static int charcmp(char* a, char* b)
{
    for (; *a && *b; a++, b++) {
        if (*a != *b) {return 1;}
    }
    return *a != *b;
}

void iconv_exchange_with_charcmp(char* from, char* to, char* ins, size_t ins_len)
{
    iconv_exchange(from, to, ins, ins_len);
    if (charcmp(ins, g_outs) != 0) {
        t_error("compare error [type]%s, [ins] %s, [outs] %s\n",
            to, ins, g_outs);
    }
}

void test_to_ignore_skip(void)
{
    char* expect = "Big5 示例";
    iconv_exchange("utf8", "big5//IGNORE", g_ins_zh, g_ins_zh_len);
    if (strcmp(expect, g_outs) != 0) {
        t_error("iconv with ignore error: [ins] %s  [outs] %s  [expect] %s\n",
            g_ins_zh, g_outs, expect);
    }
}

void test_to_translit_skip(void)
{
    char* expect = "Big5 ??示例";
    iconv_exchange("utf8", "big5//TRANSLIT", g_ins_zh, g_ins_zh_len);
    if (strcmp(expect, g_outs) != 0) {
        t_error("iconv with translit error: [ins] %s  [outs] %s  [expect] %s\n",
            g_ins_zh, g_outs, expect);
    }
}

void test_errno_ilseq(void)
{
    char* ins_ptr = g_ins_zh;
    size_t ins_bytes = g_ins_zh_len;
    char* outs_ptr = g_outs;
    size_t outs_bytes = BUFFER_SIZE;
    size_t res = iconv_test("utf8", "big5", &ins_ptr, &ins_bytes, &outs_ptr, &outs_bytes);
    if (res != (size_t)-1 || errno != EILSEQ) {
        t_error("Failed: [ins] %s  [outs] %s  [res] %d [errno] %s\n",
            g_ins_zh, g_outs, res, strerror(errno));
    }
}

void test_errno_e2big(void)
{
    char* ins_ptr = g_ins_zh;
    size_t ins_bytes = g_ins_zh_len;
    char* outs_ptr = g_outs;
    size_t outs_bytes = 10;
    size_t res = iconv_test("utf8", "utf16", &ins_ptr, &ins_bytes, &outs_ptr, &outs_bytes);
    if (res != (size_t)-1 || errno != E2BIG) {
        t_error("Failed: [ins] %s  [outs] %s  [res] %d [errno] %s\n",
            g_ins_zh, g_outs, res, strerror(errno));
    }
}

void test_empty_type(void)
{
    StatefulCombined *scd = 0;
    scd = (void *)iconv_open("", "");
    if (strcmp((void*)scd->to, "UTF-8") != 0 || strcmp((void*)scd->from, "UTF-8") != 0) {
        t_error("verify error: empty type not default fill utf8");
    }
    iconv_close_with_strerror(scd);
}

int main(void)
{
    g_ins_zh_len = strlen(g_ins_zh);

    for (int i = 0; i < g_target_num; i++) {
        g_target_str = (void*)g_mappings[i].target;
        g_source_str = (void*)g_mappings[i].source;

        // test all avail alias with iconv_open
        alias_test();

        // test iconv without ignore
        iconv_exchange_with_charcmp("utf16", g_target_str, g_ins, g_ins_len);

        // test iconv with ignore (norm input->not skip)
        char* target_str_with_ignore = malloc(strlen(g_target_str) + IGNORE_SIZE);
        strcpy(target_str_with_ignore, g_target_str);
        strcat(target_str_with_ignore, "//IGNORE");
        iconv_exchange_with_charcmp("utf16", target_str_with_ignore, g_ins, g_ins_len);
        iconv_exchange_with_charcmp("utf16//IGNORE", target_str_with_ignore, g_ins, g_ins_len);
        free(target_str_with_ignore);
    }

    // test iconv with ignore (special input -> skip)
    test_to_ignore_skip();

    // test iconv with translit (special input -> translit)
    test_to_translit_skip();

    // test basic type (iso885916)
    iconv_exchange_with_charcmp("utf16", "iso885916", g_ins, g_ins_len);

    // test errno
    test_errno_ilseq();
    test_errno_e2big();

    // empty type -> fill UTF-8
    test_empty_type();

    return t_status;
}
#endif