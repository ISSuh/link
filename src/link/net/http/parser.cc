/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/parser.h"

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <set>

#include "link/base/logging.h"
#include "link/net/http/constant.h"
#include "link/net/http/method.h"

namespace nlink {
namespace net {
namespace http {

inline bool CheckEndOfStringByIndex(const std::string& str, size_t pos) {
  return pos == str.size() - 1;
}

Method ParseMethod(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos)) &&
         (message[pos] != kSpace)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string method_str = message.substr(*current_pos, size);
  Method method = StringToMethod(method_str);
  if (method == Method::INVALID) {
    LOG(WARNING) << "[ParseMethod] invalid request method. " << method_str;
    *state = Parser::ParseState::PARSE_ERROR;
    return Method::INVALID;
  }

  *current_pos = pos + kSpaceSize;
  *state = Parser::ParseState::PARSE_REQUEST_URI;
  return method;
}

const std::string ParseRequestPath(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos)) &&
         (message[pos] != kSpace)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string path = message.substr(*current_pos, size);
  if (path.empty()) {
    LOG(WARNING) << "[ParseRequestPath] invalid request path. ";
    *state = Parser::ParseState::PARSE_ERROR;
    return "";
  }

  *current_pos = pos + kSpaceSize;
  *state = Parser::ParseState::PARSE_HTTP_VERSION;
  return path;
}

HttpStatusCode ParseStatusCode(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos)) &&
         (message[pos] != kSpace)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string status_code_str = message.substr(*current_pos, size);
  int32_t status_code = std::stoi(status_code_str);
  if (status_code_str.empty() ||
      (status_code <= 0 || 505 < status_code)) {
    LOG(WARNING) << "[ParseStatusCode] invalid status code. ";
    *state = Parser::ParseState::PARSE_ERROR;
    return HttpStatusCode::INVALID_STATTUS_CODE;
  }

  *current_pos = pos + kCRorLFSize;
  *state = Parser::ParseState::PARSE_STATUS_REASON;
  return static_cast<HttpStatusCode>(status_code);
}

const std::string ParseStatusReason(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos + 1)) &&
        !(message[pos] == kCR && message[pos + 1] == kLF)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string reason = message.substr(*current_pos, size);
  if (reason.empty()) {
    LOG(WARNING) << "[ParseStatusReason] invalid status reason. ";
    *state = Parser::ParseState::PARSE_ERROR;
    return "";
  }

  *current_pos = pos + kCRLFSize;
  *state = Parser::ParseState::PARSE_HEADER;
  return reason;
}

Version ParseRequestHttpVersion(
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos + 1)) &&
        !(message[pos] == kCR && message[pos + 1] == kLF)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string version_str = message.substr(*current_pos, size);
  Version version = StringToVersion(version_str);
  if (version == Version::INVALID) {
    LOG(WARNING) << "[ParseMethod] invalid request http version. "
                 << version_str;
    *state = Parser::ParseState::PARSE_ERROR;
    return Version::INVALID;
  }

  *current_pos = pos + kCRLFSize;
  *state = Parser::ParseState::PARSE_HEADER;
  return version;
}

Version ParseResponseHttpVersion(
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state) {
  size_t pos = *current_pos;
  while (!CheckEndOfStringByIndex(message, pos) &&
         (message[pos] != kSpace)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string version_str = message.substr(*current_pos, size);
  Version version = StringToVersion(version_str);
  if (version == Version::INVALID) {
    LOG(WARNING) << "[ParseMethod] invalid response http version. "
                 << version_str;
    *state = Parser::ParseState::PARSE_ERROR;
    return Version::INVALID;
  }

  *current_pos = pos + kSpaceSize;
  *state = Parser::ParseState::PARSE_STATUS_CODE;
  return version;
}

Version ParseHttpVersion(
  Parser::Type type,
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state) {
  switch (type) {
    case Parser::Type::REQUEST:
      return ParseRequestHttpVersion(message, current_pos, state);
    case Parser::Type::RESPONSE:
      return ParseResponseHttpVersion(message, current_pos, state);
    default:
      return Version::INVALID;
  }
}

void ParseHeaders(
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state,
  HttpHeader* headers) {
  size_t remained_size = message.size() - *current_pos;
  std::stringstream stream(message.substr(*current_pos, remained_size));
  std::string header_str("");
  size_t pos = *current_pos;
  while (std::getline(stream, header_str, kLF)) {
    pos += header_str.size() + kCRorLFSize;
    if (header_str.size() == kCRorLFSize) {
      break;
    }

    if (!headers->ParseAndSet(header_str)) {
      *state = Parser::ParseState::PARSE_ERROR;
      return;
    }
  }

  *current_pos = pos;
  *state = Parser::ParseState::PARSE_BODY;
}

const std::string ParseBody(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t remained_size = message.size() - *current_pos;
  if (*current_pos >= message.size()) {
    return "";
  }

  std::string body = message.substr(*current_pos, remained_size);
  *current_pos += remained_size;
  *state = Parser::ParseState::PARSE_DONE;
  return body;
}

}  // namespace http
}  // namespace net
}  // namespace nlink
