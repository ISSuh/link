/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_server_component.h"

#include "link/base/macro.h"
#include "link/base/callback/bind.h"
#include "link/base/logging.h"
#include "link/net/socket/asio/tcp_server.h"

namespace nlink {
namespace component {

TcpServerComponent::TcpServerComponent(SocketComponent::Handler handlers)
  : server_(new net::TcpServer()),
    accept_handler_(handlers.accept_handler),
    close_handler_(handlers.close_handler),
    read_handler_(handlers.read_handler),
    write_handler_(handlers.write_handler) {
  server_->RegistIOHandler(
    base::Bind(&TcpServerComponent::InternalReadHandler, this),
    base::Bind(&TcpServerComponent::InternalWriteHandler, this));
}

TcpServerComponent::~TcpServerComponent() = default;

base::EventChannel* TcpServerComponent::GetEventChannel() {
  return dynamic_cast<base::EventChannel*>(server_.get());
}

void TcpServerComponent::Open(
  const std::string& address, int32_t port) {
  server_->Listen(net::IpEndPoint(address, port));
  server_->Accept(
    base::Bind(&TcpServerComponent::InternalAcceptHandler, this),
    base::Bind(&TcpServerComponent::InternalCloseHandler, this));
}

void TcpServerComponent::Close() {
  server_->Close();
}

void TcpServerComponent::InternalAcceptHandler(
  std::shared_ptr<net::Session> session) {
  accept_handler_.Run(session);
}

void TcpServerComponent::InternalCloseHandler(
  std::shared_ptr<net::Session> session) {
  close_handler_.Run(session);
}

void TcpServerComponent::InternalReadHandler(const base::Buffer& buffer) {
  read_handler_.Run(buffer);
}

void TcpServerComponent::InternalWriteHandler(size_t length) {
  write_handler_.Run(length);
}

TcpServerComponent* TcpServerComponent::CreateComponent(
  SocketComponent::Handler handlers) {
  return new TcpServerComponent(handlers);
}

}  // namespace component
}  // namespace nlink
