/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "client_module.h"

#include <chrono>
#include <thread>

#include <link/handle/link_handle.h>
#include <link/component/ipc/socket/tcp_client_component.h>
#include <link/base/logging.h>

using namespace nlink;

void ExampleClientModule::Init() {
  LOG(INFO) << "ExampleClientModule::init";
  GetArgument("address", &address_);
  GetArgument("port", &port_);
}

void ExampleClientModule::Run() {
  LOG(INFO) << "ExampleClientModule::Run";
  LOG(INFO) << "address : " << address_;
  LOG(INFO) << "port : " << port_;

  handle::LinkHandle handle;
  handle.Initialize();

  component::TcpClientComponent* client =
    component::TcpClientComponent::CreateComponent();

  handle.RegistComponent(client);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  client->Connect(address_, port_);

  handle.Run();


}

void ExampleClientModule::Shutdown() {
  LOG(INFO) << "ExampleClientModule::Terminate";
}
