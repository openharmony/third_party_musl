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


	.globl	abs64_test
	.p2align	2
	.type	abs64_test,@function
abs64_test:
	adrp	x8, :got:abs64_ptr1
	ldr	x8, [x8, :got_lo12:abs64_ptr1]
	ldr	x8, [x8]
	ldr	w8, [x8]
	adrp	x9, :got:abs64_ptr2
	ldr	x9, [x9, :got_lo12:abs64_ptr2]
	ldr	x9, [x9]
	ldr	w9, [x9]
	add	w0, w8, w9
	ret
.Lfunc_end0:
	.size	abs64_test, .Lfunc_end0-abs64_test


	.type	abs64_arr,@object
	.data
	.globl	abs64_arr
	.p2align	2
abs64_arr:
	.word	1
	.word	2
	.size	abs64_arr, 8


	.type	abs64_ptr1,@object
	.globl	abs64_ptr1
	.p2align	3
abs64_ptr1:
	.xword	abs64_arr
	.size	abs64_ptr1, 8


	.type	abs64_ptr2,@object
	.globl	abs64_ptr2
	.p2align	3
abs64_ptr2:
	.xword	abs64_arr+4
	.size	abs64_ptr2, 8



	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym abs64_arr
	.addrsig_sym abs64_ptr1
	.addrsig_sym abs64_ptr2
