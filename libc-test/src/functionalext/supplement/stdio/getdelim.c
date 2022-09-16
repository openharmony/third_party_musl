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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include "functionalext.h"

/**
 * @tc.name      : getdelim_0100
 * @tc.desc      : Verify that the file content can be read with the specified delimiter (parameters are valid)
 * @tc.level     : Level 0
 */
void getdelim_0100(void)
{
    ssize_t result;
    char *wrstring = "helloworld";
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fp);
    fseek(fp, 0, SEEK_SET);
    result = getdelim(&line, &len, 'l', fp);
    EXPECT_TRUE("getdelim_0100", result > 0);
    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0200
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the s parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0200(void)
{
    ssize_t result;
    char *line = NULL;
    char *wrstring = "helloworld";
    size_t a = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    result = getdelim(&line, NULL, 'l', fp);
    EXPECT_EQ("getdelim_0200", result, -1);
    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0300
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the n parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0300(void)
{
    ssize_t result;
    char *wrstring = "helloworld";
    size_t a = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    result = getdelim(NULL, &a, 'l', fp);
    EXPECT_EQ("getdelim_0300", result, -1);
    remove("getdelim.txt");
}

int main(int argc, char *argv[])
{
    getdelim_0100();
    getdelim_0200();
    getdelim_0300();

    return t_status;
}