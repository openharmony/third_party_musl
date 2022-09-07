/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : __fpurge_0100
 * @tc.desc      : Each parameter value is valid and can be used to completely clear the data in the buffer.
 * @tc.level     : Level 0
 */
void __fpurge_0100()
{
    char buf[100];
    char *str = "This is a test";
    FILE *fp = fopen("fpurge.txt", "w");
    fputs(str, fp);
    size_t ret = __fpurge(fp);
    fgets(buf, 100, fp);
    EXPECT_EQ("__fpurge_0100", strstr(buf, str), 0);
    EXPECT_EQ("__fpurge_0100", ret, 0);
    fclose(fp);
    remove("fpurge.txt");
}

int main()
{
    __fpurge_0100();
    return t_status;
}