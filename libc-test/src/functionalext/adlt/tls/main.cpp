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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "functionalext.h"
#include "common.h"
#include <securec.h>

extern "C" {
  bool waitFlag;
}

#include "aCpp.h"

#define THR_DEBUG 0

namespace ver {

namespace C {
extern "C" {
#include "aC.h"
void *mainThreadFunc(void *param);
const c_array_t initData = {C_INIT};
__thread c_array_t mainTlsData = {C_INIT};
};
} // namespace C

namespace CPP {
void mainThreadFunc(void *param);
const cpp_data_t initData = {CPP_TLS_DATA_INIT};
thread_local cpp_data_t mainTlsData = {CPP_TLS_DATA_INIT};
} // namespace CPP

} // namespace ver

// external callbacks
void *ver::C::mainThreadFunc(void *param) {
  threadparam_t *threadData = (threadparam_t *)param;
  for(size_t i = 0; i < C_ARRAY_LEN; ++i) {
    ver::C::mainTlsData[i] += static_cast<c_data_t>(threadData->data) + static_cast<c_data_t>(i);
  }

#if THR_DEBUG
  unsigned long id = (unsigned long)pthread_self();
  printf("C main: Thread 0x%lx: Entered! id: %d TlsData: %g\n",
         id, threadData->id, (double)ver::C::mainTlsData[C_ARRAY_LEN - 1]);
#endif

  while (waitFlag) {
    sched_yield();
  }

  for(size_t i = 0; i < C_ARRAY_LEN; ++i) {
    ver::C::mainTlsData[i] += 20;
  }

  *(c_data_t *)threadData->dest = ver::C::mainTlsData[C_ARRAY_LEN - 1];

#if THR_DEBUG
  printf("C main: Thread 0x%lx: End loop! id: %d Result: %g\n",
         id, threadData->id, (double)(*(c_data_t *)threadData->dest));
#endif
  return nullptr;
}

void ver::CPP::mainThreadFunc(void *param) {
  threadparam_t *threadData = static_cast<threadparam_t *>(param);
  ver::CPP::mainTlsData += static_cast<cpp_data_t>(threadData->data);

#if THR_DEBUG
  std::__thread_id id = std::this_thread::get_id();
  std::cout << "CPP main: "
            << "Thread 0x" << std::hex << id << ": Entered! "
            << "id: " << threadData->id << " "
            << "TlsData: " << ver::CPP::mainTlsData << "\n";
#endif

  while (waitFlag) {
    std::this_thread::yield();
  }

  ver::CPP::mainTlsData += 20;
  *(cpp_data_t *)threadData->dest = ver::CPP::mainTlsData;

#if THR_DEBUG
  std::cout << "CPP main: "
            << "Thread 0x" << std::hex << id << ": End loop! "
            << "id: " << threadData->id << " "
            << "Result: " << *(cpp_data_t *)threadData->dest << "\n";
#endif
}

// tests
const int NUM_THREADS = 4;
c_data_t resultsC[NUM_THREADS];
cpp_data_t resultsCPP[NUM_THREADS];

static void TLS_internal_callback() {
  int dataC[NUM_THREADS] = {200, 2, 5, 7};
  int dataCPP[NUM_THREADS] = {201, 3, 7, 8};
  memset_s(resultsC, NUM_THREADS * sizeof(c_data_t), 0, NUM_THREADS * sizeof(c_data_t));
  memset_s(resultsCPP, NUM_THREADS * sizeof(cpp_data_t), 0, NUM_THREADS * sizeof(cpp_data_t));

  waitFlag = true;
  ver::C::startThreads(NUM_THREADS, dataC, nullptr, resultsC);
  ver::CPP::startThreads(NUM_THREADS, dataCPP, nullptr, resultsCPP);

  waitFlag = false;
  ver::C::stopThreads();
  ver::CPP::stopThreads();

  for (int i = 0; i < NUM_THREADS; i++) {
    EXPECT_EQ(__func__, resultsC[i],
      static_cast<c_data_t>(dataC[i] + ver::C::initData[C_ARRAY_LEN - 1] + C_ARRAY_LEN - 1 + 1));
    EXPECT_EQ(__func__, resultsCPP[i], static_cast<cpp_data_t>(dataCPP[i] + ver::CPP::initData + 1));
  }
}

static void TLS_external_callback() {
  int dataC[NUM_THREADS] = {100, 20, 50, 70};
  int dataCPP[NUM_THREADS] = {101, 21, 51, 71};
  memset_s(resultsC, NUM_THREADS * sizeof(c_data_t), 0, NUM_THREADS * sizeof(c_data_t));
  memset_s(resultsCPP, NUM_THREADS * sizeof(cpp_data_t), 0, NUM_THREADS * sizeof(cpp_data_t));

  waitFlag = true;
  ver::C::startThreads(NUM_THREADS, dataC, ver::C::mainThreadFunc, resultsC);
  ver::CPP::startThreads(NUM_THREADS, dataCPP, ver::CPP::mainThreadFunc,
                         resultsCPP);

  waitFlag = false;
  ver::C::stopThreads();
  ver::CPP::stopThreads();

  for (int i = 0; i < NUM_THREADS; i++) {
    EXPECT_EQ(__func__, resultsC[i],
      static_cast<c_data_t>(dataC[i] + ver::C::initData[C_ARRAY_LEN - 1] + C_ARRAY_LEN - 1 + 20));
    EXPECT_EQ(__func__, resultsCPP[i], static_cast<cpp_data_t>(dataCPP[i] + ver::CPP::initData + 20));
  }
}

int main(int argc, char **argv) {
  TLS_internal_callback();
  TLS_external_callback();
  return t_status;
}
