/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/tcp_server.h"

#include <memory>
#include <utility>

namespace nlink {
namespace net {

TcpServer::TcpServer()
  : acceptor_(nullptr) {
}

TcpServer::~TcpServer() {
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
  acceptor_->Accept(std::move(callback));
}

void TcpServer::Close() {
  acceptor_->Close();
}

}  // namespace net
}  // namespace nlink
