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
#include "link/base/event/event_util.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/net/socket/tcp_server_socket.h"
#include "link/base/logging.h"

namespace nlink {
namespace component {

class TcpServerComponent : public SocketComponent {
 public:
  static TcpServerComponent* CreateTcpServerComponent(
    const std::string& name,
    base::EventChannelController* event_controller);

  void RegistReadCabllack();

 private:
  TcpServerComponent(
    const std::string& name,
    base::EventChannelController* event_controller);
  virtual ~TcpServerComponent();

  // base::EventObserver
  base::Discriptor discriptor() override;
  base::EventObserver::Type type() override;
  void HandleEvent(const base::Event& event) override;

  void DoAccept();

  std::unique_ptr<net::TcpServerSocket> tcp_server;
  DISAALOW_COPY_AND_ASSIGN(TcpServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
