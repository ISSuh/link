/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/socket_descriptor.h"

#include <sys/socket.h>
#include <sys/types.h>

namespace link {
namespace net {

SocketDescriptor CreatePlatformSocket(
  int32_t family, int32_t type, int32_t protocol) {
  SocketDescriptor socket_fd = socket(family, type, protocol);
  return socket_fd;
}

}  // namespace net
}  // namespace link
