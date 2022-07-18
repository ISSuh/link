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
    socket_create_callback_(callback),
    try_connection_count_(0),
    is_connected_(false) {
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

  socket_create_callback_(socket->Descriptor());

  PostConnectTask(socket);
}

void TcpConnector::DoConnect(std::shared_ptr<TcpSocket> socket) {
  ++try_connection_count_;

  int32_t res = socket->Connect(
    address_,
    [this, socket](int32_t err) mutable {
      this->InternalConnectHnadler(socket, err);
    });

  if (IOError::ERR_IO_PENDING == res) {
    PostConnectTask(socket);
    return;
  }
}

void TcpConnector::PostConnectTask(std::shared_ptr<TcpSocket> socket) {
  if (is_connected_) {
    return;
  }

  task_runner_->PostTask(
    [this, socket]() {
      this->DoConnect(socket);
    });
}

void TcpConnector::InternalConnectHnadler(
  std::shared_ptr<TcpSocket> socket, int32_t err) {
  if (err == IOError::OK) {;
    std::shared_ptr<Session> session =
      std::make_shared<TcpSocketSession>(std::move(socket));
    if (connect_handler_) {
      connect_handler_(session);
    }
  } else {
    PostConnectTask(socket);
  }
}

}  // namespace io
}  // namespace nlink
