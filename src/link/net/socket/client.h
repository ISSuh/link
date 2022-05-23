/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_CLIENT_H_
#define SRC_LINK_NET_SOCKET_CLIENT_H_

#include <vector>

#include "link/net/base/ip_endpoint.h"

namespace nlink {
namespace net {

class Client : public base::EventChannel {
 public:
  virtual void Connect(IpEndPoint endpoint) = 0;
  virtual void DisConnect() = 0;
  virtual void Write(const std::vector<uint8_t>& buffer) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_CLIENT_H_
