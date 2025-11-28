## ResTraceMove &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
​       resTraceMove - memory move operation tracing instrumentation.
 
#### **SYNOPSIS**
 
​       #include <memory_trace.h>
 
       void resTraceMove(unsigned long long mask, void* oldAddr, void* newAddr, size_t newSize);
 
#### **DESCRIPTION**
 
​       This function is used to trace memory move operations by recording the old and new memory addresses and size.
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
      Ensure oldAddr and newAddr are valid memory addresses, and newSize matches the actual allocation size.
 
#### CONFORMING TO
 
​      This is a platform-specific extension and is not part of any POSIX standard.
 
#### COLOPHTON
 
​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)