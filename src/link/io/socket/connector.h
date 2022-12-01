/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_CONNECTOR_H_
#define LINK_IO_SOCKET_CONNECTOR_H_

#include <memory>

#include "link/base/callback.h"
#include "link/base/event/event_dispatcher.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/session.h"

namespace nlink {
namespace io {

class Connector {
 public:
  virtual ~Connector() = default;

  virtual void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) = 0;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_CONNECTOR_H_
