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

#include "link/base/stack_trace.h"

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
  TcpSocket* socket = new TcpSocket(options);
  socket->Open(AddressFamily::ADDRESS_FAMILY_IPV4);

  socket_create_callback_.Run(socket->Descriptor());
  DoConnect(socket);
}

void TcpConnector::DoConnect(TcpSocket* socket) {
  ++try_connection_count_;

  int32_t res = socket->Connect(
    address_,
    [this, &socket](int32_t err) {
      this->InternalConnectHnadler(socket, err);
    });

  if (IOError::ERR_IO_PENDING == res) {
    HandlePendingConnect(socket);
    return;
  }
}

void TcpConnector::HandlePendingConnect(TcpSocket* socket) {
  task_runner_->PostTask(
    base::Bind(&TcpConnector::DoConnect, this, socket));
}

void TcpConnector::InternalConnectHnadler(
  TcpSocket* socket, int32_t err) {
  if (err == IOError::OK) {
    std::shared_ptr<TcpSocket> tcp_socket(socket);
    std::shared_ptr<Session> session =
      std::make_shared<TcpSocketSession>(task_runner_, tcp_socket);
    if (!connect_handler_.is_null()) {
      connect_handler_.Run(session);
    }
  } else {
    task_runner_->PostTask(
      base::Bind(&TcpConnector::DoConnect, this, socket));
  }
}

}  // namespace io
}  // namespace nlink
