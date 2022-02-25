/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

namespace {
#include <sys/unistd.h>
}

#include <utility>

#include "link/net/socket/posix/posix_socket.h"
#include "link/net/base/net_error.h"
#include "link/base/logging.h"

namespace link {
namespace net {

bool SetNonBlocking(int fd) {
  const int flags = fcntl(fd, F_GETFL);
  if (flags == -1)
    return false;
  if (flags & O_NONBLOCK)
    return true;
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    return false;
  return true;
}

PosixSocket::PosixSocket()
  : socket_fd_(kInvalidSocket) {
}
PosixSocket::~PosixSocket() {
}

int32_t PosixSocket::Open(AddressFamily address_family) {
  socket_fd_ = CreatePlatformSocket(
    address_family, SOCK_STREAM,
    address_family == AF_UNIX ? 0 : IPPROTO_TCP);

  if (socket_fd_ < 0) {
    LOG(ERROR) << " - socket create failed";
    return errno;
  }

  if (!SetNonBlocking(socket_fd_)) {
    LOG(ERROR) << __func__ << " - set non blocking fail";
    int32_t res = errno;
    Close();
    return res;
  }
  return OK;
}

int32_t PosixSocket::AdoptConnectedSocket(
  SocketDescriptor socket, const SockaddrStorage& address) {
  int32_t res = AdoptUnconnectedSocket(socket);
  if (res != OK)
    return res;

  SetPeerAddress(address);
  return OK;
}

int32_t PosixSocket::AdoptUnconnectedSocket(SocketDescriptor socket) {
  socket_fd_ = socket;

  if (!SetNonBlocking(socket_fd_)) {
    int rv = errno;
    Close();
    return rv;
  }
  return OK;
}

int32_t PosixSocket::Bind(const SockaddrStorage& address) {
  int res = bind(socket_fd_, address.addr, address.addr_len);
  if (res < 0) {
    LOG(ERROR) << __func__ << " - failed";
    return errno;
  }
  return OK;
}

int32_t PosixSocket::Listen(int32_t connection) {
  int res = listen(socket_fd_, connection);
  if (res < 0) {
    LOG(ERROR) << __func__ << " - failed";
    return errno;
  }
  return OK;
}

int32_t PosixSocket::Accept(
  std::shared_ptr<PosixSocket> socket,
  base::CompletionCallback callback) {
  int32_t res = DoAccept(socket);
  if (res != ERR_IO_PENDING) {
    return res;
  }

  accept_callback_ = std::move(callback);
  accept_socket_ = socket;
  return OK;
}

int32_t PosixSocket::DoAccept(std::shared_ptr<PosixSocket> socket) {
  SockaddrStorage new_peer_address;
  int32_t new_socket = accept(
    socket_fd_, new_peer_address.addr, &new_peer_address.addr_len);

  if (new_socket < 0) {
    return errno;
  }

  std::shared_ptr<PosixSocket> accepted_socket(new PosixSocket());
  int res = accepted_socket->AdoptConnectedSocket(new_socket, new_peer_address);
  if (res != OK) {
    return res;
  }

  socket = std::move(accepted_socket);
  return OK;
}

int32_t PosixSocket::Connect(
  const SockaddrStorage& address,
  base::CompletionCallback callback) {
  SetPeerAddress(address);

  int32_t res = DoConnect();
  if (res != ERR_IO_PENDING) {
    return res;
  }

  int32_t os_error = 0;
  socklen_t len = sizeof(os_error);
  if (getsockopt(socket_fd_, SOL_SOCKET, SO_ERROR, &os_error, &len) == 0) {
    // TCPSocketPosix expects errno to be set.
    errno = os_error;
  }

  if (errno != OK && errno != ERR_IO_PENDING) {
    return errno;
  }

  write_callback_ = std::move(callback);
  waiting_connect_ = true;
  return ERR_IO_PENDING;
}

int32_t PosixSocket::DoConnect() {
  int32_t res = connect(
    socket_fd_, peer_address_->addr, peer_address_->addr_len);
  return res;
}

int32_t PosixSocket::Close() {
  if (socket_fd_ != kInvalidSocket) {
    if (::close(socket_fd_) < 0) {
      LOG(ERROR) << __func__ << " - failed";
    }
    socket_fd_ = kInvalidSocket;
  }

  waiting_connect_ = false;
  peer_address_.reset();
  return OK;
}

int32_t PosixSocket::Read(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  int res = ReadIfReady(
      buf, len,
      base::Bind(&PosixSocket::RetryRead, this));
  if (res == ERR_IO_PENDING) {
    read_buf_.reset(buf);
    read_buf_len_ = len;
    read_callback_ = std::move(callback);
  }
  return res;
}

int32_t PosixSocket::ReadIfReady(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  int32_t res = DoRead(buf, len);
  if (res != ERR_IO_PENDING) {
    return res;
  }

  read_if_ready_callback_ = std::move(callback);
  return ERR_IO_PENDING;
}

int32_t PosixSocket::DoRead(Buffer* buf, int32_t len) {
  int32_t res = ::read(socket_fd_, buf->data(), len);
  return res;
}

void PosixSocket::RetryRead(int32_t res) {
  if (res == OK) {
    res = ReadIfReady(
        read_buf_.get(), read_buf_len_,
        base::Bind(&PosixSocket::RetryRead, this));
    if (res == ERR_IO_PENDING) {
      return;
    }
  }
  read_buf_ = nullptr;
  read_buf_len_ = 0;
  read_callback_.Run(res);
}

int32_t PosixSocket::Write(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  int res = DoWrite(buf, len);
  if (res == ERR_IO_PENDING) {
    res = WaitForWrite(buf, len, std::move(callback));
  }
  return res;
}

int32_t PosixSocket::DoWrite(Buffer* buf, int32_t len) {
  int32_t res = ::send(socket_fd_, buf->data(), len, MSG_NOSIGNAL);
  return res;
}

int32_t PosixSocket::WaitForWrite(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  write_buf_.reset(buf);
  write_buf_len_ = len;
  write_callback_ = std::move(callback);
  return ERR_IO_PENDING;
}

void PosixSocket::SetPeerAddress(const SockaddrStorage& peer_address) {
  peer_address_.reset(new SockaddrStorage(peer_address));
}

bool PosixSocket::IsConnected() const {
  if (socket_fd_ == kInvalidSocket || waiting_connect_) {
      return false;
  }

  char c;
  int32_t res = recv(socket_fd_, &c, 1, MSG_PEEK);
  if (res == 0) {
    return false;
  }

  if (res == -1 &&
      errno != EAGAIN &&
      errno != EWOULDBLOCK) {
    return false;
  }
  return true;
}

}  // namespace net
}  // namespace link
