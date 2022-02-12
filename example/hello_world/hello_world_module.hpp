/**
 * Copyright 2020 The ISSuh Authors. All Rights Reserved.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#ifndef EXAMPLE_HELLO_WORLD_HELLO_WORLD_MODULE_HPP_
#define EXAMPLE_HELLO_WORLD_HELLO_WORLD_MODULE_HPP_

#include <string>

#include <smt/module/Module.hpp>

class SampleModule final : public link::module::UserModule {
 public:
  void init() override;
  void run() override;
  void terminate() override;

 private:
  int m_int_test;
  float m_float_test;
  bool m_bool_test;
  std::string m_string_test;
};

REGIST_MODULE(SampleModule);

#endif  // EXAMPLE_HELLO_WORLD_HELLO_WORLD_MODULE_HPP_
