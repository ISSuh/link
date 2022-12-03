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
#else
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#endif

namespace nlink {
namespace io {

const SocketDescriptor kInvalidSocket = -1;

SocketDescriptor CreatePlatformSocket(int32_t family, int32_t type) {
  SocketDescriptor socket_fd = kInvalidSocket;
#if defined(__linux__)
  int32_t protocol = family == AF_UNIX ? 0 : IPPROTO_TCP;
  socket_fd = socket(family, type, protocol);
#else
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
#endif
  return socket_fd;
}

}  // namespace io
}  // namespace nlink
