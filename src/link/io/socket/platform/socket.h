/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_SOCKET_H_
#define LINK_IO_SOCKET_PLATFORM_SOCKET_H_

#include <vector>
#include <memory>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/io/base/address_family.h"
#include "link/io/base/sockaddr_storage.h"
#include "link/io/socket/platform/socket_handle.h"

namespace nlink {
namespace io {

class Socket {
 public:
  enum class Type : int32_t {
    TCP = 1,
    UDP = 2
  };

  Socket();
  virtual ~Socket();

  int32_t Open(AddressFamily address_family, Socket::Type type);
  int32_t AdoptConnectedSocket(
    SocketDescriptor socket_fd, const SockaddrStorage& peer_address);
  int32_t AdoptUnconnectedSocket(SocketDescriptor socket_fd);

  int32_t Bind(const SockaddrStorage& address);
  int32_t Listen(int32_t connection);
  int32_t Accept(
    std::unique_ptr<Socket>* socket, base::CompletionCallback callback);

  int32_t Connect(
    const SockaddrStorage& address, base::CompletionCallback callback);
  int32_t Close();

  int32_t Read(base::Buffer* buffuer, base::CompletionCallback callback);
  int32_t ReadIfReady(base::Buffer* buffer, base::CompletionCallback callback);
  int32_t CancelReadIfReady();

  int32_t Write(base::Buffer* buffer, base::CompletionCallback callback);
  int32_t WaitForWrite(
    base::Buffer* buffer, base::CompletionCallback callback);

  bool IsConnected() const;
  int32_t GetPeerAddress(SockaddrStorage* address) const;

  SocketDescriptor Descriptor() const;

 private:
  void SetPeerAddress(const SockaddrStorage& address);

  int32_t DoAccept(std::unique_ptr<Socket>* socket);
  void AcceptCompleted();

  int32_t DoConnect();
  void ConnectCompleted();

  int32_t DoRead(base::Buffer* buffer);
  void RetryRead(int32_t res);
  void ReadCompleted();

  int32_t DoWrite(base::Buffer* buffer);
  void WriteCompleted();

  // std::unique_ptr<SocketHandle> handle_;
  SocketDescriptor descriptor_;

  std::unique_ptr<Socket>* accepted_socket_;
  std::unique_ptr<SockaddrStorage> peer_address_;

  bool waiting_connect_ = false;

  std::unique_ptr<base::Buffer> pending_read_buffer_;
  base::CompletionCallback peding_read_callback_;

  std::unique_ptr<base::Buffer> pending_write_buffer_;
  base::CompletionCallback peding_write_callback_;

  base::CompletionCallback accept_callback_;
  base::CompletionCallback connection_callback_;
  base::CompletionCallback read_if_ready_callback_;

  DISAALOW_COPY_AND_ASSIGN(Socket)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_H_
