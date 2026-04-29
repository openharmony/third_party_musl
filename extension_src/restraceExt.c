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
 * @tc.name      : restraceExt_0100
 * @tc.desc      : Verify that memory allocation is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void restraceExt_0100(void)
{
    char* mem = (char *)malloc(1 * sizeof(char));
    restraceExt(RES_ARK_GLOBAL_HANDLE, mem, 1, TAG_RES_ARK_GLOBAL_HANDLE, true, true);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0200
 * @tc.desc      : Verify that memory allocation is successful with RES_ARKTS_HEAP_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0200(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    restraceExt(RES_ARKTS_HEAP_MASK, mem, 10, TAG_RES_ARKTS_HEAP_MASK, true, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0300
 * @tc.desc      : Verify that memory allocation is successful with RES_JS_HEAP_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0300(void)
{
    char* mem = (char *)malloc(20 * sizeof(char));
    restraceExt(RES_JS_HEAP_MASK, mem, 20, TAG_RES_JS_HEAP_MASK, true, true);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0400
 * @tc.desc      : Verify that memory allocation is successful with RES_KMP_HEAP_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0400(void)
{
    char* mem = (char *)malloc(50 * sizeof(char));
    restraceExt(RES_KMP_HEAP_MASK, mem, 50, TAG_RES_KMP_HEAP_MASK, false, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0500
 * @tc.desc      : Verify that memory allocation is successful with RES_SO_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0500(void)
{
    char* mem = (char *)malloc(100 * sizeof(char));
    restraceExt(RES_SO_MASK, mem, 100, TAG_RES_SO_MASK, true, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0600
 * @tc.desc      : Verify that memory allocation is successful with RES_RN_HEAP_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0600(void)
{
    char* mem = (char *)malloc(32 * sizeof(char));
    restraceExt(RES_RN_HEAP_MASK, mem, 32, TAG_RES_DMABUF, true, true);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0700
 * @tc.desc      : Verify that memory allocation is successful with RES_DMABUF_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0700(void)
{
    char* mem = (char *)malloc(64 * sizeof(char));
    restraceExt(RES_DMABUF_MASK, mem, 64, TAG_RES_DMABUF, false, true);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0800
 * @tc.desc      : Verify that memory allocation is successful with RES_ASHMEM_MASK
 * @tc.level     : Level 0
 */
void restraceExt_0800(void)
{
    char* mem = (char *)malloc(128 * sizeof(char));
    restraceExt(RES_ASHMEM_MASK, mem, 128, TAG_RES_ASHMEM_MASK, true, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_0900
 * @tc.desc      : Verify that memory allocation is successful with RES_ARK_LOCAL_HANDLE
 * @tc.level     : Level 0
 */
void restraceExt_0900(void)
{
    char* mem = (char *)malloc(16 * sizeof(char));
    restraceExt(RES_ARK_LOCAL_HANDLE, mem, 16, TAG_RES_ARK_LOCAL_HANDLE, false, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_1000
 * @tc.desc      : Verify that memory allocation is successful with is_using=false and isWeakRef=false
 * @tc.level     : Level 0
 */
void restraceExt_1000(void)
{
    char* mem = (char *)malloc(8 * sizeof(char));
    restraceExt(RES_ARK_GLOBAL_HANDLE, mem, 8, TAG_RES_ARK_GLOBAL_HANDLE, false, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_1100
 * @tc.desc      : Verify that memory allocation is successful with is_using=true and isWeakRef=false
 * @tc.level     : Level 0
 */
void restraceExt_1100(void)
{
    char* mem = (char *)malloc(8 * sizeof(char));
    restraceExt(RES_ARK_GLOBAL_HANDLE, mem, 8, TAG_RES_ARK_GLOBAL_HANDLE, true, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_1200
 * @tc.desc      : Verify that memory allocation is successful with is_using=false and isWeakRef=true
 * @tc.level     : Level 0
 */
void restraceExt_1200(void)
{
    char* mem = (char *)malloc(8 * sizeof(char));
    restraceExt(RES_ARK_GLOBAL_HANDLE, mem, 8, TAG_RES_ARK_GLOBAL_HANDLE, false, true);
    free(mem);
}

/**
 * @tc.name      : restraceExt_1300
 * @tc.desc      : Verify that memory allocation with addr is NULL
 * @tc.level     : Level 2
 */
void restraceExt_1300(void)
{
    restraceExt(RES_ARKTS_HEAP_MASK, NULL, 10, TAG_RES_ARKTS_HEAP_MASK, true, true);
}

/**
 * @tc.name      : restraceExt_1400
 * @tc.desc      : Verify that memory allocation with size is 0
 * @tc.level     : Level 2
 */
void restraceExt_1400(void)
{
    char* mem = (char *)malloc(10 * sizeof(char));
    restraceExt(RES_ARKTS_HEAP_MASK, mem, 0, TAG_RES_ARKTS_HEAP_MASK, true, false);
    free(mem);
}

/**
 * @tc.name      : restraceExt_1500
 * @tc.desc      : Verify that memory allocation with large size
 * @tc.level     : Level 1
 */
void restraceExt_1500(void)
{
    size_t largeSize = 1024 * 1024;
    char* mem = (char *)malloc(largeSize);
    restraceExt(RES_ARKTS_HEAP_MASK, mem, largeSize, TAG_RES_ARKTS_HEAP_MASK, true, true);
    free(mem);
}

int main(void)
{
    restraceExt_0100();
    restraceExt_0200();
    restraceExt_0300();
    restraceExt_0400();
    restraceExt_0500();
    restraceExt_0600();
    restraceExt_0700();
    restraceExt_0800();
    restraceExt_0900();
    restraceExt_1000();
    restraceExt_1100();
    restraceExt_1200();
    restraceExt_1300();
    restraceExt_1400();
    restraceExt_1500();
    return t_status;
}