/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "client_module.h"

#include <thread>
#include <chrono>

#include <link/handle/link_handle.h>
#include <link/base/json_wrapper.h>

using namespace nlink;

void ExampleHttpClientModule::Init() {
  LOG(INFO) << "ExampleHttpClientModule::init";
}

void ExampleHttpClientModule::Run() {
  LOG(INFO) << "ExampleHttpClientModule::Run";
  std::string path_for_get;
  std::string path_for_post;
  GetArgument("path_for_get", &path_for_get);
  GetArgument("path_for_post", &path_for_post);

  handle_.Initialize();

  http_client_.CreateAndRegistComponent(GetTaskRunner(), &handle_);

  http_client_.Get(path_for_get);


  base::Json json = {
    {"Id", 12345},
    {"Customer", "John Smith"},
    {"Quantity", 1},
    {"Price", 10.0}};

  std::string body(json.dump(4));
  http_client_.Post(path_for_post, "application/json", body);
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
