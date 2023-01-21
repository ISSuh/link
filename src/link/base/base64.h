/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_BASE64_H_
#define LINK_BASE_BASE64_H_

#include <string>

namespace nlink {
namespace base {
namespace base64 {

std::string Encode(const std::string& source, bool is_url = false);

std::string Decode(
    const std::string& source, bool remove_linebreaks = false);

}  // namespace base64
}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_BASE64_H_
