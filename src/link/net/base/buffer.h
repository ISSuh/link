/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_BUFFER_H_
#define LINK_NET_BASE_BUFFER_H_

#include <vector>

namespace nlink {
namespace net {

class Buffer {
 public:
  Buffer() {}
  virtual ~Buffer() {}

  uint8_t* data() { return &data_[0]; }

 private:
  std::vector<uint8_t> data_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_BASE_BUFFER_H_
