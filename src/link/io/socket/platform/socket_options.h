/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_SOCKET_OPTIONS_H_
#define LINK_IO_SOCKET_PLATFORM_SOCKET_OPTIONS_H_

#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

const int32_t kDefaultTcpKeepAliveSeconds = 45;

struct SocketOptions {
  SocketOptions()
    : non_bloking(true),
      no_delay(true),
      reuse_socket(true),
      keep_alive(true),
      keep_alive_seconds(kDefaultTcpKeepAliveSeconds) {}

  bool non_bloking;
  bool no_delay;
  bool reuse_socket;
  bool keep_alive;
  int32_t keep_alive_seconds;
};

int32_t SetNoDelay(SocketDescriptor fd, bool no_delay);
int32_t SetReuseAddr(SocketDescriptor fd, bool reuse);
int32_t SetKeepAlive(SocketDescriptor fd, bool enable, int32_t delay);

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_OPTIONS_H_
