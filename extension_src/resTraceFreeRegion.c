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
 * @tc.name      : resTraceFreeRegion_0100
 * @tc.desc      : Verify that memory free region is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0100(void)
{
    char* mem = (char *)malloc(1 * sizeof(char));
    resTraceFreeRegion(RES_ARKTS_HEAP_MASK, mem, 1);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0200
 * @tc.desc      : Verify that memory free region is successful with RES_JS_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0200(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    resTraceFreeRegion(RES_JS_HEAP_MASK, mem, 10);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0300
 * @tc.desc      : Verify that memory free region is successful with RES_KMP_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0300(void)
{
    char* mem = (char *)malloc(100 * sizeof(char));
    resTraceFreeRegion(RES_KMP_HEAP_MASK, mem, 100);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0400
 * @tc.desc      : Verify that memory free region is successful with RES_SO_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0400(void)
{
    char* mem = (char *)malloc(50 * sizeof(char));
    resTraceFreeRegion(RES_SO_MASK, mem, 50);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0500
 * @tc.desc      : Verify that memory free region is successful with RES_RN_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0500(void)
{
    char* mem = (char *)malloc(32 * sizeof(char));
    resTraceFreeRegion(RES_RN_HEAP_MASK, mem, 32);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0600
 * @tc.desc      : Verify that memory free region is successful with RES_DMABUF_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0600(void)
{
    char* mem = (char *)malloc(128 * sizeof(char));
    resTraceFreeRegion(RES_DMABUF_MASK, mem, 128);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0700
 * @tc.desc      : Verify that memory free region is successful with RES_ASHMEM_MASK
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0700(void)
{
    char* mem = (char *)malloc(64 * sizeof(char));
    resTraceFreeRegion(RES_ASHMEM_MASK, mem, 64);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0800
 * @tc.desc      : Verify that memory free region is successful with RES_ARK_GLOBAL_HANDLE
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0800(void)
{
    char* mem = (char *)malloc(16 * sizeof(char));
    resTraceFreeRegion(RES_ARK_GLOBAL_HANDLE, mem, 16);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_0900
 * @tc.desc      : Verify that memory free region is successful with RES_ARK_LOCAL_HANDLE
 * @tc.level     : Level 0
 */
void resTraceFreeRegion_0900(void)
{
    char* mem = (char *)malloc(8 * sizeof(char));
    resTraceFreeRegion(RES_ARK_LOCAL_HANDLE, mem, 8);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_1000
 * @tc.desc      : Verify that memory free region with addr is NULL
 * @tc.level     : Level 2
 */
void resTraceFreeRegion_1000(void)
{
    resTraceFreeRegion(RES_ARKTS_HEAP_MASK, NULL, 10);
}

/**
 * @tc.name      : resTraceFreeRegion_1100
 * @tc.desc      : Verify that memory free region with size is 0
 * @tc.level     : Level 2
 */
void resTraceFreeRegion_1100(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    resTraceFreeRegion(RES_ARKTS_HEAP_MASK, mem, 0);
    free(mem);
}

/**
 * @tc.name      : resTraceFreeRegion_1200
 * @tc.desc      : Verify that memory free region with large size
 * @tc.level     : Level 1
 */
void resTraceFreeRegion_1200(void)
{
    size_t largeSize = 1024 * 1024;
    char* mem = (char *)malloc(largeSize);
    resTraceFreeRegion(RES_ARKTS_HEAP_MASK, mem, largeSize);
    free(mem);
}

int main(void)
{
    resTraceFreeRegion_0100();
    resTraceFreeRegion_0200();
    resTraceFreeRegion_0300();
    resTraceFreeRegion_0400();
    resTraceFreeRegion_0500();
    resTraceFreeRegion_0600();
    resTraceFreeRegion_0700();
    resTraceFreeRegion_0800();
    resTraceFreeRegion_0900();
    resTraceFreeRegion_1000();
    resTraceFreeRegion_1100();
    resTraceFreeRegion_1200();
    return t_status;
}
