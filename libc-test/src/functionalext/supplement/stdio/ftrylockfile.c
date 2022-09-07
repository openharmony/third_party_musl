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

#include <limits.h>
#include <stdio.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : ftrylockfile_0100
 * @tc.desc      : The parameter is valid, the ftrylockfile function can lock the file.
 * @tc.level     : Level 0
 */
void ftrylockfile_0100(void)
{
    int ret = -1;
    FILE *fptr = fopen("ftrylockfile.txt", "w+");
    ret = ftrylockfile(fptr);
    EXPECT_EQ("ftrylockfile_0100", ret, 0);
    funlockfile(fptr);
    fclose(fptr);
    remove("ftrylockfile.txt");
}

TEST_FUN G_Fun_Array[] = {
    ftrylockfile_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}