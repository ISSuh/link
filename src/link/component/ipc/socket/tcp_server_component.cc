/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_server_component.h"

#include <vector>

#include "link/base/macro.h"
#include "link/base/callback/bind.h"
#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"

namespace nlink {
namespace component {

TcpServerComponent::TcpServerComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers)
  : server_(io::SocketFactory::CreateTcpServer(task_runner)),
    accept_handler_(handlers.accept_handler),
    close_handler_(handlers.close_handler),
    read_handler_(handlers.read_handler),
    write_handler_(handlers.write_handler) {
  LinkComponent::AttachChannelsToObserver(server_.get());

  server_->RegistIOHandler(
    base::Bind(&TcpServerComponent::InternalReadHandler, this),
    base::Bind(&TcpServerComponent::InternalWriteHandler, this));
}

TcpServerComponent::~TcpServerComponent() = default;

void TcpServerComponent::Open(
  const std::string& address, int32_t port) {
  server_->Listen(io::IpEndPoint(address, port));
  server_->Accept(
    base::Bind(&TcpServerComponent::InternalAcceptHandler, this),
    base::Bind(&TcpServerComponent::InternalCloseHandler, this));
}

void TcpServerComponent::Close() {
  server_->Close();
}

void TcpServerComponent::InternalAcceptHandler(
  std::shared_ptr<io::Session> session) {
  accept_handler_.Run(session);
}

void TcpServerComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  close_handler_.Run(session);
}

void TcpServerComponent::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  read_handler_.Run(buffer, session);
}

void TcpServerComponent::InternalWriteHandler(size_t length) {
  write_handler_.Run(length);
}

TcpServerComponent* TcpServerComponent::CreateComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers) {
  return new TcpServerComponent(task_runner, handlers);
}

}  // namespace component
}  // namespace nlink
