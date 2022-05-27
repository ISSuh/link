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
  close_handler_ = close_handler_;

  connector_->Connect(address,
    base::Bind(&TcpClient::InternalConnectHandler, this));
}

void TcpClient::Disconnect() {
  session_->Close();
}

void TcpClient::Write(const base::Buffer& buffer) {
  session_->Write(buffer, base::Bind(&TcpClient::WriteHandler, this));
}

void TcpClient::RegistReadHandler(handler::ReadHandler handler) {
  read_handler_ = handler;
}

void TcpClient::RegistWriteHandler(handler::WriteHandler handler) {
  write_handler_ = handler;
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

// void TcpClient::OnSessionClose() {
// }

// void TcpClient::WriteHandler(std::size_t length) {
//   LOG(INFO) << __func__ << " - length : " << length;
// }

void TcpClient::InternalConnectHandler(
  std::shared_ptr<ClientSideSession> client_session) {
  session_ = client_session;

  connect_handler_.Run(client_session);
}


}  // namespace net
}  // namespace nlink
