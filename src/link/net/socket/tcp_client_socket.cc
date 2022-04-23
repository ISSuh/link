/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/tcp_client_socket.h"

#include <memory>
#include <utility>

#include "link/net/base/net_error.h"

namespace link {
namespace net {

TcpClientSocket::TcpClientSocket(const IpEndPoint& address)
  : TcpClientSocket(std::unique_ptr<TcpSocket>(new TcpSocket()), address) {
}

TcpClientSocket::TcpClientSocket(
  std::unique_ptr<TcpSocket> connection_socket,
  const IpEndPoint& peer_address)
  : socket_(std::move(connection_socket)),
    address_(std::unique_ptr<IpEndPoint>(new IpEndPoint(peer_address))) {
}

TcpClientSocket::~TcpClientSocket() {
  if (IsConnected()) {
    Disconnect();
  }
}

int32_t TcpClientSocket::Bind(const IpEndPoint& address) {
  if (address_) {
    return ERR_UNEXPECTED;
  }

  int32_t result = OK;
  if (!socket_->IsValid()) {
    result = OpenSocket(address.GetFamily());
    if (result != OK) {
      return result;
    }
  }

  result = socket_->Bind(address);
  if (result != OK) {
    return result;
  }

  address_.reset(new IpEndPoint(address));
  return OK;
}

int32_t TcpClientSocket::Connect(base::CompletionCallback callback) {
  // If connecting or already connected, then just return OK.
  if (socket_->IsValid()) {
    return OK;
  }

  connection_state_ = CONNECT_STATE_CONNECT;

  int32_t res = DoConnectLoop(OK);
  if (res == ERR_IO_PENDING) {
    connect_callback_ = std::move(callback);
  }
  return res;
}

void TcpClientSocket::Disconnect() {
  total_received_bytes_ = 0;

  // If connecting or already connected, record that the socket has been
  // disconnected.
  socket_->Close();

  connect_callback_.Reset();
  read_callback_.Reset();
  write_callback_.Reset();
}

bool TcpClientSocket::IsConnected() const {
  return socket_->IsConnected();
}

int32_t TcpClientSocket::Read() {

}

int32_t TcpClientSocket::Write() {

}

int32_t TcpClientSocket::OpenSocket(AddressFamily family) {
  int result = socket_->Open(family);
  if (result != OK) {
    return result;
  }
  // socket_->SetDefaultOptionsForClient();
  return OK;
}

int32_t TcpClientSocket::DoConnectLoop(int32_t result) {
  int32_t res = result;
  do {
    ConnectState state = connection_state_;
    connection_state_ = CONNECT_STATE_NONE;
    switch (state) {
      case CONNECT_STATE_CONNECT:
        res = DoConnect();
        break;
      case CONNECT_STATE_CONNECT_COMPLETE:
        res = DoConnectComplete(res);
        break;
      default:
        res = ERR_UNEXPECTED;
        break;
    }
  } while (res != ERR_IO_PENDING && connection_state_ != CONNECT_STATE_NONE);

  return res;
}

int32_t TcpClientSocket::DoConnect() {
  const IpEndPoint& endpoint = *address_;

  connection_state_ = CONNECT_STATE_CONNECT_COMPLETE;

  if (socket_->IsValid()) {
    // DCHECK(bind_address_);
  } else {
    int result = OpenSocket(endpoint.GetFamily());
    if (result != OK)
      return result;

    if (address_) {
      result = socket_->Bind(*address_);
      if (result != OK) {
        socket_->Close();
        return result;
      }
    }
  }

  // // Notify |socket_performance_watcher_| only if the |socket_| is reused to
  // // connect to a different IP Address.
  // if (socket_->socket_performance_watcher() && current_address_index_ != 0)
  //   socket_->socket_performance_watcher()->OnConnectionChanged();

  // start_connect_attempt_ = base::TimeTicks::Now();

  // // Start a timer to fail the connect attempt if it takes too long.
  // base::TimeDelta attempt_timeout = GetConnectAttemptTimeout();
  // if (!attempt_timeout.is_max()) {
  //   DCHECK(!connect_attempt_timer_.IsRunning());
  //   connect_attempt_timer_.Start(
  //       FROM_HERE, attempt_timeout,
  //       base::BindOnce(&TCPClientSocket::OnConnectAttemptTimeout,
  //                      base::Unretained(this)));
  // }

  return socket_->Connect(
    endpoint, base::Bind(&TcpClientSocket::DidCompleteConnect, this));
}

int32_t TcpClientSocket::DoConnectComplete(int32_t result) {
  if (result == OK) {
    return OK;
  }

  // Don't try the next address if entering suspend mode.
  // if (result == ERR_NETWORK_IO_SUSPENDED)
  //   return result;

  // Close whatever partially connected socket we currently have.
  Disconnect();

  // Otherwise there is nothing to fall back to, so give up.
  return result;
}

void TcpClientSocket::OnConnectAttemptTimeout() {

}

}  // namespace net
}  // namespace link
