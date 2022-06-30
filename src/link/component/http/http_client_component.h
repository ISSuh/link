/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_

#include <string>
#include <memory>

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
  HttpClientComponent(const std::string& host, uint16_t port);
  virtual ~HttpClientComponent();

  // LinkComponent
  base::EventChannel* GetEventChannel() override;

  void InternalConnectHandler(std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  bool is_https;
  std::string url_;
  uint16_t port_;


  std::unique_ptr<io::Client> client_;

  DISAALOW_COPY_AND_ASSIGN(HttpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
