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


	.globl	ld64_got_lo12_nc_test
	.p2align	2
	.type	ld64_got_lo12_nc_test,@function
ld64_got_lo12_nc_test:
	adrp	x8, :got:ld64_got_lo12_nc_var // R_AARCH64_ADR_GOT_PAGE
	ldr	x8, [x8, :got_lo12:ld64_got_lo12_nc_var] // R_AARCH64_LD64_GOT_LO12_NC
	ldr	w0, [x8]
	ret
.Lfunc_end0:
	.size	ld64_got_lo12_nc_test, .Lfunc_end0-ld64_got_lo12_nc_test


	.type	ld64_got_lo12_nc_var,@object
	.data
	.globl	ld64_got_lo12_nc_var
	.p2align	2
ld64_got_lo12_nc_var:
	.word	1
	.size	ld64_got_lo12_nc_var, 4


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym ld64_got_lo12_nc_var
