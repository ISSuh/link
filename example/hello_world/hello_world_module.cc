/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "hello_world_module.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <link/net/socket/tcp_server_socket.h>
#include <link/handle/link_handle.h>
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

  handle::LinkHandle handle;
  handle.Initialize();

  net::TcpServerSocket server;
  net::IpEndPoint end_point("0.0.0.0", 33660);
  server.Listen(end_point, 10);

  handle.RegistEventObserver(&server);

  // auto tcp_server_component = component::CreateComponent();
  // handle.RegistComponents({tcp_server_component});

  while (true) {
    handle.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void SampleModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
