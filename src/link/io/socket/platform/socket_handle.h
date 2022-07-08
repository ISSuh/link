/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_SOCKET_HANDLE_H_
#define LINK_IO_SOCKET_PLATFORM_SOCKET_HANDLE_H_

#include <cstdint>

#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

class SocketHandle {
 public:
  explicit SocketHandle(SocketDescriptor fd);
  ~SocketHandle();

  const SocketDescriptor Descriptor() const;

  bool SetNoDelay() const;
  bool SetBlocking() const;
  bool SetNonBlocking() const;

 private:
  void Close();

  SocketDescriptor socket_descriptor_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_HANDLE_H_
