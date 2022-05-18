/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "hello_world_module.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <link/handle/link_handle.h>
#include <link/component/rpc/rpc_server_component.h>
#include <link/base/logging.h>

using namespace nlink;

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

  handle::LinkHandle handle;
  handle.Initialize();

  component::RpcServerComponent* rpc_server =
    component::RpcServerComponent::CreateComponent();

  handle.RegistComponent(rpc_server);

  rpc_server->Open();

  handle.Run();
}

void SampleModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
