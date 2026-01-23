# OpenHarmony musl 现状

## libc
当前libc.so符号列表：
https://gitcode.com/openharmony/third_party_musl/blob/master/libc.map.txt

## musl-libc面向应用开发者的C-API
https://gitcode.com/openharmony/interface_sdk_c/blob/master/third_party/musl/ndk_script/adapter/libc.ndk.json

## 系统侧libc新的变化
2026/1/7 <math.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__acosf_finite`: 计算有限单精度浮点数值的反余弦函数值
- 新增`__atan2_finite`: 计算有限数值的反正切函数atan2值
- 新增`__exp_finite`: 计算有限数值的自然指数函数值
- 新增`__exp2_finite`: 计算有限数值的以2为底的指数函数值
- 新增`__exp2f_finite`: 计算有限单精度浮点数值的以2为底的指数函数值
- 新增`__finite`: 判断一个浮点数是否为有限值，检查该数值是否既不是无穷大（inf）也不是NaN（非数字）
- 新增`__finitef`: 判断一个单精度浮点数float 类型是否为有限值
- 新增`__isinf`: 判断一个双精度浮点数是否为无穷大（正无穷或负无穷）
- 新增`__isinff`: 判断一个单精度浮点数float 类型是否为无穷大（正无穷或负无穷）
- 新增`__isnan`: 判断一个浮点数的值是否为NaN
- 新增`__isnanf`: 判断一个单精度浮点数是否为NaN
- 新增`__log_finite`: 计算有限数值的自然对数
- 新增`__log10_finite`: 计算有限数值的以10为底的对数
- 新增`__log2_finite`: 计算有限数值的以2为底的对数
- 新增`__log2f_finite`: 计算有限单精度浮点数值的以2为底的对数（log₂）值
- 新增`__pow_finite`: 计算有限数值的幂运算结果
- 新增`__powf_finite`: 计算有限单精度浮点数值的幂运算结果

2026/1/7 <musl_version.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`get_musl_version`: 用于获取当前系统中所使用的musl库的版本字符串

2026/1/7 <pthread.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__register_atfork`: 用于注册在fork()系统调用发生时需要执行的钩子函数（hook）
- 新增`__pthread_key_create`: 用于创建线程特定数据键的函数
- 新增`pthread_attr_getaffinity_np`: 用于获取线程属性里CPU亲和性的函数
- 新增`pthread_attr_setaffinity_np`: 用于设置线程属性里CPU亲和性的函数
- 新增`pthread_attr_extension_init`: 线程扩展属性初始化
- 新增`pthread_attr_extension_destroy`: 销毁线程扩展属性分配的内存
- 新增`set_pthread_extended_function_policy`: 设置是否启用线程亲和性系列接口标记
- 新增`get_pthread_extended_function_policy`: 获取是否启用线程亲和性系列接口标记

2026/1/7 <setjmp.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__longjmp_chk`: 进行参数检查的longjmp函数

2026/1/7 <stdio.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__asprintf_chk`: 进行参数检查的asprintf函数
- 新增`__fprintf_chk`: 进行参数检查的fprintf函数
- 新增`__printf_chk`: 进行参数检查的printf函数
- 新增`__vasprintf_chk`: 进行参数检查的vasprintf函数
- 新增`__vfprintf_chk`: 进行参数检查的vfprintf函数

2026/1/7 <stdlib.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__realpath_chk`: 进行参数检查的realpath函数
- 新增`strtoll_l`: 将字符串转换为long long值
- 新增`strtoull_l`: 将字符串转换为unsigned long long数值

2026/1/7 <string.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__rawmemchr`: 用于在内存中搜索一个特定字节的首次出现位置
- 新增`__strdup`: 创建一个新的字符串，这个字符串是原始字符串的副本，并为这个副本分配新的内存空间
- 新增`__strndup`: 用于安全地复制一个字符串的前若干个字符，并自动在末尾添加空字符以构成合法C字符串
- 新增`__strtok_r`: 可重入（线程安全）的字符串分割函数

2026/1/7 <syslog.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__syslog_chk`: 进行参数检查的syslog函数

2026/1/7 <sys/auxv.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__getauxval`: 用于获取ELF辅助向量中的条目

2026/1/7 <sys/socket.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__cmsg_nxthdr`: 获取下一个控制消息头指针

2026/1/7 <time.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`timelocal`: 使用timer的值来填充tm结构，timer的值被分解为tm结构，并用本地时区表示

2026/1/7 <unistd.h>、<fcntl.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`fcntl64`: 用于执行文件控制操作的系统调用封装函数

2026/1/7 <wchar.h>  
新增以下接口专为musl_extended_function为true设计使用：
- 新增`__mbrlen`: 计算多字节字符的长度，同时可跟踪转换状态

2025/12/8 <log_base.h>
- 新增`HiLogBasePrint`: HilogBase日志打印接口
- 新增`HiLogBaseIsLoggable`: 在打印日志前调用该接口，用于检查指定领域标识、日志标识和级别的日志是否可以打印

2025/11/20
- 新增`dlopen_config_abs_path_policy`: 控制dlopen过程中是否针对入参进行绝对路径的校验

2025/11/4 <dlfcn.h>
- 新增`dlns_set_ld_permitted_path`: 在指定的namespace中设置ld_path的父目录准许路径，该接口存在权限管控
- 新增`dlns_get_plugin_default_permitted_path`: 在特定的namespace中获取ld_path的父目录准许路径
- 新增`dlns_add_plugin_default_ld_dictionary`: 在特定的namespace中添加满足特定规则的lib_paths路径

2025/04/24 <malloc.h>
- 新增`malloc_check_from_ptr`: 判断一个指针指向的内存地址是否由标准C库的内存分配器分配（since API 19）

2023/12/2 <unistd.h>
- `getproctid`: 获取当前线程的全局id，即从init命名空间看到的id

2023/11/22 <unistd.h>
- `getprocpid`: 获取当前进程的全局id，即从init命名空间看到的id

2023/11/20 <netdb.h>

这些函数提供了在网络编程中操作和管理预定义主机设置的功能。通过这些函数，可以清除、移除、设置单个主机或多个主机的预定义主机设置
- `predefined_host_clear_all_hosts`: 清除所有预定义主机的设置
- `predefined_host_remove_host`: 移除指定的预定义主机设置
- `predefined_host_set_host`: 设置指定主机的预定义主机设置
- `predefined_host_set_hosts`: 设置多个主机的预定义主机设置
- `predefined_host_lookup_ip`: 查询指定主机的预定义主机设置
- `removednsresolvehook`: 移除 DNS 解析钩子(hook)
- `setdnsresolvehook`: 设置 DNS 解析钩子(hook)，允许在网络解析过程中插入自定义逻辑来拦截和修改 DNS 解析请求

2023/11/20 <dlfcn.h>
- `dlns_set_namespace_permitted_paths`: 设置动态链接命名空间的允许路径，指定在哪些路径下搜索动态链接库
- `dlns_set_namespace_separated`: 设置动态链接命名空间的分隔标志，用于指定在动态链接库搜索路径中应用不同的分隔符规则

2023/11/15 <gwp_asan.h>
- `libc_gwp_asan_unwind_fast`: 堆栈展开
- `libc_gwp_asan_has_free_mem`: 检查 GWP-ASan 堆栈检测器是否有可用的空闲内存
- `libc_gwp_asan_ptr_is_mine`: 检查指定的指针是否由 GWP-ASan 堆栈检测器所分配
- `may_init_gwp_asan`: 在需要时初始化 GWP-ASan 堆栈检测器


2023/10/18 <execinfo.h>
- `backtrace`: 获取堆栈地址列表
- `backtrace_symbols`: 通过堆栈列表获取对应的符号
- `backtrace_symbols_fd`: 通过堆栈列表获取对应的符号的同时将符号写入fd中
- `backtrace_symbols_fmt`: 从`backtrace`函数获取的函数调用栈信息转换为格式化的可读字符串数组
- `backtrace_symbols_fd_fmt`: 从`backtrace`函数获取的函数调用栈信息格式化输出到指定的文件描述符中。
- `arc4random`: 生成伪随机数
- `arc4random_buf`: 生成指定长度的随机字节序列，并将其存储在指定的缓冲区中
- `arc4random_uniform`: 生成一个在指定范围内的随机整数




2023/10/26 <netdb.h>
- `getaddrinfo_ext`: 扩展版本的 getaddrinfo 函数，用于获取指定主机名或服务名的网络地址信息

2023/5/29
- 隐藏`InitParameterClient`、`InitSharedMem`、`SystemReadParam`、`SystemFindParameter`、`SystemGetParameterCommitId`、`SystemGetParameterValue`、`GetSystemCommitId`、`TestParameterReaderPerformance`符号

2023/5/9
- libc.map.txt规范化修改，详见 https://gitee.com/openharmony/third_party_musl/pulls/895/files

2023/5/4
- fortify接口变更
https://gitee.com/openharmony/third_party_musl/pulls/886

2023/4/26
- 新增`SetThreadInfoCallback`符号

2023/3/14
- 新增`add_special_handler_at_last`和`remove_all_special_handler`符号

2023/2/17
- 隐藏`__mem_trace`符号，新增`memtrace`符号

2023/2/18
- 隐藏`__mem_typeset`符号，新增`__mem_trace`符号

2023/1/13
- 新增`add_special_signal_handler`和`remove_special_signal_handler`符号

2022/12/13
- 新增`__lldb_mmap`符号

2022/11/11
- libc去除符号版本信息

2022/10/27
- 去掉隐藏符号`pthread_cancel`

2022/10/12
- 去掉隐藏符号`pthread_setcancelstate`



## 应用开发者目前还不能使用的C-API
下列所展示的函数当前在应用开发中不能使用，在不同权限等级下可能会不一样

### 内核拦截
下列接口目前由于内核拦截还不能使用
- shmget
- shmat
- shmdt
- shmctl
- semget
- semctl
- semop
- semtimedop
- name_to_handle_at
- open_by_handle_at
- seteuid
- setegid

下列接口目前由于`seccomp`还不能使用，`seccomp`会限制系统调用的访问权限
- setfsgid
- setfsuid
- setuid
- setgid
- setreuid
- setregid
- setresgid
- sethostname
- setgroups
- setdomainname
- setxattr
- clock_settime
- chroot
- quotactl
- mknod
- mknodat

### 应用开发功能限制
- <shm.h> 共享内存: 在进程退出时不能被回收，有安全风险
    - shmget: 用于创建或获取共享内存段的标识符。
    - shmat: 用于将共享内存段附加到进程的地址空间中。
    - shmdt: 用于将共享内存段从进程的地址空间中分离。
    - shmctl: 用于对共享内存段进行控制操作，如获取/设置共享内存的状态信息、删除共享内存段等。
- <sem.h> 共享内存: 在进程退出时不能被回收，有安全风险
    - semget: 用于创建或获取信号量集的标识符。
    - semctl: 用于对信号量集进行控制操作，如获取/设置信号量的状态信息、删除信号量集等。
    - semop: 用于对信号量进行原子化操作，如增加或减少信号量的值。
    - semtimedop: 用于在指定的时间范围内进行对信号量的原子化操作，支持超时功能。
- <fsuid.h> 文件系统用户标识和文件系统组标识: 会导致进程获取不该获取的权限，导致系统具有安全风险
    - setfsgid: 用于设置进程的文件系统组标识（File System Group Identifier）。
    - setfsuid: 用于设置进程的文件系统用户标识（File System User Identifier）。
- 设置关于进程、文件系统的用户、组标识: 会导致进程获取不该获取的权限，导致系统具有安全风险
    - setuid、setgid: 用于设置进程的实际用户标识和实际组标识，以及有效用户标识和有效组标识。这些标识决定了进程对系统资源的访问权限。
    - seteuid、setegid: 用于设置进程的有效用户标识和有效组标识。这些标识用于权限检查和访问控制。
    - setreuid、setregid: 用于设置进程的实际用户标识和有效用户标识，或实际组标识和有效组标识。这些函数可以将实际标识和有效标识设置为不同的值。
    - setfsgid、setfsuid: 用于设置进程的文件系统组标识和文件系统用户标识。这些标识用于对文件系统资源的访问控制。
    - setresgid: 用于设置进程的实际组标识、有效组标识和保存的组标识。
    - setresgid: 用于同时设置进程的实际用户标识、有效用户标识和保存的用户标识。
    - setgroups: 用于设置进程的附加组标识列表。
- 设置系统的主机名和域名: 修改主机名和域名，具有安全风险
    - sethostname: 用于设置主机名。
    - setdomainname: 用于设置域名。
- 用于设置文件或目录的扩展属性: 写入文件系统的属性，具有安全风险
    - setxattr: 用于设置文件扩展属性。
- 文件句柄操作: 访问任意文件
    - name_to_handle_at: 用于将指定路径的文件名转换为持有文件句柄（file handle）的文件描述符。
    - open_by_handle_at: 用于根据给定的文件句柄（file handle）打开文件，并返回相应的文件描述符。
- 系统时钟: 修改系统时间
    - clock_settime: 用于设置指定的系统时钟（clock）的时间值。
- 进程根路径: 当前进程访问到不该访问的文件路径
    - chroot: 用于设置当前进程执行的根路径。
- <quota.h> 管理磁盘配额相关函数
    - quotactl: 用于管理磁盘配额（disk quota）的相关操作，包括获取、设置和检查磁盘配额信息。
- <stat.h> 
    - mknod: 用于创建一个特殊文件节点（special file node），可以用于创建设备文件、管道文件等。
    - mknodat: 类似于 mknod 的系统调用函数，但相对于指定的文件描述符所在的目录进行操作，可以更灵活地控制文件的创建位置。

### 不同设备形态可能会导致内核特性的使能方式有所不同
这些接口需要 /dev/shm 路径
影响：将会无法使用命名信号量的相关功能
- sem_open 用于创建或打开一个具有指定名称的命名信号量。
- sem_close 用于关闭一个已打开的信号量。
- sem_unlink 用于从系统中删除一个命名信号量。

这些接口需要内核使能 `FANOTIFY` 特性
影响：无法使用 fanotify 机制进行文件系统事件的监控和规则添加
- fanotify_init 用于初始化一个 fanotify 实例，用于监控文件系统事件。
- fanotify_mark 用于向 fanotify 实例添加监控规则，以便捕获特定文件或目录的事件。

### SELinux 限制
这些接口需要特殊的设备文件，比如/dev/ptmx，但是当前这些特殊的设备文件由于SELinux的限制，无法打开
影响：使用伪终端相关功能
- forkpty 函数在创建子进程的同时创建一个伪终端（pty）并建立父子进程之间的连接。
- ptsname 函数用于获取伪终端（pty）的设备文件名。
- ptsname_r 函数用于线程安全地获取伪终端（pty）的设备文件名。
- openpty 函数创建一个新的伪终端（pty）对，并返回其主从设备文件描述符。
- posix_openpt 函数打开一个伪终端（pty）设备，并返回其文件描述符。
- unlockpt 函数解锁伪终端（pty）的主设备文件，使其可用于打开。

### 空实现或默认失败的函数
这些接口通常用于登录日志管理和用户会话跟踪等系统级任务。它们提供了对 utmp 文件的访问和操作。在musl中这些函数功能并未实现，在OpenHarmony中也并未向开发者提供
- setutent: 将文件指针重置到 utmp 文件的开头，以便重新开始读取 utmp 文件的记录
- pututline: 将一个 struct utmp 结构体的内容写入 utmp 文件，用于记录用户登录和注销等系统事件
- getutent: 从 utmp 文件中读取下一个记录，并返回一个指向 struct utmp 结构体的指针，以便获取记录中的信息
- utmpname: 设置要在其中查找 utmp 文件的路径名。通过调用 utmpname 函数，可以更改默认的 utmp 文件路径
