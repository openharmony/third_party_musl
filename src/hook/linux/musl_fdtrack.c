/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack.h"
#include "stdatomic_impl.h"

_Atomic(fdtrack_hook) __fdtrack_hook;
bool __fdtrack_enabled = false;

void set_fdtrack_enabled(bool newValue)
{
	__fdtrack_enabled = newValue;
}

bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value)
{
	return atomic_compare_exchange_strong(&__fdtrack_hook, expected, value);
}

#endif