# CFI 控制流完整性知识

本文记录 CFI（Control Flow Integrity）机制的框架设计、专有名词和修改约束。
CFI 是动态链接器的子系统，整体架构见 [[dynlink]]。

## 设计目的

CFI 防止间接调用劫持攻击——攻击者通过篡改函数指针/vtable 将控制流重定向到恶意代码。通过在每个间接跨 DSO 调用前校验目标地址的合法性，确保控制流只能跳转到编译器预期的合法目标。

OH 实现对应 LLVM 的 CFI Cross-DSO 方案：Clang 的 `-fsanitize=cfi -fsanitize-cfi-cross-dso` 在每次间接调用前注入对 `__cfi_slowpath` 的调用，链接器侧负责在加载时建立地址→校验函数的映射表（CFI Shadow）。

全部为 OH 新增，上游 musl 无。

## 框架设计

### 两层防护

| 层 | 机制 | 平台 |
|---|---|---|
| CFI Shadow | 虚拟地址空间中的稀疏数组，映射目标地址→`__cfi_check` 函数 | 全架构 |
| CFI Modifier / PAC | `PT_OHOS_CFI_MODIFIER` ELF 段 + SIGILL handler，arm64 指针认证的软件后备 | aarch64 only |

### CFI Shadow 工作流

1. **加载时**：DSO 加载后调用 `map_dso_to_cfi_shadow()`，将 DSO 的地址范围映射到其 `__cfi_check` 函数
2. **调用时**：编译器注入的 `__cfi_slowpath(target_addr)` 被调用 → 将 target_addr 散列到 shadow → 提取 shadow value → 反推出 `__cfi_check` 地址 → 调用校验
3. **卸载时**：`unmap_dso_from_cfi_shadow()` 将 shadow 值重置为 invalid

### Shadow Value 三种状态

| 值 | 含义 |
|---|---|
| 0 (invalid) | 地址不属于任何已加载 DSO |
| 1 (uncheck) | DSO 未启用 CFI，跳过校验 |
| ≥2 (valid) | 编码了 `__cfi_check` 地址的偏移量 |

### Shadow 更新：双缓冲原子切换

不直接写入 shadow，而是先写入临时映射，再通过 `mremap(MREMAP_FIXED)` 原子替换。保证 shadow 读操作始终看到一致的状态。

### CFI Modifier（aarch64 PAC 后备）

- ELF segment 类型 `PT_OHOS_CFI_MODIFIER`（0x6833fc30）携带排序的 (offset, modifier) 对
- `pac_reset_handler` SIGILL handler 捕获 `ILL_ILLPACCFI`，校验调用者 PC 在 `icall_item` 范围内且 modifier 匹配，然后跳过故障 `blr` 指令

## 专有名词

| 名词 | 定义 |
|---|---|
| **CFI Shadow** | 虚拟地址空间中的稀疏数组，将 DSO 地址映射到 `__cfi_check` 函数 |
| **shadow value** | shadow 数组中的一个 uint16_t 条目，编码校验状态 |
| **__cfi_slowpath** | 编译器注入的 CFI 校验入口，每次间接跨 DSO 调用前执行 |
| **__cfi_slowpath_diag** | 带诊断信息的 slowpath 变体 |
| **__cfi_check** | 每个 DSO 内的校验函数，由 Clang 生成，校验目标地址是否在该 DSO 的合法间接调用目标集合中 |
| **icall_item** | 间接调用目标描述结构：pc_check（PC 校验值）、valid 标志、base、modifier_begin/end |
| **cfi_modifier** | PAC 指令的 (offset, modifier) 对，用于 arm64 指针认证校验 |
| **PT_OHOS_CFI_MODIFIER** | OH 自定义 ELF program header 类型（0x6833fc30），携带 modifier 数组 |
| **LIBRARY_ALIGNMENT** | DSO 加载对齐要求 = 256KB，保证 shadow 散列唯一性 |
| **__cfi_fail_report** | CFI 校验失败时的报告函数（noreturn），定义在 `include/stdlib.h` |

## 关键机制

| 机制 | 位置 | 说明 |
|---|---|---|
| CFI 核心运行时 | `ldso/linux/cfi.c` (667行) | slowpath/slowpath_diag、shadow 创建/映射/解除 |
| CFI 类型定义 | `ldso/linux/cfi.h` (31行) | LIBRARY_ALIGNMENT、init/map/unmap 声明 |
| icall_item 结构 | `src/internal/dynlink.h` | PC 校验和 modifier 范围 |
| Shadow 映射 | `ldso/linux/dynlink.c` (map_dso_to_cfi_shadow) | 加载/卸载时更新 shadow |
| Modifier 解析 | `ldso/linux/dynlink.c` (PT_OHOS_CFI_MODIFIER) | 解析 ELF segment 填入 dso->modifier_begin/end |
| ADLT + CFI | `ldso/linux/dynlink_adlt.h` | ADLT 子库的 `__cfi_check__<ndso_index>` 命名 |

## 约束

- **DSO 加载地址必须 256KB 对齐**：否则 shadow 散列不唯一，可能产生散列冲突
- **`__cfi_check` 地址必须 4K 对齐**：shadow value 编码依赖
- **单个 DSO 的 CFI 检查范围上限约 256MB**：超出部分退化为 uncheck
- **Shadow 更新必须用 mremap 原子切换**：不可直接就地写入 shadow
- **ADLT 子库的 `__cfi_check` 命名规则**：`__cfi_check__<ndso_index>`，ADLT index 变化影响符号名
- **CFI modifier 数组已排序**：依赖二分查找，不可改变排序规则
