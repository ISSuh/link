/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_H_
#define LINK_NET_HTTP_REQUEST_H_

#include <string>

namespace nlink {
namespace net {
namespace http {

class Request {
 public:
  Request();
  ~Request();

  bool HasHeader() const;
  size_t ContentLength() const;

 private:
 
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
