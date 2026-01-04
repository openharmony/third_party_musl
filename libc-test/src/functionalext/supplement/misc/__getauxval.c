/**
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

#include <sys/auxv.h>
#include "functionalext.h"

/*
 * @tc.name      : __getauxval_0100
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SECURE)
 * @tc.level     : Level 0
 */
void __getauxval_0100(void)
{
    unsigned long int result;
    result = __getauxval(AT_SECURE);
    EXPECT_EQ("__getauxval_0100", result, 0);
}

/*
 * @tc.name      : __getauxval_0200
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO)
 * @tc.level     : Level 0
 */
void __getauxval_0200(void)
{
    unsigned long int result;
    result = __getauxval(AT_SYSINFO);
    EXPECT_EQ("__getauxval_0200", result, 0);
}

/*
 * @tc.name      : __getauxval_0300
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO_EHDR)
 * @tc.level     : Level 0
 */
void __getauxval_0300(void)
{
    unsigned long int result;
    result = __getauxval(AT_SYSINFO_EHDR);
    EXPECT_TRUE("__getauxval_0300", result != 0);
}

/*
 * @tc.name      : __getauxval_0400
 * @tc.desc      : Verify that __getauxval returns 0 and sets errno for non-existent item
 * @tc.level     : Level 0
 */
void __getauxval_0400(void)
{
    unsigned long int result;
    errno = 0;
    result = __getauxval(0xFFFFFFFF);
    EXPECT_EQ("__getauxval_0400", result, 0);
    EXPECT_EQ("__getauxval_0400", errno, ENOENT);
}

/*
 * @tc.name      : __getauxval_0500
 * @tc.desc      : Verify boundary conditions
 * @tc.level     : Level 0
 */
void __getauxval_0500(void)
{
    unsigned long int result;
    errno = 0;
    result = __getauxval(0);
    EXPECT_EQ("__getauxval_0500", result, 0);
    EXPECT_EQ("__getauxval_0500", errno, ENOENT);
}

int main(int argc, char *argv[])
{
    __getauxval_0100();
    __getauxval_0200();
    __getauxval_0300();
    __getauxval_0400();
    __getauxval_0500();
    return t_status;
}