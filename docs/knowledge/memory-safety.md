# 内存安全知识

本文记录 musl 内存安全相关机制的框架设计、专有名词和修改约束。覆盖四个子系统：
malloc 分配器、fortify 缓冲区溢出保护、GWP-ASan、fdsan 文件描述符检测。

## 设计目的

musl 作为 C 标准库实现，其内存安全和分配器是系统稳定性的基石。四个子系统各自解决不同层面的问题：

- **malloc 分配器**：控制堆内存的分配/释放策略，影响全局性能和内存碎片
- **fortify**：在 _FORTIFY_SOURCE 宏启用的编译场景下，运行时检测缓冲区溢出，阻止越界写攻击
- **GWP-ASan**：通过采样机制，以极低开销检测堆内存的 use-after-free、buffer overflow/underflow、double-free
- **fdsan**：检测文件描述符的 double-close 和 use-after-close，定位 fd 管理 bug

## 框架设计

### malloc：三种分配器

| 分配器 | 目录 | 来源 | 选择方式 |
|---|---|---|---|
| mallocng | `src/malloc/mallocng/` | 上游 musl 默认 | `--with-malloc=mallocng`（默认） |
| oldmalloc | `src/malloc/oldmalloc/` | musl 经典 dlmalloc 风格 | `--with-malloc=oldmalloc` |
| jemalloc | `src/malloc/jemalloc/linux/` | 外部 jemalloc 集成 | 编译时 `USE_JEMALLOC` 宏 |

**设计约束**：支持 LD_PRELOAD 替换（`replaced.c` 跟踪 `__malloc_replaced`），替换后 libc 内部也走替换后的分配器。

### fortify：运行时 _chk 函数

编译时通过 `_FORTIFY_SOURCE` 宏启用，头文件中的内联函数在编译期计算缓冲区大小，运行时调用 `__xxx_chk` 函数校验。校验失败调用 `__fortify_error()` → abort。

覆盖约 40 个函数：open/openat/poll/recvfrom/sendto/read/write/gets/fgets/snprintf/memcpy/memset/strcpy/strcat/strlen 等。

上游 musl 无此机制。

### GWP-ASan：采样型堆检测

以概率 1/2500 将 malloc 的分配重定向到带 guard page 的独立内存池。检测到错误时记录 backtrace + metadata，可选 abort 或恢复。

### fdsan：fd 所有权追踪

给每个 fd 打上 64-bit owner tag（close_tag），close 时校验 tag 匹配。不匹配说明是 double-close 或 use-after-close。

## 专有名词

| 名词 | 定义 |
|---|---|
| **mallocng** | musl 新版元数据分配器，基于 struct meta/group 的 slab 分配 |
| **oldmalloc** | musl 经典 dlmalloc 风格分配器，chunk header + free list |
| **jemalloc** | 第三方高性能分配器，通过 wrapper 集成 |
| **MALLOC_FREELIST_HARDENED** | mallocng 的 free list 指针加密，防 freelist 攻击 |
| **_FORTIFY_SOURCE** | 编译宏，启用后编译器插入 _chk 调用 |
| **__fortify_error** | fortify 校验失败时的统一处理函数（stderr + abort） |
| **GWP-ASan** (Guard Page Allocator) | 采样型堆错误检测器，分配带 guard page 的内存 |
| **guard page** | GWP-ASan 中分配块两侧的不可访问页，越界访问触发 SIGSEGV |
| **sample rate** | GWP-ASan 采样频率，默认 1/2500 |
| **fdsan** | File Descriptor Sanitizer，fd 所有权追踪 |
| **close_tag** | fdsan 给每个 fd 打的 64-bit owner 标签 |
| **FdTable** | fdsan 的 fd→tag 映射表，128 槽 + 溢出表 |

## 关键机制

| 机制 | 位置 | 说明 |
|---|---|---|
| mallocng 核心 | `src/malloc/mallocng/` | malloc/free/realloc/aligned_alloc |
| oldmalloc 核心 | `src/malloc/oldmalloc/` | 经典分配器，含 LiteOS debug 扩展 |
| jemalloc wrapper | `src/malloc/jemalloc/linux/` | USE_JEMALLOC 编译开关 |
| malloc hook 层 | `src/hook/linux/malloc_common.c` | 可插拔分配器调度，GWP-ASan 集成点 |
| LD_PRELOAD 替换追踪 | `src/malloc/replaced.c` | __malloc_replaced 标记 |
| fortify _chk 实现 | `src/fortify/linux/fortify.c` (~40个函数) | 运行时 buffer size 校验 |
| fortify 头文件宏 | `porting/linux/user/include/fortify/fortify.h` | __FORTIFY_COMPILATION/RUNTIME |
| GWP-ASan 核心 | `src/gwp_asan/linux/gwp_asan.c` | 采样分配、guard page 管理、错误报告 |
| GWP-ASan 参数控制 | `musl.debug.gwp_asan` sys_param | 运行时配置采样率和最大并发分配数 |
| fdsan 核心 | `src/fdsan/linux/fdsan.c` | fd tag 管理、owner 校验、错误分级 |

## 约束

### malloc
- **不可破坏 LD_PRELOAD 替换路径**：`__malloc_replaced` 标记后，libc 内部分配必须走替换后的分配器
- **不可修改 malloc/free/realloc 等公共 API 的行为语义**
- **分配器切换时必须通过 `--with-malloc=` 或编译宏**，不可在运行时切换
- **jemalloc 集成有 DFX 接口依赖**：`USE_JEMALLOC_DFX_INTF` 控制 mallinfo 等统计接口

### fortify
- **_chk 函数签名不可随意修改**：和头文件中的内联函数签名严格绑定
- **不可降低校验强度**：减少检查项 = 引入安全漏洞
- **不可在 _chk 函数内调用可能触发 fortify 的函数**：防止递归校验

### GWP-ASan
- **非采样分配必须零开销**：采样逻辑只在命中 sample rate 时触发
- **fork 安全**：子进程必须正确处理继承的 GWP-ASan 状态
- **signal handler 必须 async-signal-safe**：错误报告路径在信号上下文中执行
- **采样率不可默认设为 0 或全量**：0 关闭检测，全量严重影响性能

### fdsan
- **不可破坏正常 fd 语义**：close/dup/dup2 行为必须和标准一致
- **close 路径开销必须极低**：fd close 是高频操作
