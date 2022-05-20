/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/session.h"

#include <utility>

namespace nlink {
namespace net {

void ConnectHnadler(const asio::error_code& error) {
  if (!error) {
    // todo
  }
}

Session::Session(asio::ip::tcp::socket socket, Delegate* delegate)
  : socket_(std::move(socket)),
    delegate_(delegate) {
}

Session::~Session() {}

void Session::Open() {
  DoRead();
}

void Session::Connect(const std::string& address, int32_t port) {
  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address), port);
  socket_.async_connect(endpoint, ConnectHnadler);
}

void Session::Close() {
  socket_.close();
}

void Session::DoWrite() {
}

void Session::DoRead() {
}


}  // namespace net
}  // namespace nlink
