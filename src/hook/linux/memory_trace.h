/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef _MEMORY_TRACE_H
#define _MEMORY_TRACE_H

#include <stdbool.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TAG_RES_FD_OPEN            "RES_FD_OPEN"
#define TAG_RES_FD_EPOLL           "RES_FD_EPOLL"
#define TAG_RES_FD_EVENTFD         "RES_FD_EVENTFD"
#define TAG_RES_FD_SOCKET          "RES_FD_SOCKET"
#define TAG_RES_FD_PIPE            "RES_FD_PIPE"
#define TAG_RES_FD_DUP             "RES_FD_DUP"
#define TAG_RES_FD_ALL             "RES_FD_ALL"
#define TAG_RES_THREAD_PTHREAD     "RES_THREAD_PTHREAD"
#define TAG_RES_GPU_VK             "RES_GPU_VK"
#define TAG_RES_GPU_GLES_IMAGE     "RES_GPU_GLES_IMAGE"
#define TAG_RES_GPU_GLES_BUFFER    "RES_GPU_GLES_BUFFER"
#define TAG_RES_GPU_CL_IMAGE       "RES_GPU_CL_IMAGE"
#define TAG_RES_GPU_CL_BUFFER      "RES_GPU_CL_BUFFER"
#define TAG_RES_GPU_ALL            "RES_GPU_ALL"
#define TAG_RES_DMABUF             "RES_DMABUF"

/* each bit represents resource hook point.
 * |63 ... 32|31 ... 22|21 ... 12|11 - 10|9 ... 0|
 * |RESERVED |  DMABUF |   GPU   |THREAD |   FD  |
 */

//FD
#define RES_FD_OPEN                (1 << 0)
#define RES_FD_EPOLL               (1 << 1)
#define RES_FD_EVENTFD             (1 << 2)
#define RES_FD_SOCKET              (1 << 3)
#define RES_FD_PIPE                (1 << 4)
#define RES_FD_DUP                 (1 << 5)
#define RES_FD_MASK                (0x3F)
//Thread
#define RES_THREAD_PTHREAD         (1 << 10)
#define RES_THREAD_MASK            (0x3 << 10)
//GPU Memory
#define RES_GPU_VK                 (1 << 12)
#define RES_GPU_GLES_IMAGE         (1 << 13)
#define RES_GPU_GLES_BUFFER        (1 << 14)
#define RES_GPU_CL_IMAGE           (1 << 15)
#define RES_GPU_CL_BUFFER          (1 << 16)
#define RES_GPU_MASK               (0x1F << 12)
//ION Memory
#define RES_ION_MASK               (0x1F << 22)
//RESERVED
#define RES_RESERVED_MASK          (0xFFFFFF << 32)

void memtrace(void* addr, size_t size, const char* tag, bool is_using);
void restrace(unsigned long long mask, void* addr, size_t size, const char* tag, bool is_using);
#ifdef __cplusplus
}
#endif
#endif
