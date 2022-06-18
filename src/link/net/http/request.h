/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_H_
#define LINK_NET_HTTP_REQUEST_H_

#include <string>

#include "link/net/http/method.h"
#include "link/net/http/version.h"
#include "link/net/http/url.h"
#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Request {
 public:
  Request();
  Request(Method method, Url url, Version version = Version::HTTP_1_1);
  Request(
    Method method, Url url, const Header& header);
  Request(
    Method method, Url url, Version version,
    const Header& header, const std::string& body);
  ~Request();

  Header GetHeader() const;
  const std::string GetBody() const;

  bool HasHeader() const;
  size_t ContentLength() const;
  const std::string ContentType() const;

  const std::string Serialize() const;

 private:
  Version version_;
  Method method_;
  Url url_;

  Header header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
