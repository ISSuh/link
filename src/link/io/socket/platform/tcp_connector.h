/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_

#include <memory>

#include "link/base/task/task_runner.h"
#include "link//io/socket/connector.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpConnector : public Connector {
 public:
  using SocketCreatedCallbak = base::Callback<void(SocketDescriptor)>;

  TcpConnector(
    base::TaskRunner* task_runner, SocketCreatedCallbak callback);
  virtual ~TcpConnector();

  void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) override;

 private:
  void DoConnect(TcpSocket* socket);
  void HandlePendingConnect(TcpSocket* socket);
  void InternalConnectHnadler(TcpSocket* socket, int32_t err);

  base::TaskRunner* task_runner_;
  SocketCreatedCallbak socket_create_callback_;

  IpEndPoint address_;
  handler::ConnectHandler connect_handler_;

  uint64_t try_connection_count_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_CONNECTOR_H_
