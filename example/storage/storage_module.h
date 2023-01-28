/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_STORAGE_STORAGE_MODULE_H_
#define EXAMPLE_STORAGE_STORAGE_MODULE_H_

#include <string>
#include <memory>

#include <link/module/base/user_module.h>
#include <link/storage/database/database.h>

class SampleStorageModule final : public nlink::module::UserModule {
 public:
  MODULE_CONSTRUCTOR(SampleStorageModule)

  void Init() override;
  void Run() override;
  void Shutdown() override;

 private:
  std::string storage_path_;
  std::unique_ptr<nlink::storage::Database> db_;
};

REGIST_MODULE(SampleStorageModule);

#endif  // EXAMPLE_STORAGE_STORAGE_MODULE_H_
