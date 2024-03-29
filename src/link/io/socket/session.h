/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SESSION_H_
#define LINK_NET_SOCKET_SESSION_H_

#include <memory>

#include "link/base/buffer.h"
#include "link/base/callback.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/handler.h"

namespace nlink {
namespace io {

class Session {
 public:
  virtual ~Session() = default;

  virtual int32_t SessionId() const = 0;

  // virtual const IpEndPoint LocalEndPoint() = 0;
  // virtual const IpEndPoint RemoteEndPoint() = 0;
  virtual bool IsConnected() const = 0;

  virtual void Open(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler,
    handler::CloseHandler close_handler) = 0;
  virtual void Close() = 0;

  virtual void Read() = 0;
  virtual void Write(std::shared_ptr<base::Buffer> buffer) = 0;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SESSION_H_
