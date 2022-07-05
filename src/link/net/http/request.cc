/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/request.h"

#include <sstream>

#include "link/net/http/constant.h"

namespace nlink {
namespace net {
namespace http {

HttpHeader CreateDefaultHeader() {
  HttpHeader header;
  header.Set("User-Agent", "nLink 0.0.1");
  header.Set("Connection", "Close");

  return header;
}

Request::Request()
  : Request(Method::INVALID, Uri()) {
}

Request::Request(Method method, Uri uri, Version version)
  : Request(method, uri, version, CreateDefaultHeader(), "") {
}

Request::Request(
  Method method, Uri uri, const HttpHeader& header)
  : Request(method, uri, Version::HTTP_1_1, header, "") {
}

Request::Request(
  Method method, Uri uri, Version version,
  const HttpHeader& header, const std::string& body)
  : version_(version),
    method_(method),
    uri_(uri),
    header_(header),
    body_(body) {
  header_.Set("Host", uri_.HostAndPortIfHasPort());
  header_.Set("Accept", "*/*");
}

Request::~Request() {
}

HttpHeader Request::Header() const {
  if (!HasHeader()) {
    return HttpHeader();
  }
  return header_;
}

const std::string Request::Body() const {
  if (!HasBody()) {
    return "";
  }
  return body_;
}

bool Request::HasHeader() const {
  return !header_.Empty();
}

bool Request::HasBody() const {
  return !body_.empty();
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
  stream << MethodToString(method_) << ' '
         << uri_.PathWithQueryAndFragment() << ' '
         << VersionToString(version_)  << kCRLF;

  stream << header_.Serialize();
  stream << kCRLF;
  stream << body_;
  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
