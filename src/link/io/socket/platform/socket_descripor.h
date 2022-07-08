/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_SOCKET_DESCRIPOR_H_
#define LINK_IO_SOCKET_PLATFORM_SOCKET_DESCRIPOR_H_

#include <cstdint>
#include <cerrno>

namespace nlink {
namespace io {

#if defined(__linux__)
typedef int32_t SocketDescriptor;
#define NLINK_ERRNO errno
#endif

extern const SocketDescriptor kInvalidSocket;

SocketDescriptor CreatePlatformSocket(int32_t family, int32_t type);

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_DESCRIPOR_H_
