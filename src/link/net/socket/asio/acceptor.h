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
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/session.h"

namespace nlink {
namespace net {

class Acceptor {
 public:
  using AcceptorHandler = base::Callback<void(std::shared_ptr<Session>)>;

  static Acceptor* CreateAcceptor(base::DispatcherConext* dispatcher_context);

  virtual void Listen(const IpEndPoint& address) = 0;
  virtual void Accept(AcceptorHandler handler) = 0;
  virtual void Close() = 0;
};



}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ACCEPTOR_H_
