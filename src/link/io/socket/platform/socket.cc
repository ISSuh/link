/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/socket.h"

#include <sys/unistd.h>

#include <cstring>
#include <utility>

#include "link/base/logging.h"
#include "link/io/base/io_error.h"
#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

int32_t MapConnectError(int32_t os_error) {
  switch (os_error) {
    case EINPROGRESS:
      return ERR_IO_PENDING;
    case EACCES:
      return ERR_NETWORK_ACCESS_DENIED;
    case ETIMEDOUT:
      return ERR_CONNECTION_TIMED_OUT;
    default: {
      int32_t net_error = SystemErrorToNetError(os_error);
      if (net_error == ERR_FAILED) {
        return ERR_CONNECTION_FAILED;
      }
      return net_error;
    }
  }
}

Socket::Socket()
  : handle_(nullptr) {
}

Socket::~Socket() {
}

int32_t Socket::Open(AddressFamily address_family, Socket::Type type) {
  SocketDescriptor socket_fd = CreatePlatformSocket(
    address_family, static_cast<int32_t>(type));

  if (socket_fd <= kInvalidSocket) {
    LOG(ERROR) << "[Socket::Open] can not create socket. "
               << std::strerror(NLINK_ERRNO);
    return NLINK_ERRNO;
  }

  handle_.reset(new SocketHandle(socket_fd));
  if (!handle_->SetNonBlocking()) {
    LOG(ERROR) << "[Socket::Open] can not setting to nonblocking. "
               << std::strerror(NLINK_ERRNO);

    int32_t err = NLINK_ERRNO;
    Close();
    return err;
  }
  return IOError::OK;
}

int32_t Socket::AdoptConnectedSocket(
  SocketDescriptor socket_fd, const SockaddrStorage& peer_address) {
  int32_t res = AdoptUnconnectedSocket(socket_fd);
  if (res != OK) {
    return res;
  }

  SetPeerAddress(peer_address);
  return IOError::OK;
}

int32_t Socket::AdoptUnconnectedSocket(SocketDescriptor socket_fd) {
  handle_.reset(new SocketHandle(socket_fd));

  if (!handle_->SetBlocking()) {
    LOG(ERROR) << "[Socket::AdoptUnconnectedSocket] fail setting nonblocking. "
               << std::strerror(NLINK_ERRNO);

    int32_t err = NLINK_ERRNO;
    Close();
    return err;
  }
  return IOError::OK;
}

int32_t Socket::Bind(const SockaddrStorage& address) {
  int32_t res = bind(handle_->Descriptor(), address.addr, address.addr_len);
  if (res < 0) {
    LOG(ERROR) << "[Socket::Bind] can not bind. " << std::strerror(NLINK_ERRNO);
    return NLINK_ERRNO;
  }
  return IOError::OK;
}

int32_t Socket::Listen(int32_t connection) {
  int res = listen(handle_->Descriptor(), connection);
  if (res < 0) {
    LOG(ERROR) << "[Socket::Listen] can not listen. "
               << std::strerror(NLINK_ERRNO);
    return NLINK_ERRNO;
  }
  return IOError::OK;
}

int32_t Socket::Accept(
  std::unique_ptr<Socket>* socket, base::CompletionCallback callback) {
  int32_t res = DoAccept(socket);
  if (res != IOError::ERR_IO_PENDING) {
    return res;
  }

  accept_callback_ = std::move(callback);
  accepted_socket_ = socket;
  return IOError::OK;
}

int32_t Socket::DoAccept(std::unique_ptr<Socket>* socket) {
  SockaddrStorage new_peer_address;
  SocketDescriptor new_socket = accept(
    handle_->Descriptor(), new_peer_address.addr, &new_peer_address.addr_len);

  if (new_socket <= kInvalidSocket) {
    LOG(ERROR) << "[Socket::DoAccept] can not create peer socket. "
               << std::strerror(NLINK_ERRNO);
    return NLINK_ERRNO;
  }

  std::unique_ptr<Socket> accepted_socket(new Socket());
  int res = accepted_socket->AdoptConnectedSocket(new_socket, new_peer_address);
  if (res != IOError::OK) {
    return res;
  }

  *socket = std::move(accepted_socket);
  return IOError::OK;
}

void Socket::AcceptCompleted() {
  int32_t res = DoAccept(accepted_socket_);
  if (res == IOError::ERR_IO_PENDING) {
    return;
  }

  accepted_socket_ = nullptr;
  std::move(accept_callback_).Run(res);
}

int32_t Socket::Connect(
  const SockaddrStorage& address, base::CompletionCallback callback) {
  SetPeerAddress(address);

  int32_t res = DoConnect();
  if (res != IOError::ERR_IO_PENDING) {
    if (res == IOError::OK) {
      ConnectCompleted();
    }
    return res;
  }

  int32_t os_error = 0;
  socklen_t len = sizeof(os_error);

  res =
    getsockopt(handle_->Descriptor(), SOL_SOCKET, SO_ERROR, &os_error, &len);
  if (0 == res) {
    // TCPSocketPosix expects errno to be set.
    // TODO(issuh): implement set errno macro for other platform
    errno = os_error;
  }

  res = MapConnectError(NLINK_ERRNO);
  if (res != IOError::OK &&
      res != IOError::ERR_IO_PENDING) {
    return res;
  }

  connection_callback_ = std::move(callback);
  waiting_connect_ = true;
  return IOError::ERR_IO_PENDING;
}

int32_t Socket::DoConnect() {
  int32_t res = connect(
    handle_->Descriptor(), peer_address_->addr, peer_address_->addr_len);
  return res;
}

void Socket::ConnectCompleted() {
  int32_t os_error = 0;
  socklen_t len = sizeof(os_error);
  int32_t res =
    getsockopt(handle_->Descriptor(), SOL_SOCKET, SO_ERROR, &os_error, &len);
  if (0 == res) {
    // TCPSocketPosix expects errno to be set.
    // TODO(issuh): implement set errno macro for other platform
    errno = os_error;
  }

  res = MapConnectError(NLINK_ERRNO);
  if (res == IOError::ERR_IO_PENDING) {
    return;
  }

  waiting_connect_ = false;
  std::move(connection_callback_).Run(res);
}

int32_t Socket::Close() {
  if (nullptr != handle_) {
    handle_.reset();
  }
  return IOError::OK;
}

int32_t Socket::Read(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int res = ReadIfReady(
      buffer,
      base::Bind(&Socket::RetryRead, this));
  if (res == IOError::ERR_IO_PENDING) {
    pending_read_buffer_.reset(buffer);
    peding_read_callback_ = std::move(callback);
  }
  return res;
}

int32_t Socket::ReadIfReady(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int32_t res = DoRead(buffer);
  if (res != IOError::ERR_IO_PENDING) {
    return res;
  }

  read_if_ready_callback_ = std::move(callback);
  return IOError::ERR_IO_PENDING;
}

int32_t Socket::CancelReadIfReady() {
  read_if_ready_callback_.Reset();
  return IOError::OK;
}

int32_t Socket::DoRead(base::Buffer* buffer) {
  int32_t size =
    read(handle_->Descriptor(), &buffer[0], buffer->Size());
  return size;
}

void Socket::RetryRead(int32_t res) {
  if (res == IOError::OK) {
    res = ReadIfReady(
        pending_read_buffer_.get(),
        base::Bind(&Socket::RetryRead, this));
    if (res == IOError::ERR_IO_PENDING) {
      return;
    }
  }

  pending_read_buffer_ = nullptr;
  peding_read_callback_.Run(res);
  peding_read_callback_.Reset();
}

void Socket::ReadCompleted() {
  std::move(read_if_ready_callback_).Run(IOError::OK);
}

int32_t Socket::Write(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int32_t res = DoWrite(buffer);
  if (res == IOError::ERR_IO_PENDING) {
    WaitForWrite(buffer, std::move(callback));
  }
  return res;
}

int32_t Socket::WaitForWrite(
  base::Buffer* buffer, base::CompletionCallback callback) {
  pending_write_buffer_.reset(buffer);
  peding_write_callback_ = std::move(callback);
  return IOError::ERR_IO_PENDING;
}

int32_t Socket::DoWrite(base::Buffer* buffer) {
  int32_t size = send(
      handle_->Descriptor(), buffer->RawData(), buffer->Size(), MSG_NOSIGNAL);
  return size;
}

// TODO(issuh): should implement pending write process
void Socket::WriteCompleted() {
  int32_t res = DoWrite(pending_write_buffer_.get());
  if (res == IOError::ERR_IO_PENDING) {
    return;
  }

  pending_write_buffer_.reset();
  std::move(peding_write_callback_).Run(IOError::OK);
}

bool Socket::IsConnected() const {
  if (nullptr == handle_ ||
      handle_->Descriptor() == kInvalidSocket ||
      waiting_connect_) {
      return false;
  }

  char c;
  int32_t res = recv(handle_->Descriptor(), &c, 1, MSG_PEEK);
  if (res == 0) {
    return false;
  }

  // TODO(issuh): implement set errno macro for other platform
  if (res == -1 &&
      NLINK_ERRNO != EAGAIN &&
      NLINK_ERRNO != EWOULDBLOCK) {
    return false;
  }
  return true;
}

void Socket::SetPeerAddress(const SockaddrStorage& address) {
  if (nullptr == peer_address_) {
    peer_address_.reset(new SockaddrStorage(address));
  }
}

int32_t Socket::GetPeerAddress(SockaddrStorage* address) const {
  if (peer_address_ != nullptr) {
    return IOError::ERR_SOCKET_NOT_CONNECTED;
  }

  *address = *peer_address_;
  return IOError::OK;
}

SocketDescriptor Socket::Descriptor() const {
  return handle_->Descriptor();
}


}  // namespace io
}  // namespace nlink
