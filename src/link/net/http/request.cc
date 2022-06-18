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

Request::Request()
  : Request(Method::INVALID, Url()) {
}

Request::Request(Method method, Url url, Version version)
  : Request(method, url, version, Header(), "") {
}

Request::Request(
  Method method, Url url, const Header& header)
  : Request(method, url, Version::HTTP_1_1, header, "") {
}

Request::Request(
  Method method, Url url, Version version,
  const Header& header, const std::string& body)
  : version_(version),
    method_(method),
    url_(url),
    header_(header),
    body_(body) {
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
  return !header_.Empty();
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
  if (!HasHeader()) {
    return "";
  }

  std::stringstream stream;
  stream << MethodToString(method_) << ' '
         << url_.PathWithQueryAndFragment() << ' '
         << VersionToString(version_)  << CRLF;

  stream << header_.Serialize();
  stream << CRLF;
  stream << body_;
  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
