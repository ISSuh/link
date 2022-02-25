/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_
#define LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_

#include <cstdint>

namespace link {
namespace net {

using SocketDescriptor = int32_t;
const SocketDescriptor kInvalidSocket = -1;

SocketDescriptor CreatePlatformSocket(
  int32_t family, int32_t type, int32_t protocol);

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_
