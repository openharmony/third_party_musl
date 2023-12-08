#include <gtest/gtest.h>

using namespace testing::ext;

class MiscSetpriorityTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: setpriority_001
 * @tc.desc: This test ensures that when calling the setpriority function to set the process priority, the return value
 *           is 0, indicating successful setting of the process priority.
 * @tc.type: FUNC
 */
HWTEST_F(MiscSetpriorityTest, setpriority_001, TestSize.Level1)
{
    int setResult = setpriority(0, gettid(), 1);
    EXPECT_EQ(0, setResult);
}