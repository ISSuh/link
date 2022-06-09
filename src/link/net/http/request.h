/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_H_
#define LINK_NET_HTTP_REQUEST_H_

#include <string>

#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Request {
 public:
  Request();
  ~Request();

  bool HasHeader() const;
  size_t ContentLength() const;

  std::string ToString() const;

 private:
  Header header_;
  
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
