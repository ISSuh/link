/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_

#include <vector>
#include <memory>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/platform/socket.h"
#include "link/io/socket/platform/socket_options.h"

namespace nlink {
namespace io {

class TcpSocket {
 public:
  explicit TcpSocket(SocketOptions options);
  ~TcpSocket();

  int32_t Open(AddressFamily address_family);
  int32_t AdoptConnectedSocket(
    SocketDescriptor socket_fd, const IpEndPoint& peer_address);
  int32_t AdoptUnconnectedSocket(SocketDescriptor socket_fd);

  int32_t Bind(const IpEndPoint& address);
  int32_t Listen(int32_t connection);
  void Accept(
    std::unique_ptr<TcpSocket>* socket,
    IpEndPoint* address,
    base::CompletionCallback callback);

  void Connect(
    const IpEndPoint& address, base::CompletionCallback&& callback);
  int32_t Close();

  int32_t Read(base::Buffer* buffer, base::CompletionCallback callback);
  int32_t ReadIfReady(base::Buffer* buffer, base::CompletionCallback callback);
  int32_t CancelReadIfReady();

  int32_t Write(base::Buffer* buffer, base::CompletionCallback callback);

  bool IsConnected() const;

  SocketDescriptor Descriptor() const;

 private:
  int32_t BuildNewTcpSocket(
    std::unique_ptr<TcpSocket>* tcp_socket, IpEndPoint* address);

  int32_t HandleConnectCompleted(int32_t res);

  void ReadCompleted(
    base::Buffer* buffer, base::CompletionCallback callback, int32_t res);
  void ReadIfReadyCompleted(base::CompletionCallback callback, int32_t res);
  int32_t HandleReadCompleted(base::Buffer* buffer, int32_t res);

  void WriteCompleted(
    base::CompletionCallback callback, int32_t res);
  int32_t HandleWriteCompleted(int32_t res);

  SocketOptions options_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Socket> accepted_socket_;

  DISAALOW_COPY_AND_ASSIGN(TcpSocket)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_
