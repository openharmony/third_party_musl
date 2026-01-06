## __finitef &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __finitef - Determine whether a float number is a finite value.

#### **SYNOPSIS**

       #include <math.h>

       int __finitef(float x);

#### **DESCRIPTION**

​       The __finitef() shall determine whether its argument value is a finite value. 

#### **RETURN VALUE**

​       The __finitef() shall return a non-zero value if and only if its argument is neither infinite nor a "not-a-number" (NaN) value, and 0 otherwise.

#### **ERRORS**

​    No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __finitef() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <float.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float f = 50.0f;

    printf("The result is : %d\n", __finitef(HUGE_VALF)); // The result is : 0
    printf("The result is : %d\n", __finitef(NAN)); // The result is : 0
    printf("The result is : %d\n", __finitef(0)); // The result is : 1
    printf("The result is : %d\n", __finitef(f)); // The result is : 1

    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)