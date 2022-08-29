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
#include "link/io/socket/platform/socket_descripor.h"

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
  int32_t Accept(std::unique_ptr<Socket>* socket);

  int32_t Connect(const SockaddrStorage& address);
  int32_t Close();

  void Read(base::Buffer* buffuer, base::CompletionCallback callback);
  void Write(base::Buffer* buffer, base::CompletionCallback callback);

  int32_t GetPeerAddress(SockaddrStorage* address) const;
  bool IsConnected() const;
  SocketDescriptor Descriptor() const;

 private:
  void SetPeerAddress(const SockaddrStorage& address);

  int32_t DoAccept(std::unique_ptr<Socket>* socket);

  int32_t DoConnect();
  void ConnectCompleted();

  int32_t DoRead(base::Buffer* buffer);
  int32_t DoWrite(base::Buffer* buffer);

  SocketDescriptor descriptor_;
  std::unique_ptr<SockaddrStorage> peer_address_;

  bool waiting_connect_ = false;

  DISAALOW_COPY_AND_ASSIGN(Socket)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_H_
