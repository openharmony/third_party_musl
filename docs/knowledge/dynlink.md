# 动态链接器知识

本文只记录动态链接器（ldso）的框架设计、OH 新增功能、专有名词和修改约束。
namespace 隔离机制见 [[namespace]]，CFI 校验见 [[cfi]]。

## 框架设计

### 两层架构

```
ldso/dlstart.c + ldso/dynlink.c       ← musl 上游标准链接器
ldso/linux/dynlink.c                    ← OH 扩展层，叠加在上游之上
ldso/linux/dynlink_adlt.h              ← ADLT 数据结构
ldso/linux/dynlink_rand.c              ← 链接器随机化
ldso/linux/ld_log.c                    ← 链接器日志
```

- musl 标准层和 OH 扩展层职责分离，OH 特有逻辑只放在 `ldso/linux/`，不污染上游文件
- 扩展层通过条件编译和函数钩子接入标准链接流程

### 三阶段启动模型

| 阶段 | 文件 | 运行时环境 |
|---|---|---|
| 阶段1 自举 | `ldso/dlstart.c` | 无 libc、无全局变量、无重定位 |
| 阶段2 标准链接 + OH 扩展 | `ldso/dynlink.c` + `ldso/linux/dynlink.c` | libc 可用 |
| 阶段3 进入 main | `crt/` | 完整运行时 |

### crt 启动文件设计

| 文件 | 用途 |
|---|---|
| `crt/crt1.c` + `crt/<arch>/` | 静态链接入口 |
| `crt/Scrt1.c` | PIE 入口 |
| `crt/rcrt1.c` | PIE + 静态入口 |

## 专有名词

| 名词 | 定义 | 与上游 musl 的关系 |
|---|---|---|
| **dlstart** | 链接器自举代码，阶段1，在无运行时环境下完成 ldso 自身重定位 | musl 原有，不可修改 |
| **dynlink** | 标准 musl 动态链接器 | musl 原有 |
| **ADLT** (Advanced Dynamic Linking Technology) | OH 新增的"部分加载+大页"机制，支持从合并文件中按索引加载子库，无需完整加载 ELF | OH 独有 |
| **combined file** | ADLT 合并文件，多个 SO 打包进单一文件 | OH 独有 |
| **prelink / prelinker** | 预链接机制，提前计算重定位地址，减少运行时链接开销 | OH 新增 |
| **GOT entry tracking** | ADLT 场景下的 GOT 表项追踪，支持延迟重定位 | OH 独有，依赖 ADLT |
| **ldso random** (`dynlink_rand.c`) | 链接器加载地址随机化 | OH 新增安全特性 |
| **dlns** (dynamic linker namespace) | 动态链接器命名空间隔离 | OH 新增，见 [[namespace]] |
| **__dls2** | 阶段1→阶段2 的跳转函数 | musl 原有 |

## OH 新增功能点

| 功能 | 位置 | 说明 |
|---|---|---|
| ADLT 合并文件加载 | `ldso/linux/dynlink_adlt.h` + `ldso/linux/dynlink.c` | 从 PT_ADLT segment 的合并文件中按 index 定位并加载子库 |
| ADLT 大页支持 | `ldso/linux/dynlink_adlt.h` | 加载段对齐到大页边界 |
| ADLT GOT 追踪 | `ldso/linux/dynlink.c` | 延迟重定位的 GOT 表项管理 |
| prelink 预链接 | `ldso/linux/dynlink.c` | 预计算重定位减少运行时开销 |
| 链接器随机化 | `ldso/linux/dynlink_rand.c` | 随机化 ldso 加载基址 |
| 链接器独立日志 | `ldso/linux/ld_log.c` | 链接器专用日志，不依赖 hilog |
| dlopen_ns 扩展 | `ldso/linux/dynlink.c` | dlopen 时按 namespace 隔离加载 |

## 约束

- **OH 和 musl 分层不污染**：OH 特有逻辑只放在 `ldso/linux/`，不改动 `ldso/dlstart.c` 和 `ldso/dynlink.c` 的行为语义
- **ADLT index 边界**：[0, ADLT_MAX_NSO)，越界无运行时检查
- **init/fini 执行顺序**：影响全局构造/析构，不可随意调整
- **链接器随机化不削弱**：安全特性，不可降低熵值或默认关闭
- **ADLT 文件格式兼容性**：合并文件格式是发布接口，修改解析逻辑需向后兼容
