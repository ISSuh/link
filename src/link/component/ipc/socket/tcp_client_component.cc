/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_client_component.h"

#include <vector>

#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"

namespace nlink {
namespace component {

TcpClientComponent::TcpClientComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers)
  : client_(io::SocketFactory::CreateTcpClient(task_runner)),
    connect_handler_(handlers.connect_handler),
    close_handler_(handlers.close_handler),
    read_handler_(handlers.read_handler),
    write_handler_(handlers.write_handler) {
  LinkComponent::AttachChannelsToObserver(client_.get());

  client_->RegistIOHandler(
    base::Bind(&TcpClientComponent::InternalReadHandler, this),
    base::Bind(&TcpClientComponent::InternalWriteHandler, this));
}

TcpClientComponent::~TcpClientComponent() = default;

void TcpClientComponent::Connect(
  const std::string& address, int32_t port) {
  client_->Connect(
    io::IpEndPoint(address, port),
    base::Bind(&TcpClientComponent::InternalConnectHandler, this),
    base::Bind(&TcpClientComponent::InternalCloseHandler, this));
}

void TcpClientComponent::DisConnect() {
  client_->Disconnect();
}

void TcpClientComponent::Write(const base::Buffer& buffer) {
  client_->Write(buffer);
}

void TcpClientComponent::InternalConnectHandler(
  std::shared_ptr<io::Session> session) {
  if (connect_handler_.is_null()) {
    return;
  }
  connect_handler_.Run(session);
}

void TcpClientComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (close_handler_.is_null()) {
    return;
  }
  close_handler_.Run(session);
}

void TcpClientComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (read_handler_.is_null()) {
    return;
  }
  read_handler_.Run(buffer, session);
}

void TcpClientComponent::InternalWriteHandler(size_t length) {
  if (write_handler_.is_null()) {
    return;
  }
  write_handler_.Run(length);
}

TcpClientComponent* TcpClientComponent::CreateComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers) {
  return new TcpClientComponent(task_runner, handlers);
}

}  // namespace component
}  // namespace nlink
