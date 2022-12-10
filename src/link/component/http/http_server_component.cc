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
#include "link/net/http/parser.h"

namespace nlink {
namespace component {

HttpServerComponent* HttpServerComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner) {
  if (!channel_controller) {
    return nullptr;
  }
  return new HttpServerComponent(channel_controller, task_runner);
}

HttpServerComponent::HttpServerComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner)
  : HttpComponent(channel_controller),
    task_runner_(task_runner),
    http_server_(std::make_unique<HttpServer>(task_runner)) {
  LinkComponent::AttachChannelsToController(http_server_->ServerContext());
}

HttpServerComponent::~HttpServerComponent() = default;

void HttpServerComponent::Open(const std::string& address, int32_t port) {
  io::IpEndPoint endpoint(address, port);
  http_server_->Open(endpoint);
}

void HttpServerComponent::Close() {
  http_server_->Close();
}

void HttpServerComponent::Route(
  const std::string& path,
  net::http::handler::ResponseHandler handler) {
  http_server_->Route(path, handler);
}

}  // namespace component
}  // namespace nlink
