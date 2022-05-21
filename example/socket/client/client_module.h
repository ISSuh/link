/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_SOCKET_CLIENT_SERVER_CLIENT_MODULE_H_
#define EXAMPLE_SOCKET_CLIENT_SERVER_CLIENT_MODULE_H_

#include <string>

#include <link/module/base/user_module.h>

class ExampleClientModule final : public nlink::module::UserModule {
 public:
  MODULE_CONSTRUCTOR(ExampleClientModule)

  void Init() override;
  void Run() override;
  void Shutdown() override;

 private:
  std::string address_;
  int32_t port_;
};

REGIST_MODULE(ExampleClientModule);

#endif  // EXAMPLE_SOCKET_CLIENT_SERVER_CLIENT_MODULE_H_
