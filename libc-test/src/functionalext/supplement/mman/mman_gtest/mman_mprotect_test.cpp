#include <errno.h>
#include <gtest/gtest.h>
#include <sys/mman.h>

using namespace testing::ext;

class MmanMprotectTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mprotect_001
 * @tc.desc: This test verifies that the mprotect() function successfully sets the memory buffer to writable, and when
 *           the mprotect() function is later used to set the memory buffer to read-only, it returns an error and sets
 *           errno to EPERM.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMprotectTest, mprotect_001, TestSize.Level1)
{
    size_t pageSize = getpagesize();
    void* memoryBuffer = memalign(pageSize, 8 * pageSize);
    int protectResult = mprotect(memoryBuffer, pageSize, PROT_WRITE);
    EXPECT_EQ(0, protectResult);
    char* bufferPtr = (char*)memoryBuffer;
    strcpy(bufferPtr, "test");
    protectResult = mprotect(memoryBuffer, pageSize, PROT_READ);
    EXPECT_EQ(0, protectResult);
    free(memoryBuffer);
}