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
#include "link/net/socket/session.h"

namespace nlink {
namespace net {
namespace handler {

using ConnectHandler = base::Callback<void(std::shared_ptr<Session>)>;
using CloseHandler = base::Callback<void(std::shared_ptr<Session>)>;
using ReadHandler = base::Callback<void(const base::Buffer&)>;
using WriteHandler = base::Callback<void(size_t)>;

}  // namespace handler
}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_SOCKET_ASIO_HANDLER_H_
