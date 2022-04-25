/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/tcp_client_component.h"

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

TcpClientComponent* TcpClientComponent::CreateTcpClientComponent(
  const std::string& name, base::EventChannelController* event_controller) {
  return new TcpClientComponent(name, event_controller);
}

TcpClientComponent::TcpClientComponent(
  const std::string& name,
  base::EventChannelController* event_controller)
  : SocketComponent(name, event_controller) {}

TcpClientComponent::~TcpClientComponent() = default;

base::Discriptor TcpClientComponent::discriptor() {
  return tcp_server->SocketDiscriptor();
}

base::EventObserver::Type TcpClientComponent::type() {
  return base::EventObserver::Type::CLIENT;
}

void TcpClientComponent::HandleEvent(const base::Event& event) {
  LOG(INFO) << __func__ << " - " << base::EventTypeToString(event);
  switch (event.type()) {
    case base::Event::Type::ACCEPT: {
      break;
    }
    default:
      break;
    }
}

}  // namespace component
}  // namespace link
