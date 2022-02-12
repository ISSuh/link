/**
 * Copyright 2020 The ISSuh Authors. All Rights Reserved.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include "hello_world_module.hpp"

void SampleModule::init() {
  std::cout << "SampleModule::init()" << '\n';

  getArgument<int>("int_test", m_int_test);
  getArgument<float>("float_test", m_float_test);
  getArgument<bool>("bool_test", m_bool_test);
  getArgument<std::string>("string_test", m_string_test);
}

void SampleModule::run() {
  std::cout << "SampleModule::init()" << '\n';

  std::cout << m_int_test << std::endl;
  std::cout << m_float_test << std::endl;
  std::cout << m_bool_test << std::endl;
  std::cout << m_string_test << std::endl;
}

void SampleModule::terminate() {
  std::cout << "SampleModule::terminate()" << '\n';
}
