/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_server_component.h"

#include <string>
#include <memory>
#include <utility>

#include "link/base/macro.h"
#include "link/base/event/event_util.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/net/socket/tcp_server_socket.h"
#include "link/net/socket/tcp_client_socket.h"
#include "link/net/base/ip_endpoint.h"
#include "link/base/logging.h"

namespace link {
namespace component {

TcpServerComponent* TcpServerComponent::CreateTcpServerComponent(
  const std::string& name, base::EventChannelController* event_controller) {
  return new TcpServerComponent(name, event_controller);
}

TcpServerComponent::TcpServerComponent(
  const std::string& name,
  base::EventChannelController* event_controller)
  : SocketComponent(name, event_controller) {}

TcpServerComponent::~TcpServerComponent() = default;

base::Discriptor TcpServerComponent::discriptor() {
  return tcp_server->SocketDiscriptor();
}

base::EventObserver::Type TcpServerComponent::type() {
  return base::EventObserver::Type::SERVER;
}

void TcpServerComponent::HandleEvent(const base::Event& event) {
  LOG(INFO) << __func__ << " - " << base::EventTypeToString(event);
  switch (event.type()) {
    case base::Event::Type::ACCEPT: {
      DoAccept();
      break;
    }
    default:
      break;
    }
}

void TcpServerComponent::DoAccept() {
  // Test code
  // net::SockaddrStorage new_peer_address;
  // int32_t new_socket = accept(
  //   discriptor(), new_peer_address.addr, &new_peer_address.addr_len);

  // LOG(INFO) << __func__ << " - accept ret : " << new_socket;
  std::unique_ptr<net::TcpSocket> new_socket =
    std::unique_ptr<net::TcpSocket>(new net::TcpSocket());

  net::IpEndPoint ip_endpoint;
  tcp_server->Accept(&new_socket, base::bind(), &ip_endpoint);

  net::TcpClientSocket client_socket(std::move(new_socket), ip_endpoint);

  base::EventChannelController* event_controller = EventConrolloer();
  event_controller->AttachChannel();
}


}  // namespace component
}  // namespace link
