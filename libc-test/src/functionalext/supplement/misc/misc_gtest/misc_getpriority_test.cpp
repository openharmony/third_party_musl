#include <gtest/gtest.h>

using namespace testing::ext;

class MiscGetpriorityTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

#define MAXPRIORITY 10
#define MINPRIORITY (-10)
#define SPECIALMAXPRIORITY 19
#define SPECIALMINPRIORITY (-20)

/**
 * @tc.name: getpriority_001
 * @tc.desc: This test verifies that the getpriority function, which obtains the process priority, can execute
 *           successfully and the returned value meets the expected range, while checking for any errors.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_001, TestSize.Level1)
{
    errno = 0;
    int processPriority = getpriority(0, getpid());
    EXPECT_EQ(0, errno);
    EXPECT_TRUE(processPriority != MINPRIORITY);
    EXPECT_TRUE(processPriority <= MAXPRIORITY);
}

/**
 * @tc.name: getpriority_002
 * @tc.desc: This testing viewpoint verifies that the getpriority function, which obtains the priority of the process
 *           group, can execute successfully and the returned value meets the expected range, while checking for any
 *           errors.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_002, TestSize.Level1)
{
    errno = 0;
    int processGroupPriority = getpriority(1, getpgid(getpid()));
    EXPECT_EQ(0, errno);
    EXPECT_TRUE(processGroupPriority >= MINPRIORITY);
    EXPECT_TRUE(processGroupPriority <= MAXPRIORITY);
}

/**
 * @tc.name: getpriority_003
 * @tc.desc: This testing viewpoint verifies that the getpriority function, which obtains user priority, can be
 *           successfully executed and the returned value meets the expected range, while checking for any errors.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_003, TestSize.Level1)
{
    errno = 0;
    int userPriority = getpriority(2, getuid());
    EXPECT_EQ(0, errno);
    EXPECT_TRUE(userPriority >= SPECIALMINPRIORITY);
    EXPECT_TRUE(userPriority <= SPECIALMAXPRIORITY);
}