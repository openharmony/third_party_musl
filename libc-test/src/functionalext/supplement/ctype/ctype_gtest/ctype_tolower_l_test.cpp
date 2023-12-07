#include <ctype.h>
#include <gtest/gtest.h>
#include <locale.h>
using namespace testing::ext;

class CtypeTolowerlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tolower_l_001
 * @tc.desc: Verify whether the tolower_l() function correctly transforms characters to lowercase within the
 *           specified locale by comparing the expected result with the actual output.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeTolowerlTest, tolower_l_001, TestSize.Level1)
{
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", nullptr);
    uselocale(loc);
    char str = 'A';
    char lowerCase = tolower_l(str, loc);
    EXPECT_EQ('a', lowerCase);
}