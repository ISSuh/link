/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_client_component.h"

#include "link/base/logging.h"
#include "link/net/base/uri.h"
#include "link/net/http/response_parser.h"

namespace nlink {
namespace component {

HttpClientComponent::HttpClientComponent(
  const std::string& host, uint16_t port) {
}

HttpClientComponent::~HttpClientComponent() {
}

void HttpClientComponent::Get(
  const std::string& path, RequestHanelder handler) {
  const std::string scheme = is_https ? "https" : "http";
  const std::string host = url_ + std::to_string(port_);
  net::Uri uri(scheme, host, path);
  net::http::Request request(net::http::Method::GET, uri);

  request.Serialize();

}

void HttpClientComponent::Get(
  const std::string& path,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Post(
  const std::string& path,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {

}

void HttpClientComponent::Post(
  const std::string& path,
  const net::http::HttpHeader& header,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {

}

void HttpClientComponent::Put(
  const std::string& path, RequestHanelder handler) {

}

void HttpClientComponent::Put(
  const std::string& path,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Delete(
  const std::string& path, RequestHanelder handler) {

}

void HttpClientComponent::Delete(
  const std::string& path,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {

}

void HttpClientComponent::Options(
  const std::string& path, RequestHanelder handler) {
  
}

void HttpClientComponent::Options(
  const std::string& path,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {
  
}

void HttpClientComponent::Fetch(
  net::http::Method method,
  const std::string& path,
  RequestHanelder handler,
  const net::http::HttpHeader& header,
  const std::string& content_type,
  const std::string& body) {
  switch (method) {
  case net::http::Method::GET:
    Get(path, header, handler);
    break;
  case net::http::Method::POST:
    Post(path, header, content_type, body, handler);
    break;
  case net::http::Method::PUT:
    Put(path, header, handler);
    break;
  case net::http::Method::DELETE:
    Delete(path, header, handler);
    break;
  default:
    LOG(WARNING) << "[HttpClientComponent::Fetch] unsupport method. "
                 << MethodToString(method);
    break;
  }
}

base::EventChannel* HttpClientComponent::GetEventChannel() {
}

void HttpClientComponent::InternalConnectHandler(
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }
}

void HttpClientComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }
}

void HttpClientComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  net::http::Response response = net::http::ResponseParser::Parse(buffer);


}

void HttpClientComponent::InternalWriteHandler(size_t length) {
}

}  // namespace component
}  // namespace nlink
