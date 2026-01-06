## __longjmp_chk &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __longjmp_chk - Core functions for implementing Non-local jumps.

#### **SYNOPSIS**

       #include <setjmp.h>

       void __longjmp_chk(jmp_buf env, int val)

#### **DESCRIPTION**

​       The __longjmp_chk() function is equivalent to the longjmp() function.

#### **RETURN VALUE**

​       The __longjmp_chk() functions do not return.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __longjmp_chk() | Thread safety | MT-Safe |
|                         | Signal safety | MT-Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <setjmp.h>
#include <stdio.h>

void handle(void)
{
    printf("failed, this function should be unreachable\n"); // Cannot print
}

int main(void)
{
    jmp_buf jb;
    int value = setjmp(jb);
    if (value == 0) {
        __longjmp_chk(jb, 456);
        handle();
    } else {
        if (value != 456) {
            printf("__longjmp_chk failed\n"); // Cannot print
        }
    }

    return 0;
}
```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)