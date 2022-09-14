/*
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

#include <stdio.h>
#include <stdbool.h>
#include <resolv.h>
#include "functionalext.h"

/**
 * @tc.name      : res_send_0100
 * @tc.desc      : Verify that the response can be sent (all parameters are valid)
 * @tc.level     : Level 0
 */
void res_send_0100(void)
{
    bool flag = false;
    int result;
    unsigned char buf1[1024] = "127.0.0.1";
    unsigned char buf2[1024] = "\0";
    result = res_send(buf1, 0, buf2, 10);
    if (result >= 0) {
        flag = true;
    }
    EXPECT_TRUE("res_send_0100", flag);
}

int main(int argc, char *argv[])
{
    res_send_0100();
    return t_status;
}