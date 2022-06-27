/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_PARSER_H_
#define LINK_NET_HTTP_REQUEST_PARSER_H_

#include <string>

#include "link/base/buffer.h"
#include "link/net/http/response.h"

namespace nlink {
namespace net {
namespace http {

class ResponseParser {
 public:
  virtual ~ResponseParser() = default;

  static Response Parse(const base::Buffer& buffer, bool is_https = false);
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_PARSER_H_
