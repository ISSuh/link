/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_TCP_SERVER_H_
#define LINK_NET_SOCKET_TCP_SERVER_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/base/event/event_dispatcher.h"
#include "link/net/base/ip_endpoint.h"
#include "link/net/socket/server.h"
#include "link/net/socket/asio/acceptor.h"
#include "link/net/socket/asio/session_manager.h"

namespace nlink {
namespace net {

class TcpServer : public Server {
 public:
  TcpServer();
  virtual ~TcpServer();

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

  // Server
  int32_t Listen(const IpEndPoint& address) override;
  int32_t Accept(base::CompletionCallback callback) override;

 private:
  std::unique_ptr<Acceptor> acceptor_;
  SessionManager session_manager_;

  DISAALOW_COPY_AND_ASSIGN(TcpServer)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_TCP_SERVER_H_
