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
#include <stdlib.h>
#include <pthread.h>
#include "functionalext.h"

void *pthreadFunc(FILE *fptr)
{
    pthread_t newthid;
    newthid = pthread_self();
    char buf[] = "test1";
    fwrite(buf, 1, sizeof(buf), fptr);
    fseek(fptr, 0, SEEK_END);
    return NULL;
}

/**
 * @tc.name      : flockfile_0100
 * @tc.desc      : Verify that files can be locked
 * @tc.level     : Level 0
 */
void flockfile_0100(void)
{
    FILE *fptr;
    pthread_t tid;
    char buf[] = "test";
    char buff[1024] = "\0";
    bzero(buff, sizeof(buf));
    fptr = fopen("test.txt", "w+");
    ftrylockfile(fptr);
    fwrite(buf, 1, strlen(buf), fptr);
    fseek(fptr, 0, SEEK_END);
    int ret1 = pthread_create(&tid, NULL, (void *)pthreadFunc, fptr);
    sleep(3);
    rewind(fptr);
    fgets(buff, 1024, fptr);
    EXPECT_STREQ("flockfile_0100", buf, buff);
    fclose(fptr);
    remove("test.txt");
}

int main()
{
    flockfile_0100();
    return t_status;
}