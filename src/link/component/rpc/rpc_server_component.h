/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_
#define LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_

#include <memory>

#include "link/base/macro.h"
#include "link/component/rpc/rpc_component.h"
#include "link/net/socket/asio/tcp_server.h"

namespace nlink {
namespace component {

class RpcComponent;

class RpcServerComponent : public RpcComponent {
 public:
  static RpcServerComponent* CreateComponent();

  void Open();
  void Close();
  void RegistService();

 private:
  base::EventChannel* GetEventChannel() override;

  RpcServerComponent();
  virtual ~RpcServerComponent();

  std::unique_ptr<net::Server> server_;

  DISAALOW_COPY_AND_ASSIGN(RpcServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_
