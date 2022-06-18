/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/method.h"

#include <map>

namespace nlink {
namespace net {
namespace http {

std::map<std::string, Method> kMethodMap = {
  {"GET", Method::GET},
  {"HEAD", Method::HEAD},
  {"POST", Method::POST},
  {"PUT", Method::PUT},
  {"DELETE", Method::DELETE},
  {"CONNECT", Method::CONNECT},
  {"OPTIONS", Method::OPTIONS},
  {"TRACE", Method::TRACE}};

const std::string MethodToString(Method method) {
  switch (method) {
    case Method::GET:
      return "GET";
    case Method::HEAD:
      return "HEAD";
    case Method::POST:
      return "POST";
    case Method::PUT:
      return "PUT";
    case Method::DELETE:
      return "DELETE";
    case Method::CONNECT:
      return "CONNECT";
    case Method::OPTIONS:
      return "OPTIONS";
    case Method::TRACE:
      return "TRACE";
    default:
      return "";
  }
}

Method StringToMethod(const std::string method_str) {
  if (kMethodMap.find(method_str) == kMethodMap.end()) {
    return Method::INVALID;
  }
  return kMethodMap[method_str];
}

}  // namespace http
}  // namespace net
}  // namespace nlink
