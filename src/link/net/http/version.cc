/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/version.h"

#include <map>

namespace nlink {
namespace net {
namespace http {

std::map<std::string, Version> kVersionMap = {
  {"HTTP/1.0", Version::HTTP_1_0},
  {"HTTP/1.1", Version::HTTP_1_1}};

const std::string VersionToString(Version version) {
  switch (version) {
  case Version::HTTP_1_0:
    return "HTTP/1.0";
  case Version::HTTP_1_1:
    return "HTTP/1.1";
  default:
    return "";
  }
}

Version StringToVersion(const std::string version_str) {
  if (kVersionMap.find(version_str) == kVersionMap.end()) {
    return Version::INVALID;
  }
  return kVersionMap[version_str];
}

}  // namespace http
}  // namespace net
}  // namespace nlink
