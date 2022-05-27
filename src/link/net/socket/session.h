/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SESSION_H_
#define LINK_NET_SOCKET_SESSION_H_

#include "link/base/buffer.h"
#include "link/net/base/ip_endpoint.h"

namespace nlink {
namespace net {

class Session {
 public:
  virtual const IpEndPoint LocalEndPoint() = 0;
  virtual const IpEndPoint RemoteEndPoint() = 0;
  virtual bool IsConnected() const = 0;

  virtual void Open() = 0;
  virtual void Close() = 0;

  virtual void Write(const base::Buffer& buffer) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SESSION_H_
