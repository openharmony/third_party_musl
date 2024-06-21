/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
#include "test.h"
#include <errno.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 200

static int g_encodeNum = 76;
static char *g_encodeArray[] = {
    "utf8",        "wchart",   "ucs2be",      "ucs2le",    "utf16be",     "utf16le",   "ucs4be",      "utf32be",
    "ucs4le",      "utf32le",  "ascii",       "usascii",   "iso646",      "iso646us",  "utf16",       "ucs4",
    "utf32",       "ucs2",     "eucjp",       "shiftjis",  "sjis",        "iso2022jp", "gb18030",     "gbk",
    "gb2312",      "big5",     "bigfive",     "cp950",     "big5hkscs",   "euckr",     "ksc5601",     "ksx1001",
    "cp949",       "iso88591", "latin1",      "iso88592",  "iso88593",    "iso88594",  "iso88595",    "iso88596",
    "iso88597",    "iso88598", "iso88599",    "iso885910", "iso885911",   "tis620",    "iso885913",   "iso885914",
    "iso885915",   "latin9",   "iso885916",   "cp1250",    "windows1250", "cp1251",    "windows1251", "cp1252",
    "windows1252", "cp1253",   "windows1253", "cp1254",    "windows1254", "cp1255",    "windows1255", "cp1256",
    "windows1256", "cp1257",   "windows1257", "cp1258",    "windows1258", "koi8r",     "koi8u",       "cp437",
    "cp850",       "cp866",    "cp1047",      "ibm1047"};
static int g_excludeToEncodeNum = 11;
static char *g_excludeToEncodeArray[] = {"gb18030",   "gbk",   "gb2312",  "big5",    "bigfive", "cp950",
                                         "big5hkscs", "euckr", "ksc5601", "ksx1001", "cp949"};

void CloseIconv(iconv_t cd)
{
    if (iconv_close(cd)) {
        printf("closed iconv failed, error: %s \n", strerror(errno));
    }
}

int IconvTest(char *to, char *from, char *input, size_t inputLen, char *output, size_t *outputLen)
{
    iconv_t cd = iconv_open(to, from);
    if (cd == (iconv_t)-1) {
        t_error("iconv opened failed, from: %s, to: %s, error: %s \n", from, to, strerror(errno));
        CloseIconv(cd);
        return 1;
    }
    if (iconv(cd, &input, &inputLen, &output, outputLen)) {
        t_error("iconv converted failed, from: %s, to: %s, error: %s \n", from, to, strerror(errno));
        CloseIconv(cd);
        return 1;
    }
    CloseIconv(cd);
    return 0;
}

int IsExcludeToEncode(char *ecode)
{
    for (int i = 0; i < g_excludeToEncodeNum; i++) {
        if (strcmp(ecode, g_excludeToEncodeArray[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

void CompareIconvResult(char *to, char *from, char *src, size_t srcLen)
{
    if (IsExcludeToEncode(to) == 0) {
        return;
    }
    char *input = src;
    size_t inputLen = srcLen;
    char output[BUFFER_SIZE];
    size_t outputLen = BUFFER_SIZE;
    if (IconvTest(to, from, input, inputLen, output, &outputLen)) {
        return;
    }

    if (IsExcludeToEncode(from) == 0) {
        return;
    }
    char *newInput = output;
    size_t newInputLen = BUFFER_SIZE - outputLen;
    char newOutput[BUFFER_SIZE];
    size_t newOutputLen = BUFFER_SIZE;
    if (IconvTest(from, to, newInput, newInputLen, newOutput, &newOutputLen)) {
        return;
    }

    if (srcLen != BUFFER_SIZE - newOutputLen) {
        t_error("compare error,from: %s, to: %s, newOutput: %s,\n", from, to, newOutput);
        return;
    }
    char *l = src;
    char *r = newOutput;
    size_t step = sizeof(char);
    for (size_t i = 0; i < srcLen; i += step) {
        if (*l != *r) {
            t_error("compare error,from: %s, to: %s, newOutput: %s,\n", from, to, newOutput);
            return;
        }
        l++;
        r++;
    }
}

int main(void)
{
    char *str = "Hello world";
    for (int i = 0; i < g_encodeNum; i++) {
        char *from = g_encodeArray[i];
        char *input = str;
        size_t inputLen = strlen(input);

        if (IsExcludeToEncode(from) != 0) {
            char output[BUFFER_SIZE];
            size_t outputLen = BUFFER_SIZE;
            if (IconvTest(from, "utf8", input, inputLen, output, &outputLen)) {
                continue;
            }
            input = output;
            inputLen = BUFFER_SIZE - outputLen;
        }

        for (int j = 0; j < g_encodeNum; j++) {
            char *to = g_encodeArray[j];
            CompareIconvResult(to, from, input, inputLen);
        }
    }

    return t_status;
}