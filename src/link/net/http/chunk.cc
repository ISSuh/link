/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/chunk.h"

#include <sstream>

#include "link/net/http/constant.h"

namespace nlink {
namespace net {
namespace http {

Chunk::Chunk()
  : content_("") {
}

Chunk::Chunk(const std::string& content)
  : content_(content) {
}

const std::string Chunk::Content() const {
  return content_;
}

const std::string Chunk::Serialize() const {
  std::stringstream stream;
  stream << std::hex << content_.size() << std::dec;
  stream << kCRLF;
  stream << content_;
  stream << kCRLF;

  return stream.str();
}

}  // namespace http
}  // namespace net
}  // namespace nlink
