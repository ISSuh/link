/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_PARSER_H_
#define LINK_NET_HTTP_REQUEST_PARSER_H_

#include <string>

#include "link/base/buffer.h"
#include "link/net/http/request.h"

namespace nlink {
namespace net {
namespace http {

class RequestParser {
 public:
  virtual ~RequestParser() = default;

  static Request Parse();
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_PARSER_H_
