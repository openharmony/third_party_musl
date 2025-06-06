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

#include <arpa/inet.h>
#include <netdb.h>
#include "functionalext.h"

/**
 * @tc.name      : gethostent_0100
 * @tc.desc      : Able to get host information.
 * @tc.level     : Level 0
 */
void gethostent_0100(void)
{
    struct hostent *host = NULL;
    sethostent(1);
    host = gethostent();
    EXPECT_EQ("gethostent_0100", host, 0);
    endhostent();
}

int main(int argc, char *argv[])
{
    gethostent_0100();
    return t_status;
}