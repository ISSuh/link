/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_server_component.h"

#include <memory>

#include "link/base/logging.h"
#include "link/base/buffer.h"
#include "link/io/socket/socket_factory.h"
#include "link/net/http/request_parser.h"

namespace nlink {
namespace component {

HttpServerComponent* HttpServerComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner) {
  if (!channel_controller || !task_runner) {
    return nullptr;
  }
  return new HttpServerComponent(channel_controller, task_runner);
}

HttpServerComponent::HttpServerComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner)
  : HttpComponent(channel_controller),
    task_runner_(task_runner),
    server_(io::SocketFactory::CreateTcpServer(task_runner)) {
  LinkComponent::AttachChannelsToController(server_.get());

  server_->RegistIOHandler(
    [this](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    });
}

HttpServerComponent::~HttpServerComponent() = default;

void HttpServerComponent::Open(const std::string& address, int32_t port) {
  server_->Listen(io::IpEndPoint(address, port),
    [this](std::shared_ptr<io::Session> session) {
      this->InternalAcceptHandler(session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void HttpServerComponent::Close() {
  server_->Close();
}

void HttpServerComponent::Route(
  const std::string& path,
  net::http::handler::ResponseHandler handler) {
  routing_.RegistHandler(path, handler);
}

void HttpServerComponent::InternalAcceptHandler(
  std::shared_ptr<io::Session> session) {
}

void HttpServerComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
}

void HttpServerComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (buffer.IsEmpty()) {
    return;
  }

  net::http::Response response;
  net::http::Request request = net::http::RequestParser::Parse(buffer);
  auto url  = request.RequestUri();
  if (!url.HasPath()) {
    return;
  }

  auto handler = routing_.Route(url.Path());
  if (handler) {
    handler(request, &response);
  }
}

void HttpServerComponent::InternalWriteHandler(size_t length) {
}


}  // namespace component
}  // namespace nlink
