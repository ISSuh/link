/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ACCEPTOR_H_
#define LINK_NET_SOCKET_ACCEPTOR_H_

#include <memory>

#include "link/base/callback/callback.h"
#include "link/base/event/event_dispatcher.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/session.h"

namespace nlink {
namespace io {

class Acceptor {
 public:
  static Acceptor* CreateAcceptor(base::DispatcherConext* dispatcher_context);

  virtual bool Listen(const IpEndPoint& address) = 0;
  virtual void Accept(handler::AcceptHandler handler) = 0;
  virtual void Close() = 0;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ACCEPTOR_H_
