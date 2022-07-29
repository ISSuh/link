/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/socket_options.h"

#include "link/io/base/io_error.h"

#if defined(__WIN32__) || defined(__WIN64__)
#elif defined(__linux__)
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif

namespace nlink {
namespace io {

int32_t SetTCPNoDelay(SocketDescriptor fd, bool no_delay) {
#if defined(__WIN32__) || defined(__WIN64__)
  BOOL on = no_delay ? TRUE : FALSE;
#elif defined(__linux__)
  int32_t on = no_delay ? 1 : 0;
#endif
  int32_t res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
                      reinterpret_cast<const char*>(&on), sizeof(on));
  return res == -1 ? SystemErrorToNetError(NLINK_ERRNO) : IOError::OK;
}

int32_t SetReuseAddr(SocketDescriptor fd, bool reuse) {
#if defined(__WIN32__) || defined(__WIN64__)
  BOOL boolean_value = reuse ? TRUE : FALSE;
#elif defined(__linux__)
  int32_t boolean_value = reuse ? 1 : 0;
#endif
  int32_t res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                      reinterpret_cast<const char*>(&boolean_value),
                      sizeof(boolean_value));
  return res == -1 ? SystemErrorToNetError(NLINK_ERRNO) : IOError::OK;
}

int32_t SetReusePort(SocketDescriptor fd, bool reuse) {
#if defined(__WIN32__) || defined(__WIN64__)
  BOOL boolean_value = reuse ? TRUE : FALSE;
#elif defined(__linux__)
  int32_t boolean_value = reuse ? 1 : 0;
#endif
  int32_t res = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                      reinterpret_cast<const char*>(&boolean_value),
                      sizeof(boolean_value));
  return res == -1 ? SystemErrorToNetError(NLINK_ERRNO) : IOError::OK;
}

int32_t SetKeepAlive(SocketDescriptor fd, bool enable, int32_t delay) {
  int32_t on = enable ? 1 : 0;
  if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on))) {
    return SystemErrorToNetError(NLINK_ERRNO);
  }

  if (!enable) {
    return IOError::OK;
  }

#if defined(__linux__)
  if (setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &delay, sizeof(delay))) {
    return SystemErrorToNetError(NLINK_ERRNO);
  }

  if (setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &delay, sizeof(delay))) {
    return SystemErrorToNetError(NLINK_ERRNO);
  }
#endif

  return IOError::OK;
}

int32_t SetNonBlocking(SocketDescriptor fd) {
  uint32_t flag = 1;
  int32_t res = ioctl(fd, FIONBIO, &flag);
  return res == -1 ? SystemErrorToNetError(NLINK_ERRNO) : IOError::OK;
}

}  // namespace io
}  // namespace nlink
