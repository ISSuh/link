/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_acceptor.h"

#include <utility>

#include "link/base/logging.h"
#include "link/io/base/io_error.h"
#include "link/io/socket/platform/tcp_socket_client.h"
#include "link/io/socket/platform/tcp_session.h"

namespace nlink {
namespace io {

TcpAcceptor::TcpAcceptor(
  base::TaskRunner* task_runner,
  SocketCreatedCallbak socket_create_callback)
  : task_runner_(task_runner),
    socket_create_callback_(socket_create_callback),
    socket_(nullptr),
    is_connected_(false) {
}

TcpAcceptor::~TcpAcceptor() {
}

bool TcpAcceptor::Listen(const IpEndPoint& address) {
  if (nullptr != socket_) {
    LOG(ERROR) << "[TcpAcceptor::Listen] socket already listened.";
    return false;
  }

  SocketOptions default_options;
  socket_ = std::make_unique<TcpSocket>(default_options);

  int32_t res = socket_->Open(default_options.address_family);
  if (IOError::OK != res) {
    return false;
  }

  res = socket_->Bind(address);
  if (IOError::OK != res) {
    return false;
  }

  res = socket_->Listen(default_options.listener_connection);
  if (IOError::OK != res) {
    return false;
  }

  socket_create_callback_(socket_->Descriptor(), true);
  return true;
}

void TcpAcceptor::Accept(handler::AcceptHandler handler) {
  is_connected_ = false;
  DoAccept(std::move(handler));
}

void TcpAcceptor::Close() {
  
}

void TcpAcceptor::DoAccept(handler::AcceptHandler handler) {
  if (is_connected_) {
    return;
  }

  std::unique_ptr<TcpSocket> peer_socket = nullptr;
  IpEndPoint peer_address;
  socket_->Accept(&peer_socket, &peer_address,
    [this, &peer_socket, handler](int32_t res) {
      this->InternalAcceptHandler(std::move(peer_socket), handler, res);
    });
}

void TcpAcceptor::PostAcceptTask(handler::AcceptHandler handler) {
  task_runner_->PostTask(
    [this, handler]() {
      this->DoAccept(std::move(handler));
    });
}

void TcpAcceptor::InternalAcceptHandler(
  std::unique_ptr<TcpSocket> peer_socket,
  handler::AcceptHandler handler,
  int32_t res) {
  switch (res) {
    case IOError::OK:
      CreateNewSessionAndRegist(
        std::move(peer_socket), std::move(handler));
      break;
    case 11:
      break;
    default:
      task_runner_->PostTask([this, handler]() {
        this->DoAccept(handler);
      });
      break;
  }
}

void TcpAcceptor::CreateNewSessionAndRegist(
  std::unique_ptr<TcpSocket> peer_socket,
  handler::AcceptHandler handler) {
  is_connected_ = true;

  socket_create_callback_(peer_socket->Descriptor(), false);

  std::shared_ptr<Session> session =
    std::make_shared<TcpSocketSession>(task_runner_, std::move(peer_socket));

  if (handler) {
    handler(session);
  }
}

}  // namespace io
}  // namespace nlink
