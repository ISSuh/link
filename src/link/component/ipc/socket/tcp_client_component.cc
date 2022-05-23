/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_client_component.h"

#include "link/base/logging.h"
#include "link/net/socket/asio/tcp_client.h"

namespace nlink {
namespace component {

TcpClientComponent::TcpClientComponent()
  : client_(new net::TcpClient()) {
}

TcpClientComponent::~TcpClientComponent() = default;

void TcpClientComponent::Connect(
  const std::string& address, int32_t port) {
  client_->Connect(net::IpEndPoint(address, port));
}

void TcpClientComponent::DisConnect() {
  client_->CloseChannel();
  client_->DisConnect();
}

void TcpClientComponent::Write(const std::vector<uint8_t>& buffer) {
  client_->Write(buffer);
}

base::EventChannel* TcpClientComponent::GetEventChannel() {
  return dynamic_cast<base::EventChannel*>(client_.get());
}

TcpClientComponent* TcpClientComponent::CreateComponent() {
  return new TcpClientComponent();
}

}  // namespace component
}  // namespace nlink
