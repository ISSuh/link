/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_TCP_SERVER_H_
#define LINK_NET_SOCKET_TCP_SERVER_H_

#include <memory>
#include <set>
#include <vector>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/task/task_runner.h"
#include "link/base/callback/callback.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/server.h"
#include "link/io/socket/acceptor.h"
#include "link/io/socket/session.h"
#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

class TcpSocketServer : public Server {
 public:
  explicit TcpSocketServer(base::TaskRunner* task_runner);
  virtual ~TcpSocketServer();

  // Server
  bool Listen(
    const IpEndPoint& address,
    handler::AcceptHandler accept_handler,
    handler::CloseHandler close_handler) override;
  void Accept(
    handler::AcceptHandler accept_handler,
    handler::CloseHandler close_handler) override;
  void Close() override;
  void RegistIOHandler(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler) override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

 private:
  void InternalAcceptHandler(std::shared_ptr<Session> session);
  void InternalCloseHandler(std::shared_ptr<Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  void RegistChannel(SocketDescriptor descriptor);

  void CloseAllSessions();

  base::TaskRunner* task_runner_;
  base::DispatcherConext* dispatcher_context_;

  std::unique_ptr<Acceptor> acceptor_;
  std::set<std::shared_ptr<Session>> sessions_;

  handler::AcceptHandler accept_handler_;
  handler::CloseHandler close_handler_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpSocketServer)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_TCP_SERVER_H_
