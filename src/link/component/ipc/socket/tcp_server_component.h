/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/component/rpc/rpc_component.h"
#include "link/net/socket/server.h"

namespace nlink {
namespace component {

class RpcComponent;

class TcpServerComponent : public RpcComponent {
 public:
  static TcpServerComponent* CreateComponent();

  void Open(const std::string& address, int32_t port);
  void Close();

 private:
  base::EventChannel* GetEventChannel() override;

  TcpServerComponent();
  virtual ~TcpServerComponent();

  std::unique_ptr<net::Server> server_;

  DISAALOW_COPY_AND_ASSIGN(TcpServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
