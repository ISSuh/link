/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/response.h"

#include <sstream>

#include "link/net/http/constant.h"

namespace nlink {
namespace net {
namespace http {

Response::StatusLine::StatusLine()
  : StatusLine(HttpStatusCode::NOT_FOUND, "", Version::HTTP_1_1) {
}

Response::StatusLine::StatusLine(
  HttpStatusCode status_code,
  const std::string& url_path,
  Version http_version)
  : status(status_code),
    path(url_path),
    version(http_version) {
}

Response::Response()
  : Response(HttpStatusCode::NOT_FOUND) {
}

Response::Response(HttpStatusCode status, Version version)
  : Response(status, version, HttpHeader(), "") {
}

Response::Response(HttpStatusCode status, const HttpHeader& header)
  : Response(status, Version::HTTP_1_1, header, "") {
}

Response::Response(
  HttpStatusCode status, Version version,
  const HttpHeader& header, const std::string& body)
  : status_(status),
    version_(version),
    header_(header),
    body_(body) {
}

Response::Response(StatusLine status_line)
  : Response(status_line, HttpHeader()) {
}

Response::Response(
  StatusLine status_line, const HttpHeader& header)
  : Response(status_line, HttpHeader(), "", "") {
}

Response::Response(
  StatusLine status_line,
  const HttpHeader& header,
  const std::string& body,
  const std::string& content_type)
  : status_line_(status_line),
    header_(header),
    body_(body) {
  if (!body.empty() && !content_type.empty()) {
    header_.Set("content-type", content_type);
    header_.Set("content-length", body.size());
  }
}

Response::~Response() {
}

HttpHeader Response::Header() const {
  return header_;
}

void Response::SetHeaderItem(const std::string& key, const std::string& value) {
  header_.Set(key, value);
}

void Response::SetHeaderItem(const std::string& key, int32_t value) {
  header_.Set(key, value);
}

const std::string Response::Body() const {
  return body_;
}

void Response::SetBody(
  const std::string& content, const std::string& content_type) {
  body_ = content;
  header_.Set("content-type", content_type);
  header_.Set("content-length", content.size());
}

bool Response::HasBody() const {
  return !body_.empty();
}

size_t Response::ContentLength() const {
  return body_.size();
}

const std::string Response::ContentType() const {
  if (!HasBody()) {
    return "";
  }

  const std::string content_type_key("content-type");
  const std::string content_type = header_.Find(content_type_key);
  return content_type;
}

const std::string Response::Serialize() const {
  const std::string status_code_num_string = std::to_string(
    static_cast<int32_t>(status_));

  std::stringstream stream;
  stream << VersionToString(version_) << ' '
         << status_code_num_string << ' '
         << HttpStatusCodeToString(status_)
         << kCRLF;

  stream << header_.Serialize();
  stream << kCRLF;
  stream << body_;
  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
