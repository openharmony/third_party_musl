## __isinff &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __isinff - Determine whether a float number is infinite.

#### **SYNOPSIS**

       #include <math.h>

       int __isinff(float x);

#### **DESCRIPTION**

​       The __isinff() shall determine whether its argument value is an infinity. 

#### **RETURN VALUE**

​       The __isinff() shall return 1 if x is positive infinity, -1 if x is negative infinity, and 0 otherwise.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __isinff() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

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

    printf("The result is : %d\n", __isinff(HUGE_VALF)); // The result is : 1
    printf("The result is : %d\n", __isinff(-HUGE_VALF)); // The result is : -1
    printf("The result is : %d\n", __isinff(NAN)); // The result is : 0
    printf("The result is : %d\n", __isinff(0)); // The result is : 0
    printf("The result is : %d\n", __isinff(f)); // The result is : 0

    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)