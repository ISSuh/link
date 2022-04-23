/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SERVER_SOCKET_H_
#define LINK_NET_SOCKET_SERVER_SOCKET_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/tcp_socket.h"

namespace link {
namespace net {

class ServerSocket {
 public:
  ServerSocket() = default;
  virtual ~ServerSocket() = default;

  virtual int32_t Listen(const IpEndPoint& address, int32_t backlog) = 0;
  virtual int32_t GetLocalAddress(IpEndPoint* address) const = 0;

  virtual int32_t Accept(
    std::unique_ptr<TcpSocket>* socket,
    base::CompletionCallback callback) = 0;

  virtual int32_t Accept(
    std::unique_ptr<TcpSocket>* socket,
    base::CompletionCallback callback,
    IpEndPoint* peer_address) = 0;

  virtual base::Discriptor SocketDiscriptor() = 0;

 private:
  DISAALOW_COPY_AND_ASSIGN(ServerSocket)
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_SERVER_SOCKET_H_
