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
    socket_(nullptr),
    try_connection_count_(0),
    is_connected_(false) {
}

TcpConnector::~TcpConnector() {
}

void TcpConnector::Connect(
  const IpEndPoint& address, handler::ConnectHandler handler) {
  if (address.Empty()) {
    return;
  }

  address_ = address;

  SocketOptions options;
  socket_ = std::make_unique<TcpSocket>(options);
  socket_->Open(AddressFamily::ADDRESS_FAMILY_IPV4);

  PostConnectTask(std::move(handler));
}

void TcpConnector::DoConnect(handler::ConnectHandler handler) {
  if (is_connected_) {
    return;
  }

  ++try_connection_count_;

  base::CompletionCallback connect_callback =
    [this, handler](int32_t err) {
      this->InternalConnectHnadler(std::move(handler), err);
    };

  socket_->Connect(address_, std::move(connect_callback));
}

void TcpConnector::PostConnectTask(handler::ConnectHandler handler) {
  task_runner_->PostTask(
    [this, handler]() {
      this->DoConnect(std::move(handler));
    });
}

void TcpConnector::InternalConnectHnadler(
  handler::AcceptHandler handler, int32_t res) {
  switch (res) {
    case IOError::OK:
      CreateAndRegistNewSession(std::move(handler));
      break;
    default:
      PostConnectTask(std::move(handler));
      break;
  }
}

void TcpConnector::CreateAndRegistNewSession(
  handler::AcceptHandler handler) {
  is_connected_ = true;

  socket_create_callback_(socket_->Descriptor());

  std::shared_ptr<Session> session =
    std::make_shared<TcpSocketSession>(task_runner_, std::move(socket_));
  if (handler) {
    handler(session);
  }
}

}  // namespace io
}  // namespace nlink
