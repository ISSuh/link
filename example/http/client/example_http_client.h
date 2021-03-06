/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_HTTP_CLIENT_EXAMPLE_HTTP_CLIENT_H_
#define EXAMPLE_HTTP_CLIENT_EXAMPLE_HTTP_CLIENT_H_

#include <string>
#include <memory>

#include <link/base/buffer.h>
#include <link/base/task/task_runner.h>
#include <link/handle/link_handle.h>
#include <link/component/http/http_client_component.h>

class ExampleHttpClient {
 public:
  ExampleHttpClient();
  ~ExampleHttpClient();

  void CreateAndRegistComponent(
    nlink::base::TaskRunner* task_runner,
    nlink::handle::LinkHandle* handle);

  bool IsReceivedResponse() const;

  void Get(const std::string& path);

 private:
  void GetHandler(const nlink::net::http::Response& response);

  nlink::component::HttpClientComponent* client_component_;
  bool received_reponse_;
};

#endif  // EXAMPLE_HTTP_CLIENT_EXAMPLE_HTTP_CLIENT_H_
