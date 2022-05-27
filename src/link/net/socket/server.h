/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SERVER_H_
#define LINK_NET_SOCKET_SERVER_H_

#include <vector>
#include <memory>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/base/event/event_channel.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/session.h"

namespace nlink {
namespace net {

class Server : public base::EventChannel {
 public:
  using AcceptHandler = base::Callback<void(std::shared_ptr<Session> session)>;
  using CloseHandler = base::Callback<void(std::shared_ptr<Session> session)>;
  using ReadHandler = base::Callback<void(const base::Buffer& buffer)>;
  using WriteHandler = base::Callback<void(size_t length)>;

  virtual int32_t Listen(const IpEndPoint& address) = 0;
  virtual int32_t Accept(base::CompletionCallback callback) = 0;
  virtual void Close() = 0;

  virtual void RegistAcceptHandler(AcceptHandler handler) = 0;
  virtual void RegistCloseHandler(CloseHandler handler) = 0;
  virtual void RegistReadHandler(ReadHandler handler) = 0;
  virtual void RegistWriteHandler(WriteHandler handler) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SERVER_H_
