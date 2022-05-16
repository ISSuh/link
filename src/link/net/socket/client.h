/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_CLIENT_H_
#define SRC_LINK_NET_SOCKET_CLIENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"

namespace nlink {
namespace net {

class Client {
 public:
  virtual void Connect() = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_CLIENT_H_
