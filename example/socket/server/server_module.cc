/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "server_module.h"

#include <link/handle/link_handle.h>
#include <link/base/logging.h>

#include "example_server.h"

using namespace nlink;

void ExampleServerModule::Init() {
  LOG(INFO) << "ExampleServerModule::init";
  GetArgument("address", &address_);
  GetArgument("port", &port_);
}

void ExampleServerModule::Run() {
  LOG(INFO) << "ExampleServerModule::Run";
  LOG(INFO) << "address : " << address_;
  LOG(INFO) << "port : " << port_;

  handle::LinkHandle handle;
  handle.Initialize();

  ExampleServer server;
  server.RegistComponent(&handle);
  server.ServerOpen(address_, port_);

  handle.Run();
}

void ExampleServerModule::Shutdown() {
  LOG(INFO) << "ExampleServerModule::Terminate";
}
