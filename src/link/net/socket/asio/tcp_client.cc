/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/tcp_client.h"

#include <utility>

#include "link/base/event/event_dispatcher.h"
#include "link/base/logging.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

TcpClient::TcpClient()
  : connector_(nullptr), session_(nullptr) {
}

TcpClient::~TcpClient() {
}

void TcpClient::Connect(
  IpEndPoint address,
  handler::ConnectHandler connect_handler,
  handler::CloseHandler close_handler) {
  if (!session_) {
    return;
  }

  connect_handler_ = connect_handler;
  close_handler_ = close_handler;

  connector_->Connect(address,
    base::Bind(&TcpClient::InternalConnectHandler, this));
}

void TcpClient::Disconnect() {
  session_->Close();
}

void TcpClient::Write(const base::Buffer& buffer) {
  if (buffer.IsEmpty()) {
    return;
  }
  session_->Write(buffer);
}

void TcpClient::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
}

void TcpClient::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }
  connector_.reset(Connector::CreateConnector(context));
}

void TcpClient::CloseChannel() {
  Disconnect();
}

void TcpClient::HandleEvent(const base::Event& event) {
}

void TcpClient::InternalConnectHandler(std::shared_ptr<Session> session) {
  session_ = session;

  session_->Open(
    base::Bind(&TcpClient::InternalReadHandler, this),
    base::Bind(&TcpClient::InternalWriteHandler, this),
    base::Bind(&TcpClient::InternalCloseHandler, this));

  connect_handler_.Run(session);
}

void TcpClient::InternalCloseHandler(std::shared_ptr<Session> session) {
  close_handler_.Run(session);
}

void TcpClient::InternalReadHandler(const base::Buffer& buffer) {
  read_handler_.Run(buffer);
}

void TcpClient::InternalWriteHandler(size_t length) {
  write_handler_.Run(length);
}


}  // namespace net
}  // namespace nlink
