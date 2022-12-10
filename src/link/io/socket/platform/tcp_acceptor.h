/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_
#define SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_

#include <memory>
#include <functional>

#include "link/base/task/task_runner.h"
#include "link/io/socket/acceptor.h"
#include "link/io/socket/client.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpAcceptor : public Acceptor {
 public:
  using SocketCreatedCallbak = base::Callback<void(SocketDescriptor, bool)>;

  TcpAcceptor(
    std::weak_ptr<base::TaskRunner> task_runner,
    SocketCreatedCallbak socket_created_callback);
  virtual ~TcpAcceptor();

  bool Listen(const IpEndPoint& address) override;
  void Accept(handler::AcceptHandler handler) override;
  void Close() override;

 private:
  void DoAccept(handler::AcceptHandler handler);
  void PostAcceptTask(handler::AcceptHandler handler);
  void InternalAcceptHandler(
    std::unique_ptr<TcpSocket> peer_socket,
    handler::AcceptHandler handler,
    int32_t res);

  void CreateNewSessionAndRegist(
    std::unique_ptr<TcpSocket> peer_socket,
    handler::AcceptHandler handler);

  std::weak_ptr<base::TaskRunner> task_runner_;
  SocketCreatedCallbak socket_create_callback_;

  std::unique_ptr<TcpSocket> socket_;
  bool is_connected_;
};

}  // namespace io
}  // namespace nlink

#endif  // SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_
