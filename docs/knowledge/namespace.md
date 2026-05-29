# 命名空间隔离知识

本文只记录动态链接器命名空间隔离机制的设计目的、专有名词和修改约束。
namespace 是动态链接器的子系统，整体架构见 [[dynlink]]。

## 设计目的

命名空间解决**同名 SO 符号冲突**问题。在 OpenHarmony 系统中，不同模块可能依赖同名但不同版本的 SO。在同一个全局符号空间中，同名 SO 只会加载先被搜索到的那个，后加载的模块被迫使用不匹配的版本，导致符号或行为异常。

通过将 DSO 加载隔离到不同 namespace：
- 每个 namespace 拥有独立的 SO 搜索路径和加载上下文
- 同名 SO 在不同 namespace 中各自加载、互不干扰
- 通过继承机制选择性共享 SO，避免完全隔离导致的代码冗余

## 框架设计

### 核心模型

```
进程
  ├─ default namespace（始终存在）
  ├─ ns_1（dlns_create 创建，可继承 default 的 SO）
  ├─ ns_2（独立 namespace，完全隔离）
  └─ ...
```

### 两种创建方式

| 方式 | 入口 | 适用场景 |
|---|---|---|
| 配置文件 | `/etc/ld-musl-namespace-<arch>.ini` | 系统级预定义，启动时解析 |
| API 动态创建 | `dlns_create()` / `dlns_create2()` | 应用运行时按需创建 |

### 三种隔离模式

| 模式 | 行为 |
|---|---|
| 普通模式 | namespace 有自己的 lib_paths，SO 在各自路径下搜索 |
| separated 模式 | 额外限制：只能加载 allowed_libs 中的 SO，且必须来自 permitted_paths |
| 继承模式 | namespace A 可继承 B 的 shared_libs，B 的 SO 对 A 可见 |

## 专有名词

| 名词 | 定义 | 说明 |
|---|---|---|
| **namespace** (ns_t) | DSO 加载隔离容器 | OH 独有 |
| **default namespace** | 系统默认 namespace，始终存在 | 不可删除 |
| **separated** | 严格隔离模式，限制可加载的 SO 名称和路径 | 安全特性 |
| **permitted_paths** | separated 模式下允许加载 SO 的路径白名单 | 前缀匹配，不做子目录递归 |
| **allowed_libs** | separated 模式下允许加载的 SO 文件名白名单 | 和 separated 配合使用 |
| **inherit** (ns_inherit) | namespace 间单向共享关系 | 子 ns 可用父 ns 的 SO，反之不行 |
| **shared_libs** | 继承关系中允许共享的库列表 | `allow_all_shared_libs` 表示全共享 |
| **ld_permitted_path** | 链接器自身路径白名单 | 安全限制 |
| **Dl_namespace** | 用户态 namespace 句柄 | 仅包含 name[256] |
| **ns_configor** | INI 配置文件解析器 | 解析 `ld-musl-namespace-*.ini` |
| **section.dir.map** | 配置中场景→可执行路径的映射段 | 决定进程启动时加载哪个场景 |

## 关键机制

| 机制 | 位置 | 说明 |
|---|---|---|
| namespace 核心管理 | `ldso/linux/namespace.c` | ns_t/nslist 生命周期、SO 加载时的 namespace 校验 |
| INI 配置解析 | `ldso/linux/ns_config.c` | 解析 `/etc/ld-musl-namespace-<arch>.ini` |
| dlopen_ns | `ldso/linux/dynlink.c` | 在指定 namespace 中加载 DSO |
| dlns_create/create2 | `ldso/linux/dynlink.c` | 运行时动态创建 namespace |
| dlns_inherit | `ldso/linux/dynlink.c` | 运行时设置 namespace 继承 |
| separated 隔离 | `ldso/linux/namespace.c` | 加载时校验 permitted_paths + allowed_libs |
| dlns_set_module_namespace_lib_path | `ldso/linux/dynlink.c` | 模块级路径设置，有权限校验 |
| 跨架构配置 | `config/ld-musl-namespace-*.ini` | aarch64/arm/x86_64/riscv64/loongarch64 各一份 |

## 约束

- **"default" 不可删除/不可同名创建**
- **配置文件格式是稳定接口**：新增字段必须向后兼容，不可修改已有 key 语义
- **配置文件依赖架构**：新增架构必须同步提供 `ld-musl-namespace-<arch>.ini`
- **继承单向**：A 继承 B ≠ B 能访问 A 的 SO
- **separated 模式下加载不符合白名单的 SO 会失败**
- **permitted_paths 前缀匹配**：不做子目录递归校验
- **dlns_set_module_namespace_lib_path 有权限校验**：只能修改非配置 namespace
- **NS_NAME_MAX = 255**
