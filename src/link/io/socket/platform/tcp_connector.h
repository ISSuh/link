/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_

#include <memory>
#include <functional>

#include "link/base/timer.h"
#include "link/base/task/task_runner.h"
#include "link//io/socket/connector.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpConnector : public Connector {
 public:
  using SocketCreatedCallbak = std::function<void(SocketDescriptor)>;

  TcpConnector(
    std::weak_ptr<base::TaskRunner> task_runner, SocketCreatedCallbak callback);
  virtual ~TcpConnector();

  void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) override;

 private:
  void DoConnect(handler::ConnectHandler handler);
  void PostConnectTask(handler::ConnectHandler handler);
  void InternalConnectHnadler(handler::ConnectHandler handler, int32_t res);
  void CreateAndRegistNewSession(handler::ConnectHandler handler);

  std::weak_ptr<base::TaskRunner> task_runner_;
  SocketCreatedCallbak socket_create_callback_;

  std::unique_ptr<TcpSocket> socket_;
  IpEndPoint address_;

  uint64_t try_connection_count_;
  bool is_connected_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
