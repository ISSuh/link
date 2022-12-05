/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_CHUNK_H_
#define LINK_NET_HTTP_CHUNK_H_

#include <string>

namespace nlink {
namespace net {
namespace http {

class Chunk {
 public:
  Chunk();
  explicit Chunk(const std::string& content);

  const std::string Content() const;
  const std::string Serialize() const;

 private:
  std::string content_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_CHUNK_H_
