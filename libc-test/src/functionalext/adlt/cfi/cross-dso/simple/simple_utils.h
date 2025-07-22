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
#ifndef _ADLT_CFI_CROSS_DSO_SIMPLE_UTILS_H_
#define _ADLT_CFI_CROSS_DSO_SIMPLE_UTILS_H_

static inline void mem_barrier(void *arg) {
  __asm__ __volatile__("" : : "r"(arg) : "memory");
}

template <typename T, unsigned I> struct Deriver : T {
  Deriver() { mem_barrier(new Deriver<T, I - 1>); }
  virtual void a() {}
  virtual void b() {}
  virtual void c() {}
};

template <typename T> struct Deriver<T, 0> : T {
  virtual void a() {}
  void b() {}
};

template <typename T> struct Deriver<T, 1> : T {
  Deriver() { mem_barrier(new Deriver<T, 0>); }
  virtual void a() {}
  virtual void b() {}
  virtual void c() {}
  virtual void d() {}
  virtual void e() {}
  virtual void f() {}
  virtual void g() {}
};

template <typename T> void make_derive() {
#ifdef B32
  mem_barrier(new Deriver<T, 10>);
#endif

#ifdef B64
  mem_barrier(new Deriver<T, 25>);
#endif

#ifdef BM
  mem_barrier(new Deriver<T, 40>);
#endif
}

#endif
