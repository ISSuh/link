/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/base64.h"

#include "link/third_party/cpp-base64/base64.h"

namespace nlink {
namespace base {
namespace base64 {

std::string Encode(const std::string& source, bool is_url) {
  if (source.empty()) {
    return "";
  }
  return base64_encode(source, is_url);
}

std::string Decode(const std::string& source, bool remove_linebreaks) {
  if (source.empty()) {
    return "";
  }
  return base64_decode(source, remove_linebreaks);
}

}  // namespace base64
}  // namespace base
}  // namespace nlink
