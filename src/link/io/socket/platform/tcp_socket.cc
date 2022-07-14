/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_socket.h"

#include <utility>
#include <cstring>

#include "link/base/callback/bind.h"
#include "link/base/logging.h"
#include "link/io/base/io_error.h"

namespace nlink {
namespace io {

TcpSocket::TcpSocket(SocketOptions options)
  : options_(options) {
}

TcpSocket::~TcpSocket() {
}

int32_t TcpSocket::Open(AddressFamily address_family) {
  socket_.reset(new Socket());
  int32_t res = socket_->Open(address_family, Socket::Type::TCP);
  if (res != IOError::OK) {
    socket_.reset();
  }
  return IOError::OK;
}

int32_t TcpSocket::AdoptConnectedSocket(
  SocketDescriptor socket_fd, const IpEndPoint& peer_address) {
  SockaddrStorage storage;
  if (!peer_address.ToSockAddr(storage.addr, &storage.addr_len) ||
      peer_address.Empty()) {
    return IOError::ERR_ADDRESS_INVALID;
  }

  socket_.reset(new Socket());
  int32_t res = socket_->AdoptConnectedSocket(socket_fd, storage);
  if (res != IOError::OK) {
    socket_.reset();
  }
  return IOError::OK;
}

int32_t TcpSocket::AdoptUnconnectedSocket(SocketDescriptor socket_fd) {
  socket_.reset(new Socket());
  int32_t res = socket_->AdoptUnconnectedSocket(socket_fd);
  if (res != IOError::OK) {
    socket_.reset();
  }
  return IOError::OK;
}

int32_t TcpSocket::Bind(const IpEndPoint& address) {
  SockaddrStorage storage;
  if (!address.ToSockAddr(storage.addr, &storage.addr_len)) {
    return IOError::ERR_ADDRESS_INVALID;
  }
  return socket_->Bind(storage);
}

int32_t TcpSocket::Listen(int32_t connection) {
  return socket_->Listen(connection);
}

int32_t TcpSocket::Accept(
  std::unique_ptr<TcpSocket>* tcp_socket,
  IpEndPoint* address,
  base::CompletionCallback callback) {
  int32_t res = socket_->Accept(
    &accepted_socket_,
    base::Bind(&TcpSocket::AcceptCompleted, this,
      tcp_socket, address, std::move(callback)));
  if (res != IOError::ERR_IO_PENDING) {
    res = HandleAcceptCompleted(tcp_socket, address, res);
  }
  return res;
}

void TcpSocket::AcceptCompleted(
  std::unique_ptr<TcpSocket>* tcp_socket,
  IpEndPoint* address,
  base::CompletionCallback callback,
  int32_t res) {
  std::move(callback).Run(HandleAcceptCompleted(tcp_socket, address, res));
}

int32_t TcpSocket::HandleAcceptCompleted(
  std::unique_ptr<TcpSocket>* tcp_socket, IpEndPoint* address, int32_t res) {
  if (res == IOError::OK) {
    res = BuildNewTcpSocket(tcp_socket, address);
  }
  return res;
}

int32_t TcpSocket::Connect(
  const IpEndPoint& address, base::CompletionCallback callback) {
  SockaddrStorage storage;
  if (!address.ToSockAddr(storage.addr, &storage.addr_len)) {
    LOG(INFO) << "TcpSocket::Connect - fail convert addr to sockaddr";
    return IOError::ERR_ADDRESS_INVALID;
  }

  int32_t res = socket_->Connect(
    storage,
    base::Bind(&TcpSocket::ConnectCompleted, this, std::move(callback)));
  if (res != IOError::ERR_IO_PENDING) {
    res = HandleConnectCompleted(res);
  }
  return res;
}

void TcpSocket::ConnectCompleted(
  base::CompletionCallback callback, int32_t res) {
  std::move(callback).Run(HandleConnectCompleted(res));
}

int32_t TcpSocket::HandleConnectCompleted(int32_t res) {
  if (res == IOError::ERR_ADDRESS_UNREACHABLE) {
    res = IOError::ERR_INTERNET_DISCONNECTED;
  }
  return res;
}

int32_t TcpSocket::Close() {
  socket_->Close();
  socket_.reset();
}

int32_t TcpSocket::Read(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int32_t res = socket_->Read(
      buffer,
      base::Bind(&TcpSocket::ReadCompleted, this, buffer, std::move(callback)));
  if (res != IOError::ERR_IO_PENDING) {
    res = HandleReadCompleted(buffer, res);
  }
  return res;
}

int32_t TcpSocket::ReadIfReady(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int res = socket_->ReadIfReady(
      buffer,
      base::Bind(&TcpSocket::ReadIfReadyCompleted, this, std::move(callback)));
  if (res != IOError::ERR_IO_PENDING) {
    res = HandleReadCompleted(buffer, res);
  }
  return res;
}

int32_t TcpSocket::CancelReadIfReady() {
  return socket_->CancelReadIfReady();
}

void TcpSocket::ReadCompleted(
  base::Buffer* buffer, base::CompletionCallback callback, int32_t res) {
  std::move(callback).Run(HandleReadCompleted(buffer, res));
}

void TcpSocket::ReadIfReadyCompleted(
  base::CompletionCallback callback, int32_t res) {
  std::move(callback).Run(res);
}

int32_t TcpSocket::HandleReadCompleted(base::Buffer* buffer, int32_t res) {
  if (res < 0) {
    LOG(ERROR) << "[TcpSocket::HandleWriteCompleted] socket write fail."
               << std::strerror(NLINK_ERRNO);
    return res;
  }
  return res;
}

int32_t TcpSocket::Write(
  base::Buffer* buffer, base::CompletionCallback callback) {
  int32_t res = socket_->Write(
    buffer,
    base::Bind(&TcpSocket::WriteCompleted, this, buffer, std::move(callback)));
  if (res != IOError::ERR_IO_PENDING) {
    res = HandleWriteCompleted(buffer, res);
  }
  return res;
}

void TcpSocket::WriteCompleted(
  base::Buffer* buffer, base::CompletionCallback callback, int32_t res) {
  std::move(callback).Run(HandleWriteCompleted(buffer, res));
}

int32_t TcpSocket::HandleWriteCompleted(base::Buffer* buffer, int32_t res) {
  if (res < 0) {
    LOG(ERROR) << "[TcpSocket::HandleWriteCompleted] socket write fail."
               << std::strerror(NLINK_ERRNO);
  }
  return res;
}

int32_t TcpSocket::BuildNewTcpSocket(
  std::unique_ptr<TcpSocket>* tcp_socket, IpEndPoint* address) {
  SockaddrStorage storage;
  if (accepted_socket_->GetPeerAddress(&storage) != IOError::OK ||
      !address->FromSockAddr(storage.addr, storage.addr_len)) {
    accepted_socket_.reset();
    return IOError::ERR_ADDRESS_INVALID;
  }

  SocketOptions default_options;
  tcp_socket->reset(new TcpSocket(default_options));
  (*tcp_socket)->socket_ = std::move(accepted_socket_);
  return IOError::OK;
}

bool TcpSocket::IsConnected() const {
  return socket_->IsConnected();
}

SocketDescriptor TcpSocket::Descriptor() const {
  return socket_->Descriptor();
}

}  // namespace io
}  // namespace nlink
