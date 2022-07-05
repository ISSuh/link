/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_

#include <vector>
#include <string>
#include <set>
#include <memory>

#include "link/base/task/task_runner.h"
#include "link/base/callback/callback.h"
#include "link/component/http/http_component.h"
#include "link/net/http/method.h"
#include "link/net/http/header.h"
#include "link/net/http/request.h"
#include "link/net/http/response.h"
#include "link/component/ipc/socket/tcp_client_component.h"

namespace nlink {
namespace component {

class HttpComponent;

class HttpClientComponent : public HttpComponent {
 public:
  using RequestHanelder = base::Callback<void(const net::http::Response&)>;

  static HttpClientComponent* CreateComponent(
    base::TaskRunner* task_runner);

  void Get(const std::string& path, RequestHanelder handler);
  void Get(
    const std::string& path,
    const net::http::HttpHeader& header,
    RequestHanelder handler);

  void Post(
    const std::string& path,
    const std::string& content_type,
    const std::string& body,
    RequestHanelder handler);
  void Post(
    const std::string& path,
    const net::http::HttpHeader& header,
    const std::string& content_type,
    const std::string& body,
    RequestHanelder handler);

  void Put(const std::string& path, RequestHanelder handler);
  void Put(
    const std::string& path,
    const net::http::HttpHeader& header,
    RequestHanelder handler);

  void Delete(const std::string& path, RequestHanelder handler);
  void Delete(
    const std::string& path,
    const net::http::HttpHeader& header,
    RequestHanelder handler);

  void Options(const std::string& path, RequestHanelder handler);
  void Options(
    const std::string& path,
    const net::http::HttpHeader& header,
    RequestHanelder handler);

  void Fetch(
    net::http::Method method,
    const std::string& path,
    RequestHanelder handler,
    const net::http::HttpHeader& header = net::http::HttpHeader(),
    const std::string& content_type = "",
    const std::string& body = "");

 private:
  explicit HttpClientComponent(base::TaskRunner* task_runner);
  virtual ~HttpClientComponent();

  // std::unique_ptr<io::Client> CreateIOClientAndConnet(
  //   const std::string& address, int32_t port, RequestHanelder request_handler);

  void InternalConnectHandler(
    const net::http::Request& request,
    io::Client* client,
    std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    RequestHanelder request_handler,
    const base::Buffer& buffer,
    std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  base::TaskRunner* task_runner_;
  std::set<std::unique_ptr<io::Client>> clients_;

  DISAALOW_COPY_AND_ASSIGN(HttpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
