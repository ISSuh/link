/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_HTTP_SERVER_EXAMPLE_HTTP_SERVER_H_
#define EXAMPLE_HTTP_SERVER_EXAMPLE_HTTP_SERVER_H_

#include <string>
#include <memory>

#include <link/base/buffer.h>
#include <link/base/task/task_runner.h>
#include <link/handle/link_handle.h>
#include <link/component/http/http_server_component.h>
#include <link/component/http/http_component.h>

class TestHandler {
 public:
  void HelloWorld(
    const nlink::net::http::Request&, nlink::net::http::Response*);
  void User(const nlink::net::http::Request&, nlink::net::http::Response*);

 private:
  uint64_t user_id_;
};

class ExampleHttpServer {
 public:
  ExampleHttpServer(
    nlink::base::TaskRunner* task_runner,
    nlink::handle::LinkHandle* handle);
  ~ExampleHttpServer();

  void OpenServer(const std::string& address, int32_t port);
  void CloseServer();

  void RegistRoute();

 private:
  nlink::component::HttpServerComponent* server_component_;
  TestHandler test_handler_;
};

#endif  // EXAMPLE_HTTP_SERVER_EXAMPLE_HTTP_SERVER_H_
