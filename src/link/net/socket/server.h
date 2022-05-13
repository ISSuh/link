/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SERVER_H_
#define LINK_NET_SOCKET_SERVER_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"

namespace link {
namespace net {

class Server {
 public:
  virtual int32_t Listen(const IpEndPoint& address) = 0;
  virtual int32_t Accept(base::CompletionCallback callback) = 0;

 private:
  DISAALOW_COPY_AND_ASSIGN(Server)
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_SERVER_H_
