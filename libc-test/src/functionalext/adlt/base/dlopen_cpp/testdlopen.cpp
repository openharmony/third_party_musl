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
#include <cstdio>
#include <dlfcn.h>
#include <string>

#include "cpp-lib-iface.h"
#include "testdlopen.h"

namespace testdlopen {
const int defaultCtorInitValue = 0;

// open library
static void *openLib(const char *filename) {
  printf("\tLoading library %s\n", filename);
  void *handle = dlopen(filename, RTLD_LAZY);
  if (!handle) {
    printf("Error loading library!\n");
    dlerror();
  }

  return handle;
}

// close library
static void closeLib(void *handle) {
  printf("\tClosing library\n");
  dlclose(handle);
}

std::string testVer(const char *filename) {
  printf("Testing CppDynamicLib::getVersion\n");
  void *handle = openLib(filename);
  if (!handle) {
    printf("Exiting!\n");
    return "test failure";
  }
  
  printf("\tLoading symbol create\n");
  create_t* create_class = (create_t*) dlsym(handle, "create");
  printf("\tLoading symbol destroy\n");
  destroy_t* destroy_class = (destroy_t*) dlsym(handle, "destroy");
  if (!create_class || !destroy_class) {
    printf("Exiting!\n");
    return "test failure";
  }

  printf("\tInstantiating CppDynamicLib class\n");
  ICppDynamicLib* testclass = create_class(defaultCtorInitValue);

  printf("\tCalling getVerion\n");
  std::string ver = testclass->getVersion();

  printf("\tDestroying CppDynamicLib class\n");
  destroy_class(testclass);

  closeLib(handle);

  printf("Exiting getVersion test\n");
  return ver;
}

int testPrintf(const char *filename, const char *text) {
  printf("Testing CppDynamicLib testPrintf\n");
  void *handle = openLib(filename);
  if (!handle) {
    printf("Exiting!\n");
    return -1;
  }
  
  printf("\tLoading symbol create\n");
  create_t* create_class = (create_t*) dlsym(handle, "create");
  printf("\tLoading symbol destroy\n");
  destroy_t* destroy_class = (destroy_t*) dlsym(handle, "destroy");
  if (!create_class || !destroy_class) {
    printf("Exiting!\n");
    return -1;
  }

  printf("\tInstantiating CppDynamicLib class\n");
  ICppDynamicLib* testclass = create_class(defaultCtorInitValue);

  printf("\tCalling testPrintf\n");
  testclass->testPrintf(text);

  printf("\tDestroying CppDynamicLib class\n");
  destroy_class(testclass);

  closeLib(handle);

  printf("Exiting testPrintf test\n");
  return 0;
}

int testMax(const char *filename, int a, int b) {
  printf("Testing CppDynamicLib: getMax\n");
  void *handle = openLib(filename);
  if (!handle) {
    printf("Exiting!\n");
    return -1;
  }
  
  printf("\tLoading symbol create\n");
  create_t* create_class = (create_t*) dlsym(handle, "create");
  printf("\tLoading symbol destroy\n");
  destroy_t* destroy_class = (destroy_t*) dlsym(handle, "destroy");
  if (!create_class || !destroy_class) {
    printf("Exiting!\n");
    return -1;
  }

  printf("\tInstantiating CppDynamicLib class\n");
  ICppDynamicLib* testclass = create_class(defaultCtorInitValue);

  printf("\tCalling getMax\n");
  int max = testclass->getMax(a, b);

  printf("\tDestroying CppDynamicLib class\n");
  destroy_class(testclass);

  closeLib(handle);

  printf("Exiting getMax test\n");
  return max;
}

int testCtor(const char *filename, int initStorage) {
  printf("Testing getStorage\n");
  void *handle = openLib(filename);
  if (!handle) {
    printf("Exiting!\n");
    return initStorage-1;
  }
  
  printf("\tLoading symbol create\n");
  create_t* create_class = (create_t*) dlsym(handle, "create");
  printf("\tLoading symbol destroy\n");
  destroy_t* destroy_class = (destroy_t*) dlsym(handle, "destroy");
  if (!create_class || !destroy_class) {
    printf("Exiting!\n");
    return initStorage-1;
  }

  printf("\tInstantiating CppDynamicLib class\n");
  ICppDynamicLib* testclass = create_class(initStorage);

  printf("\tCalling getStorage\n");
  auto storage = testclass->getStorage();

  printf("\tDestroying CppDynamicLib class\n");
  destroy_class(testclass);

  closeLib(handle);

  printf("Exiting getStorage test\n");
  return storage;
}
} // namespace testdlopen