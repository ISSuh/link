/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_util.h"
#include "link/base/logging.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/net/socket/client.h"

namespace nlink {
namespace component {

class RpcComponent;

class TcpClientComponent : public SocketComponent {
 public:
  static TcpClientComponent* CreateComponent();

  void Connect(const std::string& address, int32_t port);
  void DisConnect();

 private:
  TcpClientComponent();
  virtual ~TcpClientComponent();

  base::EventChannel* GetEventChannel() override;

  std::unique_ptr<net::Client> client_;

  DISAALOW_COPY_AND_ASSIGN(TcpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_
