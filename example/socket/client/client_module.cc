/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "client_module.h"

#include <string>
#include <thread>
#include <chrono>

#include <link/handle/link_handle.h>
#include <link/base/logging.h>

using namespace nlink;

const int32_t kSecond = 100;
const int32_t KSleepInterval = 1 * kSecond;

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

  client_.CreateAndRegistComponent(GetTaskRunner(), &handle);
  client_.Connect(address_, port_);

  int32_t count = 0;
  while (count < 1) {
    handle.RunOnce();
    if (!client_.IsConnected()) {
      LOG(INFO) << "[ExampleClientModule] wait for connect";
      continue;
    }

    // std::string message = "test-" + std::to_string(count);
    const uint32_t message_size = 10 * 1024 * 1024;
    std::string message(message_size, 'a');
    client_.Write(message);
    LOG(INFO) << "[ExampleClientModule] write : " << message.size();

    ++count;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  client_.Disconnect();
}

void ExampleClientModule::Shutdown() {
  LOG(INFO) << "ExampleClientModule::Terminate";
}
