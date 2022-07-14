/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link//io/socket/platform/tcp_connector.h"

#include <memory>

#include "link/base/logging.h"
#include "link/io/socket/platform/tcp_session.h"
#include "link/io/base/io_error.h"

namespace nlink {
namespace io {

TcpConnector::TcpConnector(
  base::TaskRunner* task_runner, SocketCreatedCallbak callback)
  : task_runner_(task_runner),
    socket_create_callback_(callback) {
}

TcpConnector::~TcpConnector() {
}

void TcpConnector::Connect(
  const IpEndPoint& address, handler::ConnectHandler handler) {
  address_ = address;
  connect_handler_ = handler;

  SocketOptions options;
  std::shared_ptr<TcpSocket> socket = std::make_shared<TcpSocket>(options);
  socket->Open(AddressFamily::ADDRESS_FAMILY_IPV4);

  socket_create_callback_.Run(socket->Descriptor());
  DoConnect(socket);
}

void TcpConnector::DoConnect(std::shared_ptr<TcpSocket> socket) {
  int32_t res = socket->Connect(address_,
    base::Bind(
      &TcpConnector::InternalConnectHnadler, this, socket));

  if (IOError::ERR_IO_PENDING) {
    HandlePendingConnect(socket);
    return;
  }
}

void TcpConnector::HandlePendingConnect(std::shared_ptr<TcpSocket> socket) {
  task_runner_->PostTask(
    base::Bind(&TcpConnector::DoConnect, this, socket));
}

void TcpConnector::InternalConnectHnadler(
  std::shared_ptr<TcpSocket> socket, int32_t err) {
  if (err != IOError::OK) {
    std::shared_ptr<Session> session =
      std::make_shared<TcpSocketSession>(task_runner_, socket);
    if (!connect_handler_.is_null()) {
      connect_handler_.Run(session);
    }
  } else {
    ++try_connection_count_;
    task_runner_->PostTask(
      base::Bind(&TcpConnector::DoConnect, this, socket));
  }
}

}  // namespace io
}  // namespace nlink
