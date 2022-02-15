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
  void Init() override;
  void Run() override;
  void Terminate() override;

 private:
  int m_int_test;
  float m_float_test;
  bool m_bool_test;
  std::string m_string_test;
};

REGIST_MODULE(SampleModule);

#endif  // EXAMPLE_HELLO_WORLD_MODULE_H_
