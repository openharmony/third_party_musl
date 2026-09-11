# musl libc Locale 接口使用指南

> 本文档面向 OpenHarmony 应用和系统开发者，描述 musl libc locale 相关接口的使用场景和注意事项。
> 依据 ISO C99/C11 和 POSIX.1-2008 标准，OpenHarmony 在标准基础上扩展了 ICU 委托机制和白名单注册。

---

## 1. Locale 设置与管理

### setlocale

**头文件：** `<locale.h>`

**函数签名：** `char *setlocale(int category, const char *locale)`

**使用场景：**

设置当前进程的全局 locale。应用在启动时默认使用 `"C"` locale，如需切换到特定地区的 locale（如港澳地区的 `zh_HK.Big5`），调用此函数。

- `category` 指定影响的 locale 类别，常用 `LC_ALL`（全部类别），也可单独设置 `LC_CTYPE`（字符分类）、`LC_NUMERIC`（数值格式）、`LC_MONETARY`（货币格式）等
- `locale` 为目标 locale 名称，如 `"zh_HK.Big5"`、`"zh_CN"`、`"en_US.UTF-8"`、`"C"`、`"POSIX"`
- 返回非 NULL 表示成功（返回当前 locale 名字符串），返回 NULL 表示该 locale 不可用

**OpenHarmony 双白名单机制：**

OpenHarmony 使用两套独立的白名单协同工作：

1. **Locale 注册白名单**（`locale_map.c` 中的 `g_valid_locale_table[]`）：决定哪些 locale 名在 `setlocale`/`newlocale` 时被标记为 `ICU_VALID` flag，触发 ICU 数据目录加载。此白名单包含 `zh_CN`、`zh_CN.UTF-8`、`en_US.UTF-8`、`zh_HK.Big5`、`zh_HK.Big5-HKSCS`。

2. **wctype 委托白名单**（`locale_impl.c` 中的 `g_icu_wctype_locales[]`）：决定 14 个 wctype `_l` 函数（`iswalpha_l`、`iswdigit_l`、`towupper_l` 等）是否走 ICU `u_is*` 路径。此白名单包含 `zh_CN`、`en_US.UTF-8`、`zh_HK.Big5`、`zh_HK.Big5-HKSCS`，**不含** `zh_CN.UTF-8`。

两套白名单的差异：`zh_CN.UTF-8` 在注册白名单中被标记为 `ICU_VALID`（可使用 ICU 数值解析等），但在 wctype 委托白名单中不匹配，因此 `isw*_l` 系列函数走 musl 内置字符分类表（fallback）。这是既有行为，保持不变。

**注意事项：**

- `setlocale` 失败时返回 NULL，POSIX 未规定此时 errno 的行为，不要依赖 errno 判断失败原因
- locale 名含斜杠 `/` 时会回退为 `C.UTF-8`
- `setlocale(LC_ALL, NULL)` 用于查询当前 locale 而不修改

### newlocale

**头文件：** `<locale.h>`

**函数签名：** `locale_t newlocale(int category_mask, const char *locale, locale_t base)`

**使用场景：**

创建一个独立的 locale 对象（`locale_t`），不影响全局 locale。适用于多线程场景下每个线程使用不同 locale，或同一函数内需要切换 locale 但不想污染全局状态。

- `category_mask` 指定类别掩码，常用 `LC_ALL_MASK`
- `locale` 为 locale 名称，如 `"zh_HK.Big5"`
- `base` 通常传 `NULL`（从默认 locale 创建），也可传入已有 locale_t 进行部分覆盖
- 返回非 NULL 表示成功，返回 NULL 表示失败

**注意事项：**

- 创建的 `locale_t` 须通过 `freelocale` 释放，避免内存泄漏
- `newlocale` 创建的 locale_t 可传给所有 `*_l` 后缀函数（如 `iswalnum_l`、`strtod_l`）
- 线程安全：不同线程各自持有独立的 locale_t，互不影响

### uselocale

**头文件：** `<locale.h>`

**函数签名：** `locale_t uselocale(locale_t loc)`

**使用场景：**

设置或查询当前线程的 locale。与 `setlocale` 的进程级全局 locale 不同，`uselocale` 设置的 locale 仅影响当前线程。

- 传入 locale_t 设置当前线程 locale，返回之前的线程 locale
- 传入 `NULL` 查询当前线程 locale 而不修改
- 传入 `LC_GLOBAL_LOCALE` 恢复为使用全局 locale（即 `setlocale` 设置的 locale）

**注意事项：**

- `uselocale` 设置的 locale 仅影响当前线程，其他线程不受影响
- 通过 `uselocale` 设置的 locale_t 在不再使用时应调用 `freelocale` 释放

### freelocale

**头文件：** `<locale.h>`

**函数签名：** `void freelocale(locale_t loc)`

**使用场景：**

释放通过 `newlocale` 或 `duplocale` 创建的 locale_t 对象。

**注意事项：**

- 不要释放 `LC_GLOBAL_LOCALE`（全局 locale 不需要释放）
- 不要释放正在被 `uselocale` 使用的 locale_t（先恢复为 `LC_GLOBAL_LOCALE` 再释放）
- 不要重复释放同一个 locale_t

### duplocale

**头文件：** `<locale.h>`

**函数签名：** `locale_t duplocale(locale_t loc)`

**使用场景：**

复制一个 locale_t 对象，生成独立的副本。适用于需要在多个上下文中使用同一 locale 但各自管理生命周期的场景。

- 传入 `LC_GLOBAL_LOCALE` 可复制当前全局 locale
- 返回新的 locale_t，须通过 `freelocale` 释放

---

## 2. Locale 信息查询

### localeconv

**头文件：** `<locale.h>`

**函数签名：** `struct lconv *localeconv(void)`

**使用场景：**

查询当前 locale 的数值和货币格式化信息。返回的 `struct lconv` 包含小数点符号、千分位分隔符、货币符号等字段，用于本地化数值显示。

常用字段：
- `decimal_point`：小数点符号（如 `"."` 或 `","`）
- `thousands_sep`：千分位分隔符
- `currency_symbol`：本地货币符号（如 `"$"` 或 `"HK$"`)
- `mon_decimal_point`：货币小数点
- `int_curr_symbol`：国际货币符号（如 `"HKD "`）

**注意事项：**

- 返回的指针指向 musl 内部静态存储，**调用方不需释放**
- 指针在下次 `setlocale` 调用后可能失效（字段值可能改变），**不要跨 `setlocale` 调用缓存该指针**
- 在 ICU 委托 locale（如 `zh_HK.Big5`）下，`localeconv` 返回的字段值来自 ICU，反映港澳地区格式

---

## 3. 宽字符分类与转换

以下 12 个 `isw*_l` 函数和 2 个 `tow*_l` 函数在 OpenHarmony 白名单 locale 下委托 ICU 进行宽字符分类/转换，支持非 ASCII 字符（如 Big5/HKSCS 字符集的中文字符）的正确分类。非白名单 locale 下回退到 musl 内置 ASCII 字符分类表。

### iswalnum_l / iswalpha_l / iswblank_l / iswcntrl_l / iswdigit_l / iswgraph_l / iswlower_l / iswprint_l / iswpunct_l / iswspace_l / iswupper_l / iswxdigit_l

**头文件：** `<wctype.h>`

**函数签名：** `int isw*_l(wint_t wc, locale_t loc)`

**使用场景：**

判断宽字符的类别属性（字母、数字、标点、空格、控制字符等），使用指定的 locale_t。适用于在特定 locale 下处理非 ASCII 宽字符的场景。

- 在 `zh_HK.Big5` locale 下，这些函数委托 ICU `u_is*` 函数，正确分类 Big5/HKSCS 字符集中的中文字符、全角标点等
- 在非 ICU 委托 locale（如 `C` locale）下，使用 musl 内置字符分类表，仅支持 ASCII 范围

**注意事项：**

- `loc` 参数须通过 `newlocale` 创建，传入 NULL 或无效 locale_t 行为未定义
- 函数不设置 errno
- 对于 ASCII 字符（'a'-'z'、'A'-'Z'、'0'-'9'），所有 locale 下的分类结果一致

### towupper_l / towlower_l

**头文件：** `<wctype.h>`

**函数签名：** `wint_t towupper_l(wint_t wc, locale_t loc)` / `wint_t towlower_l(wint_t wc, locale_t loc)`

**使用场景：**

将宽字符转换为大写或小写，使用指定的 locale_t。在 `zh_HK.Big5` locale 下委托 ICU `u_toupper`/`u_tolower` 函数，支持非 ASCII 字符的大小写转换（如全角字母、希腊字母等）。

**注意事项：**

- 返回转换后的宽字符；如果字符无大小写形式，返回原字符
- 函数不设置 errno

---

## 4. 数值解析

### strtod_l / wcstod_l

**头文件：** `<stdlib.h>`（strtod_l）、`<wchar.h>`（wcstod_l）

**函数签名：**
- `double strtod_l(const char *restrict s, char **restrict p, locale_t loc)`
- `double wcstod_l(const wchar_t *restrict s, wchar_t **restrict p, locale_t loc)`

**使用场景：**

将字符串（或宽字符串）解析为 double 值，使用指定的 locale_t 决定小数点、数字分隔符等格式。适用于在特定 locale 下解析本地化数值字符串。

- 在 `zh_HK.Big5` locale 下委托 ICU `unum_parseDouble` 函数，按港澳地区数值格式解析
- `endptr`（若非 NULL）指向解析终止的下一个字符位置，完整解析时指向字符串末尾的 `\0`

**注意事项：**

- 成功时 errno **不保证被清零**——调用方应在调用前显式设置 `errno = 0`，调用后检查 errno 是否为 `ERANGE` 来判断溢出
- 解析 `"inf"`/`"infinity"` 返回 `INFINITY`
- 解析 `"nan"` 在 ICU 委托 locale 下返回 `0.0`（ICU `unum_parseDouble` 不支持 nan），在非 ICU locale 下返回 `NAN`（musl 内置 `__floatscan` 支持）
- 解析结果超出 double 范围时，ICU 委托 locale 返回 `INFINITY`（不保证设置 `errno=ERANGE`），非 ICU locale 返回 `±HUGE_VAL` 且 `errno = ERANGE`
- `wcstod_l` 的语义与 `strtod_l` 完全一致，仅输入类型不同（宽字符）

---

## 5. 编码转换

### iconv_open

**头文件：** `<iconv.h>`

**函数签名：** `iconv_t iconv_open(const char *tocode, const char *fromcode)`

**使用场景：**

创建一个编码转换句柄，用于将文本从一种字符编码转换为另一种。OpenHarmony musl 内置了 Big5/HKSCS 编码别名（`big5`/`big5hkscs`/`cp950` 等，大小写不敏感），独立于 locale 注册机制。

常用编码名：
- `"UTF-8"`：UTF-8 编码
- `"big5"` / `"bigfive"` / `"cp950"`：Big5 编码（繁体中文）
- `"big5hkscs"`：Big5-HKSCS 编码（含香港增补字符集）

**注意事项：**

- 成功返回有效 `iconv_t` 句柄，失败返回 `(iconv_t)-1` 且 `errno = EINVAL`（不支持的字符集名）
- 传入含 locale 前缀的复合名（如 `"zh_HK.big5"`）会被拒绝，须使用裸字符集别名（如 `"big5"`）
- 句柄在 `iconv_close` 前可重复用于多次转换

### iconv

**头文件：** `<iconv.h>`

**函数签名：** `size_t iconv(iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft)`

**使用场景：**

执行编码转换，将输入缓冲区的字节从源编码转换为目标编码写入输出缓冲区。

- 返回 `0` 表示转换完成且无不可逆转换
- 返回正数表示有不可逆转换（字符被替换为近似字符）
- 返回 `(size_t)-1` 表示出错，此时 `errno` 区分错误类型

**错误处理：**

| errno | 含义 | inbuf 指向 | 处理方式 |
|-------|------|------------|----------|
| `EILSEQ` | 源编码中遇到非法字节序列 | 非法字符起始位置 | 可从该位置继续转换（跳过非法字节或修正输入） |
| `EINVAL` | 输入末尾含不完整多字节序列 | 不完整序列起始位置 | 补充更多输入数据后继续转换 |
| `E2BIG` | 输出缓冲区空间不足 | 尚未转换的输入位置 | 扩大输出缓冲区后继续转换 |

**注意事项：**

- 转换失败时 `inbuf` 指向出错位置，输出缓冲区中已包含成功转换的部分数据
- 对于不完整序列（`EINVAL`），可补充输入后从 `inbuf` 当前位置继续调用 `iconv`，无需重新 `iconv_open`
- Big5/HKSCS 补充平面字符（如 U+233E6）在 UTF-8 中为 4 字节编码

### iconv_close

**头文件：** `<iconv.h>`

**函数签名：** `int iconv_close(iconv_t cd)`

**使用场景：**

释放 `iconv_open` 创建的转换句柄，释放相关资源。

**注意事项：**

- 返回 `0` 表示成功
- 句柄关闭后不可再用，使用已关闭的句柄为未定义行为
- 不要重复关闭同一个句柄

---

## 6. OpenHarmony 特有机制说明

### ICU 委托机制

OpenHarmony musl libc 在编译时通过 `FEATURE_ICU_LOCALE` 宏启用 ICU 集成。在白名单 locale 下，wctype 字符分类、大小写转换、数值解析和格式化操作委托给 ICU 库（`libhmicuuc.z.so` / `libhmicui18n.z.so`），以支持非 ASCII 字符的正确本地化行为。

- ICU 版本通过运行时 `GetIcuVersion` 动态获取，符号名按版本拼接（如 `u_isalnum_73`）
- ICU 数据目录通过 `SetOhosIcuDirectory` 设置，在 locale 注册成功后自动触发
- 非 ICU 委托 locale 下，所有接口回退到 musl 内置标准行为（ASCII 字符分类表、C locale 默认格式）

### Locale 名编码后缀截断

OpenHarmony 在调用 ICU 函数前，从 locale 名的 `.` 处截断，提取语言区域名传给 ICU。例如 `zh_HK.Big5` 截断为 `zh_HK`，`zh_CN.UTF-8` 截断为 `zh_CN`。此截断仅影响 ICU 函数的入参，不影响 `setlocale` 返回的完整 locale 名和 `localeconv` 返回的格式化字段。

### 双白名单机制

OpenHarmony 使用两套独立的白名单：

| 白名单 | 文件 | 数组 | 用途 | 包含的 locale |
|--------|------|------|------|---------------|
| Locale 注册白名单 | `locale_map.c` | `g_valid_locale_table[]` | `setlocale`/`newlocale` 时标记 `ICU_VALID` flag，触发 ICU 数据加载 | `zh_CN`、`zh_CN.UTF-8`、`en_US.UTF-8`、`zh_HK.Big5`、`zh_HK.Big5-HKSCS` |
| wctype 委托白名单 | `locale_impl.c` | `g_icu_wctype_locales[]` | 14 个 wctype `_l` 函数是否走 ICU `u_is*` 路径 | `zh_CN`、`en_US.UTF-8`、`zh_HK.Big5`、`zh_HK.Big5-HKSCS` |

两套白名单的差异在于 `zh_CN.UTF-8`：它在注册白名单中被标记为 `ICU_VALID`（可使用 ICU 数值解析、货币格式化等），但在 wctype 委托白名单中不匹配，`isw*_l` 系列函数走 musl 内置字符分类表。这是既有行为，保持不变。

新增 locale 支持时需同时修改两处数组。

---

## 7. 典型使用示例

### 场景一：港澳应用设置 locale 并解析数值

```c
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    /* 设置港澳 locale */
    if (setlocale(LC_ALL, "zh_HK.Big5") == NULL) {
        fprintf(stderr, "Failed to set zh_HK.Big5 locale\n");
        return 1;
    }

    /* 创建线程级 locale_t 用于 *_l 函数 */
    locale_t loc = newlocale(LC_ALL_MASK, "zh_HK.Big5", NULL);
    if (loc == NULL) {
        fprintf(stderr, "Failed to create locale_t\n");
        return 1;
    }

    /* 解析数值字符串 */
    errno = 0;
    char *endptr = NULL;
    double value = strtod_l("3.14", &endptr, loc);
    if (errno == ERANGE) {
        fprintf(stderr, "Value out of range\n");
        freelocale(loc);
        return 1;
    }
    printf("Parsed value: %f\n", value);

    /* 查询货币符号 */
    struct lconv *lc = localeconv();
    printf("Currency symbol: %s\n", lc->currency_symbol);

    freelocale(loc);
    return 0;
}
```

### 场景二：线程级 locale 用于宽字符分类

```c
#include <locale.h>
#include <wctype.h>
#include <stdio.h>

void process_text(locale_t loc)
{
    /* 在指定 locale 下判断宽字符类别 */
    wint_t ch = 0x4E00;  /* CJK 字符「一」 */
    if (iswalpha_l(ch, loc)) {
        printf("U+4E00 is a letter in this locale\n");
    }
    if (iswpunct_l(0xFF0C, loc)) {
        printf("U+FF0C (fullwidth comma) is punctuation\n");
    }
}

int main(void)
{
    /* 创建独立 locale_t，不影响全局 locale */
    locale_t hk_loc = newlocale(LC_ALL_MASK, "zh_HK.Big5", NULL);
    if (hk_loc == NULL) {
        fprintf(stderr, "Failed to create locale_t\n");
        return 1;
    }

    process_text(hk_loc);

    freelocale(hk_loc);
    return 0;
}
```

### 场景三：Big5 与 UTF-8 编码转换

```c
#include <iconv.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int convert_big5_to_utf8(const char *big5_in, size_t in_len, char *utf8_out, size_t out_len)
{
    iconv_t cd = iconv_open("UTF-8", "big5");
    if (cd == (iconv_t)-1) {
        fprintf(stderr, "iconv_open failed: unsupported charset\n");
        return -1;
    }

    char *inptr = (char *)big5_in;
    char *outptr = utf8_out;
    size_t inbytesleft = in_len;
    size_t outbytesleft = out_len;

    size_t result = iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft);
    if (result == (size_t)-1) {
        if (errno == EILSEQ) {
            fprintf(stderr, "Invalid Big5 sequence at offset %ld\n", inptr - big5_in);
        } else if (errno == E2BIG) {
            fprintf(stderr, "Output buffer too small\n");
        }
        iconv_close(cd);
        return -1;
    }

    iconv_close(cd);
    return out_len - outbytesleft;  /* 返回输出的字节数 */
}
```

---

## 8. 修改记录

### 26.1.0

- 新增 `zh_HK.Big5` 和 `zh_HK.Big5-HKSCS` locale 支持，加入 ICU 白名单
