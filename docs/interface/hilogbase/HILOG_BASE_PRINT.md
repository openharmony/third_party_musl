## HILOG_BASE_PRINT &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   


#### **NAME**

​       HiLogBasePrint - prints logs.

#### **SYNOPSIS**

​       #include <log_base.h>

       int HiLogBasePrint(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...);

#### **DESCRIPTION**

​       Outputs logs of the specified **type**, **level**, **domain**, **tag**, and variables determined by the format specifier and privacy identifier in the printf format.

**Parameters**

| Name| Description| 
| -------- | -------- |
| type | Log type. The type for third-party applications is defined by **LOG_APP**. | 
| level | Log level. The value can be **LOG_DEBUG**, **LOG_INFO**, **LOG_WARN**, **LOG_ERROR**, and **LOG_FATAL**. | 
| domain | Service domain. Its value is a hexadecimal integer ranging from 0x0 to 0xFFFF. If the value exceeds the range, logs cannot be printed. | 
| tag | Log tag, which is a string used to identify the class, file, or service. A tag can contain a maximum of 31 bytes. If a tag exceeds this limit, it will be truncated. Chinese characters are not recommended because garbled characters or alignment problems may occur.| 
| fmt | Format string, which is an enhancement of a printf format string and supports the privacy identifier. Specifically, **{public}** or **{private}** is added between the % character and the format specifier in each parameter. | 
| ... | Parameter list corresponding to the parameter type in the format string. The number and type of parameters must be mapped onto the identifier in the format string. | 

​       Note: This function is MT-Safe(multi-thread safe) and signal-safe.

#### **RETURN VALUE**

**0** or a larger value if the operation is successful; a value smaller than **0** otherwise.
Possible failure causes: The **LogLevel** passed in is lower than the allowed log level; the **domain** is out of range; the **tag** is a null pointer; the CPU is overloaded; the memory is insufficient; the number of logs on the device is too large.

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
    int result = HiLogBasePrint(LOG_APP, LOG_INFO, 0x0001, "testTag", "%{public}s World %{private}d", "Hello", 3);
    if (result >= 0) {
       printf("Prints hilog success!");
    } else {
       printf("Prints hilog failed!");
    }
    return 0;
}
```

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)