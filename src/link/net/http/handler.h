/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_HANDLER_H_
#define LINK_NET_HTTP_HANDLER_H_

#include <functional>

#include "link/net/http/request.h"
#include "link/net/http/response.h"

namespace nlink {
namespace net {
namespace http {
namespace handler {

using RequsetHandler = std::function<void(const net::http::Response&)>;
using ResponseHandler =
  std::function<void(const net::http::Request&, net::http::Response*)>;

}  // namespace handler
}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_HANDLER_H_
