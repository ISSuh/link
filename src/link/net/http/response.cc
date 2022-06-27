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

Response::~Response() {
}

HttpHeader Response::Header() const {
  if (!HasHeader()) {
    return HttpHeader();
  }
  return header_;
}
const std::string Response::Body() const {
  return body_;
}

bool Response::HasHeader() const {
  return !header_.Empty();
}

size_t Response::ContentLength() const {
  return body_.size();
}

const std::string Response::ContentType() const {
  const std::string content_type_key("Content-type");
  const std::string content_type = header_.Find(content_type_key);
  return content_type;
}

const std::string Response::Serialize() const {
  if (!HasHeader()) {
    return "";
  }

  const std::string status_code_num_string = std::to_string(
    static_cast<int32_t>(status_));

  std::stringstream stream;
  stream << VersionToString(version_) << ' '
         << status_code_num_string << ' '
         << HttpStatusCodeToString(status_)
         << VersionToString(version_)  << kCRLF;

  stream << header_.Serialize();
  stream << kCRLF;
  stream << body_;
  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
