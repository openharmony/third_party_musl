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
#include <stdlib.h>
#include "sampledata.h"

static const size_t DATALEN = 1024;

size_t get_sample_data_len() { return DATALEN; }
size_t *get_sample_data() {
	static char *p = 0;
	if (!p) {
	p = malloc(get_sample_data_len());
	for (int i=0; i<get_sample_data_len(); i++) {
		p[i] = i&0xff;
	}
	}
	return (size_t*)p;
}

