/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_SERVER_HTTP_UTIL_H_
#define LINK_COMPONENT_HTTP_SERVER_HTTP_UTIL_H_

#include "link/net/http/response.h"

namespace nlink {
namespace component {
namespace http {

net::http::Response Default404Error();

}  // namespace http
}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_SERVER_HTTP_UTIL_H_
