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

#include <stdlib.h>
#include "functionalext.h"
#include "filepath_util.h"

/*
 * @tc.name      : exit_0100
 * @tc.desc      : Verify that the file content of the auxiliary application output information is viewed
 *                 by calling the exit(0) function.
 * @tc.level     : Level 0
 */
void exit_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(path);
    char cmd[PATH_MAX] = {0};
    snprintf(cmd, sizeof(cmd), "cd %s; ./exittest01", path);
    system(cmd);

    char str[100] = {0};
    const char *ptr = "/data/Exittest01.txt";
    FILE *fp = fopen(ptr, "r");
    if (!fp) {
        t_error("%s fopen failed\n", __func__);
    }
    int ret = fseek(fp, 0, SEEK_SET);
    if (ret != 0) {
        t_error("%s fseek failed\n", __func__);
    }

    size_t rsize = fread(str, sizeof(str), 1, fp);
    if (strcmp(str, "exit before")) {
        t_error("%s exit failed\n", __func__);
    }

    fclose(fp);
    remove(ptr);
}

/*
 * @tc.name      : exit_0200
 * @tc.desc      : Verify that the file content of the auxiliary application output information
 *                 is viewed by calling the exit(1) function.
 * @tc.level     : Level 0
 */
void exit_0200(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(path);
    char cmd[PATH_MAX] = {0};
    snprintf(cmd, sizeof(cmd), "cd %s; ./exittest02", path);
    system(cmd);

    char abc[100] = {0};
    const char *ptr = "/data/Exittest02.txt";
    FILE *fp = fopen(ptr, "r");
    if (!fp) {
        t_error("%s fopen failed\n", __func__);
    }
    int ret = fseek(fp, 0, SEEK_SET);
    if (ret != 0) {
        t_error("%s fseek failed\n", __func__);
    }
    char *content = fgets(abc, 27, fp);
    if (strcmp(content, "exit before")) {
        t_error("%s exit failed\n", __func__);
    }

    fclose(fp);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    exit_0100();
    exit_0200();
    return t_status;
}