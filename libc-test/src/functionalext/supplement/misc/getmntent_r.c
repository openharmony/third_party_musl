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

#include <mntent.h>
#include <pthread.h>
#include <stdbool.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t SUCCESS = 0;
static pthread_barrier_t g_barrier;
#define WAIT() pthread_barrier_wait(&g_barrier)
void *getmntent_rOne()
{
    char c[1000];
    char str[100];
    FILE *fptr = NULL;
    char strings[1024];
    if ((fptr = fopen("/proc/mounts", "r")) == NULL) {
        printf("Error! opening file");
    }
    fscanf(fptr, "%[^\n]", c);
    fclose(fptr);
    sscanf(c, "%[^ ]", str);
    struct mntent *m = NULL;
    struct mntent mntbuf;
    memset(&m, 0x00, sizeof(m));
    memset(&strings[0], 0x00, sizeof(strings));
    FILE *ffp = setmntent("/proc/mounts", "r");
    if (!ffp) {
        printf("error:%s\n", strerror(errno));
    }
    WAIT();
    m = getmntent_r(ffp, &mntbuf, strings, sizeof(strings));
    if (m != NULL) {
        EXPECT_EQ("getmntent_r_0100", strcmp(m->mnt_fsname, str), 0);
        EXPECT_EQ("getmntent_r_0100", strcmp(mntbuf.mnt_fsname, str), 0);
    } else {
        printf("error\n");
    }
    endmntent(ffp);
    return m;
}

/**
 * @tc.name      : getmntent_r_0100
 * @tc.desc      : Multiple threads simultaneously read file information.
 * @tc.level     : Level 0
 */
void getmntent_r_0100(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, getmntent_rOne, NULL);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, getmntent_rOne, NULL);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("getmntent_r_0100", SUCCESS, ret);
}

/**
 * @tc.name      : getmntent_r_0200
 * @tc.desc      : The f parameter is invalid, reading the file information failed.
 * @tc.level     : Level 2
 */
void getmntent_r_0200(void)
{
    char c[1000];
    char str[100];
    FILE *fptr = NULL;
    char strings[1024];
    if ((fptr = fopen("/data/getmntent.txt", "w")) == NULL) {
        printf("Error! opening file");
    }
    fclose(fptr);
    struct mntent *m = NULL;
    struct mntent mntbuf;
    memset(&m, 0x00, sizeof(m));
    memset(&strings[0], 0x00, sizeof(strings));
    FILE *ffp = setmntent("/data/getmntent.txt", "r");
    if (!ffp) {
        printf("error:%s\n", strerror(errno));
    }
    m = getmntent_r(ffp, &mntbuf, strings, sizeof(strings));
    EXPECT_EQ("getmntent_r_0200", m, NULL);
    endmntent(ffp);
    remove("/data/getmntent.txt");
}

TEST_FUN G_Fun_Array[] = {
    getmntent_r_0100,
    getmntent_r_0200,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}