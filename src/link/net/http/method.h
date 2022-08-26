/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_METHOD_H_
#define LINK_NET_HTTP_METHOD_H_

#include <string>

namespace nlink {
namespace net {
namespace http {

enum class Method {
  INVALID,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE
};

const std::string MethodToString(Method method);
Method StringToMethod(const std::string method_str);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_METHOD_H_
