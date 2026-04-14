/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <pwd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name : eaccess_0100
 * @tc.desc : The parameter filename is the specified file and exists,
 *            the parameter amode is F_OK,function returns successfully.
 * @tc.level : Level 0
 */
void eaccess_0100(void) {
    const char *ptr = "eaccess_test.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("eaccess_0100", fptr, NULL);
    int result = eaccess(ptr, F_OK);
    EXPECT_EQ("eaccess_0100", result, SUCCESS);
    result = fclose(fptr);
    EXPECT_EQ("eaccess_0100", result, SUCCESS);
    result = remove(ptr);
    EXPECT_EQ("eaccess_0100", result, SUCCESS);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name : eaccess_0200
 * @tc.desc : Parameter filename is a directory that exists,
 *            amode is F_OK, function returns successfully.
 * @tc.level : Level 0
 */
void eaccess_0200(void) {
    const char *dirName = "eaccess_test_dir";
    mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    int ret = mkdir(dirName, mode);
    EXPECT_EQ("eaccess_0200", ret, SUCCESS);
    int result = eaccess(dirName, F_OK);
    EXPECT_EQ("eaccess_0200", result, SUCCESS);
    result = rmdir(dirName);
    EXPECT_EQ("eaccess_0200", result, SUCCESS);
}

/**
 * @tc.name : eaccess_0300
 * @tc.desc : Parameter filename is a symbolic link pointing to an existing file, 
 *            amode is R_OK, function returns successfully (follows link).
 * @tc.level : Level 0
 */
void eaccess_0300(void) {
    const char *targetFile = "target_test.txt";
    const char *linkName = "symlink_test";
    
    FILE *fptr = fopen(targetFile, "w");
    EXPECT_PTRNE("eaccess_0300", fptr, NULL);
    int ret = fprintf(fptr, "musl");
    EXPECT_EQ("eaccess_0300", ret, 4);
    ret = fclose(fptr);
    EXPECT_EQ("eaccess_0300", ret, SUCCESS);
    ret = symlink(targetFile, linkName);
    EXPECT_EQ("eaccess_0300", ret, SUCCESS);
    int result = eaccess(linkName, R_OK);
    EXPECT_EQ("eaccess_0300", result, SUCCESS);
    
    result = remove(linkName);
    EXPECT_EQ("eaccess_0300", result, SUCCESS);
    result = remove(targetFile);
    EXPECT_EQ("eaccess_0300", result, SUCCESS);
}

/**
 * @tc.name : eaccess_0400
 * @tc.desc : Parameter filename is a FIFO (named pipe), amode is W_OK, 
 *            function returns successfully if pipe can be opened for writing.
 * @tc.level : Level 0
 */
void eaccess_0400(void) {
    const char *fifoName = "fifo_test";
    int ret = mkfifo(fifoName, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    EXPECT_EQ("eaccess_0400", ret, SUCCESS);
    int result = eaccess(fifoName, W_OK);
    EXPECT_EQ("eaccess_0400", result, SUCCESS);
    
    result = remove(fifoName);
    EXPECT_EQ("eaccess_0400", result, SUCCESS);
}

/**
 * @tc.name : eaccess_0500
 * @tc.desc : Parameter filename is a character device (e.g., /dev/null if exists), 
 *            amode is W_OK, function checks write permission.
 * @tc.level : Level 0
 */
void eaccess_0500(void) {
    const char *device = "/dev/null";
    int result = eaccess(device, W_OK);

    if (access(device, F_OK) == SUCCESS) { 
        EXPECT_EQ("eaccess_0500", result, SUCCESS);
    }
}

/**
 * @tc.name : eaccess_0600
 * @tc.desc : Parameter filename is a socket file, amode is F_OK, 
 *            socket file not exists.
 * @tc.level : Level 0
 */
void eaccess_0600(void) {
    const char *socketPath = "socket_test_0600";
    struct sockaddr_un addr;
    int sockfd = -1;

    addr.sun_family = 0;
    for (size_t i = 0; i < sizeof(addr.sun_path); i++) {
        addr.sun_path[i] = 0;
    }
    addr.sun_family = AF_UNIX;
    size_t len = strlen(socketPath);
    if (len < sizeof(addr.sun_path)) {
        size_t i;
        for (i = 0; i < len; i++) {
            addr.sun_path[i] = socketPath[i];
        }
        addr.sun_path[i] = '\0';
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd >= 0) {
            socklen_t addr_len = sizeof(addr);
            int bindResult = bind(sockfd, (struct sockaddr*)&addr, addr_len);
            if (bindResult == 0) {
                int result = eaccess(socketPath, F_OK);
                EXPECT_EQ("eaccess_0600", result, 0);
                result = unlink(socketPath);
                EXPECT_EQ("eaccess_0600", result, 0);
            }
           int ret = close(sockfd);
           EXPECT_EQ("eaccess_0600", ret, 0);
        }
    }
}

/**
 * @tc.name : eaccess_0700
 * @tc.desc : Parameter filename is a regular file without read permission for others, 
 *            current real user tries R_OK, expecting SUCCESS (assuming owner runs test).
 * @tc.level : Level 0
 */
void eaccess_0700(void) {
    const char *ptr = "access_test.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("eaccess_0700", fptr, NULL);
    int result = fprintf(fptr, "musl");
    EXPECT_EQ("eaccess_0700", result, 4);
    result = fclose(fptr);
    EXPECT_EQ("eaccess_0700", result, SUCCESS);
    chmod(ptr, S_IRUSR | S_IWUSR);
    
    result = eaccess(ptr, R_OK);
    EXPECT_EQ("eaccess_0700", result, SUCCESS);
    
    result = remove(ptr);
    EXPECT_EQ("eaccess_0700", result, SUCCESS);
}

/**
 * @tc.name : eaccess_0800
 * @tc.desc : Parameter filename is a regular file, amode is X_OK for a non-executable file, 
 *            function returns failure (Permission denied).
 * @tc.level : Level 0
 */
void eaccess_0800(void) {
    const char *ptr = "noexec_test.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("eaccess_0800", fptr, NULL);

    chmod(ptr, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    int result = eaccess(ptr, X_OK);
    EXPECT_EQ("eaccess_0800", result, FAILED);
    
    result = remove(ptr);
    EXPECT_EQ("eaccess_0800", result, SUCCESS);
}

/**
 * @tc.name : eaccess_0900
 * @tc.desc : Parameter filename is NULL or empty string, function returns failure (EINVAL).
 * @tc.level : Level 0
 */
void eaccess_0900(void) {
    int result1 = eaccess(NULL, F_OK);
    EXPECT_EQ("eaccess_0900_null", result1, FAILED);
    int result2 = eaccess("", F_OK);
    EXPECT_EQ("eaccess_0900_empty", result2, FAILED);
}

/**
 * @tc.name : eaccess_1000
 * @tc.desc : Parameter filename is a symbolic link loop, function returns failure (ELOOP).
 * @tc.level : Level 2
 */
void eaccess_1000(void) {
    const char *link1 = "loop1";
    const char *link2 = "loop2";
    
    symlink(link2, link1);
    symlink(link1, link2);
    
    int result = eaccess(link1, F_OK);
    EXPECT_EQ("eaccess_1000", result, FAILED);
    
    result = remove(link1);
    EXPECT_EQ("eaccess_1000", result, SUCCESS);
    result = remove(link2);
    EXPECT_EQ("eaccess_1000", result, SUCCESS);
}

TEST_FUN G_Fun_Array[] = { 
    eaccess_0100,
    eaccess_0200,
    eaccess_0300,
    eaccess_0400,
    eaccess_0500,
    eaccess_0600,
    eaccess_0700,
    eaccess_0800,
    eaccess_0900,
    eaccess_1000,
};

int main(int argc, char *argv[]) {
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
