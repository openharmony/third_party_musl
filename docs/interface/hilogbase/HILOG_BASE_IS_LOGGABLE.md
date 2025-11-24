## HILOG_BASE_IS_LOGGABLE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   


#### **NAME**

​       HiLogBaseIsLoggable - checks whether the log can be printed.

#### **SYNOPSIS**

​       #include <log_base.h>

       bool HiLogBaseIsLoggable(unsigned int domain, const char *tag, LogLevel level);

#### **DESCRIPTION**

​       Checks whether logs of the specified service domain, tag, and level can be printed.

**Parameters**

| Name| Description| 
| -------- | -------- |
| domain | Service domain. Its value is a hexadecimal integer ranging from 0x0 to 0xFFFF. If the value exceeds the range, logs cannot be printed. | 
| tag | Log tag, which is a string used to identify the class, file, or service. A tag can contain a maximum of 31 bytes. If a tag exceeds this limit, it will be truncated. Chinese characters are not recommended because garbled characters or alignment problems may occur. | 
| level | Log level. The value can be **LOG_DEBUG**, **LOG_INFO**, **LOG_WARN**, **LOG_ERROR**, and **LOG_FATAL**. | 

​       Note: This function is MT-Safe(multi-thread safe) and signal-safe.

#### **RETURN VALUE**

**true** if the specified logs can be output; **false** otherwise.

#### ATTRIBUTES

| Attribute     | Value    |
| ------------- | -------- |
| Thread safety | MT-safe  |
| Signal safety | signal-safe |

#### HISTORY

​       -- 2025 

#### EXAMPLES

```c
// This testcase must be compile and execute in system-side
#include <log_base.h>
#include <stdio.h>

int main()
{
    bool isLoggble = HiLogBaseIsLoggable(0x0001, "testTag", LOG_INFO);
    if (isLoggble) {
       printf("The log can be printed when domainID is 0x0001, tag is "testTag", and LogLevel is LOG_INFO.");
    } else {
       printf("The log cannot be printed when domainID is 0x0001, tag is "testTag", and LogLevel is LOG_INFO.");
    }
    return 0;
}
```

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)