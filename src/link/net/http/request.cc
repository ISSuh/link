/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/request.h"

#include <sstream>

namespace nlink {
namespace net {
namespace http {

const char* CRLF = "\r\n";

Request::Request(const std::string& method, Url url)
  : Request(method, url, Header()) {
}

Request::Request(
  const std::string& method, Url url, const Header& header)
  : version_("HTTP/1.1"),
    method_(method),
    url_(url),
    header_(header) {
}

Request::~Request() {
}

Header Request::GetHeader() const {
  if (!HasHeader()) {
    return Header();
  }
  return header_;
}
const std::string Request::GetBody() const {
  return body_;
}

bool Request::HasHeader() const {
  return header_.Empty();
}

size_t Request::ContentLength() const {
  return body_.size();
}

const std::string Request::ContentType() const {
  const std::string content_type_key("Content-type");
  const std::string content_type = header_.Find(content_type_key);
  return content_type;
}

const std::string Request::Serialize() const {
  std::stringstream stream;
  const std::string header(header_.Serialize());

  if (header.empty()) {
    return "";
  }

  stream << method_ << ' '
         << url_.PathWithQueryAndFragment() << ' '
         << version_  << CRLF;

  stream << "Host:" << url_.Host() << CRLF;
  stream << header;
  stream << CRLF;
  stream << body_;
  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
