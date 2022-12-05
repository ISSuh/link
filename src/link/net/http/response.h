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
  struct StatusLine {
    StatusLine();
    StatusLine(
      HttpStatusCode status_code,
      const std::string& url_path,
      Version http_version);

    HttpStatusCode status;
    std::string path;
    Version version;
  };

  Response();
  explicit Response(StatusLine status_line);
  Response(StatusLine status_line, const HttpHeader& header);
  Response(
    StatusLine status_line,
    const HttpHeader& header,
    const std::string& body);
  Response(
    StatusLine status_line,
    const HttpHeader& header,
    const std::string& body,
    const std::string& content_type);

  ~Response();

  HttpHeader Header() const;
  void SetHeaderItem(const std::string& key, const std::string& value);
  void SetHeaderItem(const std::string& key, int32_t value);
  const std::string FindHeaderItem(const std::string key) const;

  const std::string Body() const;
  void SetBody(const std::string& content, const std::string& content_type);

  bool HasBody() const;
  size_t ContentLength() const;
  const std::string ContentType() const;

  const std::string Serialize() const;

  bool IsChunk() const;

 private:
  StatusLine status_line_;
  HttpHeader header_;
  std::string body_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_RESPONSE_H_
