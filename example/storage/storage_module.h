/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_STORAGE_STORAGE_MODULE_H_
#define EXAMPLE_STORAGE_STORAGE_MODULE_H_

#include <string>

#include <link/module/base/user_module.h>

class SampleStorageModule final : public nlink::module::UserModule {
 public:
  MODULE_CONSTRUCTOR(SampleStorageModule)

  void Init() override;
  void Run() override;
  void Shutdown() override;

 private:
};

REGIST_MODULE(SampleStorageModule);

#endif  // EXAMPLE_STORAGE_STORAGE_MODULE_H_
