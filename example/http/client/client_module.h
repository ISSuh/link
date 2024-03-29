/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_HTTP_CLIENT_CLIENT_MODULE_H_
#define EXAMPLE_HTTP_CLIENT_CLIENT_MODULE_H_

#include <string>

#include <link/module/base/user_module.h>

#include "example_http_client.h"

class ExampleHttpClientModule final : public nlink::module::UserModule {
 public:
  MODULE_CONSTRUCTOR(ExampleHttpClientModule)

  void Init() override;
  void Run() override;
  void Shutdown() override;

 private:
  void WaitingForResponse();

  nlink::handle::LinkHandle handle_;
  ExampleHttpClient http_client_;
};

REGIST_MODULE(ExampleHttpClientModule);

#endif  // EXAMPLE_HTTP_CLIENT_CLIENT_MODULE_H_
