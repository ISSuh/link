/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/request_parser.h"

#include <string>

#include "link/base/logging.h"
#include "link/net/http/parser.h"

namespace nlink {
namespace net {
namespace http {

Request RequestParser::Parse(const base::Buffer& buffer, bool is_https) {
  std::string request_str(buffer.ToString());
  size_t request_size = request_str.size();

  Method method = Method::INVALID;
  std::string path("");
  Version version = Version::INVALID;

  HttpHeader header;
  std::string body("");

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
        break;
      }
      default: {
        LOG(ERROR) << "[RequestParser::Parse] invalid parse state";
        parsing = false;
        break;
      }
    }
  }

  const std::string host = header.Find("Host");
  if (host.empty()) {
    return Request();
  }

  std::string scheme = is_https ? "https" : "http";
  Uri::Authority authority = {"", "", host, 0};
  Uri uri(scheme, authority, path);
  return Request(method, uri, version, header, body);
}

}  // namespace http
}  // namespace net
}  // namespace nlink
