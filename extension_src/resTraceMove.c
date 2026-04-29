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
 * @tc.name      : resTraceMove_0100
 * @tc.desc      : Verify that memory move is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void resTraceMove_0100(void)
{
    char* oldMem = (char *)malloc(1 * sizeof(char));
    char* newMem = (char *)malloc(1 * sizeof(char));
    resTraceMove(RES_ARKTS_HEAP_MASK, oldMem, newMem, 1);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0200
 * @tc.desc      : Verify that memory move is successful with RES_JS_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0200(void)
{
    char* oldMem = (char *)malloc(10 * sizeof(char));
    char* newMem = (char *)malloc(20 * sizeof(char));
    resTraceMove(RES_JS_HEAP_MASK, oldMem, newMem, 20);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0300
 * @tc.desc      : Verify that memory move is successful with RES_KMP_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0300(void)
{
    char* oldMem = (char *)malloc(100 * sizeof(char));
    char* newMem = (char *)malloc(200 * sizeof(char));
    resTraceMove(RES_KMP_HEAP_MASK, oldMem, newMem, 200);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0400
 * @tc.desc      : Verify that memory move is successful with RES_SO_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0400(void)
{
    char* oldMem = (char *)malloc(50 * sizeof(char));
    char* newMem = (char *)malloc(100 * sizeof(char));
    resTraceMove(RES_SO_MASK, oldMem, newMem, 100);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0500
 * @tc.desc      : Verify that memory move is successful with RES_RN_HEAP_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0500(void)
{
    char* oldMem = (char *)malloc(32 * sizeof(char));
    char* newMem = (char *)malloc(64 * sizeof(char));
    resTraceMove(RES_RN_HEAP_MASK, oldMem, newMem, 64);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0600
 * @tc.desc      : Verify that memory move is successful with RES_DMABUF_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0600(void)
{
    char* oldMem = (char *)malloc(128 * sizeof(char));
    char* newMem = (char *)malloc(256 * sizeof(char));
    resTraceMove(RES_DMABUF_MASK, oldMem, newMem, 256);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0700
 * @tc.desc      : Verify that memory move is successful with RES_ASHMEM_MASK
 * @tc.level     : Level 0
 */
void resTraceMove_0700(void)
{
    char* oldMem = (char *)malloc(64 * sizeof(char));
    char* newMem = (char *)malloc(128 * sizeof(char));
    resTraceMove(RES_ASHMEM_MASK, oldMem, newMem, 128);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0800
 * @tc.desc      : Verify that memory move is successful with RES_ARK_GLOBAL_HANDLE
 * @tc.level     : Level 0
 */
void resTraceMove_0800(void)
{
    char* oldMem = (char *)malloc(16 * sizeof(char));
    char* newMem = (char *)malloc(32 * sizeof(char));
    resTraceMove(RES_ARK_GLOBAL_HANDLE, oldMem, newMem, 32);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_0900
 * @tc.desc      : Verify that memory move is successful with RES_ARK_LOCAL_HANDLE
 * @tc.level     : Level 0
 */
void resTraceMove_0900(void)
{
    char* oldMem = (char *)malloc(8 * sizeof(char));
    char* newMem = (char *)malloc(16 * sizeof(char));
    resTraceMove(RES_ARK_LOCAL_HANDLE, oldMem, newMem, 16);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_1000
 * @tc.desc      : Verify that memory move with oldAddr is NULL
 * @tc.level     : Level 2
 */
void resTraceMove_1000(void)
{
    char* newMem = (char *)malloc(10 * sizeof(char));
    resTraceMove(RES_ARKTS_HEAP_MASK, NULL, newMem, 10);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_1100
 * @tc.desc      : Verify that memory move with newAddr is NULL
 * @tc.level     : Level 2
 */
void resTraceMove_1100(void)
{
    char* oldMem = (char *)malloc(10 * sizeof(char));
    resTraceMove(RES_ARKTS_HEAP_MASK, oldMem, NULL, 10);
    free(oldMem);
}

/**
 * @tc.name      : resTraceMove_1200
 * @tc.desc      : Verify that memory move with newSize is 0
 * @tc.level     : Level 2
 */
void resTraceMove_1200(void)
{
    char* oldMem = (char *)malloc(10 * sizeof(char));
    char* newMem = (char *)malloc(10 * sizeof(char));
    resTraceMove(RES_ARKTS_HEAP_MASK, oldMem, newMem, 0);
    free(oldMem);
    free(newMem);
}

/**
 * @tc.name      : resTraceMove_1300
 * @tc.desc      : Verify that memory move with large size
 * @tc.level     : Level 1
 */
void resTraceMove_1300(void)
{
    size_t largeSize = 1024 * 1024;
    char* oldMem = (char *)malloc(largeSize);
    char* newMem = (char *)malloc(largeSize * 2);
    resTraceMove(RES_ARKTS_HEAP_MASK, oldMem, newMem, largeSize * 2);
    free(oldMem);
    free(newMem);
}

int main(void)
{
    resTraceMove_0100();
    resTraceMove_0200();
    resTraceMove_0300();
    resTraceMove_0400();
    resTraceMove_0500();
    resTraceMove_0600();
    resTraceMove_0700();
    resTraceMove_0800();
    resTraceMove_0900();
    resTraceMove_1000();
    resTraceMove_1100();
    resTraceMove_1200();
    resTraceMove_1300();
    return t_status;
}
