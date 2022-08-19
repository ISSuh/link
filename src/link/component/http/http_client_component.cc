/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_client_component.h"

#include <utility>

#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"
#include "link/net/base/uri.h"
#include "link/net/http/parser.h"

namespace nlink {
namespace component {

HttpClientComponent* HttpClientComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner) {
  if (!channel_controller || !task_runner) {
    return nullptr;
  }
  return new HttpClientComponent(channel_controller, task_runner);
}

HttpClientComponent::HttpClientComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner)
  : HttpComponent(channel_controller),
    task_runner_(task_runner) {
}

HttpClientComponent::~HttpClientComponent() = default;

void HttpClientComponent::Get(
  const std::string& path, RequestHanelder handler) {
  net::http::HttpHeader default_header;
  Get(path, default_header, handler);
}

void HttpClientComponent::Get(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {
  DoFetch(net::http::Method::GET, url_string, header, handler);
}

void HttpClientComponent::Post(
  const std::string& url_string,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {
  net::http::HttpHeader header;
  header.Set("Content-Type", content_type);
  Post(url_string, header, body, handler);
}

void HttpClientComponent::Post(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  const std::string& body,
  RequestHanelder handler) {
  DoFetchWithBody(
    net::http::Method::POST, url_string, header, body, handler);
}

void HttpClientComponent::Put(
  const std::string& url_string,
  const std::string& content_type,
  const std::string& body,
  RequestHanelder handler) {
  net::http::HttpHeader header;
  header.Set("Content-Type", content_type);
  Post(url_string, header, body, handler);
}

void HttpClientComponent::Put(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  const std::string& body,
  RequestHanelder handler) {
  DoFetchWithBody(
    net::http::Method::PUT, url_string, header, body, handler);
}

void HttpClientComponent::Delete(
  const std::string& url_string, RequestHanelder handler) {
  net::http::HttpHeader default_header;
  Delete(url_string, default_header, handler);
}

void HttpClientComponent::Delete(
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {
  DoFetch(net::http::Method::DELETE, url_string, header, handler);
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
    case net::http::Method::DELETE:
      DoFetch(method, url_string, header, handler);
      break;
    case net::http::Method::POST:
    case net::http::Method::PUT:
      DoFetchWithBody(
        method, url_string, header, body, handler);
      break;
    default:
      LOG(WARNING) << "[HttpClientComponent::Fetch] unsupport method. "
                  << MethodToString(method);
      break;
  }
}

void HttpClientComponent::DoFetch(
  net::http::Method method,
  const std::string& url_string,
  const net::http::HttpHeader& header,
  RequestHanelder handler) {
  net::Uri uri = net::Uri::Parse(url_string);
  if (!uri.HasScheme() || !uri.HasHost()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::Request request(method, uri, header);
  CreateIOClientAndConnet(request, handler);
}

void HttpClientComponent::DoFetchWithBody(
  net::http::Method method,
  const std::string& url_string,
  const net::http::HttpHeader& header,
  const std::string& body,
  RequestHanelder handler) {
  net::Uri uri = net::Uri::Parse(url_string);
  if (!uri.HasScheme() || !uri.HasHost()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::Request request(method, uri, header, body);
  CreateIOClientAndConnet(request, handler);
}

void HttpClientComponent::CreateIOClientAndConnet(
  const net::http::Request& request, RequestHanelder handler) {
  io::Client* client = io::SocketFactory::CreateTcpClient(task_runner_);
  LinkComponent::AttachChannelsToController(client);

  client->RegistIOHandler(
    [this, handler](
      const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      this->InternalReadHandler(handler, buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    });

  auto uri = request.RequestUri();
  client->Connect(
    io::IpEndPoint(uri.Host(), uri.Port()),
    [this, req = std::move(request), client]
    (std::shared_ptr<io::Session> session) {
      this->InternalConnectHandler(req, client, session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void HttpClientComponent::InternalConnectHandler(
  const net::http::Request& request,
  io::Client* client,
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }

  std::shared_ptr<base::Buffer> buffer =
    std::make_shared<base::Buffer>(request.Serialize());

  LOG(INFO) << __func__ << " - request : \n" << request.Serialize();

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
  if (buffer.IsEmpty()) {
    return;
  }

  net::http::Response response = net::http::Parser::ParseResponse(buffer);
  if (!request_handler) {
    return;
  }
  request_handler(response);
}

void HttpClientComponent::InternalWriteHandler(size_t length) {
}

}  // namespace component
}  // namespace nlink
