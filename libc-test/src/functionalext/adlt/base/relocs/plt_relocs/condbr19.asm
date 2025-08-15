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


	.globl	condbr19_f1
	.p2align	2
	.type	condbr19_f1,@function
condbr19_f1:
	b.eq condbr19_label
.Lfunc_end0:
	.size	condbr19_f1, .Lfunc_end0-condbr19_f1


.globl	condbr19_label
.p2align	2
condbr19_label:
	mov	w0, #1
	ret


	.globl	condbr19_test
	.p2align	2
	.type	condbr19_test,@function
condbr19_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	bl	condbr19_f1
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	condbr19_test, .Lfunc_end1-condbr19_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym condbr19_f1
