/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/socket.h"

#include <cstring>

#include "link/base/logging.h"
#include "link/io/base/io_error.h"
#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

Socket::Socket()
  : handle_(nullptr) {
}

Socket::~Socket() {
}

int32_t Socket::Open(AddressFamily address_family, SocketType type) {
  SocketDiscriptor socket_fd = CreatePlatformSocket(
    address_family, static_cast<int32_t>(type));

  if (socket_fd == kInvalidSocket) {
    LOG(ERROR) << "[Socket::Open] can not create socket. "
               << std::strerror(NLINK_ERRNO);
    return NLINK_ERRNO;
  }

  handle_.reset(new SocketHandle(socket_fd));
  if (!handle_->SetBlocking()) {
    LOG(ERROR) << "[Socket::Open] can not setting to nonblocking. "
               << std::strerror(NLINK_ERRNO);

    Close();
    handle_.reset();
    int32_t err = NLINK_ERRNO;
    return err;
  }
  return IOError::OK;
}

int32_t Socket::Bind() {
}

int32_t Socket::Listen() {
}

int32_t Socket::Accept() {
}

int32_t Socket::Connect() {
}

int32_t Socket::Close() {
  if (nullptr != handle_) {
    handle_.reset();
  }
  return IOError::OK;
}

int32_t Socket::Read() {
}

int32_t Socket::Write() {
}

bool Socket::IsConnected() {
}

}  // namespace io
}  // namespace nlink
