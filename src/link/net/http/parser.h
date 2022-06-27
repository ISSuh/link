/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_PARSER_H_
#define LINK_NET_HTTP_PARSER_H_

#include <string>

#include "link/base/buffer.h"
#include "link/net/http/method.h"
#include "link/net/http/version.h"
#include "link/net/http/status_code.h"
#include "link/net/http/header.h"
#include "link/net/http/request.h"

namespace nlink {
namespace net {
namespace http {

class Parser {
 public:
  enum class Type {
    REQUEST,
    RESPONSE
  };

  enum class ParseState {
    PARSE_METHOD,
    PARSE_REQUEST_URI,
    PARSE_HTTP_VERSION,
    PARSE_STATUS_CODE,
    PARSE_STATUS_REASON,
    PARSE_HEADER,
    PARSE_HEADER_END,
    PARSE_BODY,
    PARSE_BODY_END,
    PARSE_DONE,
    PARSE_ERROR,
  };
};

Method ParseMethod(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

const std::string ParseRequestPath(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

HttpStatusCode ParseStatusCode(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

const std::string ParseStatusReason(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

Version ParseHttpVersion(
  Parser::Type type,
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state);

void ParseHeaders(
  const std::string& message,
  size_t* current_pos,
  Parser::ParseState* state,
  HttpHeader* headers);

const std::string ParseBody(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_PARSER_H_
