/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_RESPONSE_H_
#define LINK_NET_HTTP_RESPONSE_H_

#include <string>

#include "link/net/base/uri.h"
#include "link/net/http/method.h"
#include "link/net/http/status_code.h"
#include "link/net/http/version.h"
#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Response {
 public:
  Response();
  explicit Response(HttpStatusCode status, Version version = Version::HTTP_1_1);
  Response(HttpStatusCode status, const HttpHeader& header);
  Response(
    HttpStatusCode status, Version version,
    const HttpHeader& header, const std::string& body);
  ~Response();

  HttpHeader Header() const;
  const std::string Body() const;

  bool HasHeader() const;
  bool HasBody() const;

  size_t ContentLength() const;
  const std::string ContentType() const;

  const std::string Serialize() const;

 private:
  HttpStatusCode status_;
  Version version_;

  HttpHeader header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_RESPONSE_H_
