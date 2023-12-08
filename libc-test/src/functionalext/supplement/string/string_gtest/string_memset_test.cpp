#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t SET_LEN = 5;

class StringMemsetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memset_001
 * @tc.desc: Verify the behavior of the memset interface when the length is zero, ensuring that the original string is
 *           not modified.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemsetTest, memset_001, TestSize.Level1)
{
    char str[] = "hello, world";
    memset(str, '!', SET_LEN);
    EXPECT_STREQ(str, "!!!!!, world");
}