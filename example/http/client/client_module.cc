/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "client_module.h"

#include <thread>
#include <chrono>

#include <link/handle/link_handle.h>

using namespace nlink;

void ExampleHttpClientModule::Init() {
  LOG(INFO) << "ExampleHttpClientModule::init";
  GetArgument("path_for_get", &path_for_get_);
}

void ExampleHttpClientModule::Run() {
  LOG(INFO) << "ExampleHttpClientModule::Run";
  LOG(INFO) << "path_for_get : " << path_for_get_;
  handle_.Initialize();

  http_client_.CreateAndRegistComponent(GetTaskRunner(), &handle_);

  http_client_.Get(path_for_get_);
  WaitingForResponse();
}

void ExampleHttpClientModule::Shutdown() {
  LOG(INFO) << "ExampleHttpClientModule::Terminate";
}

void ExampleHttpClientModule::WaitingForResponse() {
  while (!http_client_.IsReceivedResponse()) {
    handle_.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
