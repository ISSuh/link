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
#include <cstdlib>

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
    LOG(WARNING) << "[ParseStatusReason] invalid status reason.";
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
    pos += header_str.size() + kCRLFSize + kCRLFSize;
    if (header_str.size() == kCRorLFSize) {
      break;
    }

    // remove CR on header line
    header_str.pop_back();

    if (!headers->Set(header_str)) {
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
    *state = Parser::ParseState::PARSE_DONE;
    return "";
  }

  std::string body = message.substr(*current_pos, remained_size);

  *current_pos += remained_size;
  *state = Parser::ParseState::PARSE_DONE;

  return body;
}

int32_t ParseChunkSize(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos + 1)) &&
        !(message[pos] == kCR && message[pos + 1] == kLF)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string chunk_size_str = message.substr(*current_pos, size);
  int32_t chunk_size = std::strtol(chunk_size_str.c_str(), nullptr, 16);

  *current_pos = pos + kCRLFSize;
  *state = Parser::ParseState::PARSE_CHUNK_BODY;

  return chunk_size;
}

const std::string ParseChunkBody(
  const std::string& message, size_t* current_pos, Parser::ParseState* state) {
  size_t pos = *current_pos;
  while ((!CheckEndOfStringByIndex(message, pos + 1)) &&
        !(message[pos] == kCR && message[pos + 1] == kLF)) {
    ++pos;
  }

  size_t size = pos - *current_pos;
  const std::string chunk_body_str = message.substr(*current_pos, size);

  *current_pos = pos + kCRLFSize;
  *state = Parser::ParseState::PARSE_DONE;

  return chunk_body_str;
}

Request Parser::ParseRequest(const base::Buffer& buffer, bool is_https) {
  std::string request_str(buffer.ToString());

  Method method = Method::INVALID;
  std::string path("");
  Version version = Version::INVALID;

  HttpHeader header;
  std::string body("");

  bool parse_error = false;
  bool parsing = true;
  Parser::ParseState state = Parser::ParseState::PARSE_METHOD;
  size_t current_pos = 0;
  while (parsing) {
    switch (state) {
      case Parser::ParseState::PARSE_METHOD: {
        method = ParseMethod(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_REQUEST_URI: {
        path = ParseRequestPath(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_HTTP_VERSION: {
        version = ParseHttpVersion(
          Parser::Type::REQUEST, request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_HEADER: {
        ParseHeaders(request_str, &current_pos, &state, &header);
        break;
      }
      case Parser::ParseState::PARSE_BODY: {
        body = ParseBody(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_DONE: {
        parsing = false;
        break;
      }
      case Parser::ParseState::PARSE_ERROR: {
        LOG(ERROR) << "[RequestParser::Parse] request parse error";
        parsing = false;
        parse_error = true;
        break;
      }
      default: {
        LOG(ERROR) << "[RequestParser::Parse] invalid parse state";
        parsing = false;
        break;
      }
    }
  }

  if (parse_error) {
    return Request();
  }

  const std::string host = header.Find("Host");
  if (host.empty()) {
    return Request();
  }

  std::string scheme = is_https ? "https" : "http";
  Uri::Authority authority = {"", "", host, 0};
  Uri uri(scheme, authority, path);

  Request::RequestLine request_line = {method, uri, version};
  return Request(request_line, header, body);
}

Response Parser::ParseResponse(const base::Buffer& buffer, bool is_https) {
  std::string request_str(buffer.ToString());

  HttpStatusCode status = HttpStatusCode::NOT_FOUND;
  std::string reason("");
  Version version = Version::INVALID;

  HttpHeader header;
  std::string body("");

  bool parse_error = false;
  bool parsing = true;
  Parser::ParseState state = Parser::ParseState::PARSE_HTTP_VERSION;
  size_t current_pos = 0;
  while (parsing) {
    switch (state) {
      case Parser::ParseState::PARSE_HTTP_VERSION: {
        version = ParseHttpVersion(
          Parser::Type::RESPONSE, request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_STATUS_CODE: {
        status = ParseStatusCode(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_STATUS_REASON: {
        reason = ParseStatusReason(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_HEADER: {
        ParseHeaders(request_str, &current_pos, &state, &header);
        break;
      }
      case Parser::ParseState::PARSE_BODY: {
        body = ParseBody(request_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_DONE: {
        parsing = false;
        break;
      }
      case Parser::ParseState::PARSE_ERROR: {
        LOG(ERROR) << "[ResponseParser::Parse] response parse error";
        parsing = false;
        parse_error = true;
        break;
      }
      default: {
        LOG(ERROR) << "[ResponseParser::Parse] invalid parse state";
        parsing = false;
        break;
      }
    }
  }

  if (parse_error) {
    return Response();
  }

  Response::StatusLine status_line(status, "", version);
  if (body.empty()) {
    return Response(status_line, header);
  }
  return Response(status_line, header, body);
}

Chunk Parser::ParseChunk(const base::Buffer& buffer) {
  std::string chunk_str(buffer.ToString());

  int32_t chunk_size = -1;
  std::string chunk_body("");

  bool parse_error = false;
  bool parsing = true;
  Parser::ParseState state = Parser::ParseState::PARSE_CHUNK_SIZE;
  size_t current_pos = 0;
  while (parsing) {
    switch (state) {
      case Parser::ParseState::PARSE_CHUNK_SIZE: {
        chunk_size = ParseChunkSize(chunk_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_CHUNK_BODY: {
        chunk_body = ParseChunkBody(chunk_str, &current_pos, &state);
        break;
      }
      case Parser::ParseState::PARSE_DONE: {
        parsing = false;
        break;
      }
      case Parser::ParseState::PARSE_ERROR: {
        LOG(ERROR) << "[RequestParser::Parse] request parse error";
        parsing = false;
        parse_error = true;
        break;
      }
      default: {
        LOG(ERROR) << "[ResponseParser::ParseChunk] invalid parse state";
        parsing = false;
        break;
      }
    }
  }

  if (parse_error) {
    return Chunk();
  }

  if (chunk_size != chunk_body.size()) {
    LOG(ERROR) << "[ResponseParser::ParseChunk] invalid size of chunk";
    return Chunk();
  }

  return Chunk(chunk_body);
}

}  // namespace http
}  // namespace net
}  // namespace nlink
