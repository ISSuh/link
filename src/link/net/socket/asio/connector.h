/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_
#define SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_

#include <memory>

#include "link/base/event/event_dispatcher.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/asio/session.h"

namespace nlink {
namespace net {

class Connector {
 public:
  static Connector* CreateConnector(asio::ip::tcp::socket socket);

  virtual void Connect(
    const IpEndPoint& address, Session::Delegate* delegate) = 0;
  virtual void Close() = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_ASIO_CONNECTOR_H_
