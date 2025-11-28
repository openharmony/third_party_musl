## ResTraceFreeRegion &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
​       ​ resTraceFreeRegion - memory region free operation tracing instrumentation.
 
#### **SYNOPSIS**
 
​       #include <memory_trace.h>
 
       void resTraceFreeRegion(unsigned long long mask, void* addr, size_t size);  
 
#### **DESCRIPTION**
 
​       This function traces the release of a contiguous memory region by recording the memory address range [addr, addr + size),
       when using Hiprofiler (ref: https://gitcode.com/openharmony/docs/blob/master/zh-cn/application-dev/dfx/hiprofiler.md).
 
​       Note: 
        This function is MT-Safe(multi-thread safe) but not signal-safe.
 
#### ATTRIBUTES
 
       | Attribute     | Value    |
       | ------------- | -------- |
       | Thread safety | MT-safe  |
       | Signal safety | Not Safe |
 
#### **ERRORS**
 
​       The following error codes may be set in errno:  
​         **ENOSYS**: The current program does not support this interface.  
 
 
#### HISTORY
 
​       -- 2025 
 
#### NOTES
 
​      If errno is set to ENOSYS, it indicates that nativehook is not enabled.
      The mask parameter must follow partial mask rules (refer to resource classification specifications for valid values).
      Ensure addr is a valid memory address and size matches the actual allocation size.
 
#### CONFORMING TO
 
​      This is a platform-specific extension and is not part of any POSIX standard.
 
#### COLOPHTON
 
​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)