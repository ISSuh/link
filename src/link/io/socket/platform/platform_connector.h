/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_PLATFORM_CONNECTOR_H_
#define LINK_IO_SOCKET_PLATFORM_PLATFORM_CONNECTOR_H_

#include <memory>

#include "link//io/socket/connector.h"
#include "link/base/task/task_runner.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class PlatormConnector {
 public:
  static PlatormConnector* CreateConnector(base::TaskRunner* task_runner);

  explicit PlatormConnector(base::TaskRunner* task_runner);
  virtual ~PlatormConnector();

  virtual void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler);

 private:
  void DoConnect(std::unique_ptr<TcpSocket> socket);
  void InternalConnectHnadler(std::unique_ptr<TcpSocket> socket, int32_t err);

  base::TaskRunner* task_runner_;

  IpEndPoint address_;
  handler::ConnectHandler connect_handler_;

  uint64_t try_connection_count_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_PLATFORM_CONNECTOR_H_
