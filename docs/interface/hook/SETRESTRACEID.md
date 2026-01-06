## SetResTraceId &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   
 
 
 
#### **NAME**
 
    setResTraceId - set per-thread resource trace context.
 
#### **SYNOPSIS**
 
    #include <memory_trace.h>
 
	void setResTraceId(uint32_t newTraceType, uint64_t newTraceID,
	uint32_t *pOldTraceType, uint64_t *pOldTraceID);
 
#### **DESCRIPTION**
 
    This function sets the calling thread's resource tracing context (trace type and trace id),
	and returns the previous context through pOldTraceType and pOldTraceID, when using Hiprofiler
	(ref: https://gitcode.com/openharmony/docs/blob/master/zh-cn/application-dev/dfx/hiprofiler.md).
 
    newTraceType is the resource classification type, and newTraceID is a 64-bit identifier used to
	associate subsequent resource events with the same logical owner (for example, a component instance).
 
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
    pOldTraceType and pOldTraceID must be valid pointers; otherwise the function does nothing.
    newTraceType/newTraceID apply only to the calling thread.
 
#### CONFORMING TO
 
    This is a platform-specific extension and is not part of any POSIX standard.
 
#### COLOPHTON
 
    this page is part of the C library user-space interface documentation.
    Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)
