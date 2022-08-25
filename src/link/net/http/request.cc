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

    Method method;
    Uri uri;
    Version version;

HttpHeader CreateDefaultHeader() {
  HttpHeader header;
  header.Set("User-Agent", "nLink 0.0.1");
  header.Set("Connection", "Close");

  return header;
}

Request::RequestLine::RequestLine()
  : RequestLine(Method::GET, Uri(), Version::HTTP_1_1) {
}

Request::RequestLine::RequestLine(
  Method http_method,
  const Uri& http_uri,
  Version http_version)
  : method(http_method),
    uri(http_uri),
    version(http_version) {
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
  Method method, Uri uri, const HttpHeader& header, const std::string& body)
  : Request(method, uri, Version::HTTP_1_1, header, body) {
}

Request::Request(
  Method method, Uri uri, Version version,
  const HttpHeader& header, const std::string& body)
  : version_(version),
    method_(method),
    uri_(uri),
    header_(header),
    body_(body) {
}

Request::Request(RequestLine request_line)
  : Request(request_line, HttpHeader()) {
}

Request::Request(
  RequestLine request_line, const HttpHeader& header)
  : Request(request_line, header, "", "") {
}

Request::Request(
  RequestLine request_line,
  const HttpHeader& header,
  const std::string& body,
  const std::string& content_type)
  : request_line_(request_line),
    header_(header),
    body_(body) {
  if (!body.empty() && !content_type.empty()) {
    header_.Set("content-type", content_type);
    header_.Set("content-length", body.size());
  }
}

Request::~Request() = default;

Uri Request::RequestUri() const {
  return uri_;
}

HttpHeader Request::Header() const {
  return header_;
}

void Request::SetHeaderItem(const std::string& key, const std::string& value) {
  header_.Set(key, value);
}

void Request::SetHeaderItem(const std::string& key, int32_t value) {
  header_.Set(key, value);
}

const std::string Request::Body() const {
  return body_;
}

void Request::SetBody(
  const std::string& content, const std::string& content_type) {
  body_ = content;
  header_.Set("content-type", content_type);
  header_.Set("content-length", content.size());
}

bool Request::HasBody() const {
  return !body_.empty();
}

size_t Request::ContentLength() const {
  return body_.size();
}

const std::string Request::ContentType() const {
  if (!HasBody()) {
    return "";
  }

  const std::string content_type_key("content-type");
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
