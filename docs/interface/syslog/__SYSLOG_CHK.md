## __SYSLOG_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __syslog_chk - write message to system logger with buffer overflow protection.

#### **SYNOPSIS**

       #include <syslog.h>

       void __syslog_chk(int priority, int flags, const char* message, ...);

#### **DESCRIPTION**

​       The __syslog_chk() function is a fortified version of the syslog() function.

​       By default, syslog() (and thus __syslog_chk()) writes log messages to "/dev/log". If writing to "/dev/log" fails, it may fall back to writing to "/dev/console" depending on the system configuration and error conditions.

#### **PARAMETERS**

​       **priority**: The priority level of the message, which is a combination of
​                    a facility value and a level value. The valid range depends on
​                    the system implementation.

​       **flags**:    Fortify protection flags. If flags is set to 0, verification is not enabled; if greater than 0, verification is enabled.

​       **message**:  The format string for the message, similar to printf() format.

​       **...**:      Variable arguments corresponding to the format specifiers
​                    in the message string.

#### **RETURN VALUE**

​       The __syslog_chk() function does not return a value.

#### **ERRORS**

​       If the message pointer is NULL, the function will set errno to EINVAL and return immediately without logging the message.

#### **ATTRIBUTES**

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __syslog_chk()          | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

 #### NOTES
 	 
 ​       This feature is designed specifically for when musl_extended_function is true.
 
 ​       **Recommendation for OpenHarmony Developers**: For better integration with the OpenHarmony ecosystem and more comprehensive logging capabilities, it is recommended to use the `hilog` API instead of directly calling `syslog()` or `__syslog_chk()`. The `hilog` API provides a more unified and powerful logging mechanism that is optimized for OpenHarmony devices.
     
#### **CONFORMING TO**

​       This is a platform-specific extension and is not part of any POSIX standard.

#### **EXAMPLES**

```c
#include <syslog.h>

int main(void) {
    // Example 1: flags=0 (no fortify protection)
    const char* module = "network";
    int error_code = 1001;
    __syslog_chk(LOG_ERR, 0, "Error in %s module: code %d", module, error_code);
    
    // Example 2: flags=2 (fortify protection enabled, expected: validation passes)
    const char* service = "webserver";
    int port = 8080;
    __syslog_chk(LOG_INFO, 2, "Service %s started on port %d", service, port);
    
    // Example 3: flags=2 with invalid parameters (expected: Musl Fortify runtime error: invalid specified parameter)
     __syslog_chk(LOG_INFO, 2, "%3$d", 0);
    
    return 0;
}
```

#### **COLOPHON**

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
