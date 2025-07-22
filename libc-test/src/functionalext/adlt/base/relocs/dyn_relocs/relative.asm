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


	.globl	relative_test
	.p2align	2
	.type	relative_test,@function
relative_test:
	adrp	x8, :got:relative_var1
	ldr	x8, [x8, :got_lo12:relative_var1]
	ldr	w8, [x8]

	adrp	x10, :got:relative_struct1
	ldr	x10, [x10, :got_lo12:relative_struct1]
	ldr	x9, [x10]
	ldr	w9, [x9]
	ldr	x10, [x10, #8]
	ldr	w10, [x10]

	adrp	x12, :got:relative_struct2
	ldr	x12, [x12, :got_lo12:relative_struct2]
	ldr	x11, [x12]
	ldr	w11, [x11]
	ldr	x12, [x12, #8]
	ldr	w12, [x12]

	add	w8, w8, w9
	add	w8, w8, w10
	add	w8, w8, w11
	add	w0, w8, w12
	ret
.Lfunc_end0:
	.size	relative_test, .Lfunc_end0-relative_test


	.type	relative_var1,@object
	.data
	.p2align	2
relative_var1:
	.word	1
	.size	relative_var1, 4


	.type	_ZL17relative_var1_tmp,@object
	.p2align	2
_ZL17relative_var1_tmp:
	.word	1
	.size	_ZL17relative_var1_tmp, 4


	.type	_ZL17relative_var2_tmp,@object
	.p2align	2
_ZL17relative_var2_tmp:
	.word	1
	.size	_ZL17relative_var2_tmp, 4


	.type	relative_struct1,@object
	.globl	relative_struct1
	.p2align	3
relative_struct1:
	.xword	_ZL17relative_var1_tmp
	.xword	_ZL17relative_var2_tmp
	.size	relative_struct1, 16


	.type	relative_struct2,@object
	.globl	relative_struct2
	.p2align	3
relative_struct2:
	.quad	_ZL17relative_var1_tmp
	.quad	_ZL17relative_var2_tmp
	.size	relative_struct2, 16


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym relative_var1
	.addrsig_sym _ZL17relative_var1_tmp
	.addrsig_sym _ZL17relative_var2_tmp
	.addrsig_sym relative_struct1
	.addrsig_sym relative_struct2
