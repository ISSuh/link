/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_REQUEST_H_
#define LINK_NET_HTTP_REQUEST_H_

#include <string>

#include "link/net/base/uri.h"
#include "link/net/http/method.h"
#include "link/net/http/version.h"
#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Request {
 public:
  Request();
  Request(Method method, Uri uri, Version version = Version::HTTP_1_1);
  Request(
    Method method, Uri uri, const HttpHeader& header);
  Request(
    Method method, Uri uri, const HttpHeader& header, const std::string& body);
  Request(
    Method method, Uri uri, Version version,
    const HttpHeader& header, const std::string& body);
  ~Request();

  Uri RequestUri() const;
  HttpHeader Header() const;
  const std::string Body() const;

  bool HasHeader() const;
  bool HasBody() const;

  size_t ContentLength() const;
  const std::string ContentType() const;

  const std::string Serialize() const;

 private:
  Version version_;
  Method method_;
  Uri uri_;

  HttpHeader header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
