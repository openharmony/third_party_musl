## DLPRELINK_REGISTER &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       dlprelink_register - register previously recorded DSO relocation cache

#### **SYNOPSIS**

       int dlprelink_register(int memfd);

#### **DESCRIPTION**

The function `dlprelink_register()` registers a DSO relocation cache specified in `memfd`, which is previously recorded using `dlprelink_record()`. Loading DSOs recorded in the cache afterwards will reuse relocations to save memory usage. The `memfd` should be sealed using `fcntl()` with `F_SEAL_SEAL | F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_FUTURE_WRITE` for security.

Only DSOs in the system internal namespaces are permitted to call this function.

#### **RETURN VALUE**

On success, the function returns 0. On error, the function returns -1 and errors can be diagnosed using `dlerror()`.

#### EXAMPLES

Refer to [libc-test/src/common/dlprelink.c](https://gitcode.com/openharmony/third_party_musl/blob/master/libc-test/src/common/dlprelink.c).

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at [docs](https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
