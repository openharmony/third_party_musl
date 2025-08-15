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


	.globl	call26_f1
	.p2align	2
	.type	call26_f1,@function
call26_f1:
	mov	w0, #1
	ret
.Lfunc_end0:
	.size	call26_f1, .Lfunc_end0-call26_f1


	.globl	call26_test
	.p2align	2
	.type	call26_test,@function
call26_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	bl	call26_f1
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	call26_test, .Lfunc_end1-call26_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym call26_f1
