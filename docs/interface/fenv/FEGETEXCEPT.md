## fegetexcept &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
​       ​ fegetexcept - retrieve FP exception mask in FPCR register.
 
#### **SYNOPSIS**

       #include <fenv.h>

       int fegetexcept(void);
 
#### **DESCRIPTION**

​       This function retrieves current floating-point exception mask. The return value is a bitmask where each bits corresponds to a type of floating-point exception (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT). A bit set to 1 indicates the corresponding exception is masked, in which condition the processor will not raise a hardware exception but set the status bits. And a bit set to 0 indicates the exception is unmasked (the processor will raise a hardware exception/signal).

​       Note: 
        This function is NOT MT-Safe and NOT signal-safe. Currently, the function has aarch64 implementation only.

#### **RETURN VALUE**

​      This function returns current value of fpcr register.

#### **ERRORS**

​       This function does NOT yield an error.

#### ATTRIBUTES
 
| Attribute     | Value       |
| ------------- | ----------- |
| Thread safety | NOT MT safe |
| Signal safety | NOT Safe    |
 
#### HISTORY

​      -- 2026

#### NOTES

​      This function is aarch64-only, and its behavior may vary across different hardware platforms and system configurations.
​       
 
#### CONFORMING TO
 
​      This is a platform-specific extension and is not part of any POSIX standard.
 
#### EXAMPLES

```c
#include <stdint.h>
#include <stdio.h>
#include <fenv.h>

#define FE_CUSTOM_EXCEPT 26 // 0b'11010
#define FE_ALL_EXCEPT 31 // 0b'11111

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