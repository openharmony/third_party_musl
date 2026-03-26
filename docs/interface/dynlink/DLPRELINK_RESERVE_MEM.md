## DLPRELINK_RESERVE_MEM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       dlprelink_reserve_mem - reserve virtual memory for loading prelinked DSOs

#### **SYNOPSIS**

       int dlprelink_reserve_mem(void);

#### **DESCRIPTION**

The function `dlprelink_reserve_mem()` reserves a range of virtual memory for loading prelinked DSOs. In order to reuse recorded relocations, prelinked DSO should be loaded at the same address as in the process calling `dlprelink_record()`.

Only DSOs in the system internal namespaces are permitted to call this function.

#### **RETURN VALUE**

On success, the function returns 0. On error, the function returns -1 and errors can be diagnosed using `dlerror()`.

#### EXAMPLES

Refer to [libc-test/src/common/dlprelink.c](https://gitcode.com/openharmony/third_party_musl/blob/master/libc-test/src/common/dlprelink.c).

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at [docs](https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
