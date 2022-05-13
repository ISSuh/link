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
#include "link/base/platform/discriptor.h"
#include "link/base/event/event.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/server_socket.h"
#include "link/net/socket/tcp_socket.h"

namespace link {
namespace net {

class TcpServerSocket : public ServerSocket {
 public:
  TcpServerSocket();
  explicit TcpServerSocket(std::unique_ptr<TcpSocket> socket);
  virtual ~TcpServerSocket();

  int32_t AdoptSocket(base::Discriptor socket);

  int32_t Listen(const IpEndPoint& address, int32_t backlog) override;
  int32_t GetLocalAddress(IpEndPoint* address) const override;

  int32_t Accept(
    std::unique_ptr<TcpSocket>* socket,
    base::CompletionCallback callback) override;

  int32_t Accept(
    std::unique_ptr<TcpSocket>* socket,
    base::CompletionCallback callback,
    IpEndPoint* peer_address) override;

  base::Discriptor SocketDiscriptor() override;

 private:
  void OnAcceptCompleted(
    std::unique_ptr<TcpSocket>* output_accepted_socket,
    IpEndPoint* output_accepted_address,
    base::CompletionCallback forward_callback,
    int result);

  std::unique_ptr<TcpSocket> socket_;
  std::unique_ptr<TcpSocket> accepted_socket_;

  IpEndPoint accepted_address_;
  bool pending_accept_;

  DISAALOW_COPY_AND_ASSIGN(TcpServerSocket)
};

}  // namespace net
}  // namespace link

#endif  // SRC_LINK_NET_SOCKET_TCP_SERVER_SOCKET_H_
