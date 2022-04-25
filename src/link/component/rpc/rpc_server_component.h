/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_
#define LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_util.h"
#include "link/component/rpc/rpc_component.h"
#include "link/net/socket/tcp_server_socket.h"
#include "link/base/logging.h"

namespace link {
namespace component {

class RpcComponent;

class RpcServerComponent : public RpcComponent {
 public:
  class Service {
   public:
    
  };

  static RpcServerComponent* CreateTcpServerComponent(
    const std::string& name,
    base::EventChannelController* event_controller);

  void Open();
  void RegistService()

 private:
  RpcServerComponent(
    const std::string& name,
    base::EventChannelController* event_controller);
  virtual ~RpcServerComponent();

  // base::EventObserver
  base::Discriptor discriptor() override;
  base::EventObserver::Type type() override;
  void HandleEvent(const base::Event& event) override;

  void DoAccept();

  std::unique_ptr<net::TcpServerSocket> tcp_server;
  DISAALOW_COPY_AND_ASSIGN(RpcServerComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_
