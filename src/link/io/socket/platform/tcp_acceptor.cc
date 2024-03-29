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
  std::weak_ptr<base::TaskRunner> task_runner,
  SocketCreatedCallbak socket_create_callback)
  : task_runner_(task_runner),
    socket_create_callback_(std::move(socket_create_callback)),
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
  PostAcceptTask(std::move(handler));
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
    [this, &peer_socket, accept_handler = std::move(handler)]
    (int32_t res) mutable {
      this->InternalAcceptHandler(
        std::move(peer_socket), std::move(accept_handler), res);
    });
}

void TcpAcceptor::PostAcceptTask(handler::AcceptHandler handler) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, accept_handler = std::move(handler)]() mutable {
        this->DoAccept(std::move(accept_handler));
      });
  }
}

void TcpAcceptor::InternalAcceptHandler(
  std::unique_ptr<TcpSocket> peer_socket,
  handler::AcceptHandler handler,
  int32_t res) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_.lock();
  if (!task_runner) {
    return;
  }

  switch (res) {
    case IOError::OK:
      CreateNewSessionAndRegist(
        std::move(peer_socket), std::move(handler));
      break;
    // TODO(issuh): temp code
    case 11:
      break;
    default:
      task_runner->PostTask(
        [this, accept_handler = std::move(handler)]() mutable {
          this->DoAccept(std::move(accept_handler));
        });
      break;
  }
}

void TcpAcceptor::CreateNewSessionAndRegist(
  std::unique_ptr<TcpSocket> peer_socket,
  handler::AcceptHandler handler) {
  is_connected_ = true;

  int32_t descriptor = peer_socket->Descriptor();
  std::shared_ptr<Session> session =
    std::make_shared<TcpSocketSession>(task_runner_, std::move(peer_socket));

  if (handler) {
    handler(session);
  }

  socket_create_callback_(descriptor, false);
}

}  // namespace io
}  // namespace nlink
