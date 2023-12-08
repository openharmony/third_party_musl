#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class StatChmodTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: chmod_001
 * @tc.desc: Verify that the "chmod" function successfully changes the permission bits of a file without
 *           encountering any errors or exceptions.
 * @tc.type: FUNC
 **/
HWTEST_F(StatChmodTest, chmod_001, TestSize.Level1)
{
    const char* dirname = "test_chmod.txt";
    mode_t mode1 = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_NE(-1, mkdir(dirname, mode1));

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    open("test_chmod.txt", O_RDWR | O_CREAT);
    EXPECT_EQ(chmod("test_chmod.txt", mode), 0);
    remove("test_chmod.txt");
}

/**
 * @tc.name: chmod_002
 * @tc.desc: Assist in the verification and validation of software that relies on the "chmod" function for file
 *           permission management.
 * @tc.type: FUNC
 **/
HWTEST_F(StatChmodTest, chmod_002, TestSize.Level1)
{
    EXPECT_EQ(-1, chmod(nullptr, 0));
}