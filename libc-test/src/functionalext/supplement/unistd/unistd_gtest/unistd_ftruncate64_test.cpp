#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdFtruncate64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ftruncate64_001
 * @tc.desc: Checked whether the ftruncate64 function successfully truncated the file to the specified size and updated
 *           the file size accordingly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdFtruncate64Test, ftruncate64_001, TestSize.Level1)
{
    FILE* fptr = fopen("test.txt", "w");
    EXPECT_EQ(0, ftruncate64(fileno(fptr), 666));
    EXPECT_EQ(0, fclose(fptr));

    struct stat statbuff;
    EXPECT_EQ(0, stat("test.txt", &statbuff));
    EXPECT_EQ(666, statbuff.st_size);

    fptr = nullptr;
}