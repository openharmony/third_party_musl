## Finite-Input Math Functions    OHOS Programmer's Manual

#### **NAME**

        __log_finite, __log2_finite, __atan2_finite, __pow_finite, __log10_finite, __exp_finite, __exp2_finite, __acosf_finite, __powf_finite, __log2f_finite, exp2f_finite - Specialized math functions for finite floating-point inputs.

#### **SYNOPSIS**

        #include <math.h>

        // double-type finite-input math functions

        double __log_finite(double x);

        double __log2_finite(double x);

        double __atan2_finite(double y, double x);

        double __pow_finite(double x, double y);

        double __log10_finite(double x);

        double __exp_finite(double x);

        double __exp2_finite(double x);

        // float-type finite-input math functions

        float __acosf_finite(float x);

        float __powf_finite(float x, float y);

        float __log2f_finite(float x);

        float exp2f_finite(float x);

#### **DESCRIPTION**

        - __log_finite: Wraps log(), computes natural logarithm (base e) of finite positive x.

        - __log2_finite: Wraps log2(), computes base-2 logarithm of finite positive x.

        - __atan2_finite: Wraps atan2(), computes four-quadrant arctangent of finite y and x.

        - __pow_finite: Wraps pow(), computes x raised to the power of y for finite x and y.

        - __log10_finite: Wraps log10(), computes base-10 logarithm of finite positive x.

        - __exp_finite: Wraps exp(), computes natural exponential (e^x) of finite x.

        - __exp2_finite: Wraps exp2(), computes base-2 exponential (2^x) of finite x.

        - __acosf_finite: Wraps acosf(), computes arccosine of finite x.

        - __powf_finite: Wraps powf(), computes x raised to the power of y for finite x and y.

        - __log2f_finite: Wraps log2f(), computes base-2 logarithm of finite positive x.

        - exp2f_finite: Designed to wrap exp2f() (compute base-2 exponential of finite x), with a current implementation inconsistency.

#### **RETURN VALUE**

        __log_finite: Returns natural logarithm of finite positive x; 

        __log2_finite: Returns base-2 logarithm of finite positive x;

        __atan2_finite: Returns four-quadrant arctangent of finite y and x;

        __pow_finite: Returns x^y for finite valid x/y;

        __log10_finite: Returns base-10 logarithm of finite positive x;

        __exp_finite: Returns e^x for finite x; returns ±infinity on overflow, 0.0 on underflow;

        __exp2_finite: Returns 2^x for finite x; returns ±infinity on overflow, 0.0 on underflow;

        __acosf_finite: Returns arccosine of finite x in [-1.0f, 1.0f];

        __powf_finite: Returns x^y for finite valid x/y;

        __log2f_finite: Returns base-2 logarithm of finite positive x;

        exp2f_finite: Designed to return 2^x for finite x (current implementation inconsistent);

#### **ERRORS**

        No errors are defined.

#### ATTRIBUTES

        | Attribute     | Value       |
        | ------------- | ----------- |
        | Thread safety | NOT MT safe |
        | Signal safety | NOT Safe    |

#### HISTORY

        -- 2026

#### CONFORMING TO

        These functions are platform-specific extensions and are not part of any POSIX standard.

#### EXAMPLES

#include <math.h>
#include <stdio.h>

#include <float.h>


'''c
int main(void)

{

    // Test double-type finite-input math functions

    double d_val = 16.0;

    double d_base = 2.0;

    double d_exp = 4.0;

    double d_atan2_y = 1.0;  // Valid finite input for __atan2_finite

    double d_atan2_x = 1.0;  // Valid finite input for __atan2_finite

    double d_log10_val = 100.0;  // Valid finite input for __log10_finite

    double d_exp2_val = 4.0;  // Valid finite input for __exp2_finite

    printf("__log_finite(16.0) = %f\n", __log_finite(d_val));

    printf("__log2_finite(16.0) = %f\n", __log2_finite(d_val));

    printf("__pow_finite(2.0, 4.0) = %f\n", __pow_finite(d_base, d_exp));

    printf("__exp_finite(0.0) = %f\n", __exp_finite(0.0));

    printf("__atan2_finite(1.0, 1.0) = %f\n", __atan2_finite(d_atan2_y, d_atan2_x));

    printf("__log10_finite(100.0) = %f\n", __log10_finite(d_log10_val));

    printf("__exp2_finite(4.0) = %f\n", __exp2_finite(d_exp2_val));

    // Test float-type finite-input math functions

    float f_val = 0.5f;

    float f_x = 1.0f;

    float f_pow_base = 2.0f;  // Valid finite input for __powf_finite

    float f_pow_exp = 3.0f;   // Valid finite input for __powf_finite

    float f_exp2f_val = 3.0f; // Valid finite input for exp2f_finite (note implementation inconsistency)

    printf("__acosf_finite(1.0f) = %f\n", __acosf_finite(f_x));

    printf("__log2f_finite(0.5f) = %f\n", __log2f_finite(f_val));

    printf("__powf_finite(2.0f, 3.0f) = %f\n", __powf_finite(f_pow_base, f_pow_exp));

    printf("exp2f_finite(3.0f) = %f\n", exp2f_finite(f_exp2f_val));  

    return 0;
}
'''

#### COLOPHTON

        this page is part of the C library user-space interface documentation.
        Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)
