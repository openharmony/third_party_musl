## YNL &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       ynl - Bessel functions of the second kind.

#### **SYNOPSIS**
       
       #include <math.h>

       long double ynl(int n, long double x);

#### **DESCRIPTION**

​       The ynl() functions is similar to yn, are versions that take and return long double values.

#### **RETURN VALUE**

​       On success, these functions return the appropriate Bessel value of the second kind for x.

​         If x is a NaN, a NaN is returned.

​         If x is negative, a domain error occurs, and the functions return -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL, respectively (POSIX.1-2001 also allows a NaN return for this case).

​         If x is 0.0, a pole error occurs, and the functions return -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL, respectively.

​         If the result underflows, a range error occurs, and the functions return 0.0.

​         If the result overflows, a range error occurs, and the functions return -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL, respectively (POSIX.1-2001 also allows a 0.0 return for this case). 

#### **ERRORS**

​       The following error can occur:  

​         **Domain error**: x is negative
               errno is set to EDOM.  An invalid floating-point exception (FE_INVALID) is raised.

​         **Pole error**: x is 0.0
               errno is set to ERANGE and an FE_DIVBYZERO exception is raised (but see BUGS).

​         **Range error**: result underflow
               errno is set to ERANGE.  No FE_UNDERFLOW exception is returned by fetestexcept for this case.

​         **Range error**: result overflow
               errno is set to ERANGE (but see BUGS).  An overflow floating-point exception (FE_OVERFLOW) is raised.

#### ATTRIBUTES

| Interface | Attribute     | Value    |
| ----------| ------------- | -------- |
| ynl()     | Thread safety | MT-safe  |
|           | Signal safety | Not Safe |

#### HISTORY

​       -- 2026 

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​             The functions returning double conform to SVr4, 4.3BSD, POSIX.1-2001, POSIX.1-2008.  The others are nonstandard functions that also exist on the BSDs.

#### EXAMPLES

```c
#include <stdio.h>
#include <math.h>

int main(void) {
    int n = 0;
    long double x = 2.0L;
    long double ret = ynl(n, x);
    printf("Normal value: ynl(%d, %.1Lf) = %Lg\n", n, x, ret);

    n = 0;
    x = -1.0L;
    ret = ynl(n, x);
    printf("Negative  value: ynl(%d, %.1Lf) = %Lg\n", n, x, ret);
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)