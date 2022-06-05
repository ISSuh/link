/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_ASIO_HANDLER_H_
#define SRC_LINK_NET_SOCKET_ASIO_HANDLER_H_

#include <memory>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/io/socket/session.h"

namespace nlink {
namespace io {

class Session;

namespace handler {

using AcceptHandler = base::Callback<void(std::shared_ptr<io::Session>)>;
using ConnectHandler = base::Callback<void(std::shared_ptr<io::Session>)>;
using CloseHandler = base::Callback<void(std::shared_ptr<io::Session>)>;
using ReadHandler =
  base::Callback<void(const base::Buffer&, std::shared_ptr<io::Session>)>;
using WriteHandler = base::Callback<void(size_t)>;

}  // namespace handler
}  // namespace io
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_ASIO_HANDLER_H_
