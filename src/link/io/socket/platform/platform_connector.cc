/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link//io/socket/platform/platform_connector.h"

#include <memory>

#include "link/base/logging.h"
#include "link/io/socket/platform/tcp_socket.h"
#include "link/io/socket/platform/tcp_socket_session.h"
#include "link/io/base/io_error.h"

namespace nlink {
namespace io {

PlatormConnector::PlatormConnector(base::TaskRunner* task_runner)
  : task_runner_(task_runner) {
}

PlatormConnector::~PlatormConnector() {
}

void PlatormConnector::Connect(
  const IpEndPoint& address, handler::ConnectHandler handler) {
  address_ = address;

  SocketOptions options;
  std::unique_ptr<TcpSocket> socket = std::make_unique<TcpSocket>(options);
  DoConnect(std::move(socket));
}

void PlatormConnector::DoConnect(std::unique_ptr<TcpSocket> socket) {
  socket->Connect(address_,
    base::Bind(
      &PlatormConnector::InternalConnectHnadler, this, std::move(socket)));
}

void PlatormConnector::InternalConnectHnadler(
  std::unique_ptr<TcpSocket> socket, int32_t err) {
  if (err != IOError::OK) {
    std::shared_ptr<Session> session = std::make_shared<TcpSocketSession>();
    if (!connect_handler_.is_null()) {
      connect_handler_.Run(session);
    }
  } else {
    ++try_connection_count_;
    task_runner_->PostTask(
      base::Bind(&PlatormConnector::DoConnect, this, std::move(socket)));
  }
}

}  // namespace io
}  // namespace nlink
