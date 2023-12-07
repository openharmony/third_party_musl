#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>


using namespace testing::ext;

class FortifyReadlinkchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __readlinkat_chk_001
 * @tc.desc: Verify the correctness of the readlink function for symbolic link files in a specific path.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyReadlinkchkTest, __readlinkat_chk_001, TestSize.Level1)
{
    char path[PATH_MAX];
    ssize_t length = __readlinkat_chk(AT_FDCWD, "/dev/stdin", path, sizeof(path), sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/proc/self/fd/0", std::string(path, length));

    length = __readlinkat_chk(AT_FDCWD, "/dev/stdout", path, sizeof(path), sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/proc/self/fd/1", std::string(path, length));

    length = __readlinkat_chk(AT_FDCWD, "/dev/stderr", path, sizeof(path), sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/proc/self/fd/2", std::string(path, length));

    length = __readlinkat_chk(AT_FDCWD, "/dev/fd", path, sizeof(path), sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/proc/self/fd", std::string(path, length));
}