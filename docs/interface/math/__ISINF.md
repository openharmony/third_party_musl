## __isinf &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __isinf - Determine whether a number is infinite.

#### **SYNOPSIS**

       ​#include <math.h>

       int __isinf(double x);

#### **DESCRIPTION**

​       The __isinf() shall determine whether its argument value is an infinity. 

#### **RETURN VALUE**

​       The __isinf() shall return 1 if x is positive infinity, -1 if x is negative infinity, and 0 otherwise.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __isinf() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <float.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float f = 50.0f;

    printf("The result is : %d\n", __isinf(HUGE_VALF)); // The result is : 1
    printf("The result is : %d\n", __isinf(-HUGE_VALF)); // The result is : -1
    printf("The result is : %d\n", __isinf(NAN)); // The result is : 0
    printf("The result is : %d\n", __isinf(0)); // The result is : 0
    printf("The result is : %d\n", __isinf(f)); // The result is : 0

    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)