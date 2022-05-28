/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_CLIENT_H_
#define SRC_LINK_NET_SOCKET_CLIENT_H_

#include <vector>
#include <memory>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/session.h"

namespace nlink {
namespace net {

class Client : public base::EventChannel {
 public:
  virtual void Connect(
    IpEndPoint endpoint,
    handler::ConnectHandler connect_handler,
    handler::CloseHandler close_handler) = 0;
  virtual void Disconnect() = 0;
  virtual void RegistIOHandler(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler) = 0;
  virtual void Write(const base::Buffer& buffer) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_CLIENT_H_
