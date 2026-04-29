/*
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

#include <stdlib.h>
#include "functionalext.h"
#include "memory_trace.h"

/**
 * @tc.name      : restrace_0100
 * @tc.desc      : Verify that memory allocation is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void restrace_0100(void)
{
    char* mem = (char *)malloc(1 * sizeof(char));
    restrace(RES_GPU_VK, mem, 1, TAG_RES_GPU_VK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_0200
 * @tc.desc      : Verify that memory allocation is successful with RES_GPU_GLES_IMAGE
 * @tc.level     : Level 0
 */
void restrace_0200(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    restrace(RES_GPU_GLES_IMAGE, mem, 10, TAG_RES_GPU_GLES_IMAGE, true);
    free(mem);
}

/**
 * @tc.name      : restrace_0300
 * @tc.desc      : Verify that memory allocation is successful with RES_GPU_GLES_BUFFER
 * @tc.level     : Level 0
 */
void restrace_0300(void)
{
    char* mem = (char *)malloc(20 * sizeof(char));
    restrace(RES_GPU_GLES_BUFFER, mem, 20, TAG_RES_GPU_GLES_BUFFER, false);
    free(mem);
}

/**
 * @tc.name      : restrace_0400
 * @tc.desc      : Verify that memory allocation is successful with RES_GPU_CL_IMAGE
 * @tc.level     : Level 0
 */
void restrace_0400(void)
{
    char* mem = (char *)malloc(50 * sizeof(char));
    restrace(RES_GPU_CL_IMAGE, mem, 50, TAG_RES_GPU_CL_IMAGE, true);
    free(mem);
}

/**
 * @tc.name      : restrace_0500
 * @tc.desc      : Verify that memory allocation is successful with RES_GPU_CL_BUFFER
 * @tc.level     : Level 0
 */
void restrace_0500(void)
{
    char* mem = (char *)malloc(100 * sizeof(char));
    restrace(RES_GPU_CL_BUFFER, mem, 100, TAG_RES_GPU_CL_BUFFER, false);
    free(mem);
}

/**
 * @tc.name      : restrace_0600
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_OPEN
 * @tc.level     : Level 0
 */
void restrace_0600(void)
{
    char* mem = (char *)malloc(32 * sizeof(char));
    restrace(RES_FD_OPEN, mem, 32, TAG_RES_FD_OPEN, true);
    free(mem);
}

/**
 * @tc.name      : restrace_0700
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_EPOLL
 * @tc.level     : Level 0
 */
void restrace_0700(void)
{
    char* mem = (char *)malloc(64 * sizeof(char));
    restrace(RES_FD_EPOLL, mem, 64, TAG_RES_FD_EPOLL, false);
    free(mem);
}

/**
 * @tc.name      : restrace_0800
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_EVENTFD
 * @tc.level     : Level 0
 */
void restrace_0800(void)
{
    char* mem = (char *)malloc(128 * sizeof(char));
    restrace(RES_FD_EVENTFD, mem, 128, TAG_RES_FD_EVENTFD, true);
    free(mem);
}

/**
 * @tc.name      : restrace_0900
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_SOCKET
 * @tc.level     : Level 0
 */
void restrace_0900(void)
{
    char* mem = (char *)malloc(256 * sizeof(char));
    restrace(RES_FD_SOCKET, mem, 256, TAG_RES_FD_SOCKET, false);
    free(mem);
}

/**
 * @tc.name      : restrace_1000
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_PIPE
 * @tc.level     : Level 0
 */
void restrace_1000(void)
{
    char* mem = (char *)malloc(512 * sizeof(char));
    restrace(RES_FD_PIPE, mem, 512, TAG_RES_FD_PIPE, true);
    free(mem);
}

/**
 * @tc.name      : restrace_1100
 * @tc.desc      : Verify that memory allocation is successful with RES_FD_DUP
 * @tc.level     : Level 0
 */
void restrace_1100(void)
{
    char* mem = (char *)malloc(16 * sizeof(char));
    restrace(RES_FD_DUP, mem, 16, TAG_RES_FD_DUP, false);
    free(mem);
}

/**
 * @tc.name      : restrace_1200
 * @tc.desc      : Verify that memory allocation is successful with RES_THREAD_PTHREAD
 * @tc.level     : Level 0
 */
void restrace_1200(void)
{
    char* mem = (char *)malloc(8 * sizeof(char));
    restrace(RES_THREAD_PTHREAD, mem, 8, TAG_RES_THREAD_PTHREAD, true);
    free(mem);
}

/**
 * @tc.name      : restrace_1300
 * @tc.desc      : Verify that memory allocation is successful with RES_ARKTS_HEAP_MASK
 * @tc.level     : Level 0
 */
void restrace_1300(void)
{
    char* mem = (char *)malloc(64 * sizeof(char));
    restrace(RES_ARKTS_HEAP_MASK, mem, 64, TAG_RES_ARKTS_HEAP_MASK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_1400
 * @tc.desc      : Verify that memory allocation is successful with RES_JS_HEAP_MASK
 * @tc.level     : Level 0
 */
void restrace_1400(void)
{
    char* mem = (char *)malloc(32 * sizeof(char));
    restrace(RES_JS_HEAP_MASK, mem, 32, TAG_RES_JS_HEAP_MASK, false);
    free(mem);
}

/**
 * @tc.name      : restrace_1500
 * @tc.desc      : Verify that memory allocation is successful with RES_KMP_HEAP_MASK
 * @tc.level     : Level 0
 */
void restrace_1500(void)
{
    char* mem = (char *)malloc(128 * sizeof(char));
    restrace(RES_KMP_HEAP_MASK, mem, 128, TAG_RES_KMP_HEAP_MASK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_1600
 * @tc.desc      : Verify that memory allocation is successful with RES_SO_MASK
 * @tc.level     : Level 0
 */
void restrace_1600(void)
{
    char* mem = (char *)malloc(256 * sizeof(char));
    restrace(RES_SO_MASK, mem, 256, TAG_RES_SO_MASK, false);
    free(mem);
}

/**
 * @tc.name      : restrace_1700
 * @tc.desc      : Verify that memory allocation is successful with RES_RN_HEAP_MASK
 * @tc.level     : Level 0
 */
void restrace_1700(void)
{
    char* mem = (char *)malloc(96 * sizeof(char));
    restrace(RES_RN_HEAP_MASK, mem, 96, TAG_RES_RN_HEAP_MASK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_1800
 * @tc.desc      : Verify that memory allocation is successful with RES_DMABUF_MASK
 * @tc.level     : Level 0
 */
void restrace_1800(void)
{
    char* mem = (char *)malloc(192 * sizeof(char));
    restrace(RES_DMABUF_MASK, mem, 192, TAG_RES_DMABUF_MASK, false);
    free(mem);
}

/**
 * @tc.name      : restrace_1900
 * @tc.desc      : Verify that memory allocation is successful with RES_ASHMEM_MASK
 * @tc.level     : Level 0
 */
void restrace_1900(void)
{
    char* mem = (char *)malloc(48 * sizeof(char));
    restrace(RES_ASHMEM_MASK, mem, 48, TAG_RES_ASHMEM_MASK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_2000
 * @tc.desc      : Verify that memory allocation is successful with RES_ARK_GLOBAL_HANDLE
 * @tc.level     : Level 0
 */
void restrace_2000(void)
{
    char* mem = (char *)malloc(24 * sizeof(char));
    restrace(RES_ARK_GLOBAL_HANDLE, mem, 24, TAG_RES_ARK_GLOBAL_HANDLE, false);
    free(mem);
}

/**
 * @tc.name      : restrace_2100
 * @tc.desc      : Verify that memory allocation is successful with RES_ARK_LOCAL_HANDLE
 * @tc.level     : Level 0
 */
void restrace_2100(void)
{
    char* mem = (char *)malloc(12 * sizeof(char));
    restrace(RES_ARK_LOCAL_HANDLE, mem, 12, TAG_RES_ARK_LOCAL_HANDLE, true);
    free(mem);
}

/**
 * @tc.name      : restrace_2200
 * @tc.desc      : Verify that memory allocation with addr is NULL
 * @tc.level     : Level 2
 */
void restrace_2200(void)
{
    restrace(RES_GPU_VK, NULL, 10, TAG_RES_GPU_VK, true);
}

/**
 * @tc.name      : restrace_2300
 * @tc.desc      : Verify that memory allocation with size is 0
 * @tc.level     : Level 2
 */
void restrace_2300(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    restrace(RES_GPU_VK, mem, 0, TAG_RES_GPU_VK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_2400
 * @tc.desc      : Verify that memory allocation with large size
 * @tc.level     : Level 1
 */
void restrace_2400(void)
{
    size_t largeSize = 1024 * 1024;
    char* mem = (char *)malloc(largeSize);
    restrace(RES_GPU_VK, mem, largeSize, TAG_RES_GPU_VK, true);
    free(mem);
}

/**
 * @tc.name      : restrace_2500
 * @tc.desc      : Verify that memory allocation with tag is NULL
 * @tc.level     : Level 2
 */
void restrace_2500(void)
{
    char* mem = (char *)malloc(64 * sizeof(char));
    restrace(RES_GPU_VK, mem, 64, NULL, true);
    free(mem);
}

int main(void)
{
    restrace_0100();
    restrace_0200();
    restrace_0300();
    restrace_0400();
    restrace_0500();
    restrace_0600();
    restrace_0700();
    restrace_0800();
    restrace_0900();
    restrace_1000();
    restrace_1100();
    restrace_1200();
    restrace_1300();
    restrace_1400();
    restrace_1500();
    restrace_1600();
    restrace_1700();
    restrace_1800();
    restrace_1900();
    restrace_2000();
    restrace_2100();
    restrace_2200();
    restrace_2300();
    restrace_2400();
    restrace_2500();
    return t_status;
}