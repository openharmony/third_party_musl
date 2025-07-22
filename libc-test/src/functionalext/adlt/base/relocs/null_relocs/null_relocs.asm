/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
	.text


	.globl	none_test
	.p2align	2
	.type	none_test,@function
none_test:
	.reloc 0, R_AARCH64_NONE, i
	mov	w0, wzr
	ret
.Lfunc_end0:
	.size	none_test, .Lfunc_end0-none_test


	.type	i,@object
	.data
	.globl	i
	.p2align	2
i:
	.word	1
	.size	i, 4


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym i
