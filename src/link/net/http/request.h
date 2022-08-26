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
  struct RequestLine {
    RequestLine();
    RequestLine(Method http_method, const Uri& http_uri);
    RequestLine(
      Method http_method,
      const Uri& http_uri,
      Version http_version);

    Method method;
    Uri uri;
    Version version;
  };

  Request();
  explicit Request(RequestLine request_line);
  Request(
    RequestLine request_line, const HttpHeader& header);
  Request(
    RequestLine request_line,
    const HttpHeader& header,
    const std::string& body);
  Request(
    RequestLine request_line,
    const HttpHeader& header,
    const std::string& body,
    const std::string& content_type);

  ~Request();

  bool IsValid() const;
  const std::string Serialize() const;

  Method ReqeustMethod() const;
  Version HttpVersion() const;

  const Uri RequestUri() const;
  const std::string UriOrigin() const;
  const std::string Path() const;

  bool HasQuery() const;
  const std::string FindQueryParam(const std::string key) const;

  const HttpHeader Header() const;
  void SetHeaderItem(const std::string& key, const std::string& value);
  void SetHeaderItem(const std::string& key, int32_t value);
  const std::string FindHeaderItem(const std::string key) const;

  const std::string Body() const;
  void SetBody(const std::string& content, const std::string& content_type);

  bool HasBody() const;
  size_t ContentLength() const;
  const std::string ContentType() const;

 private:
  RequestLine request_line_;
  HttpHeader header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_REQUEST_H_
