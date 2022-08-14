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
}

void ExampleHttpServerModule::Run() {
  LOG(INFO) << "ExampleHttpServerModule::Run";
  std::string address;
  int32_t port;
  GetArgument("address", &address);
  GetArgument("port", &port);

  handle::LinkHandle handle;
  handle.Initialize();

  ExampleHttpServer http_server(GetTaskRunner(), &handle);
  http_server.OpenServer(address, port);

  LOG(INFO) << "Open Example Http Server(" << address << ":" << port << ")";

  handle.Run();

  http_server.CloseServer();
  handle.Shutdown();
}

void ExampleHttpServerModule::Shutdown() {
  LOG(INFO) << "ExampleHttpServerModule::Terminate";
}
