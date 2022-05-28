/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SERVER_H_
#define LINK_NET_SOCKET_SERVER_H_

#include <vector>
#include <memory>

#include "link/base/event/event_channel.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/session.h"

namespace nlink {
namespace net {

class Server : public base::EventChannel {
 public:
  virtual bool Listen(const IpEndPoint& address) = 0;
  virtual void Accept(
    handler::AcceptHandler accept_handler,
    handler::CloseHandler close_handler) = 0;
  virtual void Close() = 0;
  virtual void RegistIOHandler(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SERVER_H_
