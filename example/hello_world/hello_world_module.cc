/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "hello_world_module.h"

#include <link/base/logging.h>

#include <link/net/base/url.h>

using namespace nlink;

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

  net::Url url;
  const std::string http_url =
    "https://user:password@www.google.com:443/webhp?gws_rd=ssl";

  url.Decode(http_url);
}

void SampleModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
