/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_HTTP_VERSION_H_
#define SRC_LINK_NET_HTTP_VERSION_H_

#include <string>

namespace nlink {
namespace net {
namespace http {

enum class Version {
  HTTP_1_0,
  HTTP_1_1,
  INVALID
};

const std::string VersionToString(Version method);
Version StringToVersion(const std::string Method_str);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_HTTP_VERSION_H_
