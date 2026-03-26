## DLPRELINK_RECORD &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       dlprelink_record - prelink a list of DSOs and record their relocations for reuse

#### **SYNOPSIS**

       int dlprelink_record(int memfd, const char *list_path);

#### **DESCRIPTION**

The function `dlprelink_record()` prelinks a list of DSOs specified in `list_path` and records their relocations in `memfd` for reuse. The list should contain one DSO path in each line. The `memfd` should be created using `memfd_create()` with the name `relro_cache`.

Only DSOs in the system internal namespaces are permitted to call this function.

#### **RETURN VALUE**

On success, the function returns 0. On error, the function returns -1 and errors can be diagnosed using `dlerror()`.

#### EXAMPLES

Refer to [libc-test/src/common/dlprelink.c](https://gitcode.com/openharmony/third_party_musl/blob/master/libc-test/src/common/dlprelink.c).

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at [docs](https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
