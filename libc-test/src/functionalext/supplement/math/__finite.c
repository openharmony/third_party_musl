/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#include "functionalext.h"
#include <float.h>

const int SUCCESS = 1;
const int FAILED = 0;

/**
 * @tc.name      : __finite_0100
 * @tc.desc      : The parameter is 0.0, it is judged that the parameter is a finite value.
 * @tc.level     : Level 0
 */
void __finite_0100(void)
{
    int ret = __finite(0.0);
    EXPECT_EQ("__finite_0100", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_0200
 * @tc.desc      : The parameter is DBL_MIN/2.0, and it is judged that the parameter is a finite value
 * @tc.level     : Level 0
 */
void __finite_0200(void)
{
    int ret = __finite(DBL_MIN/2.0);
    EXPECT_EQ("__finite_0200", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_0300
 * @tc.desc      : The parameter is 0.0 / 0.0, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0300(void)
{
    int ret = __finite(0.0 / 0.0);
    EXPECT_EQ("__finite_0300", ret, FAILED);
}

/**
 * @tc.name      : __finite_0400
 * @tc.desc      : The parameter is NAN, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0400(void)
{
    int ret = __finite(NAN);
    EXPECT_EQ("__finite_0400", ret, FAILED);
}

/**
 * @tc.name      : __finite_0500
 * @tc.desc      : The parameter is INFINITY, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0500(void)
{
    int ret = __finite(INFINITY);
    EXPECT_EQ("__finite_0500", ret, FAILED);
}

/**
 * @tc.name      : __finite_0600
 * @tc.desc      : The parameter is -INFINITY, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0600(void)
{
    int ret = __finite(-INFINITY);
    EXPECT_EQ("__finite_0600", ret, FAILED);
}

/**
 * @tc.name      : __finite_0700
 * @tc.desc      : The parameter is sqrt(-1.0), and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0700(void)
{
    int ret = __finite(sqrt(-1.0));
    EXPECT_EQ("__finite_0700", ret, FAILED);
}

/**
 * @tc.name      : __finite_0800
 * @tc.desc      : The parameter is exp(800), and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0800(void)
{
    int ret = __finite(exp(800));
    EXPECT_EQ("__finite_0800", ret, FAILED);
}

int main(void)
{
    __finite_0100();
    __finite_0200();
    __finite_0300();
    __finite_0400();
    __finite_0500();
    __finite_0600();
    __finite_0700();
    __finite_0800();
    return t_status;
}