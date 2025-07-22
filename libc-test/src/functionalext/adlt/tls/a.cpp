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
#include <chrono>
#include <iostream>
#include <thread>
#include "common.h"

#include "aCpp.h"

#define THR_DEBUG 0

static std::thread *threads = nullptr;
static int NUM_THREADS = 0;
static threadparam_t *tData = nullptr;

thread_local cpp_data_t cppTlsData = CPP_TLS_DATA_INIT;

void ver::CPP::threadFunc(void *param) {
  threadparam_t *threadData = static_cast<threadparam_t *>(param);
  cppTlsData += static_cast<cpp_data_t>(threadData->data);

#if THR_DEBUG
  std::__thread_id id = std::this_thread::get_id();
  std::cout << "CPP: Thread 0x" << std::hex << id << std::dec << ": "
            << "Entered! id: " << threadData->id << " " 
            << "cppTlsData: " << cppTlsData
            << "\n";
#endif

  while (waitFlag) {
    std::this_thread::yield();
  }

  cppTlsData++;
  *(cpp_data_t *)threadData->dest = cppTlsData;

#if THR_DEBUG
  std::cout << "CPP: Thread 0x" << std::hex << id << std::dec << ": "
            << "End loop! id: " << threadData->id << " " 
            << "Result: " << *(cpp_data_t *)threadData->dest
            << "\n";
#endif
}

void ver::CPP::startThreads(int num, int *data, callback_cpp_t callback,
                            cpp_data_t *outResults) {
  threads = new std::thread[num];
  tData = new threadparam_t[num];
  if (!callback) {
    callback = threadFunc;
  }
  NUM_THREADS = num;

  for (int i = 0; i < num; i++) {
#if THR_DEBUG
    std::cout << "CPP: Starting thread (id: " << std::dec << i << "): "
              << "with data: " << data[i] 
              << "\n";
#endif
    tData[i] =
        (threadparam_t){.id = i, .data = data[i], .dest = &outResults[i]};
    threads[i] = std::thread(callback, &tData[i]);
  }
}

void ver::CPP::stopThreads() {
  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].join();
  }
  delete[] threads;
  delete[] tData;

  threads = nullptr;
  tData = nullptr;
}
