
/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/buffer.h"

namespace nlink {
namespace base {

Buffer::Buffer()
  : Buffer(0) {
}

Buffer::Buffer(size_t length)
  : raw_buffer_(length) {
}

Buffer::Buffer(uint8_t* data, size_t length)
  : raw_buffer_(data, data + length) {
}

Buffer::Buffer(const Buffer& buffer)
  : raw_buffer_(buffer.raw_buffer_) {
}

Buffer::Buffer(Buffer&& buffer)
  : raw_buffer_(buffer.raw_buffer_) {
}

Buffer::~Buffer() {
}

size_t Buffer::Size() const {
  return raw_buffer_.size();
}

bool Buffer::IsEmpty() const {
  return raw_buffer_.empty();
}

void Buffer::Clear() {
  raw_buffer_.clear();
}

const std::vector<uint8_t>& Buffer::Data() const {
  return raw_buffer_;
}

const uint8_t* Buffer::RawData() const {
  return raw_buffer_.data();
}


}  // namespace base
}  // namespace nlink
