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

Request::RequestLine::RequestLine()
  : RequestLine(Method::INVALID, Uri()) {
}

Request::RequestLine::RequestLine(
  Method http_method,
  const Uri& http_uri)
  : RequestLine(http_method, http_uri, Version::INVALID) {
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
  : Request(RequestLine()) {
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
  const std::string& body)
  : request_line_(request_line),
    header_(header),
    body_(body) {
}

Request::Request(
  RequestLine request_line,
  const HttpHeader& header,
  const std::string& body,
  const std::string& content_type)
  : request_line_(request_line),
    header_(header),
    body_(body) {
  header_.Set("host", request_line_.uri.Host());

  if (!body.empty() && !content_type.empty()) {
    header_.Set("content-type", content_type);
    header_.Set("content-length", body.size());
  }

  const std::string accept = header_.Find("accept");
  if (accept.empty()) {
    header_.Set("accept", "*/*");
  }

  header_.Set("connection", "keep-alive");
}

Request::~Request() = default;

bool Request::IsValid() const {
  if (Method::INVALID == request_line_.method) {
    return false;
  }

  if (Version::INVALID == request_line_.version) {
    return false;
  }

  if (request_line_.uri.Empty()) {
    return false;
  }

  return true;
}

const std::string Request::Serialize() const {
  std::stringstream stream;
  stream << MethodToString(request_line_.method) << ' '
         << request_line_.uri.PathWithQueryAndFragment() << ' '
         << VersionToString(request_line_.version)  << kCRLF;

  stream << header_.Serialize();
  stream << kCRLF;
  stream << body_;
  return stream.str();
}

const Uri Request::RequestUri() const {
  return request_line_.uri;
}

const std::string Request::UriOrigin() const {
  return request_line_.uri.Serialize();
}

const std::string Request::Path() const {
  return request_line_.uri.Path();
}

Method Request::ReqeustMethod() const {
  return request_line_.method;
}

Version Request::HttpVersion() const {
  return request_line_.version;
}

const HttpHeader Request::Header() const {
  return header_;
}

void Request::SetHeaderItem(const std::string& key, const std::string& value) {
  header_.Set(key, value);
}

void Request::SetHeaderItem(const std::string& key, int32_t value) {
  header_.Set(key, value);
}

const std::string Request::FindHeaderItem(const std::string key) const {
  return header_.Find(key);
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

bool Request::HasQuery() const {
  return request_line_.uri.HasQuery();
}

const std::string Request::FindQueryParam(const std::string key) const {
  return request_line_.uri.QueryParam(key);
}

bool Request::IsChunk() const {
  constexpr const char* kTransferEncodingKey = "Transfer-Encoding";
  constexpr const char* kChunkValue = "chunked";
  const std::string value = FindHeaderItem(kTransferEncodingKey);

  if (value.empty()) {
    return false;
  }

  if (0 != value.compare(kChunkValue)) {
    return false;
  }
  return true;
}

}  // namespace http
}  // namespace net
}  // namespace nlink
