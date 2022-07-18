// /**
//  *
//  *  Copyright:  Copyright (c) 2022, ISSuh
//  *
//  */

// #include "link/io/socket/platform/tcp_acceptor.h"

// #include <utility>

// #include "link/base/logging.h"
// #include "link/io/base/io_error.h"
// #include "link/io/socket/platform/tcp_session.h"

// namespace nlink {
// namespace io {

// TcpAcceptor::TcpAcceptor(
//   base::TaskRunner* task_runner, SocketCreatedCallbak callback)
//   : task_runner_(task_runner),
//     socket_create_callback_(callback),
//     socket_(nullptr) {
// }

// TcpAcceptor::~TcpAcceptor() {
// }

// bool TcpAcceptor::Listen(const IpEndPoint& address) {
//   if (nullptr != socket_) {
//     LOG(ERROR) << "[TcpAcceptor::Listen] socket already listened.";
//     return false;
//   }

//   SocketOptions default_options;
//   socket_ = std::make_unique<TcpSocket>(default_options);

//   int32_t res = socket_->Open(default_options.address_family);
//   if (IOError::OK != res) {
//     return false;
//   }

//   res = socket_->Bind(address);
//   if (IOError::OK != res) {
//     return false;
//   }

//   res = socket_->Listen(default_options.listener_connection);
//   if (IOError::OK != res) {
//     return false;
//   }

//   return true;
// }

// void TcpAcceptor::Accept(handler::AcceptHandler handler) {
//   std::unique_ptr<TcpSocket> peer_socket = nullptr;
//   IpEndPoint peer_address;
//   int32_t res = socket_->Accept(&peer_socket, &peer_address,
//     [this, &peer_socket](int32_t res) {
//       this->InternalAcceptHandler(std::move(peer_socket), res);
//     });

//   if (IOError::OK == res) {
//     InternalAcceptHandler(std::move(peer_socket), res);
//   }
// }

// void TcpAcceptor::Close() {

// }

// void TcpAcceptor::DoAccept() {

// }

// void TcpAcceptor::InternalAcceptHandler(
//   std::unique_ptr<TcpSocket> peer_socket, int32_t res) {
//   if (res == IOError::OK) {;
//     std::shared_ptr<Session> session =
//       std::make_shared<TcpSocketSession>(std::move(socket));
//     if (accept_handlcer_) {
//       accept_handlcer_(session);
//     }
//   } else {
//     // PostConnectTask(socket);
//   }
// }

// }  // namespace io
// }  // namespace nlink
