/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/asio/tcp_server.h"

#include <string>
#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace io {

TcpServer::TcpServer(base::TaskRunner* task_runner)
  : task_runner_(task_runner), acceptor_(nullptr) {
}

TcpServer::~TcpServer() {
}

bool TcpServer::Listen(const IpEndPoint& address) {
  return acceptor_->Listen(address);
}

void TcpServer::Accept(
  handler::AcceptHandler accept_handler,
  handler::CloseHandler close_handler) {
  accept_handler_ = std::move(accept_handler);
  close_handler_ = std::move(close_handler);

  acceptor_->Accept(
    base::Bind(&TcpServer::InternalAcceptHandler, this));
}

void TcpServer::Close() {
  CloseAllSessions();
  acceptor_->Close();
}

void TcpServer::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
}

void TcpServer::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }

  if (nullptr == acceptor_) {
    acceptor_.reset(Acceptor::CreateAcceptor(context));
  }
}

void TcpServer::CloseChannel() {
  Close();
}

void TcpServer::HandleEvent(const base::Event& event) {
}

void TcpServer::InternalAcceptHandler(std::shared_ptr<Session> session) {
  sessions_.insert(session);

  session->Open(
    base::Bind(&TcpServer::InternalReadHandler, this),
    base::Bind(&TcpServer::InternalWriteHandler, this),
    base::Bind(&TcpServer::InternalCloseHandler, this));

  if (accept_handler_.is_null()) {
    return;
  }
  accept_handler_.Run(session);
}

void TcpServer::InternalCloseHandler(std::shared_ptr<Session> session) {
  sessions_.erase(session);

  if (close_handler_.is_null()) {
    return;
  }
  close_handler_.Run(session);
}

void TcpServer::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (read_handler_.is_null()) {
    return;
  }
  read_handler_.Run(buffer, session);
}

void TcpServer::InternalWriteHandler(size_t length) {
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


}  // namespace io
}  // namespace nlink
