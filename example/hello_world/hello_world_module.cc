/**
 * Copyright 2020 The ISSuh Authors. All Rights Reserved.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */
#include "hello_world_module.h"

#include <iostream>

#include <link/base/logging.h>

using namespace link;

void SampleModule::Init() {
  LOG(INFO) << "SampleModule::init";

  GetArgument("int_test", &int_test_);
  GetArgument("float_test", &float_test_);
  GetArgument("bool_test", &bool_test_);
  GetArgument("string_test", &string_test_);
}

void SampleModule::Run() {
  LOG(INFO) << "SampleModule::Run";

  LOG(INFO) << "int_test_ : " << int_test_;
  LOG(INFO) << "float_test_ : " << float_test_;
  LOG(INFO) << "bool_test_ : " << bool_test_;
  LOG(INFO) << "string_test_ : " << string_test_;

  regist(http_component)
  regist(rpc_component)

  while () { 
    publish()
    SpinOnce()

  }
}

void SampleModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
