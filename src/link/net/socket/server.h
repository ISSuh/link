/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SERVER_H_
#define LINK_NET_SOCKET_SERVER_H_

#include <vector>

#include "link/base/callback/callback.h"
#include "link/base/event/event_channel.h"
#include "link/net/base/ip_endpoint.h"

namespace nlink {
namespace net {

class Server : public base::EventChannel {
 public:
  using ReadHandler = base::Callback<void(const std::vector<uint8_t>& buffer)>;

  virtual int32_t Listen(const IpEndPoint& address) = 0;
  virtual int32_t Accept(base::CompletionCallback callback) = 0;
  virtual void RegistReadHandler(ReadHandler read_handler) = 0;
  virtual void Close() = 0;
};

class Writer {
 public:
};

class Reader {
 public:
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SERVER_H_
