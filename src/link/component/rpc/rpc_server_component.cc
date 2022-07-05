/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/rpc/rpc_server_component.h"

#include <vector>

#include "link/base/macro.h"
#include "link/base/callback/bind.h"
#include "link/base/logging.h"
#include "link/io/socket/asio/tcp_server.h"

namespace nlink {
namespace component {

RpcServerComponent::RpcServerComponent()
  : server_(new io::TcpServer()) {
}

RpcServerComponent::~RpcServerComponent() = default;

void RpcServerComponent::Open() {
  server_->Listen(io::IpEndPoint("0.0.0.0", 33455));
  server_->Accept(base::Bind([](int32_t a){}));
}

void RpcServerComponent::Close() {
  server_->Close();
}

void RpcServerComponent::RegistService() {
}


RpcServerComponent* RpcServerComponent::CreateComponent() {
  return new RpcServerComponent();
}

}  // namespace component
}  // namespace nlink
