/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/socket_factory.h"

#include "link/io/socket/platform/tcp_socket_client.h"

namespace nlink {
namespace io {

Client* SocketFactory::CreateTcpClient(base::TaskRunner* task_runner) {
  return new TcpSocketClient(task_runner);
}

Server* SocketFactory::CreateTcpServer(base::TaskRunner* task_runner) {
  return nullptr;
}

}  // namespace io
}  // namespace nlink
