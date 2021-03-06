/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_WRITER_H_
#define LINK_NET_SOCKET_WRITER_H_

#include "link/base/buffer.h"

namespace nlink {
namespace io {

class Writer {
 public:
  virtual void Write(const base::Buffer& buffer) = 0;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_WRITER_H_
