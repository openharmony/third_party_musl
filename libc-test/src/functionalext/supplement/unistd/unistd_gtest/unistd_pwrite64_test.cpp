#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr size_t N_BITES = 2;

class UnistdPwrite64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pwrite64_001
 * @tc.desc: checks whether the pwrite64 function behaves correctly when writing data to a file opened in write-only
 *           mode and when attempting to write data to a file opened in read-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdPwrite64Test, pwrite64_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/dev/null", O_WRONLY);
    EXPECT_EQ(N_BITES, pwrite64(fd, buf, N_BITES, 0));
    fd = open("/dev/null", O_RDONLY);
    EXPECT_EQ(-1, pwrite64(fd, buf, N_BITES, 0));
    close(fd);
}