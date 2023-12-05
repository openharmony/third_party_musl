#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/vfs.h>

using namespace testing::ext;

class FcntlFcntlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fcntl_001
 * @tc.desc: Verify the functionality of the fcntl interface's F_GETFD option.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_001, TestSize.Level1)
{
    int fd = open("testfile.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    int flags = fcntl(fd, F_GETFD);
    ASSERT_NE(flags, -1);
    EXPECT_EQ(flags & FD_CLOEXEC, 0);
    close(fd);
    EXPECT_EQ(0, remove("testfile.txt"));
}

/**
 * @tc.name: fcntl_002
 * @tc.desc: Verify whether the fcntl function successfully returns a new file descriptor, and whether this new file
 *           descriptor is a non-negative value.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_002, TestSize.Level1)
{
    int fd = open("test.txt", O_RDONLY | O_CREAT, 0644);
    int newFd = fcntl(fd, F_DUPFD, 0);
    EXPECT_GE(newFd, 0);
    close(fd);
    close(newFd);
    EXPECT_EQ(0, remove("test.txt"));
}

/**
 * @tc.name: fcntl_003
 * @tc.desc: Verify the functionality of the fcntl function by opening the /proc/version file, setting a write lock,
 *           retrieving the lock status, and closing the file.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_003, TestSize.Level1)
{
    int file = open("/proc/version", O_RDONLY);
    EXPECT_NE(file, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_start = 0;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_len = 0;
    EXPECT_TRUE(fcntl(file, F_GETLK, &fileLock) == 0);
    close(file);
}

/**
 * @tc.name: fcntl_004
 * @tc.desc: Verify the functionality of the fcntl function by opening the /proc/version file using the open64 function,
 *           setting a write lock, retrieving the lock status with the F_GETLK64 command, and closing the file.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_004, TestSize.Level1)
{
    int file = open64("/proc/version", O_RDONLY);
    EXPECT_TRUE(file != -1);
    struct flock64 fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_start = 0;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_len = 0;
    EXPECT_TRUE(fcntl(file, F_GETLK64, &fileLock) == 0);
    close(file);
}

/**
 * @tc.name: fcntl_005
 * @tc.desc: Verify whether the fcntl function successfully returns the flags and whether these flags are non-negative
 *           values.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_005, TestSize.Level1)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    EXPECT_GE(flags, 0);
}

/**
 * @tc.name: fcntl_006
 * @tc.desc: Verify whether the fcntl function successfully returns the owner process ID and whether this process ID is
 *           not equal to -1.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_006, TestSize.Level1)
{
    pid_t owner = fcntl(STDIN_FILENO, F_GETOWN);
    EXPECT_NE(owner, -1);
}

/**
 * @tc.name: fcntl_007
 * @tc.desc: Verify the functionality of the fcntl interface's F_SETFD option.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_007, TestSize.Level1)
{
    int fd = open("testfile.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    int flags = fcntl(fd, F_SETFD);
    ASSERT_NE(flags, -1);
    EXPECT_EQ(flags & FD_CLOEXEC, 0);
    close(fd);
    EXPECT_EQ(0, remove("testfile.txt"));
}

/**
 * @tc.name: fcntl_008
 * @tc.desc: Set the file status flags of the standard input file descriptor using the fcntl function with the F_SETFL
 *           flag and verifies if the return value is greater than or equal to 0.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_008, TestSize.Level1)
{
    int flags = fcntl(STDIN_FILENO, F_SETFL);
    EXPECT_GE(flags, 0);
}

/**
 * @tc.name: fcntl_009
 * @tc.desc: Add a write lock to the /proc/version file using the fcntl function, and then closes the file to verify if
 *           the write lock setting operation is successful.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_009, TestSize.Level1)
{
    int file = open("text.txt", O_RDWR | O_CREAT, 0644);
    EXPECT_NE(file, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_start = 0;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_len = 0;
    int result = fcntl(file, F_SETLK, &fileLock);
    EXPECT_EQ(result, 0);
    close(file);
    EXPECT_EQ(0, remove("text.txt"));
}

/**
 * @tc.name: fcntl_010
 * @tc.desc: Add a blocking write lock to the /proc/version file using the fcntl function, and then closes the file to
 *           verify if the write lock setting operation is successful.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_010, TestSize.Level1)
{
    int file = open("testFile.txt", O_RDWR | O_CREAT, 0644);
    EXPECT_NE(file, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_start = 0;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_len = 0;
    int result = fcntl(file, F_SETLKW, &fileLock);
    EXPECT_EQ(result, 0);
    close(file);
    EXPECT_EQ(0, remove("testFile.txt"));
}
