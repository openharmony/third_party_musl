## GetResTraceId &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
    getResTraceId - get per-thread resource trace context.
 
#### **SYNOPSIS**
 
    #include <memory_trace.h>
 
	bool getResTraceId(uint32_t *pTraceType, uint64_t *pTraceID);
 
#### **DESCRIPTION**
 
    This function retrieves the calling thread's current resource tracing context (trace type and trace id)
	into pTraceType and pTraceID, when using Hiprofiler
	(ref: https://gitcode.com/openharmony/docs/blob/master/zh-cn/application-dev/dfx/hiprofiler.md).
 
    If pTraceType or pTraceID is NULL, this function returns false.
 
    Note:
	This function is MT-Safe(multi-thread safe) but not signal-safe.
 
#### ATTRIBUTES
 
	| Attribute     | Value    |
	| ------------- | -------- |
	| Thread safety | MT-safe  |
	| Signal safety | Not Safe |
 
#### **ERRORS**
 
    The following error codes may be set in errno:  
    **ENOSYS**: The current program does not support this interface.  
 
 
#### HISTORY
 
    -- 2025
 
#### NOTES
 
    If errno is set to ENOSYS, it indicates that nativehook is not enabled.
    pTraceType and pTraceID must be valid pointers.
    The returned trace type/id belong to the calling thread.
 
#### CONFORMING TO
 
    This is a platform-specific extension and is not part of any POSIX standard.
 
#### COLOPHTON
 
    this page is part of the C library user-space interface documentation.
    Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)