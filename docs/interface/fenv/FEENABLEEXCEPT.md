## feenableexcept &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
​       ​ feenableexcept - enable certain bits in FPCR register.
 
#### **SYNOPSIS**

       #include <fenv.h>

       int feenableexcept(int);

#### **DESCRIPTION**

​       This function enables specified floating-point exception traps on aarch64, meaning the processor may raise hardware-level exceptions/signals for these exceptions instead of only setting status bits silently. The exceptions include FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT. The function takes a bitmask parameter specifying the exceptions to be enabled. Bits outside FE_ALL_EXCEPT are ignored.
​       In this implementation, enabling an exception sets the corresponding FPCR exception trap bit to 1.

​       Note: 
        This function is NOT MT-Safe and NOT signal-safe. Currently, the function has aarch64 implementation only.

#### **RETURN VALUE**

​      On success, this function returns the floating-point exception trap mask that was enabled before modification. If the requested trap bits cannot be enabled, it returns -1.

#### **ERRORS**

​      This function does not set errno. If the requested trap bits cannot be enabled, it returns -1.

#### ATTRIBUTES
 
| Attribute     | Value       |
| ------------- | ----------- |
| Thread safety | NOT MT safe |
| Signal safety | NOT Safe    |

#### HISTORY

​      -- 2026

#### NOTES
 
​       Whether to enable trap mechanism is totally optional in aarch64, and the function's behavior may vary regarding the deploying platform.
 
#### CONFORMING TO
 
​      This is a platform-specific extension and is not part of any POSIX standard.
 
#### EXAMPLES

```c
#include <stdint.h>
#include <stdio.h>
#include <fenv.h>

#define FE_CUSTOM_EXCEPT 26 // 0b'11010

int main() {
    int old_fpcr, new_fpcr, ret;
    old_fpcr = feenableexcept(FE_ALL_EXCEPT);
    new_fpcr = fedisableexcept(FE_CUSTOM_EXCEPT);
    ret = fegetexcept();
    printf("Before modification, fpcr = %d; after enable, fpcr = %d; after disable, fpcr = %d.", old_fpcr, new_fpcr, ret);
    return 0;
}
```

#### COLOPHON
 
​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)