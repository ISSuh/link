/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ACCEPTOR_H_
#define LINK_NET_SOCKET_ACCEPTOR_H_

#include <memory>

#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"

namespace link {
namespace net {

class Acceptor {
 public:
  virtual void Listen(const IpEndPoint& address) = 0;
  virtual void Accept(base::CompletionCallback callback) = 0;
};

std::unique_ptr<Acceptor> CreateAcceptor();

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_ACCEPTOR_H_
