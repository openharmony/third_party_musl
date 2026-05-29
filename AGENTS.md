# musl libc (OpenHarmony) Agent 指引

## 1. 代码地图

本 AGENTS.md 适用于仓库根目录。本仓库实现 OpenHarmony 的 C 标准库（基于 musl），核心职责是提供高效、标准兼容的 libc 实现，支持 Linux/LiteOS/UniProton 多内核、多架构。

关键路径（按风险从高到低）：

| 路径 | 职责 | 风险 |
|---|---|---|
| `ldso/linux/` | OH 扩展：namespace、CFI、ADLT、dynlink 修改 | OH 特有改量，系统启动关键路径，谨慎修改 |
| `ldso/` | 动态链接器：dlstart 阶段1 + dynlink 标准链接 | 系统启动关键路径，阶段1 无 libc 可用 |
| `include/` + `libc.map.txt` | 公共头文件 + 符号导出表 | ABI 兼容性边界，改错全局受影响 |
| `src/malloc/` | 内存分配器（mallocng、jemalloc） | 全局性能/稳定性影响 |
| `src/fortify/` | 缓冲区溢出保护（_chk 函数族） | 误报/漏报都是安全问题 |
| `src/gwp_asan/` | GWP-ASan 内存安全检测 | 采样逻辑影响性能 |
| `src/hook/` + `src/sigchain/` + `src/syscall_hooks/` | 信号链/钩子/系统调用拦截 | 时序敏感，难调试 |
| `arch/` (19架构) | 架构特定实现（asm/syscall） | 单架构改动不能破坏其他 18 个 |
| `crt/` | C 运行时启动（crt1/scrt1/rcrt1） | 启动关键路径 |
| `src/fdsan/` | 文件描述符 sanitizer | 调试路径 |
| `porting/` (6平台) | 平台适配层（旧版本代码） | 低风险，存量维护 |

高频修改路径：`src/malloc/`、`src/string/`、`src/stdio/`、`include/`、`ldso/linux/`

子级规则：`ldso/linux/README.md` 描述 namespace 机制，修改 namespace 前必读。

## 2. 知识路由

| 关键词 | 知识文件 |
|---|---|
| dlstart / dynlink / 动态链接 / 阶段1 / 自举 / ADLT / 合并文件 / prelink / 预链接 / crt / crt1 / scrt1 / rcrt1 / GOT entry / 延迟重定位 / ldso random / 链接器随机化 | `docs/knowledge/dynlink.md` |
| namespace / dlns / dlopen_ns / separated / permitted_paths / allowed_libs / inherit / ns_inherit / ld_permitted_path / Dl_namespace / ns_configor / section.dir.map / 命名空间 / 隔离 | `docs/knowledge/namespace.md` |
| malloc / mallocng / oldmalloc / jemalloc / fortify / _chk / 缓冲区溢出 / GWP-ASan / gwp_asan / guard page / 采样 / fdsan / file descriptor sanitizer / close_tag / FdTable / __fortify_error / MALLOC_FREELIST_HARDENED | `docs/knowledge/memory-safety.md` |
| sigchain / 信号链 / signal_chain_handler / hook / RESTRACE / 钩子 / MallocDispatchType / memtrace / restrace / syscall_hooks / 系统调用钩子 / svc0_entry / HOOK_ENABLE / FDTRACK / dispatch table | `docs/knowledge/signal-hook.md` |
| CFI / 控制流完整性 / cfi_slowpath / cfi_check / cfi_fail / icall_item / cfi_modifier / PT_OHOS_CFI_MODIFIER / CFI Shadow / shadow value / LIBRARY_ALIGNMENT / PAC / 间接调用 | `docs/knowledge/cfi.md` |
| API / ABI / 兼容性 / 公共头文件 / libc.map.txt / version script / 符号导出 / NDK / 头文件 / include / MUSL_EXTERNAL_FUNCTION / musl_extended_function / PC专有API | `docs/knowledge/api-compatibility.md` |

## 3. 约束边界

### 约束

- **ABI 稳定边界**：`include/` 中已发布的函数签名、返回值语义、errno 行为不得修改；`libc.map.txt` 已有符号不得删除
- **dlstart 无运行时**：阶段1 无 libc、无重定位，不得调用任何 libc 函数或使用全局变量
- **CFI 编译器耦合**：icall_item 结构与编译器校验表格式绑定，修改需同步编译器侧
- **信号链顺序**：handler 执行顺序是约定好的，不可随意调换
- **跨架构不污染**：单架构 `arch/<name>/` 的 asm 改动不得引入其他 18 个架构未实现的语义差异
- **不删 DFX**：不得为了通过测试而删除日志/错误码/sanitizer 检查
- **不引入新依赖**：除非任务明确要求

### 先问（Ask before）

- 修改 `include/` 中任何公共头文件
- 修改 `libc.map.txt`
- 修改 `ldso/` 下动态链接器代码
- 修改 `dlstart.c` 或 `crt/` 下的启动代码
- 修改 namespace 配置格式或 `ld-musl-namespace-*.ini` 解析逻辑
- 新增或修改 malloc 分配器核心逻辑（非 bug fix）
- 修改 CFI icall_item 结构定义

## 4. 验证闭环

### 编译验证

```sh
# 全量构建（从 OpenHarmony 源码根执行）
./build.sh --product-name rk3568 --build-target musl_all

# 构建并运行测试
./build.sh --product-name rk3568 --build-target libctest
```

### 验证基本要求

| 验证项 | 说明 | 触发条件 |
|---|---|---|
| 需求/问题场景测试 | 根据变更场景编写或运行针对性测试 | 必选 |
| libc-test | `./build.sh --product-name rk3568 --build-target libctest` | 必选 |
| XTS actsToolchainTest | OpenHarmony XTS 工具链兼容性测试套件 | 必选 |
| XTS actsThirdPartyMuslTest | OpenHarmony XTS 三方 musl 兼容性测试套件 | 必选 |
| 稳定性测试 | 长时间压力测试，验证无内存泄漏/死锁/崩溃 | 必选 |
| NDK 头文件/符号暴露检查 | C 库公共头文件和符号导出合规性检查 | 必选 |
| Benchmark | 性能基准测试，对比变更前后性能数据 | 根据场景 |
