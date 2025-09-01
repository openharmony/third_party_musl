#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringBcmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: bcmp_001
 * @tc.desc: Verify that the bcmp function returns the expected result of 0 when comparing two strings with identical
 *           content.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_001, TestSize.Level1)
{
    const char* str1 = "Hello";
    const char* str2 = "Hello";
    EXPECT_EQ(0, bcmp(str1, str2, strlen(str1)));
}

/**
 * @tc.name: bcmp_002
 * @tc.desc: Verify that if two strings are not equal within the given length, the return value of bcmp function is not
 * *         equal to the length of the first string.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_002, TestSize.Level1)
{
    const char* str1 = "hello";
    const char* str2 = "world";
    EXPECT_NE(bcmp(str1, str2, strlen(str1)), strlen(str1));
}

/**
 * @tc.name: bcmp_003
 * @tc.desc: Verify that if two strings are equal in the first n bytes, the return value of bcmp function is 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_003, TestSize.Level1)
{
    const char* str1 = "hello world";
    const char* str2 = "hello there";
    EXPECT_EQ(bcmp(str1, str2, 5), 0);
}

/**
 * @tc.name: bcmp_004
 * @tc.desc: Verify that if compare length is 0, the return value of bcmp function is 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_004, TestSize.Level1)
{
    const char* str1 = "hello";
    const char* str2 = "world";
    EXPECT_EQ(bcmp(str1, str2, 0), 0);
}

/**
 * @tc.name: bcmp_005
 * @tc.desc: Verify that bcmp function can correctly compare binary data.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_005, TestSize.Level1)
{
    unsigned char data1[] = {0x01, 0x02, 0x03, 0x04};
    unsigned char data2[] = {0x01, 0x02, 0x03, 0x05};
    EXPECT_NE(bcmp(data1, data2, sizeof(data1)), 0);
}

/**
 * @tc.name: bcmp_006
 * @tc.desc: Verify that bcmp function can correctly handle single character comparison.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_006, TestSize.Level1)
{
    const char* str1 = "A";
    const char* str2 = "B";
    EXPECT_NE(bcmp(str1, str2, 1), 0);
    EXPECT_EQ(bcmp(str1, str1, 1), 0);
}