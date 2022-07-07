/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/socket_descripor.h"

#if defined(__linux__)
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

namespace nlink {
namespace io {

const SocketDiscriptor kInvalidSocket = -1;

SocketDiscriptor CreatePlatformSocket(int32_t family, int32_t type) {
  int32_t protocol = family == AF_UNIX ? 0 : IPPROTO_TCP;
  SocketDiscriptor socket_fd = socket(family, type, protocol);
  return socket_fd;
}

}  // namespace io
}  // namespace nlink
