/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <unistd.h>
#include "functionalext.h"

/*
 * @tc.name      : chroot_0100
 * @tc.desc      : Verify that the file root directory can be changed (parameters are valid)
 * @tc.level     : Level 0
 */
void chroot_0100(void)
{
    char *ftpr = "/";
    int result = chroot(ftpr);
    EXPECT_EQ("chroot_0100", result, 0);
}

/*
 * @tc.name      : chroot_0200
 * @tc.desc      : Verify that the file root directory cannot be changed (parameter is NULL)
 * @tc.level     : Level 2
 */
void chroot_0200(void)
{
    char *ftpr = "data/app";
    int result = chroot(NULL);
    EXPECT_EQ("chroot_0100", result, -1);
}

/*
 * @tc.name      : chroot_0300
 * @tc.desc      : Verify that the file root directory cannot be changed (parameter exceeds maximum length)
 * @tc.level     : Level 2
 */
void chroot_0300(void)
{
    char *ftpr = "data/app/el1/bundle/public/com.example.disributedcalc/com.example.disributedcalc/assets/js";
    int result = chroot(ftpr);
    EXPECT_EQ("chroot_0100", result, -1);
}

/*
 * @tc.name      : chroot_0400
 * @tc.desc      : Verify that the file root directory cannot be changed (parameter invalid)
 * @tc.level     : Level 2
 */
void chroot_0400(void)
{
    char *ftpr = "system/app";
    int result = chroot(ftpr);
    EXPECT_EQ("chroot_0100", result, -1);
}

int main(int argc, char *argv[])
{
    chroot_0100();
    chroot_0200();
    chroot_0300();
    chroot_0400();

    return t_status;
}