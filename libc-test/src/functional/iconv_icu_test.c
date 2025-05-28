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

#include "test.h"
#include <iconv.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "functionalext.h"

#define SIZE 256    /* enough room for conversion */
#define PARAM_2 2
#define PARAM_3 3
#define PARAM_4 4
#define SAMPLESTR "abc"
#define BUFSIZE 10000

struct unalign {
    char str1[1];
    char str2[SIZE];
};

struct convcode {
    const char *tocode;
    const char *fromcode;
};

/* test builtin transformation */
static const struct convcode testcode[] = {
    {"ASCII", "ASCII"},
    {"UTF-8", "ASCII"},
    {"UCS-2BE", "ASCII"},
    {"UCS-2LE", "ASCII"},
    {"UCS-4BE", "ASCII"},
    {"UCS-4LE", "ASCII"},
};

static const int number = (int)sizeof(testcode) / sizeof(struct convcode);

static int convert(const char *tocode, const char *fromcode, char *inbufp,
                    size_t inbytesleft, char *outbufp, size_t outbytesleft)
{
    iconv_t *ic;
    size_t outbytes = outbytesleft;
    int ret;

    ic = iconv_open(tocode, fromcode);
    if (ic == (iconv_t *)-1) {
        t_error("iconv_open failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
        return -1;
    }

    while (inbytesleft > 0) {
        ret = iconv(ic, &inbufp, &inbytesleft, &outbufp, &outbytes);
        if (ret == -1) {
            t_error("iconv failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
            iconv_close (ic);
            return -1;
        }
    }

    ret = iconv_close(ic);
    if (ret == -1) {
        t_error("iconv_close failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
        return -1;
    }

    return outbytesleft - outbytes;
}

static int test_unalign(const struct convcode *codes, const char *str, int len)
{
    struct unalign *inbufp, *outbufp;
    char *inbuf, *outbuf;
    size_t inbytesleft, outbytesleft;
    int retlen;

    /* allocating unaligned buffer for both inbuf and outbuf */
    inbufp = (struct unalign *)malloc(sizeof(struct unalign));
    if (!inbufp) {
        t_error("no memory available\n");
        return -1;
    }
    inbuf = inbufp->str2;

    outbufp = (struct unalign *)malloc(sizeof(struct unalign));
    if (!outbufp) {
        t_error("no memory available\n");
        return -1;
    }
    outbuf = outbufp->str2;

    /* first iconv phase */
    memcpy(inbuf, str, len);
    inbytesleft = len;
    outbytesleft = sizeof(struct unalign);
    retlen = convert(codes->tocode, codes->fromcode, inbuf, inbytesleft,
                    outbuf, outbytesleft);
    if (retlen == -1) {
        return 1;
    }
    
    /* second round trip iconv phase */
    memcpy(inbuf, outbuf, retlen);
    inbytesleft = retlen;
    outbytesleft = sizeof(struct unalign);
    retlen = convert(codes->fromcode, codes->tocode, inbuf, inbytesleft,
                    outbuf, outbytesleft);
    if (retlen == -1) {
        return 1;
    }
    
    free(inbufp);
    free(outbufp);

    return 0;
}

/**
 * @tc.name         : iconv_0100
 * @tc.desc         : Test the interface function iconv.
 * @tc.level        : Level 0
 */

void iconv_0100(void)
{
    char utf8[5];
    wchar_t ucs4[5];
    iconv_t cd;
    char *inbuf;
    char *outbuf;
    size_t inbytes;
    size_t outbytes;
    size_t n;

    strncpy(utf8, "abcd", sizeof(utf8));

    /* From UTF8 to UCS4. */
    cd = iconv_open("UCS4", "UTF8");
    if (cd == (iconv_t)-1) {
        t_error("iconv_open failed");
        return;
    }

    inbuf = utf8;
    inbytes = PARAM_4;
    outbuf = (char *)ucs4;
    outbytes = PARAM_4 * sizeof(wchar_t);
    n = iconv(cd, &inbuf, &inbytes, &outbuf, &outbytes);
    if (n == (size_t)-1) {
        t_error("iconv failed\n");
        iconv_close(cd);
        return;
    }
    iconv_close(cd);
}

/**
 * @tc.name     : iconv_0200
 * @tc.desc     : Test the interface function iconv.
 * @tc.level    : Level 0
 */

void iconv_0200(void)
{
    char buf[3];
    const wchar_t wc[1] = L"a";
    iconv_t cd;
    char *inptr;
    size_t inlen;
    char *outptr;
    size_t outlen;
    size_t n;
    int e;
    int result = 0;

    cd = iconv_open("UCS4", "wchart");
    if (cd == (iconv_t)-1) {
        t_error("iconv_open failed");
        return;
    }

    inptr = (char *)wc;
    inlen = sizeof(wchar_t);
    outptr = buf;
    outlen = PARAM_3;

    n = iconv(cd, &inptr, &inlen, &outptr, &outlen);
    e = errno;

    if (n != (size_t)-1) {
        t_error("incorrect iconv() return value: %zd, expected -1\n", n);
        return;
    }

    if (e != E2BIG) {
        t_error("incorrect error value: %s, expected %s\n", strerror(e), strerror(E2BIG));
        return;
    }

    if (inptr != (char *)wc) {
        t_error("inptr changed ,%p , %p \n", inptr, wc);
        return;
    }

    if (inlen != sizeof(wchar_t)) {
        t_error("inlen changed \n");
        return;
    }

    if (outptr != buf) {
        t_error("outptr changed \n");
        return;
    }

    if (outlen != PARAM_3) {
        t_error("outlen changed \n");
        return;
    }

    iconv_close(cd);
    return;
}

/**
 * @tc.name         : iconv_0300
 * @tc.desc         : Test the interface function iconv.
 * @tc.level        : Level 0
 */

void iconv_0300(void)
{
    char inbuf[BUFSIZE];
    wchar_t outbuf[BUFSIZE];

    iconv_t cd;
    int i;
    char *inptr;
    char *outptr;
    size_t inbytes_left, outbytes_left;
    int count;
    int result = 0;

    for (i = 0; i < BUFSIZE; i++) {
        inbuf[i] = 'a';
    }

    cd = iconv_open("UCS-4LE", "UTF-8");

    inbytes_left = BUFSIZE;
    outbytes_left = BUFSIZE * PARAM_4;
    inptr = inbuf;
    outptr = (char *)outbuf;

    count = iconv(cd, &inptr, &inbytes_left, &outptr, &outbytes_left);
    if (count < 0) {
        if (errno == E2BIG) {
            t_error("Received E2BIG \n");
        } else {
            t_error("Received something else \n");
        }

        t_error("inptr change: %td \n", inptr - inbuf);
        t_error("inlen change: %zd \n", BUFSIZE - inbytes_left);
        t_error("outptr change: %td \n", outptr - (char *)outbuf);
        t_error("outlen change: %zd \n", BUFSIZE * PARAM_4 - outbytes_left);
    }

    iconv_close(cd);
}

/**
 * @tc.name     : iconv_0400
 * @tc.desc     : Test the interface function iconv with TRANSLIT.
 * @tc.level    : Level 0
 */

void iconv_0400(void)
{
    iconv_t cd = iconv_open("ASCII//TRANSLIT", "UTF-8");
    if (cd == (iconv_t)-1) {
        t_error("iconv_open failed");
        return;
    }

    char input[4] = { 0xc2, 0xae, 0xc2, 0xae};  /*  Registered trademark */
    char *inptr = input;
    size_t insize = sizeof(input);
    char output[1];     /*  Too short to contain "??".  */
    char *outptr = output;
    size_t outsize = sizeof(output);
    size_t ret = iconv(cd, &inptr, &insize, &outptr, &outsize);
    if (ret != (size_t)-1) {
        t_error("iconv succeeded \n");
        return;
    }

    if (errno != E2BIG) {
        t_error("iconv did not set errno to E2BIG errno: %d \n", errno);
        return;
    }

    if (inptr != input) {
        t_error("inptr changed \n");
        return;
    }

    if (insize != sizeof(input)) {
        t_error("insize changed \n");
        return;
    }

    if (outptr != output) {
        t_error("outptr changed \n");
        return;
    }

    if (outsize != sizeof(output)) {
        t_error("outsize changed \n");
        return;
    }

    if (iconv_close(cd) == -1) {
        t_error("iconv_close failed \n");
        return;
    }
}

/**
 * @tc.name         : iconv_0500
 * @tc.desc         : Test the interface function iconv.
 * @tc.level        : Level 0
 */

void iconv_0500(void)
{
    int i;
    int ret = 0;

    for (i = 0; i < number; i++) {
        ret = test_unalign(&testcode[i], (char *)SAMPLESTR, sizeof(SAMPLESTR));
        if (ret) {
            break;
        }
        printf("iconv: %s <-> %s: ok \n", testcode[i].fromcode, testcode[i].tocode);
    }
    if (ret == 0) {
        printf("Succeeded. \n");
    }
}

/**
 * @tc.name         : iconv_0600
 * @tc.desc         : Test the interface function iconv.
 * @tc.level        : Level 0
 */

void iconv_0600(void)
{
    iconv_t cd;
    char *inptr;
    char *outptr;
    size_t inlen, outlen, n;
    int e;
    int result = 0;
#if __BYTE_ORDER == __BIG_ENDIAN
    /*  On big-endian machines, ucs4le_internal_loop() swaps the bytes before
        error checking. Thus the input values has to be swapped. */
# define VALUE(val) bswap_32 (val)
#else
# define VALUE(val) val
#endif
    uint32_t inbuf[3] = { VALUE(0x41), VALUE(0x80000000), VALUE(0x42)};
    uint32_t outbuf[3] = { 0, 0, 0 };
    cd = iconv_open("WCHAR_T", "UCS-4LE");
    if (cd == (iconv_t)-1) {
        t_error("cannot convert from UCS4LE to wchar_t");
        return;
    }

    inptr = (char *)inbuf;
    inlen = sizeof(inbuf);
    outptr = (char *)outbuf;
    outlen = sizeof(outbuf);

    n = iconv(cd, &inptr, &inlen, &outptr, &outlen);
    e = errno;

    if (n != (size_t)-1) {
        t_error("incorrect iconv() return value: %zd, expected -1 \n", n);
    }

    if (e != EILSEQ) {
        t_error("incorrect error value: %s, expected %s \n", strerror(e), strerror(EILSEQ));
    }

    if (inptr != (char *)&inbuf[1]) {
        t_error("inptr=0x%p does not point to invalid character! Expected=0x%p \n", inptr, &inbuf[1]);
    }

    if (inlen != sizeof(inbuf) - sizeof(uint32_t)) {
        t_error("inlen=%zd != %zd\n", inlen, sizeof(inbuf) - sizeof(uint32_t));
    }

    if (outptr != (char *)&outbuf[1]) {
        t_error("outptr=0x%p does not point to invalid character in inbuf! Expected=0x%p \n", outptr, &outbuf[1]);
    }

    if (outlen != sizeof(inbuf) - sizeof(uint32_t)) {
        t_error("outlen=%zd != %zd\n", outlen, sizeof(outbuf) - sizeof(uint32_t));
    }

    if (outbuf[0] != 0x41 || outbuf[1] != 0 || outbuf[PARAM_2] != 0) {
        t_error("Characters conversion is incorrect!");
    }

    iconv_close(cd);

    return;
}

int main(void)
{
    iconv_0100();
    iconv_0200();
    iconv_0300();
    iconv_0500();
    iconv_0600();
    int icu_res = set_iconv_icu_enable();
    if (icu_res != 0) {
        t_error("set_iconv_icu_enable failed, error: %d \n", icu_res);
        return t_status;
    }
    iconv_0100();
    iconv_0200();
    iconv_0300();
    iconv_0400();
    iconv_0500();
    return t_status;
}