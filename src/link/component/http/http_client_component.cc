/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_client_component.h"

#include <utility>

#include "link/base/logging.h"
#include "link/io/socket/asio/tcp_client.h"
#include "link/net/base/uri.h"
#include "link/net/http/response_parser.h"

namespace nlink {
namespace component {

HttpClientComponent* HttpClientComponent::CreateComponent(
  base::TaskRunner* task_runner) {
  return new HttpClientComponent(task_runner);
}

HttpClientComponent::HttpClientComponent(
  base::TaskRunner* task_runner)
  : task_runner_(task_runner) {
}

HttpClientComponent::~HttpClientComponent() {
}

void HttpClientComponent::Get(
  const std::string& url_string, RequestHanelder handler) {
  net::Uri uri = net::Uri::Parse(url_string);
  if (!uri.HasScheme() || !uri.HasHost() || !uri.HasPort()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::Request request(net::http::Method::GET, uri);

  io::Client* client = new io::TcpClient(task_runner_);
  LinkComponent::AttachChannelsToObserver(client);

  client->RegistIOHandler(
    base::Bind(
      &HttpClientComponent::InternalReadHandler, this, handler),
    base::Bind(&HttpClientComponent::InternalWriteHandler, this));

  client->Connect(
    io::IpEndPoint(uri.Host(), uri.Port()),
    base::Bind(
      &HttpClientComponent::InternalConnectHandler, this,
        request, client),
    base::Bind(&HttpClientComponent::InternalCloseHandler, this));
}

void HttpClientComponent::Get(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Post(
  const std::string& url_string,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {

}

void HttpClientComponent::Post(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {

}

void HttpClientComponent::Put(
  const std::string& url_string, RequestHanelder handler) {

}

void HttpClientComponent::Put(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Delete(
  const std::string& url_string, RequestHanelder handler) {

}

void HttpClientComponent::Delete(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Options(
  const std::string& url_string, RequestHanelder handler) {
  
}

void HttpClientComponent::Options(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {
  
}

void HttpClientComponent::Fetch(
  net::http::Method method,
  const std::string& url_string,
  RequestHanelder handler,
  const net::http::HttpHeader& header,
  const std::string& content_type,
  const std::string& body) {
  switch (method) {
  case net::http::Method::GET:
    Get(url_string, header, handler);
    break;
  case net::http::Method::POST:
    Post(url_string, header, content_type, body, handler);
    break;
  case net::http::Method::PUT:
    Put(url_string, header, handler);
    break;
  case net::http::Method::DELETE:
    Delete(url_string, header, handler);
    break;
  default:
    LOG(WARNING) << "[HttpClientComponent::Fetch] unsupport method. "
                 << MethodToString(method);
    break;
  }
}

// std::unique_ptr<io::Client> HttpClientComponent::CreateIOClientAndConnet(
//   const std::string& address, int32_t port, RequestHanelder request_handler) {
//   std::unique_ptr<io::Client> client(new io::TcpClient(task_runner_));

//   LinkComponent::AttachChannelsToObserver(client.get());

//   client->RegistIOHandler(
//     base::Bind(
//       &HttpClientComponent::InternalReadHandler, this, request_handler),
//     base::Bind(&HttpClientComponent::InternalWriteHandler, this));

//   client->Connect(
//     io::IpEndPoint(address, port),
//     base::Bind(&HttpClientComponent::InternalConnectHandler, this),
//     base::Bind(&HttpClientComponent::InternalCloseHandler, this));

//   return std::move(client);
// }

void HttpClientComponent::InternalConnectHandler(
  const net::http::Request& request,
  io::Client* client,
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }

  base::Buffer buffer(request.Serialize());
  client->Write(buffer);

  std::unique_ptr<io::Client> client_ptr;
  client_ptr.reset(client);
  clients_.insert(std::move(client_ptr));
}

void HttpClientComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }
}

void HttpClientComponent::InternalReadHandler(
  RequestHanelder request_handler,
  const base::Buffer& buffer,
  std::shared_ptr<io::Session> session) {
  net::http::Response response = net::http::ResponseParser::Parse(buffer);
  if (!request_handler.is_null()) {
    request_handler.Run(response);
  }
}

void HttpClientComponent::InternalWriteHandler(size_t length) {
}

}  // namespace component
}  // namespace nlink
