/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_H_
#define LINK_NET_HTTP_REQUEST_H_

#include <string>

#include "link/net/http/url.h"
#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Request {
 public:
  Request(const std::string& method, Url url);
  Request(
    const std::string& method, Url url, const Header& header);
  ~Request();

  Header GetHeader() const;
  const std::string GetBody() const;

  bool HasHeader() const;
  size_t ContentLength() const;
  const std::string ContentType() const;

  const std::string Serialize() const;

 private:
  std::string method_;
  std::string version_;
  Url url_;

  Header header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
