/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_
#define LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_

#include <cstdint>

#include "link/base/platform/discriptor.h"

namespace link {
namespace net {

const base::Discriptor kInvalidSocket = -1;

base::Discriptor CreatePlatformSocket(
  int32_t family, int32_t type, int32_t protocol);

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_SOCKET_DESCRIPTOR_H_
