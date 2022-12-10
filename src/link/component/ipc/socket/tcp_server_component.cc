/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_server_component.h"

#include <vector>
#include <utility>

#include "link/base/buffer.h"
#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"

namespace nlink {
namespace component {

TcpServerComponent::TcpServerComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner,
  SocketComponent::Handler handlers)
  : SocketComponent(channel_controller),
    server_(io::SocketFactory::CreateTcpServer(task_runner)),
    accept_handler_(std::move(handlers.accept_handler)),
    close_handler_(std::move(handlers.close_handler)),
    read_handler_(std::move(handlers.read_handler)),
    write_handler_(std::move(handlers.write_handler)) {
  LinkComponent::AttachChannelsToController(server_.get());

  server_->RegistIOHandler(
    [this](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    });
}

TcpServerComponent::~TcpServerComponent() = default;

void TcpServerComponent::Open(
  const std::string& address, int32_t port) {
  server_->Listen(io::IpEndPoint(address, port),
    [this](std::shared_ptr<io::Session> session) {
      this->InternalAcceptHandler(session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void TcpServerComponent::Close() {
  server_->Close();
}

void TcpServerComponent::InternalAcceptHandler(
  std::shared_ptr<io::Session> session) {
  if (!accept_handler_) {
    return;
  }
  accept_handler_(session);
}

void TcpServerComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (!close_handler_) {
    return;
  }
  close_handler_(session);
}

void TcpServerComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (!read_handler_) {
    return;
  }
  read_handler_(buffer, session);
}

void TcpServerComponent::InternalWriteHandler(size_t length) {
  if (!write_handler_) {
    return;
  }
  write_handler_(length);
}

TcpServerComponent* TcpServerComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner,
  SocketComponent::Handler handlers) {
  if (!channel_controller) {
    return nullptr;
  }
  return new TcpServerComponent(
    channel_controller, task_runner, std::move(handlers));
}

}  // namespace component
}  // namespace nlink
