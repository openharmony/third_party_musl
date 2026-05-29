# 信号链与钩子知识

本文记录信号链（sigchain）、用户态钩子（hook/RESTRACE）和系统调用钩子（syscall_hooks）的框架设计、专有名词和修改约束。
三个子系统形成从用户态到内核的三层拦截栈。

## 设计目的

OpenHarmony 需要在标准 C 库行为之上叠加系统级监控和调试能力，但不能修改应用代码。三层拦截各司其职：

- **信号链**：确保 OH 系统信号处理器（crash 上报、内存泄漏检测、watchdog）在应用注册的 handler 之前执行，且应用无法绕过
- **用户态钩子（hook/RESTRACE）**：通过可插拔的 dispatch table，在 malloc/free/mmap/socket 等高频函数调用点注入监控逻辑（内存追踪、fd 泄漏检测、Hiprofiler 资源追踪）
- **系统调用钩子**：在 svc 指令边界拦截系统调用，用于全系统级别的低层拦截

三层从低到高：syscall_hooks（内核边界）→ sigchain（信号层）→ hook（libc 函数调用层）。

全部为 OH 新增，上游 musl 无。

## 框架设计

### sigchain：信号处理器链

- 通过 `add_special_signal_handler()` 注册 OH 特殊 handler，存储在 `sc_signal_chain` 数组（每信号最多 4 个）
- 内核级 handler 是单一的 `signal_chain_handler` 多路复用器：先执行特殊 handler，再执行用户 handler
- `intercept_pthread_sigmask()` 阻止应用屏蔽 sigchain 管理的信号
- `sigaction(SIG_DFL)` 不会移除链：通过 `SYS_rt_tgsigqueueinfo` 重新发起

### hook：可插拔 dispatch table

- `MallocDispatchType` 结构包含 17 个函数指针（malloc/free/calloc/realloc/mmap 等），通过原子指针切换
- 三级标志检查：全局 hook 标志 → 进程级 hook 标志 → 自定义 hook 标志，均为原子操作
- dispatch table 为 NULL 时直接调用原始函数，对非 hook 进程零开销
- GWP-ASan 和 malloc-opt 通过 `#ifdef` 与 hook table 共存
- `restrace()` 是 Hiprofiler 注入点，在资源分配事件（fd/thread/GPU/DMA-BUF）触发栈回溯

### syscall_hooks：系统调用拦截

- `set_syscall_hooks()` 安装 1024 条目的 syscall 号→替换 handler 映射表
- 必须在任何子线程创建前安装（检查 `!libc.threads_minus_1`），否则返回 -EPERM

## 专有名词

| 名词 | 定义 | 层级 |
|---|---|---|
| **sigchain** | 信号处理器链，OH 特殊 handler + 用户 handler 的优先级多路复用 | 信号层 |
| **special signal handler** | OH 系统级信号处理器（crash/leak/watchdog），优先于用户 handler 执行 | 信号层 |
| **signal_chain_handler** | 内核级单一多路复用器，遍历执行特殊 handler + 用户 handler | 信号层 |
| **RESTRACE** | 资源追踪钩子机制，在 malloc/socket 等分配点注入 Hiprofiler 回调 | 函数调用层 |
| **MallocDispatchType** | 可插拔分配器 dispatch table，17 个函数指针 | 函数调用层 |
| **HOOK_ENABLE** | 编译宏，开启用户态 hook 机制 | 函数调用层 |
| **restrace** / **memtrace** | Hiprofiler 注入的资源追踪/内存追踪回调 | 函数调用层 |
| **OHOS_FDTRACK_HOOK_ENABLE** | 编译宏，开启 fd 泄漏检测 | 函数调用层 |
| **syscall_hooks** | svc 指令级系统调用拦截 | 内核边界 |
| **svc0_entry** | syscall_hooks 安装的系统调用入口蹦床 | 内核边界 |

## 关键机制

| 机制 | 位置 | 说明 |
|---|---|---|
| 信号链核心 | `src/sigchain/linux/sigchain.c` (618行) | 处理器注册/多路复用/拦截 sigmask |
| hook 初始化 | `src/hook/linux/musl_preinit.c` (850行) | dispatch table 初始化、SO hook 加载 |
| malloc hook 调度 | `src/hook/linux/malloc_common.c` (233行) | malloc/free 等函数的 dispatch 分发 |
| 资源追踪 | `src/hook/linux/memory_trace.c` (220行) | memtrace/restrace 回调注入 |
| fd 泄漏检测 | `src/hook/linux/musl_fdtrack.c` (191行) | fd 追踪 hook |
| socket hook | `src/hook/linux/socket_common.c` | socket fd 创建追踪 |
| dispatch 类型定义 | `src/hook/linux/musl_malloc_dispatch.h` (96行) | MallocDispatchType 结构 |
| syscall hooks | `src/syscall_hooks/linux/syscall_hooks.c` (83行) | set_syscall_hooks 安装 |
| 构建开关 | `musl_template.gni` — `soft_hook_config` | HOOK_ENABLE/OHOS_SOCKET_HOOK_ENABLE/OHOS_FDTRACK_HOOK_ENABLE |

## 约束

### sigchain
- **信号 35（freeze/DFX）和 38（watchdog/debug）不可被阻塞或移除**
- **sigchain 必须在 sigaction(SIG_DFL) 后仍存活**，通过重新发起机制保证
- **pthread_sigmask 在 handler 内部被拦截**，防止死锁
### hook
- **dispatch table 为 NULL 时不能有额外开销**，非 hook 进程不受影响
- **三级标志检查必须是原子的**，防止竞态条件
- **GWP-ASan 和 hook 共存**：编译开关顺序必须正确

### syscall_hooks
- **必须在所有子线程创建前安装**，否则返回 -EPERM
- **HIJACK_SYSCALL_MAX = 1024**，不可超出
- **安装过程持有 __tl_lock 并阻塞应用信号**，防止竞态
