/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/socket_factory.h"

#include "link/io/socket/platform/tcp_socket_client.h"
#include "link/io/socket/platform/tcp_socket_server.h"

namespace nlink {
namespace io {

Client* SocketFactory::CreateTcpClient(
  std::weak_ptr<base::TaskRunner> task_runner) {
  return new TcpSocketClient(task_runner);
}

Server* SocketFactory::CreateTcpServer(
  std::weak_ptr<base::TaskRunner> task_runner) {
  return new TcpSocketServer(task_runner);
}

}  // namespace io
}  // namespace nlink
