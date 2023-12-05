#include <errno.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <stdint.h>

using namespace testing::ext;

class MallocMallocTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t LEN = 50;
constexpr int SIZE = 200;

/**
 * @tc.name: malloc_001
 * @tc.desc: This test verifies whether the behavior of the malloc function meets expectations, ensuring that the memory
 *           pointer allocated by malloc is not empty and that the allocated memory size meets the expected
 *           requirements.
 * @tc.type: FUNC
 */
HWTEST_F(MallocMallocTest, malloc_001, TestSize.Level1)
{
    void* memoryPtr = malloc(LEN);
    EXPECT_NE(memoryPtr, nullptr);
    EXPECT_EQ(malloc_usable_size(memoryPtr) >= 50U, true);
    free(memoryPtr);
}

/**
 * @tc.name: malloc_002
 * @tc.desc: This test verifies that after using the malloc function to allocate a memory block of size SIZE, the
 *           available size of the memory block can be correctly obtained to be 200 bytes.
 * @tc.type: FUNC
 */
HWTEST_F(MallocMallocTest, malloc_002, TestSize.Level1)
{
    char* block = static_cast<char*>(malloc(SIZE));
    EXPECT_NE(block, nullptr);
    EXPECT_EQ(200U, static_cast<size_t>(malloc_usable_size(block)));
}

/**
 * @tc.name: malloc_003
 * @tc.desc: This test verifies that when using the malloc function to allocate a memory block of size 0, the returned
 *           pointer is not empty and the memory block can be successfully released.
 * @tc.type: FUNC
 */
HWTEST_F(MallocMallocTest, malloc_003, TestSize.Level1)
{
    void* memoryBlock = malloc(0);
    EXPECT_NE(memoryBlock, nullptr);
    free(memoryBlock);
}