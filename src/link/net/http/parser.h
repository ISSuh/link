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
#include "link/net/http/response.h"
#include "link/net/http/chunk.h"

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
    PARSE_CHUNK_SIZE,
    PARSE_CHUNK_BODY,
    PARSE_DONE,
    PARSE_ERROR,
  };

  static Request ParseRequest(
    const base::Buffer& buffer, bool is_https = false);

  static Response ParseResponse(
    const base::Buffer& buffer, bool is_https = false);

  static Chunk ParseChunk(const base::Buffer& buffer);
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

int32_t ParseChunkSize(
  const std::string& message, size_t* current_pos, Parser::ParseState* state);

const std::string ParseChunkBody(
  const std::string& message, size_t* current_pos,
  Parser::ParseState* state, int32_t chunk_size);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_PARSER_H_
