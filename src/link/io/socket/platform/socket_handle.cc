/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/socket_handle.h"

#if defined(__linux__)
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

namespace nlink {
namespace io {

SocketHandle::SocketHandle(SocketDescriptor fd)
  : socket_descriptor_(fd) {
}

SocketHandle::~SocketHandle() {
  Close();
}

const SocketDescriptor SocketHandle::Descriptor() const {
  return socket_descriptor_;
}

bool SocketHandle::SetNoDelay() const {
  bool res = false;
  const int flag = 1;

#ifdef defined(__linux__)
  int32_t error = setsockopt(
    socket_descriptor_, IPPROTO_TCP, TCP_NODELAY,
    reinterpret_cast<const char*>(&flag), sizeof(flag));
  res = error != NLINK_ERRNO;
#endif

  return res;
}

bool SocketHandle::SetBlocking() const {
  bool res = false;
  uint32_t flag = 0;

#ifdef defined(__linux__)
  int32_t error = ioctl(socket_descriptor_, FIONBIO, &flag);
  res = error != NLINK_ERRNO;
#endif

  return res;
}

bool SocketHandle::SetNonBlocking() const {
  bool res = false;
  uint32_t flag = 1;

#ifdef defined(__linux__)
  int32_t error = ioctl(socket_descriptor_, FIONBIO, &flag);
  res = error != NLINK_ERRNO;
#endif

  return res;
}

void SocketHandle::Close() {
#ifdef defined(__linux__)
  ::close(socket_descriptor_);
#endif
}

}  // namespace io
}  // namespace nlink
