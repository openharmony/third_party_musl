#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdWriteTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: write_001
 * @tc.desc: checks whether the write function behaves correctly when writing data to a file opened in write-only
 *           mode and when attempting to write data to a file opened in read-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdWriteTest, write_001, TestSize.Level1)
{
    char buf[1];
    size_t ct = atoi("2");
    int fd = open("/dev/null", O_RDONLY);
    EXPECT_EQ(-1, write(fd, buf, ct));

    fd = open("/dev/null", O_WRONLY);
    EXPECT_EQ(2, write(fd, buf, ct));
    close(fd);
}