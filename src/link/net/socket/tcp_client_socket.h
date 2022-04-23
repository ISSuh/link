/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_TCP_CLIENT_SOCKET_H_
#define LINK_NET_SOCKET_TCP_CLIENT_SOCKET_H_

#include <memory>

#include "link/net/socket/client_socket.h"
#include "link/net/socket/tcp_socket.h"
#include "link/net/base/ip_endpoint.h"

namespace link {
namespace net {

class TcpClientSocket : public ClientSocket {
 public:
  explicit TcpClientSocket(const IpEndPoint& address);

  TcpClientSocket(
    std::unique_ptr<TcpSocket> connection_socket,
    const IpEndPoint& peer_address);

  virtual ~TcpClientSocket();

  int32_t Connect(base::CompletionCallback callback) override;
  void Disconnect() override;
  bool IsConnected() const override;

  int32_t Read() override;
  int32_t Write() override;

 private:
  enum ConnectState {
    CONNECT_STATE_CONNECT,
    CONNECT_STATE_CONNECT_COMPLETE,
    CONNECT_STATE_NONE,
  };

  int32_t OpenSocket(AddressFamily family);

  int32_t Bind(const IpEndPoint& address);

  int32_t DoConnectLoop(int32_t result);
  int32_t DoConnect();
  int32_t DoConnectComplete(int32_t result);
  void OnConnectAttemptTimeout();

  int32_t DoRead();

  void DidCompleteConnect(int32_t result);
  void DidCompleteRead(int32_t result);
  void DidCompleteWrite(int32_t result);
  void DidCompleteReadWrite(base::CompletionCallback callback, int32_t result);

  std::unique_ptr<TcpSocket> socket_;
  std::unique_ptr<IpEndPoint> address_;

  ConnectState connection_state_;

  int64_t total_received_bytes_;

  base::CompletionCallback connect_callback_;
  base::CompletionCallback read_callback_;
  base::CompletionCallback write_callback_;

  DISAALOW_COPY_AND_ASSIGN(TcpClientSocket)
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_TCP_CLIENT_SOCKET_H_
