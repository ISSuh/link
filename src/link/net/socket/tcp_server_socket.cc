/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/tcp_server_socket.h"

#include <utility>

#include "link/net/base/net_error.h"

namespace link {
namespace net {

TcpServerSocket::TcpServerSocket()
  : TcpServerSocket(std::unique_ptr<TcpSocket>(new TcpSocket())) {
}

TcpServerSocket::TcpServerSocket(std::unique_ptr<TcpSocket> socket)
  : socket_(std::move(socket)), pending_accept_(false) {
}

TcpServerSocket::~TcpServerSocket() = default;

int32_t TcpServerSocket::AdoptSocket(SocketDescriptor socket) {
  return socket_->AdoptUnconnectedSocket(socket);
}

int32_t TcpServerSocket::Listen(const IpEndPoint& address, int32_t backlog) {
  int result = socket_->Open(address.GetFamily());
  if (result != OK)
    return result;

  result = socket_->AllowAddressReuse();
  if (result != OK) {
    socket_->Close();
    return result;
  }

  result = socket_->Bind(address);
  if (result != OK) {
    socket_->Close();
    return result;
  }

  result = socket_->Listen(backlog);
  if (result != OK) {
    socket_->Close();
    return result;
  }

  return OK;
}

int32_t TcpServerSocket::GetLocalAddress(IpEndPoint* address) const {
  // return socket_->GetLocalAddress(address);
  return 0;
}

int32_t TcpServerSocket::Accept(
  std::unique_ptr<TcpSocket>* socket, base::CompletionCallback callback) {
  return Accept(socket, std::move(callback), nullptr);
}

int32_t TcpServerSocket::Accept(
  std::unique_ptr<TcpSocket>* socket,
  base::CompletionCallback callback,
  IpEndPoint* peer_address) {
  if (pending_accept_) {
    return ERR_UNEXPECTED;
  }

  base::CompletionCallback accept_callback = base::Bind(
      &TcpServerSocket::OnAcceptCompleted, this, socket,
      peer_address, std::move(callback));
  int result = socket_->Accept(&accepted_socket_, &accepted_address_,
                               std::move(accept_callback));
  if (result != ERR_IO_PENDING) {
    result = ConvertAcceptedSocket(result, socket, peer_address);
  } else {
    pending_accept_ = true;
  }

  return result;
}

int TcpServerSocket::ConvertAcceptedSocket(
  int result,
  std::unique_ptr<TcpSocket>* output_accepted_socket,
  IpEndPoint* output_accepted_address) {
  std::unique_ptr<TcpSocket> temp_accepted_socket(std::move(accepted_socket_));
  if (result != OK)
    return result;

  if (output_accepted_address)
    *output_accepted_address = accepted_address_;

  // *output_accepted_socket = std::make_unique<TCPClientSocket>(
  //     std::move(temp_accepted_socket), accepted_address_);

  return OK;
}

void TcpServerSocket::OnAcceptCompleted(
  std::unique_ptr<TcpSocket>* output_accepted_socket,
  IpEndPoint* output_accepted_address,
  base::CompletionCallback forward_callback,
  int result) {
  result = ConvertAcceptedSocket(result, output_accepted_socket,
                                 output_accepted_address);
  pending_accept_ = false;
  std::move(forward_callback).Run(result);
}

}  // namespace net
}  // namespace link

