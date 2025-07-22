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
#include <string.h>
#include <unistd.h>

#include <stdbool.h>
#include "common.h"

#include "aC.h"

#define THR_DEBUG 0

static pthread_t *threads = NULL;
static int NUM_THREADS = 0;
static threadparam_t *tData = NULL;

__thread c_array_t cTlsData = {C_INIT};

static void *threadFunc(void *param) {
  threadparam_t *threadData = (threadparam_t *)param;    
  for(size_t i = 0; i < C_ARRAY_LEN; ++i) {
    cTlsData[i] += (c_data_t)threadData->data + (c_data_t)i;
  }

#if THR_DEBUG
  unsigned long id = (unsigned long)pthread_self();
  printf("C: Thread 0x%lx: Entered! id: %d cTlsData: %g\n", id, threadData->id,
         (double)cTlsData[C_ARRAY_LEN - 1]);
#endif

  while (waitFlag) {
    sched_yield();
  }

  for(size_t i = 0; i < C_ARRAY_LEN; ++i) {
    cTlsData[i]++;
  }

  *(c_data_t *)threadData->dest = cTlsData[C_ARRAY_LEN - 1];

#if THR_DEBUG
  printf("C: Thread 0x%lx: End loop! id: %d Result: %g\n", id, threadData->id,
         (double)(*(c_data_t *)threadData->dest));
#endif

  return NULL;
}

void startThreads(const int num, int *data, callback_t callback, c_data_t *outResults) {
  threads = (pthread_t *)malloc(num * sizeof(pthread_t));
  tData = (threadparam_t *)malloc(num * sizeof(threadparam_t));
  if (!callback) {
    callback = threadFunc;
  }

  NUM_THREADS = num;

  int rc = -1;
  for (int i = 0; i < NUM_THREADS; i++) {
#if THR_DEBUG
    printf("C: Starting thread (id: %d): with data: %d\n", i, data[i]);
#endif
    tData[i] =
        (threadparam_t){.id = i, .data = data[i], .dest = &outResults[i]};
    rc = pthread_create(&threads[i], NULL, callback, &tData[i]);
    if (!rc) {
      continue;
    }

    printf("Failed create thread (%d): %d", i, rc);
    exit(1);
  }
}

void stopThreads() {
  int rc = -1;
  for (int i = 0; i < NUM_THREADS; i++) {
    rc = pthread_join(threads[i], NULL);
    if (!rc) {
      continue;
    }

    printf("Failed stop thread (%d): %d", i, rc);
    exit(1);
  }

  free(threads);
  free(tData);

  threads = NULL;
  tData = NULL;
}
