/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/posix/tcp_posix_socket.h"

#include <memory>
#include <utility>

#include "link/base/callback/bind.h"
#include "link/net/base/address_family.h"
#include "link/net/base/sockaddr_storage.h"
#include "link/net/base/net_error.h"
#include "link/base/logging.h"

namespace link {
namespace net {

TcpPosixSocket::TcpPosixSocket() {
}

TcpPosixSocket::~TcpPosixSocket() {
}

int32_t TcpPosixSocket::Open(AddressFamily address_family) {
  socket_.reset(new PosixSocket());
  int32_t res = socket_->Open(address_family);
  if (res != OK) {
    socket_.reset();
  }
  return res;
}

int32_t TcpPosixSocket::AdoptConnectedSocket(
  base::Discriptor socket, const IpEndPoint& peer_address) {
  SockaddrStorage storage;
  if (!peer_address.ToSockAddr(storage.addr, &storage.addr_len) ||
      peer_address.empty()) {
    return ERR_ADDRESS_INVALID;
  }

  socket_.reset(new PosixSocket());
  int32_t res = socket_->AdoptConnectedSocket(socket, storage);
  if (res != OK) {
    socket_.reset();
  }
  return res;
}

int32_t TcpPosixSocket::AdoptUnconnectedSocket(base::Discriptor socket) {
  socket_.reset(new PosixSocket());
  int32_t res = socket_->AdoptUnconnectedSocket(socket);
  if (res != OK) {
    socket_.reset();
  }
  return res;
}

int32_t TcpPosixSocket::Bind(const IpEndPoint& address) {
  SockaddrStorage storage;
  if (!address.ToSockAddr(storage.addr, &storage.addr_len)) {
    return ERR_ADDRESS_INVALID;
  }
  return socket_->Bind(storage);
}

int32_t TcpPosixSocket::Listen(int32_t connection) {
  return socket_->Listen(connection);
}

int32_t TcpPosixSocket::Accept(
  std::unique_ptr<TcpPosixSocket>* tcp_socket,
  IpEndPoint* address,
  base::CompletionCallback callback) {
  int32_t res = socket_->Accept(
      &accept_socket_,
      base::Bind(&TcpPosixSocket::AcceptCompleted, this,
                     tcp_socket, address, std::move(callback)));
  if (res != ERR_IO_PENDING) {
    res = HandleAcceptCompleted(tcp_socket, address, res);
  }
  return res;
}

int32_t TcpPosixSocket::Connect(
  const IpEndPoint& address,
  base::CompletionCallback callback) {
  SockaddrStorage storage;
  if (!address.ToSockAddr(storage.addr, &storage.addr_len))
    return ERR_ADDRESS_INVALID;

  int32_t res = socket_->Connect(
      storage, base::Bind(&TcpPosixSocket::ConnectCompleted,
                              this, std::move(callback)));
  if (res != ERR_IO_PENDING) {
    res = HandleConnectCompleted(res);
  }
  return res;
}

int32_t TcpPosixSocket::Close() {
  socket_.reset();
}

int32_t TcpPosixSocket::Read(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  int32_t res = socket_->Read(
      buf, len,
      base::Bind(
          &TcpPosixSocket::ReadCompleted,
            this, buf, std::move(callback)));
  if (res != ERR_IO_PENDING) {
    res = HandleReadCompleted(buf, res);
  }
  return res;
}

int32_t TcpPosixSocket::ReadIfReady(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  int res = socket_->ReadIfReady(
      buf, len,
      base::Bind(&TcpPosixSocket::ReadIfReadyCompleted,
                     this, std::move(callback)));
  if (res != ERR_IO_PENDING) {
    res = HandleReadCompleted(buf, res);
  }
  return res;
}

int32_t TcpPosixSocket::CancelReadIfReady() {
  return socket_->CancelReadIfReady();
}

int32_t TcpPosixSocket::Write(
  Buffer* buf, int32_t len,
  base::CompletionCallback callback) {
  base::CompletionCallback write_callback = base::Bind(
      &TcpPosixSocket::WriteCompleted,
      this, buf, std::move(callback));

  int32_t res = socket_->Write(buf, len, std::move(write_callback));
  if (res != ERR_IO_PENDING) {
    res = HandleWriteCompleted(buf, res);
  }
  return res;
}

bool TcpPosixSocket::IsConnected() const {
  if (!socket_) {
    return false;
  }
  return socket_->IsConnected();
}

int32_t TcpPosixSocket::GetPeerAddress(IpEndPoint* address) const {
  if (!IsConnected()) {
    return ERR_SOCKET_NOT_CONNECTED;
  }

  SockaddrStorage storage;
  int32_t res = socket_->GetPeerAddress(&storage);
  if (res != OK) {
    return res;
  }

  if (!address->FromSockAddr(storage.addr, storage.addr_len)) {
    return ERR_ADDRESS_INVALID;
  }
  return OK;
}

int TcpPosixSocket::AllowAddressReuse() {
  int32_t reuse = true;
  int32_t res = setsockopt(socket_->socket_fd(), SOL_SOCKET, SO_REUSEADDR,
    reinterpret_cast<const char*>(&reuse), sizeof(reuse));
  return res == -1 ? SystemErrorToNetError(errno) : OK;
}

base::Discriptor TcpPosixSocket::socket_fd() const {
  return socket_->socket_fd();
}

bool TcpPosixSocket::IsValid() const {
  return socket_ != nullptr && socket_->socket_fd() != kInvalidSocket;
}

void TcpPosixSocket::AcceptCompleted(
  std::unique_ptr<TcpPosixSocket>* tcp_socket,
  IpEndPoint* address,
  base::CompletionCallback callback,
  int32_t res) {
  std::move(callback).Run(HandleAcceptCompleted(tcp_socket, address, res));
}

int32_t TcpPosixSocket::HandleAcceptCompleted(
  std::unique_ptr<TcpPosixSocket>* tcp_socket,
  IpEndPoint* address,
  int32_t res) {
  if (res == OK) {
    res = BuildTcpSocketPosix(tcp_socket, address);
  }
  return res;
}

int32_t TcpPosixSocket::BuildTcpSocketPosix(
  std::unique_ptr<TcpPosixSocket>* tcp_socket,
  IpEndPoint* address) {
  SockaddrStorage storage;
  if (accept_socket_->GetPeerAddress(&storage) != OK ||
      !address->FromSockAddr(storage.addr, storage.addr_len)) {
    accept_socket_.reset();
    return ERR_ADDRESS_INVALID;
  }

  tcp_socket->reset(new TcpPosixSocket());
  (*tcp_socket)->socket_ = std::move(accept_socket_);
  return OK;
}

void TcpPosixSocket::ConnectCompleted(
  base::CompletionCallback callback, int32_t res) {
  std::move(callback).Run(HandleConnectCompleted(res));
}

int32_t TcpPosixSocket::HandleConnectCompleted(int32_t res) {
  if (res == ERR_ADDRESS_UNREACHABLE) {
    res = ERR_INTERNET_DISCONNECTED;
  }
  return res;
}

void TcpPosixSocket::ReadCompleted(
  Buffer* buf,
  base::CompletionCallback callback,
  int32_t res) {
  std::move(callback).Run(HandleReadCompleted(buf, res));
}

void TcpPosixSocket::ReadIfReadyCompleted(
  base::CompletionCallback callback, int32_t res) {
  HandleReadCompletedHelper(res);
  std::move(callback).Run(res);
}

int32_t TcpPosixSocket::HandleReadCompleted(Buffer* buf, int32_t res) {
  HandleReadCompletedHelper(res);
  if (res < 0) {
    return res;
  }
  return res;
}

void TcpPosixSocket::HandleReadCompletedHelper(int32_t res) {
  if (res < 0) {
    LOG(ERROR) << __func__ << " - SOCKET_READ_ERROR " << errno;
  }
}

void TcpPosixSocket::WriteCompleted(
  Buffer* buf,
  base::CompletionCallback callback,
  int32_t res) {
  std::move(callback).Run(HandleWriteCompleted(buf, res));
}

int32_t TcpPosixSocket::HandleWriteCompleted(Buffer* buf, int32_t res) {
  if (res < 0) {
    LOG(ERROR) << __func__ << " - SOCKET_WRITE_ERROR " << errno;
    return res;
  }
  return res;
}

}  // namespace net
}  // namespace link
