/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_POSIX_TCP_POSIX_SOCKET_H_
#define LINK_NET_SOCKET_POSIX_TCP_POSIX_SOCKET_H_

#include <memory>

#include "link/net/socket/posix/posix_socket.h"
#include "link/net/base/ip_endpoint.h"
#include "link/base/macro.h"
#include "link/base/callback/callback.h"

namespace link {
namespace net {

class PosixSocket;

class TcpPosixSocket {
 public:
  TcpPosixSocket();
  virtual ~TcpPosixSocket();

  int32_t Open(AddressFamily address_family);
  int32_t AdoptConnectedSocket(
    SocketDescriptor socket, const IpEndPoint& peer_address);
  int32_t AdoptUnconnectedSocket(SocketDescriptor socket);

  int32_t Bind(const IpEndPoint& address);
  int32_t Listen(int32_t connection);
  int32_t Accept(
    std::unique_ptr<TcpPosixSocket>* tcp_socket,
    IpEndPoint* address,
    base::CompletionCallback callback);
  int32_t Connect(
    const IpEndPoint& address,
    base::CompletionCallback callback);
  int32_t Close();

  int32_t Read(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);
  int32_t ReadIfReady(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);
  int32_t CancelReadIfReady();

  int32_t Write(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);

  bool IsConnected() const;
  int32_t GetPeerAddress(IpEndPoint* address) const;

 private:
  void AcceptCompleted(std::unique_ptr<TcpPosixSocket>* tcp_socket,
                       IpEndPoint* address,
                       base::CompletionCallback callback,
                       int32_t res);
  int32_t HandleAcceptCompleted(std::unique_ptr<TcpPosixSocket>* tcp_socket,
                            IpEndPoint* address,
                            int32_t res);
  int32_t BuildTcpSocketPosix(std::unique_ptr<TcpPosixSocket>* tcp_socket,
                          IpEndPoint* address);

  void ConnectCompleted(base::CompletionCallback callback, int32_t res);
  int32_t HandleConnectCompleted(int32_t res);

  void ReadCompleted(Buffer* buf,
                     base::CompletionCallback callback,
                     int32_t res);
  void ReadIfReadyCompleted(base::CompletionCallback callback, int32_t res);
  int32_t HandleReadCompleted(Buffer* buf, int32_t res);
  void HandleReadCompletedHelper(int32_t res);

  void WriteCompleted(Buffer* buf,
                      base::CompletionCallback callback,
                      int32_t res);
  int32_t HandleWriteCompleted(Buffer* buf, int32_t res);

  std::unique_ptr<PosixSocket> socket_;
  std::unique_ptr<PosixSocket> accept_socket_;

  DISAALOW_COPY_AND_ASSIGN(TcpPosixSocket);
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_POSIX_TCP_POSIX_SOCKET_H_
