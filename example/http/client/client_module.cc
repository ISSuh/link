/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "client_module.h"

#include <link/handle/link_handle.h>

using namespace nlink;

void ExampleHttpClientModule::Init() {
  LOG(INFO) << "ExampleHttpClientModule::init";
  GetArgument("address", &address_);
  GetArgument("port", &port_);
}

void ExampleHttpClientModule::Run() {
  LOG(INFO) << "ExampleHttpClientModule::Run";
  LOG(INFO) << "address : " << address_;
  LOG(INFO) << "port : " << port_;

  handle::LinkHandle handle;
  handle.Initialize();

  handle.Run();
}

void ExampleHttpClientModule::Shutdown() {
  LOG(INFO) << "ExampleHttpClientModule::Terminate";
}
