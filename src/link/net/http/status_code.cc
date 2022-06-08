/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/status_code.h"

namespace nlink {
namespace net {
namespace http {

std::string HttpStatusCodeToStr(HttpStatusCode status) {
  switch (status) {
    case HttpStatusCode::CONTINUE:
      return "Continue";
    case HttpStatusCode::SWITCHING_PROTOCOLS:
      return "Switching Protocols";
    case HttpStatusCode::EARLY_HINTS:
      return "Early Hints";
    case HttpStatusCode::OK:
      return "OK";
    case HttpStatusCode::CREATED:
      return "Created";
    case HttpStatusCode::ACCEPTED:
      return "Accepted";
    case HttpStatusCode::NON_AUTHORITATIVE_INFORMATION:
      return "Non-Authoritative Information";
    case HttpStatusCode::NO_CONTENT:
      return "No Content";
    case HttpStatusCode::RESET_CONTENT:
      return "Reset Content";
    case HttpStatusCode::PARTIAL_CONTENT:
      return "Partial Content";
    case HttpStatusCode::MULTIPLE_CHOICES:
      return "Multiple Choices";
    case HttpStatusCode::MOVED_PERMANENTLY:
      return "Moved Permanently";
    case HttpStatusCode::FOUND:
      return "Found";
    case HttpStatusCode::SEE_OTHER:
      return "See Other";
    case HttpStatusCode::NOT_MODIFIED:
      return "Not Modified";
    case HttpStatusCode::USE_PROXY:
      return "Use Proxy";
    case HttpStatusCode::TEMPORARY_REDIRECT:
      return "Temporary Redirect";
    case HttpStatusCode::PERMANENT_REDIRECT:
      return "Permanent Redirect";
    case HttpStatusCode::BAD_REQUEST:
      return "Bad Request";
    case HttpStatusCode::UNAUTHORIZED:
      return "Unauthorized";
    case HttpStatusCode::PAYMENT_REQUIRED:
      return "Payment Required";
    case HttpStatusCode::FORBIDDEN:
      return "Forbidden";
    case HttpStatusCode::NOT_FOUND:
      return "Not Found";
    case HttpStatusCode::METHOD_NOT_ALLOWED:
      return "Method Not Allowed";
    case HttpStatusCode::NOT_ACCEPTABLE:
      return "Not Acceptable";
    case HttpStatusCode::PROXY_AUTHENTICATION_REQUIRED:
      return "Proxy Authentication Required";
    case HttpStatusCode::REQUEST_TIMEOUT:
      return "Request Timeout";
    case HttpStatusCode::CONFLICT:
      return "Conflict";
    case HttpStatusCode::GONE:
      return "Gone";
    case HttpStatusCode::LENGTH_REQUIRED:
      return "Length Required";
    case HttpStatusCode::PRECONDITION_FAILED:
      return "Precondition Failed";
    case HttpStatusCode::REQUEST_ENTITY_TOO_LARGE:
      return "Request Entity Too Large";
    case HttpStatusCode::REQUEST_URI_TOO_LONG:
      return "Request-URI Too Long";
    case HttpStatusCode::UNSUPPORTED_MEDIA_TYPE:
      return "Unsupported Media Type";
    case HttpStatusCode::REQUESTED_RANGE_NOT_SATISFIABLE:
      return "Requested Range Not Satisfiable";
    case HttpStatusCode::EXPECTATION_FAILED:
      return "Expectation Failed";
    case HttpStatusCode::INVALID_XPRIVET_TOKEN:
      return "Invalid XPrivet Token";
    case HttpStatusCode::TOO_EARLY:
      return "Too Early";
    case HttpStatusCode::TOO_MANY_REQUESTS:
      return "Too Many Requests";
    case HttpStatusCode::INTERNAL_SERVER_ERROR:
      return "Internal Server Error";
    case HttpStatusCode::NOT_IMPLEMENTED:
      return "Not Implemented";
    case HttpStatusCode::BAD_GATEWAY:
      return "Bad Gateway";
    case HttpStatusCode::SERVICE_UNAVAILABLE:
      return "Service Unavailable";
    case HttpStatusCode::GATEWAY_TIMEOUT:
      return "Gateway Timeout";
    case HttpStatusCode::VERSION_NOT_SUPPORTED:
      return "HTTP Version Not Supported";
    default:
      return "not defined";
  }
}

}  // namespace http
}  // namespace net
}  // namespace nlink
