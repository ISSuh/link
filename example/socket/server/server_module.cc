/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "server_module.h"

#include <link/handle/link_handle.h>
#include <link/component/ipc/socket/tcp_server_component.h>
#include <link/base/logging.h>

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

  component::TcpServerComponent* server =
    component::TcpServerComponent::CreateComponent();

  handle.RegistComponent(server);

  server->Open(address_, port_);

  handle.Run();
}

void ExampleServerModule::Shutdown() {
  LOG(INFO) << "ExampleServerModule::Terminate";
}
