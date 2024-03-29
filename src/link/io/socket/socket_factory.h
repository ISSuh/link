/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_SOCKET_FACTORY_H_
#define LINK_IO_SOCKET_SOCKET_FACTORY_H_

#include <memory>

#include "link/base/task/task_runner.h"
#include "link/io/socket/client.h"
#include "link/io/socket/server.h"

namespace nlink {
namespace io {

class SocketFactory {
 public:
  static Client* CreateTcpClient(
    std::weak_ptr<base::TaskRunner> task_runner);

  static Server* CreateTcpServer(
    std::weak_ptr<base::TaskRunner> task_runner);
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_SOCKET_FACTORY_H_
