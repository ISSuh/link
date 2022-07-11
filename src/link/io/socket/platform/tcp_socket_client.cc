/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_socket_client.h"

#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace io {

TcpSocketClient::TcpSocketClient(base::TaskRunner* task_runner)
  : task_runner_(task_runner), session_(nullptr) {
}

TcpSocketClient::~TcpSocketClient() {
}

void TcpSocketClient::Connect(
  IpEndPoint address,
  handler::ConnectHandler connect_handler,
  handler::CloseHandler close_handler) {
  connect_handler_ = connect_handler;
  close_handler_ = close_handler;
}

void TcpSocketClient::Disconnect() {
  if (nullptr == session_) {
    return;
  }
  session_->Close();
}

void TcpSocketClient::Write(const base::Buffer& buffer) {
  if (nullptr == session_ || buffer.IsEmpty()) {
    return;
  }
  session_->Write(buffer);
}

void TcpSocketClient::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {
  if (nullptr == session_ || buffer.IsEmpty()) {
    return;
  }
  session_->Write(buffer, write_handler, read_handler);
}

void TcpSocketClient::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
}

void TcpSocketClient::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }

  if (nullptr == connector_) {
    connector_.reset(Connector::CreateConnector(context));
  }
}

void TcpSocketClient::CloseChannel() {
  Disconnect();
}

void TcpSocketClient::HandleEvent(const base::Event& event) {
}

void TcpSocketClient::InternalConnectHandler(std::shared_ptr<Session> session) {
  session_ = session;

  session_->Open(
    base::Bind(&TcpSocketClient::InternalReadHandler, this),
    base::Bind(&TcpSocketClient::InternalWriteHandler, this),
    base::Bind(&TcpSocketClient::InternalCloseHandler, this));

  connect_handler_.Run(session);
}

void TcpSocketClient::InternalCloseHandler(std::shared_ptr<Session> session) {
  close_handler_.Run(session);
}

void TcpSocketClient::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  read_handler_.Run(buffer, session);
}

void TcpSocketClient::InternalWriteHandler(size_t length) {
  write_handler_.Run(length);
}


}  // namespace io
}  // namespace nlink
