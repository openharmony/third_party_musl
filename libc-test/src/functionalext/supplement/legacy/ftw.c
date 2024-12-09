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

#include <ftw.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_PATH_DEPTH 5
#define TEST_FTW_PATH "/data/local/tmp/ftwPath"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

const int FAIL = -1;
int fn(const char *file, const struct stat *sb, int flag)
{
    return 0;
}

/**
 * @tc.name      : ftw_0100
 * @tc.desc      : Each parameter is valid and can traverse the directory tree.
 * @tc.level     : Level 0
 */
void ftw_0100()
{
    int ret = -1;
    ret = ftw(TEST_FTW_PATH, fn, 500);
    EXPECT_NE("ftw_0100", ret, FAIL);
}

void remove_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    char filepath[PATH_MAX];
  
    if (!(dir = opendir(path))) {
        return;
    }
  
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
  
            int result = snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
            if (result >= sizeof(filepath)) {
                t_error("%s error in snprintf! \n", __func__);
            }
            remove_directory(filepath);
        } else {
            int result = snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
            if (result >= sizeof(filepath)) {
                t_error("%s error in snprintf! \n", __func__);
            }
            if (remove(filepath) == -1) {
                t_error("%s error in remove test nftw filepath! \n", __func__);
            }
        }
    }
  
    closedir(dir);
  
    // Now we can remove the empty directory
    if (rmdir(path) == -1) {
        t_error("%s error in rmdir test nftw path! \n", __func__);
    }
}

void nftw_build_testfile(const char *path)
{
    // Create directory
    if (mkdir(path, 0755) == -1) {
        t_error("%s error in mkdir test nftw path! %s \n", __func__, path);
        return;
    }
    
    char file[PATH_MAX];
    int result = snprintf(file, sizeof(file), "%s/normal_file.txt", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    // Create plain file
    int fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    result = snprintf(file, sizeof(file), "%s/non-executable_file.txt", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    // Create non-executable file
    fd = open(file, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    result = snprintf(file, sizeof(file), "%s/unauthorized_file.txt", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    // Create unauthorized file
    fd = open(file, O_WRONLY | O_CREAT, 0000);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    result = snprintf(file, sizeof(file), "%s/.hidden_file.txt", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    // Create Hidden Files
    fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        t_error("%s error in open hidden_file.txt! \n", __func__);
        return;
    }
    close(fd);
  
    result = snprintf(file, sizeof(file), "%s/read_only_file.txt", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    //Create Read-only files
    fd = open(file, O_WRONLY | O_CREAT, 0444);
    if (fd == -1) {
        t_error("%s error in open read_only_file.txt! \n", __func__);
        return;
    }
    close(fd);
  
    result = snprintf(file, sizeof(file), "%s/symlink_to_normal_file", path);
    if (result >= sizeof(file)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    // Create Symbolic links
    if (symlink("normal_file.txt", file) == -1) {
        t_error("%s error in open symlink_to_normal_file.txt! \n", __func__);
        return;
    }
}

void nftw_build_testDir()
{
    nftw_build_testfile(TEST_FTW_PATH);
    char path[PATH_MAX];
    int result = snprintf(path, sizeof(path), "%s", TEST_FTW_PATH);
    if (result >= sizeof(path)) {
        t_error("%s error in snprintf! \n", __func__);
    }
    for (int i = 0 ; i < TEST_PATH_DEPTH ; i++) {
        result = snprintf(path, sizeof(path), "%s/data", path);
        if (result >= sizeof(path)) {
            t_error("%s error in snprintf! \n", __func__);
        }
        nftw_build_testfile(path);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_lock(&g_mutex);
    nftw_build_testDir();
    ftw_0100();
    remove_directory(TEST_FTW_PATH);
    pthread_mutex_unlock(&g_mutex);
    return t_status;
}