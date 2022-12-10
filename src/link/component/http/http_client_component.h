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
#include <functional>

#include "link/base/task/task_runner.h"
#include "link/base/callback.h"
#include "link/net/http/method.h"
#include "link/net/http/header.h"
#include "link/net/http/request.h"
#include "link/net/http/response.h"
#include "link/component/ipc/socket/tcp_client_component.h"
#include "link/component/http/http_component.h"

namespace nlink {
namespace component {

class HttpComponent;

class HttpClientComponent : public HttpComponent {
 public:
  using RequestHandler = base::Callback<void(const net::http::Response&)>;
  using ChunkHandler = base::Callback<void(const base::Buffer&)>;

  static HttpClientComponent* CreateComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner);

  void Get(const std::string& path, RequestHandler handler);
  void Get(
    const std::string& uri_string,
    const net::http::HttpHeader& header,
    RequestHandler handler);

  void Post(
    const std::string& uri_string,
    const std::string& content_type,
    const std::string& body,
    RequestHandler handler);
  void Post(
    const std::string& uri_string,
    const net::http::HttpHeader& header,
    const std::string& body,
    RequestHandler handler);

  void Put(
    const std::string& uri_string,
    const std::string& content_type,
    const std::string& body,
    RequestHandler handler);
  void Put(
    const std::string& uri_string,
    const net::http::HttpHeader& header,
    const std::string& body,
    RequestHandler handler);

  void Delete(const std::string& uri_string, RequestHandler handler);
  void Delete(
    const std::string& uri_string,
    const net::http::HttpHeader& header,
    RequestHandler handler);

  void Fetch(
    net::http::Method method,
    const std::string& uri_string,
    RequestHandler handler,
    const net::http::HttpHeader& header = net::http::HttpHeader(),
    const std::string& content_type = "",
    const std::string& body = "");

 private:
  HttpClientComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner);
  virtual ~HttpClientComponent();

  void DoFetch(
    net::http::Method method,
    const net::Uri& uri,
    const net::http::HttpHeader& header,
    RequestHandler handler);

  void DoFetchWithBody(
    net::http::Method method,
    const net::Uri& uri,
    const net::http::HttpHeader& header,
    const std::string& body,
    const std::string& content_type,
    RequestHandler handler);

  void CreateIOClientAndConnet(
    const net::http::Request& request, RequestHandler handler);

  void InternalConnectHandler(
    const net::http::Request& request,
    io::Client* client,
    std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    RequestHandler request_handler,
    const base::Buffer& buffer,
    std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  std::weak_ptr<base::TaskRunner> task_runner_;
  std::set<std::unique_ptr<io::Client>> clients_;

  bool will_response_chunk_;

  DISAALOW_COPY_AND_ASSIGN(HttpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
