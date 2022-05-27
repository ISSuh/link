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

void TcpClient::Connect(IpEndPoint address) {
  if (!session_) {
    return;
  }
  connector_->Connect(address);
}

void TcpClient::DisConnect() {
}

void TcpClient::Write(const std::vector<uint8_t>& buffer) {
  // session_->Write(buffer, base::Bind(&TcpClient::WriteHandler, this));
}

void TcpClient::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }
  connector_.reset(Connector::CreateConnector(context));
}

void TcpClient::CloseChannel() {
  session_->Close();
}

void TcpClient::HandleEvent(const base::Event& event) {
}

// void TcpClient::OnSessionClose() {
// }

// void TcpClient::WriteHandler(std::size_t length) {
//   LOG(INFO) << __func__ << " - length : " << length;
// }

void TcpClient::InternalConnectHandler(
  std::shared_ptr<ClientSideSession> session) {
  session_ = session;
}


}  // namespace net
}  // namespace nlink
