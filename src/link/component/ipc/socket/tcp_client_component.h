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
#include "link/component/ipc/socket/socket_component.h"
#include "link/net/socket/tcp_server_socket.h"
#include "link/base/logging.h"

namespace link {
namespace component {

class TcpClientComponent : public SocketComponent {
 public:
  static TcpClientComponent* CreateTcpClientComponent(
    const std::string& name,
    base::EventChannelController* event_controller);

  void RegistReadCabllack();
  void RegistSendCabllack();

 private:
  TcpClientComponent(
    const std::string& name,
    base::EventChannelController* event_controller);
  virtual ~TcpClientComponent();

  // base::EventObserver
  base::Discriptor discriptor() override;
  base::EventObserver::Type type() override;
  void HandleEvent(const base::Event& event) override;

  void DoAccept();

  std::unique_ptr<net::TcpServerSocket> tcp_server;
  DISAALOW_COPY_AND_ASSIGN(TcpClientComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_
