/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "server_module.h"

#include <link/handle/link_handle.h>
#include <link/base/logging.h>

using namespace nlink;

void ExampleHttpServerModule::Init() {
  LOG(INFO) << "ExampleHttpServerModule::init";
  GetArgument("address", &address_);
  GetArgument("port", &port_);
}

void ExampleHttpServerModule::Run() {
  LOG(INFO) << "ExampleHttpServerModule::Run";
  LOG(INFO) << "address : " << address_;
  LOG(INFO) << "port : " << port_;

  handle::LinkHandle handle;
  handle.Initialize();

  handle.Run();
}

void ExampleHttpServerModule::Shutdown() {
  LOG(INFO) << "ExampleHttpServerModule::Terminate";
}
