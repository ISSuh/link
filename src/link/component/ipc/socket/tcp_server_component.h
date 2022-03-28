/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_observer.h"
#include "link/net/socket/tcp_server_socket.h"

namespace link {
namespace component {

class TcpServerComponent : public base::EventObserve {
 public:

 private:
  TcpServerComponent() = default;
  ~TcpServerComponent() = default;

  base::Discriptor discriptor() override {
    return tcp_server->SocketDiscriptor();
  }

  base::Type type() {
    return base::EventObserver::Type::SERVER;
  }

  void HandleEvent(const base::Event& event) {
    LOG(INFO) << __func__ << " - " << base::EventTypeToString(event);
    switch (event.type()) {
    case base::Event::Type::ACCEPT: {
      // Test code
      SockaddrStorage new_peer_address;
      int32_t new_socket = accept(
        discriptor(), new_peer_address.addr, &new_peer_address.addr_len);

      LOG(INFO) << __func__ << " - accept ret : " << new_socket;
      break;
    }
    case base::Event::Type::READ:
      break;
    default:
      break;
    }
  }

  std::unique_ptr<net::TcpServerSocket> tcp_server;
  DISAALOW_COPY_AND_ASSIGN(TcpServerComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
