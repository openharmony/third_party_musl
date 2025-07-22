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
#include "functionalext.h"
#include "constant.h"
#include "errexit.h"
#include "sampledata.h"
#include "foo.h"

static void LdMuslDlOpenDirestTests_Test0() {    
    EXPECT_EQ(__func__, CONSTANT, get_constant());
    set_buf(get_sample_data(), get_sample_data_len() );
    size_t len = get_buflen();
    EXPECT_EQ(__func__, len, get_sample_data_len());
    EXPECT_EQ(__func__, 0, cmp_buf(get_sample_data()));
}

int main(int argc, char **argv)
{
    LdMuslDlOpenDirestTests_Test0();
    return t_status;
}

