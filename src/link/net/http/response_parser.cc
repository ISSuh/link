/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/response_parser.h"

#include <string>

#include "link/base/logging.h"
#include "link/net/http/parser.h"

namespace nlink {
namespace net {
namespace http {

Response ResponseParser::Parse(const base::Buffer& buffer, bool is_https) {
  std::string request_str(buffer.ToString());
  size_t response_size = request_str.size();

  HttpStatusCode status = HttpStatusCode::NOT_FOUND;
  std::string reason("");
  Version version = Version::INVALID;

  HttpHeader header;
  std::string body("");

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
        break;
      }
      default: {
        LOG(ERROR) << "[ResponseParser::Parse] invalid parse state";
        parsing = false;
        break;
      }
    }
  }

  return Response(status, version, header, body);
}

}  // namespace http
}  // namespace net
}  // namespace nlink
