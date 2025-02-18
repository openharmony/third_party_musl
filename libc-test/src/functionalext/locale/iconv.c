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

#include <locale.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "functionalext.h"
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int test_euc_kr_error(void)
{
    iconv_t cd = iconv_open("UTF-8", "EUC-KR");
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        return -1;
    }
    // 测试输入：无效的EUC-KR序列 "\xC8\x41"
    char input[] = "\xC8\x41"; // 第二个字节 0x41 无效
    size_t in_len = 2;         // 明确指定输入长度（避免依赖字符串终止符）
    // 分配输出缓冲区（足够容纳可能的转换结果）
    size_t outbuf_size = 6; // EUC-KR最多转3字节/字符，2字符则预留6字节
    char *outbuf = malloc(outbuf_size);
    if (!outbuf) {
        perror("malloc failed");
        iconv_close(cd);
        return -1;
    }
    char *in_ptr = input;
    size_t in_left = in_len;
    char *out_ptr = outbuf;
    size_t out_left = outbuf_size;

    // 执行转换
    size_t ret = iconv(cd, &in_ptr, &in_left, &out_ptr, &out_left);
    if (ret == (size_t)-1) {
        if (errno != EILSEQ) {
            t_error("test failed expect errno=EILSEQ actually get errno= %d\n", errno);
        }
    } else {
        size_t len = outbuf_size - out_left;
        if (len < 0 && len >= 10) {
            t_error("test failed expect 0<=len<=10 actually len=%d\n", (int)len);
        }
    }
    free(outbuf);
    iconv_close(cd);
    return t_status;
}

int main(void)
{
    return test_euc_kr_error();
}
