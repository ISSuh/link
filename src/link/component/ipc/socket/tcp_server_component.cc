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

TcpServerComponent::TcpServerComponent()
  : server_(new net::TcpServer()) {
}

TcpServerComponent::~TcpServerComponent() = default;

base::EventChannel* TcpServerComponent::GetEventChannel() {
  return dynamic_cast<base::EventChannel*>(server_.get());
}

void TcpServerComponent::Open(const std::string& address, int32_t port) {
  server_->Listen(net::IpEndPoint(address, port));
  server_->Accept(base::Bind([](int32_t a){}));
}

void TcpServerComponent::Close() {
  server_->Close();
}

TcpServerComponent* TcpServerComponent::CreateComponent() {
  return new TcpServerComponent();
}

}  // namespace component
}  // namespace nlink
