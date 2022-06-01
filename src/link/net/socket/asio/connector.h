/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_
#define SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_

#include <memory>

#include "link/base/callback/callback.h"
#include "link/base/event/event_dispatcher.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/session.h"

namespace nlink {
namespace net {

class Connector {
 public:
  static Connector* CreateConnector(base::DispatcherConext* dispatcher_context);

  virtual void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_
