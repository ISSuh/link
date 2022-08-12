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

  while (client_.IsRunning()) {
    handle.RunOnce();
  }

  client_.Disconnect();
}

void ExampleClientModule::Shutdown() {
  LOG(INFO) << "ExampleClientModule::Terminate";
}
