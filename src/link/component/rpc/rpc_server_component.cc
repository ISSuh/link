/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/rpc/rpc_server_component.h"

#include "link/base/macro.h"
#include "link/base/callback/bind.h"
#include "link/base/logging.h"

namespace nlink {
namespace component {

RpcServerComponent::RpcServerComponent()
  : server_(new net::TcpServer()) {
}

RpcServerComponent::~RpcServerComponent() = default;

base::EventChannel* RpcServerComponent::GetEventChannel() {
  base::EventChannel* chaneel =
    static_cast<base::EventChannel*>(server_.get());
  return chaneel;
}

void RpcServerComponent::Open() {
  server_->Listen(net::IpEndPoint("0.0.0.0", 33455));
  server_->Accept(base::Bind([](int32_t a){}));
}

void RpcServerComponent::Close() {
}

void RpcServerComponent::RegistService() {
}


RpcServerComponent* RpcServerComponent::CreateComponent() {
  return new RpcServerComponent();
}

}  // namespace component
}  // namespace nlink
