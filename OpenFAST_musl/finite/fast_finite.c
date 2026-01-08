/**
* Copyright (C) 2026 Huawei Device Co., Ltd.
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
#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MUSL_EXTERNAL_FUNCTION
#if defined(__aarch64__)
double __log_finite(double x) {return log(x);}
double __log2_finite(double x) {return log2(x);}
double __atan2_finite(double y, double x) {return atan2(y,x);}
double __pow_finite(double x, double y) {return pow(x,y);}
double __log10_finite(double x) {return log10(x);}
double __exp_finite(double x) {return exp(x);}
double __exp2_finite(double x) {return exp2(x);}

float __acosf_finite(float x) {return acosf(x);}
float __powf_finite(float x, float y) {return powf(x,y);}
float __log2f_finite(float x) {return log2f(x);}
float exp2f_finite(float x) {return exp2f(x);}
#endif
#endif

#ifdef __cplusplus
}
#endif