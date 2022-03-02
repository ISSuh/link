/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_NET_SOCKET_TCP_SERVER_SOCKET_H_
#define SRC_LINK_NET_SOCKET_TCP_SERVER_SOCKET_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/net/socket/tcp_socket.h"
#include "link/net/socket/client_socket.h"
#include "link/net/base/ip_endpoint.h"

namespace link {
namespace net {

class TcpServerSocket {
 public:
  TcpServerSocket();
  ~TcpServerSocket();

  int AdoptSocket(SocketDescriptor socket);

  // net::ServerSocket implementation.
  int Listen(const IpEndPoint& address, int backlog);

  int Accept(std::unique_ptr<ClientSocket>* socket,
             base::CompletionCallback callback,
             IpEndPoint* peer_address);


 private:
  std::unique_ptr<TcpSocket> socket_;

  std::unique_ptr<TcpSocket> accept_socket_;
  IpEndPoint accepted_address_;
  bool pending_accept_;

  DISAALOW_COPY_AND_ASSIGN(TcpServerSocket)
};

}  // namespace net
}  // namespace link

#endif  // SRC_LINK_NET_SOCKET_TCP_SERVER_SOCKET_H_
