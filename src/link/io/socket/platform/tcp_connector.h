/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_

#include <memory>
#include <functional>

#include "link/base/task/task_runner.h"
#include "link//io/socket/connector.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpConnector : public Connector {
 public:
  using SocketCreatedCallbak = std::function<void(SocketDescriptor)>;

  TcpConnector(
    base::TaskRunner* task_runner, SocketCreatedCallbak callback);
  virtual ~TcpConnector();

  void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) override;

 private:
  void DoConnect(std::shared_ptr<TcpSocket> socket);
  void PostConnectTask(std::shared_ptr<TcpSocket> socket);
  void InternalConnectHnadler(std::shared_ptr<TcpSocket> socket, int32_t err);

  base::TaskRunner* task_runner_;
  SocketCreatedCallbak socket_create_callback_;

  IpEndPoint address_;
  handler::ConnectHandler connect_handler_;

  uint64_t try_connection_count_;
  bool is_connected_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
