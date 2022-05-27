/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/tcp_server.h"

#include <string>
#include <memory>
#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace net {

TcpServer::TcpServer()
  : acceptor_(nullptr) {
}

TcpServer::~TcpServer() {
}

void TcpServer::RegistAcceptHandler(Server::AcceptHandler handler) {
  accept_handler_ = std::move(handler);
}

void TcpServer::RegistCloseHandler(Server::CloseHandler handler) {
  close_handler_ = std::move(handler);
}

void TcpServer::RegistReadHandler(Server::ReadHandler handler) {
  read_handler_ = std::move(handler);
}

void TcpServer::RegistWriteHandler(Server::WriteHandler handler) {
  write_handler_ = std::move(handler);
}

void TcpServer::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }
  acceptor_.reset(Acceptor::CreateAcceptor(context));
}

void TcpServer::CloseChannel() {
  Close();
}

void TcpServer::HandleEvent(const base::Event& event) {
}

int32_t TcpServer::Listen(const IpEndPoint& address) {
  acceptor_->Listen(address);
}

int32_t TcpServer::Accept(base::CompletionCallback callback) {
  acceptor_->Accept(
    base::Bind(&TcpServer::AcceptHandler, this));
}

void TcpServer::Close() {
  CloseAllSessions();
  acceptor_->Close();
}

void TcpServer::AcceptHandler(std::shared_ptr<Session> session) {
  sessions_.insert(session);
  session->Open(base::Bind(&TcpServer::ReadHandler, this));

  if (accept_handler_.is_null()) {
    return;
  }
  accept_handler_.Run(session);
}

void TcpServer::CloseHandler(std::shared_ptr<Session> session) {
  sessions_.erase(session);

  if (close_handler_.is_null()) {
    return;
  }
  close_handler_.Run(session);
}

void TcpServer::ReadHandler(const base::Buffer& buffer) {
  if (read_handler_.is_null()) {
    return;
  }
  read_handler_.Run(buffer);
}

void TcpServer::WriteHandler(size_t length) {
  if (write_handler_.is_null()) {
    return;
  }
  write_handler_.Run(length);
}

void TcpServer::CloseAllSessions() {
  for (auto session : sessions_) {
    session->Close();
  }
  sessions_.clear();
}


}  // namespace net
}  // namespace nlink
