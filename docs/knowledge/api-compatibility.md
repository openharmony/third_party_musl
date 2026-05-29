# 公共 API/ABI 兼容性知识

本文记录 musl 公共 API/ABI 兼容性边界的框架设计、专有名词和修改约束。

## 设计目的

libc 是系统最底层的公共库，所有进程依赖它。API（头文件）和 ABI（符号导出）的任何不兼容变更都会导致系统级故障。维护这块边界需要两个机制协调：

- **头文件（`include/`）**：定义 API 契约——函数签名、类型、常量、errno 语义
- **符号导出表（`libc.map.txt`）**：定义 ABI 契约——哪些符号对动态链接可见

## 框架设计

### 三层边界

```
include/         ← API 层：函数签名、类型、常量（编译期契约）
libc.map.txt     ← ABI 层：符号可见性（链接期契约）
```

### 头文件组织

| 目录 | 内容 | 架构相关 |
|---|---|---|
| `include/` | 公共头文件根目录 | 否 |
| `include/sys/` | POSIX 系统头文件（stat/socket/mman/ioctl...） | 否 |
| `include/arpa/` | Internet 协议头文件（inet/nameser...） | 否 |
| `include/netinet/` | 网络协议头文件（tcp/udp/ip/icmp...） | 否 |
| `arch/generic/bits/` | 通用 bits 兜底 | 否 |
| `arch/<arch>/bits/` | 架构特定 bits（每架构独立） | 是 |

### libc.map.txt 结构

GNU ld version script（2121行），控制 libc.so 的符号可见性：

```
{ global:     ← 导出符号列表（2117个），每行 "symbol;"
    ...
  local: *;   ← 隐藏所有未列出符号
}
```

通过 GN 构建的 `-Wl,--version-script=libc.map.txt` 生效。

### MUSL_EXTERNAL_FUNCTION：PC 专有 API 控制

通过编译宏 `MUSL_EXTERNAL_FUNCTION` 控制一套**仅 PC 可见的扩展 API**，移动端/嵌入式设备不暴露。由 GN 构建标志 `musl_extended_function` 控制：

- **PC 构建**：`musl_extended_function = true` → 定义 `MUSL_EXTERNAL_FUNCTION`，扩展 API 在头文件中可见
- **移动端/嵌入式构建**：`musl_extended_function = false` → 宏未定义，扩展 API 被 `#ifdef` 屏蔽

受控函数分布在头文件中，每个由 `#ifdef MUSL_EXTERNAL_FUNCTION` 包裹，包括但不限于：`dladdr1`、`pthread_cancel`/`pthread_testcancel`、`__pthread_key_create`、`__register_atfork`、`strtoll_l`/`strtoull_l`、`timelocal`、`__realpath_chk`、`__sched_cpufree`、`getauxval` 等。

关键规则：**PC 专有 API 只在头文件中通过 `#ifdef MUSL_EXTERNAL_FUNCTION` 控制可见性，不可从 `libc.map.txt` 中单独删符号来控制**。移动端构建通过不定义该宏来隐去这些 API，无需维护两套符号表。

### 新增符号流程

1. 在 `src/<domain>/` 实现函数
2. 在 `include/<header>.h` 声明（公共 API）或内部头文件声明（内部符号）
3. 在 `libc.map.txt` 的 `global:` 块中新增条目
4. 运行 `libc-test/src/api/` 编译测试确认头文件正确性

## 专有名词

| 名词 | 定义 |
|---|---|
| **API** | Application Programming Interface，编译期契约（头文件中的函数签名/类型/常量） |
| **ABI** | Application Binary Interface，链接期契约（libc.so 导出的符号集合和调用约定） |
| **libc.map.txt** | ld version script，控制 libc.so 符号导出的唯一源头（2121行，2117个符号） |
| **version script** | GNU ld 链接器脚本格式：`global:` 列出导出符号，`local: *;` 隐藏其余 |
| **NDK 合规** | NDK（Native Development Kit）对公开头文件和符号导出的合规性要求 |
| **MUSL_EXTERNAL_FUNCTION** | 编译宏，控制 PC 专有 API 的可见性。由 GN 标志 `musl_extended_function` 控制，PC 构建开启，移动端关闭 |
| **musl_src.gni** | GN 构建文件，指定 version script 路径和编译的源文件列表 |

## 关键机制

| 机制 | 位置 | 说明 |
|---|---|---|
| 公共头文件 | `include/`、`include/sys/`、`include/arpa/`、`include/netinet/` | API 契约 |
| 架构 bits | `arch/<arch>/bits/`（19架构） | 每架构独立的类型/常量定义 |
| 符号导出控制 | `libc.map.txt` | ABI 契约，ld version script |
| API 编译测试 | `libc-test/src/api/` | 验证每个头文件可编译且声明完整 |
| OH 扩展头文件 | `include/fortify/`、`include/linux/`、`include/info/` | OH 特有 API |

## 约束

- **已有类型/常量/宏不得修改语义**：`sizeof`、枚举值、宏展开结果不可变
- **新增公共 API 必须同时更新头文件 + libc.map.txt**：漏一个导致 API/ABI 不一致
- **OH 扩展头文件放在独立目录**：不与 musl 标准头文件混放
- **OH 扩展符号命名加前缀**：避免与上游 musl 或未来 POSIX 标准冲突
- **架构 bits 修改必须验证所有 19 个架构**：`arch/<arch>/bits/` 每架构独立
