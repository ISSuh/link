/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_SOCKET_FACTORY_H_
#define LINK_IO_SOCKET_SOCKET_FACTORY_H_

#include "link/base/task/task_runner.h"
#include "link/io/socket/client.h"
#include "link/io/socket/server.h"

namespace nlink {
namespace io {

class SocketFactory {
 public:
  static Client* CreateTcpClient(base::TaskRunner* task_runner);
  static Server* CreateTcpServer(base::TaskRunner* task_runner);
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_SOCKET_FACTORY_H_
