
/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_BASE_BUFFER_H_
#define SRC_LINK_BASE_BUFFER_H_

#include <cstdint>
#include <vector>

namespace nlink {
namespace base {

class Buffer {
 public:
  Buffer();
  explicit Buffer(size_t length);
  Buffer(uint8_t* data, size_t length);
  Buffer(const Buffer& buffer);
  Buffer(Buffer&& buffer);

  ~Buffer();

  size_t Size() const;
  bool IsEmpty() const;
  void Clear();

  const std::vector<uint8_t>& Data() const;
  const uint8_t* RawData() const;

 private:
  std::vector<uint8_t> raw_buffer_;
};

}  // namespace base
}  // namespace nlink

#endif  // SRC_LINK_BASE_BUFFER_H_
