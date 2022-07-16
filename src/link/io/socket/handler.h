/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_HANDLER_H_
#define LINK_NET_SOCKET_ASIO_HANDLER_H_

#include <memory>
#include <functional>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/io/socket/session.h"

namespace nlink {
namespace io {

class Session;

namespace handler {

using AcceptHandler = std::function<void(std::shared_ptr<io::Session>)>;
using ConnectHandler = std::function<void(std::shared_ptr<io::Session>)>;
using CloseHandler = std::function<void(std::shared_ptr<io::Session>)>;
using ReadHandler =
  std::function<void(const base::Buffer&, std::shared_ptr<io::Session>)>;
using WriteHandler = std::function<void(size_t)>;

}  // namespace handler
}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_HANDLER_H_
