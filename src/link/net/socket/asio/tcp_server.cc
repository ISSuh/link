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

void TcpServer::RegistReadHandler(Server::ReadHandler read_handler) {

}

void TcpServer::Close() {
  CloseAllSessions();
  acceptor_->Close();
}

void TcpServer::AcceptHandler(std::shared_ptr<Session> session) {
  sessions_.insert(session);
  session->Open(base::Bind(&TcpServer::ReadHandler, this));
}

void TcpServer::ReadHandler(const std::vector<uint8_t>& buffer) {
  std::string temp(buffer.begin(), buffer.end());
  LOG(INFO) << __func__ << " - temp : " << temp << " / size : " << temp.size();
}

void TcpServer::CloseHandler(std::shared_ptr<Session> session) {
  sessions_.erase(session);
}

void TcpServer::CloseAllSessions() {
  for (auto session : sessions_) {
    session->Close();
  }
  sessions_.clear();
}


}  // namespace net
}  // namespace nlink
