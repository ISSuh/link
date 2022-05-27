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
  using ConnectHandler = base::Callback<void(std::shared_ptr<Session>)>;
  using CloseHandler = base::Callback<void(std::shared_ptr<Session>)>;
  using ReadHandler = base::Callback<void(const base::Buffer&)>;
  using WriteHandler = base::Callback<void(size_t)>;

  virtual void Connect(
    IpEndPoint endpoint,
    ConnectHandler connect_handler, CloseHandler close_handler) = 0;
  virtual void DisConnect() = 0;
  virtual void Write(const base::Buffer& buffer, WriteHandler handler) = 0;

  virtual void RegistReadHandler(ReadHandler handler) = 0;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_CLIENT_H_
