#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/acosf_data.h"
#include "math_test_data/acos_data.h"

using namespace testing::ext;

class MathAcosTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: acos_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acos interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acosData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_acosData[i].expected, acos(g_acosData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acos_002
* @tc.desc: When the parameter of acos is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, acos(1.0));
}

/**
* @tc.name: acos_003
* @tc.desc: Test acos with boundary value -1.0
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_003, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(M_PI, acos(-1.0));
}

/**
* @tc.name: acos_004
* @tc.desc: Test acos with zero input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_004, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(M_PI_2, acos(0.0));
}

/**
* @tc.name: acos_005
* @tc.desc: Test acos with positive 0.5 input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_005, TestSize.Level1)
{
    EXPECT_NEAR(M_PI_3, acos(0.5), 1e-15);
}

/**
* @tc.name: acos_006
* @tc.desc: Test acos with negative 0.5 input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_006, TestSize.Level1)
{
    EXPECT_NEAR(2 * M_PI_3, acos(-0.5), 1e-15);
}

/**
* @tc.name: acos_007
* @tc.desc: Test acos with input greater than 1.0 (out of domain)
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_007, TestSize.Level1)
{
    errno = 0;
    double result = acos(1.1);
    EXPECT_TRUE(isnan(result));
    EXPECT_EQ(errno, EDOM);
}

/**
* @tc.name: acos_008
* @tc.desc: Test acos with input less than -1.0 (out of domain)
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_008, TestSize.Level1)
{
    errno = 0;
    double result = acos(-1.1);
    EXPECT_TRUE(isnan(result));
    EXPECT_EQ(errno, EDOM);
}

/**
* @tc.name: acos_009
* @tc.desc: Test acos with NaN input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_009, TestSize.Level1)
{
    double nanInput = NAN;
    double result = acos(nanInput);
    EXPECT_TRUE(isnan(result));
}

/**
* @tc.name: acos_010
* @tc.desc: Test acos with positive infinity input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_010, TestSize.Level1)
{
    double infInput = INFINITY;
    double result = acos(infInput);
    EXPECT_TRUE(isnan(result));
}

/**
* @tc.name: acos_011
* @tc.desc: Test acos with negative infinity input
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_011, TestSize.Level1)
{
    double negInfInput = -INFINITY;
    double result = acos(negInfInput);
    EXPECT_TRUE(isnan(result));
}

/**
* @tc.name: acos_012
* @tc.desc: Test acos with multiple precision values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_012, TestSize.Level1)
{
    struct TestData {
        double input;
        double expected;
    } testData[] = {
        {sqrt(2)/2, M_PI_4},       // ~0.7071 -> π/4
        {-sqrt(2)/2, 3*M_PI_4},    // ~-0.7071 -> 3π/4
        {sqrt(3)/2, M_PI_6},       // ~0.8660 -> π/6
        {-sqrt(3)/2, 5*M_PI_6}     // ~-0.8660 -> 5π/6
    };

    for (size_t i = 0; i < sizeof(testData)/sizeof(testData[0]); ++i) {
        EXPECT_NEAR(testData[i].expected, acos(testData[i].input), 1e-15);
    }
}

/**
* @tc.name: acosf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acosf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acosfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_acosfData[i].expected, acosf(g_acosfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acosf_002
* @tc.desc: When the parameter of acosf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, acosf(1.0f));
}

/**
* @tc.name: acosf_003
* @tc.desc: Test acosf with boundary values of valid input range [-1.0f, 1.0f]
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_003, TestSize.Level1)
{
    // Test lower boundary
    EXPECT_FLOAT_EQ(M_PI, acosf(-1.0f));
    // Test upper boundary
    EXPECT_FLOAT_EQ(0.0f, acosf(1.0f));
    // Test midpoint
    EXPECT_FLOAT_EQ(M_PI_2, acosf(0.0f));
}

/**
* @tc.name: acosf_004
* @tc.desc: Test acosf with values slightly inside the valid range
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_004, TestSize.Level1)
{
    const float epsilon = 1e-5f;

    // Just above -1.0f
    float result1 = acosf(-1.0f + epsilon);
    EXPECT_TRUE(result1 < M_PI && result1 > 0.0f);

    // Just below 1.0f
    float result2 = acosf(1.0f - epsilon);
    EXPECT_TRUE(result2 > 0.0f && result2 < M_PI);
}

/**
* @tc.name: acosf_005
* @tc.desc: Test acosf with values outside the valid input range
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_005, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);

    // Test value greater than 1.0f
    float result1 = acosf(1.1f);
    EXPECT_TRUE(isnan(result1));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
    feclearexcept(FE_ALL_EXCEPT);

    // Test value less than -1.0f
    float result2 = acosf(-1.1f);
    EXPECT_TRUE(isnan(result2));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
}

/**
* @tc.name: acosf_006
* @tc.desc: Test acosf with special floating-point values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_006, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);

    // Test NaN input
    float nanInput = NAN;
    float resultNan = acosf(nanInput);
    EXPECT_TRUE(isnan(resultNan));

    // Test +infinity
    float infInput = INFINITY;
    float resultInf = acosf(infInput);
    EXPECT_TRUE(isnan(resultInf));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
    feclearexcept(FE_ALL_EXCEPT);

    // Test -infinity
    float negInfInput = -INFINITY;
    float resultNegInf = acosf(negInfInput);
    EXPECT_TRUE(isnan(resultNegInf));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
}

/**
* @tc.name: acosf_007
* @tc.desc: Test acosf symmetry property: acosf(x) + acosf(-x) = π
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_007, TestSize.Level1)
{
    const float tolerance = 1e-5f;
    float xValues[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};

    for (float x : xValues) {
        float posResult = acosf(x);
        float negResult = acosf(-x);
        EXPECT_NEAR(posResult + negResult, M_PI, tolerance);
    }
}

/**
* @tc.name: acosf_008
* @tc.desc: Test acosf with subnormal input values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_008, TestSize.Level1)
{
    // Subnormal value (very small positive number)
    float subnormal = 1.0e-40f;
    float expected = M_PI_2 - subnormal; // Approximation for small x: acos(x) ≈ π/2 - x
    EXPECT_NEAR(acosf(subnormal), expected, 1e-35f);

    // Negative subnormal value
    float negSubnormal = -1.0e-40f;
    float negExpected = M_PI_2 + negSubnormal; // Approximation for small |x|: acos(-x) ≈ π/2 + x
    EXPECT_NEAR(acosf(negSubnormal), negExpected, 1e-35f);
}

/**
* @tc.name: acosl_001
* @tc.desc: When the parameter of acosl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, acosl(1.0L));
}

/**
* @tc.name: acosl_002
* @tc.desc: Test acosl with boundary values of valid input range [-1.0L, 1.0L]
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_002, TestSize.Level1)
{
    // Test lower boundary (-1.0L)
    EXPECT_DOUBLE_EQ(M_PIl, acosl(-1.0L));
    // Test midpoint (0.0L)
    EXPECT_DOUBLE_EQ(M_PIl_2, acosl(0.0L));
    // Test upper boundary (1.0L) is already covered in acosl_001
}

/**
* @tc.name: acosl_003
* @tc.desc: Test acosl with values slightly inside the valid range
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_003, TestSize.Level1)
{
    const long double epsilon = 1e-10L;

    // Just above -1.0L
    long double result1 = acosl(-1.0L + epsilon);
    EXPECT_TRUE(result1 < M_PIl && result1 > 0.0L);

    // Just below 1.0L
    long double result2 = acosl(1.0L - epsilon);
    EXPECT_TRUE(result2 > 0.0L && result2 < M_PIl);
}

/**
* @tc.name: acosl_004
* @tc.desc: Test acosl with values outside the valid input range
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_004, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);

    // Test value greater than 1.0L
    long double result1 = acosl(1.1L);
    EXPECT_TRUE(isnanl(result1));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
    feclearexcept(FE_ALL_EXCEPT);

    // Test value less than -1.0L
    long double result2 = acosl(-1.1L);
    EXPECT_TRUE(isnanl(result2));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
}

/**
* @tc.name: acosl_005
* @tc.desc: Test acosl with special floating-point values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_005, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);

    // Test NaN input
    long double nanInput = NAN;
    long double resultNan = acosl(nanInput);
    EXPECT_TRUE(isnanl(resultNan));

    // Test +infinity
    long double infInput = INFINITY;
    long double resultInf = acosl(infInput);
    EXPECT_TRUE(isnanl(resultInf));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
    feclearexcept(FE_ALL_EXCEPT);

    // Test -infinity
    long double negInfInput = -INFINITY;
    long double resultNegInf = acosl(negInfInput);
    EXPECT_TRUE(isnanl(resultNegInf));
    EXPECT_EQ(FE_INVALID, fetestexcept(FE_INVALID));
}

/**
* @tc.name: acosl_006
* @tc.desc: Test acosl symmetry property: acosl(x) + acosl(-x) = π
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_006, TestSize.Level1)
{
    const long double tolerance = 1e-10L;
    long double xValues[] = {0.0L, 0.25L, 0.5L, 0.75L, 1.0L};

    for (long double x : xValues) {
        long double posResult = acosl(x);
        long double negResult = acosl(-x);
        EXPECT_NEAR(posResult + negResult, M_PIl, tolerance);
    }
}

/**
* @tc.name: acosl_007
* @tc.desc: Test acosl with subnormal input values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_007, TestSize.Level1)
{
    // Subnormal value (very small positive number)
    long double subnormal = 1.0e-40L;
    long double expected = M_PIl_2 - subnormal; // Approximation for small x
    EXPECT_NEAR(acosl(subnormal), expected, 1e-35L);

    // Negative subnormal value
    long double negSubnormal = -1.0e-40L;
    long double negExpected = M_PIl_2 + negSubnormal; // Approximation for small |x|
    EXPECT_NEAR(acosl(negSubnormal), negExpected, 1e-35L);
}

/**
* @tc.name: acosl_008
* @tc.desc: Test acosl with high-precision reference values
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_008, TestSize.Level1)
{
    // High-precision expected values calculated from mathematical references
    EXPECT_NEAR(acosl(0.5L), 1.0471975511965977461542144610931676L, 1e-15L);
    EXPECT_NEAR(acosl(-0.5L), 2.0943951023931954923084289221863352L, 1e-15L);
    EXPECT_NEAR(acosl(sqrtl(2.0L)/2), 0.7853981633974483096156608458198757L, 1e-15L);
}