## feenableexcept &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
​       ​ feenableexcept - enable certain bits in FPCR register.
 
#### **SYNOPSIS**
 
​       #include <fenv.h>
 
       int feenableexcept(int);

#### **DESCRIPTION**

​       This function enables(unmasks) specified floating-point exceptions, meaning the processor will raise hardware-level exceptions/signals for these exceptions instead of only setting status bits silently. The exceptions include FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT. The function takes a bitmask parameter specifying the exceptions to be enabled, and returns the previous FP exception mask.
​       Essentially, "enable" and "unmask" mean setting corresponding bits in exception mask to 0.

​       Note: 
        This function is NOT MT-Safe and NOT signal-safe. Currently, the function has aarch64 implementation only.

#### ATTRIBUTES
 
       | Attribute     | Value       |
       | ------------- | ----------- |
       | Thread safety | NOT MT safe |
       | Signal safety | NOT Safe    |

#### **ERRORS**

​      The following error codes may be set in errno:

​      **​EUNAVAIL**：the trap machanism is no enabled and trying to turn certain exception on is unavailable.

#### NOTES
 
​       Whether to enable trap mechanism is totally optional in aarch64, and the function's behavior may vary regarding the deploying platform.
 
#### CONFORMING TO
 
​      This is a platform-specific extension and is not part of any POSIX standard.
 
#### COLOPHTON
 
​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)