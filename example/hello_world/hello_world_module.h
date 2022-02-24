/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_HELLO_WORLD_MODULE_H_
#define EXAMPLE_HELLO_WORLD_MODULE_H_

#include <string>

#include <link/module/base/user_module.h>

class SampleModule final : public link::module::UserModule {
 public:
  MODULE_CONSTRUCTOR(SampleModule)

  void Init() override;
  void Run() override;
  void Shutdown() override;

 private:
  int32_t int_test_;
  float float_test_;
  bool bool_test_;
  std::string string_test_;
};

REGIST_MODULE(SampleModule);

#endif  // EXAMPLE_HELLO_WORLD_MODULE_H_
