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

namespace nlink {
namespace net {

class TcpServer : public Server {
 public:
  explicit TcpServer(base::DispatcherConext* dispatcher_context)
    : acceptor_(CreateAcceptor(dispatcher_context)) {
  }

  int32_t Listen(const IpEndPoint& address) override {
    acceptor_->Listen(address);
  }

  int32_t Accept(base::CompletionCallback callback) override {
    acceptor_->Accept(std::move(callback));
  }

 private:
  std::unique_ptr<Acceptor> acceptor_;
  
  DISAALOW_COPY_AND_ASSIGN(TcpServer)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_TCP_SERVER_H_
