/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_client_component.h"

#include <vector>
#include <utility>

#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"

namespace nlink {
namespace component {

TcpClientComponent::TcpClientComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner,
  SocketComponent::Handler handlers)
  : SocketComponent(channel_controller),
    client_(io::SocketFactory::CreateTcpClient(task_runner)),
    connect_handler_(handlers.connect_handler),
    close_handler_(handlers.close_handler),
    read_handler_(handlers.read_handler),
    write_handler_(handlers.write_handler) {
  LinkComponent::AttachChannelsToController(client_.get());

  client_->RegistIOHandler(
    [this](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    });
}

TcpClientComponent::~TcpClientComponent() = default;

void TcpClientComponent::Connect(
  const std::string& address, int32_t port) {
  client_->Connect(
    io::IpEndPoint(address, port),
    [this](std::shared_ptr<io::Session> session) {
      this->InternalConnectHandler(session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void TcpClientComponent::Disconnect() {
  client_->Disconnect();
}

void TcpClientComponent::Write(std::shared_ptr<base::Buffer> buffer) {
  client_->Write(std::move(buffer));
}

bool TcpClientComponent::IsConnected() const {
  return client_->IsConnected();
}

void TcpClientComponent::InternalConnectHandler(
  std::shared_ptr<io::Session> session) {
  if (!connect_handler_) {
    return;
  }
  connect_handler_(session);
}

void TcpClientComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (!close_handler_) {
    return;
  }
  close_handler_(session);
}

void TcpClientComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (!read_handler_) {
    return;
  }
  read_handler_(buffer, session);
}

void TcpClientComponent::InternalWriteHandler(size_t length) {
  if (!write_handler_) {
    return;
  }
  write_handler_(length);
}

TcpClientComponent* TcpClientComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  base::TaskRunner* task_runner,
  SocketComponent::Handler handlers) {
  if (!channel_controller || !task_runner) {
    return nullptr;
  }
  return new TcpClientComponent(channel_controller, task_runner, handlers);
}

}  // namespace component
}  // namespace nlink
