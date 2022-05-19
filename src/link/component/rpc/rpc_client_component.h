/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_RPC_RPC_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_RPC_RPC_CLIENT_COMPONENT_H_

#include <memory>

#include "link/base/macro.h"
#include "link/component/rpc/rpc_component.h"
#include "link/net/socket/asio/tcp_client.h"

namespace nlink {
namespace component {

class RpcComponent;

class RpcClientComponent : public RpcComponent {
 public:
  static RpcClientComponent* CreateComponent();

  void Connect();
  void DisConnect();

 private:
  base::EventChannel* GetEventChannel() override;

  RpcClientComponent();
  virtual ~RpcClientComponent();

  std::unique_ptr<net::Client> client_;

  DISAALOW_COPY_AND_ASSIGN(RpcClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_RPC_RPC_CLIENT_COMPONENT_H_
