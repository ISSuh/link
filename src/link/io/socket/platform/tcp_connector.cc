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
  std::weak_ptr<base::TaskRunner> task_runner, SocketCreatedCallbak callback)
  : task_runner_(task_runner),
    socket_create_callback_(std::move(callback)),
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
    [this, connect_handler = std::move(handler)](int32_t err) mutable {
      this->InternalConnectHnadler(std::move(connect_handler), err);
    };

  socket_->Connect(address_, std::move(connect_callback));
}

void TcpConnector::PostConnectTask(handler::ConnectHandler handler) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, connect_handler = std::move(handler)]() mutable {
        this->DoConnect(std::move(connect_handler));
      });
  }
}

void TcpConnector::InternalConnectHnadler(
  handler::ConnectHandler handler, int32_t res) {
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
  handler::ConnectHandler handler) {
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
