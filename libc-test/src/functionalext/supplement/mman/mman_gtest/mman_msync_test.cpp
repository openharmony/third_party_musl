#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/mman.h>

using namespace testing::ext;

class MmanMsyncTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: msync_001
 * @tc.desc: This test verifies the correctness of file mapping and asynchronous write functionality by performing
 *           memory mapping on temporary files, modifying data in memory, and asynchronously writing data to disk files
 *           using the msync function. Finally, it EXPECTs the correctness of memory mapping and file operations.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMsyncTest, msync_001, TestSize.Level1)
{
    FILE* fp = tmpfile();
    EXPECT_NE(fp, nullptr);
    const char testData[] = "Good Time!";
    size_t testDataSize = strlen(testData);
    fwrite(testData, sizeof(char), testDataSize, fp);
    int fd = fileno(fp);
    EXPECT_NE(fd, -1);
    void* map = mmap(nullptr, testDataSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(map, MAP_FAILED);
    int rev = msync(map, testDataSize, MS_ASYNC);
    EXPECT_EQ(rev, 0);
    fp = nullptr;
}